/* mx/src/ye/yeevlg.c */


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
#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
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
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YEEVLOGI_H
#include <yeevlogI.h>
#endif
#ifndef YEEVENTI_ORACLE
#include <yeeventI.h>
#endif
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
#endif
#ifndef YEMSG_ORACLE
#include <yemsg.h>
#endif
#ifndef YEEVF_ORACLE
#include <yeevf.h>
#endif
#ifndef YEEVLG_ORACLE
#include <yeevlg.h>
#endif
#ifndef YOSX_ORACLE
#include <yosx.h>
#endif
#ifndef YOI_ORACLE
#include <yoi.h>
#endif
#ifndef YOGIOP_ORACLE
#include <yogiop.h>
#endif



#define YEEVLG_FAC "YEEVLD"

#define YEEVLG_MIN_BSIZ	    (200)

#define YEEVLG_TIME_DIGS    ((sword)3)

#define YEEVLG_BINFILE_MARKER ((ub4)0xfeedbeef)


#define YEEVLG_DEF_FILTER_SPEC	"maxsev 6" 

typedef struct yeevlglg yeevlglg;
typedef struct yeevlgs yeevlgs;
typedef struct yeevlgi yeevlgi;

static CONST_W_PTR struct yeevl_logProc__tyimpl yeevl_logProc__impl =
 {
  yeevl_logProc_connectSender_i,
  yeevl_logProc_createLog_i,
  yeevl_logProc_openLog_i,
  yeevl_logProc_shutdown_i,
  yeevl_logProc__get_defQuals_i,
  yeevl_logProc__set_defQuals_i,
  yeevl_logProc__get_senders_i,
  yeevl_logProc__get_logs_i,
  yeevl_logProc_createReceiver_i,
  yeevl_logProc_createDiscList_i,
  yeevl_logProc_raise_i,
  yeevl_logProc_raiseMany_i,
  yeevl_logProc_forward_i,
  yeevl_logProc__get_info_i,
  yeevl_logProc__get_receivers_i,
  yeevl_logProc__get_filters_i,
  yeevl_logProc__set_filters_i,
  yeevl_logProc__get_limit_drop_i,
  yeevl_logProc__set_limit_drop_i,
  yeevl_logProc__get_globalEventHighWater_i,
  yeevl_logProc__set_globalEventHighWater_i,
  yeevl_logProc__get_globalEventRestart_i,
  yeevl_logProc__set_globalEventRestart_i
 };

static CONST_W_PTR struct yeevl_sender__tyimpl yeevl_sender__impl =
 {
  yeevl_sender_destroy_i,
  yeevl_sender__get_info_i
 };


static CONST_W_PTR struct yeevl_log__tyimpl yeevl_log__impl =
 {
  yeevl_log_deleteRecord_i,
  yeevl_log_getRecord_i,
  yeevl_log_getRecordRange_i,
  yeevl_log_getRecordTime_i,
  yeevl_log__get_fixedattr_i,
  yeevl_log__get_varattr_i,
  yeevl_log__set_varattr_i,
  yeevl_log_destroy_i,
  yeevl_log_push_i,
  yeevl_log_pushMany_i,
  yeevl_log_pull_i,
  yeevl_log_pullMany_i,
  yeevl_log_tryPull_i,
  yeevl_log_tryPullMany_i,
  yeevl_log__get_name_i
 };

static CONST_W_PTR struct yeevl_listIterator__tyimpl yeevl_listIterator__impl =
 {
  yeevl_listIterator_getNextOne_i,
  yeevl_listIterator_getNextN_i,
  yeevl_listIterator_destroy_i
 };




struct yeevlglg
{
  yeevl_log	self_yeevlglg;
  yeevlgcx	*cx_yeevlglg;
  ysle		*le_yeevlglg;	

  yeevl_yeevld	fixattr_yeevlglg;
  yeevl_yeevla	varattr_yeevlglg;
  yeevReceiver	r_yeevlglg;
  sb4		seq_yeevlglg;	
  sysfp		*fp_yeevlglg;	
  boolean	fperr_yeevlglg;	
  sysb8		wpos_yeevlglg;	
  sysb8		bpos_yeevlglg;	
  sysb8		rpos_yeevlglg;	
  sb4		rrec_yeevlglg;	
  boolean	alarmed_yeevlglg; 
  boolean	swapping_yeevlglg; 
};



struct yeevlgs
{
  ysspNode  node_yeevlgs;	
  yeevl_sender	self_yeevlgs;	
  yeevlgcx  *cx_yeevlgs;
  ysevt	    *devt_yeevlgs;	

  
  yeevl_yeevls  obj_yeevlgs;
};



struct yeevlgi
{
  yeevl_listIterator	self_yeevlgi;
  yeevlgcx  *cx_yeevlgi;
  ysle	    *le_yeevlgi;	
  yeevlglg  *lg_yeevlgi;	
  boolean   bytime_yeevlgi;	
  sb4	    startRec_yeevlgi;	
  sb4	    stopRec_yeevlgi;	
  sysb8	    startTime_yeevlgi;	
  sysb8	    stopTime_yeevlgi;	
  boolean   first_yeevlgi;	
  yeevDiscList	dl_yeevlgi;	
};


struct yeevlgcx
{
  ysque		*q_yeevlgcx;
  yeevReceiver	r_yeevlgcx;	
  yeev		ev_yeevlgcx;	
  yoenv		env_yeevlgcx;	
  yeevDiscList	ddl_yeevlgcx;	
  sysb8		curEvent_yeevlgcx; 

  yslst		*impls_yeevlgcx; 
  boolean	impldestroy_yeevlgcx; 
  yslst		*logs_yeevlgcx;	
  yslst		*iters_yeevlgcx; 

  ysspTree	senders_yeevlgcx; 
  ub4		nsenders_yeevlgcx;

  yeevl_logProc	self_yeevlgcx;	

  yemsgcx	*yemsg_yeevlgcx; 
  sysb8		b1024_yeevlgcx;
  char		*txtbuf_yeevlgcx; 
  size_t	stxtbuf_yeevlgcx; 
};


externdef ysmtagDecl(yeevlgcx_tag) = "yeevlgcx";
externdef ysmtagDecl(yeevlgs_tag) = "yeevlgs";
externdef ysmtagDecl(yeevlglg_tag) = "yeevlglg";
externdef ysmtagDecl(yeevlgi_tag) = "yeevlgi";
externdef ysmtagDecl(yeevBinfile_tag) = "yeevlg binfile 2.0";
externdef ysmtagDecl(yeevTextfile_tag) = "yeevlg textfile 1.0";


externdef ysidDecl( YEEVLG_EX_NO_YEEV ) = "::yeevlg::noyeev";
externdef ysidDecl( YEEVLG_EX_STARTUP ) = "::yeevlg::startup";

STATICF void yeevlgCleanup( yeevlgcx *cx );


STATICF void yeevlgSenderHandler( yeevr *rec, dvoid *usrp );

STATICF void yeevlgConsole( sword level, CONST char *fmt, ...);
STATICF char *yeevlgGets( sysfp *fp, char *buf, size_t bufsiz );
STATICF void yeevlgSplit( char *str, char **fields, sword nf );

STATICF void yeevlgTextOpen( yeevlglg *lg, CONST char *mode,
			    CONST char **errptr );
STATICF void yeevlgBinOpen( yeevlglg *lg, CONST char *mode,
			   CONST char **errptr );


STATICF void yeevlgLogHandler( yeevr *rec, dvoid *usrp );


#ifdef NEVER
STATICF boolean yeevlg_adminState( CONST char *s, yeevl_adminState *type );
STATICF boolean yeevlg_operState( CONST char *s, *type );
#endif


STATICF yeevl_log yeevlgLogCreate( yeevl_logProc or, yoenv* ev,
				yeevl_yeevld* fixedattrs,
				yeevl_yeevla* varattrs,
				char *mode );

STATICF void yeevlgLogFlush( yeevlglg *lg );
STATICF void yeevlgLogDestroy( dvoid *dvlg );

STATICF yeevl_listIterator yeevlgIteratorCreate(yeevlgcx *cx,
						yeevlglg *lg,
						boolean bytime,
						CONST sb4 *startRec,
						CONST sb4 *stopRec,
						CONST sysb8 *startTime,
						CONST sysb8 *stopTime,
						yeevdSeq *dlist );
						
STATICF void yeevlgIteratorDestroy( dvoid *dvdi );

STATICF void yeevlgLimitCheck( yeevlglg *lg, ub4 sizekb );

STATICF void yeevlgRollover( yeevlglg *lg );

STATICF void yeevlgPutTextFile( yeevlglg *lg, yeevl_yeevlr *lrec,
			       yeevr *rec, boolean checkLimit );
STATICF void yeevlgPutBinFile( yeevlglg *lg, yeevl_yeevlr *lrec,
			      yeevr *rec, boolean checkLimit );
STATICF void yeevlgPutSysBin( yeevlglg *lg, yeevl_yeevlr *lrec,
			     yeevr *rec, boolean checkLimit );

STATICF void yeevlgEncodeRecord( yeevl_yeevlr *lrec, yeevr *rec, ysbv **bv,
				sword *nbv, ub4 *reclen );

STATICF void yeevlgFlush( yeevlglg *rlg );
STATICF void yeevlgSysfpClose( yeevlglg *lg );








yeevlgcx *yeevlgInit( ysque *q, yeev ev, boolean public )
{
  char *impl = (char*)0;
  ysle *e;
  boolean destroyf;
  yslst *filters;
  yeevdSeq dlist;
  ub4 i;
  char *f;
  yeevlgcx *cx;

  cx = (yeevlgcx*)ysmGlbAlloc( sizeof(*cx), yeevlgcx_tag );
  CLRSTRUCT(*cx);

  cx->ev_yeevlgcx = ev;
  cx->q_yeevlgcx = q;
  yoEnvInit( &cx->env_yeevlgcx );

  
  cx->impldestroy_yeevlgcx = FALSE;
  cx->impls_yeevlgcx = ysResGet("yeevlg.implementation-name");
  if( !cx->impls_yeevlgcx )
  {
    cx->impldestroy_yeevlgcx = TRUE;
    cx->impls_yeevlgcx = ysLstCreate();
  }
  if( !ysLstCount( cx->impls_yeevlgcx ) )
    DISCARD ysLstEnq( cx->impls_yeevlgcx, (dvoid*)0 );

  DISCARD ysspNewTree( &cx->senders_yeevlgcx, yoCmp );
  cx->iters_yeevlgcx = ysLstCreate();
  cx->logs_yeevlgcx = ysLstCreate();

  

  
  for( e = ysLstHead( cx->impls_yeevlgcx ) ; e ; e = ysLstNext( e ) )
  {
    impl = (char*)ysLstVal( e );

    yoSetImpl( yeevl_logProc__id, impl, yeevl_logProc__stubs,
	      (dvoid*)&yeevl_logProc__impl,
	      (yoload)0, public, (dvoid*)cx );
    yoImplReady( yeevl_logProc__id, impl, cx->q_yeevlgcx );

    yoSetImpl( yeevl_log__id, impl, yeevl_log__stubs,
	      (dvoid*)&yeevl_log__impl, (yoload)0, FALSE, (dvoid*)cx );
    yoImplReady( yeevl_log__id, impl, cx->q_yeevlgcx );
  }

  if( impl )
  {
    yoSetImpl( yeevl_log__id, (char*)0, yeevl_log__stubs,
	      (dvoid*)&yeevl_log__impl, (yoload)0, FALSE, (dvoid*)cx );
    yoImplReady( yeevl_log__id, (char*)0, cx->q_yeevlgcx );
  }

  yoSetImpl( yeevl_listIterator__id, (char*)0, yeevl_listIterator__stubs,
	    (dvoid*)&yeevl_listIterator__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoImplReady( yeevl_listIterator__id, (char*)0, cx->q_yeevlgcx );

  yoSetImpl( yeevl_sender__id, (char*)0, yeevl_sender__stubs,
	    (dvoid*)&yeevl_sender__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoImplReady( yeevl_sender__id, (char*)0, cx->q_yeevlgcx );

  
  cx->self_yeevlgcx =
    (yeevl_logProc)yoCreate( yeevl_logProc__id, (char*)impl,
			    (yoRefData*)0, (char*)0, (dvoid*)cx );

  
  destroyf = FALSE;
  filters = ysResGet( "yeevlg.default-filter-spec" );
  if( !filters )
  {
    destroyf = TRUE;
    filters = ysLstCreate();
  }
  if( !ysLstCount(filters) )
    DISCARD ysLstEnq( filters, (dvoid*)YEEVLG_DEF_FILTER_SPEC );
  dlist._maximum = ysLstCount( filters );
  if( !dlist._maximum )
    dlist._maximum = 1;
  dlist._buffer = (yeevd*)ysmGlbAlloc( sizeof(yeevd) * (size_t)dlist._maximum,
				      "dlist"); 
  for( i = 0, e = ysLstHead( filters ); e ; i++, e = ysLstNext(e) )
  {
    f = (char*)ysLstVal(e);
    dlist._buffer[i].qual_yeevd = f;
    dlist._buffer[i].dest_yeevd = (yeevReceiver)cx->self_yeevlgcx;
  }
  dlist._length = i;
  cx->ddl_yeevlgcx = yeev_createDiscList( cx->ev_yeevlgcx,
					 &cx->env_yeevlgcx, &dlist );

  ysmGlbFree((dvoid*)dlist._buffer);

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)302, YSLSEV_INFO, (char*)0,
	   YSLSB4(ysLstCount(filters)), YSLEND);
  for( i = 0, e = ysLstHead( filters ); e ; i++, e = ysLstNext(e) )
  {
    f = (char*)ysLstVal(e);
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)303, YSLSEV_INFO, (char*)0,
	     YSLSB4(i), YSLSTR(yeuStr(f)), YSLEND);
  }
  if( destroyf )
    ysLstDestroy( filters, (ysmff)0 );

  cx->yemsg_yeevlgcx = yemsgInit();
  sysb8ext( &cx->b1024_yeevlgcx, 1024 );

  cx->txtbuf_yeevlgcx = (char*)0;
  cx->stxtbuf_yeevlgcx = 0;

  return( cx );
}

void yeevlgTerm( yeevlgcx *cx )
{
  char *impl = (char*)0;
  ysle *e;
  yeevl_logProc	or;
  
  if( !cx )
    return;

  if( cx->self_yeevlgcx )
  {
    or = cx->self_yeevlgcx;
    cx->self_yeevlgcx = (yeevl_logProc)0;
    yoDispose( (dvoid*)or );
  
    for( e = ysLstHead( cx->impls_yeevlgcx ) ; e ; e = ysLstNext( e ) )
    {
      impl = (char*)ysLstVal( e );
      yoImplDeactivate( yeevl_logProc__id, impl );
      yoImplDeactivate( yeevl_log__id, impl );
    }
    if( impl )
      yoImplDeactivate( yeevl_log__id, (char*)0 );
    yoImplDeactivate( yeevl_sender__id, (char*)0 );
    yoImplDeactivate( yeevl_listIterator__id, (char*)0 );

    if( cx->txtbuf_yeevlgcx )
      ysmGlbFree( (dvoid*)cx->txtbuf_yeevlgcx );
    
    if( cx->impldestroy_yeevlgcx )
      ysLstDestroy( cx->impls_yeevlgcx, (ysmff)0 );
    yeevlgCleanup( cx );
  
    yemsgTerm(cx->yemsg_yeevlgcx);

    ysmGlbFree( (dvoid*)cx );
  }
}


boolean yeevlgFType( CONST char *s,  yeevl_fullType *type  )
{
  if( !s )
    return FALSE;
  if( !strcmp( s, "halt" ) )
    *type = yeevl_halt_fullType;
#ifdef NEVER
  else if( !strcmp( s, "wrap" ) )
    *type = yeevl_wrap_fullType;
#endif
  else if( !strcmp( s, "rollover" ) )
    *type = yeevl_rollover_fullType;
  else
    return FALSE;
  return TRUE;
}



boolean yeevlgLType( CONST char *s,  yeevl_logType *type )
{
  if( !s )
    return FALSE;
  else if( !strcmp( s, "textfile" ) )
    *type = yeevl_textfile_logType;
  else if( !strcmp( s, "binfile" ) )
    *type = yeevl_binfile_logType;
  else if( !strcmp( s, "sysbin" ) )
    *type = yeevl_sysbin_logType;
  else if( !strcmp( s, "console" ) )
    *type = yeevl_console_logType;
  else if( !strcmp( s, "syslog" ) )
    *type = yeevl_syslog_logType;
  else if( !strcmp( s, "tty" ) )
    *type = yeevl_tty_logType;
  else
    return FALSE;
  return TRUE;
}



yeevl_logProc yeevlgLogProc( yeevlgcx *cx )
{

  return cx->self_yeevlgcx;
}

yeevDiscList yeevlgDefaultDiscList( yeevlgcx *cx )
{

  return cx->ddl_yeevlgcx;
}










STATICF void yeevlgCleanup( yeevlgcx *cx )
{
  ysspNode *n, *next;
  yeevlgs *lds;
  
  for( n = ysspFHead( &cx->senders_yeevlgcx ); n ; n = next )
  {
    next = ysspFNext( n );
    lds = (yeevlgs*)n;
    yeevl_sender_destroy( lds->self_yeevlgs, &cx->env_yeevlgcx );
  }

  yeevDiscList_destroy( cx->ddl_yeevlgcx, &cx->env_yeevlgcx );
  ysLstDestroy( cx->iters_yeevlgcx, yeevlgIteratorDestroy );
  ysLstDestroy( cx->logs_yeevlgcx, yeevlgLogDestroy );
}





STATICF void yeevlgSenderHandler( yeevr *rec, dvoid *usrp )
{
  yeevlgs *lds = (yeevlgs*)usrp;

  
  ysClock( &lds->obj_yeevlgs.lastsend_yeevls );
  
  
  if( !rec->orighost_yeevr )
  {
    rec->orighost_yeevr = lds->obj_yeevlgs.host_yeevls;
    rec->origpid_yeevr = lds->obj_yeevlgs.pid_yeevls;
    rec->origaff_yeevr = lds->obj_yeevlgs.affinity_yeevls;
    rec->origprog_yeevr = lds->obj_yeevlgs.prog_yeevls;
  }

  
  yeev_forward( lds->cx_yeevlgs->ev_yeevlgcx,
	       &lds->cx_yeevlgs->env_yeevlgcx, rec );
}





STATICF void yeevlgLogHandler( yeevr *rec, dvoid *usrp )
{
  yeevlglg  *lg = (yeevlglg*)usrp;
  char buf[ 1024 + (80*4) ];
  ysstr *sp;
  yeevl_yeevlr    lrec;
  char *aff;
  ystm tm;
  char tbuf[ YSTM_BUFLEN ];
  
  yseTry
  {
    do                        
    {
      aff = rec->origaff_yeevr && *rec->origaff_yeevr ?
	rec->origaff_yeevr : (char*)0;

      
      if( lg->swapping_yeevlglg )
	break;

      if( lg->fixattr_yeevlglg.oper_yeevld == yeevl_disabled_operState ||
	 lg->varattr_yeevlglg.admin_yeevla == yeevl_locked_adminState )
	break;

      

      lrec.record_id = ++lg->seq_yeevlglg;
      lrec.deleted = FALSE;

      switch( lg->fixattr_yeevlglg.type_yeevld )
      {
      case yeevl_textfile_logType:

	yeevlgPutTextFile( lg, &lrec, rec, TRUE );
	break;

      case yeevl_binfile_logType:

	yeevlgPutBinFile( lg, &lrec, rec, TRUE );
	break;

      case yeevl_sysbin_logType:

	yeevlgPutSysBin( lg, &lrec, rec, TRUE );
	break;

      case yeevl_console_logType:

	sp = yeevFormat(lg->cx_yeevlglg->yemsg_yeevlgcx,
			rec->prod_yeevr, rec->fac_yeevr, rec->msgid_yeevr,
			&rec->val_yeevr, TRUE);
    
	ysConvClock( (sysb8*)&rec->origtime_yeevr, &tm );
	DISCARD ysStrClock( tbuf, &tm, FALSE, YEEVLG_TIME_DIGS );
	ysFmtStr(buf, "%s %d %s:%s%s%s %s %s-%d %d %s%s%s%s",
		 tbuf,
		 rec->origseq_yeevr,
		 rec->orighost_yeevr,
		 rec->origpid_yeevr,
		 aff ? ":" : "",
		 aff ? aff : "",
		 rec->origprog_yeevr,	     
		 rec->prod_yeevr,
		 rec->msgid_yeevr,
		 rec->sev_yeevr,
		 rec->assoc_yeevr ? "[" : "",
		 rec->assoc_yeevr ? rec->assoc_yeevr : "",
		 rec->assoc_yeevr ? "] " : "",
		 ysStrToText(sp) );
	ysStrDestroy( sp );
	yslError("%s\n", buf );
	break;

      case yeevl_syslog_logType:

	ysConvClock( (sysb8*)&rec->origtime_yeevr, &tm );
	DISCARD ysStrClock( tbuf, &tm, FALSE, YEEVLG_TIME_DIGS );
	sp = yeevFormat(lg->cx_yeevlglg->yemsg_yeevlgcx,
			rec->prod_yeevr, rec->fac_yeevr, rec->msgid_yeevr,
			&rec->val_yeevr, TRUE);
	ysFmtStr(buf, "%s %d %s:%s%s%s %s %s-%d %d %s%s%s%s",
		 tbuf,
		 rec->origseq_yeevr,
		 rec->orighost_yeevr,
		 rec->origpid_yeevr,
		 aff ? ":" : "",
		 aff ? aff : "",
		 rec->origprog_yeevr,
		 rec->prod_yeevr,
		 rec->msgid_yeevr,
		 rec->sev_yeevr,
		 rec->assoc_yeevr ? "[" : "",
		 rec->assoc_yeevr ? rec->assoc_yeevr : "",
		 rec->assoc_yeevr ? "] " : "",
		 ysStrToText(sp) );
	ysStrDestroy( sp );

	yeevlgConsole( (sword)rec->sev_yeevr, "%s", buf );
	break;

      case yeevl_tty_logType:

	ysConvClock( (sysb8*)&rec->origtime_yeevr, &tm );
	DISCARD ysStrClock( tbuf, &tm, FALSE, YEEVLG_TIME_DIGS );
	sp = yeevFormat(lg->cx_yeevlglg->yemsg_yeevlgcx,
			rec->prod_yeevr, rec->fac_yeevr, rec->msgid_yeevr,
			&rec->val_yeevr, TRUE);
	ysFmtStr(buf, "%s %d %s:%s%s%s %s %s-%d %d %s%s%s%s",
		 tbuf,
		 rec->origseq_yeevr,
		 rec->orighost_yeevr,
		 rec->origpid_yeevr,
		 aff ? ":" : "",
		 aff ? aff : "",
		 rec->origprog_yeevr,
		 rec->prod_yeevr,
		 rec->msgid_yeevr,
		 rec->sev_yeevr,
		 rec->assoc_yeevr ? "[" : "",
		 rec->assoc_yeevr ? rec->assoc_yeevr : "",
		 rec->assoc_yeevr ? "] " : "",
		 ysStrToText(sp) );
	ysStrDestroy( sp );
	yslPrint("%s\n", buf );
	break;

      default:
	
	ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)320, YSLSEV_ERR, (char*)0,
		 YSLSB4(lg->fixattr_yeevlglg.type_yeevld ),
		 YSLSTR("yeevlgLogHandler"),
		 YSLEND);
	break;
      }
      if( !ysSvcPending( lg->cx_yeevlglg->q_yeevlgcx ) )
	yeevlgLogFlush( lg );
    } while( FALSE );
  }
  yseCatchAll
  {
    
    yslError("yeevlgLogHandler: exception %s", yseExid );
  }
  yseEnd;
}

#ifdef NEVER

STATICF boolean yeevlg_adminState( CONST char *s, yeevl_adminState *type  )
{
  if( !s )
    return FALSE;
  if( !strcmp( s, "locked") )
    *type = yeevl_locked_adminState;
  if( !strcmp( s, "unlocked") )
    *type = yeevl_unlocked_adminState;
  else
    return FALSE;

  return TRUE;
}


STATICF yeevl_operState yeevlg_operState( CONST char *s )
{
  if( !s )
    return FALSE;
  if( !strcmp( s, "enabled") )
    *type = yeevl_enabled_operState;
  if( !strcmp( s, "disabled") )
    *type = yeevl_disabled_operState;
  else
    return FALSE;

  return TRUE;
}

#endif


STATICF yeevl_log yeevlgLogCreate( yeevl_logProc or, yoenv* ev,
				  yeevl_yeevld* fixedattrs,
				  yeevl_yeevla* varattrs,
				  char *mode )
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevlglg *lg;
  CONST char	*errptr;
  yeevl_OperationFailed exv;

  lg = (yeevlglg*)ysmGlbAlloc(sizeof(*lg), yeevlglg_tag );
  lg->cx_yeevlglg = cx;

  
  lg->fixattr_yeevlglg.type_yeevld = fixedattrs->type_yeevld; 
  lg->seq_yeevlglg = 0;
  if( fixedattrs->file_yeevld )
    lg->fixattr_yeevlglg.file_yeevld = ysStrDup(fixedattrs->file_yeevld);
  else
    lg->fixattr_yeevlglg.file_yeevld = (char*)0;

  if( *mode == 'w' )
    lg->fixattr_yeevlglg.oper_yeevld = yeevl_enabled_operState;
  else
    lg->fixattr_yeevlglg.oper_yeevld = yeevl_disabled_operState;

  
  lg->varattr_yeevlglg.maxLogSizeKb_yeevla = varattrs->maxLogSizeKb_yeevla;
  lg->varattr_yeevlglg.capAlarmThresholdKb_yeevla =
    varattrs->capAlarmThresholdKb_yeevla;
  lg->varattr_yeevlglg.keepCount_yeevla = varattrs->keepCount_yeevla;
  lg->varattr_yeevlglg.fullAction_yeevla = varattrs->fullAction_yeevla;
  lg->varattr_yeevlglg.admin_yeevla = varattrs->admin_yeevla;

  lg->wpos_yeevlglg = *sysb8zero;
  lg->bpos_yeevlglg = *sysb8zero;
  lg->rpos_yeevlglg = *sysb8zero;
  lg->rrec_yeevlglg = -1;	
  lg->alarmed_yeevlglg = FALSE;
  lg->swapping_yeevlglg = FALSE;

  
  if( lg->varattr_yeevlglg.fullAction_yeevla == yeevl_rollover_fullType )
    yeevlgRollover( lg );

  switch( fixedattrs->type_yeevld )
  {
  case yeevl_textfile_logType:
    yeevlgTextOpen( lg, mode, &errptr );
    break;

  case yeevl_binfile_logType:
    yeevlgBinOpen( lg, mode, &errptr );
    break;

  case yeevl_sysbin_logType:
    
    exv.reason = ysStrDupWaf( (char*)"sysbin log not implemented", yoAlloc );
    yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
    break;

  case yeevl_console_logType:

    
    ysmGlbFree( (dvoid*)lg->fixattr_yeevlglg.file_yeevld );
    lg->fixattr_yeevlglg.file_yeevld = ysStrDup("console");
    
    
    break;

  case yeevl_syslog_logType:
    ysmGlbFree( (dvoid*)lg->fixattr_yeevlglg.file_yeevld );
    lg->fixattr_yeevlglg.file_yeevld = ysStrDup("syslog");
    
    
    break;

  case yeevl_tty_logType:

    ysmGlbFree( (dvoid*)lg->fixattr_yeevlglg.file_yeevld );
    lg->fixattr_yeevlglg.file_yeevld = ysStrDup("tty");
    
    break;

  default:

    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)320, YSLSEV_ERR, (char*)0,
	     YSLSB4(lg->fixattr_yeevlglg.type_yeevld),
	     YSLSTR("yeevlgLogCreate"),
	     YSLEND);
    break;
  }

  
  lg->r_yeevlglg = (yeevReceiver)
    yeevl_logProc_createReceiver( or, ev,
				 lg->fixattr_yeevlglg.file_yeevld );
  DISCARD yeevReceiverSetHandler( lg->r_yeevlglg,yeevlgLogHandler,(dvoid*)lg);

  lg->self_yeevlglg = (yeevl_log)yoCreate( yeevl_log__id, (char*)0,
					  (yoRefData*)0, (char*)0,
					  (dvoid*)lg);

  lg->le_yeevlglg = ysLstEnq( cx->logs_yeevlgcx, (dvoid*)lg );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)322, YSLSEV_DEBUG(1), (char*)0,
	   YSLPTR(lg->self_yeevlglg),
	   YSLSTR(lg->fixattr_yeevlglg.file_yeevld),
	   YSLPTR(lg->r_yeevlglg),
	   YSLEND);

  return( (yeevl_log)yoDuplicate((dvoid*)lg->self_yeevlglg) );
}


STATICF void yeevlgLogFlush( yeevlglg *lg )
{
  switch( lg->fixattr_yeevlglg.type_yeevld )
  {
  case yeevl_binfile_logType:
  case yeevl_textfile_logType:

    if( !lg->fp_yeevlglg )
      yseThrow( YS_EX_BADPARAM );

    if( !sysfpFlush( lg->fp_yeevlglg ) && !lg->fperr_yeevlglg )
    {
      
      lg->fperr_yeevlglg = TRUE;

      
      ysRecord(  YS_PRODUCT, YEEVLG_FAC, (ub4)331, YSLSEV_ERR, (char*)0,
	       YSLSTR("yeevlgLogFlush"),
	       YSLSTR(lg->fixattr_yeevlglg.file_yeevld), YSLEND);
    }
    break;

  case yeevl_sysbin_logType:
    
    break;

  default:
    
    break;
  }
}


STATICF void yeevlgLogDestroy( dvoid *dvlg )
{
  yeevlglg *lg = (yeevlglg*)dvlg;
  
  yeevReceiver_destroy( lg->r_yeevlglg, &lg->cx_yeevlglg->env_yeevlgcx );
  yoDispose( (dvoid*)lg->self_yeevlglg );

  yeevlgLogFlush( lg );

  switch( lg->fixattr_yeevlglg.type_yeevld )
  {
  case yeevl_textfile_logType:
  case yeevl_binfile_logType:
    if( lg->fp_yeevlglg )
      yeevlgSysfpClose( lg );
    break;

  case yeevl_sysbin_logType:
    
    break;

  default:
    
    break;
  }

  if( lg->fixattr_yeevlglg.file_yeevld )
    ysmGlbFree( (dvoid*)lg->fixattr_yeevlglg.file_yeevld );

  ysmGlbFree( (dvoid*)lg );
}




STATICF yeevl_listIterator yeevlgIteratorCreate(yeevlgcx *cx,
						yeevlglg *lg,
						boolean bytime,
						CONST sb4 *startRec,
						CONST sb4 *stopRec,
						CONST sysb8 *startTime,
						CONST sysb8 *stopTime,
						yeevdSeq *dlist )
{
  yeevlgi    *ldi;
  
  ldi = (yeevlgi*)ysmGlbAlloc(sizeof(*ldi), yeevlgi_tag );
  ldi->cx_yeevlgi = cx;
  ldi->lg_yeevlgi = lg;
  ldi->bytime_yeevlgi = bytime;
  ldi->startRec_yeevlgi = *startRec;
  ldi->stopRec_yeevlgi = *stopRec;
  ldi->startTime_yeevlgi = *startTime;
  ldi->stopTime_yeevlgi = *stopTime;
  ldi->first_yeevlgi = TRUE;
  if( dlist->_length )
    ldi->dl_yeevlgi =
      yeev_createDiscList( cx->ev_yeevlgcx, &cx->env_yeevlgcx, dlist );
  else
    ldi->dl_yeevlgi = (yeevDiscList)0;

  ldi->self_yeevlgi =
    (yeevl_listIterator)yoCreate( yeevl_listIterator__id, (char*)0,
				 (yoRefData*)0, (char*)0, (dvoid*)ldi);

  ldi->le_yeevlgi = ysLstEnq( cx->iters_yeevlgcx, (dvoid*)ldi );
  return( ldi->self_yeevlgi );
}




STATICF void yeevlgIteratorDestroy( dvoid *dvldi )
{
  yeevlgi *ldi = (yeevlgi*)dvldi;
  
  yoDispose( (dvoid*)ldi->self_yeevlgi );

  if( ldi->dl_yeevlgi )
    yeevDiscList_destroy( ldi->dl_yeevlgi, &ldi->cx_yeevlgi->env_yeevlgcx );

  ysmGlbFree( (dvoid*)ldi );
}





STATICF boolean yeevlGetRec( yeevlglg *lg, yeevl_yeevlr *lrec )
{
  boolean rv = TRUE;		
  boolean binfile = TRUE;
  boolean byteorder;
  ub4	reclen = 0;
  ub4	marker;
  char	reclenbuf[ 16 ];	
  char	buf[ YEEVLG_MIN_BSIZ ];
  yeevr	*rec = &lrec->record;
  size_t bsiz;
  char	*fields[10];
  yeevlgcx *cx = lg->cx_yeevlglg;
  char *p;
  yeevl_OperationFailed exv;
  
  ysbv bv, bv2;
  yosx *x;

  bv2.buf = (ub1*)reclenbuf;
  bv2.len = sizeof(reclenbuf);
  
  
  yeevlgFlush( lg );

  

  if( lg->fixattr_yeevlglg.type_yeevld == yeevl_binfile_logType )
  {
    if( !lg->fp_yeevlglg )
      yseThrow( YS_EX_BADPARAM );

    DISCARD sysfpSeek( lg->fp_yeevlglg, &lg->rpos_yeevlglg );
    rv = bv2.len != sysfpRead( lg->fp_yeevlglg, (dvoid*)bv2.buf, bv2.len );
  }
  else if( lg->fixattr_yeevlglg.type_yeevld == yeevl_textfile_logType )
  {
    if( !lg->fp_yeevlglg )
      yseThrow( YS_EX_BADPARAM );

    DISCARD sysfpSeek( lg->fp_yeevlglg, &lg->rpos_yeevlglg );
    binfile = FALSE;
    rv = FALSE;
  }
  else if( lg->fixattr_yeevlglg.type_yeevld == yeevl_sysbin_logType )
  {
    ;
    
  }
  else
    yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);

  
  if( rv )
    return( rv );

  if( binfile )			
  {
    x = yosxDecode( (sword)0, &bv2, 1 );
    byteorder = (boolean)yosxGetUB1( x );
    yosxDecSet( x, byteorder );
    marker = yosxGetUB4( x ); 
    if( marker != YEEVLG_BINFILE_MARKER )
    {
      exv.reason = ysStrDupWaf( (char*)"malformed record", yoAlloc );
      yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
    }
    marker = yosxGetUB4( x ); 
    reclen = yosxGetUB4( x );
    yosxDecEnd( x );
    bv.len = (size_t)reclen;
    bv.buf = (ub1*)ysmGlbAlloc( (size_t)reclen, "bv" );
  }

  

  if( lg->fixattr_yeevlglg.type_yeevld == yeevl_binfile_logType )
  {
    rv = reclen != sysfpRead( lg->fp_yeevlglg, (dvoid*)bv.buf,
			     (size_t)reclen);
  }
  else if( lg->fixattr_yeevlglg.type_yeevld == yeevl_sysbin_logType )
  {
    ;
    
  }
  else if( lg->fixattr_yeevlglg.type_yeevld == yeevl_textfile_logType )
  {
    
    do
    {
      
      if( !yeevlgGets( lg->fp_yeevlglg, buf, sizeof( buf )))
      {
	rv = TRUE;
	break;
      }
      yeevlgSplit( buf, fields, 3 );
      DISCARD sysb8fromstr( &lrec->timestamp, fields[0] );
      lrec->record_id = (sb4)atol( fields[1] );
      lrec->deleted = FALSE;
      bsiz = (size_t)atol( fields[2] );

      
      if( bsiz > cx->stxtbuf_yeevlgcx )
      {
	if( cx->stxtbuf_yeevlgcx )
	  cx->txtbuf_yeevlgcx =
	    (char*)ysmGlbRealloc( (dvoid*)cx->txtbuf_yeevlgcx, bsiz );
	else
	  cx->txtbuf_yeevlgcx =
	    (char*)ysmGlbAlloc( bsiz, "yeevlg txtbuf");
	cx->stxtbuf_yeevlgcx = bsiz;
      }
      
      if( !yeevlgGets( lg->fp_yeevlglg,
		      cx->txtbuf_yeevlgcx, cx->stxtbuf_yeevlgcx ) )
      {
	rv = TRUE;
	break;
      }
      yeevlgSplit( cx->txtbuf_yeevlgcx, fields, 7);
      rec->orig_yeevr = (yeev)yoStrToRef( fields[0] );
      rec->origseq_yeevr = (ub4)atol( fields[1] );
      sysb8fromstr( (sysb8*)&rec->origtime_yeevr, fields[2] );
      rec->orighost_yeevr = ysStrDup( fields[3] );
      rec->origpid_yeevr = ysStrDup( fields[4] );
      rec->origaff_yeevr = ysStrDup( fields[5] );
      rec->origprog_yeevr = ysStrDup( fields[6] );
      
      
      if( !yeevlgGets( lg->fp_yeevlglg,
		      cx->txtbuf_yeevlgcx, cx->stxtbuf_yeevlgcx ) )
      {
	rv = TRUE;
	break;
      }
      yeevlgSplit( cx->txtbuf_yeevlgcx, fields, 7);
      rec->forw_yeevr = (yeev)yoStrToRef( fields[0] );
      rec->hops_yeevr = (ub4)atol( fields[1] );
      rec->prod_yeevr = ysStrDup( fields[2] );
      rec->fac_yeevr = ysStrDup( fields[3] );
      rec->msgid_yeevr = (ub4)atol( fields[4] );
      rec->sev_yeevr = (ub4)atol( fields[5] );
      rec->assoc_yeevr = ysStrDup( fields[6] );
      
      
      if( !yeevlgGets( lg->fp_yeevlglg,
		      cx->txtbuf_yeevlgcx, cx->stxtbuf_yeevlgcx ) )
      {
	rv = TRUE;
	break;
      }
      yeevlgSplit( cx->txtbuf_yeevlgcx, fields, 1);
      rec->val_yeevr._type = (yotk*)yoTcString;
      rec->val_yeevr._value = ysmGlbAlloc( sizeof(char**), "rec char**");

      
      if( !strncmp( fields[0], "{ ", 2 ) )
	fields[0] += 2;
      p = fields[0] + strlen( fields[0] ) - 2;
      if( p > fields[0] && !strcmp( p, " }" ) )
	*p = 0;

      *(char**)rec->val_yeevr._value = ysStrDup( fields[0] );

      
    } while( FALSE );
  }
  else
    yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);
  
  if( binfile )			
  {
    if( rv )
    {
      
      
      ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)323, YSLSEV_ERR, (char*)0,
	       YSLSTR(lg->fixattr_yeevlglg.file_yeevld), YSLEND);
    }
    else
    {
      x = yosxDecode( byteorder, &bv, 1 );
      yotkDecode( YCTC_yeevl_yeevlr, (dvoid*)lrec, x, (ysmaf)0 );
      lg->rrec_yeevlglg = lrec->record_id; 
      yosxDecEnd( x );
    }
    ysmGlbFree( (dvoid*)bv.buf );
  }

  
  if( !rv )
    sysfpTell( lg->fp_yeevlglg, &lg->rpos_yeevlglg );

  return rv;
}





STATICF void yeevlgLimitCheck( yeevlglg *lg, ub4 sizekb )
{
  ub4	wpos;
  sysb8	wposl;
  yeevl_LogCapacityAlarm cev;
  yeevl_LogCapacityLimit lev;
  yeevr	rec;
  CONST char *errptr;
  yeevl_yeevlr	lrec;	
  yeevlgs *lds;

  sizekb++;

  sysb8div( &wposl, &lg->wpos_yeevlglg, &lg->cx_yeevlglg->b1024_yeevlgcx );
  wpos = (ub4)sysb8msk( &wposl );

  if( lg->varattr_yeevlglg.capAlarmThresholdKb_yeevla && !lg->alarmed_yeevlglg 
     && wpos > lg->varattr_yeevlglg.capAlarmThresholdKb_yeevla )
  {
    lg->alarmed_yeevlglg = TRUE;
    cev.theLog = lg->self_yeevlglg;
    cev.fattrs = lg->fixattr_yeevlglg;
    cev.vattrs = lg->varattr_yeevlglg;
    ysRecord( YS_PRODUCT, YEEVLG_FAC, (ub4)309, YSLSEV_WARNING, (char*)0,
	     YSLANY( YCTC_yeevl_LogCapacityAlarm, (dvoid*)&cev), YSLEND );
  }

  if( lg->varattr_yeevlglg.maxLogSizeKb_yeevla 
     && wpos > (lg->varattr_yeevlglg.maxLogSizeKb_yeevla - sizekb) )
  {
    
    lev.theLog = lg->self_yeevlglg;
    rec.origseq_yeevr = 0;
    rec.fac_yeevr = (char*)YEEVLG_FAC;
    rec.prod_yeevr = (char*)YS_PRODUCT;
    rec.assoc_yeevr = (char*)0;
    rec.sev_yeevr = YSLSEV_WARNING;	
    rec.msgid_yeevr = (ub4)310;
    rec.val_yeevr._type = (yotk*)YCTC_yeevl_LogCapacityLimit;
    rec.val_yeevr._value = (dvoid*)&lev;

    
    lg->swapping_yeevlglg = TRUE;
    yeev_raise_i( lg->cx_yeevlglg->ev_yeevlgcx,
		 &lg->cx_yeevlglg->env_yeevlgcx, &rec );
    lg->swapping_yeevlglg = FALSE;

    lrec.record_id = ++lg->seq_yeevlglg;

    switch( lg->fixattr_yeevlglg.type_yeevld )
    {
    case yeevl_textfile_logType:
    case yeevl_binfile_logType:

      

      switch( lg->varattr_yeevlglg.fullAction_yeevla )
      {
      case yeevl_halt_fullType:

	
	lg->fixattr_yeevlglg.oper_yeevld = yeevl_disabled_operState;
	lev.fattrs = lg->fixattr_yeevlglg;
	lev.vattrs = lg->varattr_yeevlglg;

	
	if( lg->fixattr_yeevlglg.type_yeevld == yeevl_textfile_logType )
	  yeevlgPutTextFile( lg, &lrec, &rec, FALSE );
	else
	  yeevlgPutBinFile( lg, &lrec, &rec, FALSE );

	
	yeevlgLogFlush( lg );
	yeevlgSysfpClose( lg );
	break;

      case yeevl_rollover_fullType:

	lev.fattrs = lg->fixattr_yeevlglg;
	lev.vattrs = lg->varattr_yeevlglg;

	
	if( lg->fixattr_yeevlglg.type_yeevld == yeevl_textfile_logType )
	  yeevlgPutTextFile( lg, &lrec, &rec, FALSE );
	else
	  yeevlgPutBinFile( lg, &lrec, &rec, FALSE );

	
	yeevlgLogFlush( lg );
	yeevlgSysfpClose( lg );

 	
	yeevlgRollover( lg ); 

	
	yseTry
	{
	  if( lg->fixattr_yeevlglg.type_yeevld == yeevl_textfile_logType )
	    yeevlgTextOpen( lg, "w+", &errptr );
	  else
	    yeevlgBinOpen( lg, "w+", &errptr );
	  lg->seq_yeevlglg = 0;
	  lg->rrec_yeevlglg = -1;	
	  lg->alarmed_yeevlglg = FALSE;
	  lg->swapping_yeevlglg = FALSE;

	  
	  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)325, YSLSEV_WARNING, (char*)0,
		   YSLSTR(lg->fixattr_yeevlglg.file_yeevld), YSLEND);
	}
	yseCatchAll
	{
	  lg->fixattr_yeevlglg.oper_yeevld = yeevl_disabled_operState;

	  
	  yslError( "yeevlgLimitCheck: failed to open new %s: %s\n",
		   lg->fixattr_yeevlglg.file_yeevld, errptr );

	  
	  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)324, YSLSEV_ERR, (char*)0,
		   YSLSTR(lg->fixattr_yeevlglg.file_yeevld),
		   YSLSTR(errptr), YSLEND);
	}
	yseEnd;
	break;

      default:
	
	break;
      }
      break;

    case yeevl_sysbin_logType:
      
      break;

    default:
      
      break;
    }
  }
}




STATICF void yeevlgRollover( yeevlglg *lg )
{
  char	path1[ SYSFP_MAX_PATHLEN ];
  char	path2[ SYSFP_MAX_PATHLEN ];
  size_t dotidx, len;
  ub4 i;

  DISCARD strcpy( path1, lg->fixattr_yeevlglg.file_yeevld );
  DISCARD strcpy( path2, lg->fixattr_yeevlglg.file_yeevld );
  for( len = dotidx = strlen(path1); dotidx > 0 ; dotidx-- )
    if( path1[dotidx] == '.' )
      break;

  if( !dotidx )
    dotidx = len;
  
  switch( lg->fixattr_yeevlglg.type_yeevld )
  {
  case yeevl_textfile_logType:
  case yeevl_binfile_logType:

    for( i = lg->varattr_yeevlglg.keepCount_yeevla ; i > 1 ; i-- )
    {
      ysFmtStr( path1 + dotidx, ".%d", i );
      ysFmtStr( path2 + dotidx, ".%d", i -1 );

      DISCARD sysfpRemove( path1 );
      DISCARD sysfpRename( path2, path1 );
    }

    ysFmtStr( path1 + dotidx, ".1" );

    DISCARD sysfpRemove( path1 );
    DISCARD sysfpRename( lg->fixattr_yeevlglg.file_yeevld, path1 );
    break;

  case yeevl_sysbin_logType:
    
    break;

  default:
    
    break;
  }
}




STATICF void yeevlgPutTextFile( yeevlglg *lg, yeevl_yeevlr *lrec,
			       yeevr *rec, boolean checkLimit )
{
  char *os;
  char *fs;
  ysstr *sp;
  char buf[ 1024 + (80*4) ];
  char *aff;
  size_t bsiz;

  if( !lg->fp_yeevlglg )
    yseThrow( YS_EX_BADPARAM );

  aff = rec->origaff_yeevr && *rec->origaff_yeevr ?
    rec->origaff_yeevr : (char*)0;

  ysClock( &lrec->timestamp );
  sp = yeevFormat(lg->cx_yeevlglg->yemsg_yeevlgcx,
		  rec->prod_yeevr, rec->fac_yeevr, rec->msgid_yeevr,
		  &rec->val_yeevr, TRUE);
  os = yoRefToStr((dvoid*)rec->orig_yeevr);
  fs = yoRefToStr((dvoid*)rec->forw_yeevr);

  yseTry
  {

    
    bsiz = strlen( sp ) + strlen( os ) + strlen( fs ) + YEEVLG_MIN_BSIZ;

    
    
    if( checkLimit )
      yeevlgLimitCheck( lg, (ub4)(bsiz + 100 + 1000) / (ub4)1024 );

    
    if( lg->fp_yeevlglg )
    {
      DISCARD sysfpSeek(  lg->fp_yeevlglg, &lg->wpos_yeevlglg );

      
      ysFmtStr( buf, "%Ld;%d;%d\n", &lrec->timestamp, lg->seq_yeevlglg, bsiz );
      DISCARD sysfpPrint( lg->fp_yeevlglg, "%s", buf );

      
      ysFmtStr( buf, "O=%s;S=%d;T=%Ld;H=%s;P=%s;A=%s;N=%s\n",
	       os, rec->origseq_yeevr, &rec->origtime_yeevr,
	       rec->orighost_yeevr, rec->origpid_yeevr, aff ? aff : "",
	       rec->origprog_yeevr );
      DISCARD sysfpPrint( lg->fp_yeevlglg, "%s", buf );

      DISCARD sysfpPrint( lg->fp_yeevlglg,
			 "F=%s;h=%d;p=%s;f=%s;m=%d;l=%d;a=%s\n",
			 fs, rec->hops_yeevr,
			 rec->prod_yeevr, rec->fac_yeevr,
			 rec->msgid_yeevr, rec->sev_yeevr,
			 rec->assoc_yeevr ? rec->assoc_yeevr : "");

      DISCARD sysfpPrint( lg->fp_yeevlglg, "v=%s\n", ysStrToText(sp) );
      sysfpTell( lg->fp_yeevlglg, &lg->wpos_yeevlglg );
    }
  }
  yseFinally
  {
    ysStrDestroy( sp );
    yoFree( (dvoid*)os );
    yoFree( (dvoid*)fs );
  }
  yseEnd;
}





STATICF void yeevlgPutBinFile( yeevlglg *lg, yeevl_yeevlr *lrec,
			      yeevr *rec, boolean checkLimit )
{
  ysbv	*bv;
  sword	nbv;
  ub4	i;
  ub4	reclen;

  yeevlgEncodeRecord( lrec, rec, &bv, &nbv, &reclen );

  
  if( checkLimit )
    yeevlgLimitCheck( lg, (reclen + (2*sizeof(ub4)) + 2048) / 1024 );

  
  if( lg->fp_yeevlglg )
  {
    for( reclen = i = 0 ; i < (ub4)nbv ; i++ )
    {
      DISCARD sysfpWrite( lg->fp_yeevlglg, (dvoid*)bv[i].buf, bv[i].len );
      ysmGlbFree( (dvoid*)bv[i].buf );
    }
    
    sysfpTell( lg->fp_yeevlglg, &lg->wpos_yeevlglg );
  }
  ysmGlbFree( (dvoid*)bv );
}



STATICF void yeevlgPutSysBin( yeevlglg *lg, yeevl_yeevlr *lrec,
			     yeevr *rec, boolean checkLimit )

{
  
}





STATICF void yeevlgEncodeRecord( yeevl_yeevlr *lrec, yeevr *rec, ysbv **bv,
				sword *nbv, ub4 *reclen )
{
  yosx *x;
  yosxPos lp, sp, ep;

  
  
  
  lrec->record = *rec;
  ysClock( &lrec->timestamp );

  
  x = yosxEncode( (size_t)0, ysmGlbHeap() );

  
  yosxPutUB1(x, SYSX_BYTE_ORDER); 
  yosxPutUB4(x, YEEVLG_BINFILE_MARKER); 
  yosxPutUB4(x, (ub4)0);	
  yosxGetPos(x,&lp); 
  yosxPutUB4(x, (ub4)0);	

  
  yosxGetPos(x,&sp);   
  yotkEncode( YCTC_yeevl_yeevlr, (dvoid*)lrec, x, YOGIIOR_TAG_YO );
  yosxGetPos(x,&ep);   
  *reclen = yosxGetLength( x, &sp, &ep );
  yosxPutUB4AtPos(x,&lp,*reclen);
  yosxEncEnd( x, bv, nbv );
}





STATICF void yeevlgConsole( sword level, CONST char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  syslConsole(level, fmt, ap);
  va_end(ap);
}






STATICF char *yeevlgGets( sysfp *fp, char *buf, size_t bufsiz )
{
  sword ch;
  size_t i;

  
  bufsiz--;
  for( i = 0 ; i < bufsiz && !sysfpGetc( fp, &ch ) && ch != '\n' ; )
    buf[i++] = (char)ch;

  buf[i] = 0;
  return( i ? buf : (char*)0 );
}




STATICF void yeevlgTextOpen( yeevlglg *lg, CONST char *mode,
			    CONST char **errptr )
{
  char buf[ 512 ];
  yeevl_OperationFailed exv;
  
  if( !(lg->fp_yeevlglg = sysfpOpen( lg->fixattr_yeevlglg.file_yeevld,
				    mode, SYSFPKIND_TEXT, errptr )) )
  {
    yslError("%s: sysfpOpen error on file %s, mode %s, kind %s, reason %s",
	     "yeevlgTextOpen",
	     lg->fixattr_yeevlglg.file_yeevld,
	     mode, "text", *errptr ); 
	     
    
    ysRecord(  YS_PRODUCT, YEEVLG_FAC, (ub4)330, YSLSEV_ERR, (char*)0,
	     YSLSTR("yeevlgTextOpen"),
	     YSLSTR(lg->fixattr_yeevlglg.file_yeevld),
	     YSLSTR(mode),
	     YSLSTR("text"),
	     YSLSTR(*errptr), YSLEND );

    exv.reason = ysStrDupWaf( *errptr, yoAlloc );
    yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
  }
  lg->fperr_yeevlglg = FALSE;
  if( *mode == 'w' )		
  {
    DISCARD sysfpPrint( lg->fp_yeevlglg, "%s\n", yeevTextfile_tag);
  }
  else			
  {
    DISCARD yeevlgGets( lg->fp_yeevlglg, buf, sizeof(buf) );
    if( strcmp( buf, yeevTextfile_tag ) )
    {
      exv.reason = ysStrDupWaf( "not textfile log", yoAlloc );
      yeevlgSysfpClose( lg );
      yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
    }
  }
  sysfpTell( lg->fp_yeevlglg, &lg->bpos_yeevlglg );
  lg->rpos_yeevlglg = lg->wpos_yeevlglg = lg->bpos_yeevlglg;
}




STATICF void yeevlgBinOpen( yeevlglg *lg, CONST char *mode,
			   CONST char **errptr )
{
  char buf[ 100 ];
  yeevl_OperationFailed exv;
  
  if( !(lg->fp_yeevlglg = sysfpOpen( lg->fixattr_yeevlglg.file_yeevld,
				    mode, SYSFPKIND_BINARY, errptr )) )
  {
    yslError("%s: sysfpOpen error on file %s, mode %s, kind %s, reason %s",
	     "yeevlgBinOpen",
	     lg->fixattr_yeevlglg.file_yeevld,
	     mode, "binary", *errptr ); 
	     
    
    ysRecord(  YS_PRODUCT, YEEVLG_FAC, (ub4)330, YSLSEV_ERR, (char*)0,
	     YSLSTR("yeevlgBinOpen"),
	     YSLSTR(lg->fixattr_yeevlglg.file_yeevld),
	     YSLSTR(mode),
	     YSLSTR("binary"),
	     YSLSTR(*errptr), YSLEND );

    exv.reason = ysStrDupWaf( *errptr, yoAlloc );
    yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
  }
  lg->fperr_yeevlglg = FALSE;
  if( *mode == 'w' )		
  {
    DISCARD sysfpWrite(lg->fp_yeevlglg, (dvoid*)yeevBinfile_tag,
		       sizeof(yeevBinfile_tag) );
  }
  else				
  {
    DISCARD sysfpRead( lg->fp_yeevlglg, (dvoid*)buf, sizeof(yeevBinfile_tag) );
    if( memcmp( buf, (dvoid*)yeevBinfile_tag, sizeof(yeevBinfile_tag) ) )
    {
      exv.reason = ysStrDupWaf( "not binfile log", yoAlloc );
      yeevlgSysfpClose( lg );
      yseThrowObj( YEEVL_EX_OPERATIONFAILED, exv );
    }
  }
  sysfpTell( lg->fp_yeevlglg, &lg->bpos_yeevlglg );
  lg->rpos_yeevlglg = lg->wpos_yeevlglg = lg->bpos_yeevlglg;
}





STATICF void yeevlgSplit( char *str, char **fields, sword nf )
{
  while( *str && nf-- )
  {
    *fields = str;
    while(*str && *str != ';' )
    {
      if( *str && *str++ == '=' )
      {
	*fields = str;
	if( !nf )
	  break;
      }
    }
    if( *str && *str == ';' )
      *str++ = 0;
    fields++;
  }
  *fields = (char*)0;
}





STATICF void yeevlgFlush( yeevlglg *rlg )
{
  yeevlglg  *wlg;
  ysle	    *e;

  for( e = ysLstHead( rlg->cx_yeevlglg->logs_yeevlgcx) ; e ; e = ysLstNext(e))
  {
    wlg = (yeevlglg*)ysLstVal( e );
    if( wlg != rlg 
       && wlg->fixattr_yeevlglg.oper_yeevld == yeevl_enabled_operState
       && !strcmp( wlg->fixattr_yeevlglg.file_yeevld,
		  rlg->fixattr_yeevlglg.file_yeevld ))
      yeevlgLogFlush( wlg );
  }
}






STATICF void yeevlgConnDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			     size_t argsz )
{
  yeevlgs *lds = (yeevlgs*)usrp;
  yeevl_sender_destroy_i( lds->self_yeevlgs, &lds->cx_yeevlgs->env_yeevlgcx);
}




STATICF void yeevlgSysfpClose( yeevlglg *lg )
{
  if( !lg->fp_yeevlglg )
    yseThrow( YS_EX_BADPARAM );

  sysfpClose( lg->fp_yeevlglg );
  lg->fp_yeevlglg = (sysfp*)0;
  lg->fperr_yeevlglg = FALSE;
}






yeevReceiver yeevl_logProc_createReceiver_i( yeevl_logProc or, yoenv* ev,
					    char* name)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  char buf[256];

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)340, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  ysFmtStr( buf, "log:%s", name );
  return( yeev_createReceiver( cx->ev_yeevlgcx, ev, buf ) );
}

yeevDiscList yeevl_logProc_createDiscList_i( yeevl_logProc or, yoenv* ev,
					    yeevdSeq* dlist)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)341, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeev_createDiscList( cx->ev_yeevlgcx, ev, dlist ) );
}

void yeevl_logProc_raise_i( yeevl_logProc or, yoenv* ev, yeevr* rec)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)342, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeev_raise( cx->ev_yeevlgcx, ev, rec );
}

void yeevl_logProc_raiseMany_i( yeevl_logProc or, yoenv* ev, yeevrList* recs)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)343, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeev_raiseMany( cx->ev_yeevlgcx, ev, recs );
}

void yeevl_logProc_forward_i( yeevl_logProc or, yoenv* ev, yeevr* rec)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)344, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeev_forward( cx->ev_yeevlgcx, ev, rec );
}



yeevl_sender yeevl_logProc_connectSender_i( yeevl_logProc or, yoenv* ev,
					   yeev evsender,
					   char* host, char* pid,
					   char* affinity,
					   char* prog,
					   yeevdSeq* quals)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevlgs *lds;
  ysspNode *n;
  yeevl_yeevls	*s;
  char buf[ 256 ];
  char qbuf[ 160 ];
  char *rs;
  yeevdSeq dlist;
  ub4 i;
  yeevl_OperationFailed fex;

  
  if( cx->nsenders_yeevlgcx )
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)345, YSLSEV_DEBUG(8), (char*)0,
	     YSLSB4(cx->nsenders_yeevlgcx ), YSLEND);
  
  if( !cx->ev_yeevlgcx )
  {
    fex.reason = ysStrDupWaf("yeev not up", yoAlloc);
    yseThrowObj( YEEVL_EX_OPERATIONFAILED, fex );
  }

  if( (n = ysspLookup( (dvoid*)evsender, &cx->senders_yeevlgcx ) ) )
  {
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)346, YSLSEV_DEBUG(8), (char*)0, 
	     YSLPTR(evsender), YSLPTR(cx->ev_yeevlgcx), YSLEND);

    lds = (yeevlgs *)n;

    
    if( yoIsEq( (dvoid*)evsender, (dvoid*)cx->ev_yeevlgcx ) )
    {
      s = &lds->obj_yeevlgs;
      quals->_maximum = quals->_length = 1;
      quals->_buffer = (yeevd*)yoAlloc(sizeof(yeevd) );
      rs = yoRefToStr( (dvoid*)evsender );
      ysFmtStr( qbuf, "NOT maxsev 16", rs );
      yoFree( (dvoid*)rs );
      quals->_buffer[0].qual_yeevd = ysStrDupWaf( qbuf, yoAlloc );
      quals->_buffer[0].dest_yeevd =
	(yeevReceiver)yoDuplicate((dvoid*)s->rcvr_yeevls);
      
      return( (yeevl_sender)yoDuplicate( (dvoid*)lds->self_yeevlgs ) );
    }
    else
      yseThrow( YEEVL_EX_ALREADYCONNECTED );
  }

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)347, YSLSEV_DEBUG(8), (char*)0,
	   YSLPTR(evsender), YSLEND);

  lds = (yeevlgs*)ysmGlbAlloc(sizeof(*lds), yeevlgs_tag );
  lds->cx_yeevlgs = cx;
  lds->node_yeevlgs.key_ysspNode = yoDuplicate( (dvoid*)evsender );
  lds->self_yeevlgs = (yeevl_sender)yoCreate( yeevl_sender__id, (char*)0,
					     (yoRefData*)0, (char*)0,
					     (dvoid*)lds );
  
  
  lds->devt_yeevlgs = ysEvtCreate( yeevlgConnDeath, (dvoid*)lds,
				  cx->q_yeevlgcx, TRUE );
  yoWatchOwner( (dvoid*)evsender, lds->devt_yeevlgs );

  if( affinity && *affinity )
    ysFmtStr( buf, "%s:%s:%s", host, pid, affinity );
  else
    ysFmtStr( buf, "%s:%s", host, pid );
    
  s = &lds->obj_yeevlgs;
  s->rcvr_yeevls = yeev_createReceiver( cx->ev_yeevlgcx,
				       &cx->env_yeevlgcx, buf );
  DISCARD yeevReceiverSetHandler( s->rcvr_yeevls,
				 yeevlgSenderHandler, (dvoid*)lds );
  s->sender_yeevls = (yeev)lds->node_yeevlgs.key_ysspNode;
  s->host_yeevls = ysStrDup( host );
  s->pid_yeevls = ysStrDup( pid );
  s->affinity_yeevls = ysStrDup( affinity );
  s->prog_yeevls = ysStrDup( prog );
  ysClock( &s->lastsend_yeevls );
  DISCARD ysspEnq( &lds->node_yeevlgs, &cx->senders_yeevlgcx );
  cx->nsenders_yeevlgcx++;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)348, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  
  if( yoIsEq( (dvoid*)evsender, (dvoid*)cx->ev_yeevlgcx ) )
  {
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)349,YSLSEV_DEBUG(8), (char*)0,YSLNONE);

    quals->_maximum = quals->_length = 1;
    quals->_buffer = (yeevd*)yoAlloc(sizeof(yeevd) );
    rs = yoRefToStr( (dvoid*)evsender );
    ysFmtStr( qbuf, "NOT maxsev 16", rs );
    yoFree( (dvoid*)rs );
    quals->_buffer[0].qual_yeevd = ysStrDupWaf( qbuf, yoAlloc );
    quals->_buffer[0].dest_yeevd =
      (yeevReceiver)yoDuplicate((dvoid*)s->rcvr_yeevls);
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)350,YSLSEV_DEBUG(8), (char*)0,YSLNONE);

    
    dlist = yeevDiscList__get_dlist( cx->ddl_yeevlgcx, &cx->env_yeevlgcx );
    
    
    for( i = 0; i < dlist._length ; i++ )
    {
      yoRelease( (dvoid*)dlist._buffer[i].dest_yeevd );
      dlist._buffer[i].dest_yeevd =
	(yeevReceiver)yoDuplicate((dvoid*)s->rcvr_yeevls);
    }
    
    
    yeevdSeq__copy( quals, &dlist, yoAlloc );
    yeevdSeq__free( &dlist, yoFree );
  }

  if( cx->nsenders_yeevlgcx > 1 )
  {
    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)351, YSLSEV_NOTICE, (char*)0,
	     YSLPTR(lds->self_yeevlgs), YSLPTR(s->rcvr_yeevls),
	     YSLSTR(buf), YSLEND);
  }

  

  return( (yeevl_sender)yoDuplicate( (dvoid*)lds->self_yeevlgs ) );
}

yeevl_log yeevl_logProc_createLog_i( yeevl_logProc or, yoenv* ev,
				    yeevl_yeevld* fixedattrs,
				    yeevl_yeevla* varattrs)
{
  
  ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)352, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeevlgLogCreate( or, ev, fixedattrs, varattrs, (char*)"w+" ) );
}

yeevl_log yeevl_logProc_openLog_i( yeevl_logProc or, yoenv* ev,
				  yeevl_yeevld* fixedattrs,
				  yeevl_yeevla* varattrs)
{
  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)353, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeevlgLogCreate( or, ev, fixedattrs, varattrs, (char*)"r" ) );
} 


void yeevl_logProc_shutdown_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)354, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yoShutdown( cx->q_yeevlgcx );
}


yeevInfo yeevl_logProc__get_info_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)355, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeev__get_info( cx->ev_yeevlgcx, ev ) );
}

yeevReceiverList yeevl_logProc__get_receivers_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)356, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeev__get_receivers( cx->ev_yeevlgcx, ev ) );
}

yeevDiscList yeevl_logProc__get_filters_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)357, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( yeev__get_filters( cx->ev_yeevlgcx, ev ) );
}

void yeevl_logProc__set_filters_i( yeevl_logProc or, yoenv* ev,
				  yeevDiscList val)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)358, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeev__set_filters( cx->ev_yeevlgcx, ev, val );
}

boolean yeevl_logProc__get_limit_drop_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  boolean rv;

  rv = yeev__get_limit_drop( cx->ev_yeevlgcx, ev );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)359, YSLSEV_DEBUG(8), (char*)0,
	   YSLSTR(rv? "TRUE": "FALSE"), YSLEND);

  return( rv );
}

void yeevl_logProc__set_limit_drop_i( yeevl_logProc or, yoenv* ev, boolean val)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)360, YSLSEV_DEBUG(8), (char*)0,
	   YSLSTR(val ? "TRUE" : "FALSE"), YSLEND);

  yeev__set_limit_drop( cx->ev_yeevlgcx, ev, val );
}


ub4 yeevl_logProc__get_globalEventHighWater_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  ub4 rv = yeev__get_globalEventHighWater( cx->ev_yeevlgcx, ev );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)361, YSLSEV_DEBUG(8), (char*)0,
	   YSLUB4(rv), YSLEND);

  return( rv );
}

void yeevl_logProc__set_globalEventHighWater_i( yeevl_logProc or, yoenv* ev,
					       ub4 val)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)362, YSLSEV_DEBUG(8), (char*)0,
	   YSLUB4(val), YSLEND);

  yeev__set_globalEventHighWater( cx->ev_yeevlgcx, ev, val );
}

ub4 yeevl_logProc__get_globalEventRestart_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  ub4 rv = yeev__get_globalEventRestart( cx->ev_yeevlgcx, ev );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)363, YSLSEV_DEBUG(8), (char*)0,
	   YSLUB4(rv), YSLEND);

  return( rv );
}

void yeevl_logProc__set_globalEventRestart_i( yeevl_logProc or, yoenv* ev,
					     ub4 val)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)364, YSLSEV_DEBUG(8), (char*)0,
	   YSLUB4(val), YSLEND);

  yeev__set_globalEventRestart( cx->ev_yeevlgcx, ev, val );
}



yeevDiscList yeevl_logProc__get_defQuals_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)365, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( (yeevDiscList)yoDuplicate( (dvoid*)cx->ddl_yeevlgcx ) );
}


void yeevl_logProc__set_defQuals_i( yeevl_logProc or, yoenv* ev,
				   yeevDiscList val)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  
  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)366, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  if( val )
  {
    yoRelease( (dvoid*)cx->ddl_yeevlgcx );
    cx->ddl_yeevlgcx = (yeevDiscList)yoDuplicate((dvoid*)val);
  }
}



yeevl_yeevlSenderList
yeevl_logProc__get_senders_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevl_yeevlSenderList rv;
  ysspNode  *n;
  yeevlgs *lds;
  ub4 i;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)367, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  rv._maximum = cx->nsenders_yeevlgcx;
  rv._buffer =
    (yeevl_sender*)yoAlloc( (size_t)rv._maximum*sizeof(yeevl_sender) );
  
  for( i= 0, n = ysspFHead( &cx->senders_yeevlgcx ) ; n ; n = ysspFNext( n ) )
  {
    lds = (yeevlgs*)n;
    rv._buffer[i++] = (yeevl_sender)yoDuplicate( (dvoid*)lds->self_yeevlgs );
  }
  rv._length = i;
  return rv;
}


yeevl_yeevlList yeevl_logProc__get_logs_i( yeevl_logProc or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevl_yeevlList rv;
  ub4 i;
  ysle *e;
  yeevlglg  *lg;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC,(ub4)368, YSLSEV_DEBUG(8), (char*)0,YSLNONE); 

  rv._maximum = ysLstCount( cx->logs_yeevlgcx );
  rv._buffer =
    (yeevl_log*)ysmGlbAlloc( sizeof(yeevl_log) * (size_t)rv._maximum,
			    "yeev_log");
  
  for( i = 0, e = ysLstHead(cx->logs_yeevlgcx); e ; i++, e = ysLstNext(e) )
  {
    lg = (yeevlglg*)ysLstVal(e);
    rv._buffer[i] = (yeevl_log)yoDuplicate( (dvoid*)lg->self_yeevlglg );
  }
  rv._length = i;
  return rv;
}





void yeevl_sender_destroy_i( yeevl_sender or, yoenv* ev)
{
  yeevlgs *lds = (yeevlgs*)yoGetState((dvoid*)or );
  yeevReceiver r;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)371, YSLSEV_NOTICE, (char*)0,
	   YSLPTR(lds->self_yeevlgs),
	   YSLPTR(lds->obj_yeevlgs.rcvr_yeevls),
	   YSLSTR(lds->obj_yeevlgs.host_yeevls),
	   YSLSTR(lds->obj_yeevlgs.pid_yeevls),
	   YSLSTR( lds->obj_yeevlgs.affinity_yeevls ? ":" : ""),
	   YSLSTR( lds->obj_yeevlgs.affinity_yeevls ?
		  lds->obj_yeevlgs.affinity_yeevls : ""),
	   YSLEND);

  r = (yeevReceiver)yoDuplicate( (dvoid*)lds->obj_yeevlgs.rcvr_yeevls );
  ysspRemove( &lds->node_yeevlgs, &lds->cx_yeevlgs->senders_yeevlgcx );
  yeevl_yeevls__free( &lds->obj_yeevlgs, (ysmff)0 );

  yseTry
  {
    yeevReceiver_destroy( r, &lds->cx_yeevlgs->env_yeevlgcx );
  }
  yseCatch( YO_EX_BADOBJ )
  {
    
  }
  yseEnd;

  ysEvtDestroy( lds->devt_yeevlgs );
  yoDispose( (dvoid*)or );
  ysmGlbFree( (dvoid*)lds );
}


yeevl_yeevls yeevl_sender__get_info_i( yeevl_sender or, yoenv* ev)
{
  yeevlgs *lds = (yeevlgs*)yoGetState((dvoid*)or );
  yeevl_yeevls rv;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)372, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevl_yeevls__copy( &rv, &lds->obj_yeevlgs, yoAlloc );  
  return( rv );
}





void yeevl_log_destroy_i( yeevl_log or, yoenv* ev)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)373, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  DISCARD ysLstRem( lg->cx_yeevlglg->logs_yeevlgcx, lg->le_yeevlglg );
  yeevlgLogDestroy( (dvoid*)lg );
}

void yeevl_log_push_i( yeevl_log or, yoenv* ev, yeevr* rec)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)374, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevReceiver_push( lg->r_yeevlglg, ev, rec );
}

void yeevl_log_pushMany_i( yeevl_log or, yoenv* ev, yeevrList* recs)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)375, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevReceiver_pushMany( lg->r_yeevlglg, ev, recs );
}


void yeevl_log_pull_i( yeevl_log or, yoenv* ev, yeevr* rec)
{
  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)376, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  
  CLRSTRUCT(*rec);
}


void yeevl_log_pullMany_i( yeevl_log or, yoenv* ev, yeevrList* recs)
{
  
  recs->_length = recs->_maximum = 0;
  recs->_buffer = (yeevr*)0;
}


boolean yeevl_log_tryPull_i( yeevl_log or, yoenv* ev, yeevr* rec)
{
  
  CLRSTRUCT(*rec);
  return( FALSE );
}


boolean yeevl_log_tryPullMany_i( yeevl_log or, yoenv* ev, yeevrList* recs)
{
  
  recs->_length = recs->_maximum = 0;
  recs->_buffer = (yeevr*)0;
  return( FALSE );
}


void yeevl_log_deleteRecord_i( yeevl_log or, yoenv* ev, sb4 id)
{
  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)377, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  
  yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);
}


void yeevl_log_getRecord_i( yeevl_log or, yoenv* ev, sb4 id,
			   yeevl_yeevlr* record)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  yeevl_yeevlr lrec;
  boolean lhave = FALSE;
  boolean got = FALSE;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)378, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  switch( lg->fixattr_yeevlglg.type_yeevld )
  {
  case yeevl_binfile_logType:
  case yeevl_textfile_logType:

    
    if( id < lg->rrec_yeevlglg )
    {
      lg->rpos_yeevlglg = lg->bpos_yeevlglg;
      lg->rrec_yeevlglg = 0;
    }

    
    while( id < lg->rrec_yeevlglg )
    {
      if( lhave )
	yeevl_yeevlr__free( &lrec, (ysmff)0 );
      if( yeevlGetRec( lg, &lrec ) )
	break;
      lhave = TRUE;
      lg->rrec_yeevlglg = lrec.record_id;
      sysfpTell( lg->fp_yeevlglg, &lg->rpos_yeevlglg ); 
    }

    if( lhave && id == lrec.record_id )
    {
      got = TRUE;
      yeevl_yeevlr__copy( record, &lrec, yoAlloc ); 
    }

    if( lhave )
      yeevl_yeevlr__free( &lrec, (ysmff)0 );

    if( !got )
      yseThrow(YEEVL_EX_NOTFOUND);

    break;

  case yeevl_sysbin_logType:
    
    yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);
    break;

  default:

    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)320, YSLSEV_ERR, (char*)0,
	     YSLSB4(lg->fixattr_yeevlglg.type_yeevld), 
	     YSLSTR("yeevl_log_getRecord_i"), YSLEND);

    yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);
    break;
  }
}

void yeevl_log_getRecordRange_i( yeevl_log or, yoenv* ev, sb4 id,
				sb4 count, yeevdSeq *dlist, boolean keep,
				yeevl_yeevlrList* records,
				yeevl_listIterator* li)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  sb4 z = 0;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)379, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  *li = yeevlgIteratorCreate( cx, lg, FALSE, &id, &z,
			     sysb8zero, sysb8zero, dlist );
  if( count && (!yeevl_listIterator_getNextN( *li, ev, count, records ) ||
		records->_length != (ub4)count ) )
  {
    if( !keep )
    {
      yeevl_listIterator_destroy( *li, ev );
      *li = (yeevl_listIterator)0;
    }
  }
}

void yeevl_log_getRecordTime_i( yeevl_log or, yoenv* ev,
			       sysb8 early, sysb8 late, sb4 count,
			       yeevdSeq *dlist, boolean keep,
			       yeevl_yeevlrList* records,
			       yeevl_listIterator* li)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  sysb8	bt;
  sysb8 et;
  sb4	z = 0;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)380, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  sysb8set( &bt, &early );
  sysb8set( &et, &late );
  *li = yeevlgIteratorCreate( cx, lg, TRUE, &z, &z,
			     &bt, &et, dlist );
  if( count && (!yeevl_listIterator_getNextN( *li, ev, count, records ) ||
		records->_length != (ub4)count ) )
  {
    if( !keep )
    {
      yeevl_listIterator_destroy( *li, ev );
      *li = (yeevl_listIterator)0;
    }
  }
}


char* yeevl_log__get_name_i( yeevl_log or, yoenv* ev)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)381, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  return( ysStrDupWaf( lg->fixattr_yeevlglg.file_yeevld, yoAlloc ) );
}


yeevl_yeevld yeevl_log__get_fixedattr_i( yeevl_log or, yoenv* ev)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  yeevl_yeevld rv;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)382, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevl_yeevld__copy( &rv, &lg->fixattr_yeevlglg, yoAlloc );
  return rv;
}


yeevl_yeevla yeevl_log__get_varattr_i( yeevl_log or, yoenv* ev)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  yeevl_yeevla rv;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)383, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevl_yeevla__copy( &rv, &lg->varattr_yeevlglg, yoAlloc );
  return rv;
}


void yeevl_log__set_varattr_i( yeevl_log or, yoenv* ev, yeevl_yeevla* val)
{
  yeevlglg *lg = (yeevlglg*)yoGetState((dvoid*)or);
  yeevl_yeevla lval;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)384, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  yeevl_yeevla__copy( &lval, val, (ysmaf)0 );
  yeevl_yeevla__free( &lg->varattr_yeevlglg, (ysmff)0 );
  yeevl_yeevla__copy( &lg->varattr_yeevlglg, val, (ysmaf)0 );
  yeevl_yeevla__free( &lval, (ysmff)0 );
}


boolean yeevl_listIterator_getNextOne_i( yeevl_listIterator or, yoenv* ev,
					yeevl_yeevlr* record)
{
  boolean rv;
  yeevl_yeevlrList list;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)385, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  rv = yeevl_listIterator_getNextN( or, ev, (sb4)1, &list );
  if( list._length )
    yeevl_yeevlr__copy( record, &list._buffer[0], yoAlloc );
  yeevl_yeevlrList__free( &list, yoFree );
  return rv;
}


boolean yeevl_listIterator_getNextN_i( yeevl_listIterator or, yoenv* ev,
				      sb4 count, yeevl_yeevlrList* records)
{
  yeevlgi *ldi = (yeevlgi*)yoGetState( (dvoid*)or );
  yeevlglg *lg = ldi->lg_yeevlgi;
  boolean got, stop, lhave;
  yeevl_yeevlr lrec;

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)386, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  got = stop = lhave = FALSE;
  
  records->_length = records->_maximum = 0;
  records->_buffer = (yeevl_yeevlr*)0;

  if( !count )
    return FALSE;

  switch( lg->fixattr_yeevlglg.type_yeevld )
  {
  case yeevl_binfile_logType:
  case yeevl_textfile_logType:
  case yeevl_sysbin_logType:

    if( ldi->first_yeevlgi )
    {
      
      for( got = FALSE; !got ; )
      {
	if( lhave )
	  yeevl_yeevlr__free( &lrec, (ysmff)0 );
	if( (stop = yeevlGetRec( lg, &lrec )))
	  break;
	lhave = TRUE;
	lg->rrec_yeevlglg = lrec.record_id;

	if( ldi->bytime_yeevlgi )
	{
	  if( sysb8cmp( &lrec.timestamp, >= , &ldi->startTime_yeevlgi ) )
	    got = TRUE;
	}
	else			
	{
	  if( lrec.record_id >= ldi->startRec_yeevlgi )
	    got = TRUE;
	}
      }
    }

    if( got )
      ldi->first_yeevlgi = FALSE;

    records->_length = 0;
    records->_maximum = (ub4)count;
    records->_buffer =
      (yeevl_yeevlr*)yoAlloc( (size_t)count * sizeof( yeevl_yeevlr ));
    while( records->_length < (ub4)count && !stop )
    {
      if( ldi->bytime_yeevlgi )
      {
	if( !sysb8cmp( &ldi->stopTime_yeevlgi, ==, sysb8zero ) &&
	   sysb8cmp( &lrec.timestamp, >= , &ldi->stopTime_yeevlgi ) )
	  break;
      }
      else
      {
	if( ldi->stopRec_yeevlgi && (lrec.record_id >= ldi->stopRec_yeevlgi) )
	  break;
      }

      if( !lhave )
	if( (stop = yeevlGetRec( lg, &lrec )))
	  break;
	
      if(ldi->dl_yeevlgi ? yeevEvaluate(&lrec.record, ldi->dl_yeevlgi) : TRUE)
	yeevl_yeevlr__copy( &records->_buffer[records->_length++],
			   &lrec, yoAlloc ); 

      yeevl_yeevlr__free( &lrec, (ysmff)0 );
      lhave = FALSE;
    }

    if( lhave )
      yeevl_yeevlr__free( &lrec, (ysmff)0 );

    break;

  default:

    
    ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)320, YSLSEV_ERR, (char*)0,
	     YSLSB4(ldi->lg_yeevlgi->fixattr_yeevlglg.type_yeevld),
	     YSLSTR("yeevl_listIterator_getNextN_i"), YSLEND);

    yseThrow(YEEVL_EX_NOTSUPPORTEDFORTHISLOGTYPE);
    break;
  }
  return stop;
}


void yeevl_listIterator_destroy_i( yeevl_listIterator or, yoenv* ev)
{
  yeevlgcx *cx = (yeevlgcx*)yoGetImplState( (dvoid*)or );
  yeevlgi *ldi = (yeevlgi*)yoGetState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YEEVLG_FAC, (ub4)387, YSLSEV_DEBUG(8), (char*)0,YSLNONE);

  DISCARD ysLstRem( cx->iters_yeevlgcx, ldi->le_yeevlgi );
  yeevlgIteratorDestroy( (dvoid*)ldi );
}

