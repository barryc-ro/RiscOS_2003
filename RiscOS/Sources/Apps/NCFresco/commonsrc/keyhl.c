/* > keyhl.c
 *
 */


#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "memcheck/MemCheck.h"

#include "interface.h"
#include "memwatch.h"
#include "webfonts.h"

#include "antweb.h"
#include "object.h"
#include "rid.h"
#include "stream.h"
#include "util.h"

/* ----------------------------------------------------------------------------- */

static int item_to_descriptor_index(be_doc doc, be_item ti)
{
    antweb_selection_descr *link;
    int i;
    for (i = 0, link = doc->selection_list.list; i < doc->selection_list.count; i++, link++)
    {
	if (link->item.tag == doc_selection_tag_TEXT &&
	    link->item.data.text.item == ti)
	{
	    return i;
	}
    }

    return -1;
}

static be_item selection_to_item(antweb_selection_t *link)
{
    be_item new_item = NULL;
    if (link) switch (link->tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
	new_item = link->data.text.item;
	if (new_item->aref)
	    new_item = link->data.text.item->aref->first;
	break;

    case doc_selection_tag_AREF:
	new_item = link->data.aref->first;
	break;

    case doc_selection_tag_MAP:
	new_item = link->data.map.item;
	break;
    }
    return new_item;
}

static be_item descriptor_to_item(antweb_selection_descr *link)
{
    return selection_to_item(link ? &link->item : NULL);
}

/* ----------------------------------------------------------------------------- */

static void aref_union_box(be_doc doc, rid_aref_item *aref, int flags, wimp_box *box_out)
{
    be_item item = aref->first;
    wimp_box box_first;

    backend_doc_item_bbox(doc, item, &box_first);
    *box_out = box_first;

    for (item = item->next; item && item->aref == aref; item = item->next)
    {
	wimp_box box;
	backend_doc_item_bbox(doc, item, &box);
	coords_union(&box, box_out, box_out);
    }

    /* this will force multi-line links to only extend the height of
       the first item when moving up and down. */
    if (flags & be_link_VERT)
    {
	box_out->y0 = box_first.y0;
	box_out->y1 = box_first.y1;
    }
    else
    {
/* 	box_out->x0 = box_first->x0; */
/* 	box_out->x1 = box_first->x1; */
    }
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
#elif 0
    if (flags & be_link_BACK)
    {
	/* if moving highlight backwards (up) then item is onscreen if top line is on screen */
        if (box.y0 >= bounds->y0 && box.y0 < bounds->y1)
            return TRUE;
    }
    else
    {
	/* if moving highlight forwards (down) then item is onscreen if bottom line is on screen */
	if (box.y1 > bounds->y0 && box.y1 <= bounds->y1)
            return TRUE;
    }
#elif 0
    /* changed to this variety to make it easier to get onto images that are larger than the screen */
    if (box.y0 < bounds->y0 && box.y1 > bounds->y1)
    {
	/* see if link encompasses screen */
	return TRUE;
    }
    else if (flags & be_link_BACK)
    {
	if (box.y0 >= bounds->y0 && box.y0 < bounds->y1)
	    return TRUE;
    }
    else
    {
	if (box.y1 > bounds->y0 && box.y1 <= bounds->y1)
	    return TRUE;
    }
#else
    return box.y0 < bounds->y1 && box.y1 >= bounds->y0;
#endif
    return FALSE;
}

/* ----------------------------------------------------------------------------- */

#define match_item_NONE		0
#define match_item_LINK		1
#define match_item_TEXT		2
#define match_item_NUMBERS	3

static BOOL match_item(be_item ti, int flags, rid_aref_item *aref)
{
    BOOL aref_valid = ti->aref && (ti->aref->href || (ti->aref->flags & rid_aref_LABEL));
    BOOL aref_changed_enough = ti->aref != aref || (flags & (be_link_INCLUDE_CURRENT | be_link_ONLY_CURRENT));

    if (ti->tag == rid_tag_TEXTAREA)
    {
	if (((rid_text_item_textarea *)ti)->area->base.tabindex == -1)
	    return match_item_NONE;

	if ((flags & be_link_TEXT) == 0)
	{
	    if (aref_valid && !aref_changed_enough)
		return match_item_NONE;
	}
	return match_item_TEXT;
    }

    if (ti->tag == rid_tag_INPUT)
    {
	rid_input_tag tag = ((rid_text_item_input *)ti)->input->tag;
	rid_input_item *tii = ((rid_text_item_input *)ti)->input;

	if (tii->base.tabindex == -1)
	    return match_item_NONE;

	if (flags & be_link_TEXT)
	    return tag == rid_it_TEXT || tag == rid_it_PASSWD ?
		((tii->flags & (rid_if_NUMBERS|rid_if_PBX)) == rid_if_NUMBERS ? match_item_NUMBERS : match_item_TEXT) :
	    match_item_NONE;

	if (aref_valid && !aref_changed_enough)
	    return match_item_NONE;

	return tag == rid_it_TEXT || tag == rid_it_PASSWD ?
		((tii->flags & (rid_if_NUMBERS|rid_if_PBX)) == rid_if_NUMBERS ? match_item_NUMBERS : match_item_TEXT) :
	    match_item_LINK;
    }

    if ((flags & be_link_TEXT) == 0)
    {
	if (ti->tag == rid_tag_OBJECT)
	{
	    rid_text_item_object *tio = (rid_text_item_object *)ti;
	    if (tio->object->type == rid_object_type_PLUGIN && ti->width > 4)
		return match_item_LINK;
	}

	if (ti->tag == rid_tag_SELECT)
	{
	    if (aref_valid && !aref_changed_enough)
		return match_item_NONE;
	    return match_item_LINK;
	}

	if (ti->tag == rid_tag_IMAGE && ((rid_text_item_image *)ti)->usemap)
	    return match_item_LINK;

	/* check for tag specifically in case a table gets an AREF around it */
	if ((ti->tag == rid_tag_TEXT || ti->tag == rid_tag_IMAGE || ti->tag == rid_tag_OBJECT) &&
	    aref_valid && aref_changed_enough)
	    return match_item_LINK;
    }

    return match_item_NONE;
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

    LNKDBG(("antweb_build_selection_list(): doc %p old list %p\n", doc, doc->selection_list.list));

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
	int match = match_item(ti, be_link_ONLY_CURRENT, NULL);
	if (match)
	{
	    /* FIXME: this is rather inefficient - need a better way */
	    backend_doc_item_bbox(doc, ti, &link->bbox);

	    link->item.tag = doc_selection_tag_TEXT;
	    link->item.data.text.item = ti;

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

    LNKDBG(("antweb_build_selection_list(): count %d new list %p\n", count, doc->selection_list.list));
}

/* ----------------------------------------------------------------------------- */

static antweb_selection_descr *antweb_highlight_scan_xy(be_doc doc, const antweb_selection_t *initial, const wimp_box *from, int flags, const wimp_box *bounds)
{
    antweb_selection_descr *link;
    rid_aref_item *start_aref;
    rid_text_item *start_link;

    /* loop storage for main link finding */
    int min_dist = INT_MAX, min_secdist = INT_MAX;
    antweb_selection_descr *min_link = NULL;

    /* loop storage for fallback link finding */
    int min_dist1 = INT_MAX, min_secdist1 = INT_MAX;
    antweb_selection_descr *min_link1 = NULL;

    int i;

    LNKDBGN(("antweb_highlight_scan_xy: doc %p initial %p from x=%d-%d y=%d-%d flags %x\n", doc, initial, from->x0, from->x1, from->y0, from->y1, flags));
    LNKDBGN(("antweb_highlight_scan_xy: bounds from x=%d-%d y=%d-%d\n", bounds->x0, bounds->x1, bounds->y0, bounds->y1));

    /* see if there is an aref we mustn't match */
    start_aref = (flags & be_link_INCLUDE_CURRENT) == 0 && initial && initial->tag == doc_selection_tag_AREF ? initial->data.aref : NULL;
    start_link = (flags & be_link_INCLUDE_CURRENT) == 0 && initial && initial->tag == doc_selection_tag_TEXT ? initial->data.text.item : NULL;

    LNKDBGN(("antweb_highlight_scan_xy: start_aref %p start_link %p\n", start_aref, start_link));

    /* go through all the elements looking for the minimum distance */
    for (i = 0, link = doc->selection_list.list; i < doc->selection_list.count; i++, link++)
    {
	int dist = -1, secdist = INT_MAX;
	int dist1 = -1, secdist1 = INT_MAX;
	BOOL on_screen = FALSE;

 	LNKDBGN(("                        : link %p item %p box    x=%d-%d y=%d-%d\n", link, link->item.data.text.item, link->bbox.x0, link->bbox.x1, link->bbox.y0, link->bbox.y1));

	if (link->item.tag == doc_selection_tag_TEXT)
	{
	    if (start_aref != NULL && link->item.data.text.item->aref == start_aref)	/* mustn't be part of same link */
	    {
 		LNKDBGN(("                        : same aref\n"));
		continue;
	    }

	    /* if it is an imagemap/form element it could be the same item without being the same link */
	    if (start_link != NULL && link->item.data.text.item == start_link)
	    {
 		LNKDBGN(("                        : same item\n"));
		continue;
	    }

	}

	/* check and see if this item is visible */
#if 1
	on_screen = link->bbox.y0 < bounds->y1 && link->bbox.y1 >= bounds->y0;
#else
	if (link->bbox.y0 < bounds->y0 && link->bbox.y1 > bounds->y1)
	{
	    /* see if link encompasses screen */
	    on_screen = TRUE;
	}
	else if (flags & be_link_BACK)
	{
	    /* see if bottom line is visible */
	    if (link->bbox.y0 >= bounds->y0 && link->bbox.y0 < bounds->y1)
		on_screen = TRUE;
	}
	else
	{
	    /* see if top line is visible */
	    if (link->bbox.y1 > bounds->y0 && link->bbox.y1 <= bounds->y1)
		on_screen = TRUE;
	}
#endif
	/* if not visible then continue on to next immediately */
	if (!on_screen)
	{
 	    LNKDBGN(("                        : not on screen\n"));
	    continue;
	}

	/* match the correct edge of the box for the direction being travelled */

	/* We use the opposite sides of the start rectangle from
	 * the direction we are travelling so that we can handle overlaps
	 * and so that revisiting the original returns 0 dist.
	 */
	if (flags & be_link_VERT)
	{
	    /* for vertical movement we can match any overlap of the
	     * source box and the destination box but we prefer
	     * matches to the left */

	    if (from->x0 < link->bbox.x1 && from->x1 > link->bbox.x0)
	    {
		dist = flags & be_link_BACK ? link->bbox.y0 - from->y0 : from->y1 - link->bbox.y1;		/* up : down */

		/* get the distance from the left edge of the source to the left edge of the target
		 * if left source overlaps target then that counts as 0 (ie minimum distance) */
		secdist = link->bbox.x0 - from->x0;
		if (secdist < 0)
		    secdist = 0;
	    }
	    else
	    {
		/* check fallback link */
		/* changed to use the same side on each link so that
                   NetChannel sites work - maybe I meant this
                   originally??? */
		if (flags & be_link_BACK)
		{
/* 		    dist1 = link->bbox.y0 - from->y1;	 */	/* dist between roofs */
		    dist1 = link->bbox.y0 - from->y0;		/* dist between roofs */
		}
		else
		{
/* 		    dist1 = from->y0 - link->bbox.y1;	 */	/* dist between roofs */
		    dist1 = from->y1 - link->bbox.y1;		/* dist between roofs */
		}

		secdist1 = from->x0 - link->bbox.x1;
		if (secdist1 < 0)
		    secdist1 = link->bbox.x0 - from->x1;
	    }
	}
	else
	{
	    /* for horizontal movement we can match any overlap of the
	     * source box and the destination box but we prefer
	     * matches near the top of the box */
	    if (from->y0 < link->bbox.y1 && from->y1 > link->bbox.y0)
	    {
		dist = flags & be_link_BACK ? from->x1 - link->bbox.x1 : link->bbox.x0 - from->x0;	/* left : right */

		secdist = from->y1 - link->bbox.y1;
		if (secdist < 0)
		    secdist = 0;
	    }
	    else
	    {
		/* check fallback link, wrap around to the next line and scan from the edge */
		if (flags & be_link_BACK)
		{
		    dist1 = link->bbox.y0 - from->y0;		/* dist between roofs */
		    secdist1 = -link->bbox.x1;			/* dist from RH edge */
		}
		else
		{
		    dist1 = from->y1 - link->bbox.y1;		/* dist between roofs */
		    secdist1 = link->bbox.x0;			/* dist from LH edge */
		}
	    }
	}

	/* is this better than the previous link ? */
	if (dist > 0 &&							/* dist == 0 implies the one we started on so ignore */
	    (dist < min_dist						/* if simply closer */
		|| (dist == min_dist && secdist < min_secdist)))	/* if same primary distance and closer secondary distance */
	{
	    min_dist = dist;
	    min_secdist = secdist;
	    min_link = link;
	}

	/* is this better than the previous fallback link ? */
	if (dist1 > 0 &&
	    (dist1 < min_dist1
	    || (dist1 == min_dist1 && secdist1 < min_secdist1)))
	{
	    min_dist1 = dist1;
	    min_secdist1 = secdist1;
	    min_link1 = link;
	}
    }

    if ((flags & be_link_DONT_WRAP_H) == 0 && min_link == NULL)
    {
	LNKDBG(("antweb_highlight_scan_xy: using fallback link %p dist %d sec %d\n", min_link1, min_link1 ? min_dist1 : 0, min_link1 ? min_secdist1 : 0));
	min_link = min_link1;
    }
    else
    {
	LNKDBG(("antweb_highlight_scan_xy: using main link %p dist %d sec %d\n", min_link, min_link ? min_dist : 0, min_link ? min_secdist : 0));
    }

    return min_link;
}

/* ----------------------------------------------------------------------------- */

static antweb_selection_descr *antweb_highlight_scan_link(be_doc doc, antweb_selection_t *initial, int flags, const wimp_box *bounds)
{
/*     int x0, x1, y, h; */
    wimp_box bbox;
    be_item item = NULL;

    LNKDBG(("antweb_highlight_scan_link: doc %p selection %p item %p flags %x tag %d\n", doc, initial, initial ? initial->data.text.item : 0, flags, initial ? initial->tag : -1));

    if (initial == NULL)
	return NULL;

    /* decide which item/position to start from */
    switch (initial->tag)
    {
    case doc_selection_tag_AREF:
	item = initial->data.aref->first;
	aref_union_box(doc, initial->data.aref, flags, &bbox);
	break;

    case doc_selection_tag_TEXT:
	item = initial->data.text.item;
	backend_doc_item_bbox(doc, item, &bbox);
	break;

    case doc_selection_tag_MAP:
	item = initial->data.map.item;
	backend_doc_item_bbox(doc, item, &bbox);
	break;
    }

/*     x0 = bbox.x0; */
/*     x1 = bbox.x1; */

/* #if 1 */
/*     w = item->width/2; */
/*     if (w > webfonts[WEBFONT_BASE].space_width) */
/* 	w = webfonts[WEBFONT_BASE].space_width; */
/*     x = bbox.x0 + w; */
/* #else */
/*     x = (bbox.x0 + bbox.x1) / 2; */
/* #endif */

/*     h = item ? item->max_up + item->max_down : 0; */
/*     if (h > webfonts[WEBFONT_BASE].max_up) */
/* 	h = webfonts[WEBFONT_BASE].max_up; */
/*     y = bbox.y1 - h/2; */

    return antweb_highlight_scan_xy(doc, initial, &bbox, flags, bounds);
}

/* ----------------------------------------------------------------------------- */

static be_item scan_links_2D(be_doc doc, be_item item, int flags, const wimp_box *bounds)
{
    antweb_selection_t sel;
    antweb_selection_descr *link;

    LNKDBG(("backend_highlight_link_2D: doc %p item %p aref %p flags %x\n", doc, item, item ? item->aref : NULL, flags));

    if (item == NULL)
    {
	wimp_box box;

	if (flags & be_link_VERT)
	{			/* horizontal line at top or bottom of window */
	    box.x0 = -0x4000;
	    box.x1 = 0x4000;
	    box.y0 = box.y1 = flags & be_link_BACK ? -0x4000 : 0x4000;
	}
	else
	{			/* vertical line at left or right of window */
	    box.x0 = flags & be_link_BACK ? 0x4000 : -0x4000;
	    box.x1 = flags & be_link_BACK ? 0x4000 : -0x4000;
	    box.y0 = -0x4000;
	    box.y1 = 0x4000;
	}

	link = antweb_highlight_scan_xy(doc, NULL, &box, flags, bounds);
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
	    sel.data.text.item = item;
	}

	link = antweb_highlight_scan_link(doc, &sel, flags, bounds);
    }

    LNKDBG(("backend_highlight_link_2D: return link %p tag %d item %p\n", link, link ? link->item.tag : 0, link ? link->item.data.text.item : 0));

    return descriptor_to_item(link);
}

static be_item scan_links_linear(be_doc doc, be_item item, int flags, const wimp_box *bounds)
{
    int i;
    int inc, term;
    be_item ti;
    rid_aref_item *aref;

    LNKDBG(("scan_links_linear: doc %p item %p flags %x\n", doc, item, flags));

    /* get increment and end */
    inc = flags & be_link_BACK ? -1 : +1;
    term = flags & be_link_BACK ? -1 : doc->selection_list.count;

    /* set up start position */
    if (item)
    {
	aref = item->aref;

	i = item_to_descriptor_index(doc, item);
	if (i == -1)
	    i = term;

	if ((flags & (be_link_INCLUDE_CURRENT|be_link_ONLY_CURRENT)) == 0 && i != term)
	    i += inc;
    }
    else
    {
	aref = NULL;

	if (flags & be_link_BACK)
	    i = doc->selection_list.count-1;
	else
	    i = 0;
    }

    ti = NULL;

    /* search from current position to end of list */
    while (i != term)
    {
/* 	LNKDBG(("scan_links_linear: 1st pass i %d\n", i)); */

	ti = descriptor_to_item(&doc->selection_list.list[i]);

	if (match_item(ti, flags, aref))
	{
	    if ((flags & be_link_VISIBLE) == 0 || be_item_onscreen(doc, ti, bounds, flags))
		break;
	}

	if (flags & be_link_ONLY_CURRENT)
	{
	    i = term;		/* exit after first check
				 */
	    break;
	}
	else
	{
	    i += inc;
	}
    }

    /* search from one extent to the other */
    if (i == term && (flags & (be_link_DONT_WRAP | be_link_ONLY_CURRENT)) == 0)
    {
	i = flags & be_link_BACK ? doc->selection_list.count - 1 : 0;

	while (i != term)
	{
/* 	    LNKDBG(("scan_links_linear: 2nd pass i %d\n", i)); */

	    ti = descriptor_to_item(&doc->selection_list.list[i]);

	    if (match_item(ti, flags | be_link_INCLUDE_CURRENT, aref))
	    {
		if ((flags & be_link_VISIBLE) == 0 || be_item_onscreen(doc, ti, bounds, flags))
		    break;
	    }

	    i += inc;
	}
    }

    LNKDBG(("scan_links_linear: return i %d item %p\n", i, ti));

    return i == term ? NULL : ti;
}

/* ----------------------------------------------------------------------------- */

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

static void be_update_item_highlight(be_doc doc, be_item ti, BOOL selected)
{
    wimp_box trim_box;
    BOOL needs_box = TRUE;

    LNKDBG(("be_update_item_highlight: doc %p ti %p tag %d\n", doc, ti, ti ? ti->tag : -1));

/*     if (object_table[ti->tag].update_highlight) */
/*         object_table[ti->tag].update_highlight(ti, doc); */
/*     else */
/*         antweb_update_item(doc, ti); */

    if (object_table[ti->tag].update_highlight)
	needs_box = object_table[ti->tag].update_highlight(ti, doc, 0, &trim_box);
    else
	memset(&trim_box, 0, sizeof(trim_box));

    if (needs_box)
    {
	wimp_box bbox;
	backend_doc_item_bbox(doc, ti, &bbox);

	highlight_offset_border(&bbox);
	highlight_update_border(doc, &bbox, selected);
    }
    else
	antweb_update_item_trim(doc, ti, &trim_box, TRUE);
}

/* ----------------------------------------------------------------------------- */
/* exported to frontend via interface.h */
/* ----------------------------------------------------------------------------- */

/*
 * Highlight the nearest link in the given direction from the given point or link.
 */

be_item backend_highlight_link_xy(be_doc doc, be_item item, const wimp_box *box, int flags)
{
    be_item ti;
    wimp_box bounds, margins;

    LNKDBG(("Highlight from item %p, flags=0x%x, line=%p\n", item, flags, item ? item->line : NULL));

    /* get the screen bounds for visibility */
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

    if ((flags & be_link_XY) && box)
    {
	antweb_selection_descr *link;

	link = antweb_highlight_scan_xy(doc, NULL, box, flags, &bounds);

	ti = descriptor_to_item(link);
    }
    else
    {
	/* add check for not caretise else do normal case */
	if ((flags & (be_link_INCLUDE_CURRENT | be_link_CARETISE)) == be_link_INCLUDE_CURRENT && item)
	{
	    if ((flags & be_link_VISIBLE) == 0 ||
		be_item_onscreen(doc, item, &bounds, flags))
		return item;
	}

	if ((flags & (be_link_ONLY_CURRENT|be_link_TEXT)) == 0)
	{
	    ti = scan_links_2D(doc, item, flags, &bounds);
	}
	else
	{
	    ti = scan_links_linear(doc, item, flags, &bounds);
	}
    }

    /* check for highlighting needed */
    if ((flags & be_link_DONT_HIGHLIGHT) == 0)
    {
	BOOL item_changed = item != ti && (item == NULL || item->aref == NULL || item->aref != ti->aref);

	if ((flags & be_link_CLEAR_REST) && (!ti || (!item_changed && (flags & (be_link_ONLY_CURRENT|be_link_CARETISE)) == 0)))
	    backend_remove_highlight(doc);

        if (ti)
        {
	    int x, y;

	    LNKDBG(("New link at %p\n", ti));

	    if ((flags & be_link_VISIBLE) == 0 || (flags & be_link_MOVE_POINTER))
		stream_find_item_location(ti, &x, &y);

	    /* removed the test on the basis that if VISIBLE was set then the item must be partially on screen
	     * so we's like it to be totally on screen
	     */
 	    if ((flags & be_link_DONT_FORCE_ON) == 0)
	    {
#if USE_MARGINS
		frontend_view_ensure_visable_full(doc->parent,
						  x + doc->margin.x0, x + doc->margin.x0 + ti->width,
						  y + ti->max_up + doc->margin.y1, y - ti->max_down + doc->margin.y1, flags);
#else
		frontend_view_ensure_visable(doc->parent, x, y + ti->max_up, y - ti->max_down);
#endif
	    }

            if (item_changed || (flags & (be_link_ONLY_CURRENT|be_link_CARETISE)))
	    {
		int match = match_item(ti, be_link_TEXT, NULL);
		if (match == match_item_NUMBERS || (match == match_item_TEXT && (flags & be_link_CARETISE)))
		{
		    int offset;
#if 1
		    offset = -1;
#else
		    if (flags & be_link_VERT)
			offset = be_item_has_caret(doc, item) ? doc->selection.data.text.input_offset : -1;
		    else
			offset = flags & be_link_BACK ? -1 : 0;			/* end : beginning */
#endif
		    LNKDBG(("move_highlight: caretise flags %x old offset %d offset %d old item %p old input %p\n", flags, doc->selection.data.text.input_offset, offset, item, doc->selection.data.text.item));
		    backend_set_caret(doc, ti, offset);
		}
		else
		{
		    backend_set_highlight(doc, ti, FALSE);
		}

		if (flags & be_link_MOVE_POINTER)
#if USE_MARGINS
		    frontend_pointer_set_position(doc->parent, x + (ti->width + ti->pad)/2 + doc->margin.x0, y + (ti->max_up - ti->max_down)/2 + doc->margin.y1);
#else
		    frontend_pointer_set_position(doc->parent, x + (ti->width + ti->pad)/2, y);
#endif
	    }
        }
    }

    LNKDBG(("About to return %p\n", ti));

    return ti;
}

/* old simple form without position info */

be_item backend_highlight_link(be_doc doc, be_item item, int flags)
{
    return backend_highlight_link_xy(doc, item, NULL, flags);
}

/* ----------------------------------------------------------------------------- */

static void be_update_link(be_doc doc, antweb_selection_t *selection, int selected)
{
    be_item ti;

    LNKDBG(("be_update_link: doc %p type %d selected %d\n", doc, selection ? selection->tag : -1, selected));

    if (selection == NULL)
	return;

#if NEW_HL
    highlight_update_link(doc, selection, selected);
#else
    switch (selection->tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
	ti = selection->data.text.item;

	ti->flag = adjust_flag(ti->flag, selected, NULL);

	be_update_item_highlight(doc, ti, selected);

	break;

    case doc_selection_tag_AREF:
	for (ti = selection->data.aref->first; ti && ti->aref == selection->data.aref; ti = rid_scanfr(ti))
	{
	    ti->flag = adjust_flag(ti->flag, selected, NULL);

	    be_update_item_highlight(doc, ti, selected);
	}
	break;

    case doc_selection_tag_MAP:
	ti = selection->data.map.item;

	ti->flag = adjust_flag(ti->flag, selected, NULL);

	be_update_item_highlight(doc, ti, selected);
	break;
    }
#endif
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
	be_update_item_highlight(doc, ti, TRUE);
    }
}

/* ----------------------------------------------------------------------------- */

/* caret related routines  */

void antweb_default_caret(antweb_doc *doc, BOOL take_caret)
{
    if (take_caret || frontend_view_has_caret(doc->parent))
    {
	rid_text_item *ti = be_doc_read_caret(doc);
#ifdef STBWEB
        /* pdh: I think we want to do this all the time in Fresco */
	if (ti)
#endif
	    backend_set_caret(doc, ti, doc->selection.data.text.input_offset);
/* 	antweb_place_caret(doc, ti, doc->selection.data.text.input_offset); */
    }
}

/* ----------------------------------------------------------------------------- */

os_error *backend_doc_cursor(be_doc doc, int motion, int *used)
{
    rid_text_item *ti, *old_ti;

    old_ti = be_doc_read_caret(doc);
    if (old_ti == NULL)
    {
	*used = 0;
	return NULL;
    }

    ti = NULL;
    switch (motion)
    {
    case be_cursor_UP:
	ti = backend_highlight_link(doc, old_ti, be_link_TEXT | be_link_BACK | be_link_CARETISE | be_link_DONT_WRAP);
	break;

    case be_cursor_UP | be_cursor_WRAP:
	ti = backend_highlight_link(doc, old_ti, be_link_TEXT | be_link_BACK | be_link_CARETISE);
	break;

    case be_cursor_UP | be_cursor_LIMIT:
	ti = backend_highlight_link(doc, NULL, be_link_TEXT | be_link_BACK | be_link_CARETISE | be_link_DONT_WRAP);
	break;

    case be_cursor_DOWN:
	ti = backend_highlight_link(doc, old_ti, be_link_TEXT | be_link_CARETISE | be_link_DONT_WRAP);
	break;

    case be_cursor_DOWN | be_cursor_WRAP:
	ti = backend_highlight_link(doc, old_ti, be_link_TEXT | be_link_CARETISE);
	break;

    case be_cursor_DOWN | be_cursor_LIMIT:
	ti = backend_highlight_link(doc, NULL, be_link_TEXT | be_link_CARETISE | be_link_DONT_WRAP);
	break;
    }

    *used = old_ti != ti;

    return NULL;
}

/* ----------------------------------------------------------------------------- */

be_item be_doc_read_caret(be_doc doc)
{
    return doc->selection.tag == doc_selection_tag_TEXT &&
	doc->selection.data.text.input_offset != doc_selection_offset_NO_CARET ?
	doc->selection.data.text.item : NULL;
}

BOOL be_item_has_caret(be_doc doc, be_item ti)
{
    return ti == be_doc_read_caret(doc);
}

/* ----------------------------------------------------------------------------- */

void backend_set_highlight(be_doc doc, be_item item, BOOL bPersist)
{
    if (item == NULL)
    {
	backend_remove_highlight(doc);
	doc->bHighlightPersistent = FALSE;
    }
    else
    {
        if (item->aref)
        {
	    if (doc->selection.tag == doc_selection_tag_AREF &&
	        doc->selection.data.aref == item->aref)
    	    return;

    	    backend_remove_highlight(doc);

    	    doc->selection.tag = doc_selection_tag_AREF;
    	    doc->selection.data.aref = item->aref;
#ifndef BUILDERS
    	    highlight_boundary_build(doc);
#endif
        }
        else
        {
	    if (doc->selection.tag == doc_selection_tag_TEXT &&
	        doc->selection.data.text.item == item)
	        return;

    	    backend_remove_highlight(doc);

	    doc->selection.tag = doc_selection_tag_TEXT;
	    doc->selection.data.text.item = item;
	    doc->selection.data.text.input_offset = doc_selection_offset_NO_CARET;

#ifndef BUILDERS
	    highlight_boundary_build(doc);
#endif
        }
        doc->bHighlightPersistent = bPersist;
    }

#if NEW_HL
    highlight_boundary_refresh(doc);
#else
    be_update_link(doc, &doc->selection, TRUE);
#endif
}

BOOL backend_highlight_is_persistent( be_doc doc )
{
    return doc->bHighlightPersistent;
}

void backend_set_caret(be_doc doc, be_item ti, int offset)
{
    int repos = object_caret_REPOSITION;

    if (ti == NULL)
    {
	backend_remove_highlight(doc);
	return;
    }

    if (!be_item_has_caret(doc, ti))
    {
	backend_remove_highlight(doc);

	doc->selection.tag = doc_selection_tag_TEXT;
	doc->selection.data.text.item = ti;

#ifndef BUILDERS
	highlight_boundary_build(doc);
#endif

	repos = object_caret_FOCUS;
    }

    doc->selection.data.text.input_offset = offset;

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

void backend_remove_highlight(be_doc doc)
{
    /* see if anyone had the caret */
    be_item old_ti;
    antweb_selection_t old_sel;

    if (!doc)
	return;

    old_ti = be_doc_read_caret(doc);
    old_sel = doc->selection;

    /* set none selected */
    doc->selection.tag = doc_selection_tag_NONE;

    /* redraw the selected links */
#if NEW_HL
    highlight_boundary_refresh(doc);
    highlight_boundary_clear(doc);
#else
    be_update_link(doc, &old_sel, FALSE);
#endif

    /* tell the object the caret has been removed */
    if (old_ti)
    {
	if (object_table[old_ti->tag].caret)
	    object_table[old_ti->tag].caret(old_ti, doc->rh, doc, object_caret_BLUR);

#ifdef STBWEB
	/* Give the window the input focus but no visable caret */
	if (frontend_view_has_caret(doc->parent))
	    frontend_view_caret(doc->parent, 0, 0, -1, 0);
#endif
    }
#ifndef STBWEB
    /* pdh: I think this is what desktop Fresco wants */
    frontend_view_caret( doc->parent, 0, 0, -1, FALSE );
#endif
}

be_item backend_read_highlight(be_doc doc, BOOL *had_caret)
{
    if (!doc)
	return NULL;

    if (had_caret)
    {
	*had_caret = doc->selection.tag == doc_selection_tag_TEXT &&
	    doc->selection.data.text.input_offset != doc_selection_offset_NO_CARET;
    }

    return selection_to_item(&doc->selection);
}

int backend_is_selected(be_doc doc, be_item ti)
{
    antweb_selection_t *sel = &doc->selection;
    BOOL selected = FALSE;

    if (sel) switch (sel->tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
	selected = ti == sel->data.text.item;
	break;

    case doc_selection_tag_AREF:
	selected = ti->aref == sel->data.aref;
	break;

    case doc_selection_tag_MAP:
	selected = ti == sel->data.map.item;
	break;
    }
    return selected;
}

/* ----------------------------------------------------------------------------- */

/* eof keyhl.c */
