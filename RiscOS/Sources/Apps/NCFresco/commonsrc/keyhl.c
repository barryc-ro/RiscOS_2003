/* > keyhl.c

 *

 */


#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "interface.h"
#include "memwatch.h"
#include "webfonts.h"

#include "antweb.h"
#include "object.h"
#include "rid.h"
#include "stream.h"

#ifndef LINK_DEBUG
#define LINK_DEBUG 0
#endif

#if LINK_DEBUG
#define LKDBG(a) fprintf a
#else
#define LKDBG(a)
#endif

/* ----------------------------------------------------------------------------- */

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

/* ----------------------------------------------------------------------------- */

#if LINK_SORT
static int link_sort_compare_x(const void *o1, const void *o2)
{
    const antweb_selection_descr **pl1 = (const antweb_selection_descr **)o1;
    const antweb_selection_descr **pl2 = (const antweb_selection_descr **)o2;
    return (*pl1)->bbox.x0 - (*pl2)->bbox.x0;
}

static int link_sort_compare_y(const void *o1, const void *o2)
{
    const antweb_selection_descr **pl1 = (const antweb_selection_descr **)o1;
    const antweb_selection_descr **pl2 = (const antweb_selection_descr **)o2;
    return (*pl1)->bbox.y0 - (*pl2)->bbox.y0;
}
#endif

/* exported internally to rebuild list whenever a reformat occurs */

void antweb_build_selection_list(be_doc doc)
{
    be_item ti;
    antweb_selection_descr *link_list, *link;
#if LINK_SORT
    antweb_selection_descr **sort_list, **sort;
#endif
    int count;

    LKDBG((stderr, "antweb_build_selection_list(): doc %p old list %p\n", doc, doc->selection_list.list));

    /* free any old list */
    mm_free(doc->selection_list.list);
    
    /* count how many we have */
    count = 0;
    for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if (match_item(ti, be_link_ONLY_CURRENT, NULL))
	    count++;
    }

    /* allocate space and fill in values */
    link_list = mm_calloc(sizeof(*link_list), count);
    link = link_list;

#if LINK_SORT
    /* temporary list for whilst sorting into order */
    sort_list = mm_calloc(sizeof(*sort_list), count);
    sort = sort_list;
#endif
    for (ti = doc->rh->stream.text_list; ti; ti = rid_scanfr(ti))
    {
	if (match_item(ti, be_link_ONLY_CURRENT, NULL))
	{
	    /* FIXME: this is rather inefficient - need a better way */
	    backend_doc_item_bbox(doc, ti, &link->bbox);
	    
	    link->item.tag = doc_selection_tag_TEXT;
	    link->item.data.text = ti;

	    link++;

#if LINK_SORT
	    /* fill in the temp value to be sorted */
	    *sort++ = link;
#endif
	}
    }

#if LINK_SORT
    /* order them horizontally */
    qsort(sort_list, sizeof(*sort_list), count, link_sort_compare_x);

    sort = sort_list;

    ll->left = *sort++;
    link = ll->left;
    for (i = 1; i < count; i++)
    {
	link->next = *sort++;	/* set the next pointer */
	link = link->next_x;	/* step onto the pointer */
    }
    link->next_x = NULL;
    

    /* fillin sort array again */
    sort = sort_list;
    link = link_list;
    for (i = 0; i < count; i++)
	*sort++ = link;
    

    /* order them vertically */
    qsort(sort_list, sizeof(*sort_list), count, link_sort_compare_y);

    sort = sort_list;

    ll->top = *sort++;
    link = ll->top;
    for (i = 1; i < count; i++)
    {
	link->next = *sort++;	/* set the next pointer */
	link = link->next_y;	/* step onto the pointer */
    }
    link->next_y = NULL;
#endif
    
    /* write out vars */
    doc->selection_list.count = count;
    doc->selection_list.list = link_list;

    LKDBG((stderr, "antweb_build_selection_list(): count %d new list %p\n", count, doc->selection_list.list));
}

static antweb_selection_descr *antweb_highlight_scan_xy(be_doc doc, const antweb_selection_t *initial, int from_x, int from_y, int flags)
{
    antweb_selection_descr *link;
    rid_aref_item *start_aref;

    int min_dist = INT_MAX;
    antweb_selection_descr *min_link = NULL;

    int i;
    
    LKDBG((stderr, "antweb_highlight_scan_xy: doc %p initial %p from %d,%d flags %x\n", doc, initial, from_x, from_y, flags));

    start_aref = initial && initial->tag == doc_selection_tag_AREF ? initial->data.aref : NULL;

    LKDBG((stderr, "antweb_highlight_scan_xy: start_aref %p\n", start_aref));

    for (i = 0, link = doc->selection_list.list; i < doc->selection_list.count; i++, link++)
    {
	int dist = -1;
	
	/* match the correct edge of the box for the direction being travelled */
	if (flags & be_link_VERT)
	{
	    if (from_x > link->bbox.x0 && from_x <= link->bbox.x1)
		dist = flags & be_link_BACK ? link->bbox.y0 - from_y : from_y - link->bbox.y1;
	}
	else
	{
	    if (from_y > link->bbox.y0 && from_y <= link->bbox.y1)
		dist = (flags & be_link_BACK) == 0 ? link->bbox.x0 - from_x : from_x - link->bbox.x1;
	}

	if (dist > 0 &&		/* dist == 0 implies the one we started on so ignore */
	    dist < min_dist && 
	    (link->item.tag != doc_selection_tag_TEXT || link->item.data.text->aref != start_aref)) /* can't be part of same link */
	{
	    min_dist = dist;
	    min_link = link;
	}
    }

    LKDBG((stderr, "antweb_highlight_scan_xy: closest link %p at %d units aref %p\n", min_link, min_dist,
	   min_link->item.tag == doc_selection_tag_TEXT ? min_link->item.data.text->aref : NULL));

    return min_link;
}

static antweb_selection_descr *antweb_highlight_scan_link(be_doc doc, antweb_selection_t *initial, int flags)
{
    int x, y, w, h;
    be_item item = NULL;
    wimp_box bbox;

    LKDBG((stderr, "antweb_highlight_scan_link: doc %p selection %p item %p flags %x\n", doc, initial, initial ? initial->data.text : 0, flags));

    if (initial == NULL)
	return NULL;
    
    /* decide which item/position to start from */
    switch (initial->tag)
    {
    case doc_selection_tag_AREF:
	item = initial->data.aref->first;
#if 0
	if ((flags & be_link_BACK) == 0)
	{
	    while (item->next && item->next->aref == initial->data.aref)
		item = item->next;
	}
#endif
	break;

    case doc_selection_tag_TEXT:
	item = initial->data.text;
	break;

    case doc_selection_tag_AREA:
	item = initial->data.map.item;
	break;
    }

    if (item == NULL)
	return NULL;
    
    backend_doc_item_bbox(doc, item, &bbox);

#if 1
    w = item->width/2;
    if (w > webfonts[WEBFONT_BASE].space_width)
	w = webfonts[WEBFONT_BASE].space_width;
    x = bbox.x0 + w;
#else
    x = (bbox.x0 + bbox.x1) / 2;
#endif
    
    h = item->max_up + item->max_down;
    if (h > webfonts[WEBFONT_BASE].max_up)
	h = webfonts[WEBFONT_BASE].max_up;
    y = bbox.y1 - h/2;
    
    return antweb_highlight_scan_xy(doc, initial, x, y, flags);
}

/* ----------------------------------------------------------------------------- */

static be_item backend_highlight_link_2D(be_doc doc, be_item item, int flags)
{
    antweb_selection_t sel;
    antweb_selection_descr *link;
    be_item new_item;

    LKDBG((stderr, "backend_highlight_link_2D: doc %p item %p flags %x\n", doc, item, flags));

    if (item == NULL)
    {
	link = antweb_highlight_scan_xy(doc, NULL,
					flags & be_link_VERT ? -0x4000 : flags & be_link_BACK ? 0x4000 : -0x4000,
					flags & be_link_VERT ? (flags & be_link_BACK ? 0x4000 : -0x4000) : -0x4000,
					flags);
    }
    else
    {
	if (item->aref)
	{
	    sel.tag = doc_selection_tag_AREF;
	    sel.data.aref = item->aref;
	}
	else
	{
	    sel.tag = doc_selection_tag_TEXT;
	    sel.data.text = item;
	}

	link = antweb_highlight_scan_link(doc, &sel, flags);
    }

    LKDBG((stderr, "backend_highlight_link_2D: return link %p tag %d item %p\n", link, link ? link->item.tag : 0, link ? link->item.data.text : 0));

    new_item = NULL;
    if (link) switch (link->item.tag)
    {
    case doc_selection_tag_TEXT:
	new_item = link->item.data.text;
	if (new_item->aref)
	    new_item = link->item.data.text->aref->first;
	break;

    case doc_selection_tag_AREA:
	new_item = link->item.data.map.item;
	break;
    }
    
    return new_item;
}				

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

/* ----------------------------------------------------------------------------- */

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


/* ----------------------------------------------------------------------------- */
/* exported to frontend via interface.h */
/* ----------------------------------------------------------------------------- */

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

    if ((flags & (be_link_ONLY_CURRENT|be_link_TEXT)) == 0)
	ti = backend_highlight_link_2D(doc, item, flags);
    else
    {
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

/* ----------------------------------------------------------------------------- */

/*
 * if selected is -1 then it toggles the state of the selected bit
 */

be_item backend_update_link(be_doc doc, be_item item, int selected)
{
    be_item ti;
    BOOL changed;

    if (item == NULL)
        return NULL;

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

	if (changed)
	    be_update_item_highlight(doc, ti);
    }

    return item->aref->first;
}

/* ----------------------------------------------------------------------------- */

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

/* ----------------------------------------------------------------------------- */

/* eof keyhl.c */
