/* -*-c-*- */

#include <stddef.h>

#include "kernel.h"

#include "files.h"
#include "version.h"

int ro_fopen(const char *fname, int mode)
{
    int e = _kernel_osfind( (mode == RO_OPEN_READ) ? 0x43: 0x83 , (char *)fname);
    if (e == _kernel_ERROR)
    {
	_kernel_oserror *ep = _kernel_last_oserror();
	if (ep)
	    usrtrc("fopen: %s - %x %s\n", fname, ep->errnum, ep->errmess);
	else
	    usrtrc("fopen: %s - error\n", fname);
	e = 0;
    }
    return e;
}

void ro_fclose(int fh)
{
    if (fh == 0)
	return;

    _kernel_osfind(0, (char*) fh);
}

int ro_fwrite(void *ptr, int size, int items, int fh)
{
    _kernel_osgbpb_block gpb;

    gpb.dataptr = ptr;
    gpb.nbytes = size * items;

    _kernel_osgbpb(2, fh, &gpb);

    return (size * items) - gpb.nbytes;
}

int ro_fwritepos(void *ptr, int size, int items, int fh, int pos)
{
    _kernel_osgbpb_block gpb;

    gpb.dataptr = ptr;
    gpb.nbytes = size * items;
    gpb.fileptr = pos;

    _kernel_osgbpb(1, fh, &gpb);

    return (size * items) - gpb.nbytes;
}

int ro_fread(void *ptr, int size, int items, int fh)
{
    _kernel_osgbpb_block gpb;

    gpb.dataptr = ptr;
    gpb.nbytes = size * items;

    _kernel_osgbpb(4, fh, &gpb);

    return (size * items) - gpb.nbytes;
}

int ro_freadpos(void *ptr, int size, int items, int fh, int pos)
{
    _kernel_osgbpb_block gpb;

    gpb.dataptr = ptr;
    gpb.nbytes = size * items;
    gpb.fileptr = pos;

    _kernel_osgbpb(3, fh, &gpb);

    return (size * items) - gpb.nbytes;
}

int ro_ensure_size(int fh, int size)
{
    return _kernel_osargs(3, fh, size);
}

int ro_get_extent(int fh)
{
    return _kernel_osargs(2, fh, 0);
}

char *ro_ferror(void)
{
    _kernel_oserror *e = _kernel_last_oserror();
    return e ? e->errmess : "";
}

/* eof files.c */
