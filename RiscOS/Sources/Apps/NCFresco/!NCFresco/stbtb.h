/* -*-c-*- */

/* stbtb.h */

/* main handlers */
extern int tb_init(int *m_list, int *wimp_version);
extern void tb_events(int *event, fe_view v);
extern void tb_cleanup(void);
extern void tb_optimise(void);

/* extern BOOL tb_key_handler(wimp_caretstr *cs, int key); */
extern void tb_event_handler(int event, fe_view v);

/* emulation calls for Risco_OSLib stuff */
extern void *tb_resspr_area(void);
extern void tb_resspr_init(void);
extern void tb_msgs_init(void);
extern char *tb_msgs_lookup(char *tag);
extern void tb_res_init(char *program_name);

/* status bar control */
extern void tb_status_show(int small_only);
extern void tb_status_hide(int only_if_small);
extern int tb_is_status_showing(void);
extern int tb_status_height(void);
extern int tb_status_w(void);
extern BOOL tb_status_has_highlight(void);
extern void tb_status_update_fades(fe_view v);
extern void tb_status_resize(int xdiff, int ydiff);
extern int tb_status_redraw(wimp_redrawstr *r);
extern void tb_status_refresh_if_small(void);
extern void tb_status_box(wimp_box *box);

#define light_state_OFF         0
#define light_state_CONNECTING  1
#define light_state_FETCHING    2
#define light_state_FINISHED    3

extern void tb_status_set_lights(int state);
extern void tb_status_set_secure(int on);
extern void tb_status_set_direction(int up);
extern void tb_status_set_modem(int on);
extern void tb_status_set_key_leds(void);

#define status_type_FETCH   0
#define status_type_URL     1
#define status_type_TITLE   2
#define status_type_LINK    3
#define status_type_HELP    4
#define status_type_ERROR   5

extern os_error *tb_status_set_message(int type, const char *msg);
extern os_error *tb_status_set_messagef(int type, const char *msg, ...);
extern int tb_status_check_message(wimp_mousestr *mp);

extern void tb_status_rotate(void);
extern void tb_status_rotate_reset(void);
extern void tb_status_init(void);

extern BOOL tb_status_unstack_possible(void);
extern BOOL tb_status_unstack(BOOL do_exit_fn);
extern void tb_status_new(fe_view v, int bar_num);
extern BOOL tb_status_highlight(BOOL gain);
extern void tb_status_unstack_all(BOOL leave_bar_up);
extern void tb_status_highlight_stop(void);

extern BOOL tb_status_check_pointer(wimp_mousestr *mp);

#define tb_status_button_INACTIVE	0
#define tb_status_button_ACTIVE		1
#define tb_status_button_FADED		2
#define tb_status_button_UNPRESSED	3
#define tb_status_button_PRESSED	4
extern void tb_status_button(int event, int active);

extern void tb_open_url(void);
extern void tb_open_url_and_close(void);

extern void tb_codec_state_change(int state, int opening, int closing);
extern void tb_codec_kill(void);

/* menu control */
extern int tb_menu_showing(void);
extern void tb_menu_show(fe_view v, int which_menu);
extern void tb_menu_help(void);
extern void tb_menu_refresh(fe_view v);
extern void tb_menu_hide(void);


/* dbox control */
extern os_error *tb_find_open(fe_view v);
extern int tb_find_redraw(wimp_redrawstr *r);
extern os_error *tb_print_options_open(fe_view v);
extern int tb_print_redraw(wimp_redrawstr *r);

extern void tb_bar_add(const void *info);
extern void tb_bar_name(const void *info);
extern int tb_bar_get_num_from_name(const char *name);


/* eof stbtb.h */
