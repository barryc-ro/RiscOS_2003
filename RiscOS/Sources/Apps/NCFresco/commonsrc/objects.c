/* -*-c-*- */

/* objects.c */

/* CHANGE LOG

 * 13/8/96: SJM: created

 */

#include "antweb.h"
#include "images.h"
#include "objects.h"

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

    if (backend_doc_item_bbox(doc, ti, box) == NULL)
    {
	box->x0 += obj->hspace + obj->bwidth;
	box->x1 -= obj->hspace + obj->bwidth;
	box->y0 += obj->vspace + obj->bwidth;
	box->y1 -= obj->vspace + obj->bwidth;

	return TRUE;
    }
    return FALSE;
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

	if (obj->type == rid_object_type_PLUGIN && obj->state.plugin.pp)
	{
	    wimp_box box;
	    objects_bbox(doc, ti, &box);

	    /* new - check for change in box */
	    if (box.x0 != obj->state.plugin.box[0] || box.x1 != obj->state.plugin.box[2] || 
		box.y0 != obj->state.plugin.box[1] || box.y1 != obj->state.plugin.box[3])
	    {
		*(wimp_box *)obj->state.plugin.box = box;

#ifndef BUILDERS
		plugin_send_reshape(obj->state.plugin.pp, &box);
#endif
	    }
	}
    }
}

/* eof objects.c */
