/* -*-c-*- */

/* oimage.c */

/*
 *    3/96: SJM: re-did oimage_click to handle clientside maps and do its own click and place_caret
 * 18/3/96: SJM: Added IMAGE_SCALE_TO_TAG switch for scaling images
 * 21/3/96: SJM: Removed my bogus max_down in oimage_click, added active link
 * 28/3/96: SJM: Changed image scaling again - doen't do strange things with one size and default image
 * 02/4/96: SJM: oimage_click with ctl held will select the object and deselect everything else.
 * 09/4/96: SJM: redraw will always redraw the box if selected whatever the type
 * 18/4/96: SJM: changed use of be_update_link to backend_update_link
 *  3/5/96: SJM: added display_scale use and changed unset tii->ww and hh to be -1 rather than 0
 *  7/5/96: SJM: added hspace, vspace
 * 11/6/96: SJM: do config image scaling here rather than in image_info().
 * 13/6/95: SJM: added _update_highlight method
 * 20/6/95: SJM: added check for imagemap error in oclick()
 * 19/7/96: SJM: made alt text display obey the size define in the HTML, wrapping if necessary
 *  2/8/96: SJM: border drawing only depends on bwidth and selected. bwidth is set correctly by builders.c
 * 04/9/96: SJM: check for EXPIRE flag in image_find
 * 11/9/96: SJM: check for no null area->href (to handle NOHREF areas correctly).
 */

/* Methods for image objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memwatch.h"
#include "unwind.h"

#include "akbd.h"
#include "wimp.h"
#include "bbc.h"

#include "antweb.h"
#include "interface.h"
#include "images.h"
#include "rid.h"
#include "webfonts.h"
#include "url.h"
#include "consts.h"
#include "config.h"
#include "util.h"
#include "render.h"
#include "rcolours.h"

#include "dfsupport.h"

#include "imagemap.h"
#include "object.h"
#include "version.h"
#include "oimage.h"
#include "gbf.h"

/* ----------------------------------------------------------------------------- */

#ifndef DRAW_AREA_HIGHLIGHT
#define DRAW_AREA_HIGHLIGHT 0
#endif

#define PLINTH_PAD 16

#define NONE_STRING	"[IMAGE]"

/* ----------------------------------------------------------------------------- */

static void oimage_size_alt_text(antweb_doc *doc, const char *alt, const rid_stdunits *req_ww, const rid_stdunits *req_hh, rid_image_flags flags, const rid_text_item *ti, int fwidth, int *iw, int *ih, int scalefactor )
{
    font_string fs;
    int whichfont;
    struct webfont *wf;
    int imw, imh;

    IMGDBG(("Sizing alt text: %d,%d\n", *iw, *ih));

    /* if we are not deferring images and either have alt text or both sizes are specified then use what we've got */
    if (alt == NULL || (/* !defer_images &&  */req_ww->type != value_none && req_hh->type == value_absunit))
    {
	switch (req_ww->type)
	{
	case value_pcunit:
	    if (fwidth)
		*iw = (int)(req_ww->u.f * fwidth / 100);
	    break;

	case value_absunit:
	    *iw = (int)(req_ww->u.f * scalefactor / 100);
	    break;
	}

	if (req_hh->type == value_absunit)
	    *ih = (int)(req_hh->u.f * scalefactor / 100);
        return;
    }

#ifndef BUILDERS
    {
	whichfont = antweb_getwebfont(doc, (rid_text_item *)ti, ALT_FONT);
	wf = &webfonts[whichfont];
/* 	wf = &webfonts[ALT_FONT]; */

        /* if no width given then use what we need */
        if (/* defer_images ||  */req_ww->type == value_none)
        {
#if 1
	    fs.x = webfont_font_width(whichfont, alt ? alt : NONE_STRING);
#else
	    font_setfont(wf->handle);

	    fs.x = 1 << 30;
	    fs.y = 1 << 30;
	    fs.split = -1;

	    fs.s = alt ? (char *)alt : NONE_STRING;
	    fs.term = strlen(fs.s);
	    font_strwidth(&fs);

	    fs.x /= MILIPOINTS_PER_OSUNIT;
#endif
	    imw = fs.x + PLINTH_PAD;
	    imh = (/* defer_images ||  */req_hh->type != value_absunit)
	             ? (wf->max_up + wf->max_down + PLINTH_PAD*2)
	             : (int)(req_hh->u.f * scalefactor / 50.0); /* "*2/100" */
        }
        else
        {
            /* else wrap the text into the space given */
            int ww = 0, height = 0;

    	    switch (req_ww->type)
	    {
	    case value_pcunit:
    	        ww = (int)(req_ww->u.f * fwidth / 100);
		if (ww == 0)
		    ww = *iw;
		break;

	    case value_absunit:
    	        ww = (int)(req_ww->u.f * scalefactor / 100);
		break;
	    }

	    if (ww > PLINTH_PAD)
	    {
		write_text_in_box_height(alt, ww - PLINTH_PAD, whichfont, &height);
		imh = height + PLINTH_PAD;
	    }
	    else
		imh = *ih;

	    imw = ww;
	}
    }
#else  /* BUILDERS */
    imw = strlen(alt ? alt : NONE_STRING);
    imh = 16;
#endif /* BUILDERS */
    *iw = imw;
    *ih = imh;

    IMGDBG(("Done sizing alt text: %d,%d\n", *iw, *ih));
}

void oimage_size_image(antweb_doc *doc, const char *alt, const rid_stdunits *req_ww, const rid_stdunits *req_hh, rid_image_flags flags, rid_text_item *ti, int scale_value, int fwidth, int *iw, int *ih)
{
    int width, height;

    width = (*iw * scale_value)/100;
    height = (*ih * scale_value)/100;

    IMGDBG(("oimage_size_image: old width %d, height %d, scale %d%% rid flags 0x%x text_flags 0x%x\n", *iw, *ih, scale_value, flags, ti->flag));

    /* if we have an image
     *   if two sizes specified then use those
     *   if one size specified then use that and maintain aspect ratio
     *   if no sizes specified then use the images own size
     *     if width specified as % then calculate that
     * if we don't have an image yet
     *   if two sizes specified then use those
     *   if one size specified then use that and the default images size
     *   if no sizes specified then use the default images size
     * note: width and height are in OS coords
     */

    if (flags & rid_image_flag_REAL)
    {
        /* if a width or height is specified */
	if (req_ww->type != value_none || req_hh->type == value_absunit)
	{
            double aspect = (double)width/height;

#ifdef BUILDERS
	    aspect = 1.0;
#endif

    	    switch (req_ww->type)
    	    {
	    case value_pcunit:
	    {
		int ww = (int)(req_ww->u.f * fwidth/100);
		if (ww != 0)
		    width = ww;
		break;
	    }
            case value_absunit:
		width = (int)(req_ww->u.f * scale_value/100);

		IMGDBG(("osi: abswidth %f * scale %d%% gives width %d\n",
		        req_ww->u.f, scale_value, width ));
		break;

    	    case value_none:
    	       /* If width is not specified, calculate from height */
    	        width = (int)(req_hh->u.f * aspect * scale_value/100);
		break;
    	    }

    	    height = (req_hh->type == value_absunit)
    	                 ? (int)(req_hh->u.f * scale_value / 100 )
    	                 : (int)((double)width /* * scale_value/100 */ / aspect);
    	}
    }
    else
    {
        /* if not real then size from the text */
#ifndef BUILDERS
	oimage_size_alt_text(doc, alt, req_ww, req_hh, flags, ti, fwidth, &width, &height, scale_value);
#else
	width = 16;
	height = 16;
#endif
    }

    IMGDBG(("Now width %d, height %d\n", width, height));

    *iw = width;
    *ih = height;
}

image oimage_fetch_image(antweb_doc *doc, const char *src, BOOL need_size)
{
    char *url, *base;
    int ffl;
    image imh;

    ffl = (doc->flags & doc_flag_DEFER_IMAGES) || gbf_active(GBF_LOW_MEMORY) ? image_find_flag_DEFER : 0;
    if ((doc->flags & doc_flag_FROM_HISTORY) == 0)
	ffl |= image_find_flag_CHECK_EXPIRE;
    if (need_size)
	ffl |= image_find_flag_NEED_SIZE;
    if (doc->flags & doc_flag_FAST_LOAD)
	ffl |= image_find_flag_URGENT;

    base = BASE(doc);
    url = url_join(base, src);

    IMGDBG(("oimage_fetch calls image_find, doc=%p, doc->parent=%p\n", doc, doc->parent ));

    imh = NULL;
    image_find(url, base, ffl,
	       &antweb_doc_image_change, doc, render_get_colour(render_colour_BACK, doc),
	       &imh);

    mm_free(url);

    return imh;
}

void oimage_flush_image(image im)
{
    int f;
    if (im &&
	image_info(im, NULL, NULL, NULL, &f, NULL, NULL) == NULL &&
	(f & image_flag_FETCHED) == 0)
    {
	image_mark_to_flush(im, image_find_flag_DEFER);
    }
}

#ifndef BUILDERS
void oimage_render_text(rid_text_item *ti, antweb_doc *doc, object_font_state *fs, wimp_box *bbox, const char *text)
{
    if (text)
    {
	wimp_box box;
	int whichfont;
/* 	struct webfont *wf; */
	int tfc;

	whichfont = antweb_getwebfont(doc, ti, ALT_FONT);
#if 0
 	wf = &webfonts[whichfont];
/* 	wf = &webfonts[ALT_FONT]; */

	if (fs->lf != wf->handle)
	{
	    fs->lf = wf->handle;
	    font_setfont(fs->lf);
	}
#endif
	if (fs->lfc != (tfc = render_link_colour(ti, doc) ) || fs->lbc != render_colour_BACK)
	{
	    fs->lfc = tfc;
	    fs->lbc = render_colour_BACK;

	    render_set_font_colours(fs->lfc, render_colour_BACK, doc);
	}

	box.x0 = bbox->x0 + PLINTH_PAD/2;
	box.x1 = bbox->x1 - PLINTH_PAD/2;
	box.y0 = bbox->y0 + PLINTH_PAD/2;
	box.y1 = bbox->y1 - PLINTH_PAD/2;
	write_text_in_box(whichfont, text, &box);
    }
}
#endif

void oimage_render_border(rid_text_item *ti, antweb_doc *doc, wimp_box *bbox, int bw)
{
    int dx = frontend_dx;
    int dy = frontend_dy;
    int x, y, w, h;

    render_set_colour(render_text_link_colour(ti, doc), doc);

    x = bbox->x0 - bw;
    y = bbox->y0 - bw;
    w = bbox->x1 - bbox->x0 + bw*2;
    h = bbox->y1 - bbox->y0 + bw*2;

    if (bw <= 1)
    {
	bbc_rectangle(x, y, w-dx, h-dy);
	bbc_rectangle(x+2, y+2, w-2*2-dx, h-2*2-dy);
    }
    else
    {
	bbc_rectanglefill(x, y, w-dx, bw-dy);
	bbc_rectanglefill(x, y + h - bw, w-dx, bw-dy);
	bbc_rectanglefill(x, y + bw, bw-dx, h - (bw*2)-dy);
	bbc_rectanglefill(x + w - bw, y + bw, bw-dx, h - (bw*2)-dy);
    }
}

int oimage_decode_align(rid_pos_item *pi, rid_image_flags flags, int height)
{
    /* pdh: not sure whether pi can be NULL, so assume it can be */
    int max_up;

    if (flags & rid_image_flag_ATOP)
    /* TOP and TEXTTOP */
    {
	max_up = pi ? pi->max_up : webfonts[WEBFONT_BASE].max_up;
	IMGDBG(("decode_align: setting max_up to %d, %sfrom pos_item\n",
	        max_up, pi ? "" : "not " ));
    }
    else if (flags & rid_image_flag_ABOT)
    {
	if (flags & rid_image_flag_ABSALIGN)
	/* ABSBOTTOM */
	{
	    max_up = height - webfonts[WEBFONT_BASE].max_down;
	}
	else
	/* BOTTOM and BASELINE */
	{
	    max_up = height;
	}
    }
    else if (flags & rid_image_flag_ABSALIGN)
    /* ABSMIDDLE */
    {
	max_up = (webfonts[WEBFONT_BASE].max_up -
		      webfonts[WEBFONT_BASE].max_down + height) >> 1;
    }
    else
    /* MIDDLE */
    {
	max_up = height / 2;
    }

    return max_up;
}

BOOL oimage_handle_usemap(rid_text_item *ti, antweb_doc *doc, int x, int y, wimp_bbits bb, rid_map_item *map)
{
    BOOL handled = FALSE;

    if (map && (map->flags & rid_map_ERROR) == 0)
    {
	rid_area_item *area;
	area = imagemap_find_area(map, x, y);

	if (area && area->href)	/* an area with NOHREF will have area->href == NULL */
	{
	    BOOL follow_link = TRUE;
	    if (config_display_time_activate)
	    {
		int was_selected = backend_is_selected(doc, ti);

		/* highlight the area (and dehighlight image) */
/* 		tii->data.usemap.selection = area; */
#ifndef BUILDERS
		backend_update_link_activate(doc, ti, 1);

		follow_link = wait_for_release(config_display_time_activate);

		/* de highlight the area */
		backend_update_link_activate(doc, ti, 0);
#endif
		/* restore the original selection if there was one */
/* 		tii->data.usemap.selection = NULL; */
#ifndef BUILDERS
		if (was_selected)
		    backend_set_highlight(doc, ti, FALSE);
#endif
	    }

	    if (follow_link)
	    {
		rid_aref_item aref;
		memset(&aref, 0, sizeof(aref));
		aref.href = area->href;
		frontend_complain(antweb_handle_url(doc, &aref, NULL,
						    bb & wimp_BRIGHT ? "_blank" : area->target));
	    }
	}

	/* if there is no area then there is no link to follow */
	handled = TRUE;
    }
    /* if we can't find the right map then fall through to check server map */
    return handled;
}

/* ----------------------------------------------------------------------------- */

static BOOL oimage_renderable(rid_text_item_image *tii, antweb_doc *doc)
{
    int fl;
    if (tii->im == NULL)
	return FALSE;

    if (doc->flags & doc_flag_NO_PICS)
	return FALSE;

    image_info((image) tii->im, 0, 0, 0, &fl, 0, 0);
    if (fl & image_flag_REALTHING)
	return TRUE;

    if (tii->alt == NULL && ((doc->flags & doc_flag_DEFER_IMAGES) != 0 || (tii->hh.type == value_none && tii->ww.type == value_none)))
	return TRUE;

    return FALSE;
}

/* ----------------------------------------------------------------------------- */

/* size function called from format.c for percentage sized images */
void oimage_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth)
{
    rid_text_item_image *tii = (rid_text_item_image *) ti;
    rid_pos_item *pi = (fwidth == 0) ? NULL : ti->line;
    int width = -1, height = -1;
    image_flags fl;
    int min;

    IMGDBG(("oimage_size: src '%s' im %p bwidth %d, hspace %d, vspace %d, fwidth %d\n",
	    tii->src, tii->im, tii->bwidth, tii->hspace, tii->vspace, fwidth));

#ifndef BUILDERS
    /* Borris sez doing this here is a good way of wasting
       time. Between adding the object to the tree (parsing), and
       formatting (which triggers the sizing), there could be quite a
       time lag. This will impact the latency of image fetching,
       especially with a fast turnaround server. */
    if (!gbf_active(GBF_EARLYIMGFETCH))
    {
	if (tii->im == NULL)
	    tii->im = oimage_fetch_image(doc, tii->src, tii->ww.type == value_none || tii->hh.type == value_none);
    }
    image_info((image) tii->im, &width, &height, 0, &fl, 0, 0);

#else
    fl = image_flag_REALTHING;
#endif


    IMGDBG(("oimage_size: real %d width %d height %d\n", fl & image_flag_REALTHING ? 1 : 0, width, height));

    if (fl & image_flag_REALTHING)
	tii->flags |= rid_image_flag_REAL;

    oimage_size_image(doc, tii->alt, &tii->ww, &tii->hh, tii->flags, ti, doc->scale_value, fwidth, &width, &height);

    /* DAF: Formatter doesn't invisible objects please */
    if (width < 1)
	width = 1;
    if (height < 1)
	height = 1;

    tii->hgap = (tii->bwidth + tii->hspace) *doc->scale_value/100 * 2;
    min = (tii->bwidth ? 2 : 0) + (tii->hspace ? 2 : 0);
    if (tii->hgap < min)
	tii->hgap = min;

    tii->vgap = (tii->bwidth + tii->vspace) *doc->scale_value/100 * 2;
    min = (tii->bwidth ? 2 : 0) + (tii->vspace ? 2 : 0);
    if (tii->vgap < min)
	tii->vgap = min;

    width += tii->hgap * 2;
    height += tii->vgap * 2;

    ti->width = width;
    ti->pad = 0;

    ti->max_up = oimage_decode_align(pi, tii->flags, height);
    ti->max_down = height - ti->max_up;

    IMGDBG(("oimage_size:       width %d height %d hgap %d vgap %d item width %d max up %d down %d\n", width, height, tii->hgap, tii->vgap, ti->width, ti->max_up, ti->max_down));
}

/* size method */
void oimage_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    oimage_size_allocate(ti, rh, doc, 0);
}

void oimage_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc,
		   int hpos, int bline, object_font_state *fs,
		   wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_image *tii = (rid_text_item_image *) ti;
    wimp_box bbox;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    if (update == object_redraw_BACKGROUND)
	return;
    
    bbox.x0 = hpos + tii->hgap;
    bbox.y0 = bline - ti->max_down + tii->vgap;
    bbox.x1 = hpos + ti->width - tii->hgap;
    bbox.y1 = bline + ti->max_up - tii->vgap;

    if (oimage_renderable(tii, doc))
    {
	int oox = ox, ooy = oy;
#if USE_MARGINS
	oox -= doc->margin.x0;
	ooy -= doc->margin.y1;
#endif /* USE_MARGINS */

	image_render((image) tii->im, bbox.x0, bbox.y0,
		     (bbox.x1 - bbox.x0)/2,
		     (bbox.y1 - bbox.y0)/2,
		     doc->scale_value, antweb_render_background, doc,
		     oox, ooy);
    }
    else
    {
	char *alt;

	render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
		  bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0, doc);

	alt = tii->alt;
	if (!alt)
	{
	    alt = strrchr(tii->src, '/');
	    if (alt)
		alt++;
	    else
		alt = tii->src;
	}

	oimage_render_text(ti, doc, fs, &bbox, alt);
    }

    if (tii->bwidth)
    {
	int bw = tii->bwidth *doc->scale_value/100 * 2;
	if (bw < 2)
	    bw = 2;
 	oimage_render_border(ti, doc, &bbox, bw);

#if DRAW_AREA_HIGHLIGHT
        if (tii->usemap && tii->data.usemap.selection)
        {
            imagemap_draw_area(doc, tii->imh, tii->data.usemap.selection, x, bline + ti->max_up - tii->vgap);
        }
#endif
    }
#endif /* BUILDERS */
}

void oimage_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_image *tii = (rid_text_item_image *) ti;

    image_loose((image) tii->im, &antweb_doc_image_change, doc);
}

char *oimage_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    char qbuffer[64];
    rid_text_item_image *tii = (rid_text_item_image *) ti;
    int flags;

    image_info((image) tii->im, NULL, NULL, NULL, &flags, NULL, NULL);

    qbuffer[0] = 0;

    /* Only show image if the ALT key is held down */
    if ((flags & image_flag_DEFERRED) && kbd_pollalt())
    {
	frontend_complain(image_flush((image) tii->im, 0));
	return NULL;
    }

#if 0				/* SJM: 190297 this should never have been there really */
    /* try selecting items */
    if (akbd_pollctl() && (flags & image_flag_FETCHED))
    {
        backend_select_item(doc, ti, -1);
        antweb_place_caret(doc);
        return NULL;
    }
#endif

    if (tii->usemap || (tii->flags & rid_image_flag_ISMAP) != 0)
    {
	/* Remember that the y is in work area co-ordinates */

	x = x - tii->hgap;
	y = (ti->max_up) - tii->vgap - y;

        image_os_to_pixels((image)tii->im, &x, &y, doc->scale_value);

	if (tii->usemap &&
	    oimage_handle_usemap(ti, doc, x, y, bb, imagemap_find_map(rh, tii->usemap)))
	    return NULL;

	/* if we can't find the right map then fall through to check server map */
        if ((tii->flags & rid_image_flag_ISMAP) != 0)
	    sprintf(qbuffer, "?%d,%d", x, y);
    }

    /* follow link or just place caret */
    if (ti->aref && (ti->aref->href || ti->aref->flags & rid_aref_LABEL))
    {
	BOOL follow_link = TRUE;
        if (config_display_time_activate)
        {
            backend_update_link_activate(doc, ti, 1);
            follow_link = wait_for_release(config_display_time_activate);
            backend_update_link_activate(doc, ti, 0);
	}

	if (follow_link)
	    frontend_complain(antweb_handle_url(doc, ti->aref, qbuffer,
						bb & wimp_BRIGHT ? "_blank" : ti->aref->target));
    }
    else
    {
        antweb_default_caret(doc, FALSE);

        /* pdh: right, selection for clipboarding comes here */
        backend_set_highlight( doc, ti, TRUE );
    }

#endif /* BUILDERS */

    return NULL;
}

void oimage_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    rid_text_item_image *tii = (rid_text_item_image *) ti;

    if (tii->alt)
	fprintf(f, "<%s>", tii->alt);
    else
	fputs("<IMAGE>", f);
}

void *oimage_image_handle(rid_text_item *ti, antweb_doc *doc, int reason)
{
    rid_text_item_image *tii = (rid_text_item_image *) ti;

    switch (reason)
    {
    case object_image_HANDLE:
    case object_image_OBJECT:
	return tii->im;

    case object_image_ABORT:
	oimage_flush_image(tii->im);
	break;

    case object_image_BOX:
    {
	static wimp_box box;
	box.x0 = tii->hgap;
	box.x1 = -box.x0;
	box.y0 = tii->vgap;
	box.y1 = -box.y0;
	return &box;
    }
    }

    return NULL;
}

void oimage_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    rid_text_item_image *tii = (rid_text_item_image *) ti;
    int is_link = (ti->aref != NULL && ti->aref->href != NULL);
    int bw = tii->bwidth;
    wimp_box tb;
    wimp_paletteword palw;

    palw = render_get_colour(render_link_colour(ti, doc), doc);

    if (tii->im)
    {
	tb.x0 = x + 2*bw + 2*tii->hspace;
	tb.y0 = y - ti->max_down + 2*bw + 2*tii->vspace;
	tb.x1 = x + ti->width - 2*bw - 2*tii->hspace;
	tb.y1 = y + ti->max_up - 2*bw - 2*tii->vspace;

	image_save_as_draw(tii->im, fh, &tb, fileoff);
    }

    if (tii->im == NULL || (bw && is_link))
    {
	tb.x0 = x;
	tb.y0 = y - ti->max_down;
	tb.x1 = x + ti->width;
	tb.y1 = y + ti->max_up;

	if (bw == 0)
	    bw = 1;

	df_write_border(fh, &tb, palw, 2*bw, fileoff);
    }

    df_stretch_bb(bb, &tb);
#endif /* BUILDERS */
}

int oimage_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box)
{
    if (box)
    {
	memset(box, 0, sizeof(*box));
#if 0
	rid_text_item_image *tii = (rid_text_item_image *)ti;

	box->x0 =   tii->hspace*2 + tii->bwidth*2;
	box->x1 = - box->x0;
	box->y0 =   tii->vspace*2 + tii->bwidth*2;
	box->y1 = - box->y0;
#endif
    }
    return TRUE;
}

/* ----------------------------------------------------------------------------- */

/* eof oimage.c */
