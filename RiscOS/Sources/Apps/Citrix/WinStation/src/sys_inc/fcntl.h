/* > unistd.h

 * © SJ Middleton, 1993

 */

#ifndef __unistd_h
# define __unistd_h

#ifndef __size_t
#  define __size_t 1
typedef unsigned int size_t;
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


#ifndef SEEK_SET
#define SEEK_SET 0 /* start of stream (see fseek) */
#define SEEK_CUR 1 /* current position in stream (see fseek) */
#define SEEK_END 2 /* end of stream (see fseek) */
#endif

extern long lseek(int fd, long lpos, int whence);

typedef unsigned int off_t;

#define fileno(a) (((int *)(a))[5])

#endif

/* eof unistd.h */
