/* -*-c-*- */

/* memwatch.c */

/* 27/03/96: SJM: Change wimp_reporterror to frontend calls
 * 27/09/96: DAF: usrtrc, rather than automatically to stderr for production.
 * 02/10/96: DAF: Made MEMZERO use a more destructive value
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"

#include "wimp.h"
#include "msgs.h"

#include "verstring.h"
#include "makeerror.h"

#include "images.h"

#include "unwind.h"
#include "interface.h"
#include "version.h"

#if MEMLIB
#include "../memlib/memheap.h"

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
    strcpy(panicerr.errmess, msgs_lookup("memfatal"));
    panicerr.errnum = 0;

    frontend_fatal_error(&panicerr);
    exit(1);
    return 0;
}

extern int antweb_doc_abort_all(void);

/* Return 1 if more memory was made by freeing images
 * Return 2 if mroe memory was made by aborting documents
 * Return 0 and give error if no memory can be freed.
 */

extern int mm_can_we_recover(int abort)
{
    int r = 0;

    if (image_memory_panic())
	r = 1;			/* recovered through discarding images */

#ifdef STBWEB
    if (r == 0 && antweb_doc_abort_all())
	r = 2;			/* recovered through abort transfers */
#endif

    if (r == 0)			/* if can't recover any memory */
    {
#ifdef STBWEB
	if (abort)
#endif
	    mm_no_more_memory();

 	return 0;
    }
				/* if recovered some memory then give message */
    strcpy(panicerr.errmess, msgs_lookup("memlow"));
    panicerr.errnum = 0;

/*    wimp_reporterror(&panicerr, (wimp_errflags) (1<<4), program_name); */
    frontend_complain(&panicerr);

    return r;
}

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
    fprintf(stderr, "mm_link: item at %p\n", m);
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
    fprintf(stderr, "mm_unlink: item at %p\n", m);
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

#if MEMWATCH >= 3
    if (x >= MEMWATCH_MIN_INTEREST)
	fprintf(stderr, "mm_malloc: item size 0x%x '%s' '%s'", x, caller(1), caller(2));
#endif

    newptr = malloc(x + sizeof(mm_chain) + sizeof(mm_tail));

#if MEMWATCH >= 3
    if (x >= MEMWATCH_MIN_INTEREST)
	fprintf(stderr, " = %p\n", newptr ? newptr - 1 : NULL);
#endif

    if (newptr == NULL)
    {
	mm_can_we_recover(TRUE);
	newptr = malloc(x + sizeof(mm_chain) + sizeof(mm_tail));

	if (newptr == NULL)
	    mm_no_more_memory();
    }

    newptr->size = x;
    newptr->magic = MM_MAGIC_HEAD;
#if MEMWATCH >= 4 && 0
    fprintf(stderr, "about to call caller()\n");
#endif
    newptr->caller = caller(1);
    newptr->caller2 = caller(2);

#if MEMWATCH >= 4 && 0
    fprintf(stderr, "caller() called\n");
#endif
    tail = MM_TAIL(newptr);
    tail->magic = MM_MAGIC_TAIL;

    mm_link(newptr);

    return (void*)(newptr+1);
}

void *mm_calloc(size_t x, size_t y)
{
    int size;
    mm_chain *new;
    mm_tail *tail;

    num_calloc++;

    size = x * y;

#if MEMWATCH >= 3
    if (size >= MEMWATCH_MIN_INTEREST)
	fprintf(stderr, "mm_calloc: item size 0x%x '%s' '%s'", size, caller(1), caller(2));
#endif
    new = calloc(1, size + sizeof(mm_chain) + sizeof(mm_tail));

#if MEMWATCH >= 3
    if (size >= MEMWATCH_MIN_INTEREST)
	fprintf(stderr, " = %p\n", new ? new - 1 : NULL);
#endif

    if (new == NULL)
    {
	mm_can_we_recover(TRUE);
	new = calloc(1, size + sizeof(mm_chain) + sizeof(mm_tail));
	if (new == NULL)
	    mm_no_more_memory();
    }

    new->size = size;
    new->magic = MM_MAGIC_HEAD;

#if MEMWATCH >= 4 && 0
    fprintf(stderr, "about to call caller()\n");
#endif
    new->caller = caller(1);
    new->caller2 = caller(2);

#if MEMWATCH >= 4 && 0
    fprintf(stderr, "caller() called\n");
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

    /* realloc(NULL, size) is equivalent to malloc(size) */

    if (x == NULL)
	return mm_malloc(y);

#if MEMWATCH >= 3
    if (y >= MEMWATCH_MIN_INTEREST)
	fprintf(stderr, "mm_realloc: item at %p, old size 0x%x, new size 0x%x, %s %s\n", x, m->size, y, caller(1), caller(2));
#endif

    tail = MM_TAIL(m);

    assert(m->magic == MM_MAGIC_HEAD);
    assert(tail->magic == MM_MAGIC_TAIL);

    mm_unlink(m);

    tail = MM_TAIL(m);
    tail->magic = 0;

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
	return;
    }

    num_free++;

    m = ((mm_chain *) x)-1;

#if MEMWATCH >= 3
    fprintf(stderr, "mm_free: item at %p\n", x);
#endif
    tail = MM_TAIL(m);

    if ((m->magic != MM_MAGIC_HEAD) || (tail->magic != MM_MAGIC_TAIL))
    {
	int i;
	char *fnp;

	fprintf(stderr, "Problem freeing block at 0x%p, hmagic=0x%08x, tmagic=0x%08x\n",
		m, m->magic, tail->magic);

	i = 1;
	do
	{
	    fnp = caller(i);
	    if (fnp)
		fprintf(stderr, "mm_free caller(%d)='%s'\n", i, fnp);
	    i++;
	} while (fnp && strcmp(fnp, "main") != 0);
	exit(1);
    }

    if ( (m->magic == MM_MAGIC_HEAD_FREED)
         || (tail->magic == MM_MAGIC_TAIL_FREED) )
    {
	fprintf(stderr, "Double-freed! first by %s from %s\n",
	                m->caller, m->caller2 );
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
	    fprintf(f, "### Bad magic number on head of item at %p\n", m);
	    err = 1;
	}

	t = MM_TAIL(m);

	if (t->magic != MM_MAGIC_TAIL)
	{
	    fprintf(f, "### Bad magic number on tail of item at %p\n", m);
	    err = 1;
	}

	m_last = m;
    }

    if (m_last != mm_last)
    {
	fprintf(f, "### Memory chain ends do not match up (%p != %p)\n", m_last, mm_last);
	err = 1;
    }

    if (err == 0)
    {
	fprintf(f, "Memory checks OK\n");
    }
    else
	exit(1);
}

void mm__summary(FILE *f)
{
    fprintf(f, "Heap function usage counts:\n"
	    "mm_malloc : %d\n"
	    "mm_calloc : %d\n"
	    "mm_realloc: %d\n"
	    "mm_free   : %d\n"
	    "mm_free0  : %d\n",
	    num_malloc, num_calloc, num_realloc, num_free, num_free0);
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
	    fprintf(f, "Bad magic number on head of item at %p\n", m);
	    err = 1;
	}

	t = MM_TAIL(m);

	if (t->magic != MM_MAGIC_TAIL)
	{
	    fprintf(f, "Bad magic number on tail of item at %p\n", m);
	    err = 1;
	}

	fprintf(f, "Block at 0x%p, size=%6d, caller='%s', caller2='%s'\n",
		m, m->size, m->caller ? m->caller : none, m->caller2 ? m->caller2 : none);

	total_size += m->size;
	real_total_size += ((m->size + 3) &~ 3) + sizeof(mm_chain) + sizeof(mm_tail) + 8; /* 8 is for the heap itself (guard and size/flags)  */

	m_last = m;
    }

    fprintf(f, "\nTotal size requested %6d\n", total_size);
    fprintf(f, "Total size used      %6d\n\n", real_total_size);

    if (m_last != mm_last)
    {
	fprintf(f, "Memory chain ends do not match up (%p != %p)\n", m_last, mm_last);
	err = 1;
    }

    if (err == 0)
    {
	fprintf(f, "Memory checks OK\n");
    }

    mm__summary(f);
}

#else
/* SJM: changed these back to plain alloc names as they will be #define's anyawy if MEMLIb is defined  */
extern void *mm_malloc(size_t x)
{
    void *p;
    p=malloc(x);
    if (p)
	return p;

    if (x == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    p=malloc(x);
    if (p)
	return p;

    mm_no_more_memory();

    return NULL;
}

extern void *mm_calloc(size_t x, size_t y)
{
    void *p;
    p=calloc(x, y);
    if (p)
	return p;

    if (x*y == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    p=calloc(x, y);
    if (p)
	return p;

    mm_no_more_memory();

    return NULL;
}

extern void *mm_realloc(void *x, size_t y)
{
    void *p;
    p=realloc(x, y);
    if (p)
	return p;

    if (y == 0)
	return NULL;

    mm_can_we_recover(TRUE);

    p=realloc(x, y);
    if (p)
	return p;

    mm_no_more_memory();

    return NULL;
}

extern void mm_free(void *x)
{
    if ( x )
        free(x);
}

extern void mm__dump(FILE *f)
{
    usrtrc( "Memwatch compiled at too low a level for mm_dump to do anything.\n");
}

extern void mm__summary(FILE *f)
{
    usrtrc( "Memwatch compiled at too low a level for mm_summary to do anything.\n");
}

extern void mm__check(FILE *f)
{
    usrtrc( "Memwatch compiled at too low a level for mm_ckeck to do anything.\n");
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

/* eof memwatch.c */
