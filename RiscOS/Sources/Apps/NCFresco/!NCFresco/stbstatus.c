/* -*-c-*- */

/* WIMP code for the ANTWeb WWW browser */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "memwatch.h"

#include "coords.h"
#include "wimp.h"
#include "res.h"
#include "resspr.h"
#include "bbc.h"
#include "msgs.h"
#include "akbd.h"
#include "font.h"
#include "swis.h"
#include "alarm.h"
#include "colourtran.h"
#include "visdelay.h"

#include "interface.h"
#include "rid.h"
#include "render.h"
#include "stbview.h"

#include "status.h"
#include "config.h"

#include "stbstatus.h"
#include "stbutils.h"
#include "util.h"

/* os coords
 * status win goes from 0 to 220
 * visible area is from 116 to 220
 * object type   136x112
 * slider height      42
 * fetch status   94x132
 */

#define statuswin_OBJECT_TYPE_X     (object_type_box.x0)
#define statuswin_OBJECT_TYPE_Y     (object_type_box.y0)

#define statuswin_SLIDER_X          (slider_box.x0)
#define statuswin_SLIDER_X_LIMIT    (slider_box.x1)
#define statuswin_SLIDER_WIDTH      (statuswin_SLIDER_X_LIMIT - statuswin_SLIDER_X)
#define statuswin_SLIDER_Y          (slider_box.y0)

#define statuswin_TITLE_X           (title_box.x0)
#define statuswin_TITLE_X_LIMIT     (title_box.x1)
#define statuswin_TITLE_Y           (title_box.y0)

#define statuswin_TITLE_FONT_NAME   "Homerton.Medium"
#define statuswin_TITLE_FONT_X      (14*16)
#define statuswin_TITLE_FONT_Y      (14*16)

#define statuswin_FETCH_X           (fetch_status_box.x0)
#define statuswin_FETCH_Y           (fetch_status_box.y0)

/* -------------------------------------------------------------------------- */

static BOOL info_level = 0;
       int statuswin_height = 0;

static wimp_box object_type_box = { 0 };
static wimp_box fetch_status_box = { 0 };
static wimp_box slider_box = { 0 };
static wimp_box title_box = { 0 };

static char *statuswin_message = NULL;
       wimp_w statuswin_w = -1;

statuswin_state_t statuswin_state = statuswin_CLOSED;

/* ------------------------------------------------------------------------------------------- */

void statuswin_sprite_init(void)
{
    int w,h;
    int topline;

    fe_get_sprite_size("slice", NULL, &statuswin_height);

    /* get positions on the status window*/
    topline = statuswin_height - 8;

    fe_get_sprite_size("type0", &w, &h);
    object_type_box.x0 = text_safe_box.x0;
    object_type_box.x1 = object_type_box.x0 + w;
    object_type_box.y1 = topline;
    object_type_box.y0 = object_type_box.y1 - h;

    fe_get_sprite_size("cross1", &w, &h);
    fetch_status_box.x1 = text_safe_box.x1;
    fetch_status_box.x0 = fetch_status_box.x1 - w;
    fetch_status_box.y1 = topline;
    fetch_status_box.y0 = fetch_status_box.y1 - h;

    fe_get_sprite_size("bubl", NULL, &h);
    slider_box.x0 = object_type_box.x1 + 64;
    slider_box.x1 = fetch_status_box.x0 - 64;

    title_box.x0 = slider_box.x0;
    title_box.x1 = slider_box.x1;

    title_box.y1 = topline;
    title_box.y0 = title_box.y1 - h;

    slider_box.y1 = title_box.y0 - 16;
    slider_box.y0 = slider_box.y1 - h;
}

/*
 * We assume that the three parts of the bubble can use the same scaling and translation tables
 */

static os_error *statuswin_draw_slider(char *sprites[], int start, int end, int y, wimp_box *clip)
{
    sprite_factors factors;
    sprite_pixtrans *pixtrans = NULL;
    BOOL wide;
    os_error *e;

    sprite_id l_id, c_id, r_id;
    int l_w, c_w, r_w;

    int x;

/*    if (clip) bbc_gwindow(clip->x0, clip->y0, clip->x1-1, clip->y1-1);*/

    e = fe_get_sprite(sprites[0], &l_id);
    if (!e) e = fe_get_sprite(sprites[1], &c_id);
    if (!e) e = fe_get_sprite(sprites[2], &r_id);

    if (!e) e = fe_get_sprite_scale(&l_id, &factors, &l_w, NULL);
    if (!e) e = fe_get_sprite_scale(&c_id, NULL, &c_w, NULL);
    if (!e) e = fe_get_sprite_scale(&r_id, NULL, &r_w, NULL);

    if (!e) e = fe_get_sprite_pixtrans(&l_id, &pixtrans, &wide);

#if 0
    from = 0;
    to = end - r_w;
    if (clip)
    {
        from = ((clip->x0 - (start + l_w)) / c_w) * c_w;
        if (from < 0) from = 0;

        if (to > clip->x1)
            to = clip->x1;
    }

    if (end - start > l_w + r_w) for (x = (start + l_w) + from; !e && x < to; x += c_w)
    {
        e = fe_sprite_put_scaled(resspr_area(), &c_id, wide, x, y, &factors, pixtrans);
    }
#else
    for (x = start + l_w; !e && x < end - r_w; x += c_w)
    {
        if (!clip || (x+c_w >= clip->x0 && x < clip->x1))
            e = fe_sprite_put_scaled(resspr_area(), &c_id, wide, x, y, &factors, pixtrans);
    }
#endif

    if (!e && (clip == NULL || clip->x0 < start + l_w))
        e = fe_sprite_put_scaled(resspr_area(), &l_id, wide, start, y, &factors, pixtrans);

    if (!e && (clip == NULL || clip->x1 > end - r_w))
        e = fe_sprite_put_scaled(resspr_area(), &r_id, wide, end - r_w, y, &factors, pixtrans);

    mm_free(pixtrans);

/*    if (clip) bbc_gwindow(0, 0, screen_box.x1-1, screen_box.y1-1);*/

    return e;
}

static char *well_sprites[] = { "bubl", "bubc", "bubr" };
static char *inner_sprites[] = { "left", "centre", "right" };

static os_error *statuswin_draw_fetch_status(fe_view v, BOOL anti_twit)
{
    os_error *e = NULL;
    if (statuswin_state == statuswin_OPEN || statuswin_state == statuswin_OPEN_SMALL)
    {
        wimp_box box;
        if (statuswin_state == statuswin_OPEN)
        {
            e = fe_plot_sprite("scale0", statuswin_FETCH_X, statuswin_FETCH_Y, &box);
            if (!e && anti_twit)
                fe_anti_twitter(&box);
        }
        if (!e) e = statuswin_update_fetch_status(v, v->fetch_status);
    }
    return e;
}

os_error *statuswin_update_fetch_info(fe_view v, char *msg)
{
    if (info_level >= 2)
    {
        mm_free(statuswin_message);
        statuswin_message = mm_strdup(msg);

        return statuswin_draw_title(v, TRUE);
    }
    return NULL;
}

os_error *statuswin_update_fetch_status(fe_view v, int status)
{
    wimp_paletteword col;
    wimp_box box;
    int gcol;
    int counter;
    be_doc doc;

    if (statuswin_state != statuswin_OPEN && statuswin_state != statuswin_OPEN_SMALL)
    {
        v->fetch_status = status;
        return NULL;
    }

    /* have we a document at all?*/
    doc = v->fetching ? v->fetching : v->displaying;
    if (doc == NULL)
        return NULL;

    switch (status)
    {
        case sb_status_PROGRESS_LOCAL:
        case status_COMPLETED_FILE:
        case status_COMPLETED_DIR:
            v->fetch_document = 256;
            if (fe_plot_sprite(statuswin_state == statuswin_OPEN_SMALL ? "tick2" : "tick1", statuswin_FETCH_X, statuswin_FETCH_Y, &box) == NULL)
                fe_anti_twitter(&box);
            break;

        case sb_status_PROGRESS_ABORTED:
        case status_FAIL_CONNECT:
        case status_FAIL_REQUEST:
        case status_FAIL_DNS:
        case status_FAIL_PASSWORD:
            v->fetch_document = 256;
            if (fe_plot_sprite(statuswin_state == statuswin_OPEN_SMALL ? "cross2" : "cross1", statuswin_FETCH_X, statuswin_FETCH_Y, &box) == NULL)
                fe_anti_twitter(&box);
            break;

        default:
        {
            int y;

            if (v->fetch_status == sb_status_PROGRESS_LOCAL ||
                v->fetch_status == sb_status_PROGRESS_ABORTED ||
                v->fetch_status >= status_COMPLETED_FILE)
            {
                if (statuswin_state == statuswin_OPEN)
                {
                    if (fe_plot_sprite("scale0", statuswin_FETCH_X, statuswin_FETCH_Y, &box) == NULL)
                        fe_anti_twitter(&box);
                }
            }

            if (status < 199)
                col.word = 0x00EEEE00;
            else if (status < 499)
                col.word = 0x00CC0000;

            counter = ((alarm_timenow() - v->fetch_counter)*4) % 512;
            if (counter > 256)
                counter = 512 - counter;

            col.bytes.red = col.bytes.red * counter/256;
            col.bytes.green = col.bytes.green * counter/256;
            col.bytes.blue = col.bytes.blue * counter/256;

            box.x0 = fetch_status_box.x0 + 16*2;
            box.y0 = fetch_status_box.y0 + 16*2;
            box.x1 = fetch_status_box.x1 - fetch_status_box.x0 - 18*2*2 - 1;
            box.y1 = fetch_status_box.y1 - fetch_status_box.y0 - 16*2*2 - 1;

            if (box.x1 > box.x0 && box.y1 > box.y0)
            {
                y = v->fetch_document ? v->fetch_document*box.y1/256 : box.y1;
                colourtran_setGCOL(col, 0, 0, &gcol);
                bbc_rectanglefill(box.x0, box.y0, box.x1, y);

                if (y != box.y1)
                {
                    wimp_paletteword black;
                    black.word = 0;
                    colourtran_setGCOL(black, 0, 0, &gcol);
                    bbc_rectanglefill(box.x0, box.y0 + y + 2, box.x1, box.y1 - (y + 2));
                }
            }
            break;
        }
    }
    v->fetch_status = status;

    return NULL;
}

os_error *statuswin_update_object_type(fe_view v, BOOL anti_twit)
{
    static char *object_type_sprites[] =
    {
        "type0", "typel", "typem", "typet"
    };
    os_error *e = NULL;
    if (statuswin_state == statuswin_OPEN)
    {
        wimp_box box;
        e = fe_plot_sprite(object_type_sprites[v->current_object_type], statuswin_OBJECT_TYPE_X, statuswin_OBJECT_TYPE_Y, &box);
        if (!e && anti_twit)
            fe_anti_twitter(&box);
    }
    return e;
}

os_error *statuswin_draw_title(fe_view v, BOOL anti_twit)
{
    os_error *e = NULL;
    if (statuswin_state == statuswin_OPEN)
    {
        e = statuswin_draw_slider(well_sprites, statuswin_TITLE_X, statuswin_TITLE_X_LIMIT, statuswin_TITLE_Y, NULL);

        if (!e && v->displaying)
        {
            int flags;
            char *title, *url;
            font fh = NULL;
            int n_chars;

            e = backend_doc_info(v->displaying, &flags, NULL, &url, &title);
            if (!e)
            {
                if (info_level == 2 && statuswin_message)
                    title = statuswin_message;
                else if (info_level >= 1 || title == NULL || title[0] == 0)
                    title = url;

                if (title == NULL || title[0] == 0)
                    title = "Untitled";

                e = font_find(statuswin_TITLE_FONT_NAME, statuswin_TITLE_FONT_X, statuswin_TITLE_FONT_Y, 0, 0, &fh);
            }

#if 1
            n_chars = strlen(title);
    	    if (!e)
    	    {
    	        os_regset r;

    	        r.r[0] = fh;
    	        r.r[1] = (int)title;
    	        r.r[2] = 0x300; /*font_GIVEN_FONT | font_KERN;*/
    	        r.r[3] = (statuswin_TITLE_X_LIMIT - 32)*400;
    	        r.r[4] = INT_MAX;
    	        r.r[5] = r.r[6] = 0;
    	        r.r[7] = n_chars;
    	        e = os_swix(Font_ScanString, &r);
    	        if (!e)
    	            n_chars = (char *)r.r[1] - title;
fprintf(stderr, "status: title '%s' %d chars\n", title, n_chars);
            }
#else
    	    if (!e)
    	    {
    	        os_regset r;
    	        int xsize, ysize;
    	        double xratio;

    	        r.r[0] = fh;
    	        r.r[1] = (int)title;
    	        r.r[2] = 0x300; /*font_GIVEN_FONT | font_KERN;*/
    	        r.r[3] = r.r[4] = INT_MAX;
    	        r.r[5] = r.r[6] = r.r[7] = 0;
    	        e = os_swix(Font_ScanString, &r);
    	        if (!e)
    	        {
    	            e = font_converttoos(r.r[3], r.r[4], &xsize, &ysize);
    	            if (!e)
    	            {
    	                xratio = (double)xsize / ((double)statuswin_TITLE_X_LIMIT - statuswin_TITLE_X - 32);
                        if (xratio > 1)
                        {
                            font_lose(fh);
                            fh = 0;
                            e = font_find(statuswin_TITLE_FONT_NAME, (int)(statuswin_TITLE_FONT_X/xratio), statuswin_TITLE_FONT_Y, 0, 0, &fh);
                        }
                    }
                }
            }
#endif
            if (!e)
            {
                font fh1 = fh;
                int offset = 14;
                wimp_paletteword fg, bg;

                fg.word = 0xffffff00;
                bg.word = 0x00880000;   /* greenish*/

                e = colourtran_setfontcolours(&fh1, &bg, &fg, &offset);
            }
#if 1
            if (!e)
            {
    	        os_regset r;
                r.r[1] = (int)title;
                r.r[2] = font_OSCOORDS | (config_display_blending ? 0x800: 0) | (1<<7);
                r.r[3] = statuswin_TITLE_X + 16;
                r.r[4] = statuswin_TITLE_Y + 12;
                r.r[7] = n_chars;
                e = os_swix(Font_Paint, &r);
            }
#else
            if (!e) e = font_paint(title, font_OSCOORDS | (config_display_blending ? 0x800: 0), statuswin_TITLE_X + 16, statuswin_TITLE_Y + 12);
#endif
            if (fh) font_lose(fh);
        }

        if (anti_twit)
            fe_anti_twitter(&title_box);
    }
    return e;
}

/* get work area coords of visible area and*/
/* convert to slider position of visible area*/

static void fe_get_slider_coords(fe_view v, int *s, int *e)
{
#if 1
    int ytop = -v->margin.y1;
    int ybottom = v->margin.y0;
    int slider_width = -v->doc_height + ytop + ybottom;
    wimp_wstate state;

    wimp_get_wind_state(v->w, &state);

    if (slider_width <= 0)
        slider_width = 1;

    if (s)
    {
        *s = statuswin_SLIDER_X + (ytop - state.o.y) * statuswin_SLIDER_WIDTH / slider_width;
        if (*s < statuswin_SLIDER_X)
            *s = statuswin_SLIDER_X;
        if (*s > statuswin_SLIDER_X_LIMIT)
            *s = statuswin_SLIDER_X_LIMIT;
    }
    if (e)
    {
        int end = - (state.o.y - (state.o.box.y1 - state.o.box.y0));
        *e = statuswin_SLIDER_X + end * statuswin_SLIDER_WIDTH / slider_width;
        if (*e < statuswin_SLIDER_X)
            *e = statuswin_SLIDER_X;
        if (*e > statuswin_SLIDER_X_LIMIT)
            *e = statuswin_SLIDER_X_LIMIT;
    }
#else
    if (s)
    {
        int start = - v->scroll_y;
        if (start < 0)
            start = 0;

        if (v->doc_height)
            *s = statuswin_SLIDER_X + start * statuswin_SLIDER_WIDTH / (-v->doc_height);
        else
            *s = 0;

        if (*s < statuswin_SLIDER_X)
            *s = statuswin_SLIDER_X;
    }

    if (e)
    {
/*        int end = - (v->visible.y0 + v->scroll_y - v->visible.y1);*/
        int end = - (screen_box.y0 + v->scroll_y - screen_box.y1);
        if (end > -v->doc_height)
            end = -v->doc_height;

        if (v->doc_height)
            *e = statuswin_SLIDER_X + end * statuswin_SLIDER_WIDTH / (-v->doc_height);
        else
            *e = statuswin_SLIDER_X_LIMIT;

        if (*e > statuswin_SLIDER_X_LIMIT)
            *e = statuswin_SLIDER_X_LIMIT;
    }
#endif
}

static os_error *statuswin_draw_well(wimp_box *bb)
{
    return statuswin_draw_slider(well_sprites, statuswin_SLIDER_X, statuswin_SLIDER_X_LIMIT, statuswin_SLIDER_Y, bb);
}

static os_error *statuswin_draw_inner(fe_view v, wimp_box *bb)
{
    return statuswin_draw_slider(inner_sprites, v->slider_start, v->slider_end, statuswin_SLIDER_Y + 4, bb);
}

os_error *statuswin_refresh_slider(fe_view v)
{
    int start, end;
    wimp_box box;
    os_error *e = NULL;

    if (statuswin_state != statuswin_OPEN)
        return NULL;

    /* find new coords*/
    box = slider_box;
    fe_get_slider_coords(v, &start, &end);
#if 1
    if (start > v->slider_start)
    {
        box.x0 = v->slider_start;
        box.x1 = start;
        e = statuswin_draw_well(&box);
        fe_anti_twitter(&box);
    }

    if (!e && end < v->slider_end)
    {
        box.x0 = end;
        box.x1 = v->slider_end;
        e = statuswin_draw_well(&box);
        fe_anti_twitter(&box);
    }
    if (!e && (start != v->slider_start || end != v->slider_end))
    {
        box.x0 = v->slider_start = start;
        box.x1 = v->slider_end = end;
        statuswin_draw_inner(v, NULL);

        box.y0 += 4;
        box.y1 -= 2;
/*        fe_anti_twitter(&box);*/
    }
#else
    e = statuswin_draw_well(NULL);
    if (!e)
    {
        v->slider_start = start;
        v->slider_end = end;
        statuswin_draw_inner(v, NULL);
        fe_anti_twitter(&slider_box);
    }
#endif

    return e;
}

static os_error *statuswin_redraw(fe_view v, wimp_redrawstr *r)
{
    sprite_id id;
    sprite_factors factors;
    sprite_pixtrans *pixtrans = NULL;
    BOOL wide;
    os_error *e;
    int w,x;

    e = fe_get_sprite("slice", &id);
    if (!e) e = fe_get_sprite_scale(&id, &factors, &w, NULL);
    if (!e) e = fe_get_sprite_pixtrans(&id, &pixtrans, &wide);

    if (statuswin_state == statuswin_OPEN)
    {
        for (x = 0; !e && x < r->box.x1; x += w)
            if (x+w >= r->box.x0)
                e = fe_sprite_put_scaled(resspr_area(), &id, wide, x, 0, &factors, pixtrans);
    }
    else
    {
        int gcol;
        wimp_paletteword col;
/*
        if (v && v->displaying)
            col = render_get_colour(render_colour_BACK, v->displaying);
        else
 */
            col.word = 0;
        colourtran_setGCOL(col, 0, 0, &gcol);
        bbc_rectanglefill(r->g.x0, r->g.y0, r->g.x1 - r->g.x0, r->g.y1 - r->g.y0);
    }

    mm_free(pixtrans);

    if (!e)
        e = statuswin_update_object_type(v, FALSE);
    if (!e)
        e = statuswin_draw_fetch_status(v, FALSE);
    if (!e)
        e = statuswin_draw_well(NULL);
    if (!e)
    {
        fe_get_slider_coords(v, &v->slider_start, &v->slider_end);
        e = statuswin_draw_inner(v, NULL);
    }
    if (!e)
        e = statuswin_draw_title(v, FALSE);
    return e;
}

/* ------------------------------------------------------------------------------------------- */

#define BORDER  0   /* was 2*/

os_error *statuswin_open(fe_view v, BOOL open_small)
{
    os_error *e = NULL;
    wimp_wstate state;

#if 1
    if (statuswin_state == statuswin_OPEN_SMALL && !open_small)
    {
        e = wimp_get_wind_state(statuswin_w, &state);

        state.o.box.x0 = screen_box.x0+BORDER;
        state.o.box.x1 = screen_box.x1-BORDER;
        state.o.box.y0 = screen_box.y0+BORDER;
        state.o.box.y1 = statuswin_height+BORDER;

        state.o.x = state.o.box.x0 - (screen_box.x0+BORDER);
        state.o.y = - state.o.box.y1;

        statuswin_state = statuswin_OPEN;

        e = wimp_open_wind(&state.o);
        if (!e)
        {
            wimp_redrawstr r;
            r.w = state.o.w;
            r.box.x0 = r.box.y0 = -0x4000;
            r.box.x1 = r.box.y1 = 0x4000;
            e = wimp_force_redraw(&r);
        }
    }
    else if (statuswin_state == statuswin_CLOSED)
    {
        wimp_wind win;
        memset(&win, 0, sizeof(win));

        if (open_small)
        {
            win.box = fetch_status_box;
        }
        else
        {
            win.box.x0 = screen_box.x0+BORDER;
            win.box.x1 = screen_box.x1-BORDER;
            win.box.y0 = screen_box.y0+BORDER;
            win.box.y1 = statuswin_height+BORDER;
        }

        win.behind = (wimp_w) -1;
        win.flags = (wimp_wflags)(wimp_WNEW + wimp_WTRESPASS);
        win.colours[wimp_WCWKAREABACK] = 255;
        win.colours[wimp_WCTITLEFORE] = 255;

        win.ex.x1 = screen_box.x1-2*BORDER;
        win.ex.y1 = statuswin_height;

        win.scx = win.box.x0 - (screen_box.x0+BORDER);
        win.scy = - win.box.y1; /*-statuswin_height;*/

        win.minsize = 0x00010001;

        win.workflags = (wimp_iconflags)(wimp_IBTYPE*wimp_BCLICKDEBOUNCE);
        e = wimp_create_wind(&win, &statuswin_w);

        if (!e) e = wimp_get_wind_state(statuswin_w, &state);
        if (!e) e = wimp_open_wind(&state.o);

        if (!e) statuswin_state = open_small ? statuswin_OPEN_SMALL : statuswin_OPEN;
    }
#else
    {
        wimp_wind *wp = template_syshandle("statuswin");
        wimp_openstr o;

        wimp_create_wind(wp, &statustool_w);

        o.w = statustool_w;
        o.box.x0 = screen_box.x0+2;
        o.box.x1 = screen_box.x1-2;
        o.box.y1 = screen_box.y1-2;
        o.box.y0 = o.box.y1 - 172 + margin_box.y1 - 4;
        o.x = -margin_box.x0;
        o.y = -margin_box.y1;
        o.behind = -1;
        e = wimp_open_wind(&o);
    }
#endif

    return e;
}

void statuswin_clear(BOOL only_if_small)
{
    if (statuswin_state == statuswin_OPEN_SMALL || !only_if_small)
    {
        wimp_delete_wind(statuswin_w);
        statuswin_w = -1;

        statuswin_state = statuswin_CLOSED;
    }

#if 0
    if (statustool_w != -1)
    {
        wimp_delete_wind(statustool_w);
        statustool_w = -1;
    }
#endif
}

os_error *statuswin_toggle(fe_view v)
{
    os_error *e = NULL;
    if (statuswin_state == statuswin_OPEN)
        statuswin_clear(FALSE);
    else
        e = statuswin_open(v, FALSE);
    return e;
}

BOOL statuswin_check_redraw(fe_view v, wimp_w w)
{
    wimp_redrawstr r;
    int more;

    if (statuswin_w != w)
        return FALSE;
#if 0
    fprintf(stderr, "redraw: w %x view %s\n", w, v && v->name ? v->name : "<none>");
#endif
    r.w = w;
    wimp_redraw_wind(&r, &more);

    while (more)
    {
        statuswin_redraw(v, &r);

        fe_anti_twitter(&r.g);
        wimp_get_rectangle(&r, &more);
    }

    return TRUE;
}

os_error *statuswin_info_level(fe_view v, int level)
{
    info_level = level;
    return statuswin_draw_title(v, TRUE);
}

/* ------------------------------------------------------------------------------------------- */
