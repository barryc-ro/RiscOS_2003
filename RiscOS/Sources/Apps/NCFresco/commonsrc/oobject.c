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
#include "msgs.h"

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

     OBJDBG(("oobject_size_allocate: ti%p fwidth %d\n", ti, fwidth));
/*     OBJDBG(("oobject: sizing line %p fwidth %d floats %p\n", ti->line,  */
/* 	    ti->line && ti->line->st ? ti->line->st->fwidth : 0,  */
/* 	    ti->line ? ti->line->floats : 0)); */

    /* no border specified implies thin border - but only if this is a link */
    obj->bwidth = get_value(ti, &obj->userborder, tio->base.aref || obj->usemap ? 2 : 0, fwidth);

    obj->hspace = get_value(ti, &obj->userhspace, 0, fwidth);
    obj->vspace = get_value(ti, &obj->uservspace, 0, fwidth);

    antweb_doc_ensure_font( doc, ALT_FONT );
    
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

	oimage_size_image(doc, obj->standby, &obj->userwidth, &obj->userheight, obj->iflags, ti, doc->scale_value, fwidth, &width, &height);
	break;
    }

#ifndef BUILDERS
    case rid_object_type_PLUGIN:
	/* Convert from user values to OS unit values */
	width = get_value(ti, &obj->userwidth, 128, fwidth);
	height = get_value(ti, &obj->userheight, 128, fwidth);

	OBJDBG(("oobject_size_allocate: %dx%d\n", width, height));
	
	if ((obj->classid_ftype != -1 || obj->data_ftype != -1) &&
	    (config_sound_background || width > 2 || height > 2))	/* don't start if invisible(ish) and bgsound configured off */
	{
	    wimp_box box;

	    if (obj->state.plugin.pp == NULL)
	    {
		/* if we haven't started the plugin then fillin bogus off-screen values */
		box.x0 = 0;
		box.y0 = 0x1000;
	    }
	    else
	    {
		/* if we have started then fillin the current bottom left position (should we go from base line?) */
		box.x0 = obj->state.plugin.box[0];
		box.y0 = obj->state.plugin.box[1];
	    }

	    /* always fill in the width and height in case fwidth has changed */
	    box.x1 = box.x0 + width;
	    box.y1 = box.y0 + height;

	    /* move or open */
	    objects_set_position(doc, ti, &box);
	}
	else
	{
	    oimage_size_image(doc, obj->standby, &obj->userwidth, &obj->userheight, obj->iflags, ti, doc->scale_value, fwidth, &width, &height);
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

    if (width < 1)
	width = 1;
    if (height < 1)
	height = 1;
    
    ti->width = width;
    ti->pad = 0;

    ti->max_up = oimage_decode_align(ti->line, obj->iflags, height);
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
    BOOL do_plinth = TRUE;
    char *alt, alt_buf[128];

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    if (update == object_redraw_BACKGROUND)
	return;
    
    bw = obj->bwidth;

    bbox.x0 = hpos + obj->hspace + bw;
    bbox.y0 = bline - ti->max_down + obj->vspace + bw;
    bbox.x1 = bbox.x0 + ti->width - obj->hspace*2 - bw*2;
    bbox.y1 = bbox.y0 + ti->max_up + ti->max_down - obj->vspace*2 - bw*2;

/*  OBJDBG(("oobject: plugin type %d\n", obj->type)); */

    alt = NULL;
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
	    alt = obj->standby;
	}
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	break;

    case rid_object_type_PLUGIN:
    case rid_object_type_UNKNOWN:
	/* use standby text or mime type for the alt text */
	alt = obj->standby;
	if (!alt)
	{
	    char *type, *file, *base_file;
	    if (obj->classid_ftype != -1)
	    {
		type = get_plugin_type_name(obj->classid_ftype);
		if (!type)
		    type = obj->classid_mime_type;

		base_file = obj->classid;
	    }
	    else
	    {
		type = get_plugin_type_name(obj->data_ftype);
		if (!type)
		    type = obj->data_mime_type;

		base_file = obj->data;
	    }

	    file = strrchr(base_file, '/');
	    if (file)
		file++;
	    else
		file = base_file;

	    if (obj->state.plugin.pp == NULL)
		strcpy(alt_buf, msgs_lookup("noplugin"));
	    else
		alt_buf[0] = 0;
	    if (type)
	    {
		strlencat(alt_buf, " ", sizeof(alt_buf));
		strlencat(alt_buf, type, sizeof(alt_buf));
	    }
	    if (file)
	    {
		strlencat(alt_buf, " ", sizeof(alt_buf));
		strlencat(alt_buf, file, sizeof(alt_buf));
	    }
	    alt = alt_buf;
	}

	do_plinth = ti->width != 0;
	break;
    }

    if (do_plinth)
	render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
		  bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0, doc);

    OBJDBG(("oobject_redraw: alt text '%s' (standby '%s' mimes '%s'/'%s')\n",
	    strsafe(alt), strsafe(obj->standby), strsafe(obj->classid_mime_type), strsafe(obj->data_mime_type)));

    if (alt)
	oimage_render_text(ti, doc, fs, &bbox, alt);

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

    switch (reason)
    {
    case object_image_HANDLE:
	return obj->type == rid_object_type_IMAGE ? obj->state.image.im : NULL;

    case object_image_OBJECT:
	return obj->type == rid_object_type_IMAGE ? obj->state.image.im :
	    obj->type == rid_object_type_PLUGIN ? obj->state.plugin.pp :
		NULL;

    case object_image_ABORT:
	if (obj->type == rid_object_type_IMAGE)  
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
