/* -*-c-*- */

/* 01-03-96 DAF Handling of new table structures */
/* 21-03-96 DAF New formatter */
/* 26-03-96 DAF Rewrote borders more carefully */
/* 22-03-96 SJM new functions antweb_update_item_trim, antweb_handle_url, backend_item_pos_info
                added fetching to dispose_doc.
 * 26-03-96 SJM replaced calls to frontend_view_set_dim with be_set_dim which can reformat if a scroll bar
                has been added.
   01-04-96 SJM update backend_reset_width to cope with relaying frames
   02-04-96 SJM added backend_temp_file_name, backend_temp_file_register, backend_check_meta
   03-04-96 SJM added backend_select_item, backend_find_selected
   15-04-96 SJM fixed item_pos_info so ISMAP link doesn't overwrite USEMAP link,
                and so that coordinates are calculated properly.
   18-04-96 SJM disabled link following when ctrl held down (as with oimage_click()), make select instead
                make place_caret() return previous position
                make update_link also affect non-links.
   30-04-96 SJM added new fields to frontend_view_dimensions for laying out frames
                added hooks to layout for frame resizing in dispose_doc and new functions
   07-05-96 SJM image_tile change
   09-05-96 SJM Fixed goto_fragment. If fragment is first in document then ai->first==NULL.
   20-05-96 SJM in doc_abort discard any images that are coming down
   23-05-96 SJM calls to cache functions now go to access_ wrappers
   04-06-96 SJM new call to image_os_to_pixels for image coord translation
   11-06-96 SJM image_info() no longer does config image scaling so changes to offset this
   20-06-96 SJM added check for imagemap error in doc_pos_info.
   24-06-96 SJM removed use of basetarget. changed image_change fns to cope with image_cb_status_WORLD
   26-06-96 NvS Many bits of code moved out into either stream.c or table.c
   08-07-96 SJM Moved view_visit call into its own be_view_visit() call.
   18-07-96 SJM Added a SECURE bit to the item flags and doc flags.
   19-07-96 SJM changes submit form in line with form element list changes
		tries optimising redraw areas in doc_image_change to avoid redrawing borders
		in animations - doesn't seem to do the job
   23-07-96 SJM added antweb_get_edges() for object code to use for sizeing
   07-08-96 SJM doc_image_change now only updates the changed portion of an animation (needed for anti-twittering)
   15-08-96 NvS Tweeks to backend_item_pos_info to deal better with local image maps without
                any link on the image and to return NULL links when there is not map link
   19-08-96 SJM change to be_set_dim, loops incase both scroll bars change one qfter another and calls object update code
   27-08-96 NvS Fixed a small bug in the progressive parse reformat code that could cause NULL pointer deindexing.
   29-08-96 NvS Fixed background image code to get redraw correct when another view has flushed the background image.
   10-09-96 SJM rectangle_fn now returns int so render_background can return a bg identifier.
   11-09-96 SJM split backend_doc_click into two parts so that activate_link doesn't have to go to x,y and badk to a pointer
   18-09-96 SJM changed backend_update_link to only redraw links that change state
   26-02-97 DAF FVPR changes.
*/

/* WIMP code for the ANTWeb WWW browser */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <setjmp.h>

#include "memwatch.h"

#include "os.h"
#include "wimp.h"
#include "msgs.h"
#include "font.h"
#include "swis.h"
#include "colourtran.h"
#include "bbc.h"
#include "alarm.h"
#include "akbd.h"

#include "rid.h"
#include "antweb.h"
#include "webfonts.h"
#include "url.h"
#include "util.h"
#include "makeerror.h"
#include "images.h"
#include "tables.h"
#include "indent.h"

#include "status.h"

#include "filetypes.h"
#include "consts.h"
#include "object.h"
#include "auth.h"
#include "config.h"
#include "parsers.h"

#include "interface.h"
#include "savedoc.h"

#include "render.h"
#include "rcolours.h"
/*#include "fastfont.h"*/

#include "printing.h"
#include "drawfile.h"
#include "imagemap.h"
#include "layout.h"

#include "version.h"
#include "verstring.h"

#include "myassert.h"

#include "stream.h"
#include "debug.h"
#include "backend.h"
#include "access.h"

#include "objects.h"
#include "fvpr.h"
#include "pluginfn.h"
#include "format.h"
#include "gbf.h"
#include "object.h"

#ifdef STBWEB_BUILD
#include "http.h"
#else
#include "../http/httppub.h"
#endif

#include "coalesce.h"

#if DEBUG
#include "unwind.h"
#endif

#ifdef PLOTCHECK
#include "rectplot.h"
#endif

#include "guarded.h"

#if UNICODE
#include "Unicode/encoding.h"
#include "Unicode/languages.h"
#endif

#ifndef ITERATIVE_PANIC
#define ITERATIVE_PANIC 0
#endif

/* This is on its own because the include file that it is in includes lots of others */
/* Not that this is a good excuse. */
extern void rid_zero_widest_height_from_item(rid_text_item *item);

/**********************************************************************/

rid_text_item *antweb_prev_text_item(rid_text_item *ti);
static void antweb_doc_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url);
static access_complete_flags antweb_doc_complete(void *h, int status, char *cfile, char *url);
static void be_update_image_info(be_doc doc);
static void antweb_doc_background_change(void *h, void *i, int status, wimp_box *box);
static void be_refresh_document(int at, void *h);
static void be_doc_fetch_bg(antweb_doc *doc);
static void be_dispose_doc_contents( be_doc doc );

void be_caption_stream_origin(rid_table_item *table, int *dx, int *dy);
void be_cell_stream_origin(rid_table_item *table, rid_table_cell *cell, int *dx, int *dy);

#if TIMEOUT
static void CheckLastModified( be_doc doc );
extern void TimeoutError(void); /* grody.s */
#endif

#ifdef BUILDERS
extern
#else
static
#endif
void be_document_reformat_tail(antweb_doc *doc, rid_text_item *oti, int user_width);

/**********************************************************************/

static be_doc document_list = NULL;

/**********************************************************************/

/* pdh: A bit of a kludge here to stop all windows falling over
 * when one does. Type 5's are caught and the corresponding views
 * shut down. See antweb_doc_progress for usage. If "inside" a guarded
 * block, memwatch raises SIGUSR1 rather than exit()ing. Likewise
 * ASSERT raises SIGUSR2.
 */

#if CATCHTYPE5
typedef void sighandler(int);

static antweb_doc *currentdoc = NULL;
jmp_buf jbGuard;
int guarded = 0;
sighandler *oldtype5 = NULL;
sighandler *oldtype8 = NULL;
sighandler *oldtype9 = NULL;

#ifdef __acorn
#pragma no_check_stack
#endif

void antweb_signal_handler( int sig )
{
    if ( currentdoc )
    {
        if ( ( currentdoc->flags & doc_flag_WRECKED ) == 0 )
        {
            be_dispose_doc_contents( currentdoc );
    	    currentdoc->flags |= doc_flag_WRECKED;
	    mm_minor_panic( sig == SIGUSR1 ? "memlow" : "memerror" );
	}
    }
    longjmp( jbGuard, 1 );
}

void sig_guard( antweb_doc *doc )
{
    if ( guarded++ == 0 )
    {
        currentdoc = doc;
        oldtype5 = signal( SIGSEGV, antweb_signal_handler );
        oldtype8 = signal( SIGUSR1, antweb_signal_handler );
        oldtype9 = signal( SIGUSR2, antweb_signal_handler );
    }
}

void sig_unguard( void )
{
    if ( --guarded == 0 )
    {
        signal( SIGSEGV, oldtype5 );
        signal( SIGUSR1, oldtype8 );
        signal( SIGUSR2, oldtype9 );
        currentdoc = NULL;
    }
}

BOOL sig_guarded( void )
{
    return guarded > 0;
}

#ifdef __acorn
#pragma check_stack
#endif

#endif

/**********************************************************************

  DAF: SPIT! A precise definition of the values being retrieved would
  be nice. Hopefully I've got the right values.

 */

#if 1
int antweb_get_edges(const rid_text_item *ti, int *left, int *right)
{
    int leftend = 0, rightend = 0;

    if (ti->line)
    {
	leftend = ti->line->left_margin;
#if 1
        /* pdh: left_margin includes any floaters these days */
        rightend = ti->line->floats ? ti->line->floats->right_margin
                                    : ti->line->st->fwidth;
#else
	rightend = ti->line->st->fwidth;

        if (ti->line->floats)
        {
	    rid_float_item *fi;
	    int most;

	    for (most = 0, fi = ti->line->floats->left; fi != NULL; fi = fi->next)
		most = fi->entry_margin + ti->width;

	    if (most > 0)
		leftend = most;

	    for (most = 0, fi = ti->line->floats->right; fi != NULL; fi = fi->next)
		most = fi->entry_margin - ti->width;

	    if (most > 0)
		rightend = most;
        }
#endif
    }

    if (left)
        *left = leftend;
    if (right)
        *right = rightend;

    return rightend - leftend;
}
#else
int antweb_get_edges(const rid_text_item *ti, int *left, int *right)
{
    int leftend = 0, rightend = 0;

    if (ti->line)
    {
        if (ti->line->st)
            rightend = ti->line->st->fwidth;

        if (ti->line->floats)
        {
            if (ti->line->floats->left && ti->line->floats->left->ti)
        	leftend += ti->line->floats->left->ti->width;

            if (ti->line->floats->right && ti->line->floats->right->ti)
    	        rightend -= ti->line->floats->right->ti->width;
        }
    }

    if (left)
        *left = leftend;
    if (right)
        *right = rightend;

    return rightend - leftend;
}
#endif

/*****************************************************************************

    item is a table.  i is an image handle.  Determine if the table contains
    an item using the image handle or not.

*/

static int rid_table_holds_image(rid_text_item *item, void *i, antweb_doc *doc)
{
        rid_table_item *table = ((rid_text_item_table *)item)->table;
        rid_text_item *tabscan;
        rid_table_cell *cell;
        int x = -1, y = 0, have_image = FALSE;

        ASSERT(item->tag == rid_tag_TABLE);

        if (table->caption)
        {
                for (tabscan = table->caption->stream.text_list;
                     tabscan != NULL;
                     tabscan = rid_scanf(tabscan) )
                {
                        if (tabscan->tag == rid_tag_TABLE)
                        {
			    if (rid_table_holds_image(tabscan, i, doc) )
			    {
                                have_image = TRUE;
                                break;
			    }
                        }
                        else if (object_table[tabscan->tag].imh != NULL &&
                                (object_table[tabscan->tag].imh)(tabscan, doc, object_image_OBJECT) == i)
                        {
                                have_image = TRUE;
                                break;
                        }
                }

        }

        if (!have_image)
        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL )
        {
                for (tabscan = cell->stream.text_list;
                     tabscan != NULL;
                     tabscan = rid_scanf(tabscan) )
                {
                        if (tabscan->tag == rid_tag_TABLE)
                        {
			    if (rid_table_holds_image(tabscan, i, doc) )
			    {
                                have_image = TRUE;
                                break;
			    }
                        }
                        else if (object_table[tabscan->tag].imh != NULL &&
                           (object_table[tabscan->tag].imh)(tabscan, doc, object_image_OBJECT) == i)
                        {
                                have_image = TRUE;
                                break;
                        }
                }
        }

        return have_image;
}


/**********************************************************************/

#define CLESS(x)	(cless ? toupper(x) : (x))

static int be_find_test_text(be_doc doc, be_item ti, char *text, int flags)
{
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    rid_header *rh = doc->rh;
    char *p, *q;
    int c1;
    int i, j;
    int cless;
    int hit = 0;

    flexmem_noshift();

    cless = flags & be_find_CASELESS;

    c1 = CLESS(text[0]);
    p = rh->texts.data + tit->data_off;

    for(i=0; p[i]; i++)
    {
	if( CLESS(p[i]) == c1)
	{
	    rid_text_item *ti2 = ti;
	    q = p+i+1;
	    j = 1;

	    while (text[j])
	    {
		while (*q == 0)
		{
		    rid_text_item_text *tit2;

		    /*do { ti2 = ti2->next;*/
		    /*} while (ti2 && ti2->tag != rid_tag_TEXT);*/
                    ti2 = rid_scan(ti2, SCAN_RECURSE | SCAN_FILTER | rid_tag_TEXT);

		    tit2 = (rid_text_item_text *) ti2;

		    if (tit2)
			q = rh->texts.data + tit2->data_off;
		    else
			break;
		}

		if ((*q == 0) || (CLESS(*q) != CLESS(text[j])) )
		    break;

		q++;
		j++;
	    }

	    if (text[j] == 0)
	    {
		hit = 1;
		break;
	    }
	}
    }

    flexmem_shift();

    return hit;
}

be_item backend_find(be_doc doc, be_item start, char *text, int flags)
{
    const int scan_flag = ((flags & be_find_BACKWARDS) ? SCAN_BACK : SCAN_FWD) | SCAN_RECURSE;

    if (text[0] == 0)
	return NULL;

    if (start == NULL)
    {
	start = (flags & be_find_BACKWARDS) ?
	    doc->rh->stream.text_last :
	    doc->rh->stream.text_list;
    }
    else
    {
        start = rid_scan(start, scan_flag);
    }

    while(start)
    {
	if ((start->tag == rid_tag_TEXT) && be_find_test_text(doc, start, text, flags))
	{
	    wimp_box bb;

	    if (backend_doc_item_bbox(doc, start, &bb) == NULL)
	    {
		frontend_view_ensure_visable(doc->parent, 0, bb.y1, bb.y1);
	    }

	    return start;
	}
        start = rid_scan(start, scan_flag);
    }

    return NULL;
}

os_error *backend_doc_item_bbox(be_doc doc, be_item ti, wimp_box *bb)
{
    if (ti == NULL)
	return makeerror(ERR_BAD_CONTEXT);

    if (!stream_find_item_location(ti, &(bb->x0), &(bb->y0)))
	return makeerror(ERR_BAD_CONTEXT);

#if USE_MARGINS
    bb->x0 += doc->margin.x0;
    bb->y0 += doc->margin.y1;
#endif

    bb->x1 = bb->x0 + ti->width;
    bb->y1 = bb->y0 + ti->max_up;
    bb->y0 = bb->y0 - ti->max_down;

    return NULL;
}


/*****************************************************************************

Remember: we want the offset within the item, relative to
the base line of the item, which may not be the bottom
left pixel of the item.

Entry gives us an offset relative to 0,0 as leftmost point of
the baseline.

*/

os_error *backend_doc_locate_item(be_doc doc, int *x, int *y, be_item *ti)
{
    RENDBGN(("Locate item: doc = %p, x=%d, y=%d\n", doc, *x, *y));

    *ti = NULL;

    if (doc == NULL || doc->rh == NULL)	/* The validity of the stream is checked for us */
	return NULL;

#if USE_MARGINS
    *x -= doc->margin.x0;
    *y -= doc->margin.y1;
#endif
    return stream_find_item_at_location(&doc->rh->stream, x, y, ti);
}

/*
 * This function was split off from antweb_doc_click so that others can calls it, a target of
 * NULL will use the document default. a target of _blank forces a new window to open.
 */

os_error *antweb_handle_url(be_doc doc, rid_aref_item *aref, const char *query, const char *target)
{
    os_error *e = NULL;
    BOOL new_win = target && strcasecomp(target, "_blank") == 0;
    const char *href = aref->href;

    BENDBG(( "antweb_handle_url: doc %p aref %p for '%s' target '%s'\n", doc, aref, strsafe(href), strsafe(target)));

    if (aref->flags & rid_aref_LABEL)
    {
	be_item ti = NULL;
	if (href)
	{
	    ti = backend_locate_id(doc, href);
	}
	else
	{
	    ti = aref->first;
	    while (ti && ti->aref == aref)
	    {
		if (ti->tag == rid_tag_INPUT || ti->tag == rid_tag_SELECT || ti->tag == rid_tag_TEXTAREA)
		    break;

		ti = rid_scanf(ti);
	    }
	}

	backend_activate_link(doc, ti, 0);
    }
    else
#ifndef STBWEB			/* NCFresco handles the fragment link through open_url */
    if (!href)
    {
	return NULL;
    }
    else if ( !new_win && href[0] == '#' )
    {
        /* Special case a move to a fragment in the same document and the same window */
        e = backend_goto_fragment(doc, (char *)&href[1]);
    }
    else
#endif
    {
	char *base;
	char *dest;

	base = url_join(BASE(doc), (char *)href);
#ifndef STBWEB			/* NCFresco handles the fragment link through open_url */
	if ( doc->url
	     && *(doc->url)
	     && strncmp( base, doc->url, strlen(doc->url) ) == 0
	     && base[strlen(doc->url)] == '#' )
        {
            /* Verbose way of specifying a fragment of the same document,
             * but it does happen.
             */
            mm_free( base );
            return backend_goto_fragment( doc, 1+(char*)strchr(href,'#') );
        }
#endif
	if (query && *query)
	{
	    dest = url_join(base, (char *)query);
	    mm_free(base);
	}
	else
	    dest = base;

	if (akbd_pollsh())
	{
	    e = savedoc(dest, "Document", -1);
	}
	else
	{
	    e = frontend_open_url(dest, /* new_win?NULL:*/ doc->parent, (char *)target, NULL, 0);
	}

	mm_free(dest);
    }
    return e;
}

/*
 * This function no longer follows links for objects that have a click function. The must
 * call antweb_handle_url or antweb_place_caret themselves.
 */

static void backend__doc_click(be_doc doc, be_item ti, int x, int y, wimp_bbits bb)
{
    /* _don't_ correct the x,y coordinates for margins here */
    doc->threaded++;

    if (ti)
    {
        if (object_table[ti->tag].click != NULL)
        {
	    (object_table[ti->tag].click)(ti, doc->rh, doc, x, y, bb);
	}
	else if (ti->aref && (ti->aref->href || (ti->aref->flags & rid_aref_LABEL)))
	{
#if 1 /* pdh 03-Apr-98: was 0, but this made ctrl-click on images follow the
       * link as well as selecting the image.
       */
	    if (akbd_pollctl())
	    {
#if 0 /* pdh 03-Apr-98: was !defined( BUILDERS ) */
	        backend_update_link(doc, ti, -1);
#endif
	    }
	    else
#endif
	    {
		BOOL follow_link = TRUE;
                if (config_display_time_activate)
                {
#ifndef BUILDERS
	            backend_update_link_activate(doc, ti, 1);
                    follow_link = wait_for_release(config_display_time_activate);

                    /* pdh: Fresco seemed to want this, NCFresco may not */
                    if ( follow_link )
                        ti->aref->flags |= rid_aref_CHECKED_CACHE + rid_aref_IN_CACHE;

	            backend_update_link_activate(doc, ti, 0);
#endif
    	        }
    	        else
    	        {
    	            ti->aref->flags |= rid_aref_CHECKED_CACHE + rid_aref_IN_CACHE;
    	        }

		if (follow_link)
		    frontend_complain(antweb_handle_url(doc, ti->aref, NULL,
							(bb & wimp_BRIGHT) ? "_blank" : ti->aref->target));
            }
        }
        else
        {
	    antweb_default_caret(doc, TRUE);
	}
    }
    else
    {
	antweb_default_caret(doc, TRUE);
    }

    doc->threaded--;
    if (doc->pending_delete)
	backend_dispose_doc(doc);
}

os_error *backend_doc_click(be_doc doc, int x, int y, wimp_bbits bb)
{
    rid_text_item *ti;
    os_error *ep;

    ep = backend_doc_locate_item(doc, &x, &y, &ti);

    BENDBG(( "doc click on item %p offset %d,%d error %p tag %d click fn %p\n",
	    ti, x, y, ep, ti ? ti->tag : -1, ti ? object_table[ti->tag].click : 0));

    if (!ep)
	backend__doc_click(doc, ti, x, y, bb);

    return ep;
}

/*
 * This variant on backend_item_info takes the coords relative to the baseline and can thus decode the
 * link within a client side image map.
 */

os_error *backend_item_pos_info(be_doc doc, be_item ti, int *px, int *py, int *flags, char **link, void **im, char **title)
{
    int f = 0;
    void *imh = NULL;

/*     if (ti) { BENDBG(( "Asked for info on item at %p\n", ti)); } */

    if (link)
        *link = NULL;

    if (title)
	*title = NULL;

    if (ti)
    {
	/* amazingly this didn't seem to get called before
	 *
	 * pdh: moved out of "if" below, as we always need imh in order
	 * to set be_item_info_IMAGE ... Fresco uses this to know whether
	 * to offer an Image-> submenu eg Image->Save

	 * sjm: moved out further so that its use can be shared and so that
	 * it is correctly set for all types that use images.
	 */

	if (object_table[ti->tag].imh != NULL)
	    imh = (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

        if (ti->tag == rid_tag_IMAGE)
	{
	    rid_text_item_image *tii = (rid_text_item_image *)ti;
	    int xx = *px - tii->hgap;
	    int yy = (ti->max_up) - tii->vgap - *py;

	    image_os_to_pixels((image)tii->im, &xx, &yy, doc->scale_value);
	    *px = xx;
	    *py = yy;

	    if (((rid_text_item_image *)ti)->usemap)
	    {
		f |= be_item_info_USEMAP;

		if (link || title)
		{
		    rid_map_item *map;
		    rid_area_item *area;

		    map = imagemap_find_map(doc->rh, tii->usemap);
		    if (map && (map->flags & rid_map_ERROR) == 0)
		    {
			area = imagemap_find_area(map, xx, yy);
			if (area && area->href)
			{
			    if (link)
				*link = area->href;
			    if (title)
				*title = area->alt;
			    f |= be_item_info_LINK;
			}
		    }
		}
	    }
        }

	if (ti->aref)
	{
	    if (ti->aref->flags & rid_aref_LABEL)
	    {
		f |= be_item_info_LABEL;

		if (link && !*link)
		    *link = ti->aref->href;
	    }
	    else if (ti->aref->href && (f & be_item_info_USEMAP) == 0)
	    {
		f |= be_item_info_LINK;

		if (link && !*link)
		    *link = ti->aref->href;

		if (title && !*title)
		    *title = ti->aref->title;

		if (ti->tag == rid_tag_IMAGE && ( ((rid_text_item_image *) ti)->flags & rid_image_flag_ISMAP) )
		    f |= be_item_info_ISMAP;
	    }
	}

	if (ti->tag == rid_tag_INPUT)
	{
	    rid_text_item_input *tii = (rid_text_item_input *) ti;
	    rid_input_item *ii = tii->input;

	    switch (ii->tag)
	    {
	    case rid_it_TEXT:
	    case rid_it_PASSWD:
		f |= be_item_info_INPUT;
		/* Nasty NCFresco stuff. Set bit if the input doesn't require a keyboard */
		if ((ii->flags & (rid_if_NUMBERS|rid_if_PBX)) == rid_if_NUMBERS)
		    f |= be_item_info_NUMBERS;
		break;
	    case rid_it_IMAGE:
		/* Do set the ISMAP flag for an IMAGE unless the NOCURSOR flag is set */
		if (imh && (ii->flags & rid_if_NOCURSOR) == 0)
		    f |= be_item_info_ISMAP;
		/* deliberate fall-through */

	    case rid_it_SUBMIT:
		f |= be_item_info_ACTION;
		if (ii->base.parent->action)
		{
	            char *action = url_join(BASE(doc), ii->base.parent->action);
                    if (strncasecomp(action, "https:", 6) == 0)
                        f |= be_item_info_SECURE;
                    mm_free(action);

		    /* Don't set the link flag, use the fact that be_item_info_ACTION is set */
		    if (link)
			*link = ii->base.parent->action;
		}
		break;
	    case rid_it_RESET:
	    case rid_it_BUTTON:
	    case rid_it_RADIO:
	    case rid_it_CHECK:
	    default:
		f |= be_item_info_BUTTON;
		break;
	    }
	}

        if (ti->tag == rid_tag_TEXTAREA)
            f |= be_item_info_INPUT;

        if (ti->tag == rid_tag_SELECT)
            f |= be_item_info_MENU;

	if (ti->tag == rid_tag_OBJECT)
	{
	    rid_text_item_object *tio = (rid_text_item_object *)ti;
	    switch (tio->object->type)
	    {
	    case rid_object_type_PLUGIN:
		f |= be_item_info_PLUGIN;
		break;
	    }
	}

    }
    else
    {
	if (doc != 0)
	{
	    if (doc->rh->bgt & rid_bgt_IMAGE)
	    {
		imh = doc->rh->tile.im;
	    }
	}
    }

    if (im)
    {
	*im = imh;
    }

    if (imh)
    {
	f |= be_item_info_IMAGE;
    }

    if (flags)
	*flags = f;
#if 0
    fprintf(stderr, "Info on item: flags = %d, link='%s'\n", f, ((f & be_item_info_LINK) && link && *link) ? *link : "<none>");
#endif
    return NULL;
}

os_error *backend_item_info(be_doc doc, be_item ti, int *flags, char **link, void **im)
{
    int x=0, y=0;

    return backend_item_pos_info(doc, ti, &x, &y, flags, link, im, NULL);
}

os_error *backend_doc_info(be_doc doc, int *flags, int *ftype, char **url, char **title)
{
    int f = 0;

    if (flags)
    {

	if (doc->rh->flags & rid_hf_ISINDEX)
	    f |= be_doc_info_ISINDEX;
	if (doc->rh->bgt & rid_bgt_IMAGE)
	    f |= be_doc_info_HAS_BG;
	if (doc->rh->flags & rid_hf_HTML_ERRS)
	    f |= be_doc_info_HTML_ERRS;
	if (access_test_cache(doc->url) != access_test_NOT_PRESENT)
	    f |= be_doc_info_IN_CACHE;
	if (doc->ah || doc->ph)
	    f |= (be_doc_info_FETCHING | be_doc_info_INCOMPLETE);
	if (doc->flags & doc_flag_INCOMPLETE)
	    f |= be_doc_info_INCOMPLETE;

	/* FIXME: this is a bit inelegant...*/
	if (strncasecomp(doc->url, "https:", 6) == 0)
	    f |= be_doc_info_SECURE;

	*flags = f;
    }

    if (ftype)
    {
	/* Finding out the file type might be a pain, only do it if we need it */
	if (doc->cfile == NULL || (*ftype = file_type(doc->cfile)) == -1)
	    *ftype = FILETYPE_HTML;
    }

    if (url)
	*url = doc->url;
    if (title)
	*title = doc->rh ? doc->rh->title : NULL;

    return NULL;
}

os_error *backend_doc_file_info(be_doc doc, int *ft, int *load, int *exec, int *size)
{
    if (ft)
    {
	if (doc->cfile == NULL || (*ft = file_type(doc->cfile)) == -1)
	    *ft = FILETYPE_HTML;
    }

    if (load)
	*load = doc->file_load_addr;
    if (exec)
	*exec = doc->file_exec_addr;
    if (size)
	*size = doc->file_size;

    return NULL;
}

os_error *backend_image_info(be_doc doc, void *imh, int *flags, int *ftype, char **url)
{
    image im = (image) imh;
    int fi;
    int f = 0;

    if (im == NULL)
	return makeerror(ERR_NOT_AN_IMAGE);

    image_info(im, 0, 0, 0, &fi, ftype, url);
    if (fi & image_flag_FETCHED)
	f |= be_image_info_FETCHED;

    if (fi & image_flag_RENDERABLE)
	f |= be_image_info_RENDERABLE;

    if (fi & image_flag_DEFERRED)
	f |= be_image_info_DEFERED;

    if (fi & image_flag_ERROR)
	f |= be_image_info_ERROR;

    if (fi & image_flag_INTERLACED)
	f |= be_image_info_INTERLACED;

    if (fi & image_flag_MASK)
	f |= be_image_info_MASK;

    if (fi & image_flag_ANIMATION)
	f |= be_image_info_ANIMATION;

    if (fi & image_flag_BLACKLIST)
	f |= be_image_info_BLACKLIST;

    if (flags)
	*flags = f;

    return NULL;
}

extern os_error *antweb_doc_ensure_font( be_doc doc, int whichfont )
{
#ifndef BUILDERS
    if ( !GETFONTUSED( doc, whichfont ) )
    {
        SETFONTUSED( doc, whichfont );
        return webfont_find_font( whichfont );
    }
#endif
    return NULL;
}

int antweb_getwebfont(antweb_doc *doc, rid_text_item *ti, int base)
{
    return backend_getwebfont(doc, ti->flag & rid_flag_WIDE_FONT, ti->language, ti->st.wf_index, base);
}

int backend_getwebfont(be_doc doc, BOOL wide, int language, int font1, int base)
{
    int whichfont = -1;

#if UNICODE
    if (wide)
    {
	static int fonts[] =
	{
	    -1,	/* unknown/any */
	    -1, /* WEBFONT_ENGLISH */
	    WEBFONT_JAPANESE,
	    WEBFONT_CHINESE,
	    WEBFONT_KOREAN,
	    WEBFONT_GREEK,
	    WEBFONT_RUSSIAN,
	    WEBFONT_HEBREW
	};

	int lang = language ? language : doc->rh->language_num;
	whichfont = (font1 & WEBFONT_SIZE_MASK) | fonts[lang];
    }
#else
    if (wide)
	whichfont = (font1 & WEBFONT_SIZE_MASK) | WEBFONT_JAPANESE;
#endif

    if (whichfont == -1)
    {
	if (base != -1)
	    whichfont = base;
	else
	    whichfont = font1;
    }

    /* pdh: autofit bodge */
    if ( gbf_active( GBF_AUTOFIT ) )
    {
        if ( doc->scale_value < 100
             && ( (whichfont & WEBFONT_FLAG_SPECIAL) == 0 )
             && ( gbf_active(GBF_AUTOFIT_ALL_TEXT) || (whichfont & WEBFONT_FLAG_FIXED) == WEBFONT_FLAG_FIXED )
             && ( (whichfont & WEBFONT_SIZE_MASK) > 0 ) )
        {
           /* make it one size smaller */
           TASSERT( WEBFONT_SIZE_SHIFT == 0 );

           whichfont -= 1;
        }
    }

    antweb_doc_ensure_font( doc, whichfont );

#if UNICODE && defined(RISCOS)
    /* if we are claiming a wide font then set it to the appropriate encoding */
    if (wide)
	webfont_set_wide_format(whichfont);
#endif

    return whichfont;
}

os_error *backend_image_size_info(be_doc doc, void *imh, int *width, int *height, int *bpp)
{
    image im = (image) imh;
    os_error *ep;
    int x, y;

    if (im == NULL)
	return makeerror(ERR_NOT_AN_IMAGE);

    ep = image_info(im, &x, &y, bpp, 0, 0, 0);
    if (ep)
	return ep;

    /* don't need image scaling here as image_ionfo returns real size */
    image_os_to_pixels(im, &x, &y, 100);

    if (width)
        *width = x;
    if (height)
        *height = y;

    return NULL;
}

os_error *backend_image_file_info(be_doc doc, void *imh, int *load, int *exec, int *size)
{
    image im = (image) imh;

    if (im == NULL)
	return makeerror(ERR_NOT_AN_IMAGE);

    return image_file_info(im, load, exec, size);
}

void backend_defer_images( be_doc doc )
{
    rid_text_item *ti;
    rid_header *rh = doc->rh;

    if ( rh )
    {
        ti = rh->stream.text_list;

        while ( ti )
        {
	    if (object_table[ti->tag].imh != NULL)
	    {
	        image i;

	        i = (image) (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

	        image_defer( i );
	    }

            ti = rid_scanfr(ti);
        }

        if ( rh->tile.im )
            image_defer( rh->tile.im );
    }
}

void backend_stop_animation( be_doc doc )
{
    rid_text_item *ti;
    rid_header *rh = doc->rh;

    if ( rh )
    {
        ti = rh->stream.text_list;

        while ( ti )
        {
	    if (object_table[ti->tag].imh != NULL)
	    {
	        image i;

	        i = (image) (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

#ifndef BUILDERS
	        image_stop_animation( i );
#endif
	    }

            ti = rid_scanfr(ti);
        }
#ifndef BUILDERS
        if ( rh->tile.im )
            image_stop_animation( rh->tile.im );    /* scary */
#endif
    }
}

os_error *backend_doc_flush_image(be_doc doc, void *imh, int flags)
{
    rid_text_item *ti;
    os_error *ep = NULL;
    int ffi;
    rid_header *rh = doc->rh;

    ffi = (flags & be_openurl_flag_DEFER_IMAGES) ? image_find_flag_DEFER
                                                 : image_find_flag_FORCE;

    if (rh && rh->tile.im && (imh == 0 || imh == rh->tile.im))
	rh->tile.width = 0;

    if (imh)
    {
	ep = image_flush((image) imh, ffi);
    }
    else
    {
	if (rh)
	{
	    ti = rh->stream.text_list;

	    /* Free any data that we have added to the text list */
	    /* Scan through tables to catch contained images */
	    while (ti)
	    {
		if (object_table[ti->tag].imh != NULL)
		{
		    image i;

		    i = (image) (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

		    image_mark_to_flush(i, ffi);
		}

                ti = rid_scanfr(ti);
	    }

	    if (rh->tile.im)
		image_mark_to_flush(rh->tile.im, ffi);

	    ep = image_flush_marked();

	}
    }

    return ep;
}

static void be_dispose_doc_contents( be_doc doc )
{
    rid_text_item *ti;
    int i;

    BENDBG(( "doc%p: disposing of contents, url is '%s'\n", doc, doc->url ? doc->url : "<none>"));

    alarm_removeall(doc);

#ifdef STBWEB
    /* check for ondispose URL action */
    if (doc->rh->refreshtime == -2)
	be_refresh_document(0, doc);
#endif

#if DEBUG > 2
    if (doc->rh) dump_header(doc->rh);
#endif

    if (doc->ph)
    {
	doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, NULL);
	doc->ph = NULL;
    }

    if (doc->rh)
    {
	ti = doc->rh->stream.text_list;

	/* Free any data that we have added to the text list */
        /* Tables sort themselves out */
	while (ti)
	{
            rid_text_item *ti_next = rid_scanf(ti);

	    if (object_table[ti->tag].dispose != NULL)
		(object_table[ti->tag].dispose)(ti, doc->rh, doc);

	    /*ti = ti->next;*/
            ti = ti_next;
	}

	if (doc->rh->tile.im)
	{
	    image_loose((image) (doc->rh->tile.im), &antweb_doc_background_change, doc);
	}

	rid_free(doc->rh);
	doc->rh = NULL;
    }

#ifndef BUILDERS
    for ( i=0; i < WEBFONT_COUNT; i++ )
    {
        if ( GETFONTUSED( doc, i ) )
            webfont_lose_font( i );
    }
#endif
    memset( &doc->fontusage, 0, 8*sizeof(unsigned int) );

    /* SJM free spacing list */
    layout_free_spacing_list(doc);
}

static os_error *backend__dispose_doc(be_doc doc)
{
    if (doc->ah)
    {
	BENDBG(( "Calling access_abort on 0x%p\n", doc->ah));
	access_abort(doc->ah);
    }

    be_dispose_doc_contents( doc );

    if (doc->cfile)
	mm_free(doc->cfile);

    if (doc->url)
	mm_free(doc->url);

    if (doc->frag)
	mm_free(doc->frag);

    if (doc->paginate)
	awp_free_pages(doc);

    /* SJM if an imagemap is fetching then dispose of it */
    if (doc->fetching)
        backend_dispose_doc(doc->fetching);

    /* free selection list */
    mm_free(doc->selection_list.list);

    doc->magic = 0;

    mm_free(doc);

    BENDBG(( "Document disposed of\n"));

    return NULL;
}


#if DEBUG
static void dump_document_list(void)
{
#if 0
    be_doc dd;
    fprintf(stderr, "dump doc: dump list\n");
    for (dd = document_list; dd; dd = dd->next)
    {
	fprintf(stderr, "dump doc: dd %p dd->next %p\n", dd, dd->next);
    }
    fprintf(stderr, "dump doc: end\n");
#endif
}
#endif

static void antweb_unlink_doc(be_doc doc)
{
    /* remove from document list */
    if (doc == document_list)
	document_list = doc->next;
    else
    {
	be_doc dd;
	for (dd = document_list; dd; dd = dd->next)
	{
/* 	    BENDBG(( "dispose_doc: dd %p dd->next %p\n", dd, dd->next)); */
	    if (dd->next == doc)
	    {
		dd->next = doc->next;
		break;
	    }
	}
    }
}

#if 0
static BOOL antweb_doc_in_list(be_doc doc)
{
    be_doc dd;
    for (dd = document_list; dd; dd = dd->next)
	if (dd == doc)
	    return TRUE;
    return FALSE;
}
#endif

os_error *backend_dispose_doc(be_doc doc)
{
    BENDBG(( "doc%p: dispose_doc called from %s from %s\n", doc, caller(1), caller(2) ));

#if 0
    fprintf(stderr, "dispose_doc: checklist: in %p doc->next %p document_list %p\n", doc, doc->next, document_list);

    dump_document_list();
#endif

    if (doc->threaded)
    {
	doc->pending_delete++;
	doc->parent = NULL;
	return NULL;
    }
    else if (--doc->pending_delete > 0)
	return NULL;

    antweb_unlink_doc(doc);

    BENDBG(( "dispose_doc: calling real dispose doc\n"));

    return backend__dispose_doc(doc);
}

static void be_undate_image_stata(be_doc doc, void *i)
{
    image_flags f;
    int dsf, ds;

    image_data_size((image) i, &f, &dsf, &ds);

    if (f & image_flag_FETCHED)
	doc->im_fetched++;
    else if (f & image_flag_DEFERRED)
	doc->im_deferred++;
    else if (f & image_flag_WAITING)
	doc->im_unfetched++;
    else if (f & image_flag_ERROR)
	doc->im_error++;
    else
    {
	doc->im_fetching++;
	doc->im_so_far += dsf;
	if (ds == -1)
	    doc->im_in_transit = ds;
	if (doc->im_in_transit != -1)
	    doc->im_in_transit += ds;
    }
}

static void be_update_image_info(be_doc doc)
{
    void *i;
    rid_text_item *ti;
    int o_fd, o_fg, o_un, o_er, o_in, o_so, o_df;

    if (doc == NULL || doc->rh == NULL || doc->rh->stream.text_list == NULL)
	return;

    o_fd = doc->im_fetched;
    o_fg = doc->im_fetching;
    o_un = doc->im_unfetched;
    o_er = doc->im_error;
    o_df = doc->im_deferred;
    o_in = doc->im_in_transit;
    o_so = doc->im_so_far;

    doc->im_fetched = 0;
    doc->im_fetching = 0;
    doc->im_unfetched = 0;
    doc->im_error = 0;
    doc->im_deferred = 0;
    doc->im_in_transit = 0;
    doc->im_so_far = 0;

    for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if (object_table[ti->tag].imh != NULL && (i = (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE)) != NULL)
	{
	    be_undate_image_stata(doc, i);
	}
    }

    if (doc->rh->tile.im)
	be_undate_image_stata(doc, doc->rh->tile.im);

    if ((o_fd != doc->im_fetched) ||
	(o_fg != doc->im_fetching) ||
	(o_un != doc->im_unfetched) ||
	(o_er != doc->im_error) ||
	(o_df != doc->im_deferred) ||
	(o_in != doc->im_in_transit) ||
	(o_so != doc->im_so_far) )
    {
	frontend_view_status(doc->parent, sb_status_IMAGE,
			     doc->im_fetched, doc->im_fetching, doc->im_unfetched,
			     doc->im_error, doc->im_deferred, doc->im_so_far, doc->im_in_transit);
    }

    /* pdh: why was this removed? */
    if ( doc->im_fetching == 0 && doc->ah == 0 && doc->ph == 0
         && gbf_active(GBF_FVPR) )
        fvpr_progress_stream_flush( &doc->rh->stream );
}

os_error *backend_screen_changed(int flags)
{
    image_palette_change();

    return NULL;
}

static void be_view_redraw(antweb_doc *doc, wimp_box *box)
{
#if USE_MARGINS
    wimp_box bb = *box;
    bb.x0 += doc->margin.x0;
    bb.x1 += doc->margin.x0;
    bb.y0 += doc->margin.y1;
    bb.y1 += doc->margin.y1;
#endif

    /*

       Having update the display tree, update our notion of what can
       be displayed progressively (FVPR-wise). We place this here so
       that changes from image data receiving (don't know size to
       knowing size state change specifically) can correctly propogate
       their changes.

       */
/*
#ifndef BUILDERS
    fvpr_proess_stream(&doc->rh->stream);
#endif
*/
#if USE_MARGINS
    frontend_view_redraw(doc->parent, &bb);
#else
    frontend_view_redraw(doc->parent, box);
#endif
}

static void be_view_block_move(antweb_doc *doc, wimp_box *box, int x, int y)
{
#if USE_MARGINS
    wimp_box bb = *box;
    bb.x0 += doc->margin.x0;
    bb.x1 += doc->margin.x0;
    bb.y0 += doc->margin.y1;
    bb.y1 += doc->margin.y1;
    x += doc->margin.x0;
    y += doc->margin.y1;
    frontend_view_block_move(doc->parent, &bb, x, y);
#else
    frontend_view_block_move(doc->parent, box, x, y);
#endif
}


static void be_dummy_render(wimp_box *g)
{
#ifndef BUILDERS
    int junk;
    colourtran_setGCOL(config_colours[render_colour_BACK], (1<<8) | (1<<7), 0, &junk);
#endif
    bbc_move(g->x0, g->y0);
    bbc_plot(bbc_RectangleFill + bbc_DrawAbsBack, g->x1, g->y1);
}

/*
 * Return code for render background. look at bottom 2 bits only
 *   3 means no background drawn.
 *   2 unused
 *   1 means this colour background drawn
 *   0 means this is a ptr identifying this document
 * Ideally we'd use the image handle as the unique pointer but this doesn't work if
 * the bg image has a mask. This is slightly less efficient but should do.

 FIXME: DOESN'T CATER FOR TABLE CELL BACKGROUNDS.

 */

int antweb_render_background(wimp_redrawstr *rr, void *h, int update)
{
    antweb_doc *doc = (antweb_doc *) h;
    int do_fill, do_tile;
    int ox, oy;
    rid_header *rh = doc->rh;
    int rc = -1;

    if (update)
	return -1;

    do_tile = (doc->flags & doc_flag_DOC_COLOURS) &&
	(rh->bgt & rid_bgt_IMAGE) &&
	rh->tile.im &&
	(rh->tile.width != 0);

    if ( do_tile )
    {
        image_flags f;

        image_info( (image) rh->tile.im, 0,0,0, &f, 0,0 );

        if ( !(f & image_flag_RENDERABLE) )
            do_tile = FALSE;
    }

    do_fill = (doc->flags & doc_flag_NO_FILL) == 0;

    /* if rr == NULL, special code just to check the background type */
    if (rr == NULL)
    {
	if (do_tile)
	    return (int)doc;
	if (do_fill)
	    return render_set_bg_colour(render_colour_BACK, doc) | 1;

	return -1;
    }

    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

    if (do_tile)
    {
	RENDBG(("Tiling background\n"));

#ifndef BUILDERS
	image_tile((image) rh->tile.im, ox, oy, &(rr->g), render_get_colour(render_colour_BACK, doc), doc->scale_value);
#endif

	rc = (int)doc;
    }

    if (!do_tile && do_fill)
    {
	int col;
	RENDBG(("Clearing background\n"));

	col = render_set_bg_colour(render_colour_BACK, doc);
	bbc_move(rr->g.x0, rr->g.y0);
	bbc_plot(bbc_RectangleFill + bbc_DrawAbsBack, rr->g.x1, rr->g.y1);

	rc = col | 1;
    }

    if (doc->flags & doc_flag_DOC_COLOURS)
    {
	object_font_state fs;
	int oox, ooy;

	fs.lf = fs.lfc = -1;
#if USE_MARGINS
	oox = ox + doc->margin.x0;
	ooy = oy + doc->margin.y1;
#endif
        stream_render(&doc->rh->stream, doc,
		      oox, ooy,
		      rr->g.x0, rr->g.y1 - ooy, rr->g.x1, rr->g.y0 - ooy,
		      &fs, &rr->g, object_redraw_BACKGROUND);
    }

    return rc;
}

/*--- Individual event routines for the window ---*/

int backend_render_rectangle(wimp_redrawstr *rr, void *h, int update)
{
    antweb_doc *doc = (antweb_doc *) h;
/*    rid_pos_item *pi;*/
/*    rid_text_item *ti;*/
    int ox, oy;
    int top, bot;
    int left, right;
    object_font_state fs;
    rid_header *rh;

    if (doc == NULL || doc->rh == NULL)	/* If the stream is empty it gets caught later */
    {
	be_dummy_render(&(rr->g));
	return 0;
    }

    if (doc->magic != ANTWEB_DOC_MAGIC)
	return 0;

    rh = doc->rh;

    /* ox and oy are the screen coordinates of the work area origin */
    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
    RENDBG(("backend_render_rectangle: doc%p ox=%d, oy=%d, encoding=%d, margins %d,%d\n", doc, ox, oy, doc->encoding_user, doc->margin.x0, doc->margin.y1));
#else
    RENDBG(("backend_render_rectangle: doc%p ox=%d, oy=%d, encoding=%d, margins %d,%d\n", doc, ox, oy, doc->encoding_user, 0, 0));
#endif

    if (rh)
    {
	top = rr->g.y1 - oy;
	bot = rr->g.y0 - oy;

	RENDBG(("Top work area = %d, bottom = %d, stream %p list %p\n", top, bot, &doc->rh->stream, doc->rh->stream.pos_list));

	left = rr->g.x0;
	right = rr->g.x1;

	antweb_render_background(rr, h, update);

	fs.lf = fs.lfc = -1;

        /* Now into the recursive stream rendering function */
        stream_render(&doc->rh->stream, doc,
		      ox, oy,
		      left, top, right, bot,
		      &fs, &rr->g, update);


#ifdef RISCOS
	/* render frame borders */
 	layout_render_bevels(rr, doc);

	/* make another pass through the document to draw the
	 * highlights
	 */
	highlight_render(rr, doc);
#endif
    }

    RENDBG(("Render done\n"));

    return 0;
}


/*
 * This function allows a request to update part of an item. The trim box is the margin of the box
 * that shouldn't be touched.
 * wont_plot_all is TRUE if when this item is plotted it doesn't automatically redraw the background
 * eg text and radio items are true
 * textarea or checkboxes are false
 * this allows the anti-twittering redraw to stop multiple filtering occurring.
 */

void antweb_update_item_trim(antweb_doc *doc, rid_text_item *ti,
			     wimp_box *trim, BOOL wont_plot_all)
{
    wimp_box box;
    os_error *e;

    if ((doc->flags & doc_flag_DISPLAYING) == 0
        || (doc->parent == NULL) )
        return;

    e = backend_doc_item_bbox(doc, ti, &box);
#if 0
    DBG(("backend_doc_item_bbox returns %x %s %d,%d %d,%d\n", e ? e->errnum : 0, e ? e->errmess : "", box.x0,  box.y0,  box.x1,  box.y1));
#endif
    if (e == NULL)
    {
        if (trim)
        {
            box.x0 += trim->x0;
            box.y1 += trim->y1;
            box.x1 += trim->x1;
            box.y0 += trim->y0;
        }

        frontend_view_update(doc->parent, &box, &backend_render_rectangle, doc, wont_plot_all);
    }
}

void antweb_update_item(antweb_doc *doc, rid_text_item *ti)
{
    antweb_update_item_trim(doc, ti, NULL, TRUE);
}

/*****************************************************************************/

static void be_tfetch_stream(antweb_doc *doc, rid_text_item *ti)
{
    for (;ti != NULL; ti = rid_scanf(ti))
    {
	/* Not ideal, but *encompasses* desired behaviour */
	switch (ti->tag)
	{
	case rid_tag_IMAGE:
	case rid_tag_INPUT:
	case rid_tag_OBJECT:
	    (object_table[ti->tag].size)(ti, doc->rh, doc);
	    break;

	case rid_tag_TABLE:
	{
	    int x,y;
	    rid_table_cell *cell;
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;

	    for (x=-1,y=0; (cell = rid_next_root_cell(table, &x,&y)) != NULL; )
		be_tfetch_stream(doc, cell->stream.text_list);
	}
	break;
	}
    }
}

extern os_error *antweb_trigger_fetching(antweb_doc *doc)
{
    rid_text_item *ti;

    FMTDBG(("antweb_trigger_fetching: start\n"));

    /* Pass one: size everything up */

    doc->im_error = doc->im_unfetched = doc->im_fetched = doc->im_fetching = 0;

#if 1
    /* DAF: See startbody() for where this now happens */
    /* pdh: Doesn't entirely actually completely belong here, but we want
     * the background image to be fetched first, not last */
    if ((doc->rh->bgt & rid_bgt_IMAGE) && (doc->rh->tile.im == NULL))
    {
        BENDBG(( "Calling fetch_bg from document_sizeitems\n" ));
        be_doc_fetch_bg(doc);
    }
#endif

    if (gbf_active(GBF_NEW_FORMATTER))
    {
	be_tfetch_stream(doc, doc->rh->stream.text_list);
    }
    else
    {
	ti = doc->rh->stream.text_list;

	/* First do each individual item */
	while (ti)
	{   /* Tables will recurse on child objects */
	    (object_table[ti->tag].size)(ti, doc->rh, doc);

	    /* might be no pos list, so no scanfr() */
	    /*ti = ti->next;*/
	    ti = rid_scanf(ti);
	}
    }

    FMTDBG(("antweb_trigger_fetching done\n"));
    FMTDBG(("Images: %d waiting, %d fetching, %d fetched, %d errors.\n",
	    doc->im_unfetched, doc->im_fetching, doc->im_fetched, doc->im_error));

    return NULL;
}

static void be_formater_loop(antweb_doc *doc, rid_header *rh, rid_text_item *ti, int scale_value)
{
        rid_text_stream *st = &rh->stream;
        rid_fmt_info fmt;

        memset(&fmt, 0, sizeof(fmt));
        memset(&st->width_info, 0, sizeof(st->width_info));

        FMTDBG(("be_formater_loop - building\n"));

/*     fvpr_progress_stream(&doc->rh->stream); */

	{
	    fe_view_dimensions fvd;
	    frontend_view_get_dimensions(doc->parent, &fvd);
	    rid_toplevel_format(doc, rh, NULL, rh->stream.fwidth, fvd.layout_height);
	}

	objects_check_movement(doc);
#ifndef BUILDERS
	antweb_build_selection_list(doc);
#endif

        FMTDBG(("be_formater_loop done\n"));

#if DEBUG > 2
	fprintf(stderr, "\n\nrid_header after more formatting\n\n");
        dump_header(rh);
#endif
}

static os_error *antweb_document_format_no_fvpr(antweb_doc *doc, int user_width)
{
    rid_text_item *ti;

    FMTDBG(("antweb_document_format() entered\n"));

    BENDBG(( "Document title: '%s' \ndoc %p, rid_header %p\n",
	    doc->rh->title ? doc->rh->title : "<none>", doc, doc->rh));

    /* Include tables in free of pos items */
    rid_free_pos_tree(doc->rh->stream.pos_list);
    doc->rh->stream.pos_list = doc->rh->stream.pos_last = NULL;

    ti = doc->rh->stream.text_list;
    if (ti == NULL)
	return NULL;

    /* Zero w|h for all table descendent streams as well */
    rid_zero_widest_height(&doc->rh->stream);

    be_formater_loop(doc, doc->rh, ti, doc->scale_value);

/*     fvpr_progress_stream(&doc->rh->stream); */

#if DEBUG && 0
    FMTDBG(("antweb_document_format() done doc %p, rid_header %p\n", doc, doc->rh));

    if (doc->rh->tile.src)
    {
	fprintf(stderr, "src %p '%s', im %p, w %d, h %d, bgt %d\n",
		doc->rh->tile.src, doc->rh->tile.src,
		doc->rh->tile.im, doc->rh->tile.width, doc->rh->tile.height, doc->rh->bgt);
    }

    fprintf(stderr, "back %d, fore %d, link %d, vlink %d, alink %d\n",
	    doc->rh->colours.back,
	    doc->rh->colours.fore,
	    doc->rh->colours.link,
	    doc->rh->colours.vlink,
	    doc->rh->colours.alink);

    fprintf(stderr, "margin left=%d, top=%d\n", doc->rh->margin.left, doc->rh->margin.top);
#endif

    DBG(("antweb_document_format: format finished minwidth %d fwidth %d scale %d\n", doc->rh->stream.width_info.minwidth, doc->rh->stream.fwidth, doc->scale_value));;

    return NULL;
}

os_error *antweb_document_format(antweb_doc *doc, int user_width)
{
    antweb_document_format_no_fvpr( doc, user_width );
    /* pdh: always returns NULL (sigh) */
    if ( gbf_active(GBF_FVPR) )
        fvpr_progress_stream( &doc->rh->stream );
    return NULL;
}


#ifdef BUILDERS
extern
#else
static
#endif
void be_document_reformat_tail(antweb_doc *doc, rid_text_item *oti, int user_width)
{
    int height;
    rid_text_item *ti = NULL;
    rid_pos_item *new;
    wimp_box bb;

    FMTDBG(("be_document_reformat_tail(%p,%p,%d)\n", doc, oti,user_width));
    if ( doc )
        FMTDBG(("be_document_reformat_tail: doc->rh=%p\n", doc->rh ));

    if (doc->rh->flags & rid_hf_FULL_REFORMAT)
    {
	oti = NULL;
	FMTDBG(("\n\nWARNING - FORCING FULL REFORMAT\n\n"));
    }

    bb.x0 = 0;

    if (oti)
    {
#if 1
	if (oti->tag == rid_tag_TABLE)
	    ti = oti;
	else if (!oti->line)
	    ti = NULL;
	else
#endif
            if ( oti->line )
                ti = oti->line->first;
    }

    if ( !ti )
	ti = doc->rh->stream.text_list;

    if ( !ti )
	return;

    FMTDBG(("Line start item is %p\n", ti));

    new = ti->line;

    FMTDBG(("Line pos item 'new' is %p\n", new));

    /* SJM: this seems to happen, I don't know why */
    if (new == NULL)
	return;

    /* Zero all existing widest and height values in nested items */
    rid_zero_widest_height_from_item(ti);

    /* Chop off the old list before the last non-empty entry. */
    if (new->prev)
    {
	/* This can be a bit nasty.  We don't want rid_free_pos_tree to free floats kept above */
	if (new->prev->floats)
	{
	    rid_pos_item *pi;
	    rid_float_item *fl, *fr;

	    fl = new->prev->floats->left;
	    fr = new->prev->floats->right;

	    if (fl || fr)
	    {
		for (pi = new; (pi &&
				pi->floats &&
				(pi->floats->left == fl ||
				 pi->floats->right == fr ) ); pi=pi->next)
		{
		    if (fl && pi->floats->left == fl)
			pi->floats->left = NULL;
		    if (fr && pi->floats->right == fr)
			pi->floats->right = NULL;
		}
	    }

	}

	new->prev->next = NULL;
    }
    else
	doc->rh->stream.pos_list = NULL;

    FMTDBG(("Set the back pointer\n"));

    doc->rh->stream.pos_last = new->prev;

    FMTDBG(("Pos list chopped\n"));

    /* The starting height is top of the old last line */

    doc->rh->stream.height = height = new->top;
    bb.y1 = height;

    /* Include tables in freeing of pos items */
    rid_free_pos_tree(new);

    FMTDBG(("Calling the formatter loop\n"));

    be_formater_loop(doc, doc->rh, ti, doc->scale_value);

    if ( gbf_active(GBF_FVPR) )
        fvpr_progress_stream(&doc->rh->stream);

    FMTDBG(("be_formater_loop() done\n"));

#ifndef BUILDERS
    antweb_build_selection_list(doc);
#endif

    bb.x1 = doc->rh->stream.widest > user_width ? doc->rh->stream.widest : user_width;
    bb.y0 = doc->rh->stream.height;

    if (doc->parent && (doc->flags & doc_flag_DISPLAYING))
    {
        DICDBG(("docrt: redrawing view from %d to %d\n", bb.y0, bb.y1 ));
	be_view_redraw(doc, &bb);
    }
}

/*
 * This replacement for frontend_view_set_dimensions checks the return value. If set this means
 * that the horizontal width may have changed and should be reread and the document reformatted.
 */

static void be_set_dimensions(be_doc doc)
{
    int w, h;
    w = doc->rh->stream.widest;
    h = doc->rh->stream.height;

    BENDBG(( "be_set_dimensions: doc%p to %dx%d\n", doc, w, h));

#if !defined(STBWEB) && !defined(BUILDERS)
    /* pdh: Fresco wants this, don't know whether NCFresco does */
    /* SJM: Neither should use it as it wipes out any changes made to the margins by BODY attributes */
    frontend_view_margins( doc->parent, &doc->margin );
#endif

#if USE_MARGINS
    w += doc->margin.x0 - doc->margin.x1;
    h -= doc->margin.y0 - doc->margin.y1;
#endif

    while (frontend_view_set_dimensions(doc->parent, w, h))
    {
        fe_view_dimensions fvd;
        frontend_view_get_dimensions(doc->parent, &fvd);

        doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
        doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif
	ASSERT(doc->rh->stream.fwidth > 0);

	doc->scale_value = 100;
        antweb_document_format(doc, doc->rh->stream.fwidth);

/* 	objects_check_movement(doc); moved to inside antweb_document_format() */

	/* update w and h after reformatting */
	w = doc->rh->stream.widest;
	h = doc->rh->stream.height;

#if USE_MARGINS
	w += doc->margin.x0 - doc->margin.x1;
	h -= doc->margin.y0 - doc->margin.y1;
#endif
    }
}

os_error *backend_reset_width(be_doc doc, int width)
{
    fe_view_dimensions fvd;

    FMTDBG(("backend_reset_width(%p, %d)\n", doc, width));

    if (doc->rh->frames)
    {
        /* always relay if this is called */
	frontend_view_get_dimensions(doc->parent, &fvd);

        layout_layout(doc, fvd.layout_width, fvd.layout_height, 1, NULL, 0);

        doc->rh->stream.widest = fvd.wa_width;
        doc->rh->stream.height = fvd.wa_height;
#if 0
        doc->rh->stream.widest -= doc->margin.x0 - doc->margin.x1;
        doc->rh->stream.height -= doc->margin.y0 - doc->margin.y1;
#endif

#ifndef STBWEB
        be_set_dimensions(doc);
#endif
    }
    else
    {
        int old_user_width = doc->rh->stream.fwidth;
        frontend_view_get_dimensions(doc->parent, &fvd);

#if USE_MARGINS
	fvd.user_width -= doc->margin.x0 - doc->margin.x1;
#endif

        /* only reformat if the width has actually changed */
        if (old_user_width != fvd.user_width)
        {
            doc->rh->stream.fwidth = fvd.user_width;
	    ASSERT(doc->rh->stream.fwidth > 0);

	    doc->scale_value = 100;
            antweb_document_format(doc, doc->rh->stream.fwidth);

            be_set_dimensions(doc);
        }
    }

    antweb_default_caret(doc, FALSE);

    return NULL;
}

os_error *backend_goto_fragment(be_doc doc, char *frag)
{
    rid_aref_item *ai;

    if (doc == NULL || doc->rh == NULL)
	return makeerror(ERR_BAD_DOCUMENT);

    BENDBG(( "Going to fragment '%s'\n", frag ? frag : "<none>"));
    if (frag)
    {
        rid_text_item *first;
	wimp_box bb;

	ai = doc->rh->aref_list;

	while (ai)
	{
	    if (ai->name && strcasecomp(ai->name, frag) == 0)
		break;
	    ai = ai->next;
	}

#ifndef STBWEB			/* for NCFresco if it is unknown then use the top of the page */
	if (ai == NULL)
	    return doc->ah || doc->ph ? NULL : makeerror(ERR_NO_SUCH_FRAG); /* only give error if document fully downloaded */
#endif

        first = ai && ai->first ? ai->first : NULL;
	bb.y1 = 0;
	if (first)
	    backend_doc_item_bbox(doc, first, &bb);

	BENDBG(( "backend_goto_fragment: ensuring item %p (aref %p) visable, top at %d\n", first, ai, bb.y1));

	/* doesn't need margin adjustment, done in doc_item_bbox */
	frontend_view_ensure_visable(doc->parent, 0, bb.y1, bb.y1);
    }

    return NULL;
}

static void be_update_image_size(antweb_doc *doc, void *i)
{
    rid_text_item *ti;

    for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if ((object_table[ti->tag].imh != NULL) &&
	    ((object_table[ti->tag].imh)(ti, doc, object_image_OBJECT) == i) )
	{
            assert(ti->tag !=rid_tag_TABLE);
	    (object_table[ti->tag].size)(ti, doc->rh, doc);
	}
    }
}

/* This function gets called as a callback when an image changes */

/* When an image changes it can be a) above the visable area, b)
 * intersecting with the visable area or c) below the visable area.
 * If it is below we can pretty much ignore the image; the reformat is
 * needed but we do not need any redraw.  If the image intersects with
 * the visable area the a redraw is needed at least for the image and
 * maybe for the rest of tyhe paragraph.  If the image is above the
 * visable area the we need to find out if it has caused any
 * reformatting.  We note the first visable object before and after
 * the reformat.  If the first visable object is still at the
 * beginning of a line after the reformat and no changes have fallen
 * within the old visable area then we can be sure that the format of
 * what was visable has not changed and we can try to get away with no
 * redrawing.  For good measure if we have not forced a whole-sale
 * redraw then we should update the images on the screen.  Maybe we
 * should even force a redraw since the new image may be trasparent.
 */
/*
    When an image is within a table (a table cell or a table caption) and
    it's size changes, then the redraw rectangle needs growing to enclose the
    table the image is contained within.  For reliable results, grow outwards
    until all parent tables have been included.  For more efficient results,
    when considering a table, resize and reformat it and then only include it
    if the old and new sizes are different.  If the sizes are different,
    likewise consider the next enclosing table, etc.  There's probably better
    still.

    This comes out pretty messy with tables.  Volunteers to rewrite it
    with better optimisations welcomed! Finding a better algorithm
    than cloning the pos list would be excellent!

    To avoid having to think too much, if the image is contained
    anywhere within a table, we try to pretend the outermost table is what
    has changed.

    We now take a clone of the pos list and free it afterwards. This
    is more work but it should work correctly. Remember the free must
    be rid_free_pos() and not rid_free_pos_tree(), as we don't wish to
    disturb any child pos lists.  When we perform the scan locating
    height, to obtain a pos pointer to remember, this must be from the
    cloned pos list, as the existing pos list is about to be freed by
    the format operation.

*/
/* pdh: F/X: sacrifices goat before attempting to debug this function
 */
/* SJM: ihandle is only used to compare against the value of the image
   handle read via the object method. */

static void antweb_doc_image_change2( void *h, void *ihandle, int status,
                                      wimp_box *box_update )
{
    antweb_doc *doc = (antweb_doc *) h;
    int changed = FALSE;
    rid_pos_item *pi;
    rid_pos_item *clonedposlist = NULL;
    rid_text_item *first_ti;
    rid_text_item *ti;
    int top, bottom, line_top = 0;
    wimp_box bounds;

#if DEBUG
    if (status != image_cb_status_WORLD && status != image_cb_status_NOACTION)
    {
	DICDBG(("Image change called, doc=0x%p, status %d, image 0x%p\n", doc, status, ihandle));
    }
#endif

    switch (status)
    {
    case image_cb_status_WORLD:
    case image_cb_status_NOACTION:
	frontend_view_status(doc->parent, sb_status_WORLD);
	return;

    case image_cb_status_CACHE:
	be_update_image_info(doc);
	return;

    case image_cb_status_UPDATE_ANIM:
	break;

    default:
	/* SJM: moved to end of function as otherwise when last image
           comes in the fvpr flush happens before the fvpr checking in
           the function below and the screen isn't updated */
/* 	be_update_image_info(doc); */
	break;
    }

    if ( ((doc->flags & doc_flag_DISPLAYING) == 0)
          || doc->parent == NULL )
    {
	be_update_image_size(doc, ihandle);

	/* SJM: moved from top of function */
	if (status != image_cb_status_UPDATE_ANIM)
	    be_update_image_info(doc);
	return;
    }

    frontend_view_bounds(doc->parent, &bounds);

    top = bounds.y1;
    bottom = bounds.y0;

    if (doc->rh->stream.pos_list == NULL)	/* If the image is on disc then we can be called before the first format */
    {
	DICDBG(("Image change quit early\n"));

	/* SJM: moved from top of function */
	if (status != image_cb_status_UPDATE_ANIM)
	    be_update_image_info(doc);
	return;
    }

    if (status == image_cb_status_REFORMAT)
    {
	clonedposlist = rid_clone_pos_list(doc->rh->stream.pos_list);

/* 	for (pi = clonedposlist; pi->next && pi->next->top > top; pi = pi->next) */
/* 	    ; */
	/* SJM: the above is not good enough with floating images around */
	pi = clonedposlist;

#if 1
	/* SJM: 17Jul97: went back to alowing first_ti to be NULL
           (code below checks for this case) otherwise strange results
           can occur with single floating tables */
        first_ti = pi->first;
   	line_top = pi->top;
#else
        /* pdh: if all our items are floating, pi->first may never get set.
         * In this case we use the first item in the text list... if this is
         * null too there can't be any images and we won't be here.
         */
        first_ti = pi->first ? pi->first : doc->rh->stream.text_list;

	/* SJM: 17Jul97: set line_top based on position from first ti
           otherwise we get spurious large shifts down below */
  	line_top = first_ti->line ? first_ti->line->top : 0;
/*   	line_top = pi->top; */
#endif

	ti = doc->rh->stream.text_list;

	DICDBG(("Starting rescan: pi %p pi->first %p first_ti %p first_ti->line %p ti %p ti tag %d line_top %d\n",
		pi, pi->first, first_ti, first_ti ? first_ti->line : 0, ti, ti->tag, line_top));

	/* Need to scan recursively to actually find the images, but use */
	/* the outermost item when looking for redraw sizes, etc. */
	/* Pretty gruesome really. Go for region covered by table */
	/* and image, just in case. */

	while (ti)
	{
	    if (object_table[ti->tag].imh != NULL && (object_table[ti->tag].imh)(ti, doc, object_image_OBJECT) == ihandle)
	    {
		int ow, omu, omd;
		BOOL fvpr = FALSE;

		ow = ti->width;
		omu = ti->max_up;
		omd = ti->max_down;

		(object_table[ti->tag].size)(ti, doc->rh, doc);

		/* pdh: Test for same size not good enough, as an image may
		 * go from being *maybe* 68x68 to *definitely* 68x68 and thus
		 * will not have been fvpr'd. We must test for not being
		 * fvpr'd as well.
		 *     Good job the final image on gi/~dean *is* 68x68 or I'd
		 * not have spotted this!
		 */
		if ( gbf_active( GBF_FVPR )
		     && !(ti->flag & rid_flag_FVPR) )
		    fvpr = TRUE;

		if ( ( ow != ti->width || omu != ti->max_up
		       || omd != ti->max_down )
		     || fvpr )
		{
		    rid_text_item *outeritem = rid_outermost_item(ti);

		    if (ti->line->top < bottom) /* top of line below the bottom of view */
			changed |= (1 << 2); /* Set bit for below */
		    else if ( (ti->line->top - (ti->line->max_up + ti->line->max_down)) > top) /* Bottom of line above top of view */
			changed |= (1 << 1); /* Set bit for above */
		    else
			changed |= (1 << 0); /* Set bit for in view */

		    if (outeritem->line->top < bottom) /* top of line below the bottom of view */
			changed |= (1 << 2); /* Set bit for below */
		    else if ( (outeritem->line->top - (outeritem->line->max_up + outeritem->line->max_down)) > top) /* Bottom of line above top of view */
			changed |= (1 << 1); /* Set bit for above */
		    else
			changed |= (1 << 0); /* Set bit for in view */

                    DICDBG(("im%p: has changed (%d-%d %d-%d %d-%d %s)\n",
                            ihandle, ow, ti->width, omu, ti->max_up,
                            omd, ti->max_down, fvpr ? "yes" : "no" ));
		}
	    }

	    /* Needs pos list, hence moved above yucky nulling */
	    ti = rid_scanfr(ti) /*ti->next*/ ;
	}
    }
    else
    {
	/* This value of pi MUST NOT be used inside the changed code... */
/*  	for (pi = doc->rh->stream.pos_list; pi->next && pi->next->top > top; pi = pi->next) */
/* 	    ; */
	/* SJM: the above is not good enough with floating images */
	pi = doc->rh->stream.pos_list;

	/* This is the first displayed item in the view */
 	first_ti = pi->first;
	line_top = pi->top;

	/* ... but we set changed=0 os it's OK */
	changed = 0;
    }

    if (changed)
    {
	int old_height;
	int net_shift, shift_pending;
	int top_of_zone;
	int is_shift_pending = FALSE;
	rid_pos_item *opi;
	int new_bottom;
	wimp_box box;
	int fvprstatus = 0;   /* 1 = fvpr has changed 2 = fvpr changed above */

	/* If we have the caret, hide it for the moment */
	if (frontend_view_has_caret(doc->parent))
	{
	    frontend_view_caret(doc->parent, 0, 0, -1, 0);
	}

	box = bounds;		/* We want x0 and x1 at the least */

	DICDBG(("Changed = %d\n", changed));

	old_height = doc->rh->stream.height;

	DICDBG(("About to reformat\n"));

	antweb_document_format_no_fvpr(doc, doc->rh->stream.fwidth);

        /* pdh: added this */
        if ( gbf_active(GBF_FVPR)
             && fvpr_progress_stream( &doc->rh->stream ) )
        {
            changed = 7;
            fvprstatus = 1;
            DICDBG(("Fvpr has changed\n"));
        }

	DICDBG(("Reformat done\n"));

	if (doc->rh->stream.height != old_height)
	{
	    DICDBG(("Old height was %d, new height is %d\n",
		    doc->rh->stream.height, old_height));

	    be_set_dimensions(doc);
	}

	DICDBG(("Checking what has changed (first_ti=%p first_ti->line=%p) top %d\n",
	        first_ti, first_ti ? first_ti->line : NULL, first_ti && first_ti->line ? first_ti->line->top : 0 ));

	if (changed & 3)
	{
	    net_shift = 0;
	    if (first_ti && first_ti->line)
	    {
		shift_pending = first_ti->line->top - line_top;
		top_of_zone = first_ti->line->top; /* Always in new format coordinates */

		is_shift_pending = TRUE;
	    }
	    else
	    {
		/* SJM: 19Jul97 if we have no first item then there
                   are only floaters on the page (or is that just the
                   first line?) so we do no shifts and rely on the
                   whole area being redrawn */
		shift_pending = 0;
		top_of_zone = line_top;

		is_shift_pending = FALSE;
	    }

	    if (shift_pending != 0)
	    {
		DICDBG(("Shift = %d\n", shift_pending));
#if USE_MARGINS
		frontend_view_ensure_visable(doc->parent, -1, top + shift_pending + doc->margin.y1, bottom + shift_pending + doc->margin.y1);
#else
		frontend_view_ensure_visable(doc->parent, -1, top + shift_pending, bottom + shift_pending);
#endif
	    }
	    new_bottom = bottom + shift_pending;

	    DICDBG(("Scroll shift = %d\n", shift_pending));

	    /* For each line in the old view: */
	    /* NB: This pi can only come from the cloned list because if it was from the
	       original list the changed flag would have to be zero */
	    for (opi = pi; opi && (opi->top > bottom); opi = opi->next)
	    {
		int reuse = FALSE;

		DICDBG(("Looking at line that was at %d\n", opi->top));

		/* If there is nothing on the line or the new version of the line is out of view */
		if ((opi->first == NULL) || (opi->first->line->top < new_bottom))
		{
		    DICDBG(("breaking out of loop opi->first %p\n", opi->first));
		    break;
		}

		if ((opi->next == NULL) ||
		    ((opi->first->line->next != NULL) &&
		     (opi->first->line->first == opi->first) &&
		     (opi->next->first == opi->first->line->next->first)))
		{
		    rid_text_item *tiscan;
		    int have_image = FALSE;

                    /* Prevent reuse if the image occurs in the outermost line being examined, */
                    /* a table which is at the outermost level contains the image or the image */
		    /* floating on this line */

		    /* It could be worse, but not much... */
		    if ((opi->first->line->floats != 0) &&
			((opi->first->line->floats->left != 0) &&
			 ((tiscan = opi->first->line->floats->left->ti) != 0) &&
			 (tiscan->tag == rid_tag_TABLE ?
			  rid_table_holds_image(tiscan, ihandle, doc) :
			  ((object_table[tiscan->tag].imh != NULL) &&
			   (object_table[tiscan->tag].imh)(tiscan, doc, object_image_OBJECT) == ihandle)) ||
			 (opi->first->line->floats->right != 0) &&
			 ((tiscan = opi->first->line->floats->right->ti) != 0) &&
			 (tiscan->tag == rid_tag_TABLE ?
			  rid_table_holds_image(tiscan, ihandle, doc) :
			  ((object_table[tiscan->tag].imh != NULL) &&
			   (object_table[tiscan->tag].imh)(tiscan, doc, object_image_OBJECT) == ihandle) ) ) )
		    {
			have_image = TRUE;
		    }
		    else
		    {
			for(tiscan = opi->first;
			    tiscan && (tiscan->line != opi->first->line->next);
			    tiscan = rid_scanf(tiscan) /*tiscan->next*/ )
			{

			    DICDBGN(("<"));

			    if (tiscan->tag == rid_tag_TABLE)
			    {
                                if (rid_table_holds_image(tiscan, ihandle, doc))
                                {
				    have_image = TRUE;
				    break;
                                }
			    }
			    else if (object_table[tiscan->tag].imh != NULL && (object_table[tiscan->tag].imh)(tiscan, doc, object_image_OBJECT) == ihandle)
			    {
				have_image = TRUE;
				break;
			    }

			    DICDBGN((">"));

			}
		    }

		    if (!have_image)
			reuse = TRUE;
		}

		DICDBG(("Line reuse = %d\n", reuse));

		if ( fvprstatus == 1 && !reuse )
		{
		    /* We must give up, we've not rendered any lines below here
		     */
		    DICDBG(("We've not rendered any lines below here\n"));
		    is_shift_pending = FALSE;
		    break;
		}

		/* 	if the line is reusable: */
		if (reuse)
		{
		    /* 	if no shift pending: */
		    if (!is_shift_pending)
		    {
			DICDBG(("Reuse line with no shift pending\n"));

			/*	redraw from top of dirty zone to top of this line */
			box.y1 = top_of_zone;
			box.y0 = opi->first->line->top;

                        /* pdh: do we want this? */
			DICDBG(("Force redraw %d..%d\n", box.y1, box.y0));

			be_view_redraw(doc, &box);
			/* */
			/*	set shift pending to shift needed for this line minus net shift so far */
			top_of_zone = opi->first->line->top;
			shift_pending = (opi->first->line->top - opi->top) - net_shift;
			is_shift_pending = TRUE;

			DICDBG(("Set shift of %d pending from %d down\n", shift_pending, top_of_zone));

		    }
		    /* pdh: FIXME: commenting this back in makes it redraw
		     * less, but some pages, e.g. ant.ant.co.uk, don't fully
		     * appear. Sacrificed wrong sort of goat I expect.
		     */
		    else
		    {
		        if ( shift_pending == 0 && net_shift == 0 )
		            top_of_zone = opi->first->line->top;
		    }
		}
		/* 	if the line is not reusable: */
		else
		{
		    /* 	if shift pending: */
		    if (is_shift_pending)
		    {
			DICDBG(("Don't reuse line with a shift pending\n"));

			if (shift_pending) /* Only work the effort is the shift is non-zero */
			{
			    /* shift from top of shift to above this line by shift pending */
			    box.y1 = top_of_zone - shift_pending;
			    box.y0 = new_bottom - shift_pending;

			    DICDBG(("Non-zero shift, y1 = %d, y0 = %d, new y0 = %d\n",
				    box.y1, box.y0, box.y0 + shift_pending));

			    be_view_block_move(doc, &box, box.x0, box.y0 + shift_pending );
			    /* add shift pending to net shift so far */
			    net_shift += shift_pending;

			    DICDBG(("Net shift now %d\n", net_shift));
			}
			/* 		make top of this line as to of dirty zone */
			top_of_zone = opi->first->line->top;
			is_shift_pending = FALSE;

			DICDBG(("Top of dirty zone = %d\n", top_of_zone));
		    }
		}
	    }

	    DICDBG(("Finished scanning viable range is_shift_pending %d shift_pending %d\n", is_shift_pending, shift_pending));

	    if (is_shift_pending)
	    {
		if (shift_pending)
		{
		    /* 	shift from top of shift to bottom of view by shift pending */
		    box.y1 = top_of_zone - shift_pending;
		    box.y0 = new_bottom - shift_pending;

		    DICDBG(("Non-zero shift (left over), y1 = %d, y0 = %d, new y0 = %d\n",
			    box.y1, box.y0, box.y0 + shift_pending));

		    be_view_block_move(doc, &box, box.x0, box.y0 + shift_pending );
		}
	    }
	    else
	    {
		/* 	redraw from top of dirty to bottom of view */
		box.y1 = top_of_zone;
		box.y0 = new_bottom;

		DICDBG(("Force redraw from %d to %d\n", box.y1, box.y0));

		be_view_redraw(doc, &box);
	    }
	}

	antweb_default_caret(doc, FALSE);
    }
    else
    {
	int flags;
	int do_redraw;
/* 	int real_thing_flag; */
	int imw, imh;

	/* SJM: changes to use frame_info to get right mask value */
#if 0
	wimp_box bbox;
	image_info_frame((image)ihandle, &bbox, NULL, &flags);
#else
	image_info((image)ihandle, &imw, &imh, NULL, &flags, NULL, NULL);
#endif

/* 	real_thing_flag = (flags & image_flag_REALTHING) ? rid_image_flag_REAL : 0; */

	/* If told to UPDATE when we don't need a full redraw */
	do_redraw = (status == image_cb_status_UPDATE || status == image_cb_status_UPDATE_ANIM) ? 0 : 1;

	DICDBG(("Updating images with no size change.  Top=%d, borrom=%d, redraw = %d, first_ti %p\n",
		top, bottom, do_redraw, first_ti));

	/* The new image must be the same size.  Redraw the image when on the screen. 	*/
        /* This recurses through tables as well - should work okay */
/* 	for (ti = first_ti; ti && (ti->line->top > bottom); ti = rid_scanfr(ti)) */
	for (ti = first_ti ? first_ti : doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
	{
	    void *imhandle = object_table[ti->tag].imh != NULL ? (object_table[ti->tag].imh)(ti, doc, object_image_OBJECT) : NULL;

	    DICDBGN(("ti=%p, line=%p, top=%d, imh=0x%p\n", ti, ti->line, ti->line->top, imhandle));

	    if (imhandle == ihandle)
	    {
		wimp_box *tbox = object_table[ti->tag].imh(ti, doc, object_image_BOX);

		/* Removed the tii reference as it may not be an IMG !!! */
/* 		rid_text_item_image* tii = (rid_text_item_image*) ti; */
/* 		int hb = tii->hspace*2 + tii->bwidth*2; */
/* 		int vb = tii->vspace*2 + tii->bwidth*2; */

		if (do_redraw)
		{
		    wimp_box bb;
		    int x, y;
		    stream_find_item_location(ti, &x, &y);

		    DICDBG(("Stream coords %d,%d\n", x, y));

		    bb.x0 = x;
		    bb.x1 = x + ti->width;
		    bb.y0 = y - ti->max_down;
		    bb.y1 = y + ti->max_up;

		    be_view_redraw(doc, &bb);

/* 		    if (real_thing_flag) */
/* 			tii->flags |= rid_image_flag_REAL; */
/* 		    else */
/* 			tii->flags &= ~rid_image_flag_REAL; */
		}
		else
		{
		    /* We need all this but we cant get the hb/vb values from here */
                    wimp_box trim = *tbox;

		    DICDBG(("trim box %d,%d %d,%d\n", trim.x0, trim.y0, trim.x1, trim.y1));

		    /* SJM: masked images wont redraw all the screen
		     * and we don't want to redraw the border
		     * and we have animation frames to take into account
		     */

		    if (box_update)
		    {
			int xm = ti->width - (tbox->x0 - tbox->x1),
			    xd = imw,
			    ym = ti->max_up + ti->max_down - (tbox->y0 - tbox->y1),
			    yd = imh;

			DICDBG(("upda box %d,%d %d,%d\n", box_update->x0, box_update->y0, box_update->x1, box_update->y1));

			/* this box is in os coords relative to the original image size and needs to be
			 * scaled appropriately
			 */

			trim.x0 += box_update->x0*xm/xd;
			trim.x1  = box_update->x1*xm/xd - ti->width - tbox->x1;
			trim.y0 += box_update->y0*ym/yd;
			trim.y1  = box_update->y1*ym/yd - (ti->max_up + ti->max_down) - tbox->y1;

/* 			DICDBGN(("animation: box update %d,%d %d,%d trim %d,%d %d,%d width %d up/down %d,%d hb %d vb %d\n", */
/* 				box_update->x0, box_update->y0, box_update->x1, box_update->y1, */
/* 				trim.x0, trim.y0, trim.x1, trim.y1, */
/* 				ti->width, ti->max_up, ti->max_down, hb, vb)); */
		    }

		    antweb_update_item_trim(doc, ti, &trim,
		        (flags & image_flag_FETCHED ? 0 : fe_update_IMAGE_RENDERING) +
		        (flags & ((image_flag_MASK | image_flag_ANIMATION) == image_flag_MASK) ? fe_update_WONT_PLOT_ALL : 0) );
		    /* SJM: FIXME: depending on animation flag is a bad idea here */
		}
	    }
	}

	DICDBG(("Images updated\n"));
    }

    /* NOT rid_free_pos_tree() though */
    if (clonedposlist)
	rid_free_pos(clonedposlist);

    DICDBG(("Image change finished\n"));

    /* SJM: moved from top of function */
    if (status != image_cb_status_UPDATE_ANIM)
	be_update_image_info(doc);
}

void antweb_doc_image_change(void *h, void *i, int status, wimp_box *box_update)
{
#if CATCHTYPE5
    be_doc doc = (be_doc)h;

    if ( doc->flags & doc_flag_WRECKED )
        return;

    sig_guard( doc );

    if ( setjmp(jbGuard) == 0 )
#endif
        antweb_doc_image_change2( h, i, status, box_update );

#if CATCHTYPE5
    sig_unguard();
#endif
}

static void be_view_visit(antweb_doc *doc)
{
#ifdef STBWEB
    if (doc->frag)
    {
        char *url_and_frag = mm_malloc(strlen(doc->url) + 1 + strlen(doc->frag) + 1);
        strcpy(url_and_frag, doc->url);
        strcat(url_and_frag, "#");
        strcat(url_and_frag, doc->frag);

        frontend_view_visit(doc->parent, doc, url_and_frag, doc->rh->title);

        mm_free(url_and_frag);
    }
    else
        frontend_view_visit(doc->parent, doc, doc->url, doc->rh->title);
#else
    frontend_view_visit(doc->parent, doc, doc->url, doc->rh->title);
#endif
}

static void antweb_init_page(antweb_doc *doc)
{
    if ((doc->flags & doc_flag_DISPLAYING) == 0)
    {
	fe_view_dimensions fvd;

	BENDBG(( "doc%p: antweb_init_page: setting displaying\n", doc));

	frontend_view_get_dimensions(doc->parent, &fvd);

	doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
	doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif
	ASSERT(doc->rh->stream.fwidth > 0);

	antweb_document_format(doc, doc->rh->stream.fwidth);

	BENDBG(( "doc%p: calling visit\n", doc));

	/* We assume we can use the title here because it is supposed to be
	   in the header and the header is supposed to come before the body. */

	be_view_visit(doc);

	doc->flags |= doc_flag_DISPLAYING;

	be_update_image_info(doc);

	antweb_default_caret(doc, FALSE);
    }
    else
        BENDBG(( "doc%p: antweb_init_page: already displaying\n", doc ));
}


static void antweb_doc_background_change(void *h, void *i, int status, wimp_box *box)
{
#ifndef BUILDERS
    int width, height;
    int fl;
    antweb_doc *doc = (antweb_doc *) h;

/*  BENDBG(( "antweb_doc_background_change(): doc %p status %d\n", doc, status)); */

    switch (status)
    {
        case image_cb_status_WORLD:
            frontend_view_status(doc->parent, sb_status_WORLD);
            return;

        case image_cb_status_CACHE:
        case image_cb_status_NOACTION:
            be_update_image_info(doc);
            return;

        default:
            be_update_image_info(doc);
            break;
    }

    image_info((image) doc->rh->tile.im, &width, &height, 0, &fl, 0, 0);

    if ((fl & image_flag_FETCHED) && (fl & image_flag_RENDERABLE))
    {
	doc->rh->tile.width = width * doc->scale_value / 100;
	doc->rh->tile.height = height * doc->scale_value / 100;

	/* If the BG colour is not set and the image has no mask set the BG colour to average */
	/* do this before init page as that may cause a fade which needs the bg colour */
	if (((doc->rh->bgt & rid_bgt_COLOURS) == 0) &&
	    ((fl & image_flag_MASK) == 0))
	{
	    BENDBG(( "Setting background colour\n"));

	    doc->rh->bgt |= rid_bgt_COLOURS;
	    doc->rh->colours.back = image_average_colour(i);

	    BENDBG(( "New bg colour is 0x%08x\n", doc->rh->colours.back));
	}

        antweb_init_page(doc);

	if (doc->flags & doc_flag_DISPLAYING)
	    frontend_view_redraw(doc->parent, NULL);
    }
    else
    {
	if (status == image_cb_status_REFORMAT)
	{
	    /* Told to reformat but we have no image?  It must have been flushed */
	    doc->rh->tile.width = doc->rh->tile.height = 0;

	    if (doc->flags & doc_flag_DISPLAYING)
		frontend_view_redraw(doc->parent, NULL);
	}
    }

#endif /* BUILDERS */
    box = box;
}

extern pparse_details *be_lookup_parser(int ft)
{
    int i;

    if (image_type_test(ft))
	ft = FILETYPE_ANY_IMAGE;

    for(i=0; file_parsers[i].ftype != -1 && file_parsers[i].ftype != ft; i++)
	;

    BENDBG(( "Using parser %d for file type 0x%03x\n", i, ft));

    return &file_parsers[i];
}

#define PPARSE_BUFSIZE	4096

static void be_pparse_doc(antweb_doc *doc, int fh, int from, int to)
{
    char *buffer;		/* changed from auto to malloc */
    os_gbpbstr gpb;
    os_error *ep;
    int len;
    rid_text_item *ti;
    rid_header *rh;

    /* SJM: 28Oct97: unnecessary - removed */
/*  if (doc->rh == NULL) */
/* 	doc->rh = (((pparse_details*)doc->pd)->rh)(doc->ph); */

    buffer = mm_malloc(PPARSE_BUFSIZE);

    rh = doc->rh;
    /* SJM: 28Oct97: unnecessary - removed */
/*  rh->doc = doc; */

    ti = rh->stream.text_last;
    PPDBG(("Old last item is 0x%p\n", ti));
    while (from < to)
    {
	len = (to-from) > PPARSE_BUFSIZE ? PPARSE_BUFSIZE : (to-from);

	gpb.action = 3;
	gpb.file_handle = fh;
	gpb.data_addr = buffer;
	gpb.number = len;
	gpb.seq_point = from;
	PPDBG(("Reading %d bytes from file %d.\n", len, fh));
	ep = os_gbpb(&gpb);
	if (ep == NULL)
	{
	    PPDBG(("Sending %d bytes to the parser.\n", len));
	    (((pparse_details*)doc->pd)->data)(doc->ph, buffer, len, (from + len) < to );
	}
	else
	{
	    PPDBG(("Getting file data returned error: %s\n", ep->errmess));
	    PPDBG(("In: file=%d, buffer=%p, number=%d, seq=%d\n",
		    fh, buffer, len, from));
	    PPDBG(("Out: file=%d, buffer=%p, number=%d, seq=%d\n",
		    fh, buffer, len, from));
	}

	from += len;
    }

    mm_free(buffer);

#if USE_MARGINS
#if DEBUG
    if ((rh->margin.left != -1 && rh->margin.left*2 != doc->margin.x0) ||
	(rh->margin.top != -1 && rh->margin.top*2 != doc->margin.y1))
	PPDBG(("setting margins to %d,%d\n", doc->margin.x0, doc->margin.y1));
#endif
    if (rh->margin.left != -1)
    {
	doc->margin.x0 = rh->margin.left*2;
	doc->margin.x1 = -doc->margin.x0;
    }

    if (rh->margin.top != -1)
    {
	doc->margin.y0 = rh->margin.top*2;
	doc->margin.y1 = -doc->margin.y0;
    }
#endif

    if (ti == NULL)
    {
	ti = rh->stream.text_list;
	PPDBG(("No old value, first item is at 0x%p\n", ti));
    }
    else
    {
        /* Inefficient but correct - and easy! */
        if (ti->tag != rid_tag_TABLE && ti->tag != rid_tag_TEXTAREA)/* SJM: 240196: quick hack to add texteareas here */
            ti = rid_scanf(ti);
	PPDBG(("Moved on from last item to 0x%p\n", ti));
    }

    /* pdh: do this *BEFORE* starting any other image fetches! (please) */
    if ( (doc->rh->bgt & rid_bgt_IMAGE)
         && (doc->rh->tile.im == NULL) )
    {
        BENDBG(( "Calling fetch_bg from pparse_doc\n"));
	be_doc_fetch_bg(doc);
    }

    /* SJM: 30/09/97: Does this do anything useful anymore/ever ?? */
    PPDBG(("Sizing objects from 0x%p\n", ti));
    while (ti)
    {
	(object_table[ti->tag].size)(ti, rh, doc);

        ti = rid_scanf(ti);
    }
    PPDBG(("Done sizing\n"));
}

static void be_doc_fetch_bg(antweb_doc *doc)
{
    char *url;

    BENDBG(( "be_doc_fetch_bg(): doc %p\n", doc));

    /* record time at which we started fetching */
    doc->start_time = clock();

    url = url_join(BASE(doc), doc->rh->tile.src);

    image_find(url, BASE(doc),
	       (doc->flags & doc_flag_FROM_HISTORY ? 0 : image_find_flag_CHECK_EXPIRE) |
	       image_find_flag_URGENT | image_flag_NO_BLOCKS |
	       (gbf_active(GBF_LOW_MEMORY) ? image_find_flag_DEFER : 0),
	       &antweb_doc_background_change, doc, render_get_colour(render_colour_BACK, doc),
	       (image*) &(doc->rh->tile.im));

    antweb_doc_background_change(doc, doc->rh->tile.im, image_cb_status_UPDATE, NULL);

    mm_free(url);
}

/* This chunk used to be inside antweb_doc_progress. It passes data
 * to the parser, either by reading it from the file or fby closing
 * the parser down, and then decides what reformatting to do.  */

static void progress_parse_and_format(antweb_doc *doc, int fh, int lastptr, int so_far)
{
    rid_text_item *oti;
    rid_form_item *ofi;
    rid_select_item *osi;
    rid_option_item *ooi;

    oti = doc->rh->stream.text_last;
    ofi = doc->rh->form_last;
    osi = ofi ? ofi->last_select : NULL;
    ooi = osi ? osi->last_option : NULL;

    PPDBG(("progress_parse_and_format: doc%p fh %d lastptr %d so_far %d\n", doc, fh, lastptr, so_far));
    PPDBG(("progress_parse_and_format: old text %p form %p select %p object %p\n", oti, ofi, osi, ooi));

    if (fh)
    {
	be_pparse_doc(doc, fh, lastptr, so_far);

	PPDBG(("pparse from progress done\n"));
    }
    else
    {
        doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, doc->cfile);
	doc->ph = NULL;

	PPDBG(("parser closedown done\n"));
    }

    /* If the last item is a table, we must still be within the */
    /* table, so redisplaying it probably isn't a bad thing to do */
    /* Once the table is completed, another pad tag will be added */
    /* and we can skip over it as normal */

    if (oti != doc->rh->stream.text_last ||			/* if we have a new top-level text item, OR */
	(oti != NULL &&						/*   we have some text, AND */
	 (oti->tag == rid_tag_TABLE ||				/*     it's a table, OR ONE OF */
	  ofi != doc->rh->form_last ||				/*     we are in a different form */
	  osi != (ofi ? ofi->last_select : NULL) ||		/*     we are in a different select item */
	  ooi != (osi ? osi->last_option : NULL) ) ) )		/*     we are in a different object */
    {
	PPDBG(( "Text list seems to have changed.\n"));

	if ((doc->flags & doc_flag_DISPLAYING) == 0)
	{
	    BOOL waiting_for_bg = FALSE;

	    /* if we have an image possibly fetching see if it is here */
	    if (doc->rh->tile.im)
	    {
		image_flags flags = 0;
		image_info(doc->rh->tile.im, NULL, NULL, NULL, &flags, NULL, NULL);
		if (flags & image_flag_FETCHED)
		{
		    BENDBG(( "background is here already\n"));
		}
		else if (clock() < doc->start_time + config_display_time_background)
		{
		    waiting_for_bg = TRUE;
		    BENDBG(( "waiting for background\n"));
		}
		else
		{
		    BENDBG(( "background wait timeout\n"));
		}
	    }

	    if (!waiting_for_bg)
	    {
	        BENDBG(( "doc%p: calling init_page from parse_and_format\n",
	                 doc ));
		antweb_init_page(doc);
	    }
	}
	else
	{
	    BENDBG(( "Tail changed, checking for select items\n"));
	    BENDBG(( "ofi=%p, osi=%p, ooi=%p\n", ofi, osi, ooi));

	    /* We had an option in the last visable select and
	       either the last form no longer has a select or
	       the last select of the last form is not the same */
	    if ((ooi || (osi && osi->last_option)) &&
		((!doc->rh->form_last->last_select) ||
		 (ooi != doc->rh->form_last->last_select->last_option) ) )
	    {
		PPDBG(( "Select found, reformatting the lot, doc=%p rh=%p\n", doc, doc->rh ));

		/* dispose of the item and resize it */
		if (osi && osi->base.display && object_table[osi->base.display->tag].dispose)
		{
		    (object_table[osi->base.display->tag].dispose)(osi->base.display, doc->rh, doc);
		    (object_table[osi->base.display->tag].size)(osi->base.display, doc->rh, doc);
		}

		/* Let's make sure we get it right */
		antweb_document_format(doc, doc->rh->stream.fwidth);

		/* Force a major redraw if the item is in or above the visable */
#if 0
		if (oti == NULL)
		    oti = doc->rh->stream.text_list;

		if (oti)
		{
		    hiline = ((oti->line->top > osi->base.display->line->top) ?
			      oti->line :
			      osi->base.display->line );
		    frontend_view_bounds(doc->parent, &bb);

		    if (hiline->top > bb.y0)
		    {
			bb.y1 = hiline->top;
			be_view_redraw(doc, &bb);
		    }
		}
#else
		frontend_view_redraw(doc->parent, NULL);
#endif
		PPDBG(( "Tail reformat (with select items) done\n"));
	    }
	    /* if we haven't started yet then we need to do a full format rather than a tail format */
	    else if (doc->rh->stream.text_list == NULL || doc->rh->stream.text_list->line == NULL)
	    {
		antweb_document_format(doc, doc->rh->stream.fwidth);
		frontend_view_redraw(doc->parent, NULL);
	    }
	    else
	    {
		PPDBG(( "Reformatting the tail\n"));
		TASSERT( doc->rh );
		be_document_reformat_tail(doc, oti, doc->rh->stream.fwidth);
		PPDBG(( "Tail reformat done\n"));
	    }
	}

	if (doc->flags & doc_flag_DISPLAYING)
	    be_set_dimensions(doc);
    }
}

/* This will tend to redraw tables as they are arriving. */
/* It should be okay once the </TABLE> has been processed. */

static void antweb_doc_progress2(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    antweb_doc *doc = (antweb_doc *) h;
    static antweb_doc *threaded = NULL;

    ACCDBG(("doc%p: antweb_doc_progress(%d) called\n", doc, status ));

#if TIMEOUT
    CheckLastModified( doc );
#endif

    /* Make the world turn a little */
    frontend_view_status(doc->parent, sb_status_WORLD);

    if ( status == status_COMPLETED_PART )
    {
        PPDBG(("doc%p: disposing of contents due to completed part\n", doc ));
        be_dispose_doc_contents( doc );
        doc->lbytes = 0;
        doc->flags &= ~doc_flag_DISPLAYING;
    }

    if (status == doc->lstatus && so_far == doc->lbytes)
	return;

    if (threaded)
    {
	usrtrc( "/n/n/n************* Progress reentered !!!! **************/n/n/n");
	usrtrc( "Old doc=%p, new doc=%p\n", threaded, doc);
    }
    else
    {
	threaded = doc;
    }

    PPDBG(("doc%p: adp, so_far=%d ftype=%d fh=%d status=%d\n",
           doc, so_far, ftype, fh, status ));

    /* SJM: 18Jul97: don't allow COMPLETED_PART here so that we don't read data until we've switched files */
    if (so_far > 0 && ftype != -1 && fh &&
         ((status == status_GETTING_BODY)/*  || (status == status_COMPLETED_PART) */ ) )
    {
	int lastptr = doc->lbytes;

	if (doc->lstatus != status_GETTING_BODY || lastptr == -1)
	    lastptr = 0;

	PPDBG(("Data arriving; type = 0x%03x, file=%d, last had %d, now got %d\n",
	       ftype, fh, lastptr, so_far));

	/* lookup the parser */
	if (doc->pd == NULL)
	    doc->pd = be_lookup_parser(ftype);

	if (doc->ph == NULL && ((pparse_details*)doc->pd)->new)
	{
	    int encoding = 0;
	    int encoding_source = rid_encoding_source_USER;

#if UNICODE
	    /* work out what encoding to start in */
	    encoding = doc->encoding_user;
	    DBG(("set_encoding USER%s %d\n", doc->encoding_user_override ? "_FIXED" : "", encoding));

	    if (doc->encoding_user_override)
		encoding_source = rid_encoding_source_USER_FIXED;
	    else
	    {
		int enc = access_get_encoding(doc->ah);
		if (enc)
		{
		    encoding = enc;
		    encoding_source = rid_encoding_source_HTTP;
		}
	    }
#endif
	    PPDBG(("About to make a new parser stream\n"));

	    PPDBG(("antweb_doc_progress: encoding user %d override %d - set %d rh %d source %d\n",
		 doc->encoding_user, doc->encoding_user_override,
		 encoding, doc->rh->encoding, encoding_source));

	    /* initialise the parser */
	    doc->ph = (((pparse_details*)doc->pd)->new)(url, ftype, encoding);
	    if (doc->ph)
	    {
		/* new: check if URL has changed - since we set the URL on entry now */
		if (doc->url == NULL || strcmp(doc->url, url) != 0)
		{
		    mm_free(doc->url);
		    doc->url = strdup(url);
		}

		/* get the rid_header handle */
		doc->rh = (((pparse_details*)doc->pd)->rh)(doc->ph);

		/* SJM: 28Oct97: set up back pointer here */
		doc->rh->doc = doc;
		doc->rh->encoding_source = encoding_source;

#if 0
		/* can this ever be called? SJM: 28Oct97: No - removed */
		if (doc->rh->stream.text_last)
		{
		    PPDBG(("Sizing the first few items...\n"));
		    antweb_trigger_fetching(doc);
		    PPDBG(("... done\n"));
		}
#endif
	    }
	}

#ifndef BUILDERS
	/* can't put this code in complete as http_close will have been called */
	if (doc->rh && (doc->flags & doc_flag_HAD_HEADERS) == 0)
	{
	    http_header_item *list = access_get_headers(doc->ah);

	    BENDBG(( "doc%p: add meta ah %p list %p\n", doc, doc->ah, list));

	    for (; list; list = list->next)
	    {
		rid_meta_item *m = mm_calloc(sizeof(rid_meta_item), 1);

		BENDBG(( "doc%p: add meta rh %p list %p\n", doc, doc->rh, list));

		m->httpequiv = strdup(list->key);
		m->content = strdup(list->value);

		rid_meta_connect(doc->rh, m);
	    }

	    doc->flags |= doc_flag_HAD_HEADERS;
	}
#endif

	if (doc->ph)
	    progress_parse_and_format(doc, fh, lastptr, so_far);
    }

    doc->lstatus = status;
    doc->lbytes = so_far;

    if (status == status_GETTING_BODY || status == status_REQUEST_BODY)
    {
	frontend_view_status(doc->parent,
			     status == status_GETTING_BODY
			        ? access_fromcache(doc->ah) ? sb_status_LOADING
			                                    : sb_status_FETCHED
			        : sb_status_SENT,
			     so_far, size);
    }
    else
    {
	frontend_view_status(doc->parent, sb_status_PROGRESS, status);
    }

#if DEBUG >= 3
    PPDBG(( "Progress done\n"));

    if (doc->rh) dump_header(doc->rh);

#endif

    threaded = NULL;
}

#if TIMEOUT


/*
 * Following string is "last-modified" encoded by the following algorithm:
 *
 * A$="LAST-MODIFIED"
 * L%=LENA$
 * S%=ASC"P"
 * FOR I%=1 TO L%
 *   C% = ASCMID$(A$,I%,1)
 *   D% = C%
 *   C% = (C%-S%) AND &FF
 *   S% = D%
 *   PRINT C%
 * NEXT
 */
static const char lastmodified[] = { 252,245,18,1,217,32,2,245,5,253,3,252,255 };

unsigned int timeout_time_t = TIMEOUT_TIME_T;

static void CheckLastModified( be_doc doc )
{
    char header[20];
    unsigned char c = 'P';
    const char *answer;
    int i;
    time_t t;

    for ( i=0; i<13; i++ )
    {
        c += lastmodified[i];
        header[i] = (char)c;
    }
    header[13] = '\0';

    answer = backend_check_meta( doc, header );

    memset( header, 0, sizeof(header) );

    if ( !answer )
        return;

    t = HTParseTime( answer );

    if ( t > timeout_time_t )       /* unsigned comparison, we hope */
        TimeoutError();
}

#endif


    /*===============================*/


static void antweb_doc_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
#if CATCHTYPE5
    be_doc doc = (be_doc)h;

    if ( doc->flags & doc_flag_WRECKED )
        return;

    sig_guard( doc );

    if ( setjmp(jbGuard) == 0 )
#endif
        antweb_doc_progress2( h, status, size, so_far, fh, ftype, url );

#if CATCHTYPE5
    sig_unguard();
#endif
}

        /*========================*/

/*
 * If the status is status_FAILL_CONNECT then 'cfile' is actually the error message.
 */

static access_complete_flags antweb_doc_complete2(void *h, int status, char *cfile, char *url)
{
    antweb_doc *doc = (antweb_doc *) h;
    int ft, r;

    frontend_view_status(doc->parent, sb_status_PROGRESS, status);

#if TIMEOUT
    CheckLastModified( doc );
#endif

    BENDBG(("doc%p: antweb_doc_complete(%d) called\n", doc, status ));

    ACCDBG(("Access completed, doc=%p, status=%d, file='%s', url='%s'\n",
	    doc, status, strsafe(cfile), url));

    doc->flags &= ~doc_flag_DIRECTORY;

    /* moved this to the start so we can abort if the file size is 0 */
    if (status == status_COMPLETED_FILE)
    {
	os_filestr ofs;

	ofs.action = 5;
	ofs.name = cfile;

	os_file(&ofs);

	doc->file_load_addr = ofs.loadaddr;
	doc->file_exec_addr = ofs.execaddr;
	doc->file_size = ofs.start;
#ifndef BUILDERS
	if ( doc->ah && access_was_directory( doc->ah ) )
	    doc->flags |= doc_flag_DIRECTORY;
#endif
    }

    if (status != status_COMPLETED_FILE || doc->file_size == 0)
    {
	/* if failed to connect pass cfile (actually error message) in the title field
	 * pretty nasty but then we're gonna rewrite this anyway aren't we?
	 */

	if (cfile == NULL)
	{
	    char tag[16];
	    sprintf(tag, "status%d", status);
	    cfile = msgs_lookup(tag);
	}

#ifndef BUILDERS

	if (status == status_BAD_FILE_TYPE)
	{
	    char *name = get_file_type_name(access_get_ftype(doc->ah));	/* unsupported file type */
	    frontend_view_visit(doc->parent, NULL, url,	(char *)makeerrorf(ERR_BAD_FILE_TYPE, strsafe(name)));
	}
	else
	{
	    frontend_view_visit(doc->parent, NULL, url,
			    status == status_FAIL_REDIAL ?
				NULL :								/* don't want to display the error in this case */
			    status == status_FAIL_LOCAL ?
				(char *)makeerror(ERR_NO_DISC_SPACE) :				/* local error (probably out of disc space) */
			    status == status_FAIL_DNS ?
				(char *)makeerrorf(ERR_CANT_GET_URL, strsafe(url), cfile) :	/* cannot find the web page */
				(char *)makeerror(ERR_UNSUPORTED_SCHEME));			/* cannot display the web page */
	}
#endif

	BENDBG(("doc%p: fe_view_visit returns, setting ah=NULL\n", doc ));

	doc->ah = NULL;

	backend_dispose_doc(doc);
	return 0;
    }

    doc->ah = NULL;

    doc->cfile = strdup(cfile);
    if (doc->url == NULL)
	doc->url = strdup(url);

    BENDBG(( "Completed opening %s\n", url));
    BENDBG(( "Cache file is '%s'\n", doc->cfile));

    ft = file_type(cfile);

    if (doc->ph)
    {
	const char *refresh;

        PPDBG(("Closing parser down\n"));

#if 1
	progress_parse_and_format(doc, 0, 0, 0);
#else
	doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, doc->cfile);
	doc->ph = NULL;
#endif

	/* pdh: @@@@ FIXME
	 * This doesn't belong here (there may be images arriving) but I can't
	 * see some pages without it!
	 * sjm: moved here rather than outside this 'if' so that entire document
	 * has definitely been parsed.
	 */
	if (doc->rh && doc->im_fetching == 0 && gbf_active(GBF_FVPR))
	    fvpr_progress_stream_flush( &doc->rh->stream );

	if ((doc->rh->bgt & rid_bgt_IMAGE) && (doc->rh->tile.im == NULL))
	{
	    BENDBG(( "Calling fetch_bg from doc_complete\n" ));
	    be_doc_fetch_bg(doc);
	}

	if ((doc->flags & doc_flag_DISPLAYING) == 0)
	{
	    BENDBG(( "doc%p: calling init_page from complete\n", doc ));
	    antweb_init_page(doc);
	}

        /* SJM: layout frames */
        if (doc->rh->frames)
        {
            fe_view_dimensions fvd;
	    int dividers[4], max;

            frontend_view_get_dimensions(doc->parent, &fvd);
	    max = frontend_view_get_dividers(doc->parent, dividers);

	    layout_layout(doc, fvd.layout_width, fvd.layout_height, 0, dividers, max);

	    doc->rh->stream.widest = fvd.wa_width;
	    doc->rh->stream.height = fvd.wa_height;
        }

        /* SJM: check the images for external client-side imagemaps */
        frontend_complain(imagemap_check_all_images(doc));

        /* SJM: see if width changes and reformat */
        be_set_dimensions(doc);

	be_update_image_info(doc);

#if !defined(STBWEB) && !defined(BUILDERS)
	if (frontend_view_has_caret(doc->parent))
	{
	    antweb_place_caret(doc,
			       backend_highlight_link(doc, doc->rh->stream.text_list, be_link_INCLUDE_CURRENT | be_link_TEXT | be_link_DONT_HIGHLIGHT | be_link_VISIBLE),
			       -1);
	}
#endif

	/* Override the visability of the caret */
#ifdef STBWEB
	if ((doc->flags & doc_flag_FROM_HISTORY) == 0)
#endif
	    frontend_complain(backend_goto_fragment(doc, doc->frag));

	/* check for a refresh tag */
	refresh = backend_check_meta(doc, "REFRESH");
	if (refresh)
	{
	    /* New way for new parse_http_header */
	    static const char *content_tag_list[] = { "URL", 0 };
	    name_value_pair vals[2];
	    char *s = strdup(refresh);

    	    parse_http_header(s, content_tag_list, vals, sizeof(vals)/sizeof(vals[0]));

    	    doc->rh->refreshurl = strdup(vals[0].value);
	    /* YUCH - conditional; ? : */
	    doc->rh->refreshtime = vals[1].name == NULL ? -1 :
#ifdef STBWEB
		!strcasecomp(vals[1].name, "ondispose") ? -2 :
#endif
		atoi(vals[1].name);

	    mm_free(s);

	    if (doc->rh->refreshtime >= 0)
		alarm_set(alarm_timenow()+(doc->rh->refreshtime * 100), be_refresh_document, doc);
	}

	frontend_view_status(doc->parent, sb_status_FINISHED);

	r = access_CACHE;
    }
    else
    {
	BENDBG(( "Got document of type 0x%03x, passing to the front end.\n", ft));

#ifndef BUILDERS
	r = access_CACHE | access_LOCK;	/* maintain the lock on the file as it is still required */

	if (frontend_plugin_handle_file_type(ft))
	{
	    /* tell the frontend we've completed the file */
	    frontend_pass_doc(doc->parent, NULL, NULL, -1);

	    /* clear the lock if the helper fails to start */
	    if (plugin_helper(doc->url, ft, NULL, doc->parent, cfile) == NULL)
		r &= ~access_LOCK;
	}
	else
	{
	    frontend_pass_doc(doc->parent, doc->url, cfile, ft);
	}
#endif

	BENDBG(( "Returned from frontend\n"));

	backend_dispose_doc(doc);
    }

    BENDBG(( "'Compleated' function done.\n"));

    return r;
}

static access_complete_flags antweb_doc_complete(void *h, int status, char *cfile, char *url)
{
    access_complete_flags result;
#if CATCHTYPE5
    be_doc doc = (be_doc)h;

    if ( doc->flags & doc_flag_WRECKED )
        return 0;

    sig_guard( doc );
    if ( setjmp(jbGuard) == 0 )
#endif
        result = antweb_doc_complete2(h,status,cfile,url);
#if CATCHTYPE5
    else
        result = 0;

    sig_unguard();

    if ( doc->flags & doc_flag_WRECKED )
        return 0;
#endif

    return result;
}


os_error *backend_doc_abort(be_doc doc)
{
    BENDBG(( "Entering backend_doc_abort for %p\n", doc));

    if ((doc->flags & doc_flag_DISPLAYING) == 0)
	return NULL;

    if (doc->ah)
    {
	BENDBG(( "Calling access_abort\n"));
	access_abort(doc->ah);
	doc->ah = NULL;

	doc->flags |= doc_flag_INCOMPLETE;
    }

    if (doc->ph)
    {
	doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, NULL);
	doc->ph = NULL;

	doc->flags |= doc_flag_INCOMPLETE;
    }

    /* stop downloading any images in transit */
    if (doc->rh)
    {
	rid_text_item *ti;

	for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
	{
            if (object_table[ti->tag].imh)
		(object_table[ti->tag].imh)(ti, doc, object_image_ABORT);
	}

	image_flush_marked();

	/* Fetching has surely become zero now */
	be_update_image_info( doc );

	/* If we have a partial background image then throw it away */
	if (doc->rh->tile.im)
	{
	    image_flags f;
	    if (image_info(doc->rh->tile.im, NULL, NULL, NULL, &f, NULL, NULL) == NULL &&
		(f & image_flag_FETCHED) == 0)
	    {
		image_loose(doc->rh->tile.im, antweb_doc_image_change, doc);
		doc->rh->tile.im = NULL;
	    }
	}
    }

    if ( gbf_active(GBF_FVPR) )
        fvpr_progress_stream_flush( &doc->rh->stream );

    if (doc->flags & doc_flag_INCOMPLETE)
	frontend_view_status(doc->parent, sb_status_ABORTED);

    return NULL;
}

#if ITERATIVE_PANIC
/*
 * level 0 means abort external fetches
 * level 1 means abort internal fetches
 */

int antweb_doc_abort_all(int level)
{
    be_doc doc;
    int count = 0;
    for (doc = document_list; doc; doc = doc->next)
    {
	int incomplete = (doc->flags & doc_flag_INCOMPLETE);

	if (!incomplete)
	{
	    if ((level > 0 || doc->url == NULL ||
		 (strncasecomp(doc->url, "ncint:", sizeof("ncint:")-1) != 0 &&
		  strncasecomp(doc->url, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) != 0))
		 &&
		 (doc->ah || doc->ph))
	    {
		DBG(("antweb_doc_abort_all: doc%p aborted through lack of memory\n", doc));

		/* set first to ensure we aren't reentered */
		doc->flags |= doc_flag_INCOMPLETE;

		if (doc->ah)
		{
		    access_abort(doc->ah);
		    doc->ah = NULL;
		}

 		if (doc->ph)
 		{
 		    doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, NULL);
 		    doc->ph = NULL;
 		}

		if (doc->rh && gbf_active(GBF_FVPR) )
		    fvpr_progress_stream_flush( &doc->rh->stream );

/* 		frontend_view_status(doc->parent, sb_status_ABORTED); */

		count++;
	    }
	}
    }
    return count;
}
#endif

extern os_error *backend_doc_set_flags(be_doc doc, int mask, int eor)
{
    if (mask & be_openurl_flag_ANTIALIAS)
	doc->flags &= ~doc_flag_ANTIALIAS;

    if (mask & be_openurl_flag_BODY_COLOURS)
	doc->flags &= ~doc_flag_DOC_COLOURS;

    if (mask & be_openurl_flag_DEFER_IMAGES)
	doc->flags &= ~doc_flag_DEFER_IMAGES;

    if (mask & be_openurl_flag_SOLID_HIGHLIGHT)
	doc->flags &= ~doc_flag_SOLID_HIGHLIGHT;

    if (mask & be_openurl_flag_HISTORY)
	doc->flags &= ~doc_flag_FROM_HISTORY;

    if (mask & be_openurl_flag_FAST_LOAD)
	doc->flags &= ~doc_flag_FAST_LOAD;


    if (eor & be_openurl_flag_ANTIALIAS)
	doc->flags ^= doc_flag_ANTIALIAS;

    if (eor & be_openurl_flag_BODY_COLOURS)
	doc->flags ^= doc_flag_DOC_COLOURS;

    if (eor & be_openurl_flag_DEFER_IMAGES)
	doc->flags ^= doc_flag_DEFER_IMAGES;

    if (eor & be_openurl_flag_SOLID_HIGHLIGHT)
	doc->flags ^= doc_flag_SOLID_HIGHLIGHT;

    if (eor & be_openurl_flag_HISTORY)
	doc->flags ^= doc_flag_FROM_HISTORY;

    if (eor & be_openurl_flag_FAST_LOAD)
	doc->flags ^= doc_flag_FAST_LOAD;

    return NULL;
}

static void be_refresh_document(int at, void *h)
{
    be_doc doc = (be_doc) h;

    BENDBG(( "doc%p: be_refresh_document()\n", doc ));

    if (doc->url || doc->rh->refreshurl)
    {
	char *url = url_join(BASE(doc), doc->rh->refreshurl ? doc->rh->refreshurl : doc->url);
	frontend_complain(frontend_open_url(url, doc->parent, NULL, NULL, 1));
	mm_free(url);
    }
}

#if 0
extern void backend_set_margin(be_doc doc, wimp_box *margin)
{
    if ((doc->flags & doc_flag_DISPLAYING) == 0)
    {
	doc->margin = *margin;
    }
    else if (doc->margin.x0 != margin->x0 ||
	     doc->margin.x1 != margin->x1 ||
	     doc->margin.y0 != margin->y0 ||
	     doc->margin.y1 != margin->y1)
    {
	doc->margin = *margin;
	backend_reset_width(doc, 0);
    }
}
#endif


extern os_error *backend_open_url(fe_view v, be_doc *docp,
				  char *url, fe_post_info *bfile, char *referer,
				  int flags)
{
    antweb_doc *new;
    os_error *ep;
    char *use_url;
    char *frag;
    access_post_info post, *postp;

    *docp = new = mm_calloc(1, sizeof(antweb_doc));

    new->magic = ANTWEB_DOC_MAGIC;
    new->parent = v;
    new->scale_value = config_display_scale_image;
    new->encoding_user = config_encoding_user;
    new->encoding_user_override = flags & be_openurl_flag_NO_ENCODING_OVERRIDE ? FALSE : config_encoding_user_override;

    /* new: add the url here */
    new->url = strdup(url);

    new->threaded = 1;

    BENDBG(( "doc%p: new in backend_open_url: url '%s' checklist: document_list %p next %p\n",
	   new, strsafe(url), document_list, document_list ? document_list->next : NULL));

    /* add to list of documents, must do now in case we dispose of doc before returning from access_url */
    new->next = document_list;
    document_list = new;

#if DEBUG
    dump_document_list();
#endif

#ifndef BUILDERS
    backend_doc_set_flags(new, -1, flags);

    if (config_display_links_underlined)
	new->flags |= doc_flag_UL_LINKS;
#endif
    use_url = strdup(url);
    if ( (frag = strrchr(use_url, '#')) != 0)
    {
	*frag = 0;
	new->frag = strdup(frag+1);
    }

    BENDBG(( "Calling access function flags %x\n", flags));

#if USE_MARGINS
    frontend_view_margins(v, &new->margin);
#endif

    if (bfile)
    {
	post.body_file = bfile->body_file;
	post.content_type = bfile->content_type;
	postp = &post;
    }
    else
	postp = NULL;

    ep = access_url(use_url,
		    (flags & be_openurl_flag_NOCACHE ? access_NOCACHE : 0) |
		    (flags & be_openurl_flag_HISTORY ? 0 : access_CHECK_EXPIRE) |
		    (flags & be_openurl_flag_FAST_LOAD ? access_MAX_PRIORITY : 0) |
		    access_CHECK_FILE_TYPE | access_PRIORITY,
		    NULL, postp, referer,
		    &antweb_doc_progress, &antweb_doc_complete, new, &new->ah);

    mm_free(use_url);

    if (ep == 0)
    {
	frontend_view_status(v, sb_status_URL, url);

	if (new->ah == NULL)
	{
	    /* backend_dispose_doc will have been hopefully called from complete */
	    frontend_view_status(v, sb_status_PROGRESS, status_COMPLETED_CACHE);
	}
	else
	{
	    /* transfer is underway normally */
	    BENDBG(( "New access handle is 0x%p\n", new->ah));
	}
#if 0
	if (!antweb_doc_in_list(new))
	{
	    /* document has already been freed from within access_url() */
	    *docp = NULL;
	}
#endif
    }
    else
    {
	/* get the error number safely */
	MemCheck_checking checking = MemCheck_SetChecking(0, 0);
	int errnum = ep->errnum;
	MemCheck_RestoreChecking(checking);

	switch (errnum)
	{
	case ANTWEB_ERROR_BASE + ERR_USED_HELPER:
	    frontend_url_punt(v, url, bfile);
	    frontend_view_status(v, sb_status_HELPER);
	    ep = 0;
	    break;

	case ANTWEB_ERROR_BASE + ERR_NO_ACTION:
	    frontend_view_status(v, sb_status_FINISHED);
	    break;

	default:
	    frontend_view_status(v, sb_status_FINISHED);
	    break;
	}

#if 1
	/* check not already marked for deletion just in case */
	if (!new->pending_delete)
	{
	    BENDBG(("doc%p: error, not pending, calling dispose\n", new));
	    backend_dispose_doc(new);
	    *docp = NULL;
	    return ep;

	    /* DON'T drop through and try to dispose again */
	}
#else
	/* only free if still in list in case already freed */
	if (antweb_doc_in_list(new))
	{
	    antweb_unlink_doc(new);
	    mm_free(new);
	    *docp = NULL;
	}
#endif
    }

    new->threaded--;
    if (new->pending_delete)
    {
	BENDBG(("doc%p: error, pending=%d, calling dispose\n", new, new->pending_delete));
	backend_dispose_doc(new);
	*docp = NULL;
    }

    return ep;
}

BOOL backend_doc_saver_text(char *fname, void *h)
{
    be_doc doc = (be_doc) h;
    FILE *f;

    f = mmfopen(fname, "w");
    if (f == NULL)
	return FALSE;

    stream_write_as_text(doc->rh, &(doc->rh->stream), f);

    mmfclose(f);

    frontend_saver_last_name(fname);

    return TRUE;
}

BOOL backend_doc_saver_draw(char *fname, void *h)
{
    return drawfile_doc_saver_draw(fname, (be_doc) h);
}

BOOL backend_image_saver_sprite(char *fname, void *h)
{
    image im = (image) h;
    int OK;

    IMGDBG(("im%p: saving as sprite %s\n", im, fname ));

    OK = (frontend_complain(image_save_as_sprite(im, fname)) == NULL);

    if (OK)
	frontend_saver_last_name(fname);

    return OK;
}

BOOL backend_image_saver_jpeg(char *fname, void *h)
{
    image im = (image) h;
    int OK;

    IMGDBG(("im%p: saving as jpeg %s\n", im, fname ));

    OK = (frontend_complain(image_save_as_jpeg(im, fname)) == NULL);

    if (OK)
	frontend_saver_last_name(fname);

    return OK;
}

os_error *backend_doc_key(be_doc doc, int key, int *used)
{
    rid_text_item *ti = be_doc_read_caret(doc);

    *used = 0;

    doc->threaded++;

    if (ti && object_table[ti->tag].key)
    {
	*used = (object_table[ti->tag].key)(ti, doc->rh, doc, key);
	/* This is expected to do the redraw if needed */
    }

    doc->threaded--;
    if (doc->pending_delete)
	backend_dispose_doc(doc);

    return NULL;
}

#ifdef STBWEB
os_error *backend_doc_images(be_doc doc, int *waiting, int *fetching, int *fetched, int *errors, int* in_trans, int *so_far)
{
    *waiting = doc->im_unfetched;
    *fetching = doc->im_fetching;
    *fetched = doc->im_fetched;
    *errors = doc->im_error;
    *in_trans = doc->im_in_transit;
    *so_far = doc->im_so_far;

    return NULL;
}
#endif

int backend_total_images(be_doc doc )
{
    return doc ? doc->im_unfetched + doc->im_fetching
                 + doc->im_fetched + doc->im_error
               : 0;
}

/* ============================================================================= */

os_error *backend_activate_link(be_doc doc, be_item item, int flags)
{
    if (!item)
	return NULL;
    BENDBG(( "activate link %p (type %d)\n", item, item->tag));

    backend__doc_click(doc, item, item->tag == rid_tag_INPUT ? item->width : 0, 0,
			      (flags & 1) ? wimp_BRIGHT : wimp_BLEFT );

    return NULL;
}

/* veneers onto access.c functions */

#ifdef STBWEB
char *backend_temp_file_name(void)
{
    return access_scrapfile();
}
#endif

#ifdef STBWEB
void backend_temp_file_register(char *url, char *file_name)
{
    access_insert(url, file_name, cache_flag_OURS);
}
#endif

const char *backend_check_meta(be_doc doc, const char *name)
{
    rid_meta_item *m;

    if ( !doc || !doc->rh )
        return NULL;

    for (m = doc->rh->meta_list; m; m = m->next)
    {
        if ((m->name && strcasecomp(m->name, name) == 0) ||
            (m->httpequiv && strcasecomp(m->httpequiv, name) == 0))
            return m->content;
    }

    return NULL;
}

#if FRAMES
int backend_frame_resize_bounds(be_doc doc, int x, int y, wimp_box *box, int *handle)
{
    return layout_frame_resize_bounds(doc, x, y, box, handle);
}

void backend_frame_resize(be_doc doc, int x, int y, int handle)
{
    layout_frame_resize(doc, x, y, handle);
}
#endif

#ifdef STBWEB
void backend_image_expire(be_doc doc, void *imh)
{
#ifdef BUILDERS
#define image_expire(x)
#endif

    if (imh)
    {
	image_expire((image) imh);
    }
    else
    {
	rid_header *rh = doc->rh;
	if (rh)
	{
	    be_item ti = rh->stream.text_list;

	    /* Free any data that we have added to the text list */
	    /* Scan through tables to catch contained images */
	    while (ti)
	    {
		if (object_table[ti->tag].imh != NULL)
		    image_expire((object_table[ti->tag].imh)(ti, doc, object_image_HANDLE));

                ti = rid_scanfr(ti);
	    }

	    if (rh->tile.im)
		image_expire(rh->tile.im);
	}
    }
}
#endif

#ifdef STBWEB
void backend_doc_reformat(be_doc doc)
{
    if (doc)
    {
	doc->scale_value = 100;

	antweb_trigger_fetching(doc);
	antweb_document_format(doc, doc->rh->stream.fwidth);

	be_set_dimensions(doc);

	antweb_default_caret(doc, FALSE);
    }
}
#endif

#ifndef FRESCO
void backend_doc_set_scaling(be_doc doc, int scale_value)
{
    if (doc && (doc->scale_value != scale_value) )
    {
	doc->scale_value = scale_value;

	antweb_trigger_fetching(doc);
	antweb_document_format(doc, doc->rh->stream.fwidth);

	be_set_dimensions(doc);

	antweb_default_caret(doc, FALSE);
    }
}
#endif

#ifndef BUILDERS

/*
 * FIXME: Note that really this routine should enumerate the plugins
 * for the ALL and HELPERS cases.
 */

static void be__plugin_abort_or_action(be_item item, int action)
{
    plugin pp = NULL;

    if (item && item->tag == rid_tag_OBJECT)
    {
	rid_text_item_object *tio = (rid_text_item_object *) item;
	rid_object_item *obj = tio->object;

	if (obj->type == rid_object_type_PLUGIN)
	    pp = obj->state.plugin.pp;
    }

    if (action == be_plugin_action_CLOSE)
	plugin_send_close(pp);
    else if (action == be_plugin_action_ABORT)
	plugin_send_abort(pp);
    else
	plugin_send_action(pp, action);
}
#endif

extern void backend_plugin_action(be_doc doc, be_item item, int action)
{
#ifndef BUILDERS
    if (item == be_plugin_action_item_HELPERS)
    {
	be__plugin_abort_or_action(NULL, action);
    }
    else if (item == be_plugin_action_item_ALL)
    {
	if (doc)
	{
	    be_item ti = rid_scan(doc->rh->stream.text_list, SCAN_THIS | SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT);
	    while (ti)
	    {
		be__plugin_abort_or_action(ti, action);

		ti = rid_scan(ti, SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT);
	    }
	}
    }
    else
    {
	be__plugin_abort_or_action(item, action);
    }
#endif
}

extern void backend_plugin_info(be_doc doc, void *pp, int *flags, int *state)
{
#ifndef BUILDERS
    plugin_get_info(pp, flags, state);
#endif

    NOT_USED(doc);
}


/*---------------------------------------------------------------------------*
 * backend_mark_page_visited                                                 *
 * Go through all pages marking links to the given url as visited (so it     *
 * shows up in visited colour when next redrawn)                             *
 *---------------------------------------------------------------------------*/

void backend_mark_page_visited( const char *url )
{
    be_doc doc = document_list;

    while ( doc )
    {
        rid_header *rh = doc->rh;

        if ( rh )
        {
            rid_aref_item *aref = rh->aref_list;

            while ( aref )
            {
                if ( aref->href && !strcmp( aref->href, url ) )
                    aref->flags &= ~rid_aref_CHECKED_CACHE;

                aref = aref->next;
            }
        }

        doc = doc->next;
    }
}


extern be_item backend_locate_id(be_doc doc, const char *id)
{
    rid_header *rh = doc->rh;
    if (rh)
    {
	be_item ti = rh->stream.text_list;
	rid_aref_item *last_aref = NULL;

 	BENDBG(( "locate_id: id='%s'\n", id));

	while (ti)
	{
	    rid_aref_item *aref = ti->aref;
	    char *this_id;

	    /* first check for an anchor match */
	    if (aref && aref != last_aref)
	    {
		switch (ti->tag)
		{
		case rid_tag_TEXT:
		case rid_tag_IMAGE:
		    if (aref->name && strcmp(aref->name, id) == 0)
			return ti;
		    break;

		case rid_tag_INPUT:
		case rid_tag_TEXTAREA:
		case rid_tag_SELECT:
		    if ((aref->flags & rid_aref_LABEL) && aref->name && strcmp(aref->name, id) == 0)
			return ti;
		    break;
		}
		last_aref = aref;
	    }

	    /* then get the id for this item */
	    this_id = NULL;
	    switch (ti->tag)
	    {
	    case rid_tag_INPUT:
		this_id = ((rid_text_item_input *)ti)->input->base.id;
		break;

	    case rid_tag_TEXTAREA:
		this_id = ((rid_text_item_textarea *)ti)->area->base.id;
		break;

	    case rid_tag_SELECT:
		this_id = ((rid_text_item_select *)ti)->select->base.id;
		break;

	    case rid_tag_OBJECT:
		this_id = ((rid_text_item_object *)ti)->object->id;
		break;

	    case rid_tag_TABLE:
		this_id = ((rid_text_item_table *)ti)->table->id;
		break;
	    }

/* 	    BENDBG(( "locate_id: ti=%p tag %d this_id='%s'\n", ti, ti->tag, strsafe(this_id))); */

	    /* and compare what is going in */
	    if (this_id && strcmp(this_id, id) == 0)
		return ti;

	    ti = rid_scanfr(ti);
	}
    }
    return NULL;
}

#ifdef STBWEB
extern int backend_doc_encoding(be_doc doc, int *encoding_user, int *encoding_user_override)
{
    if (encoding_user)
	*encoding_user = doc ? doc->encoding_user : config_encoding_user;

    if (encoding_user_override)
	*encoding_user_override = doc ? doc->encoding_user_override : config_encoding_user_override;

    return doc ? doc->rh->encoding : 0;
}
#endif

extern int backend_doc_item_language(be_doc doc, be_item ti)
{
    return ti && ti->language ? ti->language : doc ? doc->rh->language_num : 0;
}

void antweb_uncache_image_info(antweb_doc *doc)
{
    rid_text_item *ti;
    rid_header *rh = doc->rh;
#ifndef BUILDERS
    if (!rh)
	return;

    for (ti = rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if (object_table[ti->tag].imh != NULL)
	{
	    image i;

	    i = (image) (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

	    image_uncache_info(i);
	}
    }

    if (rh->tile.im)
	image_uncache_info(rh->tile.im);
#endif
}

/* eof backend.c */
