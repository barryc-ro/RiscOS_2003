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
/* memlib is on include path so don't need the explicit path */
#include "memflex.h"
#endif
