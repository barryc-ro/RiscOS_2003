/* -*-c-*- */

/* statuswin.h */


typedef enum
{
    statuswin_CLOSED,
    statuswin_OPEN,
    statuswin_OPEN_SMALL
} statuswin_state_t;

extern statuswin_state_t statuswin_state;
extern int statuswin_height;
extern wimp_w statuswin_w;

extern void statuswin_clear(BOOL only_if_small);
extern os_error *statuswin_open(fe_view v, BOOL open_small);
extern os_error *statuswin_refresh_slider(fe_view v);
extern void statuswin_sprite_init(void);
extern os_error *statuswin_update_fetch_status(fe_view v, int status);
extern os_error *statuswin_draw_title(fe_view v, BOOL anti_twit);
extern os_error *statuswin_update_object_type(fe_view v, BOOL anti_twit);

extern BOOL statuswin_check_redraw(fe_view v, wimp_w w);
extern os_error *statuswin_info_level(fe_view v, int level);
extern os_error *statuswin_update_fetch_info(fe_view v, char *msg);
extern os_error *statuswin_toggle(fe_view v);
