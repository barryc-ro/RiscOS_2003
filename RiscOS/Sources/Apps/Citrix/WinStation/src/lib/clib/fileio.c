/* > unistd.c

 */

#include "swis.h"

#include <stddef.h>
#include <stdio.h>

#include "fcntl.h"

int open(const char *filename, int mode)
{
    int flags = 0, fhandle = 0;
    switch (mode & O_OMASK)
    {
        case O_RDONLY:
            flags = 0x4F;
            break;
        case O_WRONLY:
            flags = 0x8F;
            break;
        case O_RDWR:
            flags = mode & O_CREAT ? 0x8F : 0xCF;
            break;
    }

    _swix(OS_Find, _IN(0)|_IN(1)|_OUT(0), flags, filename, &fhandle);

    return fhandle > 0 ? fhandle : -1;
}

int close(int fhandle)
{
    return _swix(OS_Find, _IN(0)|_IN(1), 0, fhandle) == NULL ? 0 : -1;
}

int read(int fhandle, void *data, size_t size)
{
    return _swix(OS_GBPB, _IN(0)|_IN(1)|_IN(2)|_IN(3), 4, fhandle, data, size) == NULL ? size : -1;
}

int write(int fhandle, void *data, size_t size)
{
    return _swix(OS_GBPB, _IN(0)|_IN(1)|_IN(2)|_IN(3), 2, fhandle, data, size) == NULL ? size : -1;
}

long lseek(int fd, long lpos, int whence)
{
    switch (whence)
    {
        case SEEK_SET:
            break;

        case SEEK_CUR:
        {
            int pos = 0;
            _swix(OS_Args, _IN(0)|_IN(1)|_OUT(2), 0, fd, &pos);
            lpos += pos;
            break;
        }

        case SEEK_END:
        {
            int ext = 0;
            _swix(OS_Args, _IN(0)|_IN(1)|_OUT(2), 2, fd, &ext);
            lpos = ext - lpos;
            break;
        }
    }

    return _swix(OS_Args, _IN(0)|_IN(1)|_IN(2), 1, fd, lpos) == NULL ? lpos : -1;
}

int flush(int fhandle)
{
    return _swix(OS_Args, _INR(0,1), 255, fhandle) ? 0 : -1;
}

/* eof unistd.c */
