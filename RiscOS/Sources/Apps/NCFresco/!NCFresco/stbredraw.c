/* > stbredraw,c

 */


#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "bbc.h"
#include "colourtran.h"
#include "wimp.h"

#include "config.h"
#include "consts.h"
#include "interface.h"
#include "util.h"
#include "stbview.h"
#include "stbutils.h"
#include "stbfe.h"
#include "debug.h"

#define REDRAW_OVERLAP	4	/* OS units overlap between wimp rectangles to allow for anti-twittering */

/* ----------------------------------------------------------------------------*/

static void get_dimensions(fe_view v, const wimp_openstr *op, fe_view_dimensions *fvd);

/* ----------------------------------------------------------------------------*/

static int fe_scroll_request(fe_view v, wimp_openstr *o, int x, int y)
{
    int dx, dy;
    int signx, signy;
    int smooth_scroll = config_display_smooth_scrolling;
    int mask = smooth_scroll ? smooth_scroll - 1 : 0;
    BOOL scrolled = FALSE;
    int old_x = o->x;
    int old_y = o->y;

    signx = +1;
    dx = 0;
    switch (x)
    {
        case 3:
            signx = -1;
	    o->x = v->doc_width + (o->box.x1 - o->box.x0) - v->margin.x1;
	    smooth_scroll = 0;
	    break;
        case -3:
	    o->x = -v->margin.x0;
	    smooth_scroll = 0;
	    break;

        case -2:
            signx = -1;
            /* deliberate*/
        case 2:
            dx = ((o->box.x1 - o->box.x0)&~mask) - 8;
            break;

        case -1:
            signx = -1;
            /* deliberate*/
        case 1:
            dx = 32;
            break;

    default:
	dx = abs(x);
	signx = x < 0 ? -1 : +1;
	break;
    }

    if (dx && smooth_scroll)
        for (; dx > 0; dx -= smooth_scroll)
        {
            o->x += MIN(dx, smooth_scroll)*signx;
            frontend_fatal_error(wimp_open_wind(o));
/*             fe_event_process(); */
        }


    signy = +1;
    dy = 0;
    switch (y)
    {
        case -2:
            signy = -1;
            /* deliberate*/
        case 2:
            dy = ( ( (o->box.y1+v->margin.y1) - (o->box.y0+v->margin.y0) ) &~ mask ) - 32;
/*             if (dy < 0) */
/*  	        dy = 32; */
            break;

        case -1:
            signy = -1;
            /* deliberate*/
        case 1:
            dy = 32;
            break;

        case -3:
            signy = -1;
	    o->y = v->doc_height + (o->box.y1 - o->box.y0) - v->margin.y0;
	    smooth_scroll = 0;
	    break;
        case 3:
	    o->y = -v->margin.y1;
	    smooth_scroll = 0;
	    break;

    default:
	dy = abs(y);
	signy = y < 0 ? -1 : +1;
	break;
    }

    if (dy && smooth_scroll)
        for (; dy > 0; dy -= smooth_scroll)
        {
            o->y += MIN(dy, smooth_scroll)*signy;
            frontend_fatal_error(wimp_open_wind(o));
/*             fe_event_process(); */
        }

    if (!smooth_scroll)
    {
        o->x += dx*signx;
        o->y += dy*signy;
        frontend_fatal_error(wimp_open_wind(o));
    }
    
    if (o->x != old_x || o->y != old_y)
	scrolled = TRUE;

    if (scrolled)
	fe_scroll_changed(v);

    if (v->stretch_document && signy > 0)
    {
        fe_view_dimensions fvd;
        int stretch;

        get_dimensions(v, o, &fvd);

        stretch = (fvd.wa_height - v->margin.y0) - ((o->y + v->margin.y1) + fvd.min_height);
        if (stretch < 0)
            stretch = 0;

        if (stretch != v->stretch_document)
        {
            v->stretch_document = stretch;
            frontend_view_set_dimensions(v, 0, 0);
        }
    }

    return scrolled;
}

#if 0
static void draw_view_outline(wimp_w w)
{
    wimp_redrawstr outline;
    wimp_paletteword col;
    int gcol;

    outline.w = w;
    wimp_getwindowoutline(&outline);

    col.word = 0x88888800;
    colourtran_setGCOL(col, 0, 0, &gcol);
    bbc_rectangle(outline.box.x0-2, outline.box.y0-2, outline.box.x1-outline.box.x0+2*2-1, outline.box.y1-outline.box.y0+2*2-1);
    bbc_rectangle(outline.box.x0-4, outline.box.y0-4, outline.box.x1-outline.box.x0+2*4-1, outline.box.y1-outline.box.y0+2*4-1);
}
#endif

/* ----------------------------------------------------------------------------*/

int fe_view_scroll_x(fe_view v, int val)
{
    if (v->w)
    {
        wimp_wstate state;
        wimp_get_wind_state(v->w, &state);
        return fe_scroll_request(v, &state.o, val, 0);
    }
    return 0;
}

int fe_view_scroll_y(fe_view v, int val)
{
    if (v->w)
    {
        wimp_wstate state;
        wimp_get_wind_state(v->w, &state);
        return fe_scroll_request(v, &state.o, 0, val);
    }
    return 0;
}

/* ----------------------------------------------------------------------------*/

int frontend_view_redraw(fe_view v, wimp_box *bb)
{
    wimp_redrawstr r;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    if (!v->w)
        return 0;

    r.w = v->w;
    if (bb)
    {
	r.box = *bb;
        /* expand redraw box to allow for anti-twittering*/
        /* the wimp will clip this to the window*/
/*        r.box.y0 -= 2;*/
/*        r.box.y1 += 2;*/
    }
    else
    {
	r.box.x0 = -v->margin.x0;
	r.box.y1 = -v->margin.y1;
	r.box.x1 = 1 << 30;
	r.box.y0 = - r.box.x1;
    }

    wimp_force_redraw(&r);

    return 0;
}

int frontend_view_update(fe_view v, wimp_box *bb, fe_rectangle_fn fn, void *h, int flags)
{
    int            more;
    wimp_redrawstr r;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

    r.w = v->w;
    r.box = *bb;

    /* expand redraw box to allow for anti-twittering*/
    /* the wimp will clip this to the window*/
    r.box.y0 -= REDRAW_OVERLAP;
    r.box.y1 += REDRAW_OVERLAP;

    frontend_fatal_error(wimp_update_wind(&r, &more));

    while (more)
    {
	/* 3rd param must be 0 if we want the background to be redrawn by the backend redraw function 
	 * If it is 1 then it assumes we have already cleared the background or don't care about overpainting.
	 */
	fn(&r, h, (flags & fe_update_WONT_PLOT_ALL) == 0 || (flags & fe_update_IMAGE_RENDERING) != 0);

#if 0
	/* if we are top view above a selected view and are in web mode */
        if (fe_find_top(selected_view) == v && v->browser_mode == fe_browser_mode_WEB)
            draw_view_outline(selected_view->w);
#endif

        if ((flags & fe_update_IMAGE_RENDERING) == 0)
            fe_anti_twitter(&r.g);

	wimp_get_rectangle(&r, &more);
    }

    return 0;
}

int frontend_view_block_move(fe_view v, wimp_box *bb, int newx, int newy)
{
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    if (v->w)
        frontend_fatal_error(wimp_blockcopy(v->w, bb, newx, newy));

    return 0;
}

static void get_dimensions(fe_view v, const wimp_openstr *op, fe_view_dimensions *fvd)
{
    fvd->user_width = (op->box.x1 + v->margin.x1) - (op->box.x0 + v->margin.x0);
    fvd->doc_width = v->doc_width;
    fvd->wa_width = fvd->user_width > fvd->doc_width ? fvd->user_width : fvd->doc_width;

    fvd->min_height = - (op->box.y1 + v->margin.y1) + (op->box.y0 + v->margin.y0);
    fvd->doc_height = v->doc_height;
    /* This looks the wrong way around because height goes down from the top */
    fvd->wa_height = fvd->min_height < fvd->doc_height ? fvd->min_height : fvd->doc_height;
/*    fvd->wa_height = fvd->min_height + fvd->doc_height;*/

    if (v->parent == NULL)
    {
        fvd->layout_width = fvd->user_width;
        fvd->layout_height = fvd->min_height;
    }
    else
    {
#if 1
        fvd->layout_width  =    v->box.x1 - v->box.x0;
        fvd->layout_height = - (v->box.y1 - v->box.y0);
#else
        fvd->layout_width = op->box.x1 - op->box.x0 + (v->y_scroll_bar ? toolsprite_width : 0);/* + 2*2;*/
        fvd->layout_height = - (op->box.y1 - op->box.y0) - (v->y_scroll_bar ? toolsprite_height : 0);/* + 2*2);*/
#endif
    }
}

int frontend_view_get_dimensions(fe_view v, fe_view_dimensions *fvd)
{
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    if (v->w)
    {
        wimp_wstate state;
        wimp_get_wind_state(v->w, &state);

        get_dimensions(v, &state.o, fvd);
    }
    else
    {
        fvd->wa_width  = fvd->doc_width  = fvd->layout_width  = fvd->user_width =    v->box.x1 - v->box.x0;
        fvd->wa_height = fvd->doc_height = fvd->layout_height = fvd->min_height = - (v->box.y1 - v->box.y0);
    }

    return 0;
}

int frontend_view_set_dimensions(fe_view v, int width, int height)
{
    wimp_redrawstr r;
    int bbh, sbh;
    wimp_wstate ws;
    BOOL old_y_scroll_bar;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    old_y_scroll_bar = v->y_scroll_bar;

    if (width)
	v->doc_width = width;
    if (height)
	v->doc_height = height;

/*
    if (v->sb)
	statusbar_bar_heights(v->sb, &bbh, &sbh);
    else
 */
	bbh = sbh = 0;

    /* if not top view*/
    if (v->parent)
    {
        /* if we have subframes*/
        if (v->children)
        {
            if (v->w)
            {
    STBDBGN(("delete window %x of %p\n", v->w, v));
                wimp_delete_wind(v->w);
                v->w = 0;

                /* when we delete the window this invalidates these fields */
                v->x_scroll_bar = v->y_scroll_bar = FALSE;
                /* memset(&v->margin, 0, sizeof(v->margin)); */
            }
        }
        else
        {
            if (!v->w)
            {
                wimp_box box = v->box;
                feutils_window_create(&box, &v->margin, NULL, fe_bg_colour(v), &v->w);
    STBDBGN(("recreate win %x from view %p\n", v->w, v));
            }
        }
    }

    if (v->w)
    {
	int need_reopen = 0;
	fe_view_dimensions fvd;

	frontend_fatal_error(wimp_get_wind_state(v->w, &ws));
	get_dimensions(v, &ws.o, &fvd);

	r.box.x0 = - v->margin.x0;
	r.box.y1 = bbh - v->margin.y1;
#if 1
	r.box.x1 = fvd.wa_width - v->margin.x1;
	r.box.y0 = fvd.wa_height - (sbh + v->margin.y0) - v->stretch_document;
#else
	r.box.x0 = - v->margin.x0;
	r.box.y1 = bbh - v->margin.y1;
	r.box.x1 = v->width > v->doc_width ? v->width : v->doc_width;
	r.box.x1 -= v->margin.x1;
	/* This looks the wrong way around because height goes down from the top */
	r.box.y0 = v->height < v->doc_height ? v->height : v->doc_height;
	r.box.y0 -= sbh + v->margin.y0;
#endif

	/* Scroll position to high up */
	if (ws.o.y > r.box.y1)
	{
	    /* Set scroll position to highest pos. */
	    ws.o.y = r.box.y1;
	    need_reopen = 1;
	}

	/* Bottom of window below bottom of work area */
	if (ws.o.y - (ws.o.box.y1 - ws.o.box.y0) < r.box.y0)
	{
	    /* If view is too tall make view as tall as is legal */
	    if ((ws.o.box.y1 - ws.o.box.y0) > (r.box.y1 - r.box.y0))
	    {
		ws.o.box.y0 = ws.o.box.y1 - (r.box.y1 - r.box.y0);
	    }
	    /* Move bottom of window within work area */
	    ws.o.y = r.box.y0 + (ws.o.box.y1 - ws.o.box.y0);
	    need_reopen = 1;
	}

	/* Right hand edge of edge of work area */
	if (ws.o.x + (ws.o.box.x1 - ws.o.box.x0) > r.box.x1)
	{
	    /* If view is too wide make view as wide as is legal. */
	    if ((ws.o.box.x1 - ws.o.box.x0) > (r.box.x1 - r.box.x0))
	    {
		ws.o.box.x1 = ws.o.box.x0 + (r.box.x1 - r.box.x0);
	    }
	    /* Move right hand edge into view */
	    ws.o.x = r.box.x1 - (ws.o.box.x1 - ws.o.box.x0);
	    need_reopen = 1;
	}

        /* check and see if scroll bars have appeared or disappeared*/
        if (v->scrolling == fe_scrolling_AUTO && feutils_check_window_bars(&r.box, &ws.o, &v->x_scroll_bar, &v->y_scroll_bar, fe_bg_colour(v)))
        {
            v->w = ws.o.w;
        }
        else
        {
	    r.w = v->w;
	    wimp_set_extent(&r);

	    if (need_reopen)
	    {
/*	    ws.o.behind = statusbar_fix_pos(&ws.o, v->sb);*/
#if DEBUG
                fprintf(stderr, "set dimensions %d,%d %d,%d (%d,%d)\n",
                    ws.o.box.x0, ws.o.box.y0, ws.o.box.x1, ws.o.box.y1, ws.o.x, ws.o.y);
#endif
	        frontend_fatal_error(wimp_open_wind(&ws.o));
                fe_scroll_changed(v);
/*	    statusbar_fix_pos(&ws.o, v->sb);*/
            }
	}
    }

    return old_y_scroll_bar != v->y_scroll_bar && v->w != 0;
}

int frontend_view_bounds(fe_view v, wimp_box *box)
{
    wimp_wstate ws;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    if (v->w)
        frontend_fatal_error(wimp_get_wind_state(v->w, &ws));
    else
    {
        ws.o.box = v->box;
        ws.o.x = -v->margin.x0;
        ws.o.y = -v->margin.y1;
    }

    box->y1 = ws.o.y;
    box->y0 = ws.o.y - (ws.o.box.y1 - ws.o.box.y0);

    if (config_display_control_top)
	box->y1 -= fe_status_height_top(v);
    else
	box->y0 += fe_status_height_top(v);

    box->x0 = ws.o.x;
    box->x1 = ws.o.x + (ws.o.box.x1 - ws.o.box.x0);
#if 0
fprintf(stderr, "stbredraw: view bounds  %d,%d %d,%d (status %d)\n", box->x0, box->y0, box->x1, box->y1, fe_status_height_top(v));
#endif
    return 1;
}

int frontend_view_margins(fe_view v, wimp_box *box)
{
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

#if USE_MARGINS
    *box = v->backend_margin;
#else
    *box = v->margin;

    if (fe_status_height_top(v))
        box->y1 = -STATUS_TOP_MARGIN;

    STBDBGN(("stbredraw: view margins %d,%d %d,%d (status %d)\n", box->x0, box->y0, box->x1, box->y1, fe_status_height_top(v)));
#endif
    return 1;
}

int frontend_view_ensure_visable(fe_view v, int x, int top, int bottom)
{
    wimp_wstate state;
    int w, h;
    int mh;
    int need_to_reopen = 0;
    int need_to_set_dims = 0;
    int bbh, sbh;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

    frontend_fatal_error(wimp_get_wind_state(v->w, &state));

    bbh = - v->margin.y1;
    sbh =   v->margin.y0;
    h = (state.o.box.y1 - bbh) - (state.o.box.y0 + sbh);    /* height of visible area (within margins)*/
    mh = h > -(v->doc_height) ? h : -(v->doc_height);       /* height of document, or visible area (+ve)*/
    w = state.o.box.x1 - state.o.box.x0;

    if (v->stretch_document)
    {
        v->stretch_document = 0;
        need_to_set_dims = 1;
    }

    if (top == bottom && top - h < -mh /*- sbh*/)
    {
        v->stretch_document = (-mh/* - sbh*/) - (top - h);
        need_to_set_dims = 1;
    }

    if (top == bottom ||	/* Special case: force to the top is top and bottom equal */
	top > (state.o.y - bbh) ||
	bottom < (state.o.y + sbh - (state.o.box.y1 - state.o.box.y0)) )
    {
        state.o.y = top;
	state.o.y += bbh;

	need_to_reopen = 1;
    }

    if ((x != -1) && (x < state.o.x + v->margin.x0))
    {
	/* It is off the left, put it at the left edge */
	state.o.x = x - v->margin.x0;
	need_to_reopen = 1;
    }

    if ((x != -1) && (x > state.o.x + w + v->margin.x1))
    {
	/* It is off the right, put it at the right */
	state.o.x = x - w - v->margin.x1;
	need_to_reopen = 1;
    }

    if (need_to_set_dims)
    {
        frontend_view_set_dimensions(v, 0, 0);
    }

    if (need_to_reopen)
    {
	STBDBGN(("ensure %d-%d visible %d,%d %d,%d (%d,%d) height %d\n",
		 top, bottom,
		 state.o.box.x0, state.o.box.y0, state.o.box.x1, state.o.box.y1, state.o.x, state.o.y, v->doc_height));

	frontend_fatal_error(wimp_open_wind(&state.o));

        fe_scroll_changed(v);
    }

    return 1;
}

/* ----------------------------------------------------------------------------*/

int frontend_view_caret(fe_view v, int x, int y, int height, int on_screen)
{
    wimp_caretstr cs;
    int r;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

    STBDBGN(("viewcaret: @ %d,%d h %d on %d current %p\n", x, y, height, on_screen, v->current_link));

    if (on_screen && height > 0)
    {
	frontend_view_ensure_visable(v, x, y + height, y);
    }

    cs.w = v->w;
    cs.i = -1;
    if (height < 0)
	cs.height = (-height) | (1 << 25);	/* Invisible caret */
    else
	cs.height = height;
    cs.index = 0;
    cs.x = x;
    cs.y = y;

    r = frontend_complain(wimp_set_caret_pos(&cs)) == NULL;

    if (height >= 0 && v->current_link)
    {
        backend_update_link(v->displaying, v->current_link, 0);
        v->current_link = backend_place_caret(v->displaying, backend_place_caret_READ);
    }

    STBDBGN(("viewcaret: end %p \n", v->current_link));

    return r;
}

int frontend_view_has_caret(fe_view v)
{
    wimp_caretstr cs;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    return (wimp_get_caret_pos(&cs) == NULL && cs.w == v->w);
}

/* ----------------------------------------------------------------------------*/

/* eof stbredraw.c */
