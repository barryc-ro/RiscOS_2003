/************************************************************************/
/* © Acorn Computers Ltd, 1992.                                         */
/*                                                                      */
/* This file forms part of an unsupported source release of RISC_OSLib. */
/*                                                                      */
/* It may be freely used to create executable images for saleable       */
/* products but cannot be sold in source form or as an object library   */
/* without the prior written consent of Acorn Computers Ltd.            */
/*                                                                      */
/* If this file is re-distributed (even if modified) it should retain   */
/* this copyright notice.                                               */
/*                                                                      */
/************************************************************************/

/*
 * Title:    heap.c
 * Purpose:  provide malloc-style heap allocation in a flex block
 * History:  IDJ: 06-Feb-92: prepared for source release
 *			 SJM: 10/93:	removed checks for flex budging as we've removed the
 *                     		option from flex. Also if flex uses dynamic area
 *							then heap defaults to using malloc/free.
 *           SJM 12/93: took out defaulting to alloc/free
 */

#if MEMLIB

#include <limits.h>

#include "dynamic.h"
#include "swis.h"

#include "debug.h"
#include "interface.h"
#include "util.h"
#include "unwind.h"

#include "heap.h"

DynamicArea heap__da;
static int da_size;

void *heap__base;
int heap__size;

static int heap__max = 0;
static int heap__largest = 0;

#undef heap_init
#undef heap_alloc
#undef heap_free
#undef heap_realloc

#define HEAP_BLOCK_OVERHEAD	8
#define HEAP_OVERHEAD		16

static void cleanup(void)
{
#if DEBUG
    heap__dump(stderr);
#endif

    DynamicArea_Free(&heap__da);
}

void heapda_init(const char *programname)
{
    da_size = heap__size = MemoryPageSize();
    
    /* create the dynamic area */
    frontend_fatal_error(DynamicArea_Alloc(da_size, programname, &heap__da, &heap__base));

    /* initialise heap as being in the dynamic area we just created */
    frontend_fatal_error((os_error *)_swix(OS_Heap, _INR(0,1) | _IN(3), 0, heap__base, heap__size));
    
    atexit(cleanup);

    STBDBGN(("heapda_init: da size %d heap base %p size %d\n", da_size, heap__base, heap__size));
}

void *heapda_realloc(void *oldptr, unsigned int size_request)
{
    os_error *e;
    int size;
    void *newptr;
    int old_heap_size = heap__size, old_da_size = da_size;

#if DEBUG
    if (oldptr)
	STBDBGN(("heapda_realloc: %p to %d (heap %d da %d) (%s/%s/%s/%s)\n", oldptr, size_request, heap__size, da_size, caller(1), caller(2), caller(3), caller(4)));
#endif
    /* if shrink to zero then just remove it */
    if (size_request == 0)
    {
	heapda_free(oldptr);
	return NULL;
    }

    /* work out delta change request  */
    if (oldptr != NULL)
    {
	int new_size;

	/* read total allocated size of block */
	frontend_fatal_error((os_error *)_swix(OS_Heap, _INR(0,2) | _OUT(3), 6, heap__base, oldptr, &new_size));

	/* calculate change including overheads */
	size = size_request - (new_size - HEAP_BLOCK_OVERHEAD);
    }
    else
	size = size_request;

    /* try and allocate or reallocate block in current heap */
    while ((e = (os_error *)_swix(OS_Heap, _INR(0,3) | _OUT(2),
		      oldptr == NULL ? 2 : 4, heap__base, oldptr, size,
		      &newptr)) != NULL)
    {
	int adjust;
	
	/* see if failure to allocate block was not due to lack of memory */
	if (e->errnum != 0x0184 /* HeapFail_Alloc */)
	    frontend_fatal_error(e);

	/* adjust dynamic area by a page */
	adjust = MemoryPageSize();
	DynamicArea_Realloc(heap__da, &adjust);
	da_size += adjust;

/* 	STBDBGN(("heapda_realloc: da %d heap %d\n", da_size, heap__size)); */

	/* if we can't extend DA then return NULL */
	if (adjust == 0)
	{
	    /* reset to what we started with */
	    frontend_fatal_error((os_error *)_swix(OS_Heap, _INR(0,1)|_IN(3), 5, heap__base, old_heap_size - heap__size));
	    heap__size = old_heap_size;

	    adjust = old_da_size - da_size;
	    DynamicArea_Realloc(heap__da, &adjust);
	    da_size -= adjust;
	    
	    STBDBGN(("heapda_realloc: failed (heap %d da %d)\n", heap__size, da_size));

	    return NULL;
	}

	/* adjust heap to match da */
	frontend_fatal_error((os_error *)_swix(OS_Heap, _INR(0,1)|_IN(3), 5, heap__base, da_size - heap__size));
	heap__size = da_size;

	/* and then try again */
    }

#ifdef MemCheck_MEMCHECK
    if (e == NULL)
    {
	if (oldptr)
	    MemCheck_UnRegisterMiscBlock(oldptr);

	MemCheck_RegisterMiscBlock(newptr, size_request);
    }
#endif

    STBDBGN(("heapda_realloc: returns %p (heap %d da %d)\n", newptr, heap__size, da_size));

#if DEBUG
    if (heap__max < heap__size)
	heap__max = heap__size;

    if (heap__largest < size_request)
	heap__largest = size_request;
#endif
    
    /* return new ptr or null */
    return e ? NULL : newptr;
}

void *heapda_alloc(unsigned int size)
{
    STBDBGN(("heapda_alloc: %d (heap %d da %d) (%s/%s/%s/%s)\n", size, heap__size, da_size, caller(1), caller(2), caller(3), caller(4)));
    return heapda_realloc(NULL, size);
}

void heapda_free(void *heapptr)
{
    int moved;
    _kernel_swi_regs r;
    
    STBDBGN(("heapda_free: free %p (%d) (heap %d da %d) (%s/%s)\n", heapptr, ((int *)heapptr)[-1], heap__size, da_size, caller(1), caller(2)));

    if (heapptr == NULL)
	return;

    /* release block */
    frontend_fatal_error((os_error *)_swix(OS_Heap, _INR(0,2), 3, heap__base, heapptr));

    /* extend heap (with neg. increment to shrink) - don't want to know about errors */
    /* can't be _swix as we need r3 despite the fact that it will return an error */
    r.r[0] = 5;
    r.r[1] = (int)heap__base;
    r.r[3] = INT_MAX;
    _kernel_swi(OS_Heap, &r, &r);
    moved = r.r[3];

    STBDBGN(("heapda_free: shrunk by %d\n", moved));

    /* shrink da */
    if (moved > 0)
    {
	int adjust;
	
	/* adjust real size of heap */
	heap__size -= moved;

	/* try and free some pages */
	adjust = heap__size - da_size;

	STBDBGN(("heapda_free: heap__size %d change da by %d\n", heap__size, adjust));

	DynamicArea_Realloc(heap__da, &adjust);
	da_size -= adjust;

	STBDBGN(("heapda_free: da_size %d (adjust %d)\n", da_size, adjust));
    }

    STBDBGN(("heapda_free: (heap %d da %d)\n", heap__size, da_size));

#ifdef MemCheck_MEMCHECK
    MemCheck_UnRegisterMiscBlock(heapptr);
#endif
}

#else

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "werr.h"
#include "heap.h"
#include "os.h"
#include "msgs.h"
#include "kernel.h"
#include "da.h"
#include "util.h"
#include "debug.h"

#include "memwatch.h"

static void *heap__base;
static int da_number, da_size, heap_size;

#define heap__BLOCKSIZE 4096
#define OS_Heap 0x1D

#undef heap_init
#undef heap_alloc
#undef heap_free
#undef heap_realloc

/*
 * This is very naughty to know this, but it is impossible to resize a block
 * if you don't.
 */

#define HEAP_BLOCK_OVERHEAD	8

static void cleanup(void)
{
#if DEBUG
    heap__dump(stderr);
#endif
    da_free(&da_number, &da_size, &heap__base);
}

void heapda_init(const char *programname)
{
    os_regset r;
    os_error *e;
    char buffer[32];

    strncpysafe(buffer, programname, sizeof(buffer));
    strlencat(buffer, " heap", sizeof(buffer));

    /* create DA to be used as heap */
    if (!da_create(buffer, &da_number, &da_size, &heap__base) ||
	!da_adjust(da_number, &da_size, heap__BLOCKSIZE))
    {
	werr(TRUE, msgs_lookup("heap1:Not enough memory to create heap"));
    }
    
    /* initialise heap as being in the flex block we just created */
    r.r[0] = 0;
    r.r[1] = (int) heap__base;
    r.r[3] = da_size;
    if ((e = os_swix(OS_Heap, &r)) != 0)
	werr (TRUE, e->errmess);

    heap_size = da_size;
    
    atexit(cleanup);
}

void *heapda_realloc(void *oldptr, unsigned int orig_size)
{
    os_regset r;
    os_error *e;
    int size;

    if (orig_size == 0)
    {
	heapda_free(oldptr);
	return NULL;
    }

    r.r[1] = (int) heap__base;
    r.r[2] = (int) oldptr;

    if (oldptr != NULL)
    {
	r.r[0] = 6;
	if ((e = os_swix(OS_Heap, &r)) != 0)
	    werr(TRUE, msgs_lookup("heap2:Heap_alloc error: %s"), e->errmess);
	size = orig_size - (r.r[3] - HEAP_BLOCK_OVERHEAD);
    }
    else
	size = orig_size;

    /* see if we can satisfy request in heap as it is */
    r.r[0] = oldptr == NULL ? 2 : 4;   /* claim block from heap manager */
    r.r[3] = (int) size;

    while ((e = os_swix(OS_Heap, &r)) != 0)
    {
	os_error *ext;

	/* see if failure to allocate block */
	if (e->errnum != 0x0184 /* HeapFail_Alloc */)
	    werr(1, msgs_lookup("heap3:Fatal internal heap error"));

	if (!da_adjust(da_number, &da_size, da_size + heap__BLOCKSIZE))
	    return NULL;

	STBDBGN(("heap: alloc block heap extend by %d to %d\n", da_size - heap_size, da_size));

	r.r[0] = 5;
	r.r[1] = (int)heap__base;
	r.r[3] = (int)da_size - heap_size;
	if ((ext = os_swix(OS_Heap, &r)) != 0)
	    werr(TRUE, msgs_lookup("heap2:Heap_alloc error: %s"), ext->errmess);

	heap_size = da_size;

	r.r[0] = oldptr == NULL ? 2 : 4;         /* claim block again */
	r.r[1] = (int) heap__base;
	r.r[2] = (int) oldptr;
	r.r[3] = (int) size;
	/*e = os_swix(OS_Heap, &r);*/
    }

#ifdef MemCheck_MEMCHECK
    if (e == NULL)
    {
	if (oldptr)
	    MemCheck_UnRegisterMiscBlock(oldptr);

	MemCheck_RegisterMiscBlock((void *)r.r[2], orig_size);
    }
#endif
    
    if (e == 0)
	return (void *)r.r[2];
    else
	return (void *)0;
}

void *heapda_alloc(unsigned int size)
{
    return heapda_realloc(NULL, size);
}

void heapda_free(void *heapptr)
{
    os_regset r;
    os_error *e;

    if (heapptr == NULL)
	return;

    r.r[0] = 3;   /* release block */
    r.r[1] = (int) heap__base;
    r.r[2] = (int) heapptr;
    if((e = os_swix(OS_Heap, &r)) != 0)
	werr(FALSE, msgs_lookup("heap4:Heap_free error: %s"), e->errmess);

    r.r[0] = 5;   /* extend heap (with neg. increment to shrink) */
    r.r[1] = (int)heap__base;
    r.r[3] = -INT_MAX;
    os_swix(OS_Heap, &r);

    if (r.r[3] > 0)
    {
	heap_size -= r.r[3];
	da_adjust(da_number, &da_size, heap_size);
    }

    MemCheck_UnRegisterMiscBlock(heapptr);
    
    STBDBGN(("heap: free block heap shrunk by %d to %d da %d\n", r.r[3], heap_size, da_size));
}

#endif


#if DEBUG

/*
 * Heap description

Allocated blocks

struct
{
    int size;
    char bytes[size];
}

free blocks

struct
{
    int link;
    int size;
    char bytes[size];
}

block sizes include the descriptor words

 */

typedef struct
{
    int HeapWord;       /* = Heap, 0x70616548 */
    int free_offset;
    int base_offset;
    int end_offset;
} heap_descr_t;


static int scan_usedlist(char *base, int from, int to, FILE *f)
{
    int count = 0;

    do
    {
	int *used_blk = (int *)(base + from);

	FDBG((f, "used 0x%p %8d - %8d (%8d)\n",
	      base + (from + 4),
	      from + 4, from + *used_blk, *used_blk - 4));

	from += *used_blk;
	count++;
    }
    while (from < to);

    return count;
}

static void scan_list(heap_descr_t *hp, FILE *f)
{
    int free_count = 0,
    	used_count = 0;

    int current = 16;
    int next_free = hp->free_offset == 0 ? hp->base_offset : hp->free_offset + 4;

    char *hp_base = (char *)hp;

    do
    {
	if (current < next_free)
	    used_count += scan_usedlist(hp_base, current, next_free, f);

	if (next_free == hp->base_offset)
	{
	    if (hp->end_offset != hp->base_offset)
		FDBG((f, "free %8d - %8d (%8d)\n",
			hp->base_offset, hp->end_offset, hp->end_offset - hp->base_offset));

	    next_free = 0;
	}
	else
	{
	    int *free_blk = (int *)(hp_base + next_free);

	    FDBG((f, "free %8d - %8d (%8d)\n",
		    next_free, next_free + free_blk[1], free_blk[1]));

	    current = next_free + free_blk[1];

	    if (free_blk[0])
		next_free += free_blk[0];
	    else
		next_free = hp->base_offset;
	}

	free_count++;
    }
    while (next_free);

    FDBG((f, "\nused %d blocks\nfree %d blocks\n", used_count, free_count));
}

void heap__dump(FILE *f)
{
    heap_descr_t *hp = heap__base;
#ifdef MemCheck_MEMCHECK
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);
#endif
    FDBG((f, "\nHeap word   %x\n"
	    "Free offset %x (%d)\n"
	    "Base offset %x (%d)\n"
	    "End  offset %x (%d)\n\n",
	    hp->HeapWord,
	    hp->free_offset, hp->free_offset,
	    hp->base_offset, hp->base_offset,
	    hp->end_offset, hp->end_offset));


    FDBG((f, "Largest object %dK heap %dK\n", heap__largest/1024, heap__max/1024));

    scan_list(hp, f);

#ifdef MemCheck_MEMCHECK
    MemCheck_RestoreChecking(checking);
#endif
}

void heap__info(FILE *f)
{
    heap_descr_t *hp = heap__base;

    FDBG((f, "\nHeap word   %x\n"
	    "Free offset %x (%d)\n"
	    "Base offset %x (%d)\n"
	    "End  offset %x (%d)\n\n",
	    hp->HeapWord,
	    hp->free_offset, hp->free_offset,
	    hp->base_offset, hp->base_offset,
	    hp->end_offset, hp->end_offset));
}

#else

void heap__dump(FILE *f)
{
    FDBG((f, "Heap debugging not compiled in\n"));
}

void heap__info(FILE *f)
{
    FDBG((f, "Heap debugging not compiled in\n"));
}

#endif

/* eof heap.c */
