/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * syssp.h - OMX Spawning Package
 */

#ifndef SYSSP_ORACLE
#define SYSSP_ORACLE

/*
 * Public constant definitions
 */
#define SYSSP_STS_STARTED   ((sword) 1)
#define SYSSP_STS_FAILURE   ((sword) 2)

#define SYSSP_STS_EXITOK    ((sword) 1)
#define SYSSP_STS_EXITNOTOK ((sword) 2)
#define SYSSP_STS_ALIVE     ((sword) 3)
#define SYSSP_STS_RUNNING   ((sword) 4)
#define SYSSP_STS_STOPPED   ((sword) 5)
#define SYSSP_STS_NOTFOUND  ((sword) 6)

/*
 * This package is intended for use by certain processes that require the
 * ability to start new processes to perform certain tasks.  Because this
 * is a highly operating-system-specific function, this package may be
 * implemented completely, partially or not at all.
 *
 * A complete implementation is capable of starting an arbitrary executable
 * given the pathname.  A partial implementation recognizes a well-known set
 * of pathnames and only those executables can be spawned.
 *
 * To indicate a complete or partial implementation, define the value of
 * SYSSP_IMPLEMENTED to be TRUE; otherwise, FALSE.
 */
#define SYSSP_IMPLEMENTED  TRUE

#ifdef SYSSP_IMPLEMENTED

/*
 * sysspInit - initialize spawning package
 *
 * DESCRIPTION
 * sysspInit() is called before any other routine in the spawning library
 * to allow the package to initialize in any way necessary to implement
 * the functionality provided.  sysspInit() should return a non-null pointer
 * to a private context to indicate success (even if the context is empty).
 * A return value of a null pointer indicates failure.
 *
 * sysspTerm() will be called to deallocate resources used by the spawning
 * library, and in particular, the spawning context.  After this is called,
 * no other spawning function except sysspInit() may be called.
 */
typedef struct sysspctx sysspctx;

sysspctx *sysspInit(dvoid *osdp);
void sysspTerm(sysspctx *ctx);

/*
 * sysspSpawn - spawn a process
 *
 * DESCRIPTION
 * sysspSpawn() is called to spawn a process.  ctx is the context obtained
 * from sysspInit().  path is the name of the executable specified in an
 * O/S-dependent manner.  This string should be obtained from an external
 * location and passed unchanged to this routine.  argc and argv together
 * specify the portable argument list that eventually would be passed to
 * the generic entrypoint of the new program (see s0ysmain.c).  This routine
 * may do any processing on this argument list required to make it suitable
 * for the operating system.
 *
 * If the process is successfully spawned, the routine should return
 * SYSSP_STS_STARTED and write the process ID into the buffer pointed
 * to by pid, which is len bytes long (and guaranteed to be at least 64
 * bytes).  This pid information is treated as opaque and will be passed
 * back to other routines used here for interpretation.  It should also
 * match the result obtained from syspGetPid() if called by the spawned
 * process.
 *
 * If the process could not be started, the routine should return
 * SYSSP_STS_FAILURE.
 */
sword sysspSpawn(sysspctx *ctx, char *pid, size_t len,
		 CONST char *path, sword argc, char **argv);

/*
 * sysspPSpawn - parallel spawn
 *
 * DESCRIPTION
 * sysspPSpawn() is called to spawn many processes.  The parameters
 * are interpreted the same as for sysspSpawn() with the following
 * exceptions:
 *
 * affinity_set is a specification of where and how many processes are
 * to be launched.  The affinity_set is specified in an O/S-dependent
 * manner, and should be obtained from an external location and passed
 * unchanged to this routine.
 *
 * pids is a list of the process IDs that were created as a result of
 * this spawn.  The list should be created by this routine and each
 * element in the list should be an allocated string containing the
 * individual process IDs (that is, the element values should be castable
 * to char *).  The list may be freed by the caller using
 *
 *    ysLstDestroy(pids, ysmFGlbFree);
 *
 * On success, the routine should return SYSSP_STS_STARTED.  On failure,
 * the routine should return SYSSP_STS_FAILURE after cleaning up any
 * processes that may already have been started.  (No pid list will be
 * returned.)
 */
sword sysspPSpawn(sysspctx *ctx, yslst **pids,
		  CONST char *path, CONST char *affinity_set,
		  sword argc, char **argv);

/*
 * sysspMakeSet - make an affinity set
 *
 * DESCRIPTION
 * sysspMakeSet() makes an affinity set suitable for sysspPSpawn()
 * that will cause cnt processes to be spawned.  This routine should
 * be used when, rather than having an exact affinity set, only the
 * number of spawned processes is indicated.  The affinity set should
 * be written to the buffer pointed to by buf, which should be at least
 * len bytes long (should be at least 64 bytes to be assured of success).
 * On success, this routine returns TRUE; otherwise, FALSE.
 *
 * If zero is passed for cnt, then the platform should choose some
 * reasonable default that corresponds in some way to the hardware
 * resources available (e.g. all processors, all processors but one,
 * etc.).
 */
boolean sysspMakeSet(sysspctx *ctx, sword cnt, char *buf, size_t len);

/*
 * sysspCountSet - count an affinity set
 *
 * DESCRIPTION
 * sysspCountSet() counts the number of processes that will be launched
 * by a given affinity set.  If the affinity set is not valid, this
 * routine returns -1.
 */
sword sysspCountSet(sysspctx *ctx, CONST char *affinity_set);

/*
 * sysspGetStatus - get the status of a process
 *
 * DESCRIPTION
 * sysspGetStatus() gets the status of a process given a process ID.
 * There are several possible results:
 *
 * SYSSP_STS_NOTFOUND - the process ID is not valid
 * SYSSP_STS_EXITOK - the process has exited cleanly
 * SYSSP_STS_EXITNOTOK - the process died in some abnormal fashion
 * SYSSP_STS_ALIVE - the process ID is valid but no other status is available
 * SYSSP_STS_RUNNING - the process is running
 * SYSSP_STS_STOPPED - the process is stopped
 *
 * Note that not all platforms will be able to report all status values.
 * Moreover, the interpretations of the status values are somewhat loose.
 * At a minimum, SYSSP_STS_NOTFOUND and SYSSP_STS_ALIVE should be able to
 * be reported, or the package is not truly implementable.
 */
sword sysspGetStatus(sysspctx *ctx, CONST char *pid);

/*
 * sysspForget - forget the status of a process
 *
 * DESCRIPTION
 * The spawning package should retain information about any process
 * successfully spawned by the package until this routine is called
 * with the appropriate process ID.  Thus, even if a spawned process
 * exited, sysspGetStatus() can be called repeatedly to get the exit
 * status until this routine is called.  If the process ID is not
 * recognized, this call should simply ignore it.
 */
void sysspForget(sysspctx *ctx, CONST char *pid);

/*
 * sysspKill - terminate a process
 *
 * DESCRIPTION
 * sysspKill() terminates a process named by pid in an O/S-dependent
 * fashion.  This is different from requesting a higher-level shutdown
 * of a process.  In general, this should be used when there is no other
 * communication mechanism available to terminate the process.
 *
 * The nice parameter is used to distinguish the way in which the process
 * is terminated.  The difference here is the difference between SIGTERM
 * (when nice is TRUE) and SIGKILL (when nice is FALSE) on UNIX-based
 * systems, in that SIGTERM can generally be caught and handled cleanly,
 * whereas SIGKILL cannot be refused.  This distinction may have no meaning
 * on your platform in which case you can ignore this parameter.
 *
 * This routine should return SYSSP_STS_NOTFOUND if the pid is invalid;
 * SYSSP_STS_ALIVE if the pid is valid but identifies a process that cannot
 * be killed (e.g. lack of privilege, etc.); SYSSP_STS_EXITOK if nice is
 * TRUE and implementable and the termination message was delivered; and
 * SYSSP_STS_EXITNOTOK if nice is FALSE (or a "nice" termination is not
 * possible), and the termination message was delivered.
 *
 * Killing a spawned process does not automatically "forget" it.
 */
sword sysspKill(sysspctx *ctx, CONST char *pid, boolean nice);

#else  /* SYSSP_IMPLEMENTED != TRUE */

#define sysspInit(osdp)                                  ((sysspctx *) 0)
#define sysspTerm(ctx)
#define sysspSpawn(ctx, pid, len, path, argc, argv)      SYSSP_STS_FAILURE
#define sysspPSpawn(ctx, pids, path, afset, argc, argv)  SYSSP_STS_FAILURE
#define sysspMakeSet(ctx, cnt, buf, len)                 FALSE
#define sysspCountSet(ctx, afset)                        0
#define sysspGetStatus(ctx, pid)                         SYSSP_STS_NOTFOUND
#define sysspForget(ctx, pid)
#define sysspKill(ctx, pid, nice)                        SYSSP_STS_NOTFOUND

#endif /* SYSSP_IMPLEMENTED */

#endif /* SYSSP_ORACLE */
