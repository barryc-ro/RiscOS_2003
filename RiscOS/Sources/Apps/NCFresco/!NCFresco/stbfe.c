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
#include "unwind.h"

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
#include "print.h"

#ifdef STBWEB_BUILD
#include "http.h"
#else
#include "../http/httppub.h"
#endif

#include "cookie.h"
#include "gbf.h"

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
#include "frameutils.h"
#include "configfile.h"
#include "antweb.h"

#if MEMLIB
#include "memheap.h"
#endif

#ifdef HierProf_PROFILE
#include "hierprof/HierProf.h"
#endif

#ifdef Trace_TRACE
#include "Trace/Trace.h"
#include "Trace/Stacker.h"
#endif

#ifndef NEW_WEBIMAGE
#define NEW_WEBIMAGE	1
#endif

#if NEW_WEBIMAGE
#ifdef STBWEB_BUILD
#include "webimage.h"
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

#define AUTOSCROLL_EDGE_THRESHOLD	24	/* closeness to edge to start auto-scrolling in OS units */
#define AUTOSCROLL_DELAY		100	/* delay before auto-scrolling takes affect */
#define AUTOSCROLL_HOVER_AREA		8	/* space to stay within during DELAY period */

#define NCFRESCO_CURRENT_URL		PROGRAM_NAME"$CurrentURL"
#define NCFRESCO_CURRENT_TITLE		PROGRAM_NAME"$CurrentTitle"

/* -------------------------------------------------------------------------- */

#define TaskModule_RegisterService      0x4D302
#define TaskModule_DeRegisterService    0x4D303
#define wimp_MSERVICE                   0x4D300

#if USE_DIALLER_STATUS
#define Service_DiallerStatus           0xB4

#define dialler_DISCONNECTED            0
#define dialler_CONNECTED_OUTGOING      (1<<2)
#endif

#define Service_ShutdownComplete	0x80

#define Service_SmartCard		0xBA
#define smartcard_INSERTED		0x01

#define Service_Standby			0xAD
#define standby_SHUTDOWN		0x01

#define KeyWatch_Register		0x4E940
#define KeyWatch_Unregister		0x4E941

#define PPP_AlterSettings		0x4B620
#define PPP_Status			0x4B621

#define MESSAGE_OFFER_FOCUS		0x14

/* #define Service_NVRAM			0xE0 */
/* #define nvram_INITIALISED		0 */
/* #define nvram_DYING			1 */
/* #define nvram_CHANGED			2 */

/* -------------------------------------------------------------------------- */


extern void parse_frames(int yes);
extern void image_get_cooler_table(void);

static void fe_drag(const wimp_mousestr *m);
/* static BOOL fe_resize(const wimp_mousestr *m); */
static void fe_status_mode(fe_view v, int mode, int override);
static void check_pending_scroll(fe_view v);

/* static int fe_check_resize(fe_view start, int x, int y, wimp_box *box, int *handle, fe_view *resizing_v); */
static void fe_update_page_info(fe_view v);
/* static void fe_force_fit(fe_view v, BOOL force); */
static void re_read_config(int flags);

/* -------------------------------------------------------------------------- */

static wimp_t task_handle;
int wimp_version;

BOOL use_toolbox = FALSE;
static char *progname;

int debug_level;

/* Used to point to a block in the RMA for a long URL */
static char *fe_stored_url = NULL;

int fe_pending_key = 0;

fe_view main_view = 0;
static fe_view dbox_view = 0;

/* only used by ncfrescointernal:playmovie I think */
fe_view last_click_view = NULL;
int last_click_x, last_click_y;

static be_item highlight_last_link = NULL;

static int fast_poll = 0;

#if USE_DIALLER_STATUS
static int connection_up = fe_interface_UP;
static int connection_count = 0;
#endif

static int toolbar_pending_mode_change = FALSE;

static fe_message_handler_item *message_handlers = NULL;

static int user_status_open = TRUE;

int keyboard_state = fe_keyboard_OFFLINE;


/* On screen keyboard variables */

wimp_t on_screen_kbd = 0;	/* task handle of OSK application */
wimp_box on_screen_kbd_pos;	/* position in screen coordinates of OSK window */
wimp_w on_screen_kbd_w;

/* Keywatch variables */

static int *keywatch_pollword = 0;		/* address of keywatch module pollword */
static int keywatch_last_key_val = 0;		/* value read from pollword on last wimp key event */
static BOOL keywatch_from_handset = TRUE;	/* was last key press from a handset - initialise to true in case mouse used first */

os_error pending_error = { -1, "" };
static char *pending_error_retry = NULL;

int fe_pending_event = 0;

/* ----------------------------------------------------------------------------------------------------- */

pointermode_t pointer_mode = (pointermode_t)-1;

static wimp_mousestr pointer_last_pos = { 0 };
static int pointer_ignore_next = 0;		/* 0= no ignore, 1=just ignore, 2 = ignore and read position */

void fe_pointer_mode_update(pointermode_t mode)
{
    STBDBGN(("ptr_mode_update: was %d now %d '%s '%s\n", pointer_mode, mode, caller(1), caller(2)));

    switch (mode)
    {
        case pointermode_OFF:
            _kernel_osbyte(106, 0, 0);
	    if (pointer_mode == pointermode_ON)
		fe_frame_link_redraw_all(main_view);
            break;

        case pointermode_ON:
            if (pointer_mode != pointermode_ON)
	    {
                pointer_reset_shape();
		fe_frame_link_redraw_all(main_view);
	    }
            break;
    }
    pointer_mode = mode;
}

void frontend_pointer_set_position(fe_view v, int x, int y)
{
    coords_pointstr p;

    p.x = x;
    p.y = y;

    if (v)
    {
	coords_cvtstr cvt = frameutils_get_cvt(v);

	coords_point_toscreen(&p, &cvt);

	/* check that the position is actually over the window or else
           it messes everything up */
	if (p.x < cvt.box.x0)
	    p.x = cvt.box.x0;
	if (p.x > cvt.box.x1-2)
	    p.x = cvt.box.x1-2;

	if (p.y < cvt.box.y0)
	    p.y = cvt.box.y0;
	if (p.y > cvt.box.y1-2)
	    p.y = cvt.box.y1-2;
    }

    pointer_set_position(p.x, p.y);

    pointer_ignore_next = 1;
    pointer_last_pos.x = p.x &~ 1;
    pointer_last_pos.y = p.y &~ 1;

    STBDBG(("pointer_set_position: %d,%d\n", p.x, p.y));
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
    { "ptr_caret", 4, 0 },
    { "ptr_click", 0, 22 },
    { "ptr_menu", 0, 4 },
    { "ptr_push", 4, 0 },
    { "ptr_resizew", 16, 0 },
    { "ptr_resizeh", 0, 16 },
    { "ptr_push", 4, 0 },	/* This is meant to be the secure submit cursor */
    { "ptr_map", 16, 16 },
    { "ptr_mapp", 16, 16 }	/* pressed map - used when loading new page */
};

#define fe_pointer_MAP              (1U<<31)
#define fe_pointer_DRAG             (1U<<30)
#define fe_pointer_RESIZE_WIDTH     (1U<<29)
#define fe_pointer_RESIZE_HEIGHT    (1U<<28)
#define fe_pointer_MAP_PRESS        (1U<<27)

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
        ptr_num = 2;
    else if (item_flags & be_item_info_MENU)
        ptr_num = 1; /* 5;  */
    else if (item_flags & be_item_info_LINK)
        ptr_num = 1;
    else if (item_flags & be_item_info_SECURE)
        ptr_num = 8;
    else if ((unsigned)item_flags & fe_pointer_MAP)   /* force map for the map mode */
        ptr_num = 9;
    else if ((unsigned)item_flags & fe_pointer_DRAG)   /* dragging mode */
        ptr_num = 5;
    else if ((unsigned)item_flags & fe_pointer_RESIZE_WIDTH)   /* resize w  */
        ptr_num = 6;
    else if ((unsigned)item_flags & fe_pointer_RESIZE_HEIGHT)   /* resize h */
        ptr_num = 7;
    else if ((unsigned)item_flags & fe_pointer_MAP_PRESS)   /* force map for the map mode */
        ptr_num = 10;
    return ptr_num;
}

static int pointer_current = 0;

void fe_set_pointer(int item_flags)
{
    int num;

    num = fe_get_pointer_number(item_flags);

    if (!config_display_fancy_ptr && num > 0 && num < 5) /* no_fancy disables hand and caret pointers */
	num = 0;

    if (num != pointer_current)
    {
        pointer_info *info = &pointers[num];

        STBDBG(( "stbfe: set pointer %d '%s' mode %d\n", num, strsafe(info->name), pointer_mode));

        if (info->name)
        {
	    if (_swix(OS_SpriteOp, _INR(0,7),
		      256+36,
		      resspr_area(), info->name,
		      2, info->x, info->y,
		      0, 0) == NULL ||
		_swix(Wimp_SpriteOp, _INR(0,7),
		      36,
		      0, info->name,
		      2, info->x, info->y,
		      0, 0) == NULL)
	    {
		_kernel_osbyte(106, 2, 0);
	    }
        }
        else
        {
            if (pointer_mode == pointermode_OFF)
		_kernel_osbyte(106, 0, 0);
            else
		_kernel_osbyte(106, 1, 0);
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

    pw = render_get_colour(render_colour_BACK, frameutils_find_top(v)->displaying);
    wimp_readpalette(&pal);

    return find_closest_colour(pw.word, (int *)pal.c, 16);
}

/* ----------------------------------------------------------------------------------------------------- */

int caretise(void)
{
    return !keywatch_from_handset || on_screen_kbd ? be_link_CARETISE : 0;
}

int movepointer(void)
{
    return on_screen_kbd || pointer_mode == pointermode_ON ? 0 : be_link_MOVE_POINTER;
}

/* ----------------------------------------------------------------------------------------------------- */

static void fe_type_file(fe_view v, const char *file_name)
{
    FILE *f;
    if ((f = mmfopen(file_name, "r")) != NULL)
    {
        os_error *e = NULL;
        int used = TRUE;
        while (!e && !feof(f) && used)
            e = backend_doc_key(v->displaying, fgetc(f), &used);
        mmfclose(f);
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
	wimp_box bb;
	tb_status_box(&bb);

	if (config_display_control_top)
	{
	    if (box->y1 > bb.y0)
		box->y1 = bb.y0;
	}
	else
	{
	    if (box->y0 < bb.y1)
		box->y0 = bb.y1;
	}
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

static int *fade_ref_list = NULL;
static int fade_ref_count = 0;

static int fade_frame_rectangle(wimp_redrawstr *r, void *h, int update)
{
    int colour = (int)h;
    int ref;

    STBDBG(("fade_frame_rectangle: %d,%d %d,%d\n", r->g.x0, r->g.y0, r->g.x1, r->g.y1));

    if (_swix(ScreenFade_FadeRectangle, _INR(0,7) | _OUT(1),
	  0 + (1<<8),
	  r->g.x0, r->g.y0, r->g.x1, r->g.y1,
	  config_display_time_fade,
	  colour,
	  FADE_LINE_SPACING,
	  &ref) != NULL)
    {
	return 0;
    }

    fade_ref_list = mm_realloc(fade_ref_list, ++fade_ref_count * sizeof(*fade_ref_list));
    fade_ref_list[fade_ref_count-1] = ref;

    return 0;
}

static os_error *fade_frame_child(fe_view v, void *handle)
{
    STBDBG(("fade_frame_child: v %p\n", v));

    if (v->children == NULL && v->w)
    {
	int colour = (int)handle;
	wimp_box box;

	box.x0 = box.y0 = -0x4000;
	box.x1 = box.y1 =  0x4000;

	/* set up fades */
	frontend_view_update(v, &box, fade_frame_rectangle, (void *)colour, fe_update_IMAGE_RENDERING);
    }
    return NULL;
}

/* wait for fades to finish */
static void fade_finish_wait(void)
{
    int fading, i;

    STBDBGN(("fade_finish_wait: count %d\n", fade_ref_count));

    do
    {
	int status;

	fading = 0;
	for (i = 0; i < fade_ref_count; i++)
	{
	    _swix(ScreenFade_GetFadeInfo, _INR(0,1) | _OUT(0),
		  0, fade_ref_list[i], &status);

	    if ((status & 0xff) != 0xff)
		fading++;
	}
    }
    while (fading);

    STBDBGN(("fade_finish_wait: releasing\n"));

    /* release the fades */
    for (i = 0; i < fade_ref_count; i++)
	_swix(ScreenFade_ReleaseFade, _INR(0,1), 0, fade_ref_list[i]);

    /* free up list */
    mm_free(fade_ref_list);
    fade_ref_list = NULL;
    fade_ref_count = 0;
}

void frontend_fade_frame(fe_view v, wimp_paletteword colour)
{
    wimp_box box;
/*     int i, fading; */

    STBDBG(("fade_frame: v %p\n", v));
    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return;

    if (config_display_time_fade == 0)
	return;

    if (v->children)
    {
	iterate_frames(v->children, fade_frame_child, (void *)colour.word);
    }
    else
    {
	box.x0 = box.y0 = -0x4000;
	box.x1 = box.y1 =  0x4000;

	/* set up fades */
	frontend_view_update(v, &box, fade_frame_rectangle, (void *)colour.word, fe_update_IMAGE_RENDERING);
    }

    fade_finish_wait();

    STBDBG(("fade_frame: out\n"));
}

#if 0
void frontend_fade_frame_dispose(fe_view v)
{
    wimp_wstate state;
    wimp_get_wind_state(v->w, &state);

    iterate_frames(main_view, fade_frame_child, (void *)colour.word);

    frontend_view_update(NULL, state.o.box, fade_frame_rectangle, NULL, fe_update_IMAGE_RENDERING);

    fade_finish_wait();
}
#endif

/* ----------------------------------------------------------------------------------------------------- */

static int decode_string(const char *input, const char *matches[], int nmatches)
{
    int i;
    if (input) for (i = 0; i < nmatches; i++)
	if (strcasecomp(input, matches[i]) == 0)
	    return i;
    return -1;
}

static os_error *children__update_state(fe_view v, void *handle)
{
    /* update current state in the history list */
    fe_history_update_current_state(v);
    return NULL;
}

static void children_update_state(fe_view v)
{
    if (v->children)
	iterate_frames(v->children, children__update_state, NULL);
}

/*
 * This function is called when a fetch finishes and the current page is not
 * updated for some reason. There is an optional error message to display
 */

void fe_no_new_page(fe_view v, os_error *e)
{
    if (e)
    {
	/* report error if there was one */
	frontend_complain(e);

	/* log page as failed */
	session_log("", session_FAILED);
    }

    if (!v)
	return;

    /* restore the url of the displaying document */
    if (v->displaying)
    {
	char *durl;
	if (backend_doc_info(v->displaying, NULL, NULL, &durl, NULL) == NULL &&
	    durl)
	    frontend_view_status(v, sb_status_URL, durl);
    }

    /* clear various variables that store state whilst fetching and rendering */
    v->fetching = NULL;
    v->had_completed = TRUE;
    v->images_waiting = 0;

    v->fetching_data.xscroll = v->fetching_data.yscroll = 0;
    v->fetching_data.hist = NULL;

    /* update the toolbar */
    if (use_toolbox)
	tb_status_update_fades(frameutils_find_top(v));

    fe_update_page_info(v);

    /* hide progress indicator if popped up */
    fe_status_clear_fetch_only();

    /* ensure highlight is somewhere */
/*   fe_ensure_highlight(v, 0); */
}

enum
{
    content_tag_SELECTED = 0,
    content_tag_TOOLBAR,
    content_tag_MODE,
    content_tag_LINEDROP,
    content_tag_POSITION,
    content_tag_NOHISTORY,
    content_tag_SOLIDHIGHLIGHT,
    content_tag_NOSCROLL,
    content_tag_FASTLOAD,
    content_tag_URL,
    content_tag_USER,
    content_tag_USERNAME,
    content_tag_BLANKRESET,
    content_tag_ONLOAD,
    content_tag_ONUNLOAD,
    content_tag_ENSURETOOLBAR,
    content_tag_ONBLUR,
    content_tag_SUBMITONUNLOAD,
    content_tag_SELECTBUTTON,
    content_tag_SPECIALSELECT,
    content_tag_NOANTITWITTER
};

static const char *content_tag_list[] =
{
    "SELECTED", "TOOLBAR", "MODE", "LINEDROP",
    "POSITION", "NOHISTORY", "SOLIDHIGHLIGHT", "NOSCROLL",
    "FASTLOAD", "URL", "USER", "USERNAME",
    "BLANKRESET", "ONLOAD", "ONUNLOAD", "ENSURETOOLBAR",
    "ONBLUR", "SUBMITONUNLOAD", "SELECTBUTTON", "SPECIALSELECT",
    "NOANTITWITTER"
};

/*
 * If there is a fragment id then 'url' parameter will always contain it.

 * backend does a format of what it has and then calls visit.

 */

int frontend_view_visit(fe_view v, be_doc doc, char *url, char *title)
{
    char *previous_url;
    int previous_mode;
    int mode;
    int toolbar_state, toolbar_pending;
    char *ncmode;
    int old_keyboard_state, linedrop;
    BOOL is_server_push;

    STBDBG(( "frontend_view_visit url '%s' title '%s' doc %p\n", strsafe(url), strsafe(title), doc));

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return 0;

    if (doc == NULL)    /* lookup failed re visit the current one   */
    {
	fe_no_new_page(v, (os_error *)title);
	return 1;
    }

    /* check for retargeting the document (WINDOW-TARGET header) */
    /* if there is a name and it is different to the one in this frame then
     * pass visit onto another view
     */
    {
	const char *name = backend_check_meta(doc, "WINDOW-TARGET");
	fe_view vv;
	if (name == NULL)
	{
	    /* no redirect - check the fetching ptr */
#if DEBUG
	    if (v->fetching != doc)
		usrtrc("Erroneous call to view visit\n");
#endif
	}
	else if (strcmp(name, v->name) == 0)
	{
	    /* redirected to this window - no action */
	}
	else if ((vv = fe_locate_view(name)) != NULL) /* this will catch redirect to top */
	{
	    /* redirected to an existing window - clear this fetching ptr */
	    v->fetching = NULL;
	    v->had_completed = TRUE;

	    /* let new view take over */
	    v = vv;
	}
	else if (strncmp(name, "__", 2) == 0)
	{
	    /* redirected to a transient window - clear fetching */
	    v->fetching = NULL;
	    v->had_completed = TRUE;

	    /* let new view take over */
	    v = fe_dbox_view(name);
	}
	else if (v->name)
	{
	    /* redirected to a non-existent window - change the name
	     * of this one unless it is the top one */
	    mm_free(v->name);
	    v->name = mm_strdup(name);
	}
    }

    /* clear fetching ptr */
    v->fetching = NULL;

    /* this is only for STB use really  */
    session_log(url, session_CONNECTED);

    /* set system variable for current page */
    if (v == main_view)
    {
	_kernel_setenv(NCFRESCO_CURRENT_URL, url);
	_kernel_setenv(NCFRESCO_CURRENT_TITLE, strsafe(title));
    }

    previous_url = NULL;
    previous_mode = v->browser_mode;

    /* ensure caret isn't displayed currently on page */
    if (frontend_view_has_caret(v))
 	backend_remove_highlight(v->displaying);

    if (v == main_view)
    {
	fe_view vv = fe_find_top_popup(main_view);

	STBDBG(("frontend_view_visit: popup %p trans %d pos %d\n", vv, vv->open_transient, vv->transient_position));

	if (vv->open_transient &&
	    (vv->transient_position == fe_position_TOOLBAR_WITH_COORDS || vv->transient_position == fe_position_TOOLBAR)
	    )
	{
	    fe_dispose_view(vv);
	}
    }

    /* fade down previous displayed frame as long as it's not the same one (server-push) */
    is_server_push = v->displaying == doc;
    if (v->displaying && !is_server_push)
    {
	char *durl;

	backend_doc_info(v->displaying, NULL, NULL, &durl, NULL);
	previous_url = mm_strdup(durl);

	if (fe_map_view() == v)
            fe_map_mode(NULL, NULL);

	/* store the positions of this frame in the history */
	fe_history_update_current_state(v);

	/* do before disposing */
	frontend_fade_frame(v, render_get_colour(render_colour_BACK, doc));

        backend_dispose_doc(v->displaying);
    }

    /* call fe_history_update_current_state for each child before removing them all */
    children_update_state(v);

    /* dispose of any children - each one will call fe_history_update_current_state before being removed */
    fe_dispose_view_children(v);

    STBDBG(("vw%p: doc %p now displaying (transient %d)\n", v, doc,
            v->open_transient ));

    v->displaying = doc;
/*     v->current_link = NULL; */
    v->find_last_item = NULL;

    /* initialise mode and toolbar_state */
    mode = fe_browser_mode_WEB;
    toolbar_state = -1;
    toolbar_pending = -1;

    mm_free(v->real_url);
    v->real_url = NULL;

    v->offline_mode = keyboard_state;

    linedrop = -1;
    old_keyboard_state = keyboard_state;

    v->select_button = 0;

#if !ANT_NCFRESCO
    /* force on antitwitter */
    gbf_flags |= GBF_ANTI_TWITTER;
#endif

    /* check for special page instructions - not all relevant to child pages */
    {
	if ((ncmode = mm_strdup(backend_check_meta(doc, "NCBROWSERMODE"))) != NULL)
	{
	    static const char *on_off_list[] = { "OFF", "ON" };
	    static const char *keyboard_list[] = { "ONLINE", "OFFLINE" };			/* order must agree with #defines in stbview.h */
	    static const char *position_list[] = { "FULLSCREEN", "CENTERED", "TOOLBAR" };	/* order must agree with #defines in stbview.h */
	    name_value_pair vals[sizeof(content_tag_list)/sizeof(content_tag_list[0])];
	    wimp_box box;
	    int new_keyboard_state;

	    STBDBG(("ncbrowsermode: %s\n", ncmode));

	    /* decode the parameters */
	    parse_http_header(ncmode, content_tag_list, vals, sizeof(vals)/sizeof(vals[0]));

	    /* set the item to be selected when opened */
	    mm_free(v->selected_id);
	    v->selected_id = mm_strdup(vals[content_tag_SELECTED].value);

	    /* these items are only valid for the top level items */
	    if (v->parent == NULL)
	    {
		/* these itemsa are only valid for non popups */
		if (!v->open_transient)
		{
		    /* set toolbar state */
		    toolbar_state = decode_string(vals[content_tag_TOOLBAR].value, on_off_list, sizeof(on_off_list)/sizeof(on_off_list[0]));

		    /* set mode (keyboard) state */
		    if ((new_keyboard_state = decode_string(vals[content_tag_MODE].value, keyboard_list, sizeof(keyboard_list)/sizeof(keyboard_list[0]))) != -1)
			v->offline_mode = keyboard_state = new_keyboard_state;
		    else
			v->offline_mode = keyboard_state = fe_keyboard_ONLINE;

		    /* set up a pending line drop */
		    if (vals[content_tag_LINEDROP].value)
			linedrop = atoi(vals[content_tag_LINEDROP].value);

		    /* check for user change */
		    if (vals[content_tag_USER].value)
		    {
			STBDBG(("ncbrowsermode: user '%s' '%s'\n", vals[content_tag_USER].value, vals[content_tag_USERNAME].value));

			v->pending_user = atoi(vals[content_tag_USER].value);
			v->pending_user_name = mm_strdup(vals[content_tag_USERNAME].value);
		    }

		    if (vals[content_tag_NOANTITWITTER].value)
		    {
			STBDBG(("ncbrowsermode: disable antitwitter\n"));
			gbf_flags &= ~GBF_ANTI_TWITTER;
		    }
		}
		/* open valid for transients */
		else
		{
		    /* see if this popup requires a particular toolbar beneath it */
		    if (vals[content_tag_ENSURETOOLBAR].value)
		    {
			toolbar_state = TRUE;
			toolbar_pending = atoi(vals[content_tag_ENSURETOOLBAR].value);
		    }
		}

		/* read position for window, safe area relative */
		v->transient_position = decode_string(vals[content_tag_POSITION].value, position_list, sizeof(position_list)/sizeof(position_list[0]));
		STBDBG(("v->transient_position==%d\n", v->transient_position));
		if (v->transient_position == fe_position_UNSET && vals[content_tag_POSITION].value)
		{
		    box.x0 = box.x1 = box.y0 = box.y1 = -1;
		    sscanf(vals[content_tag_POSITION].value, "%d,%d,%d,%d", &box.x0, &box.y0, &box.x1, &box.y1);

		    if (box.x0 != -1)
		    {
			if (box.y0 != -1)
			{
			    if (box.x1 != -1 && box.y1 != -1)
			    {
				v->box.x0 = text_safe_box.x0 + box.x0*2;
				v->box.y0 = text_safe_box.y0 + box.y0*2;
				v->box.x1 = text_safe_box.x0 + box.x1*2;
				v->box.y1 = text_safe_box.y0 + box.y1*2;

				v->transient_position = fe_position_COORDS;
			    }
			    else
			    {
				v->dbox_x = box.x0*2;
				v->dbox_y = box.y0*2;
				v->transient_position = fe_position_CENTERED_WITH_COORDS;
			    }
			}
			else
			{
			    v->dbox_y = box.y0*2;
			    v->transient_position = fe_position_TOOLBAR_WITH_COORDS;
			}
		    }

		    if (v->transient_position != fe_position_UNSET)
			memset(&v->margin , 0, sizeof(v->margin));
		}

		/* check nohistory flag */
		if (vals[content_tag_NOHISTORY].value)
		    v->dont_add_to_history = TRUE;

		/* check for override URL for ncoptions pages */
		mm_free(v->real_url);
		v->real_url = mm_strdup(vals[content_tag_URL].value);

		/* check for resetting the screen blanker */
		if (vals[content_tag_BLANKRESET].value)
		    _swix(ScreenBlanker_Control, _IN(0), 1);
	    }

	    /* check highlight flag */
	    if (vals[content_tag_SOLIDHIGHLIGHT].value)
		backend_doc_set_flags(v->displaying, be_openurl_flag_SOLID_HIGHLIGHT, be_openurl_flag_SOLID_HIGHLIGHT);

	    /* check no scroll flag - note this can override that specified in the frameset */
	    if (vals[content_tag_NOSCROLL].value)
		v->scrolling = fe_scrolling_NO;
	    /* if this is top level then ensure it is set to invisible */
	    else if (v->parent == NULL)
		v->scrolling = fe_scrolling_INVISIBLE;

	    /* check fast load flag */
	    v->fast_load = vals[content_tag_FASTLOAD].value != 0;

	    /* check load and unload functions */
	    mm_free(v->onload);
	    v->onload = mm_strdup(vals[content_tag_ONLOAD].value);

	    mm_free(v->onunload);
	    v->onunload = mm_strdup(vals[content_tag_ONUNLOAD].value);

	    mm_free(v->onblur);
	    v->onblur = mm_strdup(vals[content_tag_ONBLUR].value);

	    mm_free(v->submitonunload);
	    v->submitonunload = mm_strdup(vals[content_tag_SUBMITONUNLOAD].value);

	    if (vals[content_tag_SELECTBUTTON].value)
		v->select_button = (int)strtoul(vals[content_tag_SELECTBUTTON].value, NULL, 0);

	    STBDBG(("selectbutton: 0x%x\n", v->select_button));

	    mm_free(v->specialselect);
	    v->specialselect = mm_strdup(vals[content_tag_SPECIALSELECT].value);

	    mm_free(ncmode);
	}
	else
	{
	    /* reset all values if NCBROWSERMODE header not set */
	    /* for top level only reset to no scrolling */
	    if (v->parent == NULL)
		v->scrolling = /* config_display_frames_scrollbars && config_display_frames_top_level ? fe_scrolling_AUTO :  */fe_scrolling_INVISIBLE;

	    v->fast_load = FALSE;
/* 	    v->dont_add_to_history = FALSE; Don't need to reset this as it is always set correctly in open_url */
	    v->transient_position = fe_position_UNSET;

	    v->offline_mode = keyboard_state = fe_keyboard_ONLINE;
	}
    }

    /* if linedrop is given then set it */
    if (linedrop != -1)
    {
	STBDBG(("frontend_view_visit: linedrop %d\n", linedrop));
	_swix(PPP_AlterSettings, _INR(0,2), 0, 0, linedrop);
    }
    /* if you go to an online page then reset the time to the default */
    else if (/* old_keyboard_state == fe_keyboard_OFFLINE && */ keyboard_state == fe_keyboard_ONLINE)
    {
	int def_linedrop;
	if (_swix(PPP_Status, _INR(0,1) | _OUT(2), 0, 0, &def_linedrop) == NULL)
	    _swix(PPP_AlterSettings, _INR(0,2), 0, 0, def_linedrop);
	STBDBG(("frontend_view_visit: linedrop reset to default %d\n", def_linedrop));
    }

    /* if not transient page then ensure codec toolbar removed */
    if (!v->open_transient && use_toolbox)
    {
	tb_codec_kill();
    }

    /* check for special page instructions - but only on top page  */
    if (v->parent == NULL)
    {
        const char *bmode;
	if (ncmode == NULL && (bmode = backend_check_meta(doc, "BROWSERMODE")) != NULL)
	{
	    mode = strcasecomp(bmode, "DESKTOP") == 0 ? fe_browser_mode_DESKTOP :
		strcasecomp(bmode, "DBOX") == 0 ? fe_browser_mode_DBOX :
		strcasecomp(bmode, "HOTLIST") == 0 ? fe_browser_mode_HOTLIST :
		strcasecomp(bmode, "HISTORY") == 0 ? fe_browser_mode_HISTORY :
		strcasecomp(bmode, "APP") == 0 ? fe_browser_mode_APP :
		fe_browser_mode_WEB;
	}

	/* decide on what to store in the way of return pages */
	if (previous_mode == fe_browser_mode_DESKTOP)
	{
	    mm_free(v->return_page);
	    v->return_page = mm_strdup(previous_url);
	}

	if (mode == fe_browser_mode_APP)
	{
	    if (previous_mode != fe_browser_mode_APP)
	    {
		mm_free(v->app_return_page);
		v->app_return_page = mm_strdup(previous_url);
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

	if (!v->open_transient)
	{
	    fe_status_mode(v, mode, toolbar_state);
	}
	else if (toolbar_pending != -1)
	{
	    fe_status_mode(v, mode, toolbar_state);
	    fe_status_open_toolbar(v, toolbar_pending);
/* 	    tb_status_set_active(); */
	}
	else
	    /* ensure mode is set so that keys are passed through OK */
	    v->browser_mode = mode;

	/* new scheme is embed info about the button to select into the HTML */
	if (v->select_button && use_toolbox)
	{
	    STBDBG(("frontend_view_visit: selecting button %x\n", v->select_button));
	    tb_status_button(v->select_button, tb_status_button_ACTIVE);
	}
    }
    else
    {
        fe_status_mode(v, v->parent->browser_mode, -1);

	v->dont_add_to_history = v->parent->dont_add_to_history;
    }


    {
	/* transfer any fetching_data values over in to the real world */
	fe_view top = frameutils_find_top(v);
	if (top->fetching_data.hist)
	{
	    top->hist_at = top->fetching_data.hist;
	    top->fetching_data.hist = NULL;
	}
	else if (!v->from_frame)
	{
	    if (!is_server_push)
		fe_history_visit(v, url, title);
	}

	/* redraw window - unless transient as that hasn't been opened yet */
	if (!v->open_transient)
	    frontend_view_redraw(v, NULL);

	/* set URL and secure flag and page info */
	frontend_view_status(v, sb_status_URL, url);

	if (use_toolbox)
	{
	    if (!v->open_transient && v->parent == NULL)
	    {
		int f;
		backend_doc_info(v->displaying, &f, NULL, NULL, NULL);
		tb_status_set_secure(f & be_doc_info_SECURE);
	    }
	    tb_status_set_lights(light_state_FETCHING);
	}

	fe_update_page_info(v);

	/* scroll to top of page if it doesn't look like any better offer will come along */
	if (!v->open_transient && (!fe_check_download_finished(v) || !v->fetching_data.yscroll))
	    frontend_view_ensure_visable(v, -1, 0, 0);
    }

    mm_free(previous_url);

    return 1;
}

/* ----------------------------------------------------------------------------------------------------- */

fe_view fe_dbox_view(const char *name)
{
    fe_frame_info info;
    wimp_box box;
    fe_view view;

/*     fe_dbox_dispose(); */

    STBDBG(( "fe_dbox_view: %s\n", name));

    info.name = (char *)name;
    info.noresize = TRUE;
    info.scrolling = fe_scrolling_NO;
    info.margin = margin_box;

/*   adjust_box_for_toolbar(&info.margin); */

    LAYDBG(("fe_dbox_view: info.margin = (%d,%d)..(%d,%d)\n",
    	    info.margin.x0 , info.margin.y0 , info.margin.x1 , info.margin.y1 ));

    if (use_toolbox)
	tb_menu_hide();

    box = screen_box;
    box.y1 = box.y0;

    frontend_complain(fe_new_view(NULL, &box, &info, FALSE, &view));

    if (view)
    {
	fe_view v;
/* 	wimp_get_caret_pos(&dbox_saved_caret); */
/* 	if (view->w) */
/* 	    fe_get_wimp_caret(view->w); */

#if LINK_DBOX
	/* chain onto end of main_view */
	for (v = main_view; v->next; v = v->next)
	    ;

	v->next = view;
	view->prev = v;
#endif

	view->open_transient = TRUE;
    }

    return view;
}

void fe_dbox_dispose(void)
{
    if ( main_view->next )
    {
        if ( main_view->next != dbox_view )
            fe_dispose_view(main_view->next);
    }
    if (dbox_view)
    {
	fe_dispose_view(dbox_view);
    }
    dbox_view = NULL;
    main_view->next = NULL;
}

/* ----------------------------------------------------------------------------------------------------- */

#if 0
static void fe_move_window_to_top(fe_view v)
{
    wimp_wstate state;
    int dy;

    if (!v || !v->w)
	return;

    frontend_fatal_error(wimp_get_wind_state(v->w, &state));

    dy = text_safe_box.y1 - state.o.box.y1;
    state.o.box.y1 += dy;
    state.o.box.y0 += dy;
    frontend_fatal_error(wimp_open_wind(&state.o));

    v->box = state.o.box;
}
#endif

/* ----------------------------------------------------------------------------------------------------- */

void fe_dbox_cancel(void)
{
    if (fe_current_passwd)
        fe_passwd_abort();
#if 1 /*ANT_NCFRESCO*/
    else
        fe_dbox_dispose();
#endif
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

#define PPrimer_ChangePrinter	0x4B100

static char *print_targets[] =
{
    "__print",
    "__printlegal",
    "__printletter"
};

static char *print_sizes[] =
{
    "",
    "legal",
    "letter"
};

static int print_events[] =
{
    fevent_PRINT,
    fevent_PRINT_LEGAL,
    fevent_PRINT_LETTER
};

static os_error *fe__print(fe_view v, int size)
{
    os_error *e = NULL;
    int old_size = -1, old_escape_key;

    if (_swix(PDriver_Info, 0) != NULL)
        return makeerror(ERR_NO_PRINTER);

    sound_event(snd_PRINT_START);

    if (size != fe_print_DEFAULT)
    {
	if (nvram_read("PaperSize", &old_size))
	    nvram_write("PaperSize", size == fe_print_LEGAL ? 2 : 1);

	_swix(PPrimer_ChangePrinter, 0);
    }

    /* activate the appropriate button, and move highlight to BACK */
    if (use_toolbox)
    {
#if 0
	fe_view vv;

	/* remove the view of frames to print */
	if ((vv = fe_locate_view(print_targets[size])) != NULL)
	    fe_dispose_view(vv);
#endif
	tb_status_button(print_events[size], tb_status_button_UNPRESSED);
	tb_status_button(print_events[size], tb_status_button_ACTIVE);
	tb_status_button(fevent_TOOLBAR_EXIT, tb_status_button_PRESSED);
    }

    /* set escape key to ENTER */
    _swix(OS_Byte, _INR(0,2) | _OUT(1), 220, 13, 0, &old_escape_key);
    STBDBG(("fe__print: escape key set, old %d\n", old_escape_key));

    if (!e)
	e = awp_print_document(v->displaying, config_print_scale,
			       (config_print_nopics ? awp_print_NO_PICS : 0) |
			       (config_print_nobg ? awp_print_NO_BACK : 0) |
			       (config_print_nocol ? awp_print_NO_COLOUR : 0) |
			       (config_display_links_underlined ? awp_print_UNDERLINE : 0) |
			       (config_print_sideways ? awp_print_SIDEWAYS : 0) |
			       (config_print_collated ? awp_print_COLLATED : 0) |
			       (config_print_reversed ? awp_print_REVERSED : 0),
			       print__copies);

    /* reset escape key */
    _swix(OS_Byte, _INR(0,2), 220, old_escape_key, 0);

    if (use_toolbox)
    {
	tb_status_button(print_events[size], tb_status_button_INACTIVE);
	tb_status_button(print_events[size], tb_status_button_PRESSED);
    }

    if (old_size != -1)
    {
	nvram_write("PaperSize", old_size);
	_swix(PPrimer_ChangePrinter, 0);
    }

    sound_event(snd_PRINT_END);

    return e;
}

os_error *fe_print(fe_view v, int size)
{
    os_error *e = NULL;
    fe_view vv;

    STBDBG(("fe_print: v%p size %d children %p\n", v, size, v->children));

    if (v->children)
    {
	char buffer[64];

	sprintf(buffer, "ncint:openpanel?name=printframes&size=%s", print_sizes[size]);

	if ((vv = fe_locate_view(print_targets[size])) != NULL)
	{
	    sound_event(snd_MENU_HIDE);
	    fe_dispose_view(vv);
	}
	else if (fe_popup_open())
	    sound_event(snd_WARN_BAD_KEY);
	else
	{
	    e = frontend_open_url(buffer, NULL, print_targets[size], NULL, NULL, fe_open_url_NO_CACHE | fe_open_url_NO_REFERER);
	    if (!e && use_toolbox) tb_status_button(print_events[size], tb_status_button_ACTIVE);
	}
    }
    else
    {
#if 0
	/* check to see if print frames dbox is still up and remove it */
	if ((vv = fe_locate_view(print_targets[size])) != NULL)
	{
	    sound_event(snd_MENU_HIDE);
	    fe_dispose_view(vv);
	}
#endif
	e = fe__print(v, size);
    }

    /* translate errors */
    if (e)
    {
	STBDBG(("fe_print: error %x %s\n", e->errnum, e->errmess));

	if (e->errnum != 0x11 /* Escape */ && e->errnum != 0x5C9 /* Print cancelled */ && e->errnum != ANTWEB_ERROR_BASE + ERR_NO_PRINTER)
	{
	    char buf[128];

	    sprintf(buf, "ncint:printpage?size=");
	    strcat(buf, print_sizes[size]);
	    strcat(buf, "&source=");
	    fe_frame_specifier_create(v, buf, sizeof(buf));

	    pending_error_retry = mm_strdup(buf);

	    e = makeerror(ERR_PRINT_FAILED);
	}
    }

    return e;
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

	if (use_toolbox)
	    e = tb_status_set_message(status_type_FETCH, buffer);
    }

    return e;
}

static void check_pending_scroll(fe_view v)
{
    if (v->fetching_data.yscroll || v->fetching_data.xscroll)
    {
        wimp_box bb;
        int h, hh;

	STBDBG(("check_pending_scrooll %d,%d\n", v->fetching_data.yscroll, v->fetching_data.xscroll));

        frontend_view_bounds(v, &bb);

        h = bb.y1 - bb.y0;
        hh = h > -v->doc_height ? h : -v->doc_height;

	/* if the desired point is within a screen's worth of the
	 * current end of the document then jump to it - the page will
	 * stretch appropriately */
        if (v->fetching_data.yscroll - h >= -hh)
        {
            frontend_view_ensure_visable(v, v->fetching_data.xscroll, v->fetching_data.yscroll, v->fetching_data.yscroll);

            v->fetching_data.yscroll = 0;
	    v->fetching_data.xscroll = 0;
        }
    }
}

static int fe_transient_set_size(fe_view v)
{
    int changed = FALSE;
    int h;

    STBDBG(("vw%p: fe_transient_set_size(%d)\n", v, v->transient_position));

    switch (v->transient_position)
    {
    case fe_position_TOOLBAR_WITH_COORDS:
    case fe_position_TOOLBAR:
	v->box.x0 = text_safe_box.x0;
	v->box.x1 = text_safe_box.x1;
	h = v->transient_position == fe_position_TOOLBAR_WITH_COORDS || !use_toolbox ? v->dbox_y : tb_status_height();

	if (config_display_control_top)
	{
	    v->box.y1 = text_safe_box.y1 - h + 8; /* +8 to get it slightly above the toolbar */
	    v->box.y0 = v->box.y1 - (-v->doc_height);
	}
	else
	{
	    v->box.y0 = text_safe_box.y0 + h - 8; /* -8 to get it slightly below the toolbar */
	    v->box.y1 = v->box.y0 + (-v->doc_height);
	}
	memset(&v->margin, 0, sizeof(v->margin));
	changed = TRUE;
	break;

    case fe_position_CENTERED_WITH_COORDS:
    case fe_position_CENTERED:
    case fe_position_UNSET:
    {
	int h = -v->doc_height/*  - v->backend_margin.y1 + v->backend_margin.y0 */;
	int ww, hh;

	ww = v->transient_position == fe_position_CENTERED_WITH_COORDS ? v->dbox_x : DBOX_SIZE_X;
	hh = v->transient_position == fe_position_CENTERED_WITH_COORDS ? v->dbox_y : DBOX_SIZE_Y;

	if (h < hh)
	    h = hh;
	v->box.x0 = (text_safe_box.x0 + text_safe_box.x1)/2 - ww/2;
	v->box.x1 = v->box.x0 + ww;
	v->box.y0 = (text_safe_box.y0 + text_safe_box.y1)/2 - h/2;
	v->box.y1 = v->box.y0 + h;
	memset(&v->margin, 0, sizeof(v->margin));
	changed = TRUE;
	break;
    }

    case fe_position_FULLSCREEN:
 	v->box = screen_box;
	if (use_toolbox)
	{
	    if (config_display_control_top)
		v->margin.y1 -= tb_status_height();
	    else
		v->margin.y0 += tb_status_height();
	}
	changed = TRUE;
	break;

    case fe_position_COORDS:
	changed = TRUE;
	break;
    }

    return changed;
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

	vals[2] += v->images_had;
	vals[3] += v->images_waiting;

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

static os_error *fe__lookfor_selected(fe_view v, void *handle)
{
    int *vals = handle;
    if (v->selected_id && vals[0] == 0)
	vals[0] = (int)v;
    return NULL;
}

void fe_ensure_highlight_after_fetch(fe_view v)
{
    fe_view top = frameutils_find_top(v);
    fe_view vcaret = NULL;

    /*
     * When all frames have downloaded then we need to place the caret.
     * First see who has the caret.
     * If not one of us then ignore it.
     * If a frame with a window has the caret then ensure the link is visible and caretise if necessary.
     * If a frame without a window has the caret then move the link into the first of its children that does have a window.
     */

    STBDBG(( "stbfe: check download finished current link %p top %p\n", v, top));

    /* if opening a transient then that definitely gets the highlight */
    if (top->open_transient)
	vcaret = top;

    /* else see if one of the frames has a selected id */
    if (vcaret == NULL)
    {
	int vals[1];
	vals[0] = 0;
	iterate_frames(top, fe__lookfor_selected, vals);
	vcaret = (fe_view)vals[0];
    }

    /* else see where the caret is now */
    if (vcaret == NULL)
    {
	wimp_caretstr cs;
	wimp_get_caret_pos(&cs);

	/* if on the toolbar then there may be some specific decisions,
	   for now - put it back onto the main page*/
	if (fe_status_window_handle())
	{
	    vcaret = main_view;
	    if (vcaret->children)
		vcaret = fe_next_frame(vcaret, TRUE);
	}

	if (vcaret == NULL)
	{
	    /* find which frame has the caret */
	    vcaret = fe_find_window(top, cs.w);

	    STBDBG(( "stbfe: w %d vcaret %p\n", cs.w, vcaret));

	    /* if the window has children then move into the first child frame */
	    if (vcaret && vcaret->children)
	    {
		vcaret = fe_next_frame(vcaret, TRUE);

		STBDBG(( "stbfe: new vcaret %p\n", vcaret));
	    }
	}
    }

    /* if the frame has data then update the link position */
    if (vcaret && vcaret->displaying)
    {
	int flags = 0;
	be_item item = backend_read_highlight(vcaret->displaying, NULL);

	/* if there is a selected id then move to that */
	if (v->selected_id)
	{
	    item = backend_locate_id(vcaret->displaying, v->selected_id);
	    if (item)
		flags = be_link_ONLY_CURRENT | caretise();

	    /* clear the selection so it doesn't get used again */
	    mm_free(v->selected_id);
	    v->selected_id = NULL;
	}

	if ((flags & be_link_ONLY_CURRENT) == 0)
	{
	    flags = be_link_VISIBLE | be_link_INCLUDE_CURRENT | caretise();
	    if (pointer_mode != pointermode_OFF)
		flags |= be_link_TEXT;

	    /* need to remove ONLY_CURRENT otherwise it can't ensure the link is visible */
/* 	    flags |= be_link_ONLY_CURRENT; */
	}

	/* if there was anything to move to then set the caret in the window */
	if (backend_highlight_link(vcaret->displaying, item, be_link_ONLY_CURRENT | flags | movepointer()) != NULL ||
	    ((flags & be_link_ONLY_CURRENT) == 0 && backend_highlight_link(vcaret->displaying, item, flags | movepointer()) != NULL) ||
	    backend_highlight_link(vcaret->displaying, NULL, flags | movepointer()) != NULL)
	    fe_get_wimp_caret(vcaret->w);

	STBDBG(( "stbfe: selected view %p\n", vcaret));
    }
}

static os_error *reopen_frames(fe_view v)
{
    BOOL changed = FALSE;

    if (v->open_transient)
	changed = fe_transient_set_size(v);

    STBDBG(("reopen_frames: v%p changed %d\n", v, changed));
    feutils_open_behind_toolbar(v->w);

    feutils_resize_window(&v->w, &v->margin, &v->box, &v->x_scroll_bar, &v->y_scroll_bar, v->doc_width, -v->doc_height, fe_scrolling_NO, fe_bg_colour(v));

    if (changed)
    {
	if (v->displaying)
	    backend_reset_width(v->displaying, 0);

	/* resize again in case document got longer */
	feutils_resize_window(&v->w, &v->margin, &v->box, &v->x_scroll_bar, &v->y_scroll_bar, v->doc_width, -v->doc_height, fe_scrolling_NO, fe_bg_colour(v));

	frontend_view_redraw(v, NULL); /* put inside because of ncint:select */
    }

    return NULL;
}

int fe_check_download_finished(fe_view v)
{
    fe_view top = frameutils_find_top(v);
    BOOL finished = FALSE;

    frontend_view_status(v, sb_status_WORLD);

    if (fe_is_download_finished(top))
    {
	STBDBG(( "fe_check_download_finished: v%p finished transient %d %s %s\n", v, top->open_transient, caller(1), caller(2)));

	/* clear various variables */
	v->fetching = NULL;

	v->fetching_data.xscroll = v->fetching_data.yscroll = 0;
	v->fetching_data.hist = NULL;

	/* update toolbar fades */
	if (use_toolbox)
	    tb_status_update_fades(top);

        /* get rid of temporary globe   */
        fe_status_clear_fetch_only();
	fe_update_page_info(v);

	finished = TRUE;

	/* if it is a transient window then open here */
	if (top->open_transient)
	{
#if 1
	    reopen_frames(top);
#else
	    BOOL changed = fe_transient_set_size(top);

 	    feutils_resize_window(&top->w, &top->margin, &top->box, &top->x_scroll_bar, &top->y_scroll_bar, v->doc_width, -v->doc_height, fe_scrolling_NO, fe_bg_colour(top));

	    if (changed)
	    {
		backend_reset_width(top->displaying, 0);

		/* resize again in case document got longer */
		feutils_resize_window(&top->w, &top->margin, &top->box, &top->x_scroll_bar, &top->y_scroll_bar, v->doc_width, -v->doc_height, fe_scrolling_NO, fe_bg_colour(top));

		frontend_view_redraw(top, NULL); /* put inside because of ncint:select */
	    }
#endif

#if 0
	    /* 04/11/97: don't do this as then OSK doesn't work */
	    if (strncmp(top->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) == 0)
		pointer_limit(top->box.x0, top->box.y0, top->box.x1-frontend_dx, top->box.y1-frontend_dy);
#endif
	}
	else
	{
/* 	    if (config_display_scale_fit) */
/* 		fe_force_fit(top, TRUE); */
	}

	/* check for a user change. We don't do it in visit because
           the page needs to be fully downloaded first (ie out of the
           cache!) */
	if (v->children == NULL && !top->open_transient && v->pending_user != -1)
	{
	    int user = v->pending_user;
	    char *current_user_s = getenv(PROFILE_NUM_VAR);
	    if (current_user_s == NULL || atoi(current_user_s) != user)
	    {
		char buffer[64];

		STBDBG(("setuser: user %d name '%s'\n", user, v->pending_user_name));

		/* set the current user variable */
		sprintf(buffer, "%d", user);
		_kernel_setenv(PROFILE_NUM_VAR, buffer);

		/* set the user name variable */
		translate_escaped_text(strsafe(v->pending_user_name), buffer, sizeof(buffer));
		_kernel_setenv(PROFILE_NAME_VAR, buffer);

		/* re read the config and flush the cache */
		re_read_config(0);

		/* close and open popups */
	    }
	    mm_free(v->pending_user_name);
	    v->pending_user_name = NULL;
	    v->pending_user = -1;
	}

	/* internal function to call when finished loading? */
	if (v->onload)
	{
	    frontend_open_url(v->onload, v, NULL, NULL, NULL, 0);
	    mm_free(v->onload);
	    v->onload = NULL;
	}

	/* optimise various memory uses */
	if (1)
	{
	    hotlist_optimise();

	    fe_global_history_optimise();
	    fe_history_optimise(main_view);

	    if (use_toolbox)
		tb_optimise();
	    fe_internal_optimise();

	    cookie_optimise();
  	    access_optimise_cache();
 	    auth_optimise();
/* 	    plugin_list_optimise(); */

	    /* when finished page, check if stash is full, in which
               case clear the low memory flag */
	    if (mm_poll())
		gbf_flags &= ~GBF_LOW_MEMORY;
	}

	/* optionally dump out the current memory use */
	if (getenv("NCFresco$MemTrace"))
	{
	    extern char   *flexptr__base;
	    extern char   *flexptr__free;
	    extern char   *flexptr__slot;
	    extern void   *heap__base;
	    extern int malloc_size, malloc_da, heap__size, heap__da, flex__da;

	    int area = -1;
	    int us = -1, next = -1, free;

	    wimp_slotsize(&us, &next, &free);
	    usrtrc("slot: size %dK next %dK free %dK\n", us/1024, next/1024, free/1024);

	    usrtrc("flex: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), flex__da)/1024, (flexptr__slot - flexptr__base)/1024, (flexptr__free - flexptr__base)/1024);
	    usrtrc("heap: area %dK size %dK top %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), heap__da)/1024, heap__size/1024, ((int *)heap__base)[3]/1024);
	    usrtrc("mall: area %dK size %dK\n", _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), malloc_da)/1024, malloc_size/1024);

	    do
	    {
		_swix(OS_DynamicArea, _INR(0,1) | _OUT(1), 3, area, &area);
		if (area != -1)
		{
		    int size;
		    char *name;
		    _swix(OS_DynamicArea, _INR(0,1) | _OUT(2) | _OUT(8), 2, area, &size, &name);

		    usrtrc("da: size %10dK name '%s'\n", size/1024, name);
		}
	    }
	    while (area != -1);
	}
    }

    if (finished && pointer_mode == pointermode_OFF)
	fe_ensure_highlight_after_fetch(v);

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
    v = frameutils_find_top(v_orig);

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

    case sb_status_FETCHED:     /* string giving amount of html document fetched */
    case sb_status_LOADING:     /* string giving amount of html document loaded from a local file */
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
	    errors = va_arg(ap, int),
	    deferred = va_arg(ap, int);
/* 	    so_far = va_arg(ap, int), */
/* 	    in_trans = va_arg(ap, int); */

	if (use_toolbox)
	{
	    v_orig->images_had = fetched + errors + deferred;
	    v_orig->images_waiting = waiting + fetching;

	    STBDBG(( "stbfe: view status v %p images %d/%d\n", v_orig, fetched + errors + deferred, waiting + fetching));

	    e = set_fetch_message(v_orig);
	}
	else
	{
	    int total = fetched + errors + waiting + fetching + deferred;
	    v->fetch_images = total ? (fetched + errors + deferred)*256/total : 256;

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

    case sb_status_PLUGIN:
    {
	void *pp = va_arg(ap, void *);
	int busy = va_arg(ap, int);
	int state = va_arg(ap, int);
	int helpers_open = va_arg(ap, int);

	STBDBG(("stbfe_view_status: pp %p busy %d state %d helpers_open %d\n", pp, busy, state, helpers_open));

	if (use_toolbox)
	    tb_codec_state_change(state, helpers_open != 0, helpers_open == 0);
	break;
    }
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

    return FALSE;
}

/* ------------------------------------------------------------------------------------------- */

int frontend_memory_panic(void)
{
    return 0;
}

/* ------------------------------------------------------------------------------------------- */


#if USE_DIALLER_STATUS

int frontend_interface_state(void)
{
    int state = connection_up;
    STBDBG(("is_interface_up: var=%d\n", connection_up));

    if (connection_up == fe_interface_ERROR)
	connection_up = fe_interface_DOWN;

    return state;
}

#if 0
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
#endif

/* ------------------------------------------------------------------------------------------- */

/*
 * Called from the backend when a file is downloadeded that we can't deal with.
 */

void frontend_pass_doc(fe_view v, char *url, char *cfile, int ftype)
{
    wimp_msgstr msg;

    STBDBG(( "frontend_pass_doc url '%s' cfile '%s' ft %x\n", strsafe(url), strsafe(cfile), ftype));

    if (cfile)
    {
	memset(&msg, 0, sizeof(msg));
	msg.hdr.size = sizeof(wimp_msgstr);
	msg.hdr.action = wimp_MDATAOPEN;
	msg.data.dataopen.type = ftype;
	strcpy(msg.data.dataopen.name, cfile);
	frontend_fatal_error(wimp_sendmessage(wimp_ESENDWANTACK, &msg, 0));
    }

    if (v)
    {
	fe_no_new_page(v, NULL);
/* 	fe_check_download_finished(v); */
    }

    STBDBG(( "frontend_pass_doc: out\n"));
}

/*
 * Called from the backend when an unknown type of URL is accessed.
 */

void frontend_url_punt(fe_view v, char *url, char *bfile)
{
    wimp_eventstr e;
    urlopen_data *ud = (urlopen_data*) &(e.data.msg.data);
    int len;
    char *target = NULL;

    STBDBG(( "frontend_url_punt '%s'\n", url));

    if (v == NULL)
	v = main_view;
    else
	fe_no_new_page(v, NULL);
#if 0
    if (strncasecomp(url, INTERNAL_PREFIX, INTERNAL_PREFIX_SIZE) == 0)
    {
        fe_pending_url = mm_strdup(url);
        fe_pending_bfile = mm_strdup(bfile);
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
    STBDBG(( "resize: v %p w %x frame to %4d,%4d %4d,%4d\n", v, v->w, box->x0, box->y0, box->x1, box->y1));

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

void frontend_frame_layout(fe_view v, int nframes, fe_frame_info *info, int refresh_only, int dividers_max)
{
    int i;
    coords_cvtstr cvt;
    fe_view child = NULL;
    fe_view top;

    if (!v || v->magic != ANTWEB_VIEW_MAGIC)
	return;

    /* get rid of the old layout before creating the new one */
    if (refresh_only)
        child = v->children;
    else
        fe_dispose_view_children(v); /* this should be unnecessary as visit() should have already removed them */

    /* ensure at top of visible screen  */
    frontend_view_ensure_visable(v, -v->margin.x0, 0, 0);

    top = frameutils_find_top(v);
    top->dividers_max = dividers_max;

    cvt = frameutils_get_cvt(v);
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

            if (frontend_complain(fe_new_view(v, &box, ip, !top->open_transient, &vv)) == NULL && vv)
	    {
		char *url;
		int flags = fe_open_url_FROM_FRAME;

		vv->frame_index = i;
		url = ip->src;

		/* if we are coming from history then need to use the
		 * urls from the history item and fill in the scroll
		 * offsets */
		if (top->hist_at)
		{
		    char specifier[32];
		    char *new_url;

		    specifier[0] = 0;
		    fe_frame_specifier_create(vv, specifier, sizeof(specifier));
		    new_url = fe_history_lookup_specifier(top, specifier, &vv->fetching_data.xscroll, &vv->fetching_data.yscroll);
		    if (new_url)
		    {
			url = new_url;
			flags |= fe_open_url_FROM_HISTORY;
		    }
		    STBDBG(("frontend_frame_layout: history override '%s' gets url '%s' offsets %dx%d\n", specifier, strsafe(new_url), vv->fetching_data.xscroll, vv->fetching_data.yscroll));
		}

		if (url)
		{
		    os_error *e = frontend_open_url(url, vv, NULL, NULL, NULL, flags);
		    if (e && e->errnum != ANTWEB_ERROR_BASE + ERR_NO_SUCH_FRAG)
			frontend_complain(e);
		}
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

static os_error *reformat_view(fe_view v, void *handle)
{
    if (v->w)
    {
	if (v->had_completed)
	{
	    backend_doc_reformat(v->displaying);
	    fe_refresh_window(v->w, NULL);
	}
    }
    return NULL;
}

void fe_font_size_set(int value, BOOL absolute)
{
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

    frontend_complain(webfonts_reinitialise());

    iterate_frames(main_view, reformat_view, NULL);

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

static os_error *fe__bgsound_set(fe_view v, void *handle)
{
    if (config_sound_background)
    {
	backend_plugin_action(v->displaying, be_plugin_action_item_ALL, plugin_state_PLAY);
    }
    else
    {
	backend_plugin_action(v->displaying, be_plugin_action_item_ALL, plugin_state_STOP);
    }

    return NULL;
}

void fe_bgsound_set(int state)
{
    if (state == -1)
	config_sound_background = !config_sound_background;
    else
	config_sound_background = state;

    /* set nvram here so that SOUND key toggles it as well as menu */
    nvram_write(NVRAM_SOUND_TAG, config_sound_background);

    /* cancel playback on all sounds */
    iterate_frames(main_view, fe__bgsound_set, NULL);
}

void fe_beeps_set(int state, BOOL sound)
{
    int new_state = state == -1 ? !config_sound_fx : state;

    /* set state in right order to ensure sound gets heard */
    if (new_state)
    {
	config_sound_fx = TRUE;
	if (sound)
	    sound_event(snd_BEEPS_ON);
    }
    else
    {
	if (sound)
	    sound_event(snd_BEEPS_OFF);
	config_sound_fx = FALSE;
    }
}

/* ------------------------------------------------------------------------------------------- */

/*
 * Force this window to have no horizontal scrolling
 */

#if 0
static void fe_force_fit(fe_view v, BOOL force)
{
    int scale_value = 100;

    v = frameutils_find_top(v);

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
		    ((v->box.x1 + v->backend_margin.x1 + v->margin.x1) - (v->box.x0 + v->backend_margin.x0 + v->margin.x0)) * 100 / v->doc_width :
		    100;

		STBDBG(("fe_force_fit: v %p force %d scaling %d %%\n", v, force, scale_value));

		if (!force || scale_value < 98)
		{
		    backend_doc_set_scaling(v->displaying, scale_value);
		    backend_doc_reformat(v->displaying);
		    fe_refresh_window(v->w, NULL);
		}
	    }
	}
	while (v);
    }
    else
    {
	int width = (v->box.x1 + v->backend_margin.x1) - (v->box.x0 + v->backend_margin.x0);

	scale_value = force && v->doc_width > 0 ? width * 100 / v->doc_width : 100;

	STBDBG(("fe_force_fit: v %p force %d width %d docwidth %d scaling %d %%\n", v, force, width, v->doc_width, scale_value));

	if (!force || scale_value < 98)
	{
	    backend_doc_set_scaling(v->displaying, scale_value);
	    backend_doc_reformat(v->displaying);
	    fe_refresh_window(v->w, NULL);
	}
    }

    visdelay_end();

}
#endif

void fe_scaling_set(int state)
{
    if (state == -1)
	config_display_scale_fit = !config_display_scale_fit;
    else
	config_display_scale_fit = state;

    if (config_display_scale_fit)
	gbf_flags |= GBF_AUTOFIT;
    else
	gbf_flags &= ~GBF_AUTOFIT;

    if (main_view->displaying)
	backend_reset_width(main_view->displaying, 0);

/*  fe_force_fit(main_view, config_display_scale_fit); */
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
    v = fe_find_top_nopopup(v);

/*     if (v->dont_add_to_history) */
/*         url = v->hist_at ? v->hist_at->url : NULL; */
/*     else */
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
/* key handlers */
/* ------------------------------------------------------------------------------------------- */

BOOL fe_writeable_handle_keys(fe_view v, int key)
{
    int used = FALSE;

    if (v && v->displaying)
        backend_doc_key(v->displaying, key, &used);

    STBDBG(("fe_writeable_handle_keys: v %p key %d used %d\n", v, key, used));

    if (used == be_doc_key_SUBMIT && on_screen_kbd)
    {
	fe_keyboard_close();
    }

    if (used == be_doc_key_FILLED)
    {
	/* special mode for offline pages - vanish after filling an item */
	if (keyboard_state == fe_keyboard_OFFLINE && on_screen_kbd)
	    fe_keyboard_close();

	return fevent_HIGHLIGHT_FORWARD;
    }

    return used ? 0 : -1;
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
    BOOL quiet = (BOOL)handle;
    os_error *e = NULL;
    if (v->fetching)
    {
	backend_dispose_doc(v->fetching);
	v->fetching = NULL;

	if (!quiet)
	    frontend_view_status(v, sb_status_ABORTED);
    }
    else if (v->displaying)
    {
        e = backend_doc_abort(v->displaying);

	if (!quiet)
	    frontend_view_status(v, sb_status_ABORTED);
    }
    return e;
    handle = handle;
}

os_error *fe_abort_fetch(fe_view v, BOOL quiet)
{
    if (!quiet)
	sound_event(snd_ABORT);
    return iterate_frames(v, fe__abort_fetch, (void *)quiet);
}

/* ------------------------------------------------------------------------------------------- */

#define INFO_BUFSIZE	2048

void fe_open_info(fe_view v, be_item ti, int x, int y, BOOL clear_others)
{
    if (v && v->displaying && keyboard_state == fe_keyboard_ONLINE)
    {
	char *link, *title;
	char *buffer;
	fe_view current;
	char *current_url = NULL;

	if ((current = fe_locate_view(TARGET_INFO)) != NULL && current->displaying)
	    backend_doc_info(current->displaying, NULL, NULL, &current_url, NULL);

	buffer = mm_malloc(INFO_BUFSIZE);
	strcpy(buffer, "ncint:openpanel?name=info&source=");

	/* add the specifier for this buffer */
	fe_frame_specifier_create(v, buffer, INFO_BUFSIZE);

	if (ti &&
	    backend_item_pos_info(v->displaying, ti, &x, &y, NULL, &link, NULL, &title) == NULL &&
	    link)
	{
	    int len;

	    /* add the url of any link to be displayed */
	    strlencat(buffer, "&url=", INFO_BUFSIZE);
	    len = strlen(buffer);
	    url_escape_cat(buffer + len, link, INFO_BUFSIZE - len);

	    /* add any TITLE shown in the link */
	    if (title)
	    {
		strlencat(buffer, "&title=", INFO_BUFSIZE);
		len = strlen(buffer);
		url_escape_cat(buffer + len, title, INFO_BUFSIZE - len);
	    }
	}

	STBDBG(("fe_open_info: clear_others %d current %p url '%s' buf '%s'\n", clear_others, current, strsafe(current_url), buffer));

	if (current_url != NULL ||
	    fe_internal_check_popups(clear_others))
	{
	    if (current_url == NULL || strcmp(current_url, buffer) != 0)
	    {
		frontend_open_url(buffer, NULL, TARGET_INFO, NULL, NULL, fe_open_url_NO_CACHE);
	    }
	    else if (current)
	    {
		sound_event(snd_MENU_HIDE);
		fe_dispose_view(current);
	    }
	}

	mm_free(buffer);
    }
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

static clipboard_itemstr clip_list_jpeg[] =
{
    { FILETYPE_JPEG, 0, clip_save_text },
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

    selected = backend_read_highlight(v->displaying, NULL);
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
        int ft;

        clipboard_Destroy();    /* must destroy first as we are reusing the filename    */

        ft = image_preferred_save_filetype( (image)im );

        if ( ft == FILETYPE_JPEG )
        {
            if (backend_image_saver_jpeg(CLIP_FILE, im))
            {
                clipboard_Destroy();
                /* register as the clip board - the filename is the data */
                clipboard_CreateList( clip_list_jpeg, clip_destroy,
                                      CLIP_FILE, CLIP_FILE_LEN, NULL);
            }
        }
        else
        {
            if (backend_image_saver_sprite(CLIP_FILE, im))
            {
                clipboard_Destroy();
                /* register as the clip board - the filename is the data */
                clipboard_CreateList( clip_list_sprite, clip_destroy,
                                      CLIP_FILE, CLIP_FILE_LEN, NULL);
            }
        }
    }
}

#ifdef ANT_NCFRESCO
/* pdh: copy image to clipboard if one's selected, otherwise text */
void fe_copy_whatever_to_clipboard( fe_view v )
{
    void *im;

    if ( get_selected_image(v) )
        fe_copy_image_to_clipboard( v );
    else
        fe_copy_text_to_clipboard( v );
}
#endif

/* ------------------------------------------------------------------------------------------- */

int fe_hotlist_add_possible(fe_view v)
{
    return v && v->displaying && v->browser_mode == fe_browser_mode_WEB;
}

os_error *fe_hotlist_add(fe_view v)
{
    char *url, *title;
    time_t t;

    if (!fe_hotlist_add_possible(v))
        return NULL;

    if (frontend_complain(backend_doc_info(v->displaying, NULL, NULL, &url, &title)))
	return NULL;

    t = time(NULL) + 2;

    if (use_toolbox)
    {
	tb_status_button(fevent_HOTLIST_ADD, tb_status_button_ACTIVE);
	tb_status_button(fevent_HOTLIST_ADD, tb_status_button_UNPRESSED);
    }

    frontend_complain(hotlist_add(url, title));

    while (time(NULL) < t)
	;

    if (use_toolbox)
    {
	tb_status_button(fevent_HOTLIST_ADD, tb_status_button_PRESSED);
	tb_status_button(fevent_HOTLIST_ADD, tb_status_button_INACTIVE);
    }

    return NULL;
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
        be_item select = backend_read_highlight(v->displaying, NULL);
        e = backend_item_info(v->displaying, select, NULL, &selected_url, NULL);
        if (!e && selected_url)
        {
            e = hotlist_remove(selected_url);
	    if (!e) e = fe_internal_toggle_panel("favs", TRUE); /* I don't thuink this works anymore */
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

static int fe_status_set_margins(fe_view v, int new_state_open)
{
    wimp_wstate state;
    int movement = 0;
    wimp_box mbox;
    int status_height = use_toolbox ? tb_status_height() : 0;

    if (config_display_frames_top_level)
	mbox.y0 = mbox.y1 = 0;
    else
	mbox = margin_box;

    /* reopen window tiled with status window   */
    wimp_get_wind_state(v->w, &state);
    if (new_state_open)
    {
	if (config_display_control_top)
	{
	    v->margin.y1 = mbox.y1 - (status_height + STATUS_TOP_MARGIN);
	    movement = status_height + STATUS_TOP_MARGIN;

	    if (on_screen_kbd && v->children == NULL)
		v->margin.y1 -= on_screen_kbd_pos.y1 - on_screen_kbd_pos.y0;
	}
	else
	{
	    v->margin.y0 = mbox.y0 + status_height + STATUS_TOP_MARGIN;

	    if (on_screen_kbd && v->children == NULL)
		v->margin.y0 += on_screen_kbd_pos.y1 - on_screen_kbd_pos.y0;
	}
    }
    else
    {
	if (config_display_control_top)
	{
	    v->margin.y1 = mbox.y1;
	    movement = -status_height - STATUS_TOP_MARGIN;
	    if (state.o.y > -mbox.y1)
		movement = state.o.y + mbox.y1;
	}
	else
	{
	    v->margin.y0 = mbox.y0;
	}
    }

    STBDBG(("fe_status_set_margins: v %p y %d to %d dy %d\n", v, v->margin.y0, v->margin.y1, movement));

    wimp_open_wind(&state.o);

    /* cause a reformat */
    if (v->displaying)
    {
	frontend_view_set_dimensions(v, 0, 0);
	backend_reset_width(v->displaying, 0);
    }

    return movement;
}

os_error *fe_status_state(fe_view v, int state)
{
    BOOL is_open = use_toolbox ? tb_is_status_showing() : FALSE;
    BOOL new_state_open = state == -1 ? !is_open : state;

    if (new_state_open != is_open)
    {
	int movement = fe_status_set_margins(v, state);

	if (use_toolbox)
	{
	    if (new_state_open)
	    {
		tb_status_update_fades(v);
		tb_status_show(FALSE);
	    }
	    else
		tb_status_hide(FALSE);
	}

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
		fe_view_scroll_y(v, movement, fe_view_scroll_ENSURE);
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

    if (keyboard_state != fe_keyboard_ONLINE)
    {
	fevent_handler(fevent_OFFLINE_PAGE, v);
    }
    else
    {
	v = fe_find_top_nopopup(v);

	/* changed so that when the user toggles state it always results in a change */
	if (use_toolbox)
	    user_status_open = !tb_is_status_showing();

	/* get rid of all the popups */
	if (!user_status_open)
	{
	    while (v->next)
	    {
	        STBDBG(("vw%p: status_toggle calls dispose_view\n", v->next ));
	        fe_dispose_view(v->next);
	    }
	}

	if (use_toolbox)
	    e = fe_status_state(v, user_status_open);
	else
	    statuswin_toggle(v);
    }
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
#if 0
    /* don't show spinny if current mode is offline */
    if (keyboard_state == fe_keyboard_OFFLINE)
	return;
#endif
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
		fe_status_state(frameutils_find_top(v), override);
	    }
	    else switch (mode)
	    {
		/* if desktop mode then disable toolbar */
	    case fe_browser_mode_DESKTOP:
/* 	    case fe_browser_mode_DBOX: leave it on for RCA use */
	    case fe_browser_mode_APP:
		fe_status_state(frameutils_find_top(v), 0);
		break;

		/* otherwise leave it in previous state */
	    default:
		fe_status_state(frameutils_find_top(v), user_status_open);
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

/* new multipurpose unstack/remove window/close popups */

os_error *fe_status_unstack(fe_view source_v)
{
    fe_view top = source_v ? fe_find_top_nopopup(source_v) : main_view;

    STBDBG(("fe_status_unstack: source %p %s top %p %s\n",
	    source_v, source_v ? source_v->name : NULL,
	    top, top ? top->name : NULL));

    if (stbmenu_is_open())
    {
	stbmenu_close();
    }
    else if (on_screen_kbd)
    {
	fe_keyboard_close();
    }
    else if (fe_passwd_abort())
    {
    }
    else
    {
	fe_view v = fe_find_top_popup(source_v ? source_v : main_view);

	if (v && v->open_transient)
	{
	    if (strncmp(v->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) != 0) /* can't close external dboxes */
	    {
/* 		fe_internal_deleting_view(v); */
		fe_dispose_view(v);
	    }
	}
	else if (use_toolbox & tb_status_unstack(TRUE))
	{
	}
	else
	    /* hardwire this to main_view 'cos I'm not sure what the current code really does if it isn't */
            fe_history_move(main_view/* source_v */, history_PREV);
    }

    if (use_toolbox)
	tb_status_update_fades(top);

    return NULL;
}

BOOL fe_status_unstack_possible(fe_view source_v)
{
    fe_view v;

    if (!use_toolbox)
	return FALSE;

    if (stbmenu_is_open() || on_screen_kbd)
	return TRUE;

    v = fe_find_top_popup(source_v ? source_v : main_view);

    if (v && v->open_transient)
    {
	return strncmp(v->name, TARGET_DBOX, sizeof(TARGET_DBOX)-1) != 0; /* externally opened boxes cannot be opened by this method */
    }

    if (tb_status_unstack_possible())
	return TRUE;

    return fe_history_possible(source_v, history_PREV);
}

/*
 * bar is from 0 to however many toolbars there are
 */

os_error *fe_status_open_toolbar(fe_view v, int bar)
{
    if (use_toolbox)
    {
	tb_status_new(v, bar);
	tb_status_update_fades(v);
    }
    return NULL;
}

void fe_status_unstack_all(void)
{
    fe_view v;

    if (stbmenu_is_open())
	stbmenu_close();

    if (on_screen_kbd)
	fe_keyboard_close();

    fe_passwd_abort();

    v = fe_find_top_popup(main_view);

    if (v && v->open_transient)
    {
/* 	    fe_internal_deleting_view(v); */
        STBDBG(("vw%p: status_unstack_all calls dispose_view\n", v ));
	fe_dispose_view(v);
    }

    if (use_toolbox)
	tb_status_unstack_all(TRUE);
}

/* ------------------------------------------------------------------------------------------- */

/* This should only be used for informtional uses - not triggering a highlight move */

void fe_scroll_changed(fe_view v, int x, int y)
{
    STBDBG(("fe_scroll_changed(): v %p\n", v));

    if (use_toolbox)
    {
    }
    else
    {
        statuswin_refresh_slider(v);
    }
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

    cvt = frameutils_get_cvt(v);
    coords_box_toscreen(box, &cvt);
    return TRUE;
}

void fe_fake_click(fe_view v, int x, int y)
{
    coords_cvtstr cvt;
    coords_pointstr p;

    if (!v || !v->displaying)
        return;

    cvt = frameutils_get_cvt(v);
    p.x = x;
    p.y = y;
    coords_point_toworkarea(&p, &cvt);

    last_click_x = p.x;
    last_click_y = p.y;
    last_click_view = v;

    backend_doc_click(v->displaying, p.x, p.y, wimp_BLEFT);
}

#if 0
/* pdh: now called frameutils_get_cvt and in commonsrc/frameutils.c */
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
#endif

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
    cvt = frameutils_get_cvt(v);
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
    tb_status_set_message(status_type_URL,
			  strncasecomp(url, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) == 0 ||
			  strncasecomp(url, "ncint:", sizeof("ncint:")-1) == 0 ?
			  NULL : url);
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

/* ------------------------------------------------------------------------------------------- */

static int scrollsafe(int scroll)
{
    return scroll * 4;
}

BOOL fe_check_autoscroll(fe_view v, const wimp_mousestr *mp)
{
    BOOL scrolled = FALSE;

    /* check for auto-scrolling */
    if (v->w && v->scrolling != fe_scrolling_NO)
    {
	static int autoscroll_xedge = 0, autoscroll_yedge = 0, autoscroll_time = 0;
	wimp_wstate state;
	int xedge, yedge;
	wimp_box box;

	wimp_get_wind_state(v->w, &state);

	box = state.o.box;
	adjust_box_for_toolbar(&box);

	/* check edge returns how far over the threshold we are */
	xedge = check_edge_proximity(mp->x, state.o.box.x0, state.o.box.x1, AUTOSCROLL_EDGE_THRESHOLD);
	yedge = check_edge_proximity(mp->y, box.y0, box.y1, AUTOSCROLL_EDGE_THRESHOLD);

/* 	STBDBG(("autoscroll: box %d to %d, mouse %d speed %d\n", box.y0, box.y1, mp->y, yedge)); */

	/* we start scrolling if we stay in one place for AUTOSCROLL_DELAY
	 * we continue scrolling as long as we are over the threshold
	 */
	if (autoscroll_xedge || autoscroll_yedge)
	{
	    autoscroll_xedge = xedge;
	    autoscroll_yedge = yedge;
	    scrolled = fe_scroll_request(v, &state.o, scrollsafe(xedge), scrollsafe(yedge));
	}
	else if (xedge || yedge)
	{
	    int now = alarm_timenow();
	    if (abs(xedge - autoscroll_xedge) <= AUTOSCROLL_HOVER_AREA && abs(yedge - autoscroll_yedge) < AUTOSCROLL_HOVER_AREA)
	    {
		if (now - autoscroll_time >= AUTOSCROLL_DELAY)
		    scrolled = fe_scroll_request(v, &state.o, scrollsafe(xedge), scrollsafe(yedge));
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

    return scrolled;
}

/* ------------------------------------------------------------------------------------------- */

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

    /* update key leds */
    if (use_toolbox)
	tb_status_set_key_leds();

    frontend_fatal_error(wimp_get_point_info(&m));
    pointer_moved = m.x != pointer_last_pos.x || m.y != pointer_last_pos.y;

    /* update last pointer position */
    pointer_last_pos = m;

#if 0
    if (pointer_moved)
    {
	STBDBG(( "p %d,%d w %x v %p main %p\n", m.x, m.y, m.w, v, main_view));
/* 	dump_views(main_view, 2); */
    }
#endif
    /* are we dragging the page around? */
    if (dragging_view)
    {
        fe_drag(&m);
        return;
    }

    /* are we resizing a frame? */
    if ( frameutils_are_we_resizing() )
    {
        if ( frameutils_resize(&m) )
            fe_set_pointer( 0 );
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

    /* check for menu mode stuff - returns if over menu window */
    stbmenu_check_pointer(&m);

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
	    be_item item = backend_read_highlight(v->displaying, NULL);

	    highlight_moved = item != highlight_last_link;
	    highlight_last_link = item;

	    if (item)
		frontend_complain(backend_item_info(v->displaying, item, &flags, &link, NULL));

	    if (highlight_moved)
		fe_update_link(v, flags, link);
	}

	/* check if the pointer has been moved and we need to go to pointer mode    */
	if (pointer_moved && (v_over || m.w == fe_status_window_handle()))
	{
	    STBDBG(( "idle: pointer moved to %d,%d had_modechange %d\n", m.x, m.y, pointer_ignore_next));

	    if (pointer_ignore_next)
	    {
		if (pointer_ignore_next == 2)
		    frontend_fatal_error(wimp_get_point_info(&pointer_last_pos));
		pointer_ignore_next = 0;
	    }
	    else
	    {
		fe_pointer_mode_update(pointermode_ON);
	    }
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

        if (!dragging_view && !frameutils_are_we_resizing() )
            fe_set_pointer(0);

	if (use_toolbox)
	    tb_status_check_pointer(&m);

	return;
    }

    v = v_over;

    /* see if highlight moved at all */
    {
	be_item item = backend_read_highlight(v->displaying, NULL);

	highlight_moved = item != highlight_last_link;
	highlight_last_link = item;
    }

    /* if old mode then pulse a bit */
    if (!use_toolbox)
    {
        fe_view vv = frameutils_find_top(v);
        if (vv && vv->fetch_document != 256)
            statuswin_update_fetch_status(vv, vv->fetch_status);
    }

    /* if we are a frameset then select resizable pointer   */
    if (v->children)
    {
        int type = frameutils_check_resize(v, m.x, m.y, NULL, NULL, NULL);

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

    /* check for autoscrolling */
    fe_check_autoscroll(v, &m);

    /* otherwise update link from current position  */
    {
	be_item ti = NULL, oldti;
        get_item_info(v, &m, &flags, &link, &ti);
        if (pointer_moved)
            fe_update_link(v, flags, link);

        /* don't want the pointer changing if on a blank section of a usemap    */
        if ((flags & be_item_info_USEMAP) && (!link || !*link))
            flags &= ~(be_item_info_USEMAP | be_item_info_ISMAP | be_item_info_LINK);

        /* also update pointer shape    */
        fe_set_pointer(flags);

	/* zero ti if not something useful */
#ifdef ANT_NCFRESCO
        oldti = backend_read_highlight( v->displaying, NULL );
#endif
        {
            if ((flags & (be_item_info_LINK | be_item_info_INPUT | be_item_info_MENU | be_item_info_BUTTON | be_item_info_ACTION | be_item_info_LABEL)) == 0)
	        ti = NULL;
	}

	/* try dragging highlight */
	if ( !akbd_pollctl() && pointer_moved && v->displaying
	     && ti != oldti )
    	{
#ifdef ANT_NCFRESCO
            if ( oldti == NULL || !be_item_has_caret(v->displaying,oldti)
                 && (ti || !backend_highlight_is_persistent(v->displaying) ) )
#endif
    	    {
        	if (ti)
                {
    		    backend_set_highlight(v->displaying, ti, FALSE );
    	        }
    	        else
    	        {
     		    backend_remove_highlight(v->displaying);
     		}
   	    }

/* 	    v->current_link = ti; */
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

    /* if it says no scrolling then assume it really means it */
    if (v->scrolling == fe_scrolling_NO)
	return;

    cvt = frameutils_get_cvt(v);

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

#if 0
/* pdh: this lot now all in commonsrc/frameutils.c */
fe_view resizing_view = NULL;
static void fe_resizing_start(fe_view v, const wimp_mousestr *m, const wimp_box *bounds, int type);
static BOOL fe_resize(const wimp_mousestr *m);
#endif

void fe_resize_abort(void)
{
    if ( frameutils_are_we_resizing() )
    {
        frameutils_resize_abort();
        fe_set_pointer(0);
	if (use_toolbox)
	    tb_status_set_message(status_type_HELP, NULL);
    }
}

#if 0
/* pdh: now called frameutils_check_resize and in commonsrc/frameutils.c */
static int fe_check_resize(fe_view start, int x, int y, wimp_box *box, int *handle, fe_view *resizing_v)
{
}
#endif

/* ------------------------------------------------------------------------------------------- */

static int fe_mouse_handler(fe_view v, wimp_mousestr *m)
{
    STBDBG(( "click: w %x view %s\n", m->w, v && v->name ? v->name : "<none>"));

    if (!v)
    {
	switch (m->bbits)
	{
	case wimp_BRIGHT:
	    fevent_handler(config_mode_mouse_adjust, main_view);
	    break;
	case wimp_BMID:
	    if (keyboard_state == fe_keyboard_OFFLINE)
		fevent_handler(fevent_STATUS_OFF, main_view);
	    else
		fevent_handler(config_mode_mouse_menu, main_view);
	    break;
	}

	return FALSE;
    }

    fe_pointer_mode_update(pointermode_ON);

    switch (m->bbits)
    {
    case wimp_BMID:
	if (keyboard_state == fe_keyboard_OFFLINE ||
	    v->browser_mode == fe_browser_mode_DESKTOP ||
	    v->browser_mode == fe_browser_mode_DBOX ||
	    v->browser_mode == fe_browser_mode_APP)
	{
	    fevent_handler(fevent_STATUS_OFF, v);
	}
	else
	{
	    fevent_handler(config_mode_mouse_menu, v);
	}
	break;

    case wimp_BDRAGLEFT:
	if (v->displaying)
	{
	    if (v->children)
	    {
		frameutils_maybe_resize(v, m );
	    }
	    else
	    {
		fe_dragging_start(v, m);
	    }
	}
	break;

    case wimp_BLEFT:
	if (v->displaying)
	    fe_activate_link(v, m->x, m->y, m->bbits);
	break;

    case wimp_BRIGHT:
	fevent_handler(config_mode_mouse_adjust, v);
#if 0
	if (keyboard_state == fe_keyboard_ONLINE && v->displaying && config_mode_mouse_adjust == 1)
	{
	    wimp_wstate state;
	    be_item ti;

	    wimp_get_wind_state(m->w, &state);
	    coords_point_toworkarea((coords_pointstr *)&m->x, (coords_cvtstr *)&state.o.box);

	    ti = NULL;
	    backend_doc_locate_item(v->displaying, &m->x, &m->y, &ti);

	    fe_status_state(main_view, TRUE);
	    fe_open_info(v, ti, m->x, m->y, FALSE);
/* 	    frontend_complain(fe_status_open_toolbar(v, fevent_TOOLBAR_DETAILS - fevent_TOOLBAR_MAIN)); */
	}
#endif
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
		wimp_paletteword pw;
		pw.word = 0;
                colourtran_setGCOL(pw /* config_colours[render_colour_BACK] */, 0, 0, &gcol);
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

#define MESSAGE_NCKEYBOARD_WINDOW_SIZE	0x4E701
#define MESSAGE_NCKEYBOARD_CLOSE	0x4E702
#define MESSAGE_NCKEYBOARD_CLOSED	0x4E703

static void fe_keyboard_closed(void)
{
    STBDBG(("fe_keyboard_closed\n"));

    on_screen_kbd = 0;

    fe_status_set_margins(main_view, TRUE);

    if (pointer_mode == pointermode_OFF)
    {
	fe_view v;

	v = fe_find_top_popup(main_view);
	if (!v || v == main_view)
	    v = fe_selected_view();
	if (!v)
	    v = main_view;

	/* putting the VERT and BACK flags on means it will try and move the toolbar if it fails */
	fe_move_highlight(v, be_link_INCLUDE_CURRENT | be_link_VISIBLE | be_link_TEXT | be_link_VERT | (config_display_control_top ? be_link_BACK : 0));
    }

    tb_status_button(fevent_OPEN_KEYBOARD, tb_status_button_INACTIVE);
}

static void fe_keyboard_set_position(wimp_box *box, wimp_t t)
{
    STBDBG(("fe_keyboard_set_position: %d,%d %d,%d\n", box->x0, box->y0, box->x1, box->y1));

    /* record task handle and bounding box */
    on_screen_kbd = t;
    on_screen_kbd_pos = *box;

    tb_status_button(fevent_OPEN_KEYBOARD, tb_status_button_ACTIVE);

    /* extend margin of view if not frames*/
    fe_status_set_margins(main_view, TRUE);

    /* if a popup is open then move to top of OSK or screen */
    {
	fe_view v = fe_find_top_popup(main_view);
	if (v && v->open_transient && (v->transient_position == fe_position_CENTERED || v->transient_position == fe_position_CENTERED_WITH_COORDS))
	{
	    wimp_wstate state;
	    int dy;

	    frontend_fatal_error(wimp_get_wind_state(v->w, &state));

	    dy = box->y1 - state.o.box.y0;
	    if (dy > text_safe_box.y1 - state.o.box.y1)
		dy = text_safe_box.y1 - state.o.box.y1;

	    state.o.box.y1 += dy;
	    state.o.box.y0 += dy;
	    frontend_fatal_error(wimp_open_wind(&state.o));

	    STBDBG(("fe_keyboard_set_position: moved dbox %p by %+d to %d/%d\n", v, dy, state.o.box.y0, state.o.box.y1));

	    v->box = state.o.box;
	}
    }

    /* check that item with caret in it is still in view */
    {
	fe_view v = fe_selected_view();
	if (v)
	{
	    wimp_box box;
	    be_item ti;

	    ti = backend_read_highlight(v->displaying, NULL);
	    if (ti)
	    {
		backend_doc_item_bbox(v->displaying, ti, &box);

		frontend_view_ensure_visable(v, box.x0, box.y1, box.y0);
	    }
	    else
	    {
		/* 30Jun097 - ensure caret is somewhere */
		backend_highlight_link(v->displaying, NULL, caretise() | movepointer() | be_link_TEXT);
	    }
	}
    }
}

static void fe_keyboard__open(void)
{
    char buffer[256];
    int n;
    wimp_box box;

    STBDBG(("fe_keyboard_open\n"));

    if (getenv("Alias$NCKeyBoard"))
    {
	fe_view v;
 	BOOL toolbar_popup_open = FALSE;

	if (keyboard_state == fe_keyboard_ONLINE && !tb_is_status_showing())
	{
	    user_status_open = TRUE;
	    fe_status_state(main_view, user_status_open);
	}

	n = sprintf(buffer, "NCKeyboard %s",
		    keyboard_state == fe_keyboard_ONLINE ? " -extension browser" : "");

	box.y0 = text_safe_box.y1;
	box.y1 = text_safe_box.y0;
	if (tb_is_status_showing())
	    tb_status_box(&box);

#if 1
	v = fe_find_top_popup(main_view);
	if (v->open_transient &&
	    (v->transient_position == fe_position_TOOLBAR_WITH_COORDS || v->transient_position == fe_position_TOOLBAR)
	    )
	{
	    coords_union(&v->box, &box, &box);
	    toolbar_popup_open = TRUE;
	}
#else
	/* add in open url box if present */
	if ((v = fe_locate_view("__url")) != NULL)
	{
	    coords_union(&v->box, &box, &box);
	    url_open = TRUE;
	}
#endif

	if (config_display_control_top)
	{
	    sprintf(buffer + n, " -scrolldown %d", box.y0/2);
	}
	else
	{
	    BOOL from_top = FALSE;
	    v = fe_selected_view();

	    if (v && !v->open_transient)
	    {
		if (v->parent)
		{
		    from_top = v->box.y1 < box.y1 + 400 + 100;
		}
		else if (v->scrolling == fe_scrolling_NO)
		{
		    BOOL has_caret;
		    be_item ti = backend_read_highlight(v->displaying, &has_caret);
		    if (ti && has_caret)
		    {
			wimp_box box;
			backend_doc_item_bbox(v->displaying, ti, &box);
			from_top = box.y0 < (screen_box.y1 + screen_box.y0)/2;
		    }
		}
	    }

	    if (from_top)
		sprintf(buffer + n, " -scrolldown %d", text_safe_box.y1/2);
	    else
		sprintf(buffer + n, " -scrollup %d", box.y1/2);
	}

	fe_start_task(buffer, NULL);
    }
}

void fe_keyboard_close(void)
{
    STBDBG(("fe_keyboard_close\n"));
    if (on_screen_kbd)
    {
	wimp_msgstr msg;
	msg.hdr.action = (wimp_msgaction)MESSAGE_NCKEYBOARD_CLOSE;
	msg.hdr.size = sizeof(msg.hdr);
	msg.hdr.your_ref = 0;
	wimp_sendmessage(wimp_ESEND, &msg, on_screen_kbd);

	fe_keyboard_closed();
    }
}

void fe_keyboard_open(fe_view v)
{
    if (on_screen_kbd)
    {
	fe_dispose_view(main_view->next);

	fe_keyboard_close();
    }
    else
	fe_keyboard__open();
}

/* ------------------------------------------------------------------------------------------- */

int fe_encoding(fe_view v, int encoding)
{
    STBDBG(("fe_encoding: v %p encoding %d\n", v, encoding));

    if (encoding == be_encoding_READ)
	return backend_doc_encoding(v ? v->displaying : NULL, encoding);

    /* should do this for all frames really */
    backend_doc_encoding(v ? v->displaying : NULL, encoding);

/*     frontend_view_redraw(v, NULL); */
    if (v && v->displaying)
    {
	backend_reset_width(v->displaying, 0);
	fe_refresh_window(v->w, NULL);
    }

    return 0;
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

    /* work-around problem with MovieFS KickStart module */
    if (dataopen->type == 0x13)
	dataopen->type = file_type_real(dataopen->name);

    if (frontend_can_handle_file_type(dataopen->type))
    {
        char buffer[256];
        sprintf(buffer, "/%s", dataopen->name);

/* 	file_lock(dataopen->name, TRUE); */

        frontend_complain(fe_start_task(buffer, NULL));
    }

    /* backend will have locked it before starting flinging it around */
    file_lock(dataopen->name, FALSE);
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
		char *s = mm_strdup(getenv(STBWEB_RETURNED_FRAMES));

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
				frontend_complain(frontend_open_url(url1, main_view, target1 ? target1 : target, NULL, NULL, fe_open_url_NO_REFERER));

			    mm_free(target1);
			    mm_free(url1);
			}
		    }
		    while (ss && *ss);
		}

		mm_free(s);
	    }
	}
	else
	{    /* give error  */
	    char tag[32], *s;
	    strcpy(tag, "noscheme_");
	    strlencat(tag, scheme, sizeof(tag));
	    strlencat(tag, ":", sizeof(tag));
	    strlwr(tag);

	    s = msgs_lookup(tag);
	    if (s && s[0])
	    {
		os_error e;
		e.errnum = 0;
		strncpysafe(e.errmess, s, sizeof(e.errmess));

		frontend_complain(&e);
	    }
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

static void read_gbf(void)
{
    char *s;
    char buf[12];
    int eor, bic;

    /* reread gbf values */
    if ((s = getenv("NCFresco$GbfEor")) != NULL)
    {
	eor = (int)strtoul(s, NULL, 0);
	_swix(OS_SetVarVal, _INR(0,2), "NCFresco$GbfEor", NULL, -1);
    }
    else
	eor = 0;

    if ((s = getenv("NCFresco$GbfBic")) != NULL)
    {
	bic = (int)strtoul(s, NULL, 0);
	_swix(OS_SetVarVal, _INR(0,2), "NCFresco$GbfBic", NULL, -1);
    }
    else
	bic = 0;

    gbf_flags = (gbf_flags &~ bic) ^ eor;

    sprintf(buf, "0x%x", gbf_flags);
    _kernel_setenv("NCFresco$GBF", buf);
}

static os_error *mark_frames(fe_view v, void *handle)
{
    if (v->children == NULL)
	v->pending_mode_change = TRUE;

    return NULL;
    NOT_USED(handle);
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

    dbginit();

    STBDBG(( "modechange: old eig %d,%d new %d,%d\n", dx, dy, frontend_dx, frontend_dy));
    STBDBG(( "modechange: new size %d,%d\n", screen_box.x1, screen_box.y1));

    read_gbf();

    /* Inform backend to recache fonts and colours */
    if ((frontend_dx != dx) || (frontend_dy != dy))
	frontend_complain(webfonts_reinitialise());

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
	iterate_frames(v, mark_frames, NULL);
    }
    else
	fe_refresh_window(-1, NULL);

    /* re read the pointer position as the OS tends to move it around at this point */
    pointer_ignore_next = 2;
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

    curl = mm_strdup(url);
    len = strlen(curl);

    while(len && isspace(curl[len-1]))
	len--;

    curl[len] = 0;

    url_canonicalise(&curl);

    frontend_complain(frontend_open_url(curl, NULL, target, body_file, NULL, fe_open_url_NO_REFERER));

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

    if (access_is_scheme_supported(scheme))
    {
	fe__handle_openurl(msg, url, target, body_file);
    }

    mm_free(scheme);
}

/* ------------------------------------------------------------------------------------------- */

#if 0
/*
 * w is the window handle that currently contains the focus.
 * flags are all reserved.
 *
 * This is offered around by the Watchdog if the task owning the
 * topmost window is not the same task that has the input focus.
 *
 * IF we think we should have the caret then we should take it
 * with wimp_set_caret_pos() and return TRUE to claim the message.
 */

static BOOL window_is_visible(wimp_w w)
{
    wimp_redrawstr r;
    int more;
    BOOL visible = FALSE;
    r.w = w;
    r.box.x0 = r.box.y0 = -0x4000;
    r.box.x1 = r.box.y1 =  0x4000;
    wimp_update_wind(&r, &more);
    while (more)
    {
	visible = TRUE;
	wimp_get_rectangle(&r, &more);
    }
    return visible;
}

static os_error *window__is_ours(fe_view v, void *handle)
{
    int *vals = handle;
    if (v->w == vals[0])
	vals[1] = 1;
    return NULL;
}

static BOOL window_is_ours(wimp_w w)
{
    int vals[2];

    if (w == fe_status_window_handle())
	return TRUE;

    vals[0] = w;
    vals[1] = 0;
    iterate_frames(main_view, window__is_ours, vals);
    return vals[1];
}
#endif

/*
 * The focus is being offered to us. This means We should take it.
 */

static int offer_window_focus_handler(wimp_w w, int flags)
{
    fe_get_wimp_caret(w);
    return 1;
    NOT_USED(flags);
}

/* ------------------------------------------------------------------------------------------- */

void fe_user_unload(void)
{
    BOOL toolbar = tb_is_status_showing();

    STBDBG(("fe_user_unload:\n"));

    /* on a shutdown flush the cache and stuff */
    re_read_config(ncfresco_loaddata_NOT_ALL | ncfresco_loaddata_FLUSH);

    fe_status_unstack_all();

    /* this ensures that when we come out of standby the toolbar is as when we left it */
    if (!toolbar)
	tb_status_hide(FALSE);
}

void fe_user_load(void)
{
    STBDBG(("fe_user_load:\n"));

    /* flush and reload everything */
    re_read_config(0);
}

/* ------------------------------------------------------------------------------------------- */

/* PREQUIT on an NC is sent before the confirm shutdown box is opened
 * and again when it is confirmed */

static void fe_handle_prequit(void)
{
    if (fe_map_view())
	fe_map_mode(NULL, NULL);

    stbmenu_close();
}

/* ------------------------------------------------------------------------------------------- */

static BOOL fe_config_filter(int phase, const char *name, const void *value)
{
    static FILE *fh = NULL;
    BOOL discard = FALSE;

    STBDBGN(("fe_config_filter: phase %d name '%s'\n", phase, strsafe(name)));

    switch (phase)
    {
    case config_filter_phase_START:
	break;

    case config_filter_phase_STOP:
	stbkeys_list_loaded();
	break;

    case config_filter_phase_ADD:
	if (strcmp(name, "key") == 0)
	{
	    stbkeys_list_add(value);
	    discard = TRUE;
	}
	else if (strcmp(name, "toolbar") == 0)
	{
	    tb_bar_add(value);
	    discard = TRUE;
	}
	else if (strcmp(name, "toolbar.name") == 0)
	{
	    tb_bar_name(value);
	    discard = TRUE;
	}
	break;

    case config_filter_phase_START_WRITE:
	fh = (FILE *)value;
	break;

    case config_filter_phase_STOP_WRITE:
	fh = NULL;
	break;

    case config_filter_phase_WRITE:
	if (fh)
	{
	    if (strcmp(name, "key") == 0)
	    {
		discard = TRUE;
	    }
	    else if (strcmp(name, "toolbar") == 0)
	    {
		discard = TRUE;
	    }
	    else if (strcmp(name, "toolbar.name") == 0)
	    {
		discard = TRUE;
	    }
	}
	break;
    }
    return discard;
}

static void read_nvram(void)
{
    int t;

    /* Update from NVRAM */
    if (nvram_read(NVRAM_PRINT_COLOUR_TAG, &t))
	config_print_nocol = !t;

    if (nvram_read(NVRAM_PRINT_ORIENTATION_TAG, &t))
	config_print_sideways = t;

    if (nvram_read(NVRAM_PRINT_BG_TAG, &t))
	config_print_nobg = !t;

    if (nvram_read(NVRAM_PRINT_IMAGES_TAG, &t))
	config_print_nopics = !t;


    if (nvram_read(NVRAM_FONTS_TAG, &t))
	fe_font_size_set(t, TRUE);

    if (nvram_read(NVRAM_BEEPS_TAG, &t))
	fe_beeps_set(t, FALSE);

    if (nvram_read(NVRAM_SOUND_TAG, &t))
	fe_bgsound_set(t);

    if (nvram_read(NVRAM_SCALING_TAG, &t))
	fe_scaling_set(t);


    if (nvram_read(NVRAM_DISPLAY_BG_TAG, &t))
	config_display_body_colours = t;

    if (nvram_read(NVRAM_DISPLAY_IMAGES_TAG, &t))
	config_defer_images = !t;
}

static void re_read_config(int flags)
{
    STBDBG(("stbfe: reading config flags %x\n", flags));

    if ((flags & ncfresco_loaddata_NOT_ALL) == 0)
	flags |= ncfresco_loaddata_CONFIG | ncfresco_loaddata_COOKIES | ncfresco_loaddata_PASSWORDS |
	    ncfresco_loaddata_HOTLIST | ncfresco_loaddata_PLUGINS | ncfresco_loaddata_ALLOW | ncfresco_loaddata_FLUSH;

    /* flush all data first */
    if (flags & ncfresco_loaddata_FLUSH)
    {
	fe_history_dispose(main_view);
	fe_global_history_dispose();

	cookie_dispose_all();

	access_flush_cache();

	auth_dispose();

	plugin_list_dispose();

	fe_internal_flush();

	hotlist_shutdown();
    }

    if ((flags & ncfresco_loaddata_CONFIG) &&
	file_type("<"PROGRAM_NAME"$Config>") != -1)
    {
	config_read_file_by_name("<"PROGRAM_NAME"$Config>");

	/* force all the window stuff to be re read */
	frontend_dx = frontend_dy = 0;
	fe_mode_changed();
    }

    if (flags & ncfresco_loaddata_PASSWORDS)
	auth_init_passwords();

    if (flags & ncfresco_loaddata_ALLOW)
	auth_init_allow();

    if ((flags & ncfresco_loaddata_COOKIES) && config_cookie_enable)
	cookie_read_file(config_cookie_file);

    if (flags & ncfresco_loaddata_PLUGINS)
	plugin_list_read_file();

    if (flags & ncfresco_loaddata_HOTLIST)
	hotlist_init();

    if (flags & ncfresco_loaddata_NVRAM)
	read_nvram();
}

static void re_read_config_data(int flags, const char *filename)
{
    config_read_file_by_name(filename);
    NOT_USED(flags);
}

static void write_config_data(int flags, const char *filename)
{
    config_write_file_by_name(filename);
    NOT_USED(flags);
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

	    /* IP is up */
	    if (new_state == dialler_CONNECTED_OUTGOING)
            {
		connection_up = fe_interface_UP;

		tb_status_set_modem(TRUE);

                /* only reload stuff on first connection */
                if (connection_count++ == 0)
                {
		    re_read_config(0);
                }
            }
	    /* IP is down */
	    else if (new_state == dialler_DISCONNECTED)
	    {
		tb_status_set_modem(FALSE);

		/* only reset if we were up, otherwise it will wipe
                   out the error condition before we have a chance to
                   look at it. */
		if (connection_up == fe_interface_UP)
		    connection_up = fe_interface_DOWN;
	    }
	    /* error has occurred */
	    else if ((new_state & 0xf0) == 0x80)
	    {
		connection_up = fe_interface_ERROR;
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
	    if (config_event_smartcard_out)
		fevent_handler(config_event_smartcard_out, 0);
	    break;
#if 0
	    fe_history_dispose(main_view);
 	    fe_global_history_dispose();

	    cookie_dispose_all();

	    access_flush_cache();

	    auth_dispose();

	    plugin_list_dispose();
#endif
	}

	else if (delta & smartcard_INSERTED)
	{			/* smartcard inserted */
	    if (config_event_smartcard_in)
		fevent_handler(config_event_smartcard_in, 0);
	}
	break;
    }

    case Service_Standby:
    {
	int flags = r->r[0];
	STBDBG(("service_standby: flags %08x event %x\n", flags, config_event_standby_out));
	if ((flags & standby_SHUTDOWN) == 0)
	    if (config_event_standby_out)
		fevent_handler(config_event_standby_out, 0);
	break;
    }

    case Service_ShutdownComplete:
	if (config_event_standby_in)
	    fevent_handler(config_event_standby_in, 0);
	break;

#if 0
    /* This is defined out because the feature has flaws relating to
       accessing strings and so isn't implemented in the NVRAM module */
    case Service_NVRAM:
    {
	int reason = r->r[0];

	if (reason == nvram_CHANGED)
	{
	    const char *tag = (const char *)r->r[2];
	    int old = r->r[3];
	    int new_val = r->r[4];

	    if (old != new_val)	/* since we don't use any string elements */
	    {
		if (strcasecomp(tag, NVRAM_FONTS_TAG) == 0)
		{
		    fe_font_size_set(new_val, TRUE);
		}
		else if (strcasecomp(tag, NVRAM_SOUND_TAG) == 0)
		{
		    fe_bgsound_set(new_val);
		}
		else if (strcasecomp(tag, NVRAM_BEEPS_TAG) == 0)
		{
		    fe_beeps_set(new_val, FALSE);
		}
		else if (strcasecomp(tag, NVRAM_SCALING_TAG) == 0)
		{
		    fe_scaling_set(new_val);
		}
		else if (strcasecomp(tag, NVRAM_PRINT_COLOUR_TAG) == 0)
		{
		    config_print_nocol = !new_val;
		}
	    }
	}

	break;
    }
#endif
    }
}

#define ERROR_BUFSIZE	512

static void check_error(void)
{
    if (pending_error.errnum != -1)
    {
	char *buf = mm_malloc(ERROR_BUFSIZE);
	int n;

	n = sprintf(buf, "ncint:openpanel?name=error&error=E%x&message=", pending_error.errnum);
	url_escape_cat(buf+n, pending_error.errmess, ERROR_BUFSIZE-n);

	if (pending_error_retry)
	{
	    strcat(buf, "&again=");
	    n = strlen(buf);
	    url_escape_cat(buf+n, pending_error_retry, ERROR_BUFSIZE-n);
	}

	frontend_open_url(buf, NULL, TARGET_ERROR, NULL, 0, 0);

	/* clear error flag */
	pending_error.errnum = -1;
	mm_free(buf);
    }

    if (pending_error_retry)
    {
	mm_free(pending_error_retry);
	pending_error_retry = NULL;
    }
}

/* ------------------------------------------------------------------------------------------- */

#if DEBUG
extern void *my_kernel_alloc(unsigned int size);

static void setup_allocs(void)
{
    _kernel_register_allocs(my_kernel_alloc, free);
}
#endif

/* ------------------------------------------------------------------------------------------- */
/* Main event process loop  */
/* ------------------------------------------------------------------------------------------- */

void fe_event_process(void)
{
    wimp_eventstr e;
    wimp_t sender;

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

    if (fe_pending_event)
    {
	fevent_handler(fe_pending_event, main_view);
	fe_pending_event = 0;
    }

    /* do the standard poll loop    */
    if (fast_poll)
    {
/* 	frontend_fatal_error((os_error *)_swix(Wimp_Poll, _INR(0,1) | _OUT(0) | _OUT(2), (wimp_emask)(wimp_EMPTRLEAVE), &e.data, &e.e, &sender)); */
	frontend_fatal_error(wimp_poll((wimp_emask)(wimp_EMPTRLEAVE), &e));
    }
    else
    {
        int next = alarm_timenow() + 10;
        int next_alarm_time;

        if (alarm_next(&next_alarm_time))
        {
            if (next > next_alarm_time)
                next = next_alarm_time;
        }

/* 	frontend_fatal_error((os_error *)_swix(Wimp_PollIdle, _INR(0,2) | _OUT(0) | _OUT(2), (wimp_emask)(wimp_EMPTRLEAVE), &e.data, next, &e.e, &sender)); */
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

	    STBDBGN(("EOPEN: v%p %d,%d %d,%d\n", v, e.data.o.box.x0, e.data.o.box.y0, e.data.o.box.x1, e.data.o.box.y1));

	    if (v && v->pending_mode_change)
	    {
		if (v->parent == NULL)
		{
		    if (!config_display_frames_top_level)
		    {
			v->margin = margin_box;
			adjust_box_for_toolbar(&v->margin);
		    }

		    feutils_resize_window(&v->w, &v->margin, &screen_box, &v->x_scroll_bar, &v->y_scroll_bar, 0, 0, v->scrolling, fe_bg_colour(v));

		    if (v->displaying)
			backend_reset_width(v->displaying, 0);
		}

		v->pending_mode_change = FALSE;
		if (v->w)
		    fe_refresh_window(v->w, NULL);
	    }
	    else if (toolbar_pending_mode_change && use_toolbox && e.data.o.w == tb_status_w())
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
            fe_view v;

	    /* see if key was from handset and update last_key */
	    keywatch_from_handset = FALSE;

	    if (keywatch_pollword)
	    {
		STBDBGN(( "key: last key %d pollword %d", keywatch_last_key_val, *keywatch_pollword));

		if (keywatch_last_key_val == *keywatch_pollword)
		    keywatch_from_handset = TRUE;

		keywatch_last_key_val = *keywatch_pollword;
	    }

	    /* process key press */
	    v = find_view(e.data.key.c.w);
	    if (!v)
		v = fe_selected_view();
	    if (!v)
		v = main_view;

	    STBDBG(( "\nkey: view %p '%s' key %x handset %d offline %d browser mode %d\n", v, v && v->name ? v->name : "", e.data.key.chcode, keywatch_from_handset, keyboard_state, v->browser_mode));

	    fe_key_handler(v, &e, use_toolbox,
			   keyboard_state == fe_keyboard_OFFLINE ? fe_browser_mode_DESKTOP :
			   v->browser_mode);
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

	    STBDBGN(("losecaret: w %x v %p kw %p\n", e.data.c.w, v, keywatch_pollword));

	    if (v)
            {
		/* remove the highlighting from the view but keep a record of where it was (in current_link) */
		backend_remove_highlight(v->displaying);
/*              v->current_link = NULL; */

		/* if it was selected then clear and redraw the frame links */
                if (v->is_selected)
                {
                    fe_view v_top = frameutils_find_top(v);
                    if (v_top && v_top != v && pointer_mode == pointermode_OFF)
			fe_frame_link_clear_all(v);
                }

		if (v->onblur)
		    frontend_open_url(v->onblur, NULL, NULL, NULL, NULL, 0);
	    }
#if 0
	    /* if status bar then remove the highlight from it */
	    else if (e.data.c.w == tb_status_w())
	    {
		tb_status_highlight(FALSE);
	    }
#endif
	    break;
        }

        case wimp_EGAINCARET:
        {
	    fe_view v_new;

	    v_new = find_view(e.data.c.w);

	    STBDBGN(("gaincaret: w %x v %p\n", e.data.c.w, v_new));

	    /* if a new view is getting the input focus */
	    if (v_new)
	    {
		fe_view v_old, v_top;

		v_old = fe_selected_view();

		/* remove the input focus from the last selected view */
		if (v_old)
		{
		    v_old->is_selected = FALSE;

		    /* schedule a redraw on the old link positions */
		    fe_frame_link_redraw_all(v_old);

		    /* then free the links */
		    fe_frame_link_array_free(v_old);

/* 		    v_old->current_link = NULL; */
		}

		/* set the new view as the selected one */
		v_new->is_selected = TRUE;

		/* if this isn't the top view then build and redraw the frame link array */
		v_top = frameutils_find_top(v_new);
		if (v_new != v_top)
		{
		    fe_frame_link_array_build(v_new);
		    if (pointer_mode == pointermode_OFF)
			fe_frame_link_redraw_all(v_new);		/* redraw them all */
		}
	    }
	    else if (use_toolbox && e.data.c.w == tb_status_w() && pointer_mode == pointermode_OFF)
		tb_status_highlight(FALSE);
            break;
        }

        case wimp_EPTRENTER:
	{
	    int mode;
	    fe_view v = find_view(e.data.c.w);

            fe_update_page_info(v);

	    if (v || config_mode_cursor_toolbar)
		fe_get_wimp_caret(e.data.c.w);

	    /* see if iconhigh is running, no iconhigh means not active obviously */
	    if (_swix(IconHigh_GetDirection, _IN(0) | _OUT(3), 0, &mode) != NULL)
		mode = 0;
	    STBDBG(("ptrenter: w%x iconhigh mode %d\n", e.data.c.w, mode));

	    if (mode != 0)
	    {
		/* disable iconhigh */
		_swix(IconHigh_Stop, _IN(0), 0);

		/* turn off pointer */
		fe_pointer_mode_update(pointermode_OFF);

		/* if moving onto the toolbar */
		if (use_toolbox && e.data.c.w == tb_status_w())
		{
		    /* take the highlight */
 		    tb_status_highlight(TRUE);
		}
		else
		{
		    fe_view v;

		    /* if moving onto a view then ensure the highlight is somewhere*/
		    v = find_view(e.data.c.w);
		    if (v)
		    {
			fe_ensure_highlight(v, be_link_INCLUDE_CURRENT);
		    }

		    /* close OSK if open, unless moving onto a
                     * transient like open url or password */
		    if (!v || !v->open_transient)
			fe_keyboard_close();
		}
	    }
	    break;
	}

        case wimp_EPTRLEAVE:
	    /* currently masked out */
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

		case ncfresco_reason_READ_CONFIG:
		    usrtrc("readconfig:\n");
		    re_read_config_data(msg->data.words[1], (const char *)&msg->data.words[2]);
		    break;

		case ncfresco_reason_WRITE_CONFIG:
		    usrtrc("writeconfig:\n");
		    write_config_data(msg->data.words[1], (const char *)&msg->data.words[2]);
		}
		break;

	    case MESSAGE_NCKEYBOARD_WINDOW_SIZE:
		fe_keyboard_set_position((wimp_box *)&msg->data.words[0], msg->hdr.task);
		break;

	    case MESSAGE_NCKEYBOARD_CLOSED:
		fe_keyboard_closed();
		break;

	    case MESSAGE_OFFER_FOCUS:
		offer_window_focus_handler((wimp_w)msg->data.words[0], msg->data.words[1]);
		break;

#if DEBUG
	    case wimp_MINITTASK:
	    {
		int i;
		for (i = 8; i < sizeof(msg->data.chars); i++)
		    if (msg->data.chars[i] < ' ')
		    {
			msg->data.chars[i] = 0;
			break;
		    }

		DBG(("taskinit: %x %dK %s\n", msg->hdr.task, msg->data.words[1]/1024, &msg->data.chars[8]));
		break;
	    }
	    case wimp_MCLOSETASK:
		DBG(("taskclos: %x\n", msg->hdr.task));
		break;
#endif

	    case wimp_MPREQUIT:
		fe_handle_prequit();
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

	    case MESSAGE_ERROR_BROADCAST:
		check_error();
		break;
	    }
	    break;

        case 0x200: /* toolbox events   */
	    STBDBG(("toolbox event\n"));
	    if (use_toolbox)
	    {
		fe_view selected_view = fe_selected_view();
		tb_events((int *)&e.data, selected_view ? selected_view : main_view);
	    }
            break;
    }

    /* only check error here if we are not waiting for the bounce */
    if (config_mode_errors == mode_errors_OWN)
	check_error();

    dbgpoll();
    mm_poll();

#if DEBUG
    {
	extern int stack_extensions;
	static int old_stack_extensions = 0;
	if (old_stack_extensions != stack_extensions)
	{
	    DBG(("stack_extensions: %d\n", stack_extensions));
	    old_stack_extensions = stack_extensions;
	}
    }
#endif
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
    sprintf( er.errmess,
             msgs_lookup("fatal1:"),
             signal);

#if DEVELOPMENT
    usrtrc("fatal signal %d\n", signal);
    usrtrc("by '%s' from '%s'\n", caller(1), caller(2));
#endif
    _swix(Wimp_ReportError, _INR(0,5), &er, wimp_EOK /*| (1<<8)*/, PROGRAM_NAME, NULL, NULL, 0);
/*    wimp_reporterror(&er, wimp_EOK, PROGRAM_NAME); */
/*    frontend_fatal_error(&er); */

   /* if a debug version then return here so that there is the chance of a backtrace */
#if DEBUG == 0
   exit(EXIT_FAILURE);
#endif
}

static void signal_init(void)
{
    DBG(("signal_init\n"));
    oldhandler = signal(SIGABRT, &handler);
    oldhandler = signal(SIGFPE, &handler);
    oldhandler = signal(SIGILL, &handler);
    oldhandler = signal(SIGINT, &escape_handler);
    oldhandler = signal(SIGSEGV, &handler);
    oldhandler = signal(SIGTERM, &handler);
}

static int my_wimp_initialise(int *message_list, int *wimp_version)
{
    os_regset r;
    r.r[0] = *wimp_version;
    r.r[1] = TASK_MAGIC;
    r.r[2] = (int) program_title;
    r.r[3] = (int) message_list;

    frontend_fatal_error(os_swix(Wimp_Initialise,&r));

    *wimp_version = r.r[0];

    return r.r[1];
}

/* ------------------------------------------------------------------------------------------- */

static void fe_tidyup(void)
{
    DBG(( "\n*** Exit function called ***\n\n"));

    MemCheck_SetChecking(0,0);

#if USE_DIALLER_STATUS
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle);
#endif
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_SmartCard, task_handle);
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_ShutdownComplete, task_handle);
    _swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_Standby, task_handle);

    /* disable keywatch stuff */
    if (keywatch_pollword)
    {
	frontend_complain((os_error*)_swix(KeyWatch_Unregister, _IN(0), keywatch_pollword));
	keywatch_pollword = NULL;
    }

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
    MESSAGE_PLUGIN_BUSY,

    MESSAGE_NCKEYBOARD_WINDOW_SIZE,
    MESSAGE_NCKEYBOARD_CLOSED,

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

    MESSAGE_ERROR_BROADCAST,

#if DEBUG
    wimp_MINITTASK,
    wimp_MCLOSETASK,
#endif

    wimp_MMODECHANGE,
    wimp_PALETTECHANGE,
    wimp_MSERVICE,
    MESSAGE_OFFER_FOCUS,
    wimp_MPREQUIT,
    wimp_MCLOSEDOWN
};

static BOOL fe_initialise(void)
{
    os_error *e;
    int t;
    char buffer[32];

    /* Initialise the WIMP stuff */
    visdelay_init();
    visdelay_begin();

    signal_init();

    dbginit();

    _swix(Wimp_ReadSysInfo, _IN(0)|_OUT(0), 7, &wimp_version);
    if (wimp_version < 380)
	wimp_version = 350;
    else
	wimp_version = 380;

#if !MEMLIB
    /* Now bring up the flex system->.. */
    flex_init(program_name);
#else
    strncpysafe(buffer, program_name, sizeof(buffer));
    strlencat(buffer, " data", sizeof(buffer));
    MemFlex_Initialise2(buffer);

    strncpysafe(buffer, program_name, sizeof(buffer));
    strlencat(buffer, " heap", sizeof(buffer));
    MemHeap_Initialise(buffer);
#endif

    if (use_toolbox)
    {
        task_handle = tb_init(message_codes, &wimp_version);

        tb_res_init(program_name);
        tb_resspr_init();
        tb_msgs_init();
    }
    else
    {
        task_handle = my_wimp_initialise(message_codes, &wimp_version);

        res_init(program_name);
        resspr_init();
        msgs_init();
    }

    STBDBG(( "task handle: %x\n", task_handle));

#if !MEMLIB
    heap_init(program_name);
#else
    strncpysafe(buffer, program_name, sizeof(buffer));
    strlencat(buffer, " image ws", sizeof(buffer));
    heap_init(buffer);
#endif

    atexit(&fe_tidyup);

/*  gbf_flags &= ~GBF_TRANSLATE_UNDEF_CHARS; this needs to not be defined to build a japanese version */

    read_gbf();

    /* Init our configuration */
    config_set_filter(fe_config_filter);
    config_init();

    /* Init the NVRAM based configuration */
    if (nvram_read(NVRAM_PRINT_COLOUR_TAG, &t))
	config_print_nocol = !t;

    if (nvram_read(NVRAM_PRINT_ORIENTATION_TAG, &t))
	config_print_sideways = t;

    if (nvram_read(NVRAM_PRINT_BG_TAG, &t))
	config_print_nobg = !t;

    if (nvram_read(NVRAM_PRINT_IMAGES_TAG, &t))
	config_print_nopics = !t;

    if (nvram_read(NVRAM_DISPLAY_BG_TAG, &t))
	config_display_body_colours = t;

    if (nvram_read(NVRAM_DISPLAY_IMAGES_TAG, &t))
	config_defer_images = !t;

    if (nvram_read(NVRAM_FONTS_TAG, &config_display_scale))
	config_display_scale = config_display_scales[config_display_scale];

    nvram_read(NVRAM_BEEPS_TAG, &config_sound_fx);
    nvram_read(NVRAM_SOUND_TAG, &config_sound_background);

    nvram_read(NVRAM_SCALING_TAG, &config_display_scale_fit);
    if (config_display_scale_fit)
	gbf_flags |= GBF_AUTOFIT;
    else
	gbf_flags &= ~GBF_AUTOFIT;

    switch (config_display_scale_fit_mode)
    {
    case 0:
	gbf_flags &= ~GBF_AUTOFIT_ALL_TEXT;
	break;
    case 1:
	gbf_flags |= GBF_AUTOFIT_ALL_TEXT;
	break;
    }

    gbf_init();

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

    /* init toolbar state using the buttons field of config */
    user_status_open = config_display_control_buttons;

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
    frontend_complain(webfonts_initialise());

    if (!use_toolbox)
	statuswin_sprite_init();

    {
        fe_frame_info info;
	int i;
	wimp_box *size;

	memset(&info, 0, sizeof(info));

        info.name = TARGET_VERY_TOP;
        info.noresize = TRUE;

	if (config_display_frames_top_level)
	{
	    info.scrolling = fe_scrolling_INVISIBLE;
	    size = &text_safe_box;
	}
	else
	{
	    info.scrolling = fe_scrolling_INVISIBLE;
	    *(wimp_box *)&info.margin = margin_box;
	    size = &screen_box;
	}

	for (i = 0; i < 4; i++)
	    info.dividers[i] = i;

        frontend_fatal_error(fe_new_view(NULL, size, &info, TRUE, &main_view));
        main_view->status_open = TRUE;
    }

    /* enable keywatch and initialise last_key_val */
    if (_swix(KeyWatch_Register, _IN(0) | _OUT(0), 0, &keywatch_pollword) == NULL && keywatch_pollword)
    {
	keywatch_last_key_val = *keywatch_pollword;
    }

    fe_get_wimp_caret(main_view->w);

#if USE_DIALLER_STATUS
    e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle);
    if (!e)
#endif
	e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_SmartCard, task_handle);
    if (!e)
	e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_ShutdownComplete, task_handle);
    if (!e)
	e = (os_error *)_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_Standby, task_handle);
#if DEBUG
    if (e)
    {
        STBDBG(( "taskmodule: %x %s\n", e->errnum, e->errmess));
    }
#endif

    fe_pointer_mode_update(pointermode_OFF);
    frontend_fatal_error(wimp_get_point_info(&pointer_last_pos));

    visdelay_end();

    return 1;
}


/* ------------------------------------------------------------------------------------------- */

#if STBWEB_ROM && 0
/* int __root_stack_size = 64*1024; */
int __root_stack_size = 4*1024;
/* extern int disable_stack_extension; */
#endif

#undef DBG
#define DBG(a) usrtrc(a)

int main(int argc, char **argv)
{
    int init_ok;

#if DEBUG
    setup_allocs();
#endif

/* #if STBWEB_ROM */
/*     disable_stack_extension = 1; */
/* #endif */

#ifdef HierProf_PROFILE
    HierProf_ProfileAllFunctions();
#endif

    setlocale(LC_ALL, "");
    setbuf(stderr, NULL);   /* no caching   */

    MemCheck_Init();
#if 0
    MemCheck_RegisterArgs(argc, argv);
    MemCheck_InterceptSCLStringFunctions();
    MemCheck_SetAutoOutputBlocksInfo(FALSE);
    MemCheck_RegisterMiscBlock(0, 0); /* stop memcpy(x, 0, 0) from giving an error */
#endif
    MemCheck_SetStoreMallocFunctions(FALSE);
    MemCheck_SetChecking(0,0);

#ifdef Trace_TRACE
/*     Trace_SetHandlers(Trace_Stacker_StartHandler, Trace_Stacker_StopHandler); */
    Trace_IgnoreFunctions("*dbg *dbgn caller");
/*     Trace_InterceptAllFunctions(); */
/*   Trace_Stacker_SetOutputFunction((Trace_Stacker_outputfn)vfdbg, db_sess); */
#endif
    DBG(("main\n"));
    init_usrtrc();

    DBG(("main0\n"));

    progname = argv[0];
    argv++;
    argc--;

#if STBWEB_ROM
    use_toolbox = TRUE;
#else
    {
	char *s = getenv(PROGRAM_NAME"$UseTB");
	use_toolbox = s && atoi(s);
    }
#endif

    DBG(("main2\n"));
    os_cli("pointer 0");

    DBG(("fe_initialise+\n"));
    init_ok = fe_initialise();
    DBG(("fe_initialise-\n"));

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
    		    welcome_url = mm_strdup(argv[0]);
    		}
    		else
                    frontend_complain(fe_file_to_url(argv[0], &welcome_url));
            }

	    argv++;
	    argc--;
	}

        if (welcome_url)
        {
            frontend_complain(frontend_open_url(welcome_url, main_view, NULL, NULL, NULL, fe_open_url_NO_REFERER));
            mm_free(welcome_url);
            welcome_url = NULL;
        }
        else
        {
	    frontend_complain(fe_internal_open_page(main_view, "home", FALSE));
        }

	/* The main event loop */
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
