/* -*-C-*- */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "version.h"


static int emit = 0;

extern void init_usrtrc(void)
{
    if (getenv(PROGRAM_NAME"$Trace") != NULL)
	emit = 1;
}

#pragma -v1
extern void usrtrc(const char *fmt, ...)
#pragma -v0
{
    if (emit)
    {
	va_list arglist;
	va_start(arglist, fmt);
	vfprintf(stderr, fmt, arglist);
	fflush(stderr);
	va_end(arglist);
    }
}

/* eof usrtrc.c */
