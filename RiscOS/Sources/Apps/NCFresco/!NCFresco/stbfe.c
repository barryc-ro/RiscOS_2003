/* -*-c-*- */

/* WIMP code for the ANTWeb WWW browser */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <locale.h>

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
#include "kernel.h"
#include "flexwrap.h"
#include "heap.h"
#include "colourtran.h"
#include "visdelay.h"
#include "pointer.h"

#include "template.h"

#include "interface.h"
#include "access.h"
#include "stbview.h"
#include "webfonts.h"
#include "url.h"
#include "util.h"
#include "makeerror.h"
#include "images.h"
#include "urlopen.h"

#include "rid.h"
#include "render.h"
#include "rcolours.h"

#include "debug.h"
#include "status.h"

#include "filetypes.h"
#include "consts.h"
#include "auth.h"
#include "hotlist.h"
#include "config.h"
#include "version.h"
#include "licence.h"
#include "plugin.h"

#ifdef STBWEB_BUILD
#include "http.h"
#else
#include "../http/httppub.h"
#endif

#include "cookie.h"

#include "clipboard.h"
#include "printing.h"
#include "htmlparser.h"
#include "pluginfile.h"

#include "stbstatus.h"
#include "stbutils.h"
#include "stbmenu.h"
#include "stbhist.h"
#include "stbtb.h"
#include "stbfe.h"
#include "stbopen.h"
#include "fevents.h"

#if MEMLIB
#include "memheap.h"
#endif

#include "hierprof/HierProf.h"

#ifndef NEW_WEBIMAGE
#define NEW_WEBIMAGE	1
#endif

#if NEW_WEBIMAGE
#ifdef STBWEB_BUILD
#include "libs/webimage/webimage.h"
#else
#include "../webimage/webimage.h"
#endif
#endif

/* If HEAPCHECK is non-zero then heavy heap checking is enabled and you need to link with ansilib */
#ifndef HEAPCHECK
#define HEAPCHECK 0
#endif

#ifndef STBWEB_ROM
#define STBWEB_ROM 0
#endif

#define USE_DIALLER_STATUS 1

#define LINK_DBOX 1

#ifndef CHECK_IF_WITH_REGISTRY
#define CHECK_IF_WITH_REGISTRY 0
#endif

/* -------------------------------------------------------------------------- */

#define ROUND(a) (((a)+1)&~1)

#define INFO_POLL_TIME	20	/* centi-seconds */

#define Y_SCROLL(h)     ROUND((h)*4/5)
#define Y_SCROLL_LINE   32
#define X_SCROLL        100

#if 0
#define INTERNAL_PREFIX         PROGRAM_NAME"internal:"
#define INTERNAL_PREFIX_SIZE    (sizeof(INTERNAL_PREFIX)-1)
#endif

#define CLIP_FILE       "<Wimp$ScrapDir>."PROGRAM_NAME"Clip"
#define CLIP_FILE_LEN   sizeof(CLIP_FILE)

#define AUTOSCROLL_EDGE_THRESHOLD	64	/* closeness to edge to start auto-scrolling in OS units */
#define AUTOSCROLL_DELAY		100	/* delay before auto-scrolling takes affect */

/* -------------------------------------------------------------------------- */

#define TaskModule_RegisterService      0x4D302
#define TaskModule_DeRegisterService    0x4D303
#define wimp_MSERVICE                   0x4D300

#if USE_DIALLER_STATUS
#define Service_DiallerStatus           0xB4

#define dialler_DISCONNECTED            0
#define dialler_CONNECTED_OUTGOING      (1<<2)
#endif

#define Service_SmartCard		0xBA
#define smartcard_INSERTED		0x01

/* -------------------------------------------------------------------------- */


extern void parse_frames(int yes);
extern void image_get_cooler_table(void);

static void fe_drag(const wimp_mousestr *m);
static BOOL fe_resize(const wimp_mousestr *m);
static void fe_status_mode(fe_view v, int mode, int override);
static os_error *fe_status_state(fe_view v, int state);

static void check_pending_scroll(fe_view v);

static int fe_check_resize(fe_view start, int x, int y, wimp_box *box, int *handle, fe_view *resizing_v);
static void fe_update_page_info(fe_view v);

static fe_view fe_next_frame(fe_view v, BOOL next);

/* -------------------------------------------------------------------------- */

static wimp_t task_handle;

BOOL use_toolbox = FALSE;
static char *progname;

int debug_level;

/* Used to point to a block in the RMA for a long URL */
static char *fe_stored_url = NULL;

#if 0
static char *fe_pending_url = NULL;
static char *fe_pending_bfile = NULL;
#endif
static int fe_pending_key = 0;

fe_view main_view = 0;
static fe_view dbox_view = 0;

/* only used by ncfrescointernal:playmovie I think */
fe_view last_click_view = NULL;
int last_click_x, last_click_y;

static be_item highlight_last_link = NULL;

static int fast_poll = 0;

#if USE_DIALLER_STATUS
static int connection_up = 1;
static int connection_count = 0;
#endif

static int toolbar_pending_mode_change = FALSE;

static fe_message_handler_item *message_handlers = NULL;

static int user_status_open = TRUE;

static int linedrop_time = 0;
static int keyboard_state = fe_keyboard_OFFLINE;

/* ----------------------------------------------------------------------------------------------------- */

pointermode_t pointer_mode = (pointermode_t)-1;

static wimp_mousestr pointer_last_pos = { 0 };
/* static BOOL pointer_last_shift = FALSE; */

void fe_pointer_mode_update(pointermode_t mode)
{
    switch (mode)
    {
        case pointermode_OFF:
/*            case pointermode_INPUT:   */
/*                if (config_mode_keyboard) */
            os_cli("pointer 0");
            break;

        case pointermode_ON:
            if (pointer_mode != pointermode_ON)
                pointer_reset_shape();
            break;
    }
    pointer_mode = mode;
}

/* ----------------------------------------------------------------------------------------------------- */

typedef struct
{
    char *name;
    int x, y;       /* hotspot  */
} pointer_info;

#define ptr_DEFAULT 0
#define ptr_HAND    1
#define ptr_MAP     2
#define ptr_CARET   3
#define ptr_CLICK   4
#define ptr_MENU    5

static pointer_info pointers[] =
{
    { NULL, 0, 0 },
    { "ptr_hand", 4, 0 },
    { "ptr_map", 16, 16 },
    { "ptr_caret", 4, 0 },
    { "ptr_click", 0, 22 },
    { "ptr_menu", 0, 4 },
    { "ptr_push", 4, 0 },
    { "ptr_resizew", 16, 0 },
    { "ptr_resizeh", 0, 16 },
    { "ptr_push", 4, 0 }	/* This is meant to be the secure submit cursor */
};

#define fe_pointer_MAP              (1U<<31)
#define fe_pointer_DRAG             (1U<<30)
#define fe_pointer_RESIZE_WIDTH     (1U<<29)
#define fe_pointer_RESIZE_HEIGHT    (1U<<28)

static int fe_get_pointer_number(int item_flags)
{
    int ptr_num = 0;
    if (item_flags & be_item_info_USEMAP)
        ptr_num = 1;
    else if (item_flags & be_item_info_ISMAP)
        ptr_num = 1; /*2;   */
    else if (item_flags & (be_item_info_BUTTON | be_item_info_ACTION))
        ptr_num = 1; /*4;   */
    else if (item_flags & be_item_info_INPUT)
        ptr_num = 3;
    else if (item_flags & be_item_info_MENU)
        ptr_num = 1; /* 5;  */
    else if (item_flags & be_item_info_LINK)
        ptr_num = 1;
    else if (item_flags & be_item_info_SECURE)
        ptr_num = 9;
    else if ((unsigned)item_flags & fe_pointer_MAP)   /* force map for the map mode */
        ptr_num = 2;
    else if ((unsigned)item_flags & fe_pointer_DRAG)   /* dragging mode */
        ptr_num = 6;
    else if ((unsigned)item_flags & fe_pointer_RESIZE_WIDTH)   /* resize w  */
        ptr_num = 7;
    else if ((unsigned)item_flags & fe_pointer_RESIZE_HEIGHT)   /* resize h */
        ptr_num = 8;
    return ptr_num;
}

static int pointer_current = 0;

void fe_set_pointer(int item_flags)
{
    int num;

    if (!config_display_fancy_ptr)
	return;
    
    num = fe_get_pointer_number(item_flags);

    if (num != pointer_current)
    {
        pointer_info *info = &pointers[num];

        STBDBG(( "stbfe: set pointer %d '%s' mode %d\n", num, strsafe(info->name), pointer_mode));

        if (info->name)
        {
            sprite_id id;
            id.tag = sprite_id_name;
            id.s.name = info->name;
            if (pointer_set_shape(resspr_area(), &id, info->x, info->y) != NULL)
                pointer_set_shape(wimp_spritearea, &id, info->x, info->y);
        }
        else
        {
            if (pointer_mode == pointermode_OFF)
                os_cli("pointer 0");
            else
                pointer_reset_shape();
        }

        pointer_current = num;
    }
}

/* ----------------------------------------------------------------------------------------------------- */

int fe_bg_colour(fe_view v)
{
    wimp_paletteword pw;
    wimp_palettestr pal;

    if (v == NULL)		/* can happen on first page */
	return 0;
    
    pw = render_get_colour(render_colour_BACK, fe_find_top(v)->displaying);
    wimp_readpalette(&pal);

    return find_closest_colour(pw.word, (int *)pal.c, 16);
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_linedrop(int called_at, void *handle)
{
    os_error *e = os_cli(msgs_lookup("hangup"));
    if (e && e->errnum != 214)	/* file not found */
	frontend_complain(e);
    
    linedrop_time = 0;

    NOT_USED(called_at);
    NOT_USED(handle);
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_open_keyboard(fe_view v)
{
    char buffer[256];
    int n;

    n = sprintf(buffer, "NCKeyboard %s",
	    keyboard_state == fe_keyboard_ONLINE ? "-extension 0" : "");
    
    if (config_display_control_top)
	sprintf(buffer + n, "-scrolldown %d", text_safe_box.y1 - tb_status_height());
    else
	sprintf(buffer + n, "-scrollup %d", text_safe_box.y0 + tb_status_height());

    fe_start_task(buffer, NULL);
    NOT_USED(v);
}

/* ----------------------------------------------------------------------------------------------------- */

static BOOL fe_caretise(fe_view v)
{
    /* if the highlighted object is actually an INPUT then place caret  */
    if (v->current_link)
    {
        int flags;

        if (frontend_complain(backend_item_info(v->displaying, v->current_link, &flags, NULL, NULL)) == NULL &&
            (flags & be_item_info_INPUT))
        {
            frontend_complain(backend_activate_link(v->displaying, v->current_link, wimp_BLEFT));
            return TRUE;
        }
    }
    return FALSE;
}

/* ----------------------------------------------------------------------------------------------------- */

static void fe_type_file(fe_view v, const char *file_name)
{
    FILE *f;
    if ((f = fopen(file_name, "r")) != NULL)
    {
        os_error *e = NULL;
        int used = TRUE;
        while (!e && !feof(f) && used)
            e = backend_doc_key(v->displaying, fgetc(f), &used);
        fclose(f);
    }
}

/* ----------------------------------------------------------------------------------------------------- */

os_error *iterate_frames(fe_view top, os_error *(*fn)(fe_view v, void *handle), void *handle)
{
    fe_view v;
    os_error *e = NULL;
    for (v = top; !e && v; v = v->next)
    {
        e = fn(v, handle);

        if (!e && v->children)
            e = iterate_frames(v->children, fn, handle);
    }
    return e;
}

static void adjust_box_for_toolbar(wimp_box *box)
{
    if (use_toolbox && tb_is_status_showing())
    {
	if (config_display_control_top)
	    box->y1 -= tb_status_height() + STATUS_TOP_MARGIN;
	else
	    box->y0 += tb_status_height() + STATUS_TOP_MARGIN;
    }
}

/* ----------------------------------------------------------------------------------------------------- */

/* Unused functions */

os_error *savedoc(char *url, char *leaf, int ft)
{
    return NULL;
    url = url;
    leaf = leaf;
    ft = ft;
}

void frontend_saver_last_name(char *fname)
{
    fname = fname;
}

/* ----------------------------------------------------------------------------------------------------- */

#define FADE_LINE_SPACING		8

#define ScreenFade_FadeRectangle	0x4e5c0
#define ScreenFade_ProcessorUsage	0x4e5c1
#define ScreenFade_GetFadeInfo		0x4e5c2
#define ScreenFade_ReleaseFade		0x4e5c3

void frontend_fade_frame(fe_view v, wimp_paletteword colour)
{
    wimp_wstate state;
    int ref, status;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return;
    
    if (config_display_time_fade == 0)
	return;
    
    if (v->w)
	wimp_get_wind_state(v->w, &state);
    else
	state.o.box = v->box;

    if (v->parent == NULL && use_toolbox && tb_is_status_showing())
    {
	if (config_display_control_top)
	    state.o.box.y1 -= tb_status_height() + (screen_box.y1 - text_safe_box.y1);
	else
	    state.o.box.y0 += tb_status_height() + (text_safe_box.y0 - screen_box.y0);
    }
    

    if (_swix(ScreenFade_FadeRectangle, _INR(0,7) | _OUT(1),
	  0 + (1<<8),
	  state.o.box.x0, state.o.box.y0, state.o.box.x1, state.o.box.y1,
	  config_display_time_fade,
	  colour.word,
	  FADE_LINE_SPACING,
	  &ref) != NULL)
    {
	return;
    }
    
    do
    {
	_swix(ScreenFade_GetFadeInfo, _INR(0,1) | _OUT(0),
	      0, ref, &status);
    }
    while ((status & 0xff) != 0xff);

    _swix(ScreenFade_ReleaseFade, _INR(0,1), 0, ref);
    
    tb_status_refresh_if_small();
}

/* ----------------------------------------------------------------------------------------------------- */

static int decode_string(const char *input, const char *matches[], int nmatches)
{
    int i;
    for (i = 0; i < nmatches; i++)
	if (strcasecomp(input, matches[i]) == 0)
	    return i;
    return -1;
}

/*
 * If there is a fragment id then 'url' parameter will always contain it.
 */

int frontend_view_visit(fe_view v, be_doc doc, char *url, char *title)
{
    char *previous_url;
    int previous_mode;
    
    STBDBG(( "frontend_view_visit url '%s' title '%s' doc %p\n", strsafe(url), strsafe(title), doc));

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;
    
    if (doc == NULL)    /* lookup failed re visit the current one   */
    {
	char *durl;
	int f, ft;

 	frontend_complain(makeerrorf(ERR_CANT_GET_URL, strsafe(title)));

	if (v->displaying)
	{
	    backend_doc_info(v->displaying, &f, &ft, &durl, NULL);
	    if (durl)
		frontend_view_status(v, sb_status_URL, durl );
	}

        session_log("", session_FAILED);

	v->fetching = NULL;
	v->had_completed = TRUE;

        if (use_toolbox)
            tb_status_update_fades(fe_find_top(v));

        fe_status_clear_fetch_only();

	return 1;
    }

    session_log(url, session_CONNECTED);

    if (v->fetching == doc)
	v->fetching = NULL;
    else
	fprintf(stderr, "Erroneous call to view visit\n");

    previous_url = NULL;
    previous_mode = v->browser_mode;

    if (v->displaying)
    {
	char *durl;
	backend_doc_info(v->displaying, NULL, NULL, &durl, NULL);
	previous_url = strdup(durl);
	
	if (fe_map_view() == v)
            fe_map_mode(NULL, NULL);

        backend_dispose_doc(v->displaying);
    }

    fe_dispose_view_children(v);

    v->displaying = doc;
    v->current_link = NULL;
    v->find_last_item = NULL;

    /* really not sure about this one */
    if (frontend_view_has_caret(v))
	backend_place_caret(v->displaying, NULL);
    
    if (previous_url)
	frontend_fade_frame(v, render_get_colour(render_colour_BACK, v->displaying));

    /* check for special page instructions - but only on top page   */
    if (v->parent == NULL)
    {
        char *ncmode;
        const char *bmode;
	int mode = fe_browser_mode_WEB;
	int toolbar_state = -1;

	if ((ncmode = strdup(backend_check_meta(doc, "NCBROWSERMODE"))) != NULL)
	{
	    static const char *content_tag_list[] = { "SELECTED", "TOOLBAR", "KEYBOARD", "LINEDROP" };
	    static const char *on_off_list[] = { "OFF", "ON" };
	    static const char *keyboard_list[] = { "ONLINE", "OFFLINE" }; /* order must agree with #defines in stbview.h */
	    name_value_pair vals[4];

	    parse_http_header(ncmode, content_tag_list, vals, sizeof(vals)/sizeof(vals[0]));

	    mm_free(v->selected_id);
	    v->selected_id = strdup(vals[0].value);
	    toolbar_state = decode_string(vals[1].value, on_off_list, sizeof(on_off_list)/sizeof(on_off_list[0]));
	    keyboard_state = decode_string(vals[1].value, on_off_list, sizeof(keyboard_list)/sizeof(keyboard_list[0]));
	    
	    /* set up a pending line drop */
	    if (vals[1].value && linedrop_time == 0)
	    {
		linedrop_time = atoi(vals[1].value)*100;
/* 		set line drop linedrop_time */
/* 		linedrop_time += alarm_timenow() */
/* 		alarm_set(linedrop_time, fe_linedrop, &linedrop_time); */
	    }
	    else
	    {
/* 		set line drop default */
	    }

	    mm_free(ncmode);
	}

	if ((bmode = backend_check_meta(doc, "BROWSERMODE")) != NULL)
	{
	    mode = strcasecomp(ncmode, "DESKTOP") == 0 ? fe_browser_mode_DESKTOP :
		strcasecomp(ncmode, "DBOX") == 0 ? fe_browser_mode_DBOX :
		strcasecomp(ncmode, "HOTLIST") == 0 ? fe_browser_mode_HOTLIST :
		strcasecomp(ncmode, "HISTORY") == 0 ? fe_browser_mode_HISTORY :
		strcasecomp(ncmode, "APP") == 0 ? fe_browser_mode_APP :
		fe_browser_mode_WEB;
	}
	
	/* decide on what to store in the way of return pages */
	if (previous_mode == fe_browser_mode_DESKTOP)
	{
	    mm_free(v->return_page);
	    v->return_page = strdup(previous_url);
	}

	if (mode == fe_browser_mode_APP)
	{
	    if (previous_mode != fe_browser_mode_APP)
	    {
		mm_free(v->app_return_page);
		v->app_return_page = strdup(previous_url);
	    }
	}
	else
	{
	    mm_free(v->app_return_page);
	    v->app_return_page = NULL;
	}
	
	switch (mode)
        {
	case fe_browser_mode_WEB:
	    break;

	case fe_browser_mode_HOTLIST:
	case fe_browser_mode_HISTORY:
	case fe_browser_mode_DBOX:
	case fe_browser_mode_DESKTOP:
	case fe_browser_mode_APP:
	    v->dont_add_to_history = TRUE;
	    break;
	}

        fe_status_mode(v, mode, toolbar_state);
    }
    else
    {
        fe_status_mode(v, v->parent->browser_mode, -1);
    }

    v->pending_scroll = fe_history_visit(v, url, title);

    frontend_view_redraw(v, NULL);

    frontend_view_status(v, sb_status_URL, url);

    if (use_toolbox)
    {
	int f;
	backend_doc_info(v->displaying, &f, NULL, NULL, NULL);
	tb_status_set_secure(f & be_doc_info_SECURE);
    }

    fe_update_page_info(v);

    if (!fe_check_download_finished(v) || !v->pending_scroll)
        frontend_view_ensure_visable(v, -1, 0, 0);

    STBDBG(( "stbfe: fetching lights\n"));
    
    if (use_toolbox)
	tb_status_set_lights(light_state_FETCHING);

    mm_free(previous_url);
        
    return 1;
}

/* ----------------------------------------------------------------------------------------------------- */

static wimp_caretstr dbox_saved_caret;

void fe_dbox_dispose(void)
{
    if (dbox_view)
    {
	fe_view selected_view = fe_selected_view();
	STBDBG(( "fe_dbox_dispose: sel %p db %p\n", selected_view, dbox_view));

#if LINK_DBOX && 0
	/* unlink from chain */
	if (dbox_view->prev)
	    dbox_view->prev->next = dbox_view->next;
	if (dbox_view->next)
	    dbox_view->next->prev = dbox_view->prev;
#endif
	/* return caret whence it came */
	if (selected_view == dbox_view)
        {
	    wimp_set_caret_pos(&dbox_saved_caret);
/*             fe_get_wimp_caret(main_view->w); */
/*             selected_view = main_view; */
        }

        fe_dispose_view(dbox_view);
        dbox_view = NULL;

        fe_status_mode(main_view, main_view->browser_mode, -1);

	/* update main view with the correct URL */
        if (main_view->displaying)
        {
            char *url;
            backend_doc_info(main_view->displaying, NULL, NULL, &url, NULL);
            frontend_view_status(main_view, sb_status_URL, url);
        }
    }
}

fe_view fe_dbox_view(const char *name)
{
    fe_frame_info info;
    wimp_box box;
    fe_view view;

/*     fe_dbox_dispose(); */

    STBDBG(( "fe_dbox_view: \n"));

    info.name = (char *)name;
    info.noresize = TRUE;
    info.scrolling = fe_scrolling_NO;
    info.margin = margin_box;

    adjust_box_for_toolbar(&info.margin);

    if (use_toolbox)
	tb_menu_hide();

    if (strcasecomp(name, TARGET_PASSWORD) == 0 ||
	strcasecomp(name, TARGET_DBOX) == 0)
    {
	box.x0 = (text_safe_box.x0*3 + text_safe_box.x1  )/4;
	box.x1 = (text_safe_box.x0   + text_safe_box.x1*3)/4;
	box.y0 = (text_safe_box.y0*3 + text_safe_box.y1  )/4;
	box.y1 = (text_safe_box.y0   + text_safe_box.y1*3)/4;
    }
    else if (strcasecomp(name, TARGET_INFO) == 0)
    {
	box.x0 = text_safe_box.x0;
	box.x1 = text_safe_box.x1;
	box.y0 = text_safe_box.y0 + tb_status_height() + 32;
	box.y1 = (text_safe_box.y0 + text_safe_box.y1)/2;
    }
    else			/* favs, history, help */
    {
	box.x0 = text_safe_box.x0;
	box.x1 = (text_safe_box.x0 + text_safe_box.x1*3)/4;
	box.y0 = text_safe_box.y0 + tb_status_height() + 32;
	box.y1 = text_safe_box.y1 - 32;
    }	

    frontend_complain(fe_new_view(NULL, &box, &info, &view));

    if (view)
    {
	fe_view v;
/* 	wimp_get_caret_pos(&dbox_saved_caret); */
	if (view->w)
	    fe_get_wimp_caret(view->w);

#if LINK_DBOX
	/* chain onto end of main_view */
	for (v = main_view; v->next; v = v->next)
	    ;

	v->next = view;
	view->prev = v;
#endif
    }

    return view;
}

/* ----------------------------------------------------------------------------------------------------- */

void fe_dbox_cancel(void)
{
    if (fe_current_passwd)
        fe_passwd_abort();
    else
        fe_dbox_dispose();
}

/* ----------------------------------------------------------------------------------------------------- */

fe_view fe_locate_view(const char *name)
{
    return fe_find_target(main_view, name);
}

static os_error *fe__selected_view(fe_view v, void *handle)
{
    fe_view *pv = handle;
    if (v->is_selected)
	*pv = v;
    return NULL;
}

fe_view fe_selected_view(void)
{
    fe_view v = NULL;
    iterate_frames(main_view, fe__selected_view, &v);
    return v;
}

void fe_submit_form(fe_view v, const char *id)
{
    if (v && v->displaying)
	backend_submit_form(v->displaying, id, FALSE);
}

/* ------------------------------------------------------------------------------------------- */

int fe_doc_flag_state(fe_view v, int flags)
{
    return v && (v->flags & flags) != 0;
}

os_error *fe_doc_flag_toggle(fe_view v, int flags)
{
    os_error *e = NULL;

    v->flags ^= flags;

    config_display_body_colours = (v->flags & be_openurl_flag_BODY_COLOURS) != 0;
    config_defer_images = (v->flags & be_openurl_flag_DEFER_IMAGES) != 0;

    if (v->browser_mode == fe_browser_mode_WEB)
    {
        e = backend_doc_set_flags(v->displaying, 0, flags);

        if (flags & be_openurl_flag_DEFER_IMAGES)
            fe_reload(v);
        else
            fe_refresh_screen(NULL);

	if (use_toolbox)
	    tb_menu_refresh(v);
    }
    return e;
}

/* ----------------------------------------------------------------------------------------------------- */

int print__copies = 1, print__ul = 1;

int fe_print_possible(fe_view v)
{
    return v && v->displaying && _swix(PDriver_Info, 0) == NULL;
}

const char *fe_printer_name(void)
{
    char *s = getenv("Printer$");
    return s ? s : "";
}

os_error *fe_print(fe_view v)
{
    char *s;

    s = getenv("Printer$");
    if (s == NULL || s[0] == 0)
        return makeerror(ERR_NO_PRINTER);

/*     tb_status_set_message(status_type_ERROR, "Printing"); */
    
    return awp_print_document(v->displaying, config_print_scale,
        (config_print_nopics ? awp_print_NO_PICS : 0) |
        (config_print_nobg ? awp_print_NO_BACK : 0) |
        (config_print_nocol ? awp_print_NO_COLOUR : 0) |
        (print__ul ? awp_print_UNDERLINE : 0) |
        (config_print_sideways ? awp_print_SIDEWAYS : 0) |
        (config_print_collated ? awp_print_COLLATED : 0) |
        (config_print_reversed ? awp_print_REVERSED : 0),
        print__copies);
}

/* ------------------------------------------------------------------------------------------- */

static os_error *set_fetch_message(fe_view v)
{
    char buffer[256];
    int n;
    os_error *e;

#if 0
    int waiting, fetching, fetched, errors, in_trans, so_far;
    e = backend_doc_images(v->displaying, &waiting, &fetching, &fetched, &errors, &in_trans, &so_far);
    if (!e)
#endif
    {
        if (v->fetch_of > 0)
            n = sprintf(buffer, msgs_lookup("stfp"), v->fetch_rx*100/v->fetch_of);
        else
            n = sprintf(buffer, msgs_lookup("stfb"), v->fetch_rx);

        n += sprintf(buffer+n, msgs_lookup("stim"), v->images_had, v->images_had + v->images_waiting);

        e = tb_status_set_message(status_type_FETCH, buffer);
    }

    return e;
}

static void check_pending_scroll(fe_view v)
{
    if (v->pending_scroll)
    {
        wimp_box bb;
        int h, hh;

        frontend_view_bounds(v, &bb);

        h = bb.y1 - bb.y0;
        hh = h > -v->doc_height ? h : -v->doc_height;

        if (v->pending_scroll - h >= -hh)
        {
            frontend_view_ensure_visable(v, -1, v->pending_scroll, v->pending_scroll);
            v->pending_scroll = 0;
        }
    }
}

static os_error *fe__download_finished(fe_view v, void *handle)
{
    int *vals = handle;

    STBDBG(( "  %p", v));

    if (!v)
        return NULL;

    if (v->fetching)
    {
	vals[1]++;
	STBDBG(( " fetching\n"));
        return NULL;
    }

    if (v->displaying)
    {
	/* increment document count */
	if (v->had_completed)
	    vals[0]++;
        else
	    vals[1]++;
	
	/* check displayed message if completed document */
	if (v->had_completed && v->fetch_of > 0 && v->fetch_rx != v->fetch_of)
	{
	    v->fetch_rx = v->fetch_of;

	    if (use_toolbox && v->parent == NULL)
		set_fetch_message(v);
	}

#if 1
	vals[2] += v->images_had;
	vals[3] += v->images_waiting;
#else
        int waiting, fetching, fetched, errors, in_trans, so_far;

	/* see how many images left */
	if (backend_doc_images(v->displaying, &waiting, &fetching, &fetched, &errors, &in_trans, &so_far) == NULL)
	{
	    vals[1] += fetched + errors;
	    vals[2] += waiting + fetching;
	}
#endif
	STBDBG(( " page %d images %d/%d\n", v->had_completed, v->images_had, v->images_waiting));
    }

    return NULL;
}

static int fe_is_download_finished(fe_view v)
{
    int vals[4];

    STBDBG(( "stbfe: is download finished\n"));

    vals[0] = vals[1] = vals[2] = vals[3] = 0;
    iterate_frames(v, fe__download_finished, vals);
 
    STBDBG(( "stbfe: is download finished pages %d/%d images %d/%d\n", vals[0], vals[1], vals[2], vals[3]));

    return vals[1] == 0 && (vals[3] == 0 || (v->flags & be_openurl_flag_DEFER_IMAGES));
}

int fe_check_download_finished(fe_view v)
{
    fe_view top = fe_find_top(v);
    BOOL finished = FALSE;

    frontend_view_status(v, sb_status_WORLD);

    if (fe_is_download_finished(top))
    {
	/* update toolbar fades */
	if (use_toolbox)
	    tb_status_update_fades(top);

        /* get rid of temporary globe   */
        fe_status_clear_fetch_only();
	fe_update_page_info(v);

	finished = TRUE;
    }

    /*
     * When all frames have downloaded then we need to place the caret.
     * First see who has the caret.
     * If not one of us then ignore it.
     * If a frame with a window has the caret then ensure the link is visible and caretise if necessary.
     * If a frame without a window has the caret then move the link into the first of its children that does have a window.
     */
    
    if (finished)
    {
	wimp_caretstr cs;
	fe_view vcaret;

	STBDBG(( "stbfe: check download finished current link %p/%p top %p\n", v, v->current_link, top));

	wimp_get_caret_pos(&cs);

	/* find which frame has the caret */
	vcaret = fe_find_window(top, cs.w);

	STBDBG(( "stbfe: w %d vcaret %p\n", cs.w, vcaret));

	/* if top window has the caret and children then move into thew first child frame */
	if (vcaret == top && top->children)
	{
	    vcaret = fe_next_frame(top, TRUE);

	    STBDBG(( "stbfe: new vcaret %p\n", vcaret));
	}

	/* if the frame has data then update the link position */
	if (vcaret && vcaret->displaying)
	{
	    int flags = be_link_VISIBLE | be_link_INCLUDE_CURRENT;
	    if (pointer_mode != pointermode_OFF)
		flags |= be_link_TEXT;

	    vcaret->current_link = backend_highlight_link(vcaret->displaying, vcaret->current_link, flags);

	    STBDBG(( "stbfe: current link %p/%p\n", vcaret, vcaret->current_link));
	    if (!fe_caretise(vcaret))
	    {
		STBDBG(( "stbfe: place caret %p\n", vcaret));

		backend_place_caret(vcaret->displaying, NULL);

		STBDBG(( "stbfe: placed caret %p\n", vcaret));
	    }
	}
    }

    return finished;
}

/* ------------------------------------------------------------------------------------------- */

int frontend_view_status(fe_view v_orig, int status_type, ...)
{
    os_error *e = NULL;
    va_list ap;
    fe_view v;

    if (!v_orig || v_orig->magic != ANTWEB_VIEW_MAGIC)
	return 0;
    
    /* go to top of stack   */
    v = fe_find_top(v_orig);

    va_start(ap, status_type);

    if (status_type != -1) { STBDBG(( "view status %d\n", status_type)); };

    switch (status_type)
    {
    case sb_status_WORLD:       /* just mark time   */
	if (use_toolbox)
	    tb_status_rotate();
	else
	    e = statuswin_update_fetch_status(v, v->fetch_status);
	break;

    case sb_status_URL:         /* called by backend_open_url - ie starting to connect  */
    {
	char *url = va_arg(ap, char *);
	if (use_toolbox)
	{
	    if (v == v_orig)
	    {
		if (url && strncmp(url, PROGRAM_NAME"internal", sizeof(PROGRAM_NAME"internal")-1) == 0)
		    e = tb_status_set_message(status_type_URL, "");
		else
		    e = tb_status_set_message(status_type_URL, url);
	    }

	    tb_status_update_fades(v);
	}
	else
	{
	    statuswin_update_fetch_info(v, strsafe(url));
	    statuswin_draw_title(v, TRUE);
	}
	break;
    }
	
    case sb_status_FETCHED:     /* string giving amount of html document fetched    */
	if (use_toolbox)
	{
	    v_orig->fetch_rx = va_arg(ap, int);
	    v_orig->fetch_of = va_arg(ap, int);
	    e = set_fetch_message(v_orig);

	    STBDBG(( "stbfe: view status v %p fetching %d of %d bytes\n", v_orig, v_orig->fetch_rx, v_orig->fetch_of));
    
	    tb_status_set_lights(light_state_FETCHING);
	}
	else
	{
	    v->fetch_rx = va_arg(ap, int);
	    v->fetch_of = va_arg(ap, int);

	    v->fetch_document = v->fetch_of <= 0 ? 256 : v->fetch_rx*256/v->fetch_of;

	    e = statuswin_update_fetch_status(v, v->fetch_status);
	    if (!e) e = statuswin_update_fetch_info(v, "");
	}

	check_pending_scroll(v_orig);
	break;

    case sb_status_SENT:        /* string giving amount of request data sent to server  */
    {
	int tx = va_arg(ap, int);
	int of = va_arg(ap, int);
	if (use_toolbox)
	{
	    if (of > 0)
		e = tb_status_set_messagef(status_type_FETCH, msgs_lookup("stsp"), tx*100/of);
	    else
		e = tb_status_set_messagef(status_type_FETCH, msgs_lookup("stsb"), tx);
	}
	else
	{
	    e = statuswin_update_fetch_status(v, v->fetch_status);
	    if (!e) e = statuswin_update_fetch_info(v, "");
	}
	break;
    }

    case sb_status_PROGRESS:    /* token detailing progress aborted, fromcch, or status<HTTP error code>    */
    {
	int status = va_arg(ap, int);
	char s[16];

	sprintf(s, "status%03d", status);
	if (use_toolbox)
	{
	    e = tb_status_set_message(status_type_FETCH, msgs_lookup(s));
	}
	else
	{
	    e = statuswin_update_fetch_status(v, status);
	    if (!e) e = statuswin_update_fetch_info(v, msgs_lookup(s));
	}
	break;
    }

    case sb_status_IMAGE:       /* string giving images loaded  */
    {
	int fetched = va_arg(ap, int),
	    fetching = va_arg(ap, int),
	    waiting = va_arg(ap, int),
	    errors = va_arg(ap, int);
/* 	    so_far = va_arg(ap, int), */
/* 	    in_trans = va_arg(ap, int); */
	
	if (use_toolbox)
	{
	    v_orig->images_had = fetched + errors;
	    v_orig->images_waiting = waiting + fetching;

	    STBDBG(( "stbfe: view status v %p images %d/%d\n", v_orig, fetched + errors, waiting + fetching));

	    e = set_fetch_message(v_orig);
	}
	else
	{
	    int total = fetched + errors + waiting + fetching;
	    v->fetch_images = total ? (fetched + errors)*256/total : 256;

	    e = statuswin_update_fetch_status(v, v->fetch_status);
	    if (!e) e = statuswin_update_fetch_info(v, "");
	}

	check_pending_scroll(v_orig);
	fe_check_download_finished(v_orig);
	break;
    }
	
    case sb_status_ABORTED:
	STBDBG(( "stbfe: view status v %p aborted\n", v_orig));

	if (use_toolbox)
	{
	    e = tb_status_set_message(status_type_FETCH, msgs_lookup("sta"));
	}
	else
	{
	    e = statuswin_update_fetch_status(v, sb_status_PROGRESS_ABORTED);
	    if (!e) e = statuswin_update_fetch_info(v, msgs_lookup("sta"));
	}

	v_orig->had_completed = TRUE;
	v_orig->images_waiting = 0;
	fe_check_download_finished(v_orig);
	break;

    case sb_status_FINISHED:
	STBDBG(( "stbfe: view status v %p finished\n", v_orig));

	check_pending_scroll(v_orig);
	    
	v_orig->had_completed = TRUE;
	fe_check_download_finished(v_orig);
	break;

        /* these messages are sent by our null handler depending on */
        /* what's under the pointer or highlighted  */
    case sb_status_IDLE:
	break;

    case sb_status_LINK:
	break;

    case sb_status_MAP:
	break;

    case sb_status_HELPER:
	break;
    case sb_status_HELP:
	break;
    }

    va_end(ap);

    frontend_complain(e);

    return 0;
}

/* ------------------------------------------------------------------------------------------- */

int frontend_test_history(char *url)
{
    return fe_test_history(main_view, url);
}

/* ------------------------------------------------------------------------------------------- */

int frontend_can_handle_file_type(int ft)
{
    char *s, buffer[32];

    STBDBG(("frontend_can_handle_file_type(): ft %x (%03x)\n", ft, ft));

    sprintf(buffer, "Alias$@RunType_%03x", ft);
    s = getenv(buffer);
    if (s && *s)
        return TRUE;

    fe_report_error(msgs_lookup("nold"));

    return FALSE;
}

/* ------------------------------------------------------------------------------------------- */


#if USE_DIALLER_STATUS
int frontend_is_interface_up(void)
{
    STBDBG(("is_interface_up: var=%d\n", connection_up));
    return connection_up;
}

int frontend_is_interface_down(void)
{
#if CHECK_IF_WITH_REGISTRY
    STBDBG(("is_interface_down: var=%d fn=%d\n", connection_up, ncreg_interface_is_down()));
    
    if (!connection_up)
        return 1;

    if (ncreg_interface_is_down())
    {
        connection_up = 0;
        return 1;
    }
    return 0;
#else
    STBDBG(("is_interface_up: var=%d\n", connection_up));
    return !connection_up;
#endif
}
#endif

/* ------------------------------------------------------------------------------------------- */

void frontend_pass_doc(fe_view v, char *url, char *cfile, int ftype)
{
    wimp_msgstr msg;

    STBDBG(( "frontend_pass_doc url '%s' cfile '%s' ft %x\n", url, cfile, ftype));

    memset(&msg, 0, sizeof(msg));
#if 0
#else
    msg.hdr.size = sizeof(wimp_msgstr);
    msg.hdr.action = wimp_MDATAOPEN;
    msg.data.dataopen.type = ftype;
    strcpy(msg.data.dataopen.name, cfile);
    frontend_fatal_error(wimp_sendmessage(wimp_ESENDWANTACK, &msg, 0));
#endif
    
    if (v)
    {
	v->fetching = NULL;

	v->had_completed = TRUE;
	fe_check_download_finished(v);
    }
    STBDBG(( "frontend_pass_doc: out\n"));
}

void frontend_url_punt(fe_view v, char *url, char *bfile)
{
    wimp_eventstr e;
    urlopen_data *ud = (urlopen_data*) &(e.data.msg.data);
    int len;
    char *target = NULL;

    STBDBG(( "frontend_url_punt '%s'\n", url));

    if (v == NULL)
	v = main_view;

    if (v)
    {
	v->had_completed = TRUE;
	fe_check_download_finished(v);
    }

#if 0
    if (strncasecomp(url, INTERNAL_PREFIX, INTERNAL_PREFIX_SIZE) == 0)
    {
        fe_pending_url = strdup(url);
        fe_pending_bfile = strdup(bfile);
        return;
    }
#endif
    
    if (fe_stored_url)
    {
	rma_free(fe_stored_url);
	fe_stored_url = NULL;
    }

    len = strlen(url);

    /* Use strictly less */
    if (bfile == NULL && len < sizeof(ud->url))
    {
	strcpy(ud->url, url);
	e.data.msg.hdr.size = sizeof(wimp_msghdr) + ((len + 4) & ~3);
    }
    else
    {
        if (bfile) len += strlen(bfile) + 1;
        if (target) len += strlen(target) + 1;

	ud->indirect.tag = 0;
	ud->indirect.url.ptr = fe_stored_url = rma_alloc(len + 1);
	if (ud->indirect.url.ptr)
	{
	    char *ptr = ud->indirect.url.ptr;

	    strcpy(ptr, url);
	    ptr += strlen(url) + 1;

	    if (bfile)
	    {
	        ud->indirect.body_file.ptr = ptr;
	        strcpy(ptr, bfile);
	        ptr += strlen(bfile) + 1;
	    }

	    if (target)
	    {
	        ud->indirect.target.ptr = ptr;
	        strcpy(ptr, target);
	        /* ptr += strlen(target) + 1; */
	    }
	}

	e.data.msg.hdr.size = sizeof(wimp_msghdr) + sizeof(ud->indirect);
    }

    e.data.msg.hdr.your_ref = 0;
    e.data.msg.hdr.action = (wimp_msgaction) wimp_MOPENURL;

    frontend_fatal_error(wimp_sendmessage(wimp_ESENDWANTACK, &(e.data.msg), 0));
}

/* ------------------------------------------------------------------------------------------- */

static void fe_frame_resize(fe_view v, const wimp_box *box)
{
    STBDBG(( "resize: v %p frame to %4d,%4d %4d,%4d\n", v, box->x0, box->y0, box->x1, box->y1));

    v->box = *box;

    if (v->w)
        feutils_resize_window(&v->w, &v->margin, box,
            &v->x_scroll_bar, &v->y_scroll_bar,
            v->doc_width, -v->doc_height, v->scrolling, fe_bg_colour(v));

    if (v->displaying)
        backend_reset_width(v->displaying, 0);

    if (v->w)
        fe_refresh_window(v->w, NULL);
}

/*
 * The method for refreshing assumes that the frames will always be presented in the same order
 */

void frontend_frame_layout(fe_view v, int nframes, fe_frame_info *info, int refresh_only)
{
    int i;
    coords_cvtstr cvt;
    fe_view child = NULL;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return;
    
  /* get rid of the old layout before creating the new one */
    if (refresh_only)
        child = v->children;
    else
        fe_dispose_view_children(v);

    /* ensure at top of visible screen  */
    frontend_view_ensure_visable(v, -v->margin.x0, 0, 0);

    cvt = fe_get_cvt(v);
    for (i = 0; i < nframes; i++)
    {
        fe_frame_info *ip = &info[i];
        wimp_box box;

        box = *(wimp_box *)&ip->box;
        coords_box_toscreen(&box, &cvt);

        if (refresh_only)
        {
            if (child)
            {
                fe_frame_resize(child, &box);
                child = child->next;
            }
        }
        else
        {
            fe_view vv;

            /* correct for the margins we've put in
            if (v->parent != NULL)
                coords_offsetbox(&box, -v->margin.x0, -v->margin.y1, &box);
		*/

            frontend_complain(fe_new_view(v, &box, ip, &vv));
            if (ip->src)
	    {
                os_error *e = frontend_open_url(ip->src, vv, NULL, NULL, fe_open_url_FROM_FRAME);
		if (e && e->errnum != ANTWEB_ERROR_BASE + ERR_NO_SUCH_FRAG)
		    frontend_complain(e);
/* 		if (vv->fetching) */
/* 		    backend_set_margin(vv->fetching, &vv->backend_margin); */
	    }
	}
    }
}

/* ------------------------------------------------------------------------------------------- */

static int font_size_value = 0;

BOOL fe_font_size_set_possible(int value, BOOL absolute)
{
    if (!absolute)
	value += font_size_value;

    return value >= 0 && value < config_SCALES;
}

void fe_font_size_set(int value, BOOL absolute)
{
    fe_view v;

    if (absolute)
    {
	if (value >= 0 && value < config_SCALES)
	    font_size_value = value;
    }
    else
    {
	font_size_value += value;

	if (font_size_value < 0)
	    font_size_value = 0;
	else if (font_size_value >= config_SCALES)
	    font_size_value = config_SCALES - 1;
    }

    config_display_scale = config_display_scales[font_size_value];

    visdelay_begin();
    
    frontend_complain(webfonts_init());

    v = main_view;
    if (v->children)
    {
	do
	{
	    
	    v = fe_next_frame(v, TRUE);
	    if (v)
	    {
		backend_doc_reformat(v->displaying);
		fe_refresh_window(v->w, NULL);
	    }
	}	    
	while (v);
    }
    else
    {
	backend_doc_reformat(v->displaying);
	fe_refresh_window(v->w, NULL);
    }

    visdelay_end();
}

/*
 * Initial font size scaling is in % so this function initialises the index correctly.
 */

static void fe_font_size_init(void)
{
    int i;
    for (i = 0; i < config_SCALES; i++)
	if (config_display_scales[i] >= config_display_scale)
	{
	    font_size_value = i;
	    break;
	}
}

/* ------------------------------------------------------------------------------------------- */

/* 
 * Force this window to have no horizontal scrolling
 */

void fe_force_fit(fe_view v, BOOL force)
{
    int scale_value = 100;

    v = fe_find_top(v);
    
    if (!v->displaying)
	return;

    v = main_view;
    if (v->children)
    {
	do
	{
	    v = fe_next_frame(v, TRUE);
	    if (v)
	    {
		scale_value = force && v->doc_width > 0 ?
		    ((v->box.x1 + v->backend_margin.x1) - (v->box.x0 + v->backend_margin.x0)) * 100 / v->doc_width :
		    100;

		STBDBG(("fe_force_fit: v %p force %d scaling %d %%\n", v, force, scale_value));
    
		backend_doc_set_scaling(v->displaying, scale_value);
		backend_doc_reformat(v->displaying);
		fe_refresh_window(v->w, NULL);
	    }
	}	    
	while (v);
    }
    else
    {
	scale_value = force && v->doc_width > 0 ?
	    ((v->box.x1 + v->backend_margin.x1) - (v->box.x0 + v->backend_margin.x0)) * 100 / v->doc_width :
	    100;

	STBDBG(("fe_force_fit: v %p force %d scaling %d %%\n", v, force, scale_value));
    
	backend_doc_set_scaling(v->displaying, scale_value);
	backend_doc_reformat(v->displaying);
	fe_refresh_window(v->w, NULL);
    }

    visdelay_end();

}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_paste(fe_view v)
{
    static int list[2] = { FILETYPE_TEXT,-1 };
    wimp_msgdatasave datasave;
    datasave.w = v->w;
    datasave.i = -1;
    datasave.x = 0;
    datasave.y = 0;
    if (clipboard_Paste(list, &datasave))
        fe_type_file(v, CLIP_FILE);

    return NULL;
}

os_error *fe_paste_url(fe_view v)
{
    char *url;
    os_error *e;

    if (!v || !v->displaying)
        return NULL;

    e = NULL;
    if (v->dont_add_to_history)
        url = v->hist_at ? v->hist_at->url : NULL;
    else
        e = backend_doc_info(v->displaying, NULL, NULL, &url, NULL);

    if (!e && url)
    {
        int c, used = TRUE;
        while (!e && used && (c = *url++) != 0)
            e = backend_doc_key(v->displaying, c, &used);
    }

    return e;
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
        if ( (flags & (be_item_info_ISMAP | be_item_info_USEMAP)) ||
	     (v->browser_mode != fe_browser_mode_DESKTOP && v->browser_mode != fe_browser_mode_APP &&		/* input=image is just a graphical button on a desktop page */
	      (flags & (be_item_info_ACTION|be_item_info_IMAGE)) == (be_item_info_ACTION|be_item_info_IMAGE)))
        {
            coords_pointstr p;
            coords_cvtstr cvt = fe_get_cvt(v);

            p.x = (box.x0 + box.x1)/2;
            p.y = (box.y0 + box.y1)/2;
            coords_point_toscreen(&p, &cvt);

            fe_pointer_set_position(p.x, p.y);
            fe_map_mode(v, v->current_link);
        }
        else if (flags & be_item_info_INPUT)
        {
            e = backend_activate_link(v->displaying, v->current_link, 0);
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
/* key handlers */
/* ------------------------------------------------------------------------------------------- */

BOOL fe_writeable_handle_keys(fe_view v, int key)
{
    int used = FALSE;

    if (v && v->displaying)
        backend_doc_key(v->displaying, key, &used);

    return used;
}

/* ------------------------------------------------------------------------------------------- */
/* miscellaneous veneers between stbevent and backend   */
/* ------------------------------------------------------------------------------------------- */

static os_error *fe__abort_fetch_possible(fe_view v, void *handle)
{
    int *possible = handle;

    if (!v)
        return NULL;

    if (v->fetching)
    {
        (*possible)++;
        return NULL;
    }

    if (v->displaying)
    {
	if (!v->had_completed || v->images_waiting)
        {
            (*possible)++;
      	    return NULL;
      	}
    }

    return NULL;
}

int fe_abort_fetch_possible(fe_view v)
{
    int possible = 0;
    iterate_frames(v, fe__abort_fetch_possible, &possible);
    return possible != 0;
}

static os_error *fe__abort_fetch(fe_view v, void *handle)
{
    os_error *e = NULL;
    if (v->fetching)
    {
	backend_dispose_doc(v->fetching);
	v->fetching = NULL;

	frontend_view_status(v, sb_status_ABORTED);
    }
    else if (v->displaying)
    {
        e = backend_doc_abort(v->displaying);

	frontend_view_status(v, sb_status_ABORTED);
    }
    return e;
    handle = handle;
}

os_error *fe_abort_fetch(fe_view v)
{
    return iterate_frames(v, fe__abort_fetch, NULL);
}

/* ------------------------------------------------------------------------------------------- */

os_error *fe_home(fe_view v)
{
    os_error *e;
    char *home_url;

    e = fe_file_to_url(config_doc_default, &home_url);

    if (!e && home_url)
    {
        if (config_mode_platform == platform_CAMB_TRIAL)
        {
            if (v->displaying)
            {
                char *url;
                e = backend_doc_info(v->displaying, NULL, NULL, &url, NULL);
                if (!e && strcasecomp(home_url, url) == 0)
                {
                    fe_message(msgs_lookup("goodbye:"));
                    exit(0);
                }
            }
        }

        if (!e)
            e = frontend_open_url(home_url, v, TARGET_TOP, NULL, 0);
    }
    return e;
}

os_error *fe_search_page(fe_view v)
{
    os_error *e;
    char *url;

    e = fe_file_to_url(config_document_search, &url);
    if (!e && url)
	e = frontend_open_url(url, v, NULL, NULL, 0);

    return e;
}

os_error *fe_offline_page(fe_view v)
{
    os_error *e;
    char *url;

    e = fe_file_to_url(config_document_offline, &url);
    if (!e && url)
	e = frontend_open_url(url, v, NULL, NULL, 0);

    return e;
}

/* ------------------------------------------------------------------------------------------- */

static fe_view fe_next_frame(fe_view v, BOOL next)
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

void fe_move_highlight_frame_direction(fe_view v, int x, int y)
{
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

    if (v && v->displaying && v->current_link)
        backend_update_link(v->displaying, v->current_link, 0);

    if (vv)
    {
        fe_get_wimp_caret(vv->w);

        if (vv->displaying)
        {
            vv->current_link = backend_highlight_link(vv->displaying, NULL, (next ? 0 : be_link_BACK) | be_link_VISIBLE | be_link_DONT_HIGHLIGHT);

            if (vv->current_link)
            {
                if (!fe_caretise(vv))
                    backend_update_link(vv->displaying, vv->current_link, 1);
            }
            else
            {
                backend_place_caret(vv->displaying, NULL);
            }
        }
    }
}

void fe_move_highlight(fe_view v, int flags)
{
    be_item old_link, new_link;
    fe_view new_v;
    BOOL scrolled;

    if (!v)
        return;

    if (use_toolbox && (flags & be_link_VERT))
	tb_status_set_direction(flags & be_link_BACK ? 1 : 0);
    
    pointer_mode = pointermode_ON;  /* so that scroll_changed doesn't reposition highlight  */

    if (v->displaying)
    {
	/* if we had a caret shown then move from here  */
	/* else from previous selection */
	old_link = backend_place_caret(v->displaying, backend_place_caret_READ);
	if (old_link == NULL)
	    old_link = v->current_link;

	/* try to move to next highlight if there is one    */
	new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP | be_link_DONT_HIGHLIGHT);
	new_v = v;
    }
    else
    {
	new_v = NULL;
	new_link = NULL;
	old_link = NULL;
    }

    STBDBG(( "fe_move_highlight: old_link %p old_v %p new_link %p new_v %p\n", old_link, v, new_link, new_v));

    /* check for moving to the status bar */
    if (new_link == NULL && config_mode_cursor_toolbar && (flags & be_link_VERT) &&
	((config_display_control_top && (flags & be_link_BACK)) || (!config_display_control_top && (flags & be_link_BACK) == 0)))
    {
	if (tb_status_highlight(TRUE))
	    return;
    }

    /* if we have reached top/bottom of page then scroll and try again for a highlight  */
    if (new_link)
    {
        scrolled = FALSE;
    }
    else
    {
	if (v->displaying)
	{
	    scrolled = fe_view_scroll_y(v, flags & be_link_BACK ? +1 : -1);

	    new_link = backend_highlight_link(v->displaying, old_link, flags | be_link_DONT_WRAP | be_link_DONT_HIGHLIGHT);
	}
	else
	    scrolled = FALSE;

	STBDBG(( "fe_move_highlight: scrolled %d new_link %p\n", scrolled, new_link));

        /* if we couldn't scroll (bottom/top of page) try to move to next/prev frame */
        if (!new_link && !scrolled)
        {
	    /* loop until we get to a frame which non-existent, the same as we started on or has a document */
	    new_v = fe_next_frame(v, flags & be_link_BACK ? 0 : 1);

            /* don't want to wrap if this is the only frame */
            if (new_v == v)
                new_v = NULL;

            /* if can't move on then beep */
            if (new_v == NULL)
	    {
                bbc_vdu(7);
	    }
            else if (new_v->displaying)
            {
                /* else scroll new frame and find first visible link */
                fe_view_scroll_y(new_v, flags & be_link_BACK ? -3 : +3);
                new_link = backend_highlight_link(new_v->displaying, NULL, flags | be_link_DONT_HIGHLIGHT);
            }
        }

	STBDBG(( "fe_move_highlight: new_link %p new_v %p\n", new_link, new_v));
    }

    /* new_v == v, new_link != NULL: may have scrolled, new link         - update link
     * new_v == v, new_link == NULL: scrolled, no new link visible       - no action
     * new_v == NULL, couldn't scroll or change frame                    - beep
     * new_v != v, new_link != NULL: changed frame, new link             - update caret, link
     * new_v == v, new_link == NULL: changed frame, no new link visiible - update caret, old link
     */

    /* if we have a new link or the frame has changed then update stuff */
    if (new_v && (new_link || new_v != v))
    {
	STBDBG(( "fe_move_highlight: update_link\n"));

        new_v->current_link = new_link;

        if (old_link)
            backend_update_link(v->displaying, old_link, 0);

        /* if the highlighted object is actually an INPUT then place caret  */
        if (new_v->displaying)
	{
	    if (!fe_caretise(new_v))
	    {
		backend_update_link(new_v->displaying, new_link, 1);
		backend_place_caret(new_v->displaying, NULL);
	    }
	}
	else
	{
	    fe_get_wimp_caret(new_v->w);
	}
    }

    fe_pointer_mode_update(pointermode_OFF);
}

/* ------------------------------------------------------------------------------------------- */

void fe_cursor_movement(fe_view v, int x, int y)
{
    int used;

    if (y)
	tb_status_set_direction(y > 0);

    backend_doc_cursor(v->displaying, y > 0 ? be_cursor_UP : be_cursor_DOWN, &used);
    if (!used)
	fe_view_scroll_y(v, y);
}

/* ------------------------------------------------------------------------------------------- */

/*
 * This is called when the clipboard is asked to save its data to an application
 */

static BOOL clip_save_text(const char *to, int file_type, void *handle)
{
    const char *from = clipboard_Data(NULL);
    return frontend_complain((os_error *)_swix(OS_FSControl, _INR(0,3), 0x1A, from, to, 0)) == NULL;
    handle = handle;
    file_type = file_type;
}

static clipboard_itemstr clip_list_text[] =
{
    { 0xfff, 0, clip_save_text },
    { -1 }
};

static clipboard_itemstr clip_list_sprite[] =
{
    { 0xff9, 0, clip_save_text },
    { -1 }
};

/*
 * When destroying the clipboard then remove the file
 */

static void clip_destroy(void *data)
{
    char *file_name = clipboard_Data(NULL);
    remove(file_name);
    data = data;
}

int fe_copy_text_possible(fe_view v)
{
    return v && v->displaying;
}

void fe_copy_text_to_clipboard(fe_view v)
{
    clipboard_Destroy();    /* must destroy first as we are reusing the filename    */
    if (backend_doc_saver_text(CLIP_FILE, v->displaying))
    {
        /* register as the clip board - the filename is the data    */
        clipboard_CreateList(clip_list_text, clip_destroy, CLIP_FILE, CLIP_FILE_LEN, NULL);
    }
}

/* if nothing selected then use the background image    */
static void *get_selected_image(fe_view v)
{
    be_item selected;
    void *im;

    if (!v || !v->displaying)
        return NULL;

    selected = backend_find_selected(v->displaying);
    if (!selected)
    {
        backend_item_info(v->displaying, NULL, NULL, NULL, &im);
        return im;
    }

    frontend_complain(backend_item_info(v->displaying, selected, NULL, NULL, &im));
    return im;
}

int fe_copy_image_possible(fe_view v)
{
    int flags;
    if (!v || !v->displaying)
        return FALSE;

    backend_doc_info(v->displaying, &flags, NULL, NULL, NULL);
    return (flags & be_doc_info_HAS_BG) != 0 || get_selected_image(v) != NULL;
}

void fe_copy_image_to_clipboard(fe_view v)
{
    void *im;

    im = get_selected_image(v);
    if (im)
    {
        clipboard_Destroy();    /* must destroy first as we are reusing the filename    */

        if (backend_image_saver_sprite(CLIP_FILE, im))
        {
            /* register as the clip board - the filename is the data    */
            clipboard_CreateList(clip_list_sprite, clip_destroy, CLIP_FILE, CLIP_FILE_LEN, NULL);
        }
    }
}

/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */

int fe_hotlist_add_possible(fe_view v)
{
    return v && v->displaying && v->browser_mode == fe_browser_mode_WEB;
}

os_error *fe_hotlist_add(fe_view v)
{
    char *url, *title;
    os_error *e;

    if (!fe_hotlist_add_possible(v))
        return NULL;

    e = backend_doc_info(v->displaying, NULL, NULL, &url, &title);
    if (!e)
    {
/* 	tb_status_set_message(status_type_ERROR, "Adding page to favorites list"); */
        e = hotlist_add(url, title);
/* 	tb_status_set_message(status_type_ERROR, 0); */
    }
    return e;
}

/*
 * If we are on the hotlist page then this wants to remove the selected item
 * else remove this page.
 */

os_error *fe_hotlist_remove(fe_view v)
{
    os_error *e;

    if (!v || !v->displaying)
        return NULL;

    if (v->browser_mode == fe_browser_mode_HOTLIST)
    {
        char *selected_url;
        be_item select = backend_find_selected(v->displaying);
        e = backend_item_info(v->displaying, select, NULL, &selected_url, NULL);
        if (!e && selected_url)
        {
            e = hotlist_remove(selected_url);
            if (!e) e = fe_hotlist_open(v);
        }
    }
    else
    {
        char *this_url;
        e = backend_doc_info(v->displaying, NULL, NULL, &this_url, NULL);
        if (!e) e = hotlist_remove(this_url);
    }
    return e;
}

/* ------------------------------------------------------------------------------------------- */

static os_error *fe_status_state(fe_view v, int state)
{
    BOOL is_open = tb_is_status_showing();
    BOOL new_state_open = state == -1 ? !is_open : state;

    if (new_state_open != is_open)
    {
        wimp_wstate state;
	int movement = 0;
	
        /* reopen window tiled with status window   */
        wimp_get_wind_state(v->w, &state);
        if (new_state_open)
        {
	    if (config_display_control_top)
	    {
		v->margin.y1 = margin_box.y1 - (tb_status_height() + STATUS_TOP_MARGIN);
		movement = tb_status_height() + STATUS_TOP_MARGIN;
	    }
	    else
	    {
		v->margin.y0 = margin_box.y0 + tb_status_height() + STATUS_TOP_MARGIN;
	    }
	}
        else
        {
	    if (config_display_control_top)
	    {
		v->margin.y1 = margin_box.y1;
		movement = -tb_status_height() - STATUS_TOP_MARGIN;
		if (state.o.y > -margin_box.y1)
		    movement = state.o.y + margin_box.y1;
	    }
	    else
	    {
		v->margin.y0 = margin_box.y0;
	    }
	}
        wimp_open_wind(&state.o);

        /* cause a reformat */
        if (v->displaying)
        {
            frontend_view_set_dimensions(v, 0, 0);
            backend_reset_width(v->displaying, 0);
        }

        if (new_state_open)
        {
            tb_status_update_fades(v);
            tb_status_show(FALSE);
        }
        else
            tb_status_hide(FALSE);

        if (v->displaying)
	{
#if 0				/* this version ensure background is redrawn correctly */
	    if (v->children == NULL)
		fe_view_scroll_y(v, movement);
	    fe_refresh_window(v->w, NULL);
#else
	    if (v->children)
		fe_refresh_window(v->w, NULL);
	    else
		fe_view_scroll_y(v, movement);
#endif
	}
	
/*         user_status_open = new_state_open; */
    }
    return NULL;
}

int fe_status_height_top(fe_view v)
{
    if (v->parent != NULL)
        return 0;
    if (!use_toolbox)
        return 0;
    return tb_is_status_showing() ? tb_status_height() : 0;
}

os_error *fe_status_toggle(fe_view v)
{
    os_error *e = NULL;

    v = fe_find_top(v);

    user_status_open = !user_status_open;

    if (use_toolbox)
        e = fe_status_state(v, user_status_open);
    else
        statuswin_toggle(v);

    return e;
}

int fe_status_open(fe_view v)
{
    return v && user_status_open;
}

void fe_status_clear_fetch_only(void)
{
    if (use_toolbox)
    {
	/* Hide toolbar if only up temporarily */
        tb_status_hide(TRUE);

	STBDBG(( "stbfe: finished lights\n"));
    
        tb_status_set_lights(light_state_FINISHED);

	/* reset 'globe' state */
	tb_status_rotate_reset();
    }
    else
    {
        statuswin_clear(TRUE);
    }
}

void fe_status_open_fetch_only(fe_view v)
{
    if (use_toolbox)
    {
        tb_status_show(TRUE);
	STBDBG(( "stbfe: connecting lights\n"));
    
        tb_status_set_lights(light_state_CONNECTING);
    }
    else
    {
        statuswin_open(v, TRUE);
    }
}

wimp_w fe_status_window_handle(void)
{
    return !use_toolbox ? statuswin_w : tb_is_status_showing() ? tb_status_w() : -1;
}

static void fe_status_mode(fe_view v, int mode, int override)
{
    v->browser_mode = mode;

    if (use_toolbox)
    {
	if (v->parent == NULL)
	{
	    if (override != -1)
	    {
		fe_status_state(fe_find_top(v), override);
	    }
	    else switch (mode)
	    {
		/* if desktop mode then disable toolbar */
	    case fe_browser_mode_DESKTOP:
/* 	    case fe_browser_mode_DBOX: leave it on for RCA use */
	    case fe_browser_mode_APP:
		fe_status_state(fe_find_top(v), 0);
		break;

		/* otherwise leave it in previous state */
	    default:
		fe_status_state(fe_find_top(v), user_status_open);
		break;
	    }
	}
    }
    else
    {
        if (mode != fe_browser_mode_WEB)
            statuswin_clear(FALSE);
    }
}

os_error *fe_status_info_level(fe_view v, int level)
{
    if (use_toolbox)
    {
    }
    else
    {
        return statuswin_info_level(v, level);
    }
    return NULL;
}

os_error *fe_status_unstack(fe_view v)
{
    tb_status_unstack();
    return NULL;
    NOT_USED(v);
}

/*
 * bar is from 0 to however many toolbars there are
 */

os_error *fe_status_open_toolbar(fe_view v, int bar)
{
    tb_status_new(v, bar);
    tb_status_update_fades(v);
    return NULL;
}

/* ------------------------------------------------------------------------------------------- */

void fe_scroll_changed(fe_view v, int x, int y)
{
    if (use_toolbox)
    {
    }
    else
    {
        statuswin_refresh_slider(v);
    }

    if (pointer_mode == pointermode_OFF && v && v->displaying && v->current_link)
    {
        wimp_box box, bb;
        int dir;

        if (backend_doc_item_bbox(v->displaying, v->current_link, &box) == NULL)
        {
            frontend_view_bounds(v, &bb);

            dir = box.y1 < bb.y0 ? be_link_BACK : 0;
            v->current_link = backend_highlight_link(v->displaying, v->current_link, dir | be_link_VISIBLE | be_link_INCLUDE_CURRENT);
            fe_caretise(v);
        }
    }
}

/* ------------------------------------------------------------------------------------------- */

int fe_key_lookup(int chcode, const key_list *keys)
{
    const key_list *d;
    for (d = keys; d->key; d++)
        if (d->key == chcode)
        {
            if (d->flags & key_list_CLICK)
                fe_click_sound();

            if (d->flags & key_list_REPEAT)
            {
                int key;
                while (akbd_pollkey(&key))
                    if (key != chcode)
                    {
                        fe_pending_key = key;
                        break;
                    }
            }
            else
                _swix(OS_Byte, _IN(0), 0x78);

            return d->event;
        }
    return -1;
}

/* ------------------------------------------------------------------------------------------- */

void fe_menu_event_handler(int event)
{
    stbmenu_event_handler(event);
}

/* ------------------------------------------------------------------------------------------- */

BOOL fe_item_screen_box(fe_view v, be_item ti, wimp_box *box)
{
    coords_cvtstr cvt;

    if (!v || !ti || !v->displaying)
        return FALSE;

    backend_doc_item_bbox(v->displaying, ti, box);

    cvt = fe_get_cvt(v);
    coords_box_toscreen(box, &cvt);
    return TRUE;
}

void fe_fake_click(fe_view v, int x, int y)
{
    coords_cvtstr cvt;
    coords_pointstr p;

    if (!v || !v->displaying)
        return;

    cvt = fe_get_cvt(v);
    p.x = x;
    p.y = y;
    coords_point_toworkarea(&p, &cvt);

    last_click_x = p.x;
    last_click_y = p.y;
    last_click_view = v;

    backend_doc_click(v->displaying, p.x, p.y, wimp_BLEFT);
}

coords_cvtstr fe_get_cvt(fe_view v)
{
    coords_cvtstr cvt;
    if (v->w)
    {
        wimp_wstate state;
        if (frontend_complain(wimp_get_wind_state(v->w, &state)) != NULL)
	{
            STBDBG(( "win %x v %p\n", v->w, v));
	}
        return *(coords_cvtstr *)&state.o.box;
    }

    cvt.box = v->box;
    cvt.scx = cvt.scy = 0;
    return cvt;
}

/* ------------------------------------------------------------------------------------------- */
/* Idle handling - status stuff */
/* ------------------------------------------------------------------------------------------- */

static os_error *get_item_info(fe_view v, const wimp_mousestr *mp, int *flags, char **link, be_item *ti_out)
{
    coords_pointstr p;
    coords_cvtstr cvt;
    be_item ti;
    os_error *e;

    p = *(coords_pointstr *)&mp->x;
    cvt = fe_get_cvt(v);
    coords_point_toworkarea(&p, &cvt);

    e = backend_doc_locate_item(v->displaying, &p.x, &p.y, &ti);
    if (!e) e = backend_item_pos_info(v->displaying, ti, &p.x, &p.y, flags, link, NULL, NULL);
    if (!e && ti_out)
	*ti_out = ti;
    
    return e;
}

/* Update status bar with current page info */

static void fe_update_page_info(fe_view v)
{
    char *url;
    char *title;

    if (!use_toolbox)
        return;

    if (!v || !v->displaying)
    {
/*
	tb_status_set_message(status_type_URL, NULL);
	tb_status_set_message(status_type_TITLE, NULL);
 */
        return;
    }

    backend_doc_info(v->displaying, NULL, NULL, &url, NULL);
    do
    {
        backend_doc_info(v->displaying, NULL, NULL, NULL, &title);
        if (!title)
            v = v->parent;
    }
    while (v && !title);

    tb_status_set_message(status_type_HELP, NULL);
    tb_status_set_message(status_type_URL, strncasecomp(url, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) == 0 ? NULL : url);
    tb_status_set_message(status_type_TITLE, title);
}

static void fe_update_link(fe_view v, int flags, const char *url)
{
    if (use_toolbox)
    {
#if 1
        if (flags & be_item_info_ACTION)
	{
            tb_status_set_message(status_type_HELP, flags & be_item_info_SECURE ? msgs_lookup("submits") : msgs_lookup("submiti"));
	}
        else
	{
/*          tb_status_set_message(status_type_HELP, NULL); */
            tb_status_set_message(status_type_LINK, url);
	}
#else
        char buf[256];
        int type;
        char *msg;

        if (url && url[0])
        {
            strcpy(buf, msgs_lookup("lnk"));
            strlencat(buf, (char *)url, sizeof(buf));
            type = status_type_LINK;
            msg = buf;
        }
        else if (v->displaying)
        {
            if (akbd_pollsh())
            {
                char *url;
                backend_doc_info(v->displaying, NULL, NULL, &url, NULL);

                strcpy(buf, msgs_lookup("url"));
                strlencat(buf, url, sizeof(buf));
                type = status_type_URL;
                msg = url;
            }
            else
            {
                char *title;
                do
                {
                    backend_doc_info(v->displaying, NULL, NULL, NULL, &title);
                    if (!title)
                        v = v->parent;
                }
                while (v && !title);

                strcpy(buf, msgs_lookup("ttl"));
                strlencat(buf, title ? title : msgs_lookup("none"), sizeof(buf));
                type = status_type_TITLE;
                msg = title ? title : msgs_lookup("none");
            }
        }

        tb_status_set_message(type, msg);
#endif
    }
    else
    {
        int type = fe_object_type_NONE;
        if (flags & (be_item_info_USEMAP | be_item_info_ISMAP))
            type = fe_object_type_MAP;
        else if (flags & (be_item_info_BUTTON | be_item_info_ACTION | be_item_info_INPUT | be_item_info_MENU))
            type = fe_object_type_FORM;
        else if (flags & be_item_info_LINK)
            type = fe_object_type_LINK;

        if (v->current_object_type != type)
        {
            v->current_object_type = type;
            statuswin_update_object_type(v, TRUE);
        }
    }
}

static fe_view find_view(wimp_w w)
{
    fe_view v;

    v = fe_find_window(dbox_view, w);
    if (!v)
	v = fe_find_window(main_view, w);

    return v;
}

static int check_edge_proximity(int pos, int left, int right, int threshold)
{
    int dleft = pos - left;
    int dright = right - pos;
    int r = 0;

    if (dleft < dright)
    {
	if (dleft < threshold)
	    r = -1;
    }
    else
    {
	if (dright < threshold)
	    r = +1;
    }
    return r;
}

#if 0
static void dump_views(fe_view top, int indent)
{
    fe_view v;
    for (v = top; v; v = v->next)
    {
        STBDBG(( "%*s v %p w %x\n", indent, "", v, v->w));
        if (v->children)
            dump_views(v->children, indent+2);
    }
}
#endif

static void fe_idle_handler(void)
{
    wimp_mousestr m;
    BOOL pointer_moved, highlight_moved;
    fe_view v, v_over;

    int flags;
    char *link;

    frontend_fatal_error(wimp_get_point_info(&m));
    pointer_moved = m.x != pointer_last_pos.x || m.y != pointer_last_pos.y;

    /* update last pointer position */
    pointer_last_pos = m;

#if 0
    if (pointer_moved)
    {
	STBDBG(( "p %d,%d w %x v %p main %p\n", m.x, m.y, m.w, v, main_view));
	dump_views(main_view, 2);
    }
#endif
    /* are we dragging the page around? */
    if (dragging_view)
    {
        fe_drag(&m);
        return;
    }

    /* are we resizing a frame? */
    if (resizing_view)
    {
        fe_resize(&m);
        return;
    }

    v_over = find_view(m.w);

    flags = 0;
    link = NULL;

    /* if map mode then move pointer and update links   */
    if (fe_map_view() != NULL)
    {
        /* in map mode then update the link (if usemap) */
        if (v_over)
        {
	    v = v_over;
            get_item_info(v, &m, &flags, &link, NULL);
            if (pointer_moved)
                fe_update_link(v, flags, link);
        }

        /* and check for the pointer moving */
        fe_map_check_pointer_move(&m);
        return;
    }

    /* if pointer not moved and menu up then special menu help code */
    if (!pointer_moved && use_toolbox && tb_menu_showing())
    {
	tb_menu_help();
	return;
    }

    /* if pointer is off (IR mode) update links from highlight  */
    if (pointer_mode == pointermode_OFF)
    {
	fe_view v = fe_selected_view();

	if (v)
	{
	    highlight_moved = v->current_link != highlight_last_link;
	    highlight_last_link = v->current_link;

	    if (v->current_link)
		frontend_complain(backend_item_info(v->displaying, v->current_link, &flags, &link, NULL));

	    if (highlight_moved)
		fe_update_link(v, flags, link);
	}

	/* check if the pointer has been moved and we need to go to pointer mode    */
	if (pointer_moved)
	{
	    STBDBG(( "idle: pointer moved\n"));
	    fe_pointer_mode_update(pointermode_ON);
	}
        return;
    }

    /* if not over a view, or over a control icon then get help message and exit */
    if (!v_over || m.i < -1)
    {
        /* send a wimp message to ourselves so that the toolbox can dig out the help message.   */
        if (pointer_moved && use_toolbox && tb_is_status_showing())
        {
            if (!tb_status_check_message(&m))
            {
                wimp_msgstr msg;
                msg.hdr.size = sizeof(wimp_msghdr) + sizeof(wimp_msghelprequest);
                msg.hdr.your_ref = 0;
                msg.hdr.action = wimp_MHELPREQUEST;
                msg.data.helprequest.m = m;
                wimp_sendwmessage(wimp_ESENDWANTACK, &msg, m.w, m.i);
            }
        }

        if (!dragging_view && !resizing_view)
            fe_set_pointer(0);

	if (use_toolbox)
	    tb_status_check_pointer(&m);

	return;
    }

    v = v_over;
    highlight_moved = v->current_link != highlight_last_link;
    highlight_last_link = v->current_link;

    /* if old mode then pulse a bit */
    if (!use_toolbox)
    {
        fe_view vv = fe_find_top(v);
        if (vv && vv->fetch_document != 256)
            statuswin_update_fetch_status(vv, vv->fetch_status);
    }

    /* if we are a frameset then select resizable pointer   */
    if (v->children)
    {
        int type = fe_check_resize(v, m.x, m.y, NULL, NULL, NULL);

        switch (type)
        {
            case be_resize_WIDTH:
                fe_set_pointer(fe_pointer_RESIZE_WIDTH);
                break;
            case be_resize_HEIGHT:
                fe_set_pointer(fe_pointer_RESIZE_HEIGHT);
                break;
            case be_resize_NONE:
                fe_set_pointer(0);
                break;
        }

        if (use_toolbox && type != be_resize_NONE)
            tb_status_set_message(status_type_HELP, msgs_lookup("hrsz1"));

        return;
    }

    /* check for auto-scrolling */
    if (v->w)
    {
	static int autoscroll_xedge = 0, autoscroll_yedge = 0, autoscroll_time = 0;
	wimp_wstate state;
	int xedge, yedge;
	wimp_box box;

	wimp_get_wind_state(v->w, &state);

	box = state.o.box;
	adjust_box_for_toolbar(&box);

	xedge = check_edge_proximity(m.x, state.o.box.x0, state.o.box.x1, AUTOSCROLL_EDGE_THRESHOLD);
	yedge = check_edge_proximity(m.y, box.y0, box.y1, AUTOSCROLL_EDGE_THRESHOLD);

	if (xedge || yedge)
	{
	    int now = alarm_timenow();
	    if (xedge == autoscroll_xedge && yedge == autoscroll_yedge)
	    {
		if (now - autoscroll_time >= AUTOSCROLL_DELAY)
		    fe_scroll_request(v, &state.o, xedge, yedge);
	    }
	    else
	    {
		autoscroll_xedge = xedge;
		autoscroll_yedge = yedge;
		autoscroll_time = now;
	    }
	}
	else
	{
	    autoscroll_xedge = autoscroll_yedge = 0;
	}
    }
    
    /* otherwise update link from current position  */
    {
	be_item ti = NULL;
        get_item_info(v, &m, &flags, &link, &ti);
        if (pointer_moved)
            fe_update_link(v, flags, link);

        /* don't want the pointer changing if on a blank section of a usemap    */
        if ((flags & be_item_info_USEMAP) && (!link || !*link))
            flags &= ~(be_item_info_USEMAP | be_item_info_ISMAP | be_item_info_LINK);

        /* also update pointer shape    */
        fe_set_pointer(flags);

	/* try dragging highlight */
	if (!akbd_pollctl() && pointer_moved && v->displaying && ti != v->current_link)
	{
	    if (ti)
		backend_highlight_link(v->displaying, ti, be_link_ONLY_CURRENT | be_link_CLEAR_REST);
	    else
		backend_clear_selected(v->displaying);

	    v->current_link = ti;
	}
    }
}

/* ------------------------------------------------------------------------------------------- */
/* dragging */
/* ------------------------------------------------------------------------------------------- */

/* these used by dragging and resizing  */
static int dragging_last_x, dragging_last_y;

/* this used by dragging only   */
fe_view dragging_view = NULL;

static void fe_dragging_start(fe_view v, const wimp_mousestr *m)
{
    wimp_dragstr drag;
    wimp_box extent;
    coords_cvtstr cvt;
    wimp_mousestr mm;

    /* check if we are still over the window we are ment to be dragging and the button is still held down */
    wimp_get_point_info(&mm);
    if (mm.w != m->w || mm.bbits == 0)
        return;

    cvt = fe_get_cvt(v);

    extent.x1 = v->doc_width - cvt.scx + cvt.box.x0 - v->margin.x1;
    extent.x0 =              - cvt.scx + cvt.box.x0 - v->margin.x0;
    extent.y0 = v->doc_height - cvt.scy + cvt.box.y1 - v->margin.y0;
    extent.y1 =               - cvt.scy + cvt.box.y1 - v->margin.y1;

    if (extent.x1 < cvt.box.x1)
        extent.x1 = cvt.box.x1;
    if (extent.x0 > cvt.box.x0)
        extent.x0 = cvt.box.x0;
    if (extent.y1 < cvt.box.y1)
        extent.y1 = cvt.box.y1;
    if (extent.y0 > cvt.box.y0)
        extent.y0 = cvt.box.y0;

    drag.window = v->w;
    drag.type = wimp_USER_HIDDEN;
    drag.parent.x0 = m->x - (extent.x1 - cvt.box.x1);
    drag.parent.x1 = m->x - (extent.x0 - cvt.box.x0);
    drag.parent.y0 = m->y - (extent.y1 - cvt.box.y1);
    drag.parent.y1 = m->y - (extent.y0 - cvt.box.y0);

    STBDBG(( "dragging: start box %d,%d %d,%d\n", drag.parent.x0, drag.parent.y0, drag.parent.x1, drag.parent.y1));

    if (frontend_complain(wimp_drag_box(&drag)) == NULL)
    {
        dragging_view = v;
        dragging_last_x = m->x;
        dragging_last_y = m->y;

        STBDBG(( "dragging: started at %d,%d\n", m->x, m->y));

        fe_set_pointer(fe_pointer_DRAG);
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, msgs_lookup("hscrl"));
        fast_poll++;
    }
}

static void fe_drag(const wimp_mousestr *m)
{
    if (m->bbits == 0)
    {
        dragging_view = NULL;
        fe_set_pointer(0);
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, NULL);
        fast_poll--;

        STBDBG(( "dragging: buttons released\n"));

    }
    else if (m->x != dragging_last_x || m->y != dragging_last_y)
    {
        wimp_wstate state;

        STBDBG(( "dragging: view %p to %d,%d\n", dragging_view, m->x, m->y));

        if (frontend_complain(wimp_get_wind_state(dragging_view->w, &state)) == NULL)
        {
            state.o.x -= m->x - dragging_last_x;
            state.o.y -= m->y - dragging_last_y;
            frontend_complain(wimp_open_wind(&state.o));

            dragging_last_x = m->x;
            dragging_last_y = m->y;
        }
        else
        {
            STBDBG(( "dragging: cancelled due to error\n"));

            dragging_view = NULL;
            fe_set_pointer(0);
	    if (use_toolbox)
		tb_status_set_message(status_type_HELP, NULL);
            fast_poll--;
        }
    }
}

/* ------------------------------------------------------------------------------------------- */

fe_view resizing_view = NULL;

static void fe_resizing_start(fe_view v, const wimp_mousestr *m, const wimp_box *bounds, int type)
{
    wimp_dragstr drag;

    drag.window = fe_find_top(v)->w;    /* we aleays actually dragging the top window   */
    drag.type = wimp_USER_FIXED;
    drag.parent = *bounds;
    drag.box = *bounds;
    if (type == be_resize_WIDTH)
        drag.box.x0 = drag.box.x1 = m->x;
    else
        drag.box.y0 = drag.box.y1 = m->y;

    STBDBG(( "resizing: start box %d,%d %d,%d\n", drag.parent.x0, drag.parent.y0, drag.parent.x1, drag.parent.y1));

    if (frontend_complain(wimp_drag_box(&drag)) == NULL)
    {
        dragging_last_x = m->x;
        dragging_last_y = m->y;

	resizing_view = v;                  /* record the view containing the framese we are resizing   */
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, msgs_lookup("hrsz2"));
	
        STBDBG(( "resizing: started at %d,%d\n", m->x, m->y));
    }
}

static BOOL fe_resize(const wimp_mousestr *m)
{
    if (m->bbits == 0)
    {
        fe_view v = resizing_view;
        coords_cvtstr cvt = fe_get_cvt(v);

        resizing_view = NULL;	
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, NULL);

        STBDBG(( "resizing: buttons released\n"));

        backend_frame_resize(v->displaying,
            coords_x_toworkarea(m->x, &cvt),
            coords_y_toworkarea(m->y, &cvt),
            v->resize_handle);

        fe_set_pointer(0);
        return TRUE;
    }
#if 0
    else if (m->x != dragging_last_x || m->y != dragging_last_y)
    {
#if 0
        STBDBG(( "resizing: view %p to %d,%d\n", resizing_view, m->x, m->y));
#endif
        dragging_last_x = m->x;
        dragging_last_y = m->y;
    }
#endif
    return FALSE;
}

void fe_resize_abort(void)
{
    if (resizing_view)
    {
        wimp_drag_box(NULL);

        resizing_view = NULL;
        fe_set_pointer(0);
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, NULL);
    }
}

static int fe_check_resize(fe_view start, int x, int y, wimp_box *box, int *handle, fe_view *resizing_v)
{
    fe_view v;
    for (v = start; v; v = v->next)
    {
#if 0
        STBDBG(( "resize: view %p\n", v));
#endif
        if (v->displaying)
        {
            coords_cvtstr cvt = fe_get_cvt(v);

            int type = backend_frame_resize_bounds(v->displaying,
                coords_x_toworkarea(x, &cvt),
                coords_y_toworkarea(y, &cvt),
                box, handle);

            if (type != be_resize_NONE)
            {
                if (resizing_v)
                    *resizing_v = v;
                return type;
            }
        }

        if (v->children)
        {
            int type = fe_check_resize(v->children, x, y, box, handle, resizing_v);
            if (type != be_resize_NONE)
                return type;
        }
    }
    return be_resize_NONE;
}

/* ------------------------------------------------------------------------------------------- */

static int fe_mouse_handler(fe_view v, wimp_mousestr *m)
{
    STBDBG(( "click: w %x view %s\n", m->w, v && v->name ? v->name : "<none>"));

    if (!v)
        return FALSE;

/*     fe_get_wimp_caret(m->w);   */                                  /* in case we had lost it   */
    fe_pointer_mode_update(pointermode_ON);

    switch (m->bbits)
    {
        case wimp_BMID:
            if (use_toolbox &&
		v->browser_mode != fe_browser_mode_DESKTOP &&
		v->browser_mode != fe_browser_mode_DBOX && 
		v->browser_mode != fe_browser_mode_APP)
                fe_status_toggle(v);
            break;

        case wimp_BDRAGLEFT:
            if (v->displaying)
            {
                if (v->children)
                {
                    wimp_box box;
                    int type;
                    fe_view vv;
                    int resize_handle;

                    type = fe_check_resize(v, m->x, m->y, &box, &resize_handle, &vv);

                    if (type != be_resize_NONE)
                    {
                        coords_cvtstr cvt = fe_get_cvt(vv);
                        vv->resize_handle = resize_handle;
#if 0
                        STBDBG(( "stbfe: resize %d\n", type));
#endif
                        coords_box_toscreen(&box, &cvt);
                        fe_resizing_start(vv, m, &box, type);
                    }
                }
                else
                {
                    fe_dragging_start(v, m);
                }
            }
            break;

        case wimp_BLEFT:
/*         case wimp_BRIGHT: */
            if (v->displaying)
            {
                wimp_wstate state;
                wimp_get_wind_state(m->w, &state);
                coords_point_toworkarea((coords_pointstr *)&m->x, (coords_cvtstr *)&state.o.box);

                last_click_x = m->x;
                last_click_y = m->y;
                last_click_view = v;
                backend_doc_click(v->displaying, m->x, m->y, m->bbits);
            }
            break;
    }

    return TRUE;
}

/* ------------------------------------------------------------------------------------------- */

typedef struct fe_redraw_box
{
    struct fe_redraw_box *next;
    wimp_box box;
} fe_redraw_box;

static void fe_redraw_handler(fe_view v, wimp_w w)
{
    fe_redraw_box *box_list = NULL;
    wimp_redrawstr r;
    int more;
    os_error *e;

#if 0
    STBDBG(( "redraw: w %x view %s\n", w, v && v->name ? v->name : "<none>"));
#endif

    r.w = w;
    e = wimp_redraw_wind(&r, &more);

    while (more && !e)
    {
        if (v)
        {
            if (v->displaying)
            {
                fe_redraw_box *box;

                box = mm_calloc(sizeof(fe_redraw_box), 1);

                box->box = r.g;
                coords_box_toworkarea(&box->box, (coords_cvtstr *)&r.box);

                box->next = box_list;
                box_list = box;
            }
            else
            {
                int gcol;
                colourtran_setGCOL(config_colours[render_colour_BACK], 0, 0, &gcol);
                bbc_rectanglefill(r.g.x0, r.g.y0, r.g.x1 - r.g.x0, r.g.y1 - r.g.y0);
            }
        }
        else
        {
	    if (use_toolbox)
		tb_status_redraw(&r);

/*          tb_find_redraw(&r) && */
/*          tb_print_redraw(&r)) */
/*          fe_anti_twitter(&r.g); */
        }
        e = wimp_get_rectangle(&r, &more);
    }

    frontend_fatal_error(e);
    
    while (box_list)
    {
        fe_redraw_box *next = box_list->next;

        frontend_view_update(v, &box_list->box, backend_render_rectangle, v->displaying, fe_update_WONT_PLOT_ALL);

        mm_free(box_list);
        box_list = next;
    }
}


/* ------------------------------------------------------------------------------------------- */
/* Message handlers */
/* ------------------------------------------------------------------------------------------- */

static char *get_ptr(urlopen_data *u, string_value sv)
{
    return sv.offset == 0 ? 0 : sv.offset <= 236 ? (char *)u + sv.offset : sv.ptr;
}

/* ------------------------------------------------------------------------------------------- */

void fe_plugin_event_handler(int event, fe_view v)
{
    plugin_list_toggle_flags(event & 0xfff, 0, plugin_info_flag_DISABLED);

    if (use_toolbox)
	tb_menu_refresh(v);
}

/* ------------------------------------------------------------------------------------------- */

wimp_t frontend_plugin_start_task(int filetype)
{
    char buffer[32];
    wimp_t task = 0;

    sprintf(buffer, "Alias$@PlugInType_%03x", filetype);
    if (getenv(buffer))
	fe_start_task(buffer+6, &task);

    return task;
}

int frontend_plugin_handle_file_type(int filetype)
{
    char buffer[32];
    sprintf(buffer, "Alias$@PlugInType_%03x", filetype);
    if (getenv(buffer) != NULL)
    {
	plugin_info *info = plugin_list_get_info(filetype);
	return info == NULL || (info->flags & plugin_info_flag_DISABLED) == 0;
    }
    return FALSE;
}

int frontend_message_send(wimp_msgstr *mp, wimp_t dest_t)
{
    return frontend_complain(wimp_sendmessage(wimp_ESENDWANTACK, mp, dest_t)) == NULL;
}

int frontend_message_add_handler(frontend_message_handler fn, void *handle)
{
    fe_message_handler_item *item;

    if (fn == NULL)
	return 0;

    item = mm_calloc(sizeof(*item), 1);
    item->fn = fn;
    item->handle = handle;

    item->next = message_handlers;
    message_handlers = item;

    return 1;
}

int frontend_message_remove_handler(frontend_message_handler fn, void *handle)
{
    fe_message_handler_item *item, *prev;

    for (item = message_handlers, prev = NULL; item; prev = item, item = item->next)
    {
	if (item->fn == fn && item->handle == handle)
	{
	    if (prev)
		prev->next = item->next;
	    else
		message_handlers = item->next;
	    mm_free(item);
	    break;
	}
    }

    return 1;
}

wimp_w frontend_get_window_handle(fe_view v)
{
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;
    
    return v->w;
}

/*
 * Try all the message handlers registered until one returns TRUE
 */

static BOOL fe_send_message(wimp_eventstr *e)
{
    fe_message_handler_item *item;

    for (item = message_handlers; item; item = item->next)
	if (item->fn(e, item->handle))
	    return TRUE;

    return FALSE;
}

/* ------------------------------------------------------------------------------------------- */

static void fe_doc_bounce(wimp_msgdataopen *dataopen)
{
    STBDBG(("fe_doc_bounce(): w %x\n", dataopen->w));
    STBDBG(("fe_doc_bounce(): i %x\n", dataopen->i));
    STBDBG(("fe_doc_bounce(): x %x\n", dataopen->x));
    STBDBG(("fe_doc_bounce(): y %x\n", dataopen->y));
    STBDBG(("fe_doc_bounce(): size %x\n", dataopen->size));
    STBDBG(("fe_doc_bounce(): type %x\n", dataopen->type));
    STBDBG(("fe_doc_bounce(): name '%s'\n", dataopen->name));

    if (frontend_can_handle_file_type(dataopen->type))
    {
        char buffer[256];
        sprintf(buffer, "/%s", dataopen->name);

	file_lock(dataopen->name, TRUE);
	
        frontend_complain(fe_start_task(buffer, NULL));

	file_lock(dataopen->name, FALSE);
    }
}

#define STBWEB_RETURNED_FRAMES PROGRAM_NAME"$ReturnedFrames"

/* We have got back a URL that no appliaction wanted to use.  Try to start an application */



static void fe_url_bounce(wimp_msgstr *msg)
{
    urlopen_data *ud = (urlopen_data *)&msg->data;
    char *scheme, *netloc, *path, *params, *query, *fragment;
    char *url, *body_file, *target;

    STBDBG(("fe_url_bounce: tag %d ptr %p s '%s'\n", ud->indirect.tag, ud->indirect.url.ptr, ud->indirect.tag ? ud->url : get_ptr(ud, ud->indirect.url)));

    body_file = target = NULL;
    if (ud->indirect.tag == 0)
    {
        url = get_ptr(ud, ud->indirect.url);
        if (msg->hdr.size > 28)
        {
            body_file = get_ptr(ud, ud->indirect.body_file);
            target = get_ptr(ud, ud->indirect.target);
	    STBDBG(("fe_url_bounce: body '%s' target '%s'\n", body_file, target));
        }
    }
    else
    {
        url = ud->url;
    }

    url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);

    if (scheme == NULL)
    {
    }
    else
    {
	char buffer[262];	        /* 6 chars on the front for the 'Alias$' */
	BOOL run_cli, run_task;

	strcpy(buffer, "Alias$URLOpenCLI_");
	strcat(buffer, scheme);
	run_cli = getenv(buffer) != NULL;

	if (run_cli)
	    run_task = FALSE;
	else
	{
	    strcpy(buffer, "Alias$URLOpen_");
	    strcat(buffer, scheme);
	    run_task = getenv(buffer) != NULL;
	}
	
	/* check for an unloaded external helper    */
	if (run_cli || run_task)
	{
	    /* run external helper  */
	    strcat(buffer, " ");
	    strncat(buffer, url, sizeof(buffer) - strlen(buffer) - 1);

	    if (body_file)
	    {
		strcat(buffer, " ");
		strncat(buffer, body_file, sizeof(buffer) - strlen(buffer) - 1);
	    }

	    /* clear return var */
	    _swix(OS_SetVarVal, _INR(0,2), STBWEB_RETURNED_FRAMES, NULL, -1);

	    if (run_cli)
		frontend_complain(os_cli(buffer + 6));
	    else
		frontend_complain(fe_start_task(buffer + 6, NULL));

	    /* check for any pages to load  */
	    {
		char *s = strdup(getenv(STBWEB_RETURNED_FRAMES));

		STBDBG(("stbfe: returned_frames '%s'\n", s ? s : "<empty>"));

		if (s)
		{
		    char *ss = s;
		    do
		    {
			/* skip spaces */
			ss += strspn(ss, " ");
			if (*ss)
			{
			    char *url1, *target1;
			    BOOL finished;
			    int len = strcspn(ss, " ");

			    if (ss[len] == 0)
				finished = TRUE;
			    else
			    {
				ss[len] = 0;
				finished = FALSE;
			    }

			    target1 = extract_value(ss, "target=");
			    url1 = extract_value(ss, "url=");

			    if (finished)
				ss = NULL;
			    else
				ss += len+1;

			    if (url1 && url1[0])
				frontend_complain(frontend_open_url(url1, main_view, target1 ? target1 : target, NULL, 0));
				
			    mm_free(target1);
			    mm_free(url1);
			}
		    }
		    while (ss && *ss);
		}

		mm_free(s);
	    }
	}
#if 0
	/* check for internal helper    */
	else if (strcasecomp(scheme, "mailto") == 0 && can_do_mailto())
	{
	    frontend_complain(fe_open_mailto(path));
	}
#endif
	else
	{    /* give error  */
	    char tag[32], *s;
	    sprintf(tag, "noscheme_%s:", scheme);
	    s = msgs_lookup(tag);
	    if (s && s[0])
		fe_report_error(s);
	    else
		frontend_complain(makeerrorf(ERR_UNSUPORTED_SCHEME, scheme ? scheme : msgs_lookup("none")));
	}
    }
    
    url_free_parts(scheme, netloc, path, params, query, fragment);

    if (fe_stored_url)
    {
	rma_free(fe_stored_url);
	fe_stored_url = NULL;
    }
}

static void fe_mode_changed(void)
{
    int dx, dy;
    wimp_box old_screen, old_text;

    dx = frontend_dx;
    dy = frontend_dy;

    old_screen = screen_box;
    old_text = text_safe_box;

    /* Recalculate frontend parameters */
    feutils_init_1();
    feutils_init_2();

    STBDBG(( "modechange: old eig %d,%d new %d,%d\n", dx, dy, frontend_dx, frontend_dy));
    STBDBG(( "modechange: new size %d,%d\n", screen_box.x1, screen_box.y1));

    /* Inform backend to recache fonts and colours */
    if ((frontend_dx != dx) || (frontend_dy != dy))
	frontend_complain(webfonts_init());

    frontend_complain(backend_screen_changed(be_change_MODE));

    /* If the toolbar is open then reopen with icons in new positions */
    if (use_toolbox/*  && tb_is_status_showing() */)
    {
        tb_status_resize(screen_box.x1 - old_screen.x1, screen_box.y1 - old_screen.y1);
    }

    /* If the safe area has changed then reformat the page */
    if (text_safe_box.x0 != old_text.x0 || text_safe_box.y0 != old_text.y0 || text_safe_box.x1 != old_text.x1 || text_safe_box.y1 != old_text.y1)
    {
	fe_view v = main_view;

	/* mark the top level */
	v->pending_mode_change = TRUE;

	/* mark the children so they don't get reopened */
	do
	{
	    
	    v = fe_next_frame(v, TRUE);
	    if (v)
		v->pending_mode_change = TRUE;
	}	    
	while (v);
    }
    else
	fe_refresh_window(-1, NULL);
}

static void fe_handle_dataopen(wimp_msgstr *msg)
{
    int ftype;

    ftype = msg->data.dataload.type;
    if (ftype == FILETYPE_HTML || ftype == FILETYPE_GOPHER || ftype == FILETYPE_URL)
    {
	frontend_complain(fe_show_file(main_view, msg->data.dataload.name, FALSE));

	msg->hdr.your_ref = msg->hdr.my_ref;
	msg->hdr.action = wimp_MDATALOADOK;
	frontend_fatal_error(wimp_sendmessage(wimp_ESEND, msg, msg->hdr.task));
    }
}

static void fe_handle_datasave(wimp_msgstr *msg)
{
    fe_view v = find_view(msg->data.dataload.w);

    if (v) switch (msg->data.dataload.type)
    {
        default:
            if (!image_type_test(msg->data.dataload.type))
                break;
            /* deliberate fall-through */
        case FILETYPE_TEXT:
        case FILETYPE_HTML:
        case FILETYPE_GOPHER:
        case FILETYPE_URL:
	    msg->hdr.your_ref = msg->hdr.my_ref;
    	    msg->hdr.action = wimp_MDATASAVEOK;
    	    frontend_fatal_error(wimp_sendmessage(wimp_ESEND, msg, msg->hdr.task));
            break;
    }
}

static void fe_handle_dataload(wimp_msgstr *msg)
{
    fe_view v = find_view(msg->data.dataload.w);
    BOOL handled = FALSE;

    if (!v)
        return;

    switch (msg->data.dataload.type)
    {
        case FILETYPE_TEXT:
            if (v->displaying == NULL)
	        frontend_complain(fe_show_file(v, msg->data.dataload.name, FALSE));
	    else
	        fe_type_file(v, msg->data.dataload.name);
            break;

        case FILETYPE_HTML:
        case FILETYPE_GOPHER:
        case FILETYPE_URL:
        case FILETYPE_DIRECTORY:
        case 0x2000:
	    frontend_complain(fe_show_file(v, msg->data.dataload.name, FALSE));
	    handled = TRUE;
            break;

        default:
            if (image_type_test(msg->data.dataload.type))
            {
	        frontend_complain(fe_show_file(v, msg->data.dataload.name, FALSE));
	        handled = TRUE;
	    }
            break;
    }

    if (handled)
    {
	msg->hdr.your_ref = msg->hdr.my_ref;
	msg->hdr.action = wimp_MDATALOADOK;
	frontend_fatal_error(wimp_sendmessage(wimp_ESEND, msg, msg->hdr.task));
    }
}

static void fe__handle_openurl(wimp_msgstr *msg, char *url, char *target, char *body_file)
{
    char *curl;
    int len;

    curl = strdup(url);
    len = strlen(curl);

    while(len && isspace(curl[len-1]))
	len--;

    curl[len] = 0;

    url_canonicalise(&curl);

    frontend_complain(frontend_open_url(curl, NULL, target, body_file, 0));

    mm_free(curl);

    msg->hdr.your_ref = msg->hdr.my_ref;
    frontend_fatal_error(wimp_sendmessage(wimp_EACK, msg, msg->hdr.task));
}

static void fe_handle_openurl(wimp_msgstr *msg)
{
    urlopen_data *ud = (urlopen_data*) &msg->data;
    char *scheme;
    char *url, *body_file, *target;

    body_file = target = NULL;
    if (ud->indirect.tag == 0)
    {
        url = get_ptr(ud, ud->indirect.url);
        if (msg->hdr.size > 28)
        {
            body_file = get_ptr(ud, ud->indirect.body_file);
            target = get_ptr(ud, ud->indirect.target);
        }
    }
    else
    {
        url = ud->url;
    }

    STBDBG(( "stbfe: openurl url='%s' bfile='%s' target='%s'\n", url, strsafe(body_file), strsafe(target)));

    url_parse(url, &scheme, NULL, NULL, NULL, NULL, NULL);

    if (/* strcasecomp(scheme, "ncfrescointernal") == 0 || */
	access_is_scheme_supported(scheme))
    {
	fe__handle_openurl(msg, url, target, body_file);
    }

    mm_free(scheme);
}

/* ------------------------------------------------------------------------------------------- */

static void re_read_config(int flags)
{
    STBDBG(("stbfe: reading auth and cookie\n"));

    auth_init();

    if (config_cookie_enable)
	cookie_read_file(config_cookie_file);

    plugin_list_read_file();
    hotlist_init();

    if (file_type("<"PROGRAM_NAME"$Config>") != -1)
    {
	config_read_file_by_name("<"PROGRAM_NAME"$Config>");

	/* force all the window stuff to be re read */
	frontend_dx = frontend_dy = 0;
	fe_mode_changed();	
    }
    
    flags = flags;
}

static void fe_handle_service_message(wimp_msgstr *msg)
{
    os_regset *r = (os_regset *)&msg->data.words[0];

    STBDBG(( "stbfe: service %x r2=%d\n", r->r[1], r->r[2]));

    switch (r->r[1])
    {
#if USE_DIALLER_STATUS
    case Service_DiallerStatus:
        {
            int new_state = r->r[2];

	    STBDBG(("DiallerStatus: state=%d, count=%d\n", new_state, connection_count));
	    
	    if (new_state == dialler_CONNECTED_OUTGOING)
            {
		connection_up = 1;

                /* only reload stuff on first connection */
                if (connection_count++ == 0)
                {
		    re_read_config(0);
                }
            }
	    else if (new_state == dialler_DISCONNECTED)
	    {
		connection_up = 0;
	    }
            break;
        }
#endif

    case Service_SmartCard:
    {
	/* remove all saved lists in memory,
	 * there shouldn't be any saved files except for those in the NFS home directory
	 * which will be logged out by now.
	 */
/* 	int status = r->r[2]; */
	int delta = r->r[4];
	if (delta == -1)
	{			/* smartcard removed */
	    fe_dbox_cancel();

	    fe_history_dispose(main_view);
 	    fe_global_history_dispose();

	    cookie_dispose_all();

	    access_flush_cache();

	    auth_dispose();

	    plugin_list_dispose();
	}
	else if (delta & smartcard_INSERTED)
	{			/* smartcard inserted */
	}
	break;
    }
    }
}

/* ------------------------------------------------------------------------------------------- */
/* Main event process loop  */
/* ------------------------------------------------------------------------------------------- */

void fe_event_process(void)
{
    wimp_eventstr e;

#if 0
    if (fe_pending_url)
    {
        fe_internal_url(fe_pending_url, fe_pending_bfile);
        mm_free(fe_pending_bfile);
        mm_free(fe_pending_url);
        fe_pending_bfile = NULL;
        fe_pending_url = NULL;
    }
#endif
    if (fe_pending_key)
    {
        fe_view v = fe_selected_view();

        e.data.key.chcode = fe_pending_key;
        fe_pending_key = 0;

        if (v)
        {
            e.data.key.c.w = v->w;
            fe_key_handler(v, &e, use_toolbox, v->browser_mode);
        }
    }

    /* do the standard poll loop    */
    if (fast_poll)
    {
        frontend_fatal_error(wimp_poll((wimp_emask)(wimp_EMPTRLEAVE), &e));
    }
    else
    {
        int next = alarm_timenow() + 20;
        int next_alarm_time;

        if (alarm_next(&next_alarm_time))
        {
            if (next > next_alarm_time)
                next = next_alarm_time;
        }

        frontend_fatal_error(wimp_pollidle((wimp_emask)(wimp_EMPTRLEAVE), &e, next));
    }

    switch (e.e)
    {
        case wimp_ENULL:
            fe_idle_handler();

            /* keep the alarms running  */
            {
                int time;
                if (alarm_next(&time) && alarm_timenow() >= time)
                    alarm_callnext();
            }
            break;

        case wimp_ECLOSE:
            wimp_close_wind(e.data.o.w);
            break;

        case wimp_EOPEN:
        {
            fe_view v = find_view(e.data.o.w);
	    if (v && v->pending_mode_change)
	    {
		if (v->parent == NULL)
		{
		    v->margin = margin_box;

		    adjust_box_for_toolbar(&v->margin);

		    feutils_resize_window(&v->w, &v->margin, &screen_box, &v->x_scroll_bar, &v->y_scroll_bar, 0, 0, v->scrolling, fe_bg_colour(v));

		    if (v->displaying)
			backend_reset_width(v->displaying, 0);
		}

		v->pending_mode_change = FALSE;
		if (v->w)
		    fe_refresh_window(v->w, NULL);
	    }
	    else if (toolbar_pending_mode_change && e.data.o.w == tb_status_w())
	    {
		tb_status_hide(FALSE);
		tb_status_show(FALSE);
		
		toolbar_pending_mode_change = FALSE;
	    }
	    else
	    {
		wimp_open_wind(&e.data.o);
	    }
            break;
	}

        case wimp_EBUT:
            if (!stbmenu_check_mouse(&e.data.but.m))
                fe_mouse_handler(find_view(e.data.but.m.w), &e.data.but.m);
	    break;

        case wimp_EKEY:
        {
            fe_view v = find_view(e.data.key.c.w);

            STBDBG(( "key: view %p '%s' key %x\n", v, v && v->name ? v->name : "", e.data.key.chcode));

	    if (v || !tb_key_handler(&e.data.key.c, e.data.key.chcode))
		fe_key_handler(v ? v : main_view, &e, use_toolbox, v->browser_mode);
            break;
        }

        case wimp_EREDRAW:
            if (!statuswin_check_redraw(main_view, e.data.o.w) &&
                !stbmenu_check_redraw(e.data.o.w))
                fe_redraw_handler(find_view(e.data.o.w), e.data.o.w);
            break;

        case wimp_ELOSECARET:
        {
            fe_view v = find_view(e.data.c.w);
            if (v)
            {
                if (v->current_link)
                {
                    backend_update_link(v->displaying, v->current_link, 0);
                    v->current_link = NULL;
                }

#if 0
                if (v == selected_view)
                {
                    fe_view v_top;

                    selected_view = NULL;

                    v_top = fe_find_top(v);
                    if (v_top && v_top != v)
                        fe_refresh_window(v_top->w, NULL);
                }
#endif
            }
	    else
	    {
		tb_status_highlight(FALSE);
	    }
            break;
        }

        case wimp_EGAINCARET:
        {
            fe_view v_old, v_new;

	    v_old = fe_selected_view();
	    v_new = find_view(e.data.c.w);

	    v_old->is_selected = FALSE;
	    v_new->is_selected = TRUE;

#if 0
            fe_view v_top;

            selected_view = v;

            v_top = fe_find_top(v);
            if (v_top && v != v_top)
                fe_refresh_window(v_top->w, NULL);
#endif
            break;
        }

        case wimp_EPTRENTER:
            fe_update_page_info(find_view(e.data.c.w));

	    fe_get_wimp_caret(e.data.c.w);
            break;

        case wimp_EPTRLEAVE:
            break;

        case wimp_ESEND:
        case wimp_ESENDWANTACK:
        {
            wimp_msgstr *msg = &e.data.msg;

	    STBDBG(( "msg: %d/%x\n", e.e, msg->hdr.action));

            if (!clipboard_eventhandler(&e, NULL) &&
		!fe_send_message(&e))
                switch (msg->hdr.action)
            {
	        case wimp_MOPENURL:
	            fe_handle_openurl(&e.data.msg);
	            break;
 	        case wimp_MDATAOPEN:
		    fe_handle_dataopen(&e.data.msg);
		    break;
 	        case wimp_MDATALOAD:
		    fe_handle_dataload(&e.data.msg);
		    break;
 	        case wimp_MDATASAVE:
		    fe_handle_datasave(&e.data.msg);
		    break;
	        case wimp_MMODECHANGE:
	            fe_mode_changed();
	            break;
	        case wimp_PALETTECHANGE:
	            backend_screen_changed(be_change_PALETTE);
	            break;

                case wimp_MHELPREPLY:
                    if (use_toolbox)
                        tb_status_set_message(status_type_HELP, msg->data.helpreply.text);
                    break;

                case wimp_MSERVICE:
                    fe_handle_service_message(&e.data.msg);
                    break;

	    case wimp_MNCFRESCO:
		switch (msg->data.words[0])
		{
		case ncfresco_reason_LOAD_DATA:
		    usrtrc("userinfo:\n");
		    re_read_config(msg->data.words[1]);
		    break;

		case ncfresco_reason_DIE:
		    usrtrc("die:\n");
                    exit(0);
		    break;
		}
		break;
		
	    case wimp_MCLOSEDOWN:
		usrtrc("closedown:\n");
		exit(0);
		break;
            }
            break;
        }

        case wimp_EACK:

	    if (!clipboard_eventhandler(&e, NULL) &&
		!fe_send_message(&e))
	        switch (e.data.msg.hdr.action)
	    {
	        case wimp_MOPENURL:
	            fe_url_bounce(&e.data.msg);
	            break;

                case wimp_MDATAOPEN:
                    fe_doc_bounce(&e.data.msg.data.dataopen);
                    break;

	    case wimp_MHELPREQUEST:
		if (use_toolbox)
		    tb_status_set_message(status_type_HELP, NULL);
		break;
	    }
	    break;

        case 0x200: /* toolbox events   */
	    if (use_toolbox)
	    {
		fe_view selected_view = fe_selected_view();
		tb_events((int *)&e.data, selected_view ? selected_view : main_view);
	    }
            break;
    }
}

/* ------------------------------------------------------------------------------------------- */
/* Initialisation and finalisation  */
/* ------------------------------------------------------------------------------------------- */

/* Some bits nicked from wimpt to initialise the system */

typedef void SignalHandler(int);
static SignalHandler *oldhandler;

static void escape_handler(int sig)
{
   sig = sig; /* avoid compiler warning */
   (void) signal(SIGINT, &escape_handler);
}

static void handler(int signal)
{
   os_error er;
   er.errnum = 0;
   sprintf(
       er.errmess,
       msgs_lookup("fatal1:%s has suffered a fatal internal error (type=%i) and must exit immediately"),
       program_title,
       signal);
/*   wimp_reporterror(&er, (wimp_errflags)0, program_title);    */
   fe_report_error(er.errmess);
}

static void signal_init(void)
{
    oldhandler = signal(SIGABRT, &handler);
    oldhandler = signal(SIGFPE, &handler);
    oldhandler = signal(SIGILL, &handler);
    oldhandler = signal(SIGINT, &escape_handler);
    oldhandler = signal(SIGSEGV, &handler);
    oldhandler = signal(SIGTERM, &handler);
}

static int my_wimp_initialise(int *message_list)
{
    os_regset r;
    r.r[0] = 350;
/*     r.r[1] = *(int *) "TASK"; */
    r.r[1] = 0x4B534154;
    r.r[2] = (int) program_title;
    r.r[3] = (int) message_list;

    frontend_fatal_error(os_swix(Wimp_Initialise,&r));

    return r.r[1];
}

/* ------------------------------------------------------------------------------------------- */

static void fe_tidyup(void)
{
    STBDBG(( "\n*** Exit function called ***\n\n"));

    MemCheck_SetChecking(0,0); 
    
#if USE_DIALLER_STATUS
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle);
#endif
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_SmartCard, task_handle);

    if (main_view)
    {
	fe_dispose_view(main_view);
	main_view = 0;
    }
    
    if (dbox_view)
    {
	fe_dispose_view(dbox_view);
	dbox_view = 0;
    }

    if (use_toolbox)
	tb_cleanup();
    
    clipboard_Destroy();

    webfonts_tidyup();
    image_tidyup();
    /* Image needs to come before access in the tidyup because image used access */
    access_tidyup();

    plugin_list_write_file();
    plugin_list_dispose();

    fe_global_history_dispose();
    cookie_dispose_all();

    auth_write_realms(config_auth_file, config_auth_file_crypt ? auth_passwd_UUCODE : auth_passwd_PLAIN);
    auth_dispose();

    hotlist_shutdown();
    
    if (fe_stored_url)
	rma_free(fe_stored_url);

    config_tidyup();		/* do this last or else we may destory sometging useful! */

    pointer_reset_shape();
    _swix(Hourglass_Smash, 0);

    /* Before we go, look for memory leeks */
    mm_dump();
}

static int message_codes[] =
{
    MESSAGE_PLUGIN_OPEN,
    MESSAGE_PLUGIN_OPENING,
    MESSAGE_PLUGIN_CLOSE,
    MESSAGE_PLUGIN_CLOSED,
    MESSAGE_PLUGIN_RESHAPE,
    MESSAGE_PLUGIN_RESHAPE_REQUEST,
    MESSAGE_PLUGIN_UNLOCK,
    MESSAGE_PLUGIN_FOCUS,
    MESSAGE_PLUGIN_STREAM_NEW,
    MESSAGE_PLUGIN_STREAM_DESTROY,
    MESSAGE_PLUGIN_STREAM_WRITE,
    MESSAGE_PLUGIN_STREAM_WRITTEN,
    MESSAGE_PLUGIN_STREAM_AS_FILE,
    MESSAGE_PLUGIN_URL_ACCESS,
    MESSAGE_PLUGIN_URL_NOTIFY,
    MESSAGE_PLUGIN_STATUS,
    
    wimp_MDATAOPEN,
    wimp_MDATALOAD,
    wimp_MDATALOADOK,
    wimp_MDATASAVE,
    wimp_MDATASAVEOK,
    Message_ClaimEntity,
    Message_DataRequest,
    wimp_MHELPREPLY,

    wimp_MOPENURL,
    wimp_MNCFRESCO,
    
    wimp_MMODECHANGE,
    wimp_PALETTECHANGE,
    wimp_MSERVICE,
    wimp_MCLOSEDOWN
};

static BOOL fe_initialise(void)
{
    os_error *e;

#if HEAPCHECK
    void __heap_checking_on_all_allocates(void);
    void __heap_checking_on_all_deallocates(void);

    __heap_checking_on_all_allocates();
    __heap_checking_on_all_deallocates();
#endif

    /* Initialise the WIMP stuff */
    visdelay_init();
    visdelay_begin();

    signal_init();

    dbginit();

    if (use_toolbox)
    {
        task_handle = tb_init(message_codes);

        tb_res_init(program_name);
        tb_resspr_init();
        tb_msgs_init();
    }
    else
    {
        task_handle = my_wimp_initialise(message_codes);

        res_init(program_name);
        resspr_init();
        msgs_init();
    }

#if !MEMLIB
    /* Now bring up the flex system->.. */
    flex_init(program_name);
    heap_init(program_name);
#else
    MemFlex_Initialise2(program_name);
    MemHeap_Initialise(program_name);
#endif
    atexit(&fe_tidyup);

    STBDBG(( "task handle: %x\n", task_handle));

    /* Init our configuration */
    config_init();
    fe_font_size_init();

    feutils_init_1();
    feutils_init_2();

    if (use_toolbox)
        tb_status_init();

    sgml_support_initialise();
    parse_frames(config_display_frames);

    /* Init the ANTWeb bits */
    frontend_fatal_error(access_init(0));
    image_init();
    auth_init();
    plugin_list_read_file();
    hotlist_init();
    
    /* Check the licence */
    if (frontend_complain(licence_init()) != NULL)
    {
	exit(0);
    }

#ifdef IMAGE_COOLING
    image_get_cooler_table();
#endif

#if NEW_WEBIMAGE
    imginit("<"PROGRAM_NAME"$Dir>.DLLs.");
#endif

    /* ... and imidiately allow budging */
    /* with ROM Risc OSLib budging is on by default */
/*    _kernel_register_slotextend(flex_budge);  */


    fe_stored_url = NULL;
    frontend_complain(webfonts_init());

    statuswin_sprite_init();

    {
        fe_frame_info info;
        info.name = TARGET_VERY_TOP;
        info.noresize = TRUE;
        info.scrolling = fe_scrolling_NO;
        *(wimp_box *)&info.margin = margin_box;
        frontend_fatal_error(fe_new_view(NULL, &screen_box, &info, &main_view));
        main_view->status_open = TRUE;
    }

    fe_get_wimp_caret(main_view->w);

#if USE_DIALLER_STATUS
    e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle);
    if (!e)
#endif
	e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_SmartCard, task_handle);
#if DEBUG
    if (e)
    {
        STBDBG(( "taskmodule: %x %s\n", e->errnum, e->errmess));
    }
#endif

    /* initialise pointer state, based on new SWI   */
#if 0
    {
        int pointer = 1;
        _swix(OS_Pointer, _IN(0) | _OUT(0), 2, &pointer);
        fe_pointer_mode_update(pointer ? pointermode_ON : pointermode_OFF);
    }
#endif
    frontend_fatal_error(wimp_get_point_info(&pointer_last_pos));

    visdelay_end();

    return 1;
}


/* ------------------------------------------------------------------------------------------- */

#if STBWEB_ROM
int __root_stack_size = 32*1024;
extern int disable_stack_extension;
#endif

int main(int argc, char **argv)
{
    int init_ok;

#if STBWEB_ROM
    disable_stack_extension = 1;
#endif

    HierProf_ProfileAllFunctions();
    
    setlocale(LC_ALL, "");
    setbuf(stderr, NULL);   /* no caching   */

    MemCheck_Init();
    MemCheck_RegisterArgs(argc, argv);
    MemCheck_InterceptSCLStringFunctions();
    MemCheck_SetAutoOutputBlocksInfo(FALSE);
    MemCheck_RegisterMiscBlock(0, 0); /* stop memcpy(x, 0, 0) from giving an error */
    MemCheck_SetStoreMallocFunctions(TRUE);
    MemCheck_SetChecking(0,0); 
    
    init_usrtrc();

    progname = argv[0];
    argv++;
    argc--;

    if (argc > 0 && (strncmp(argv[0], "-x", 2)==0) )
    {
	debug_level = atoi(argv[0] + 2);
	argc--;
	argv++;
    }

    if (argc > 0 && strcmp(argv[0], "-t") == 0)
    {
        use_toolbox = TRUE;
	argc--;
	argv++;
    }

    init_ok = fe_initialise();

    if (init_ok)
    {
        char *welcome_url = NULL;

	while (argc > 0)
	{
            if (welcome_url == NULL)
            {
                /* extract the first command line argument as welcome page  */
                if (strcasecomp(argv[0], "-URL") == 0)
                {
    		    argv++;
    		    argc--;
    		    welcome_url = strdup(argv[0]);
    		}
    		else
                    frontend_complain(fe_file_to_url(argv[0], &welcome_url));
            }

	    argv++;
	    argc--;
	}

        if (welcome_url)
        {
            frontend_complain(frontend_open_url(welcome_url, main_view, NULL, NULL, 0));
            mm_free(welcome_url);
            welcome_url = NULL;
        }
        else
        {
            fe_home(main_view);
        }

	/* The main event loop */
	fe_pointer_mode_update(pointermode_OFF);
	while (TRUE)
	    fe_event_process();
    }

    return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* Hack functions   */
/* ------------------------------------------------------------------------------------------- */

/* to cope with running possibly under the toolbox we have these functions here */

#undef resspr_area
extern sprite_area *resspr_area(void);

#undef msgs_lookup
extern char *msgs_lookup(char *tag);

sprite_area *fe_resspr_area(void)
{
    return use_toolbox ? (sprite_area *)tb_resspr_area() : resspr_area();
}

char *fe_msgs_lookup(char *tag)
{
    return use_toolbox ? tb_msgs_lookup(tag) : msgs_lookup(tag);
}

/* ----------------------------------------------------------------------------------------------------- */

/*
STBDBG(( "scheme = %s\n", scheme ? scheme : ""));
STBDBG(( "netloc = %s\n", netloc ? netloc : ""));
STBDBG(( "path = %s\n", path ? path : ""));
STBDBG(( "params = %s\n", params ? params : ""));
STBDBG(( "query = %s\n", query ? query : ""));
STBDBG(( "fragment = %s\n", fragment ? fragment : ""));
 */
