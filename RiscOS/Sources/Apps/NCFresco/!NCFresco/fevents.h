/* > fevents.h

 *

 */

/* add this to a frame event to make it apply to the whole window */
#define fevent_WINDOW                       0x1000
#define fevent_CLEAR_POPUPS		    0x0800

#define fevent_CLASS_MASK                   0xf000
#define fevent_CLASS_GLOBAL                 0x1000
#define fevent_CLASS_FRAME                  0x2000
#define fevent_CLASS_WINDOW                 (fevent_CLASS_FRAME + fevent_WINDOW)
#define fevent_CLASS_MENU                   0x8000
#define fevent_CLASS_MAP                    0x9000
#define fevent_CLASS_PLUGIN                 0xA000

#define fevent_FRAME_CLASS_MASK		    0x0700
#define fevent_FRAME_CLASS_ACTIONS	    0x0000 /* fevent_SUB_CLASS_MASK */
#define fevent_FRAME_CLASS_URLS		    0x0100 /* 256 URLs */
#define fevent_FRAME_CLASS_ENCODING	    0x0200 /* 256 encodings */

#define fevent_SUB_CLASS_MASK               0x00f0
#define fevent_SUB_CLASS_TOGGLE             0x0000
#define fevent_SUB_CLASS_HISTORY            0x0010
#define fevent_SUB_CLASS_HOTLIST            0x0020
#define fevent_SUB_CLASS_MISC               0x0030
#define fevent_SUB_CLASS_CLIPBOARD          0x0040
#define fevent_SUB_CLASS_SCROLL             0x0050
#define fevent_SUB_CLASS_HIGHLIGHT          0x0060
#define fevent_SUB_CLASS_FOCUS              0x0070
#define fevent_SUB_CLASS_OPEN               0x0080
#define fevent_SUB_CLASS_STATUS             0x0090
#define fevent_SUB_CLASS_TOOLBAR            0x00A0
#define fevent_SUB_CLASS_MISC2              0x00B0
#define fevent_SUB_CLASS_CODEC              0x00C0
#define fevent_SUB_CLASS_TOOLBAR2           0x00D0
#define fevent_SUB_CLASS_FRAME_LINK         0x00E0

/* global events */

#define fevent_GLOBAL_TOGGLE_ANTI_TWITTER   0x1000
#define fevent_GLOBAL_QUIT                  0x1001
#define fevent_GLOBAL_SHOW_HELP             0x1002
#define fevent_GLOBAL_OPEN_URL              0x1003  /* open from the contents of the toolbar URL */
#define fevent_GLOBAL_RESIZE_ABORT          0x1004
#define fevent_GLOBAL_TOGGLE_COOLING        0x1005
#define fevent_GLOBAL_SHOW_VERSION          0x1006
#define fevent_GLOBAL_OPEN_MEM_DUMP	    0x1007
#define fevent_GLOBAL_CYCLE_JPEG	    0x1008
#define fevent_GLOBAL_ICONISE		    0x1009
#define fevent_GLOBAL_DEICONISE		    0x100A

#define fevent_GLOBAL_FONT_MASK		    0x000f
#define fevent_GLOBAL_FONT_SET		    0x1100 /* up to 14 different font settings */
#define fevent_GLOBAL_FONT_INC		    0x110e
#define fevent_GLOBAL_FONT_DEC		    0x110f

/* frame or window events */

#define fevent_TOGGLE_FRAMES                0x2000
#define fevent_TOGGLE_STATUS                0x2001
#define fevent_TOGGLE_ANTI_ALIAS            0x2002
#define fevent_TOGGLE_COLOURS               0x2003
#define fevent_TOGGLE_IMAGES                0x2004
#define fevent_STATUS_OFF	            0x2005

#define fevent_HISTORY_SHOW                 0x2010 /* show combined list */
#define fevent_HISTORY_BACK                 0x2011
#define fevent_HISTORY_BACK_ALL             0x2012
#define fevent_HISTORY_FORWARD              0x2013
#define fevent_HISTORY_FORWARD_ALL          0x2014
#define fevent_HISTORY_SHOW_ALPHA           0x2015
#define fevent_HISTORY_SHOW_RECENT          0x2016
#define fevent_HISTORY_SHOW_SWITCHABLE      0x2017
#define fevent_HISTORY_SHOW_ALPHA_FRAMES    0x2018
#define fevent_HISTORY_SHOW_RECENT_FRAMES   0x2019
#define fevent_HISTORY_SHOW_SWITCHABLE_FRAMES 0x201A

#define fevent_HOTLIST_SHOW                 0x2020  /* just show the hotlist */
#define fevent_HOTLIST_ADD                  0x2021
#define fevent_HOTLIST_REMOVE               0x2022
#define fevent_HOTLIST_WIPE                 0x2023
#define fevent_HOTLIST_SHOW_WITH_URL        0x2024  /* show the hotlist with the URL line */
#define fevent_HOTLIST_SHOW_DELETE	    0x2025
#define fevent_HOTLIST_FLUSH_DELETE	    0x2026 /* delete marked entries */
#define fevent_HOTLIST_SHOW_SWITCHABLE	    0x2027
#define fevent_HOTLIST_SHOW_FRAMES	    0x2028
#define fevent_HOTLIST_SHOW_DELETE_FRAMES   0x2029
#define fevent_HOTLIST_SHOW_SWITCHABLE_FRAMES 0x202A

#define fevent_RELOAD                       0x2030
#define fevent_STOP_LOADING                 0x2031
#define fevent_HOME                         0x2032
#define fevent_PRINT                        0x2033
#define fevent_CLOSE                        0x2034
#define fevent_MENU                         0x2035
#define fevent_FIND_AGAIN                   0x2036
#define fevent_MENU_DEBUG                   0x2037
#define fevent_DBOX_CANCEL                  0x2038
#define fevent_OPEN_URL                     0x2039

#define fevent_SEARCH_PAGE		    0x203C
#define fevent_OFFLINE_PAGE		    0x203D
#define fevent_INFO_PAGE		    0x203E
#define fevent_SEND_URL			    0x203F

#define fevent_COPY_IMAGE                   0x2040
#define fevent_COPY_TEXT                    0x2041
#define fevent_PASTE                        0x2042
#define fevent_PASTE_URL                    0x2043
#define fevent_DELETE_TEXT                  0x2044
#define fevent_COPY_WHATEVER                0x2045  /* pdh */

#define fevent_SCROLL_LEFT                  0x2050
#define fevent_SCROLL_RIGHT                 0x2051
#define fevent_SCROLL_UP                    0x2052
#define fevent_SCROLL_DOWN                  0x2053
#define fevent_SCROLL_PAGE_UP               0x2054
#define fevent_SCROLL_PAGE_DOWN             0x2055
#define fevent_SCROLL_TOP                   0x2056
#define fevent_SCROLL_BOTTOM                0x2057
#define fevent_SCROLL_OR_CURSOR_UP          0x2058
#define fevent_SCROLL_OR_CURSOR_DOWN        0x2059
#define fevent_SCROLL_FAR_LEFT              0x205A
#define fevent_SCROLL_FAR_RIGHT             0x205B
#define fevent_SCROLL_PAGE_LEFT             0x205C
#define fevent_SCROLL_PAGE_RIGHT            0x205D

#define fevent_HIGHLIGHT_BACK               0x2060
#define fevent_HIGHLIGHT_FORWARD            0x2061
#define fevent_HIGHLIGHT_UP                 0x2062
#define fevent_HIGHLIGHT_DOWN               0x2063
#define fevent_HIGHLIGHT_REMOVE             0x2064
#define fevent_HIGHLIGHT_ACTIVATE           0x2065
#define fevent_HIGHLIGHT_PREV_FRAME         0x2066
#define fevent_HIGHLIGHT_NEXT_FRAME         0x2067
#define fevent_HIGHLIGHT_FRAME_UP	    0x2068
#define fevent_HIGHLIGHT_FRAME_DOWN         0x2069
#define fevent_HIGHLIGHT_FRAME_LEFT	    0x206A
#define fevent_HIGHLIGHT_FRAME_RIGHT        0x206B

#define fevent_USER_UNLOAD                  0x2070
#define fevent_USER_LOAD		    0x2071

#define fevent_OPEN_PRINT_OPTIONS           0x2080
#define fevent_OPEN_FIND                    0x2081
#define fevent_OPEN_DISPLAY_OPTIONS         0x2082
#define fevent_OPEN_KEYBOARD		    0x2083
#define fevent_OPEN_RELATED_STUFF	    0x2084
#define fevent_OPEN_FONT_SIZE		    0x2085
#define fevent_OPEN_SOUND		    0x2086
#define fevent_OPEN_BEEPS		    0x2087
#define fevent_OPEN_SCALING		    0x2088

#define fevent_STATUS_INFO_LEVEL            0x2090  /* +F */

#define fevent_TOOLBAR_MAIN		    0x20A0
#define fevent_TOOLBAR_FAVS		    0x20A1
#define fevent_TOOLBAR_EXTRAS		    0x20A2
#define fevent_TOOLBAR_HISTORY		    0x20A3
#define fevent_TOOLBAR_PRINT		    0x20A4
#define fevent_TOOLBAR_DETAILS		    0x20A5
/* #define fevent_TOOLBAR_RELATED		    0x20A6 */
/* #define fevent_TOOLBAR_OPENURL		    0x20A7 */
#define fevent_TOOLBAR_NC1		    0x20A8
#define fevent_TOOLBAR_CODEC		    0x20A9
#define fevent_TOOLBAR_CUSTOM		    0x20AA
#define fevent_TOOLBAR_CYCLE		    0x20AE
#define fevent_TOOLBAR_EXIT		    0x20AF /* unstack */

#define fevent_PRINT_LETTER		    0x20B0
#define fevent_PRINT_LEGAL		    0x20B1
#define fevent_OPEN_WRITEABLE		    0x20B2
#define fevent_STOP_OR_RELOAD		    0x20B3
#define fevent_SOUND_TOGGLE		    0x20B4
#define fevent_SOUND_ON			    0x20B5
#define fevent_SOUND_OFF		    0x20B6
#define fevent_BEEPS_TOGGLE		    0x20B7
#define fevent_BEEPS_ON			    0x20B8
#define fevent_BEEPS_OFF		    0x20B9
#define fevent_SCALING_TOGGLE		    0x20BA
#define fevent_SCALING_ON		    0x20BB
#define fevent_SCALING_OFF		    0x20BC
#define fevent_SPECIAL_SELECT		    0x20BD

/* The order here must match the table in stbevent.c */
#define fevent_CODEC_STOP		    0x20C0
#define fevent_CODEC_PLAY		    0x20C1
#define fevent_CODEC_PAUSE		    0x20C2
#define fevent_CODEC_REWIND		    0x20C3
#define fevent_CODEC_FAST_FORWARD	    0x20C4
#define fevent_CODEC_RECORD		    0x20C5
#define fevent_CODEC_MUTE		    0x20C6
#define fevent_CODEC_CLOSING		    0x20C7 /* codec toolbar is closing */
#define fevent_CODEC_STOP_PAGE		    0x20C8 /* if page loading then stop it, else stop codec playing */
#define fevent_CODEC_OPENING		    0x20C9 /* codec toolbar is opening */

#define fevent_TOOLBAR_MOVE_LEFT	    0x20D0
#define fevent_TOOLBAR_MOVE_RIGHT	    0x20D1
#define fevent_TOOLBAR_MOVE_UP		    0x20D2
#define fevent_TOOLBAR_MOVE_DOWN	    0x20D3
#define fevent_TOOLBAR_ACTIVATE		    0x20D4
#define fevent_TOOLBAR_PAGE_UP		    0x20D5
#define fevent_TOOLBAR_PAGE_DOWN	    0x20D6

#define fevent_FRAME_LINK_LEFT		    0x20E0
#define fevent_FRAME_LINK_RIGHT		    0x20E1
#define fevent_FRAME_LINK_UP		    0x20E2
#define fevent_FRAME_LINK_DOWN		    0x20E3
#define fevent_FRAME_LINK_ACTIVATE	    0x20E4

#define fevent_URLS_MASK	            0x00ff
#define fevent_ENCODING_MASK	            0x00ff

/* menu events */
#define fevent_MENU_UP                      (fevent_CLASS_MENU + 0x001)
#define fevent_MENU_DOWN                    (fevent_CLASS_MENU + 0x002)
#define fevent_MENU_PAGE_UP                 (fevent_CLASS_MENU + 0x003)
#define fevent_MENU_PAGE_DOWN               (fevent_CLASS_MENU + 0x004)
#define fevent_MENU_TOP                     (fevent_CLASS_MENU + 0x005)
#define fevent_MENU_BOTTOM                  (fevent_CLASS_MENU + 0x006)
#define fevent_MENU_SELECT                  (fevent_CLASS_MENU + 0x007)
#define fevent_MENU_CANCEL                  (fevent_CLASS_MENU + 0x008)
#define fevent_MENU_TOGGLE                  (fevent_CLASS_MENU + 0x009)

/* map events */
#define fevent_MAP_MOVE_MASK                (fevent_CLASS_MAP + 0x00F)
#define fevent_MAP_MOVE                     (fevent_CLASS_MAP + 0x000)
#define fevent_MAP_MOVE_LEFT                0x0001
#define fevent_MAP_MOVE_RIGHT               0x0002
#define fevent_MAP_MOVE_UP                  0x0004
#define fevent_MAP_MOVE_DOWN                0x0008
#define fevent_MAP_SELECT                   (fevent_CLASS_MAP + 0x010)
#define fevent_MAP_STOP                     (fevent_CLASS_MAP + 0x011)
#define fevent_MAP_CANCEL                   (fevent_CLASS_MAP + 0x012)

/* plugin toggle events */

#define fevent_PLUGIN_TOGGLE		    0x6000

/* eof fevents.h */
