/* -*-c-*- */
/* CHANGE LOG

 * 13/8/96: SJM: created

 */

/* oobject.c */

/* Methods for OBJECT (plugin) objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "swis.h"
#include "drawftypes.h"
#include "wimp.h"
#include "font.h"
#include "bbc.h"

#include "interface.h"
#include "antweb.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "util.h"
#include "render.h"
#include "rcolours.h"
#include "filetypes.h"

#include "object.h"
#include "version.h"
#include "dfsupport.h"

#include "images.h"
#include "objects.h"
#include "pluginfn.h"
#include "imagemap.h"
#include "oimage.h"
#include "gbf.h"

/* ----------------------------------------------------------------------------- */

#define PLINTH_PAD	16
#define ALT_FONT	(WEBFONT_SIZE(2) + WEBFONT_FLAG_FIXED)
#define NONE_STRING	"[IMAGE]"

/* ----------------------------------------------------------------------------- */

/* 
 */

static int get_value(rid_text_item *ti, rid_stdunits *val, int def, int fwidth)
{
    switch (val->type)
    {
    case value_none:
	return def;
    case value_integer:
	return val->u.i*2;
    case value_absunit:
	return (int)val->u.f;
    case value_pcunit:
	return (int)(val->u.f*fwidth/100);
    }
    return 0;
}


static BOOL oobject_renderable(rid_object_item *obj, antweb_doc *doc)
{
    int fl;
    if (obj->state.image.im == NULL)
	return FALSE;

    if (doc->flags & doc_flag_NO_PICS)
	return FALSE;
    
    image_info((image) obj->state.image.im, 0, 0, 0, &fl, 0, 0);
    if (fl & image_flag_REALTHING)
	return TRUE;

    if (obj->standby == NULL && ((doc->flags & doc_flag_DEFER_IMAGES) != 0 || (obj->userheight.type == value_none && obj->userwidth.type == value_none)))
	return TRUE;
    
    return FALSE;
}

/* ----------------------------------------------------------------------------- */

/*
 * OBJECT METHODS
 */

void oobject_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth)
{
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    int width, height;

     OBJDBG(("oobject_size_allocate: ti%p fwidth %d\n", ti, width));
/*     OBJDBG(("oobject: sizing line %p fwidth %d floats %p\n", ti->line,  */
/* 	    ti->line && ti->line->st ? ti->line->st->fwidth : 0,  */
/* 	    ti->line ? ti->line->floats : 0)); */

    /* no border specified implies thin border - but only if this is a link */
    obj->bwidth = get_value(ti, &obj->userborder, tio->base.aref || obj->usemap ? 2 : 0, fwidth);

    obj->hspace = get_value(ti, &obj->userhspace, 0, fwidth);
    obj->vspace = get_value(ti, &obj->uservspace, 0, fwidth);

    switch (obj->type)
    {
    case rid_object_type_IMAGE:
    {
	image_flags fl;
	if (obj->state.image.im == NULL)
	    obj->state.image.im = oimage_fetch_image(doc, obj->data, obj->userwidth.type == value_none || obj->userheight.type == value_none);
   
	image_info((image) obj->state.image.im, &width, &height, 0, &fl, 0, 0);

	if (fl & image_flag_REALTHING)
	    obj->iflags |= rid_image_flag_REAL;

	oimage_size_image(obj->standby, &obj->userwidth, &obj->userheight, obj->iflags, config_defer_images, doc->scale_value, fwidth, &width, &height);
	break;
    }

#ifndef BUILDERS
    case rid_object_type_PLUGIN:
	/* Convert from user values to OS unit values */
	width = get_value(ti, &obj->userwidth, 0, fwidth);
	height = get_value(ti, &obj->userheight, 0, fwidth);

	if (obj->state.plugin.pp == NULL &&
	    (obj->classid_ftype != -1 || obj->data_ftype != -1))
/* 	    width > 0 && height > 0) */
	{
	    obj->state.plugin.pp = plugin_new(obj, doc, ti);
	    
	    /* position plugin initially off screen */
	    if (!objects_bbox(doc, ti, (wimp_box *)obj->state.plugin.box))
	    {
		obj->state.plugin.box[0] = 0;
		obj->state.plugin.box[1] = 0x1000;
		obj->state.plugin.box[2] = obj->state.plugin.box[0] + width;
		obj->state.plugin.box[3] = obj->state.plugin.box[1] + height;
	    }

	    plugin_send_open(obj->state.plugin.pp, (wimp_box *)obj->state.plugin.box, 0 /* open_flags */);
	    
	    if (doc->object_handler_count++ == 0)
		frontend_message_add_handler(plugin_message_handler, doc);
	}
	else
	{
	    oimage_size_image(obj->standby, &obj->userwidth, &obj->userheight, obj->iflags, config_defer_images, doc->scale_value, fwidth, &width, &height);
	}
	break;
#endif
	
    default:
	/* Get values for text_item */
	width = get_value(ti, &obj->userwidth, 0, fwidth);
	height = get_value(ti, &obj->userheight, 0, fwidth);
	break;
    }
    
    width += (obj->bwidth + obj->hspace) * 2;
    height += (obj->bwidth + obj->vspace) * 2;

    ti->width = width;
    ti->pad = 0;

    ti->max_up = oimage_decode_align(obj->iflags, height);
    ti->max_down = height - ti->max_up;

    OBJDBG(("oobject: size to w=%d,u=%d,d=%d\n", ti->width, ti->max_up, ti->max_down));
}

void oobject_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    oobject_size_allocate(ti, rh, doc, 0);
}

void oobject_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    wimp_box bbox;
    int bw;
    BOOL do_alt = FALSE, do_plinth = TRUE;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    bw = obj->bwidth;

    bbox.x0 = hpos + obj->hspace + bw;
    bbox.y0 = bline - ti->max_down + obj->vspace + bw;
    bbox.x1 = bbox.x0 + ti->width - obj->hspace*2 - bw*2;
    bbox.y1 = bbox.y0 + ti->max_up + ti->max_down - obj->vspace*2 - bw*2;

/*  OBJDBG(("oobject: plugin type %d\n", obj->type)); */

    switch (obj->type)
    {
    case rid_object_type_IMAGE:
	if (oobject_renderable(obj, doc))
	{
	    int oox = ox, ooy = oy;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif
	    image_render((image) obj->state.image.im, bbox.x0, bbox.y0,
			 (bbox.x1 - bbox.x0)/2,
			 (bbox.y1 - bbox.y0)/2,
			 doc->scale_value, antweb_render_background, doc,
			 oox, ooy);

	    do_plinth = FALSE;
	}
	else
	{
	    do_alt = TRUE;
	}
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	break;

    case rid_object_type_PLUGIN:
	do_alt = TRUE;
	do_plinth = ti->width != 0;
	break;

    case rid_object_type_UNKNOWN:
	break;
    }

    if (do_plinth)
	render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
		  bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0, doc);

    if (do_alt)
	oimage_render_text(ti, doc, fs, &bbox, obj->standby);
#endif /* BUILDERS */
}

void oobject_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
#ifndef BUILDERS
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;

    switch (obj->type)
    {
    case rid_object_type_IMAGE:

        IMGDBG(("oobject_dispose calls image_lose, doc=%p, doc->parent=%p\n", doc, doc->parent ));

	image_loose((image) obj->state.image.im, &antweb_doc_image_change, doc);
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	break;

    case rid_object_type_PLUGIN:
	if (obj->state.plugin.pp) /* must check here to get handler_count correct */
	{
	    plugin_destroy(obj->state.plugin.pp);
	    obj->state.plugin.pp = NULL;

	    if (--doc->object_handler_count == 0)
		frontend_message_remove_handler(plugin_message_handler, doc);
	}
	break;

    case rid_object_type_UNKNOWN:
	break;
    }
#endif
}

char *oobject_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    switch (obj->type)
    {
    case rid_object_type_IMAGE:
    {
	int flags;

	image_info((image) obj->state.image.im, NULL, NULL, NULL, &flags, NULL, NULL);

	/* Only show image if the ALT key is held down */
	if ((flags & image_flag_DEFERRED) && kbd_pollalt())
	{
	    frontend_complain(image_flush((image) obj->state.image.im, 0));
	    return NULL;
	}
    
	if (obj->usemap || obj->map)
	{
	    /* Remember that the y is in work area co-ordinates */
	    x = x - obj->bwidth - obj->hspace;
	    y = ((ti->max_up) - obj->bwidth - obj->vspace) - y;

	    image_os_to_pixels((image)obj->state.image.im, &x, &y, doc->scale_value);

	    if (oimage_handle_usemap(ti, doc, x, y, bb,
				     obj->map ? obj->map : imagemap_find_map(rh, obj->usemap)))
		return NULL;
	}

	/* follow link or just place caret */
	if (ti->aref && ti->aref->href)
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
						    bb & wimp_BRIGHT ? "_blank" : ti->aref->target));
	}
	else
	    antweb_default_caret(doc, FALSE);

	return NULL;
    }

    case rid_object_type_INTERNAL:
	/* Pass to oimage code ? */
	break;

    case rid_object_type_FRAME:
	/* Won't be received */
	break;

	/* This is received when the highlight is moved to the plugin and enter pressed */
#ifndef BUILDERS
    case rid_object_type_PLUGIN:
	if (!plugin_send_focus(obj->state.plugin.pp))
	    sound_event(snd_WARN_BAD_KEY);
	break;
#endif /* BUILDERS */

    case rid_object_type_UNKNOWN:
	break;
    }

#endif
    return "";			/* Follow links, no fuss at all */
}

void *oobject_image_handle(rid_text_item *ti, antweb_doc *doc, int reason)
{
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;

    if (obj->type == rid_object_type_IMAGE)  switch (reason)
    {
    case object_image_HANDLE:
	return obj->state.image.im;

    case object_image_ABORT:
	oimage_flush_image(obj->state.image.im);
	break;

    case object_image_BOX:
    {
	static wimp_box box;
	box.x0 = obj->hspace + obj->bwidth;
	box.x1 = -box.x0;
	box.y0 = obj->vspace + obj->bwidth;
	box.y1 = -box.y0;
	return &box;
    }
    }

    return NULL;
}


void oobject_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    switch (obj->type)
    {
    case rid_object_type_IMAGE:
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	fprintf(f, "[FRAME]");
	break;

    case rid_object_type_PLUGIN:
	fprintf(f, "[OBJECT]");
	break;

    case rid_object_type_UNKNOWN:
	break;
    }
}

void oobject_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    switch (obj->type)
    {
    case rid_object_type_IMAGE:
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	break;

    case rid_object_type_PLUGIN:
	break;

    case rid_object_type_UNKNOWN:
	break;
    }

}

/* ----------------------------------------------------------------------------- */

/* eof oobject.c */
