/* -*-c-*- */

/* History:
 *  30-Sep-96 pdh Change to be more cautious about what it thinks is a name
 *
 */

#include <stdio.h>
#include <setjmp.h>
#include "kernel.h"
#include "unwind.h"

#ifdef MemCheck_MEMCHECK
#include "memcheck/MemCheck.h"
#endif

#define FRAME_SCP 0
#define FRAME_LR -1
#define FRAME_SP -2
#define FRAME_FP -3

extern char *procname(int pc)
{
    unsigned int *i;
    char *name;

    pc &= 0x03FFFFFC;

    i = (unsigned int*) (long) pc;

    while ((pc - ((int) (long) i)) < 10240)
    {
	if (((*i)>>24) == 0xff && ( (*i & 0xffffff) < 80 ) )
	{
	    name = (char*) i - (*i & 0xffffff);
	    return name;
	}
	i--;
    }
#if 0
    fprintf(stderr, "Failed to find name for pc=0x%08x\n", pc);
#endif
    return "<unknown>";
}

/*****************************************************************************

  GCC warns: argument `n' might be clobbered by `longjmp' or `vfork'
  This probably doesn't matter for RISC OS, but it suggests your tempting
  fate.

  Ideally, production builds should not have function names
  embedded. If this is so, then we can't make much use of this
  function, so ensure it is not visible.

  */

#if DEVELOPMENT
extern char *caller(int n)
{
    jmp_buf jb;
    int *fp;
    char *s;

#ifdef MemCheck_MEMCHECK
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);
#endif

    setjmp(jb);

    n++;

    fp = (int *) jb[6];

    while (n && fp)
    {
	fp = (int *) (long) fp[FRAME_FP];
	n--;
    };

    s = fp ? procname(fp[FRAME_SCP]) : NULL;

#ifdef MemCheck_MEMCHECK
    MemCheck_RestoreChecking(checking);
#endif

    return s;
}
#endif
