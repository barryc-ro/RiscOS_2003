/* flexwrap.h */

/* Make it easy to switch flex implementations */

#ifndef MEMLIB
#error "Use of flexwrap.h when undecided heap strategy"
#endif

#if !MEMLIB
/* The original and not best */
/* but keeps the standalone code going */
#include "flex.h"
#else

/* Lots of foo because of MemCheck */

#ifdef MemCheck_MEMCHECK
#define MemFlex_Alloc(a,n)  MemFlex_MemCheck_Alloc(a,n)
#define MemFlex_Free(a)     MemFlex_MemCheck_Free(a)
#define MemFlex_MidExtend(a,at,by)  MemFlex_MemCheck_MidExtend(a,at,by)

#define SubFlex_Free(a,p)       SubFlex_MemCheck_Free(a,p)
#define SubFlex_Alloc(a,n,o)    SubFlex_MemCheck_Alloc(a,n,o)
#define SubFlex_Initialise(a)   SubFlex_MemCheck_Initialise(a)
#endif

/* memlib is on include path so don't need the explicit path */
#include "memflex.h"
#include "subflex.h"
#endif
