/* -*-c-*- */

/* memwatch.c */

/* 27/03/96: SJM: Change wimp_reporterror to frontend calls
 * 27/09/96: DAF: usrtrc, rather than automatically to stderr for production.
 * 02/10/96: DAF: Made MEMZERO use a more destructive value
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "assert.h"

#include "wimp.h"
#include "msgs.h"

#include "verstring.h"
#include "makeerror.h"

#include "images.h"

#include "unwind.h"
#include "interface.h"
#include "version.h"
#include "flexwrap.h"
#include "gbf.h"
#include "profile.h"

#include "guarded.h"

#ifndef ITERATIVE_PANIC
#define ITERATIVE_PANIC 0
#endif

#if MEMLIB

#ifdef STBWEB_BUILD
#include "memheap.h"
#include "mallinfo.h"
#else
#include "../memlib/memheap.h"
#include "../memlib/mallinfo.h"
#endif

/* ISTR that ANSI sez you shouldn't do this... */

#ifdef malloc
#undef malloc
#endif /* malloc */

#ifdef calloc
#undef calloc
#endif /* calloc */

#ifdef realloc
#undef realloc
#endif /* realloc */

#ifdef free
#undef free
#endif /* free */

#define malloc MemHeap_malloc
#define free MemHeap_free
#define calloc MemHeap_calloc
#define realloc MemHeap_realloc
#endif /* MEMLIB */

#if MEMLIB && defined(MemCheck_MEMCHECK)
#define mc_add_block(p,x) MemCheck_RegisterMiscBlock(p,x)
#define mc_resize_block(p,x) MemCheck_ResizeMiscBlock(p,x)
#define mc_del_block(p) MemCheck_UnRegisterMiscBlock(p)
#define CHECKVAR MemCheck_checking checking
#define CHECKOFF checking = MemCheck_SetChecking(0,0)
#define CHECKON  MemCheck_RestoreChecking(checking)
#else
#define mc_add_block(p,x)
#define mc_resize_block(p,x)
#define mc_del_block(p)
#define CHECKVAR
#define CHECKOFF
#define CHECKON
#endif

#include "memwatch.h"
#include "debug.h"

/* Zero memory on freeing */
#ifndef MEMZERO
/* IF debugging, we'll have the zeroing! */
#define MEMZERO DEBUG
#endif

#if MEMWATCH > 0
static int num_malloc = 0,
    num_calloc = 0,
    num_realloc = 0,
    num_free = 0,
    num_free0 = 0;
#endif

#define MEMWATCH_MIN_INTEREST	0
#define MEMZERO_VALUE		0x69

char *none="<none>";

static os_error panicerr;

static int mm_no_more_memory(void)
{
#if CATCHTYPE5
    /* pdh: if we're really going to fall over, raise a signal so that the
     * current document can be aborted. Leaks memory, but can only be better
     * for desktop users than exit(1) is!
     */
    if ( sig_guarded() )
        raise( SIGUSR1 );
#endif

    strcpy(panicerr.errmess, msgs_lookup("memfatal"));
    panicerr.errnum = 0;

    frontend_fatal_error(&panicerr);
    exit(1);
    return 0;
}

#if CATCHTYPE5

#ifdef __acorn
#pragma no_check_stack
#endif

extern void mm_minor_panic( char *token )
{
    /* pdh: just an error reporting thing really, but takes advantage of
     * memwatch's nice static os_error so we don't have to allocate
     * anything.
     */
    strcpy( panicerr.errmess, msgs_lookup(token) );
    panicerr.errnum = 0;
    frontend_complain( &panicerr );
}

#ifdef __acorn
#pragma check_stack
#endif

#endif

extern int antweb_doc_abort_all(int level);

/* Return 1 if more memory was made by freeing images
 * Return 2 if more memory was made by aborting documents
 * Return 0 and give error if no memory can be freed.
 */

#if ITERATIVE_PANIC

#define EMERGENCY_MEMORY_STASH	(256*1024)
#define EMERGENCY_MEMORY_UNIT	(EMERGENCY_MEMORY_STASH/8)

static void *emergency_memory = NULL;

extern BOOL mm_poll(void)
{
    BOOL full = TRUE;
    /* bring stash back to full size */
#if !MEMLIB
    if (emergency_memory == NULL)
    {
	if (flex_alloc(&emergency_memory, EMERGENCY_MEMORY_STASH))
	{
	    DBG(("mm_poll: allocated stash %d\n", EMERGENCY_MEMORY_STASH));
	}
	else
	{
	    DBG(("mm_poll: failed to allocate stash\n"));
	}
    }
    else
#endif
    {
	int size = emergency_memory ? flex_size(&emergency_memory) : 0;
	if (size < EMERGENCY_MEMORY_STASH)
	{
	    if (flex_extend(&emergency_memory, size + EMERGENCY_MEMORY_UNIT))
	    {
		size += EMERGENCY_MEMORY_UNIT;

		DBG(("mm_poll: reallocate stash to %d\n", size));

		if (size >= EMERGENCY_MEMORY_STASH)
		    gbf_flags &= ~GBF_LOW_MEMORY;
		if (size > EMERGENCY_MEMORY_UNIT)
		    gbf_flags &= ~GBF_VERY_LOW_MEMORY;

		if (size < EMERGENCY_MEMORY_STASH)
		    full = FALSE;
	    }
	    else
	    {
		DBG(("mm_poll: failed to reallocate stash to %d\n", size + EMERGENCY_MEMORY_UNIT));

		full = FALSE;
	    }
	}
    }
    return full;
}

extern int mm_can_we_recover(int abort)
{
    int r = 0;

    /* Note this is set here but only cleared in memwatch when the
       stash is made complete. End of downloading in frontend should
       also check for stash being full and whether it should then
       clear the flag */
    gbf_flags |= GBF_LOW_MEMORY;

    if (image_memory_panic())
	r = 1;			/* recovered through discarding images */

    if (r == 0 && frontend_memory_panic())
	r = 4;			/* recovered from frontend */

#if 0
    if (r == 0 && emergency_memory)
    {
	int size = flex_size(&emergency_memory);
	if (size > EMERGENCY_MEMORY_STASH/2)
	{
	    if (flex_extend(&emergency_memory, size - EMERGENCY_MEMORY_UNIT))
		r = 6;
	    else
	    {
		DBG(("mm_can_we_recover: emergency shrink failed!!!\n"));
	    }
	}
    }
#endif

    if (r == 0 && antweb_doc_abort_all(0))
	r = 5;			/* recovered through abort transfers */

    if (r == 0 && emergency_memory)
    {
	int size = flex_size(&emergency_memory);
	if (size)
	{
	    if (flex_extend(&emergency_memory, size - EMERGENCY_MEMORY_UNIT))
	    {
		r = 2;
		DBG(("mm_poll: use stash, down to %d\n", size - EMERGENCY_MEMORY_UNIT));
		if (size <= EMERGENCY_MEMORY_UNIT)
		    gbf_flags |= GBF_VERY_LOW_MEMORY;
	    }
	    else
	    {
		DBG(("mm_can_we_recover: emergency shrink failed!!!\n"));
	    }
	}
    }

    if (r == 0 && antweb_doc_abort_all(1))
	r = 3;			/* recovered through abort transfers */

    DBG(("mm_can_we_recover: r %d abort %d %s/%s/%s\n", r, abort, caller(1), caller(2), caller(3)));

    if (r == 0)			/* if can't recover any memory */
    {
	if (abort)
	    mm_no_more_memory();

 	return 0;
    }

				/* if recovered some memory then give message */
    strcpy(panicerr.errmess, msgs_lookup("memlow"));
    panicerr.errnum = 0;

/*    wimp_reporterror(&panicerr, (wimp_errflags) (1<<4), program_name); */
#ifndef STBWEB
    frontend_complain(&panicerr);
#endif
    return r;
}

#else /* ITERATIVE_PANIC */

extern BOOL mm_poll(void)
{
    return TRUE;
}

extern int mm_can_we_recover(int abort)
{
    int r = 0;

    if (image_memory_panic())
	r = 1;			/* recovered through discarding images */

    if (r == 0)			/* if can't recover any memory */
    {
	mm_no_more_memory();

 	return 0;
    }
				/* if recovered some memory then give message */
    strcpy(panicerr.errmess, msgs_lookup("memlow"));
    panicerr.errnum = 0;

    frontend_complain(&panicerr);

    return r;
}
#endif /* ITERATIVE_PANIC */

/* ====================================================================== */

#if MEMWATCH >= 2

typedef struct mm_chain {
    struct mm_chain *next, *prev;
    int size;
    int magic;
    char *caller;
    char *caller2;
} mm_chain;

typedef struct {
    int magic;
} mm_tail;

#define RND(x) (((x)+3)&(~3))

#define MM_TAIL(m) (mm_tail *) (((char *) ((m)+1))+ RND((m)->size) );

#define MM_MAGIC_HEAD	0x563482fb
#define MM_MAGIC_TAIL	0x324dc839

#define MM_MAGIC_HEAD_FREED 0x666f6f57
#define MM_MAGIC_TAIL_FREED 0x776f654d

static mm_chain *mm_first = NULL, *mm_last = NULL;

static void mm_link(mm_chain *m)
{
#if MEMWATCH >= 4
    DBG(("mm_link: item at %p\n", m));
#endif
    if (mm_first)
    {
	m->prev = mm_last;
	m->next = NULL;
	mm_last->next = m;
	mm_last = m;
    }
    else
    {
	mm_first = mm_last = m;
	m->prev = m->next = 0;
    }
}

static void mm_unlink(mm_chain *m)
{
#if MEMWATCH >= 4
    DBG(("mm_unlink: item at %p\n", m));
#endif
    if (m->prev)
	m->prev->next = m->next;
    else
	mm_first = m->next;

    if (m->next)
	m->next->prev = m->prev;
    else
	mm_last = m->prev;
}

void *mm_malloc(size_t x)
{
    mm_chain *newptr;
    mm_tail *tail;

    num_malloc++;

    PINC_MALLOC;
    PADD_HEAP_BYTES(x);

#if MEMWATCH >= 3
    if (x >= MEMWATCH_MIN_INTEREST)
	DBG(("mm_malloc: item size 0x%x '%s' '%s'", x, caller(1), caller(2)));
#endif

#if ITERATIVE_PANIC
    do
    {
	newptr = malloc(x + sizeof(mm_chain) + sizeof(mm_tail));

	if (newptr)
	    break;
#if MEMWATCH >= 3
	if (x >= MEMWATCH_MIN_INTEREST)
	    DBG((" = %p\n", newptr ? newptr - 1 : NULL));
#endif
    }
    while (mm_can_we_recover(TRUE));

    if (newptr == NULL)
	mm_no_more_memory();
#else /* ITERATIVE_PANIC */
    newptr = malloc(x + sizeof(mm_chain) + sizeof(mm_tail));

#if MEMWATCH >= 3
    if (x >= MEMWATCH_MIN_INTEREST)
	DBG((" = %p\n", newptr ? newptr - 1 : NULL));
#endif

    if (newptr == NULL)
    {
	mm_can_we_recover(TRUE);
	newptr = malloc(x + sizeof(mm_chain) + sizeof(mm_tail));

	if (newptr == NULL)
	    mm_no_more_memory();
    }
#endif /* ITERATIVE_PANIC */

    newptr->size = x;
    newptr->magic = MM_MAGIC_HEAD;
#if MEMWATCH >= 4 && 0
    DBG(("about to call caller()\n"));
#endif
    newptr->caller = caller(1);
    newptr->caller2 = caller(2);

#if MEMWATCH >= 4 && 0
    DBG(("caller() called\n"));
#endif
    tail = MM_TAIL(newptr);
    tail->magic = MM_MAGIC_TAIL;

    mm_link(newptr);

    return (void*)(newptr+1);
}

void *mm_calloc(size_t x, size_t y)
{
    const int size = x * y;
    mm_chain *new;
    mm_tail *tail;

    num_calloc++;

    PINC_CALLOC;
    PADD_HEAP_BYTES(size);
    PADD_HEAP_ZERO(size);

#if MEMWATCH >= 3
    if (size >= MEMWATCH_MIN_INTEREST)
	DBG(("mm_calloc: item size 0x%x '%s' '%s'", size, caller(1), caller(2)));
#endif

#if ITERATIVE_PANIC
    do
    {
	new = calloc(1, size + sizeof(mm_chain) + sizeof(mm_tail));

#if MEMWATCH >= 3
	if (size >= MEMWATCH_MIN_INTEREST)
	    DBG((" = %p\n", new ? new - 1 : NULL));
#endif
	if (new)
	    break;
    }
    while (mm_can_we_recover(TRUE));

    if (new == NULL)
	mm_no_more_memory();
#else /* ITERATIVE_PANIC */
    new = calloc(1, size + sizeof(mm_chain) + sizeof(mm_tail));

#if MEMWATCH >= 3
    if (size >= MEMWATCH_MIN_INTEREST)
	DBG((" = %p\n", new ? new - 1 : NULL));
#endif

    if (new == NULL)
    {
	mm_can_we_recover(TRUE);
	new = calloc(1, size + sizeof(mm_chain) + sizeof(mm_tail));
	if (new == NULL)
	    mm_no_more_memory();
    }
#endif /* ITERATIVE_PANIC */
    new->size = size;
    new->magic = MM_MAGIC_HEAD;

#if MEMWATCH >= 4 && 0
    DBG(("about to call caller()\n"));
#endif
    new->caller = caller(1);
    new->caller2 = caller(2);

#if MEMWATCH >= 4 && 0
    DBG(("caller() called\n"));
#endif
    tail = MM_TAIL(new);
    tail->magic = MM_MAGIC_TAIL;

    mm_link(new);

    return (void*)(new+1);
}

void *mm_realloc(void *x, size_t y)
{
    mm_chain *m = ((mm_chain *) x)-1;
    mm_tail *tail;

    num_realloc++;

    PINC_REALLOC;
    if (y < 0)
    {
	PADD_HEAP_SHRINK(-y);
    }
    else
    {
	PADD_HEAP_GROW(y);
    }

    /* realloc(NULL, size) is equivalent to malloc(size) */

    if (x == NULL)
	return mm_malloc(y);

#if MEMWATCH >= 3
    if (y >= MEMWATCH_MIN_INTEREST)
	DBG(("mm_realloc: item at %p, old size 0x%x, new size 0x%x, %s %s\n", x, m->size, y, caller(1), caller(2)));
#endif

    tail = MM_TAIL(m);

    assert(m->magic == MM_MAGIC_HEAD);
    assert(tail->magic == MM_MAGIC_TAIL);

    mm_unlink(m);

    tail = MM_TAIL(m);
    tail->magic = 0;

#if ITERATIVE_PANIC
    do
    {
	m = realloc(m, y + sizeof(mm_chain) + sizeof(mm_tail));

	if (m)
	    break;
    }
    while (mm_can_we_recover(TRUE));

    if (m == NULL)
	mm_no_more_memory();
#else /* ITERATIVE_PANIC */
/* Borris: put the &&0 in - 9/9/96 */
#if MEMZERO && 0
#error Realloc does not zero memory
#else
    m = realloc(m, y + sizeof(mm_chain) + sizeof(mm_tail));
#endif

    if (m == NULL)
    {
	mm_can_we_recover(TRUE);
	m = realloc(m, y + sizeof(mm_chain) + sizeof(mm_tail));
	if (m == NULL)
	    mm_no_more_memory();
    }
#endif /* ITERATIVE_PANIC */
    m->size = y;

    tail = MM_TAIL(m);
    tail->magic = MM_MAGIC_TAIL;

    mm_link(m);

    return (void*) (m+1);
}

void mm_free(void *x)
{
    mm_chain *m;
    mm_tail *tail;

    if (x == NULL)
    {
	num_free0++;
	PINC_FREE0;
	return;
    }

    num_free++;
    PINC_FREE;

    m = ((mm_chain *) x)-1;

#if MEMWATCH >= 3
    DBG(("mm_free: item at %p\n", x));
#endif
    tail = MM_TAIL(m);

    if ((m->magic != MM_MAGIC_HEAD) || (tail->magic != MM_MAGIC_TAIL))
    {
	int i;
#ifdef RISCOS
	char *fnp;
#endif

	DBG(("Problem freeing block at 0x%p, hmagic=0x%08x, tmagic=0x%08x\n",
		m, m->magic, tail->magic));

	i = 1;
	/* Unfortunately, this isn't so easy on many other platforms! */
#ifdef RISCOS
	do
	{
	    fnp = caller(i);
	    if (fnp)
		DBG(("mm_free caller(%d)='%s'\n", i, fnp));
	    i++;
	} while (fnp && strcmp(fnp, "main") != 0);
#endif
	exit(1);
    }

    if ( (m->magic == MM_MAGIC_HEAD_FREED)
         || (tail->magic == MM_MAGIC_TAIL_FREED) )
    {
	DBG(("Double-freed! first by %s from %s\n",
	                m->caller, m->caller2 ));
	exit(1);
	return;
    }

    assert(m->magic == MM_MAGIC_HEAD);
    assert(tail->magic == MM_MAGIC_TAIL);

    m->magic = 0;
    tail->magic = 0;

    mm_unlink(m);
#if MEMZERO
    memset(m, MEMZERO_VALUE, m->size + sizeof(mm_chain) + sizeof(mm_tail));
#endif

    m->magic = MM_MAGIC_HEAD_FREED;
    tail->magic = MM_MAGIC_TAIL_FREED;

    m->caller = caller(1);
    m->caller2 = caller(2);

    free(m);
}

void mm__check(FILE *f)
{
    mm_chain *m, *m_last = NULL;
    mm_tail *t;
    int err = 0;

    for(m = mm_first; m; m = m->next)
    {
	if (m->magic != MM_MAGIC_HEAD)
	{
	    FDBG((f, "### Bad magic number on head of item at %p\n", m));
	    err = 1;
	}

	t = MM_TAIL(m);

	if (t->magic != MM_MAGIC_TAIL)
	{
	    FDBG((f, "### Bad magic number on tail of item at %p\n", m));
	    err = 1;
	}

	m_last = m;
    }

    if (m_last != mm_last)
    {
	FDBG((f, "### Memory chain ends do not match up (%p != %p)\n", m_last, mm_last));
	err = 1;
    }

    if (err == 0)
    {
	FDBG((f, "Memory checks OK\n"));
    }
    else
	exit(1);
}

void mm__summary(FILE *f)
{
#if MEMLIB && defined(STBWEB)
    struct mallinfo info = MemHeap_mallinfo();

    FDBG((f, "total space:            %dK\n", info.arena/1024));
    FDBG((f, "total allocated space:  %dK\n", info.uordblks/1024));
    FDBG((f, "total non-inuse space:  %dK\n", info.fordblks/1024));
    FDBG((f, "total non-inuse chunks: %d\n", info.ordblks));
    FDBG((f, "top releasable space:   %dK\n", info.keepcost/1024));
    FDBG((f, "mmap regions:           %d\n", info.hblks));
    FDBG((f, "mmap space:             %dK\n", info.hblkhd/1024));
#endif
    FDBG((f, "Heap function usage counts:\n"
	    "mm_malloc : %d\n"
	    "mm_calloc : %d\n"
	    "mm_realloc: %d\n"
	    "mm_free   : %d\n"
	    "mm_free0  : %d\n",
	    num_malloc, num_calloc, num_realloc, num_free, num_free0));
}

void mm__dump(FILE *f)
{
    mm_chain *m, *m_last = NULL;
    mm_tail *t;
    int err = 0;
    int total_size = 0;
    int real_total_size = 0;

    for(m = mm_first; m; m = m->next)
    {
	if (m->magic != MM_MAGIC_HEAD)
	{
	    FDBG((f, "Bad magic number on head of item at %p\n", m));
	    err = 1;
	}

	t = MM_TAIL(m);

	if (t->magic != MM_MAGIC_TAIL)
	{
	    FDBG((f, "Bad magic number on tail of item at %p\n", m));
	    err = 1;
	}

        /* pdh: report pointer as known to caller */
	FDBG((f, "Block at 0x%p, size=%6d, caller='%s', caller2='%s'\n",
		m+1, m->size, m->caller ? m->caller : none, m->caller2 ? m->caller2 : none));

	total_size += m->size;
	real_total_size += ((m->size + 3) &~ 3) + sizeof(mm_chain) + sizeof(mm_tail) + 8; /* 8 is for the heap itself (guard and size/flags)  */

	m_last = m;
    }

    FDBG((f, "\nTotal size requested %6d\n", total_size));
    FDBG((f, "Total size used      %6d\n\n", real_total_size));

    if (m_last != mm_last)
    {
	FDBG((f, "Memory chain ends do not match up (%p != %p)\n", m_last, mm_last));
	err = 1;
    }

    if (err == 0)
    {
	FDBG((f, "Memory checks OK\n"));
    }

    mm__summary(f);
}

/* ====================================================================== */
#else
/* SJM: changed these back to plain alloc names as they will be #define's anyawy if MemLib is defined  */

extern void *mm_malloc(size_t x)
{
    void *p;
    CHECKVAR;

#if ITERATIVE_PANIC
    do
    {
	CHECKOFF;
	p=malloc(x);
	CHECKON;

	if (p)
	{
	    mc_add_block(p,x);
	    return p;
	}

	if (x == 0)
	    return NULL;
    }
    while (mm_can_we_recover(TRUE));
#else /* ITERATIVE_PANIC */
    CHECKOFF;
    p=malloc(x);
    CHECKON;

    if (p)
    {
        mc_add_block(p,x);
	return p;
    }

    if (x == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    CHECKOFF;
    p=malloc(x);
    CHECKON;

    if (p)
    {
        mc_add_block(p,x);
	return p;
    }
#endif /* ITERATIVE_PANIC */
    mm_no_more_memory();

    return NULL;
}

extern void *mm_calloc(size_t x, size_t y)
{
#if 1
    void *p = mm_malloc(x*y);
    if ( p )
        memset( p, 0, x*y );
    return p;
#else
    void *p;
    p=calloc(x, y);
    if (p)
    {
        mc_add_block(p,x*y)
	return p;
    }

    if (x*y == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    p=calloc(x, y);
    if (p)
    {
        mc_add
	return p;
    }

    mm_no_more_memory();

    return NULL;
#endif
}

extern void *mm_realloc(void *x, size_t y)
{
    void *p;
    CHECKVAR;

#if ITERATIVE_PANIC
    do
    {
	CHECKOFF;
	p=realloc(x, y);
	CHECKON;

	if (p)
	{
	    if ( x )
	    {
		mc_del_block(x);
	    }
	    mc_add_block(p,y);
	    return p;
	}

	if (y == 0)
	    return NULL;
    }
    while (mm_can_we_recover(TRUE));
#else /* ITERATIVE_PANIC */
    CHECKOFF;
    p=realloc(x, y);
    CHECKON;

    if (p)
    {
        if ( x )
	{
            mc_del_block(x);
	}
        mc_add_block(p,y);
 	return p;
    }

    if (y == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    CHECKOFF;
    p=realloc(x, y);
    CHECKON;

    if (p)
    {
        if ( x )
	{
            mc_del_block(x);
	}
        mc_add_block(p,y);
	return p;
    }
#endif /* ITERATIVE_PANIC */
    mm_no_more_memory();

    return NULL;
}

extern void mm_free(void *x)
{
    if ( x )
    {
        CHECKVAR;

        mc_del_block(x);

        CHECKOFF;
        free(x);
        CHECKON;
    }
}

extern void mm__dump(FILE *f)
{
    usrtrc( "Memwatch compiled at too low a level for mm_dump to do anything.\n");
}

extern void mm__summary(FILE *f)
{
#if MEMLIB && defined(STBWEB)
    struct mallinfo info = MemHeap_mallinfo();

    fprintf(f, "total space:            %dK\n", info.arena/1024);
    fprintf(f, "total allocated space:  %dK\n", info.uordblks/1024);
    fprintf(f, "total non-inuse space:  %dK\n", info.fordblks/1024);
    fprintf(f, "total non-inuse chunks: %d\n", info.ordblks);
    fprintf(f, "top releasable space:   %dK\n", info.keepcost/1024);
    fprintf(f, "mmap regions:           %d\n", info.hblks);
    fprintf(f, "mmap space:             %dK\n", info.hblkhd/1024);
#else
    usrtrc( "Memwatch compiled at too low a level for mm_summary to do anything.\n");
#endif
}

extern void mm__check(FILE *f)
{
    usrtrc( "Memwatch compiled at too low a level for mm_check to do anything.\n");
}

#endif

void mm_dump(void)
{
    mm__dump(stderr);
}

void mm_summary(void)
{
    mm__summary(stderr);
}

void mm_check(void)
{
    mm__check(stderr);
}

#if MEMLIB && defined( MemCheck_MEMCHECK )
/* pdh: Lots of filth here for wrapping flex and subflex accesses */

#undef MemFlex_Alloc
#undef MemFlex_Free
#undef MemFlex_MidExtend
#undef SubFlex_Initialise
#undef SubFlex_Alloc
#undef SubFlex_Free

/* Declare the real ones (the header declared the fake ones, confused yet?)
 */

extern os_error *MemFlex_Alloc( flex_ptr anchor, int size );
extern os_error *MemFlex_Free( flex_ptr anchor );
extern os_error *MemFlex_MidExtend( flex_ptr anchor, int at, int by );

extern os_error *SubFlex_Initialise( flex_ptr master );
extern os_error *SubFlex_Alloc( subflex_ptr anchor, int size, flex_ptr master );
extern os_error *SubFlex_Free( subflex_ptr anchor, flex_ptr master );

os_error *MemFlex_MemCheck_Alloc( flex_ptr a, int size )
{
    CHECKVAR;
    os_error *e;

    CHECKOFF;
    if ( *((void**)a) )
        MemCheck_UnRegisterFlexBlock(a);
    e = MemFlex_Alloc( a, size );
    if ( !e )
        MemCheck_RegisterFlexBlock( a, size );
    CHECKON;
    return e;
}

os_error *MemFlex_MemCheck_Free( flex_ptr a )
{
    CHECKVAR;
    os_error *e;

    CHECKOFF;
    MemCheck_UnRegisterFlexBlock( a );
    e = MemFlex_Free( a );
    CHECKON;
    return e;
}

os_error *MemFlex_MemCheck_MidExtend( flex_ptr a, int at, int by )
{
    CHECKVAR;
    os_error *e;
    int newsize;

    CHECKOFF;
    newsize = MemFlex_Size( a ) + by;
    e = MemFlex_MidExtend(a, at, by);
    if ( !e )
        MemCheck_ResizeFlexBlock( a, newsize );
    CHECKON;
    return e;
}

os_error *SubFlex_MemCheck_Initialise( flex_ptr master )
{
    CHECKVAR;
    os_error *e;

    CHECKOFF;
    e = SubFlex_Initialise( master );
    if ( !e )
        MemCheck_RegisterFlexBlock( master, MemFlex_Size(master) );
    CHECKON;
    return e;
}

os_error *SubFlex_MemCheck_Alloc( subflex_ptr anchor, int size, flex_ptr master )
{
    CHECKVAR;
    os_error *e;

    CHECKOFF;
    e = SubFlex_Alloc( anchor, size, master );
    MemCheck_ResizeFlexBlock( master, MemFlex_Size(master) );
    CHECKON;
    return e;
}

os_error *SubFlex_MemCheck_Free( subflex_ptr anchor, flex_ptr master )
{
    CHECKVAR;
    os_error *e;

    CHECKOFF;
    e = SubFlex_Free( anchor, master );
    MemCheck_ResizeFlexBlock( master, MemFlex_Size(master) );
    CHECKON;
    return e;
}

#endif

/* eof memwatch.c */
