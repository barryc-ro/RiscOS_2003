/* Copyright (c) 1995, 1996 by Oracle Corporation.  All Rights Reserved.
 *
 * sysi.c - OMN system-dependent implementation
 *
 * DESCRIPTION
 * When planning the OSD implementation for Media Net, some consideration
 * should be given ahead of time to how to the process is going to detect
 * and wait for external events.  An external event is something like a
 * user-defined interrupt (e.g. SIGINT, SIGTERM), or inbound data (whether
 * network I/O or keyboard I/O).
 *
 * External events may be posted to OMN through one of two asynchronously
 * callable routines:  ysIntr() and ytnPush().  ysIntr() is used to report
 * user-defined interrupts.  ytnPush() is used to deliver inbound network
 * data.  It is not necessary to call either of these two routines.
 * Especially in the case of a network driver, ytnPush() need only be used
 * when a network driver either does not or cannot buffer inbound data.
 * Whether or not you use ytnPush() asynchronously will largely depend on
 * whether it is more efficient for your O/S to deliver asynchronous
 * notification or respond to periodic polling.  If only or the other is
 * possible, this will dictate the decision for you.  In any case, the
 * mechanics of the network driver(s) for the platform should be decided
 * upon first.  Then, the routines in here, sysiDisable(), sysiEnable(),
 * sysiPause(), and syslGetc() can be written in conjunction with that
 * decision to achieve correct behavior.
 *
 * The routine sysiDisable() is used to notify the OSD layer that these
 * asynchronous callbacks should be temporarily delayed.  They may be
 * delivered once sysiEnable() is called.
 *
 * The OSD layer is asked to wait for an event is two ways:  sysiPause()
 * and syslGetc().
 *
 * sysiPause() is asked to block the process until a certain amount of
 * time elapses OR an external event occurs.  Ideally, sysiPause() returns
 * sooner if an external event occurs, possibly immediately.  sysiPause()
 * may be called in between a sysiDisable()/sysiEnable() pair, in which
 * case it should be prepare to deliver any pending asynchronous callbacks
 * immediately and then return.
 *
 * syslGetc() is basically a superset of sysiPause() that needs to be able
 * to detect keyboard I/O as well.  (Keyboard I/O should otherwise be
 * ignored).
 *
 * IMPLEMENTATION NOTES
 * This implementation is written assuming that the operating system will
 * not deliver asynchronous notification for all low-level events.  This
 * was done primarily to demonstrate how an implementation might work where
 * some combination of polling and self-managed critical regions is necessary.
 * It happens that Solaris is one of the few platforms where all low-level
 * events can be made to cause signals to be delivered, but this is a rarity
 * and so we demonstrate the more common, but slightly more complex case
 * where this is not possible.
 *
 * (We also note that extra care would need to be taken if this were running
 * in a threaded environment since external signals like the ones we are
 * concerned with here get delivered to what is essentially an arbitrary
 * process, and it takes a lot of work to control that.)
 *
 * In this implementation, we never mask out O/S signals.  Also, since we
 * don't ytnPush() asynchronously, sysiDisable() only blocks out the
 * delivery of interrupt and termination notification.  sysiPause()
 * implements its blocking using the WaitForMultipleObjectsEx() mechanism,
 * obliging waiters to register objects using ssysAddWaitObject and 
 * ssysRemWaitObject.
 *
 * The first half of this file contains routines that are affected by
 * these decisions.  The second half of this file contains the miscellaneous
 * routines that are not affected by these decisions.
 *
 * MODIFICATIONS
 *  dbrower   09/ 9/96 -  Change to 3.2, adding ssysAdd/RemWaitObject
 *			  interfaces as the way to coordinate sleeps.
 *			  FIXME/WARNING -- this approach is limited to
 *			  MAXIMUM_WAIT_OBJECTS handles, which is currently
 *			  64.  This will _not_work well if use is
 *			  uncontrolled or we are obliged to have zillions
 *			  of network connections, each consuming an object.
 *  dbrower    09/17/96 - Need to init WinSock in ysInit before you
 *                        can use gethostname (in syspGetHostName macro).
 *  dbrower   11/26/96 -  Add syslOsdInit for syslPrint/Error callbacks;
 *			  Change syslPrint to be funcs instead of macros.
    dbrower   12/30/96 -  Fix 433321 -- force \r to \n in syslGetc.
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif

#include <time.h>
#include <mmsystem.h>

#include <winsock.h>
#include <stdio.h>

/*
 * The routines below are implemented in consideration of choices made about
 * how to deal with asynchronous events and waiting.
 */

/*
 * Private Declarations
 */

/*
 * ys Reference Count - incremented for every ssysiInit and decremented for 
 * every ssysiTerm 
 */
static ub4 ssysiRefCnt = 0;

typedef struct ssysOsdInfo ssysOsdInfo;

/* This is needed for non-Media Net network event notification, eg. IIOP */

struct ssysExtNWEvent
{
  HANDLE       s;
  DWORD        thrid;                       /* the thread id for this event */
  HANDLE       selevt;                          /* the event to be notified */
  ssysOsdInfo  *osdp;                           /* back pointer to OSD info */
  boolean      term;                        /* to inform the thread to exit */
};
typedef struct ssysExtNWEvent ssysExtNWEvent;

/* The size of this structure must be less than SYSX_OSDPTR_SIZE in sysx.h! */

struct ssysOsdInfo
{
  boolean        disable;                 /* true if interrupts are disabled */
  CONST char    *pending;                          /* ysIntr() value pending */
  HANDLE	proc;                                 /* this process handle */
  HANDLE	consoleStdin;                              /* console handle */
  DWORD		nwait;                        /* number of things in waitFor */
  HANDLE	*waitFor;                              /* things to wait for */
  CRITICAL_SECTION  crit;               /* critical section lock for osdinfo */
  char		*conbuf;                        /* pointer to consule buffer */
  char		*conptr;                   /* pointer in conbuf for syslGetc */
  char		*conend;                            /* end of data in conbuf */

  ssysExtNWEvent *extEvents;                 /* non-media net network events */
  HANDLE       pauseevt;                         /* the event to be notified */

  syslPFunc	pfunc;                                  /* stdout print func */
  syslPFunc	efunc;                                  /* stderr print func */
};



externdef ysidDecl(SYSI_EX_TOO_MANY_WAIT_OBJECTS) = "::sysi::tooManyWaitObjs";
externdef ysidDecl(SYSI_EX_BAD_OSDPTR_SIZE) = "::sysi::Bad_SYSX_OSDPTR_SIZE";

STATICF void ssysIntrHndlr(int sig);
STATICF void ssysTermHndlr(int sig);
STATICF BOOL sysiCtrlHandler( DWORD fdwCtrlType );

void syslOsdInit( dvoid *osdp, syslPFunc pfunc, syslPFunc efunc )
{
  ssysOsdInfo *op;

  /* for safety, in case we add stuff here but not in sysx.h */
  if( sizeof(*op) > SYSX_OSDPTR_SIZE )
    ysePanic( SYSI_EX_BAD_OSDPTR_SIZE );

  op = (ssysOsdInfo *) osdp;
  DISCARD memset( (dvoid*)op, sizeof(op), 0 );
  op->pfunc = pfunc;
  op->efunc = efunc;
}

/*
 * sysiInit - OSD layer initialization
 */
boolean sysiInit(dvoid *osdp)
{
  ssysOsdInfo *op;
  /* need winsock 1.1 */
  WORD wVersionRequested = MAKEWORD(1,1);
  WSADATA wsaData;
  BOOL fSuccess;
  int err;

  /* for safety, in case we add stuff here but not in sysx.h */
  if( sizeof(*op) > SYSX_OSDPTR_SIZE )
    ysePanic( SYSI_EX_BAD_OSDPTR_SIZE );

  op = (ssysOsdInfo *) osdp;

  op->disable = FALSE;
  op->pending = (ysid *) 0;

  op->proc = GetCurrentProcess();
  op->consoleStdin = GetStdHandle(STD_INPUT_HANDLE);
  op->nwait = 0;
  op->waitFor = (HANDLE*)malloc(sizeof(HANDLE) * MAXIMUM_WAIT_OBJECTS );
  op->conend = op->conptr = op->conbuf = malloc(BUFSIZ);

  op->extEvents = NULL;
  op->pauseevt = NULL;

  /* need winsock for gethostname */
  err = WSAStartup(wVersionRequested, &wsaData);
  /* FIXME err ignored for now */

  InitializeCriticalSection( &op->crit );

  /* trap signals -- unfortunately, SIGINT comes in on a new thread. */
  DISCARD signal( SIGINT, ssysIntrHndlr );
  DISCARD signal( SIGTERM, ssysTermHndlr );

  /* so also do it the WIN32s way */
  fSuccess = SetConsoleCtrlHandler( (PHANDLER_ROUTINE) sysiCtrlHandler, TRUE);
  if(!fSuccess)
    yslError("sysiInit: setConsoleCtrlHandler failed!?\n", fSuccess );

  return TRUE;
}

/*
 * sysiTerm - OSD layer termination
 */
void sysiTerm(dvoid *osdp)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;

  DeleteCriticalSection( &op->crit );
  free( op->waitFor );
}

/*
 * syslGetc - get a character from the console
 */
boolean syslGetc(dvoid *osdp, sword *ch, ub4 delay)
{
  ssysOsdInfo *op = (ssysOsdInfo*)osdp;
  DWORD	err;
  DWORD	actual;
  boolean eofseen;

  /* anything buffered? */
  if( op->conptr < op->conend )
  {
    *ch = (sword)*op->conptr++;
    eofseen = FALSE;
  }
  else 
  {
    /* have any data already?  */
    if( (err = WaitForSingleObject( op->consoleStdin, 0 )) == WAIT_TIMEOUT )
    {
      /* then wait a while for something to happen */
      DISCARD ssysAddWaitObject( (dvoid*)op, op->consoleStdin );
      sysiPause( osdp, 1, delay ); /* alertable */
      ssysRemWaitObject( op, op->consoleStdin );
      ysYield();		/* give others opportunity to work */
      err = WaitForSingleObject( op->consoleStdin, 0 );
    }

    if( err == WAIT_TIMEOUT )	/* not ready after sysiPause returned */
    {
      *ch = 0;
      eofseen = FALSE;
    }
    else if( err == WAIT_OBJECT_0 ) /* ready now. */
    {
      eofseen = !ReadFile( op->consoleStdin, op->conbuf, BUFSIZ,
			  &actual, NULL );
      if( !eofseen )
      {
	op->conptr = op->conbuf;
	op->conend = op->conbuf + actual;
	*ch = (sword)*op->conptr++;
      }
    }
    else			/* something else -- call it eof */
    {
      eofseen = TRUE;
    }
  }

  /* if a '\r' character has been seen then we have a pending '\n'.  Get it */
  if( !eofseen && (*ch == '\r') )
  {
    /* make sure at least one more character there */
    if (op->conend > op->conptr) 
      *ch = (sword)*op->conptr++;
  }

  return eofseen;
}

sysimsk sysiGetNoMask(void)
{
  return ~(ub4)0;
}

/*
 * sysiDisable - disable signal handler
 */
sysimsk sysiDisable(dvoid *osdp)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;
  EnterCriticalSection( &op->crit );
  op->disable = TRUE;
  LeaveCriticalSection( &op->crit );
  return ~(ub4)0;
}

/*
 * sysiEnable - enable signal handler
 */
void sysiEnable(dvoid *osdp, sysimsk mask)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;

  EnterCriticalSection( &op->crit );

  if (op->disable && op->pending)
    {
      ysIntr(op->pending);
      op->pending = (ysid *) 0;
    }

  op->disable = FALSE;

  LeaveCriticalSection( &op->crit );
}

/*
 * sysiPause - pause for a number of microseconds
 */
void sysiPause(dvoid *osdp, sysimsk mask, ub4 delay)
{
  ssysOsdInfo *op = (ssysOsdInfo *) ysGetOsdPtr();
  boolean      olddis;
  BOOL alertable = (mask) ? TRUE : FALSE;

  op = (ssysOsdInfo *) osdp;

  EnterCriticalSection( &op->crit );
  olddis = op->disable;
  if( op->disable && op->pending )
  {
    ysIntr(op->pending);
    op->pending = (ysid *) 0;
  }
  else
  {
    /* wait for any of them */
    op->disable = FALSE;
    LeaveCriticalSection( &op->crit );
    WaitForMultipleObjectsEx( op->nwait, op->waitFor, 0,
			     delay/1000, alertable );
    EnterCriticalSection( &op->crit );
    op->disable = olddis;
  }
  LeaveCriticalSection( &op->crit );
}

boolean ssysAddWaitObject( dvoid *osdp, HANDLE h )
{
  ssysOsdInfo *op;
  int rv;

  op = (ssysOsdInfo *)osdp;

  EnterCriticalSection( &op->crit );

  if( op->nwait >= MAXIMUM_WAIT_OBJECTS )
    rv = FALSE;
  else
  {
    op->waitFor[ op->nwait++ ] = h;
    rv = TRUE;
  }
  LeaveCriticalSection( &op->crit );
  return rv;
}

void ssysRemWaitObject( dvoid *osdp, HANDLE h )
{
  ssysOsdInfo *op = (ssysOsdInfo *)osdp;
  DWORD	i, j;

  EnterCriticalSection( &op->crit );

  for( i = 0 ; i < op->nwait ; i++ )
  {
    if( op->waitFor[ i ] == h )
    {
      op->nwait--;
      for (j = i; j < op->nwait; j++)
	op->waitFor[ j ] = op->waitFor[ j + 1 ];
      break;
    }
  }

  LeaveCriticalSection( &op->crit );
}


/*
 * ssysIntrHndlr - SIGINT handler
 */
static void ssysIntrHndlr(int sig)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) ysGetOsdPtr();

  EnterCriticalSection( &op->crit );

  if (op->disable)
    op->pending = YS_EX_INTERRUPT;
  else
    ysIntr(YS_EX_INTERRUPT);

  LeaveCriticalSection( &op->crit );

  /* re-enable handling of signal */
  DISCARD signal( sig, ssysIntrHndlr );
}

/*
 * ssysTermHndlr - SIGTERM handler
 */
static void ssysTermHndlr(int sig)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) ysGetOsdPtr();

  EnterCriticalSection( &op->crit );

  if (op->disable)
    op->pending = YS_EX_SHUTDOWN;
  else
    ysIntr(YS_EX_SHUTDOWN);

  LeaveCriticalSection( &op->crit );

  DISCARD signal( sig, ssysTermHndlr );
}


/*
 * The routines below are implemented independent of any asynchronous
 * or polling model implementation choices.
 */

/*
 * syscPG - process-wide global pointer
 */
externdef dvoid *syscPG;

/*
 * sysmInit - memory initialization
 */
boolean sysmInit(dvoid *osdp, ysmaf *afp, ysmrf *rfp, ysmff *ffp,
		 ysbv **bv, sword *nbv)
{
  *afp = (ysmaf) malloc;
  *rfp = (ysmrf) realloc;
  *ffp = (ysmff) free;

  *bv = (ysbv *) 0;
  *nbv = 0;

  return TRUE;
}

boolean syspGetAffinity(dvoid *osdp, char *buf, size_t len)
{
  ssysOsdInfo *op = (ssysOsdInfo*)osdp;
  DWORD	procmask, sysmask;

  /* FIXME -- what about default aff?  should return empty string? */

  GetProcessAffinityMask( op->proc, &procmask, &sysmask );
  sprintf( buf, "%x", procmask );
  return TRUE;
}


/*
 * syspGetCpuTime - get CPU usage
 */
boolean syspGetCpuTime(dvoid *osdp, sysb8 *cputm)
{
  FILETIME ctime, extime, ktime, utime;
  sysb8 work;
  ssysOsdInfo *op = (ssysOsdInfo*)osdp;

  GetProcessTimes( op->proc, &ctime, &extime, &ktime, &utime );

  /* convert FILETIME to sysb8, and add */
  sysb8mak( cputm, ktime.dwHighDateTime, ktime.dwLowDateTime );
  sysb8mak( &work, utime.dwHighDateTime, utime.dwLowDateTime );
  sysb8add( cputm, cputm, &work );

  /* scale from nano seconds to milliseconds */
  sysb8ext( &work, 1000 );
  sysb8div( cputm, cputm, &work );

  return TRUE;
}

/*
 * syspGetMemUsage - get memory usage
 */
boolean syspGetMemUsage(dvoid *osdp, ub4 *kbytes)
{
  MEMORYSTATUS	ms;

  ms.dwLength = sizeof(ms);
  GlobalMemoryStatus( &ms );
  *kbytes = ms.dwTotalVirtual / 1024;

  return TRUE;
}

void syslPrint( const char *fmt, va_list ap )
{
  ssysOsdInfo *op;

  if ( !syscGetPG() )
    return;

  op = (ssysOsdInfo *) ysGetOsdPtr();

  if( !op->pfunc )
    vfprintf(stdout, fmt, ap);
  else
    op->pfunc( fmt, ap );
}

void syslError( const char *fmt, va_list ap )
{
  ssysOsdInfo *op;

  if ( !syscGetPG() )
    return;

  op = (ssysOsdInfo *) ysGetOsdPtr();

  if( !op->efunc )
    vfprintf(stderr, fmt, ap);
  else
    op->efunc( fmt, ap );
}


/*
 * syslConsole - print/log a message to the console
 */
void syslConsole(sword level, const char *fmt, va_list ap)
{
  char buf[1024];
  WORD etype;
  
  DISCARD ssyslOpenSysLog();
  
  ysFmtVaStrl( buf, sizeof(buf), fmt, ap );

  switch( level )
  {
  case YSLSEV_EMERG:
  case YSLSEV_ALERT:
  case YSLSEV_CRIT: 
  case YSLSEV_ERR:
    etype = EVENTLOG_ERROR_TYPE;
    break;
  case YSLSEV_WARNING:
    etype = EVENTLOG_WARNING_TYPE;
    break;
  case YSLSEV_NOTICE: 
  case YSLSEV_INFO:
  default:
    etype = EVENTLOG_INFORMATION_TYPE;
    break;
  }
  /* pass on our level+1 as the message id. */
  ssyslSysLog( etype, (WORD)0, (WORD)level + 1, buf);
}


/*
 * systmGetTicks - return tick count in microsecond
 */
ub4   systmGetTicks( void )
{
  /* Due to a bug in QueryPerformanceCounter() on HP NetServer,
   * such that it wraps in 1 second, we devised this workaround.
   */
  
  LARGE_INTEGER    curtime;
  ub4              now, now_msecs, diff_usecs, diff_msecs, low, high, secs;
  double           now_dbl;
  static ub4       usecs = 0, last, last_msecs;
  static double    freq, last_dbl = 0.0;
  static boolean   first_time = TRUE, wrapped = FALSE;

/* macro to convert LARGE_INTEGER to double */
#define largeint2double(li) \
    ((double)li.HighPart*4294967296.0 + (double)li.LowPart)  

  if (first_time)
  {
    if (QueryPerformanceFrequency(&curtime) == FALSE || curtime.HighPart)
      freq = 0.0;
    else
      freq = (double)curtime.LowPart/1000000.0;
  }

  /*
   * If hardware counter is unavailable, just use msec clock.
   * Unfortunate but it is the best we can do.
   */
  if (freq == 0.0)
  {
    now_msecs = GetTickCount(); 
    usecs = now_msecs*1000;
  }
  else
  {
    /* Get time now */
    QueryPerformanceCounter(&curtime);
    now_dbl  = largeint2double(curtime)/freq;
    now      = (ub4)now_dbl;
    
    /* If no timer wrap, just do the straightforward and fast approach */
    if (!wrapped)
    {
      if (now_dbl >= last_dbl)
      {
	/* No wrap occured yet */
	last       = now;
	last_dbl   = now_dbl;
	usecs = now;
      }
      else
      {
	/* It wrapped for the first time.  Do complicated case from now on */
	wrapped    = TRUE;
	usecs      = 1000000+now;
	last       = now;
	last_msecs = GetTickCount();
      }
    }
    else
    {
      /*
       * Wrapping timer case below
       */
    
      /* Compute usecs since last sample */
      if (now < last) diff_usecs = 1000000-(last-now); 
      else diff_usecs = now-last; 
    
      /*
       * Check for timer wrap.  Round down to 980 msecs to avoid boundary
       * cases since the msec timer is accurate only to 15 msecs.
       */
      now_msecs  = GetTickCount();
      diff_msecs = now_msecs-last_msecs;
      if (diff_msecs > 980)
      {
	secs        = diff_msecs/1000;
	diff_usecs += secs*1000000;

	/* Make sure boundary cases aren't throwing us off by 1 second */
	low  = (diff_msecs-500)*1000;
	high = (diff_msecs+500)*1000;
	if (diff_usecs < low) diff_usecs += 1000000;
	else if (diff_usecs > high) diff_usecs -= 1000000;
      }
    
      usecs     += diff_usecs;
      last       = now;
      last_msecs = now_msecs;
    }
  }
  return usecs;
}


/*
 * systmGetClock - return time of day 
 */
boolean systmGetClock(sysb8 *clock)
{
  sysb8ext(clock, (time((time_t *) 0)));
  return TRUE;
}

/*
 * systmClockToTime - convert clock value to structured form
 */
void    systmClockToTime(sysb8 *clock, ystm *sttm)
{
  struct tm *local;
  time_t     tpsec;

  tpsec = (time_t) sysb8msk(clock);
  local = localtime(&tpsec);

  sttm->sec = local->tm_sec;
  sttm->min = local->tm_min;
  sttm->hour = local->tm_hour;
  sttm->mday = local->tm_mday;
  sttm->mon = local->tm_mon + 1;
  sttm->year = local->tm_year + 1900;
}




/*
 * ssysiInit - ysInit wrapper. Calls ysInit only when ys Reference Count is 0.
 *
 * This routine is used to ensure that ysInit is only called once. 
 */                                                       
boolean ssysiInit(dvoid *osdp, CONST char *nm)
{
  if (ssysiRefCnt++ == 0)
    ysInit(osdp, nm);

  return TRUE;
}                    

/*
 * ssysiTerm - ysTerm wrapper. Calls ysTerm only when ys Reference Count is 0.
 */
void ssysiTerm(dvoid *osdp)
{   
  if (--ssysiRefCnt == 0)
    ysTerm(osdp);
}


/*
 * ssyslOpenSysLog - open an even log entry by registering a new subkey
 */
boolean ssyslOpenSysLog(void)
{
  HKEY hk;
  DWORD dwData; 
  char szBuf[MAX_PATH];
  char  subkey[MAX_PATH];
  int   len, type;

  static boolean opened = FALSE;

  if (opened)
    return TRUE;
  
  strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
  subkey[strlen(SSYSI_SVC_PARMS_PREFIX)-1] = '\0';
  len = sizeof(szBuf);

  if ((RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS) ||
      (RegQueryValueEx(hk, SSYSI_PRODUCT_HOME_KEY, NULL, &type, szBuf, &len)
	   != ERROR_SUCCESS))
  {
    /* take the default */
    strcpy( szBuf, SSYSI_DEF_PRODUCT_HOME );
  }
  RegCloseKey(hk);

  strcat(szBuf, SSYSI_EVENT_MSGFILE );

  /* 
   * Add your source name as a subkey under the Application 
   * key in the EventLog service portion of the registry. 
   */ 
  if (RegCreateKey(HKEY_LOCAL_MACHINE, SSYSI_EVENT_LOGKEY, &hk ))
    return FALSE;

  /* Add the Event ID message-file name to the subkey. */
  if (RegSetValueEx(hk,             /* subkey handle         */ 
          "EventMessageFile",       /* value name            */ 
          0,                        /* must be zero          */ 
          REG_EXPAND_SZ,            /* value type            */ 
          (LPBYTE) szBuf,           /* address of value data */ 
          strlen(szBuf) + 1))       /* length of value data  */ 
    return FALSE;

  /* Set the supported types flags. */ 
  dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE |
           EVENTLOG_INFORMATION_TYPE; 
  if (RegSetValueEx(hk,      /* subkey handle                */ 
          "TypesSupported",  /* value name                   */ 
          0,                 /* must be zero                 */ 
          REG_DWORD,         /* value type                   */ 
          (LPBYTE) &dwData,  /* address of value data        */ 
          sizeof(DWORD)))    /* length of value data         */ 
    return FALSE;

  RegCloseKey(hk);
  opened = TRUE;
  return TRUE;
}

/*
 * ssyslSysLog - log an event entry.
 */
void ssyslSysLog(WORD etype, WORD ecat, WORD eid, CONST char * buf)
{
  HANDLE h;
  LPCTSTR aszMsg[2];		/* array of string */

  ssyslOpenSysLog();

  /* use local computer, OMN source name */
  if( !(h = RegisterEventSource(NULL, SSYSI_EVTSRC)))
    return;

  aszMsg[0] = buf;
  if (!ReportEvent(h,					 /* event log handle */
                   etype,				       /* event type */
                   ecat,		   /* source specific event category */
                   eid,                                  /* event identifier */
                   NULL,		      /* no user security identifier */
                   1,				  /* one substitution string */
                   0,						  /* no data */
                   (LPCTSTR *) aszMsg,		  /* address of string array */
                   (LPVOID)NULL))                         /* address of data */
    exit(1);
 
  DeregisterEventSource(h);
  return;
}


STATICF BOOL sysiCtrlHandler( DWORD fdwCtrlType )
{
  if( fdwCtrlType == CTRL_C_EVENT )
    {
      ssysIntrHndlr( SIGINT );
      return TRUE;
    }
  return FALSE;
}

/* sysiGetPlatformId() - get platform id from windows version information
 *
 * returns: VER_PLATFORM_WIN32_NT (winnt) or VER_PLATFORM_WIN32_WINDOWS (win95)
 */
STATICF int sysiGetPlatformId(void)
{
  OSVERSIONINFO osverinfo;

  osverinfo.dwOSVersionInfoSize = sizeof (osverinfo);
  if (GetVersionEx(&osverinfo))
    return (osverinfo.dwPlatformId);
  else
    return 0;
}

/*
 * The following routines should actually be in MN, but that has problems:
 * The IIOP bridge does not access NIO and has only the YS context, and so it 
 * should call YS and YS should call MN. But YS is supposed to be the lowest 
 * layer. Do we allow YS to be dependent on MN? I would think not.
 */
static void ssysSelectThread( ssysExtNWEvent *extEvent );

int ssysAddFd( dvoid *osdp, ub4 fd, ub4 dummy)
{
  HANDLE          hThread;
  ssysOsdInfo     *op = (ssysOsdInfo *) osdp;
  ub4             i;
  DWORD           thrid;

  EnterCriticalSection( &op->crit );
  if (op->extEvents == NULL)
  {
    if ((op->extEvents = (ssysExtNWEvent *)malloc(sizeof(ssysExtNWEvent) * MAXIMUM_WAIT_OBJECTS)) == NULL)
    {
      LeaveCriticalSection( &op->crit );
      return 0;
    }
  }
  memset(op->extEvents, '\0', sizeof(ssysExtNWEvent)*MAXIMUM_WAIT_OBJECTS);

  for (i = 1; i < MAXIMUM_WAIT_OBJECTS; i++)
  {
    if (op->extEvents[i].s == (SOCKET)fd)
    {
      /* we already have an entry, just return the slot */
      LeaveCriticalSection( &op->crit );
      return i;
    }
    if (!op->extEvents[i].thrid && !op->extEvents[i].term)
      break;
  }
  if (i == MAXIMUM_WAIT_OBJECTS)
  {
    LeaveCriticalSection( &op->crit );
    return 0;
  }
  op->extEvents[i].s = (SOCKET)fd;
  op->extEvents[i].thrid = thrid;
  op->extEvents[i].selevt = CreateEvent( 0, TRUE, FALSE, 0);
  if (op->pauseevt == NULL)
  {
    op->pauseevt = CreateEvent( 0, FALSE, FALSE, 0);
    ssysAddWaitObject( op, op->pauseevt );
  }
  op->extEvents[i].osdp = op;
  LeaveCriticalSection( &op->crit );

  hThread = CreateThread( 0, 4096, 
                          (LPTHREAD_START_ROUTINE)ssysSelectThread,
                          (LPVOID)&op->extEvents[i], 0, &thrid );
  if (!hThread)
  {
    CloseHandle( op->extEvents[i].selevt );
    op->extEvents[i].thrid = 0;
    return 0;
  }

  return i;
}

void ssysRemFd( dvoid *osdp, int slot, ub4 dummy )
{
  ssysOsdInfo     *op = (ssysOsdInfo *) osdp;

  EnterCriticalSection( &op->crit );
  if (slot)
  {
    /* ask the thread to exit when it is ready */
    op->extEvents[slot].term = TRUE;
  }
  LeaveCriticalSection( &op->crit );
}

void ssysSetFdEvent( dvoid *osdp, int slot )
{
  ssysOsdInfo     *op = (ssysOsdInfo *) osdp;

  EnterCriticalSection( &op->crit );
  SetEvent( op->extEvents[slot].selevt );
  LeaveCriticalSection( &op->crit );
}

static void ssysSelectThread( ssysExtNWEvent *extEvent )
{
  fd_set          fdset;
  struct timeval  to;
  ssysOsdInfo     *op = extEvent->osdp;
  boolean         rdy = FALSE;
  int             cnt;

  while (TRUE)
  {
    FD_ZERO(&fdset);
    FD_SET(extEvent->s, &fdset);

    /* timeout period is 60 seconds */
    to.tv_sec = 60;
    to.tv_usec = 0;
  
    /* wait for timeout */
    cnt = select(0, &fdset, NULL, NULL, &to);
    if (cnt > 0)
    {
      rdy = TRUE;
      EnterCriticalSection( &op->crit );
      SetEvent( op->pauseevt );
      LeaveCriticalSection( &op->crit );
    }

    EnterCriticalSection( &op->crit );
    if (extEvent->term)
    {
      extEvent->s     = 0;
      extEvent->thrid = 0;
      extEvent->term  = 0;
      CloseHandle( extEvent->selevt );
    }
    LeaveCriticalSection( &op->crit );
  }
}
