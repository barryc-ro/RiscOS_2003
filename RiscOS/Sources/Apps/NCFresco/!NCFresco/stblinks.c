/* > stblinks.c

 *

 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "interface.h"
#include "memwatch.h"

#include "config.h"
#include "util.h"

#include "stbfe.h"
#include "stbtb.h"
#include "stbview.h"
#include "stbutils.h"

/* ------------------------------------------------------------------------------------------- */

/*
 * Using the vert and back flags travel out from box and work out what view we end up in
 */

static os_error *fe__locate_view_by_position(fe_view v, void *handle)
{
    int *vals = handle;
    int pdist, sdist, s1, s2;

    if (v->children)
	return NULL;
    
    pdist = s1 = s2 = 0;
    switch (vals[4] & (be_link_VERT|be_link_BACK))
    {
    case 0:			/* -> */
	pdist = v->box.x0 - vals[2];

	s1 = abs(v->box.y0 - vals[3]);
	s2 = abs(v->box.y1 - vals[3]);
	break;

    case be_link_VERT:		/* \/ */
	pdist = vals[3] - v->box.y1;

	s1 = abs(v->box.x0 - vals[2]);
	s2 = abs(v->box.x1 - vals[2]);
	break;

    case be_link_VERT | be_link_BACK: /* /\ */
	pdist = v->box.y0 - vals[3];

	s1 = abs(v->box.x0 - vals[2]);
	s2 = abs(v->box.x1 - vals[2]);
	sdist = s1 < s2 ? s1 : s2;
	break;

    case be_link_BACK:		/* <- */
	pdist = vals[2] - v->box.x1;

	s1 = abs(v->box.y0 - vals[3]);
	s2 = abs(v->box.y1 - vals[3]);
	break;
    }

    sdist = s1 < s2 ? s1 : s2;

    if (pdist > 0 &&
	(pdist < vals[0] || (pdist == vals[0] && sdist > 0 && sdist < vals[1])))
    {
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
    wimp_box box = fe_get_cvt(v).box;
    fe_move_highlight_xy(v, &box, flags | be_link_XY | be_link_VISIBLE | be_link_DONT_WRAP_H | be_link_DONT_WRAP);
}

void fe_move_highlight_frame(fe_view v, BOOL next)
{
    fe_view vv;

    vv = fe_next_frame(v ? v : main_view, next);
    if (vv == NULL)
    {
        vv = fe_find_top(v);	/* was main_view */
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
            vv->current_link = backend_highlight_link(vv->displaying, NULL, (next ? 0 : be_link_BACK) | be_link_VISIBLE | caretise() | be_link_MOVE_POINTER);
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
    if (use_toolbox &&
	config_mode_cursor_toolbar && tb_is_status_showing() &&
	((config_display_control_top && (flags & be_link_BACK)) || (!config_display_control_top && (flags & be_link_BACK) == 0)))
    {
	fe_pointer_mode_update(pointermode_OFF);

	if (tb_status_highlight(TRUE))
	    return TRUE;

	pointer_mode = pointermode_ON;
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
    flags |= caretise() | be_link_MOVE_POINTER;
    
    /* move from given position - ignore any current link */
    if ((flags & be_link_XY) && box)
    {
	wimp_box cbox;
	wimp_wstate state;

	/* decide which view we are going to jump into */
	if ((v = fe_locate_view_by_position(box, flags)) == NULL)
	    return;
	
	/* convert coords into relative to that view */
	cbox = *box;
	
	wimp_get_wind_state(v->w, &state);
	coords_box_toworkarea(&cbox, (coords_cvtstr *)&state.o.box);
	
	/* see what we can find */
	v->current_link = backend_highlight_link_xy(v->displaying, NULL, &cbox, flags);

	/* ensure caret and pointer are in window */
	fe_get_wimp_caret(v->w);
	if (v->current_link == NULL)
	    frontend_pointer_set_position(v, (v->box.x1 - v->box.x0)/2, (v->box.y1 - v->box.y0)/2);

	return;
    }

    if (!v)
	return;

    old_link = v->displaying ? backend_read_highlight(v->displaying, NULL) : NULL;
    new_link = NULL;
    new_v = NULL;

    STBDBG(( "fe_move_highlight: old_link %p old_v %p new_link %p new_v %p\n", old_link, v, new_link, new_v));

/*     pointer_mode = pointermode_ON;  */ /* so that scroll_changed doesn't reposition highlight  */

    if (v->parent || v->children)	/* frames */
    {
	/* try and find link on page without using fallback link */
	if (v->displaying && (new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP | be_link_DONT_WRAP_H)) != NULL)
	{
	    v->current_link = new_link;
	    return;
	}

	/* move to frame link icon */
	if (0)
	{
	}
	else
	{
	    /* try scrolling and highlighting again */
	    if (v->displaying && v->scrolling != fe_scrolling_NO)
	    {
		if (flags & be_link_VERT)
		    scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1);
		else
		    scrolled = fe_view_scroll_x(v, flags & be_link_BACK ? -1 : +1);

		if (scrolled)
		{
		    if ((new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP)) != NULL)
		    {
			v->current_link = new_link;
		    }

		    /* return if scrolled whether or not new link found */
		    return;
		}
	    }
	}

	/* no scroll no wrap - find next frame */
	new_v = fe_locate_view_by_position(&v->box, flags);

	/* if another frame then move to it and look for link */
	if (new_v)
	{
	    if (new_v->displaying)
		v->current_link = backend_highlight_link_xy(new_v->displaying, NULL, &v->box, flags | be_link_XY);

	    /* give caret to new window */
	    fe_get_wimp_caret(new_v->w);
	    if (v->current_link == NULL)
		v->current_link = backend_highlight_link(v->displaying, NULL, flags | be_link_DONT_WRAP);
	    return;
	}

	if (move_to_toolbar(flags))
	    return;
	
	/* if can't move on then beep */
	sound_event(snd_WARN_NO_FIELD);
    }
    else				/* no frames */
    {
	/* try and find link on page */
	if (v->displaying && (new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP)) != NULL)
	{
	    v->current_link = new_link;
	    return;
	}

	/* try moving to toolbar */
	if (move_to_toolbar(flags))
	{
	    v->current_link = NULL;
	    return;
	}

	if (v->displaying && v->scrolling != fe_scrolling_NO)
	{
	    if (flags & be_link_VERT)
		scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1);
	    else
		scrolled = fe_view_scroll_x(v, flags & be_link_BACK ? -1 : +1);

	    if (scrolled)
	    {
		if ((new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP)) != NULL)
		{
		    v->current_link = new_link;
		}

		/* return if scrolled whether or not new link found */
		return;
	    }
	}

	/* give warning noise */
	sound_event(snd_WARN_NO_FIELD);
    }
}

void fe_move_highlight_xy(fe_view v, wimp_box *box, int flags)
{
    pointer_mode = pointermode_ON;  /* so that scroll_changed doesn't reposition highlight  */

    fe__move_highlight_xy(v, box, flags);

    fe_pointer_mode_update(pointermode_OFF);
}

void fe_move_highlight(fe_view v, int flags)
{
    fe_move_highlight_xy(v, NULL, flags);
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
    
    STBDBG(("fe_frame_link: consider v %p\n", v));

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
	    box.x1 = box.x0 + icon_large/2;
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
	    box.y1 = box.y0 + icon_large/2;
	    box.x1 = box.x0 + icon_small;
	}
    }

    /* if it is then add to list */
    if (side != -1)
    {
	frame_link *fl = mm_calloc(sizeof(*fl), 1);

	fl->next = (frame_link *)vals[1];
	vals[1] = (int)fl;

	fl->side = side;
	fl->v = v;
	fl->box = box;

	STBDBG(("fe_frame_link: add v %p side %d box x:%d-%d y:%d-%d\n", v, side, box.x0, box.x0, box.y0, box.y1));
    }
    
    return NULL;
}

#define ICON_SIZE_SMALL	16
#define ICON_SIZE_LARGE	32

/* Given a frame, find the frames that border it and build a
 * frame_link list of all the details needed.
 */

void fe_frame_link_array_build(fe_view v)
{
    int vals[5];

    fe_frame_link_array_free(v);

    vals[0] = (int) v;		/* target view */
    vals[1] = NULL;		/* list is put in here */
    vals[2] = ICON_SIZE_SMALL;	/* icon's small side size (OS) */
    vals[3] = ICON_SIZE_LARGE;	/* icon's large side size (OS) */

    STBDBG(("fe_frame_link_array_build: v %p\n", v));

    iterate_frames(fe_find_top(v), fe_frame_link_array__build, vals);

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

int frontend_view_get_dividers(fe_view v, int *dividers)
{
    memcpy(dividers, v->dividers, sizeof(v->dividers));
    return fe_find_top(v)->dividers_max;
}

/* ------------------------------------------------------------------------------------------- */

void fe_cursor_movement(fe_view v, int x, int y)
{
    int flags;

    if (y)
	tb_status_set_direction(y > 0);

    flags = be_link_TEXT | be_link_CARETISE; /* always caretise as its coming from the on screen keyboard */

    if (y > 0)
	flags |= be_link_BACK;

    if (on_screen_kbd == 0)
	flags |= be_link_MOVE_POINTER;
    
    v->current_link = backend_highlight_link(v->displaying, v->current_link, flags);					     
}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_handle_enter(fe_view v)
{
    int flags;
    char *link;
    os_error *e;
    void *im;
    wimp_box box;

    STBDBG(( "stbfe: handle enter in %p\n", v->current_link));

    if (!v || !v->displaying || !v->current_link)
        return NULL;

    e = backend_item_info(v->displaying, v->current_link, &flags, &link, &im);
    if (!e)
        e = backend_doc_item_bbox(v->displaying, v->current_link, &box);
    if (!e)
    {
        if ((flags & (be_item_info_ISMAP | be_item_info_USEMAP)))
        {
            coords_pointstr p;
/*             coords_cvtstr cvt = fe_get_cvt(v); */

            p.x = (box.x0 + box.x1)/2;
            p.y = (box.y0 + box.y1)/2;
/*             coords_point_toscreen(&p, &cvt); */

            frontend_pointer_set_position(v, p.x, p.y);
            fe_map_mode(v, v->current_link);
        }
        else if (flags & be_item_info_INPUT)
        {
	    STBDBG(( "stbfe: activate link in %p\n", v->current_link));

/*             e = backend_activate_link(v->displaying, v->current_link, 0); */

	    v->current_link = backend_highlight_link(v->displaying, v->current_link, be_link_MOVE_POINTER | be_link_TEXT | be_link_VERT | be_link_CARETISE | be_link_INCLUDE_CURRENT);

	    fe_keyboard_open(v);
	}
        else
        {
            last_click_x = box.x0;
            last_click_y = box.y0;
            last_click_view = v;

	    STBDBG(( "stbfe: activate link in %p\n", v->current_link));

	    e = backend_activate_link(v->displaying, v->current_link, 0);
	}
    }

    return e;
}

/* ------------------------------------------------------------------------------------------- */

/* eof stblinks..c */

   
