/* > stbhist.c
 *
 */

#include <stdio.h>
#include <string.h>

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

#include "stbview.h"
#include "stbhist.h"
#include "stbutils.h"
#include "stbopen.h"

/* --------------------------------------------------------------------------------------------- */

#define N_RECENT_SITES  7

/* --------------------------------------------------------------------------------------------- */

typedef struct fe_global_history_item fe_global_history_item;
typedef struct fe_global_history_fragment fe_global_history_fragment;
typedef struct fe_history_frame_item fe_history_frame_item;

struct fe_history_frame_item
{
    char *url;	    	    	    /* url without fragment */
    int url_hash;
    int x_scroll, y_scroll; 	    /* offsets */
};    

struct fe_history_item
{
    fe_history_item *next, *prev;

    int n_frames;   	    	    /* n-1 are current, n is replacement */

    union
    {
        struct
        {
	    char *title;
        } noframes;
        struct
        {
    	    char *frame_specifier;  	    /* frame top for replacement */
    	} frames;
    } data;

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
    unsigned int url_hash;            /* hash lookup of title string */
};

/* --------------------------------------------------------------------------------------------- */

static fe_global_history_item *global_hist_list = NULL;
static int global_count = 0;

/* --------------------------------------------------------------------------------------------- */

/*
static char *strip_fragment(const char *url)
{
    const char *frag = strrchr(url, '#');
    if (frag)
    {
        char *new_url = strdup(url);
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
        *fragment_out = strdup(frag+1);
    }
    else
    {
        *url_out = strdup(url);
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

/* ---------------------------------------------------------------------------------------------*/

static void fe_global__add(const char *bare_url, const char *fragment, const char *title)
{
    fe_global_history_item *item, *prev;
    fe_global_history_item *new_item;
    int diff;
    unsigned int h;
    BOOL insert, add_frag;

    STBDBG(("hist: global add '%s' '%s'\n", bare_url, strsafe(title)));

    h = string_hash(bare_url);

    /* locate current copy */
    insert = TRUE;
    add_frag = FALSE;
    for (prev = NULL, item = global_hist_list; item; prev = item, item = item->next)
    {
        diff = strcmp(strsafe(item->title), strsafe(title));

	STBDBG(("hist: compare %s %s diff=%d\n", strsafe(item->title), strsafe(title), diff));

        /* if the title and the URL are the same */
        if (diff == 0 && item->url_hash == h && strcmp(item->url, bare_url) == 0)
        {
            insert = FALSE;

            /* add the fragment on if necessary */
            if (fragment && check_frag_list(item->frag_list, fragment) == NULL)
                add_frag = TRUE;
            break;
        }

        /* if we have gone past where the title would be then insert before here */
        if (diff > 0)
        {
            if (fragment)
                add_frag = TRUE;
            break;
        }
    }

    STBDBG(("hist: global insert %d addfrag %d after %p '%s' '%s' \n",insert, add_frag,  prev, prev ? prev->url : "", prev ? strsafe(prev->title) : ""));

    /* insert new item after prev and before 'item' */
    if (insert)
    {
        new_item = mm_calloc(sizeof(*item), 1);

        new_item->title = strdup(title);
        new_item->url = strdup(bare_url);
        new_item->url_hash = h;

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
        fe_global_history_fragment *f = mm_malloc(sizeof(fe_global_history_fragment *) + strlen(fragment) + 1);

        strcpy(f->fragment, fragment);

        f->next = new_item->frag_list;
        new_item->frag_list = f;
    }

    global_count++;
}

static void fe_global_add(const char *url, const char *title)
{
    char *bare_url, *fragment;
    fragment_parse(url, &bare_url, &fragment);

    fe_global__add(bare_url, fragment, title);

    mm_free(bare_url);
    mm_free(fragment);
}

os_error *fe__global_write_list(FILE *f)
{
    fe_global_history_item *item;
    int i;

    for (item = global_hist_list, i = 0; item; item = item->next, i++)
    {
#if 0
        fe_global_history_fragment *fp;
        STBDBG(("hist: write %p '%s'\n", item, item->url));
        for (fp = item->frag_list; fp; fp = fp->next)
            STBDBG(("hist: frag '%s'\n", fp->fragment));
#endif

	fprintf(f, msgs_lookup("histAIa"), i);

	url_escape_to_file(item->url, f);

        fprintf(f, msgs_lookup("histAIb"), item->title ? item->title : item->url);
        fputc('\n', f);
    }

    return NULL;
}

os_error *fe_global_write_list(FILE *f)
{
    os_error *e;
    char *s;

    fprintf(f, msgs_lookup("histAT"), 0);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histA1"), strsafe(s));

    e = fe__global_write_list(f);
    
    fputs(msgs_lookup("histAF"), f);

    return e;
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

/* ---------------------------------------------------------------------------------------------*/

static int fe_hist_write_item(FILE *f, const fe_history_item *item, int i)
{
    const fe_history_frame_item *hfi;

    /* only wirte out an item if there are no frames */
    if (item->n_frames > 1)
	return FALSE;
    
    fprintf(f, msgs_lookup("histRIa"), i);

    hfi = &item->frame[0];

    url_escape_to_file(hfi->url, f);

    if (item->data.noframes.title)
    {
        char *frag = strrchr(hfi->url, '#');
        if (frag)
            fprintf(f, msgs_lookup("histRIb"), item->data.noframes.title, frag);
        else
            fprintf(f, msgs_lookup("histAIb"), item->data.noframes.title);
    }
    else
    {
        fprintf(f, msgs_lookup("histRIb"), hfi->url);
    }

    fputc('\n', f);

    return TRUE;
}

os_error *fe_history_write_list(FILE *f, const fe_history_item *start, const fe_history_item *current)
{
    const fe_history_item *item;
    int i;
    char *s;

    /* count how far through the list we are */
    for (i = 0, item = start; item && item != current; i++, item = item->prev)
	;
	
    fprintf(f, msgs_lookup("histRT"), i);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histR1"), strsafe(s));

    for (i = 0, item = start; item; i++, item = item->prev)
        fe_hist_write_item(f, item, i);

    fputs(msgs_lookup("histRF"), f);
    fputc('\n', f);
    return NULL;
}

os_error *fe_history_write_combined_list(FILE *f, const fe_history_item *start, const fe_history_item *current)
{
    const fe_history_item *item;
    int i;
    char *s;

    /* back track to place to start */
    /* ANC-00288: added check on item in case current == NULL */
    for (i = 0, item = current; i < N_RECENT_SITES/2 && item && item->next; i++, item = item->next)
        ;

    fprintf(f, msgs_lookup("histCT"), i);
    s = getenv(PROFILE_NAME_VAR);
    fprintf(f, msgs_lookup("histC1"), strsafe(s));

    /* write current places */
    for (i = 0; i < N_RECENT_SITES && item; i++, item = item->prev)
    {
        if (item == current && i != 0)
            fputs(msgs_lookup("histS1"), f);

        fe_hist_write_item(f, item, i);

        if (item == current && i != N_RECENT_SITES - 1)
            fputs(msgs_lookup("histS2"), f);
    }

    fprintf(f, msgs_lookup("histC2"), strsafe(s));

    /* write global list */
    fe__global_write_list(f);

    fputs(msgs_lookup("histCF"), f);
    fputc('\n', f);
    return NULL;
}

os_error *fe_history_show(fe_view v)
{
    return frontend_open_url("ncfrescointernal:openpanel?name=historycombined", v, TARGET_HISTORY, NULL, fe_open_url_NO_CACHE);
}

/* ---------------------------------------------------------------------------------------------*/

/* history functions */

static void fe_free_hist_item(fe_history_item *item)
{
    if (item)
    {
	int i;

	for (i = 0; i < item->n_frames; i++)
	    mm_free(item->frame[i].url);

	if (item->n_frames > 1)
	    mm_free(item->data.frames.frame_specifier);
	else
	    mm_free(item->data.noframes.title);

	mm_free(item);
    }
}

static void fe_history_truncate(fe_view v)
{
    fe_history_item *chop, *c2;

    STBDBGN(("Truncate: last=%p, at=%p at->next=%p\n", v->last, v->hist_at, v->hist_at->next));

    if (v->hist_at == v->last)
	return;

    for (chop = v->hist_at->next; chop; chop = c2)
    {
	c2 = chop->next;

	STBDBGN(("Chop=%p, c2=%p\n", chop, c2));

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

static fe_history_item *fe_history_add(fe_view v, const char *url, const char *title)
{
    fe_history_item *h;

    /* Trim off the history tail if it is there */
    /* This should be trimmed in local history but still kept in global history */
    fe_history_truncate(v);

    if ((h = mm_calloc(1, sizeof(*h))) == NULL)
	return NULL;

    h->frame[0].url = strdup(url);
    h->frame[0].url_hash = string_hash(url);

    h->n_frames = 1;
    h->data.noframes.title = strdup(title);

    v->hist_count++;
    
    STBDBG(("history: %p add '%s' total %d\n", h, url, v->hist_count));

    if (v->first)
    {
	STBDBGN(("h=%p, first=%p, last=%p\n", h, v->first, v->last));

	v->last->next = h;
	h->prev = v->last;
	v->last = h;

	if (v->hist_count > config_history_length)
	{
	    fe_history_item *chop;

	    chop = v->first;

	    STBDBGN(("Chop=%p\n", chop));

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
 */

os_error *fe_history_move(fe_view v, int direction)
{
    fe_history_item *dest = fe_history_get_item(v, direction);

    if (!dest)
	return makeerror(ERR_NO_HISTORY);
    
    switch (direction)
    {
    case history_FIRST:
    case history_PREV:
	sound_event(snd_HISTORY_BACK);
	break;

    case history_NEXT:
    case history_LAST:
	sound_event(snd_HISTORY_FORWARD);
	break;
    }

    /* set up the values to be used when we get the document loaded */
    v->fetching_data.xscroll = dest->frame[0].x_scroll;
    v->fetching_data.yscroll = dest->frame[0].y_scroll;
    v->fetching_data.hist = dest;
    
    /* open the new url */
    return frontend_open_url(dest->frame[0].url, v, NULL, 0, fe_open_url_FROM_HISTORY | fe_open_url_NO_REFERER);
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
    fe_history_item *item = v->hist_at;

    STBDBG(("history: visit '%s'\n", url));

    if (v->dont_add_to_history)
        return 0;

    if (item)
    {
	STBDBG(("history: prev '%s' current '%s' next '%s'\n", 
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

    /* otherwise add it to the history */
    item = fe_history_add(v, url, title);
    if (item)
        v->hist_at = item;

    /* and to the global history if not a frame element */
    if (v->parent == NULL)
        fe_global_add(url, title);

    return 0;
}

/* Store the current state of the view in 'hist_at' */

void fe_history_update_current_state(fe_view v)
{
    if (v->children == NULL && v->parent == NULL && v->hist_at && v->w)
    {
	wimp_wstate state;
	wimp_get_wind_state(v->w, &state);

	STBDBG(("history: %p '%s' write scroll pos %d\n", v->hist_at, v->hist_at->frame[0].url, state.o.box.y1));

	v->hist_at->frame[0].x_scroll = state.o.x + v->margin.x0;
	v->hist_at->frame[0].y_scroll = state.o.y + v->margin.y1;
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

/* eof stbhist.c*/
