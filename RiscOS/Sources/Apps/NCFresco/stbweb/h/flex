/****************************************************************************
 * This source file was written by Acorn Computers Limited. It is part of   *
 * the RISCOS library for writing applications in C for RISC OS. It may be  *
 * used freely in the creation of programs for Archimedes. It should be     *
 * used with Acorn's C Compiler Release 3 or later.                         *
 *                                                                          *
 ***************************************************************************/

/*
 * Title  : flex.h
 * Purpose: provide memory allocation for interactive programs requiring
 *          large chunks of store. Such programs must respond to memory
 *          full errors.
 *
 */

#ifndef __flex_h
#define __flex_h

typedef void **flex_ptr;

#define flex_alloc(a,n) flexda_alloc(a,n)
#define flex_free(a) flexda_free(a)
#define flex_size(a) flexda_size(a)
#define flex_extend(a,n) flexda_extend(a,n)
#define flex_midextend(a,b,n) flexda_midextend(a,b,n)
#define flex_budge(a,n) flexda_budge(a,n)
#define flex_dont_budge(a,n) flexda_dont_budge(a,n)
#define flex_init(a) flexda_init(a)

extern int flexda_alloc(flex_ptr anchor, int n);
extern void flexda_free(flex_ptr anchor);
extern int flexda_size(flex_ptr anchor);
extern int flexda_extend(flex_ptr anchor, int newsize);
extern int flexda_midextend(flex_ptr anchor, int at, int by);
extern int flexda_budge(int n, void **a);
extern int flexda_dont_budge(int n, void **a);
extern void flexda_init(const char *programname);

#endif

/* end flex.h */
