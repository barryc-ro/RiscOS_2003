#ifndef __myassert_h
#define __myassert_h

#ifndef __debug_h
#include "debug.h"
#endif

#ifdef PLOTCHECK
# ifdef __ASSERT
#  undef __ASSERT
# endif
# define __ASSERT		plotcheck_assert
#endif

#if DEBUG && !defined(STBWEB)
 #define TASSERT(expr)          ((expr)?(void)0:__ASSERT(0,#expr,__FILE__,__LINE__))
#else
 #define TASSERT(__ignore)              ((void)0)
#endif


#if defined(NDEBUG) || defined(STBWEB)
 #define ASSERT(__ignore)               ((void)0)
 #define ASSERT2(__ignore1, __ignore2)    ((void)0)
#else
 extern void __ASSERT( int, const char *, const char *, int );
 #define ASSERT(expr)           ((expr)?(void)0:__ASSERT(0,#expr,__FILE__,__LINE__))
 #define ASSERT2(expr, msg)     ((expr)?(void)0:__ASSERT(0, msg, __FILE__,__LINE__))
#endif

#endif /* __myassert_h */
