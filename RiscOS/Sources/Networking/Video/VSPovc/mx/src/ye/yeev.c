/* mx/src/ye/yeev.c */


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
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif
#ifndef YEEVENTI_ORACLE
#include <yeeventI.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
#endif
#ifndef YSMSC_ORACLE
#include <ysmsc.h>
#endif
#ifndef SYSXCD_ORACLE
#include <sysxcd.h>
#endif
#ifndef YSX_ORACLE
#include <ysx.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif
#ifndef YSLOG_ORACLE
#include <yslog.h>
#endif
#ifndef YSFE_ORACLE
#include <ysfe.h>
#endif
#ifndef CTYPE_ORACLE
#include <ctype.h>
#endif
#ifndef YSMSG_ORACLE
#include <ysmsg.h>
#endif



typedef struct yeevcx yeevcx;	
typedef struct yeevdx yeevdx;	
typedef struct yeevdlx yeevdlx;	
typedef struct yeevrx yeevrx;	
typedef struct yeevdn yeevdn;	
typedef struct yeevoe yeevoe;	
typedef struct yeevmd yeevmd;	
typedef struct yeevpw yeevpw;	

static CONST_W_PTR struct yeevDiscList__tyimpl yeevDiscList__impl =
 {
  yeevDiscList_destroy_i,
  yeevDiscList_replace_i,
  yeevDiscList_append_i,
  yeevDiscList_remove_i,
  yeevDiscList_listDest_i,
  yeevDiscList_replaceDest_i,
  yeevDiscList_destroyDest_i,
  yeevDiscList__get_dlist_i,
  yeevDiscList__get_numEntries_i
 };

static CONST_W_PTR struct yeevReceiver__tyimpl yeevReceiver__impl =
 {
  yeevReceiver_destroy_i,
  yeevReceiver_push_i,
  yeevReceiver_pushMany_i,
  yeevReceiver_pull_i,
  yeevReceiver_pullMany_i,
  yeevReceiver_tryPull_i,
  yeevReceiver_tryPullMany_i,
  yeevReceiver__get_name_i
 };

static CONST_W_PTR struct yeev__tyimpl yeev__impl =
 {
  yeev_createReceiver_i,
  yeev_createDiscList_i,
  yeev_raise_i,
  yeev_raiseMany_i,
  yeev_forward_i,
  yeev__get_info_i,
  yeev__get_receivers_i,
  yeev__get_filters_i,
  yeev__set_filters_i,
  yeev__get_limit_drop_i,
  yeev__set_limit_drop_i,
  yeev__get_globalEventHighWater_i,
  yeev__set_globalEventHighWater_i,
  yeev__get_globalEventRestart_i,
  yeev__set_globalEventRestart_i
 };




struct yeevrx
{
  yeevReceiver	self_yeevrx;
  char		*name_yeevrx;	
  yeevHndlr	hndlr_yeevrx;	
  dvoid		*usrp_yeevrx;	
  yslst		*q_yeevrx;	
  yslst		*evq_yeevrx;	
  ysle		*le_yeevrx;	
};


struct yeevdx
{
  boolean   valid_yeevdx;	
  yeevd	    qual_yeevdx;	
  ysfe	    *cqual_yeevdx;	
  ysle	    *le_yeevdx;		
};


struct yeevmd
{
  ysspNode  node_yeevmd;	
  yslst	    *recs_yeevmd;	
  sb4	    noi_yeevmd;		
  sb4	    noe_yeevmd;		
  boolean   dead_yeevmd;	
}; 


struct yeevdn
{
  ysspNode  node_yeevdn;	
  yslst	    *quals_yeevdn;	
  yeevdlx   *dl_yeevdn;		
};


struct yeevdlx
{
  yeevDiscList	self_yeevdlx;
  ysspTree	dests_yeevdlx;	
  ysle		*le_yeevdlx;	
};


struct yeevoe
{
  yeevcx    *cx_yeevoe;
  yeevmd    *md_yeevoe;		
  sb4	    noe_yeevoe;		
  yoenv	    env_yeevoe;		
};

struct yeevpw
{
  char *ptr_yeevpw;		
  char buf_yeevpw[1];		
} ;



typedef enum
{
  none_yeevcs,
  logimpl_yeevcs,
  default_yeevcs,
  connected_yeevcs
} yeevcs;


struct yeevcx
{
  ysque	    *q_yeevcx;
  yeev	    self_yeevcx;
  sb4	    trace_yeevcx;	
  yoenv	    env_yeevcx;		

  yssnk	    *sink_yeevcx;	

  yeevdlx   *fcx_yeevcx;	

  ysevt	    *dsem_yeevcx;	
  sb4	    noe_yeevcx;		
  boolean   limited_yeevcx;	
  boolean   doDrop_yeevcx;	
  sb4	    ndropped_yeevcx;	

  char	    *logimpl_yeevcx;	

  yeevl_sender sender_yeevcx;	
  yeevcs       cstate_yeevcx;	
  boolean      csync_yeevcx;	
  yslst	       *cwait_yeevcx;	
  yoenv	       cenv_yeevcx;	
  yeevdSeq     quals_yeevcx;	
    
  yeevReceiver	logr_yeevcx;	
  boolean   shutdown_yeevcx;	

  yeevReceiver	panic_yeevcx;	
  sb4   inSend_yeevcx;	

  

  yeevInfo  info_yeevcx;
  yeevDiscList	filters_yeevcx;	
  ub4	    globLimit_yeevcx;	
  ub4	    globRestart_yeevcx;	
  
  yslst	    *dls_yeevcx;	
  yslst	    *rcvs_yeevcx;	
  ysspTree  dests_yeevcx;	
  ysfemap   *map_yeevcx;	
  sword	    nmap_yeevcx;	
};



#define YEEV_DEF_GLOBAL_LIMIT (200)


#define YEEV_DEF_LOG_IMPL   ((char*)0)


#define YEEV_FAC    "YEEV"


#define YEEV_TIME_DIGS 3

externdef ysidDecl(YEEV_EX_BAD_SINK) = "yeev::badsink";
externdef ysidDecl(YEEV_EX_ALREADY_SUNK) = "yeev::already_sunk";
externdef ysidDecl(YEEV_EX_NOLOG) = "yeev::nolog";
externdef ysidDecl(YEEV_EX_BAD_CSTATE) = "yeev::bad_cstate";

externdef ysmtagDecl(yeevcx_tag) = "yeevcx";
externdef ysmtagDecl(yeevch_tag) = "yeevch";
externdef ysmtagDecl(yeevdx_tag) = "yeevdx";
externdef ysmtagDecl(yeevdlx_tag) = "yeevdlx";
externdef ysmtagDecl(yeevrx_tag) = "yeevrx";
externdef ysmtagDecl(yeevr_tag) = "yeevr";
externdef ysmtagDecl(yeevdn_tag) = "yeevdn";
externdef ysmtagDecl(yeevoe_tag) = "yeevoe";

#define YEEVFEVAL_ORIG		(YSLOGFEVAL_MAX)
#define YEEVFEVAL_ORIGSEQ	(YSLOGFEVAL_MAX + 1)
#define YEEVFEVAL_TIME		(YSLOGFEVAL_MAX + 2)
#define YEEVFEVAL_HOST		(YSLOGFEVAL_MAX + 3)
#define YEEVFEVAL_PID		(YSLOGFEVAL_MAX + 4)
#define YEEVFEVAL_AFF		(YSLOGFEVAL_MAX + 5)
#define YEEVFEVAL_FORW		(YSLOGFEVAL_MAX + 6)
#define YEEVFEVAL_HOPS		(YSLOGFEVAL_MAX + 7)
#define YEEVFEVAL_PROG		(YSLOGFEVAL_MAX + 8)
#define YEEVFEVAL_MAX		(YSLOGFEVAL_MAX + 9)

STATICF dvoid *yeevFESb8Parse( CONST char *str );
STATICF sword yeevFESb8Cmp( CONST dvoid *a, CONST dvoid*b );    

static CONST_DATA ysfemap yeevFEmap[] =
{
  { "orig", YEEVFEVAL_ORIG,
     yoStrToRef,	yoRelease,      yoIsEq,		ysFENoRelop },
  { "forw", YEEVFEVAL_FORW,
      yoStrToRef,	yoRelease,	yoIsEq,		ysFENoRelop },
  { "origseq", YEEVFEVAL_ORIGSEQ,
      ysFEIntParse,	ysFEDfltFree,	ysFENoPred,	ysFEIntRelop },
  { "time", YEEVFEVAL_TIME,
      yeevFESb8Parse,	ysFEDfltFree,	ysFENoPred,	yeevFESb8Cmp },
  { "host", YEEVFEVAL_HOST,
      ysFEDfltParse,	ysFEDfltFree,	ysFEStrPred,	ysFENoRelop },
  { "pid", YEEVFEVAL_PID,
      ysFEDfltParse,	ysFEDfltFree,	ysFEStrPred,	ysFENoRelop },
  { "aff", YEEVFEVAL_AFF,
      ysFEDfltParse,	ysFEDfltFree,	ysFEStrPred,	ysFENoRelop },
  { "prog", YEEVFEVAL_PROG,
      ysFEDfltParse,	ysFEDfltFree,	ysFEStrPred,	ysFENoRelop },
  { "hops", YEEVFEVAL_HOPS,
      ysFEIntParse,	ysFEDfltFree,	ysFENoPred,	ysFEIntRelop },
};




STATICF void yeSinkTty(dvoid *rusrp, yslrec *rec);

STATICF char *yeevCopyCacheStr( CONST char *s );
STATICF void yeevFreeCacheStr( char *s );

STATICF ub4 yeevNQual( yeevdlx *dl );
STATICF void yeevCompileQual( yeevcx *cx, yeevdx *d );
STATICF boolean yeevEvaluateList( yslst *dl, yeevr* rec);

STATICF yeevdn *yeevDestList( yeevdlx *dl, yeevReceiver r );
STATICF void yeevAddToDestList( yeevcx *cx, yeevdn *dn, yeevd *qual );
STATICF void yeevFreeDestQuals( yeevdn *dn );
STATICF void yeevFreeDest( yeevdn *dn );
STATICF void yeevFreeQual( dvoid *x );
STATICF void yeevFreeRec( dvoid *r );
STATICF void yeevFreeReceiver( dvoid *x );
STATICF void yeevFreeDiscList( dvoid *x );
STATICF void yeevFreeDiscEnts( dvoid *x );

STATICF void yeevGetLog( yeevcx *cx, boolean sync );
STATICF void yeevLogHandler( dvoid *usrp, CONST ysid *exid,
			    dvoid *arg, size_t argsz );
STATICF void yeevEConnect( yeevcx *cx, dvoid *arg );
STATICF void yeevBadDest( yeevcx *cx, yeevmd *dest );

STATICF boolean yeevSend( yeevcx *cx, yeevr *rec );
STATICF void yeevComplete(dvoid *usrp, CONST ysid *exid,
			  dvoid *arg, size_t argsz);

STATICF void yeevDummy(dvoid *usrp, CONST ysid *exid,
			  dvoid *arg, size_t argsz);

STATICF void yeevYsSink( dvoid *rusrp, yslrec *rec );

STATICF yeevmd* yeevManyDestGet( yeevcx *cx, yeevReceiver r );
STATICF void yeevManyDestBatch( yeevmd *md );
STATICF void yeevManyDestSend( yeevcx *cx, yeevmd *md );
STATICF void yeevManyDestFree( yeevcx *cx, yeevmd *md );

STATICF void yeevAllBatch( yeevcx *cx );
STATICF void yeevAllSend( yeevcx *cx );

STATICF void yeevDestDeath( dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz );









yeev yeevInit( ysque *q )
{
  yeevcx   *cx = (yeevcx*)ysmGlbAlloc( sizeof(*cx), yeevcx_tag );
  char	    *arg;
  yeevdSeq  quals;
  yeevDiscList filters;

  CLRSTRUCT(*cx);
  arg = ysResGetLast("yeev.trace-level");
  cx->trace_yeevcx = arg ? atoi(arg) : 0;
  cx->q_yeevcx = q;
  cx->rcvs_yeevcx = ysLstCreate();
  cx->dls_yeevcx = ysLstCreate();
  DISCARD ysspNewTree( &cx->dests_yeevcx, yoCmp );

  
  cx->map_yeevcx = (ysfemap*)0;
  cx->nmap_yeevcx = 0;

  yoEnvInit( &cx->env_yeevcx );

  cx->sink_yeevcx = (yssnk*)0;
  cx->info_yeevcx.host_yeevInfo = yeevCopyCacheStr((char*)ysGetHostName());
  cx->info_yeevcx.pid_yeevInfo = yeevCopyCacheStr((char*)ysGetPid());
  cx->info_yeevcx.affinity_yeevInfo = yeevCopyCacheStr((char*)ysGetAffinity());
  cx->info_yeevcx.prog_yeevInfo = yeevCopyCacheStr((char*)ysProgName());

  
  cx->globLimit_yeevcx = (arg=ysResGetLast("yeev.global-limit")) ?
    (ub4)atol(arg) : YEEV_DEF_GLOBAL_LIMIT;
  cx->globRestart_yeevcx = (arg=ysResGetLast("yeev.global-restart")) ?
    (ub4)atol(arg) : (cx->globLimit_yeevcx - (cx->globLimit_yeevcx / 5));

  cx->shutdown_yeevcx = FALSE;
  cx->doDrop_yeevcx = TRUE;
  cx->inSend_yeevcx = 0;

  

  yoSetImpl( yeev__id, (char*)0, yeev__stubs,
	    (dvoid*)&yeev__impl, (yoload)0, FALSE, (dvoid*)cx );

  yoSetImpl( yeevReceiver__id, (char*)0, yeevReceiver__stubs,
	    (dvoid*)&yeevReceiver__impl, (yoload)0, FALSE, (dvoid*)cx );

  yoSetImpl( yeevDiscList__id, (char*)0,
	    yeevDiscList__stubs, (dvoid*)&yeevDiscList__impl,
	    (yoload)0, FALSE, (dvoid*)cx );

  cx->self_yeevcx = (yeev)yoCreate( yeev__id, (char*)0 ,
				    (yoRefData*)0, 
				    (char*)0, (dvoid*)0);
  
  
  quals._length = quals._maximum = 0;
  quals._buffer = (yeevd*)0;
  filters = yeev_createDiscList_i( cx->self_yeevcx, &cx->env_yeevcx, &quals );
  yeev__set_filters_i( cx->self_yeevcx, &cx->env_yeevcx, filters );

  yoImplReady( yeev__id, (char*)0, cx->q_yeevcx );
  yoImplReady( yeevReceiver__id, (char*)0, cx->q_yeevcx );
  yoImplReady( yeevDiscList__id, (char*)0, cx->q_yeevcx );
  
  
  cx->logimpl_yeevcx = (char*)ysResGetLast("yeev.log-impl");
  cx->sender_yeevcx = (yeevl_sender)0;
  cx->cstate_yeevcx = none_yeevcs;
  cx->csync_yeevcx = FALSE;
  cx->cwait_yeevcx = ysLstCreate();
  yoEnvInit( &cx->cenv_yeevcx );

  

  return( cx->self_yeevcx );
}




void yeevTerm( yeev ev )
{
  yeevcx *cx = (yeevcx*)yoGetImplState( (dvoid*)ev );
  ysspNode *n, *next;
  ysevt *evt;

  cx->shutdown_yeevcx = TRUE; 

  
  yeevAllSend( cx );

  
  while ((cx->cstate_yeevcx == logimpl_yeevcs) 
	 || (cx->cstate_yeevcx == default_yeevcs))
    ysYield();

  
  if( cx->noe_yeevcx && cx->trace_yeevcx )
    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)150, YSLSEV_DEBUG(0), (char*)0, 
	     YSLSB4(cx->noe_yeevcx), YSLEND );

  while( cx->noe_yeevcx )
    ysYield();

  
  if( cx->sink_yeevcx )
    ysSinkDestroy( cx->sink_yeevcx );

  yeevDiscList_destroy( cx->filters_yeevcx, &cx->env_yeevcx );
  
  if( cx->sender_yeevcx )
  {
    
      
    

    evt = ysEvtCreate( yeevDummy, (dvoid*)0, (ysque*)0, TRUE );
    yeevl_sender_destroy_nw( cx->sender_yeevcx, &cx->env_yeevcx, evt );
    ysEvtDestroy( evt );
			
    yoRelease((dvoid*)cx->sender_yeevcx);
    cx->sender_yeevcx = (yeevl_sender)0;
  }

  ysLstDestroy( cx->cwait_yeevcx, (ysmff)ysEvtDestroy );
  
  yoDispose( (dvoid*)ev );
  yoImplDeactivate( yeev__id, (char*)0 );

  if( cx->noe_yeevcx )
  {
    
    cx->dsem_yeevcx = ysSemCreate((dvoid*)0 );
    ysSemWait( cx->dsem_yeevcx );
    ysSemDestroy( cx->dsem_yeevcx );
  }

  

  ysLstDestroy( cx->dls_yeevcx, yeevFreeDiscList ); 
  ysLstDestroy( cx->rcvs_yeevcx, yeevFreeReceiver );

  for( n = ysspFHead(  &cx->dests_yeevcx ); n ; n = next )
  {
    next = ysspFNext( n );
    yeevManyDestFree( cx, (yeevmd*)n );
  }

  
  yoImplDeactivate( yeevReceiver__id, (char*)0 );
  yoImplDeactivate( yeevDiscList__id, (char*)0 ); 

  if( cx->map_yeevcx )
    ysmGlbFree( (dvoid*)cx->map_yeevcx );
  
  yoEnvFree( &cx->env_yeevcx );

  ysmGlbFree( (dvoid*)cx );
}




boolean yeevHaveLogger( yeev ev )
{
  yeevcx *cx = (yeevcx*)yoGetImplState( (dvoid*)ev );

  if( cx->cstate_yeevcx != connected_yeevcs )
    yeevGetLog( cx, TRUE );

  return( cx->cstate_yeevcx == connected_yeevcs );
}




yeevHndlr yeevReceiverSetHandler(yeevReceiver or,
				 yeevHndlr hndlr, dvoid *usrp)
{
  yeevrx *rcv = (yeevrx*)yoGetState((dvoid*)or);
  yeevHndlr oh = rcv->hndlr_yeevrx;
  yeevr *rec;

  rcv->hndlr_yeevrx = hndlr;
  rcv->usrp_yeevrx = usrp;

  if( hndlr && ysLstCount( rcv->q_yeevrx ) )
  {
    
    while( (rec = (yeevr*)ysLstDeq( rcv->q_yeevrx ) ) )
    {
      (*hndlr)( rec, usrp );
      yeevr__free( rec, yotkFreeStr );
      ysmGlbFree( (dvoid*)rec );
    }
  }
  return( oh );
}


void yeevBatchStart( yeev ev )
{
  yeevcx *cx = (yeevcx*)yoGetImplState( (dvoid*)ev );
  yeevAllBatch( cx );
}



void yeevBatchSend( yeev ev )
{
  yeevcx *cx = (yeevcx*)yoGetImplState( (dvoid*)ev );
  yeevAllSend( cx );
}





void yeevSinkAttach(yeev evr, ub4 level)
{
  yeevcx    *cx = (yeevcx*)yoGetImplState((dvoid*)evr);
  ub4	    ylevel;
  char	    buf[ 100 ];

  
  if( cx->cstate_yeevcx != connected_yeevcs )
    yeevGetLog( cx, TRUE );

  

  ylevel = level;
  if ( level > (YSLSEV_MAX - 2) )
    ylevel = YSLSEV_MAX - 2;
  ysFmtStr( buf, "maxsev %d", ylevel );

  if( cx->sink_yeevcx )
    ysePanic( YEEV_EX_ALREADY_SUNK );
  else
    cx->sink_yeevcx = ysSinkCreate( "yeevSink", yeevYsSink, (dvoid*)cx );

  ysSinkSetFilter( cx->sink_yeevcx, ysFilterSimple, (dvoid*)0,
		  (ysFilterCB*)0, (dvoid**)0 );
  ysAddFilter( cx->sink_yeevcx, buf );
}



void yeevSinkDetach( yeev evr )
{
  yeevcx    *cx = (yeevcx*)yoGetImplState((dvoid*)evr);

  if( !cx->sink_yeevcx )
  {
    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)160, YSLSEV_WARNING, (char*)0,
	     YSLNONE);
    yseThrow( YEEV_EX_BAD_SINK );
  }
  else
  {
    ysSinkDestroy( cx->sink_yeevcx );
    cx->sink_yeevcx = (yssnk*)0;
  }
}


boolean yeevEvaluate( yeevr *rec, yeevDiscList dl )
{
  yeevdlx *dlx = (yeevdlx*)yoGetState( (dvoid*)dl );
  ysspNode  *n;
  yeevdn *dn;
  
  boolean rv = FALSE;

  
  for( n = ysspFHead( &dlx->dests_yeevdlx ) ; n && !rv; n = ysspFNext(n) )
  {
    dn = (yeevdn*)n;
    rv = yeevEvaluateList( dn->quals_yeevdn, rec );
  }
  return( rv );
}







yeev yeevRemoteLog( yeev ev, ysque *q )
{
  yssnk *sink;
  yslst *lst;
  ysle *le;

  if( !ev )
    ev = yeevInit( q );

  if(yeevHaveLogger(ev))
  {
    yeevSinkAttach(ev, YSLSEV_DEBUG(8));

    

    if((sink=ysSinkFind("tty")))
      ysSinkDestroy(sink);

    

    lst = ysResGet("ye.log.tty.filter");
    if( lst && ysLstCount(lst) )
    {
      sink = ysSinkCreate("tty", yeSinkTty, yoGetImplState((dvoid*)ev));
      ysSinkSetFilter(sink, ysFilterSimple, (dvoid *) 0,
		      (ysFilterCB *) 0, (dvoid **) 0);
      for (le = ysLstHead(lst); le; le = ysLstNext(le))
	ysAddFilter(sink, (char *) ysLstVal(le));
    }
  }
  return( ev );
}









STATICF  void yeSinkTty(dvoid *rusrp, yslrec *rec)
{
  yeevcx *cx = (yeevcx*)rusrp;
  ysmsgd *msgd;
  char   *bufp, buf[512], out[512];
  CONST char	*assoc;
  ystm	tm;
  char tbuf[ YSTM_BUFLEN ];
  sysb8	now;
 
  if (rec)
    {
      msgd = ysMsgFind(rec->prod, rec->fac);
      bufp = ysMsgGet(msgd, rec->msgid, buf, sizeof(buf));
      ysMsgFmt(out, sizeof(out), (sword)rec->argvec.narg,
	       rec->argvec.args, bufp);

      ysClock(&now);
      ysConvClock( &now, &tm );
      DISCARD ysStrClock( tbuf, &tm, FALSE, YEEV_TIME_DIGS );
      assoc = rec->assoc && *rec->assoc ? rec->assoc : (CONST char*)0;

      yslError("%s %d %s:%s%s%s %s %s-%d %d %s%s%s { %s }\n",
	       tbuf,
	       rec->seqid,
	       cx->info_yeevcx.host_yeevInfo,
	       cx->info_yeevcx.pid_yeevInfo,
	       cx->info_yeevcx.affinity_yeevInfo ? ":" : "",
	       cx->info_yeevcx.affinity_yeevInfo ?
	       cx->info_yeevcx.affinity_yeevInfo : "",
	       cx->info_yeevcx.prog_yeevInfo,
	       rec->prod, rec->msgid, rec->sev,
	       assoc ? "[" : "",
	       assoc ? assoc : "",
	       assoc ? "] " : "",
	       out);
    }
}





STATICF char *yeevCopyCacheStr( CONST char *s )
{
  char *rv = (char*)0;

  if( s )
    yotkCopyVal( yoTcString, (dvoid*)&rv, (dvoid*)&s, yotkAllocStr );  

  return rv;
}




STATICF void yeevFreeCacheStr( char *s )
{
  yotkFreeVal( yoTcString, (dvoid*)&s, yotkFreeStr );
}






STATICF void yeevYsSink( dvoid *rusrp, yslrec *lrec )
{
  yeevcx *cx = (yeevcx*)rusrp;
  yeevr rec;
  yeevYsLogEvent yevt;

  if( !lrec )
    return;

  
  rec.origseq_yeevr = lrec->seqid;
  rec.fac_yeevr = (char*)lrec->fac;
  rec.prod_yeevr = (char*)lrec->prod;
  rec.assoc_yeevr = (char*)lrec->assoc;
  rec.sev_yeevr = lrec->sev;
  rec.msgid_yeevr = lrec->msgid;

  rec.val_yeevr._type = YCTC_yeevYsLogEvent;
  rec.val_yeevr._value = (dvoid*)&yevt;
  yevt.seqid = lrec->seqid;
  yevt.vals._length = lrec->argvec.narg;
  yevt.vals._buffer = (yoany*)lrec->argvec.args;

  
  yeev_raise_i( cx->self_yeevcx, &cx->env_yeevcx, &rec );
}





STATICF yeevdn *yeevDestList( yeevdlx *dl, yeevReceiver r )
{
  yeevdn *dn;

  if( !(dn = (yeevdn*)ysspLookup( (dvoid*)r, &dl->dests_yeevdlx ) ) )
  {
    dn = (yeevdn*)ysmGlbAlloc( sizeof(*dn), yeevdn_tag );
    dn->node_yeevdn.key_ysspNode = (dvoid*)yoDuplicate(r);
    dn->quals_yeevdn = ysLstCreate();
    dn->dl_yeevdn = dl;
    DISCARD ysspEnq( &dn->node_yeevdn, &dl->dests_yeevdlx ); 

  }
  return dn;
}





STATICF void yeevAddToDestList( yeevcx *cx, yeevdn *dn, yeevd *qual )
{
  yeevdx *dx = (yeevdx*)ysmGlbAlloc(sizeof(*dx), yeevdx_tag );

  dx->valid_yeevdx = FALSE;
  yeevd__copy( &dx->qual_yeevdx, qual, yotkAllocStr );
  yeevCompileQual( cx, dx );
  dx->le_yeevdx = ysLstEnq( dn->quals_yeevdn, (dvoid*)dx );
}




STATICF ub4 yeevNQual( yeevdlx *dl )
{
  ysspNode *n;
  yeevdn *dn;
  ub4 rv = 0;
  
  for( n = ysspFHead( &dl->dests_yeevdlx ) ; n ; n = ysspFNext( n ) )
  {
    dn = (yeevdn*)n;
    rv += ysLstCount( dn->quals_yeevdn );
  }
  return( rv );
}




STATICF void yeevCompileQual( yeevcx *cx, yeevdx *dx )
{
  ysfe	*fe;
  CONST ysfemap   *map;
  sword nmap;

  yeev_qual_invalid pex;

  if( !cx->map_yeevcx )
  {
    map = ysLogGetMap( &nmap );
    cx->map_yeevcx = (ysfemap*)ysmGlbAlloc( (nmap + sizeof(yeevFEmap) /
					     sizeof(*map)) * sizeof(*map),
					   "yeev fe map" ); 
    DISCARD memcpy( (dvoid*)cx->map_yeevcx, (dvoid*)map, nmap * sizeof(*map));
    DISCARD memcpy( (dvoid*)(cx->map_yeevcx + nmap), (dvoid*)yeevFEmap,
		   sizeof(yeevFEmap) );
    cx->nmap_yeevcx = (sword)(nmap + (sizeof(yeevFEmap) / sizeof(*map)));
  }

  
  if( dx->qual_yeevdx.qual_yeevd && !*dx->qual_yeevdx.qual_yeevd )
  {
    yeevFreeCacheStr( dx->qual_yeevdx.qual_yeevd );
    dx->qual_yeevdx.qual_yeevd = (char*)0;
  }
  if( !dx->qual_yeevdx.qual_yeevd )
    dx->qual_yeevdx.qual_yeevd = yeevCopyCacheStr( "maxsev 16" );

  if( !( fe = ysFECompile( dx->qual_yeevdx.qual_yeevd,
			  cx->nmap_yeevcx, cx->map_yeevcx )) )
  {
    dx->valid_yeevdx = FALSE;

    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)170, YSLSEV_INFO, (char*)0,
	     YSLSTR(dx->qual_yeevdx.qual_yeevd), YSLEND);
    pex.qual = dx->qual_yeevdx.qual_yeevd;
    yseThrowObj( EX_YEEV_QUAL_INVALID, pex );
  }
  dx->valid_yeevdx = TRUE;
  dx->cqual_yeevdx = fe;
}




STATICF boolean yeevEvaluateList( yslst *dl, yeevr* rec )
{
  ysle *e;
  yeevdx *dx;

  dvoid *vals[YEEVFEVAL_MAX];
  
  vals[YSLOGFEVAL_PROD] = (dvoid *) rec->prod_yeevr;
  vals[YSLOGFEVAL_FAC] = (dvoid *) rec->fac_yeevr;
  vals[YSLOGFEVAL_MSGID] = (dvoid*)&rec->msgid_yeevr;
  vals[YSLOGFEVAL_SEV] = (dvoid*)&rec->sev_yeevr;
  vals[YSLOGFEVAL_ASSOC] =
    (dvoid *) (rec->assoc_yeevr ? rec->assoc_yeevr : "");

  vals[YSLOGFEVAL_SEQID] = (dvoid*)&rec->origseq_yeevr;

  vals[ YEEVFEVAL_ORIG ] = (dvoid*)rec->orig_yeevr;
  vals[ YEEVFEVAL_ORIGSEQ ] = (dvoid*)&rec->origseq_yeevr;
  vals[ YEEVFEVAL_TIME ] = (dvoid*)&rec->origtime_yeevr;
  vals[ YEEVFEVAL_HOST ] = (dvoid*)rec->orighost_yeevr;
  vals[ YEEVFEVAL_PID ] =  (dvoid*)rec->origpid_yeevr;
  vals[ YEEVFEVAL_AFF ] = (dvoid*)rec->origaff_yeevr;
  vals[ YEEVFEVAL_FORW ] = (dvoid*)rec->orig_yeevr;
  vals[ YEEVFEVAL_HOPS ] = (dvoid*)&rec->hops_yeevr;
  vals[ YEEVFEVAL_PROG ] = (dvoid*)rec->origprog_yeevr;

  for( e = ysLstHead( dl ); e ; e = ysLstNext( e ) )
  {
    dx = (yeevdx*)ysLstVal(e);
    if( !dx->valid_yeevdx )
      continue;

    if( ysFEEval(dx->cqual_yeevdx, YEEVFEVAL_MAX, vals ) )
      return( TRUE );
  }
  return( FALSE );
}



STATICF void yeevFreeDestQuals( yeevdn *dn )
{
  dvoid *x;

  while( (x = ysLstDeq( dn->quals_yeevdn ) ) )
    yeevFreeQual( x );
}





STATICF void yeevFreeQual( dvoid *x )
{
  yeevdx *dx = (yeevdx*)x;

  yeevd__free( &dx->qual_yeevdx, yotkFreeStr );
  if( dx->valid_yeevdx )
    ysFEFree( dx->cqual_yeevdx );
  ysmGlbFree( x );
}




STATICF void yeevFreeDest( yeevdn *dn )
{
  yoRelease(dn->node_yeevdn.key_ysspNode);
  ysLstDestroy( dn->quals_yeevdn, yeevFreeQual );
  ysspRemove( &dn->node_yeevdn, &dn->dl_yeevdn->dests_yeevdlx );
  ysmGlbFree( (dvoid*)dn );
}




STATICF void yeevFreeRec( dvoid *x )
{
  yeevr__free( (yeevr*)x, yotkFreeStr );
  ysmGlbFree( (dvoid *) x );
}




STATICF void yeevFreeReceiver( dvoid *x )
{
  yeevrx *r = (yeevrx*)x;

  yoDispose((dvoid*)r->self_yeevrx);
  ysLstDestroy( r->evq_yeevrx, (ysmff)ysEvtDestroy );
  ysLstDestroy( r->q_yeevrx, yeevFreeRec );
  if( r->name_yeevrx )
    ysmGlbFree((dvoid*)r->name_yeevrx);
  ysmGlbFree( (dvoid*)r );
}





STATICF void yeevFreeDiscList( dvoid *x )
{
  yeevdlx *dl = (yeevdlx *)x;

  yeevFreeDiscEnts( x );
  yoDispose((dvoid*)dl->self_yeevdlx);
  ysmGlbFree( (dvoid*)dl );
}





STATICF void yeevFreeDiscEnts( dvoid *x )
{
  yeevdlx *dl = (yeevdlx *)x;
  yeevdn *dn;

  while( (dn = (yeevdn*)ysspDeq( &dl->dests_yeevdlx.root_ysspTree )) )
  {
    yoRelease(dn->node_yeevdn.key_ysspNode);
    ysLstDestroy( dn->quals_yeevdn, (ysmff)yeevFreeQual );
    ysmGlbFree( (dvoid*)dn );
  }
}





STATICF void yeevGetLog( yeevcx *cx, boolean sync )
{
  ysevt *sem = (ysevt*)0;

  if( sync && cx->cstate_yeevcx != connected_yeevcs )
  {
    

    if( cx->cstate_yeevcx == none_yeevcs )
      cx->csync_yeevcx = TRUE;
    else
      DISCARD ysLstEnq( cx->cwait_yeevcx,
		       (dvoid*)(sem = ysSemCreate( (dvoid*)0 ))); 
  }

  switch( cx->cstate_yeevcx )
  {
  case none_yeevcs:    
    yeevLogHandler( (dvoid*)cx, YEEV_EX_NOLOG, (dvoid*)0, (size_t)0 );
    break;

  case logimpl_yeevcs:
  case default_yeevcs:
  case connected_yeevcs:
    
    break;

  default:
    yslError("yeevGetLog: unexpected cstate %d\n", cx->cstate_yeevcx);
    ysePanic( YEEV_EX_BAD_CSTATE );
  }

  if( cx->csync_yeevcx )
    cx->csync_yeevcx = FALSE;   

  if( sem )
  {
    ysSemWait( sem );
    ysSemDestroy( sem );
  }
}




STATICF void yeevLogHandler( dvoid *usrp, CONST ysid *exid,
			    dvoid *arg, size_t argsz )
{
  yeevcx	*cx = (yeevcx*)usrp;
  yeevl_logProc noreg lg = (yeevl_logProc)0;
  ysevt		*sem;
  yeevl_sender	sender;

  NOREG(lg);

  switch( cx->cstate_yeevcx )
  {
  case none_yeevcs:		

    if( cx->logimpl_yeevcx )
    {
      
      cx->cstate_yeevcx = logimpl_yeevcs;
      lg = (yeevl_logProc) yoBind( yeevl_logProc__id, cx->logimpl_yeevcx,
				  (yoRefData*)0, (char*)0 );
      break;
    }
    

  case logimpl_yeevcs:		

    if( !exid )
      yeevEConnect( cx, arg );	
    else
    {
      
      cx->cstate_yeevcx = default_yeevcs;
      lg = (yeevl_logProc)yoBind( yeevl_logProc__id, (char*)YEEV_DEF_LOG_IMPL,
				 (yoRefData*)0, (char*)0 );
    }
    break;

  case default_yeevcs:		

    if( !exid )
      yeevEConnect( cx, arg );	
    else
    {
      
      cx->cstate_yeevcx = none_yeevcs;
    }
    break;
    

    
  case connected_yeevcs:
    lg = (yeevl_logProc)0;
    break;
    
  default:

    yslError("yeevLogHandler: unexpected state %d\n", cx->cstate_yeevcx);
    ysePanic( YEEV_EX_BAD_CSTATE );
    break;
  }

  
  if( lg )
  {
    if( cx->csync_yeevcx )
    {
      yseTry
      {
	sender =
	  yeevl_logProc_connectSender( lg, &cx->env_yeevcx,
				      cx->self_yeevcx,
				      cx->info_yeevcx.host_yeevInfo,
				      cx->info_yeevcx.pid_yeevInfo,
				      cx->info_yeevcx.affinity_yeevInfo,
				      cx->info_yeevcx.prog_yeevInfo,
				      &cx->quals_yeevcx );
	arg = (dvoid*)&sender; 
	yeevEConnect( cx, arg );
	yoRelease( (dvoid*)sender );
      }
      yseCatch(YO_EX_BADOBJ)	;
      yseCatch(YEEVL_EX_OPERATIONFAILED) ;
      yseEnd;

      
      if( cx->cstate_yeevcx != connected_yeevcs )
	yeevLogHandler( (dvoid*)cx, YEEV_EX_NOLOG, (dvoid*)0, (size_t)0 );
    }
    else
    {
      yeevl_logProc_connectSender_nw( lg, &cx->env_yeevcx,
				     cx->self_yeevcx,
				     cx->info_yeevcx.host_yeevInfo,
				     cx->info_yeevcx.pid_yeevInfo,
				     cx->info_yeevcx.affinity_yeevInfo,
				     cx->info_yeevcx.prog_yeevInfo,
				     &cx->quals_yeevcx,
				     ysEvtSimple(yeevLogHandler, (dvoid*)cx));
    }
    yoRelease( (dvoid*)lg );
  }

  
  if( cx->cstate_yeevcx == connected_yeevcs )
  {
    
    while( (sem = (ysevt*)ysLstDeq( cx->cwait_yeevcx ) ) )
      ysTrigger( sem, (ysid*)0, (dvoid*)0, (size_t)0 ); 
  }
}





STATICF void yeevEConnect( yeevcx *cx, dvoid *arg )
{
  cx->sender_yeevcx = (yeevl_sender)yoDuplicate(*(dvoid**)arg);
  cx->cstate_yeevcx = connected_yeevcs;
  cx->logr_yeevcx =
    (yeevReceiver)yoDuplicate((dvoid*)cx->quals_yeevcx._buffer[0].dest_yeevd);
 
  yeevDiscList_append_i( cx->filters_yeevcx, &cx->env_yeevcx,
			&cx->quals_yeevcx );
  yeevdSeq__free( &cx->quals_yeevcx, yoFree );
}





STATICF void yeevBadDest( yeevcx *cx, yeevmd *md )
{
  dvoid *victim;		
  yeevdlx *dl;
  ysle	*e;
  yeevdn    *dn;

  if( !cx->shutdown_yeevcx )
  {
    victim = md->node_yeevmd.key_ysspNode;
    md->dead_yeevmd = TRUE;

#ifdef NEVER
    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)190, YSLSEV_DEBUG(8), (char*)0,
	     YSLPTR(md), YSLPTR(victim), YSLEND );
#endif

    
    if( yoIsEq( (dvoid*)victim, (dvoid*)cx->logr_yeevcx ) )
    {
#ifdef NEVER
      
      ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)191, YSLSEV_DEBUG(8), (char*)0,
	       YSLNONE);
#endif
      cx->sender_yeevcx = (yeevl_sender)0;
      cx->cstate_yeevcx = none_yeevcs;

      
      if( !md->noi_yeevmd )
      {
	yoRelease( (dvoid*)cx->logr_yeevcx );
	
	yoRelease( (dvoid*)cx->sender_yeevcx );
      }
    }

    
    for( e = ysLstHead( cx->dls_yeevcx ) ; e ; e = ysLstNext(e) )
    {
      
      dl = (yeevdlx*) ysLstVal(e);
      if( (dn = (yeevdn*)ysspLookup( victim, &dl->dests_yeevdlx ) ) )
	yeevFreeDest( dn );
    }

    yeevManyDestFree( cx, md );
  }
}




STATICF boolean yeevSend( yeevcx *cx, yeevr *rec )
{
  ysspNode  *n;
  yeevdlx   *dl;
  yeevdn    *dn;
  ysevt	    *evt;
  yeevoe    *oe;
  yowiden dummy = (yowiden)0;
  yeevmd    *md;
  yeevr	    *srec;
  boolean   qualified = FALSE;

  
  if( cx->inSend_yeevcx && !cx->sender_yeevcx )
    return FALSE;

  cx->inSend_yeevcx++;

  

  if( cx->noe_yeevcx > (cx->globLimit_yeevcx / 4) )
    ysYield();

  
  if( !cx->sender_yeevcx )
    yeevGetLog( cx, FALSE );	

  
  dl = cx->fcx_yeevcx;
  for( n = ysspFHead( &dl->dests_yeevdlx ) ; n ; n = ysspFNext(n) )
  {
    dn = (yeevdn*)n;
    if( yeevEvaluateList( dn->quals_yeevdn, rec ) )
    {
      qualified = TRUE;
      md = yeevManyDestGet( cx, (yeevReceiver)n->key_ysspNode );

      
      if( yoLocalObj( (CORBA_Object)n->key_ysspNode, &dummy ) )
      {
#ifdef NEVER
	
	ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)195, YSLSEV_DEBUG(8), (char*)0,
		 YSLNONE);
#endif
	
	yeevReceiver_push((yeevReceiver)n->key_ysspNode, &cx->env_yeevcx, rec);
      }
      else			
      {
	
	if( !cx->limited_yeevcx && (cx->noe_yeevcx > cx->globLimit_yeevcx) )
	{
	  if( !cx->doDrop_yeevcx )
	  {
	    while( cx->noe_yeevcx > cx->globRestart_yeevcx )
	      ysYield();
	  }
	  else
	  {
	    cx->info_yeevcx.nlimits_yeevInfo++; 
	    cx->limited_yeevcx = TRUE;

	    
	    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)200, YSLSEV_ERR, (char*)0,
		     YSLSB4(cx->globLimit_yeevcx), YSLEND );
	  }
	}
	if( cx->limited_yeevcx )
	{
	  cx->ndropped_yeevcx++; 
	  cx->info_yeevcx.ndropped_yeevInfo++; 
	  break;
	}

	if( !md->dead_yeevmd )	
	{
	  cx->noe_yeevcx++;
	  md->noe_yeevmd++;
	  if( md->recs_yeevmd )
	  {
#ifdef NEVER
	    
	    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)201, YSLSEV_DEBUG(8), (char*)0,
		     YSLEND);
#endif
	    
	    srec = (yeevr*)ysmGlbAlloc( sizeof(*srec), "yeevr" );
	    yeevr__copy( srec, rec, yotkAllocStr );
	    DISCARD ysLstEnq( md->recs_yeevmd, (dvoid*)srec );

	    
	    if(ysLstCount( md->recs_yeevmd ) > (cx->globLimit_yeevcx/4) )
	    {
	      yeevManyDestSend( cx, md );
	      md->recs_yeevmd = ysLstCreate();
	    }
	  }
	  else			
	  {
#ifdef NEVER
	    
	    ysRecord(YS_PRODUCT, YEEV_FAC, 202, YSLSEV_DEBUG(8), (char*)0,
		     YSLNONE);
#endif
	    
	    cx->info_yeevcx.npush_yeevInfo++;
	    md->noi_yeevmd++;
	    oe = (yeevoe*)ysmGlbAlloc(sizeof(*oe), yeevoe_tag );
	    oe->md_yeevoe = md;
	    oe->cx_yeevoe = cx;
	    oe->noe_yeevoe = 1;
	    yoEnvInit( &oe->env_yeevoe );

	    evt = ysEvtSimple( yeevComplete, (dvoid*)oe );
	    yeevReceiver_push_nw( (yeevReceiver)md->node_yeevmd.key_ysspNode,
				 &oe->env_yeevoe, rec, evt );
	  }
	}
      }
    }
  }
  cx->inSend_yeevcx--;
  return( qualified );
}



STATICF void yeevDummy(dvoid *usrp, CONST ysid *exid,
			  dvoid *arg, size_t argsz)
{
  
}





STATICF void yeevComplete(dvoid *usrp, CONST ysid *exid,
			  dvoid *arg, size_t argsz)
{
  yeevoe *oe = (yeevoe*)usrp;
  yeevmd *md;
  yeevcx *cx;
  sb4	dropped;

  
  ysmCheck( usrp, yeevoe_tag );
  md = oe->md_yeevoe;
  cx = oe->cx_yeevoe;

  cx->noe_yeevcx -= oe->noe_yeevoe;
  md->noe_yeevmd -= oe->noe_yeevoe;
  md->noi_yeevmd--;

  yoEnvFree( &oe->env_yeevoe );

  if( cx->limited_yeevcx && (cx->noe_yeevcx < cx->globRestart_yeevcx) )
  {
    dropped = cx->ndropped_yeevcx;
    cx->limited_yeevcx = FALSE;
    cx->ndropped_yeevcx = 0;

    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)205, YSLSEV_ERR, (char*)0,
	     YSLSB4(dropped), YSLEND );
  }

  if( exid )
  {
#ifdef NEVER
    
    ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)206, YSLSEV_DEBUG(0), (char*)0,
	     YSLSTR(ysidToStr(exid)), YSLPTR(md), YSLEND );
#endif
    
    yeevBadDest( cx, md );
  }
  else if( md->dead_yeevmd && !md->noi_yeevmd )
  {
    yeevManyDestFree( cx, oe->md_yeevoe );
  }

  
  if( !cx->noe_yeevcx && cx->dsem_yeevcx )
    ysTrigger( cx->dsem_yeevcx, (ysid*)0, (dvoid*)0, (size_t)0 );

  ysmGlbFree( usrp );
}






STATICF yeevmd* yeevManyDestGet( yeevcx *cx, yeevReceiver r )
{
  ysspNode *n;
  yeevmd *md;

  if( (n = ysspLookup((dvoid*)r, &cx->dests_yeevcx )) )
  {
    md = (yeevmd*)n;
  }
  else
  {
    md = (yeevmd*)ysmGlbAlloc(sizeof(*md), "yeevmd");
    md->node_yeevmd.key_ysspNode = yoDuplicate((dvoid*)r);
    md->recs_yeevmd = (yslst*)0;
    md->noe_yeevmd = 0;
    md->noi_yeevmd = 0;
    md->dead_yeevmd = FALSE;
    DISCARD ysspEnq( &md->node_yeevmd, &cx->dests_yeevcx );
  }
  return( md );
}




STATICF void yeevManyDestFree( yeevcx *cx, yeevmd *md )
{
  if( md->noi_yeevmd || (md->recs_yeevmd && ysLstCount(md->recs_yeevmd)) )
  {
    if( !md->dead_yeevmd )	
      yeevManyDestSend( cx, md ); 
    md->dead_yeevmd = TRUE;	
  }

  
  if( !md->noi_yeevmd )
  {
    ysspRemove( &md->node_yeevmd, &cx->dests_yeevcx );
    
    ysmGlbFree( (dvoid*)md );
  }
}





STATICF void yeevManyDestBatch( yeevmd *md )
{
  if( !md->recs_yeevmd )
    md->recs_yeevmd = ysLstCreate();
}




STATICF void yeevManyDestSend( yeevcx *cx, yeevmd *md )
{
  yeevrList recs;
  ub4 i;
  yeevr *rec;
  yeevoe *oe;
  ysevt *evt;

  if( !md->recs_yeevmd )
    return;

  recs._length = recs._maximum = ysLstCount(md->recs_yeevmd);
  if( recs._length )
  {
    recs._buffer = (yeevr*)ysmGlbAlloc(sizeof(yeevr) * (size_t)recs._maximum,
				       "yeevrList");

    
    for( i = 0 ; i < recs._maximum ; i++ )
    {
      rec = (yeevr*)ysLstDeq( md->recs_yeevmd );
      DISCARD memcpy( (dvoid*)&recs._buffer[i], (dvoid*)rec, sizeof(yeevr));

      
      ysmGlbFree( (dvoid*)rec );
    }

    

    cx->info_yeevcx.npushm_yeevInfo++;

    oe = (yeevoe*)ysmGlbAlloc(sizeof(*oe), yeevoe_tag );
    oe->cx_yeevoe = cx;
    oe->md_yeevoe = md;
    oe->noe_yeevoe = (sb4)i;
    md->noi_yeevmd++;
    yoEnvInit( &oe->env_yeevoe );

    evt = ysEvtSimple( yeevComplete, (dvoid*)oe );
    yeevReceiver_pushMany_nw( (yeevReceiver)md->node_yeevmd.key_ysspNode,
			     &oe->env_yeevoe, &recs, evt );

    
    yeevrList__free( &recs, yotkFreeStr );
  }
  ysLstDestroy( md->recs_yeevmd, (ysmff)0 );
  md->recs_yeevmd = (yslst*)0;
}


STATICF void yeevAllBatch( yeevcx *cx )
{
  ysspNode *n;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)210, YSLSEV_DEBUG(8), (char*)0, YSLNONE);

  for( n = ysspFHead( &cx->dests_yeevcx ) ; n ; n = ysspFNext( n ) )
    yeevManyDestBatch( (yeevmd*)n );
}


STATICF void yeevAllSend( yeevcx *cx )
{
  ysspNode *n;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)211, YSLSEV_DEBUG(8), (char*)0, YSLNONE);

  for( n = ysspFHead( &cx->dests_yeevcx ) ; n ; n = ysspFNext( n ) )
    yeevManyDestSend( cx, (yeevmd*)n );
}


STATICF dvoid *yeevFESb8Parse( CONST char *str )
{
  sysb8	*rv = (sysb8*)ysmGlbAlloc( sizeof(*rv), "yeevFEsb8" );

  DISCARD sysb8fromstr( rv, str );
  return (dvoid*)rv;
}


STATICF sword yeevFESb8Cmp( CONST dvoid *a, CONST dvoid*b )
{
  sword rv;

  if( sysb8cmp( (sysb8*)a, <, (sysb8*)b ) )
    rv = -1;
  if( sysb8cmp( (sysb8*)a, ==, (sysb8*)b ) )
    rv = 0;
  else rv = 1;
  
  return rv;
}



STATICF void yeevDestDeath( dvoid *usrp, CONST ysid *exid,
			   dvoid *arg, size_t argsz )
{
#ifdef NEVER
  yeevmd *dest = (yeevmd*)usrp;

  

  yeevBadDest( dest->cx_yeevmd, dest );
#endif
}







void yeevDiscList_destroy_i( yeevDiscList or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)212, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  DISCARD ysLstRem( cx->dls_yeevcx, dl->le_yeevdlx );
  yeevFreeDiscList( yoGetState((dvoid*)or ) );
}

void yeevDiscList_replace_i( yeevDiscList or, yoenv* ev, yeevdSeq* newSeq)
{
  yeevdlx *dl = (yeevdlx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)213, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  
  yeevFreeDiscEnts( (dvoid*)dl );

  
  yeevDiscList_append_i( or, ev, newSeq );
}


void yeevDiscList_append_i( yeevDiscList or, yoenv* ev,
				   yeevdSeq* ds)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevdlx *dl = (yeevdlx*)yoGetState((dvoid*)or);
  ub4 i;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)214, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  for( i = 0; i < ds->_length ; i++ )
    yeevAddToDestList( cx,
		      yeevDestList( dl, ds->_buffer[i].dest_yeevd ),
		      &ds->_buffer[i] );
}


void yeevDiscList_remove_i( yeevDiscList or, yoenv* ev, yeevd* evd)
{
  yeevdlx *dl = (yeevdlx*)yoGetState((dvoid*)or);
  ysle *e, *next;
  yeevdn *dn;
  yeevdx *dx;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)215, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  dn = yeevDestList( dl, evd->dest_yeevd );
  for( e = ysLstHead( dn->quals_yeevdn ) ; e ; e = next )
  {
    next = ysLstNext( e );
    dx = (yeevdx*)ysLstVal(e);
    if( !yeuSafeStrcmp( dx->qual_yeevdx.qual_yeevd, evd->qual_yeevd ) )
    {
      DISCARD ysLstRem( dn->quals_yeevdn, dx->le_yeevdx );
      yeevFreeQual( (dvoid*)dx );
    }
  }
}


void yeevDiscList_listDest_i( yeevDiscList or, yoenv* ev,
			       yeevReceiver dest, yeevdSeq* ds)
{
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);
  ysspNode  *n;
  yeevdn    *dn;
  yeevdx    *dx;
  ysle	    *e = (ysle*)0;
  ub4	    i;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)216, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND);

  ds->_maximum = yeevNQual( dl );
  ds->_buffer = (yeevd*)yoAlloc(sizeof(yeevd) * (size_t)ds->_maximum );
  i = 0;

  if( (n = ysspLookup( (dvoid*)dest, &dl->dests_yeevdlx ) ) )
  {
    dn = (yeevdn*)n;
    for( e = ysLstHead( dn->quals_yeevdn ) ; e ; e = ysLstNext( e ) )
    {
      dx = (yeevdx*)ysLstVal(e);
      yeevd__copy( &ds->_buffer[i], &dx->qual_yeevdx, yoAlloc );
      i++;
    }
  }
  ds->_length = i;
}



void yeevDiscList_replaceDest_i( yeevDiscList or, yoenv* ev,
			       yeevdSeq* ds)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);
  yeevdn    *dn;
  sb4	    i;
  yeevReceiver dest;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)217, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  
  dest = (yeevReceiver)0;
  for( i = 0; i < ds->_length ; i++ )
    if( dest && !yoIsEq( (dvoid*)dest, (dvoid*)ds->_buffer[i].dest_yeevd ) )
      yseThrow( EX_YEEV_NOT_SAME_DEST );
    else
      dest = ds->_buffer[i].dest_yeevd;

  if( (dn = (yeevdn*)ysspLookup( (dvoid*)dest, &dl->dests_yeevdlx ) ) )
    yeevFreeDestQuals( dn );
  else
    dn = yeevDestList( dl, dest );

  for( i = 0; i < ds->_length ; i++ )
    yeevAddToDestList( cx, dn, &ds->_buffer[i] );
}


void yeevDiscList_destroyDest_i( yeevDiscList or, yoenv* ev,
			       yeevReceiver dest)
{
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);
  yeevdn    *dn;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)245, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLPTR(dest), YSLEND );

  if( (dn = (yeevdn*)ysspLookup( (dvoid*)dest, &dl->dests_yeevdlx ) ) )
    yeevFreeDestQuals( dn );
}



yeevdSeq yeevDiscList__get_dlist_i( yeevDiscList or, yoenv* ev)
{
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);
  yeevdSeq  rv;
  ysspNode  *n;
  yeevdn    *dn;
  yeevdx    *dx;
  ysle	    *e = (ysle*)0;
  ub4 i;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)218, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  rv._maximum = yeevNQual( dl );
  rv._buffer = (yeevd*)yoAlloc(sizeof(yeevd) * (size_t)rv._maximum );
  i = 0;
  for( n = ysspFHead( &dl->dests_yeevdlx ) ; n ; n = ysspFNext(n) )
  {
    dn = (yeevdn*)n;
    for( e = ysLstHead( dn->quals_yeevdn ) ; e ; e = ysLstNext( e ) )
    {
      dx = (yeevdx*)ysLstVal(e);
      yeevd__copy( &rv._buffer[i], &dx->qual_yeevdx, yoAlloc );
      i++;
    }
  }
  rv._length = i;
  return(rv);
}


ub4 yeevDiscList__get_numEntries_i( yeevDiscList or, yoenv* ev)
{
  yeevdlx   *dl = (yeevdlx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)219, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  return( yeevNQual( dl ) );
}





void yeevReceiver_destroy_i( yeevReceiver or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevrx *rcv = (yeevrx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)220, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  DISCARD ysLstRem( cx->rcvs_yeevcx, rcv->le_yeevrx );
  yeevFreeReceiver( (dvoid*)rcv );
}




void yeevReceiver_push_i( yeevReceiver or, yoenv* ev, yeevr* rec)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevrx *rcv = (yeevrx*)yoGetState((dvoid*)or);
  yeevr	*qr;

  cx->info_yeevcx.npush_yeevInfo++;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)221, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  
  if( rcv->hndlr_yeevrx )
  {
    (*rcv->hndlr_yeevrx)( rec, rcv->usrp_yeevrx );
  }
  else				
  {
    qr = (yeevr*)ysmGlbAlloc( sizeof(*qr), yeevr_tag );
    yeevr__copy( qr, rec, yotkAllocStr );
    DISCARD ysLstEnq( rcv->q_yeevrx, (dvoid*)qr );
    if( ysLstCount( rcv->evq_yeevrx ) )
      ysTrigger( (ysevt*)ysLstDeq( rcv->evq_yeevrx ), (ysid*)0,
		(dvoid*)0, (size_t)0);
  }
}

void yeevReceiver_pushMany_i( yeevReceiver or, yoenv* ev,
			      yeevrList* recs)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  ub4 i;

  cx->info_yeevcx.npushm_yeevInfo++;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)222, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLSB4(recs->_length), YSLEND );

  for( i = 0 ; i < recs->_length ; i++ )
    yeevReceiver_push_i( or, ev, &recs->_buffer[i] );
}



void yeevReceiver_pull_i( yeevReceiver or, yoenv* ev, yeevr* rec)
{
  yeevcx *cx = (yeevcx*)yoGetState((dvoid*)or);
  yeevrx *rcv = (yeevrx*)yoGetImplState((dvoid*)or);
  yeevr *qr;
  ysevt	*evt;
  boolean got = FALSE;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)223, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  cx->info_yeevcx.npull_yeevInfo++;

  while( !got )
  {
    if( ysLstCount( rcv->q_yeevrx ) )
    {
      
      qr = (yeevr*)ysLstDeq( rcv->q_yeevrx );
      yeevr__copy( rec, qr, yoAlloc );
      ysmGlbFree( (dvoid*)qr );
      got = TRUE;
    }
    else			
    {
      evt = ysSemCreate( (dvoid*)rcv );
      DISCARD ysLstEnq( rcv->evq_yeevrx, (dvoid*)evt );
      ysSemWait( evt );
    }
  }
}

void yeevReceiver_pullMany_i( yeevReceiver or, yoenv* ev,
			     yeevrList* recs)
{
  yeevrx *rcv = (yeevrx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)224, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  recs->_maximum = ysLstCount( rcv->q_yeevrx );

  
  if( !recs->_maximum )
    recs->_maximum = 1;

  recs->_buffer = (yeevr*)yoAlloc( (size_t)recs->_maximum * sizeof(yeevr) );
  recs->_length = 0;

  while( ysLstCount( rcv->q_yeevrx ) && recs->_length < recs->_maximum )
    yeevReceiver_pull_i( or, ev, &recs->_buffer[ recs->_length++ ] );
}



boolean yeevReceiver_tryPull_i( yeevReceiver or, yoenv* ev,
				      yeevr* rec)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevrx *rcv = (yeevrx*)yoGetState((dvoid*)or);
  yeevr *qr;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)225, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  cx->info_yeevcx.ntpull_yeevInfo++;
  if( ysLstCount( rcv->q_yeevrx ) )
  {
    qr = (yeevr*)ysLstDeq( rcv->q_yeevrx );
    yeevr__copy( rec, qr, yoAlloc );
    yeevr__free( qr, yotkFreeStr );
    ysmGlbFree( (dvoid*)qr );
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


boolean yeevReceiver_tryPullMany_i( yeevReceiver or, yoenv* ev,
				   yeevrList* recs)
{
  yeevrx *rcv = (yeevrx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)226, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  recs->_maximum = ysLstCount( rcv->q_yeevrx );
  recs->_buffer = (yeevr*)yoAlloc( (size_t)recs->_maximum * sizeof(yeevr) );
  recs->_length = 0;

  while( ysLstCount( rcv->q_yeevrx ) && recs->_length < recs->_maximum )
    yeevReceiver_pull_i( or, ev, &recs->_buffer[ recs->_length++ ] );

  return( recs->_length != 0 );
}


char* yeevReceiver__get_name_i( yeevReceiver or, yoenv* ev)
{
  char *rv;
  yeevrx *rcv = (yeevrx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)227, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND );

  rv = ysStrDupWaf(rcv->name_yeevrx, yoAlloc );
  return( rv );
}





yeevReceiver yeev_createReceiver_i( yeev or, yoenv* ev, char *name )
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevrx *rcv;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)228, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLSTR(name), YSLEND );

  rcv = (yeevrx*)ysmGlbAlloc(sizeof(*rcv), yeevch_tag );
  rcv->self_yeevrx =
    (yeevReceiver )yoCreate( yeevReceiver__id, (char*)0,
			    (yoRefData*)0, (char*)0, (dvoid*)rcv );
  rcv->name_yeevrx = ysStrDup( name );
  rcv->hndlr_yeevrx = (yeevHndlr)0;
  rcv->q_yeevrx = ysLstCreate();
  rcv->evq_yeevrx = ysLstCreate();
  rcv->le_yeevrx = ysLstEnq( cx->rcvs_yeevcx, (dvoid*)rcv );

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)229, YSLSEV_DEBUG(0), (char*)0, 
	   YSLPTR(rcv->self_yeevrx), YSLPTR(rcv), YSLSTR(name), YSLEND );

  return( (yeevReceiver)yoDuplicate((dvoid*)rcv->self_yeevrx) );
}


yeevDiscList yeev_createDiscList_i( yeev or, yoenv* ev,
					    yeevdSeq* dlist)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevdlx   *dl;
  ub4	i;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)230, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND );

  dl = (yeevdlx*)ysmGlbAlloc(sizeof(*dl), yeevdlx_tag );
  CLRSTRUCT(*dl);
  dl->self_yeevdlx =
    (yeevDiscList )yoCreate( yeevDiscList__id, (char*)0,
			     (yoRefData*)0, (char*)0, (dvoid*)dl );
  dl->le_yeevdlx = ysLstEnq( cx->dls_yeevcx, (dvoid*)dl);
  DISCARD ysspNewTree( &dl->dests_yeevdlx, yoCmp );

  for( i = 0; i < dlist->_length ; i++ )
    yeevAddToDestList( cx, yeevDestList( dl, dlist->_buffer[i].dest_yeevd ),
		      &dlist->_buffer[i] );

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)231, YSLSEV_DEBUG(0), (char*)0,
	   YSLPTR(dl->self_yeevdlx), YSLPTR(dl), YSLSB4(dlist->_length),
	   YSLEND);

  return( (yeevDiscList)yoDuplicate((dvoid*)dl->self_yeevdlx) );
}


void yeev_raise_i( yeev or, yoenv* ev, yeevr* rec)
{
  yeevcx    *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)232, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND);

  ++cx->info_yeevcx.nevent_yeevInfo;

  rec->orig_yeevr = or;
  ysClock((sysb8*)&rec->origtime_yeevr);
  rec->orighost_yeevr = cx->info_yeevcx.host_yeevInfo;
  rec->origpid_yeevr = cx->info_yeevcx.pid_yeevInfo;
  rec->origaff_yeevr = cx->info_yeevcx.affinity_yeevInfo;
  rec->origprog_yeevr = cx->info_yeevcx.prog_yeevInfo;
  rec->forw_yeevr = (yeev)or;
  rec->hops_yeevr = 0;

  DISCARD yeevSend( cx, rec );
}



void yeev_raiseMany_i( yeev or, yoenv* ev, yeevrList* recs)
{
  yeevcx    *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  ub4 i;
  yeevr	*rec;
  sysb8 now;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)233, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLSB4(recs->_length), YSLEND);

  ysClock(&now);
  yeevAllBatch( cx );
  for( i = 0; i < recs->_length ; i++ )
  {
    rec = &recs->_buffer[i];

    rec->orig_yeevr = or;
    rec->origtime_yeevr = now;
    rec->origseq_yeevr = ++cx->info_yeevcx.nevent_yeevInfo;
    rec->orighost_yeevr = cx->info_yeevcx.host_yeevInfo;
    rec->origpid_yeevr = cx->info_yeevcx.pid_yeevInfo;
    rec->origaff_yeevr = cx->info_yeevcx.affinity_yeevInfo;
    rec->origprog_yeevr = cx->info_yeevcx.prog_yeevInfo;
    rec->forw_yeevr = (yeev)or;
    rec->hops_yeevr = 0;

    DISCARD yeevSend( cx, rec );
  }
  yeevAllSend( cx );
}



void yeev_forward_i( yeev or, yoenv* ev, yeevr* rec)
{
  yeevcx    *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)234, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND);

  rec->forw_yeevr = or;
  rec->hops_yeevr++;

  

  
  DISCARD yeevSend( cx, rec );
}



yeevInfo yeev__get_info_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevInfo oinfo;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)235, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND);

  yeevInfo__copy( &oinfo, &cx->info_yeevcx, yoAlloc );
  return oinfo;
}


yeevReceiverList yeev__get_receivers_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevReceiverList l;
  ysle	*e;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)236, YSLSEV_DEBUG(8), (char*)0, 
	   YSLPTR(or), YSLEND);

  l._length = 0;
  l._maximum = ysLstCount( cx->rcvs_yeevcx );
  l._buffer =
    (yeevReceiver*)yoAlloc( (size_t)l._maximum * sizeof(l._buffer[0] ));

  for( e = ysLstHead( cx->rcvs_yeevcx ); e ; e =ysLstNext(e) )
    l._buffer[ l._length++ ] =
      (yeevReceiver)yoDuplicate((dvoid*)((yeevrx*)ysLstVal( e ))->self_yeevrx);

  return(l);
}



yeevDiscList yeev__get_filters_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  yeevDiscList rv;

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)237, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND);

  yeevDiscList__copy( &rv, &cx->filters_yeevcx, yoAlloc );
  return( rv );
}



void yeev__set_filters_i( yeev or, yoenv* ev, yeevDiscList val)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)238, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLEND);

  yoRelease( (dvoid*)cx->filters_yeevcx );
  cx->filters_yeevcx = (yeevDiscList)yoDuplicate((dvoid*)val);
  cx->fcx_yeevcx = (yeevdlx*)yoGetState((dvoid*)val);
}


boolean yeev__get_limit_drop_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)243, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLSTR(cx->doDrop_yeevcx ? "TRUE" : "FALSE"),
	   YSLEND);

  return(cx->doDrop_yeevcx);
}



void yeev__set_limit_drop_i( yeev or, yoenv* ev, boolean val)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)244, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLSTR(val ? "TRUE" : "FALSE"), YSLEND);

  cx->doDrop_yeevcx = val;
}


ub4 yeev__get_globalEventHighWater_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)239, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLUB4(cx->globLimit_yeevcx), YSLEND);

  return( cx->globLimit_yeevcx );
}


void yeev__set_globalEventHighWater_i( yeev or, yoenv* ev, ub4 val)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);
  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)240, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLUB4(val), YSLEND);

  cx->globLimit_yeevcx = val;
}


ub4 yeev__get_globalEventRestart_i( yeev or, yoenv* ev)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)241, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLUB4(cx->globRestart_yeevcx), YSLEND);

  return( cx->globRestart_yeevcx );
}


void yeev__set_globalEventRestart_i( yeev or, yoenv* ev, ub4 val)
{
  yeevcx *cx = (yeevcx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT, YEEV_FAC, (ub4)242, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(or), YSLUB4(val), YSLEND);

  cx->globRestart_yeevcx = val;
}


