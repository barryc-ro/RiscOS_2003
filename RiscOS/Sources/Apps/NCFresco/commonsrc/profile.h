/* profile.h	Profiling support for debugging.
 *
 * Note that 64 bit quantiies are done using the long long int
 * facility that GCC (and maybe some other) C compilers offer. You
 * won't be able to do 64 bit counts this way with Norcroft.
 */

#ifndef __profile_h
#define __profile_h

#ifndef __stdioh_h
#include <stdio.h>
#endif

extern void profile_init(void);
extern void profile_report(FILE *);

#ifndef PROFILE
#define PROFILE 0
#endif

#if PROFILE

/*****************************************************************************/

typedef unsigned int prof_32t;
typedef unsigned long long int prof_64t;

typedef struct { prof_32t count; char *name; } prof_rect;
typedef struct { prof_64t count; char *name; } prof_rec64t;

#define profile_INC(var)		prof_global[(var)].count++
#define profile_ADD(var, bias)		prof_global[(var)].count += (bias)
#define profile_INC64(var)		prof_global64[(var)].count++
#define profile_ADD64(var, bias)	prof_global64[(var)].count += (bias)

/*****************************************************************************/

#else

#define profile_INC(var)
#define profile_ADD(var, bias)
#define profile_INC(var)
#define profile_ADD64(var, bias)

/*****************************************************************************/

#endif /* PROFILE */

#ifndef __profdat_h
#include "profdat.h"
#endif


#endif /* __profile_h */

/* eof */
