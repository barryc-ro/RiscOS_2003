/* SubFlex.c */

/* A flex area within a flex block
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"

#include "os.h"
#include "wimp.h"
#include "werr.h"

#include "memflex.h"
#include "subflex.h"

#ifndef er
#define er(__x) { os_error *__e = __x; if (__e) return __e; }
#endif

static os_error *SubFlex__Insert( char *at, int by, int *relocatesize,
                                  flex_ptr master );

typedef struct { int* anchor; int size; } subflex__str;

typedef struct
{
    int free;
} subflex_header;

os_error *SubFlex_Initialise( flex_ptr master )
{
    er( MemFlex_Alloc( master, 256 ) );
    (*((subflex_header**)master))->free = sizeof( subflex_header );
    return NULL;
}

/*----------------------------------------------------------*
 * Allocate a flex block, returning an error if appropriate *
 *----------------------------------------------------------*/

os_error *SubFlex_Alloc( subflex_ptr anchor, int size, flex_ptr master )
{
    char *pArea = *(char**) master;
    subflex__str *block = (subflex__str*)(pArea + *(int*)anchor);
    int offset, sizeextra;
    int *relocatesize;
    subflex_header *pHeader = (subflex_header*)pArea;

    if ( size == 0 )
        return MemFlex_Free(anchor);

    size = ( size + 3 ) & ~3;

    if ( block )
    {
        block--;
        offset = block->size + sizeof(subflex__str);
        sizeextra = size - block->size;

        if ( sizeextra==0 )
            return NULL;    /* right size already */

        relocatesize = ( ((char *)block + offset) < (pArea+pHeader->free) )
                           ? &block->size
                           : NULL;
    }
    else
    {
        offset = 0;
        sizeextra = size + sizeof(subflex__str);
        block = (subflex__str *)(pArea + pHeader->free);
        relocatesize = NULL;
    }

    er( SubFlex__Insert( (char *)block + offset, sizeextra, relocatesize,
                         master ) );

    block->size = size;
    block->anchor = anchor;
    *(int*)anchor = ((char*)(block+1)) - (char*)pArea;

    return NULL;
}


/*-----------------------------------------------------------*
 * Midextend a flex block, returning an error if appropriate *
 *-----------------------------------------------------------*/

os_error *SubFlex_MidExtend( subflex_ptr anchor, int at, int by,
                             flex_ptr master )
{
    char *pArea = *(char**) master;
    subflex__str *block;
    int oldsize;

    if ( by==0 )
        return NULL;

    if ( *((int*)anchor) == 0 )
        return SubFlex_Alloc( anchor, by, master );

    block = (subflex__str*)(pArea + *(int*)anchor);

    if ( by < 0
         && SubFlex_Size( anchor, master ) + by <= 0 )
        return SubFlex_Free( anchor, master );

    /* word-aligned case is easier */
    if ( ( ( at | by ) & 3 ) == 0 )
        return SubFlex__Insert( (char *)block + at, by, &block[-1].size,
                                master );

    oldsize = block[-1].size;

    if ( by < 0 )
        memmove( (char *)block + at + by, (char *)block + at, oldsize - at );

    er( SubFlex__Insert( (char *)block + oldsize,
                         ((oldsize+by+3) & ~3)-oldsize, &block[-1].size,
                         master ) );

    if ( by > 0 )
        memmove( (char *)block + at + by, (char *)block + at, oldsize - at );

    return NULL;
}


/*---------------------------------------------*
 * Free a flex block and write the anchor to 0 *
 *---------------------------------------------*/

os_error *SubFlex_Free( subflex_ptr anchor, flex_ptr master )
{
    char *pArea = *(char**) master;
    subflex__str *block;
    char *at;
    int by;

    block = (subflex__str*)(pArea + *(int*)anchor);
    if ( block )
    {
        at = (char *)block + block[-1].size;
        by = sizeof(subflex__str) + block[-1].size;
        er( SubFlex__Insert( at, -by, &by, master ) );
        *(int*)anchor = 0;
    }

    return NULL;
}


/*-----------------------------*
 * Return size of a flex block *
 *-----------------------------*/

int SubFlex_Size( subflex_ptr anchor, flex_ptr master )
{
    char *pArea = *(char**) master;
    int offset = *(int*)anchor;
    subflex__str *block;

    if ( !offset )
        return 0;

    block = (subflex__str*)(pArea + offset);

    return block[-1].size;
}


os_error *SubFlex_Shrink( flex_ptr master )
{
    subflex_header *pHeader = *(subflex_header**)master;
    int slot = (pHeader->free + 255) & ~255;

    if ( slot <= MemFlex_Size( master ) - 512 )
        return MemFlex_Alloc( master, slot );

    return NULL;
}


/*------------------------------------------------------------------*
 * Insert/delete bytes at a given point in the flex structure       *
 * The insertion point and the amount to move must be word-aligned. *
 *------------------------------------------------------------------*/

static os_error *SubFlex__Insert( char *at, int by, int *relocatesize,
                                  flex_ptr master )
{
    char *pArea = *(char**)master;
    char *ptr, *nextptr, *end;
    subflex__str *block;
    subflex_header *pHeader = (subflex_header*)pArea;
    int newsize;

    if ( by > 0 )
    {
        newsize = pHeader->free + by;
        if ( newsize > MemFlex_Size( master ) )
        {
            int newarena = (newsize+511) & ~255;
            er( MemFlex_Alloc( master, newarena ) );
        }
    }

    memmove( at+by, at, (pArea+pHeader->free) - at );
    pHeader->free += by;

    end = pArea + pHeader->free;

    if ( relocatesize )
    {
        *relocatesize += by;
        for ( ptr = pArea+sizeof(subflex_header);
              ptr < end;
              ptr = nextptr )
        {
            block = (subflex__str *)ptr;
            nextptr = ptr + sizeof(subflex__str) + block->size;

            if ( ptr >= at+by )
                *(block->anchor) = ptr + sizeof(subflex__str) - pArea;
        }
    }

    return SubFlex_Shrink( master );
}
