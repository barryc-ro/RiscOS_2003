/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysrld.c - OSD resource load
 */

extern char **environ;

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif

/*
 * sysrLoad - resource database loader
 */
void sysrLoad(dvoid *osdp)
{
  char **scp;

  /* load environment variables */
  for (scp = environ; *scp; scp++)
    ysResParse((char *) 0, 0, *scp);
}
