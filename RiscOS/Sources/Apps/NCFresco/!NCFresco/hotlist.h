/* -*-c-*- */

/* hotlist.h */

#ifndef __hotlist_h
# define __hotlist_h

#ifndef __stdio_h
# include <stdio.h>
#endif

extern os_error *hotlist_add(const char *url, const char *title, const char *language);
extern os_error *hotlist_remove(const char *url);
extern void hotlist_write_list(FILE *fout, void *handle);
extern void hotlist_write_delete_list(FILE *fout, void *handle);
extern BOOL hotlist_read(const char *file);
extern BOOL hotlist_write(const char *file);
extern void hotlist_init(void);
extern void hotlist_shutdown(void);
extern void hotlist_remove_list(const char *list);
extern void hotlist_return_url(int index, char **url);
extern os_error *hotlist_flush_pending_delete(void);
extern void hotlist_optimise(void);

#endif

/* eof hotlist.h */
