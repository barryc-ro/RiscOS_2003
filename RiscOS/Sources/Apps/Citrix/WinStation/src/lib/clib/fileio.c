/* > fileio.c

 */

#include "windows.h"

#include "swis.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/client.h"
#include "../inc/clib.h"
#include "../inc/debug.h"

#include "fileio.h"

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

    if (LOGERR(_swix(OS_Find, _IN(0)|_IN(1)|_OUT(0), flags, filename, &fhandle)) != NULL)
    {
	TRACE((TC_ALL, TT_ERROR, "filename: '%s'", filename));
    }

    return fhandle > 0 ? fhandle : -1;
}

int close(int fhandle)
{
    return LOGERR(_swix(OS_Find, _IN(0)|_IN(1), 0, fhandle)) == NULL ? 0 : -1;
}

int read(int fhandle, void *data, size_t size)
{
    return LOGERR(_swix(OS_GBPB, _INR(0,3), 4, fhandle, data, size)) == NULL ? size : -1;
}

int write(int fhandle, void *data, size_t size)
{
    return LOGERR(_swix(OS_GBPB, _INR(0,3), 2, fhandle, data, size)) == NULL ? size : -1;
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
            LOGERR(_swix(OS_Args, _IN(0)|_IN(1)|_OUT(2), 0, fd, &pos));
            lpos += pos;
            break;
        }

        case SEEK_END:
        {
            int ext = 0;
            LOGERR(_swix(OS_Args, _IN(0)|_IN(1)|_OUT(2), 2, fd, &ext));
            lpos = ext - lpos;
            break;
        }
    }

    return _swix(OS_Args, _IN(0)|_IN(1)|_IN(2), 1, fd, lpos) == NULL ? lpos : -1;
}

int flush(int fhandle)
{
    TRACE((TC_CLIB, TT_API1, "flush: %d", fhandle));

    return LOGERR(_swix(OS_Args, _INR(0,1), 255, fhandle)) ? 0 : -1;
}

int _mkdir(const char *filename)
{
    TRACE((TC_CLIB, TT_API1, "_mkdir: '%s'", filename));

    return LOGERR(_swix(OS_File, _INR(0,1) | _IN(4), 8, filename, 0)) ? 0 : -1;
}

int mkdir(const char *filename)
{
    char *ss = strdup(filename), *s;
    int c;
    int r = -1;

    TRACE((TC_CLIB, TT_API1, "mkdir: '%s'", filename));

    for (s = ss; *s; s++)
    {
	if (*s == '.')
	{
	    *s = '\0';
	    if ((r = _mkdir(ss)) == 0)
		break;
	    *s = '.';
	}
    }

    if (r != 0)
	r = _mkdir(ss);

    free(ss);

    return r;
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

    TRACE((TC_CLIB, TT_API1, "_findfirst: '%s' info %p", path, info));

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
    
    TRACE((TC_CLIB, TT_API1, "_findfirst: handle %p", fc));

    return (long)fc;
}

int _findnext(long handle, struct _finddata_t *info)
{
    find_context *fc = (find_context *)handle;
    int buffer[128/4];
    int nread, r;
    
    TRACE((TC_CLIB, TT_API1, "_findnext: %p info %p index %d dir '%s' leaf '%s'", fc, info, fc->index, fc->dir, fc->leaf));

    do
    {
	if (LOGERR(_swix(OS_GBPB, _INR(0,6) | _OUTR(3,4),
			 10,
			 fc->dir,
			 buffer,
			 1,			// read one entry
			 fc->index,
			 sizeof(buffer),
			 fc->leaf,
			 &nread,
			 &fc->index)) != NULL)
	{
	    TRACE((TC_ALL, TT_ERROR, "_findnext: dir '%s' return -1", fc->dir));
	    
	    return -1;
	}
    }
    while (nread == 0 && fc->index != -1);
    
    if (nread != 0)
    {
	info->attrib = 0;

	if ((buffer[3] & 3) != 3)	// attributes
	    info->attrib |= _A_RDONLY;

	if (buffer[4] & 2)		// object type
	    info->attrib |= _A_SUBDIR;

	info->time_create = -1;
	info->time_access = -1;
	info->time_write = 0;	// [0] = load, [1] = exec
	info->size = buffer[2];	// length

	strncpy(info->name, (char *)&buffer[5], sizeof(info->name));
	info->name[sizeof(info->name)-1] = 0;

	TRACE((TC_CLIB, TT_API1, "_findnext: %p size %d name '%s'", fc, info->size, info->name));
    }

    r = fc->index == -1 ? -1 : 0;

    TRACE((TC_CLIB, TT_API1, "_findnext: return %d", r));

    return r;
}

int _findclose(long handle)
{
    find_context *fc = (find_context *)handle;

    TRACE((TC_CLIB, TT_API1, "_findclose: handle %p", fc));

    free(fc->dir);
    free(fc);

    return 0;
}

int _filelength(int fd)
{
    int ext;
    return LOGERR(_swix(OS_Args, _INR(0,1)|_OUT(2), 2, fd, &ext)) ? -1 : ext;
}

/* eof fileio.c */
