/* > stbkeys.c
 *
 */

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

#include "akbd.h"
#include "wimp.h"

#include "config.h"

#include "interface.h"
#include "stbfe.h"
#include "stbmenu.h"
#include "stbtb.h"
#include "stbopen.h"

#include "frameutils.h"

#include "fevents.h"
#include "util.h"

/* ------------------------------------------------------------------------------------- */

#define ctrl(a) ((a) - ('`'))

/* Model 1 handset special codes */

#define kbd_handset_HELP    0x1C0
#define kbd_handset_MENU    0x1C1
#define kbd_handset_OPEN    0x1C2
#define kbd_handset_STOP    0x1C3
#define kbd_handset_BACK    0x1C4
#define kbd_handset_FORWARD 0x1C5
#define kbd_handset_TOOLBAR 0x1C6
#define kbd_handset_HOME    0x1C7
#define kbd_handset_PRINT   0x1C8

#define kbd_rca_POWER		0x1C9
#define kbd_rca_FUNC		0x1CE
#define kbd_rca_SOUND		0x1CF
#define kbd_rca_WHO		0x1D0
#define kbd_rca_RELOAD		0x1D1
#define kbd_rca_FAV		0x1D2
#define kbd_rca_SAVE		0x1D3
#define kbd_rca_GOTO		0x1D4
#define kbd_rca_SEARCH		0x1D5
#define kbd_rca_SEND		0x1D6
#define kbd_rca_MAIL		0x1D7
#define kbd_rca_INFO		0x1D8
#define kbd_rca_PAUSE		0x1D9
#define kbd_rca_REV		0x1DE
#define kbd_rca_PLAY		0x1DF
#define kbd_rca_FWD		0x1E0
#define kbd_rca_REC		0x1E1
#define kbd_rca_GUIDE		0x1E2

#define key_mode_STB		0
#define key_mode_MODEL_1	1
#define key_mode_MODEL_100	2

/* ------------------------------------------------------------------------------------- */

/* STB key sets */

static key_list stb_map_keys[] =
{
    { akbd_LeftK,       fevent_MAP_MOVE + fevent_MAP_MOVE_LEFT, key_list_REPEAT },
    { akbd_RightK,      fevent_MAP_MOVE + fevent_MAP_MOVE_RIGHT, key_list_REPEAT },
    { akbd_UpK,         fevent_MAP_MOVE + fevent_MAP_MOVE_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MAP_MOVE + fevent_MAP_MOVE_DOWN, key_list_REPEAT },
    { akbd_PageUpK,     fevent_MAP_MOVE + fevent_MAP_MOVE_UP + fevent_MAP_MOVE_RIGHT, key_list_REPEAT },
    { akbd_PageDownK,   fevent_MAP_MOVE + fevent_MAP_MOVE_DOWN + fevent_MAP_MOVE_RIGHT, key_list_REPEAT },
    { 30,               fevent_MAP_MOVE + fevent_MAP_MOVE_UP + fevent_MAP_MOVE_LEFT, key_list_REPEAT },
    { akbd_CopyK,       fevent_MAP_MOVE + fevent_MAP_MOVE_DOWN + fevent_MAP_MOVE_LEFT, key_list_REPEAT },
    { 13,               fevent_MAP_SELECT,  key_list_CLICK },
    { akbd_Fn11,        fevent_MAP_STOP,    key_list_CLICK },
    { akbd_Fn+4,        fevent_MAP_CANCEL,  key_list_CLICK },
    { 0 }
};

static key_list stb_menu_keys[] =
{
    { akbd_UpK,         fevent_MENU_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MENU_DOWN, key_list_REPEAT },
    { akbd_PageUpK,     fevent_MENU_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,   fevent_MENU_PAGE_DOWN, key_list_REPEAT },
    { akbd_Ctl + akbd_UpK,      fevent_MENU_TOP },
    { akbd_Ctl + akbd_DownK,    fevent_MENU_BOTTOM },
    { 13,               fevent_MENU_SELECT, key_list_CLICK },
    { akbd_LeftK,       fevent_MENU_TOGGLE, key_list_CLICK },
    { akbd_RightK,      fevent_MENU_TOGGLE, key_list_CLICK },
    { 0 }
};

/* STB2 IR */
static key_list stb_movement1_keys[] =
{
    { akbd_LeftK,       fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_RightK,      fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
    { akbd_UpK,         fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_HIGHLIGHT_DOWN, key_list_REPEAT },

    { akbd_Fn+5,        fevent_SCROLL_PAGE_UP, key_list_REPEAT },    /* Previous */
    { akbd_PageUpK,     fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_Fn+7,        fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },  /* Next */
    { akbd_PageDownK,   fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },
    { akbd_Fn+8,        fevent_SCROLL_UP, key_list_REPEAT },         /* Rewind */
    { akbd_Fn10,        fevent_SCROLL_DOWN, key_list_REPEAT },       /* FFwd */
    { akbd_Sh + akbd_Fn+3,  fevent_SCROLL_LEFT, key_list_REPEAT },   /* Yellow */
    { akbd_Sh + akbd_Fn+4,  fevent_SCROLL_RIGHT, key_list_REPEAT },  /* Blue */

    { akbd_Sh + akbd_Fn+2 , fevent_HOTLIST_SHOW+fevent_WINDOW,    key_list_CLICK },

    { 0 }
};

/* STB2 keyboard */
static key_list stb_movement2_keys[] =
{
    { akbd_LeftK,       fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_RightK,      fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
    { akbd_UpK,         fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_HIGHLIGHT_DOWN, key_list_REPEAT },

    { akbd_Fn+5,        fevent_SCROLL_PAGE_UP, key_list_REPEAT },    /* Previous */
    { akbd_PageUpK,     fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_Fn+7,        fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },  /* Next */
    { akbd_PageDownK,   fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },
    { akbd_Fn+8,        fevent_SCROLL_UP, key_list_REPEAT },         /* Rewind */
    { akbd_Fn10,        fevent_SCROLL_DOWN, key_list_REPEAT },       /* FFwd */
    { akbd_Sh + akbd_Fn+3,  fevent_SCROLL_LEFT, key_list_REPEAT },   /* Yellow */
    { akbd_Sh + akbd_Fn+4,  fevent_SCROLL_RIGHT, key_list_REPEAT },  /* Blue */

    { akbd_Sh + akbd_Fn+2 , fevent_HOTLIST_SHOW_WITH_URL+fevent_WINDOW,    key_list_CLICK },
    { 0 }
};

static key_list stb_function_keys[] =
{
    { akbd_Sh + akbd_Fn+1,  fevent_HOTLIST_ADD,     key_list_CLICK },
    { akbd_Sh + akbd_Fn+2 , fevent_HOTLIST_SHOW+fevent_WINDOW,    key_list_CLICK },
    { akbd_Fn+1,            fevent_GLOBAL_SHOW_HELP, key_list_CLICK },
    { akbd_Fn+2,            fevent_TOGGLE_STATUS,   key_list_CLICK },
    { akbd_Fn+3,            fevent_HOME+fevent_WINDOW,            key_list_CLICK },
    { akbd_Fn+4,            fevent_HISTORY_BACK+fevent_WINDOW,  key_list_CLICK },
    { akbd_Fn+6,            fevent_RELOAD,          key_list_CLICK },
    { akbd_Fn+9,            fevent_HISTORY_SHOW+fevent_WINDOW,    key_list_CLICK },
    { akbd_Fn11,            fevent_STOP_LOADING+fevent_WINDOW,    key_list_CLICK },
    { akbd_PrintK,          fevent_PRINT,           key_list_CLICK },

    { '1',                  fevent_STATUS_INFO_LEVEL+0, key_list_CLICK },
    { '2',                  fevent_STATUS_INFO_LEVEL+1, key_list_CLICK },
    { '3',                  fevent_STATUS_INFO_LEVEL+2, key_list_CLICK },
    { '5',		    fevent_GLOBAL_SHOW_VERSION, key_list_CLICK },
    { '6',                  fevent_TOGGLE_FRAMES,       key_list_CLICK },
    { '8',                  fevent_TOGGLE_COLOURS,      key_list_CLICK },
    { '9',                  fevent_GLOBAL_TOGGLE_ANTI_TWITTER,  key_list_CLICK },
    { 27,                   fevent_GLOBAL_QUIT },

    { ctrl('d'),            fevent_GLOBAL_SHOW_VERSION },

    { akbd_Ctl + akbd_Fn+1, fevent_HOTLIST_REMOVE+fevent_WINDOW,      key_list_CLICK },
    { 30,                   fevent_HOME,                key_list_CLICK },
    { akbd_Ctl + akbd_Fn+4, fevent_HISTORY_FORWARD+fevent_WINDOW,  key_list_CLICK },

    { 13,                   fevent_HIGHLIGHT_ACTIVATE,  key_list_CLICK },

    { 0 }
};

#define stb_web_keys		stb_function_keys
#define stb_desktop_keys	stb_function_keys
#define stb_codec_keys		0
#define stb_resizing_keys	0
#define stb_toolbar_keys	0
#define stb_osk_keys		0
#define stb_frame_link_keys	0
#define stb_external_popup_keys	0

/* ------------------------------------------------------------------------------------- */

/* NC Model 1 key sets */

static key_list nc_map_keys[] =
{
    { akbd_LeftK,       fevent_MAP_MOVE + fevent_MAP_MOVE_LEFT, key_list_REPEAT },
    { akbd_RightK,      fevent_MAP_MOVE + fevent_MAP_MOVE_RIGHT, key_list_REPEAT },
    { akbd_UpK,         fevent_MAP_MOVE + fevent_MAP_MOVE_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MAP_MOVE + fevent_MAP_MOVE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT, key_list_REPEAT },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_LeftK, fevent_SCROLL_PAGE_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_RightK,fevent_SCROLL_PAGE_RIGHT, key_list_REPEAT },

    { akbd_Ctl + akbd_UpK,  fevent_SCROLL_TOP },
    { akbd_Ctl + akbd_DownK,fevent_SCROLL_BOTTOM },
    { akbd_Ctl + akbd_LeftK,  fevent_SCROLL_FAR_LEFT },
    { akbd_Ctl + akbd_RightK,fevent_SCROLL_FAR_RIGHT },

    { 13,               fevent_MAP_SELECT },
    { 27,               fevent_MAP_CANCEL },

    { akbd_Fn+4,        fevent_MAP_CANCEL },
    { kbd_handset_BACK, fevent_MAP_CANCEL },
    { kbd_handset_STOP, fevent_MAP_CANCEL },
    { kbd_handset_HOME, fevent_HOME + fevent_WINDOW },
    { kbd_handset_OPEN, fevent_HOTLIST_SHOW_WITH_URL + fevent_WINDOW },
    { 0 }
};

static key_list nc_menu_keys[] =
{
    { akbd_UpK,         fevent_MENU_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MENU_DOWN, key_list_REPEAT },
    { akbd_PageUpK,     fevent_MENU_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,   fevent_MENU_PAGE_DOWN, key_list_REPEAT },
    { akbd_Ctl + akbd_UpK,      fevent_MENU_TOP },
    { akbd_Ctl + akbd_DownK,    fevent_MENU_BOTTOM },
    { 13,               fevent_MENU_SELECT },
    { 27,               fevent_MENU_CANCEL },
    { akbd_LeftK,       fevent_MENU_TOGGLE, key_list_CLICK },
    { akbd_RightK,      fevent_MENU_TOGGLE, key_list_CLICK },

    { akbd_Fn+4,        fevent_MENU_CANCEL },
    { kbd_handset_BACK, fevent_MENU_CANCEL },
    { kbd_handset_STOP, fevent_MAP_CANCEL },
    { kbd_handset_HOME, fevent_HOME + fevent_WINDOW },
    { kbd_handset_OPEN, fevent_HOTLIST_SHOW_WITH_URL + fevent_WINDOW },
    { 0 }
};

static key_list nc_web_keys[] =
{
    { akbd_Fn+1,            fevent_GLOBAL_SHOW_HELP },
    { akbd_Fn+2,            fevent_MENU },
    { akbd_Fn12,            fevent_TOGGLE_STATUS },
    { ctrl('p'),            fevent_PRINT },
    { ctrl('f'),            fevent_OPEN_FIND },
    { 27,                   fevent_STOP_LOADING+fevent_WINDOW },

    { kbd_handset_HELP,             fevent_GLOBAL_SHOW_HELP },
    { kbd_handset_MENU,             fevent_MENU },
    { kbd_handset_OPEN,             fevent_HOTLIST_SHOW_WITH_URL+fevent_WINDOW },
    { kbd_handset_STOP,             fevent_STOP_LOADING+fevent_WINDOW },
    { akbd_Ctl+kbd_handset_STOP,    fevent_STOP_LOADING },
    { kbd_handset_BACK,             fevent_HISTORY_BACK+fevent_WINDOW },
    { akbd_Ctl+kbd_handset_BACK,    fevent_HISTORY_BACK },
    { kbd_handset_FORWARD,          fevent_HISTORY_FORWARD+fevent_WINDOW },
    { akbd_Ctl+kbd_handset_FORWARD, fevent_HISTORY_FORWARD },
    { kbd_handset_TOOLBAR,          fevent_TOGGLE_STATUS },
    { kbd_handset_HOME,		    fevent_HOME + fevent_WINDOW },
    { kbd_handset_PRINT,	    fevent_PRINT },
    { akbd_Ctl + kbd_handset_PRINT, fevent_OPEN_PRINT_OPTIONS },

    { akbd_Fn+3,            fevent_HOTLIST_SHOW_WITH_URL+fevent_WINDOW },
    { akbd_Fn+4,            fevent_HISTORY_BACK+fevent_WINDOW },
    { akbd_Sh+akbd_Fn+4,    fevent_HISTORY_BACK },
    { akbd_Fn+5,            fevent_HISTORY_FORWARD+fevent_WINDOW },
    { akbd_Sh+akbd_Fn+5,    fevent_HISTORY_FORWARD },
    { akbd_Fn+6,            fevent_HISTORY_SHOW+fevent_WINDOW },

    { ctrl('a'),            fevent_HOTLIST_ADD+fevent_WINDOW },
    { ctrl('s'),            fevent_HOTLIST_REMOVE+fevent_WINDOW },

    { akbd_Ctl + akbd_PrintK,fevent_OPEN_PRINT_OPTIONS },
    { ctrl('g'),            fevent_FIND_AGAIN },
    { ctrl('r'),            fevent_RELOAD + fevent_WINDOW },
    { ctrl('d'),            fevent_GLOBAL_SHOW_VERSION },
    { ctrl('l'),            fevent_PASTE_URL },

    { akbd_Ctl + akbd_Fn+3, fevent_TOGGLE_COLOURS },
    { akbd_Ctl + akbd_Fn+4, fevent_TOGGLE_IMAGES },

    { 0 }
};

static key_list nc_desktop_keys[] =
{
    { 27,                   fevent_DBOX_CANCEL },
    { akbd_Fn+4,            fevent_DBOX_CANCEL },

    { 0 }
};

static key_list nc_resizing_keys[] =
{
    { 27, fevent_GLOBAL_RESIZE_ABORT },
    { 0 }
};

/* mode keyboard = FALSE */

static key_list nc_movement2_keys[] =
{
    { akbd_Sh + akbd_TabK,              fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_TabK,                        fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_HIGHLIGHT_DOWN, key_list_REPEAT },
    { 13,                   fevent_HIGHLIGHT_ACTIVATE },

    { akbd_Ctl + akbd_UpK,  fevent_SCROLL_TOP },
    { akbd_Ctl + akbd_DownK,fevent_SCROLL_BOTTOM },
    { akbd_Ctl + akbd_LeftK,  fevent_SCROLL_FAR_LEFT },
    { akbd_Ctl + akbd_RightK,fevent_SCROLL_FAR_RIGHT },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },

    { akbd_UpK,             fevent_SCROLL_OR_CURSOR_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_SCROLL_OR_CURSOR_DOWN, key_list_REPEAT },
    { akbd_LeftK,           fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_SCROLL_RIGHT, key_list_REPEAT },

    { 0 }
};

/* mode keyboard = TRUE */

static key_list nc_movement1_keys[] =
{
#if 0
    { akbd_LeftK,           fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_SCROLL_RIGHT, key_list_REPEAT },
#else
    { akbd_LeftK,           fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_RightK,          fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
#endif
    { akbd_UpK,             fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_HIGHLIGHT_DOWN, key_list_REPEAT },
    { 13,                   fevent_HIGHLIGHT_ACTIVATE },

    { akbd_Sh + akbd_Ctl + akbd_TabK, fevent_HIGHLIGHT_PREV_FRAME },
    { akbd_Ctl + akbd_TabK, fevent_HIGHLIGHT_NEXT_FRAME },

    { ctrl('t'), fevent_HIGHLIGHT_PREV_FRAME },
    { ctrl('y'), fevent_HIGHLIGHT_NEXT_FRAME },

    { akbd_Ctl + akbd_UpK,  fevent_SCROLL_TOP },
    { akbd_Ctl + akbd_DownK,fevent_SCROLL_BOTTOM },
    { akbd_Ctl + akbd_LeftK,  fevent_SCROLL_FAR_LEFT },
    { akbd_Ctl + akbd_RightK,fevent_SCROLL_FAR_RIGHT },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT, key_list_REPEAT },

    { akbd_Sh + akbd_TabK,              fevent_SCROLL_OR_CURSOR_UP, key_list_REPEAT },
    { akbd_TabK,                        fevent_SCROLL_OR_CURSOR_DOWN, key_list_REPEAT },

    { 0 }
};

#define nc_toolbar_keys 0
#define nc_codec_keys	0
#define nc_osk_keys	0
#define nc_frame_link_keys	0
#define nc_external_popup_keys	0

/* ------------------------------------------------------------------------------------- */

/* NC100 key sets */

static key_list rca_map_keys[] =
{
    { akbd_LeftK,       fevent_MAP_MOVE + fevent_MAP_MOVE_LEFT, key_list_REPEAT },
    { akbd_RightK,      fevent_MAP_MOVE + fevent_MAP_MOVE_RIGHT, key_list_REPEAT },
    { akbd_UpK,         fevent_MAP_MOVE + fevent_MAP_MOVE_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MAP_MOVE + fevent_MAP_MOVE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT, key_list_REPEAT },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_LeftK, fevent_SCROLL_PAGE_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_RightK,fevent_SCROLL_PAGE_RIGHT, key_list_REPEAT },

    { 13,               fevent_MAP_SELECT },

    { kbd_handset_BACK, fevent_MAP_CANCEL },

    { 30,		fevent_HOME + fevent_WINDOW },
    { kbd_handset_HOME, fevent_HOME + fevent_WINDOW },
    { 0 }
};

static key_list rca_menu_keys[] =
{
    { akbd_UpK,         fevent_MENU_UP, key_list_REPEAT },
    { akbd_DownK,       fevent_MENU_DOWN, key_list_REPEAT },
    { akbd_PageUpK,     fevent_MENU_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,   fevent_MENU_PAGE_DOWN, key_list_REPEAT },

    { 13,               fevent_MENU_SELECT },
    { akbd_LeftK,       fevent_MENU_TOGGLE, key_list_CLICK },
    { akbd_RightK,      fevent_MENU_TOGGLE, key_list_CLICK },

    { kbd_handset_BACK, fevent_MENU_CANCEL },

    { 30,		fevent_HOME + fevent_WINDOW },
    { kbd_handset_HOME, fevent_HOME + fevent_WINDOW },
    { 0 }
};

static key_list rca_web_keys[] =
{
    { ctrl('a'),			fevent_HOTLIST_ADD + fevent_WINDOW },
    { ctrl('r'),			fevent_RELOAD + fevent_WINDOW },

    { akbd_Fn12,			fevent_TOGGLE_STATUS },

    { 30,				fevent_HOME + fevent_WINDOW },
    { kbd_handset_HOME,			fevent_HOME + fevent_WINDOW },

    { kbd_handset_PRINT,		fevent_PRINT },
    { akbd_PrintK,			fevent_TOOLBAR_PRINT + fevent_WINDOW },

    { kbd_handset_MENU,			fevent_TOOLBAR_DETAILS },

    { kbd_handset_TOOLBAR,          fevent_TOGGLE_STATUS },
    { kbd_handset_OPEN,             fevent_OPEN_URL+fevent_WINDOW },
    { kbd_handset_BACK,             fevent_TOOLBAR_EXIT },
    { kbd_handset_FORWARD,          fevent_HISTORY_FORWARD+fevent_WINDOW },

    { kbd_rca_SOUND,		fevent_SOUND_TOGGLE },
    { kbd_rca_RELOAD,		fevent_RELOAD },

    { kbd_rca_FAV,		fevent_TOOLBAR_FAVS },
    { kbd_rca_SAVE,		fevent_HOTLIST_ADD },
    { kbd_rca_GOTO,		fevent_OPEN_URL },
    { kbd_rca_SEARCH,		fevent_SEARCH_PAGE },
    { kbd_rca_SEND,		fevent_SEND_URL },
    { kbd_rca_INFO,		fevent_TOOLBAR_DETAILS },

    { 0 }
};

static key_list rca_desktop_keys[] =
{
    { kbd_handset_MENU,		fevent_OFFLINE_PAGE },

    { kbd_handset_BACK,         fevent_TOOLBAR_EXIT },

    { kbd_rca_SOUND,		fevent_SOUND_TOGGLE },

    { 0 }
};

static key_list rca_resizing_keys[] =
{
    { 0 }
};

/* mode keyboard = FALSE */

static key_list rca_movement2_keys[] =
{
    { akbd_LeftK,           fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_RightK,          fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
    { akbd_UpK,             fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_HIGHLIGHT_DOWN, key_list_REPEAT },
    { 13,                   fevent_HIGHLIGHT_ACTIVATE },

    { akbd_Ctl + akbd_UpK,  fevent_HIGHLIGHT_FRAME_UP },
    { akbd_Ctl + akbd_DownK,fevent_HIGHLIGHT_FRAME_DOWN },
    { akbd_Ctl + akbd_LeftK,  fevent_HIGHLIGHT_FRAME_LEFT },
    { akbd_Ctl + akbd_RightK,fevent_HIGHLIGHT_FRAME_RIGHT },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT, key_list_REPEAT },

    { akbd_Sh + akbd_TabK,              fevent_SCROLL_OR_CURSOR_UP, key_list_REPEAT },
    { akbd_TabK,                        fevent_SCROLL_OR_CURSOR_DOWN, key_list_REPEAT },

    { 0 }
};

/* mode keyboard = TRUE */

#define rca_movement1_keys	rca_movement2_keys

static key_list rca_codec_keys[] =
{
    { akbd_Sh + akbd_Ctl + akbd_Fn+1, fevent_CODEC_STOP },
    { akbd_Sh + akbd_Ctl + akbd_Fn+2, fevent_CODEC_PLAY },
    { akbd_Sh + akbd_Ctl + akbd_Fn+3, fevent_CODEC_PAUSE },
    { akbd_Sh + akbd_Ctl + akbd_Fn+4, fevent_CODEC_REWIND },
    { akbd_Sh + akbd_Ctl + akbd_Fn+5, fevent_CODEC_FAST_FORWARD },
    { akbd_Sh + akbd_Ctl + akbd_Fn+6, fevent_CODEC_RECORD },
    { akbd_Sh + akbd_Ctl + akbd_Fn+7, fevent_CODEC_MUTE },

    { kbd_handset_STOP, fevent_CODEC_STOP },
    { kbd_rca_PLAY, fevent_CODEC_PLAY },
    { kbd_rca_PAUSE, fevent_CODEC_PAUSE },
    { kbd_rca_REV, fevent_CODEC_REWIND },
    { kbd_rca_FWD, fevent_CODEC_FAST_FORWARD },
    { kbd_rca_REC, fevent_CODEC_RECORD },

    { 0 }
};

static key_list rca_toolbar_keys[] =
{
    { akbd_LeftK,           fevent_TOOLBAR_MOVE_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_TOOLBAR_MOVE_RIGHT, key_list_REPEAT },
    { akbd_Ctl + akbd_UpK,  fevent_TOOLBAR_MOVE_UP, key_list_REPEAT },
    { akbd_UpK,             fevent_TOOLBAR_MOVE_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_TOOLBAR_MOVE_DOWN, key_list_REPEAT },
    { akbd_Ctl + akbd_DownK,fevent_TOOLBAR_MOVE_DOWN, key_list_REPEAT },

    { 13,                   fevent_TOOLBAR_ACTIVATE },

    { akbd_PageUpK,         fevent_TOOLBAR_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_TOOLBAR_PAGE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP + fevent_WINDOW, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN + fevent_WINDOW, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT + fevent_WINDOW, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT + fevent_WINDOW, key_list_REPEAT },

    { 0 }
};

static key_list rca_osk_keys[] =
{
    { 0 }
};

static key_list rca_frame_link_keys[] =
{
    { akbd_LeftK,           fevent_FRAME_LINK_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_FRAME_LINK_RIGHT, key_list_REPEAT },
    { akbd_UpK,             fevent_FRAME_LINK_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_FRAME_LINK_DOWN, key_list_REPEAT },

    { 13,		    fevent_FRAME_LINK_ACTIVATE, 0 },

    { 0 }
};

static key_list rca_external_popup_keys[] =
{
    { akbd_LeftK,           fevent_HIGHLIGHT_BACK, key_list_REPEAT },
    { akbd_RightK,          fevent_HIGHLIGHT_FORWARD, key_list_REPEAT },
    { akbd_UpK,             fevent_HIGHLIGHT_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_HIGHLIGHT_DOWN, key_list_REPEAT },

    { 13,                   fevent_HIGHLIGHT_ACTIVATE },

    { 0 }
};

/* ------------------------------------------------------------------------------------- */

/* Mode dependant special keys */

static key_list platform_riscos_keys[] =
{
    { akbd_Ctl + akbd_Fn+2,		    fevent_CLOSE+fevent_WINDOW },
    { akbd_Sh + akbd_Ctl + akbd_Fn + 1,	    fevent_GLOBAL_TOGGLE_ANTI_TWITTER },
    { akbd_Sh + akbd_Ctl + akbd_Fn11,	    fevent_GLOBAL_OPEN_MEM_DUMP },

    { 27, fevent_TOOLBAR_EXIT },
    { akbd_Fn+4, fevent_HISTORY_BACK+fevent_WINDOW },
    { akbd_Fn+5, fevent_HISTORY_FORWARD+fevent_WINDOW },

    { 0 }
};

static key_list platform_nc_keys[] =
{
    { 0 }
};

static key_list platform_trial_keys[] =
{
    { 0 }
};

/* ------------------------------------------------------------------------------------- */

#define key_map_MOVEMENT	0
#define key_map_DESKTOP		1
#define key_map_CODECS		2
#define key_map_MAP		3
#define key_map_MENU		4
#define key_map_RESIZING	5
#define key_map_TOOLBAR		6
#define key_map_WEB		7
#define key_map_OSK		8
#define key_map_FRAME_LINK	9
#define key_map_EXTERNAL_POPUP	10

static key_list *get_key_map(int map)
{
    switch (config_mode_keyboard/2)
    {
    case key_mode_STB:		/* STB */
	switch (map)
	{
	case key_map_MOVEMENT:
	    return config_mode_keyboard & 1 ? stb_movement2_keys  : stb_movement1_keys;
	case key_map_CODECS:
	    return stb_codec_keys;
	case key_map_MAP:
	    return stb_map_keys;
	case key_map_MENU:
	    return stb_menu_keys;
	case key_map_RESIZING:
	    return stb_resizing_keys;
	case key_map_DESKTOP:
	    return stb_desktop_keys;
	case key_map_TOOLBAR:
	    return stb_toolbar_keys;
	case key_map_WEB:
	    return stb_web_keys;
	case key_map_OSK:
	    return stb_osk_keys;
	case key_map_FRAME_LINK:
	    return stb_frame_link_keys;
	case key_map_EXTERNAL_POPUP:
	    return stb_external_popup_keys;
	}
	break;

    case key_mode_MODEL_1:	/* NC Model 1 */
	switch (map)
	{
	case key_map_MOVEMENT:
	    return config_mode_keyboard & 1 ? nc_movement2_keys  : nc_movement1_keys;
	case key_map_CODECS:
	    return nc_codec_keys;
	case key_map_MAP:
	    return nc_map_keys;
	case key_map_MENU:
	    return nc_menu_keys;
	case key_map_RESIZING:
	    return nc_resizing_keys;
	case key_map_DESKTOP:
	    return nc_desktop_keys;
	case key_map_TOOLBAR:
	    return nc_toolbar_keys;
	case key_map_WEB:
	    return nc_web_keys;
	case key_map_OSK:
	    return nc_osk_keys;
	case key_map_FRAME_LINK:
	    return nc_frame_link_keys;
	case key_map_EXTERNAL_POPUP:
	    return nc_external_popup_keys;
	}
	break;

    case key_mode_MODEL_100:	/* RC Model 100 */
	switch (map)
	{
	case key_map_MOVEMENT:
	    return config_mode_keyboard & 1 ? rca_movement2_keys  : rca_movement1_keys;
	case key_map_CODECS:
	    return rca_codec_keys;
	case key_map_MAP:
	    return rca_map_keys;
	case key_map_MENU:
	    return rca_menu_keys;
	case key_map_RESIZING:
	    return rca_resizing_keys;
	case key_map_DESKTOP:
	    return rca_desktop_keys;
	case key_map_TOOLBAR:
	    return rca_toolbar_keys;
	case key_map_WEB:
	    return rca_web_keys;
	case key_map_OSK:
	    return rca_osk_keys;
	case key_map_FRAME_LINK:
	    return rca_frame_link_keys;
	case key_map_EXTERNAL_POPUP:
	    return rca_external_popup_keys;
	}
	break;
    }

    return NULL;
}

/* ------------------------------------------------------------------------------------- */

/* This view could be null if the caret was in the status bar
 * event = -1 means that the key hasn't been handled so pass it on (processkey)
 * event = 0 means the key has been handled somewhere else so do nothing
 * event > 0 means a real event to handle
 */

void fe_key_handler(fe_view v, wimp_eventstr *e, BOOL use_toolbox, int browser_mode)
{
    wimp_caretstr *cs = &e->data.key.c;
    int chcode = e->data.key.chcode;
    int event = -1;

    if (fe_map_view() == NULL )
	switch (chcode &~ (akbd_Sh | akbd_Ctl))
    {
    case akbd_LeftK:
    case akbd_RightK:
    case akbd_UpK:
    case akbd_DownK:
	fe_pointer_mode_update(pointermode_OFF);
	break;
    }

    if (cs->w == tb_status_w())
    {
        event = fe_key_lookup(chcode, get_key_map(key_map_TOOLBAR));
    }

    if (event == -1 && frameutils_are_we_resizing() )
    {
        event = fe_key_lookup(chcode, get_key_map(key_map_RESIZING));
        if (event == -1)
            event = 0;
    }

    if (event == -1 && stbmenu_is_open())
    {
        event = fe_key_lookup(chcode, get_key_map(key_map_MENU));
        if (event == -1)
            event = 0;
    }

    if (event == -1 && fe_map_view() != NULL)
    {
        event = fe_key_lookup(chcode, get_key_map(key_map_MAP));
        if (event == -1)
            event = 0;
    }

    if (event == -1 && fe_external_popup_open())
    {
	event = fe_key_lookup(chcode, get_key_map(key_map_EXTERNAL_POPUP));
	if (event == -1)
	    event = 0;
    }

    if (event == -1 && on_screen_kbd)
	event = fe_key_lookup(chcode, get_key_map(key_map_OSK));

    if (event == -1 && fe_writeable_handle_keys(v, chcode))
    {
        event = 0;  /* key handled, no event */
    }

    if (event == -1 && fe_frame_link_selected(v))
        event = fe_key_lookup(chcode, get_key_map(key_map_FRAME_LINK));

    if (event == -1)
	event = fe_key_lookup(chcode, get_key_map(key_map_MOVEMENT));

    if (event == -1) switch (browser_mode)
    {
    case fe_browser_mode_DESKTOP:
    case fe_browser_mode_DBOX:
    case fe_browser_mode_APP:
	event = fe_key_lookup(chcode, get_key_map(key_map_DESKTOP));
	break;

    case fe_browser_mode_WEB:
    case fe_browser_mode_HOTLIST:
    case fe_browser_mode_HISTORY:
	event = fe_key_lookup(chcode, get_key_map(key_map_WEB));
	break;
    }

    /* check for special mode dependant keys */
    if (event == -1) switch (config_mode_platform)
    {
    case platform_RISCOS_DESKTOP:
	event = fe_key_lookup(chcode, platform_riscos_keys);
	break;
    case platform_NC:
	event = fe_key_lookup(chcode, platform_nc_keys);
	break;
    case platform_CAMB_TRIAL:
	event = fe_key_lookup(chcode, platform_trial_keys);
	break;
    }

    /* check for the handset transport keys */
    if (event == -1)
	event = fe_key_lookup(chcode, get_key_map(key_map_CODECS));

    /* call event handler or pass on */
    if (event != -1)
        fevent_handler(event, v);
    else
        wimp_processkey(chcode);
}

/* ------------------------------------------------------------------------------------- */

#define stb_key_map nc_key_map

static input_key_map nc_key_map[] =
{
    { 10, key_action_NEWLINE_SUBMIT_ALWAYS },
    { 13, key_action_NEWLINE_SUBMIT_ALWAYS },
    { 8, key_action_DELETE_LEFT },
    { 127, key_action_DELETE_RIGHT },
    { 21, key_action_DELETE_ALL },
    { akbd_Ctl + akbd_CopyK, key_action_DELETE_TO_END },
    { akbd_LeftK, key_action_LEFT_OR_OFF },
    { akbd_RightK, key_action_RIGHT_OR_OFF },
    { 0x1E, key_action_START_OF_LINE },
    { akbd_Ctl + akbd_LeftK, key_action_START_OF_LINE },
    { akbd_CopyK, key_action_END_OF_LINE },
    { akbd_Ctl + akbd_RightK, key_action_END_OF_LINE },
    { akbd_UpK, key_action_UP },
    { akbd_DownK, key_action_DOWN },
    { -1, key_action_NO_ACTION }
};

static input_key_map rca_key_map[] =
{
    { 10, key_action_NEWLINE_SUBMIT_LAST },
    { 13, key_action_NEWLINE_SUBMIT_LAST },
    { 8, key_action_DELETE_LEFT },
    { 127, key_action_DELETE_RIGHT },
    { 4, key_action_DELETE_ALL_AREA },
    { akbd_Ctl + akbd_CopyK, key_action_DELETE_TO_END },
    { akbd_LeftK, key_action_LEFT_OR_OFF },
    { akbd_RightK, key_action_RIGHT_OR_OFF },
    { akbd_Ctl + akbd_LeftK, key_action_START_OF_AREA },
    { akbd_Ctl + akbd_RightK, key_action_END_OF_AREA },
    { akbd_UpK, key_action_UP },
    { akbd_DownK, key_action_DOWN },
    { -1, key_action_NO_ACTION }
};

void stbkeys_init(void)
{
    switch (config_mode_keyboard/2)
    {
    case key_mode_STB:
	set_input_key_map(stb_key_map);
	break;
    case key_mode_MODEL_1:
	set_input_key_map(nc_key_map);
	break;
    case key_mode_MODEL_100:
	set_input_key_map(rca_key_map);
	break;
    }
}

/* ------------------------------------------------------------------------------------- */

/* eof stbkeys.c */
