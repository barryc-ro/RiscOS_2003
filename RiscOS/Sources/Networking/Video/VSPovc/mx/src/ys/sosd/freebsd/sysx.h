/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysx.h - Oracle MOOSE system-dependent external declarations
 */

#ifndef SYSX_ORACLE
#define SYSX_ORACLE

#ifdef __cplusplus
#define EXTC_START extern "C" {
#define EXTC_END }
#else
#define EXTC_START
#define EXTC_END
#endif

EXTC_START

#ifndef SX_ORACLE
#include <sx.h>
#endif

#ifndef ORASTDARG
#include <stdarg.h>
#define ORASTDARG
#endif
#ifndef ORASETJMP
#include <setjmp.h>
#define ORASETJMP
#endif


/*
 * The following definitions appear in public headers but are not in
 * sx.h.  Therefore, if they are not already defined, we define them
 * here.
 */
#ifndef DISCARD
#define DISCARD (void)
#endif
#ifndef externdef
#define externdef
#endif
#ifndef externref
#define externref extern
#endif
#ifndef CONST
#define CONST const
#endif
#ifndef CONST_DATA
#define CONST_DATA const
#endif
#ifndef CONST_W_PTR
/* Dave Pawson 1/6/96
 * We may want to fix this once we get a Solaris sx.h that defines CONST_W_PTR
 * properly.
 */
#define CONST_W_PTR const
#endif

#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif

/*
 * OSD Layer State
 *
 * SYNOPSIS
 * size_t SYSX_OSDPTR_SIZE;
 *
 * DESCRIPTION
 * This is the size in bytes of the space to allocate for the OSD layer
 * state.  See ysInit() and sysiInit() for more information.  (NOTE:
 * do not define this as zero, even if you have no state; it is illegal
 * to declare an array with a zero or negative subscript.
 */
#define SYSX_OSDPTR_SIZE  ((size_t)128)

/*
 * Non-local Goto
 *
 * SYNOPSIS
 * typedef <> sysejb;
 * int        syseSet(sysejb jb);
 * void       syseJmp(sysejb jb, int val);
 * void       sysePanic(void);
 * void       sysePrtPC(char *buf, dvoid *faddr);
 *
 * DESCRIPTION
 * syseSet()/syseJmp() provide access to a setjmp()/longjmp()-style
 * non-local goto facility.  They are expected to observe the same
 * semantics as setjmp() and longjmp() as defined by ANSI C.  A non-
 * zero value is guaranteed to be passed to syseJmp().
 *
 * sysePanic() is called to generate a process fault.  It is used only
 * in the event of a catastrophic failure.  This routine should terminate
 * execution of the process and never return.  It should also attempt to
 * record system-specific information about the dying process, if possible.
 *
 * sysePrtPC() takes a function pointer address and writes a string form
 * of it to buf.
 */

#define sysejb jmp_buf

#define syseSet(jb)          setjmp(jb)
#define syseJmp(jb, val)     longjmp((jb), (val))
#define sysePanic()          abort()
#define sysePrtPC(nm, addr)  (void) sprintf((nm), "%p", (addr));

/*
 * Context Management
 *
 * SYNOPSIS
 * dvoid *syscGetPG(void);
 * void   syscSetPG(dvoid *ptr);
 *
 * DESCRIPTION
 * These routines provide access to a process-wide global variable.
 * syscSetPG() sets the value of the global pointer.  syscGetPG()
 * returns the last value set.  If it is was never set, syscGetPG()
 * should return a null pointer.  If syscSetPG() is called with a
 * null pointer, any resources required to maintain the global
 * variable may be released and reset to original conditions.
 */
externref dvoid *syscPG;                      /* process-wide global pointer */

#define syscGetPG()     syscPG
#define syscSetPG(ptr)  (syscPG = (ptr))

EXTC_END
#endif /* SYSX_ORACLE */
