/* -*-c-*- */

#include <stddef.h>

#include "kernel.h"

#include "files.h"
#include "version.h"

int ro_fopen(const char *fname, int mode)
{
    int e = _kernel_osfind( (mode == RO_OPEN_READ) ? 0x4f: 0x8f , (char *)fname);
    if (e < 0)			/* tell osfind to retiurn errors so that _kernel_lastoserror() has something to pick up on */
	e = 0;
    return e;
}

void ro_fclose(int fh)
{
    if (fh == 0)
	return;

    _kernel_osfind(0, (char*) fh);
}

#ifndef FRESCO
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

    if (_kernel_osgbpb(4, fh, &gpb) == _kernel_ERROR)
	return -1;

    return (size * items) - gpb.nbytes;
}
#endif

int ro_freadpos(void *ptr, int size, int items, int fh, int pos)
{
    _kernel_osgbpb_block gpb;

    gpb.dataptr = ptr;
    gpb.nbytes = size * items;
    gpb.fileptr = pos;

    if (_kernel_osgbpb(3, fh, &gpb) == _kernel_ERROR)
	return -1;

    return (size * items) - gpb.nbytes;
}

#ifndef FRESCO
int ro_ensure_size(int fh, int size)
{
    return _kernel_osargs(3, fh, size);
}
#endif

int ro_get_extent(int fh)
{
    return _kernel_osargs(2, fh, 0);
}

#if !defined(FRESCO) || DEBUG
char *ro_ferror(void)
{
    _kernel_oserror *e = _kernel_last_oserror();
#if DEBUG
    if (e)
	usrtrc("ro_ferror: %x %s\n", e->errnum, e->errmess);
#endif
    return e ? e->errmess : "";
}
#endif

/* eof files.c */
