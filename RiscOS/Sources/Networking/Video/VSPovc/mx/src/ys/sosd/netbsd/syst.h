/*
 * Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * syst.h - YST Threads package OSD component
 */

#ifndef SYST_ORACLE
#define SYST_ORACLE

#ifndef SYSI_ORACLE
# include <sysi.h>
#endif

/*
 * Thread Stack Creation and Destruction.
 *
 * DESCRIPTION
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

#endif /* SYST_ORACLE */
