/* mx/src/yd/ydmtd.c */


/*
ORACLE, Copyright (c) 1982, 1983, 1986, 1990 ORACLE Corporation
ORACLE Utilities, Copyright (c) 1981, 1982, 1983, 1986, 1990, 1991 ORACLE Corp

Restricted Rights
This program is an unpublished work under the Copyright Act of the
United States and is subject to the terms and conditions stated in
your  license  agreement  with  ORACORP  including  retrictions on
use, duplication, and disclosure.

Certain uncopyrighted ideas and concepts are also contained herein.
These are trade secrets of ORACORP and cannot be  used  except  in
accordance with the written permission of ORACLE Corporation.
*/





#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDMTDIDLI_ORACLE
#include <ydmtdidlI.h>
#endif
#ifndef YDRT_ORACLE
#include <ydrt.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDMT_ORACLE
#include <ydmt.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YDQ_ORACLE
#include <ydq.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDMTDIDL_ORACLE
#include <ydmtdidl.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif

#define ENTRY_POINT ydmtdMain
#include <s0ysmain.c>




#define	YDMTD_DEFAULT_TTL_MS    ((sb4)30000)


#define	YDMTD_DEFAULT_TIMEOUT_MS    (YDMTD_DEFAULT_TTL_MS)
    

#define	YDMTD_DEFAULT_LIVES    ((sb4)6)
    

#define	YDMTD_DEFAULT_POLL_LIMIT    ((sb4)8)


#define YDMTD_DEFAULT_REAP_MS	(YDMTD_DEFAULT_TTL_MS * YDMTD_DEFAULT_LIVES)


#define	YDMTD_DEFAULT_POLL_IDLE_MS	((sb4)10000)


#define	YDMTD_DEFAULT_DEST_CHECK_MS	((sb4)YDMTD_DEFAULT_TTL_MS)


#define	YDMTD_DEFAULT_Q_LEN_SEND_PCT	((sb4)25)

#define	YDMTD_DEFAULT_CHUNK_SIZE	((sb4)50)


static struct ysargmap ydmtdmap[] =
{
   { 'x', "mnorbmet.trace-level=5", 0 },
   { 't', "mnorbmet.trace-level", 1 },
   { 'd', "mnorbmet.distribute=true", 0 },
   { 'D', "mnorbmet.trace-dist-only=true", 0 },
   { 'P', "mnorbmet.trace-poll-only=true", 0 },
   { 'p', "mnorbmet.client-proc-metrics", 1 },
   { 'q', "mnorbmet.client-quit=true", 0 },
   { 'c', "mnorbmet.client-stats=true", 0 },
   { 'm', "mnorbmet.client-metrics=true", 0 },
   { 's', "mnorbmet.server=true", 0 },  
   { 'y', "mnorbmet.client-yort-metrics", 1 },
   { 0, (char *) 0, 0 }
};

typedef struct ydmtdcx ydmtdcx;	
typedef struct ydmtdd ydmtdd;	
typedef struct ydmtddx ydmtddx;	
typedef struct ydmtde ydmtde;	
typedef struct ydmtdp ydmtdp;	
typedef struct ydmtdy ydmtdy;	


struct ydmtdy
{
  ysspNode	    node_ydmtdy;	
  boolean	    ioactive_ydmtdy;	
  sb4		    lives_ydmtdy;	
  yort_timeTicks    ourUptime_ydmtdy;	
  yort_timeTicks    hisStart_ydmtdy;	
  sysb8		    exptime_ydmtdy;	
  yort_procInfo	    iopinfo_ydmtdy;	
  yort_procInfo	    pinfo_ydmtdy;	
  boolean	    phave_ydmtdy;	
  yort_dispInfoList iodlist_ydmtdy;	
  yort_dispInfoList dlist_ydmtdy;	
  boolean	    dhave_ydmtdy;	
  yort_implAllList  iolist_ydmtdy;	
  yort_implAllList  ilist_ydmtdy;	
  boolean	    ihave_ydmtdy;	
  yort_implAllList  lilist_ydmtdy;	
  boolean	    lihave_ydmtdy;	

  sysb8		    expire_ydmtdy;	
   
  ydmtdcx   *cx_ydmtdy;		
  yoenv	    ev_ydmtdy;		
  ysevt	    *sem_ydmtdy;	
  ysevt	    *tsem_ydmtdy;	
};



struct ydmtdd
{
  ysspNode	node_ydmtdd;	
  sb4		lives_ydmtdd;	
  ydmt_imtr	ydmt_ydmtdd;	
  sysb8		expire_ydmtdd;	
  sb4		nioactive_ydmtdd; 
};


struct ydmtde
{
  ydmtdcx   *cx_ydmtde;		
  ydmtdy    *yh_ydmtde;		
  ydmtdd    *dest_ydmtde;	
  yoenv	    ev_ydmtde;		
  ysevt	    *sem_ydmtde;	
  ysevt	    *tsem_ydmtde;	
};


typedef enum
{
  sleeping_ydmtdp,		
  proclist_ydmtdp,		
  iolimit_ydmtdp,		
  iterating_ydmtdp,		
  stopping_ydmtdp,		
  finished_ydmtdp		
} ydmtd_pstate;


struct ydmtdp
{
  ydmtd_pstate	    state_ydmtdp;
  sb4		    lives_ydmtdp; 
  sysb8		    pollIdle_ydmtdp; 
  ydim_yortList	    ylist_ydmtdp;   
  ub4		    cursor_ydmtdp; 
  ydim_yortIterator yi_ydmtdp;	    
  sb4		    npolls_ydmtdp;  
  sb4		    npending_ydmtdp;
  yoenv		    ev_ydmtdp;	    
  ysevt		    *sem_ydmtdp;    
  boolean	    trace_ydmtdp;   
};


typedef enum
{
  sleeping_ydmtddx,
  getmt_ydmtddx,
  stopping_ydmtddx,
  finished_ydmtddx
} ydmtd_dstate;


struct ydmtddx			
{
  ydmtd_dstate state_ydmtddx;

  sysb8	    destInterval_ydmtddx;	
  yoenv	    ev_ydmtddx;		
  ysevt	    *sem_ydmtddx;	
  sb4	    nsends_ydmtddx;	
  sb4	    npending_ydmtddx;	
  yslst	    *olist_ydmtddx;	
  ydim_imr  imr_ydmtddx;	
  boolean   trace_ydmtddx;	
};


struct ydmtdcx
{
  ysque	    *q_ydmtdcx;
  boolean   serving_ydmtdcx;	
  ydmtd	    self_ydmtdcx;

  boolean   dist_ydmtdcx;	

  
  sysb8	    timeout_ydmtdcx;	
  sb4	    lives_ydmtdcx;
  sysb8	    ttl_ydmtdcx;	
  sb4	    maxPoll_ydmtdcx;	

  ydim_imr  imr_ydmtdcx;	
  sb4	    trace_ydmtdcx;	

  ysevt	    *dsem_ydmtdcx;	
  boolean   stop_ydmtdcx;	
  sb4	    chunk_ydmtdcx;	

  ysspTree  ytree_ydmtdcx;	
  sb4	    nytree_ydmtdcx;
  ysspTree  dtree_ydmtdcx;	
  sb4	    ndtree_ydmtdcx;

  sysb8	    reap_ydmtdcx;	
  ydmtdp    pcx_ydmtdcx;	
  ydmtddx   dcx_ydmtdcx;	
};




STATICF void ydmtdClient( boolean quit, boolean stats, boolean metrics,
			 char *yortstr, char *noreg procstr );
STATICF void ydmtdServer( sb4 trace );


STATICF ydmtd ydmtdInit( ysque *q );
STATICF void ydmtdTerm( ydmtd mtd );
STATICF void ydmtdCleanup( ydmtdcx *cx );

STATICF void ydmtdSendMetrics( ydmtdcx *cx, ydmtdy *yh, ydmtdd *dest );
STATICF void ydmtdConsiderInfo( ydmtdcx *cx, ydmtdy *yh );
STATICF boolean ydmtdShouldSend(ydmtdcx *cx, yort_implAll *o, yort_implAll *n);
STATICF void ydmtdReaper( ydmtdcx *cx );
STATICF void ydmtdPFree( ydmtdy *yh );
STATICF void ydmtdIFree( ydmtdy *yh );
STATICF void ydmtdDFree( ydmtdy *yh );
STATICF void ydmtdLIFree( ydmtdy *yh );
STATICF void ydmtdYHDestroy( ydmtdy *yh );
STATICF void ydmtdDDDestroy( ydmtdd *dd );




STATICF void ydmtdPoll(dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz);
STATICF void ydmtdPollComp(dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz);
STATICF void ydmtdPollTimeout(dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz);
STATICF void ydmtdPollHead( CONST char *msg, CONST ysid *exid, ydmtdy *yh,
			   ydmtdp *p, ydmtdcx *cx );
STATICF void ydmtdPollTail( ydmtdcx *cx, ydmtdp *p );
STATICF CONST char *ydmtdPState( ydmtdp *p );
STATICF void ydmtdProcList( ydmtdcx *cx );
STATICF void ydmtdPollStop( ydmtdcx *cx );


STATICF void ydmtdDist(dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz);
STATICF void ydmtdDistComp(dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz);
STATICF void ydmtdDistTimeout(dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz);
STATICF CONST char *ydmtdDState( ydmtddx *d );
STATICF boolean ydmtdDeqDest( ydmtdcx *cx );
STATICF void ydmtdDistStop( ydmtdcx *cx );

externdef ysmtagDecl(ydmtdcx_tag) = "ydmtdcx";
externdef ysmtagDecl(ydmtde_tag) = "ydmtde";
externdef ysmtagDecl(ydmtdd_tag) = "ydmtdd";
externdef ysmtagDecl(ydmtdy_tag) = "ydmtdy";

externdef ysidDecl(YDMTD_EX_INTERNAL) = "ydmtd::internal";
externdef ysidDecl(YDMTD_EX_IOLIMIT_OVER) = "ydmtd::iolimit_over";
externdef ysidDecl(YDMTD_EX_MACHINE_DOWN) = "ydmtd::machine_down";
externdef ysidDecl(YDMTD_EX_QUEUE_DOWN) = "ydmtd::queue_down";

static CONST_W_PTR struct ydmtd__tyimpl ydmtd__impl =
 {
  ydmtd_getYortMetrics_i,
  ydmtd_getProcMetrics_i,
  ydmtd_shutdown_i,
  ydmtd__get_self_i,
  ydmtd__get_stats_i,
  ydmtd__get_metrics_i,
  ydmtd__get_ttlMs_i,
  ydmtd__set_ttlMs_i,
  ydmtd__get_pollIdleMs_i,
  ydmtd__set_pollIdleMs_i,
  ydmtd__get_destCheckMs_i,
  ydmtd__set_destCheckMs_i,
  ydmtd__get_timeoutMs_i,
  ydmtd__set_timeoutMs_i,
  ydmtd__get_maxPoll_i,
  ydmtd__set_maxPoll_i
 };







boolean ydmtdMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok, quit, stats, metrics;
  sword     sts = 0;
  char	    *arg;
  char      vbuf[80];
  noreg sb4 trace;
  char	    *yortstr;
  char	    *procstr;
  
  NOREG(trace);

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydmtdmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Metric Daemon");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  arg = ysResGetLast("mnorbmet.trace-level");
  trace = arg ? atoi(arg) : 0;

  quit = ysResGetBool("mnorbmet.client-quit");
  stats = ysResGetBool("mnorbmet.client-stats");
  metrics = ysResGetBool("mnorbmet.client-metrics");
  yortstr = ysResGetLast("mnorbmet.client-yort-metrics");
  procstr = ysResGetLast("mnorbmet.client-proc-metrics");

  yseTry
  {
    ytInit();
    yoInit();

    if( quit || stats || metrics || yortstr || procstr )
      ydmtdClient( quit, stats, metrics, yortstr, procstr );
    else
    {
      yslDetach();			
      ydmtdServer( trace );
    }
  }
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
    yslError("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;

  yseTry
  {
    if( trace ) yslError("ydmtdMain: yoTerm...\n");
    yoTerm();
    if( trace ) yslError("ydmtdMain: ytTerm...\n");
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;

  if( trace ) yslError("ydmtdMain: ysTerm...\n");
  ysTerm(osdp);
  return TRUE;
}








STATICF void ydmtdServer( sb4 trace )
{
  ydmtdcx   *cx;
  ydmtd	    mtd;
  ysque *q;
  
  if( trace )
    yslError("ydmtdServer: Becoming ORBD Metric server\n");

  q = yoQueCreate( "ydmtd" );

  
  mtd = ydmtdInit( q );
  
  cx = (ydmtdcx*)yoGetImplState( (dvoid*)mtd );

  
  
  if( trace ) yslError("ydmtdServer: Entering service loop...\n");

  cx->serving_ydmtdcx = TRUE;
  yseTry
    yoService( q ); 
  yseCatch( YS_EX_INTERRUPT )
    yslError("ydmtdIntrHdlr: interrupted, shutting down");
  yseEnd;
  cx->serving_ydmtdcx = FALSE;
  
  if( trace ) yslError("ydmtdServer: Exited service loop.\n");
  
  if( trace ) yslError("ydmtdServer: terminating object services\n");
  ydmtdTerm( mtd );
  
  
  if( trace )
    yslError("ydmtdServer: destroying queue %x\n", q );

  yoQueDestroy( q );

  if( trace )
    yslError("ydmtdServer: ready to exit\n");
}





STATICF void ydmtdClient( boolean quit, boolean dostats, boolean metrics,
			 char *yortstr, char *noreg procstr )
{
  ydmtd	    lmtd;
  yoenv	    ev;
  ydmtdStats	    stats;
  ydmtdMetricsList  mlist;
  ydmtdMetrics	    met;
  yort_proc yort;
  char *host;
  char *pid;
  char *aff;
  
  NOREG(procstr);
  yoEnvInit(&ev);
 
  lmtd = (ydmtd)yoBind(ydmtd__id, (char *)0, (yoRefData*)0, (char*)0);
  yseTry
  {
    if( dostats )
    {
      yslPrint("%8s %8s %8s %8s\n",
	       "ndests", "nyorts", "npolls", "nsends" );
      yslPrint("%8s %8s %8s %8s\n",
	       "--------", "--------", "--------", "--------" );
      stats = ydmtd__get_stats( lmtd, &ev );
      yslPrint("%8d %8d %8d %8d\n",
	       stats.ndests_ydmtdStats,
	       stats.nyorts_ydmtdStats,
	       stats.npolls_ydmtdStats,
	       stats.nsends_ydmtdStats );
      ydmtdStats__free( &stats, yoFree);
    }
    if( metrics )
    {
      mlist = ydmtd__get_metrics(lmtd, &ev); 
      yslPrint("Metrics from mtd -- %d procs\n", mlist._length);
      ydqShowMtdList( &mlist );
      ydmtdMetricsList__free(&mlist, yoFree);
    }

    if( yortstr )
    {
      yort = (yort_proc)yoStrToRef( yortstr ); 
      ydmtd_getYortMetrics( lmtd, &ev, yort, &met );
      mlist._length = mlist._maximum = 1;
      mlist._buffer = &met;
      ydqShowMtdList( &mlist );
      ydmtdMetrics__free( &met, yoFree );
      yoRelease( (dvoid*)yort );
    }

    if( procstr )
    {
      host = procstr;
      while( *procstr && *procstr != ':' )
	procstr++;
      if( *procstr && *procstr == ':' )
	*procstr++ = 0;
      pid = procstr;
      while( *procstr && *procstr != ':' )
	procstr++;
      if( *procstr && *procstr == ':' )
	*procstr++ = 0;
      aff = procstr;

      ydmtd_getProcMetrics( lmtd, &ev, host, pid, aff, &met );
      mlist._length = mlist._maximum = 1;
      mlist._buffer = &met;
      ydqShowMtdList( &mlist );
      ydmtdMetrics__free( &met, yoFree );
    }

    if( quit )
      ydmtd_shutdown( lmtd, &ev );
  }
  yseCatch(YO_EX_BADOBJ)
  {
    
  }
  yseCatchAll
    yslError("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd

  yoRelease( (dvoid*)lmtd );
  yoEnvFree(&ev);
}





STATICF ydmtd ydmtdInit( ysque *q )
{
  ydmtdcx   *cx = (ydmtdcx*)ysmGlbAlloc( sizeof(*cx), "ydmtdcx" );
  char	    *arg;
  sb4	    val;
  ydmtdp    *p = &cx->pcx_ydmtdcx;
  ydmtddx   *d = &cx->dcx_ydmtdcx;

  CLRSTRUCT(*cx);

  cx->q_ydmtdcx = q;
  cx->serving_ydmtdcx = FALSE;
  cx->imr_ydmtdcx = (ydim_imr)yoBind( ydim_imr__id, (char*)0,
				     (yoRefData*)0,(char*)0);

  arg = ysResGetLast("mnorbmet.trace-level");
  cx->trace_ydmtdcx = arg ? atoi(arg) : 0;

  cx->stop_ydmtdcx = FALSE;

  arg = ysResGetLast("mnorbmet.chunksize");
  cx->chunk_ydmtdcx = arg ? atoi(arg) : YDMTD_DEFAULT_CHUNK_SIZE;

  arg = ysResGetLast("mnorbmet.ttl-ms");
  val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_TTL_MS;
  sysb8ext( &cx->ttl_ydmtdcx, val * 1000 );

  arg = ysResGetLast("mnorbmet.max-polls");
  val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_POLL_LIMIT;
  cx->maxPoll_ydmtdcx = val;
  if( cx->maxPoll_ydmtdcx < 0 )
    cx->maxPoll_ydmtdcx = 1;

  arg = ysResGetLast("mnorbmet.reap-ms");
  val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_REAP_MS;
  sysb8ext( &cx->reap_ydmtdcx, val * 1000 );

  arg = ysResGetLast("mnorbmet.timeout-ms");
  val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_TIMEOUT_MS;
  sysb8ext( &cx->timeout_ydmtdcx, val * 1000 );

  arg = ysResGetLast("mnorbmet.lives");
  cx->lives_ydmtdcx = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_LIVES;

  DISCARD ysspNewTree( &cx->ytree_ydmtdcx, yoCmp );
  cx->nytree_ydmtdcx = 0;
  DISCARD ysspNewTree( &cx->dtree_ydmtdcx, yoCmp );
  cx->ndtree_ydmtdcx = 0;

  p->trace_ydmtdp = d->trace_ydmtddx = TRUE;
  if( ysResGetBool("mnorbmet.trace-dist-only") )
    p->trace_ydmtdp = FALSE;
  if( ysResGetBool("mnorbmet.trace-poll-only") )
    d->trace_ydmtddx = FALSE;

  
  p->state_ydmtdp = sleeping_ydmtdp;

  arg = ysResGetLast("mnorbmet.poll-idle-ms");
  val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_POLL_IDLE_MS;
  sysb8ext( &p->pollIdle_ydmtdp, val * 1000 );

  p->npolls_ydmtdp = p->npending_ydmtdp = 0;
  p->ylist_ydmtdp._length = p->ylist_ydmtdp._maximum = 0;
  p->ylist_ydmtdp._buffer = (yort_proc*)0;
  p->yi_ydmtdp = (ydim_yortIterator)0;
  yoEnvInit( &p->ev_ydmtdp );
  p->sem_ydmtdp = ysEvtCreate( ydmtdPoll, (dvoid*)cx, cx->q_ydmtdcx, TRUE );
  
  d->state_ydmtddx = finished_ydmtddx;
  if( (cx->dist_ydmtdcx = ysResGetBool( "mnorbmet.distribute" ) ) )
  {    
    
    d->state_ydmtddx = sleeping_ydmtddx;
    
    arg = ysResGetLast("mnorbmet.dest-check-ms");
    val = arg ? (sb4)atol( arg ) : YDMTD_DEFAULT_DEST_CHECK_MS;
    sysb8ext( &d->destInterval_ydmtddx, val * 1000 );
    
    d->npending_ydmtddx = d->nsends_ydmtddx = 0;
    d->olist_ydmtddx = (yslst*)0;
    yoEnvInit( &d->ev_ydmtddx );
    d->sem_ydmtddx = (ysevt*)ysEvtCreate( ydmtdDist, (dvoid*)cx,
					 cx->q_ydmtdcx, TRUE );
  }

  
  ysTrigger( p->sem_ydmtdp, YS_EX_TIMEOUT, (dvoid*)0, (size_t)0 );

  
  if( cx->dist_ydmtdcx )
    ysTrigger( d->sem_ydmtddx, (ysid*)0, (dvoid*)0, (size_t)0 );
  
  
  
  yoSetImpl( ydmtd__id, (char*)0, ydmtd__stubs,
	    (dvoid*)&ydmtd__impl, (yoload)0, TRUE, (dvoid*)cx );
  
  cx->self_ydmtdcx = (ydmtd)yoCreate( ydmtd__id, (char*)0,
				      (yoRefData*)0, (char*)0, (dvoid*)cx);
  
  yoImplReady( ydmtd__id, (char*)0, cx->q_ydmtdcx );

  return( cx->self_ydmtdcx );
}




STATICF void ydmtdTerm( ydmtd mtd )
{
  ydmtdcx *cx = (ydmtdcx*)yoGetImplState( (dvoid*)mtd );

  

  yoDispose( (dvoid*)mtd );
  yoImplDeactivate( ydmtd__id, (char*)0 );

  cx->dsem_ydmtdcx = ysSemCreate( (dvoid*)0 );
  cx->stop_ydmtdcx = TRUE;

  if( cx->trace_ydmtdcx >= 2 )
  {
    yslError("ydmtdTerm: stopping async services\n");
    yslError("ydmtdTerm: poll in state '%s', dist in state '%s'\n",
	     ydmtdPState( &cx->pcx_ydmtdcx ),
	     ydmtdDState( &cx->dcx_ydmtdcx ));
  }

  

  ysSvcLoop( cx->q_ydmtdcx, cx->dsem_ydmtdcx );
  ysSemDestroy( cx->dsem_ydmtdcx );

  

  if( cx->trace_ydmtdcx >= 2 )
  {
    yslError("ydmtdTerm: async services stopped\n");
    yslError("ydmtdTerm: poll in state '%s', dist in state '%s'\n",
	     ydmtdPState( &cx->pcx_ydmtdcx ),
	     ydmtdDState( &cx->dcx_ydmtdcx ));
  }
  ydmtdCleanup( cx );
}




STATICF void ydmtdCleanup( ydmtdcx *cx )
{
  ydmtdp *p = &cx->pcx_ydmtdcx;
  ydmtddx *d = &cx->dcx_ydmtdcx;
  ydmtdy *yh;
  ydmtdd *dd;
  if( p->yi_ydmtdp )
  {
    
    p->yi_ydmtdp = (ydim_yortIterator)0;
    if( cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdCleanup: destroying yort iterator %x\n", p->yi_ydmtdp );
    ydim_yortIterator_destroy( p->yi_ydmtdp, &p->ev_ydmtdp );
  }

  if( p->ylist_ydmtdp._buffer ) 
    ydim_yortList__free( &p->ylist_ydmtdp, yoFree );

  if( d->olist_ydmtddx )
    ysLstDestroy( d->olist_ydmtddx, (ysmff)0 );

  
  while( (yh = (ydmtdy*)ysspDeq( &cx->ytree_ydmtdcx.root_ysspTree ) ) )
    ydmtdYHDestroy( yh );

  
  while( (dd = (ydmtdd*)ysspDeq( &cx->dtree_ydmtdcx.root_ysspTree ) ) )
    ydmtdDDDestroy( dd );

  yoRelease((dvoid*)cx->imr_ydmtdcx);
  yoEnvFree( &p->ev_ydmtdp );
  ysmGlbFree( (dvoid*)cx );
}






void ydmtd_getYortMetrics_i( ydmtd or, yoenv* ev, yort_proc yort,
			    ydmtdMetrics* metrics)
{
  ydmtdcx *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  ysspNode *n;
  ydmtdy *yh;
  yort_implAllList *m = (yort_implAllList*)0;

  if( (n = ysspLookup( (dvoid*)yort, &cx->ytree_ydmtdcx ) ) )
  {
    
    yh = (ydmtdy*)n;

    yort_procInfo__copy( &metrics->pinfo_ydmtdMetrics,
			&yh->pinfo_ydmtdy, yoAlloc );

    yort_dispInfoList__copy( &metrics->dlist_ydmtdMetrics,
			    &yh->dlist_ydmtdy, yoAlloc );

    
    m = yh->ihave_ydmtdy ? &yh->ilist_ydmtdy : &yh->lilist_ydmtdy;

    yort_implAllList__copy( &metrics->metrics_ydmtdMetrics, m, yoAlloc );
  }
  else
    yseThrow(YDMTD_EX_NOTFOUND);
}


void ydmtd_getProcMetrics_i( ydmtd or, yoenv* ev, char* host, char* pid,
			    char* affinity, ydmtdMetrics* metrics)
{
  ydmtdcx *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  ysspNode *n;
  ydmtdy *yh;
  yort_procInfo *pinfo;
  yort_implAllList *m = (yort_implAllList*)0;

  for( n = ysspFHead( &cx->ytree_ydmtdcx ) ; n ; n = ysspFNext( n ) )
  {
    
    yh = (ydmtdy*)n;
    pinfo = &yh->pinfo_ydmtdy;

    if( yh->phave_ydmtdy && 
       !strcmp( host, pinfo->host_yort_procInfo ) &&
       !strcmp( pid, pinfo->pid_yort_procInfo ) &&
       !yduWildStrcmp( affinity, pinfo->affinity_yort_procInfo ))
    {
      yort_procInfo__copy( &metrics->pinfo_ydmtdMetrics,
			  &yh->pinfo_ydmtdy, yoAlloc );

      yort_dispInfoList__copy( &metrics->dlist_ydmtdMetrics,
			      &yh->dlist_ydmtdy, yoAlloc );

      
      m = yh->ihave_ydmtdy ? &yh->ilist_ydmtdy : &yh->lilist_ydmtdy;

      yort_implAllList__copy( &metrics->metrics_ydmtdMetrics, m, yoAlloc );
      return;
    }
  }
  yseThrow(YDMTD_EX_NOTFOUND);
}



ydmtdMetricsList ydmtd__get_metrics_i( ydmtd or, yoenv *ev)
{
  ydmtdcx *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  ysspNode *n;
  ydmtdy *yh;
  ydmtdMetricsList  mlist;
  yort_implAllList *m = (yort_implAllList*)0;

  mlist._maximum = (ub4)cx->nytree_ydmtdcx;
  mlist._buffer = (ydmtdMetrics*)yoAlloc( (size_t)cx->nytree_ydmtdcx *
					 sizeof(ydmtdMetrics));
  mlist._length = 0;
  for( n = ysspFHead( &cx->ytree_ydmtdcx ) ; n ; n = ysspFNext(n) )
  {
    
    yh = (ydmtdy*)n;

    
    if( !yh->phave_ydmtdy )
      continue;
    
    yort_procInfo__copy( &mlist._buffer[mlist._length].pinfo_ydmtdMetrics,
			&yh->pinfo_ydmtdy, yoAlloc );

    yort_dispInfoList__copy( &mlist._buffer[mlist._length].dlist_ydmtdMetrics,
			    &yh->dlist_ydmtdy, yoAlloc );

    
    m = yh->ihave_ydmtdy ? &yh->ilist_ydmtdy : &yh->lilist_ydmtdy;

    yort_implAllList__copy( &mlist._buffer[mlist._length].metrics_ydmtdMetrics,
			   m, yoAlloc );

    mlist._length++;
  }

  return( mlist );
}



void ydmtd_shutdown_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  ydmtdp    *p = &cx->pcx_ydmtdcx;
  ydmtddx   *d = &cx->dcx_ydmtdcx;

  if( cx->trace_ydmtdcx >= 1 )
    yslError("ydmtd_Shutdown_i: set flag to stop %d poll, %d dist\n",
	     p->npending_ydmtdp, d->npending_ydmtddx );
  
  
  yoShutdown( cx->q_ydmtdcx );
}



ydmtd ydmtd__get_self_i( ydmtd or, yoenv* ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  return( (ydmtd)yoDuplicate((dvoid*)cx->self_ydmtdcx ) );
}



sb4 ydmtd__get_pollIdleMs_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  sb4	    rv;
  
  rv = sysb8msk( &cx->pcx_ydmtdcx.pollIdle_ydmtdp ) / 1000;
  return( rv );
}


void ydmtd__set_pollIdleMs_i( ydmtd or, yoenv *ev, sb4 val)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );

  sysb8ext( &cx->pcx_ydmtdcx.pollIdle_ydmtdp, val * 1000 );
}


sb4 ydmtd__get_timeoutMs_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  sb4	    rv;
  
  rv = sysb8msk( &cx->timeout_ydmtdcx ) / 1000;
  return( rv );
}


void ydmtd__set_timeoutMs_i( ydmtd or, yoenv *ev, sb4 val)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );

  sysb8ext( &cx->timeout_ydmtdcx, val * 1000 );
}



sb4 ydmtd__get_ttlMs_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  sb4	    rv;
  
  rv = sysb8msk( &cx->ttl_ydmtdcx ) / 1000;
  return( rv );
}


void ydmtd__set_ttlMs_i( ydmtd or, yoenv *ev, sb4 val)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );

  sysb8ext( &cx->ttl_ydmtdcx, val * 1000 );
}



sb4 ydmtd__get_destCheckMs_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  sb4	    rv;

  rv = sysb8msk( &cx->dcx_ydmtdcx.destInterval_ydmtddx ) / 1000;
  return( rv );
}


void ydmtd__set_destCheckMs_i( ydmtd or, yoenv *ev, sb4 val)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );

  sysb8ext( &cx->dcx_ydmtdcx.destInterval_ydmtddx, val * 1000 );
}


sb4 ydmtd__get_maxPoll_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  return( cx->maxPoll_ydmtdcx );
}


void ydmtd__set_maxPoll_i( ydmtd or, yoenv *ev, sb4 val)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  cx->maxPoll_ydmtdcx = val;
  if( cx->maxPoll_ydmtdcx < 0 )
    cx->maxPoll_ydmtdcx = 1;
}


ydmtdStats ydmtd__get_stats_i( ydmtd or, yoenv *ev)
{
  ydmtdcx   *cx = (ydmtdcx*)yoGetImplState( (dvoid*)or );
  ydmtdStats rv;

  rv.ndests_ydmtdStats = cx->ndtree_ydmtdcx;
  rv.nyorts_ydmtdStats = cx->nytree_ydmtdcx;
  rv.npolls_ydmtdStats = cx->pcx_ydmtdcx.npolls_ydmtdp;
  rv.nsends_ydmtdStats = cx->dcx_ydmtdcx.nsends_ydmtddx;
  return( rv );
}












STATICF void ydmtdPoll(dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz)
{
  ydmtdcx   *cx = (ydmtdcx*)usrp;
  ydmtdp    *p;
  ydim_yortIterator yi;
  
  p = &cx->pcx_ydmtdcx;    
  ysEvtDestroy( p->sem_ydmtdp );
  p->sem_ydmtdp = (ysevt*)0;

  if( exid && (ysidEq(exid,YS_EX_SHUTDOWN) || ysidEq(exid,YS_EX_CANCELLED)) )
  {
    yslError("ydmtdPoll: exid %s, returning\n", ysidToStr(exid) );
    return;
  }

  if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdPoll: entry state %s ex '%s' arg %x argsz %d\n",
	     ydmtdPState(p), yduIdStr(exid), arg, argsz);
  
  
  if( cx->stop_ydmtdcx )
    p->state_ydmtdp = stopping_ydmtdp;
  else
    p->sem_ydmtdp = ysEvtCreate( ydmtdPoll, (dvoid*)cx,
				cx->q_ydmtdcx, TRUE );

  switch( p->state_ydmtdp )
  {
  case stopping_ydmtdp:

    ydmtdPollStop( cx );
    break;

  case sleeping_ydmtdp:			
    
    if( exid && !ysidEq(exid,YS_EX_TIMEOUT) )
      yslError("ydmtdPoll: unexpected exception %s in state %s\n",
	       yduIdStr(exid), ydmtdPState(p) );
    else if( !exid )
      yslError("ydmtdPoll: sleeping, but woke up without timeout!?\n");

    ydmtdReaper( cx );
    p->state_ydmtdp = proclist_ydmtdp;
    p->cursor_ydmtdp = 0;
    if( p->ylist_ydmtdp._buffer ) 
    {
      ydim_yortList__free( &p->ylist_ydmtdp, yoFree );
      p->ylist_ydmtdp._buffer = (yort_proc*)0;
      p->ylist_ydmtdp._length = p->ylist_ydmtdp._maximum = 0;
    }
    ydim_imr_listYort_nw( cx->imr_ydmtdcx,
			 &p->ev_ydmtdp, cx->chunk_ydmtdcx, (yort_proc)0, 
			 &p->ylist_ydmtdp, &p->yi_ydmtdp, p->sem_ydmtdp );
    break;

  case proclist_ydmtdp:			
    
    
    if( exid && ysidEq(exid,YT_EX_BROKEN) )
    {
      ydmtd_shutdown_i( cx->self_ydmtdcx, (yoenv*)0 );
      ydmtdPollStop( cx );
      break;
    }

    if( exid && ysidEq(exid,YDMTD_EX_IOLIMIT_OVER) )
    {
      
      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 5 )
	yslError("ydmtdPoll: exception %s in state %s, restarting poll\n",
	       yduIdStr(exid), ydmtdPState(p) );
      p->state_ydmtdp = sleeping_ydmtdp;
      ysTimer( &p->pollIdle_ydmtdp, p->sem_ydmtdp );
    }
    ydmtdProcList( cx );
    break;

  case iolimit_ydmtdp:  	
    
    
    yslError("ydmtdPoll: unexpected exid %s in state %s\n",
	     yduIdStr(exid), ydmtdPState(p) );
    break;

  case iterating_ydmtdp:		

    if( exid )
      yslError("ydmtdPoll: unexpected exception %s in state %s\n",
	       yduIdStr(exid), ydmtdPState(p) );

    if( exid || !(*(boolean*)arg) )
    {
      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 5 )
	yslError("ydmtdPoll: iterating over\n");
      yi = p->yi_ydmtdp;
      p->state_ydmtdp = proclist_ydmtdp;
      p->yi_ydmtdp = (ydim_yortIterator)0;

      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 1 )
	yslError("ydmtdPoll: destroying yi %x @ %x\n", yi, &p->yi_ydmtdp );

      ydim_yortIterator_destroy_nw( yi, &p->ev_ydmtdp, p->sem_ydmtdp );
      break;
    }
    ydmtdProcList( cx );
    break;
    
  default:

    yslError("ymtdpPoll: invalid state %d!?, exception %s\n",
	     p->state_ydmtdp, yduIdStr(exid) );
    ysePanic( exid ? exid : YDMTD_EX_INTERNAL);
    break;
  }

  if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4)
    yslError("ydmtdPoll: exit state %s, sem %x\n",
	     ydmtdPState(p), p->sem_ydmtdp );
}






STATICF void ydmtdPollComp(dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz)
{
  ydmtdy    *yh = (ydmtdy*)usrp;
  ydmtdcx   *cx = yh->cx_ydmtdy;
  ydmtdp    *p = &cx->pcx_ydmtdcx;
  sysb8	    t, tenk, cs;

  ydmtdPollHead( "ydmtdPollComp", exid, yh, p, cx );    
  if( !exid )		
  {
    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdPollComp: got yh %x pinfo %x %s/%s/%s %s y:%x ilist %x\n",
	       yh, &yh->pinfo_ydmtdy,
	       yduStr(yh->pinfo_ydmtdy.host_yort_procInfo),
	       yduStr(yh->pinfo_ydmtdy.pid_yort_procInfo),
	       yduStr(yh->pinfo_ydmtdy.affinity_yort_procInfo),
	       yduStr(yh->pinfo_ydmtdy.name_yort_procInfo),
	       yh->node_ydmtdy.key_ysspNode,
	       yh->ilist_ydmtdy );
    
    ysGetUpTime(&yh->expire_ydmtdy);
    sysb8add( &yh->expire_ydmtdy, &yh->expire_ydmtdy, &cx->reap_ydmtdcx );
    
    
    yort_procInfo__copy( &yh->pinfo_ydmtdy, &yh->iopinfo_ydmtdy,
			yotkAllocStr );
    yort_procInfo__free( &yh->iopinfo_ydmtdy, yoFree );
    yh->phave_ydmtdy = TRUE;

    yort_dispInfoList__copy( &yh->dlist_ydmtdy, &yh->iodlist_ydmtdy,
			    yotkAllocStr );
    yort_dispInfoList__free( &yh->iodlist_ydmtdy, yoFree );
    yh->dhave_ydmtdy = TRUE;

    yort_implAllList__copy( &yh->ilist_ydmtdy, &yh->iolist_ydmtdy,
			   yotkAllocStr );
    yort_implAllList__free( &yh->iolist_ydmtdy, yoFree );
    yh->ihave_ydmtdy = TRUE;

    
    if( !yh->ourUptime_ydmtdy )
    {
      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 5 )
	yslError("ydmtdPollComp: new yort\n");

      ysGetUpTime(&t);
      sysb8ext( &tenk, 10000 );
      sysb8div( &cs, &t, &tenk );
      yh->ourUptime_ydmtdy = sysb8msk( &cs );
      yh->hisStart_ydmtdy = yh->pinfo_ydmtdy.upTime_yort_procInfo;
    }
    ydmtdConsiderInfo( cx, yh );
  }
  ydmtdPollTail( cx, p );
}






STATICF void ydmtdPollTimeout(dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz)
{
  ydmtdy *yh = (ydmtdy*)usrp;
  ydmtdcx *cx = yh->cx_ydmtdy;
  ydmtdp *p = &cx->pcx_ydmtdcx;
  
  ydmtdPollHead( "ydmtdPollTimeout", exid, yh, p, cx );
  ydmtdPollTail( cx, p );
}





STATICF void ydmtdPollHead( CONST char *msg, CONST ysid *exid, ydmtdy *yh,
			   ydmtdp *p, ydmtdcx *cx )
{
  ysEvtDestroy( yh->sem_ydmtdy );
  ysEvtDestroy( yh->tsem_ydmtdy );
  yh->ioactive_ydmtdy = FALSE;
  p->npending_ydmtdp--;

  if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
  {
    yslError("%s: yh %x, ex '%s' y:%x\n",
	     msg, yh, yduIdStr(exid), yh->node_ydmtdy.key_ysspNode );
    yslError("%s: sem %x, tsem %x\n",
	     msg, yh->sem_ydmtdy, yh->tsem_ydmtdy );
  }

  if( exid && (ysidEq(exid,YT_EX_BADADDR) || ysidEq(exid,YT_EX_BROKEN)))
				yh->lives_ydmtdy = 0;
  else if( exid )		yh->lives_ydmtdy-- ;
  else				yh->lives_ydmtdy = cx->lives_ydmtdcx;
  if( yh->lives_ydmtdy < 0 )    yh->lives_ydmtdy = 0;
  
  if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4  )
    yslError("%s: npending %d, state %s  stop %d\n",
	     msg, p->npending_ydmtdp, ydmtdPState(p), cx->stop_ydmtdcx);

  
  if(!yh->lives_ydmtdy)
  {
    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
    {
      yslError("%s: Destroying yort of dead target...\n", msg);
      yslError("%s: yh->ev._buffer is %x\n", msg, yh->ev_ydmtdy._buffer );
    }
    ydim_imr_destroyYortGlobal_nw( cx->imr_ydmtdcx, &yh->ev_ydmtdy,
				  (yort_proc)yh->node_ydmtdy.key_ysspNode,
				  ysEvtDummy() );
  }
  if( exid && !ysidEq(exid,YT_EX_BADADDR) && !ysidEq(exid,YS_EX_TIMEOUT) &&
     !ysidEq(exid,YT_EX_BROKEN) )
  {
    yslError( "%s: unexpected exid %s to y:%x\n", msg,
	     yduIdStr(exid), yh->node_ydmtdy.key_ysspNode );
  }
}




STATICF void ydmtdPollTail( ydmtdcx *cx, ydmtdp *p )
{
  if( cx->stop_ydmtdcx )
    ydmtdPollStop( cx );

  if( p->state_ydmtdp == iolimit_ydmtdp &&
     p->sem_ydmtdp && p->npending_ydmtdp < cx->maxPoll_ydmtdcx )
  {
    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 2 )
      yslError("ydmtdPollTail: back under poll limit\n");
    p->state_ydmtdp = proclist_ydmtdp;
    ysTrigger( p->sem_ydmtdp, YDMTD_EX_IOLIMIT_OVER, (dvoid*)0, (size_t)0 );
  }

  if( !cx->stop_ydmtdcx )
    ysSvcAll( cx->q_ydmtdcx );
}





STATICF CONST char *ydmtdPState( ydmtdp *p )
{
  switch( p->state_ydmtdp )
  {
  case sleeping_ydmtdp:    return "sleeping";   
  case proclist_ydmtdp:    return "proclist";
  case iolimit_ydmtdp:    return "iolimit";
  case iterating_ydmtdp:   return "iterating";
  case stopping_ydmtdp:    return "stopping";
  case finished_ydmtdp:    return "finished";
  default:	    return "UNKNOWN STATE";
  }
}






STATICF void ydmtdProcList( ydmtdcx *cx )
{
  ydmtdp    *p = &cx->pcx_ydmtdcx;
  ydmtdy    *yh;	
  ysspNode  *n;
  yort_proc y;

  if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdProcList: cursor %d of %d\n",
	     p->cursor_ydmtdp, p->ylist_ydmtdp._length );

  
  while( !cx->stop_ydmtdcx &&
	p->cursor_ydmtdp < p->ylist_ydmtdp._length &&
	p->npending_ydmtdp < cx->maxPoll_ydmtdcx )
  {
    y = p->ylist_ydmtdp._buffer[ p->cursor_ydmtdp++ ];
    if( (n = ysspLookup( (dvoid*)y, &cx->ytree_ydmtdcx ) ) )
    {
      
      yh = (ydmtdy*)n;
      
      if( !yh->lives_ydmtdy || yh->ioactive_ydmtdy )
	continue;
    }
    else
    {
      
      yh = (ydmtdy*) ysmGlbAlloc(sizeof(*yh), ydmtdy_tag );
      CLRSTRUCT(*yh);
      yoEnvInit( &yh->ev_ydmtdy );
      sysb8ext( &yh->exptime_ydmtdy, 0 );
      yh->node_ydmtdy.key_ysspNode = yoDuplicate((dvoid*)y);
      yh->ourUptime_ydmtdy = 0;
      yh->lives_ydmtdy = cx->lives_ydmtdcx;
      ysClock(&yh->expire_ydmtdy);
      sysb8add( &yh->expire_ydmtdy, &yh->expire_ydmtdy, &cx->reap_ydmtdcx );
      yh->cx_ydmtdy = cx;
      DISCARD ysspEnq( &yh->node_ydmtdy, &cx->ytree_ydmtdcx );
      cx->nytree_ydmtdcx++;
    }

    

    ydmtdPFree( yh );
    ydmtdIFree( yh );
    ydmtdDFree( yh );
    yh->ioactive_ydmtdy = TRUE;
    p->npolls_ydmtdp++;
    p->npending_ydmtdp++;

    

    yh->sem_ydmtdy = ysEvtCreate( ydmtdPollComp, (dvoid*)yh,
				 cx->q_ydmtdcx, TRUE );
    yh->tsem_ydmtdy = ysEvtCreate( ydmtdPollTimeout, (dvoid*)yh,
				 cx->q_ydmtdcx, TRUE );

    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdProcList: polling y:%x, lives %d, pending %d\n",
	       y, yh->lives_ydmtdy, p->npending_ydmtdp );

    ysTimer( &cx->timeout_ydmtdcx, yh->tsem_ydmtdy );
    yort_proc_getImplAll_nw( (yort_proc)yh->node_ydmtdy.key_ysspNode,
			    &p->ev_ydmtdp,
			    (yort_implFlags)0,
			    &yh->iopinfo_ydmtdy,
			    &yh->iodlist_ydmtdy,
			    &yh->iolist_ydmtdy,
			    yh->sem_ydmtdy );
  }

  if( cx->stop_ydmtdcx )
  {
    ydmtdPollStop( cx );
  }
  else if( p->cursor_ydmtdp < p->ylist_ydmtdp._length )
  {
    
    p->state_ydmtdp = iolimit_ydmtdp;
    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdProcList: hit poll limit %d\n", cx->maxPoll_ydmtdcx );
  }
  else				
  {
    if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdProcList: empty list\n");
    
    if( p->ylist_ydmtdp._buffer ) 
    {
      ydim_yortList__free( &p->ylist_ydmtdp, yoFree );
      p->ylist_ydmtdp._buffer = (yort_proc*)0;
      p->ylist_ydmtdp._length = p->ylist_ydmtdp._maximum = 0;
    }
    if( p->yi_ydmtdp )		
    {
      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 1 )
	yslError("ydmtdPoll: iterating with yi %x for ylist %x\n",
		 p->yi_ydmtdp, &p->ylist_ydmtdp );

      p->state_ydmtdp = iterating_ydmtdp;
      p->cursor_ydmtdp = 0;
      ydim_yortIterator_next_n_nw( p->yi_ydmtdp, &p->ev_ydmtdp,
				 cx->chunk_ydmtdcx,
				 &p->ylist_ydmtdp, p->sem_ydmtdp );
    }
    else			
    {
      if( p->trace_ydmtdp && cx->trace_ydmtdcx >= 5 )
	yslError("ydmtdProcList: starting sleeping delay\n");
      p->state_ydmtdp = sleeping_ydmtdp;
      ysTimer( &p->pollIdle_ydmtdp, p->sem_ydmtdp );
    }
  }
}






STATICF void ydmtdPollStop( ydmtdcx *cx )
{
  ydmtdp    *p = &cx->pcx_ydmtdcx;
  ydmtddx   *d = &cx->dcx_ydmtdcx;

  if( cx->trace_ydmtdcx >= 1 )
    yslError("ydmtdPollStop: pending %d state %s sem %x \n",
	     p->npending_ydmtdp, ydmtdPState(p),
	     p->sem_ydmtdp );

  
  if( p->sem_ydmtdp )
  {
    if( cx->trace_ydmtdcx >= 3 )
      yslError("ydmtdPollStop: cancelling evt %x\n", p->sem_ydmtdp );
    ysEvtDestroy( p->sem_ydmtdp );
    p->sem_ydmtdp = (ysevt*)0;
  }
  p->state_ydmtdp = stopping_ydmtdp;

  
  if( !p->npending_ydmtdp && p->state_ydmtdp != finished_ydmtdp )
  {
    p->state_ydmtdp = finished_ydmtdp;
    if( !d->npending_ydmtddx && d->state_ydmtddx == finished_ydmtddx &&
       cx->dsem_ydmtdcx )
      ysTrigger( cx->dsem_ydmtdcx, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
}





STATICF void ydmtdConsiderInfo( ydmtdcx *cx, ydmtdy *yh )
{
  ysspNode  *n;
  boolean   sendit = FALSE;
  ub4	    i;
  sysb8	    now;
  yort_implAllList   *il, *lil;
  boolean isnew, changed, expired; 

  
  if( !yh->ourUptime_ydmtdy )
    return;

  isnew = changed = expired = FALSE;
  
  il = &yh->ilist_ydmtdy;
  lil = &yh->lilist_ydmtdy;
  ysClock(&now);

  
  if( !yh->lihave_ydmtdy )
    isnew = TRUE;
  else if( il->_length != lil->_length )
    changed = TRUE;
  else if( sysb8cmp( &yh->exptime_ydmtdy, <= , &now ) )
    expired = TRUE;

  if( isnew || changed || expired )  
  {
    sendit = TRUE;
    if( cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdConsiderInfo: isnew %d, changed %d, expired %d\n",
	       isnew, changed, expired );

    
    if( !cx->dist_ydmtdcx )
    {
      
      ydmtdLIFree( yh );
      
      
      DISCARD memcpy( lil, il, sizeof(*lil) );
      yh->lihave_ydmtdy = yh->ihave_ydmtdy;
      yh->ihave_ydmtdy = FALSE;
    }
  }
  else for( i = 0 ; !sendit && i < il->_length ; i++ )
    sendit = ydmtdShouldSend( cx, &lil->_buffer[i], &il->_buffer[i] );

  if( cx->dist_ydmtdcx && sendit ) 
  {
    if( cx->trace_ydmtdcx >= 4 )
      yslError("ydmtdConsiderInfo: switching/sending lihave %d ihave %d\n",
	       yh->lihave_ydmtdy, yh->ihave_ydmtdy );

    sysb8add( &yh->exptime_ydmtdy, &now, &cx->ttl_ydmtdcx );

    
    ydmtdLIFree( yh );

    
    DISCARD memcpy( lil, il, sizeof(*lil) );
    yh->lihave_ydmtdy = yh->ihave_ydmtdy;
    yh->ihave_ydmtdy = FALSE;
    for( n = ysspFHead( &cx->dtree_ydmtdcx ); n ; n = ysspFNext( n ) )
      
      ydmtdSendMetrics( cx, yh, (ydmtdd*)n );
  }
}




STATICF boolean ydmtdShouldSend(ydmtdcx *cx, yort_implAll *o, yort_implAll *n)
{
  sb4	olen = o->queue_yort_implAll.curLen_yort_queueInfo; 
  sb4	nlen = n->queue_yort_implAll.curLen_yort_queueInfo; 
  boolean rv = nlen != olen;
  
  return( cx->dist_ydmtdcx && rv );
}










STATICF void ydmtdDist(dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz)
{
  ydmtdcx   *cx = (ydmtdcx*)usrp;
  ydmtddx   *d;
  ysspNode  *n;
  ydmtdd    *dn;
  
  d = &cx->dcx_ydmtdcx;
  ysEvtDestroy( d->sem_ydmtddx );
  d->sem_ydmtddx = (ysevt*)0;

  if( exid && (ysidEq(exid,YS_EX_SHUTDOWN) || ysidEq(exid,YS_EX_CANCELLED)))
  {
    yslError("ydmtdDist: exid %s, returning\n", yduIdStr(exid) );
    return;
  }

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdDist: entry state %s, ex '%s' arg %x argsz %d\n",
	     ydmtdDState(d), yduIdStr(exid), arg, argsz );

  if( cx->stop_ydmtdcx )
    d->state_ydmtddx = stopping_ydmtddx; 
  else
    d->sem_ydmtddx = ysEvtCreate( ydmtdDist, (dvoid*)cx,
				 cx->q_ydmtdcx, TRUE );

  switch( d->state_ydmtddx )
  {
  case stopping_ydmtddx:

    ydmtdDistStop( cx );
    break;
    
  case sleeping_ydmtddx:
    
    
    
    ydmtdReaper( cx );
    d->olist_ydmtddx = yoListORBDs();
    if( !ysLstCount( d->olist_ydmtddx ) )
    {
      yslError("ydmdtDist: no orbds.  Exiting\n");
      ydmtd_shutdown_i( cx->self_ydmtdcx, (yoenv*)0 );
    }
    else
    {
      d->state_ydmtddx = getmt_ydmtddx; 
      if( !ydmtdDeqDest( cx ) )
	d->state_ydmtddx = sleeping_ydmtddx;
    }
    break;

  case getmt_ydmtddx:

    
    if( exid && !ysidEq(exid,YS_EX_TIMEOUT) && !ysidEq(exid,YT_EX_BROKEN) )
    {
      
      yslError("ydmtdDist: getmt received unexpected exception %s\n",
	       yduIdStr(exid) );
    }
    else if( !exid )
    {
      
      dn = (ydmtdd*)ysmGlbAlloc(sizeof(*dn), ydmtdd_tag );
      dn->node_ydmtdd.key_ysspNode = yoDuplicate((dvoid*)d->imr_ydmtddx);
      dn->lives_ydmtdd = cx->lives_ydmtdcx;
      dn->ydmt_ydmtdd = *(ydmt_imtr*)arg;
      ysGetUpTime(&dn->expire_ydmtdd);
      sysb8add( &dn->expire_ydmtdd, &dn->expire_ydmtdd, &cx->reap_ydmtdcx );
      dn->nioactive_ydmtdd = 0;

      cx->ndtree_ydmtdcx++;
      DISCARD ysspEnq( &dn->node_ydmtdd, &cx->dtree_ydmtdcx );

      
      for( n = ysspFHead( &cx->ytree_ydmtdcx ); n ; n = ysspFNext( n ) )
	
    	ydmtdSendMetrics( cx, (ydmtdy*)n, dn );
    }
    
    
    if( !ydmtdDeqDest( cx ) )
      d->state_ydmtddx = sleeping_ydmtddx; 

    break;

  default:

    yslError("ydmtdDist: unknown state %d, exid %s\n",
	     d->state_ydmtddx, yduIdStr(exid));
    ysePanic(exid ? exid : YDMTD_EX_INTERNAL);
  }

  if( d->state_ydmtddx == sleeping_ydmtddx )
    ysTimer( &d->destInterval_ydmtddx, d->sem_ydmtddx );

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdDist: exit state %s, sem %x\n",
	     ydmtdDState(d), d->sem_ydmtddx );

}

    




STATICF void ydmtdDistComp(dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz)
{
  ydmtde    *e = (ydmtde*)usrp;
  ydmtdcx   *cx = e->cx_ydmtde;
  ydmtddx   *d = &cx->dcx_ydmtdcx;
  ydmtdd    *dd = e->dest_ydmtde;

  ysEvtDestroy( e->sem_ydmtde );
  ysEvtDestroy( e->tsem_ydmtde );
  d->npending_ydmtddx--;

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdDistComp: e %x ex '%s' arg %x argsz %d\n",
	     e, yduIdStr(exid), arg, argsz );

  dd->nioactive_ydmtdd--;

  if( exid && (ysidEq(exid,YT_EX_BADADDR) || ysidEq(exid,YT_EX_BROKEN)) )
				dd->lives_ydmtdd = 0;
  else if( exid )		dd->lives_ydmtdd--;
  else				dd->lives_ydmtdd = cx->lives_ydmtdcx;

  if( dd->lives_ydmtdd < 0 )
    dd->lives_ydmtdd = 0;
  else if( dd->lives_ydmtdd )
  {
    ysGetUpTime(&dd->expire_ydmtdd);
    sysb8add( &dd->expire_ydmtdd, &dd->expire_ydmtdd, &cx->reap_ydmtdcx );
  }

  yoEnvFree( &e->ev_ydmtde );
  ysmGlbFree( usrp );

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdDistComp: npending %d, state %s stop %d\n",
	     d->npending_ydmtddx, ydmtdDState(d), cx->stop_ydmtdcx);

  if( cx->stop_ydmtdcx )
    ydmtdDistStop( cx );
  else
    ysSvcAll( cx->q_ydmtdcx );
}





STATICF void ydmtdDistTimeout(dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz)
{
  ydmtde    *e = (ydmtde*)usrp;
  ydmtdcx   *cx = e->cx_ydmtde;
  ydmtddx   *d = &cx->dcx_ydmtdcx;

  ysEvtDestroy( e->sem_ydmtde );
  d->npending_ydmtddx--;

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 4 )
    yslError("ydmtdDistTimeout: e %x ex '%s' arg %x argsz %d\n",
	     e, yduIdStr(exid), arg, argsz );

  e->dest_ydmtde->nioactive_ydmtdd--;

  if( !--(e->dest_ydmtde->lives_ydmtdd) )
    e->dest_ydmtde->lives_ydmtdd = 0;

  yoEnvFree( &e->ev_ydmtde );
  ysmGlbFree( usrp );

  if( cx->stop_ydmtdcx )
    ydmtdDistStop( cx );
  else
    ysSvcAll( cx->q_ydmtdcx );
}



STATICF CONST char *ydmtdDState( ydmtddx *d )
{
  switch( d->state_ydmtddx )
  {
  case sleeping_ydmtddx:    return "sleeping";   
  case getmt_ydmtddx:	    return "getmt";
  case stopping_ydmtddx:    return "stopping";
  case finished_ydmtddx:    return "finished";
  default:	    return "UNKNOWN STATE";
  }
}





STATICF boolean ydmtdDeqDest( ydmtdcx *cx )    
{
  ydmtddx *d = &cx->dcx_ydmtdcx;

  
  while( (d->imr_ydmtddx = (ydim_imr)ysLstDeq( d->olist_ydmtddx)) )
  {
    if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 5 )
      yslError("ydmtdDeqDest: checking imr %x\n", d->imr_ydmtddx );

    if( !ysspLookup( (dvoid*)d->imr_ydmtddx, &cx->dtree_ydmtdcx ) )
    {
      if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 5 )
	yslError("ydmtdDeqDest: is new imr, getting ydad_orbd and ydmt\n");

      
      ydim_imr__get_mt_nw( d->imr_ydmtddx, &d->ev_ydmtddx, d->sem_ydmtddx );
      return TRUE;
    }
  }

  if( d->trace_ydmtddx && cx->trace_ydmtdcx >= 5 )
    yslError("ydmtdDeqDest: list consumed\n");

  
  ysLstDestroy( d->olist_ydmtddx, (ysmff)0 );
  d->olist_ydmtddx = (yslst*)0;
  return( FALSE );
}



    

STATICF void ydmtdSendMetrics( ydmtdcx *cx, ydmtdy *yh, ydmtdd *dest )
{
  ydmtde    *e;
  ydmtddx   *d = &cx->dcx_ydmtdcx;

  if( !cx->dist_ydmtdcx )
    ysePanic( YDMTD_EX_INTERNAL );

  
  if( yh->ourUptime_ydmtdy && dest->lives_ydmtdd )
  {
    if( !cx->stop_ydmtdcx )
    {
      e = (ydmtde*)ysmGlbAlloc( sizeof(*e), ydmtde_tag );
      yoEnvInit( &e->ev_ydmtde );
      e->cx_ydmtde = cx;
      e->yh_ydmtde = yh;
      e->dest_ydmtde = dest;
      e->sem_ydmtde = ysEvtCreate( ydmtdDistComp, (dvoid*)e,
				  cx->q_ydmtdcx, TRUE );
      e->tsem_ydmtde = ysEvtCreate( ydmtdDistTimeout, (dvoid*)e,
				   cx->q_ydmtdcx, TRUE );
      d->npending_ydmtddx++;
      d->nsends_ydmtddx++;
      if( cx->trace_ydmtdcx >= 5 )
      {
	yslError("ydmtdSendMetrics: e %x y:%x ydim:%x s:%x ts:%x pending %d\n",
		 e, yh->node_ydmtdy.key_ysspNode,
		 dest->node_ydmtdd.key_ysspNode,
		 e->sem_ydmtde,
		 e->tsem_ydmtde,
		 d->npending_ydmtddx );
      }
      dest->nioactive_ydmtdd++;
      ysTimer( &cx->timeout_ydmtdcx, e->tsem_ydmtde );
      ydmt_imtr_addYortAll_nw( dest->ydmt_ydmtdd,
			      &e->ev_ydmtde,
			      (yort_proc)yh->node_ydmtdy.key_ysspNode,
			      &yh->lilist_ydmtdy, e->sem_ydmtde );
    }
  }
}





STATICF void ydmtdDistStop( ydmtdcx *cx )
{
  ydmtddx *d = &cx->dcx_ydmtdcx;
  ydmtdp *p = &cx->pcx_ydmtdcx;

  if( cx->trace_ydmtdcx >= 1 )
    yslError("ydmtdDistStop: %d npending, state %s, sem %x\n",
	     d->npending_ydmtddx, ydmtdDState(d),
	     d->sem_ydmtddx );

  
  if( d->sem_ydmtddx )
  {
    if( cx->trace_ydmtdcx >= 5 )
      yslError("ydmtdDistStop: cancelling evt %x\n", d->sem_ydmtddx );
    ysEvtDestroy( d->sem_ydmtddx );
    d->sem_ydmtddx = (ysevt*)0;
  }
  
  if( !d->npending_ydmtddx && d->state_ydmtddx != finished_ydmtddx )
  {
    d->state_ydmtddx = finished_ydmtddx;
    if( !p->npending_ydmtdp && p->state_ydmtdp == finished_ydmtdp )
      ysTrigger( cx->dsem_ydmtdcx, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
}





STATICF void ydmtdPFree( ydmtdy *yh )
{
  if( yh->phave_ydmtdy )
  {
    yort_procInfo__free( &yh->pinfo_ydmtdy, yotkFreeStr );
    CLRSTRUCT( yh->pinfo_ydmtdy );
    yh->phave_ydmtdy = FALSE;
  }
}






STATICF void ydmtdIFree( ydmtdy *yh )
{
  if( yh->ihave_ydmtdy )
  {
    yort_implAllList__free( &yh->ilist_ydmtdy, yotkFreeStr );
    CLRSTRUCT( yh->ilist_ydmtdy );
    yh->ihave_ydmtdy = FALSE;
  }
}


STATICF void ydmtdDFree( ydmtdy *yh )
{
  if( yh->dhave_ydmtdy )
  {
    yort_dispInfoList__free( &yh->dlist_ydmtdy, yotkFreeStr );
    CLRSTRUCT( yh->dlist_ydmtdy );
    yh->dhave_ydmtdy = FALSE;
  }
}







STATICF void ydmtdLIFree( ydmtdy *yh )
{
  if( yh->lihave_ydmtdy )
  {
    yort_implAllList__free( &yh->lilist_ydmtdy, yotkFreeStr );
    CLRSTRUCT( yh->lilist_ydmtdy );
    yh->lihave_ydmtdy = FALSE;
  }
}





STATICF void ydmtdYHDestroy( ydmtdy *yh )
{
  yoRelease( yh->node_ydmtdy.key_ysspNode );
  ydmtdPFree( yh );
  ydmtdIFree( yh );
  ydmtdLIFree( yh );
  yoEnvFree( &yh->ev_ydmtdy );
  ysmGlbFree( (dvoid*)yh );
}




STATICF void ydmtdDDDestroy( ydmtdd *dd )
{
  yoRelease( dd->node_ydmtdd.key_ysspNode );
  yoRelease( (dvoid*)dd->ydmt_ydmtdd );
  ysmGlbFree( (dvoid*)dd );
}




STATICF void ydmtdReaper( ydmtdcx *cx )
{
  ydmtdy *yh = (ydmtdy*)0;
  ydmtdd *dd = (ydmtdd*)0;
  ysspNode  *n, *next;
  sysb8	now;

  ysClock(&now);

  for( n = ysspFHead( &cx->ytree_ydmtdcx ); n ; n = next )
  {
    next = ysspFNext( n );
    
    yh = (ydmtdy*)n;
    if( !yh->lives_ydmtdy && sysb8cmp( &now, >= , &yh->expire_ydmtdy) )
    {
      if( yh->ioactive_ydmtdy )
      {
	yslError("***ydmtdReaper: dead yh %x has i/o active\n", yh );
      }
      else
      {
	if( cx->trace_ydmtdcx >= 2 )
	  yslError("ydmtdReaper: releasing dead yh %x\n", yh );
	ysspRemove( n, &cx->ytree_ydmtdcx );
	cx->nytree_ydmtdcx--;
	ydmtdYHDestroy( yh );
      }
    }
  }

  for( n = ysspFHead( &cx->dtree_ydmtdcx ); n ; n = next )
  {
    next = ysspFNext( n );
    
    dd = (ydmtdd*)n;
    if( !dd->lives_ydmtdd && sysb8cmp( &now, >= , &dd->expire_ydmtdd) )
    {
      if( dd->nioactive_ydmtdd )
      {
	yslError("***ydmtdReaper: dest dest %x (imr %x) has %d io's active.\n",
		 dd, dd->node_ydmtdd.key_ysspNode, dd->nioactive_ydmtdd );
      }
      else
      {
	if( cx->trace_ydmtdcx >= 2 )
	  yslError("ydmtdReaper: releasing dead dest %x, imr %x, mt %x\n",
		   dd, dd->node_ydmtdd.key_ysspNode, dd->ydmt_ydmtdd );
	ysspRemove( n, &cx->dtree_ydmtdcx );
	cx->ndtree_ydmtdcx--;
	ydmtdDDDestroy( dd );
      }
    }
  }
}


