/* rlib_alloc.c */

/* Lets you change RiscOSLib's memory allocator
 * (C) ANT Limited 1996
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */

#ifndef rlib_alloc_h
#define rlib_alloc_h

#ifndef __stdlib_h
#include <stdlib.h>
#endif

#if 0
#define rlib_alloc malloc
#define rlib_calloc calloc
#define rlib_free   free
#define rlib_realloc realloc
#else
extern void *rlib_alloc( size_t );
extern void *rlib_calloc( size_t, size_t );
extern void rlib_free( void* );
extern void *rlib_realloc( void*, size_t );
#endif

#endif
