/*
 * Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * syst.h - Threads package OSD component.
 *
 * DESCRIPTION:
 * This layer implements a platform's thread package.  In the simplest
 * case, the calls here are merely OSD shadows to the Portable Oracle
 * Threads Interface or "POTI" (pronouced pot-eee) defined in yst.h.  yst.c
 * implements a portable library level threads package of this interface.
 * Alternatively, if a platform provides a native threads package, this
 * layer can call through to it.  The POTI is deliberately kept simple to
 * use so that mapping a native thread interface onto it is easy - just a
 * little glue code should do.  Of course, a home grown package could be
 * implemented here as well (though that would be of questionable value).
 *
 * All OMS code that uses threads must use the POTI in generic code.  The
 * actual thread calls used by the generic code are the "s" variants
 * presented here.  Generic code must never call yst.h directly.  This
 * mechanism effectively permits use of any of the underlying threads
 * implementations mentioned above without changing generic code.
 *
 * Note that any platform may have muliple implementations of syst.h if
 * different applications on the platform must use different threads
 * packages.  For example, a Motif based Solaris application might link in
 * a version of syst.h that calls through to the Solaris kernel threads
 * package.  A different non-Motif Solaris application might link in a
 * version of syst.h that calls through to yst.h.
 */

#ifndef SYST_ORACLE
#define SYST_ORACLE

#ifndef SYSI_ORACLE
# include <sysi.h>
#endif

/*
 * Define SYST_LIBRARY to use the portable library level threads package
 * implemented in yst.c.  A non-library implementation must define these
 * functions in syst.c with the same semantics as the interface presented
 * in yst.h.  In the native OS threads case, these functions would just be
 * wrappers to the equivalent native thread package functions.
 */
#define SYST_LIBRARY

#ifdef SYST_LIBRARY

#define  systInit 	ystInit                 /* initialize thread package */
#define  systTerm	ystTerm                  /* terminate thread package */

#define  systThd        ystThd                      /* opaque thread context */
#define  systCreate 	ystCreate                         /* create a thread */
#define  systGetId 	ystGetId                     /* get unique thread id */
#define  systExit	ystExit             /* exit currently running thread */

#define  systYield	ystYield                                /* yield cpu */
#define  systSuspend	ystSuspend                         /* suspend thread */
#define  systResume	ystResume                           /* resume thread */

#else /* SYST_LIBRARY */

sword 	 systInit( ub4 totalStkMem );
sword    systTerm();

typedef  struct systThd systThd;
systThd	*systCreate( void (*main)(dvoid *arg), dvoid  *ctx, ub4 stksiz);
void   	 systGetId( systThd *thd, sysb8 *id );
sword  	 systExit( void );

void   	 systYield( systThd *nextThd );
void     systSuspend( systThd *nextThd );
void     systResume( systThd *thd );

#endif /* SYST_LIBRARY */

#ifdef SYST_LIBRARY

/*
 * Thread Stack Creation and Destruction.
 *
 * DESCRIPTION:
 * The portable thread library in yst.c calls two "real" OSDs, one to
 * allocate and prepare the thread stack and one to free it.  Unlike the
 * above functions, these two are not to be called from generic code, so
 * to avoid confusion we have separated them out.  yst.c is the only
 * legal client of these routines.
 *
 * At a minimum, systStkInit() must allocate the thread's stack and patch
 * the stack pointer area in the supplied jump buffer to point to it.  Some
 * implementations may also be able to set a stack limit in the jump buffer
 * so that stack overflow can be detected by the processor.  Other ports
 * may want to set up memory barriers to protect against stack overflow.
 * systStkInit() can also initialize the stack memory so that sensible
 * backtraces are produced from debuggers.  Finally, upon first invocation
 * of systStkInit, a port may optionally choose to set aside totalStkMem
 * bytes for stack memory.  If this value is zero, the port must do dynamic
 * allocation on every invocation, possibly at the expense of performance.
 * This value is handed down from systInit and is the same for every call.
 *
 * systStkInit() returns a handle to the allocated memory or zero on error.
 * This handle is opaque to the portable code and is passed back to
 * systStkFree() to allow freeing of the stack memory.
 */
dvoid  *systStkInit( sysejb   jbuf, ub4   stksiz, ub4 totalStkMem );
void    systStkFree( dvoid  *stkhdl );

#endif /* SYST_LIBRARY */

#endif /* SYST_ORACLE */
