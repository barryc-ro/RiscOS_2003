/* rlib_alloc.c */

/* Lets you change RiscOSLib's memory allocator
 * (C) ANT Limited 1996
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */

#include <stdlib.h>
#include "memwatch.h"

#include "rlib_alloc.h"


    /*=================================*
     *   1. Allocators for RiscOSLib   *
     *=================================*/


#if 0
/* MEMWATCH >= 2 */

/* Defeat tail-call optimisation, so these appear in caller(n) logs! */
static int foo;

void *rlib_alloc( size_t s )
{
    void *answer = mm_malloc(s);
    foo = 3;
    return answer;
}

void *rlib_calloc( size_t a, size_t b )
{
    void *answer = mm_calloc( a, b );
    foo = 2;
    return answer;
}

void rlib_free( void* p )
{
    mm_free( p );
    foo = 91;
}

void *rlib_realloc( void* p, size_t s )
{
    void *answer = mm_realloc( p, s );
    foo = 37;
    return answer;
}

#else
void *rlib_alloc( size_t s )           { return mm_malloc(s); }
void *rlib_calloc( size_t a, size_t b ) { return mm_calloc( a, b ); }
void rlib_free( void* p )               { mm_free( p ); }
void *rlib_realloc( void* p, size_t s ) { return mm_realloc( p, s ); }
#endif


    /*================================*
     *   2. Allocators for webimage   *
     *================================*/


void *heap_alloc( size_t s )        { return malloc(s); }
void heap_free( void *p )           { free(p); }


    /*=======================================*
     *   3. Allocators for webimage's DLLs   *
     *=======================================*/


/* These ALWAYS use the wimp slot */
#undef malloc
#undef free
void *code_alloc( size_t s )        { return malloc(s); }
void  code_free( void *p )          { free(p); }
