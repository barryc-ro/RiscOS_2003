/* > stbopen.h

 */

/* Targets defined by Netscape */
#define TARGET_BLANK    "_blank"
#define TARGET_SELF     "_self"
#define TARGET_PARENT   "_parent"
#define TARGET_TOP      "_top"

/* Targets defined for internal use */
#define TARGET_HELP	"__help"
#define TARGET_VERY_TOP	"__top"

extern fe_view fe_find_top(fe_view v);
extern fe_view fe_find_window(fe_view start, wimp_w w);

extern os_error *fe_show_file_in_frame(fe_view v, char *file, char *frame);
extern os_error *fe_show_file(fe_view v, char *file, int no_history);

extern int fe_reload_possible(fe_view v);
extern os_error *fe_reload(fe_view v);

extern os_error *fe_new_view(fe_view parent, const wimp_box *extent, const fe_frame_info *ip, fe_view *vp);
extern void fe_dispose_view_children(fe_view v);
extern void fe_dispose_view(fe_view v);

/* stbopen.h */
