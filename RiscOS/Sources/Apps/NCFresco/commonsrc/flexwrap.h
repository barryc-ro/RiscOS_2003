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
#include "../memlib/memflex.h"
#endif
