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

#include "objects.h"
#include "pluginfn.h"

/* ----------------------------------------------------------------------------- */

#define PLINTH_PAD	16
#define ALT_FONT	(WEBFONT_SIZE(2) + WEBFONT_FLAG_FIXED)
#define NONE_STRING	"[IMAGE]"

/* ----------------------------------------------------------------------------- */

/* 
 * FIXME: This should cope with %age values
 */

static int get_value(rid_text_item *ti, rid_stdunits *val, int def)
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
	return 0;
    }
    return 0;
}

#ifndef BUILDERS
static void draw_text(rid_text_item *ti, antweb_doc *doc, object_font_state *fs, wimp_box *bbox, const char *text)
{
    if (text)
    {
	wimp_box box;
	struct webfont *wf;
	int tfc;

	wf = &webfonts[ALT_FONT];

	if (fs->lf != wf->handle)
	{
	    fs->lf = wf->handle;
	    font_setfont(fs->lf);
	}

	if (fs->lfc != (tfc = render_link_colour(ti, doc) ) )
	{
	    fs->lfc = tfc;
	    render_set_font_colours(fs->lfc, render_colour_BACK, doc);
	}

	box.x0 = bbox->x0 + PLINTH_PAD/2;
	box.x1 = bbox->x1 - PLINTH_PAD/2;
	box.y0 = bbox->y0 + PLINTH_PAD/2;
	box.y1 = bbox->y1 - PLINTH_PAD/2;
	write_text_in_box(fs->lf, text, &box);
    }
}
#endif

#ifndef BUILDERS
static void draw_border(rid_text_item *ti, antweb_doc *doc, wimp_box *bbox, int bw)
{
    int dx = frontend_dx;
    int dy = frontend_dy;
    int x, y, w, h;

    render_set_colour(render_link_colour(ti, doc), doc);

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
#endif

/* ----------------------------------------------------------------------------- */

/*
 * OBJECT METHODS
 */

void oobject_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    int width, height;

    OBJDBG(("oobject: sizing line %p fwidth %d floats %p\n", ti->line, 
	    ti->line && ti->line->st ? ti->line->st->fwidth : 0, 
	    ti->line ? ti->line->floats : 0));

    /* Convert from user values to OS unit values */
    obj->ww = get_value(ti, &obj->userwidth, 128);
    obj->hh = get_value(ti, &obj->userheight, 128);

    /* no border specified implies thin border - but only if this is a link */
    obj->bwidth = get_value(ti, &obj->userborder, tio->base.aref || obj->usemap ? 2 : 0);

    obj->hspace = get_value(ti, &obj->userhspace, 0);
    obj->vspace = get_value(ti, &obj->uservspace, 0);

    /* Get values for text_item */
    width = obj->ww;
    height = obj->hh;

    width += (obj->bwidth + obj->hspace) * 2;
    height += (obj->bwidth + obj->vspace) * 2;

    ti->width = width;
    ti->pad = 0;

    if (obj->iflags & rid_image_flag_ATOP)
    /* TOP and TEXTTOP */
    {
	ti->max_up = webfonts[WEBFONT_BASE].max_up;
    }
    else if (obj->iflags & rid_image_flag_ABOT)
    {
	if (obj->iflags & rid_image_flag_ABSALIGN)
	/* ABSBOTTOM */
	{
	    ti->max_up = height - webfonts[WEBFONT_BASE].max_down;
	}
	else
	/* BOTTOM and BASELINE */
	{
	    ti->max_up = height;
	}
    }
    else if (obj->iflags & rid_image_flag_ABSALIGN)
    /* ABSMIDDLE */
    {
	ti->max_up = (webfonts[WEBFONT_BASE].max_up -
		      webfonts[WEBFONT_BASE].max_down + height) >> 1;
    }
    else
    /* MIDDLE */
    {
	ti->max_up = height >> 1;
    }
    ti->max_down = height - ti->max_up;

    OBJDBG(("oobject: size to w=%d,u=%d,d=%d\n", ti->width, ti->max_up, ti->max_down));

#if 1
    if (obj->type == rid_object_type_PLUGIN)
    {
	if (obj->state.plugin.pp == NULL)
	{
	    obj->state.plugin.pp = plugin_new(obj, doc);

	    /* position plugin initially off screen */

	    if (!objects_bbox(doc, ti, (wimp_box *)obj->state.plugin.box))
	    {
		obj->state.plugin.box[0] = 0;
		obj->state.plugin.box[1] = 0x1000;
		obj->state.plugin.box[2] = obj->state.plugin.box[0] + obj->ww;
		obj->state.plugin.box[3] = obj->state.plugin.box[1] + obj->hh;
	    }

	    plugin_send_open(obj->state.plugin.pp, (wimp_box *)obj->state.plugin.box);

	    if (doc->object_handler_count++ == 0)
		frontend_message_add_handler(plugin_message_handler, doc);
	}
    }
#endif
}


void oobject_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    wimp_box bbox;
    int bw;
    BOOL do_alt = FALSE, do_plinth = TRUE;

    bw = obj->bwidth;

    bbox.x0 = hpos + obj->hspace + bw;
    bbox.y0 = bline - ti->max_down + obj->vspace + bw;
    bbox.x1 = bbox.x0 + ti->width - obj->hspace*2 - bw*2;
    bbox.y1 = bbox.y0 + ti->max_up + ti->max_down - obj->vspace*2 - bw*2;

    OBJDBG(("oobject: plugin type %d\n", obj->type));

    switch (obj->type)
    {
    case rid_object_type_IMAGE:
	break;

    case rid_object_type_INTERNAL:
	break;

    case rid_object_type_FRAME:
	break;

    case rid_object_type_PLUGIN:
    {
#if 0
	wimp_box box;

	objects_bbox(doc, ti, &box);

	if (obj->state.plugin.pp == NULL)
	{
	    obj->state.plugin.pp = plugin_new(obj, doc);

	    *(wimp_box *)obj->state.plugin.box = box;
	    plugin_send_open(obj->state.plugin.pp, (wimp_box *)&box);

	    if (doc->object_handler_count++ == 0)
		frontend_message_add_handler(plugin_message_handler, doc);
	}
	else if (box.x0 != obj->state.plugin.box[0] || box.x1 != obj->state.plugin.box[2] || 
	    box.y0 != obj->state.plugin.box[1] || box.y1 != obj->state.plugin.box[3])
	{
	    *(wimp_box *)obj->state.plugin.box = box;
	    plugin_send_reshape(obj->state.plugin.pp, (wimp_box *)&box);
	}
#endif
	do_alt = TRUE;

	break;
    }

    case rid_object_type_UNKNOWN:
	break;
    }

    if (do_plinth)
	render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
		  bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0, doc);

    if (do_alt)
	draw_text(ti, doc, fs, &bbox, obj->standby);

    if ((ti->flag & rid_flag_SELECTED) || bw)
	draw_border(ti, doc, &bbox, bw);

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
    rid_text_item_object *tio = (rid_text_item_object *) ti;
    rid_object_item *obj = tio->object;
    switch (obj->type)
    {
    case rid_object_type_IMAGE:
	/* Pass to oimage code */
	break;

    case rid_object_type_INTERNAL:
	/* Pass to oimage code ? */
	break;

    case rid_object_type_FRAME:
	/* Won't be received */
	break;

	/* This is received when the highlight is moved to the plugin and enter pressed */
    case rid_object_type_PLUGIN:
	plugin_send_focus(obj->state.plugin.pp);
	break;

    case rid_object_type_UNKNOWN:
	break;
    }

    return "";			/* Follow links, no fuss at all */
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
