/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    systhr.c
  DESCRIPTION
    systhr.c - OMN system-dependent threading definitions
    
    This version implements the following named thread packages

    NATIVE	-- a cooperative version of Win32 threads,
		   where only one thread is allowed to execute at a time.
		   This is necessary for use with MN 3.2.

    NATIVEP	-- a version using Win32 threads in pre-emptive mode;
		   This cannot be used with Media Net transport, which
		   is not re-entrant.

    OMS		-- a cooperative version using the setjmp/longjmp flavor
		   of threading implemented by yst.

    OMSRT	-- another cooperative version, favored by the video
		   server folks.  UNTESTED!!!

  PUBLIC FUNCTIONS

    none -- this defines a descriptor table, which exports
            packages of thread-system wrappers for use by ysthr.c

  PRIVATE FUNCTIONS
    many...

  NOTES

    The cooperative version serializes the running thread by using a "conch"
    mutex (cf _Lord of the Flies_).  Suspension is handled by having an
    event for each thread which is set to force the resumption.  The package
    returns thread-ids which are the system thread-id.  This id is used to
    key a hash table of known threads.  This table may be populated with
    threads we started, or other threads we have discovered.  The entries
    in the table keep the event we use for suspension, and a flag indicating
    whether the thread is now cooperating with the conch discipline.

    The pre-emptive version is exactly the same as the cooperative version,
    using many of the "cop" functions unchanged.  The diference is in
    initialization, which leaves the package set up to skip the use of the
    "conch" is not used to force a single running thread.  We don't use the
    native Suspend/Resume mechanism because that would oblige us to hand out
    thread handles, which can be tricky and leak-prone to deal with.  If a
    way can be found to get a thread handle given an id, this can be
    re-examined.

    The OMS and OMSRT versions are unchanged from those on UNIX, as they
    ought to be generic.

  MODIFIED   (MM/DD/YY)
    dbrower   10/ 3/96 -  updated to working state on NATIVE (default)
			  cooperative threads and NATIVEP preemptive threads.
*/

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSTHR_ORACLE
#include <systhr.h>
#endif

/*
 * Descriptor Declarations
 *
 * Add a descriptor declaration here every time you need to add a new entry
 * to the table.
 */
static CONST_DATA systhrop systhr_cnt;
static CONST_DATA systhrop systhr_pnt;
static CONST_DATA systhrop systhr_yst;
static CONST_DATA systhrop systhr_ystrt;

/*
 * Thread Package Table
 *
 * Add the new address of the driver descriptor to the table below.  The
 * first entry is chosen as the default for the platform.
 */
externdef CONST_DATA systhrop *CONST_W_PTR systhrtab[] =
{
  &systhr_cnt,
  &systhr_pnt,
  &systhr_yst,
  &systhr_ystrt
};

/*
 * SYSTHRTAB_MAX - the number of entries in the thread package table
 */
externdef CONST_DATA sword SYSTHRTAB_MAX
  = sizeof(systhrtab) / sizeof(systhrop *);


/*********************** NATIVE COOPERATIVE IMPLEMENATION ********************/


#include <yshsh.h>

struct systcopArg
{
  void   (*start)(dvoid *);
  dvoid   *arg;
  DWORD    tid;                              /* thread id, key in hash table */
  HANDLE   sevt;                                            /* suspend event */
  boolean   conched;                                            /* has conch */
};

typedef struct systcopArg systcopArg;

STATICF sword systcopCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systcopLaunch(void *arg);
STATICF sword systcopThrEq(dvoid *elm, dvoid *key, size_t keysz);
STATICF void systcopExit(void);
STATICF void systcopYield(void);
STATICF void systcopSuspend(void);
STATICF void systcopResume(ub1 *td);
STATICF void systcopSelf(ub1 *td);
STATICF void systcopPrint(ub1 *td, char *buf, size_t len);

STATICF systcopArg *systcopFind( void );
STATICF void systnInit( boolean coop );


static CONST_DATA systhrop systhr_cnt =
{
  "NATIVE", sizeof(DWORD),
  systcopCreate, systcopExit,
  systcopYield,
  systcopSuspend, systcopResume,
  systcopSelf, systcopPrint
};

/* static, global variables for this thread wrapper. */

static boolean systcopInit = FALSE;           /* true if package initialized */
static CRITICAL_SECTION systcopCrit;      /* critical section for hash table */
static yshsh  *systcopThrTbl;                 /* global hashed threads table */
static sword   systcopThrCnt;              /* number of threads created here */
static HANDLE systcopConch;                    /* thread serialization mutex */

STATICF void systnInit( boolean coop )
{
  systcopArg *larg;

  if (systcopInit)
    return;

  systcopInit = TRUE;
  systcopThrCnt = 0;
  InitializeCriticalSection( &systcopCrit );
  systcopThrTbl = ysHshCreate((ub4) 32, ysHshKey, systcopThrEq, free);

  /* make self (initial thread) be known */
  larg = systcopFind();
  /* make the conch, owned by the initial thread. */
  if( coop )
  {
    systcopConch = CreateMutex( 0, 1, 0 );
    larg->conched = TRUE;
  }
}

STATICF sword systcopCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  systcopArg *larg;
  HANDLE h;
  sword rv;

  /* if we have never before been called, then initialize our state */
  if (!systcopInit)
    systnInit( TRUE );

  /* now create the child thread descriptor */
  larg = (systcopArg *) malloc(sizeof(systcopArg));
  larg->start = start;
  larg->arg = arg;
  larg->sevt = CreateEvent(0, 0, 1, 0);
  larg->conched = FALSE;

  /* create the child thread, save the tid */
  h = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) systcopLaunch,
		   (LPVOID)larg, 0, &larg->tid );

  memcpy( (dvoid*)td, (dvoid*)&larg->tid, sizeof(larg->tid) );

  rv = h ? 0 : 1;
  CloseHandle( h );
  return( rv );
}

STATICF void systcopLaunch(void *arg)
{
  systcopArg *larg = (systcopArg *) arg;

  if( systcopConch )
  {
    WaitForSingleObject( systcopConch, INFINITE );
    larg->conched = TRUE;
  }
  systcopThrCnt++;
  EnterCriticalSection(&systcopCrit);
  ysHshIns(systcopThrTbl, &larg->tid, sizeof(larg->tid), (dvoid *) larg);
  LeaveCriticalSection(&systcopCrit);
  larg->start(larg->arg);
  systcopExit();
}

STATICF sword systcopThrEq(dvoid *elm, dvoid *key, size_t keysz)
{
  DWORD *tid = (DWORD*)key;
  systcopArg *larg = (systcopArg *) elm;

  return (larg->tid - *tid);
}

STATICF void systcopExit(void)
{
  systcopArg *larg;
  DWORD    tid;

  /* remove the entry from the hash table */
  tid = GetCurrentThreadId();
  EnterCriticalSection(&systcopCrit);
  larg = (systcopArg *) ysHshRem(systcopThrTbl, &tid, sizeof(tid));
  LeaveCriticalSection(&systcopCrit);
  if (larg)
    {
      /* if this thread is one we started, decrement the thread count;
       * if the thread count goes to zero, then blow away the hash table
       */
      if (larg->start)
	{
	  CloseHandle( larg->sevt );
	  systcopThrCnt--;
	  if (!systcopThrCnt)
	    {
	      EnterCriticalSection(&systcopCrit);
	      ysHshDestroy(systcopThrTbl);
	      LeaveCriticalSection(&systcopCrit);
	      systcopInit = FALSE;
	    }
	}
      free((dvoid *) larg);
    }

  if( larg->conched && systcopConch )
    ReleaseMutex( systcopConch );
  ExitThread(0);
}

STATICF void systcopYield(void)
{
  systcopArg *larg;

  if (!systcopInit)
    systnInit( TRUE );

  larg = systcopFind();

  if( larg->conched && systcopConch )
    ReleaseMutex( systcopConch );
  Sleep(0);
  if( systcopConch )
  {
    WaitForSingleObject( systcopConch, INFINITE );
    larg->conched = TRUE;
  }
}

STATICF void systcopSuspend(void)
{
  systcopArg *larg;

  if (!systcopInit)
    systnInit( TRUE );

  larg = systcopFind();

  if( larg->conched && systcopConch )
    ReleaseMutex( systcopConch );

  /* wait for my event */ 
  WaitForSingleObject( larg->sevt, INFINITE );
  ResetEvent( larg->sevt );

  /* now grab the conch */
  if( systcopConch )
  {
    WaitForSingleObject( systcopConch, INFINITE );
    larg->conched = TRUE;
  }
}

STATICF void systcopResume(ub1 *td)
{
  systcopArg *larg;

  if (!systcopInit)
    systnInit( TRUE );

  EnterCriticalSection(&systcopCrit);
  larg = (systcopArg *) ysHshFind(systcopThrTbl, td, sizeof(DWORD));
  LeaveCriticalSection(&systcopCrit);

  if (!larg)
    ysePanic(YS_EX_FAILURE);
  SetEvent( larg->sevt );
}

STATICF systcopArg *systcopFind( void )
{
  systcopArg *larg;
  DWORD    tid;

  tid = GetCurrentThreadId();
  EnterCriticalSection(&systcopCrit);
  larg = (systcopArg *) ysHshFind(systcopThrTbl, &tid, sizeof(tid));
  LeaveCriticalSection(&systcopCrit);
  if (!larg)
    {
      /* new thread we are just learning about */
      larg = (systcopArg *) malloc(sizeof(systcopArg));
      larg->start = (void (*)(void *)) 0;
      larg->tid = GetCurrentThreadId();
      larg->sevt = CreateEvent(0, 0, 1, 0);
      larg->conched = FALSE;
      EnterCriticalSection(&systcopCrit);
      ysHshIns(systcopThrTbl, &larg->tid, sizeof(larg->tid), (dvoid *) larg);
      LeaveCriticalSection(&systcopCrit);
    }
  return larg;
}


STATICF void systcopSelf(ub1 *td)
{
  DWORD tid = GetCurrentThreadId();

  if (!systcopInit)
    systnInit( TRUE );
  DISCARD systcopFind();
  memcpy( (dvoid*)td, (dvoid*)&tid, sizeof(tid) );
}


STATICF void systcopPrint(ub1 *td, char *buf, size_t len)
{
  DISCARD sprintf(buf, "%d", *(DWORD*)td);
}

/***************************** NATIVE IMPLEMENATION**************************
 * The following implementation is known to be pre-emptive.  For OMN3, which
 * supports only cooperative threading, we insist on the cooperative version
 * which precedes this one.
 *
 * This one is not as neat as one might imagine, because we need to provide
 * stable thread ids, in the ThreadId range back, and need to be able to
 * suspend/resume by that id.  Usinge Win32 thread primitives, we'd need
 * to have thread handles to give to Suspend/ResumeThread, but those are
 * hard to keep around, particularly if the id was returned from a call
 * to Self(), and the caller never knows to release the handle.  So instead
 * we piggyback on the cooperative model, which already has a hash table
 * on thread-ids, and uses native events in the per-thread structure to
 * perform suspend/resume.  Sigh.  If only we could easily get a handle
 * from a thread-id...
 */

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systntvYield(void);
STATICF void systntvSuspend(void);

static CONST_DATA systhrop systhr_pnt =
{
  "NATIVEP", sizeof(DWORD),
  systntvCreate, systcopExit, systntvYield,
  systntvSuspend, systcopResume,
  systcopSelf, systcopPrint
};

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  /* if we have never before been called, then initialize our state */
  if (!systcopInit)
    systnInit( FALSE );

  return systcopCreate( td, start, arg, stksz );
}


STATICF void systntvYield(void)
{
  if (!systcopInit)
    systnInit( FALSE );

  systcopYield();
}


STATICF void systntvSuspend(void)
{
  if (!systcopInit)
    systnInit( FALSE );

  systcopSuspend();
}


/****************************** OMS IMPLEMENTATION ***************************/

#include <yst.h>

STATICF sword systystCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systystExit(void);
STATICF void systystYield(void);
STATICF void systystSuspend(void);
STATICF void systystResume(ub1 *td);
STATICF void systystSelf(ub1 *td);
STATICF void systystPrint(ub1 *td, char *buf, size_t len);

static CONST_DATA systhrop systhr_yst =
{
  "OMS", sizeof(ystThd *),
  systystCreate, systystExit, systystYield, systystSuspend, systystResume,
  systystSelf, systystPrint
};

STATICF sword systystCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  ystThd *tid;

  tid = ystCreate(start, arg, (stksz ? stksz : (ub4) 16384));
  if (tid)
    {
      *((ystThd **) td) = tid;
      return 0;
    }
  else
    return -1;
}

STATICF void systystExit(void)
{
  DISCARD ystExit();
}

STATICF void systystYield(void)
{
  ystYield((ystThd *) 0);
}

STATICF void systystSuspend(void)
{
  ystSuspend((ystThd *) 0);
}

STATICF void systystResume(ub1 *td)
{
  ystResume(*((ystThd **) td));
}

STATICF void systystSelf(ub1 *td)
{
  *((ystThd **) td) = ystSelf();
}

STATICF void systystPrint(ub1 *td, char *buf, size_t len)
{
  ystPrint(*((ystThd **) td), buf, len);
}


/************************ OMS REAL TIME IMPLEMENTATION ***********************/

static ystThd *systrtIdle = (ystThd *)NULL;
static ystThd *systrtLast = (ystThd *)NULL;

STATICF sword systrtCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systrtExit(void);
STATICF void systrtYield(void);
STATICF void systrtSuspend(void);
STATICF void systrtResume(ub1 *td);
STATICF void systrtSelf(ub1 *td);
STATICF void systrtPrint(ub1 *td, char *buf, size_t len);

static CONST_DATA systhrop systhr_ystrt =
{
  "OMSRT", sizeof(ystThd *),
  systrtCreate, systrtExit, systrtYield, systrtSuspend, systrtResume,
  systrtSelf, systrtPrint
};

STATICF sword systrtCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  ystThd *tid;

  /* This should only get called once */
  if (!systrtIdle)
  {
    tid = ystCreate(start, arg, (stksz ? stksz : (ub4) 16384));
    if (tid)
    {
      *((ystThd **) td) = tid;
      systrtIdle = tid;
      return 0;
    }
  }
  return -1;
}

STATICF void systrtExit(void)
{
  DISCARD ystExit();
}

/*
 * This proceedure allows a user to keep ysYield() from changing to another
 * thread besides the idler.  If we are in a user thread and ysYield() is 
 * called we will switch to the idler thread.  If we are in the idler thread
 * and ysYield() is called we will switch back to the thread that called 
 * ysYield() or the next thread in sequence.  We switch to the next thread
 * in sequence if the idler thread was executed as a result of ystYield().
 */

STATICF void systrtYield(void)
{
  ystThd  *self;

  self = ystSelf();

  if (self == systrtIdle)
    ystYield(systrtLast);
  else
  {
    /* save the caller's ID */
    systrtLast = self;

    /* switch to idle thread */
    ystYield(systrtIdle);

    /* clear last thread in case the idler gets called from ystYield() */
    systrtLast = (ystThd *)NULL;
  }
}

STATICF void systrtSuspend(void)
{
  ystSuspend((ystThd *) 0);
}

STATICF void systrtResume(ub1 *td)
{
  ystResume(*((ystThd **) td));
}

STATICF void systrtSelf(ub1 *td)
{
  *((ystThd **) td) = ystSelf();
}

STATICF void systrtPrint(ub1 *td, char *buf, size_t len)
{
  ystPrint(*((ystThd **) td), buf, len);
}


