
#ifndef __coords_h
# include "coords.h"
#endif

typedef enum
{
/*    pointermode_REFRESH = -1,*/
    pointermode_OFF,
    pointermode_ON
/*    pointermode_MAP,*/
/*    pointermode_INPUT*/
} pointermode_t;

extern pointermode_t pointer_mode;

extern void fe_pointer_mode_update(pointermode_t mode);
extern void fe_set_pointer(int item_flags);

extern fe_view main_view;
extern fe_view last_click_view;
extern fe_view dragging_view;
extern fe_view selected_view;
extern fe_view resizing_view;

extern int last_click_x, last_click_y;

extern int fe_copy_image_possible(fe_view v);
extern void fe_copy_image_to_clipboard(fe_view v);

extern int fe_copy_text_possible(fe_view v);
extern void fe_copy_text_to_clipboard(fe_view v);

extern os_error *fe_home(fe_view v);
extern void fe_move_highlight(fe_view v, int flags);
extern void fe_move_highlight_frame(fe_view v, BOOL next);
extern os_error *fe_handle_enter(fe_view v);

extern int fe_print_possible(fe_view v);
extern os_error *fe_print(fe_view v);
extern os_error *fe_print_options_open(fe_view v);
extern const char *fe_printer_name(void);

extern int fe_abort_fetch_possible(fe_view v);
extern os_error *fe_abort_fetch(fe_view v);

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

extern os_error *fe_hotlist_open(fe_view v);
extern os_error *fe_hotlist_and_url_open(fe_view v);
extern os_error *fe_url_open(fe_view v);

extern os_error *fe_status_toggle(fe_view v);
extern void fe_status_clear_fetch_only(void);
extern void fe_status_open_fetch_only(fe_view v);
extern wimp_w fe_status_window_handle(void);
extern int fe_status_height_top(fe_view v);
extern os_error *fe_status_info_level(fe_view v, int level);
extern int fe_status_open(fe_view v);
extern os_error *fe_status_unstack(fe_view v);
extern os_error *fe_status_open_toolbar(fe_view v, int bar);

extern void fe_show_mem_dump(void);

extern os_error *fe_open_version(fe_view v);

extern os_error *fe_display_options_open(fe_view v);

extern void fe_event_process(void);

extern void fe_scroll_changed(fe_view v);


#define key_list_CLICK      0x01    /* make an audible click*/
#define key_list_REPEAT     0x01    /* allow key to auto repeat*/

typedef struct
{
    short key;
    short event;
    int flags;
} key_list;

extern int fe_key_lookup(int chcode, const key_list *keys);
extern void fe_menu_event_handler(int event);

extern BOOL fe_item_screen_box(fe_view v, be_item ti, wimp_box *box);
extern void fe_fake_click(fe_view v, int x, int y);
extern coords_cvtstr fe_get_cvt(fe_view v);

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

extern void fe_force_fit(fe_view v, BOOL force);

extern void fe_iconise(BOOL iconise);
extern fe_view fe_locate_view(const char *name);
extern void fe_submit_form(fe_view v, const char *id);
extern void fe_open_keyboard(fe_view v);

extern os_error *iterate_frames(fe_view top, os_error *(*fn)(fe_view v, void *handle), void *handle);

/* From internal.h */

extern int print__copies, print__ul;
extern fe_passwd fe_current_passwd;

extern void fe_passwd_abort(void);

/* from stbredraw.c*/

extern int fe_view_scroll_x(fe_view v, int val);
extern int fe_view_scroll_y(fe_view v, int val);

/* from stbevents.c*/

extern void fevent_handler(int event, fe_view v);

/* from stbmap.c*/

/*extern BOOL fe_map_handle_keys(be_doc doc, be_item ti, wimp_w w, int key);*/
extern void fe_map_check_pointer_move(const wimp_mousestr *m);
extern fe_view fe_map_view(void);
extern void fe_map_mode(fe_view v, be_item ti);
extern void fe_map_event_handler(int event, fe_view v);

/* from stbkeys.c*/

#define fe_browser_mode_UNSET           (-1)
#define fe_browser_mode_WEB             0
#define fe_browser_mode_DESKTOP         1
#define fe_browser_mode_DBOX            2
#define fe_browser_mode_HOTLIST         3
#define fe_browser_mode_HISTORY         4
#define fe_browser_mode_APP		5

extern void fe_key_handler(fe_view v, wimp_eventstr *e, BOOL use_toolbox, int browser_mode);

/* from ncreg.c */

extern void ncreg_decode(void);
extern char *ncreg_enquiry(const char *tag);

/* from stbopen.c */

extern os_error *fe_search_page(fe_view v);
extern os_error *fe_offline_page(fe_view v);

/* stbfe.h*/

