/* > stbredraw,c

 */


#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "bbc.h"
#include "colourtran.h"
#include "wimp.h"

#include "interface.h"
#include "render.h"
#include "rcolours.h"
#include "gbf.h"

#include "config.h"
#include "consts.h"
#include "util.h"
#include "stbview.h"
#include "stbutils.h"
#include "stbfe.h"
#include "debug.h"
#include "stbtb.h"

#define REDRAW_OVERLAP	4	/* OS units overlap between wimp rectangles to allow for anti-twittering */

/* ----------------------------------------------------------------------------*/

static void get_dimensions(fe_view v, const wimp_openstr *op, fe_view_dimensions *fvd);

/* ----------------------------------------------------------------------------*/

int fe_scroll_request(fe_view v, wimp_openstr *o, int x, int y)
{
    int dx, dy;
    int signx, signy;
    int smooth_scroll = config_display_smooth_scrolling;
    int mask = smooth_scroll ? smooth_scroll - 1 : 0;
    BOOL scrolled = FALSE;
    int old_x = o->x;
    int old_y = o->y;
    int snd = snd_NONE;

    signx = +1;
    dx = 0;
    switch (x)
    {
        case 3:
            signx = -1;
	    o->x = v->doc_width + (o->box.x1 - o->box.x0) - v->margin.x1;
	    smooth_scroll = 0;
	    snd = snd_SCROLL_LIMIT;
	    break;
        case -3:
	    o->x = -v->margin.x0;
	    smooth_scroll = 0;
	    snd = snd_SCROLL_LIMIT;
	    break;

        case -2:
            signx = -1;
            /* deliberate*/
        case 2:
            dx = ((o->box.x1 - o->box.x0)&~mask) - 8;
	    snd = snd_SCROLL_PAGE;
            break;

        case -1:
            signx = -1;
            /* deliberate*/
        case 1:
            dx = 32;
	    snd = snd_SCROLL_LINE;
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
	    snd = snd_SCROLL_PAGE;
            break;

        case -1:
            signy = -1;
            /* deliberate*/
        case 1:
            dy = 32;
	    snd = snd_SCROLL_LINE;
            break;

        case -3:
            signy = -1;
	    o->y = v->doc_height + (o->box.y1 - o->box.y0) - v->margin.y0;
	    smooth_scroll = 0;
	    snd = snd_SCROLL_LIMIT;
	    break;
        case 3:
	    o->y = -v->margin.y1;
	    smooth_scroll = 0;
	    snd = snd_SCROLL_LIMIT;
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
    {
	sound_event(snd);
	fe_scroll_changed(v, x, y);
    }
    else
    {
/* 	sound_event(snd_SCROLL_FAILED); */
    }

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

BOOL fe_view_scroll_possible(fe_view v, int dx, int dy)
{
    wimp_wstate state;
    wimp_openstr *o;
    wimp_get_wind_state(v->w, &state);
    o = &state.o;
    if (dx < 0)
	return o->x > -v->margin.x0;
    if (dx > 0)
	return o->x < v->doc_width + (o->box.x1 - o->box.x0) - v->margin.x1;
    if (dy < 0)
	return o->y > v->doc_height + (o->box.y1 - o->box.y0) - v->margin.y0;
    if (dy > 0)
	return o->y < -v->margin.y1;
    return FALSE;
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



int fe_view_scroll_x(fe_view v, int val, int flags)
{
    STBDBG(("fe_view_scroll_x: v%p w%x val %d flags %x\n", v, v->w, val, flags));

    if (v->w && v->scrolling != fe_scrolling_NO)
    {
        wimp_wstate state;
        wimp_get_wind_state(v->w, &state);
        if (fe_scroll_request(v, &state.o, val, 0))
	{
	    if (flags & fe_view_scroll_ENSURE)
		fe_ensure_highlight(v, val < 0 ? be_link_BACK : 0);
	    return 1;
	}
	else
	{
	    if (flags & fe_view_scroll_BEEP)
		sound_event(snd_SCROLL_FAILED);
	}
    }
    return 0;
}

int fe_view_scroll_y(fe_view v, int val, int flags)
{
    STBDBG(("fe_view_scroll_y: v%p w%x val %d flags %x\n", v, v->w, val, flags));

    if (v->w && v->scrolling != fe_scrolling_NO)
    {
        wimp_wstate state;
        wimp_get_wind_state(v->w, &state);

	if (use_toolbox)
	    tb_status_set_direction(val > 0);

	if (fe_scroll_request(v, &state.o, 0, val))
	{
	    if (flags & fe_view_scroll_ENSURE)
		fe_ensure_highlight(v, be_link_VERT | (val > 0 ? be_link_BACK : 0));
	    return 1;
	}
	else
	{
	    if (flags & fe_view_scroll_BEEP)
		sound_event(snd_SCROLL_FAILED);
	}
    }
    return 0;
}

/* ----------------------------------------------------------------------------*/

static char *frame_link_sprite_uhl[] =
{
    "flleft",
    "fltop",
    "flright",
    "flbottom"
};

static char *frame_link_sprite_hl[] =
{
    "flleft1",
    "fltop1",
    "flright1",
    "flbottom1"
};

static void draw_frame_links(wimp_redrawstr *r, fe_view v, const frame_link *fl)
{
    while (fl)
    {
/* 	STBDBG(("draw_frame_links: fl %p ", fl)); */
/* 	STBDBG(("side %d pos %d,%d\n", fl->side, fl->box.x0, fl->box.y0)); */

	render_plot_icon(
	    fl->flags & frame_link_flag_SELECTED ? frame_link_sprite_hl[fl->side] : frame_link_sprite_uhl[fl->side],
	    fl->box.x0, fl->box.y0);

	fl = fl->next;
    }
}

/* ----------------------------------------------------------------------------*/

static void draw_border(wimp_redrawstr *r, fe_view v)
{
    render_plinth_from_list(0,
			    config_colour_list[render_colour_list_WINDOW_BORDER],
			    render_plinth_NOFILL,
			    v->box.x0, v->box.y0, v->box.x1 - v->box.x0, v->box.y1 - v->box.y0,
			    v->displaying);
}

static os_error *draw_bevels(fe_view v, void *handle)
{
    wimp_redrawstr *r = handle;

    /* if a frameset document then render the bevels */
    if (v->w == 0)
    {
	wimp_redrawstr rr;

	STBDBG(("draw_bevels: v%p box %d,%d,%d,%d rr %d,%d,%d,%d scroll %d%d\n",
		v, v->box.x0, v->box.y0, v->box.x1, v->box.y1,
		r->box.x0, r->box.y0, r->box.x1, r->box.y1,
		r->scx, r->scy));

	rr.box = v->box;
	rr.scx = rr.scy = 0;
	rr.g = r->g;
	backend_render_rectangle(&rr, v->displaying, TRUE);
    }

    return NULL;
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

/* 	STBDBG(("frontend_view_redraw: whole v %p from %s/%s\n", v, caller(1), caller(2))); */
    }

    wimp_force_redraw(&r);

    return 0;
}

int frontend_view_update(fe_view v, wimp_box *bb, fe_rectangle_fn fn, void *h, int flags)
{
    int            more;
    wimp_redrawstr r;
    fe_view selected;

    STBDBGN(("frontend_view_update: v%p box %d,%d %d,%d flags %x\n", v, bb->x0, bb->y0, bb->x1, bb->y1, flags));

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

    r.w = v->w;
    r.box = *bb;

    if (gbf_active(GBF_ANTI_TWITTER))
    {
	/* expand redraw box to allow for anti-twittering*/
	/* the wimp will clip this to the window*/
	r.box.y0 -= REDRAW_OVERLAP;
	r.box.y1 += REDRAW_OVERLAP;
    }

    frontend_fatal_error(wimp_update_wind(&r, &more));

    selected = v->parent == NULL && v->children ? fe_selected_view() : NULL;

    while (more)
    {
	/* 3rd param must be 0 if we want the background to be redrawn by the backend redraw function
	 * If it is 1 then it assumes we have already cleared the background or don't care about overpainting.
	 */
	fn(&r, h, (flags & fe_update_WONT_PLOT_ALL) == 0 || (flags & fe_update_IMAGE_RENDERING) != 0);

	/* if we are transient and can't scroll then draw a border */
	if (v->open_transient && v->scrolling == fe_scrolling_NO)
	    draw_border(&r, v);

	/* if we are the top frameset and this is a call to
           render_rectangle then render the bevels for all the
           frameset documents */

	if (v->parent == NULL && v->children && fn == backend_render_rectangle)
	    iterate_frames(v, draw_bevels, &r);

	/* if we are top view above a selected view and are in web mode and the pointer is off */
        if (selected && pointer_mode == pointermode_OFF)
	    draw_frame_links(&r, v, selected->frame_links);

	/* antitwitter if not in the middle of image rendering */
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

    if (v->parent == NULL)
    {
        fvd->layout_width = fvd->user_width;
	if (v->x_scroll_bar)
	    fvd->layout_width += toolsprite_width-2;

        fvd->layout_height = fvd->min_height;
	if (v->y_scroll_bar)
	    fvd->layout_height -= toolsprite_height;
    }
    else
    {
        fvd->layout_width  =    v->box.x1 - v->box.x0;
        fvd->layout_height = - (v->box.y1 - v->box.y0);
    }

    LAYDBG(("vw%p: userw=%d docw=%d waw=%d minh=%d doch=%d wah=%d lw=%d lh=%d p=%p xs=%s ys=%s\n",
            v, fvd->user_width, fvd->doc_width, fvd->wa_width, fvd->min_height,
            fvd->doc_height, fvd->wa_height, fvd->layout_width,
            fvd->layout_height, v->parent, v->x_scroll_bar ? "y" : "n",
            v->y_scroll_bar ? "y" : "n" ));
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
    wimp_wstate ws;
    BOOL old_y_scroll_bar;

    STBDBG(("stbredraw: v %p set dimensions to %d x %d\n", v, width, height));

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    old_y_scroll_bar = v->y_scroll_bar;

    if (width)
	v->doc_width = width;
    if (height)
	v->doc_height = height;

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
                feutils_window_create(&box, &v->margin, NULL, fe_bg_colour(v), TRUE, &v->w);

		STBDBGN(("recreate win %x from view %p\n", v->w, v));
            }
        }
    }
    else
    {
    }

    if (v->w)
    {
	int need_reopen = 0;
	fe_view_dimensions fvd;

	frontend_fatal_error(wimp_get_wind_state(v->w, &ws));
	get_dimensions(v, &ws.o, &fvd);

	r.box.x0 = - v->margin.x0;
	r.box.y1 = - v->margin.y1;
#if 1
	r.box.x1 = fvd.wa_width - v->margin.x1;
	r.box.y0 = fvd.wa_height - v->margin.y0 - v->stretch_document;
#if 0
	if (v->parent == NULL)
	{
	    if (v->x_scroll_bar)
		r.box.x1 += toolsprite_width-2;
	    if (v->y_scroll_bar)
		r.box.y0 -= toolsprite_height;
	}
#endif
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
	    STBDBG(("stbredraw: set extent %d,%d %d,%d\n", r.box.x0, r.box.y0, r.box.x1, r.box.y1));

	    r.w = v->w;
	    wimp_set_extent(&r);

	    if (need_reopen)
	    {
                STBDBG(("stbredraw: set dimensions %d,%d %d,%d (%d,%d)\n",
                    ws.o.box.x0, ws.o.box.y0, ws.o.box.x1, ws.o.box.y1, ws.o.x, ws.o.y));

	        frontend_fatal_error(wimp_open_wind(&ws.o));
                fe_scroll_changed(v, 0, 0);
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

    /* get on screen area visible of window */
    if (v->w)
        frontend_fatal_error(wimp_get_wind_state(v->w, &ws));
    else
    {
        ws.o.box = v->box;
        ws.o.x = -v->margin.x0;
        ws.o.y = -v->margin.y1;
    }

    /* adjust visible area for the status bar */
    if (use_toolbox && tb_is_status_showing())
    {
	wimp_box sbox;
	tb_status_box(&sbox);

/* 	STBDBGN(("viewbounds: tb box %d,%d %d,%d\n", sbox.x0, sbox.y0, sbox.x1, sbox.y1)); */
/* 	STBDBGN(("viewbounds: ws box %d,%d %d,%d\n", ws.o.box.x0, ws.o.box.y0, ws.o.box.x1, ws.o.box.y1)); */

	if (config_display_control_top)
	{
/* 	    if (ws.o.box.y1 > sbox.y0) */
/* 		ws.o.box.y1 = sbox.y0; */
	}
	else
	{
	    if (ws.o.box.y0 < sbox.y1)
		ws.o.box.y0 = sbox.y1;
	}
/* 	STBDBGN(("viewbounds: ws box %d,%d %d,%d\n", ws.o.box.x0, ws.o.box.y0, ws.o.box.x1, ws.o.box.y1)); */
    }

    /* calculate box in work area coordinates */
    box->y1 = ws.o.y;
    box->y0 = ws.o.y - (ws.o.box.y1 - ws.o.box.y0);

    box->x0 = ws.o.x;
    box->x1 = ws.o.x + (ws.o.box.x1 - ws.o.box.x0);

/*     STBDBGN(("view bounds: %d,%d %d,%d (status %d)\n", box->x0, box->y0, box->x1, box->y1, fe_status_height_top(v))); */

    return 1;
}

int frontend_view_margins(fe_view v, wimp_box *box)
{
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

#if USE_MARGINS
    *box = v->backend_margin;

#if 0
    /* zero margin where the toolbar is touching if not a frame */
    if (v->parent == NULL && use_toolbox && tb_is_status_showing())
    {
	if (config_display_control_top)
	    box->y1 = 0;
	else
	    box->y0 = 0;
    }
#endif
#else
    *box = v->margin;

    if (fe_status_height_top(v))
        box->y1 = -STATUS_TOP_MARGIN;

    STBDBGN(("stbredraw: view margins %d,%d %d,%d (status %d)\n", box->x0, box->y0, box->x1, box->y1, fe_status_height_top(v)));
#endif
    return 1;
}

extern wimp_t on_screen_kbd;
extern wimp_box on_screen_kbd_pos;

int frontend_view_ensure_visable(fe_view v, int x, int top, int bottom)
{
    return frontend_view_ensure_visable_full(v, x, x, top, bottom, 0);
}

int frontend_view_ensure_visable_full(fe_view v, int left, int right, int top, int bottom, int dir_flags)
{
    wimp_wstate state;
    int w, h;
    int mh;
    int need_to_reopen = 0;
    int need_to_set_dims = 0;
    int bbh, sbh;

    STBDBGN(("ensure_visible: v %p x %d-%d y %d-%d\n", v, left, right, top, bottom));

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

    /* for NetChannel compaibility we allow jumping to a fragment (ie
       top=bottom) to work on no scroling pages */
    if (v->scrolling == fe_scrolling_NO && top != bottom)
	return 1;

    frontend_fatal_error(wimp_get_wind_state(v->w, &state));

    bbh = - v->margin.y1;
    sbh =   v->margin.y0;

#if 0
    if (on_screen_kbd)
    {
	if (config_display_control_top)
	{
	    bbh += on_screen_kbd_pos.y1 - on_screen_kbd_pos.y0;
	}
	else
	{
	    sbh += on_screen_kbd_pos.y1 - on_screen_kbd_pos.y0;
	}
    }
#endif

    h = (state.o.box.y1 - bbh) - (state.o.box.y0 + sbh);    /* height of visible area (within margins)*/
    mh = h > -(v->doc_height) ? h : -(v->doc_height);       /* height of document, or visible area (+ve)*/
    w = state.o.box.x1 - state.o.box.x0;

    STBDBGN(("ensure_visible: compare against x %d-%d y %d-%d\n",
	     state.o.x + v->margin.x0, state.o.x + w + v->margin.x1, state.o.y - bbh, state.o.y + sbh - (state.o.box.y1 - state.o.box.y0)));

    if (v->stretch_document)
    {
        v->stretch_document = 0;
        need_to_set_dims = 1;
    }

#if 0
    if (bottom - h < -mh /*- sbh*/)
    {
        v->stretch_document = (-mh/* - sbh*/) - (bottom - h);
        need_to_set_dims = 1;

	STBDBGN(("ensure_visible: stretch\n"));
    }
#else
    if (top == bottom && top - h < -mh /*- sbh*/)
    {
        v->stretch_document = (-mh/* - sbh*/) - (top - h);
        need_to_set_dims = 1;

	STBDBGN(("ensure_visible: stretch\n"));
    }
#endif

    if (top == bottom ||	/* Special case: force to the top is top and bottom equal */
	top > (state.o.y - bbh))
    {
        state.o.y = top;
	state.o.y += bbh;

	need_to_reopen = 1;

	STBDBGN(("ensure_visible: force to top top_cmp %d bot_cmp %d\n", (state.o.y - bbh), (state.o.y + sbh - (state.o.box.y1 - state.o.box.y0))));
    }

    if (bottom < (state.o.y + sbh - (state.o.box.y1 - state.o.box.y0)))
    {
        state.o.y = bottom - sbh + (state.o.box.y1 - state.o.box.y0);

	need_to_reopen = 1;

	STBDBGN(("ensure_visible: force on bottom\n"));
    }

    if ((right != -1) && (right > state.o.x + w + v->margin.x1))
    {
	/* It is off the right, put it at the right */
	state.o.x = right - w - v->margin.x1;
	need_to_reopen = 1;

	STBDBGN(("ensure_visible: off right\n"));
    }

    /* let the left override the right */
    if ((left != -1) && (left < state.o.x + v->margin.x0))
    {
	/* It is off the left, put it at the left edge */
	state.o.x = left - v->margin.x0;
	need_to_reopen = 1;

	STBDBGN(("ensure_visible: off left\n"));
    }

    if (need_to_set_dims)
    {
        frontend_view_set_dimensions(v, 0, 0);
    }

    if (need_to_reopen)
    {
	STBDBGN(("stbredraw: ensure %d-%d visible %d,%d %d,%d (%d,%d) height %d\n",
		 top, bottom,
		 state.o.box.x0, state.o.box.y0, state.o.box.x1, state.o.box.y1, state.o.x, state.o.y, v->doc_height));

	frontend_fatal_error(wimp_open_wind(&state.o));

        fe_scroll_changed(v, 0, 0);
    }

    return 1;
}

/* ----------------------------------------------------------------------------*/

int frontend_view_caret(fe_view v, int x, int y, int hh, int on_screen)
{
    wimp_caretstr cs;
    int r;
    int height = hh < 0 ? hh : hh & 0x0000ffff; /* actual height without flags */

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 1;

    if (!v->w)
        return 1;

/*     STBDBGN(("viewcaret: v %p w %x @ %d,%d h %d on %d %s %s\n", v, v->w, x, y, height, on_screen, caller(1), caller(2))); */

    if (on_screen && height > 0)
    {
	frontend_view_ensure_visable(v, x, y + height, y);
    }

    cs.w = v->w;
    cs.i = -1;
    if (height < 0)
	cs.height = (-height) | (1 << 25);	/* Invisible caret */
    else
	cs.height = hh;
    cs.index = 0;
    cs.x = x;
    cs.y = y;

    r = frontend_complain(wimp_set_caret_pos(&cs)) == NULL;

/*     if (height >= 0 && v->current_link) */
/*         v->current_link = backend_read_highlight(v->displaying, NULL); */

/*     STBDBGN(("viewcaret: end %p \n", v->current_link)); */

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
