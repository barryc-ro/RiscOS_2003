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
 * implements its blocking using the poll() mechanism, which means that
 * all network drivers must register their open file descriptors to be
 * considered.  This is done through the two routines:  ssysAddFd() and
 * ssysRemFd().
 *
 * The first half of this file contains routines that are affected by
 * these decisions.  The second half of this file contains the miscellaneous
 * routines that are not affected by these decisions.
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <limits.h>
#include <syslog.h>
#include <stdio.h>
#include <unistd.h>

/*
 * The routines below are implemented in consideration of choices made about
 * how to deal with asynchronous events and waiting.
 */

/*
 * Private Declarations
 */
typedef struct ssysFdInfo ssysFdInfo;

struct ssysFdInfo
{
    int	    fd;			/* -1 means not in use */
    sword   icnt;		/* number of input registrations */
    sword   ocnt;		/* number of output registrations */
};

typedef struct ssysOsdInfo ssysOsdInfo;

struct ssysOsdInfo
{
  boolean        disable;                 /* true if interrupts are disabled */
  CONST char    *pending;                          /* ysIntr() value pending */
  ub4            openmax;         /* maximum number of open file descriptors */
  fd_set         rfds;
  fd_set         wfds;
  ub4		 lreg;                                 /* last reg in fdinfo */
  ub4		 nregs;                             /* active regs in fdinfo */
  ssysFdInfo	*fdinfo;                          /* info about pollable fds */
};

static void ssysIntrHndlr(void);
static void ssysTermHndlr(void);

externdef ysidDecl(SYSI_EX_BAD_OSDPTR_SIZE) = "::sysi::Bad_SYSX_OSDPTR_SIZE";

/*
 * sysiInit - OSD layer initialization
 */
boolean sysiInit(dvoid *osdp)
{
  struct sigaction act;
  struct rlimit    rlp;
  ssysOsdInfo *op;
  ub4 i;

  /* for safety, in case we add stuff here but not in sysx.h */
  if( sizeof(*op) > SYSX_OSDPTR_SIZE )
    ysePanic( SYSI_EX_BAD_OSDPTR_SIZE );

  op = (ssysOsdInfo *) osdp;

  op->disable = FALSE;
  op->pending = (ysid *) 0;

  /* trap signals */
  DISCARD memset(&act, 0, sizeof(act));
  act.sa_handler = ssysIntrHndlr;
  DISCARD sigaction(SIGINT, &act, (struct sigaction *) 0);
  act.sa_handler = ssysTermHndlr;
  DISCARD sigaction(SIGTERM, &act, (struct sigaction *) 0);

  /* initialize poll() descriptor */
  getrlimit(RLIMIT_NOFILE, &rlp);
  op->openmax = (ub4) rlp.rlim_cur;

  op->fdinfo = (ssysFdInfo*) malloc(sizeof(ssysFdInfo) * op->openmax);

  for( i = 0; i < op->openmax ; i++ )
  {
    op->fdinfo[i].fd = -1;
    op->fdinfo[i].icnt = op->fdinfo[i].ocnt = 0;
  }
  op->nregs = op->lreg = 0;
  
  return TRUE;
}

/*
 * sysiTerm - OSD layer termination
 */
void sysiTerm(dvoid *osdp)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;

  /* release poll() descriptor */
  free((dvoid *) op->fdinfo); 
}

/*
 * syslGetc - get a character from the console
 */
boolean syslGetc(dvoid *osdp, sword *ch, ub4 delay)
{
  int          sts, slot, fd;
  ub1          buf;
  boolean      eofseen;
  ssysOsdInfo *op;

  /* since we are not using stdio, it won't flush data written using
   * yslPrint() here.  We need to do it manually.
   */
  DISCARD fflush(stdout);

  op = (ssysOsdInfo *) osdp;
  fd = fileno(stdin);
  slot = ssysAddFd(osdp, fd, SSYS_FD_INPUT );
  sysiPause(osdp, sysiNoMask, delay);

  if ( FD_ISSET(fd, &op->rfds) )
    {
      /* cannot use stdio(); must do a non-buffering read here */
      eofseen = (read(fd, &buf, 1) != 1);
      if (!eofseen)
	*ch = buf;
    }
  else
    {
      *ch = '\0';
      eofseen = FALSE;
    }

  ssysRemFd(osdp, slot, SSYS_FD_INPUT);
  return eofseen;
}

/*
 * sysiNoMask - definition
 */
sysimsk sysiNoMask;

/*
 * sysiDisable - disable signal handler
 */
sysimsk sysiDisable(dvoid *osdp)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;
  op->disable = TRUE;
  return sysiNoMask;
}

/*
 * sysiEnable - enable signal handler
 */
void sysiEnable(dvoid *osdp, sysimsk mask)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) osdp;
  if (op->disable && op->pending)
    {
      ysIntr(op->pending);
      op->pending = (ysid *) 0;
    }

  op->disable = FALSE;
}

/*
 * sysiPause - pause for a number of microseconds
 */
void sysiPause(dvoid *osdp, sysimsk mask, ub4 delay)
{
  sword          hifd;
  ssysOsdInfo   *op;
  ssysFdInfo    *fp, *ep;
  boolean        olddis;
  struct timeval tv;
  fd_set         dummy;

  op = (ssysOsdInfo *) osdp;
  olddis = op->disable;

  if (op->disable && op->pending)
    {
      ysIntr(op->pending);
      op->pending = (ysid *) 0;
    }
  else
    {
      /* We are vulnerable to delaying a SIGINT or SIGTERM from the time we
       * check our pending flag above to the time we enter into the poll()
       * routine below.  If we were also vulnerable to delaying inbound I/O
       * because we were using ytnPush(), we would probably want to place an
       * upper bound on delay of say, a half-second, causing us to slow-spin.
       */
      op->disable = FALSE;

      tv.tv_sec  = delay / 1000000;
      tv.tv_usec = delay % 1000000;

      FD_ZERO(&op->rfds);
      FD_ZERO(&op->wfds);
      FD_ZERO(&dummy);
      
      for (fp = op->fdinfo, ep = fp + op->lreg, hifd = 0; fp < ep; fp++)
	if (fp->fd != -1)
	{
	  if (fp->fd > hifd) hifd = fp->fd;
	  if (fp->icnt > 0) FD_SET(fp->fd, &op->rfds);
	  if (fp->ocnt > 0) FD_SET(fp->fd, &op->wfds);
	}	

      DISCARD select(hifd+1, &op->rfds, &op->wfds, &dummy, &tv);

      if( op->pending )
      {
	ysIntr( op->pending );
	op->pending = (ysid *) 0;
      }

      op->disable = olddis;
    }
}

/*
 * ssysIntrHndlr - SIGINT handler
 */
static void ssysIntrHndlr(void)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) ysGetOsdPtr();

  if (op->disable)
    op->pending = YS_EX_INTERRUPT;
  else
    ysIntr(YS_EX_INTERRUPT);
}

/*
 * ssysTermHndlr - SIGTERM handler
 */
static void ssysTermHndlr(void)
{
  ssysOsdInfo *op;

  op = (ssysOsdInfo *) ysGetOsdPtr();
  if (op->disable)
    op->pending = YS_EX_SHUTDOWN;
  else
    ysIntr(YS_EX_SHUTDOWN);
}

/*
 * ssysAddFd - add a file descriptor
 */
int ssysAddFd(dvoid *osdp, int fd, ssysFdFlags type )
{
  ssysOsdInfo *op = (ssysOsdInfo *) osdp;
  ub4	i, j, k;
  ssysFdInfo	*fi;

  /* if no room, give up immediately */
  if( !type || op->lreg == op->openmax && op->nregs == op->openmax )
    return -1;

  /* search fdinfo for the fd, and the first unused slot */
  for( k = -1, i = 0; i < op->lreg && op->fdinfo[i].fd != fd; i++)
    if(op->fdinfo[i].fd < 0 )
      k = i;			/* remember first unused slot */

  if( op->fdinfo[i].fd < 0 )    /* new fd */
  {
    if( k != -1 )			/* have unused slot? */
      i = k;			/* ...use it */
    else if( op->lreg < op->openmax )
      op->lreg++;		/* adding another. */
    else
      return -1;

    op->nregs++;
  }
  fi = &op->fdinfo[i];
  fi->fd = fd;
	
  /* set ref counts */
  if (bit(type, SSYS_FD_INPUT))  (fi->icnt)++;
  if (bit(type, SSYS_FD_OUTPUT)) (fi->ocnt)++;

  return( i );
}

/*
 * ssysRemFd - remove a file descriptor
 */
void ssysRemFd(dvoid *osdp, int slot, ssysFdFlags type )
{
  ssysOsdInfo	*op =(ssysOsdInfo *) osdp;
  ssysFdInfo	*fi = &op->fdinfo[slot];
  
  if( slot < 0 )		/* in case he didn't check return value */
    return;


  /* adj ref cnts, note need to update. */
  if (bit(type, SSYS_FD_INPUT)) --(fi->icnt);
  if (bit(type, SSYS_FD_OUTPUT)) --(fi->ocnt);
  if (!fi->icnt && !fi->ocnt)
  {
    fi->fd = -1;
    op->nregs--;
  }

  /* crunch out trailing unused regs */
  while( op->lreg && op->fdinfo[op->lreg - 1].fd == -1 )
    op->lreg--;
}

/* 
 * ssysGetFds - return allocated array of known input fds.
 */
void ssysGetFds( ub4 *nfds, ssysRegFd **fds )
{
  ub4 i;
  ssysOsdInfo *op;
  ssysFdInfo *fp;
  ssysRegFd *ofds;

  op = (ssysOsdInfo *)ysGetOsdPtr();
  
  if( !(*nfds = op->nregs) )
    *fds = (ssysRegFd*)0;
  else
  {
    ofds = *fds = (ssysRegFd*)ysmGlbAlloc(*nfds * sizeof(**fds), "ssysGetFds");
    for ( i = 0; i < op->lreg ; i++ )
    {
      fp = &op->fdinfo[i];
      if (fp->fd != -1)
      {
	ofds->fd = fp->fd;
	ofds->op = 0;
	if (fp->icnt > 0) bis(ofds->op, SSYS_FD_INPUT);
	if (fp->ocnt > 0) bis(ofds->op, SSYS_FD_OUTPUT);
	ofds++;
      }
    }
  }
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

/*
 * syspGetCpuTime - get CPU usage
 */
boolean syspGetCpuTime(dvoid *osdp, sysb8 *cputm)
{
  struct tms buf;
  sysb8 tmp;
  clock_t tickspersec = CLK_TCK;

  times(&buf);
  sysb8ext(cputm, buf.tms_utime + buf.tms_stime);
  sysb8mulb4(cputm, cputm, 1000000);
  sysb8ext(&tmp, tickspersec);
  sysb8div(cputm, cputm, &tmp);
  return TRUE;
}

/*
 * syspGetMemUsage - get memory usage
 */
boolean syspGetMemUsage(dvoid *osdp, ub4 *kbytes)
{
  extern end;

  *kbytes = ((((caddr_t) sbrk(0)) - (caddr_t) &end + 1023) / 1024);
  return TRUE;
}

/*
 * syslConsole - print/log a message to the console
 */
void syslConsole(sword level, const char *fmt, va_list ap)
{
  static boolean log_opened = FALSE;
  char buf[1024];

  if (!log_opened)
    openlog(ysProgName(), LOG_PID, LOG_USER);

  vsprintf(buf, fmt, ap);
  syslog((level <= LOG_DEBUG ? level : LOG_DEBUG), buf);
}

/*
 * systmGetTicks - get high-resolution clock.
 */
ub4 systmGetTicks(void)
{
  struct timeval  tp;
  struct timezone tzp;
 
  gettimeofday(&tp, &tzp);
  return ((ub4) tp.tv_sec) * 1000000 + ((ub4) tp.tv_usec);
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
void systmClockToTime(sysb8 *clock, ystm *tm)
{
  struct tm *local;
  time_t     tpsec;

  tpsec = (time_t) sysb8msk(clock);
  local = localtime(&tpsec);

  tm->sec = local->tm_sec;
  tm->min = local->tm_min;
  tm->hour = local->tm_hour;
  tm->mday = local->tm_mday;
  tm->mon = local->tm_mon + 1;
  tm->year = local->tm_year + 1900;
}

