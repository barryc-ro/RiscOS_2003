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
#define TARGET_DBOX	"__dbox"
#define TARGET_PASSWORD	"__passwd"
/* #define TARGET_FAVS	"__favs" */
#define TARGET_HISTORY	"__history"
#define TARGET_INFO	"__info"
/* ;#define TARGET_OPEN	"__open" */
/* ;#define TARGET_CUSTOM	"__custom" */
#define TARGET_ERROR	"__error"
#define TARGET_FIND	"__find"

extern fe_view fe_find_top(fe_view v);
extern fe_view fe_find_window(fe_view start, wimp_w w);
extern fe_view fe_find_target(fe_view start, const char *target);
extern fe_view fe_find_top_popup(fe_view v);
extern fe_view fe_find_top_nopopup(fe_view v);
extern int fe_popup_open(void);

extern fe_view fe_frame_specifier_decode(fe_view top, const char *spec);
extern char *fe_frame_specifier_create(fe_view v, char *buf, int len);

extern os_error *fe_show_file_in_frame(fe_view v, char *file, char *frame);
extern os_error *fe_show_file(fe_view v, char *file, int no_history);

extern int fe_reload_possible(fe_view v);
extern os_error *fe_reload(fe_view v);

extern os_error *fe_new_view(fe_view parent, const wimp_box *extent, const fe_frame_info *ip, BOOL open, fe_view *vp);
extern void fe_dispose_view_children(fe_view v);
extern void fe_dispose_view(fe_view v);

extern os_error *fe_internal_url_with_source(fe_view v, const char *internal, const char *target);

/* stbopen.h */
