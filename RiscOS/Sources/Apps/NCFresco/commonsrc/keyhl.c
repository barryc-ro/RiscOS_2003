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
#include "util.h"

#ifndef LINK_DEBUG
#define LINK_DEBUG 0
#endif

#if LINK_DEBUG
#define LKDBG(a) fprintf a
#else
#define LKDBG(a)
#endif

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

static antweb_selection_descr *antweb_highlight_scan_xy(be_doc doc, const antweb_selection_t *initial, const wimp_box *from, int flags, const wimp_box *bounds)
{
    antweb_selection_descr *link;
    rid_aref_item *start_aref;

    /* loop storage for main link finding */
    int min_dist = INT_MAX, min_secdist = INT_MAX;
    antweb_selection_descr *min_link = NULL;

    /* loop storage for fallback link finding */
    int min_dist1 = INT_MAX, min_secdist1 = INT_MAX;
    antweb_selection_descr *min_link1 = NULL;

    int i;
    
    LKDBG((stderr, "antweb_highlight_scan_xy: doc %p initial %p from x=%d-%d y=%d-%d flags %x\n", doc, initial, from->x0, from->x1, from->y0, from->y1, flags));
    LKDBG((stderr, "antweb_highlight_scan_xy: bounds from x=%d-%d y=%d-%d\n", bounds->x0, bounds->x1, bounds->y0, bounds->y1));

    /* see if there is an aref we mustn't match */
    start_aref = initial && initial->tag == doc_selection_tag_AREF ? initial->data.aref : NULL;

    LKDBG((stderr, "antweb_highlight_scan_xy: start_aref %p\n", start_aref));

    /* go through all the elements looking for the minimum distance */
    for (i = 0, link = doc->selection_list.list; i < doc->selection_list.count; i++, link++)
    {
	int dist = -1, secdist = INT_MAX;
	int dist1 = -1, secdist1 = INT_MAX;
	BOOL on_screen = FALSE;

/* 	LKDBG((stderr, "                        : link %p item %p box    x=%d-%d y=%d-%d\n", link, link->item.data.text, link->bbox.x0, link->bbox.x1, link->bbox.y0, link->bbox.y1)); */

	if (link->item.tag == doc_selection_tag_TEXT)
	{
	    if (start_aref != NULL && link->item.data.text->aref == start_aref)	/* mustn't be part of same link */
	    {
/* 		LKDBG((stderr, "                        : same aref\n")); */
		continue;
	    }
	}

	/* check and see if this item is visible */
	if (flags & be_link_BACK)
	{
	    if (link->bbox.y0 >= bounds->y0 && link->bbox.y0 < bounds->y1)
		on_screen = TRUE;
	}
	else
	{
	    if (link->bbox.y1 > bounds->y0 && link->bbox.y1 <= bounds->y1)
		on_screen = TRUE;
	}

	/* if not visible then continue on to next immediately */
	if (!on_screen)
	{
/* 	    LKDBG((stderr, "                        : not on screen\n")); */
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
		if (flags & be_link_BACK)
		{
		    dist1 = link->bbox.y0 - from->y1;		/* dist between roofs */
		}
		else
		{
		    dist1 = from->y0 - link->bbox.y1;		/* dist between roofs */
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
	LKDBG((stderr, "antweb_highlight_scan_xy: using fallback link %p dist %d sec %d\n", min_link1, min_dist1, min_secdist1));
	min_link = min_link1;
    }
    else
    {
	LKDBG((stderr, "antweb_highlight_scan_xy: using main link %p dist %d sec %d\n", min_link, min_dist, min_secdist));
    }
    
    return min_link;
}

static void aref_union_box(be_doc doc, rid_aref_item *aref, wimp_box *box_out)
{
    be_item item = aref->first;

    backend_doc_item_bbox(doc, item, box_out);

    for (item = item->next; item && item->next && item->next->aref == aref; item = item->next)
    {
	wimp_box box;
	backend_doc_item_bbox(doc, item, &box);
	coords_union(&box, box_out, box_out);
    }
}

static antweb_selection_descr *antweb_highlight_scan_link(be_doc doc, antweb_selection_t *initial, int flags, const wimp_box *bounds)
{
/*     int x0, x1, y, h; */
    wimp_box bbox;
    be_item item = NULL;

    LKDBG((stderr, "antweb_highlight_scan_link: doc %p selection %p item %p flags %x\n", doc, initial, initial ? initial->data.text : 0, flags));

    if (initial == NULL)
	return NULL;
    
    /* decide which item/position to start from */
    switch (initial->tag)
    {
    case doc_selection_tag_AREF:
	item = initial->data.aref->first;
	aref_union_box(doc, initial->data.aref, &bbox);
	break;

    case doc_selection_tag_TEXT:
	item = initial->data.text;
	backend_doc_item_bbox(doc, item, &bbox);
	break;

    case doc_selection_tag_AREA:
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

static be_item backend_highlight_link_2D(be_doc doc, be_item item, int flags, const wimp_box *bounds)
{
    antweb_selection_t sel;
    antweb_selection_descr *link;
    be_item new_item;

    LKDBG((stderr, "backend_highlight_link_2D: doc %p item %p flags %x\n", doc, item, flags));

    if (item == NULL)
    {
	wimp_box box;

	if (flags & be_link_VERT)
	{
	    box.x0 = -0x4000;
	    box.x1 = 0x4000;
	    box.y0 = box.y1 = flags & be_link_BACK ? 0x4000 : -0x4000;
	}
	else
	{
	    box.x0 = flags & be_link_BACK ? 0x4000 : -0x4000;
	    box.x1 = flags & be_link_BACK ? 0x4000 : -0x4000;
	    box.y0 = box.y1 = 0;
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
	    sel.data.text = item;
	}

	link = antweb_highlight_scan_link(doc, &sel, flags, bounds);
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
/* exported to frontend via interface.h */
/* ----------------------------------------------------------------------------- */

/*
 * Highlight the nearest link in the given direction from the given point.
 */

be_item backend_highlight_link_xy(be_doc doc, be_item item, const wimp_box *box, int flags)
{
    rid_aref_item *aref;
    be_item ti;
    wimp_box bounds, margins;
    const int scan_flags = SCAN_RECURSE | ( (flags & be_link_BACK) ? SCAN_BACK : SCAN_FWD );

    LKDBG((stderr, "Highlight from item %p, flags=0x%x, line=%p\n", item, flags, item ? item->line : NULL));

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
    
	ti = NULL;
	if (link) switch (link->item.tag)
	{
	case doc_selection_tag_TEXT:
	    ti = link->item.data.text;
	    if (ti->aref)
		ti = link->item.data.text->aref->first;
	    break;
	    
	case doc_selection_tag_AREA:
	    ti = link->item.data.map.item;
	    break;
	}
    }
    else
    {
	if ((flags & (be_link_ONLY_CURRENT|be_link_TEXT)) == 0)
	{
	    /* 2D match algorithm */
	    ti = backend_highlight_link_2D(doc, item, flags, &bounds);
	}
	else
	{
	    /* work out from which item to start searchingm, and aref to match against */
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

/* 	    LKDBG((stderr, "Start search at %p, aref=%p, line=%p\n", ti, aref, ti ? ti->line : NULL)); */

	    /* search from here to the end of the list */
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
/* 		    LKDBG((stderr, "ti=%p, next=%p, line=%p\n", ti, ti->next, ti->line)); */
		}
	    }

	    /* search from the top to the end of the list */
	    if (ti == NULL && (flags & (be_link_DONT_WRAP | be_link_ONLY_CURRENT)) == 0)
	    {
		ti = (flags & be_link_BACK) ? doc->rh->stream.text_last : doc->rh->stream.text_list;

/* 		LKDBG((stderr, "No link found, ti wraped to %p\n", ti)); */

		while (ti)
		{
		    if (match_item(ti, flags | be_link_INCLUDE_CURRENT, aref))
		    {
			if ((flags & be_link_VISIBLE) == 0 || be_item_onscreen(doc, ti, &bounds, flags))
			    break;
		    }

		    ti = rid_scan(ti, scan_flags);
/* 		    LKDBG((stderr, "ti=%p, next=%p, line=%p\n", ti, ti->next, ti->line)); */
		}
	    }
	}
    }
    
    /* check for highlighting needed */
    if ((flags & be_link_DONT_HIGHLIGHT) == 0)
    {
	BOOL item_changed = item != ti && (item == NULL || item->aref == NULL || item->aref != ti->aref);

	/* de highlight original only if the highlight has ended up changing */
        if (item_changed && item && (flags & be_link_ONLY_CURRENT) == 0)
	    backend_update_link(doc, item, 0);

        if (ti)
        {
	    int x, y;

	    LKDBG((stderr, "New link at %p\n", ti));

	    if ((flags & be_link_VISIBLE) == 0 || (flags & be_link_MOVE_POINTER))
		stream_find_item_location(ti, &x, &y);
	    
	    if ((flags & be_link_VISIBLE) == 0)
	    {
#if USE_MARGINS
		frontend_view_ensure_visable(doc->parent, x, y + ti->max_up + doc->margin.y1, y - ti->max_down + doc->margin.y1);
#else
		frontend_view_ensure_visable(doc->parent, x, y + ti->max_up, y - ti->max_down);
#endif
	    }

            if (item_changed || (flags & be_link_ONLY_CURRENT))
	    {
		if ((flags & be_link_CARETISE) && match_item(ti, be_link_TEXT, NULL))
		{
		    int offset;
		    if (flags & be_link_VERT)
			offset = item == doc->input ? doc->text_input_offset : -1;
		    else
			offset = flags & be_link_BACK ? -1 : 0;			/* end : beginning */

		    LKDBG((stderr, "move_highlight: caretise flags %x old offset %d offset %d old item %p old input %p\n", flags, doc->text_input_offset, offset, item, doc->input));
		    antweb_place_caret(doc, ti, offset);
		}
		else
		{
		    antweb_place_caret(doc, NULL, -1);
		    backend_update_link(doc, ti, 1);
		}

		if (flags & be_link_MOVE_POINTER)
		    frontend_pointer_set_position(doc->parent, x + ti->width/2, y);
	    }
        }
    }

    LKDBG((stderr, "About to return %p\n", ti));

    return ti;
}

/* old simple form without position info */

be_item backend_highlight_link(be_doc doc, be_item item, int flags)
{
    return backend_highlight_link_xy(doc, item, NULL, flags);
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

/* ----------------------------------------------------------------------------- */

/* caret related routines  */

void antweb_default_caret(antweb_doc *doc, BOOL take_caret)
{
    if (take_caret || frontend_view_has_caret(doc->parent))
        antweb_place_caret(doc, doc->input, -1);
}

void antweb_place_caret(antweb_doc *doc, rid_text_item *ti, int offset)
{
    rid_text_item *old_ti = doc->input;
    int repos = object_caret_REPOSITION;

    doc->input = ti;		/* must set doc->input before calling the remove() function */
    doc->text_input_offset = offset;

    if (old_ti != ti)
    {
	LKDBG((stderr, "antweb_place_caret(): input changed from %p to %p\n", old_ti, ti));

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

/* ----------------------------------------------------------------------------- */

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

/* ----------------------------------------------------------------------------- */

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
	antweb_place_caret(doc, ti, doc->text_input_offset);
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

be_item backend_place_caret(be_doc doc, be_item item)
{
    be_item input = doc->input;

    if (item != backend_place_caret_READ)
	antweb_place_caret(doc, item, -1);

    return input;
}

/* ----------------------------------------------------------------------------- */

/* eof keyhl.c */
