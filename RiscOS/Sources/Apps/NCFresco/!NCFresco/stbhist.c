/* > stbhist.c
 *
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "msgs.h"
#include "wimp.h"

#include "config.h"
#include "access.h"
#include "interface.h"
#include "makeerror.h"
#include "memwatch.h"
#include "filetypes.h"
#include "url.h"
#include "util.h"
#include "webfonts.h"

#include "stbview.h"
#include "stbhist.h"
#include "stbutils.h"
#include "stbopen.h"
#include "frameutils.h"
#include "stbfe.h"

#define HISTDBG(a)	STBDBG(a)
#define HISTDBGN(a)	STBDBGN(a)

/* --------------------------------------------------------------------------------------------- */

#define N_RECENT_SITES  7

#define MAX_SPECIFIER_SIZE	16

/* --------------------------------------------------------------------------------------------- */

typedef struct fe_global_history_item fe_global_history_item;
typedef struct fe_global_history_fragment fe_global_history_fragment;
typedef struct fe_history_frame_item fe_history_frame_item;

struct fe_history_frame_item
{
    char specifier[MAX_SPECIFIER_SIZE];	/* which frame this is */
    char *url;				/* url without fragment */
    int url_hash;
    int x_scroll, y_scroll;		/* offsets */
};

struct fe_history_item
{
    fe_history_item *next, *prev;

    char n_frames;   	    	    /* number of frames appended */
    char is_offline;
    char language_num;
    char reserved;
    char *title;		    /* if top frame then this is it's title */

    fe_history_frame_item frame[1];
};

/* --------------------------------------------------------------------------------------------- */

struct fe_global_history_fragment
{
    fe_global_history_fragment *next;
    char fragment[1];                   /* resize block to hold fragment */
};

struct fe_global_history_item
{
    fe_global_history_item *next;       /* next history item */
    fe_global_history_fragment *frag_list;   /* linked list of fragments in this page visited */

    char *url;
    char *title;
    char language_num;
    unsigned int url_hash;		/* hash lookup of title string */

    time_t date;			/* time added to list */
};

/* --------------------------------------------------------------------------------------------- */

static fe_global_history_item *global_hist_list = NULL;
static int global_count = 0;	/* count of global URL entries (however many fragments are attached) */

/* --------------------------------------------------------------------------------------------- */

#if DEBUG
static void dump_frames(const fe_history_item *hist)
{
    int i;
    HISTDBG(("history: frame list for %p\n", hist));
    HISTDBG(("         prev %p next %p n %d offline %d title '%s'\n", hist->prev, hist->next, hist->n_frames, hist->is_offline, strsafe(hist->title)));
    for (i = 0; i < hist->n_frames; i++)
    {
	HISTDBG(("  %d: '%s' = '%s'\n", i, hist->frame[i].specifier, hist->frame[i].url));
    }
}
#endif

/*
static char *strip_fragment(const char *url)
{
    const char *frag = strrchr(url, '#');
    if (frag)
    {
        char *new_url = mm_strdup(url);
        new_url[frag - url] = 0;
        return new_url;
    }
    return (char *)url;
}
*/

static void fragment_parse(const char *url, char **url_out, char **fragment_out)
{
    const char *frag = strrchr(url, '#');
    if (frag)
    {
        *url_out = strndup(url, frag - url);
        *fragment_out = mm_strdup(frag+1);
    }
    else
    {
        *url_out = mm_strdup(url);
        *fragment_out = NULL;
    }
}

/* --------------------------------------------------------------------------------------------- */

static fe_global_history_fragment *check_frag_list(fe_global_history_fragment *list, const char *fragment)
{
    fe_global_history_fragment *f;
    for (f = list; f; f = f->next)
    {
        if (strcmp(f->fragment, fragment) == 0)
            break;
    }
    return f;
}

static void fe_global_free_item(fe_global_history_item *i)
{
    fe_global_history_fragment *f;

    if (i == NULL)
	return;

    f = i->frag_list;
    while (f)
    {
        fe_global_history_fragment *next = f->next;
        mm_free(f);
        f = next;
    }

    mm_free(i->url);
    mm_free(i->title);
    mm_free(i);

    global_count--;
}

static void fe_global_remove_oldest(void)
{
    fe_global_history_item *item, *last;

    time_t oldest_date = INT_MAX;
    fe_global_history_item *oldest_item = NULL, *oldest_last = NULL;

    for (last = NULL, item = global_hist_list; item; last = item, item = item->next)
    {
	if (oldest_date > item->date)
	{
	    oldest_date = item->date;
	    oldest_item = item;
	    oldest_last = last;
	}
    }

    if (oldest_item)
    {
	if (oldest_last)
	    oldest_last->next = oldest_item->next;
	else
	    global_hist_list = oldest_item->next;

	fe_global_free_item(oldest_item);
    }
}

/* ---------------------------------------------------------------------------------------------*/

static void fe_global__add(const char *bare_url, const char *fragment, const char *title, int language_num)
{
    fe_global_history_item *item, *prev;
    fe_global_history_item *new_item;
    unsigned int h;
    BOOL insert, add_frag;

    HISTDBG(("hist: global add '%s' '%s'\n", bare_url, strsafe(title)));

    h = string_hash(bare_url);

    /* locate current copy - list is effectively unordered in this case */
    insert = TRUE;
    add_frag = FALSE;
    prev = NULL;
    for (item = global_hist_list; item; item = item->next)
    {
        /* if the title and the URL are the same then we've found a matching entry */
        if (strcmp(strsafe(item->title), strsafe(title)) == 0 &&
	    item->url_hash == h && strcmp(item->url, bare_url) == 0)
        {
            insert = FALSE;

            /* still need to see if this fragment is marked */
            if (fragment && check_frag_list(item->frag_list, fragment) == NULL)
                add_frag = TRUE;

            break;
        }
    }

    /* locate insert position if insertion is needed */
    if (insert)
    {
	for (item = global_hist_list; item; prev = item, item = item->next)
	{
	    if (strcasecomp(strsafe(item->title), strsafe(title)) >= 0)
		break;
	}
    }

    HISTDBG(("hist: global insert %d addfrag %d after %p '%s' '%s' \n",insert, add_frag,  prev, prev ? prev->url : "", prev ? strsafe(prev->title) : ""));

    /* insert new item after prev and before 'item' */
    if (insert)
    {
        new_item = mm_calloc(sizeof(*item), 1);

        new_item->title = mm_strdup(title);
        new_item->url = mm_strdup(bare_url);
        new_item->url_hash = h;
	new_item->date = time(NULL);
	new_item->language_num = language_num;

        if (prev)
        {
            new_item->next = prev->next;
            prev->next = new_item;
        }
        else
        {
            new_item->next = global_hist_list;
            global_hist_list = new_item;
        }
    }
    else
    {
        new_item = item;
    }

    /* add fragment name to the item's list */
    if (add_frag)
    {
	/* this malloc is right because the structure has only one field, a pointer. Beware */
        fe_global_history_fragment *f = mm_malloc(sizeof(fe_global_history_fragment *) + strlen(fragment) + 1);

        strcpy(f->fragment, fragment);

        f->next = new_item->frag_list;
        new_item->frag_list = f;
    }

    /* Only count sites - not fragments */
    if (insert)
	global_count++;
}

static void fe_global_add(const char *url, const char *title, int language_num)
{
    char *bare_url, *fragment;
    fragment_parse(url, &bare_url, &fragment);

    fe_global__add(bare_url, fragment, title, language_num);

    mm_free(bare_url);
    mm_free(fragment);

    if (config_history_global_length > 0 && global_count > config_history_global_length)
	fe_global_remove_oldest();
}

static void fe__global_write_list(FILE *f, void *handle)
{
    fe_global_history_item *item;
    int i;

    for (item = global_hist_list, i = 0; item; item = item->next, i++)
    {
	const char *s = item->title ? item->title : item->url;
	const char *lang = lang_num_to_name(item->language_num);

	fprintf(f, msgs_lookup("histA.I"), lang, i, i, s);

        fputc('\n', f);
    }
}

os_error *fe_global_write_list(FILE *f, BOOL switchable, int frame)
{
#if 1
    fe_internal_write_page(f, switchable ? "histAw" : "histA", 0, frame, fe__global_write_list, NULL);
    return NULL;
#else
    os_error *e;
    char *s;
    fprintf(f, msgs_lookup("histAT"), 0);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histA1"), strsafe(s));
    fputs(msgs_lookup(switchable ? "histA2s" : "histA2"), f);

    e = fe__global_write_list(f);

    fputs(msgs_lookup("histAF"), f);
    return e;
#endif
}

static int fe_global__find_url(const char *bare_url, const char *fragment)
{
    int success;
    fe_global_history_item *item;
    unsigned int h = string_hash(bare_url);

    success = FALSE;
    for (item = global_hist_list; item; item = item->next)
    {
        if (h == item->url_hash && strcmp(bare_url, item->url) == 0)
        {
            if (fragment == NULL || check_frag_list(item->frag_list, fragment) != NULL)
                success = TRUE;

            break;
        }
    }

    return success;
}

static int fe_global_find_url(const char *url)
{
    int found;
    char *bare_url, *fragment;
    fragment_parse(url, &bare_url, &fragment);

    found = fe_global__find_url(bare_url, fragment);

    mm_free(bare_url);
    mm_free(fragment);

    return found;
}

void fe_global_history_dispose(void)
{
    fe_global_history_item *item = global_hist_list;
    while (item)
    {
	fe_global_history_item *next = item->next;
	fe_global_free_item(item);
	item = next;
    }
    global_hist_list = NULL;
}

static fe_global_history_item *fe_global_lookup_index(int index)
{
    fe_global_history_item *item;
    int i;
    for (i = 0, item = global_hist_list; i < index && item; i++, item = item->next)
	;
    return item;
}

/* Routines to shuffle data into lower memory */

static void fe_global_frag_optimise_list(fe_global_history_fragment **plist)
{
    fe_global_history_fragment *f, *l;
    for (l = NULL, f = *plist; f; l = f, f = f->next)
    {
	/* watch out for the size of this item */
	if (optimise_block((void **)&f, sizeof(fe_global_history_fragment *) + strlen(f->fragment) + 1))
	{
	    if (l)
		l->next = f;
	    else
		*plist = f;
	}
    }
}

void fe_global_history_optimise(void)
{
    fe_global_history_item *item, *last;

    for (last = NULL, item = global_hist_list; item; last = item, item = item->next)
    {
	if (optimise_block((void **)&item, sizeof(*item)))
	{
	    if (last)
		last->next = item;
	    else
		global_hist_list = item;
	}

	item->url = optimise_string(item->url);
	item->title = optimise_string(item->title);

	fe_global_frag_optimise_list(&item->frag_list);
    }
}

/* ---------------------------------------------------------------------------------------------*/

static int fe_hist_write_item(FILE *f, const fe_history_item *item, int i)
{
    const fe_history_frame_item *hfi;

    hfi = &item->frame[0];

#if 1
    /* This must always use the recent list item as it may encode which list to read it from */
    fprintf(f, msgs_lookup("histR.I"), lang_num_to_name(item->language_num), i, i, item->title ? item->title : hfi->url);
#else
    if (item->title)
    {
        char *frag = strrchr(hfi->url, '#');
        if (frag)
            fprintf(f, msgs_lookup("histR.I"), i, i, item->title, frag);
        else
            fprintf(f, msgs_lookup("histA.I"), i, i, item->title);
    }
    else
    {
        fprintf(f, msgs_lookup("histR.I"), i, i, hfi->url);
    }
#endif
    fputc('\n', f);

    return TRUE;
}

static void write_list(FILE *f, void *handle)
{
    const fe_history_item *start = (const fe_history_item *)handle;
    const fe_history_item *item;
    int i;

    /* only write out top level frame changes */
    for (i = 0, item = start; item; i++, item = item->prev)
	if (!item->is_offline && /*item->title*/ strcmp(item->frame[0].specifier, "_0") == 0)
	    fe_hist_write_item(f, item, i);
}

os_error *fe_history_write_list(FILE *f, const fe_history_item *start, const fe_history_item *current, BOOL switchable, int frame)
{
    const fe_history_item *item;
    int i;

    /* count how far through the list we are */
    for (i = 0, item = start; item && item != current; i++, item = item->prev)
	;

#if 1
    fe_internal_write_page(f, switchable ? "histRw" : "histR", i, frame, write_list, (void *)start);
#else
    char *s;
    fprintf(f, msgs_lookup("histRT"), i);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histR1"), strsafe(s));
    fputs(msgs_lookup(switchable ? "histR2s" : "histR2"), f);

    /* only write out top level frame changes */
    for (i = 0, item = start; item; i++, item = item->prev)
	if (!item->is_offline && /*item->title*/ strcmp(item->frame[0].specifier, "_0") == 0)
	    fe_hist_write_item(f, item, i);

    fputs(msgs_lookup("histRF"), f);
    fputc('\n', f);
#endif
    return NULL;
}

os_error *fe_history_write_combined_list(FILE *f, const fe_history_item *start, const fe_history_item *current)
{
    const fe_history_item *item;
    int i;
    char *s;

    STBDBG(("in fe_history_write_combined_list\n"));

    /* back track to place to start */
    /* ANC-00288: added check on item in case current == NULL */
    for (i = 0, item = current; i < N_RECENT_SITES/2 && item && item->next; i++, item = item->next)
        ;

    fprintf(f, msgs_lookup("histC.T"), i);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histC.1"), strsafe(s));

    /* write current places */
    for (i = 0; i < N_RECENT_SITES && item; i++, item = item->prev)
    {
        if (item == current && i != 0)
            fputs(msgs_lookup("histC.S.1"), f);

        fe_hist_write_item(f, item, i);

        if (item == current && i != N_RECENT_SITES - 1)
            fputs(msgs_lookup("histC.S.2"), f);
    }

    fprintf(f, msgs_lookup("histC.2"), strsafe(s));

    /* write global list */
    fe__global_write_list(f, NULL);

    fputs(msgs_lookup("histC.F"), f);
    fputc('\n', f);
    return NULL;
}

/* ---------------------------------------------------------------------------------------------*/

/* history functions */

static void fe_free_hist_item(fe_history_item *item)
{
    if (item)
    {
	int i;

	for (i = 0; i < item->n_frames; i++)
	{
/* 	    mm_free(item->frame[i].specifier); */
	    mm_free(item->frame[i].url);
	}

	mm_free(item->title);
	mm_free(item);
    }
}

static void fe_history_truncate(fe_view v)
{
    fe_history_item *chop, *c2;

    HISTDBGN(("Truncate: last=%p, at=%p at->next=%p\n", v->last, v->hist_at, v->hist_at->next));

    if (v->hist_at == v->last)
	return;

    for (chop = v->hist_at->next; chop; chop = c2)
    {
	c2 = chop->next;

	HISTDBGN(("Chop=%p, c2=%p\n", chop, c2));

	fe_free_hist_item(chop);
	v->hist_count--;
    }

    v->last = v->hist_at;
    v->last->next = 0;
}

/*
 * This url may have a fragment in it.
 * This doesn't take account of the extra frames in the block.
 */

static fe_history_item *find_entry(const char *url, fe_history_item *start, int direction)
{
    fe_history_item *item;
    int h = string_hash(url);

    item = start;
    while (item)
    {
        if (h == item->frame[0].url_hash &&
            strcmp(url, item->frame[0].url) == 0)
            return item;

        item = direction == history_PREV ? item->prev : item->next;
    }
    return NULL;
}

static fe_history_item *fe_history_add(fe_view v, const char *url, const char *title, const char *specifier)
{
    fe_history_item *h;

    /* Trim off the history tail if it is there */
    /* This should be trimmed in local history but still kept in global history */
    fe_history_truncate(v);

    if ((h = mm_calloc(1, sizeof(*h))) == NULL)
	return NULL;

    h->frame[0].url = mm_strdup(url);
    h->frame[0].url_hash = string_hash(url);
    strcpy(h->frame[0].specifier, specifier);

    h->n_frames = 1;
    h->title = strdup(title);
    h->language_num = backend_doc_item_language(v->displaying, NULL);

    v->hist_count++;

    HISTDBG(("history: %p add '%s' spec '%s' total %d\n", h, url, specifier, v->hist_count));

    if (v->first)
    {
	HISTDBGN(("h=%p, first=%p, last=%p\n", h, v->first, v->last));

	v->last->next = h;
	h->prev = v->last;
	v->last = h;

	if (v->hist_count > config_history_length)
	{
	    fe_history_item *chop;

	    chop = v->first;

	    HISTDBGN(("Chop=%p\n", chop));

	    v->first = v->first->next;
	    v->first->prev = NULL;

	    fe_free_hist_item(chop);
	    v->hist_count--;
	}
    }
    else
    {
	v->first = v->last = h;
    }

    return h;
}

/* ---------------------------------------------------------------------------------------------*/

static fe_history_item *fe_history_get_item(fe_view v, int a)
{
    fe_history_item *dest;

    if (!v || !v->displaying)
	return NULL;

    dest = v->hist_at;

    /* If the current page isn't in the history list then we want to go back
     * to hist_at directly.
     */
    if (!v->dont_add_to_history && dest)
    {
	switch (a)
	{
	case history_FIRST:
	    dest = v->first;
	    break;
	case history_PREV:
	    dest = dest->prev;
	    break;
	case history_NEXT:
	    dest = dest->next;
	    break;
	case history_LAST:
	    dest = v->last;
	    break;
	}
    }

    return dest;
}

static char *fe_history_get_url(fe_view v, int a)
{
    fe_history_item *dest = fe_history_get_item(v, a);

    if (dest == NULL)
	return NULL;

    return dest->frame[0].url;
}

int fe_history_possible(fe_view v, int direction)
{
    return fe_history_get_url(v, direction) != NULL;
}

/*
 * This should use hist_at if it is different to the current page
 * we assume v is top here
 */

os_error *fe_history_move(fe_view v, int direction)
{
    fe_history_item *dest = fe_history_get_item(v, direction);
    fe_view vv;
    int index;

    DBG(("fe_history_move: v%p dir %d hist_at %p dest %p\n", v, direction, v->hist_at, dest));

    if (!dest)
	return makeerror(ERR_NO_HISTORY);

    index = 0;

    do
    {
	vv = fe_frame_specifier_decode(v, dest->frame[index].specifier);
	if (!vv)
	    dest = dest->prev;
    }
    while (!vv && dest);

    if (vv)
    {
	/* set scroll offsets in target frame */
	vv->fetching_data.xscroll = dest->frame[index].x_scroll;
	vv->fetching_data.yscroll = dest->frame[index].y_scroll;

	/* set new hist in top frame */
	v->fetching_data.hist = dest;

	/* open the new url */
	/* the extra frames are opened by override code in frontend_frame_layout() */
	return frontend_open_url(dest->frame[index].url, vv, NULL, 0, fe_open_url_FROM_HISTORY | fe_open_url_NO_REFERER);
    }

    return makeerror(ERR_NO_HISTORY);
}


int fe_history_move_alpha_index(fe_view v, int index, char **new_url)
{
    fe_global_history_item *item = fe_global_lookup_index(index);
    if (item)
    {
	*new_url = mm_strdup(item->url);
	return TRUE;
    }

    return FALSE;
}

static BOOL fe_history_move_to(fe_view v, fe_history_item *item, char **new_url)
{
    if (item)
    {
	fe_view vv = fe_frame_specifier_decode(v, item->frame[0].specifier);

	/* set scroll offsets in target frame */
	vv->fetching_data.xscroll = item->frame[0].x_scroll;
	vv->fetching_data.yscroll = item->frame[0].y_scroll;

	/* set new hist in top frame */
	v->fetching_data.hist = item;

	if (vv == v)
	{
	    *new_url = mm_strdup(item->frame[0].url);
	    return TRUE;
	}

	/* open the new url */
	/* the extra frames are opened by override code in frontend_frame_layout() */
	frontend_open_url(item->frame[0].url, vv, NULL, 0, fe_open_url_FROM_HISTORY | fe_open_url_NO_REFERER);
    }

    return FALSE;
}

int fe_history_move_recent_index(fe_view v, int index, char **new_url)
{
    fe_history_item *item;
    int i;

    for (i = 0, item = v->last; i < index && item; i++, item = item->prev)
	;

    return fe_history_move_to(v, item, new_url);
}

int fe_history_move_recent_steps(fe_view v, int steps, char **new_url)
{
    fe_history_item *item = v->hist_at;

    if (steps < 0)
    {
	for (item = v->hist_at; item != v->first && steps != 0; steps++, item = item->prev)
	    ;
    }
    else if (steps > 0)
    {
	for (item = v->hist_at; item != v->last && steps != 0; steps--, item = item->next)
	    ;
    }

    return fe_history_move_to(v, item, new_url);
}

void fe_history_dispose(fe_view v)
{
    if (v->first)
    {
	v->hist_at = v->first;

	fe_history_truncate(v);
	fe_free_hist_item(v->first);

	v->first = v->hist_at = v->last = 0;
	v->hist_count = 0;
    }
}

/*
 * This is called whenever any page is visited.
 * v->hist_at points to the history for the (ex)current page
 *
 * url/title is for the new page being loaded in and will include
 * the fragment id if one was used.
 */

int fe_history_visit(fe_view v, const char *url, const char *title)
{
    fe_history_item *item;
    fe_view top;
    char specifier[MAX_SPECIFIER_SIZE];

    HISTDBG(("history: visit '%s'\n", url));

    if (v->dont_add_to_history)
        return 0;

    top = frameutils_find_top(v);
    item = top->hist_at;

#if 0
    if (v == top && item)
    {
        /* If the url is of the current page, ignore it */
        if (item->frame[0].url && strcmp(url, item->frame[0].url) == 0)
	    return 0;
    }
#endif

#if 0
    if (item)
    {
	HISTDBG(("history: prev '%s' current '%s' next '%s'\n",
		item->prev ? strsafe(item->prev->frame[0].url) : "",
		strsafe(item->frame[0].url),
		item->next ? strsafe(item->next->frame[0].url) : ""));

        /* If the url is of the current page, ignore it */
        if (item->frame[0].url && strcmp(url, item->frame[0].url) == 0)
	    return 0;

        /* If the user went back and then follow the same link, move forward */
        if (item->next && item->next->frame[0].url && strcmp(item->next->frame[0].url, url) == 0)
        {
	    v->hist_at = item->next;
	    return 0;
        }

        /* If the user is going back, move back */
        if (item->prev && item->prev->frame[0].url && strcmp(item->prev->frame[0].url, url) == 0)
        {
	    v->hist_at = item->prev;
	    return 0;
        }
    }
#endif

    /* otherwise add it to the history */
    specifier[0] = 0;
    fe_frame_specifier_create(v, specifier, sizeof(specifier));

    /* don't bother with title if not the top frame */
    item = fe_history_add(top, v->real_url ? v->real_url : url, v == top ? title : NULL, specifier);
    if (item)
    {
	item->is_offline = v->offline_mode == fe_keyboard_OFFLINE;
        top->hist_at = item;
    }

    /* and to the global history if not a frame element */
    if (v == top && v->offline_mode == fe_keyboard_ONLINE)
        fe_global_add(url, title, backend_doc_item_language(v->displaying, NULL));

    return 0;
}

static int locate_item(fe_history_item *hist, const char *specifier)
{
    fe_history_frame_item *hfi;
    int i;

    HISTDBG(("locate_item: hist %p n %d\n", hist, hist->n_frames));

    for (i = 0, hfi = hist->frame; i < hist->n_frames; i++, hfi++)
    {
	HISTDBG(("locate_item: i %d hfi %p spec '%s' '%s'\n", i, hfi, specifier, hfi->specifier));
	if (strcmp(specifier, hfi->specifier) == 0)
	    return i;
    }
    return -1;
}

static int count_sections(const char *s)
{
    int c, count = 0;
    while ((c = *s++) != 0)
	if (c == '_')
	    count++;
    return count;
}

static int compare_specifiers(const void *o1, const void *o2)
{
    const fe_history_frame_item *f1 = (fe_history_frame_item *)o1;
    const fe_history_frame_item *f2 = (fe_history_frame_item *)o2;
    const char *s1 = f1->specifier;
    const char *s2 = f2->specifier;
    int s1count = count_sections(s1);
    int s2count = count_sections(s2);

    /* if different lengths then the shorter is -ve */
    if (s1count != s2count)
	return s1count - s2count;

    for (;;)
    {
	int c1, c2;

	c1 = (int)strtoul(s1+1, (char **)&s1, 0);
	c2 = (int)strtoul(s2+1, (char **)&s2, 0);

	if (c1 != c2)
	    return c1 - c2;
	if (*s1 == 0 || *s2 == 0)
	    return 0;
    }

    return 0;
}

static void fe_history_write_scroll(fe_history_frame_item *hfi, fe_view v)
{
    /* write updated values in */
    if (v->w)
    {
	wimp_wstate state;
	wimp_get_wind_state(v->w, &state);

	hfi->x_scroll = state.o.x + v->margin.x0;
	hfi->y_scroll = state.o.y + v->margin.y1;
    }
    else
    {
	hfi->x_scroll = hfi->y_scroll = 0;
    }

    HISTDBG(("history: frame %p '%s' write scroll pos %d\n", hfi, hfi->url, hfi->y_scroll));
}

/* Store the current state of the view in 'hist_at' */

void fe_history_update_current_state(fe_view v)
{
    fe_view top;
    fe_history_item *hist;
    fe_history_frame_item *hfi;
    int index;
    char specifier[MAX_SPECIFIER_SIZE];

    if (v->dont_add_to_history || v->open_transient || v->displaying == NULL)
	return;

    /* must store state for all documents even framesets */
/*     if (!v->w) */
/* 	return; */

    top = frameutils_find_top(v);
    hist = top->hist_at;

    if (!hist)
	return;

    /* see if we have values for this frame */
    specifier[0] = 0;
    fe_frame_specifier_create(v, specifier, sizeof(specifier));
    if ((index = locate_item(hist, specifier)) == -1)
    {
	/* if not then need to extend block */
	char *url;

	HISTDBG(("history: %p add new frame for '%s'\n", hist, specifier));

	/* resize block - *newh includes 1 frame */
	hist = mm_realloc(hist, sizeof(*hist) + hist->n_frames * sizeof(hist->frame[0]));

	/* update various forward and back pointers */
	if (hist->prev)
	    hist->prev->next = hist;
	else
	    top->first = hist;

	if (hist->next)
	    hist->next->prev = hist;
	else
	    top->last = hist;

	/* update the fetching pointer in case we are reloading */
	if (top->fetching_data.hist == top->hist_at)
	    top->fetching_data.hist = hist;

	top->hist_at = hist;

	/* set new index and increment count */
	index = hist->n_frames++;

	hfi = &hist->frame[index];

	/* write in initial type values */
	backend_doc_info(v->displaying, NULL, NULL, &url, NULL);
	hfi->url = mm_strdup(url);
	memcpy(hfi->specifier, specifier, sizeof(specifier));

	/* fill in current scroll positions */
	fe_history_write_scroll(hfi, v);

	qsort(hist->frame, hist->n_frames, sizeof(hist->frame[0]), compare_specifiers);
	HISTDBG(("history: top first %p last %p at %p\n", top->first, top->last, top->hist_at));
#if DEBUG
	dump_frames(hist);
#endif
    }
    else
    {
	/* update current scroll positions */
	fe_history_write_scroll(&hist->frame[index], v);

	HISTDBG(("history: %p frame found at %d\n", hist, index));
    }
}

char *fe_history_lookup_specifier(fe_view v, const char *specifier, int *xoffset, int *yoffset)
{
    int index = locate_item(v->hist_at, specifier);
    if (index != -1)
    {
	fe_history_frame_item *hfi = &v->hist_at->frame[index];
	*xoffset = hfi->x_scroll;
	*yoffset = hfi->y_scroll;
	return hfi->url;
    }
    return NULL;
}

void fe_history_optimise(fe_view v)
{
    fe_history_item *item;

    for (item = v->first; item; item = item->next)
    {
	fe_history_item *was;
	fe_history_frame_item *frame;
	int i;

	/* optimise block and reset various pointers */
	was = item;
	if (optimise_block((void **)&item, sizeof(*item) + (item->n_frames-1) * sizeof(item->frame[0])))
	{
	    if (item->prev)
		item->prev->next = item;
	    else
		v->first = item;

	    if (item->next)
		item->next->prev = item;
	    else
		v->last = item;

	    if (v->hist_at == was)
		v->hist_at = item;

	    if (v->fetching_data.hist == was)
		v->fetching_data.hist = item;
	}

	item->title = optimise_string(item->title);

	/* optimise the frame URLs */
	for (i = 0, frame = item->frame; i < item->n_frames; i++, frame++)
	    frame->url = optimise_string(frame->url);
    }
}

/* ---------------------------------------------------------------------------------------------*/

/*
 * This is called from render_link_colour() and may have a fragment id in it
 */

int fe_test_history(fe_view v, const char *url)
{
    return find_entry(url, v->first, history_NEXT) != NULL || fe_global_find_url(url) || access_test_cache((char *)url);
}

/* ---------------------------------------------------------------------------------------------*/

#if DEBUG
void history_dump(BOOL global)
{
    fe_history_item *item;
    if (!global) for (item = main_view->first; item; item = item->next)
    {
	int i;
	DBG(("hist %p: prev %p next %p n %d title '%s'\n", item, item->prev, item->next, item->n_frames, strsafe(item->title)));
	for (i = 0; i < item->n_frames; i++)
	{
	    DBG(("         frame %10s url %s %dx%d\n", item->frame[i].specifier, item->frame[i].url, item->frame[i].x_scroll, item->frame[i].y_scroll));
	}
    }
}
#endif

/* ---------------------------------------------------------------------------------------------*/

/* eof stbhist.c*/
