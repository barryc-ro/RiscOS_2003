/* -*-c-*- */

/* feutils.c */

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "akbd.h"
#include "colourtran.h"
#include "bbc.h"
#include "msgs.h"
#include "swis.h"
#include "resspr.h"
#include "visdelay.h"

#include "version.h"

#include "config.h"
#include "filetypes.h"
#include "memwatch.h"
#include "url.h"
#include "unwind.h"
#include "util.h"
#include "verstring.h"
#include "gbf.h"

#include "interface.h"
#include "stbutils.h"
#include "stbfe.h"

static BOOL done_init = FALSE;

int frontend_dx, frontend_dy;
wimp_box screen_box = { 0, 0, 768*2, 576*2 };
wimp_box text_safe_box = { 80*2, 58*2, 690*2, 518*2 };
wimp_box margin_box;
wimp_box error_box;

int toolsprite_width, toolsprite_height;

/* ----------------------------------------------------------------------------------------------------- */

#define ModeFiles_MonitorType	0x4D480
#define ModeFiles_SafeArea	0x4D481

/*
 * This will look at the return from the monitorType SWI first. If that doesn't exist
 * it will look for the TV$Type system variable.
 */

BOOL is_a_tv(void)
{
    static int is_a_tv_var = -1;

    if (is_a_tv_var == -1)
    {
	os_regset r;
	os_error *e;

	r.r[0] = 0;
	e = os_swix(ModeFiles_MonitorType, &r);
	if (e)
	{
	    char *s = getenv("TV$Type");
	    is_a_tv_var = s && (strcasecomp(s, "PAL") == 0 || strcasecomp(s, "NTSC") == 0);
	}
	else
	{
	    is_a_tv_var = r.r[0] == 0 || r.r[0] == 8;
	}
    }

    return is_a_tv_var;
}

BOOL tv_safe_area(wimp_box *box)
{
    os_regset r;
    os_error *e;

    r.r[0] = 0;
    e = os_swix(ModeFiles_SafeArea, &r);

    if (!e) *box = *(wimp_box *)&r.r[0];

    return e == NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

static void get_tool_sprite_sizes(int *tool_width, int *tool_height)
{
    os_regset r;
    os_error *e;
    sprite_area *area;
    sprite_id id;
    sprite_info info;
    char name[12];
    char *suffix;

    MemCheck_checking checking = MemCheck_SetChecking(0, 0);

    /* read tool sprite area*/
    r.r[0] = 9;
    frontend_fatal_error(os_swix(Wimp_ReadSysInfo, &r));
    area = (sprite_area *)r.r[0];

    /* read iconsprites suffix*/
    r.r[0] = 2;
    frontend_fatal_error(os_swix(Wimp_ReadSysInfo, &r));
    suffix = (char *)r.r[0];

    /* discard default suffix*/
    if (suffix == NULL || strcmp(suffix, "24") == 0)
        suffix = "";

    id.tag = sprite_id_name;
    id.s.name = name;

    /* read size of h toolbar*/
    strcpy(name, "hbarmid");
    strcat(name, suffix);
    e = sprite_readsize(area, &id, &info);
    if (e)
    {
        strcpy(name, "hbarmid");
        e = sprite_readsize(area, &id, &info);

    }
    *tool_height = e ? 44 : info.height << bbc_modevar(info.mode, bbc_YEigFactor);

    /* read size of v toolbar*/
    strcpy(name, "vbarmid");
    strcat(name, suffix);
    e = sprite_readsize(area, &id, &info);
    if (e)
    {
        strcpy(name, "vbarmid");
        e = sprite_readsize(area, &id, &info);
    }
    *tool_width = e ? 44 : info.width << bbc_modevar(info.mode, bbc_XEigFactor);

    MemCheck_RestoreChecking(checking);
}

/* ----------------------------------------------------------------------------------------------------- */

/*
 * Draw the initial status box using the sprites from our resource file
 * Further updates will be just that.
 */


os_error *fe_get_sprite(char *name, sprite_id *id_out)
{
    sprite_id id;
    id.tag = sprite_id_name;
    id.s.name = name;
    id_out->tag = sprite_id_addr;
    return sprite_select_rp(resspr_area(), &id, &id_out->s.addr);
}

os_error *fe_get_sprite_scale(sprite_id *id, sprite_factors *factors, int *width, int *height)
{
    os_error *e;
    sprite_info info;
    e = sprite_readsize(resspr_area(), id, &info);
    if (!e)
    {
        int xeig = bbc_modevar(info.mode, bbc_XEigFactor);
        int yeig = bbc_modevar(info.mode, bbc_YEigFactor);

        if (width)
            *width = info.width << xeig;
        if (height)
            *height = info.height << yeig;

        if (factors)
        {
            factors->xmag = 1 << xeig;
            factors->ymag = 1 << yeig;
            factors->xdiv = frontend_dx;
            factors->ydiv = frontend_dy;
        }
    }
    return e;
}

os_error *fe_get_sprite_size(char *name, int *width, int *height)
{
    sprite_id id;
    os_error *e;
    e = fe_get_sprite(name, &id);
    if (!e) e = fe_get_sprite_scale(&id, NULL, width, height);
    return e;
}

extern int spriteextend_version;

os_error *fe_get_sprite_pixtrans(sprite_id *id, sprite_pixtrans **pixtrans_out, BOOL *wide)
{
    os_regset r;
    os_error *e;
    void *table = NULL;

    r.r[0] = (int)resspr_area();
    r.r[1] = (int)id->s.addr;
    r.r[2] = -1;
    r.r[3] = -1;
    r.r[4] = 0;
    r.r[5] = 0x1;
    if (spriteextend_version > 99)
        r.r[5] |= 0x10;

    e = os_swix(ColourTrans_GenerateTable, &r);

    if (!e)
    {
        if (wide)   /* is more than 1 byte per pixel used in table ?*/
            *wide = (r.r[4] >> (1 << bbc_modevar(((sprite_header *)id->s.addr)->mode, bbc_Log2BPP))) > 1;

        table = mm_malloc(r.r[4]);
        r.r[4] = (int)table;
        e = os_swix(ColourTrans_GenerateTable, &r);
    }

    if (!e)
        *pixtrans_out = table;
    else
        mm_free(table);

    return e;
}

os_error *fe_sprite_put_scaled(sprite_area *area, sprite_id *id, BOOL wide, int x, int y, sprite_factors *factors, sprite_pixtrans *pixtrans)
{
    os_error *e;
    e = sprite_put_scaled(area, id, wide ? 0x28 : 0x08, x, y, factors, pixtrans);
/*    if (e && e->errnum == 0x712)    // bad flags*/
/*        e = sprite_put_scaled(area, id, 0x08, x, y, factors, pixtrans);*/
    return e;
}

os_error *fe_plot_sprite(char *name, int x, int y, wimp_box *bb)
{
    sprite_id id;
    sprite_factors factors;
    sprite_pixtrans *pixtrans = NULL;
    BOOL wide;
    os_error *e;
    int w, h;

    e = fe_get_sprite(name, &id);
    if (!e) e = fe_get_sprite_scale(&id, &factors, &w, &h);
    if (!e) e = fe_get_sprite_pixtrans(&id, &pixtrans, &wide);
    if (!e) e = fe_sprite_put_scaled(resspr_area(), &id, wide, x, y, &factors, pixtrans);

    mm_free(pixtrans);

    if (bb)
    {
        bb->x0 = x;
        bb->y0 = y;
        bb->x1 = x + w;
        bb->y1 = y + h;
    }

    return e;
}

void fe_anti_twitter(const wimp_box *bb)
{
    if ( gbf_active(GBF_ANTI_TWITTER) && is_a_tv() )
    {
        os_regset r;
        os_error *e;

        r.r[0] = bb->x0;
        r.r[1] = bb->y0;
        r.r[2] = bb->x1 - bb->x0;
        r.r[3] = bb->y1 - bb->y0;

        if (r.r[2] > 0 && r.r[3] > 0)
        {
            e = os_swix(0x838C0, &r);
            if (e && e->errnum == 486 /* no such swi */)
                gbf_flags &= ~GBF_ANTI_TWITTER;
        }
    }
}

void toggle_anti_twitter(void)
{
    gbf_flags ^= GBF_ANTI_TWITTER;
}

/* ----------------------------------------------------------------------------------------------------- */

/*
 * This message formatting code ripped off from Navigator
 */

#define FONTNAME "Homerton.Medium"
#define FONTWIDTH  (32)
#define FONTHEIGHT (32)
#define LINEMAX 20

#define FG (0xFFFFFF00)
#define BG (0x77777700)

typedef struct
{
    const char *text;                   /* First char of this segment */
    int length;                         /* Length of this segment */
    int width;                          /* Width of this segment in millipoints */
} LineRec, *LinePtr;

static LineRec lines [LINEMAX];
static int numlines;

static struct
{
    int xs, ys, xl, yl, splitchar;
} coordblock = {0, 0, 0, 0, 32};

static os_error *split_message(const char *str, int width, int handle)
{
    int segwidth;
    const char *seg = str, *splitpoint;
    os_error *e = NULL;

    while (!e && numlines < LINEMAX)
    {
        os_regset r;
        if (*seg == 0)
            break;

        r.r[0] = handle;
        r.r[1] = (int)seg;
        r.r[2] = (1<<5) | (1<<8) | (1<<9);
        r.r[3] = width;
        r.r[4] = 0;
        r.r[5] = (int) &coordblock;
        e = os_swix(Font_ScanString, &r);
        if (!e)
        {
            splitpoint = (char *)r.r[1];
            segwidth = r.r[3];

            /* splitpoint should now point to a space, or the end of the string */
            lines[numlines].text = seg;
            lines[numlines].length = splitpoint - seg;
            lines[numlines].width = segwidth;
            seg = *splitpoint ? splitpoint + 1 : splitpoint;
            numlines++;
        }
    }

    return e;
}

/*
 * x0, x1, y0, y1 are the coordinates of the area into which we want to format
 * the text.  They are in OS Units.
 */

static os_error *message_paint(const char *str, const char *str2, wimp_box *box)
{
    os_error *e;
    int handle, i, y, space_x, spacing;
    int x0, y0, x1, y1;
    font_info info;

    e = font_converttopoints(box->x0, box->y0, &x0, &y0);
    if (!e) e = font_converttopoints(box->x1, box->y1, &x1, &y1);

    handle = 0;
    if (!e) e = font_find(FONTNAME, FONTWIDTH * 16, FONTHEIGHT * 16, 0, 0, &handle);

    numlines = 0;
    if (!e) e = split_message (str, x1 - x0, handle);
    if (!e && str2 != NULL && numlines <= LINEMAX - 2)
    {
        lines[numlines].text = "";
        lines[numlines].length = 0;
        lines[numlines].width = 0;
        numlines++;
        e = split_message (str2, x1 - x0, handle);
    }
    if (!e) e = font_readinfo(handle, &info);
    if (!e) e = font_converttopoints(info.maxx, info.maxy, &space_x, &spacing);

    /* Determine the Y coordinate of the top line */
    y = y0 + (y1 - y0) / 2 + numlines * spacing / 2 - spacing;
    if (y > y1 - spacing)
        y = y1 - spacing;

    /* Set the font colours */
    if (!e)
    {
        font fh = handle;
        wimp_paletteword fg, bg;
        int max_offset = 14;
        fg.word = FG;
        bg.word = BG;
        colourtran_setfontcolours(&fh, &bg, &fg, &max_offset);
    }

    /* Actually display the segments */
    for (i = 0; !e && i < numlines; i++)
    {
        os_regset r;
        r.r[0] = handle;
        r.r[1] = (int)lines[i].text;
        r.r[2] = (1<<7) | (1<<8) | (1<<9);
        r.r[3] = x0 + (x1 - x0) / 2 - lines[i].width / 2;
        r.r[4] = y;
        r.r[7] = lines[i].length;
        e = os_swix(Font_Paint, &r);

        y -= spacing;
    }

    if (handle)
        font_lose(handle);
    return e;
}

/* ----------------------------------------------------------------------------------------------------- */

static wimp_paletteword error_bg_col = { 0, 0x88, 0x88, 0x88 };

void fe_message(const char *msg)
{
    int gcol;
    wimp_box box;

    if (msg == NULL || msg[0] == 0)
        return;

    if (!done_init)
    {
        printf("%s\n", msg);
        return;
    }

    colourtran_setGCOL(error_bg_col, 0, 0, &gcol);
    bbc_rectanglefill(error_box.x0, error_box.y0, error_box.x1 - error_box.x0-1, error_box.y1 - error_box.y0-1);

    box = error_box;
    box.x0 += 32;
    box.y0 += 32;
    box.x1 -= 32;
    box.y1 -= 32;
    message_paint(msg, NULL, &box);
    fe_anti_twitter(&error_box);
}

#if 0
void fe_report_error(const char *msg)
{
#if 1
    os_error e;
    e.errnum = 0;
    strncpy(e.errmess, msg, sizeof(e.errmess));
    wimp_reporterror(&e, wimp_EOK, PROGRAM_NAME);
#else
    int key;

    bbc_vdu(bbc_Bell);

    fe_message(msg);

    wait_for_release(INT_MAX);

    while (!akbd_pollkey(&key))
    {
        os_regset r;
        os_swix(OS_Mouse, &r);
        if (r.r[2])
            break;
    }

    fe_refresh_screen(&error_box);
#endif
}
#endif

/* ----------------------------------------------------------------------------------------------------- */

os_error *feutils_open_behind_toolbar(wimp_w w)
{
    os_error *e;
    wimp_wstate state;

    STBDBG(("feutils_open_behind_toolbar: w %x\n", w));

    e = wimp_get_wind_state(w, &state);
    if (!e)
    {
	state.o.behind = fe_status_window_handle();
	e = wimp_open_wind(&state.o);
    }
    return e;
}

os_error *feutils_open_in_front(wimp_w w)   /* pdh 11-03-98 */
{
    os_error *e;
    wimp_wstate state;

    STBDBG(("feutils_open_in_front: w %x\n", w));

    e = wimp_get_wind_state(w, &state);
    if (!e)
    {
	state.o.behind = -1;
	e = wimp_open_wind(&state.o);
    }
    return e;
}

os_error *feutils_window_create(wimp_box *box, const wimp_box *margin, const fe_frame_info *ip, int bgcol, BOOL open, wimp_w *w_out)
{
    os_error *e;
    wimp_wind bg_win;

    /* open a full screen window to display onto*/
    memset(&bg_win, 0, sizeof(bg_win));

    bg_win.box = *box;
    bg_win.behind = fe_status_window_handle();
    bg_win.flags = (wimp_wflags)(wimp_WNEW + wimp_WTRESPASS);
    bg_win.colours[wimp_WCWKAREABACK] = 255;
    bg_win.colours[wimp_WCSCROLLOUTER] = 3;
    bg_win.colours[wimp_WCSCROLLINNER] = 1;

    if (ip && ip->scrolling == fe_scrolling_YES && config_display_frames_scrollbars)
    {
        bg_win.flags = (wimp_wflags)((int)bg_win.flags | (int)wimp_WHSCR | (int)wimp_WVSCR);
        bg_win.colours[wimp_WCTITLEFORE] = wimp_version >= 380 ? 255 : bgcol;
        bg_win.box.x1 -= toolsprite_width-2;
        bg_win.box.y0 += toolsprite_height;
    }
    else
    {
	bg_win.colours[wimp_WCTITLEFORE] = 255;
    }

    *box = bg_win.box;

    bg_win.ex.x0 = -margin->x0;
    bg_win.ex.x1 = bg_win.box.x1 - (bg_win.box.x0 + margin->x0);
    bg_win.ex.y1 = -margin->y1;
    bg_win.ex.y0 = -(bg_win.box.y1 - bg_win.box.y0) - margin->y1;

    bg_win.scx = -margin->x0;
    bg_win.scy = -margin->y1;

    bg_win.minsize = 0x00010001;

    STBDBG(("scrolling %d win flags %x\n", ip ? ip->scrolling : -1, bg_win.flags));
    STBDBG(("win box %d,%d %d,%d (%d,%d)\n",
        bg_win.box.x0, bg_win.box.y0, bg_win.box.x1, bg_win.box.y1, bg_win.scx, bg_win.scy));
    STBDBG(("win ex  %d,%d %d,%d\n",
        bg_win.ex.x0, bg_win.ex.y0, bg_win.ex.x1, bg_win.ex.y1));

    bg_win.workflags = (wimp_iconflags)(wimp_IBTYPE*wimp_BDEBOUNCEDRAG);
    e = wimp_create_wind(&bg_win, w_out);

    if (open)
	e = feutils_open_behind_toolbar(*w_out);

    return e;
}


/* min size of a window with a scroll bar*/

#define MIN_WIN_WIDTH   (61*2 + 19*2)
#define MIN_WIN_HEIGHT  (61*2 + 19*2)

int feutils_check_window_bars(wimp_box *extent, wimp_openstr *op, int *old_x_scroll_bar, int *old_y_scroll_bar, int bgcol)
{
    int width = op->box.x1 - op->box.x0;
    int height = op->box.y1 - op->box.y0;
    BOOL x_scroll_bar = config_display_frames_scrollbars && width >= MIN_WIN_WIDTH && extent->x1 - extent->x0 > width + (*old_y_scroll_bar ? toolsprite_width : 0);
    BOOL y_scroll_bar = config_display_frames_scrollbars && height >= MIN_WIN_HEIGHT && extent->y1 - extent->y0 > height + (*old_x_scroll_bar ? toolsprite_height : 0);

    STBDBG(( "feutils_check_window_bar from %s/%s wimp=%d\n", caller(1), caller(2), wimp_version ));

    STBDBG(( "  x %d want %d; y %d want %d\n",
             x_scroll_bar, *old_x_scroll_bar,
             y_scroll_bar, *old_y_scroll_bar ));

    if (x_scroll_bar != *old_x_scroll_bar || y_scroll_bar != *old_y_scroll_bar)
    {
        wimp_winfo winfo;
        wimp_caretstr c;
        BOOL has_caret;

        wimp_wstate state;
	wimp_w parent_w;
	int align_flags;
	int wflags;

        wimp_get_caret_pos(&c);
        has_caret = c.w == op->w;

	/* read either entire window block or just the current state */
	if (wimp_version < 380)
	{
	    STBDBG(( "feutils_check_window_bars: calling get_wind_info\n" ));

	    winfo.w = op->w;
	    wimp_get_wind_info((wimp_winfo *)((int)&winfo | 1));

	    wimp_delete_wind(op->w);
	    wflags = (int)winfo.info.flags;
	}
	else
	{
	    STBDBG(( "feutils_check_window_bars: exciting GetWindowState call\n" ));

	    state.o.w = op->w;
	    _swix(Wimp_GetWindowState, _INR(1,2) | _OUTR(3,4), &state, *(int *)"TASK", &parent_w, &align_flags);
	    wflags = (int)state.flags;
	}

	STBDBG(( "feutils_check_window_bars: get flags done\n" ));

        /* update window structure and flags */
        if (y_scroll_bar && !*old_y_scroll_bar)
        {
            wflags |= (int)wimp_WVSCR;
            op->box.x1 -= toolsprite_width-2;
        }
        else if (!y_scroll_bar && *old_y_scroll_bar)
        {
            wflags &=~ (int)wimp_WVSCR;
            op->box.x1 += toolsprite_width-2;
            if (extent->x1 - extent->x0 < op->box.x1 - op->box.x0)
                extent->x1 = extent->x0 + (op->box.x1 - op->box.x0);
        }
        if (x_scroll_bar && !*old_x_scroll_bar)
        {
            wflags |= (int)wimp_WHSCR;
            op->box.y0 += toolsprite_height;
        }
        else if (!x_scroll_bar && *old_x_scroll_bar)
        {
            wflags &= ~(int)wimp_WHSCR;
            op->box.y0 -= toolsprite_height;
            if (extent->y1 - extent->y0 < op->box.y1 - op->box.y0)
                extent->y0 = extent->y1 - (op->box.y1 - op->box.y0);
        }

	/* either recreate the window or just adjust the flags, and set the extent */
	if (wimp_version < 380)
	{
	    winfo.info.ex = *extent;
	    winfo.info.colours[wimp_WCTITLEFORE] = x_scroll_bar || y_scroll_bar ? bgcol : 255;
	    winfo.info.flags = (wimp_wflags)wflags;
	    wimp_create_wind(&winfo.info, &op->w);

	    STBDBG(("recreate win %x as %x\n", winfo.w, op->w));

	    wimp_open_wind(op);
	}
	else
	{
	    /* pdh 30/10/97: was _INR(1,2), &extent */
	    _swix(Wimp_SetExtent, _INR(0,1), op->w, extent);

            STBDBG(( "feutils_check_window_bars: making exciting OpenWindow call\n" ));

	    state.o = *op;
	    state.flags = (wimp_wflags)wflags;

	    STBDBG(("set flags to 0x%x align_flags 0x%x\n", wflags, align_flags));

	    frontend_fatal_error((os_error *)_swix(Wimp_OpenWindow, _INR(1,4), &state, *(int *)"TASK", parent_w, align_flags | 1));
	}

	/* reposition the caret */
        if (has_caret)
        {
            STBDBG(( "feutils_check_window_bars: setting caret\n" ));
            c.w = op->w;
            wimp_set_caret_pos(&c);
        }

	/* update the scroll bar values */
        *old_x_scroll_bar = x_scroll_bar;
        *old_y_scroll_bar = y_scroll_bar;

        STBDBG(("feutils_check_window_bars: completed\n"));

        return TRUE;
    }

    STBDBG(("feutils_check_window_bars: nothing to do\n"));

    return FALSE;
}

void feutils_resize_window(wimp_w *w, const wimp_box *margin, const wimp_box *box_in, int *x_scroll_bar, int *y_scroll_bar, int doc_width, int doc_height, int scrolling, int bgcol)
{
    wimp_wstate state;
    wimp_box box = *box_in;
    int new_width, new_height;
    wimp_box ex;
    int dw, dh;

    wimp_get_wind_state(*w, &state);

    new_width  = box_in->x1 - box_in->x0;
    new_height = box_in->y1 - box_in->y0;

    if (*y_scroll_bar)
        new_width -= toolsprite_width-2;
    if (*x_scroll_bar)
        new_height -= toolsprite_height;

    STBDBG(("resize: window current %d,%d %d,%d %d,%d bars x=%d y=%d\n",
        state.o.box.x0, state.o.box.y0, state.o.box.x1, state.o.box.y1,
        doc_width, doc_height, *x_scroll_bar, *y_scroll_bar));
    STBDBG(("resize: window new     %d,%d %d,%d %d,%d\n",
        box_in->x0, box_in->y0, box_in->x1, box_in->y1,
        new_width, new_height));

    dw = doc_width + margin->x0 - margin->x1;
    dh = doc_height + margin->y0 - margin->y1;
#if 1
    ex.x0 = - margin->x0;
    ex.x1 = (new_width > dw ? new_width : dw) - margin->x0;
    ex.y1 = - margin->y1;
    ex.y0 = - (new_height > dh ? new_height : dh) - margin->y1;
#else
    ex.x0 = - margin->x0;
    ex.x1 = (new_width > doc_width ? new_width : doc_width) - margin->x1;
    ex.y1 = - margin->y1;
    ex.y0 = - (new_height > doc_height ? new_height : doc_height) - margin->y0;
#endif

#if 1
/*   if (new_width > dw || new_height > dh) */
#else
    if (new_width > doc_width || new_height > doc_height)
#endif
    {
        wimp_redrawstr r;
        r.w = *w;
        r.box = ex;
        wimp_set_extent(&r);

	STBDBG(("resize: set extent to %d,%d %d,%d\n",
        r.box.x0, r.box.y0, r.box.x1, r.box.y1));
    }

    if (*x_scroll_bar)
        box.y0 += toolsprite_height;
    if (*y_scroll_bar)
        box.x1 -= toolsprite_width-2;

    STBDBG(("resize: set box to %d,%d %d,%d\n",
        box.x0, box.y0, box.x1, box.y1));

    state.o.box = box;

    if (scrolling == fe_scrolling_AUTO && feutils_check_window_bars(&ex, &state.o, x_scroll_bar, y_scroll_bar, bgcol))
    {
        *w = state.o.w;
    }
    else
    {
/*	if (state.o.behind == -1)
	    state.o.behind = fe_status_window_handle();*/

	STBDBG(("resize: open %x behind %x\n", state.o.w, state.o.behind));

        frontend_fatal_error(wimp_open_wind(&state.o));
    }
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_refresh_screen(const wimp_box *box)
{
    fe_refresh_window((wimp_w)-1, box);
}

void fe_refresh_window(wimp_w w, const wimp_box *box)
{
    wimp_redrawstr r;
    r.w = w;
    if (box)
        r.box = *box;
    else
    {
        r.box.x0 = -0x4000;
        r.box.x1 = 0x4000;
        r.box.y0 = -0x4000;
        r.box.y1 = 0x4000;
    }
    wimp_force_redraw(&r);
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_click_sound(void)
{
    if (config_sound_click)
        os_cli(config_sound_click);
}

/* ensure we have caret*/
void fe_get_wimp_caret(wimp_w w)
{
    wimp_caretstr c;
    wimp_get_caret_pos(&c);
    if (c.w != w)
    {
	c.w = w;
	c.i = -1;
	c.x = -0x4000;
	c.y = -0x4000;
	c.height = -1;
	wimp_set_caret_pos(&c);
    }
}

/* ----------------------------------------------------------------------------------------------------- */

/* init_1 must be done before wimp initialise*/

void feutils_init_1(void)
{
    char *s;

    /* global vars for rest of system*/
    frontend_dx = 1<<bbc_vduvar(bbc_XEigFactor);
    frontend_dy = 1<<bbc_vduvar(bbc_YEigFactor);

    /* find the screen coords*/
    screen_box.x0 = screen_box.y0 = 0;
    screen_box.x1 = (bbc_vduvar(bbc_XWindLimit) + 1) * frontend_dx;
    screen_box.y1 = (bbc_vduvar(bbc_YWindLimit) + 1) * frontend_dy;

    /* override to check out particular screen sizes */
    s = getenv("NCFresco$Screen");
    if (s)
	sscanf(s, "%d,%d,%d,%d", &screen_box.x0, &screen_box.y0, &screen_box.x1, &screen_box.y1);
}

/* init_2 must be done after config_init*/

void feutils_init_2(void)
{
    int offset;

    if (config_display_margin_auto && tv_safe_area(&text_safe_box))
    {
	margin_box.x0 = text_safe_box.x0 - screen_box.x0;
	margin_box.x1 = text_safe_box.x1 - screen_box.x1;
	margin_box.y0 = text_safe_box.y0 - screen_box.y0;
	margin_box.y1 = text_safe_box.y1 - screen_box.y1;
    }
    else if (config_display_margin_auto && !is_a_tv())
    {
	memset(&margin_box, 0, sizeof(margin_box));
	text_safe_box = screen_box;
    }
    else
    {
	margin_box.x0 =   ((screen_box.x1*config_display_margin.x0/100) + 3) &~ 3;
	margin_box.x1 = - ((screen_box.x1*config_display_margin.x1/100) + 3) &~ 3;
	margin_box.y0 =   ((screen_box.y1*config_display_margin.y0/100) + 3) &~ 3;
	margin_box.y1 = - ((screen_box.y1*config_display_margin.y1/100) + 3) &~ 3;

	text_safe_box.x0 = screen_box.x0 + margin_box.x0;
	text_safe_box.x1 = screen_box.x1 + margin_box.x1;

	text_safe_box.y0 = screen_box.y0 + margin_box.y0;
	text_safe_box.y1 = screen_box.y1 + margin_box.y1;
    }

    offset = ((screen_box.x1/6) + 3) &~ 3;
    error_box.x0 = screen_box.x0 + margin_box.x0;
    error_box.x1 = screen_box.x1 + margin_box.x1;

    offset = ((screen_box.y1/6) + 3) &~ 3;
    error_box.y0 = screen_box.y0 + margin_box.y0;
    error_box.y1 = screen_box.y1 + margin_box.y1;

    if (is_a_tv())
	gbf_flags |= GBF_ANTI_TWITTER;

    get_tool_sprite_sizes(&toolsprite_width, &toolsprite_height);

    done_init = TRUE;

    STBDBG(("screen box %d,%d %d,%d\n", screen_box.x0, screen_box.y0, screen_box.x1, screen_box.y1));
    STBDBG(("text_safe box %d,%d %d,%d\n", text_safe_box.x0, text_safe_box.y0, text_safe_box.x1, text_safe_box.y1));
    STBDBG(("margin box %d,%d %d,%d\n", margin_box.x0, margin_box.y0, margin_box.x1, margin_box.y1));
    STBDBG(("error box %d,%d %d,%d\n", error_box.x0, error_box.y0, error_box.x1, error_box.y1));
}

/* ----------------------------------------------------------------------------------------------------- */

/*
 * Log this to the session management if on the trial
 */

static BOOL logging = TRUE;

#define Session_ReportEvent 0x83CC2

void session_log(const char *url, int state)
{
    os_regset r;
    os_error *e;
    char buffer[256];
    char *scheme, *netloc, *path, *params, *query, *frag;

    if (!logging)
        return;

    url_parse((char *)url, &scheme, &netloc, &path, &params, &query, &frag);

    strcpy(buffer, "WWW000000000 ");

    switch (state)
    {
        case session_REQUESTED:
            strcat(buffer, "    REQUEST ");
            break;
        case session_CONNECTED:
            strcat(buffer, "    CONNECT ");
            break;
        case session_FAILED:
            strcat(buffer, "       FAIL ");
            break;
    }

    /* if a known web scheme scheme then just log domain else log full url*/
    if (strcasecomp(scheme, "http") == 0 ||
        strcasecomp(scheme, "ftp") == 0 ||
        strcasecomp(scheme, "gopher") == 0 ||
        strcasecomp(scheme, "wais") == 0)
    {
        strcat(buffer, scheme);
        strcat(buffer, "://");
        if (netloc)
            strcat(buffer, netloc);
    }
    else
    {
        strcat(buffer, url);
    }

    STBDBG(("session log '%s'\n", buffer));

    r.r[0] = 0;
    r.r[1] = 1;
    r.r[2] = 6;
    r.r[3] = (int)buffer;
    r.r[4] = strlen(buffer);
    e = os_swix(Session_ReportEvent, &r);
    if (e && e->errnum == 486 /* no such swi */)
        logging = FALSE;

    url_free_parts(scheme, netloc, path, params, query, frag);
}

/* ----------------------------------------------------------------------------------------------------- */

os_error *fe_file_to_url(char *file, char **url_out)
{
    char buffer1[256];
    char buffer2[256];
    char *path;
    char *url;
    os_error *ep;

    /* check for null or empty path*/
    if (file == NULL || file[0] == 0)
        return NULL;

    /* gstrans it*/
    if ((ep = (os_error *)_swix(OS_GSTrans, _INR(0,3), (int)file, (int)buffer1, sizeof(buffer1))) != NULL)
        return ep;

    /* if still has "-URL" prefixed then just return the URL*/
    if (strncasecomp(buffer1, "-URL", 4) == 0)
    {
        *url_out = mm_strdup(skip_space(buffer1+4));
        return NULL;
    }

    /* otherwise canonicalise and convert to URL*/
    if ((ep = ((os_error *)_swix(OS_FSControl, _INR(0,5), 37, (int) buffer1, (int) buffer2, 0, 0, sizeof(buffer2)))) != NULL)
	return ep;

    path = url_riscos_to_path(buffer2);
    url = url_unparse("file", 0, path, 0, 0, 0);
    mm_free(path);

    *url_out = url;

    return NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

char *extract_value(const char *s, const char *tag)
{
    char *tagpos, *terminator;
    char *translated;
/*     int len; */
    char *copy;

    copy = mm_strdup(s);

    /* search for tag */
    tagpos = strstr(copy, tag);
    if (tagpos == NULL)
    {
        mm_free(copy);
        return NULL;
    }

    /* inc over tag */
    tagpos += strlen(tag);

    /* look for end */
    terminator = strchr(tagpos, '&');
    if (terminator)
        *terminator = 0;
/*     len = strlen(tagpos) + 1; */

    translated = url_unescape(tagpos, TRUE);
/*     translated = mm_malloc(len); */
/*     if (s) */
/*         translate_escaped_form_text((char *)tagpos, translated, len); */

    mm_free(copy);

    return translated;
}

/* ----------------------------------------------------------------------------------------------------- */

/* Error functions */

void frontend_fatal_error(os_error *e)
{
    if (e && (e->errnum &~ 0xff000000) != 0x288)
    {
        os_error er;

#if DEVELOPMENT
        usrtrc("fatal %x '%s'\n", e->errnum, e->errmess);
        usrtrc("by '%s' from '%s'\n", caller(1), caller(2));
#endif
        STBDBG(("fatal %x '%s'\n", e->errnum, e->errmess));
        STBDBG(("by '%s' from '%s'\n", caller(1), caller(2)));

        er.errnum = e->errnum;
        sprintf(er.errmess, msgs_lookup("fatal2:"), e->errnum);

	_swix(Wimp_ReportError, _INR(0,5), &er, wimp_EOK/* | (1<<8)*/, PROGRAM_NAME, NULL, NULL, 0);
        exit(1);
    }
}

os_error *frontend_complain(os_error *e)
{
    if (e)
    {
#if DEVELOPMENT
        usrtrc("complain %x '%s'\n", e->errnum, e->errmess);
        usrtrc("by '%s' from '%s' from '%s'\n", caller(1), caller(2), caller(3));
#endif

	switch (config_mode_errors)
	{
	case mode_errors_WIMP:
	    if ((e->errnum &~ 0xff000000) != 0x288)
		wimp_reporterror(e, wimp_EOK, PROGRAM_NAME);
	    break;

	case mode_errors_OWN:
	    pending_error = *e;

	    /* send the error service call so that the error reporter knows what happened */
	    _swix(OS_ServiceCall, _INR(1,4), 0x400C0, e, 0, PROGRAM_TITLE);	/* Service_ErrorStarting */
	    break;

	case mode_errors_MESSAGE:
	{
	    wimp_msgstr msg;

	    pending_error = *e;

	    msg.hdr.size = 28;
	    msg.hdr.your_ref = 0;
	    msg.hdr.action = (wimp_msgaction)MESSAGE_ERROR_BROADCAST;
	    msg.data.words[0] = e->errnum;
	    msg.data.words[1] = (int)&pending_error;
	    wimp_sendmessage(wimp_ESENDWANTACK, &msg, 0);
	    break;
	}
	}
    }
    return e;
}

/* ----------------------------------------------------------------------------------------------------- */

/*
 * If there is a scheme at the start then ignore
 * Note parse needs // to identify the start of the netloc.
 * Otherwise it will be assumed to be the root of the path.
 */

char *check_url_prefix(const char *url)
{
    char *scheme, *netloc, *path, *params, *query, *frag;
    char *new_url;
    url_parse((char *)url, &scheme, &netloc, &path, &params, &query, &frag);

    if (scheme == NULL && (netloc || path))
    {
        char *s;

        if (netloc == NULL)
        {
            char *slash = strchr(path, '/');
            if (!slash)
            {
                netloc = path;
                path = NULL;
            }
            else if (slash != path)
            {
                char *path1;

                netloc = strndup(path, slash-path);
                path1 = mm_strdup(slash);

                mm_free(path);
                path = path1;
            }
        }

        s = netloc;
        if (strncasecomp(s, "ftp", 3) == 0)
            scheme = mm_strdup("ftp");
        else if (strncasecomp(s, "gopher", 6) == 0)
            scheme = mm_strdup("gopher");
        else
            scheme = mm_strdup("http");
    }

    new_url = url_unparse(scheme, netloc, path ? path : "/", params, query, frag);

    url_free_parts(scheme, netloc, path, params, query, frag);

    return new_url;
}

/* ----------------------------------------------------------------------------------------------------- */

/* Ensure there is at least SLOT_SIZE bytes in the next slot before starting a task */

#define SLOT_SIZE	(128*1024)

os_error *fe_start_task(const char *cli, wimp_t *task_out)
{
    int us = -1, next = -1, free;
    int next_old = -1;
    os_error *e;

    e = wimp_slotsize(&us, &next, &free);

    if (!e && next < SLOT_SIZE)
    {
	next_old = next;

	us = -1;
	next = SLOT_SIZE;
	wimp_slotsize(&us, &next, &free);
    }

    visdelay_begin();

    /* always try this, never mind whether an error occurs or not */
    _kernel_setenv(PROGRAM_NAME"$LastCLI", cli);

    e = (os_error *)_swix(Wimp_StartTask, _IN(0)|_OUT(0), cli, task_out);

    visdelay_end();

    if (!e && next_old != -1)
    {
	us = -1;
	next = next_old;
	e = wimp_slotsize(&us, &next, &free);
    }

    return e;
}


/* ----------------------------------------------------------------------------------------------------- */

int check_edge_proximity(int pos, int left, int right, int threshold)
{
    int dleft, dright, r;

    /* check threshold isn't too big for window */
    if (threshold > (right-left)/2)
	threshold = (right-left)/2;

    /* calculate distances from window */
    dleft = pos - left;
    dright = right - pos;
    r = 0;

    if (dleft < dright)
    {
	r = dleft - threshold;
	if (r > 0)
	    r = 0;
    }
    else
    {
	r = threshold - dright;
	if (r < 0)
	    r = 0;
    }
    return r;
}

wimp_w feutils_find_top_window(wimp_w awindow)
{
    wimp_wstate wws;

    STBDBG(("feutils_find_top_window: %p", awindow));

    for (;;)
    {
        if ( wimp_get_wind_state( awindow, &wws ) )
        {
            STBDBG((" gave error\n"));
            return -1;
        }

        if ( wws.o.behind == -1 )
        {
            STBDBG((" reached top\n"));
            return awindow;
        }

        awindow = wws.o.behind;
        STBDBG((" -> %p", awindow ));
    }
}

/* ----------------------------------------------------------------------------------------------------- */

/* eof feutils.c*/
