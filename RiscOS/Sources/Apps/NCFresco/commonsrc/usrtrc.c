/* -*-C-*- */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "version.h"


static int emit = 0;

extern void init_usrtrc(void)
{
    if (getenv(PROGRAM_NAME"$Trace") != NULL)
	emit = 1;

    if (emit)
    {
	time_t t;
	time(&t);
	usrtrc("\n\n"PROGRAM_NAME" started at %s\n\n", ctime(&t));
    }
}

#ifdef __acorn
#pragma -v1
#endif

extern void usrtrc(const char *fmt, ...)
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

#ifdef __acorn
#pragma -v0
#endif

/* eof usrtrc.c */
