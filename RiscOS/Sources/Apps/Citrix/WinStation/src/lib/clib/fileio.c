/* > fileio.c

 */

#include "swis.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/clib.h"
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

int mkdir(const char *filename)
{
    return _swix(OS_File, _INR(0,1) | _IN(4), 8, filename, 0) ? 0 : -1;
}

typedef struct
{
    char *dir;
    char *leaf;
    int index;
} find_context;

long _findfirst(const char *path, struct _finddata_t *info)
{
    find_context *fc = calloc(sizeof(find_context), 1);
    char *dot;

    if (!fc)
	return -1;
    
    fc->dir = strdup(path);

    dot = strrchr(fc->dir, '.');
    if (dot)
    {
	*dot = 0;
	fc->leaf = dot + 1;
    }

    if (_findnext((long)fc, info) == -1)
    {
	_findclose((long)fc);
	return -1;
    }
    
    return (long)fc;
}

int _findnext(long handle, struct _finddata_t *info)
{
    find_context *fc = (find_context *)handle;
    int buffer[128/4];
    int nread;

    do
    {
	if (_swix(OS_GBPB, _INR(0,6) | _OUTR(3,4),
		  10, fc->dir, buffer, 1, fc->index,
		  sizeof(buffer), fc->leaf,
		  &nread, &fc->index) != NULL)
	    return -1;
    }
    while (nread != 1);
    
    info->attrib = 0;

    if ((buffer[3] & 3) != 3)
	info->attrib |= _A_RDONLY;

    if (buffer[4] & 2)
	info->attrib |= _A_SUBDIR;

    info->time_create = -1;
    info->time_access = -1;
    info->time_write = 0;
    info->size = buffer[2];

    strncpy(info->name, (char *)buffer[6], sizeof(info->name));
    info->name[sizeof(info->name)-1] = 0;

    return fc->index == -1 ? -1 : 0;
}

int _findclose(long handle)
{
    find_context *fc = (find_context *)handle;

    free(fc->dir);
    free(fc);

    return 0;
}

int _filelength(int fd)
{
    int ext;
    return _swix(OS_Args, _INR(0,1)|_OUT(2), 2, fd, &ext) ? -1 : ext;
}

/* eof fileio.c */
