/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * s0ysmain.c - OSD main() for executables
 *
 * DESCRIPTION
 * This is a standard template for an OSD main().  To use this template,
 * you should invoke your compiler to compile this file with the following
 * symbols defined:
 *
 *   ENTRY_POINT  (required)  name of the portable entry point
 *
 * For instance,
 *
 *  $(OBJ)/s0myprogmain.o : $(OSD)/s0ysmain.c
 *    $(CC) $(CFLAGS) -o $@ -c  $(OSD)/s0ysmain.c -DENTRY_POINT=myprogMain
 *
 * The OSD code defines main().  This routine invokes your portable entry
 * point, passing it the following:
 *
 *   osdp - OSD pointer; not for general use
 *   nm - name of the program (typically derived from OSD argv[0])
 *   argc, argv - argument count and list (not including the program name
 *                typically passed as argv[0])
 *
 * Any OSD initialization that is required should be done in this routine.
 * The osdp pointer is reserved for use of the OSD layer.  It may point
 * to a stack or heap variable where OSD information is kept.  It is
 * available to the OSD routines during execution of the program so that
 * they may perform their tasks as they see fit.
 */

#include <signal.h>
#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif

/* DISABLE check_proto_decl */
void    exit(int status);
void    main(int argc, char **argv);
boolean ENTRY_POINT(dvoid *osdp, char *nm, sword argc, char **argv);

/*
 * main - OSD main()
 */
void main(int argc, char **argv)
{
  boolean sts;
  ub1     osdbuf[SYSX_OSDPTR_SIZE];
  char    base[SYSFP_MAX_PATHLEN];

  sysfpExtractBase(base, argv[0]);

  /* call main entrypoint */
  sts = ENTRY_POINT((dvoid *) osdbuf, base, (sword) argc - 1, argv + 1);

  /* terminate program */
  exit((int) !sts);
}

/* ENABLE check_proto_decl */
