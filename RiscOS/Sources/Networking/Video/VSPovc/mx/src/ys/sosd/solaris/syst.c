/* Copyright (c) 1995 by Oracle Corporation. All Rights Reserved.
 *
 * syst.c - OSD source file for threads package, sun4 port
 */

#ifndef SYSI_ORACLE
# include <sysi.h>
#endif

#ifndef SYST_ORACLE
# include <syst.h>
#endif

#include <sys/frame.h>

/* Memory manager tag */
static   ysmtagDecl( SystStack ) = "systStack";

/* Do OSD work required for thread initialization */
dvoid  *systStkInit( sysejb   jbuf, ub4   stksiz, ub4 totalStkMem )
{
  sb1  *stkbot;                                              /* stack bottom */
  sb1  *stktop;                                                 /* stack top */
  struct frame *nfp;                                    /* new frame pointer */

  /* Allocate memory for thread stack */
  stkbot = (sb1 *)ysmGlbAlloc((size_t)stksiz, SystStack);
  stktop = stkbot+stksiz;

  /* Cosmetic work to make backtraces in the debugger work on threads */
  nfp = (struct frame *)(stktop - (2 * sizeof(struct frame)));
  CLRSTRUCT(nfp[0]);
  CLRSTRUCT(nfp[1]);
  nfp[0].fr_savfp = &(nfp[1]);

  /* Reset the stack pointer in the jump buffer */
  jbuf[1] = (int)nfp;

  /* return handle to allocated stack memory */
  return (dvoid *)stkbot;
}

/* Do OSD cleanup on thread exit */
void   systStkFree( dvoid  *stkhdl )
{
  /* Free the stack memory */
  ysmGlbFree(stkhdl);
}
