/* -*-c-*- */

/* objects.c */

/* CHANGE LOG

 * 13/8/96: SJM: created

 */

#include "antweb.h"
#include "images.h"
#include "objects.h"
#include "gbf.h"

#include "filetypes.h"
#include "interface.h"

#include "rid.h"
#include "pluginfn.h"

rid_object_type objects_type_test(int ftype)
{
    /* If it is an image we can handle it */
    if (image_type_test(ftype))
	return rid_object_type_IMAGE;

    /* We should be abelt o handle these via floating frames eventually
    if (ftype == FILETYPE_TEXT || ftype == FILETYPE_HTML || ftype == FILETYPE_GOPHER)
	return rid_object_type_FRAME;
     */


    /* Ask the frontedn what can be handled */
#ifndef BUILDERS
    if (frontend_plugin_handle_file_type(ftype))
	return rid_object_type_PLUGIN;
#endif

    return rid_object_type_UNKNOWN;
}

/* This returns false if we are nbot yet formatted enough to get a position */

BOOL objects_bbox(be_doc doc, be_item ti, wimp_box *box)
{
    rid_text_item_object *tio = (rid_text_item_object *)ti;
    rid_object_item *obj = tio->object;

    OBJDBGN(("objects_bbox: doc%p ti%p obj%p\n", doc, ti, obj));
    
    if (obj && backend_doc_item_bbox(doc, ti, box) == NULL)
    {
	OBJDBGN(("objects_bbox: got box\n"));

	box->x0 += obj->hspace + obj->bwidth;
	box->x1 -= obj->hspace + obj->bwidth;
	box->y0 += obj->vspace + obj->bwidth;
	box->y1 -= obj->vspace + obj->bwidth;

	return TRUE;
    }
    return FALSE;
}

void objects_set_position(be_doc doc, be_item ti, const wimp_box *box)
{
#ifndef BUILDERS
    rid_text_item_object *tio = (rid_text_item_object *)ti;
    rid_object_item *obj = tio->object;

    OBJDBGN(("objects_set_position: doc %p ti %p pp %p set to %d,%d,%d,%d\n", doc, ti, obj->state.plugin.pp, box->x0, box->y0, box->x1, box->y1));

    if (obj->state.plugin.pp == NULL)
    {
	if (!gbf_active(GBF_LOW_MEMORY))				/* don't start in low memory */
	{
	    obj->state.plugin.pp = plugin_new(obj, doc, ti);

	    *(wimp_box *)obj->state.plugin.box = *box;

	    plugin_send_open(obj->state.plugin.pp, box, 0 /* open_flags */);

	    if (doc->object_handler_count++ == 0)
		frontend_message_add_handler(plugin_message_handler, doc);
	}
    }
    else
    {
	if (box->x0 != obj->state.plugin.box[0] || box->x1 != obj->state.plugin.box[2] || 
	    box->y0 != obj->state.plugin.box[1] || box->y1 != obj->state.plugin.box[3])
	{
	    *(wimp_box *)obj->state.plugin.box = *box;

	    plugin_send_reshape(obj->state.plugin.pp, box);
	}
    }
#endif
}

void objects_check_movement(be_doc doc)
{
    rid_text_item *ti;

    OBJDBGN(("objects_check_movement: doc %p rh %p text_list %p\n", doc, doc->rh, doc->rh->stream.text_list));

    for (ti = rid_scan(doc->rh->stream.text_list, SCAN_THIS | SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT);
	 ti; 
	 ti = rid_scan(ti, SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT))
    {
	rid_text_item_object *tio = (rid_text_item_object *)ti;
	rid_object_item *obj = tio->object;

	if (obj->type == rid_object_type_PLUGIN)
	{
	    wimp_box box;
	    objects_bbox(doc, ti, &box);
	    objects_set_position(doc, ti, &box);
	}
    }
}

void objects_resize(be_doc doc, be_item ti, int width, int height)
{
    rid_text_item_object *tio = (rid_text_item_object *)ti;
    rid_object_item *obj = tio->object;

    if (obj->type == rid_object_type_PLUGIN)
    {
	obj->userwidth.u.i = width;
	obj->userwidth.type = value_integer;
	obj->userheight.u.i = height;
	obj->userheight.type = value_integer;
    
	antweb_doc_image_change(doc, obj->state.plugin.pp, image_cb_status_REFORMAT, NULL);
    }
}

/* eof objects.c */
