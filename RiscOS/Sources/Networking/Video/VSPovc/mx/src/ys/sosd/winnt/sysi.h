/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysi.h - OMN system-dependent internal declarations
 */

#ifndef SYSI_ORACLE
#define SYSI_ORACLE

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif

#ifndef ORASTDDEF
#include <stddef.h>
#define ORASTDDEF
#endif
#ifndef ORASTDLIB
#include <stdlib.h>
#define ORASTDLIB
#endif
#ifndef ORASTRING
#include <string.h>
#define ORASTRING
#endif
#ifndef ORACTYPE
#include <ctype.h>
#define ORACTYPE
#endif
#ifndef ORASTDIO
#include <stdio.h>
#define ORASTDIO
#endif
#ifndef ORASIGNAL
#include <signal.h>
#define ORASIGNAL
#endif

#ifndef YS_ORACLE
#include <ys.h>
#endif

/*
 * Version Information
 *
 * DESCRIPTION
 * It may be necessary to issue releases for a particular port that do
 * not involve new versions of the generic code base.  This is reflected
 * in updates to the fourth and fifth digits of the version number.
 *
 * To define these last two digits, simply define the appropriate symbol
 * here.  The symbol should be of the form
 *
 *   SYSI_VERSION_<major>_<minor>_<patch>
 *
 * and the value should be of the form
 *
 *   "<port-release>.<port-patch>"
 *
 * EXAMPLE:  #define SYSI_VERSION_3_0_3  "0.1"
 *
 * for the first port-specific patch release of the generic code base
 * 3.0.3.  We include the generic version in the symbol name, so that,
 * if it is not defined, we will automatically assume "0.0".  This means
 * that this symbol is optional.
 */

/*
 * The following definitions are used in Media Net code but are not in
 * sx.h.  Therefore, if they are not already defined, we define them here.
 */
#ifndef STATICF
#define STATICF static
#endif
#ifndef max
#define max(x,y)  ((x) < (y) ? (y) : (x))
#endif
#ifndef min
#define min(x,y)  ((x) < (y) ? (x) : (y))
#endif
#ifndef bit
#define bit(x, y)     ((x) & (y))
#endif
#ifndef bic
#define bic(x, y)     ((x) &= (~(y)))
#endif
#ifndef bis
#define bis(x, y)     ((x) |= (y))
#endif
#ifndef bics
#define bics(x, y, z) ((z) ? bis((x), (y)) : bic((x), (y)))
#endif
#ifndef noreg
#define noreg volatile
#define NOREG(var)
#endif
#ifndef CPSTRUCT
#define CPSTRUCT(a, b) ((a) = (b))
#endif
#ifndef CLRSTRUCT
#define CLRSTRUCT(a)  DISCARD memset((dvoid *) &(a), 0, sizeof(a))
#endif
#ifndef NULLP
#define NULLP(x)  ((x *) 0)
#endif

/*
 * OSD Layer Initialization
 *
 * DESCRIPTION
 * During initialization, before any other OSD function is called,
 * sysiInit() is called to initialize the OSD program state.
 * SYSX_OSDPTR_SIZE bytes of memory are provided and pointed to by
 * osdp.  This memory is guaranteed to last for the entire execution
 * of the program.  (Typically, it is allocated on the stack of the
 * main() routine.)  It is passed back to most OSD routines, and is
 * always available using ysGetOsdPtr().  (This routine requires
 * syscGetPG(), so assuming that one global variable is available,
 * it is possible to allocate all other memory used by the program
 * off the stack.)  sysiInit() may return FALSE if it cannot initialize.
 *
 * During termination, as the last OSD function called, sysiTerm() is
 * called to allow the OSD layer to clean up any program state it may
 * have.
 *
 * SYSX_OSDPTR_SIZE is defined in sysx.h to allow third-party programs
 * to initialize properly.
 */
boolean sysiInit(dvoid *osdp);
void sysiTerm(dvoid *osdp);


/*
 * Memory Management
 *
 * SYNOPSIS
 * size_t SYSM_STRICT_ALIGN;
 *
 * boolean sysmInit(dvoid *osdp, ysmaf *afp, ysmrf *rfp, ysff *ffp,
 *                  ysbv **bv, sword *nbv);
 * void    sysmTerm(dvoid *osdp, ysbv *bv, sword nbv);
 *
 * DESCRIPTION
 * The MN layer assumes a globally-available process heap.  This heap
 * may be supplied either via OSD callback functions or by a fixed-size
 * heap.  sysmInit() is invoked during initialization to acquire this heap.
 * Either afp, rfp, and ffp should be initialized to the callback functions
 * or bv and nbv should be initialized to describe the fixed-size heap.
 * Only one or the other should be provided, not both.  sysmInit() may
 * return FALSE if it cannot initialize.
 *
 * If a heap is provided, the memory described by the buffer vector must
 * remain valid until sysmTerm() is called.  It will be called prior to
 * normal process termination.
 *
 * One possible source of a fixed-size heap is off the stack in main().
 * The osdp pointer is passed from main into the service layer and back
 * to the routines for initialization.
 *
 * SYSM_STRICK_ALIGN is the strictest alignment required on the platform.
 */
#define SYSM_STRICT_ALIGN  (sizeof(double)) 

boolean sysmInit(dvoid *osdp, ysmaf *afp, ysmrf *rfp, ysmff *ffp,
		 ysbv **bv, sword *nbv);
#define sysmTerm(osdp, bv, nbv)                                /* do nothing */

/*
 * Resource Management
 *
 * SYNOPSIS
 * void sysrLoad(dvoid *osdp);
 * ysargmap *sysrGetMap(dvoid *osdp);
 *
 * DESCRIPTION
 * sysrLoad() is called during initialization to allow platform-specific
 * initialization of the process resource database.  The typical thing
 * done by this routine is to load environment variables.  It may choose
 * to do nothing if no initialization is appropriate.
 *
 * sysrGetMap() returns a platform-specific argument map that may be used to
 * specifiy platform-specific arguments.  If there is no such argument map,
 * a null pointer should be returned.
 */
void sysrLoad(dvoid *osdp);
#define sysrGetMap(osdp)   ((ysargmap *) 0)

/*
 * Process Information
 *
 * SYNOPSIS
 * boolean syspGetHostName(dvoid *osdp, char *buf, size_t len);
 * boolean syspGetPid(dvoid *osdp, char *buf, size_t len);
 * boolean syspGetAffinity(dvoid *osdp, char *buf, size_t len);
 * boolean syspGetCpuTime(dvoid *osdp, sysb8 *cputm);
 * boolean syspGetMemUsage(dvoid *osdp, ub4 *kbytes);
 *
 * DESCRIPTION
 * These routines return information about the process.  If any of the
 * requested information is not available or meaningful, the routine
 * may return FALSE; otherwise, it should return TRUE.
 *
 * syspGetHostName(), syspGetPid(), and syspGetAffinity() should all
 * write the requested information into the given buffer, which will
 * be at least len bytes long (guaranteed to be at least 64 bytes).
 *
 * syspGetCpuTime() should return the number of CPU microseconds used
 * by the process.  This value should be written to cputm.
 *
 * syspGetMemUsage() should return the number of kilobytes of memory
 * being used by the process.  This value should be written to kbytes.
 */

#include <process.h>
#define syspGetHostName(o, b, l)  (gethostname((b), (l)) == 0)
#define syspGetPid(o, b, l)       (sprintf((b), "%d", _getpid()), TRUE)
boolean syspGetAffinity(dvoid *osdp, char *buf, size_t len);
boolean syspGetCpuTime(dvoid *osdp, sysb8 *cputm);
boolean syspGetMemUsage(dvoid *osdp, ub4 *kbytes);

/*
 * Console Management
 *
 * SYNOPSIS
 * size_t  syslWidth(void);
 * void    syslPrint(const char *fmt, va_list ap);
 * void    syslError(const char *fmt, va_list ap);
 * void    syslConsole(sword level, const char *fmt, va_list ap);
 * void    syslDetach(void);
 * boolean syslGetc(dvoid *osdp, sword *ch, ub4 delay);
 *
 * DESCRIPTION
 * These routines perform terminal I/O.  syslWidth() returns the width of
 * the terminal line in characters.  syslPrint() is equivalent to vprintf().
 * syslError() is equivalent to vfprintf(stderr, ...).
 *
 * syslConsole() should find a host logger and log an output message.
 * The level passed to syslConsole() is one of the severities defined
 * in yslog.h.  It can be ignored if necessary.  It is called after
 * the process has been detached from its controlling terminal and
 * needs to output an error message.
 *
 * syslDetach() is called to do system-dependent detach processing, like
 * setpgrp() or _vertex_detach() or whatever (maybe nothing).  Once it is
 * called, syslPrint(), syslError(), and syslGetc() will not be called.
 *
 * syslGetc() is called to get a single character from stdin.  If the
 * end of the input stream is reached, syslGetc() should return TRUE;
 * otherwise, *ch should be set to the character that was read and the
 * routine should return FALSE.  IMPORTANT:  syslGetc() should, like
 * sysiPause(), not block more than delay microseconds even if there
 * is no data to read.  It should also try to return sooner if another
 * low-level event occurred, like an interrupt or network I/O.  If
 * syslGetc() needs to return without having read a character, it
 * should set *ch to '\0' and return FALSE.
 *
 * If syslGetc() is not implementable, it may simply return TRUE all
 * the time, indicating that there is no input stream.
 * 
 */

#define syslWidth()         ((size_t) 79)

void syslPrint( const char *fmt, va_list ap );
void syslError( const char *fmt, va_list ap );
void syslConsole(sword level, const char *fmt, va_list ap);

boolean syslGetc(dvoid *osdp, sword *ch, ub4 delay);
#define syslDetach()        ((void) (FreeConsole()))

/*
 * Time Management
 *
 * SYNOPSIS
 * ub4     systmGetTicks(void);
 * boolean systmGetClock(sysb8 *clock);
 * void    systmClockToTime(sysb8 *clock, ystm *sttm);
 *
 * DESCRIPTION
 * These routines provide access to a hardware clock.
 *
 * systmGetTicks() returns a tick counter expressed in microseconds.  No
 * absolute meaning is ascribed to the value, other than that the difference
 * between any two values returned is the number of microseconds elapsed
 * between the two calls.  It is expected that the value will wrap every
 * 71 minutes.  This function should be implemented in the fastest way
 * possible for the platform.
 *
 * systmGetClock() should return the value of a real-time clock.  If one
 * is not available, just return FALSE instead of TRUE.  The clock value
 * is represented as some number of seconds since an arbitrary epoch.
 * systmClockToTime() is expected to know this epoch and convert a given
 * number of seconds into the structured form.  Usually, a function like
 * the ANSI C localtime() will do the trick.
 */

ub4 systmGetTicks(void);
boolean systmGetClock(sysb8 *clock);
void    systmClockToTime(sysb8 *clock, ystm *sttm);

/*
 * Interrupt Management
 *
 * SYNOPSIS
 * typedef <> sysimsk;
 * sysimsk sysiNoMask;
 * sysimsk sysiDisable(dvoid *osdp);
 * void    sysiEnable(dvoid *osdp, sysimsk mask);
 * void    sysiPause(dvoid *osdp, sysimsk mask, ub4 delay);
 *
 * DESCRIPTION
 * sysiDisable() and sysiEnable() are used to bracket critical regions
 * in the code.  There are two routines in OMN that are callable
 * asynchronously: ysIntr() and ytnPush().  ysIntr() is called when
 * either a user-level interrupt or shutdown request occurs.  ytnPush()
 * is called when data arrives for a network interface driver that cannot
 * or does not buffer packets.
 *
 * sysiDisable() and sysiEnable() should be defined such that from the
 * time sysiDisable() is called to the time sysiEnable() is called,
 * neither ysIntr() nor ytnPush() are called.  Ideally, these events
 * are merely delayed, not lost altogether, but that is not required.
 * sysiDisable() may return a mask that is later passed to sysiEnable().
 *
 * It is possible that sysiPause() may called during a sysiDisable()/
 * sysiEnable() sequence.  In this case, sysiPause() should wait until
 * a low-level event occurs or until delay microseconds has elapsed.  If
 * an event does occur, it should be allowed to take effect (by calling
 * ysIntr() or ytnPush()), and then sysiPause() should return immediately.
 * In either case, sysiPause() should restore the process to a "disabled"
 * state when it returns.
 *
 * Sometimes, sysiPause() will be invoked outside of a critical region,
 * simply to put the process to sleep for a while.  In this case, the
 * value for mask is sysiNoMask and upon return, the the process should
 * not be in a "disabled" state.
 */

typedef ub4 sysimsk;
#define sysiNoMask	(sysiGetNoMask())
sysimsk sysiGetNoMask(void);
sysimsk sysiDisable(dvoid *osdp);
void    sysiEnable(dvoid *osdp, sysimsk mask);
void    sysiPause(dvoid *osdp, sysimsk mask, ub4 delay);

/* function to use to print or error */
typedef void (*syslPFunc)(const char *fmt, va_list ap);

/* Function to initialize osdp, _before_ calling sysiInit with it.
   It's OK to use NULL for the functions -- you'll get console
   defaults. */
void syslOsdInit( dvoid *osdp, syslPFunc pfunc, syslPFunc efunc );

/*
 * External Event Synchronization
 *
 * SYNOPSIS
 * int ssysAddFd( dvoid *osdp, int fd, HANDLE *pEvt)
 * void ssysRemFd( dvoid *osdp, int slot )
 * void ssysSetFdEvent( dvoid osdp, int slot )
 *
 * DESCRIPTION
 * ssysAddFd() is called to add the fd which is exported to Media Net to the 
 * events that can wake up sysiPause. On NT, this function creates a thread
 * which will do a select on the fd and will wakeup sysiPause whenever there is
 * input data.
 *
 * ssysRemFd() is called to remove the fd from the list that can wake up
 * sysiPause. This call terminates the thread created in sysAddFd().
 *
 * ssysSetFdEvent() is called to let Media Net know that the external comm.
 * medium has read input data. This call wakes up the thread created in
 * ssysAddFd() which will in turn wake up sysiPause whenever it detects an
 * input packet.
 *
 */
#define SSYS_FD_INPUT 0                       /* for compatibility */
int ssysAddFd( dvoid *osdp, ub4 fd, ub4 dummy);
void ssysRemFd( dvoid *osdp, int slot, ub4 dummy );
void ssysSetFdEvent( dvoid *osdp, int slot );

#endif /* SYSI_ORACLE */



















