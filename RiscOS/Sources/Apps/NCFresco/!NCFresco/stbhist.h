/* > stbhist.h
 *
 */

#define	history_FIRST	0
#define	history_PREV	1
#define	history_NEXT	2
#define	history_LAST	3

extern void fe_history_dispose(fe_view v);
extern int fe_history_visit(fe_view v, const char *url, const char *title);
extern os_error *fe_history_move(fe_view v, int a);
extern os_error *fe_history_show(fe_view v);
extern int fe_history_possible(fe_view v, int a);
extern int fe_test_history(fe_view v, const char *url);
extern void fe_global_history_dispose(void);
extern os_error *fe_global_write_list(FILE *f);
/* extern char *fe_history_get_url(fe_view v, int direction); */
extern os_error *fe_history_write_list(FILE *f, const fe_history_item *start, const fe_history_item *current);
extern os_error *fe_history_write_combined_list(FILE *f, const fe_history_item *start, const fe_history_item *current);
extern void fe_history_update_current_state(fe_view v);

/* stbhist.c */
