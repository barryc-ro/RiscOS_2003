/* Copyright (c) 1994, 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * smtl.c - System-specific Media Tools Logging routines
 *
 * NOTES
 * WinNT OS version.
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef MTL_ORACLE
#include <mtl.h>
#endif
#ifndef SMTL_ORACLE
# include <smtl.h>
#endif
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif

/* Vendor-supplied headers. */
#include <stdio.h>

#include <time.h>

/* Global data - permitted since OSD. */
static sb4   smtlMethod;
static CONST char *smtlStr;
static FILE *smtlFp;

void smtlInit(sb4 method, CONST char *str)
{
  char fn[256];

  smtlMethod = method;
  smtlStr = str;

  switch (method)
    {
    case MtlScreenTool:
      smtlFp = stderr;
      break;
    case MtlLogTool:
      sprintf(fn, "%s.log", str);
      if (!(smtlFp = fopen(fn, "w")))
	{
	  yslError("%s: could not open log file\n", str);
	  exit(1);
	}
      else
	setbuf(smtlFp, (char *) 0);
      break;
    case MtlConsoleTool:
      if (!ssyslOpenSysLog())
        fprintf(stderr, "%s: failed to open event log\n", str);
      break;
    default:
      yslError("%s: bad log parameter\n", str);
      exit(1);
      break;
    }
}

void smtlLog(const char *fmt, va_list ap)
{
  time_t     clock;
  struct tm *tm;
  char tmt[64];
  char buf[1024];

  time(&clock);
  tm = localtime(&clock);
  strftime(tmt, 64, "%T", tm);

  vsprintf(buf, fmt, ap);
  switch (smtlMethod)
    {
    case MtlScreenTool:
    case MtlLogTool:
      fprintf(smtlFp, "%s: %s: %s\n", tmt, smtlStr, buf);
      break;
    case MtlConsoleTool:
      ssyslSysLog(EVENTLOG_INFORMATION_TYPE, 0, YSLSEV_INFO, buf);
      break;
    }
}

void smtlFatal(const char *fmt, va_list ap)
{
  smtlLog(fmt, ap);
  abort();
}

void smtlExit(const char *fmt, va_list ap)
{
  smtlLog(fmt, ap);
  exit(1);
}
