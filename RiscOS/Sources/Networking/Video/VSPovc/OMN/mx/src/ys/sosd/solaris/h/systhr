/* Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
 *
 * systhr.h - OMN system-dependent threading declarations
 */

#ifndef SYSTHR_ORACLE
#define SYSTHR_ORACLE

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif

EXTC_START

/*
 * Type Declarations
 */
typedef struct systhrop systhrop;

/*
 * systhrop - Thread Operations Package
 *
 * DESCRIPTION
 * A thread package must define several operations that constitute the
 * primitives on which the generic code depends for proper threaded
 * execution.
 *
 * AT PRESENT, MEDIA NET REQUIRES THAT A THREADS PACKAGE PROVIDE A
 * COOPERATIVE THREADING MODEL.  PRE-EMPTIVE THREADS WILL NOT WORK
 * CORRECTLY.  What we mean by cooperative is that the current thread
 * of execution will not be pre-empted in favor of another thread by
 * an asynchronous event, including page faults or expiration of a
 * timeslice.  Asynchronous events may be handled, but execution must
 * then resume on the same thread.  Threads may be switched synchronously
 * as part of any system call.  If the thread package cannot guarantee
 * this, you must use OMS threads instead.
 *
 * Name
 *   The name identifies the name of the threads package.  We suggest
 *   NATIVE for the native platform, DCE for DCE threads, and require
 *   OMS for the Oracle Media Server Threads Package.
 *
 * Descriptor Size
 *   The descsz indicates the size (in bytes) of the thread descriptor
 *   that is used by the thread package.  Several routines either pass
 *   or are expected to return thread descriptors.  Media Net does not
 *   support thread descriptors that are larger than 32 bytes.
 *
 * Create
 *   The create() routine is invoked to create a thread.  The new thread
 *   should begin execution by calling the start routine with the single
 *   argument arg.  If the start routine returns, the thread should exit.
 *   stksz specifies the size of the stack that the new thread should use.
 *   If it is zero, the thread package should choose a default.  The new
 *   thread should be runnable.  A descriptor for the created thread should
 *   be written to td; td will be allocated to hold at least descsz bytes.
 *   On success, the routine should return 0; on failure, it should return
 *   a non-zero value.
 *
 * Exit
 *   The exit() routine is invoked to terminate the current thread.
 *
 * Yield
 *   The yield() routine is invoked to yield execution of the current
 *   thread in favor of another.  The thread should remain runnable.
 *
 * Suspend
 *   The suspend() routine is invoked to immediately suspend execution
 *   of the current thread.  The call is not expected to return until
 *   the thread is resumed.
 *
 * Resume
 *   The resume() routine is invoked to resume execution of a suspended
 *   thread.  td is the thread descriptor of the thread to resume.  It
 *   was returned either from the create() or the self() routine.
 *
 * Self
 *   The self() routine is invoked to get the identity of the current
 *   thread.  The thread's descriptor should be written to td; td will
 *   be allocated to hold at least descsz bytes.
 *
 * Print
 *   The print() routine is invoked to format the thread descriptor into
 *   a form suitable for printing.  The thread descriptor passed in td
 *   was returned either from the create() or the self() routine.  The
 *   output should be written to buf, which is len bytes long.  len will
 *   always be at least 32.
 */
struct systhrop
{
  CONST char *name;                                   /* thread package name */
  size_t      descsz;                              /* thread descriptor size */

  sword  (*create)(ub1 *td, void (*start)(dvoid *), dvoid *arg, size_t stksz);
  void   (*exit)(void);
  void   (*yield)(void);
  void   (*suspend)(void);
  void   (*resume)(ub1 *td);
  void   (*self)(ub1 *td);
  void   (*print)(ub1 *td, char *buf, size_t len);
};

/*
 * systhrtab - thread package table
 *
 * DESCRIPTION
 * The porter must provide bindings here for whatever thread packages are
 * to be supported on the platform.  For instance, one may choose to support
 * native threads as well as DCE threads.  The support provided here will
 * determine which threads packages Media Net will be able to integrate with.
 *
 * The porter should ensure that the OMS portable threads package is
 * always included (on server platforms; a threads package is not required
 * on a client-only platform).  Also, if no native thread package exists,
 * the OMS portable threads package may be used as the only one available
 * on the platform.  However, there are good reasons to support other threads
 * package, such as better native tool support and better O/S integration.
 *
 * The first entry in the table will be selected by default if the
 * application does not specify one.  A threads package is not required
 * on a client-only platform.
 */
externref CONST_DATA sword SYSTHRTAB_MAX;
externref CONST_DATA systhrop *CONST_W_PTR systhrtab[];

EXTC_END
#endif /* SYSTHR_ORACLE */
