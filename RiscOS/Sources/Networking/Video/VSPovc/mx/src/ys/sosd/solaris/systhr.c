/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    systhr.c
  DESCRIPTION
    systhr.c - OMN system-dependent threading definitions
  PUBLIC FUNCTIONS
    none -- exports a table.
  PRIVATE FUNCTIONS
    <x>
  MODIFIED   (MM/DD/YY)
    dbrower   10/16/96 -  bug 412761, unaligned access to tds.
*/

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSTHR_ORACLE
#include <systhr.h>
#endif

#ifndef ERRNO_ORACLE
#include <errno.h>
#endif

/*
 * Descriptor Declarations
 *
 * Add a descriptor declaration here every time you need to add a new entry
 * to the table.
 */
static CONST_DATA systhrop systhr_csol;
static CONST_DATA systhrop systhr_sol;
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
  &systhr_csol,
  &systhr_sol,
  &systhr_yst,
  &systhr_ystrt
};

/*
 * SYSTHRTAB_MAX - the number of entries in the thread package table
 */
externdef CONST_DATA sword SYSTHRTAB_MAX
  = sizeof(systhrtab) / sizeof(systhrop *);

/***************************** NATIVE IMPLEMENATION**************************
 * The following implementation is known to be pre-emptive.  For OMN3, which
 * supports only cooperative threading, we insist on the cooperative version
 * following this one.
 */

#include <thread.h>
#include <synch.h>

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systntvExit(void);
STATICF void systntvSuspend(void);
STATICF void systntvResume(ub1 *td);
STATICF void systntvSelf(ub1 *td);
STATICF void systntvPrint(ub1 *td, char *buf, size_t len);

static CONST_DATA systhrop systhr_sol =
{
  "NATIVEP", sizeof(thread_t),
  systntvCreate, systntvExit, thr_yield, systntvSuspend, systntvResume,
  systntvSelf, systntvPrint
};

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  thread_t tid;
  sword rv;
  rv = thr_create((void *) 0, stksz, (void *(*)(void *)) start, arg,
		  THR_DETACHED, &tid);
  DISCARD memcpy( (dvoid*)td, (dvoid*)&tid, sizeof(tid) );
  return rv;
}

STATICF void systntvExit(void)
{
  thr_exit((dvoid *) 0);
}

STATICF void systntvSuspend(void)
{
  DISCARD thr_suspend(thr_self());
}

STATICF void systntvResume(ub1 *td)
{
  thread_t tid;
  DISCARD memcpy( (dvoid*)&tid, (dvoid*)td, sizeof(tid) );
  DISCARD thr_continue( tid );
}

STATICF void systntvSelf(ub1 *td)
{
  thread_t tid = thr_self();
  DISCARD memcpy( (dvoid*)td, (dvoid*)&tid, sizeof(tid) );
}

STATICF void systntvPrint(ub1 *td, char *buf, size_t len)
{
  thread_t tid;
  DISCARD memcpy( (dvoid*)&tid, (dvoid*)td, sizeof(tid) );
  DISCARD sprintf(buf, "%lu", tid );
}

/*********************** NATIVE COOPERATIVE IMPLEMENATION ********************/

#include <yshsh.h>

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
STATICF systcopArg *systcopAdopt( thread_t *tid );

static CONST_DATA systhrop systhr_csol =
{
  "NATIVE", sizeof(thread_t),
  systcopCreate, systcopExit, systcopYield, systcopSuspend, systcopResume,
  systcopSelf, systntvPrint
};

static boolean systcopInit = FALSE;           /* true if package initialized */
static sword   systcopThrCnt;              /* number of threads created here */
static mutex_t systcopConch;                         /* global package mutex */
static yshsh  *systcopThrTbl;                 /* global hashed threads table */

struct systcopArg
{
  void   (*start)(dvoid *);
  dvoid   *arg;
  thread_t tid;
  cond_t   cv;
};

STATICF sword systcopCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  systcopArg *larg;
  sword rv;
  thread_t tid = 0;

  /* if we have never before been called, then initialize our state */
  if (!systcopInit)
    {
      systcopInit = TRUE;
      systcopThrCnt = 0;
      mutex_init(&systcopConch, USYNC_THREAD, (void *) 0);
      mutex_lock(&systcopConch);
      systcopThrTbl = ysHshCreate((ub4) 32, ysHshKey, systcopThrEq, free);
    }

  /* now create the child thread descriptor */
  larg = (systcopArg *) malloc(sizeof(systcopArg));
  larg->start = start;
  larg->arg = arg;
  cond_init(&larg->cv, USYNC_THREAD, 0);

  /* create the child thread */
  systcopThrCnt++;
  rv =  thr_create((void *) 0, stksz, (void *(*)(void *)) systcopLaunch,
		    larg, THR_DETACHED, &tid );
  DISCARD memcpy( (dvoid*)td, (dvoid*)&tid, sizeof(tid) );
  return rv;
}

STATICF void systcopLaunch(void *arg)
{
  systcopArg *larg = (systcopArg *) arg;

  mutex_lock(&systcopConch);
  larg->tid = thr_self();
  ysHshIns(systcopThrTbl, &larg->tid, sizeof(larg->tid), (dvoid *) larg);
  larg->start(larg->arg);
  systcopExit();
}

STATICF sword systcopThrEq(dvoid *elm, dvoid *key, size_t keysz)
{
  thread_t *tid = (thread_t *) key;
  systcopArg *larg = (systcopArg *) elm;

  return (larg->tid - *tid);
}

STATICF void systcopExit(void)
{
  systcopArg *larg;
  thread_t    tid;

  /* remove the entry from the hash table */
  tid = thr_self();
  larg = (systcopArg *) ysHshRem(systcopThrTbl, &tid, sizeof(tid));
  if (larg)
    {
      /* if this thread is one we started, decrement the thread count;
       * if the thread count goes to zero, then blow away the hash table
       */
      if (larg->start)
	{
	  systcopThrCnt--;
	  if (!systcopThrCnt)
	    {
	      ysHshDestroy(systcopThrTbl);
	      systcopInit = FALSE;
	    }
	}
      free((dvoid *) larg);
    }

  mutex_unlock(&systcopConch);
  thr_exit((dvoid *) 0);
}

STATICF void systcopYield(void)
{
  mutex_unlock(&systcopConch);
  thr_yield();
  mutex_lock(&systcopConch);
}

STATICF void systcopSuspend(void)
{
  thread_t tid;
  systcopArg *larg = systcopAdopt( &tid );

  cond_wait(&larg->cv, &systcopConch);
}

STATICF void systcopResume(ub1 *td)
{
  systcopArg *larg;

  larg = (systcopArg *) ysHshFind(systcopThrTbl, td, sizeof(thread_t));
  if (!larg)
    ysePanic(YS_EX_FAILURE);
  cond_signal(&larg->cv);
}

STATICF void systcopSelf(ub1 *td)
{
  thread_t tid;
  DISCARD systcopAdopt( &tid );
  DISCARD memcpy( (dvoid*)td, (dvoid*)&tid, sizeof(tid) );
}

STATICF systcopArg *systcopAdopt( thread_t *tid )
{
  systcopArg *larg;

  if (!systcopInit)
    {
      systcopInit = TRUE;
      systcopThrCnt = 0;
      mutex_init(&systcopConch, USYNC_THREAD, (void *) 0);
      mutex_lock(&systcopConch);
      systcopThrTbl = ysHshCreate((ub4) 32, ysHshKey, systcopThrEq, free);
    }

  *tid = thr_self();
  larg = (systcopArg *) ysHshFind(systcopThrTbl, tid, sizeof(tid));
  if (!larg)
    {
      larg = (systcopArg *) malloc(sizeof(systcopArg));
      larg->start = (void (*)(void *)) 0;
      larg->tid = *tid;
      cond_init(&larg->cv, USYNC_THREAD, 0);
      ysHshIns(systcopThrTbl, &larg->tid, sizeof(larg->tid), (dvoid *) larg);
    }
  return larg; 
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


