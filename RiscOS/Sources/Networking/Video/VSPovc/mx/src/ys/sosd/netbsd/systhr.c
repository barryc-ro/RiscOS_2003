/* Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
 *
 * systhr.c - OMN system-dependent threading definitions
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
  &systhr_yst,
  &systhr_ystrt,
};

/*
 * SYSTHRTAB_MAX - the number of entries in the thread package table
 */
externdef CONST_DATA sword SYSTHRTAB_MAX
  = sizeof(systhrtab) / sizeof(systhrop *);

#ifdef NEVER

/***************************** NATIVE IMPLEMENATION***************************/
#include <thread.h>

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz);
STATICF void systntvExit(void);
STATICF void systntvSuspend(void);
STATICF void systntvResume(ub1 *td);
STATICF void systntvSelf(ub1 *td);
STATICF void systntvPrint(ub1 *td, char *buf, size_t len);

static CONST_DATA systhrop systhr_sol =
{
  "NATIVE", sizeof(thread_t),
  systntvCreate, systntvExit, thr_yield, systntvSuspend, systntvResume,
  systntvSelf, systntvPrint
};

STATICF sword systntvCreate(ub1 *td, void (*start)(dvoid *),
			    dvoid *arg, size_t stksz)
{
  return thr_create((void *) 0, stksz, (void *(*)(void *)) start, arg,
		    THR_DETACHED, (thread_t *) td);
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
  DISCARD thr_continue(*((thread_t *) td));
}

STATICF void systntvSelf(ub1 *td)
{
  thread_t tid = thr_self();
  CPSTRUCT(*((thread_t *) td), tid);
}

STATICF void systntvPrint(ub1 *td, char *buf, size_t len)
{
  DISCARD sprintf(buf, "%lu", *((thread_t *) td));
}

#endif

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

  tid = ystCreate(start, arg, (stksz ? stksz : (ub4) 32768));
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
