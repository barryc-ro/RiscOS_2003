/* > stbhist.c
 *
 */

#include <stdio.h>
#include <string.h>

#include "msgs.h"
#include "wimp.h"

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

#define HISTORY_LEN	32

#define N_RECENT_SITES  7

static fe_global_history_item *global_hist_list = NULL;

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

    for (item = global_hist_list; item; item = item->next)
    {
#if 0
        fe_global_history_fragment *fp;
        fprintf(stderr, "hist: write %p '%s'\n", item, item->url);
        for (fp = item->frag_list; fp; fp = fp->next)
            fprintf(stderr, "hist: frag '%s'\n", fp->fragment);
#endif

	fprintf(f, msgs_lookup("histAIa"));

	url_escape_to_file(item->url, f);

        fprintf(f, msgs_lookup("histAIb"), item->title ? item->title : item->url);
        fputc('\n', f);
    }

    return NULL;
}

os_error *fe_global_write_list(FILE *f)
{
    os_error *e;

    fputs(msgs_lookup("histAT"), f);
    fputs(msgs_lookup("histA1"), f);

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

static void fe_hist_write_item(FILE *f, const fe_history_item *item)
{
    fprintf(f, msgs_lookup("histRIa"));

    url_escape_to_file(item->url, f);

    if (item->title)
    {
        char *frag = strrchr(item->url, '#');
        if (frag)
            fprintf(f, msgs_lookup("histRIb"), item->title, frag);
        else
            fprintf(f, msgs_lookup("histAIb"), item->title);
    }
    else
    {
        fprintf(f, msgs_lookup("histRIb"), item->url);
    }

    fputc('\n', f);
}

os_error *fe_history_write_list(FILE *f, void *handle)
{
    const fe_history_item *item;
    const fe_history_item *current = (const fe_history_item *)handle;

    fputs(msgs_lookup("histRT"), f); 
    fputs(msgs_lookup("histR1"), f);

    for (item = current; item; item = item->next)
        fe_hist_write_item(f, item);

    fputs(msgs_lookup("histRF"), f);
    fputc('\n', f);
    return NULL;
}

os_error *fe_history_write_combined_list(FILE *f, void *handle)
{
    const fe_history_item *item;
    const fe_history_item *current = (const fe_history_item *)handle;
    int i;

    fputs(msgs_lookup("histCT"), f);
    fputs(msgs_lookup("histC1"), f);

    /* back track to place to start */
    /* ANC-00288: added check on item in case current == NULL */
    for (i = 0, item = current; i < N_RECENT_SITES/2 && item && item->next; i++, item = item->next)
        ;

    /* write current places */
    for (i = 0; i < N_RECENT_SITES && item; i++, item = item->prev)
    {
        if (item == current && i != 0)
            fputs(msgs_lookup("histS1"), f);

        fe_hist_write_item(f, item);

        if (item == current && i != N_RECENT_SITES - 1)
            fputs(msgs_lookup("histS2"), f);
    }

    fputs(msgs_lookup("histC2"), f);
    fputc('\n', f);

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

static void fe_free_hist_item(fe_history_item *i)
{
    if (i == NULL)
	return;

    mm_free(i->url);
    mm_free(i->title);
    mm_free(i);
}

static void fe_history_truncate(fe_view v)
{
    fe_history_item *chop, *c2;

#if 0
fprintf(stderr, "Truncate: last=%p, at=%p(%d) at->next=%p\n",
	v->last, v->hist_at, v->hist_at->seq_no, v->hist_at->next);
#endif

    if (v->hist_at == v->last)
	return;

    for(chop = v->hist_at->next; chop; chop = c2)
    {
	c2 = chop->next;
#if 0
	fprintf(stderr, "Chop=%p(%d), c2=%p\n", chop, chop->seq_no, c2);
#endif
	fe_free_hist_item(chop);
    }

    v->last = v->hist_at;
    v->last->next = 0;
}

/*
 * This url may have a fragment in it
 */

static fe_history_item *find_entry(const char *url, fe_history_item *start, int direction)
{
    fe_history_item *item;
    int h = string_hash((char *)url);

    item = start;
    while (item)
    {
        if (h == item->url_hash &&
            strcmp(url, item->url) == 0)
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

    h = mm_calloc(1, sizeof(*h));

    if (h == 0)
	return NULL;

    h->url = strdup(url);
    h->title = strdup(title);
    h->url_hash = string_hash((char *)url);

/*    h->scroll_pos = fe_get_cvt(v->w).scy;*/

    STBDBG(("history: %p add '%s'\n", h, url));

    if (v->first)
    {
#if 0
	fprintf(stderr, "h=%p, first=%p, last=%p\n", h, v->first, v->last);
#endif
	v->last->next = h;
	h->prev = v->last;
	h->seq_no = v->last->seq_no + 1;
	v->last = h;
#if 0
	fprintf(stderr, "Last#=%d, first#=%d\n", v->last->seq_no, v->first->seq_no);
#endif
	if ( (v->last->seq_no - v->first->seq_no) > HISTORY_LEN )
	{
	    fe_history_item *chop;

	    chop = v->first;
#if 0
	    fprintf(stderr, "Chop=%p\n", chop);
#endif
	    v->first = v->first->next;
	    v->first->prev = NULL;

	    fe_free_hist_item(chop);
	}

    }
    else
    {
	v->first = v->last = h;
    }

    return h;
}

/* ---------------------------------------------------------------------------------------------*/

char *fe_history_get_url(fe_view v, int a)
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

    if (dest == NULL)
	return NULL;

    return dest->url;
}

int fe_history_possible(fe_view v, int direction)
{
    return fe_history_get_url(v, direction) != NULL;
}

/*
 * This should use hist_at if it is differnet to the current page

 */

os_error *fe_history_move(fe_view v, int direction)
{
    char *url = fe_history_get_url(v, direction);

    if (url) switch (direction)
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

    return url == NULL ? makeerror(ERR_NO_HISTORY) : frontend_open_url(url, v, NULL, 0, fe_open_url_FROM_HISTORY);
}

void fe_history_dispose(fe_view v)
{
    if (v->first)
    {
	v->hist_at = v->first;
	fe_history_truncate(v);
	fe_free_hist_item(v->first);
	v->first = v->hist_at = v->last = 0;
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
		item->prev ? strsafe(item->prev->url) : "", 
		strsafe(item->url), 
		item->next ? strsafe(item->next->url) : ""));

        /* If the url is of the current page, ignore it */
        if (item->url && strcmp(url, item->url) == 0)
	    return 0;

        /* If the user went back and then follow the same link, move forward */
        if (item->next && item->next->url && strcmp(item->next->url, url) == 0)
        {
	    v->hist_at = item->next;
	    return item->next->scroll_pos;
        }

        /* If the user is going back, move back */
        if (item->prev && item->prev->url && strcmp(item->prev->url, url) == 0)
        {
	    v->hist_at = item->prev;
	    return item->prev->scroll_pos;
        }
    }

/* if it is in the history somewhere then set history to that page
    item = find_entry(url, v->last, history_PREV);
    if (item)
    {
    STBDBG(("history: %p found '%s' scroll %d\n", item, url, item->scroll_pos));
        v->hist_at = item;
        return item->scroll_pos;
    }
 */

    /* otherwise add it to the history */
    item = fe_history_add(v, url, title);
    if (item)
        v->hist_at = item;

    /* and to the global history if not a frame element */
    if (v->parent == NULL)
        fe_global_add(url, title);

    return 0;
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

#if 0
static int cache_enumerate(int *context, char **purl, char **ptitle)
{
    BOOL found_file = FALSE;

    do
    {
        int dir, i;
        cache_dir *dp;
        cache_item *ip;

        dir = *context / N_FILES_PER_DIR;
        i   = *context % N_FILES_PER_DIR;

        dp = &cache_dirs[dir];
        if (dp->items == NULL)
        {
            *context = -1;
            return 0;
        }

        ip = &dp->items[i];
        if (ip->file_num != NO_FILE)
        {
            if (purl)
                *purl = ip->url;
            if (ptitle)
                *purl = ip->title;

            found_file = TRUE;
        }

        (*context)++;
    }
    while (!found_file);

    return 1;
}
#endif

/* ----------------------------------------------------------------------------------------------- */

/* eof stbhist.c*/
