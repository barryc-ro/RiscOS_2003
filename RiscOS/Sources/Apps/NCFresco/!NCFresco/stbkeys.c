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

#include "fevents.h"

/* ------------------------------------------------------------------------------------- */

#define ctrl(a) ((a) - ('`'))

/* ------------------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------------------- */

#define kbd_handset_HELP    0x1C0
#define kbd_handset_MENU    0x1C1
#define kbd_handset_OPEN    0x1C2
#define kbd_handset_STOP    0x1C3
#define kbd_handset_BACK    0x1C4
#define kbd_handset_FORWARD 0x1C5
#define kbd_handset_TOOLBAR 0x1C6
#define kbd_handset_HOME    0x1C7
#define kbd_handset_PRINT   0x1C8

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
    { akbd_LeftK,           fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_SCROLL_RIGHT, key_list_REPEAT },
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

static key_list nc_toolbar_keys[] =
{
    { akbd_LeftK,           fevent_TOOLBAR_MOVE_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_TOOLBAR_MOVE_RIGHT, key_list_REPEAT },
    { akbd_UpK,             fevent_TOOLBAR_MOVE_UP, key_list_REPEAT },
    { akbd_DownK,           fevent_TOOLBAR_MOVE_DOWN, key_list_REPEAT },

    { 13,                   fevent_TOOLBAR_ACTIVATE },

    { akbd_PageUpK,         fevent_SCROLL_PAGE_UP, key_list_REPEAT },
    { akbd_PageDownK,       fevent_SCROLL_PAGE_DOWN, key_list_REPEAT },

    { akbd_Sh + akbd_Ctl + akbd_UpK,    fevent_SCROLL_UP, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_DownK,  fevent_SCROLL_DOWN, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_LeftK,  fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_Sh + akbd_Ctl + akbd_RightK, fevent_SCROLL_RIGHT, key_list_REPEAT },

    { 0 }
};

/* ------------------------------------------------------------------------------------- */

static key_list platform_riscos_keys[] =
{
    { akbd_Ctl + akbd_Fn+2,		    fevent_CLOSE+fevent_WINDOW },
    { akbd_Sh + akbd_Ctl + akbd_Fn + 1,	    fevent_GLOBAL_TOGGLE_ANTI_TWITTER },
    { akbd_Sh + akbd_Ctl + akbd_Fn11,	    fevent_GLOBAL_OPEN_MEM_DUMP },

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

static key_list rca_menu_keys[] =
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

static key_list rca_web_keys[] =
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

static key_list rca_desktop_keys[] =
{
    { 27,                   fevent_DBOX_CANCEL },
    { akbd_Fn+4,            fevent_DBOX_CANCEL },

    { 0 }
};

static key_list rca_resizing_keys[] =
{
    { 27, fevent_GLOBAL_RESIZE_ABORT },
    { 0 }
};

/* mode keyboard = FALSE */   

static key_list rca_movement2_keys[] =
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

static key_list rca_movement1_keys[] =
{
    { akbd_LeftK,           fevent_SCROLL_LEFT, key_list_REPEAT },
    { akbd_RightK,          fevent_SCROLL_RIGHT, key_list_REPEAT },
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

static key_list rca_codec_keys[] =
{
    { akbd_Sh + akbd_Ctl + akbd_Fn+1, fevent_CODEC_STOP },
    { akbd_Sh + akbd_Ctl + akbd_Fn+2, fevent_CODEC_PLAY },
    { akbd_Sh + akbd_Ctl + akbd_Fn+3, fevent_CODEC_PAUSE },
    { akbd_Sh + akbd_Ctl + akbd_Fn+4, fevent_CODEC_REWIND },
    { akbd_Sh + akbd_Ctl + akbd_Fn+5, fevent_CODEC_FAST_FORWARD },
    { akbd_Sh + akbd_Ctl + akbd_Fn+6, fevent_CODEC_RECORD },
    { akbd_Sh + akbd_Ctl + akbd_Fn+7, fevent_CODEC_MUTE },

    { 0 }
};

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

    if (v == NULL && cs->w == tb_status_w())
    {
        event = fe_key_lookup(chcode, nc_toolbar_keys);
    }

    if (resizing_view)
    {
        event = fe_key_lookup(chcode, nc_resizing_keys);
        if (event == -1)
            event = 0;
    }

    if (event == -1 && stbmenu_is_open())
    {
        event = fe_key_lookup(chcode, use_toolbox ? nc_menu_keys : stb_menu_keys);
        if (event == -1)
            event = 0;
    }

    if (event == -1 && fe_map_view() != NULL)
    {
        event = fe_key_lookup(chcode, use_toolbox ? nc_map_keys : stb_map_keys);
        if (event == -1)
            event = 0;
    }

    if (event == -1 && fe_writeable_handle_keys(v, chcode))
    {
        event = 0;  /* key handled, no event */
    }

    if (event == -1 && (use_toolbox && cs->w == tb_status_w()))
    {
        switch (chcode)
        {
            case akbd_LeftK:
            case akbd_RightK:
                event = 0;
                break;
        }
    }

    if (event == -1)
    {
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

        if (use_toolbox)
        {
            if (event == -1)
                event = fe_key_lookup(chcode, config_mode_keyboard ? nc_movement1_keys : nc_movement2_keys);

            if (event == -1) switch (browser_mode)
            {
                case fe_browser_mode_DESKTOP:
                case fe_browser_mode_DBOX:
                case fe_browser_mode_APP:
                    event = fe_key_lookup(chcode, nc_desktop_keys);
                    break;

                case fe_browser_mode_WEB:
                case fe_browser_mode_HOTLIST:
                case fe_browser_mode_HISTORY:
                    event = fe_key_lookup(chcode, nc_web_keys);
                    break;
            }
        }
        else
        {
            if (event == -1)
                event = fe_key_lookup(chcode, config_mode_keyboard ? stb_movement1_keys : stb_movement2_keys);

            if (event == -1)
                event = fe_key_lookup(chcode, stb_function_keys);
        }
    }

    /* check for the handset transport keys */
    if (event == -1)
	event = fe_key_lookup(chcode, rca_codec_keys);
    
    /* call event handler or pass on */
    if (event != -1)
        fevent_handler(event, v ? v : main_view);
    else
        wimp_processkey(chcode);
}

/* ------------------------------------------------------------------------------------- */

/* eof stbkeys.c */
