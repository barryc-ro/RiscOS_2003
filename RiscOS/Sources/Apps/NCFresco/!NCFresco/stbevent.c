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
#include "plugin.h"
#include "util.h"

#include "fevents.h"

#include "stbfe.h"
#include "stbopen.h"
#include "stbview.h"
#include "stbhist.h"
#include "stbutils.h"
#include "stbtb.h"
#include "frameutils.h"

static void global_event_handler(int event)
{
    switch (event &~ fevent_CLEAR_POPUPS)
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
	sound_event(snd_HELP_SHOW);
#if 0 /*def ANT_NCFRESCO*/
	frontend_complain(fe_internal_toggle_panel("help", event & fevent_CLEAR_POPUPS));
#else
	frontend_complain(fe_show_file_in_frame(main_view, config_help_file, TARGET_HELP));
#endif
	break;

    case fevent_GLOBAL_SHOW_VERSION:
	frontend_complain(fe_open_version(main_view));
	break;

    case fevent_GLOBAL_OPEN_URL:
	if (use_toolbox)
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
	fe_internal_toggle_panel("memdump", event & fevent_CLEAR_POPUPS);
	break;

    case fevent_GLOBAL_CYCLE_JPEG:
	config_display_jpeg = (config_display_jpeg + 1) & 3;
	if (config_display_jpeg < 2)
	    fe_refresh_screen(NULL);
	else
	    fe_reload(main_view);
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
    switch (event &~ fevent_CLEAR_POPUPS)
    {
        case fevent_TOGGLE_FRAMES:
            parse_frames(-1);
            frontend_complain(fe_reload(frameutils_find_top(v)));
            break;

        case fevent_TOGGLE_STATUS:
            fe_status_toggle(v);
            break;

        case fevent_STATUS_OFF:
	    frontend_complain(fe_status_state(v, FALSE));
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
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_HISTORY_SHOW:
	frontend_complain(fe_internal_toggle_panel("historycombined", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_BACK:
	if (!fe_history_possible(v, history_PREV))
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
	else if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	{
	    sound_event(snd_HISTORY_BACK);
	    frontend_complain(fe_history_move(v, history_PREV));
	}
	break;

    case fevent_HISTORY_BACK_ALL:
	if (!fe_history_possible(v, history_FIRST))
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
	else if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	{
	    sound_event(snd_HISTORY_BACK);
	    frontend_complain(fe_history_move(v, history_FIRST));
	}
	break;

    case fevent_HISTORY_FORWARD:
	if (!fe_history_possible(v, history_NEXT))
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
	else if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	{
	    sound_event(snd_HISTORY_FORWARD);
	    frontend_complain(fe_history_move(v, history_NEXT));
	}
	break;

    case fevent_HISTORY_FORWARD_ALL:
	if (!fe_history_possible(v, history_NEXT))
	{
	    sound_event(snd_WARN_BAD_KEY);
	}
	else if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	{
	    sound_event(snd_HISTORY_FORWARD);
	    frontend_complain(fe_history_move(v, history_LAST));
	}
	break;

    case fevent_HISTORY_SHOW_ALPHA:
	frontend_complain(fe_internal_toggle_panel("historyalpha", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_SHOW_RECENT:
	frontend_complain(fe_internal_toggle_panel("historyrecent", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_SHOW_SWITCHABLE:
	frontend_complain(fe_internal_toggle_panel("historyswitch", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_SHOW_ALPHA_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("historyalpha", "frame=0", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_SHOW_RECENT_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("historyrecent", "frame=0", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HISTORY_SHOW_SWITCHABLE_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("historyswitch", "frame=0", event & fevent_CLEAR_POPUPS));
	break;
    }
}

static void hotlist_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_HOTLIST_SHOW:
	frontend_complain(fe_internal_toggle_panel("favs", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_SHOW_WITH_URL:
	frontend_complain(fe_internal_toggle_panel("urlfavs", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_ADD:
	if (!fe_popup_open() || fe_locate_view(TARGET_INFO)) /* can add to hotlist when the info window is open */
	    fe_hotlist_add(v);
	else
	    sound_event(snd_WARN_BAD_KEY);
	break;

    case fevent_HOTLIST_REMOVE:
	frontend_complain(fe_hotlist_remove(v));
	break;

    case fevent_HOTLIST_WIPE:
	break;

    case fevent_HOTLIST_SHOW_DELETE:
	frontend_complain(fe_internal_toggle_panel("favsdelete", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_FLUSH_DELETE:
	frontend_complain(hotlist_flush_pending_delete());
	break;

    case fevent_HOTLIST_SHOW_SWITCHABLE:
	frontend_complain(fe_internal_toggle_panel("favswitch", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_SHOW_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("favs", "frame=0", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_SHOW_DELETE_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("favsdelete", "frame=0", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_HOTLIST_SHOW_SWITCHABLE_FRAMES:
	frontend_complain(fe_internal_toggle_panel_args("favswitch", "frame=0", event & fevent_CLEAR_POPUPS));
	break;

    }
}

static void misc_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_RELOAD:
	frontend_complain(fe_reload(v));
	break;

    case fevent_STOP_LOADING:
	frontend_complain(fe_abort_fetch(v, FALSE));
	break;

    case fevent_HOME:
	frontend_complain(fe_internal_open_page(v, "home", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_PRINT:
	frontend_complain(fe_print(v, fe_print_DEFAULT));
	break;

    case fevent_CLOSE:
	if (v->app_return_page)
	{
	    frontend_complain(frontend_open_url(v->app_return_page, v, NULL, NULL, NULL, 0));
	}
	else if (v->return_page)
	{
	    frontend_complain(frontend_open_url(v->return_page, v, NULL, NULL, NULL, 0));
	}
	else if (config_mode_platform == platform_RISCOS_DESKTOP)
	{
	    fe_message(msgs_lookup("goodbye:"));
	    exit(0);
	}
	break;

    case fevent_MENU:
	if (use_toolbox)
	    tb_menu_show(v, 0);
	break;

    case fevent_FIND_AGAIN:
	fe_find_again(v);
	break;

    case fevent_MENU_DEBUG:
	if (use_toolbox)
	    tb_menu_show(v, 1);
	break;

    case fevent_DBOX_CANCEL:
	fe_dbox_cancel();
	break;

    case fevent_OPEN_URL:
	/* if the on screen keyboard is open already then don't open the url window */
	frontend_complain(fe_internal_toggle_panel("url", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_SEARCH_PAGE:
	frontend_complain(fe_internal_open_page(v, "search", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OFFLINE_PAGE:
	fe_status_unstack_all();
	frontend_complain(fe_internal_open_page(v, "offline", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_INFO_PAGE:
	fe_open_info(v, backend_read_highlight(v->displaying, NULL), 0, 0, event & fevent_CLEAR_POPUPS);
	break;

    case fevent_SEND_URL:
	frontend_complain(frontend_open_url("ncint:sendurl", v, NULL, NULL, NULL, fe_open_url_NO_CACHE | fe_open_url_NO_REFERER));
	break;

    case fevent_PRINT_LETTER:
	frontend_complain(fe_print(v, fe_print_LETTER));
	break;

    case fevent_PRINT_LEGAL:
	frontend_complain(fe_print(v, fe_print_LEGAL));
	break;

    case fevent_OPEN_WRITEABLE:
	if (use_toolbox)
	    tb_open_url_and_close();
	break;

    case fevent_STOP_OR_RELOAD:
	/* do the operation */
	if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	{
	    if (fe_abort_fetch_possible(v))
		frontend_complain(fe_abort_fetch(v, FALSE));
	    else
		frontend_complain(fe_reload(v));
	}
	break;

    case fevent_SOUND_TOGGLE:
	fe_bgsound_set(-1);
	sound_event(config_sound_background ? snd_SOUND_ON : snd_SOUND_OFF);
	break;

    case fevent_SOUND_OFF:
	fe_bgsound_set(0);
	sound_event(snd_SOUND_OFF);
	break;

    case fevent_SOUND_ON:
	fe_bgsound_set(1);
	sound_event(snd_SOUND_ON);
	break;

    case fevent_BEEPS_TOGGLE:
	fe_beeps_set(-1, TRUE);
	break;

    case fevent_BEEPS_OFF:
	fe_beeps_set(0, TRUE);
	break;

    case fevent_BEEPS_ON:
	fe_beeps_set(1, TRUE);
	break;

    case fevent_SCALING_TOGGLE:
	fe_scaling_set(-1);
	break;

    case fevent_SCALING_OFF:
	fe_scaling_set(0);
	break;

    case fevent_SCALING_ON:
	fe_scaling_set(1);
	break;

    case fevent_SPECIAL_SELECT:
	fe_special_select(v);
	break;
    }
}

static void clipboard_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
        case fevent_COPY_IMAGE:
            fe_copy_image_to_clipboard(v);
            break;
        case fevent_COPY_TEXT:
            fe_copy_text_to_clipboard(v);
            break;
#ifdef ANT_NCFRESCO
        case fevent_COPY_WHATEVER:
            fe_copy_whatever_to_clipboard(v);
            break;
#endif
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
    switch (event &~ fevent_CLEAR_POPUPS)
    {
        case fevent_SCROLL_LEFT:
            fe_view_scroll_x(v, -1, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_RIGHT:
            fe_view_scroll_x(v, +1, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_UP:
            fe_view_scroll_y(v, +1, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_DOWN:
            fe_view_scroll_y(v, -1, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_PAGE_UP:
            fe_view_scroll_y(v, +2, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_PAGE_DOWN:
            fe_view_scroll_y(v, -2, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_TOP:
            fe_view_scroll_y(v, +3, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_BOTTOM:
            fe_view_scroll_y(v, -3, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_OR_CURSOR_UP:
	    fe_cursor_movement(v, 0, +1);
            break;
        case fevent_SCROLL_OR_CURSOR_DOWN:
	    fe_cursor_movement(v, 0, -1);
            break;
        case fevent_SCROLL_FAR_LEFT:
            fe_view_scroll_x(v, -3, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_FAR_RIGHT:
            fe_view_scroll_x(v, +3, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_PAGE_LEFT:
            fe_view_scroll_x(v, -2, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
        case fevent_SCROLL_PAGE_RIGHT:
            fe_view_scroll_x(v, +2, fe_view_scroll_ENSURE | fe_view_scroll_BEEP);
            break;
    }
}

static void highlight_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
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
	fe_move_highlight_frame_direction(v, be_link_VERT | be_link_BACK);
	break;

    case fevent_HIGHLIGHT_FRAME_DOWN:
	fe_move_highlight_frame_direction(v, be_link_VERT);
	break;

    case fevent_HIGHLIGHT_FRAME_LEFT:
	fe_move_highlight_frame_direction(v, be_link_BACK);
	break;

    case fevent_HIGHLIGHT_FRAME_RIGHT:
	fe_move_highlight_frame_direction(v, 0);
	break;
    }
}

static void focus_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_USER_UNLOAD:
	fe_user_unload();
	break;

    case fevent_USER_LOAD:
	fe_user_load();
	break;
    }
}

static void open_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_OPEN_PRINT_OPTIONS:
	frontend_complain(fe_internal_toggle_panel("printoptions", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OPEN_FIND:
	frontend_complain(fe_find_open(v));
	break;

    case fevent_OPEN_DISPLAY_OPTIONS:
	frontend_complain(fe_internal_toggle_panel("displayoptions", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OPEN_KEYBOARD:
	fe_keyboard_open(v);
	break;

    case fevent_OPEN_RELATED_STUFF:
	fe_dispose_view(fe_locate_view(TARGET_INFO));
	frontend_open_url("ncint:openpanel?name=related", v, TARGET_TOP, NULL, NULL, fe_open_url_NO_CACHE);
	break;

    case fevent_OPEN_FONT_SIZE:
	frontend_complain(fe_internal_toggle_panel("customfonts", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OPEN_SOUND:
	frontend_complain(fe_internal_toggle_panel("customsound", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OPEN_BEEPS:
	frontend_complain(fe_internal_toggle_panel("custombeeps", event & fevent_CLEAR_POPUPS));
	break;

    case fevent_OPEN_SCALING:
	frontend_complain(fe_internal_toggle_panel("customscaling", event & fevent_CLEAR_POPUPS));
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
    {
	frontend_complain(fe_status_unstack(v));
	sound_event(snd_GENERIC_BACK);
	return;
    }

    /* open toolbar and make noise */
    if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
    {
	int bar;

	event &= ~fevent_CLEAR_POPUPS;

	if (event == fevent_TOOLBAR_CYCLE)
	    bar = -1;
	else
	    bar = event - fevent_TOOLBAR_MAIN;

	frontend_complain(fe_status_open_toolbar(v, bar));
	sound_event(soundfx_ACTION_OK);
    }
}

static void frame_link_event_handler(int event, fe_view v)
{
    switch (event &~ fevent_CLEAR_POPUPS)
    {
    case fevent_FRAME_LINK_LEFT:
	fe_frame_link_move(v, be_link_BACK);
	break;

    case fevent_FRAME_LINK_RIGHT:
	fe_frame_link_move(v, 0);
	break;

    case fevent_FRAME_LINK_UP:
	fe_frame_link_move(v, be_link_VERT | be_link_BACK);
	break;

    case fevent_FRAME_LINK_DOWN:
	fe_frame_link_move(v, be_link_VERT);
	break;

    case fevent_FRAME_LINK_ACTIVATE:
	fe_frame_link_activate(v);
	break;
    }
}

static void url_event_handler(int event, fe_view v)
{
    if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
    {
	char buf[32], *s;

	sprintf(buf, PROGRAM_NAME"$EventURL%02x", event & fevent_URLS_MASK);

	s = getenv(buf);
	if (s && s[0])
	{
	    sound_event(snd_URL_SHOW);

	    frontend_complain(frontend_open_url(s, v, NULL, NULL, NULL, fe_open_url_NO_REFERER));
	}
    }
}

static void encoding_event_handler(int event, fe_view v)
{
    if (fe_internal_check_popups(event & fevent_CLEAR_POPUPS))
	fe_encoding(v, event & fevent_ENCODING_MASK);
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
	v = fe_find_top_nopopup(v);
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
	    case fevent_SUB_CLASS_TOOLBAR2:
		if (use_toolbox)
		    tb_event_handler(event, v);
		break;
	    case fevent_SUB_CLASS_FRAME_LINK:
		frame_link_event_handler(event, v);
		break;
            }
	    break;

	case fevent_FRAME_CLASS_URLS:
	    url_event_handler(event, v);
	    break;

	case fevent_FRAME_CLASS_ENCODING:
	    encoding_event_handler(event, v);
	    break;
	}
/* 	if (event & fevent_UNSTACK_TOOLBAR) */
/* 	    tb_status_unstack(TRUE); */
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

BOOL fevent_possible(int event, fe_view v)
{
    BOOL possible = TRUE;

    event &= ~fevent_CLEAR_POPUPS;

    switch (event & fevent_CLASS_MASK)
    {
    case fevent_CLASS_GLOBAL:
	break;

    case fevent_CLASS_WINDOW:
	event &= ~fevent_WINDOW;
	v = fe_find_top_nopopup(v);
	/* fall-through */

    case fevent_CLASS_FRAME:
	switch (event & fevent_FRAME_CLASS_MASK)
	{
	case fevent_FRAME_CLASS_ACTIONS:
            switch (event & fevent_SUB_CLASS_MASK)
            {
	    case fevent_SUB_CLASS_TOGGLE:
		break;

	    case fevent_SUB_CLASS_HISTORY:
		switch (event)
		{
		case fevent_HISTORY_SHOW:
		case fevent_HISTORY_SHOW_ALPHA:
		case fevent_HISTORY_SHOW_RECENT:
		case fevent_HISTORY_SHOW_SWITCHABLE:
		case fevent_HISTORY_SHOW_ALPHA_FRAMES:
		case fevent_HISTORY_SHOW_RECENT_FRAMES:
		case fevent_HISTORY_SHOW_SWITCHABLE_FRAMES:
		    break;

		case fevent_HISTORY_BACK:
		    possible = fe_history_possible(v, history_PREV);
		    break;

		case fevent_HISTORY_BACK_ALL:
		    possible = fe_history_possible(v, history_FIRST);
		    break;

		case fevent_HISTORY_FORWARD:
		    possible = fe_history_possible(v, history_NEXT);
		    break;

		case fevent_HISTORY_FORWARD_ALL:
		    possible = fe_history_possible(v, history_LAST);
		    break;
		}
		break;

	    case fevent_SUB_CLASS_HOTLIST:
		switch (event)
		{
		case fevent_HOTLIST_SHOW:
		case fevent_HOTLIST_SHOW_WITH_URL:
		case fevent_HOTLIST_REMOVE:
		case fevent_HOTLIST_WIPE:
		case fevent_HOTLIST_SHOW_DELETE:
		case fevent_HOTLIST_FLUSH_DELETE:
		case fevent_HOTLIST_SHOW_SWITCHABLE:
		case fevent_HOTLIST_SHOW_FRAMES:
		case fevent_HOTLIST_SHOW_DELETE_FRAMES:
		case fevent_HOTLIST_SHOW_SWITCHABLE_FRAMES:
		    break;

		case fevent_HOTLIST_ADD:
		    possible = !fe_popup_open() || fe_locate_view(TARGET_INFO); /* can add to hotlist when the info window is open */
		    break;
		}
		break;

	    case fevent_SUB_CLASS_MISC:
	    case fevent_SUB_CLASS_MISC2:
		switch (event)
		{
		case fevent_RELOAD:
		    possible = fe_reload_possible(v);
		    break;

		case fevent_STOP_LOADING:
		    possible = fe_abort_fetch_possible(v);
		    break;

		case fevent_PRINT:
		case fevent_PRINT_LETTER:
		case fevent_PRINT_LEGAL:
		    possible = fe_print_possible(v);
		    break;

		case fevent_HOME:
		case fevent_OPEN_URL:
		case fevent_SEARCH_PAGE:
		case fevent_STOP_OR_RELOAD:
		    possible = !fe_popup_open() && on_screen_kbd == 0;
		    break;

		case fevent_CLOSE:
		case fevent_MENU:
		case fevent_FIND_AGAIN:
		case fevent_MENU_DEBUG:
		case fevent_DBOX_CANCEL:
		case fevent_INFO_PAGE:
		case fevent_OFFLINE_PAGE:
		case fevent_SEND_URL:
		case fevent_OPEN_WRITEABLE:
		case fevent_SOUND_TOGGLE:
		case fevent_SOUND_OFF:
		case fevent_SOUND_ON:
		case fevent_BEEPS_TOGGLE:
		case fevent_BEEPS_OFF:
		case fevent_BEEPS_ON:
		case fevent_SCALING_TOGGLE:
		case fevent_SCALING_OFF:
		case fevent_SCALING_ON:
		    break;
		}

	    case fevent_SUB_CLASS_CLIPBOARD:
		break;

	    case fevent_SUB_CLASS_SCROLL:
#ifdef ANT_NCFRESCO
                /* pdh: these buttons look really odd when shaded */
                possible = TRUE;
#else
		switch (event)
		{
		case fevent_SCROLL_LEFT:
		case fevent_SCROLL_PAGE_LEFT:
		case fevent_SCROLL_FAR_LEFT:
		    possible = fe_view_scroll_possible(v, -1, 0);
		    break;

		case fevent_SCROLL_RIGHT:
		case fevent_SCROLL_PAGE_RIGHT:
		case fevent_SCROLL_FAR_RIGHT:
		    possible = fe_view_scroll_possible(v, +1, 0);
		    break;

		case fevent_SCROLL_UP:
		case fevent_SCROLL_PAGE_UP:
		case fevent_SCROLL_TOP:
		    possible = fe_view_scroll_possible(v, 0, +1);
		    break;

		case fevent_SCROLL_DOWN:
		case fevent_SCROLL_PAGE_DOWN:
		case fevent_SCROLL_BOTTOM:
		    possible = fe_view_scroll_possible(v, 0, -1);
		    break;

		case fevent_SCROLL_OR_CURSOR_UP:
		case fevent_SCROLL_OR_CURSOR_DOWN:
		    break;
		}
		break;
#endif

	    case fevent_SUB_CLASS_HIGHLIGHT:
		break;
	    case fevent_SUB_CLASS_FOCUS:
		break;

	    case fevent_SUB_CLASS_OPEN:
		/* these are all toggle options so it's always true */
		break;

	    case fevent_SUB_CLASS_STATUS:
		break;
	    case fevent_SUB_CLASS_TOOLBAR:
		break;
	    case fevent_SUB_CLASS_CODEC:
		break;
	    case fevent_SUB_CLASS_TOOLBAR2:
		break;
	    case fevent_SUB_CLASS_FRAME_LINK:
		break;
            }
	    break;

	case fevent_FRAME_CLASS_URLS:
	    break;

	case fevent_FRAME_CLASS_ENCODING:
	    break;
	}
	break;

    case fevent_CLASS_MENU:
    case fevent_CLASS_MAP:
    case fevent_CLASS_PLUGIN:
	break;
    }
    return possible;
}

/* stbevent.c */
