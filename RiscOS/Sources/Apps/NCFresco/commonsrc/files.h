/* -*-c-*- */

#define RO_OPEN_READ	0
#define RO_OPEN_WRITE	1

extern int ro_fopen(const char *fname, int mode);
extern void ro_fclose(int fh);
extern int ro_fwrite(void *ptr, int size, int items, int fh);
extern int ro_fwritepos(void *ptr, int size, int items, int fh, int pos);
extern int ro_fread(void *ptr, int size, int items, int fh);
extern int ro_ensure_size(int fh, int size);
extern int ro_freadpos(void *ptr, int size, int items, int fh, int pos);
extern char *ro_ferror(void);
extern int ro_get_extent(int fh);
