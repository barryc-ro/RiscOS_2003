/* MemFlex.c */

/* Like RiscOSLib:flex.c, but can use dynamic areas
 * (K) All Rites Reversed - Copy What You Like
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"

#include "swis.h"
#include "os.h"			/* for os_error */

#include "memflex.h"
#include "memheap.h"
#include "dynamic.h"

#ifndef er
#define er(__x) { os_error *__e = __x; if (__e) return __e; }
#endif

#ifdef DEBUG
extern char *caller(int);
#endif

#ifdef __acorn
#pragma no_check_stack
#endif

static os_error *MemFlex__Insert( char *at, int by, int *relocatesize );

/* #define xos_swi(__x,__r) os_swix ( XOS_Bit | (__x), __r ) */

#ifdef MEM_DEBUG
typedef struct { char ** anchor; int size; char *caller1; char *caller2; } flex__str;
#else
typedef struct { char ** anchor; int size; } flex__str;
#endif

#define ApplicationBase ((char *)0x8000)

char   *flexptr__base = NULL;
char   *flexptr__free;
char   *flexptr__slot;
int     flex__page;
BOOL    flex__inited = FALSE;
BOOL    FlexShrinkable = TRUE;

DynamicArea flex__da = dynamicarea_NONE;

BOOL MemFlex_Dynamic( void )
{
    return flex__da != dynamicarea_NONE;
}

static os_error *setslot(int *current, int *free)
{
    return (os_error *)_swix(Wimp_SlotSize,
			     _INR(0,1) | _OUT(0) | _OUT(2),
			     current ? *current : -1, -1,
			     current, free);
}

/*------------------------*
 * Initialise flex system *
 *------------------------*/

void MemFlex__atexit( void )
{
    if ( MemFlex_Dynamic() )
    {
        DynamicArea_Free( &flex__da );
    }
}

os_error *MemFlex_Initialise2( const char *areaname )
{
     int currentslot = -1;
/*   int nextslot = -1; */
/*   int freepool; */

    if ( !flex__inited )
    {
        flex__page = MemoryPageSize();

        if ( areaname != NULL &&
             !DynamicArea_Alloc( flex__page, areaname, &flex__da,
                                 (void**) &flexptr__base ) )
        {
            flexptr__slot = flexptr__base + flex__page;
            atexit( MemFlex__atexit );
        }
        else
        {
/*          er( wimp_slotsize( &currentslot, &nextslot, &freepool ) ); */
	    er( setslot(&currentslot, NULL) );
            flexptr__base =
                flexptr__slot = ApplicationBase + currentslot;
        }
        flexptr__free = flexptr__base;

        flex__inited = TRUE;
    }

    return NULL;
}


#define HEAP_MAGICWORD 0x70616548       /* "Heap" */

BOOL MemFlex_InFlexArea( void *block )
{
    if ( (char *)block >= flexptr__base && (char *)block < flexptr__slot )
    {
        if ( *(unsigned int *)(flexptr__base + sizeof(flex__str))
                 != HEAP_MAGICWORD
             || (char *)block
                 >= (flexptr__base + sizeof(flex__str)
                         + ((flex__str*)flexptr__base)->size) )
        return TRUE;
    }

    return FALSE;
}



int MemFlex_TotalFree( void )
{
/*  int currentslot = -1; */
/*  int nextslot = -1; */
    int freepool;

/*  wimp_slotsize( &currentslot, &nextslot, &freepool ); */
    setslot(NULL, &freepool);

    return (flexptr__slot - flexptr__free) + freepool - sizeof(flex__str);
}


/*-------------------------------------------------------*
 * Check that the given anchor references a kosher block *
 *-------------------------------------------------------*/

#ifdef MEM_DEBUG

void MemFlex__Err( char *message )
{
    os_error e;
    
    e.errnum = 0;
    sprintf(e.errmess, "Flex error: %s (%s/%s)", message, caller(2), caller(3));

    _swix(Wimp_ReportError, _INR(0,5), &e, 1 | (1<<8), "MemLib", NULL, NULL, 0);

    *((int *)-1) = 0;
}

void MemFlex__Check( flex_ptr anchor )
{
    flex__str *block;
    char *ptr, *old;
    BOOL found = FALSE;

    if ( flexptr__base == NULL )
        MemFlex__Err( "Flex not initialised" );

    if ( MemFlex_InFlexArea( anchor ) )
        MemFlex__Err( "Anchor in flex area" );

    block = anchor ? *(flex__str **)anchor : NULL;

    for ( ptr = flexptr__base, old = NULL;
          ptr < flexptr__free;
            old = ptr, ptr += sizeof(flex__str) + ((flex__str *)ptr)->size )
    {
        if ( block && (flex__str *)ptr == block-1 )
        {
            found = TRUE;
            if ( ((flex__str *)ptr)->anchor != anchor )
                MemFlex__Err( "Anchor pointer wrong" );
        }
        if ( *(((flex__str *)ptr)->anchor) != (void *)(((flex__str *)ptr)+1) )
            MemFlex__Err( "Anchor wrong" );
    }

    if ( block && !found )
        MemFlex__Err( "Not a flex block" );
}

void MemFlex_CheckNoAnchors( char *start, int size )
{
    char *end = start + size;
    char *anchor;
    char *ptr, *old;

    for ( ptr = flexptr__base, old = NULL;
          ptr < flexptr__free;
          old = ptr, ptr += sizeof(flex__str) + ((flex__str *)ptr)->size )
    {
        anchor = (char *)(((flex__str *)ptr)->anchor);
        if ( anchor >= start && anchor < end )
            MemFlex__Err( "Anchor freed" );
    }
}

void MemFlex_Dump(void *f)
{
    char *ptr;

    if (!f)
	f = stderr;
    
    if ( flexptr__base == NULL )
    {
        fprintf(f, "Flex not initialised\n" );
	return;
    }

    fprintf(f, "Flex: base %p\n", flexptr__base);
    fprintf(f, "Flex: free %p\n", flexptr__free);
    fprintf(f, "Flex: slot %p\n", flexptr__slot);
    
    for ( ptr = flexptr__base;
          ptr < flexptr__free;
            ptr += sizeof(flex__str) + ((flex__str *)ptr)->size )
    {
	flex__str *fptr = (flex__str *)ptr;

	fprintf(f, "block %p anchor %p size %6dK callers '%s' '%s'\n", fptr, fptr->anchor, fptr->size/1024, fptr->caller1, fptr->caller2);

	if (*(fptr->anchor) != (char *)(fptr + 1))
	    fprintf(f, "     %p ERROR\n", *(fptr->anchor));
    }
}

#endif


/*----------------------------------------------------------*
 * Allocate a flex block, returning an error if appropriate *
 *----------------------------------------------------------*/

os_error *MemFlex_Alloc( flex_ptr anchor, int size )
{
    flex__str * block = *(flex__str **)anchor;
    int offset, sizeextra;
    int *relocatesize;

    MemFlex__Check( anchor );

    if ( size == 0 )
        return MemFlex_Free(anchor);

    size = ( size + 3 ) & ~3;

    if ( block )
    {
        offset = (--block)->size + sizeof(flex__str);
        sizeextra = size - block->size;

        if ( sizeextra==0 )
            return NULL;    /* right size already */

        relocatesize = ( ((char *)block + offset) < flexptr__free )
                           ? &block->size
                           : NULL;
    }
    else
    {
        offset = 0;
        sizeextra = size + sizeof(flex__str);
        block = (flex__str *)flexptr__free;
        relocatesize = NULL;
    }

    er( MemFlex__Insert( (char *)block + offset, sizeextra, relocatesize ) );

    block->size = size;
    block->anchor = anchor;
#ifdef MEM_DEBUG
    if (offset == 0)
    {
    	block->caller1 = caller(1);
	block->caller2 = caller(2);
    }
#endif

    *(flex__str **)anchor = block+1;

    return NULL;
}


/*-----------------------------------------------------------*
 * Midextend a flex block, returning an error if appropriate *
 *-----------------------------------------------------------*/

os_error *MemFlex_MidExtend( flex_ptr anchor, int at, int by )
{
    flex__str *block;
    int oldsize;

    MemFlex__Check( anchor );

    if ( by==0 )
        return NULL;

    block = *(flex__str **)anchor;
    if ( block == NULL )
        return MemFlex_Alloc( anchor, by );

    if ( by < 0
         && MemFlex_Size(anchor) + by <= 0 )
        return MemFlex_Free( anchor );

    /* word-aligned case is easier */
    if ( ( ( at | by ) & 3 ) == 0 )
        return MemFlex__Insert( (char *)block + at, by, &block[-1].size );

    oldsize = block[-1].size;

    if ( by < 0 )
        memmove( (char *)block + at + by, (char *)block + at, oldsize - at );

    er( MemFlex__Insert( (char *)block + oldsize,
                         ((oldsize+by+3) & ~3)-oldsize, &block[-1].size ) );

    if ( by > 0 )
        memmove( (char *)block + at + by, (char *)block + at, oldsize - at );

    return NULL;
}


/*---------------------------------------------*
 * Free a flex block and write the anchor to 0 *
 *---------------------------------------------*/

os_error *MemFlex_Free( flex_ptr anchor )
{
    flex__str *block;
    char *at;
    int by;

    MemFlex__Check( anchor );

    block = *(flex__str **)anchor;
    if ( block )
    {
        at = (char *)block + block[-1].size;
        by = sizeof(flex__str) + block[-1].size;
        er( MemFlex__Insert( at, -by, &by ) );
        *(void **)anchor = NULL;
    }

    return NULL;
}


/*-----------------------------*
 * Return size of a flex block *
 *-----------------------------*/

int MemFlex_Size( flex_ptr anchor )
{
    flex__str *ptr;

    ptr = *(flex__str **)anchor;

    return ( ptr ? ptr[-1].size : 0 );
}


/*--------------------------------------------------------------------------*
 * Reanchor a flex block, ie. transfer ownership to another anchor pointer. *
 *--------------------------------------------------------------------------*/

os_error *MemFlex_MoveAnchor( flex_ptr dst, flex_ptr src )
{
    flex__str *ptr;

    MemFlex__Check( src );

    if ( src != dst )
    {
        if ( *(flex__str **)dst )
            er( MemFlex_Free( dst ) );

        if ( *(flex__str **)src )
        {
            ptr = *(flex__str **)src;
            ptr[-1].anchor = (char **)dst;
            *(flex__str **)dst = ptr;
            *(flex__str **)src = NULL;
        }
    }

    return NULL;
}


/*
 - Set slot end pointer to the given value.
 - Works in the dynamic area and wimpslot cases.
 */

os_error *MemFlex__SetSlot( char *newslot )
{
    int change; /* , nextslot, freepool */
    os_error *e;

    if ( MemFlex_Dynamic() )
    {
        BOOL minus;

        change = newslot - flexptr__slot;
        minus = ( change < 0 );
        e = DynamicArea_Realloc( flex__da, &change );
        flexptr__slot += minus ? -change : change;
    }
    else
    {
        change = newslot - ApplicationBase;
/*      nextslot = -1; */
/*      e = wimp_slotsize( &change, &nextslot, &freepool ); */
	e = setslot(&change, NULL);
        if (!e)
            flexptr__slot = ApplicationBase + change;
    }

    return e;
}


/*------------------------------------------------------------------*
 * Decide whether the flex area is shrinkable                       *
 * Used to set the flex area to the maximum required (for printing) *
 *------------------------------------------------------------------*/

#define ROUNDPOINTERUPTOPAGE(__p) (char *) ROUNDUPTOPAGE ( (int) (__p) )
#define ROUNDUPTOPAGE(__p) ( ( (__p) + flex__page-1 ) & ~(flex__page-1) )

os_error *MemFlex_Shrinkable( BOOL shrinkable )
{
    char *newslot = ROUNDPOINTERUPTOPAGE ( flexptr__free );

    if ( shrinkable && newslot <= flexptr__slot - 8*1024 )
    {
        er( MemFlex__SetSlot( newslot ) );
    }

    FlexShrinkable = shrinkable;

    return NULL;
}


/*------------------------------------------------------------------*
 * Insert/delete bytes at a given point in the flex structure       *
 * The insertion point and the amount to move must be word-aligned. *
 *------------------------------------------------------------------*/

static os_error *MemFlex__Insert( char *at, int by, int *relocatesize )
{
    char *newtop, *ptr, *nextptr;
    flex__str *block;

    if ( by > 0 )
    {
        newtop =flexptr__free + by;
        if ( newtop > flexptr__slot )
        {
            char *oldslot = flexptr__slot;
            char *newslot = ROUNDPOINTERUPTOPAGE ( newtop );

            er( MemFlex__SetSlot( newslot ) );
            if ( newtop > flexptr__slot )
            {
                er( MemFlex__SetSlot( oldslot ) );
                return (os_error*)1;    /* Out of memory */
            }
        }
    }

    memmove( at+by, at, flexptr__free-at );
    flexptr__free += by;

#ifdef MEM_DEBUG
    if ( by > 0 )
        memset( at, 0xCC, by );
#endif

    if ( relocatesize )
    {
        *relocatesize += by;
        for ( ptr = flexptr__base; ptr < flexptr__free; ptr = nextptr )
        {
            block = (flex__str *)ptr;
            nextptr = ptr + sizeof(flex__str) + block->size;
#ifdef MEM_DEBUG
            if ( *(block->anchor) !=
                 ptr + sizeof(flex__str) - ( ptr >= at+by ? by : 0 ) )
                MemFlex__Err( "Bogus anchor" );
#endif
            if ( ptr >= at+by )
                *(block->anchor) = ptr + sizeof(flex__str);
        }
        MemFlex__Check( NULL );
    }

    return MemFlex_Shrinkable( FlexShrinkable );
}

#ifdef __acorn
#pragma no_check_stack
#endif

int MemFlex_budge( int n, void **a )
{
    if ( MemFlex_Dynamic() )
    {
#ifdef MEM_DEBUG
	char buf[256];
        char *c1 = caller(1),
             *c2 = c1 ? caller(2) : 0,
             *c3 = c2 ? caller(3) : 0;

        sprintf(buf, "Error: MemFlex_budge called (for %d) in DA case."
                 "Callers %s,%s,%s", n, c1, c2, c3 );
	MemFlex__Err(buf);
#endif
        return 0;
    }

    *a = MemHeap_malloc( n );

    return (*a) ? n : 0;
}

int MemFlex_dont_budge( int n, void **a )
{
    return 0;
}
