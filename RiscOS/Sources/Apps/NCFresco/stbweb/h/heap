/****************************************************************************
 * This source file was written by Acorn Computers Limited. It is part of   *
 * the RISCOS library for writing applications in C for RISC OS. It may be  *
 * used freely in the creation of programs for Archimedes. It should be     *
 * used with Acorn's C Compiler Release 3 or later.                         *
 *                                                                          *
 ***************************************************************************/
/*
 * Title:   heap.h
 * Purpose: provide malloc-style heap allocation in a dynamic area
 *
 */

# ifndef __heap_h
# define __heap_h

#define heap_init(h) heapda_init(h)
#define heap_alloc(s) heapda_size(s)
#define heap_free(h) heapda_free(h)
#define heap_realloc(h, s) heapda_realloc(h, s)

extern void heapda_init(const char *programname);
extern void *heapda_alloc(unsigned int size);
extern void heapda_free(void *heapptr);
extern void *heapda_realloc(void *oldptr, unsigned int size);
extern void heap__dump(FILE *f);

#endif

/* end of heap.h */
