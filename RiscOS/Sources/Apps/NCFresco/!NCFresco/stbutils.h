/* -*-c-*- */

/* feutils.h */

#ifndef __wimp_h
# include "wimp.h"
#endif

#ifndef __stdio_h
#include <stdio.h>
#endif

extern int frontend_dx, frontend_dy;
extern wimp_box text_safe_box, screen_box, margin_box, error_box;
extern int toolsprite_width, toolsprite_height;

extern BOOL tv_safe_area(wimp_box *box);
extern BOOL is_a_tv(void);

extern os_error *fe_get_sprite(char *name, sprite_id *id_out);
extern os_error *fe_get_sprite_scale(sprite_id *id, sprite_factors *factors, int *width, int *height);
extern os_error *fe_get_sprite_size(char *name, int *width, int *height);
extern os_error *fe_get_sprite_pixtrans(sprite_id *id, sprite_pixtrans **pixtrans_out, BOOL *wide);
extern os_error *fe_plot_sprite(char *name, int x, int y, wimp_box *bb);
extern void fe_anti_twitter(const wimp_box *bb);
extern os_error *fe_sprite_put_scaled(sprite_area *area, sprite_id *id, BOOL wide, int x, int y, sprite_factors *factors, sprite_pixtrans *pixtrans);
extern void toggle_anti_twitter(void);
extern void feutils_init_1(void);
extern void feutils_init_2(void);
extern void fe_message(const char *msg);
extern void fe_report_error(const char *msg);

extern os_error *feutils_open_behind_toolbar(wimp_w w);
os_error *feutils_open_in_front(wimp_w);
extern os_error *feutils_window_create(wimp_box *box, const wimp_box *margin, const fe_frame_info *ip, int bgcol, BOOL open, wimp_w *w_out);
extern int feutils_check_window_bars(wimp_box *extent, wimp_openstr *op, int *old_x_scroll_bar, int *old_y_scroll_bar, int bgcol);
extern void feutils_resize_window(wimp_w *w, const wimp_box *margin, const wimp_box *box, int *x_scroll_bar, int *y_scroll_bar, int width, int height, int scrolling, int bgcol);

extern void fe_click_sound(void);
extern void fe_get_wimp_caret(wimp_w w);
extern void fe_pointer_set_position(int x, int y);
extern void fe_refresh_screen(const wimp_box *box);
extern void fe_refresh_window(wimp_w w, const wimp_box *box);
extern void extend_redraw_area(wimp_redrawstr *r);

#define session_REQUESTED   0
#define session_CONNECTED   1
#define session_FAILED      2

extern void session_log(const char *url, int state);

extern os_error *fe_file_to_url(char *file, char **url_out);

typedef os_error *(*fe_temp_file_builder)(FILE *f, void *handle);
extern void fe_open_temp_file(fe_view v, fe_temp_file_builder fn, void *handle, const char *name);

extern char *extract_value(const char *s, const char *tag);
extern char *check_url_prefix(const char *url);
extern os_error *fe_start_task(const char *cli, wimp_t *task_out);

extern int check_edge_proximity(int pos, int left, int right, int threshold);

wimp_w feutils_find_top_window(wimp_w awindow);
