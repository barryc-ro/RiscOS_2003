/* > stblinks.c

 *

 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "bbc.h"
#include "resspr.h"
#include "swis.h"

#include "debug.h"
#include "interface.h"
#include "memwatch.h"

#include "config.h"
#include "util.h"
#include "frameutils.h"

#include "stbfe.h"
#include "stbopen.h"
#include "stbtb.h"
#include "stbview.h"
#include "stbutils.h"

#define ICON_SIZE_SMALL	16
#define ICON_SIZE_LARGE	32

/* ------------------------------------------------------------------------------------------- */

static int opposite_side(int side)
{
    int opp = 0;
    switch (side)
    {
    case fe_divider_RIGHT:
	opp = fe_divider_LEFT;
	break;
    case fe_divider_BOTTOM:
	opp = fe_divider_TOP;
	break;
    case fe_divider_TOP:
	opp = fe_divider_BOTTOM;
	break;
    case fe_divider_LEFT:
	opp = fe_divider_RIGHT;
	break;
    }
    return opp;
}

static int direction_flags_to_side(int flags)
{
    int side = 0;
    switch (flags & (be_link_VERT|be_link_BACK))
    {
    case 0:
	side = fe_divider_RIGHT;
	break;
    case be_link_VERT:
	side = fe_divider_BOTTOM;
	break;
    case be_link_VERT | be_link_BACK:
	side = fe_divider_TOP;
	break;
    case be_link_BACK:
	side = fe_divider_LEFT;
	break;
    }
    return side;
}

static int side_to_direction_flags(int side)
{
    int flags = 0;
    switch (side)
    {
    case fe_divider_RIGHT:
	flags = 0;
	break;
    case fe_divider_BOTTOM:
	flags = be_link_VERT;
	break;
    case fe_divider_TOP:
	flags = be_link_VERT | be_link_BACK;
	break;
    case fe_divider_LEFT:
	flags = be_link_BACK;
	break;
    }
    return flags;
}

#if 0
/* box is assumed to be relative to parent window of 'v' */

static void convert_box_coords(wimp_box *box, fe_view v)
{
    coords_cvtstr cvt;

    cvt = frameutils_get_cvt(frameutils_find_top(v));
    coords_box_toscreen(&box, &cvt);

    cvt = frameutils_get_cvt(v);
    coords_box_toworkarea(&box, &cvt);
}
#endif

static int scroll_by_flags(fe_view v, int flags)
{
    int scrolled = FALSE;
    if (v->displaying && v->scrolling != fe_scrolling_NO)
    {
	if (flags & be_link_VERT)
	    scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1, 0);
	else
	    scrolled = fe_view_scroll_x(v, flags & be_link_BACK ? -1 : +1, 0);
    }
    return scrolled;
}

/* position the pointer in the centre of the visible window. Must give
 * coords to frontend_pointer_set_position in work area relative
 * coordinates */

static void centre_pointer(fe_view v)
{
    int x, y;
    coords_cvtstr cvt = frameutils_get_cvt(v);
    x = cvt.scx + (v->box.x1 - v->box.x0)/2;
    y = cvt.scy - (v->box.y1 - v->box.y0)/2;
    frontend_pointer_set_position(v, x, y);
}

static void redraw_link(fe_view v, frame_link *link)
{
    wimp_box box = link->box;
    fe_view v_top = frameutils_find_top(v);
    coords_cvtstr cvt = frameutils_get_cvt(v_top);

    coords_box_toworkarea(&box, &cvt);

    /* it is important that this is redraw not update as we schedule a redraw before deleting them */
    frontend_view_redraw(v_top, &box);
}

/* ------------------------------------------------------------------------------------------- */

/*
 * Using the vert and back flags travel out from box and work out what view we end up in
 */

static os_error *fe__locate_view_by_position(fe_view v, void *handle)
{
    int *vals = handle;
    int pdist, sdist, s1, s2;

    STBDBG(("fe__locate_view_by_position: v%p box %d,%d,%d,%d\n", v, v->box.x0, v->box.y0, v->box.x1, v->box.y1));

    /* ignore frame if it has children or if it has no contents */
    if (v->children || v->displaying == NULL)
	return NULL;
    
    pdist = s1 = s2 = 0;
    switch (vals[4] & (be_link_VERT|be_link_BACK))
    {
    case 0:			/* -> */
	pdist = v->box.x0 - vals[2];

	s1 = vals[3] - v->box.y0;
	s2 = v->box.y1 - vals[3];
	break;

    case be_link_VERT:		/* \/ */
	pdist = vals[3] - v->box.y1;

	s1 = vals[2] - v->box.x0;
	s2 = v->box.x1 - vals[2];
	break;

    case be_link_VERT | be_link_BACK: /* /\ */
	pdist = v->box.y0 - vals[3];

	s1 = vals[2] - v->box.x0;
	s2 = v->box.x1 - vals[2];
	break;

    case be_link_BACK:		/* <- */
	pdist = vals[2] - v->box.x1;

	s1 = vals[3] - v->box.y0;
	s2 = v->box.y1 - vals[3];
	break;
    }

    STBDBG(("fe__locate_view_by_position: pdist %d s1 %d s2 %d\n", pdist, s1, s2));

    /* in wrong direction or not overlapping  */
    if (pdist < 0 || ((vals[4] & be_link_DONT_WRAP) && (s1 < 0 || s2 < 0)))
	return NULL;

    if (vals[4] & be_link_DONT_WRAP)
    {
	sdist = abs(s1 - s2);
    }
    else
    {
	if (s1 >= 0 && s2 >= 0)
	    sdist = 0;
	else
	{
	    s1 = abs(s1);
	    s2 = abs(s2);
	    sdist = MIN(s1, s2);
	}
    }

    if (pdist < vals[0] || (pdist == vals[0] && sdist < vals[1]))
    {
	STBDBG(("fe__locate_view_by_position: closest\n"));

	vals[0] = pdist;
	vals[1] = sdist;
	vals[5] = (int)v;
    }
    
    return NULL;
}

static fe_view fe_locate_view_by_position(wimp_box *box, int flags)
{
    int vals[6];
    fe_view v = main_view;

    STBDBG(("fe_locate_view_by_position: v%p box %d,%d,%d,%d flags %x\n", v, box->x0, box->y0, box->x1, box->y1, flags));

    /* travel to top most popup */
    while (v->next)
	v = v->next;

    /* if no frames just return the view */
    if (v->children == NULL)
	return v;
    
    /* otherwise scan down for the best one */
    vals[0] = INT_MAX;		/* prim distance */
    vals[1] = INT_MAX;		/* sec distance */
    vals[2] = (box->x0 + box->x1) / 2;
    vals[3] = (box->y0 + box->y1) / 2;
    vals[4] = flags;
    vals[5] = NULL;		/* fe_view */

    iterate_frames(v->children, fe__locate_view_by_position, vals);
    v = (fe_view) vals[5];

    return v;
}

/* ------------------------------------------------------------------------------------------- */

static frame_link *find_closest_frame_link(fe_view v, int side, int x, int y, int towards)
{
    int min_dist;
    frame_link *link, *min_link;

    min_link = NULL;
    min_dist = INT_MAX;
    link = v->frame_links;

    STBDBG(("find_closest_frame_link: frames v %p side %d from %d,%d\n", v, side, x, y));
	    
    while (link)
    {
	STBDBG(("find_closest_frame_link: link %p side %d\n", link, link->side));

	if (link->side == side)
	{
	    int dist_x, dist_y, dist;
	    
	    dist_x = (link->box.x0 + link->box.x1)/2 - x;
	    dist_y = (link->box.y0 + link->box.y1)/2 - y;

	    dist = dist_x*dist_x + dist_y*dist_y;

	    switch (towards)
	    {
	    case fe_divider_TOP:
		if (dist_y <= 0)
		    dist = INT_MAX;
		break;
	    case fe_divider_BOTTOM:
		if (dist_y >= 0)
		    dist = INT_MAX;
		break;
	    case fe_divider_LEFT:
		if (dist_x >= 0)
		    dist = INT_MAX;
		break;
	    case fe_divider_RIGHT:
		if (dist_x <= 0)
		    dist = INT_MAX;
		break;
	    }

	    if (dist < min_dist)
	    {
		min_dist = dist;
		min_link = link;
	    }
	}
	link = link->next;
    }

    return min_link;
}

/* ------------------------------------------------------------------------------------------- */

fe_view fe_next_frame(fe_view v, BOOL next)
{
    do
    {
        if (v->children)
            v = next ? v->children : v->children_last;
        else if (next && v->next)
            v = v->next;
        else if (!next && v->prev)
            v = v->prev;
        else if (v->parent)
        {
            v = v->parent;
            if (next)
            {
                while (v && v->next == NULL)
                    v = v->parent;
                if (v)
                    v = v->next;
            }
            else
            {
                while (v && v->prev == NULL)
                    v = v->parent;
                if (v)
                    v = v->prev;
            }
        }
    }
    while (v && v->children);

    return v;
}

void fe_move_highlight_frame_direction(fe_view v, int flags)
{
    if (v->parent)
    {
	wimp_box box = frameutils_get_cvt(v).box;
	fe_move_highlight_xy(v, &box, flags | be_link_XY | be_link_VISIBLE | be_link_DONT_WRAP_H | be_link_DONT_WRAP);
    }
    else
	sound_event(snd_WARN_BAD_KEY);
}

void fe_move_highlight_frame(fe_view v, BOOL next)
{
    fe_view vv;

    vv = fe_next_frame(v ? v : main_view, next);
    if (vv == NULL)
    {
        vv = frameutils_find_top(v);	/* was main_view */
        if (next)
        {
            while (vv->children)
                vv = vv->children;
        }
        else
        {
            while (vv->children_last)
                vv = vv->children_last;
        }
    }

/*     if (v && v->displaying && v->current_link) */
/*         backend_remove_highlight(v->displaying, v->current_link, 0); */

    if (vv)
    {
        fe_get_wimp_caret(vv->w);

        if (vv->displaying)
        {
            /* vv->current_link =  */backend_highlight_link(vv->displaying, NULL, (next ? 0 : be_link_BACK) | be_link_VISIBLE | caretise() | movepointer());
        }
    }
}

/*
 * The coordinates passed here are in screen coordinates. They are the box enclosing the button
 * that was highlighted on the toolbar or the transfer link in the frame border.

 * This routine should work out which view to move into first.


 * Movement:

 * On a non-frame page
 *
 * Left/Right move in the direction given to a link
 *   if no link found then try and scroll the page
 *   if fully scrolled then wrap around to fallback item
 *      and behave like up/down
 *
 * Up/Down move in the direction given to a link
 *   if no link found and next to toolbar then move to toolbar
 *   if no link and found an no toolbar then scroll page and
 *      try to move to link again
 *
 * On a frame page
 *
 * Left/Right move in the direction given to a link
 *   if no link found then move to nearest frame link
 *      Continuing scrolls page if able
 *         otherwise moves to next frame
 *      Going back moves back on to page
 *      Up/Down move to other frame links or do nothing
 *      Enter moves to next frame
 *   if no frame link scroll
 *      if no scroll then move to next frame
 *      if no next frame then wrap to other side of this frame
 *
 * Up/Down are handled analogously
 *   Up/Down next to toolbar moves on to toolbar.
 *      Up/Down on toolbar scrolls the frame just left
 
 */

/* check for moving to the status bar */
static BOOL move_to_toolbar(int flags)
{
    if (use_toolbox && (flags & be_link_VERT) && 
	config_mode_cursor_toolbar && tb_is_status_showing() &&
	((config_display_control_top && (flags & be_link_BACK)) || (!config_display_control_top && (flags & be_link_BACK) == 0)))
    {
/* 	fe_pointer_mode_update(pointermode_OFF); */

	if (tb_status_highlight(TRUE))
	    return TRUE;

/* 	pointer_mode = pointermode_ON; */
    }

    return FALSE;
}

static void fe__move_highlight_xy(fe_view v, wimp_box *box, int flags)
{
    be_item old_link, new_link;
    fe_view new_v;
    BOOL scrolled;

#if DEBUG
    if (flags & be_link_XY)
	STBDBG(("fe_move_highlight_xy: v %p x=%d-%d y=%d-%d flags %x\n", v, box->x0, box->x1, box->y0, box->y1, flags));
    else
	STBDBG(("fe_move_highlight_xy: v %p flags %x\n", v, flags));
#endif
    
    if (use_toolbox && (flags & be_link_VERT))
	tb_status_set_direction(flags & be_link_BACK ? 1 : 0);

    /* set these flags whatever we do */
    flags |= caretise() | movepointer() | be_link_DONT_WRAP;
    
    /* move from given position - ignore any current link */
    if ((flags & be_link_XY) && box)
    {
	wimp_box cbox;
	wimp_wstate state;

	cbox = *box;
	
	/* decide which view we are going to jump into */
	if ((v = fe_locate_view_by_position(&cbox, flags & (be_link_VERT|be_link_BACK))) == NULL)
	{
#if 0
	    if (flags & be_link_VERT)
	    {
		cbox.x0 = screen_box.x0;
		cbox.x1 = screen_box.x1;
	    }
	    else
	    {
		cbox.y0 = screen_box.y0;
		cbox.y1 = screen_box.y1;
	    }

	    if ((v = fe_locate_view_by_position(&cbox, flags)) == NULL)
	    {
		STBDBG(("fe_move_highlight_xy: XY no view found\n"));
		return;
	    }
	    STBDBG(("fe_move_highlight_xy: XY view found with stretched start box\n"));
#else
	    STBDBG(("fe_move_highlight_xy: XY no view found\n"));
	    return;
#endif
	}

	/* convert coords into relative to that view */
	wimp_get_wind_state(v->w, &state);
	coords_box_toworkarea(&cbox, (coords_cvtstr *)&state.o.box);
	
	/* ensure caret and pointer are in window */
	fe_get_wimp_caret(v->w);
	new_link = backend_highlight_link_xy(v->displaying, NULL, &cbox, flags);

	if (new_link == NULL)
	    new_link = backend_highlight_link(v->displaying, NULL, flags);

	if (new_link == NULL)
	    centre_pointer(v);

	STBDBG(("fe_move_highlight_xy: XY v %p link %p\n", v, new_link));
	return;
    }

    if (!v)
	return;

    old_link = v->displaying ? backend_read_highlight(v->displaying, NULL) : NULL;
    new_link = NULL;
    new_v = NULL;

    STBDBG(( "fe_move_highlight: old_link %p old_v %p new_link %p new_v %p scrolling %d\n", old_link, v, new_link, new_v, v->scrolling));

/*     pointer_mode = pointermode_ON;  */ /* so that scroll_changed doesn't reposition highlight  */

    if (v->parent || v->children)	/* frames */
    {
	wimp_box link_box;

	/* try and find link on page without using fallback link */
	if (v->displaying && (new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP_H)) != NULL)
	{
/* 	    v->current_link = new_link; */
	    STBDBG(("fe_move_highlight_xy: frames v %p link %p\n", v, new_link));
	    return;
	}

	/* get coordinates of current link in screen coords */
	if (old_link)		/* was v->current_link */
	{
	    coords_cvtstr cvt;
	    backend_doc_item_bbox(v->displaying, old_link, &link_box);

	    cvt = frameutils_get_cvt(v);
	    coords_box_toscreen(&link_box, &cvt);
	}

	/* move to frame link icon */
	if (1)
	{
	    coords_cvtstr cvt;
	    wimp_box box;
	    int side, x, y;
	    frame_link *link;

	    /* get coords relative to top level window */
/* 	    cvt = frameutils_get_cvt(frameutils_find_top(v)); */
	    box = link_box;
/* 	    coords_box_toworkarea(&box, &cvt); */

	    /* find centre of box */
	    x = (box.x0 + box.x1) / 2;
	    y = (box.y0 + box.y1) / 2;

	    /* find side to match */
	    side = direction_flags_to_side(flags);
	    
	    /* find closest link */
	    link = find_closest_frame_link(v, side, x, y, -1);

	    /* if we've found a matching link then return */
	    if (link)
	    {
		STBDBG(("fe_move_highlight_xy: frames v %p closest frame link %p side %d\n", v, link, link->side));
		backend_remove_highlight(v->displaying);

		link->flags |= frame_link_flag_SELECTED;
		redraw_link(v, link);
		return;
	    }
	}

	/* look for new frame */
	new_v = fe_locate_view_by_position(&v->box, (flags & (be_link_VERT | be_link_BACK)) | be_link_DONT_WRAP);

	/* if no new frame then try moving to toolbar */
	/* 20Jun: can't do this if it is a transient popup that is centered*/
	if (!new_v && (!v->open_transient ||
		       !(v->transient_position == fe_position_CENTERED_WITH_COORDS ||
			 v->transient_position == fe_position_CENTERED) ||
		       strcmp(v->name, TARGET_PASSWORD) == 0))
	{
	    if (/* (flags & be_link_VERT) &&  */move_to_toolbar(flags))
		return;
	}

	STBDBG(("fe_move_highlight_xy: frames v %p try scrolling\n", v));

	/* try scrolling and highlighting again */
	if (v->displaying && v->scrolling != fe_scrolling_NO)
	{
	    if (flags & be_link_VERT)
		scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1, 0);
	    else
		scrolled = fe_view_scroll_x(v, flags & be_link_BACK ? -1 : +1, 0);
	    
	    if (scrolled)
	    {
		if ((new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP_H)) != NULL)
		{
/* 		    v->current_link = new_link; */
		}

		/* return if scrolled whether or not new link found */
		STBDBG(("fe_move_highlight_xy: frames v %p scrolled link %p (current %p)\n", v, new_link, new_link));
		return;
	    }
	}

	/* if another frame then move to it and look for link */
	if (new_v)
	{
	    if (new_v->displaying)
	    {
		coords_cvtstr cvt;

		/* get coords relative to new window */
		cvt = frameutils_get_cvt(new_v);
		coords_box_toworkarea(&link_box, &cvt);
		
		new_link = backend_highlight_link_xy(new_v->displaying, NULL, &link_box, flags | be_link_XY);
		STBDBG(("fe_move_highlight_xy: frames v %p direct link %p\n", new_v, new_link));
	    }

	    /* give caret to new window */
	    fe_get_wimp_caret(new_v->w);
	    if (new_link == NULL)
	    {
		new_link = backend_highlight_link(new_v->displaying, NULL, flags);
		STBDBG(("fe_move_highlight_xy: frames v %p indirect link %p\n", new_v, new_link));
	    }

	    if (new_link == NULL)
	    {
		centre_pointer(new_v);
		STBDBG(("fe_move_highlight_xy: frames v %p set ptr %d,%d\n", new_v, (new_v->box.x1 - new_v->box.x0)/2, - (new_v->box.y1 - new_v->box.y0)/2));
	    }
	    return;
	}

	if (v->displaying && (new_link = backend_highlight_link(v->displaying, old_link, flags)) != NULL)
	{
	    STBDBG(("fe_move_highlight_xy: frames v %p last ditch with h wrapping %p\n", v, new_link));
	    return;
	}

	/* if can't move on then beep */
	STBDBG(("fe_move_highlight_xy: frames v %p can't find new frame\n", v));
	sound_event(snd_WARN_NO_FIELD);
    }
    else				/* no frames */
    {
	/* try and find link on page */
	if (v->displaying && (new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP_H)) != NULL)
	{
/* 	    v->current_link = new_link; */
	    STBDBG(("fe_move_highlight_xy: no frames v %p new link %p\n", v, new_link));
	    return;
	}

	/* try moving to toolbar */
	if (!v->open_transient ||
	    !(v->transient_position == fe_position_CENTERED_WITH_COORDS ||
	      v->transient_position == fe_position_CENTERED) ||
	    strcmp(v->name, TARGET_PASSWORD) == 0)
	{
	    if (move_to_toolbar(flags))
	    {
/*		v->current_link = NULL; */
		return;
	    }
	}

	if (v->displaying)
	{
	    if (v->scrolling != fe_scrolling_NO)
	    {
		if (flags & be_link_VERT)
		    scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1, 0);
		else
		    scrolled = fe_view_scroll_x(v, flags & be_link_BACK ? -1 : +1, 0);

		if (scrolled)
		{
		    if ((new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP_H)) != NULL)
		    {
/* 		    v->current_link = new_link; */
		    }

		    /* just check that we can see the highlight somewhere */
		    fe_ensure_highlight(v, flags);
		    
		    /* return if scrolled whether or not new link found */
		    STBDBG(("fe_move_highlight_xy: no frames v %p scrolled new link %p\n", v, new_link));
		    return;
		}
	    }

	    if ((new_link = backend_highlight_link(v->displaying, old_link, flags)) != NULL)
	    {
		STBDBG(("fe_move_highlight_xy: no frames v %p h wrapped new link %p\n", v, new_link));
		return;
	    }
	}

	/* give warning noise */
	STBDBG(("fe_move_highlight_xy: no frames v %p can't find new link %p\n", v, new_link));
	sound_event(snd_WARN_NO_FIELD);
    }
}

void fe_move_highlight_xy(fe_view v, wimp_box *box, int flags)
{
/*     pointer_mode = pointermode_ON; */  /* so that scroll_changed doesn't reposition highlight  */

    fe__move_highlight_xy(v, box, flags);

/*     fe_pointer_mode_update(pointermode_OFF); */
}

void fe_move_highlight(fe_view v, int flags)
{
    fe_move_highlight_xy(v, NULL, flags);
}

/* ------------------------------------------------------------------------------------------- */

/* the given window has just scrolled
 * check that the highlight is still visible

 * There are several cases here.
 * 1) The link was on the page and is wholly visible still on the page after scrolling.
 *	Leave where it was
 * 2) The link was on the page and is partially visible still on the page after scrolling.
 *	Counts as not visible
 * 3) The link was on the page and is not at all visible still on the page after scrolling.
 *	Highlight a link on the page if there was one
 * 4) The link wasn't on the page although the page had the focus
 *	Highlight a link on the page if there was one
 * 4) The link was on the toolbar
 *	Leave highlight on the toolbar
 */

int fe_ensure_highlight(fe_view v, int flags)
{
    if (pointer_mode == pointermode_OFF && v && v->displaying)
    {
	backend_highlight_link(v->displaying, backend_read_highlight(v->displaying, NULL),
			       (flags & be_link_BACK) | be_link_VISIBLE | be_link_INCLUDE_CURRENT |
			       /* caretise() |  */movepointer() | be_link_DONT_FORCE_ON);
    }
    return 0;
}

/* ------------------------------------------------------------------------------------------- */

/*
 * New frame link code.
 * A frame link is a small graphic that sits in the frame border around the active frame and
 * can be moved to and from and will transfer the focus to the frame beyond.
 */


/*
 * for each frame, see if it borders the initial frame.
 * if it does then work out where the link should reside.
 */

static int get_link_pos(int v0, int v1, int t0, int t1)
{
    int pos = 0;
    if (v0 >= t0 && v1 <= t1)		/* v is within target */
	pos = (v1 + v0) / 2;
    else if (v0 <= t0 && v1 >= t1)	/* v encompasses target */
	pos = (t0 + t1) / 2;
    else if (v1 >= t1)			/* v overlaps target on right */
	pos = (t1 + v0) / 2;
    else if (v0 <= t0)			/* v overlaps target on left */
	pos = (v1 + t0) / 2;
    return pos;
}

static os_error *fe_frame_link_array__build(fe_view v, void *handle)
{
    int *vals = handle;
    fe_view target = (fe_view) vals[0];
    wimp_box box;
    int side = -1;
    int icon_small = vals[2];
    int icon_large = vals[3];
    
    STBDBGN(("fe_frame_link: consider v %p sizes %dx%d\n", v, icon_small, icon_large));

    /* if this is not a child window then ignore immediately */
    if (v->children)
	return NULL;
    
    /* find out if this is a bordering frame */
    if (target->box.x1 >= v->box.x0 && target->box.x0 <= v->box.x1)
    {
	if (target->dividers[fe_divider_TOP] == v->dividers[fe_divider_BOTTOM])
	{
	    side = fe_divider_TOP;
	    box.y0 = target->box.y1;
	}
	else if (target->dividers[fe_divider_BOTTOM] == v->dividers[fe_divider_TOP])
	{
	    side = fe_divider_BOTTOM;
	    box.y0 = target->box.y0 - icon_small;
	}

	if (side != -1)
	{
	    box.x0 = get_link_pos(v->box.x0, v->box.x1, target->box.x0, target->box.x1) - icon_large/2;
	    box.x1 = box.x0 + icon_large;
	    box.y1 = box.y0 + icon_small;
	}
    }

    if (side == -1 && target->box.y1 >= v->box.y0 && target->box.y0 <= v->box.y1)
    {
	if (target->dividers[fe_divider_LEFT] == v->dividers[fe_divider_RIGHT])
	{
	    side = fe_divider_LEFT;
	    box.x0 = target->box.x0 - icon_small;
	}
	else if (target->dividers[fe_divider_RIGHT] == v->dividers[fe_divider_LEFT])
	{
	    side = fe_divider_RIGHT;
	    box.x0 = target->box.x1;
	}

	if (side != -1)
	{
	    box.y0 = get_link_pos(v->box.y0, v->box.y1, target->box.y0, target->box.y1) - icon_large/2;
	    box.y1 = box.y0 + icon_large;
	    box.x1 = box.x0 + icon_small;
	}
    }

    /* if it is then add to list */
    if (side != -1 && (target->dividers[side] & fe_divider_BORDERLESS) == 0)
    {
	frame_link *fl = mm_calloc(sizeof(*fl), 1);

	fl->next = (frame_link *)vals[1];
	vals[1] = (int)fl;

	fl->side = side;
	fl->v = v;
	fl->box = box;

	STBDBGN(("fe_frame_link: add v %p side %d box x:%d-%d y:%d-%d\n", v, side, box.x0, box.x1, box.y0, box.y1));
    }
    
    return NULL;
}

/* Given a frame, find the frames that border it and build a
 * frame_link list of all the details needed.
 */

void fe_frame_link_array_build(fe_view v)
{
    int vals[5];
    sprite_id id;
    sprite_info info;

    fe_frame_link_array_free(v);

    /* use the left arrow which gives its width as the small size and its height as the large size */
    id.tag = sprite_id_name;
    id.s.name = "flleft";
    if (sprite_readsize(resspr_area(), &id, &info) != NULL)
    {
	sprite_area *rom, *ram;
	_swix(Wimp_BaseOfSprites, _OUTR(0,1), &rom, &ram);
	if (sprite_readsize(ram, &id, &info) != NULL)
	    sprite_readsize(rom, &id, &info);
    }
    
    vals[0] = (int) v;		/* target view */
    vals[1] = NULL;		/* list is put in here */
    vals[2] = info.width*2;	/* icon's small side size (OS) - bbc_modevar() was crashing here in the ROM!!! */
    vals[3] = info.height*2;	/* icon's large side size (OS) */

    STBDBG(("fe_frame_link_array_build: v %p size %dx%d OS units\n", v, vals[2], vals[3]));

    iterate_frames(frameutils_find_top(v), fe_frame_link_array__build, vals);

    v->frame_links = (frame_link *)vals[1];
    STBDBG(("fe_frame_link_array_build: links %p\n", v->frame_links));
}

void fe_frame_link_array_free(fe_view v)
{
    frame_link *fl = v->frame_links;
    STBDBG(("fe_frame_link: v %p free %p\n", v, v->frame_links));
    while (fl)
    {
	frame_link *next = fl->next;
	mm_free(fl);
	fl = next;
    }
    v->frame_links = NULL;
}

/* ------------------------------------------------------------------------------------------- */

static frame_link *link_selected(fe_view v)
{
    frame_link *fl = NULL;
    if (v) for (fl = v->frame_links; fl; fl = fl->next)
    {
	if (fl->flags & frame_link_flag_SELECTED)
	    break;
    }

    return fl;
}

void fe_frame_link_move(fe_view v, int flags)
{
    frame_link *link = link_selected(v);
    if (link)
    {
	int side_movement = direction_flags_to_side(flags);
	frame_link *new_link;
	wimp_box box;
	
	box = link->box;
/* 	cvt = frameutils_get_cvt(frameutils_find_top(v)); */
/* 	coords_box_toscreen(&box, &cvt); */

	STBDBG(("fe_frame_link_move: v %p sides link %d movement %d\n", v, link->side, side_movement));
	
	/* if pressed in matching direction */
	if (link->side == side_movement)
	{
	    /* try scrolling */
	    if (scroll_by_flags(v, flags))
	    {
		STBDBG(("fe_frame_link_move: scrolled, returning\n"));
		return;
	    }

	    /* else move to next frame */
	    STBDBG(("fe_frame_link_move: to next frame\n"));
	    fe_move_highlight_xy(v, &box, flags | be_link_XY | be_link_VISIBLE);
	    return;
	}

	/* if pressed in opposite direction */
	if (link->side == opposite_side(side_movement))
	{
	    STBDBG(("fe_frame_link_move: back onto page\n"));

	    link->flags &= ~frame_link_flag_SELECTED;
	    redraw_link(v, link);
	    fe_move_highlight_xy(v, &box, flags | be_link_XY | be_link_VISIBLE);
	    return;
	}

	/* if pressed in other direction try moving to other links */
	if ((new_link = find_closest_frame_link(v, link->side, (box.x0 + box.x1)/2, (box.y0 + box.y1)/2, side_movement)) != NULL)
	{
	    STBDBG(("fe_frame_link_move: to new link %p\n", new_link));

	    link->flags &= ~frame_link_flag_SELECTED;
	    new_link->flags |= frame_link_flag_SELECTED;
	    redraw_link(v, link);
	    redraw_link(v, new_link);
	    return;
	}

	/* or scroll page */
	if (scroll_by_flags(v, flags))
	    return;

	sound_event(snd_WARN_NO_FIELD);
    }
}

void fe_frame_link_activate(fe_view v)
{
    frame_link *link = link_selected(v);
    STBDBG(("fe_frame_link_activate: v %p link %p\n", v, link));
    if (link)
    {
	fe_move_highlight_xy(v, &link->box, side_to_direction_flags(link->side) | be_link_XY | be_link_VISIBLE);
    }   
}

int fe_frame_link_selected(fe_view v)
{
    return link_selected(v) != NULL;
}

static os_error *fe_frame_link_clear(fe_view v, void *handle)
{
    frame_link *fl = link_selected(v);
    if (fl && (fl->flags & frame_link_flag_SELECTED))
    {
	fl->flags &= ~frame_link_flag_SELECTED;
	redraw_link(v, fl);
    }
    return NULL;
    NOT_USED(handle);
}

void fe_frame_link_clear_all(fe_view v)
{
    iterate_frames(frameutils_find_top(v), fe_frame_link_clear, NULL);
}


static os_error *fe_frame_link_redraw(fe_view v, void *handle)
{
    frame_link *fl;
    for (fl = v->frame_links; fl; fl = fl->next)
	redraw_link(v, fl);
    return NULL;
    NOT_USED(handle);
}

void fe_frame_link_redraw_all(fe_view v)
{
    iterate_frames(frameutils_find_top(v), fe_frame_link_redraw, NULL);
}

/* ------------------------------------------------------------------------------------------- */

int frontend_view_get_dividers(fe_view v, int *dividers)
{
    memcpy(dividers, v->dividers, sizeof(v->dividers));
    return frameutils_find_top(v)->dividers_max;
}

/* ------------------------------------------------------------------------------------------- */

void fe_cursor_movement(fe_view v, int x, int y)
{
    int flags;

    if (y && use_toolbox)
	tb_status_set_direction(y > 0);

    flags = be_link_TEXT | be_link_CARETISE; /* always caretise as its coming from the on screen keyboard */

    if (y > 0)
	flags |= be_link_BACK;

    if (on_screen_kbd == 0)
	flags |= movepointer();
    
    /* v->current_link = */ backend_highlight_link(v->displaying, backend_read_highlight(v->displaying, NULL), flags);
}

/* ------------------------------------------------------------------------------------------- */

/*
 * Activate a link, called from a mouse click and a press of enter (bbits == 0)
 * Passes through to backend except for
 *   Imagemap with key press - go into map mode
 *   Text input item with no keyboard - bring up OSK
 */

os_error *fe_activate_link(fe_view v, int x, int y, int bbits)
{
    be_item ti;
    os_error *e;
    wimp_box box;
    int flags;

    if (!v || !v->displaying)
	return NULL;

    if (bbits)
    {
	wimp_wstate state;
	coords_pointstr p;

	wimp_get_wind_state(v->w, &state);
	p.x = x;
	p.y = y;
	coords_point_toworkarea(&p, (coords_cvtstr *)&state.o.box);
	x = p.x;
	y = p.y;
	if ((e = backend_doc_locate_item(v->displaying, &p.x, &p.y, &ti)) != NULL)
	    return e;
    }
    else
	ti = backend_read_highlight(v->displaying, NULL); /* v->current_link; */
    
    STBDBG(( "stbfe: activate link %p x %d y %d bbits %d\n", ti, x, y, bbits));

    if (!ti)
	return NULL;
    
    if ((e = backend_item_info(v->displaying, ti, &flags, NULL, NULL)) != NULL)
	return e;
    
    if ((e = backend_doc_item_bbox(v->displaying, ti, &box)) != NULL)
	return e;

    if (bbits == 0 && (flags & (be_item_info_ISMAP | be_item_info_USEMAP)))
    {
	frontend_pointer_set_position(v, (box.x0 + box.x1)/2, (box.y0 + box.y1)/2);
	fe_map_mode(v, ti);
    }
    else
    {
	/* if we are clicking in a text item, that is highlighted, and doesn't have the caret */
	BOOL had_caret, need_caret;

	had_caret = FALSE;

	/* this needs to check whether it is NUMBERS or not */
	need_caret = (flags & (be_item_info_INPUT|be_item_info_NUMBERS)) == be_item_info_INPUT && 
	    backend_read_highlight(v->displaying, &had_caret) == ti &&
	    !had_caret;
	
	if (bbits)
	{
	    last_click_x = x;
	    last_click_y = y;
	    last_click_view = v;
	
	    e = backend_doc_click(v->displaying, x, y, (wimp_bbits)bbits);

	    if (need_caret && !caretise())
		fe_keyboard_open(v);
	}
	else
	{
	    last_click_x = box.x0;
	    last_click_y = box.y0;
	    last_click_view = v;
	    
	    if (flags & be_item_info_INPUT)
 		backend_highlight_link(v->displaying, ti, movepointer() | be_link_TEXT | be_link_CARETISE | be_link_INCLUDE_CURRENT);
	    else
		e = backend_activate_link(v->displaying, ti, 0);

	    if (need_caret && !caretise())
		fe_keyboard_open(v);
	}
    }

    return e;
}

os_error *fe_handle_enter(fe_view v)
{
    return fe_activate_link(v, 0, 0, 0);
}

/* ------------------------------------------------------------------------------------------- */

/* eof stblinks.c */
