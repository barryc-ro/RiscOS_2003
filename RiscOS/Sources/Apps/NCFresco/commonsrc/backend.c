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

#ifdef STBWEB_BUILD
#include "http.h"
#else
#include "../http/httppub.h"
#endif

#ifndef PP_DEBUG
#define PP_DEBUG 0
#endif

#ifndef BE_DEBUG
#define BE_DEBUG DEBUG
#endif

#ifndef LINK_DEBUG
#define LINK_DEBUG 0
#endif

#if PP_DEBUG
#define PPDBG(a) fprintf a
#else
#define PPDBG(a)
#endif

#if BE_DEBUG
#define BEDBG(a) fprintf a
#else
#define BEDBG(a)
#endif

#if LINK_DEBUG
#define LKDBG(a) fprintf a
#else
#define LKDBG(a)
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

void be_caption_stream_origin(rid_table_item *table, int *dx, int *dy);
void be_cell_stream_origin(rid_table_item *table, rid_table_cell *cell, int *dx, int *dy);

/**********************************************************************/

static be_doc document_list = NULL;

/**********************************************************************/

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
                                (object_table[tabscan->tag].imh)(tabscan, doc, object_image_HANDLE) == i)
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
                           (object_table[tabscan->tag].imh)(tabscan, doc, object_image_HANDLE) == i)
                        {
                                have_image = TRUE;
                                break;
                        }
                }
        }

        return have_image;
}


/**********************************************************************/

/*
 * This mechanism bypasses the code in antweb_place_input() and so may need updating.
 */

static rid_text_item *antweb_prev_text_input(rid_text_item *ti, be_doc doc)
{
    while (ti)
    {
	if (ti && object_table[ti->tag].caret &&
	    (object_table[ti->tag].caret)(ti, doc->rh, doc, object_caret_REPOSITION))
	    break;
        ti = rid_scanbr(ti);
    }

    return ti;
}

static rid_text_item *antweb_next_text_input(rid_text_item *ti, be_doc doc)
{
    while (ti)
    {
	if (ti && object_table[ti->tag].caret &&
	    (object_table[ti->tag].caret)(ti, doc->rh, doc, object_caret_REPOSITION))
	    break;
        ti = rid_scanfr(ti);
    }

    return ti;
}

/*****************************************************************************/

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
    RENDBG(("Locate item: doc = %p, x=%d, y=%d\n", doc, *x, *y));

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

    BEDBG((stderr, "antweb_handle_url for '%s' target '%s'\n", strsafe(href), strsafe(target)));

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
#ifndef STBWEB
    if (!href)
    {
	return NULL;
    }
    else if (href[0] == '#' && !new_win)
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
	    if (akbd_pollctl())
	    {
	        backend_update_link(doc, ti, -1);
	    }
	    else
	    {
		BOOL follow_link = TRUE;
                if (config_display_time_activate)
                {
	            backend_update_link_activate(doc, ti, 1);
                    follow_link = wait_for_release(config_display_time_activate);
	            backend_update_link_activate(doc, ti, 0);
    	        }
		if (follow_link)
		    frontend_complain(antweb_handle_url(doc, ti->aref, NULL,
							(bb & wimp_BRIGHT) ? "_blank" : ti->aref->target));
            }
        }
        else
        {
	    antweb_place_caret(doc, doc->input);
	}
    }
    else
    {
	antweb_place_caret(doc, doc->input);
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

    BEDBG((stderr, "doc click on item %p offset %d,%d error %p tag %d click fn %p\n",
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

/*     if (ti) { BEDBG((stderr, "Asked for info on item at %p\n", ti)); } */

    if (link)
        *link = NULL;

    if (title)
	*title = NULL;

    if (ti)
    {
        if (ti->tag == rid_tag_IMAGE)
	{
	    rid_text_item_image *tii = (rid_text_item_image *)ti;
	    int xx = *px - (tii->bwidth * 2) - tii->hspace*2;
	    int yy = ((ti->max_up) - (tii->bwidth *2) - tii->vspace*2) - *py;

	    image_os_to_pixels((image)tii->im, &xx, &yy, doc->scale_value);
	    *px = xx;
	    *py = yy;

	    if (((rid_text_item_image *)ti)->usemap)
	    {
		/* amazingly this didn't seem to get called before*/
		if (object_table[ti->tag].imh != NULL)
		    imh = (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

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

	    /* do this first so we can check imh in switch */
	    if (object_table[ti->tag].imh != NULL)
		imh = (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

	    switch (ii->tag)
	    {
	    case rid_it_TEXT:
	    case rid_it_PASSWD:
		f |= be_item_info_INPUT;
		break;
	    case rid_it_IMAGE:
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

		    /* Do set the ISMAP flag for an image SUBMIT unless the NOCURSOR flag is set */
		    if (imh && (ii->flags & rid_if_NOCURSOR) == 0)
			f |= be_item_info_ISMAP;
		}
		break;
	    case rid_it_RESET:
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
	    imh = tio->object->state.plugin.pp;
	    f |= be_item_info_PLUGIN;
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

    if (flags)
	*flags = f;

    return NULL;
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

os_error *backend_doc_flush_image(be_doc doc, void *imh, int flags)
{
    rid_text_item *ti;
    os_error *ep = NULL;
    int ffi;
    rid_header *rh = doc->rh;

    ffi = (flags & be_openurl_flag_DEFER_IMAGES) ? image_find_flag_DEFER : 0;

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

		    i = (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE);

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

static os_error *backend__dispose_doc(be_doc doc)
{
    rid_text_item *ti;

    BEDBG((stderr, "Disposing of doc 0x%p, url is '%s'\n", doc, doc->url ? doc->url : "<none>"));

    alarm_removeall(doc);

#ifdef STBWEB
    /* check for ondispose URL action */
    if (doc->rh->refreshtime == -2)
	be_refresh_document(0, doc);
#endif

#if DEBUG  > 2
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
    }

    if (doc->ah)
    {
	BEDBG((stderr, "Calling access_abort on 0x%p\n", doc->ah));
	access_abort(doc->ah);
    }

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

    /* SJM free spacing list */
    layout_free_spacing_list(doc);

    doc->magic = 0;

    mm_free(doc);

    BEDBG((stderr, "Document disposed of\n"));

    return NULL;
}


#if DEBUG
static void dump_document_list(void)
{
    be_doc dd;
    fprintf(stderr, "dump doc: dump list\n");
    for (dd = document_list; dd; dd = dd->next)
    {
	fprintf(stderr, "dump doc: dd %p dd->next %p\n", dd, dd->next);
    }
    fprintf(stderr, "dump doc: end\n");
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
	    BEDBG((stderr, "dispose_doc: dd %p dd->next %p\n", dd, dd->next));
	    if (dd->next == doc)
	    {
		dd->next = doc->next;
		break;
	    }
	}
    }
}

static BOOL antweb_doc_in_list(be_doc doc)
{
    be_doc dd;
    for (dd = document_list; dd; dd = dd->next)
	if (dd == doc)
	    return TRUE;
    return FALSE;
}

os_error *backend_dispose_doc(be_doc doc)
{
#if DEBUG
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

    BEDBG((stderr, "dispose_doc: calling real dispose doc\n"));

    return backend__dispose_doc(doc);
}

static void be_undate_image_stata(be_doc doc, void *i)
{
    image_flags f;
    int dsf, ds;

    image_data_size((image) i, &f, &dsf, &ds);

    if (f & image_flag_FETCHED)
	doc->im_fetched++;
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
    int o_fd, o_fg, o_un, o_er, o_in, o_so;

    if (doc == NULL || doc->rh == NULL || doc->rh->stream.text_list == NULL)
	return;

    o_fd = doc->im_fetched;
    o_fg = doc->im_fetching;
    o_un = doc->im_unfetched;
    o_er = doc->im_error;
    o_in = doc->im_in_transit;
    o_so = doc->im_so_far;

    doc->im_fetched = 0;
    doc->im_fetching = 0;
    doc->im_unfetched = 0;
    doc->im_error = 0;
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
	(o_in != doc->im_in_transit) ||
	(o_so != doc->im_so_far) )
    {
	frontend_view_status(doc->parent, sb_status_IMAGE,
			     doc->im_fetched, doc->im_fetching, doc->im_unfetched,
			     doc->im_error, doc->im_so_far, doc->im_in_transit);
    }
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
    fvpr_progress_stream(&doc->rh->stream);
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

    if (update)
	return -1;

    do_tile = (doc->flags & doc_flag_DOC_COLOURS) &&
	(rh->bgt & rid_bgt_IMAGE) &&
	rh->tile.im &&
	(rh->tile.width != 0);

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

	return (int)doc;
    }

    if (do_fill)
    {
	int col;
	RENDBG(("Clearing background\n"));

	col = render_set_bg_colour(render_colour_BACK, doc);
	bbc_move(rr->g.x0, rr->g.y0);
	bbc_plot(bbc_RectangleFill + bbc_DrawAbsBack, rr->g.x1, rr->g.y1);

	return col | 1;
    }

    return -1;
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


    rh = doc->rh;

    /* ox and oy are the screen coordinates of the work area origin */
    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
#endif

    RENDBG(("Rendering rectangle.  ox=%d, oy=%d.\n", ox, oy));

    if (rh)
    {
	top = rr->g.y1 - oy;
	bot = rr->g.y0 - oy;

	RENDBG(("Top work area = %d, bottom = %d\n", top, bot));

	left = rr->g.x0;
	right = rr->g.x1;

	antweb_render_background(rr, h, update);

	fs.lf = fs.lfc = -1;

        /* Now into the recursive stream rendering function */

        stream_render(&doc->rh->stream, doc,
		      ox, oy,
		      left, top, right, bot,
		      &fs, &rr->g, update);
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

    if ((doc->flags & doc_flag_DISPLAYING) == 0)
        return;

    if (backend_doc_item_bbox(doc, ti, &box) == NULL)
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

#define LEEWAY 64
static void be_ensure_buffer_space(char **buffer, int *len, int more)
{
    int curlen = strlen(*buffer);

    if (curlen + more >= *len)
    {
	*len = *len + more + LEEWAY;
	*buffer = mm_realloc(*buffer, *len);
    }
}

static void antweb_append_query(char **buffer, char *name, char *value, int *len)
{
    /* Worst case the value will be three time its original size when escaped */
    if (value == NULL)
	value = "";

    be_ensure_buffer_space(buffer, len, (name ? strlen(name) : 0) + 3 * strlen(value) + 2);

    strcat(*buffer, "&");

    if (name)
    {
	int i;
	char *s;
	char c;

	s = *buffer + strlen(*buffer);
	for(i=0; (c = name[i]) != 0; i++)
	{
	    if (isspace(c))
		c = '+';
	    *s++ = c;
	}

	strcpy(s, "=");		/* Put termination on too */
    }
    url_escape_cat(*buffer, value, *len);
}

static void antweb_append_textarea(char **buffer, rid_textarea_item *tai, int *len)
{
    char *n;
    char *s;
    char c;
    rid_textarea_line *tal;

    if (tai->name == NULL)
	return;

    be_ensure_buffer_space(buffer, len, strlen(tai->name) + 2);

    strcat(*buffer, "&");

    s = *buffer + strlen(*buffer);
    for(n = tai->name; (c = *n) != 0; n++)
    {
	if (isspace(c))
	    c = '+';
	*s++ = c;
    }

    strcpy(s, "=");		/* Put termination on too */

    for(tal = tai->lines; tal; tal = tal->next)
    {
	be_ensure_buffer_space(buffer, len, 3 * strlen(tal->text) + 2);

	url_escape_cat(*buffer, tal->text, *len);
	if (tal->next)
	    strcat(*buffer, "%0d%0a");
    }
}

static void antweb_write_query(FILE *f, char *name, char *value, int *first)
{
    if (value == NULL)
	value = "";

    if (*first)
    {
	*first = FALSE;
    }
    else
    {
	fprintf(f, "&");
    }
    if (name)
    {
	char c;

	while ((c = *name++) != 0)
	{
	    if (isspace(c))
		c = '+';
	    fputc(c, f);
	}
	fputc('=', f);
    }
    url_escape_to_file(value, f);
}

static void antweb_write_textarea(FILE *f, rid_textarea_item *tai, int *first)
{
    char *n;
    char c;
    rid_textarea_line *tal;

    if (tai->name == NULL)
	return;

    if (*first)
    {
	*first = FALSE;
    }
    else
    {
	fprintf(f, "&");
    }

    for(n = tai->name; (c = *n) != 0; n++)
    {
	if (isspace(c))
	    c = '+';
	fputc(c, f);
    }
    fputc('=', f);

    for(tal = tai->lines; tal; tal = tal->next)
    {
	url_escape_to_file(tal->text, f);
	if (tal->next)
	    fputs("%0d%0a", f);
    }
}

BOOL backend_submit_form(be_doc doc, const char *id, int right)
{
    rid_form_item *form;

    for (form = doc->rh->form_list; form; form = form->next)
    {
	if (strcmp(form->id, id) == 0)
	{
	    antweb_submit_form(doc, form, right);
	    return TRUE;
	}
    }

    return FALSE;
}

/* The 'right' flag indicates a right click */

void antweb_submit_form(antweb_doc *doc, rid_form_item *form, int right)
{
    os_error *ep = NULL;
    rid_form_element *fis;
    char *target = right ? "_blank" : form->target;

    BEDBG((stderr, "Submit form: action='%s', method='%s'\n",
	    form->action ? form->action : "<none>",
	    (form->method == rid_fm_GET ?
	     "GET" :
	     (form->method == rid_fm_POST ?
	      "POST" :
	      "<unknown>") ) ));

    switch(form->method)
    {
    case rid_fm_GET:
        {
	    char *buffer;
	    char *dest, *dest2;
	    int buf_size = 1000;

	    dest = url_join(BASE(doc), form->action);

	    buffer = mm_malloc(buf_size);
	    buffer[0] = 0;
	    for (fis = form->kids; fis; fis = fis->next)
	    {
		switch (fis->tag)
		{
		case rid_form_element_INPUT:
		{
		    rid_input_item *iis = (rid_input_item *)fis;
		    switch (iis->tag)
		    {
		    case rid_it_HIDDEN:
			antweb_append_query(&buffer, iis->name, iis->value, &buf_size);
			break;
		    case rid_it_IMAGE:
			if ((iis->name != NULL) && (iis->data.image.x != -1))
			{
			    char buf2[12];
			    char buf3[128];

			    strcpy(buf3, strsafe(iis->name));

			    strcat(buf3, ".x");
			    sprintf(buf2, "%d", iis->data.image.x);
			    antweb_append_query(&buffer, buf3, buf2, &buf_size);

			    buf3[strlen(buf3)-1] = 'y';
			    sprintf(buf2, "%d", iis->data.image.y);
			    antweb_append_query(&buffer, buf3, buf2, &buf_size);
			}
			break;
		    case rid_it_TEXT:
		    case rid_it_PASSWD:
			antweb_append_query(&buffer, iis->name, iis->data.str, &buf_size);
			break;
		    case rid_it_CHECK:
		    case rid_it_RADIO:
			if (iis->data.radio.tick)
			{
			    antweb_append_query(&buffer, iis->name,
						iis->value ? iis->value : "on", &buf_size);
			}
			break;
		    case rid_it_SUBMIT:
			if (iis->data.button.tick && iis->name)
			{
			    antweb_append_query(&buffer, iis->name,
						iis->value ? iis->value : "", &buf_size);
			}
			break;
		    }
		    break;
		}

		case rid_form_element_SELECT:
		{
		    rid_select_item *sis = (rid_select_item *)fis;
		    rid_option_item *ois;
		    for(ois = sis->options; ois; ois = ois->next)
			if (ois->flags & rid_if_SELECTED)
			    antweb_append_query(&buffer, sis->name,
						ois->value ? ois->value : ois->text, &buf_size);
		    break;
		}

		case rid_form_element_TEXTAREA:
		{
		    rid_textarea_item *tai = (rid_textarea_item *)fis;
		    antweb_append_textarea(&buffer, tai, &buf_size);
		    break;
		}
		}
	    }
	    if (buffer[0] == 0)
		buffer[1] = 0;
	    buffer[0] = '?';
	    dest2 = url_join(dest, buffer);
	    mm_free(dest);
	    mm_free(buffer);

	    /* In theory the URL join can fail */
	    if (dest2)
	    {
	    BEDBG((stderr, "Query string is:\n'%s'\n", dest2));
		/* Never get a form query from the cache */
		ep = frontend_complain(frontend_open_url(dest2, doc->parent, target, NULL, 1));

		mm_free(dest2);
	    }
	    else
	    {
		frontend_complain(makeerror(ERR_BAD_FORM));
	    }
	}
	break;
    case rid_fm_POST:
        {
	    FILE *f;
	    char *fname;
	    int first = TRUE;
	    char *dest;

	    fname = strdup(tmpnam(0));
	    dest = strrchr(fname, 'x');
	    if (dest)
		*dest = 'y';
	    f = fopen(fname, "w");

	    for (fis = form->kids; fis; fis = fis->next)
	    {
		switch (fis->tag)
		{
		case rid_form_element_INPUT:
		{
		    rid_input_item *iis = (rid_input_item *)fis;
		    switch (iis->tag)
		    {
		    case rid_it_HIDDEN:
			antweb_write_query(f, iis->name, iis->value, &first);
			break;
		    case rid_it_IMAGE:
			if (iis->data.image.x != -1)
			{
			    char buf2[12];
			    char buf3[128];

			    strcpy(buf3, strsafe(iis->name));

			    strcat(buf3, ".x");
			    sprintf(buf2, "%d", iis->data.image.x);
			    antweb_write_query(f, buf3, buf2, &first);

			    buf3[strlen(buf3)-1] = 'y';
			    sprintf(buf2, "%d", iis->data.image.y);
			    antweb_write_query(f, buf3, buf2, &first);
			}
			break;
		    case rid_it_TEXT:
		    case rid_it_PASSWD:
			antweb_write_query(f, iis->name, iis->data.str, &first);
			break;
		    case rid_it_CHECK:
		    case rid_it_RADIO:
			if (iis->data.radio.tick)
			{
			    antweb_write_query(f, iis->name, iis->value ? iis->value : "on", &first);
			}
			break;
		    case rid_it_SUBMIT:
			if (iis->data.button.tick && iis->name)
			{
			    antweb_write_query(f, iis->name, iis->value ? iis->value : "", &first);
			}
			break;
		    }
		}
		break;

		case rid_form_element_SELECT:
		{
		    rid_select_item *sis = (rid_select_item *)fis;
		    rid_option_item *ois;
		    for(ois = sis->options; ois; ois = ois->next)
			if (ois->flags & rid_if_SELECTED)
			    antweb_write_query(f, sis->name, ois->value ? ois->value : ois->text, &first);
		    break;
		}

		case rid_form_element_TEXTAREA:
		{
		    rid_textarea_item *tai = (rid_textarea_item *)fis;
		    antweb_write_textarea(f, tai, &first);
		    break;
		}
		}
	    }

#if 0
	    fputc('\n', f);
#endif
	    fclose(f);

	    dest = url_join(BASE(doc), form->action);

	    /* Never get a form query from the cache */
	    ep = frontend_complain(frontend_open_url(dest, doc->parent, target, fname, 1));
	    mm_free(dest);
	}
	break;
    default:
	break;
    }

}

void antweb_place_caret(antweb_doc *doc, rid_text_item *ti)
{
    rid_text_item *old_ti = doc->input;
    int repos = object_caret_REPOSITION;

    doc->input = ti;		/* must set doc->input before calling the remove() function */

    if (old_ti != ti)
    {
	BEDBG((stderr, "antweb_place_caret(): input changed from %p to %p\n", old_ti, ti));

	repos = object_caret_FOCUS;

	if (old_ti && object_table[old_ti->tag].caret)
	    object_table[old_ti->tag].caret(old_ti, doc->rh, doc, object_caret_BLUR);
    }

    if (ti && object_table[ti->tag].caret)
    {
	(object_table[ti->tag].caret)(ti, doc->rh, doc, repos);
    }
    else
    {
	/* Give the window the input focus but no visable caret */
	frontend_view_caret(doc->parent, 0, 0, -1, 0);
    }
}

extern os_error *antweb_document_sizeitems(antweb_doc *doc)
{
    rid_text_item *ti;

    FMTDBG(("antweb_document_sizeitems start\n"));

    /* Pass one: size everything up */

    doc->im_error = doc->im_unfetched = doc->im_fetched = doc->im_fetching = 0;

    /* pdh: Doesn't entirely actually completely belong here, but we want
     * the background image to be fetched first, not last */
    if ((doc->rh->bgt & rid_bgt_IMAGE) && (doc->rh->tile.im == NULL))
    {
        be_doc_fetch_bg(doc);
    }

    ti = doc->rh->stream.text_list;

    /* First do each individual item */
    while (ti)
    {   /* Tables will recurse on child objects */
	(object_table[ti->tag].size)(ti, doc->rh, doc);

        /* might be no pos list, so no scanfr() */
	/*ti = ti->next;*/
        ti = rid_scanf(ti);
    }

    FMTDBG(("antweb_document_sizeitems done\n"));
    FMTDBG(("Images: %d waiting, %d fetching, %d fetched, %d errors.\n",
	    doc->im_unfetched, doc->im_fetching, doc->im_fetched, doc->im_error));

    return NULL;
}

static int antweb_formater_tidy_line(rid_pos_item *new, int width, int display_width)
{
    int spare;

    /* 24/9/96: Borris: Added new->first check */
    if (new == NULL || new->first == NULL)
    {
	usrtrc( "**** Tried to tidy NULL pos line ****\n");
	return 0;
    }

    spare = (display_width - width);

    switch (new->first->st.flags & rid_sf_ALIGN_MASK)
    {
    case rid_sf_ALIGN_JUSTIFY:
	if (new->first->width != -1 && spare > 0)
	{
	    rid_text_item *ti, *lti;
	    int n;

	    for(n=0, lti = NULL, ti = new->first; ti; ti = ti->next)
	    {
		FMTDBG(("Item=%p, pos=%p, line=%p, flags=0x%02x\n",
			ti, new, ti->line, ti->flag));

		if (((ti->flag & rid_flag_CLEARING) != 0) ||
		    ((ti->flag & (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0) )
		{
		    /* Not a floater */
		    if (ti->line != new)
			break;
		    n++;
		    lti = ti;
		}
	    }

	    FMTDBG(("Spare=%d, n=%d\n", spare, n));

	    /* Have more than one word on the line and not the end of the paragraph */
	    if ((n > 1) &&
		((lti->flag & (rid_flag_CLEARING | rid_flag_LINE_BREAK)) == 0) &&
		ti != NULL )
	    {
		new->leading = spare / (n-1);

		FMTDBG(("Leading=%d\n", new->leading));
	    }
	}
	break;
    case rid_sf_ALIGN_CENTER:
	if (new->first->width != -1 && spare > 0)
	{
	    /* Only center things that can fit on the display */

	    FMTDBGN(("center, %d %d\n", width, display_width));

	    if (width < display_width)
		new->left_margin += (spare >> 1);
	}
	break;
    case rid_sf_ALIGN_RIGHT:
	if (new->first->width != -1 && spare > 0)
	{
	    FMTDBGN(("Right aligned\n"));

	    new->left_margin += spare;
	}
    }

    return (new->max_up + new->max_down);
}


static int be_margin_proc (rid_text_stream *stream, rid_text_item *item)
{
        return item == NULL ? 0 :
                item->st.indent * INDENT_UNIT;	/* NvS We no longer as a left gap */
}

static void be_float_connect(rid_float_tmp_info *flt, rid_pos_item *pos, int flags)
{
    flt->h = flt->ti->max_up + flt->ti->max_down;
    if ( (flags & rid_fmt_BUILD_POS) != 0 )
    {
	/* Make new float item */
	flt->fi = mm_calloc(1, sizeof(*flt->fi));

	/* Fill in the float item */
	flt->fi->ti = flt->ti;
	flt->fi->pi = pos;
	flt->fi->height = flt->h;

	/* Attach floating item to first pos line */
	flt->ti->line = pos;

	if (flt->fi->ti->tag == rid_tag_TABLE)
	{
	    FMTDBG(("be_float_connect() adjusting height of floating table\n"));

	}

    }
    flt->used = 1;
}

static void be_float_tidy(rid_float_tmp_info *flt, int max_up, int max_down)
{
    if (flt->ti && flt->used)
    {
	flt->h -= (max_up + max_down);
	if (flt->h <= 0)
	{
	    flt->h = 0;
	    flt->ti = NULL;
	    flt->fi = NULL;
	}
    }
}

/*****************************************************************************

  The rid_pos_item for floating items might not refer to the line which
  ties up with the item's next pointer. To permit rid_scanb to work, those
  items that can float store a previous pointer. This routine initialises
  them (bluntly).

  */

static void fudge_prev_for_floating_items(rid_text_stream *st)
{
    rid_text_item *ti, *lastti = NULL;

    FMTDBGN(("fudge_prev_for_floating_items(%p)\n", st));

    for (ti = st->text_list; ti; ti = ti->next)
    {
	switch (ti->tag)
	{
	case rid_tag_TABLE:
	{
	    rid_text_item_table *tit = (rid_text_item_table *)ti;
	    tit->table->prev = lastti;
	}
	break;

	case rid_tag_IMAGE:
	{
	    rid_text_item_image *tii = (rid_text_item_image *)ti;
	    tii->prev = lastti;
	}
	break;
	}

	lastti = ti;
    }

    return;
}

/*****************************************************************************

    The new formatting routine. Why? It replaces two seperate routines with one
    and replaces multiple occurences of code sequences with one each. And it
    handles the ALIGN=CHAR stuff as well. There are still loose threads (eg
    widest and fmt->width).

    The margin_proc returns the offset from the left hand margin for the
    line.  This excludes any ALIGN=CHAR bias. A margin_proc must be supplied.

    The tidy_proc finishes the line.  It returns the height of the line.  It
    centers or right aligns any lines that need aligning.  It might also
    perform justification through sharing spare width (ie display_width-width).
    A tidy_proc must be supplied.

    The table_proc recurses on tables before they are examined.  It does not
    return anything.  A table_proc need not be supplied (ie use NULL).

    When alignment is active, the first occurence of the alignment character
    on each line must be vertically aligned.

    Items can mark themselves as never, maybe or always being followed by a
    line break.  We refuse a break for the first item on the line. We attempt
    to break if we are about to overflow the available space. This gives a
    basic line structure of:

    margin word ( pad word ) *

    The basic approach to formatting is

    a) Start a new line. This includes obtaining the margin indent
    value.
    b) Repeatedly add words to this line until reason is found to
    start a new line
    c) Tidy the line (eg performing centering)
    d) Repeat from  a)

    The need to split the line is resolved into MUST or DONT. A word
    can indicate that a break must occur (rid_flag_LINE_BREAK) or that
    there must *not* be a break (rid_flag_NO_BREAK). IF the word does
    not indicate either of these, then the width of the word is added
    to the current line width and standard RHS overflow formatting
    performed. This means when a new line is started, we have the word
    in hand that will start it.

    Adding correct support for rid_flag_NO_BREAK changes things as
    follows (note we view this a rare thing):

    @@@@

    A "word" can now cover multiple rid_text_items, so two pointers
    are required, one for each end - last identifies the last word
    start point: ie the start of the current word. It may be NULL due
    to floating stuff, in which case this_item (which identifies the
    RH end) should be used instead. When a word needs moving on to the
    next line, everything from last onwards is moved. This requires
    updating previously entered pos pointers (done this way around for
    less burden on typical route).

    The current code will not cater for joining rid_flag_NO_BREAK
    items together when they are floating. This is deemed "not worth
    it" at present (15/8/96).

*/

#define DONT    0
#define MUST    1
#define MAYBE   2

#define DONE    0
#define MARGIN  1
#define WORD    2
#define TIDY    3
#define	CLEAR_L	4
#define CLEAR_R	5
#define CLEAR	6
#define ENDST	7

#if DEBUG
static char *split_names[] = { "DONT", "MUST", "MAYBE" };
static char *state_names[] = { "DONE", "MARGIN", "WORD", "TIDY",
			       "CLEAR_L", "CLEAR_R", "CLEAR", "ENDST" };
#endif

extern rid_pos_item *be_formater_loop_core( rid_text_stream *st, rid_text_item *this_item, rid_fmt_info *fmt, int flags)
{
    rid_pos_item *pos = NULL;
    rid_text_item *line_first;	/* Like pos->first but always there */
    rid_text_item *last = NULL;	/* Last item in multi-item-word */
    int split = DONT;		/* Whether must, must not or maybe split word */
    int pad = 0;		/* holds padding from previous item */
    int align_margin = 0;	/* extra margin space due to character alignment */
    int left_margin = 0;	/* value for pos->left_margin if doing it */
    int width = 0;		/* total width of line so far */
    int state = MARGIN;		/* state machine control variable */
    int height = st->height;
    int widest = st->widest;
    int display_width = st->fwidth;
    rid_float_tmp_info fl, fr;
    int max_up=0, max_down=0;	/* Needed for float formatting even if there is no pos list */
    int prev_flags;

    line_first = NULL;

    memset(&fl, 0, sizeof(fl));
    memset(&fr, 0, sizeof(fr));

    FMTDBG(("be_formater_loop_core(%p, %p, %p, %x)\n", st, this_item, fmt, flags));

#if DEBUG && 0
    if (flags & rid_fmt_MIN_WIDTH  )
    {
	FMTDBG(("rid_fmt_MIN_WIDTH  \n"));
    }

    if (flags & rid_fmt_MAX_WIDTH  )
    {
	FMTDBG(("rid_fmt_MAX_WIDTH  \n"));
    }

    if (flags & rid_fmt_BUILD_POS  )
    {
	FMTDBG(("rid_fmt_BUILD_POS  \n"));
    }

    if (flags & rid_fmt_CHAR_ALIGN )
    {
	FMTDBG(("rid_fmt_CHAR_ALIGN \n"));
    }

    if (flags & rid_fmt_HAD_ALIGN  )
    {
	FMTDBG(("rid_fmt_HAD_ALIGN  \n"));
    }
#endif

    FMTDBG(("Entry: left %d, right %d, width %d or %d\n",
	     *fmt->left, *fmt->right, *fmt->width, display_width));

/*dump_stream(st, NULL );*/

    /* If there was an old pos list and the end item had floats we may need to take them on */
    if (st->pos_last && st->pos_last->floats)
    {
	int h;
	rid_floats_link *flink = st->pos_last->floats;

	FMTDBG(("be_formater_loop_core() handling previous floats\n"));

	if (flink->left)
	{
	    h = flink->left->ti->max_up + flink->left->ti->max_down;
	    h -= (flink->left->pi->top - height);
	    if (h > 0)
	    {
		fl.h = h;
		fl.used = 1;
		fl.fi = flink->left;
		fl.ti = flink->left->ti;
	    }
	}

	if (flink->right)
	{
	    h = flink->right->ti->max_up + flink->right->ti->max_down;
	    h -= (flink->right->pi->top - height);
	    if (h > 0)
	    {
		fr.h = h;
		fr.used = 1;
		fr.fi = flink->right;
		fr.ti = flink->right->ti;
	    }
	}
    }

    if (this_item != NULL && this_item->tag == rid_tag_TABLE)
    {
	FMTDBG(("Initial table proc call\n"));
	(*fmt->table_proc) (st, this_item, fmt);
	FMTDBG(("Initial table proc call finished\n"));
    }

    while (state != DONE)
    {
	FMTDBGN(("format state: %s, split %s, pos=%p, this_item=%p, width %d\n",\
		 state_names[state], split_names[split], pos, this_item, width));

#if DEBUG && 0
	dump_item(this_item, NULL);
#endif

	switch (state)
	{
	case MARGIN:
	    flags &= ~ rid_fmt_HAD_ALIGN;
	    align_margin = pad = 0;
	    split = DONT;
	    state = (this_item == NULL) ? ((fr.ti == NULL && fl.ti == NULL) ? DONE : CLEAR) : WORD;
	    left_margin = state == DONE ? 0 : (*fmt->margin_proc) (st, this_item);
	    width = left_margin;
	    if ( (flags & rid_fmt_BUILD_POS) != 0 )
	    {
		pos = mm_calloc(1, sizeof(*pos));
		rid_pos_item_connect(st, pos);
#if 0
		if (last != NULL && last != this_item)
		{
		    /* Start line at the unbreakable start */
		    /* Move group of words to this pos item */
		    pos->first = last;
		    for (; last != this_item; last = last->next)
			last->line = pos;
		    if (this_item != NULL)
		    {
			ASSERT(last->line == this_item->line);
		    }
		}
		else
#endif
		{
		    pos->first = this_item;
		}
		pos->top = height;
		pos->st = st;
	    }
	    last = line_first = this_item;
	    max_up = max_down = 0;
	    /* Deal with float left not yet used */
	    if (fl.ti && (fl.used == 0))
	    {
		FMTDBGN(("Connecting left float\n"));
		be_float_connect(&fl, pos, flags);
	    }
	    /* Deal with float right not yet used */
	    if (fr.ti && (fr.used == 0))
	    {
		FMTDBGN(("Connecting right float\n"));
		be_float_connect(&fr, pos, flags);
	    }
	    /* This link is built even if it is not the first line the float is seen on */
	    if (((flags & rid_fmt_BUILD_POS)!=0) && (fl.fi || fr.fi))
	    {
		FMTDBGN(("Building floats link.\n"));
		pos->floats = mm_calloc(1, sizeof(rid_floats_link));
		pos->floats->left = fl.fi;
		pos->floats->right = fr.fi;
	    }

	    /* Work out the length of the line, accounting for the floats */
	    display_width = st->fwidth;
	    if (fl.ti)
	    {
		TASSERT(fl.ti->width >= 0);
		FMTDBGN(("Add left floater with width %d\n", fl.ti->width));
		width += fl.ti->width;
	    }
	    if (fr.ti)
	    {
		TASSERT(fr.ti->width >= 0);
		FMTDBGN(("Add right floater with width %d\n", fl.ti->width));
		width += fr.ti->width;
	    }

	    FMTDBGN(("state now %s, left_margin %d\n", state_names[state], left_margin));
	    break;

	case WORD:
	    /* Committed to adding this word to this line */

	    ASSERT(split == DONT);

	    if (((this_item->flag & rid_flag_LEFTWARDS) ||
		 (this_item->flag & rid_flag_RIGHTWARDS) ) &&
		((this_item->flag & rid_flag_CLEARING) == 0) )
	    {
		FMTDBGN(("Floating item %p, flags=0x%02x, ", this_item, this_item->flag));

		/* We connect it now to stop it getting lost.  It will be reconnected later */
		if ( (flags & rid_fmt_BUILD_POS) != 0 )
		{
		    this_item->line = pos;
		}

		/* A floating item, some special action needed */
		if (this_item->flag & rid_flag_LEFTWARDS)
		{
		    FMTDBGN(("LEFT, "));

		    /* Floating left, either add now, add on the next line or clear left */
		    if (fl.ti)
		    {
			FMTDBGN(("already left floater, clearing.\n"));

			state = CLEAR_L;
			break;
		    }
		    else
		    {
			FMTDBGN(("initing new left floater, "));
			fl.ti = this_item;
			fl.used = 0;

			if ((this_item == line_first) ||
			    (fr.ti == line_first && this_item == line_first->next))
			{
			    FMTDBGN(("connecting.\n"));
			    /* Nothing visable on the line so far so we can go on this line */
			    be_float_connect(&fl, pos, flags);
			    TASSERT(fl.ti->width >= 0);
			    width += fl.ti->width;
			}
			else
			{
			    FMTDBGN(("defering.\n"));
			}
		    }
		}
		else
		{
		    FMTDBGN(("RIGHT, "));
		    /* Floating right, either add now, add on the next line or clear right */
		    if (fr.ti)
		    {
			FMTDBGN(("already right floater, clearing.\n"));
			state = CLEAR_R;
			break;
		    }
		    else
		    {
			FMTDBGN(("initing new right floater, "));
			fr.ti = this_item;
			fr.used = 0;

			if ((this_item == line_first) ||
			    (fl.ti == line_first && this_item == line_first->next))
			{
			    FMTDBGN(("connecting.\n"));
			    /* Nothing visable on the line so far so we can go on this line */
			    be_float_connect(&fr, pos, flags);
			    TASSERT(fr.ti->width >= 0);
			    width += fr.ti->width;
			}
			else
			{
			    FMTDBGN(("defering.\n"));
			}
		    }
		}

		if ( (flags & rid_fmt_BUILD_POS) != 0 )
		{
		    if (pos->floats == NULL)
		    {
			FMTDBGN(("Making new floats link\n"));
			pos->floats = mm_calloc(1, sizeof(rid_floats_link));
		    }

		    FMTDBGN(("Connecting float items.\n"));
		    /* Exactly one of these is redundent but the operations are idempotent */
		    pos->floats->left = fl.fi;
		    pos->floats->right = fr.fi;
		}

		FMTDBGN(("Floating done.\n"));
	    }
	    else
	    {
		if ( (flags & rid_fmt_CHAR_ALIGN) != 0 && pad + width > *fmt->left)
		{
		    *fmt->left = pad + width;
		    FMTDBGN(("Increased left by %d to %d\n", \
			    align_margin, *fmt->left));
		}

		width += pad + align_margin;
		if (this_item->width >= 0)
		    width += this_item->width;
		else
		{
		    /* @@@@ decide if this needs further investigation */
		    FMTDBGN(("\nNOT adding in negative width %d\n\n", this_item->width));
		}

		left_margin += align_margin;
		pad = this_item->pad;
		align_margin = 0;
		/*split = MAYBE;*/

		/* Now we do floats we need this even if there is no pos list */
		if (this_item->max_up > max_up)       max_up = this_item->max_up;
		if (this_item->max_down > max_down)   max_down = this_item->max_down;

		if ( (flags & rid_fmt_BUILD_POS) != 0 )
		{
		    this_item->line = pos;
		}
	    }

	    if	    ( (this_item->flag & rid_flag_CLEARING) != 0 )       { split = MUST; }
	    else if ( (this_item->flag & rid_flag_LINE_BREAK) != 0 )     { split = MUST; }
	    else if ( (this_item->flag & rid_flag_NO_BREAK) != 0 )       { split = DONT; }
	    else if ( (flags & rid_fmt_MIN_WIDTH) != 0 )            { split = MUST; }
	    else if ( (flags & rid_fmt_MAX_WIDTH) != 0 )            { split = DONT; }
	    else							{ split = MAYBE;}

	    prev_flags = this_item->flag;

	    /* Now decide whether advancing last as well as this_item. */
	    /* This might need tweaking for a while! */
	    {
		rid_text_item *next = rid_scanf(this_item);
		BOOL over = TRUE; /* Default is to move last with this_item */
#if 0
		if (last != NULL)
		{
		    if ( (last->flag & rid_flag_NO_BREAK) != 0 )
		    {
			const rid_flag x = last->flag | (next == NULL ? 0 : next->flag);
			if ( (x & (rid_flag_CLEARING | rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0 )
			{
			    FMTDBG(("NO_BREAK retaining last as %p: not move to %p\n",
				    last, next));
			    over = FALSE;
			}
		    }
		}
#endif
		this_item = next;

		if (over)
		    last = next;
	    }

	    FMTDBGN(("DONT: added word: width %d, pad %d, next %p\n", width, pad, this_item));

	    if (this_item != NULL)
	    {
		if ( (flags & (rid_fmt_CHAR_ALIGN | rid_fmt_HAD_ALIGN)) == rid_fmt_CHAR_ALIGN &&
		     this_item->tag == rid_tag_TEXT &&
		     fmt->text_data[ ((rid_text_item_text *)this_item)->data_off] == fmt->align_char )
		{
		    FMTDBGN(("Character alignment triggered\n"));
		    flags |= rid_fmt_HAD_ALIGN;
		    if (pad + width < *fmt->left)
		    {
			align_margin = *fmt->left - width - pad;
			FMTDBGN(("%d padding due to character alignment\n",\
				align_margin));
		    }
		}
		else if (this_item->tag == rid_tag_TABLE)
		{
		    FMTDBGN(("Calling table proc\n"));
		    (*fmt->table_proc) (st, this_item, fmt);
		    FMTDBGN(("Called table proc\n"));
		}
	    }
	    else
	    {
		split = MUST;	/* because there is no following item */
	    }

	    if (split == MAYBE)
	    {
		if ( align_margin + width + pad +
		     (this_item->width > 0 ? this_item->width : 0) > display_width)
		{ split = MUST; }
		else
		{ split = DONT; }
		FMTDBGN(("Resolved MAYBE to %s split\n", \
			 split == DONT ? "DONT" : "MUST"));
	    }

	    if (this_item == NULL)
	    {
		state = ENDST;
	    }
	    else if (split == MUST)
	    {
		switch (prev_flags &
			(rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS | rid_flag_CLEARING))
		{
		case rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS | rid_flag_CLEARING:
		    state = CLEAR;
		    break;
		case rid_flag_LEFTWARDS | rid_flag_CLEARING:
		    state = CLEAR_L;
		    break;
		case rid_flag_RIGHTWARDS | rid_flag_CLEARING:
		    state = CLEAR_R;
		    break;
		default:
		    state = TIDY;
		    break;
		}
	    }
	    break;

	case TIDY:
	case CLEAR_L:
	case CLEAR_R:
	case CLEAR:
	case ENDST:
	    if ( (flags & rid_fmt_CHAR_ALIGN) != 0 )
	    {
		if ( (flags & rid_fmt_HAD_ALIGN) != 0 )
		{
		    if ( width - *fmt->left > *fmt->right )
			*fmt->right = width - *fmt->left;
		}
		else
		{
		    FMTDBG(("Align char %c not present - padding\n", fmt->align_char));
		    if (width > *fmt->left)
		    {
			*fmt->left = width;
			FMTDBG(("Increased left to %d\n", *fmt->left));
		    }
		    else
		    {
			left_margin += *fmt->left - width;
			FMTDBGN(("Increased left_margin by %d to %d\n",\
				*fmt->left - width, left_margin));
			width = *fmt->left;
		    }
		}
	    }

	    if (state == CLEAR_L || state == CLEAR || state == ENDST)
	    {
		if (fl.h > (max_up+max_down))
		{
		    /* Stretch max_down to take up any space */
		    max_down = fl.h - max_up;
		}
	    }

	    if (state == CLEAR_R || state == CLEAR || state == ENDST)
	    {
		if (fr.h > (max_up+max_down))
		{
		    /* Stretch max_down to take up any space */
		    max_down = fr.h - max_up;
		}
	    }

	    if ( (flags & rid_fmt_BUILD_POS) != 0 )
	    {
		pos->left_margin = left_margin;
		pos->max_up = max_up;
		pos->max_down = max_down;
		height -= (*fmt->tidy_proc) (pos, width, display_width);
	    }

	    /* These functions clear the floating items when they have run out of length */
	    be_float_tidy(&fl, max_up, max_down);
	    be_float_tidy(&fr, max_up, max_down);

	    if (width > widest)
		widest = width;
	    state = MARGIN;
	    break;
	}
    }

    if ( (flags & rid_fmt_CHAR_ALIGN) != 0 )
    {
	if (*fmt->left + *fmt->right > widest)
	    widest = *fmt->left + *fmt->right;
    }

    if (widest > *fmt->width)
	*fmt->width = widest;

    FMTDBG(("Exit:  left %d, right %d, width %d\n",
	     *fmt->left, *fmt->right, *fmt->width));

    if ( (flags & rid_fmt_BUILD_POS) != 0 )
    {
	st->widest = widest;
	st->height = st->pos_last->top;

	fudge_prev_for_floating_items(st);

	FMTDBG(("st->widest %d, st->height %d\n", st->widest, st->height));
    }

    return pos;
}

#undef DONT
#undef MUST
#undef MAYBE
#undef DONE
#undef MARGIN
#undef WORD
#undef TIDY
#undef CLEAR
#undef CLEAR_L
#undef CLEAR_R
#undef ENDST

/*****************************************************************************/

extern int  dummy_tidy_proc(rid_pos_item *new, int width, int display_width)
{
    FMTDBG(("dummy_tidy_proc()\n"));
    return 0;
}

extern void dummy_table_proc(rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt)
{
    FMTDBG(("dummy_table_proc()\n"));
    return;
}

#if 0
static void sizing_sharing_table_proc(
    rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt)
{
    FMTDBG(("sizing_sharing_table_proc(%p, %p, %p) entered\n", stream, item, parfmt));

    if (stream->partype == rid_pt_HEADER)
    {
	rid_fmt_info fmt;
	rid_header *rh = (rid_header *)stream->parent;

	FMTDBG(("sizing_sharing_table_proc(%p, %p, %p) sizing\n", stream, item, parfmt));

	memset(&fmt, 0, sizeof(fmt));

	fmt.margin_proc = &be_margin_proc;
	fmt.tidy_proc = &dummy_tidy_proc;
	fmt.table_proc = &dummy_table_proc;
	fmt.text_data = rh->texts.data;

	/* Then get min|max widths for tables */
	rid_size_stream(stream, &fmt, 0);
    }

    FMTDBG(("sizing_sharing_table_proc(%p, %p, %p) sharing\n", stream, item, parfmt));

    rid_table_share_width(stream, item, parfmt);

    FMTDBG(("sizing_sharing_table_proc(%p, %p, %p) finished\n", stream, item, parfmt));
}
#endif

#if DEBUG
static void stomp_contained_widths(rid_text_stream *st)
{
    rid_text_item *ti;

    FMTDBG(("stomp_contained_widths(%p) entered\n", st));

    for (ti = st->text_list; ti; ti = ti->next)
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_text_item_table *tit = (rid_text_item_table *) ti;
	    rid_table_item *table = tit->table;
	    int x, y;
	    rid_table_cell *cell;

	    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	    {
		cell->flags &= ~rid_cf_NOWRAP;
		cell->userwidth.type = value_none;
		cell->userheight.type = value_none;
		stomp_contained_widths(&cell->stream);
	    }

	    for (x = 0; x < table->cells.x; x++)
	    {
		table->colhdrs[x]->userwidth.type = value_none;
	    }

	    for (x = 0; x < table->num_groups.x; x++)
	    {
		table->colgroups[x]->userwidth.type = value_none;
	    }
	}
    }

    FMTDBG(("stomp_contained_widths() finished\n"));
}
#endif

static void be_formater_loop(rid_header *rh, rid_text_item *ti, int scale_value)
{
        rid_text_stream *st = &rh->stream;
        rid_fmt_info fmt;

        memset(&fmt, 0, sizeof(fmt));
        memset(&st->width_info, 0, sizeof(st->width_info));

        FMTDBG(("be_formater_loop(%p, %p, %d) - sizing\n", rh, ti, scale_value));

        fmt.margin_proc = &be_margin_proc;
        fmt.tidy_proc = &dummy_tidy_proc;
        fmt.table_proc = &dummy_table_proc;
        fmt.text_data = rh->texts.data;

        /* Then get min|max widths for tables */
        rid_size_stream(st, &fmt, 0, ti);

#if DEBUG
	fprintf(stderr, "Width after formatting is %d, versus fwidth %d\n",
		*fmt.width, rh->stream.fwidth);

	if (*fmt.width > rh->stream.fwidth)
	{
	    FMTDBG(("\n\nSTOMPING ALL WIDTHS TO TRY TO RESTRAIN OVERALL WIDTH\n\n"));

	    memset(&fmt, 0, sizeof(fmt));
	    memset(&st->width_info, 0, sizeof(st->width_info));

	    FMTDBG(("be_formater_loop(%p, %p, %d) - STOMPED sizing\n", rh, ti, scale_value));

	    fmt.margin_proc = &be_margin_proc;
	    fmt.tidy_proc = &dummy_tidy_proc;
	    fmt.table_proc = &dummy_table_proc;
	    fmt.text_data = rh->texts.data;

	    stomp_contained_widths(&rh->stream);

	    /* Then get min|max widths for tables */
	    rid_size_stream(st, &fmt, 0, ti);
	}

#endif

        /* Do the actual format */

        FMTDBG(("be_formater_loop - building\n"));

        fmt.margin_proc = &be_margin_proc;
        fmt.tidy_proc = &antweb_formater_tidy_line;
        fmt.table_proc = &rid_table_share_width;
        fmt.text_data = rh->texts.data;
	fmt.scale_value = scale_value;

        be_formater_loop_core(st, ti, &fmt, rid_fmt_BUILD_POS);

        FMTDBG(("be_formater_loop done\n"));

#if DEBUG > 2
	fprintf(stderr, "\n\nrid_header after more formatting\n\n");
        dump_header(rh);
#endif
}


os_error *antweb_document_format(antweb_doc *doc, int user_width)
{
    rid_text_item *ti;

    FMTDBG(("antweb_document_format() entered\n"));

    BEDBG((stderr, "Document title: '%s' \ndoc %p, rid_header %p\n",
	    doc->rh->title ? doc->rh->title : "<none>", doc, doc->rh));

    /* Include tables in free of pos items */
    rid_free_pos_tree(doc->rh->stream.pos_list);
    doc->rh->stream.pos_list = doc->rh->stream.pos_last = NULL;

    ti = doc->rh->stream.text_list;
    if (ti == NULL)
	return NULL;

    /* Zero w|h for all table descendent streams as well */
    rid_zero_widest_height(&doc->rh->stream);

    be_formater_loop(doc->rh, ti, doc->scale_value);

    fvpr_progress_stream(&doc->rh->stream);

    objects_check_movement(doc);

    
#if DEBUG
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
    rid_text_item *ti;
    rid_pos_item *new;
    wimp_box bb;

    FMTDBG(("be_document_reformat_tail(%p,%p,%d)\n", doc, oti,user_width));

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
	else
#endif
	    ti = oti->line->first;
    }
    else
	ti = doc->rh->stream.text_list;

    if (ti == NULL)
	return;

    FMTDBG(("Line start item is %p\n", ti));

    new = ti->line;

    FMTDBG(("Line pos item 'new' is %p\n", new));

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

    be_formater_loop(doc->rh, ti, doc->scale_value);

    fvpr_progress_stream(&doc->rh->stream);

    FMTDBG(("be_formater_loop() done\n"));

    bb.x1 = doc->rh->stream.widest > user_width ? doc->rh->stream.widest : user_width;
    bb.y0 = doc->rh->stream.height;

    if (doc->parent && (doc->flags & doc_flag_DISPLAYING))
	be_view_redraw(doc, &bb);
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

        layout_layout(doc, fvd.layout_width, fvd.layout_height, 1);

        doc->rh->stream.widest = fvd.wa_width;
        doc->rh->stream.height = fvd.wa_height;
#if 0
        doc->rh->stream.widest -= doc->margin.x0 - doc->margin.x1;
        doc->rh->stream.height -= doc->margin.y0 - doc->margin.y1;
#endif
    }
    else
    {
        int old_user_width = doc->rh->stream.fwidth;
        frontend_view_get_dimensions(doc->parent, &fvd);

        /* only reformat if the width has actually changed */
        if (old_user_width != fvd.user_width)
        {
            doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
	    doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif
            antweb_document_format(doc, doc->rh->stream.fwidth);

            be_set_dimensions(doc);
        }
    }

    if (frontend_view_has_caret(doc->parent))
        antweb_place_caret(doc, doc->input);

    return NULL;
}

os_error *backend_goto_fragment(be_doc doc, char *frag)
{
    rid_aref_item *ai;

    if (doc == NULL || doc->rh == NULL)
	return makeerror(ERR_BAD_DOCUMENT);

    BEDBG((stderr, "Going to fragment '%s'\n", frag ? frag : "<none>"));
    if (frag)
    {
        rid_text_item *first;

	ai = doc->rh->aref_list;

	while (ai)
	{
	    if (ai->name && strcasecomp(ai->name, frag) == 0)
		break;
	    ai = ai->next;
	}

	if (ai == NULL)
	    return doc->ah || doc->ph ? NULL : makeerror(ERR_NO_SUCH_FRAG); /* only give error if document fully downloaded */

        first = ai->first ? ai->first : doc->rh->stream.text_list;
	if (first)
	{
	    wimp_box bb;

	    if (backend_doc_item_bbox(doc, first, &bb) == 0)
	    {
		BEDBG((stderr, "Ensuring item visable, top at %d\n", bb.y1));
		/* doesn't need margin adjustment, done in doc_item_bbox */
		frontend_view_ensure_visable(doc->parent, 0, bb.y1, bb.y1);
	    }
    	}
    }

    return NULL;
}

static void be_update_image_size(antweb_doc *doc, void *i)
{
    rid_text_item *ti;

    for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if ((object_table[ti->tag].imh != NULL) &&
	    ((object_table[ti->tag].imh)(ti, doc, object_image_HANDLE) == i) )
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


void antweb_doc_image_change(void *h, void *i, int status, wimp_box *box_update)
{
    antweb_doc *doc = (antweb_doc *) h;
    int changed = FALSE;
    rid_pos_item *pi;
    rid_pos_item *clonedposlist = NULL;
    rid_text_item *first_ti;
    rid_text_item *ti;
    int top, bottom, line_top = 0;
    wimp_box bounds;

    DICDBG(("Image change called, doc=0x%p, status %d, image 0x%p\n", doc, status, i));

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
	be_update_image_info(doc);
	break;
    }

    if ((doc->flags & doc_flag_DISPLAYING) == 0)
    {
	be_update_image_size(doc, i);
	return;
    }

    frontend_view_bounds(doc->parent, &bounds);

    top = bounds.y1;
    bottom = bounds.y0;

    if (doc->rh->stream.pos_list == NULL)	/* If the image is on disc then we can be called before the first format */
    {
	DICDBG(("Image change quit early\n"));

	return;
    }

    if (status == image_cb_status_REFORMAT)
    {
	clonedposlist = rid_clone_pos_list(doc->rh->stream.pos_list);

/* 	for (pi = clonedposlist; pi->next && pi->next->top > top; pi = pi->next) */
/* 	    ; */
	/* SJM: the above is not good enough with floating images around */
	pi = clonedposlist;

	first_ti = pi->first;		/* This is the first displayed item in the view */
	line_top = pi->top;

	ti = doc->rh->stream.text_list;

	DICDBG(("Starting rescan\n"));

	/* Need to scan recursively to actually find the images, but use */
	/* the outermost item when looking for redraw sizes, etc. */
	/* Pretty gruesome really. Go for region covered by table */
	/* and image, just in case. */

	while (ti)
	{
	    if (object_table[ti->tag].imh != NULL && (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE) == i)
	    {
		int ow, omu, omd;

		ow = ti->width;
		omu = ti->max_up;
		omd = ti->max_down;

		(object_table[ti->tag].size)(ti, doc->rh, doc);

		if (ow != ti->width || omu != ti->max_up || omd != ti->max_down)
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

	first_ti = pi->first;		/* This is the first displayed item in the view */
	line_top = pi->top;

	/* ... but we set changed=0 os it's OK */
	changed = 0;
    }

    if (changed)
    {
	int old_height;
	int net_shift, shift_pending;
	int top_of_zone;
	int is_shift_pending;
	rid_pos_item *opi;
	int new_bottom;
	wimp_box box;

	/* If we have the caret, hide it for the moment */
	if (frontend_view_has_caret(doc->parent))
	{
	    frontend_view_caret(doc->parent, 0, 0, -1, 0);
	}

	box = bounds;		/* We want x0 and x1 at the least */

	DICDBG(("Changed = %d\n", changed));

	old_height = doc->rh->stream.height;

	DICDBG(("About to reformat\n"));

	antweb_document_format(doc, doc->rh->stream.fwidth);

	DICDBG(("Reformat done\n"));

	if (doc->rh->stream.height != old_height)
	{
	    DICDBG(("Old height was %d, new height is %d\n",
		    doc->rh->stream.height, old_height));

	    be_set_dimensions(doc);
	}

	DICDBG(("Checking what has changed\n"));

	if (changed & 3)
	{
	    net_shift = 0;
	    is_shift_pending = TRUE;
	    shift_pending = first_ti->line->top - line_top;
	    top_of_zone = first_ti->line->top; /* Always in new format coordinates */

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
		    break;

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
			  rid_table_holds_image(tiscan, i, doc) :
			  ((object_table[tiscan->tag].imh != NULL) &&
			   (object_table[tiscan->tag].imh)(tiscan, doc, object_image_HANDLE) == i)) ||
			 (opi->first->line->floats->right != 0) &&
			 ((tiscan = opi->first->line->floats->right->ti) != 0) &&
			 (tiscan->tag == rid_tag_TABLE ?
			  rid_table_holds_image(tiscan, i, doc) :
			  ((object_table[tiscan->tag].imh != NULL) &&
			   (object_table[tiscan->tag].imh)(tiscan, doc, object_image_HANDLE) == i) ) ) )
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
                                if (rid_table_holds_image(tiscan, i, doc))
                                {
				    have_image = TRUE;
				    break;
                                }
			    }
			    else if (object_table[tiscan->tag].imh != NULL && (object_table[tiscan->tag].imh)(tiscan, doc, object_image_HANDLE) == i)
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

			DICDBG(("Force redraw from %d to %d\n", box.y1, box.y0));

			be_view_redraw(doc, &box);
			/*	set shift pending to shift needed for this line minus net shift so far */
			top_of_zone = opi->first->line->top;
			shift_pending = (opi->first->line->top - opi->top) - net_shift;
			is_shift_pending = TRUE;

			DICDBG(("Set shift of %d pending from %d down\n", shift_pending, top_of_zone));

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

	    DICDBG(("Finished scanning viable range\n"));

	    /* if shift pending: */
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
	    /* else: */
	    else
	    {
		/* 	redraw from top of dirty to bottom of view */
		box.y1 = top_of_zone;
		box.y0 = new_bottom;

		DICDBG(("Force redraw from %d to %d\n", box.y1, box.y0));

		be_view_redraw(doc, &box);
	    }
	}

	if (frontend_view_has_caret(doc->parent))
	{
	    antweb_place_caret(doc, doc->input);
	}
    }
    else
    {
	int flags;
	int do_redraw;
	int real_thing_flag;
	int imw, imh;

	/* SJM: changes to use frame_info to get right mask value */
#if 0
	wimp_box bbox;
	image_info_frame((image)i, &bbox, NULL, &flags);
#else
	image_info((image)i, &imw, &imh, NULL, &flags, NULL, NULL);
#endif

	real_thing_flag = (flags & image_flag_REALTHING) ? rid_image_flag_REAL : 0;

	/* If told to UPDATE when we don't need a full redraw */
	do_redraw = (status == image_cb_status_UPDATE || status == image_cb_status_UPDATE_ANIM) ? 0 : 1;

	DICDBGN(("Updating images with no size change.  Top=%d, borrom=%d, redraw = %d\n",
		top, bottom, do_redraw));

	/* The new image must be the same size.  Redraw the image when on the screen. 	*/
        /* This recurses through tables as well - should work okay */
/* 	for (ti = first_ti; ti && (ti->line->top > bottom); ti = rid_scanfr(ti)) */
	for (ti = first_ti; ti; ti = rid_scanfr(ti))
	{
	    void *imhandle = object_table[ti->tag].imh != NULL ? (object_table[ti->tag].imh)(ti, doc, object_image_HANDLE) : NULL;

	    DICDBGN(("ti=%p, line=%p, top=%d, imh=0x%p\n", ti, ti->line, ti->line->top, imhandle));

	    if (imhandle == i)
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

	BEDBG((stderr, "antweb_init_page(): doc %p\n", doc));

	frontend_view_get_dimensions(doc->parent, &fvd);

	doc->rh->stream.fwidth = fvd.user_width;
#if USE_MARGINS
	doc->rh->stream.fwidth -= doc->margin.x0 - doc->margin.x1;
#endif

	antweb_document_format(doc, doc->rh->stream.fwidth);

	BEDBG((stderr, "Calling visit\n"));

	/* We assume we can use the title here because it is supposed to be
	   in the header and the header is supposed to come before the body. */

	be_view_visit(doc);

	doc->flags |= doc_flag_DISPLAYING;

	be_update_image_info(doc);

	if (frontend_view_has_caret(doc->parent))
	    antweb_place_caret(doc, NULL);
    }
}


static void antweb_doc_background_change(void *h, void *i, int status, wimp_box *box)
{
#ifndef BUILDERS
    int width, height;
    int fl;
    antweb_doc *doc = (antweb_doc *) h;

    BEDBG((stderr, "antweb_doc_background_change(): doc %p status %d\n", doc, status));

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
	    BEDBG((stderr, "Setting background colour\n"));

	    doc->rh->bgt |= rid_bgt_COLOURS;
	    doc->rh->colours.back = image_average_colour(i);

	    BEDBG((stderr, "New bg colour is 0x%08x\n", doc->rh->colours.back));
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

    BEDBG((stderr, "Using parser %d for file type 0x%03x\n", i, ft));

    return &file_parsers[i];
}

static void be_pparse_doc(antweb_doc *doc, int fh, int from, int to)
{
    char buffer[4096];
    os_gbpbstr gpb;
    os_error *ep;
    int len;
    rid_text_item *ti;
    rid_header *rh;

    if (doc->rh == NULL)
	doc->rh = (((pparse_details*)doc->pd)->rh)(doc->ph);

    rh = doc->rh;

    ti = rh->stream.text_last;
    PPDBG((stderr, "Old last item is 0x%p\n", ti));
    while (from < to)
    {
	len = (to-from) > sizeof(buffer) ? sizeof(buffer) : (to-from);

	gpb.action = 3;
	gpb.file_handle = fh;
	gpb.data_addr = buffer;
	gpb.number = len;
	gpb.seq_point = from;
	PPDBG((stderr, "Reading %d bytes from file %d.\n", len, fh));
	ep = os_gbpb(&gpb);
	if (ep == NULL)
	{
	    PPDBG((stderr, "Sending %d bytes to the parser.\n", len));
	    (((pparse_details*)doc->pd)->data)(doc->ph, buffer, len, (from + len) < to );
	}
	else
	{
	    PPDBG((stderr, "Getting file data returned error: %s\n", ep->errmess));
	    PPDBG((stderr, "In: file=%d, buffer=%p, number=%d, seq=%d\n",
		    fh, buffer, len, from));
	    PPDBG((stderr, "Out: file=%d, buffer=%p, number=%d, seq=%d\n",
		    fh, buffer, len, from));
	}

	from += len;
    }

#if USE_MARGINS
#if PP_DEBUG
    if ((rh->margin.left != -1 && rh->margin.left*2 != doc->margin.x0) ||
	(rh->margin.top != -1 && rh->margin.top*2 != doc->margin.y1))
	PPDBG((stderr, "setting margins to %d,%d\n", doc->margin.x0, doc->margin.y1));
#endif
    if (rh->margin.left != -1)
	doc->margin.x0 = rh->margin.left*2;
    if (rh->margin.top != -1)
	doc->margin.y1 = -rh->margin.top*2;
#endif

    if (ti == NULL)
    {
	ti = rh->stream.text_list;
	PPDBG((stderr, "No old value, first item is at 0x%p\n", ti));
    }
    else
    {
        /* Inefficient but correct - and easy! */
        if (ti->tag != rid_tag_TABLE && ti->tag != rid_tag_TEXTAREA)/* SJM: 240196: quick hack to add texteareas here */
            ti = rid_scanf(ti);
	PPDBG((stderr, "Moved on from last item to 0x%p\n", ti));
    }
    PPDBG((stderr, "Sizing objects from 0x%p\n", ti));
    while (ti)
    {
	(object_table[ti->tag].size)(ti, rh, doc);

        ti = rid_scanf(ti);
    }


    PPDBG((stderr, "Done sizing\n"));
}

static void be_doc_fetch_bg(antweb_doc *doc)
{
    char *url;

    BEDBG((stderr, "be_doc_fetch_bg(): doc %p\n", doc));

    url = url_join(BASE(doc), doc->rh->tile.src);

    image_find(url, BASE(doc), doc->flags & doc_flag_FROM_HISTORY ? 0 : image_find_flag_CHECK_EXPIRE,
	       &antweb_doc_background_change, doc, render_get_colour(render_colour_BACK, doc),
	       (image*) &(doc->rh->tile.im));

    antweb_doc_background_change(doc, doc->rh->tile.im, image_cb_status_UPDATE, NULL);

    mm_free(url);
}


/* This will tend to redraw tables as they are arriving. */
/* It should be okay once the </TABLE> has been processed. */

static void antweb_doc_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    antweb_doc *doc = (antweb_doc *) h;
    static antweb_doc *threaded = NULL;

    /* Make the world turn a little */
    frontend_view_status(doc->parent, sb_status_WORLD);

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

    if (so_far > 0 && ftype != -1 && fh && status == status_GETTING_BODY)
    {
	int lastptr = doc->lbytes;

	if (doc->lstatus != status_GETTING_BODY || lastptr == -1)
	    lastptr = 0;

	PPDBG((stderr, "Data arriving; type = 0x%03x, file=%d, last had %d, now got %d\n",
		ftype, fh, lastptr, so_far));

	if (doc->pd == NULL)
	    doc->pd = be_lookup_parser(ftype);

	if (doc->ph == NULL && ((pparse_details*)doc->pd)->new)
	{
	    PPDBG((stderr, "About to make a new parser stream\n"));
	    doc->ph = (((pparse_details*)doc->pd)->new)(url, ftype);
	    if (doc->ph)
	    {
		/* new: check if URL has changed - since we set the URL on entry now */
		if (doc->url == NULL || strcmp(doc->url, url) != 0)
		{
		    mm_free(doc->url);
		    doc->url = strdup(url);
		}

		doc->rh = (((pparse_details*)doc->pd)->rh)(doc->ph);

		if (doc->rh->stream.text_last)
		{
		    PPDBG((stderr, "Sizing the first few items...\n"));
		    antweb_document_sizeitems(doc);
		    PPDBG((stderr, "... done\n"));
		}
	    }
	}

	if (doc->ph)
	{
	    rid_text_item *oti;
	    rid_form_item *ofi;
	    rid_select_item *osi;
	    rid_option_item *ooi;

	    oti = doc->rh->stream.text_last;
	    ofi = doc->rh->form_last;
	    osi = ofi ? ofi->last_select : NULL;
	    ooi = osi ? osi->last_option : NULL;

	    PPDBG((stderr, "Calling pparse from progress\n"));
	    be_pparse_doc(doc, fh, lastptr, so_far);

	    PPDBG((stderr, "pparse from progress done\n"));

                    /* pdh: only do this if it's not already been done */
		    if ( (doc->rh->bgt & rid_bgt_IMAGE)
		         && (doc->rh->tile.im == NULL) )
		    {
			be_doc_fetch_bg(doc);
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
		PPDBG((stderr, "Text list seems to have changed.\n"));

		if ((doc->flags & doc_flag_DISPLAYING) == 0)
		{
		    BOOL waiting_for_bg = FALSE;

		    /* if we have an image and it isn't fetching then start fetching it*/
		    if ((doc->rh->bgt & rid_bgt_IMAGE) && doc->rh->tile.im == NULL)
		    {
			be_doc_fetch_bg(doc);
			doc->start_time = clock();
		    }
#if 1
		    /* if we have an image possibly fetching see if it is here */
		    if (doc->rh->tile.im)
		    {
			image_flags flags = 0;
			image_info(doc->rh->tile.im, NULL, NULL, NULL, &flags, NULL, NULL);
			if (flags & image_flag_FETCHED)
			{
			    BEDBG((stderr, "background is here already\n"));
			}
			else if (clock() < doc->start_time + config_display_time_background)
			{
			    waiting_for_bg = TRUE;
			    BEDBG((stderr, "waiting for background\n"));
			}
			else
			{
			    BEDBG((stderr, "background wait timeout\n"));
			}
		    }
#endif

		    if (!waiting_for_bg)
			antweb_init_page(doc);
		}
		else
		{
		    BEDBG((stderr, "Tail changed, checking for select items\n"));
		    BEDBG((stderr, "ofi=%p, osi=%p, ooi=%p\n", ofi, osi, ooi));

		    /* We had an option in the last visable select and
		       either the last form no longer has a select or
		       the last select of the last form is not the same */
		    if ((ooi || (osi && osi->last_option)) &&
			((!doc->rh->form_last->last_select) ||
			 (ooi != doc->rh->form_last->last_select->last_option) ) )
		    {
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
		    }
		    else
		    {
			PPDBG((stderr, "Reformatting the tail\n"));
			be_document_reformat_tail(doc, oti, doc->rh->stream.fwidth);
			PPDBG((stderr, "Tail reformat done\n"));
		    }
		}

		if (doc->flags & doc_flag_DISPLAYING)
		    be_set_dimensions(doc);
	    }
	}
    }

    doc->lstatus = status;
    doc->lbytes = so_far;

    if (status == status_GETTING_BODY || status == status_REQUEST_BODY)
    {
	frontend_view_status(doc->parent,
			     status == status_GETTING_BODY ? sb_status_FETCHED : sb_status_SENT,
			     so_far, size);
    }
    else
    {
	frontend_view_status(doc->parent, sb_status_PROGRESS, status);
    }

#if PP_DEBUG
    PPDBG((stderr, "Progress done\n"));

    if (doc->rh) dump_header(doc->rh);

#endif

    threaded = NULL;
}

#if 0
static os_error *be_parse_file_to_end(antweb_doc *doc)
{
    os_filestr osf;
    os_error *ep;
    int fsize;
    int fh;
    os_regset r;
    rid_text_item *ti;

    osf.action = 5;
    osf.name = doc->cfile;

    ep = os_file(&osf);

    if (ep)
	return ep;

    fsize = osf.start;

    BEDBG((stderr, "Final file size is %d\n", fsize));

    r.r[0] = 0x4f;		/* Make errors rather than give a 0 handle */
    r.r[1] = (int) (long) doc->cfile;

    ep = os_find(&r);
    if (ep)
	return ep;

    fh = r.r[0];
    PPDBG((stderr, "Opened file on handle %d\n", fh));
    if (doc->lbytes != fsize)
    {
	PPDBG((stderr, "Calling pparse from parse_to_end\n"));
	be_pparse_doc(doc, fh, doc->lbytes, fsize);
	PPDBG((stderr, "pparse from parse_to_end done\n"));
    }

    /* OH BARF BARF BARF BARF */

    if (doc->rh == NULL)
	doc->rh = (((pparse_details*)doc->pd)->rh)(doc->ph);

    ti = doc->rh->stream.text_last;

    PPDBG((stderr, "Closing file %d\n", fh));
    r.r[0] = 0;
    r.r[1] = fh;

    os_find(&r);
    PPDBG((stderr, "Closing parser down\n"));
    doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, doc->cfile);
    doc->ph = NULL;

    /* REPULSIVE HACK - BUT HEY, SO THE REST OF THIS ACCESS CODE */

    while (ti)
    {
	(object_table[ti->tag].size)(ti, doc->rh, doc);

	/*ti = ti->next;*/
        ti = rid_scanf(ti);
    }



    PPDBG((stderr, "New rid_header at 0x%p\n", doc->rh));
    return NULL;
}
#endif

/*
 * If the status is status_FAILL_CONNECT then 'cfile' is actually the error message.
 */

static access_complete_flags antweb_doc_complete(void *h, int status, char *cfile, char *url)
{
    antweb_doc *doc = (antweb_doc *) h;
    int ft;

    frontend_view_status(doc->parent, sb_status_PROGRESS, status);

    doc->ah = NULL;

    ACCDBG(("Access completed, doc=%p, status=%d, file='%s', url='%s'\n",
	    doc, status, strsafe(cfile), url));

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

	frontend_view_visit(doc->parent, NULL, NULL,
			    (status == status_FAIL_CONNECT || status == status_FAIL_DNS) ? cfile : NULL);

	backend_dispose_doc(doc);
	return 0;
    }

#if 0
    /* very grubby way of passing information from access.c - to be removed when possible */
    {
	extern http_header_item *last_http_headers;
	http_header_item *list;
	for (list = last_http_headers; list; list = list->next)
	{
	    rid_meta_item *m = mm_calloc(sizeof(rid_meta_item), 1);

	    BEDBG((stderr, "doc_complete: add meta rh %p list %p\n", doc->rh, list));

	    m->httpequiv = strdup(list->key);
	    m->content = strdup(list->value);

	    rid_meta_connect(doc->rh, m);
	}
    }
#endif    
    
    doc->cfile = strdup(cfile);
    if (doc->url == NULL)
	doc->url = strdup(url);

    BEDBG((stderr, "Completed opening %s\n", url));
    BEDBG((stderr, "Cache file is '%s'\n", doc->cfile));


    ft = file_type(cfile);

    if (doc->ph)
    {
        PPDBG((stderr, "Closing parser down\n"));
        doc->rh = ((pparse_details*)doc->pd)->close(doc->ph, doc->cfile);
        doc->ph = NULL;

	/* SJM: temporary hack to try and see pages... */
	fvpr_progress_stream_flush(&doc->rh->stream);

	if ((doc->flags & doc_flag_DISPLAYING) == 0)
	{
	    BEDBG((stderr, "visit called from complete\n"));
	    antweb_init_page(doc);
	}

        /* SJM: layout frames */
        if (doc->rh->frames)
        {
            fe_view_dimensions fvd;
            frontend_view_get_dimensions(doc->parent, &fvd);

	    layout_layout(doc, fvd.layout_width, fvd.layout_height, 0);

	    doc->rh->stream.widest = fvd.wa_width;
	    doc->rh->stream.height = fvd.wa_height;
        }

        /* SJM: check the images for external client-side imagemaps */
        frontend_complain(imagemap_check_all_images(doc));

        /* SJM: see if width changes and reformat */
        be_set_dimensions(doc);

	be_update_image_info(doc);

	if ((doc->rh->bgt & rid_bgt_IMAGE) && (doc->rh->tile.im == NULL))
	{
	    be_doc_fetch_bg(doc);
	}

#ifndef STBWEB
	if (frontend_view_has_caret(doc->parent))
	{
	    doc->text_input_offset = -1;

	    antweb_place_caret(doc, backend_highlight_link(doc, doc->rh->stream.text_list,
							  be_link_INCLUDE_CURRENT | be_link_TEXT | be_link_DONT_HIGHLIGHT | be_link_VISIBLE));
	}
#endif

	/* Override the visability of the caret */
	frontend_complain(backend_goto_fragment(doc, doc->frag));

	if (doc->rh->refreshtime >= 0)
	    alarm_set(alarm_timenow()+(doc->rh->refreshtime * 100), be_refresh_document, doc);

	frontend_view_status(doc->parent, sb_status_FINISHED);
    }
    else
    {
	BEDBG((stderr, "Got document of type 0x%03x, passing to the front end.\n", ft));

	if (frontend_plugin_handle_file_type(ft))
	    plugin_helper(doc->url, ft, NULL, doc->parent, cfile);
	else
	    frontend_pass_doc(doc->parent, doc->url, cfile, ft);

	BEDBG((stderr, "Returned from frontend\n"));

	backend_dispose_doc(doc);
    }

    BEDBG((stderr, "'Compleated' function done.\n"));

    return access_CACHE;
}


os_error *backend_doc_abort(be_doc doc)
{
    BEDBG((stderr, "Entering backend_doc_abort for %p\n", doc));

    if ((doc->flags & doc_flag_DISPLAYING) == 0)
	return NULL;

    if (doc->ah)
    {
	BEDBG((stderr, "Calling access_abort\n"));
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

    if (doc->flags & doc_flag_INCOMPLETE)
	frontend_view_status(doc->parent, sb_status_ABORTED);

    return NULL;
}

#ifdef STBWEB
int antweb_doc_abort_all(void)
{
    be_doc doc;
    int count = 0;
    for (doc = document_list; doc; doc = doc->next)
    {
	int incomplete = (doc->flags & doc_flag_INCOMPLETE);

	if (!incomplete)
	{
	    if (doc->ah)
	    {
		access_abort(doc->ah);
		doc->ah = NULL;

		doc->flags |= doc_flag_INCOMPLETE;

		frontend_view_status(doc->parent, sb_status_ABORTED);

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

    return NULL;
}

static void be_refresh_document(int at, void *h)
{
    be_doc doc = (be_doc) h;

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
				  char *url, char *bfile, char *referer,
				  int flags)
{
    antweb_doc *new;
    os_error *ep;
    char *use_url;
    char *frag;

    *docp = new = mm_calloc(1, sizeof(antweb_doc));

    new->magic = ANTWEB_DOC_MAGIC;
    new->parent = v;
    new->scale_value = config_display_scale_image;

    /* new: add the url here */
/*     new->url = strdup(url); */

    /* add to list of documents, must do now in case we dispose of doc before returning from access_url */
    BEDBG((stderr, "backend_open_url: checklist: add %p document_list %p next %p\n", new, document_list, document_list ? document_list->next : NULL));
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

    BEDBG((stderr, "Calling access function flags %x\n", flags));

#if USE_MARGINS
    frontend_view_margins(v, &new->margin);
#endif

    ep = access_url(use_url,
		    (flags & be_openurl_flag_NOCACHE ? access_NOCACHE : 0) |
			(flags & be_openurl_flag_HISTORY ? 0 : access_CHECK_EXPIRE) |
			access_CHECK_FILE_TYPE,
		    NULL, bfile, referer,
		    &antweb_doc_progress, &antweb_doc_complete, new, &new->ah);

    mm_free(use_url);

    if (ep == 0)
    {
	frontend_view_status(v, sb_status_URL, url);

	if (new->ah == NULL)
	    frontend_view_status(v, sb_status_PROGRESS, status_COMPLETED_CACHE);
	else
	{
	    BEDBG((stderr, "New access handle is 0x%p\n", new->ah));
	}

	if (!antweb_doc_in_list(new))
	{
	    /* document has already been freed from within access_url() */
	    *docp = NULL;
	}
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
	    ep = NULL;
	    break;
	}

	/* only free if still in list in case already freed */
	if (antweb_doc_in_list(new))
	{
	    antweb_unlink_doc(new);
	    mm_free(new);
	    *docp = NULL;
	}
    }

    return ep;
}

BOOL backend_doc_saver_text(char *fname, void *h)
{
    be_doc doc = (be_doc) h;
    FILE *f;

    f = fopen(fname, "w");
    if (f == NULL)
	return FALSE;

    stream_write_as_text(doc->rh, &(doc->rh->stream), f);

    fclose(f);

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

    OK = (frontend_complain(image_save_as_sprite(im, fname)) == NULL);

    if (OK)
	frontend_saver_last_name(fname);

    return OK;
}

os_error *backend_doc_key(be_doc doc, int key, int *used)
{
    rid_text_item *ti = doc->input;

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

os_error *backend_doc_cursor(be_doc doc, int motion, int *used)
{
    int redraw = FALSE;
    rid_text_item *ti = doc->input;
    rid_text_item *also_redraw = NULL;
    int old_offset = doc->text_input_offset;

    doc->text_input_offset = -1;

    *used = 0;

    if (ti == NULL)
	return NULL;

    redraw = TRUE;		/* The default case negates this if we don't use the key */
    also_redraw = ti;
    switch (motion)
    {
    case be_cursor_UP:
    case (be_cursor_UP | be_cursor_WRAP):
	/*ti = antweb_prev_text_item(ti);*/
        ti = rid_scanbr(ti);
        ti = antweb_prev_text_input(ti, doc);
	if (ti)
	{
	    break;
	}
	if (motion == 0)
	{
	    ti = doc->input;
	    redraw=FALSE;
	    break;
	}
	/* Otherwise fall through */
    case (be_cursor_DOWN | be_cursor_LIMIT):
	ti = antweb_prev_text_input(doc->rh->stream.text_last, doc);
	break;

    case be_cursor_DOWN:
    case (be_cursor_DOWN | be_cursor_WRAP):
	/*ti = ti->next;*/
        ti = rid_scanfr(ti);
	ti = antweb_next_text_input(ti, doc);
	if (ti)
	{
	    break;
	}
	if (motion == be_cursor_DOWN)
	{
	    ti = doc->input;
	    redraw=FALSE;
	    break;
	}
	/* Otherwise fall through */
    case (be_cursor_UP | be_cursor_LIMIT):
	ti = antweb_next_text_input(doc->rh->stream.text_list, doc);
	break;

    default:
	redraw=FALSE;
	also_redraw = NULL;
	break;
    }

    if (ti != doc->input)
    {
/* 	doc->input = ti; */
	antweb_place_caret(doc, ti);
    }
    else
    {
	doc->text_input_offset = old_offset;
    }

    if (redraw)
    {
	antweb_update_item(doc, doc->input);

	if (also_redraw && also_redraw != doc->input)
	    antweb_update_item(doc, also_redraw);

	*used = TRUE;
    }

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

static int adjust_flag(int old_flag, int select, BOOL *changed)
{
    int new_flag = 0;

    switch (select)
    {
        case -1:
            new_flag = old_flag ^ rid_flag_SELECTED;
            break;
        case 1:
            new_flag = old_flag | rid_flag_SELECTED;
            break;
        case 0:
            new_flag = old_flag &~ rid_flag_SELECTED;
            break;
    }

    if (changed)
	*changed = (new_flag ^ old_flag) & rid_flag_SELECTED ? 1 : 0;

    return new_flag;
}

/*
 * Refresh the highlighting on an object
 * Uses a specific method if there is one else just
 * redraws the whole box.
 */

static void be_update_item_highlight(be_doc doc, be_item ti)
{
    if (object_table[ti->tag].update_highlight)
        object_table[ti->tag].update_highlight(ti, doc);
    else
        antweb_update_item(doc, ti);
}

/*
 * if selected is -1 then it toggles the state of the selected bit
 */

be_item backend_update_link(be_doc doc, be_item item, int selected)
{
    be_item ti;
    BOOL changed;

    if (item == NULL)
        return NULL;

/*     BEDBG((stderr, "backend_update_link: item %p tag %d aref %p href %s updating %d\n", item, item->tag, item->aref, item->aref ? strsafe(item->aref->href) : "", selected)); */

    /* if it isn't actually a link then toggle the flag anyway */
    if (item->aref == NULL)
    {
	doc->selection.tag = doc_selection_tag_TEXT;
	doc->selection.data.text = item;

	item->flag = adjust_flag(item->flag, selected, &changed);
	if (changed)
	    be_update_item_highlight(doc, item);
        return item;
    }
    else
    {
	doc->selection.tag = doc_selection_tag_AREF;
	doc->selection.data.aref = item->aref;
    }

    for (ti = item->aref->first; ti && ti->aref == item->aref; ti = rid_scanfr(ti))
    {
        ti->flag = adjust_flag(ti->flag, selected, &changed);

/* 	BEDBG((stderr, "                     item %p tag %d changed %d\n", ti, ti->tag, changed)); */

	if (changed)
	    be_update_item_highlight(doc, ti);
    }

    return item->aref->first;
}

void backend_update_link_activate(be_doc doc, be_item item, int activate)
{
    be_item ti;

    if (item == NULL || item->aref == NULL)
        return;

#if 0				/* This is too often wrong to be useful */
    /* When deactivating mark as visited so frame links update correctly */
    if (!activate)
        item->aref->flags |= rid_aref_IN_CACHE;
#endif

    for (ti = item->aref->first; ti && ti->aref == item->aref; ti = rid_scanfr(ti))
    {
	if (activate)
	    ti->flag |= rid_flag_ACTIVATED;
	else
	    ti->flag &= ~rid_flag_ACTIVATED;
	be_update_item_highlight(doc, ti);
    }
}

static BOOL be_item_onscreen(be_doc doc, be_item ti, const wimp_box *bounds, int flags)
{
    wimp_box box;
    backend_doc_item_bbox(doc, ti, &box);
#if 0
    if (
       ((box.y1 > bounds->y0 && box.y1 <= bounds->y1) ||
        (box.y0 >= bounds->y0 && box.y0 < bounds->y1)) &&

       ((box.x1 > bounds->x0 && box.x1 <= bounds->x1) ||
        (box.x0 >= bounds->x0 && box.x0 < bounds->x1))
        )
        return TRUE;
#else
    if (flags & be_link_BACK)
    {
        if (box.y0 >= bounds->y0 && box.y0 < bounds->y1)
            return TRUE;
    }
    else
    {
        if (box.y1 > bounds->y0 && box.y1 <= bounds->y1)
            return TRUE;
    }
#endif
    return FALSE;
}

static BOOL match_item(be_item ti, int flags, rid_aref_item *aref)
{
    BOOL aref_valid = ti->aref && (ti->aref->href || (ti->aref->flags & rid_aref_LABEL));
    BOOL aref_changed_enough = ti->aref != aref || (flags & (be_link_INCLUDE_CURRENT | be_link_ONLY_CURRENT));

    if (ti->tag == rid_tag_TEXTAREA)
    {
	if (((rid_text_item_textarea *)ti)->area->base.tabindex == -1)
	    return FALSE;

	if ((flags & be_link_TEXT) == 0)
	{
	    if (aref_valid && !aref_changed_enough)
		return FALSE;
	}
	return TRUE;
    }

    if (ti->tag == rid_tag_INPUT)
    {
	if (((rid_text_item_input *)ti)->input->base.tabindex == -1)
	    return FALSE;

	if (flags & be_link_TEXT)
	{
	    rid_input_tag tag = ((rid_text_item_input *)ti)->input->tag;
	    return tag == rid_it_TEXT || tag == rid_it_PASSWD;
	}

	if (aref_valid && !aref_changed_enough)
	    return FALSE;

	return TRUE;
    }

    if ((flags & be_link_TEXT) == 0)
    {
	if (ti->tag == rid_tag_OBJECT)
	{
	    rid_text_item_object *tio = (rid_text_item_object *)ti;
	    if (tio->object->type == rid_object_type_PLUGIN)
		return TRUE;
	}

	if (ti->tag == rid_tag_SELECT)
	{
	    if (aref_valid && !aref_changed_enough)
		return FALSE;
	    return TRUE;
	}

	if (ti->tag == rid_tag_IMAGE && ((rid_text_item_image *)ti)->usemap)
	    return TRUE;

	/* check for tag specifically in case a table gets an AREF around it */
	if ((ti->tag == rid_tag_TEXT || ti->tag == rid_tag_IMAGE || ti->tag == rid_tag_OBJECT) &&
	    aref_valid && aref_changed_enough)
	    return TRUE;
    }

    return FALSE;
}

be_item backend_highlight_link(be_doc doc, be_item item, int flags)
{
    rid_aref_item *aref;
    be_item ti;
    wimp_box bounds, margins;
    const int scan_flags = SCAN_RECURSE | ( (flags & be_link_BACK) ? SCAN_BACK : SCAN_FWD );

    LKDBG((stderr, "Highlight from item %p, flags=0x%x, line=%p\n", item, flags, item ? item->line : NULL));

    if (item == NULL)
    {
	ti = (flags & be_link_BACK) ? doc->rh->stream.text_last : doc->rh->stream.text_list;
	aref = NULL;
    }
    else
    {
        if (flags & (be_link_INCLUDE_CURRENT|be_link_ONLY_CURRENT))
            ti = item->aref ? item->aref->first : item;			/* backtrack to start of anchor sequence */
        else
            ti = rid_scan(item, scan_flags);

	aref = item->aref;
    }

    LKDBG((stderr, "Start search at %p, aref=%p, line=%p\n", ti, aref, ti ? ti->line : NULL));

    frontend_view_bounds(doc->parent, &bounds);
#if USE_MARGINS
    margins = doc->margin;
#else
    frontend_view_margins(doc->parent, &margins);
#endif
    bounds.x0 += margins.x0;
    bounds.y0 += margins.y0;
    bounds.x1 += margins.x1;
    bounds.y1 += margins.y1;

    while (ti)
    {
	if (match_item(ti, flags, aref))
	{
	    if ((flags & be_link_VISIBLE) == 0 || be_item_onscreen(doc, ti, &bounds, flags))
	        break;
	}

	if (flags & be_link_ONLY_CURRENT)
	{
	    ti = NULL;
	    break;
	}
	else
	{
	    ti = rid_scan(ti, scan_flags);
	    LKDBG((stderr, "ti=%p, next=%p, line=%p\n", ti, ti->next, ti->line));
	}
    }

    if (ti == NULL && (flags & (be_link_DONT_WRAP | be_link_ONLY_CURRENT)) == 0)
    {
	ti = (flags & be_link_BACK) ? doc->rh->stream.text_last : doc->rh->stream.text_list;

	LKDBG((stderr, "No link found, ti wraped to %p\n", ti));

	while (ti)
	{
	    if (match_item(ti, flags | be_link_INCLUDE_CURRENT, aref))
            {
	        if ((flags & be_link_VISIBLE) == 0 || be_item_onscreen(doc, ti, &bounds, flags))
	            break;
            }

            ti = rid_scan(ti, scan_flags);
	    LKDBG((stderr, "ti=%p, next=%p, line=%p\n", ti, ti->next, ti->line));
	}
    }

    if ((flags & be_link_DONT_HIGHLIGHT) == 0)
    {
	BOOL item_changed = item != ti && (item == NULL || item->aref != ti->aref);

	/* de highlight original only if the highlight has ended up changing */
        if (item_changed && item && (flags & be_link_ONLY_CURRENT) == 0)
	    backend_update_link(doc, item, 0);
	
        if (ti)
        {
	    int x, y;

	    LKDBG((stderr, "New link at %p\n", ti));

	    if ((flags & be_link_VISIBLE) == 0 && stream_find_item_location(ti, &x, &y))
	    {
#if USE_MARGINS
	        frontend_view_ensure_visable(doc->parent, x, y + ti->max_up + doc->margin.y1, y - ti->max_down + doc->margin.y1);
#else
		frontend_view_ensure_visable(doc->parent, x, y + ti->max_up, y - ti->max_down);
#endif
	    }

            if (item_changed || (flags & be_link_ONLY_CURRENT))
                backend_update_link(doc, ti, 1);
        }
    }

    LKDBG((stderr, "About to return %p\n", ti));

    return ti;
}

os_error *backend_activate_link(be_doc doc, be_item item, int flags)
{
    if (!item)
	return NULL;
    BEDBG((stderr, "activate link %p (type %d)\n", item, item->tag));

    backend__doc_click(doc, item, item->tag == rid_tag_INPUT ? item->width : 0, 0,
			      (flags & 1) ? wimp_BRIGHT : wimp_BLEFT );

    return NULL;
}

be_item backend_place_caret(be_doc doc, be_item item)
{
    be_item input = doc->input;

    if (item != backend_place_caret_READ)
	antweb_place_caret(doc, item);

    return input;
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

#ifdef STBWEB

#define TIME_FORMAT	"%a, %d %b %Y %H:%M:%S GMT"

const char *backend_check_meta(be_doc doc, const char *name)
{
    static char rbuf[32];

    rid_meta_item *m;
    for (m = doc->rh->meta_list; m; m = m->next)
    {
        if ((m->name && strcasecomp(m->name, name) == 0) ||
            (m->httpequiv && strcasecomp(m->httpequiv, name) == 0))
            return m->content;
    }

    if (strcasecomp(name, "expires") == 0)
    {
	unsigned expires;
	access_get_header_info(doc->url, NULL, NULL, &expires);
	if (expires == UINT_MAX)
	    return NULL;
	strftime(rbuf, sizeof(rbuf), TIME_FORMAT, gmtime(&expires));
	return rbuf;
    }

    if (strcasecomp(name, "last-modified") == 0)
    {
	unsigned last_modified;
	access_get_header_info(doc->url, NULL, &last_modified, NULL);
	if (last_modified == 0)
	    return NULL;
	strftime(rbuf, sizeof(rbuf), TIME_FORMAT, gmtime(&last_modified));
	return rbuf;
    }

    return NULL;
}
#endif

void backend_clear_selected(be_doc doc)
{
    be_item ti = doc->rh->stream.text_list;
    while (ti)
    {
	if (ti->flag & rid_flag_SELECTED)
	{
	    ti->flag &= ~rid_flag_SELECTED;
	    be_update_item_highlight(doc, ti);
	}
	
	ti = rid_scan(ti, SCAN_RECURSE | SCAN_FWD);
    }

    doc->selection.data.text = NULL;
}

#if 0
void backend_select_item(be_doc doc, be_item item, int select)
{
    int new_flag;

    /* alter the selection state of this item */
    new_flag = adjust_flag(item->flag, select, NULL);

    /* if the new item ends up selected then deselect everything else in the document */
    if (new_flag & rid_flag_SELECTED)
    {
	be_item ti = doc->rh->stream.text_list;
	while (ti)
	{
            if (ti->flag & rid_flag_SELECTED)
            {
                ti->flag &= ~rid_flag_SELECTED;
                be_update_item_highlight(doc, ti);
            }

            ti = rid_scan(ti, SCAN_RECURSE | SCAN_FWD);
	}
    }

    /* update the new item */
    item->flag = new_flag;
    be_update_item_highlight(doc, item);
}
#endif

#ifdef STBWEB
/* FIXME: this needs to be updated for the new selection model */
be_item backend_find_selected(be_doc doc)
{
    be_item ti = doc->rh->stream.text_list;
    while (ti)
    {
        if (ti->flag & rid_flag_SELECTED)
            break;

        ti = rid_scan(ti, SCAN_RECURSE | SCAN_FWD);
    }
    return ti;
}
#endif

#ifdef STBWEB
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

void backend_doc_reformat(be_doc doc)
{
    if (doc)
    {
	antweb_document_sizeitems(doc);
	antweb_document_format(doc, doc->rh->stream.fwidth);

	be_set_dimensions(doc);

	if (frontend_view_has_caret(doc->parent))
	    antweb_place_caret(doc, doc->input);
    }
}

void backend_doc_set_scaling(be_doc doc, int scale_value)
{
    if (doc)
    {
	doc->scale_value = scale_value;

	antweb_document_sizeitems(doc);
	antweb_document_format(doc, doc->rh->stream.fwidth);

	be_set_dimensions(doc);

	if (frontend_view_has_caret(doc->parent))
	    antweb_place_caret(doc, doc->input);
    }
}

extern void backend_plugin_action(be_doc doc, be_item item, int action)
{
    if (item && item->tag == rid_tag_OBJECT)
    {
	rid_text_item_object *tio = (rid_text_item_object *) item;
	rid_object_item *obj = tio->object;

	if (obj->type == rid_object_type_PLUGIN)
	    plugin_send_action(obj->state.plugin.pp, action);
    }
    else
	plugin_send_action(NULL, action);

    NOT_USED(doc);
}

extern void backend_plugin_info(be_doc doc, void *pp, int *flags, int *state)
{
    plugin_get_info(pp, flags, state);
    NOT_USED(doc);
}

extern be_item backend_locate_id(be_doc doc, const char *id)
{
    rid_header *rh = doc->rh;
    if (rh)
    {
	be_item ti = rh->stream.text_list;
	rid_aref_item *last_aref = NULL;

	while (ti)
	{
	    switch (ti->tag)
	    {
	    case rid_tag_TEXT:
	    case rid_tag_IMAGE:
		if (ti->aref && ti->aref != last_aref)
		{
		    if (ti->aref->name && strcmp(ti->aref->name, id) == 0)
			return ti;
		    last_aref = ti->aref;
		}
		break;
		
	    case rid_tag_INPUT:
		if (strcmp(((rid_text_item_input *)ti)->input->base.id, id) == 0)
		    return ti;
		break;

	    case rid_tag_TEXTAREA:
		if (strcmp(((rid_text_item_textarea *)ti)->area->base.id, id) == 0)
		    return ti;
		break;

	    case rid_tag_SELECT:
		if (strcmp(((rid_text_item_select *)ti)->select->base.id, id) == 0)
		    return ti;
		break;

	    case rid_tag_OBJECT:
		if (strcmp(((rid_text_item_object *)ti)->object->id, id) == 0)
		    return ti;
		break;

	    case rid_tag_TABLE:
		if (strcmp(((rid_text_item_table *)ti)->table->id, id) == 0)
		    return ti;
		break;
	    }
	    ti = rid_scanfr(ti);
	}
    }
    return NULL;
}


/* eof backend.c */
