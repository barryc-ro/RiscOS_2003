/* > stbmenu.c

 *

 */

#include <stdio.h>
#include <string.h>

#include "akbd.h"
#include "alarm.h"
#include "bbc.h"
#include "coords.h"
#include "colourtran.h"
#include "font.h"

#include "interface.h"
#include "config.h"
#include "memwatch.h"
#include "rcolours.h"
#include "render.h"
#include "webfonts.h"
#include "util.h"

#include "interface.h"
#include "stbview.h"
#include "stbmenu.h"
#include "stbutils.h"
#include "stbfe.h"
#include "fevents.h"
#include "frameutils.h"

#define X_BORDER 12
#define Y_BORDER 12

#define X_BORDER_HL 12
#define Y_BORDER_HL 12

#ifndef OPEN_AS_MENU
#define OPEN_AS_MENU 0
#endif

#define MENU_FONT   WEBFONT_BASE

#define col_HIGHLIGHT	render_colour_HIGHLIGHT	/* the highlight box colour */
#define col_FG		render_colour_MENU_F
#define col_FG_HL	render_colour_MENU_FS
#define col_BG		render_colour_MENU_B
#define col_BG_HL	render_colour_MENU_BS

/* ------------------------------------------------------------------------------------------- */

static fe_menu current_menu = 0;

/* ------------------------------------------------------------------------------------------- */

static int getwebfont(fe_menu mh)
{
    int whichfont;

    whichfont = backend_getwebfont(mh->parent->displaying,
				   mh->flags & fe_menu_flag_WIDE, mh->items[0].language, MENU_FONT, MENU_FONT);

#if 0
    if (mh->flags & fe_menu_flag_WIDE)
	whichfont = WEBFONT_SIZE(3) | WEBFONT_JAPANESE;
    else
	whichfont = MENU_FONT;
#endif

#if UNICODE
    /* if we are claiming a wide font then always set it to Unicode encoding */
    if (mh->flags & fe_menu_flag_WIDE)
	webfont_set_wide_format(whichfont);
#endif

    return whichfont;
}

/* ensure that the given item is visible */

static int get_line_space(webfont *wf)
{
    return (wf->max_up + wf->max_down + 1) &~ 1;
}

static void fe_menu_ensure_item(fe_menu mh, int item)
{
    int line_space, move;
    int top, bottom;
    wimp_wstate ws;
    wimp_box box;

    line_space = mh->line_space;

    top = - line_space*item;
    bottom = top - line_space;

    wimp_get_wind_state(mh->wh, &ws);

    box = ws.o.box;
    coords_box_toworkarea(&box, (coords_cvtstr *)&ws.o.box);

    move = 0;
    if (top > box.y1)
        move = box.y1 - top;
    if (bottom < box.y0)
        move = box.y0 - bottom;

    if (move)
    {
        ws.o.y -= move;
        wimp_open_wind(&ws.o);
    }
}

static int first_checked(fe_menu mh)
{
    int i;
    for (i = 0; i < mh->n; i++)
        if (mh->items[i].flags & fe_menu_flag_CHECKED)
            return i;
    return 0;
}

static void fe_menu_redo_window(wimp_redrawstr *rr, fe_menu mh, int update)
{
    int more;
    wimp_redrawstr r = *rr;
    int ox, oy;
    int top, bot;
    int i;
    int h;
    int line_space;
    int lfc;
    int width;
    webfont *wf;
    int whichfont;

    ox = r.box.x0 - r.scx;
    oy = r.box.y1 - r.scy;

    width = r.box.x1 - r.box.x0 - 2*X_BORDER;

    whichfont = getwebfont(mh);
    wf = &webfonts[whichfont];
    line_space = mh->line_space;

    more = TRUE;
    while (more)
    {
	int junk;
	
	top = r.g.y1 - oy;
	bot = r.g.y0 - oy;

	lfc = -1;

	/* draw the top and bottom edges */
	if (top > 0)
	{
	    colourtran_setGCOL(config_colours[col_BG], 0, 0, &junk);
	    bbc_rectanglefill(ox - X_BORDER, oy, width + 2*X_BORDER-1, Y_BORDER-1);
	}
	if (bot < -mh->n*line_space)
	{
	    colourtran_setGCOL(config_colours[col_BG], 0, 0, &junk);
	    bbc_rectanglefill(ox - X_BORDER, oy - mh->n*line_space - Y_BORDER, width + 2*X_BORDER-1, Y_BORDER-1);
	}
	
	for (h=0, i=0; i < mh->n && (h-line_space) >= top; h -= line_space, i++)
	    ;

	for ( ; i < mh->n && h >= bot; h -= line_space, i++)
	{
	    int nfc, opcol;

	    if (mh->items[i].flags & fe_menu_flag_CHECKED)
	    {
		nfc = col_FG_HL;
		opcol = col_BG_HL;
	    }
	    else
	    {
		nfc = col_FG;
		opcol = col_BG;
	    }

	    /* set the font colours */
	    if (lfc != nfc)
	    {
		int maxcols = 14;
		font fh;
		wimp_paletteword ff, bb;

		lfc = nfc;

		fh = 0;
		ff = config_colours[lfc];
		bb = config_colours[opcol];

		colourtran_setfontcolours(&fh, &bb, &ff, &maxcols);
	    }

	    /* draw the left and right edges */
	    colourtran_setGCOL(config_colours[col_BG], 0, 0, &junk);
	    bbc_rectanglefill(ox - X_BORDER, h + oy - line_space, X_BORDER, line_space-1);
	    bbc_rectanglefill(ox + width, h + oy - line_space, X_BORDER, line_space-1);

	    /* draw the background for the text */
	    colourtran_setGCOL(config_colours[opcol], 0, 0, &junk);
	    bbc_rectanglefill(ox, h + oy - line_space, width, line_space-1);

	    /* draw the text itself */
	    render_text(NULL, whichfont, mh->items[i].name, ox, h + oy - wf->max_up);
	}

	/* draw the selection box - inefficient */
	if (mh->highlight >= 0 && mh->highlight < mh->n)
	{
	    int hh = - mh->highlight * line_space;
	    render_plinth_from_list(0, config_colour_list[render_colour_list_TEXT_HIGHLIGHT], render_plinth_NOFILL,
				    ox - X_BORDER_HL, hh + oy - line_space - Y_BORDER_HL,
				    width + X_BORDER_HL*2, line_space + Y_BORDER_HL*2, NULL);
	}

	fe_anti_twitter(&r.g);
	wimp_get_rectangle(&r, &more);
    }

    /* We do not report any errors - see the manual for the reason why */
}

static void fe_menu_redraw_window(wimp_w handle, fe_menu mh)
{
    wimp_redrawstr r;
    int more;

    r.w = handle;
    frontend_fatal_error(wimp_redraw_wind(&r, &more));

    if (more)
	fe_menu_redo_window(&r, mh, 0);
}

static int fe_menu_coords_to_index(fe_menu mh, int my)
{
    wimp_wstate ws;
    int y;

    if (wimp_get_wind_state(mh->wh, &ws))
	return -1;

    y = ws.o.box.y1 - ws.o.y;

    y = my - y;

    y = -y;
    y /= mh->line_space;

    if (y < 0)
        y = 0;
    if (y >= mh->n)
        y = mh->n - 1;

    return y;
}

static void fe_menu_window_click(fe_menu mh, wimp_mousestr *m)
{
    int right = TRUE;		/* Keep the window open if it is a middle button */

    if (m->bbits & (wimp_BRIGHT | wimp_BLEFT))
    {
	right = m->bbits & wimp_BRIGHT;

	(mh->cb)(mh, mh->h, fe_menu_coords_to_index(mh, m->y), right);
    }

    if (!right)
        stbmenu_close();
}

static os_error *fe_menu_window(fe_menu mh)
{
    os_error *ep;
    wimp_wind win;    /* Pointer to window definition */

    /* Find template for the window */
    if (mh->wh == 0)
    {
	int line_space = mh->line_space;

        memset(&win, 0, sizeof(win));

        win.behind = -1;
        win.flags = wimp_WNEW;
        win.colours[wimp_WCWKAREABACK] = 0xff; /* 3; */
        win.colours[wimp_WCWKAREAFORE] = 7;
        win.colours[wimp_WCTITLEFORE] = 7;

	win.ex.x0 = -X_BORDER;
	win.ex.y1 = +Y_BORDER;
	win.ex.x1 = mh->width + X_BORDER;
	win.ex.y0 = -(mh->n * line_space) - Y_BORDER;

	win.box = win.ex;

	if (mh->size > 1)
	{
	    win.box.y0 = - (mh->size * line_space) - Y_BORDER;
	    win.flags = (wimp_wflags) (win.flags | wimp_WVSCR);
	    mh->vscroll = TRUE;

	}
        else
        {
            int max = (text_safe_box.y1 - text_safe_box.y0 - Y_BORDER*2)/line_space;
            if (max == 0)
                max = 1;

            if (mh->n > max)
            {
	        win.flags = (wimp_wflags) (win.flags | wimp_WVSCR);
                mh->size = max;
		mh->vscroll = TRUE;
            }
            else
                mh->size = mh->n;
        }

	win.scx = win.scy = 0;

        win.workflags = (wimp_iconflags)(wimp_IBTYPE*wimp_BCLICKDEBOUNCE);

	if (!config_display_frames_scrollbars)
	    win.flags = (wimp_wflags) ((int)win.flags &~ wimp_WVSCR);

	/* Create the window, dealing with errors */
	ep = frontend_complain(wimp_create_wind(&win, &(mh->wh)));
	if (ep)
	    return ep;

/*	win_register_event_handler(mh->wh, fe_menu_event_handler, mh); */
    }

    return NULL;
}

static void fe_menu_move_highlight(fe_menu mh, int dir)
{
    int new_highlight = (mh->highlight < 0 ? first_checked(mh) : mh->highlight) + dir;

    if (new_highlight < 0)
        new_highlight = 0;
    if (new_highlight >= mh->n)
        new_highlight = mh->n - 1;

    if (new_highlight != mh->highlight)
    {
        int old_highlight = mh->highlight;
        mh->highlight = new_highlight;

        fe_pointer_mode_update(pointermode_OFF);

        frontend_menu_update_item(mh, old_highlight);
        frontend_menu_update_item(mh, new_highlight);

        fe_menu_ensure_item(mh, new_highlight);
    }
}

static int get_widest_entry(int whichfont, fe_menu_item *items, int n)
{
    int i, widest;
    fe_menu_item *item;

    widest = 0;

    for (i = 0, item = items; i < n; i++, item++)
    {
	int w = webfont_font_width(whichfont, strsafe(item->name));
	if (widest < w)
	    widest = w;
    }

    return widest;
}

/* ------------------------------------------------------------------------------------------- */

void frontend_menu_update_item(fe_menu mh, int i)
{
    wimp_redrawstr r;

    STBDBG(("stbmenu: update mh %p item %d\n", mh, i));

    if (i < 0)
	return;
    
    r.w = mh->wh;
    r.box.x0 = - X_BORDER;
    r.box.x1 =   X_BORDER + mh->width;
    r.box.y1 = - mh->line_space*i       + 4 + Y_BORDER_HL; /* extra 4 is for anti-twitter protection I think */
    r.box.y0 = - mh->line_space*(i + 1) - 4 - Y_BORDER_HL;

    frontend_fatal_error(wimp_force_redraw(&r));
}

void frontend_menu_dispose(fe_menu mh)
{
    STBDBG(("stbmenu: dispose mh %p\n", mh));

    if (mh->wh)
    {
#if OPEN_AS_MENU
	/* Very important to do this */
	frontend_fatal_error(wimp_create_menu((wimp_menustr *) -1, 0, 0));
#endif
	wimp_close_wind(mh->wh);
	wimp_delete_wind(mh->wh);

	mh->wh = 0;
    }

    if (current_menu == mh)
	current_menu = NULL;

    webfont_lose_font(getwebfont(mh));

    mm_free(mh);
}

void frontend_menu_raise(fe_menu mh, int x, int y)
{
    coords_cvtstr cvt;
    coords_pointstr p;
    BOOL reopen;

    STBDBG(("stbmenu: raise mh %p\n", mh));

#if !OPEN_AS_MENU
    reopen = mh == current_menu;
    stbmenu_close();
    if (reopen)
        return;
#endif

    cvt = frameutils_get_cvt(mh->parent);
    p.x = x;
    p.y = y;
    coords_point_toscreen(&p, &cvt);

    if (pointer_mode == pointermode_OFF)
	mh->highlight = first_checked(mh);

    if (mh->wh)
    {
        wimp_wstate state;
        int overhang;

        current_menu = mh;

        state.o.w = mh->wh;
        state.o.box.x0 = p.x;
        state.o.box.y1 = p.y;
        state.o.box.x1 = p.x + mh->width + 2*X_BORDER;
        state.o.box.y0 = p.y - mh->size*mh->line_space - 2*Y_BORDER;
        state.o.x = 0;
        state.o.behind = (wimp_w)-1;

	/* See if we overhang the rh safe area */
        overhang = state.o.box.x1 - text_safe_box.x1;
	if (mh->vscroll)
	    overhang += toolsprite_width;

	/* mnove left */
        if (overhang > 0)
        {
            state.o.box.x0 -= overhang;
            state.o.box.x1 -= overhang;
        }

	/* then clip the left edge */
        if (state.o.box.x0 < text_safe_box.x0)
            state.o.box.x0 = text_safe_box.x0;

	
	/* see if we overhang the bottom of the safe area */
        overhang = - state.o.box.y0 + text_safe_box.y0;

	/* move up */
        if (overhang > 0)
        {
            state.o.box.y0 += overhang;
            state.o.box.y1 += overhang;
        }

	/* then clip the top edge */
        if (state.o.box.y1 > text_safe_box.y1)
            state.o.box.y1 = text_safe_box.y1;


	/* put the first checked in the middle of the menu */
	state.o.y = - first_checked(mh) * mh->line_space + (state.o.box.y1 - state.o.box.y0)/2;

	/* open menu window */
	frontend_fatal_error(wimp_open_wind(&state.o));

	sound_event(snd_MENU_SHOW);
    }
}

fe_menu frontend_menu_create(fe_view v, be_menu_callback cb, void *handle, int n, fe_menu_item *items, int size, int width)
{
    fe_menu menu;
    int wf_index;
    webfont *wf;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    menu = mm_calloc(sizeof(*menu), 1);

    menu->cb = cb;
    menu->h = handle;
    menu->n = n;
    menu->items = items;
    menu->size = 0; /* size; change to use as much as possible */
    menu->flags = items[0].flags & fe_menu_flag_WIDE;

    wf_index = getwebfont(menu);
    webfont_find_font(wf_index);

    wf = &webfonts[wf_index];

    menu->width = get_widest_entry(wf_index, items, n);
    menu->parent = v;
    menu->line_space = get_line_space(wf);

    fe_menu_window(menu);

    STBDBG(("stbmenu: create mh %p w %x flags 0x%x\n", menu, menu->wh, menu->flags));

    return menu;
}

/* ------------------------------------------------------------------------------------------- */

#define AUTOSCROLL_EDGE_THRESHOLD	24	/* closeness to edge to start auto-scrolling in OS units */
#define AUTOSCROLL_DELAY		100	/* delay before auto-scrolling takes affect */
#define AUTOSCROLL_HOVER_AREA		8	/* space to stay within during DELAY period */

static void fe_menu_window_autoscroll(fe_menu mh, wimp_mousestr *mp)
{
    static int autoscroll_yedge = 0, autoscroll_time = 0;
    wimp_wstate state;
    int yedge;

    if (mh == NULL || mh->wh != mp->w)
	return;

    wimp_get_wind_state(mh->wh, &state);

    /* check edge returns how far over the threshold we are */
    yedge = check_edge_proximity(mp->y, state.o.box.y0, state.o.box.y1, AUTOSCROLL_EDGE_THRESHOLD);

    STBDBG(("fe_menu_window_autoscroll: ypos %d window %d-%d yedge %d\n", mp->y, state.o.box.y0, state.o.box.y1, yedge));
    
    /* we start scrolling if we stay in one place for AUTOSCROLL_DELAY
     * we continue scrolling as long as we are over the threshold
     */
    if (autoscroll_yedge)
    {
	autoscroll_yedge = yedge;

	state.o.y += yedge*4;
	wimp_open_wind(&state.o);
    }
    else if (yedge)
    {
	int now = alarm_timenow();
	if (abs(yedge - autoscroll_yedge) < AUTOSCROLL_HOVER_AREA)
	{
	    if (now - autoscroll_time >= AUTOSCROLL_DELAY)
	    {
		state.o.y += yedge*4;
		wimp_open_wind(&state.o);
	    }
	}
	else
	{
	    autoscroll_yedge = yedge;
	    autoscroll_time = now;
	}
    }
    else
    {
	autoscroll_yedge = 0;
    }
}

static void fe_menu_highlight_under_pointer(fe_menu mh, wimp_mousestr *mp)
{
    int new_highlight;

    if (mh == NULL)
	return;

    new_highlight = mh->wh == mp->w ? fe_menu_coords_to_index(mh, mp->y) : -1;

    if (new_highlight != mh->highlight)
    {
        int old_highlight = mh->highlight;
        mh->highlight = new_highlight;

        frontend_menu_update_item(mh, old_highlight);
        frontend_menu_update_item(mh, new_highlight);
    }
}

/* ------------------------------------------------------------------------------------------- */

BOOL stbmenu_check_pointer(wimp_mousestr *mp)
{
    if (pointer_mode == pointermode_ON)
    {
	fe_menu_window_autoscroll(current_menu, mp);

	fe_menu_highlight_under_pointer(current_menu, mp);
    }
    return FALSE;
}

BOOL stbmenu_check_mouse(wimp_mousestr *mp)
{
    STBDBG(("stbmenu: mouse mh %p w %x\n", current_menu, mp->w));

    if (!current_menu)
        return FALSE;

    /* check mouse button state as we get spurious drag events whwn we open the menu */
    /* which we don't want to close it immediately */
    if (current_menu->wh != mp->w && (mp->bbits == wimp_BLEFT || mp->bbits == wimp_BRIGHT))
    {
        stbmenu_close();
        return FALSE;
    }

    fe_menu_window_click(current_menu, mp);
    
    return TRUE;
}

void stbmenu_event_handler(int event)
{
    fe_menu mh = current_menu;

    STBDBG(("stbmenu: menu %p event &%x\n", mh, event));

    if (mh) switch (event)
    {
        case fevent_MENU_TOP:
            fe_menu_move_highlight(mh, -1000);
            break;

        case fevent_MENU_PAGE_UP:
            fe_menu_move_highlight(mh, -(mh->size-1));
            break;

        case fevent_MENU_UP:
            fe_menu_move_highlight(mh, -1);
            break;

        case fevent_MENU_BOTTOM:
            fe_menu_move_highlight(mh, +1000);
            break;

        case fevent_MENU_PAGE_DOWN:
            fe_menu_move_highlight(mh, mh->size-1);
            break;

        case fevent_MENU_DOWN:
            fe_menu_move_highlight(mh, +1);
            break;

        case fevent_MENU_SELECT:
	    if (mh->cb)
	        (mh->cb)(mh, mh->h, mh->highlight, FALSE);
            stbmenu_close();
            break;

        case fevent_MENU_CANCEL:
            stbmenu_close();
            break;

        case fevent_MENU_TOGGLE:
	    if (mh->cb)
	        (mh->cb)(mh, mh->h, mh->highlight, TRUE);
            break;
    }
}

BOOL stbmenu_is_open(void)
{
    return current_menu != NULL;
}

BOOL stbmenu_check_redraw(wimp_w w)
{
    if (!current_menu || current_menu->wh != w)
        return FALSE;

    fe_menu_redraw_window(w, current_menu);

    return TRUE;
}

void stbmenu_close(void)
{
    if (current_menu)
    {
	sound_event(snd_MENU_HIDE);
#if OPEN_AS_MENU
	frontend_fatal_error(wimp_create_menu((wimp_menustr *) -1, 0, 0));
#else
        frontend_fatal_error(wimp_close_wind(current_menu->wh));
#endif
        current_menu = NULL;
    }
}

/* ------------------------------------------------------------------------------------------- */

/* eof stbmenu.c */
