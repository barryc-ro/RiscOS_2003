/* -*-c-*- */

/* memwatch.h */

#ifndef __stdlib_h
#include <stdlib.h>
#endif

#ifndef __version_h
#include "version.h"
#endif

#ifndef __stdio_h
#include <stdio.h>
#endif

#if defined(mm_malloc)
#error confused
#endif

#ifndef MEMWATCH
#	ifdef PRODUCTION
#	define MEMWATCH 0
#	else
#	define MEMWATCH 2
#	endif
#endif

extern void *mm_malloc(size_t x);
extern void *mm_calloc(size_t x, size_t y);
extern void *mm_realloc(void *x, size_t y);
extern int mm_can_we_recover(int abort);
extern void mm_free(void *x);

void mm_check(void);
void mm_summary(void);
void mm_dump(void);

void mm__check(FILE *f);
void mm__summary(FILE *f);
void mm__dump(FILE *f);

#if !defined(__MemCheck_MemCheck_h)
# include "memcheck/MemCheck.h"
#endif

/* #ifndef MemCheck_MEMCHECK */
/* typedef int MemCheck_checking; */
/* #endif */

/* eof memwatch.h */
