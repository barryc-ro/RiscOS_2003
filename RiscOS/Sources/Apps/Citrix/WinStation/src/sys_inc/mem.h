/* > mem.h

 *

 */

#ifdef MemCheck_MEMCHECK

# include "MemCheck/MemCheck.h"

# define MEMCHECK_PUSH() MemCheck_checking checking = MemCheck_SetChecking(0,0)
# define MEMCHECK_POP() MemCheck_RestoreChecking(checking);

# ifdef getc
#  undef getc
# endif

# ifdef putc
#  undef putc
# endif

#else

# define MEMCHECK_PUSH()
# define MEMCHECK_POP()

#endif

/* use MemHeap */

extern HGLOBAL GlobalAlloc(int flags, int size);
extern HGLOBAL GlobalFree( HGLOBAL ptr );

#define GlobalLock(ptr)			((void *)ptr)
#define GlobalUnlock(ptr)		

/* use MemFlex */

#ifndef ExtrasLib_MemFlex_h
typedef void *flex_ptr;
#endif

extern int FlexAlloc( flex_ptr anchor, int size);
extern void FlexFree( flex_ptr anchor );
extern int FlexMidExtend( flex_ptr anchor, int offset, int size);

extern void MemInit(void);

/* eof mem.h */
