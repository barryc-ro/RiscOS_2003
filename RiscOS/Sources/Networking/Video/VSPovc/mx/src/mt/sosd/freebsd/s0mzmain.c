/* Copyright (c) 1994, 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * s0mzmain.c - OSD main() for executables
 *
 * DESCRIPTION
 * This is a standard template for an OSD main().  To use this template,
 * you should invoke your compiler to compile this file with the following
 * symbols defined:
 *
 *  ENTRY_POINT      (required)  name of the generic main routine to call
 *  DEFAULT_MTL_TOOL (optional)  one MtlScreenTool, MtlLogTool, MtlConsoleTool
 *
 * For instance,
 *
 *  $(OBJ)/s0mzyour.o : $(OSD)/s0mzmain.c
 *    $(CC) $(CFLAGS) -o $@ -c  $(OSD)/s0mzmain.c -DENTRY_POINT=mzyourMain
 *
 * The OSD code defines main().  This routine will set up the ys layer,
 * initialize the mtl logger, and invoke your portable entry point, passing
 * it the following:
 *
 *   osdp - OSD pointer; not for general use
 *   nm - name of the program (typically derived from OSD argv[0])
 *   argc, argv - argument count and list (not including the program name
 *                typically passed as argv[0])
 *
 * NOTE:
 * This is a transitional OSD for existing code that relies on mtl.  New
 * code should use s0ysmain.c instead and move the ys layer initialization
 * to the generic code.
 */


#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif

#ifndef MTL_ORACLE
#include <mtl.h>
#endif

#include <ctype.h>
#include <sys/types.h>		                           /* For pid_t type */
#include <unistd.h>		                 /* For getpid() system call */

/* DISABLE check_proto_decl */
void    main(int argc, char **argv);

/* will default mtl tool to screen */

#ifndef DEFAULT_MTL_TOOL
#define DEFAULT_MTL_TOOL    MtlScreenTool
#endif

/* ENTRY_POINT had better be defined... */

#ifndef ENTRY_POINT
    /* Raise compile error on ANSI with #error and non/ANSI by 
       not having the #error in the first column. */
    #error "ENTRY_POINT not defined.  It must be!"
#endif 

boolean ENTRY_POINT(dvoid *osdCtx, CONST char *progName,
 		    sword argc, char **argv);

/*
 * main - OSD main()
 */
void main(int argc, char **argv)
{
  boolean sts;
  ub1 osdbuf[SYSX_OSDPTR_SIZE];
  char *cl, buf[128], base[SYSFP_MAX_PATHLEN];
  
  sysfpExtractBase(base, argv[0]);

  /* standardized initialization */
  ysInit((dvoid *) osdbuf, base);

  if (cl = (char *) getenv("CONSOLE_LOG"))
    {
      sprintf(buf, "%s/%s", cl, base);
      mtlInit( DEFAULT_MTL_TOOL, buf );
    }
  else
    mtlInit( DEFAULT_MTL_TOOL, base );

  sts = ENTRY_POINT( (dvoid *) osdbuf, base, (sword) argc - 1, argv + 1);

  ysTerm((dvoid *) osdbuf);

  /* terminate program */
  exit((int) !sts);
}


