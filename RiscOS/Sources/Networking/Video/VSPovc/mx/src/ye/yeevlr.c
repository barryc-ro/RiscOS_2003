/* mx/src/ye/yeevlr.c */


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
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
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
#ifndef YDIDL_ORACLE
#include <ydidl.h>
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

#define ENTRY_POINT yeevlrMain
#include <s0ysmain.c>



#define BATCHSIZE (sb4)10

#define FMTBUF_SIZE ((size_t)32*1024)

#define yeevlrStr( s ) (s ? s : (char *)"<null>")


static struct ysargmap yeevlrmap[] =
{
  { 'c', "mnlogreader.continuous-head=true", 0},
  { 'e', "mnlogreader.continuous-tail-events=true", 0},
  { 'f', "mnlogreader.filter-spec", 1},
  { 'i', "mnlogreader.log-file-name", 1},
  { 'm', "ys.log.msg-path", YSARG_MANY},
  { 'n', "mnlogreader.implementation-name", 1 },
  { 't', "mnlogreader.log-dest-type", 1},
  { 0, (char *) 0, 0 }
};

typedef struct yeevlrcx yeevlrcx;


struct yeevlrcx
{
  yemsgcx	*yemsg_yeevlrcx;
  yoenv		env_yeevlrcx;
  yeev		ev_yeevlrcx;
  boolean	exiting_yeevlrcx;
  boolean	cleaned_yeevlrcx;
  yeevlgcx	*lg_yeevlrcx;

  char		*fmtbuf;
  boolean	tail;		
  yeevReceiver	rcv;
  yslst		*filters;
  boolean	destroyf;	
  yeevl_log	log;
  yeevl_listIterator li;
  yeevDiscList	dlist;		

} ;


externdef ysmtagDecl(yeevlrs_tag) = "yeevlrs";


externdef ysidDecl( YEEVLR_EX_BAD_LTYPE ) = "::yeevlr::badLType";

STATICF void yeevlrPanicHdlr(CONST ysid *exid, dvoid *ptr);
STATICF void yeevlrCleanup( yeevlrcx *cx );
STATICF void yeevlr_eventHandler( yeevr *rec, dvoid *usrp );


STATICF yeevl_logType yeevlrLType( CONST char *s );






boolean yeevlrMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  noreg boolean rv = TRUE;
  char      vbuf[80];
  sb4	    sts;
  yeevlrcx  cx;
  char	    *type;
  char	    *path;
  char	    *impl;
  boolean   head = FALSE;
  boolean   nomore = FALSE;
  sysb8	    waittime;
  ysevt	    *sem;
  
  yeevl_logProc	logger;
  yeevl_yeevld fixedattrs;
  yeevl_yeevla varattrs;
  yeevl_yeevlrList  rlist;
  yeevdSeq dlist;
  
  yeevl_yeevlr	*lrec;
  ysle	    *e;
  ub4	    i;
  char	    *f;
  
  NOREG(rv);

  cx.li = (yeevl_listIterator)0; 
  cx.rcv = (yeevReceiver)0;
  cx.dlist = (yeevDiscList)0;
  cx.log = (yeevl_log)0;

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, yeevlrmap);
  if (sts == YSARG_VERSION)
  {
    yslError("Oracle Media Exchange ORB Log Reader Client Program");
    vbuf[0] = 0;
    yslError(ysVersion(vbuf, sizeof(vbuf)));
  }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  DISCARD yseSetPanic(yeevlrPanicHdlr, (dvoid*)&cx);
  
  yseTry
  {
    ytInit();
    yoInit();
    
    yoEnvInit( &cx.env_yeevlrcx );
    cx.fmtbuf = (char*)ysmGlbAlloc( FMTBUF_SIZE, "format buffer" );
  
    
    sysb8ext( &waittime, 2*1000000 );
  
    
    cx.yemsg_yeevlrcx = yemsgInit();

    type = ysResGetLast("mnlogreader.log-dest-type");
    path = ysResGetLast("mnlogreader.log-file-name");
    impl = ysResGetLast("mnlogreader.implementation-name");
    cx.tail = ysResGetBool("mnlogreader.continuous-tail-events");
    head = ysResGetBool("mnlogreader.continuous-head");
    
    if( !type )
      type = (char*)"binfile";
    if( !path )
      path = (char*)"yeevld.log";
    
    cx.ev_yeevlrcx = yeevInit( (ysque*)0 );
    cx.exiting_yeevlrcx = FALSE;
    cx.cleaned_yeevlrcx = FALSE;
    
    
    cx.lg_yeevlrcx = yeevlgInit( (ysque*)0, cx.ev_yeevlrcx, FALSE );

    cx.destroyf = FALSE;
    cx.filters = ysResGet("mnlogreader.filter-spec");
    
    
    ysFmtStr( vbuf, "%s:%s %s rcv",
	     ysGetHostName(), ysGetPid(), ysProgName());
    cx.rcv = yeev_createReceiver( cx.ev_yeevlrcx, &cx.env_yeevlrcx, vbuf );

    if( !cx.filters )
    {
      cx.destroyf = TRUE;
      cx.filters = ysLstCreate();
    }
    if( !ysLstCount(cx.filters) )
      
      DISCARD ysLstEnq( cx.filters, (dvoid*)0 ); 
    dlist._maximum = ysLstCount( cx.filters );
    if( !dlist._maximum )
      dlist._maximum = 1;
    dlist._buffer =
      (yeevd*)ysmGlbAlloc( sizeof(yeevd) * (size_t)dlist._maximum, "dlist"); 
    for( i = 0, e = ysLstHead( cx.filters ); e ; i++, e = ysLstNext(e) )
    {
      f = (char*)ysLstVal(e);
      dlist._buffer[i].qual_yeevd = f;
      dlist._buffer[i].dest_yeevd = cx.rcv;
    }
    dlist._length = i;
   
    if( cx.tail )		
    {
      if( !impl )		
	impl = "mnlogsrv";
	
      
      logger = (yeevl_logProc)yoBind( yeevl_logProc__id, impl,
				     (yoRefData*)0, (char*)0 );

      
      DISCARD yeevReceiverSetHandler(cx.rcv,yeevlr_eventHandler,(dvoid*)&cx);
  
      
      cx.dlist = yeevl_logProc__get_filters( logger, &cx.env_yeevlrcx );
      yeevDiscList_replaceDest( cx.dlist, &cx.env_yeevlrcx, &dlist );
      yoRelease( (dvoid*)logger ); 

      
      yoService( (ysque*)0 );
    }
    else			
    {
      
      logger = (yeevl_logProc)yoBind( yeevl_logProc__id, impl,
				     (yoRefData*)0, (char*)0 );

      
      fixedattrs.type_yeevld = yeevlrLType( type );
      fixedattrs.file_yeevld = path;
      fixedattrs.oper_yeevld = yeevl_disabled_operState;
      varattrs.maxLogSizeKb_yeevla = 0;
      varattrs.capAlarmThresholdKb_yeevla = 0;
      varattrs.keepCount_yeevla = 0;
      varattrs.fullAction_yeevla = yeevl_halt_fullType;
      varattrs.admin_yeevla = yeevl_locked_adminState;
	
      cx.log = yeevl_logProc_openLog( logger, &cx.env_yeevlrcx,
				     &fixedattrs, &varattrs );
      yoRelease( (dvoid*)logger ); 
	
      
      yeevl_log_getRecordRange( cx.log, &cx.env_yeevlrcx, (sb4)0, BATCHSIZE,
			       &dlist, TRUE, &rlist,
			       (yeevl_listIterator*)&cx.li );
	
      
      for( nomore = FALSE ; head || !nomore || rlist._length ; )
      {
	for( i = 0 ; i < rlist._length ; i++ )
	{
	  lrec = &rlist._buffer[i];
	  yeevShowLRec( cx.yemsg_yeevlrcx, lrec, cx.fmtbuf, FMTBUF_SIZE );
	}
	yeevl_yeevlrList__free( &rlist, yoFree );

	nomore = yeevl_listIterator_getNextN( cx.li, &cx.env_yeevlrcx,
					     BATCHSIZE, &rlist );
	if( !rlist._length && nomore && head)
	{
	  sem = ysSemCreate((dvoid*)0);
	  ysTimer( &waittime, sem );
	  ysSemWait( sem );
	  ysSemDestroy( sem );
	  sem = (ysevt*)0;
	}
      }
      yeevl_yeevlrList__free( &rlist, yoFree );
    }
  }
  yseCatchObj(YEEVL_EX_OPERATIONFAILED, yeevl_OperationFailed, exr )
  {
    yslError("%s %s: %s\n", ysProgName(), ysidToStr(yseExid), exr.reason );
    rv = FALSE;
  }
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
  {
    yslError("%s caught unexpected exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
    rv = FALSE;
  }
  yseEnd;
  
  

  yseTry
  {
    cx.exiting_yeevlrcx = TRUE;
    yeevlrCleanup( &cx );
    yeevTerm( cx.ev_yeevlrcx );

    ysmGlbFree((dvoid*)dlist._buffer);
    ysmGlbFree( (dvoid*)cx.fmtbuf );
    yoEnvFree( &cx.env_yeevlrcx );
    yemsgTerm(cx.yemsg_yeevlrcx);
  }
  yseCatch( YS_EX_INTERRUPT )
  {
    yslError("yeevlrMain: interrupt while exiting\n");
  }
  yseCatchAll
  {
    yslError("%s unexpected exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
    rv = FALSE;
  }
  yseEnd;      
  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;
  ysTerm(osdp);
  return rv;
}









STATICF void yeevlrPanicHdlr(CONST ysid *exid, dvoid *ptr)
{
  yeevlrcx *cx = (yeevlrcx *)ptr;

  yslError("yeevlrPanicHdlr: exception %s, ptr %x\n",
	   exid ? ysidToStr(exid) : "<none>" , ptr);

  if( cx && !cx->exiting_yeevlrcx )
  {
    cx->exiting_yeevlrcx = TRUE;
    yeevTerm( cx->ev_yeevlrcx );
  }
}






STATICF void yeevlrCleanup( yeevlrcx *cx )
{
  if( !cx->cleaned_yeevlrcx )
  {
    cx->cleaned_yeevlrcx = TRUE;

    if( cx->destroyf )
      ysLstDestroy( cx->filters, (ysmff)0 );
    
    
    if( cx->tail && cx->dlist )
      yeevDiscList_destroyDest( cx->dlist, &cx->env_yeevlrcx, cx->rcv );

    if( cx->li )
      yeevl_listIterator_destroy( cx->li, &cx->env_yeevlrcx );
    
    if( cx->log )
      yeevl_log_destroy( cx->log, &cx->env_yeevlrcx );
    
    if( !cx->exiting_yeevlrcx )
    {
      if( cx->rcv )
	yeevReceiver_destroy( cx->rcv, &cx->env_yeevlrcx );
    }
  }

  if( cx->lg_yeevlrcx )
  {
    yeevlgTerm( cx->lg_yeevlrcx );
    cx->lg_yeevlrcx = (yeevlgcx*)0;
  }
}





STATICF yeevl_logType yeevlrLType( CONST char *s )
{
  yeevl_logType rv = 0;

  if( !s )
    yseThrow( YEEVLR_EX_BAD_LTYPE );
  else if( !strcmp( s, "textfile" ) )
    rv = yeevl_textfile_logType;
  else if( !strcmp( s, "binfile" ) )
    rv = yeevl_binfile_logType;
  else if( !strcmp( s, "sysbin" ) )
    rv = yeevl_sysbin_logType;
  else if( !strcmp( s, "console" ) )
    rv = yeevl_console_logType;
  else if( !strcmp( s, "syslog" ) )
    rv = yeevl_syslog_logType;
  else if( !strcmp( s, "tty" ) )
    rv = yeevl_tty_logType;
  else
    yseThrow( YEEVLR_EX_BAD_LTYPE );
  return( rv );
}





STATICF void yeevlr_eventHandler( yeevr *rec, dvoid *usrp )
{
  yeevlrcx *cx = (yeevlrcx*)usrp;

  yeevShowRec( cx->yemsg_yeevlrcx, rec, cx->fmtbuf, FMTBUF_SIZE );
}




