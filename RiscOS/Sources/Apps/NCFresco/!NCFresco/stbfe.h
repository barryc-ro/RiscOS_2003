
#ifndef __coords_h
# include "coords.h"
#endif

typedef enum
{
    pointermode_OFF,
    pointermode_ON
} pointermode_t;

extern pointermode_t pointer_mode;
extern BOOL use_toolbox;
extern int wimp_version;
extern wimp_t task_handle;

extern wimp_t on_screen_kbd;
extern wimp_box on_screen_kbd_pos;

extern os_error pending_error;

extern int use_anti_twitter;

extern int fe_pending_event;
extern int fe_pending_key;

#define TASK_MAGIC	0x4B534154

#define DBOX_SIZE_X	800
#define DBOX_SIZE_Y	400

#define IconHigh_GetDirection		0x4E701
#define IconHigh_Start			0x4E702
#define IconHigh_Stop			0x4E703

#define MESSAGE_ERROR_BROADCAST		0x200000

extern void fe_pointer_mode_update(pointermode_t mode);
extern void fe_set_pointer(int item_flags);

extern fe_view main_view;
extern fe_view last_click_view;
extern fe_view dragging_view;
/* extern fe_view resizing_view; */

extern int last_click_x, last_click_y;

extern int fe_copy_image_possible(fe_view v);
extern void fe_copy_image_to_clipboard(fe_view v);

extern int fe_copy_text_possible(fe_view v);
extern void fe_copy_text_to_clipboard(fe_view v);

extern void fe_copy_whatever_to_clipboard(fe_view v);

extern os_error *fe_home(fe_view v);
extern void fe_move_highlight(fe_view v, int flags);
extern void fe_move_highlight_frame(fe_view v, BOOL next);
extern void fe_move_highlight_frame_direction(fe_view v, int flags);
extern void fe_move_highlight_xy(fe_view v, wimp_box *box, int flags);

extern int fe_print_possible(fe_view v);

/* must agree with arrays in stbfe.c */
#define fe_print_DEFAULT	0
#define fe_print_LEGAL		1
#define fe_print_LETTER		2

extern os_error *fe_print(fe_view v, int size);

extern const char *fe_printer_name(void);

extern int fe_abort_fetch_possible(fe_view v);
extern os_error *fe_abort_fetch(fe_view v, BOOL quiet);

extern int fe_doc_flag_state(fe_view v, int flags);
extern os_error *fe_doc_flag_toggle(fe_view v, int flags);

extern int fe_find_possible(fe_view v);
extern os_error *fe_find_open(fe_view v);
extern void fe_find(fe_view v, const char *text, int backwards, int casesense);
extern int fe_find_again_possible(fe_view v);
extern void fe_find_again(fe_view v);

extern os_error *fe_hotlist_add(fe_view v);
extern int fe_hotlist_add_possible(fe_view v);
extern os_error *fe_hotlist_remove(fe_view v);

extern os_error *fe_status_toggle(fe_view v);
extern void fe_status_clear_fetch_only(void);
extern void fe_status_open_fetch_only(fe_view v);
extern wimp_w fe_status_window_handle(void);
extern int fe_status_height_top(fe_view v);
extern os_error *fe_status_info_level(fe_view v, int level);
extern int fe_status_open(fe_view v);
extern os_error *fe_status_unstack(fe_view v);
extern BOOL fe_status_unstack_possible(fe_view source_v);
extern void fe_status_unstack_all(void);

extern os_error *fe_status_open_toolbar(fe_view v, int bar);
extern os_error *fe_status_state(fe_view v, int state);

extern void fe_show_mem_dump(void);

extern os_error *fe_open_version(fe_view v);
extern void fe_open_info(fe_view v, be_item ti, int x, int y, BOOL toggle);

extern void fe_event_process(void);

extern void fe_scroll_changed(fe_view v, int x, int y);
extern void fe_menu_event_handler(int event);

extern BOOL fe_item_screen_box(fe_view v, be_item ti, wimp_box *box);
extern void fe_fake_click(fe_view v, int x, int y);

extern BOOL fe_writeable_handle_keys(fe_view v, int key);

extern void fe_resize_abort(void);

extern void fe_dbox_cancel(void);
extern fe_view fe_dbox_view(const char *name);
extern void fe_dbox_dispose(void);

extern os_error *fe_paste(fe_view v);
extern os_error *fe_paste_url(fe_view v);

extern int fe_bg_colour(fe_view v);
extern int fe_check_download_finished(fe_view v);

extern void fe_plugin_event_handler(int event, fe_view v);

extern void fe_font_size_set(int value, BOOL absolute);
extern BOOL fe_font_size_set_possible(int value, BOOL absolute);
extern void fe_beeps_set(int state, BOOL make_sound);
extern void fe_bgsound_set(int state);
extern void fe_scaling_set(int state);

extern void fe_iconise(BOOL iconise);
extern fe_view fe_locate_view(const char *name);
extern fe_view fe_selected_view(void);

extern void fe_submit_form(fe_view v, const char *id);
extern void fe_keyboard_open(fe_view v);
extern void fe_keyboard_close(void);

extern os_error *iterate_frames(fe_view top, os_error *(*fn)(fe_view v, void *handle), void *handle);

extern void fe_cursor_movement(fe_view v, int x, int y);

#define fe_encoding_READ (-1)
extern int fe_encoding(fe_view v, int encoding);

extern int caretise(void);
extern int movepointer(void);

extern void fe_no_new_page(fe_view v, os_error *e);
extern void fe_ensure_highlight_after_fetch(fe_view v);
extern BOOL fe_check_autoscroll(fe_view v, const wimp_mousestr *mp);

extern void fe_user_unload(void);
extern void fe_user_load(void);

void fe_run_ncworks( void );

/* From internal.h */

extern int print__copies, print__ul;
extern fe_passwd fe_current_passwd;

extern BOOL fe_passwd_abort(void);
extern void fe_internal_deleting_view(fe_view v);
extern os_error *fe_internal_toggle_panel(const char *panel_name, int clear);
extern os_error *fe_internal_toggle_panel_args(const char *panel_name, const char *args, int clear);
extern void fe_internal_flush(void);
extern void fe_internal_optimise(void);
extern os_error *fe_internal_open_page(fe_view v, const char *page_name, int clear);

typedef void (*write_list_fn)(FILE *f, void *handle);
extern void fe_internal_write_page(FILE *f, const char *base_tag, int initial, int frame, write_list_fn write_list, void *handle);
extern BOOL fe_internal_check_popups(BOOL clear);
extern void fe_special_select(fe_view v);

/* from stbredraw.c*/

#define fe_view_scroll_ENSURE	0x01
#define fe_view_scroll_BEEP	0x02

extern int fe_view_scroll_x(fe_view v, int val, int flags);
extern int fe_view_scroll_y(fe_view v, int val, int flags);
extern BOOL fe_view_scroll_possible(fe_view v, int dx, int dy);

extern int fe_scroll_request(fe_view v, wimp_openstr *o, int x, int y);

/* from stbevents.c*/

extern void fevent_handler(int event, fe_view v);
extern BOOL fevent_possible(int event, fe_view v);

/* from stbmap.c*/

/*extern BOOL fe_map_handle_keys(be_doc doc, be_item ti, wimp_w w, int key);*/
extern void fe_map_check_pointer_move(const wimp_mousestr *m);
extern fe_view fe_map_view(void);
extern void fe_map_mode(fe_view v, be_item ti);
extern void fe_map_event_handler(int event, fe_view v);

/* from stbkeys.c */

#define fe_browser_mode_UNSET           (-1)
#define fe_browser_mode_WEB             0
#define fe_browser_mode_DESKTOP         1
#define fe_browser_mode_DBOX            2
#define fe_browser_mode_HOTLIST         3
#define fe_browser_mode_HISTORY         4
#define fe_browser_mode_APP		5

extern void fe_key_handler(fe_view v, wimp_eventstr *e, BOOL use_toolbox, int browser_mode);

extern void stbkeys_list_add(const void *info);
extern void stbkeys_list_clear(void);
extern void stbkeys_list_loaded(void);

/* from ncreg.c */

extern void ncreg_decode(void);
extern char *ncreg_enquiry(const char *tag);

/* from stbopen.c */

/* From stblinks.c */

extern void fe_frame_link_array_build(fe_view v);
extern void fe_frame_link_array_free(fe_view v);

extern fe_view fe_next_frame(fe_view v, BOOL next);

extern void fe_frame_link_move(fe_view v, int flags);
extern void fe_frame_link_activate(fe_view v);
extern int fe_frame_link_selected(fe_view v);
extern void fe_frame_link_clear_all(fe_view v);
extern void fe_frame_link_redraw_all(fe_view v);

extern os_error *fe_handle_enter(fe_view v);
extern os_error *fe_activate_link(fe_view v, int x, int y, int bbits);

extern int fe_ensure_highlight(fe_view v, int scroll_flags);

/* From stbcodec.c */

extern void codec_event_handler(int event, fe_view v);

/* stbfe.h*/

