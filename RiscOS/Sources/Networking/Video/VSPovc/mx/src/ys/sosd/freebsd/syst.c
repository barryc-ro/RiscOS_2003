/* Copyright (c) 1995 by Oracle Corporation. All Rights Reserved.
 *
 * syst.c - OSD source file for threads package, freebsd port
 */

#ifndef SYSI_ORACLE
# include <sysi.h>
#endif

#ifndef SYST_ORACLE
# include <syst.h>
#endif

/* #define SYST_FUDGE 128 whats this?? */
/*
    This is derived from disassembling the setjmp routine in
    the executable.
*/

struct frame
{
        sb4 *pc;
        sb4 *ebx_reg_var;
        sb4 *stack_pointer;
        sb4 *frame_pointer;
        sb4 *esi_reg_var;
        sb4 *edi_reg_var;
        sb4 *eax_retval;
        sb4 *psw;
};
typedef struct frame frame;


/* Memory manager tag */
static   ysmtagDecl( SystStack ) = "systStack";

/* Do OSD work required for thread initialization */
dvoid  *systStkInit( sysejb   jbuf, ub4   stksiz, ub4 totalStkMem )
{
  sb1  *stkbot;                                              /* stack bottom */
  sb1  *stktop;                                                 /* stack top */
  frame  *nfp;                                          /* new frame pointer */

  /* Allocate memory for thread stack */
  stkbot = (sb1 *)ysmGlbAlloc((size_t)stksiz, SystStack);
  stktop = stkbot+stksiz;

  /* Cosmetic work to make backtraces in the debugger work on threads */
  nfp = (struct frame *)(stktop - (2 * sizeof(struct frame)));
  CLRSTRUCT(nfp[0]);
  CLRSTRUCT(nfp[1]);
  nfp[0].frame_pointer = (sb4 *) &(nfp[1]);
  /* nfp[1].frame_pointer = (sb4 *) &(nfp[2]); */
  
  /* Reset the stack pointer in the jump buffer */
  jbuf[0]._jb[2] = nfp;
  
  /* FIXME XXX we don't yet know the actual client of this
     value, so for now, establish the initial stack pointer
     where it should be, taking a guess and setting all
     available stack pointers.
  nfp[0].stacjbuf[0]._jb[2] = nfp;
  nfp[1].stack_pointer = (sb4 *) &nfp[1]; */

  /* return handle to allocated stack memory */
  return (dvoid *)stkbot;
}
 
/* Do OSD cleanup on thread exit */
void   systStkFree( dvoid  *stkhdl )
{
  /* Free the stack memory */
  ysmGlbFree(stkhdl);
}
