/* -*-c-*- */

/* config.c */

#ifndef __config_h
#define __config_h

#ifndef __wimp_h
#include "wimp.h"
#endif

#ifndef __rcolours_h
#include "rcolours.h"
#endif

extern void config_init(void);
extern void config_tidyup(void);
extern void config_write_file(void);
extern void config_read_file(void);
extern void config_write_file_by_name(const char *file_name);
extern void config_read_file_by_name(const char *file_name);
extern int config_colour_number(char *p);

#define config_SCALES	3

struct config_str
{
    int max_files_fetching;
    char *hotlist_file;
    int truncate_length;
    int truncate_suffix;
    int deep_images;
    int defer_images;
    char *email_addr;
    char *auth_file;
    char *runable_file;
    char *allow_file;
    char *deny_file;
    char *help_file;
    char *cookie_file;
    char *history_file;

    int auth_file_crypt;
    char *animation_name;
    int animation_frames;
    char *doc_default;
    char *document_handler_user;
    char *document_handler_related;
    char *document_search;
    char *document_offline;

    int display_links_underlined;
    int display_body_colours;
    int display_antialias;
    int display_control_buttons;
    int display_control_urlline;
    int display_control_status;
    int display_control_top;
    int display_control_initial;
    int display_fancy_ptr;
    int display_map_coords;

    int display_scale;
    int display_scale_image;
    int display_scales[config_SCALES];
    int display_scale_fit;
    int display_scale_fit_mode;
    int display_width;
    int display_blending;
    wimp_box display_margin;
    int display_margin_auto;
    int display_frames;
    int display_frames_scrollbars;
    int display_frames_top_level;
    int display_smooth_scrolling;

    int display_time_activate;
    int display_time_background;
    int display_time_fade;
    int display_jpeg;
    int display_leading;
    int display_leading_percent;
    int display_char_password;

    int display_highlight_style;
    int display_highlight_width;

    int display_tables;

    int encoding_user;
    int encoding_user_override;
    int encoding_internal;
    char *encoding_accept;

    int proxy_http_on;
    int proxy_https_on;
    int proxy_gopher_on;
    int proxy_ftp_on;
    int proxy_mailto_on;

    char *proxy_http;
    char *proxy_https;
    char *proxy_gopher;
    char *proxy_ftp;
    char *proxy_mailto;

    char *proxy_http_ignore;
    char *proxy_https_ignore;
    char *proxy_gopher_ignore;
    char *proxy_ftp_ignore;

    int cache_size;
    int cache_items;
    int cache_keep;
    int cache_keep_uptodate;

    int print_nopics;
    int print_nobg;
    int print_nocol;
    int print_sideways;
    int print_scale;
    int print_collated;
    int print_reversed;
    int print_border;

    int history_length;
    int history_global_length;
    int history_persist;

    char *sound_click;
    int sound_fx;
    int sound_background;

    wimp_paletteword colours[render_colour_COUNT];
    wimp_paletteword *colour_list[render_colour_list_COUNT];

    /* Font configuration */

    int font_sizes[7];
    char *font_names[20];
    int font_scales[9];
    int font_aspects[9];

    int mode_keyboard;
    int mode_cursor_toolbar;
    int mode_platform;
    int mode_mouse_adjust;
    int mode_mouse_menu;
    int mode_errors;

    int event_smartcard_in;
    int event_smartcard_out;
    int event_standby_in;
    int event_standby_out;
    
    int cookie_enable;
    int cookie_uptodate;

    int passwords_uptodate;
    int broken_formpost;

    char *plugin_file;
    int plugin_uptodate;

    int netscape_fake;

    int hots_length;

    char *language_preference;
    int browser_version;

    char *image_blacklist;

    char *publishing_fetch_prefix;
    char *publishing_send_prefix;
    char *publishing_more;
    char *publishing_index_page;
    int publishing_active;

    int hots_run_hotlist;

    int *key;			/* place holders fort NCFresco key and toolbar maps */
    int *toolbar;
    char *toolbar_name;

    int *url_suffix;

    char *header_useragent[2];
};

extern struct config_str config_array;

#define platform_RISCOS_DESKTOP 0
#define platform_NC             1
#define platform_CAMB_TRIAL     2

#define config_max_files_fetching (config_array.max_files_fetching)
#define config_truncate_length (config_array.truncate_length)
#define config_truncate_suffix (config_array.truncate_suffix)

#define config_animation_name (config_array.animation_name)
#define config_animation_frames (config_array.animation_frames)
#define config_deep_images (config_array.deep_images)
#define config_defer_images (config_array.defer_images)

#define config_email_addr (config_array.email_addr)

#define config_hotlist_file (config_array.hotlist_file)
#define config_auth_file (config_array.auth_file)
#define config_runable_file (config_array.runable_file)
#define config_allow_file (config_array.allow_file)
#define config_deny_file (config_array.deny_file)
#define config_help_file (config_array.help_file)
#define config_cookie_file (config_array.cookie_file)
#define config_history_file (config_array.history_file)

#define config_auth_file_crypt (config_array.auth_file_crypt)
#define config_doc_default (config_array.doc_default)
#define config_document_handler_user (config_array.document_handler_user)
#define config_document_handler_related (config_array.document_handler_related)
#define config_document_search (config_array.document_search)
#define config_document_offline (config_array.document_offline)

#define config_display_links_underlined (config_array.display_links_underlined)
#define config_display_body_colours (config_array.display_body_colours)
#define config_display_antialias (config_array.display_antialias)
#define config_display_control_buttons (config_array.display_control_buttons)
#define config_display_control_urlline (config_array.display_control_urlline)
#define config_display_control_status (config_array.display_control_status)
#define config_display_control_top (config_array.display_control_top)
#define config_display_control_initial (config_array.display_control_initial)
#define config_display_fancy_ptr (config_array.display_fancy_ptr)

#define config_display_scale (config_array.display_scale)
#define config_display_scale_fit (config_array.display_scale_fit)
#define config_display_scale_fit_mode (config_array.display_scale_fit_mode)
#define config_display_scale_image (config_array.display_scale_image)
#define config_display_scales (config_array.display_scales)
#define config_display_width (config_array.display_width)
#define config_display_blending (config_array.display_blending)
#define config_display_margin (config_array.display_margin)
#define config_display_margin_auto (config_array.display_margin_auto)
#ifdef BUILDERS
#define config_display_frames 1
#else
#define config_display_frames (config_array.display_frames)
#endif
#define config_display_frames_scrollbars (config_array.display_frames_scrollbars)
#define config_display_frames_top_level (config_array.display_frames_top_level)
#define config_display_smooth_scrolling (config_array.display_smooth_scrolling)
#define config_display_time_activate (config_array.display_time_activate)
#define config_display_time_background (config_array.display_time_background)
#define config_display_time_fade (config_array.display_time_fade)
#define config_display_map_coords (config_array.display_map_coords)
#define config_display_jpeg (config_array.display_jpeg)
#define config_display_leading (config_array.display_leading)
#define config_display_leading_percent (config_array.display_leading_percent)
#define config_display_char_password (config_array.display_char_password)

#define config_encoding_user (config_array.encoding_user)
#define config_encoding_user_override (config_array.encoding_user_override)
#define config_encoding_internal (config_array.encoding_internal)
#define config_encoding_accept (config_array.encoding_accept)

#define highlight_style_SIMPLE	0
#define highlight_style_RCA	1

#define config_display_highlight_style (config_array.display_highlight_style)
#define config_display_highlight_width (config_array.display_highlight_width)
#define config_display_tables (config_array.display_tables)

#define config_proxy_http_on (config_array.proxy_http_on)
#define config_proxy_https_on (config_array.proxy_https_on)
#define config_proxy_gopher_on (config_array.proxy_gopher_on)
#define config_proxy_ftp_on (config_array.proxy_ftp_on)
#define config_proxy_mailto_on (config_array.proxy_mailto_on)

#define config_proxy_http (config_array.proxy_http)
#define config_proxy_https (config_array.proxy_https)
#define config_proxy_gopher (config_array.proxy_gopher)
#define config_proxy_ftp (config_array.proxy_ftp)
#define config_proxy_mailto (config_array.proxy_mailto)

#define config_proxy_http_ignore (config_array.proxy_http_ignore)
#define config_proxy_https_ignore (config_array.proxy_https_ignore)
#define config_proxy_gopher_ignore (config_array.proxy_gopher_ignore)
#define config_proxy_ftp_ignore (config_array.proxy_ftp_ignore)

#define config_colours (config_array.colours)
#define config_colour_list (config_array.colour_list)

#define config_font_scales (config_array.font_scales)
#define config_font_aspects (config_array.font_aspects)
#define config_font_sizes (config_array.font_sizes)
#define config_font_names (config_array.font_names)

#define config_cache_size (config_array.cache_size)
#define config_cache_items (config_array.cache_items)
#define config_cache_keep (config_array.cache_keep)
#define config_cache_keep_uptodate (config_array.cache_keep_uptodate)

#define config_print_nopics (config_array.print_nopics)
#define config_print_nobg (config_array.print_nobg)
#define config_print_nocol (config_array.print_nocol)
#define config_print_sideways (config_array.print_sideways)
#define config_print_scale (config_array.print_scale)
#define config_print_collated (config_array.print_collated)
#define config_print_reversed (config_array.print_reversed)
#define config_print_border (config_array.print_border)

#define config_history_length (config_array.history_length)
#define config_history_global_length (config_array.history_global_length)
#define config_history_persist (config_array.history_persist)

#define config_sound_click (config_array.sound_click)
#define config_sound_fx (config_array.sound_fx)
#define config_sound_background (config_array.sound_background)
#define config_mode_keyboard (config_array.mode_keyboard)
#define config_mode_cursor_toolbar (config_array.mode_cursor_toolbar)
#define config_mode_platform (config_array.mode_platform)
#define config_mode_mouse_adjust (config_array.mode_mouse_adjust)
#define config_mode_mouse_menu (config_array.mode_mouse_menu)

#define mode_errors_WIMP	0
#define mode_errors_OWN		1
#define mode_errors_MESSAGE	2

#define config_mode_errors (config_array.mode_errors)

#define config_event_smartcard_in (config_array.event_smartcard_in)
#define config_event_smartcard_out (config_array.event_smartcard_out)
#define config_event_standby_in (config_array.event_standby_in)
#define config_event_standby_out (config_array.event_standby_out)

#define config_cookie_enable (config_array.cookie_enable)
#define config_cookie_uptodate (config_array.cookie_uptodate)

#define config_passwords_uptodate (config_array.passwords_uptodate)

#define config_broken_formpost (config_array.broken_formpost)

#define config_plugin_file (config_array.plugin_file)
#define config_plugin_uptodate (config_array.plugin_uptodate)

#define config_netscape_fake (config_array.netscape_fake)
#define config_hots_length (config_array.hots_length)

#define config_publishing_more (config_array.publishing_more)
#define config_publishing_fetch_prefix (config_array.publishing_fetch_prefix)
#define config_publishing_send_prefix (config_array.publishing_send_prefix)
#define config_publishing_index_page (config_array.publishing_index_page)
#define config_publishing_active (config_array.publishing_active)

#define config_language_preference (config_array.language_preference)

#define config_hots_run_hotlist (config_array.hots_run_hotlist)

/* Fresco uses this to automatically reset old config files to contain
 * frames = yes
 */
#define config_browser_version (config_array.browser_version)

#define config_image_blacklist (config_array.image_blacklist)
#define config_url_suffix (config_array.url_suffix)

#define config_header_useragent (config_array.header_useragent)

#endif

/* eof config.h */
