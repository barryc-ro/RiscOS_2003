/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    ssysi.h
  DESCRIPTION
    header for ssysi functions, seen only by interested osd code.
  PUBLIC FUNCTIONS
    <x>
  NOTES
    <x>
  MODIFIED   (MM/DD/YY)
    dbrower   09/ 9/96 -  created.
*/

#ifndef SSYSI_ORACLE
#define SSYSI_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

/* PUBLIC TYPES AND CONSTANTS */

/* Media Net Home key in the registry*/
#ifndef SSYSI_PRODUCT_HOME_KEY
#define SSYSI_PRODUCT_HOME_KEY "MN33"
#endif

/* default Media Net home directory */
#ifndef SSYSI_DEF_PRODUCT_HOME
#define SSYSI_DEF_PRODUCT_HOME "C:\\ORANT\\MN33"
#endif

/* Where yslError replacement file is placed when running as a service */

#ifndef SSYSI_YSL_ERR_DIR
#define SSYSI_YSL_ERR_DIR "\\log\\"
#endif

/* Suffix used on SSYSI_YSL_ERR_DIR/<progname> for yslError file */
#ifndef SSYSI_YSL_ERR_SFX
#define SSYSI_YSL_ERR_SFX   ".log"
#endif

/* Prefix used for services in the registry */
#ifndef SSYSI_SVC_PREFIX
#define SSYSI_SVC_PREFIX    "OraMediaNet"
#endif

/* Name used for service arguments in the registry */
#ifndef SSYSI_SVC_ARGVAL
#define SSYSI_SVC_ARGVAL    "Arguments"
#endif

/* Where service params are kept */
#ifndef SSYSI_SVC_PARMS_PREFIX
#define SSYSI_SVC_PARMS_PREFIX "SOFTWARE\\Oracle\\MediaNet\\"
#endif

/* Where in registry the EventMessageFile is put */
#ifndef SSYSI_EVENT_LOGKEY
#define SSYSI_EVENT_LOGKEY "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\OraMediaNet"
#endif

/* Value the EventMessageFile registry entry is given */
#ifndef SSYSI_EVENT_MSGFILE
#define SSYSI_EVENT_MSGFILE "\\bin\\sysilog.exe"
#endif

/* Source given to things sent to ReportEvent */
#ifndef SSYSI_EVTSRC
#define SSYSI_EVTSRC SSYSI_SVC_PREFIX
#endif

/* PUBLIC FUNCTIONS */

/*
 * YS system-specific Init and Term routines
 *
 * wrapper for ysInit and ysTerm. ssysiInit can be called multiple times within
 * a single app. A reference counter is kept to keep track of the number of
 * references made to ssysInit. It calls ysInit the first time being called.
 * subsequent calls would only incremnet the reference counter.
 *    RETURNS
 *       TRUE  : ys initialized successfully
 *       FALSE : ys not initialized successfully
 *
 * ssysiTerm() decrements the reference counter and calls ysTerm when
 * reference counter reaches 0.
 */
boolean ssysiInit(dvoid *osdp, const char *nm);
void    ssysiTerm(dvoid *osdp);

/*
 * ssyslOpenSysLog() registers event log subkey "OraMediaNet" under WinNT's
 * Application event log key. ssyslSysLog() logs an event to that key.
 * Use EventView utility to view these system log.
 */
boolean ssyslOpenSysLog(void);
void ssyslSysLog(WORD etype, WORD ecat, WORD eid, CONST char * buf);

/* These are used to inform sysiPause about completions.  Caller is
   responsible to signal when the i/o completes, and to do the necessary
   ReleaseHandle after calling RemWaitObject */

/* returns success, TRUE if OK. */
boolean ssysAddWaitObject( dvoid *osdp, HANDLE h );
void ssysRemWaitObject( dvoid *osdp, HANDLE h );

#endif

