/* > stbmenu.c

 *

 */

#include <stdio.h>
#include <string.h>

#include "akbd.h"
#include "bbc.h"
#include "coords.h"
#include "colourtran.h"
#include "font.h"

#include "config.h"
#include "memwatch.h"
#include "rcolours.h"
#include "webfonts.h"

#include "interface.h"
#include "stbview.h"
#include "stbmenu.h"
#include "stbutils.h"
#include "stbfe.h"
#include "fevents.h"

#define X_BORDER 12
#define Y_BORDER 12

#ifndef OPEN_AS_MENU
#define OPEN_AS_MENU 0
#endif

#define MENU_FONT   WEBFONT_BASE

/* ------------------------------------------------------------------------------------------- */

static fe_menu current_menu = 0;

/* ------------------------------------------------------------------------------------------- */

/* ensure that the given item is visible */

static void fe_menu_ensure_item(fe_menu mh, int item)
{
    int line_space, move;
    int top, bottom;
    wimp_wstate ws;
    wimp_box box;

    line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;

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

    ox = r.box.x0 - r.scx;
    oy = r.box.y1 - r.scy;

    width = r.box.x1 - r.box.x0 - 2*X_BORDER;

    line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;

    more = TRUE;
    while (more)
    {
	top = r.g.y1 - oy;
	bot = r.g.y0 - oy;

	lfc = -1;

	if (font_setfont(webfonts[MENU_FONT].handle) != NULL)
	    continue;

	for(h=0, i=0; i < mh->n && (h-line_space) >= top; h -= line_space, i++)
	    ;

	for( ; i < mh->n && h >= bot; h -= line_space, i++)
	{
	    int junk;
	    int nfc = (mh->items[i].flags & fe_menu_flag_CHECKED) ? render_colour_WRITE : render_colour_INPUT_F;
	    int opcol = (nfc == render_colour_WRITE) ? render_colour_INPUT_F : render_colour_WRITE;

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

	    colourtran_setGCOL(config_colours[render_colour_WRITE], 0, 0, &junk);
	    bbc_rectanglefill(ox - X_BORDER, h + oy - line_space, X_BORDER, line_space-1);
	    bbc_rectanglefill(ox - width, h + oy - line_space, X_BORDER, line_space-1);

	    colourtran_setGCOL(config_colours[opcol], 0, 0, &junk);
	    bbc_rectanglefill(ox, h + oy - line_space, width, line_space-1);

	    font_paint(mh->items[i].name, font_OSCOORDS, ox, h + oy - webfonts[MENU_FONT].max_up);

            if (i == mh->highlight && pointer_mode == pointermode_OFF)
            {
                colourtran_setGCOL(config_colours[render_colour_HIGHLIGHT], 0, 0, &junk);
                bbc_rectangle(ox-2, h + oy - line_space, width+4-1, line_space-1);
                bbc_rectangle(ox, h + oy - line_space+2, width-1, line_space-4-1);
            }
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

static void fe_menu_window_click(fe_menu mh, wimp_mousestr *m)
{
    wimp_wstate ws;
    int y;
    int line_space;
    int right = TRUE;		/* Keep the window open if it is a middle button */

    line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;

    if (m->bbits & (wimp_BRIGHT | wimp_BLEFT))
    {
	right = m->bbits & wimp_BRIGHT;

	if (wimp_get_wind_state(mh->wh, &ws))
	    return;

	y = ws.o.box.y1 - ws.o.y;

	y = m->y - y;

	y = -y;
	y /= line_space;

	/* @@@@ Do something with the click */

	(mh->cb)(mh, mh->h, y, right);
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
	int line_space;

	line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;

        memset(&win, 0, sizeof(win));

        win.behind = -1;
        win.flags = wimp_WNEW;
        win.colours[wimp_WCWKAREABACK] = 0;
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
    int new_highlight = mh->highlight + dir;
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

/* ------------------------------------------------------------------------------------------- */

void frontend_menu_update_item(fe_menu mh, int i)
{
    wimp_redrawstr r;
    int line_space;
/*     int more; */

    STBDBG(("stbmenu: update mh %p item %d\n", mh, i));

    line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;

    r.w = mh->wh;
    r.box.x0 = - X_BORDER;
    r.box.x1 =   X_BORDER + mh->width;
    r.box.y1 = - line_space*i       + 4;
    r.box.y0 = - line_space*(i + 1) - 4;
#if 1
    frontend_fatal_error(wimp_force_redraw(&r));
#else
    frontend_fatal_error(wimp_update_wind(&r, &more));

    if (more)
	fe_menu_redo_window(&r, mh, 1);
#endif
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

    cvt = fe_get_cvt(mh->parent);
    p.x = x;
    p.y = y;
    coords_point_toscreen(&p, &cvt);

    mh->highlight = first_checked(mh);

    if (mh->wh)
    {
        wimp_wstate state;
        int overhang, line_space;

        current_menu = mh;
        line_space = webfonts[MENU_FONT].max_up + webfonts[MENU_FONT].max_down;
#if 1
        state.o.w = mh->wh;
        state.o.box.x0 = p.x;
        state.o.box.y1 = p.y;
        state.o.box.x1 = p.x + mh->width + 2*X_BORDER;
        state.o.box.y0 = p.y - mh->size*line_space - 2*Y_BORDER;
        state.o.x = 0;
        state.o.y = 0;
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

	/* open menu window */
	frontend_fatal_error(wimp_open_wind(&state.o));
#else
	frontend_fatal_error(wimp_create_menu((wimp_menustr *) mh->wh, p.x, p.y));
#endif
#if 0
        frontend_fatal_error(wimp_get_wind_state(mh->wh, &state));
        overhang = state.o.box.x1 - text_safe_box.x1;
        if (overhang > 0)
        {
            state.o.box.x0 -= overhang;
            state.o.box.x1 -= overhang;
        }

        if (state.o.box.y1 > text_safe_box.y1)
            state.o.box.y1 = text_safe_box.y1;

        if (state.o.box.y0 < text_safe_box.y0)
            state.o.box.y0 = text_safe_box.y0;

        state.o.y = -mh->highlight*line_space;
        frontend_fatal_error(wimp_open_wind(&state.o));
#endif
    }
}

fe_menu frontend_menu_create(fe_view v, be_menu_callback cb, void *handle, int n, fe_menu_item *items, int size, int width)
{
    fe_menu menu;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    menu = mm_calloc(sizeof(*menu), 1);

    menu->cb = cb;
    menu->h = handle;
    menu->n = n;
    menu->items = items;
    menu->size = size;
    menu->width = width;
    menu->parent = v;

    fe_menu_window(menu);

    STBDBG(("stbmenu: create mh %p w %x\n", menu, menu->wh));

    return menu;
}

/* ------------------------------------------------------------------------------------------- */

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
