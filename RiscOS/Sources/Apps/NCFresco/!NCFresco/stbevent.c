/* stbevent.c


 */

#include <stddef.h>
#include <stdlib.h>

#include "msgs.h"
#include "wimp.h"

#include "config.h"
#include "hotlist.h"
#include "images.h"
#include "interface.h"
#include "version.h"

#include "fevents.h"

#include "stbfe.h"
#include "stbopen.h"
#include "stbhist.h"
#include "stbutils.h"
#include "stbview.h"
#include "stbtb.h"

static void global_event_handler(int event)
{
    switch (event)
    {
        case fevent_GLOBAL_TOGGLE_ANTI_TWITTER:
            toggle_anti_twitter();
            fe_refresh_screen(NULL);
            break;

        case fevent_GLOBAL_QUIT:
            fe_message(msgs_lookup("goodbye:"));
            exit(0);
            break;

        case fevent_GLOBAL_SHOW_HELP:
	    frontend_complain(fe_show_file_in_frame(main_view, config_help_file, TARGET_HELP));
            break;

        case fevent_GLOBAL_SHOW_VERSION:
            frontend_complain(fe_open_version(main_view));
            break;

        case fevent_GLOBAL_OPEN_URL:
            tb_open_url();
            break;

        case fevent_GLOBAL_RESIZE_ABORT:
            fe_resize_abort();
            break;

        case fevent_GLOBAL_TOGGLE_COOLING:
#ifdef IMAGE_COOLING
            image_toggle_cooling();
#endif
            image_palette_change();
            fe_refresh_screen(NULL);
            break;

    case fevent_GLOBAL_OPEN_MEM_DUMP:
	fe_show_mem_dump();
	break;

    case fevent_GLOBAL_CYCLE_JPEG:
	config_display_jpeg = (config_display_jpeg + 1) & 3;
	if (config_display_jpeg < 2)
	    fe_refresh_screen(NULL);
	else
	    fe_reload(main_view);
	break;

    case fevent_GLOBAL_TOGGLE_FORCE_FIT:
	break;

    case fevent_GLOBAL_ICONISE:
	fe_iconise(TRUE);
	break;

    case fevent_GLOBAL_DEICONISE:
	fe_iconise(FALSE);
	break;

	
    case fevent_GLOBAL_FONT_INC:
	fe_font_size_set(+1, FALSE);
	break;
	
    case fevent_GLOBAL_FONT_DEC:
	fe_font_size_set(-1, FALSE);
	break;
	
    default:
	if ((event &~ fevent_GLOBAL_FONT_MASK) == fevent_GLOBAL_FONT_SET)
	{
	    fe_font_size_set(event & fevent_GLOBAL_FONT_MASK, TRUE);
	}
	break;
    }
}

extern void parse_frames(int state);

static void toggle_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_TOGGLE_FRAMES:
            parse_frames(-1);
            frontend_complain(fe_reload(fe_find_top(v)));
            break;

        case fevent_TOGGLE_STATUS:
            fe_status_toggle(v);
            break;

        case fevent_TOGGLE_COLOURS:
            frontend_complain(fe_doc_flag_toggle(v, be_openurl_flag_BODY_COLOURS));
            break;

        case fevent_TOGGLE_IMAGES:
            frontend_complain(fe_doc_flag_toggle(v, be_openurl_flag_DEFER_IMAGES));
            break;
    }
}

static void history_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_HISTORY_SHOW:
            frontend_complain(fe_history_show(v));
            break;

        case fevent_HISTORY_BACK:
            frontend_complain(fe_history_move(v, history_PREV));
            break;

        case fevent_HISTORY_BACK_ALL:
            frontend_complain(fe_history_move(v, history_FIRST));
            break;

        case fevent_HISTORY_FORWARD:
            frontend_complain(fe_history_move(v, history_NEXT));
            break;

        case fevent_HISTORY_FORWARD_ALL:
            frontend_complain(fe_history_move(v, history_LAST));
            break;

    case fevent_HISTORY_SHOW_ALPHA:
	frontend_complain(frontend_open_url("ncfrescointernal:openpanel?name=historyalpha", v, TARGET_HISTORY, NULL, fe_open_url_NO_CACHE));
	break;

    case fevent_HISTORY_SHOW_RECENT:
	frontend_complain(frontend_open_url("ncfrescointernal:openpanel?name=historyrecent", v, TARGET_HISTORY, NULL, fe_open_url_NO_CACHE));
	break;
    }
}

static void hotlist_event_handler(int event, fe_view v)
{
    switch (event)
    {
    case fevent_HOTLIST_SHOW:
	frontend_complain(fe_hotlist_open(v));
	break;

    case fevent_HOTLIST_SHOW_WITH_URL:
	frontend_complain(fe_hotlist_and_url_open(v));
	break;

    case fevent_HOTLIST_ADD:
	frontend_complain(fe_hotlist_add(v));
	break;

    case fevent_HOTLIST_REMOVE:
	frontend_complain(fe_hotlist_remove(v));
	break;

    case fevent_HOTLIST_WIPE:
	break;

    case fevent_HOTLIST_SHOW_DELETE:
	frontend_complain(frontend_open_url("ncfrescointernal:openpanel?name=favsdelete", v, TARGET_FAVS, NULL, fe_open_url_NO_CACHE));
	break;
    }
}

static void misc_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_RELOAD:
            frontend_complain(fe_reload(v));
            break;

        case fevent_STOP_LOADING:
            frontend_complain(fe_abort_fetch(v));
            break;

        case fevent_HOME:
            frontend_complain(fe_home(v));
            break;

        case fevent_PRINT:
            frontend_complain(fe_print(v));
            break;

        case fevent_CLOSE:
            if (v->app_return_page)
            {
                frontend_complain(frontend_open_url(v->app_return_page, v, NULL, NULL, 0));
            }
            else if (v->return_page)
            {
                frontend_complain(frontend_open_url(v->return_page, v, NULL, NULL, 0));
            }
            else if (config_mode_platform == platform_RISCOS_DESKTOP)
            {
                fe_message(msgs_lookup("goodbye:"));
                exit(0);
            }
            break;

        case fevent_MENU:
            tb_menu_show(v, 0);
            break;

        case fevent_FIND_AGAIN:
            fe_find_again(v);
            break;

        case fevent_MENU_DEBUG:
            tb_menu_show(v, 1);
            break;

        case fevent_DBOX_CANCEL:
            fe_dbox_cancel();
            break;

       case fevent_OPEN_URL:
            frontend_complain(fe_url_open(v));
            break;

    case fevent_FORCE_FIT:
	fe_force_fit(v, TRUE);
	break;

    case fevent_SEARCH_PAGE:
	frontend_complain(fe_search_page(v));
	break;

    case fevent_OFFLINE_PAGE:
	tb_status_unstack_all();
	frontend_complain(fe_offline_page(v));
	break;

    case fevent_INFO_PAGE:
	frontend_complain(fe_open_version(v));
	break;

    case fevent_SEND_URL:
	frontend_complain(frontend_open_url("ncfrescointernal:sendurl", v, NULL, NULL, fe_open_url_NO_CACHE));
	break;

    case fevent_OPEN_WRITEABLE:
	tb_open_url_and_close();
	break;

    case fevent_STOP_OR_RELOAD:
	if (fe_abort_fetch_possible(v))
	    frontend_complain(fe_abort_fetch(v));
	else
	    frontend_complain(fe_reload(v));
	break;
    }
}

static void clipboard_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_COPY_IMAGE:
            fe_copy_image_to_clipboard(v);
            break;
        case fevent_COPY_TEXT:
            fe_copy_text_to_clipboard(v);
            break;
        case fevent_PASTE:
            fe_paste(v);
            break;
        case fevent_PASTE_URL:
            fe_paste_url(v);
            break;
    }
}

static void scroll_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_SCROLL_LEFT:
            fe_view_scroll_x(v, -1);
            break;
        case fevent_SCROLL_RIGHT:
            fe_view_scroll_x(v, +1);
            break;
        case fevent_SCROLL_UP:
            fe_view_scroll_y(v, +1);
            break;
        case fevent_SCROLL_DOWN:
            fe_view_scroll_y(v, -1);
            break;
        case fevent_SCROLL_PAGE_UP:
            fe_view_scroll_y(v, +2);
            break;
        case fevent_SCROLL_PAGE_DOWN:
            fe_view_scroll_y(v, -2);
            break;
        case fevent_SCROLL_TOP:
            fe_view_scroll_y(v, +3);
            break;
        case fevent_SCROLL_BOTTOM:
            fe_view_scroll_y(v, -3);
            break;
        case fevent_SCROLL_OR_CURSOR_UP:
	    fe_cursor_movement(v, 0, +1);
            break;
        case fevent_SCROLL_OR_CURSOR_DOWN:
	    fe_cursor_movement(v, 0, -1);
            break;
        case fevent_SCROLL_FAR_LEFT:
            fe_view_scroll_x(v, -3);
            break;
        case fevent_SCROLL_FAR_RIGHT:
            fe_view_scroll_x(v, +3);
            break;
        case fevent_SCROLL_PAGE_LEFT:
            fe_view_scroll_x(v, -2);
            break;
        case fevent_SCROLL_PAGE_RIGHT:
            fe_view_scroll_x(v, +2);
            break;
    }
}

static void highlight_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_HIGHLIGHT_BACK:
            fe_move_highlight(v, be_link_BACK | be_link_VISIBLE);
            break;

        case fevent_HIGHLIGHT_FORWARD:
            fe_move_highlight(v, be_link_VISIBLE);
            break;

        case fevent_HIGHLIGHT_UP:
            fe_move_highlight(v, be_link_VERT | be_link_BACK | be_link_VISIBLE);
            break;

        case fevent_HIGHLIGHT_DOWN:
            fe_move_highlight(v, be_link_VERT | be_link_VISIBLE);
            break;

        case fevent_HIGHLIGHT_REMOVE:
            break;

        case fevent_HIGHLIGHT_ACTIVATE:
            frontend_complain(fe_handle_enter(v));
            break;

        case fevent_HIGHLIGHT_NEXT_FRAME:
            fe_move_highlight_frame(v, TRUE);
            break;

        case fevent_HIGHLIGHT_PREV_FRAME:
            fe_move_highlight_frame(v, FALSE);
            break;

    case fevent_HIGHLIGHT_FRAME_UP:
	fe_move_highlight_frame_direction(v, 0, +1);
	break;

    case fevent_HIGHLIGHT_FRAME_DOWN:
	fe_move_highlight_frame_direction(v, 0, -1);
	break;

    case fevent_HIGHLIGHT_FRAME_LEFT:
	fe_move_highlight_frame_direction(v, -1, 0);
	break;

    case fevent_HIGHLIGHT_FRAME_RIGHT:
	fe_move_highlight_frame_direction(v, +1, 0);
	break;
    }
}

static void focus_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_GIVE_FOCUS:
            break;
        case fevent_REMOVE_FOCUS:
            break;
    }
}

static void open_event_handler(int event, fe_view v)
{
    switch (event)
    {
        case fevent_OPEN_PRINT_OPTIONS:
            frontend_complain(fe_print_options_open(v));
            break;

        case fevent_OPEN_FIND:
            frontend_complain(fe_find_open(v));
            break;

        case fevent_OPEN_DISPLAY_OPTIONS:
            frontend_complain(fe_display_options_open(v));
            break;

        case fevent_OPEN_KEYBOARD:
	    fe_open_keyboard(v);
            break;
    }
}

static void status_event_handler(int event, fe_view v)
{
    frontend_complain(fe_status_info_level(v, event - fevent_STATUS_INFO_LEVEL));
}

static void toolbar_event_handler(int event, fe_view v)
{
    if (event == fevent_TOOLBAR_EXIT)
	frontend_complain(fe_status_unstack(v));
    else
	frontend_complain(fe_status_open_toolbar(v, event - fevent_TOOLBAR_MAIN));
}

static void url_event_handler(int event, fe_view v)
{
    char buf[32], *s;

    sprintf(buf, PROGRAM_NAME"$EventURL%02x", event & fevent_URLS_MASK);

    s = getenv(buf);
    if (s && s[0])
	frontend_complain(frontend_open_url(s, v, NULL, NULL, 0));
}

static void codec_event_handler(int event, fe_view v)
{
    
}

void fevent_handler(int event, fe_view v)
{
    STBDBG(("fevent_handler(): event %x v %p\n", event, v));

    switch (event & fevent_CLASS_MASK)
    {
    case fevent_CLASS_GLOBAL:
	global_event_handler(event);
	break;

    case fevent_CLASS_WINDOW:
	event &= ~fevent_WINDOW;
	v = fe_find_top(v);
	/* fall-through */

    case fevent_CLASS_FRAME:
	switch (event & fevent_FRAME_CLASS_MASK)
	{
	case fevent_FRAME_CLASS_ACTIONS:
            switch (event & fevent_SUB_CLASS_MASK)
            {
	    case fevent_SUB_CLASS_TOGGLE:
		toggle_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_HISTORY:
		history_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_HOTLIST:
		hotlist_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_MISC:
	    case fevent_SUB_CLASS_MISC2:
		misc_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_CLIPBOARD:
		clipboard_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_SCROLL:
		scroll_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_HIGHLIGHT:
		highlight_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_FOCUS:
		focus_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_OPEN:
		open_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_STATUS:
		status_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_TOOLBAR:
		toolbar_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_CODEC:
		codec_event_handler(event, v);
		break;
            }
	    break;

	case fevent_FRAME_CLASS_URLS:
	    url_event_handler(event, v);
	    break;
	}
	if (event & fevent_UNSTACK_TOOLBAR)
	    tb_status_unstack();
	break;

    case fevent_CLASS_MENU:
	fe_menu_event_handler(event);
	break;

    case fevent_CLASS_MAP:
	fe_map_event_handler(event, v);
	break;

    case fevent_CLASS_PLUGIN:
	fe_plugin_event_handler(event, v);
	break;
    }
}

/* stbevent.c */
