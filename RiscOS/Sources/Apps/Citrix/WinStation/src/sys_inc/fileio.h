/* > fileio.h

 * © SJ Middleton, 1993

 */

#ifndef __unistd_h
# define __unistd_h

#ifndef __size_t
#  define __size_t 1
typedef unsigned int size_t;
#endif

#ifndef __time_t
#include <time.h>
#endif

#define O_OMASK     0x00000003

#define O_RDONLY    0
#define O_WRONLY    1
#define O_RDWR      2

#define O_APPEND    0x00000004
#define O_CREAT     0x00000010
#define O_TRUNC     0x00000020

#define O_BINARY    0x00002000
#define O_TEXT      0x00001000

/*
 * Return -1 for error
 * 0 or filehandle for success
 */

extern int read(int fhandle, void *data, size_t size);
extern int open(const char *filename, int mode);
extern int write(int fhandle, void *data, size_t size);
extern int close(int fhandle);
extern int flush(int fhandle);

#define _open(a,b)	open(a,b)
#define _close(a)	close(a)
#define _read(a,b,c)	read(a,b,c)
#define _write(a,b,c)	write(a,b,c)
#define _lseek(a,b,c)	lseek(a,b,c)

#ifndef SEEK_SET
#define SEEK_SET 0 /* start of stream (see fseek) */
#define SEEK_CUR 1 /* current position in stream (see fseek) */
#define SEEK_END 2 /* end of stream (see fseek) */
#endif

extern long lseek(int fd, long lpos, int whence);

typedef unsigned int off_t;

#define fileno(a) (((int *)(a))[5])

#ifndef _FSIZE_T_DEFINED
typedef unsigned long _fsize_t; /* Could be 64 bits for Win32 */
#define _FSIZE_T_DEFINED
#endif

struct _finddata_t {
    unsigned	attrib;
    time_t	time_create;	/* -1 for FAT file systems */
    time_t	time_access;	/* -1 for FAT file systems */
    time_t	time_write;
    _fsize_t	size;
    char	name[260];
};

/* File attribute constants for _findfirst() */

#define _A_NORMAL	0x00	/* Normal file - No read/write restrictions */
#define _A_RDONLY	0x01	/* Read only file */
#define _A_HIDDEN	0x02	/* Hidden file */
#define _A_SYSTEM	0x04	/* System file */
#define _A_SUBDIR	0x10	/* Subdirectory */
#define _A_ARCH 	0x20	/* Archive file */

extern long _findfirst(const char *path, struct _finddata_t *info);
extern int _findnext(long handle, struct _finddata_t *info);
extern int _findclose(long handle);

extern int _filelength(int fh);

#define _unlink(a) remove(a)
#define _remove(a) remove(a)
#define _rmdir(a) remove(a)

extern int mkdir(const char *name);
extern int _mkdir(const char *name);

#endif

/* eof fileio.h */
