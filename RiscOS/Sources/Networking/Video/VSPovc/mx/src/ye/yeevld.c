/* mx/src/ye/yeevld.c */


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
#ifndef YOIDL_ORACLE
#include <yoidl.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
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
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif

#define ENTRY_POINT yeevldMain
#include <s0ysmain.c>



#define YEEVLD_FAC "YEEVLD"

#define YEEVLD_DEF_DEST_TYPE	yeevl_tty_logType

#define YEEVLD_DEF_FILE_NAME	"mnlogsrv.log"


#define YEEVLD_DEF_MAX_SIZE    ((ub4)0)


#define YEEVLD_DEF_ALARM_SIZE	((ub4)0)


#define YEEVLD_DEF_KEEP_COUNT	((ub4)1)


#define YEEVLD_DEF_FULL_ACTION	yeevl_rollover_fullType


static struct ysargmap yeevldmap[] =
{
    { 'a', "mnlogsrv.log-file-alarm-size-kb", 1},
    { 'b', "mnlogsrv.log-file-limit-behaviour", 1},
    { 'c', "mnlogsrv.connect-to-producer", 1},
    { 'f', "yeevlg.default-filter-spec", 1},
    { 'k', "mnlogsrv.log-file-keep-count", 1},
    { 'm', "ys.log.msg-path", YSARG_MANY},
    { 'N', "yeev.log-impl", 1 },
    { 'n', "yeevlg.implementation-name", YSARG_MANY },
    { 'o', "mnlogsrv.log-file-name", 1},
    { 'p', "mnlogsrv.log-file-max-size-kb", 1},
    { 'r', "mnlogsrv.log-file-readonly=true", 0 },
    { 't', "mnlogsrv.log-type", 1},

    { 0, (char *) 0, 0 }
};

typedef struct yeevldcx yeevldcx;


struct yeevldcx
{
  ysque		*q_yeevldcx;	
  yeev		ev_yeevldcx;	
  yeevlgcx	*lg_yeevldcx;	
  yeevReceiver	rcv_yeevldcx;	
  ysevt	    	*devt_yeevldcx;	
  yoenv		env_yeevldcx;
  boolean	panic_yeevldcx;
};


externdef ysidDecl( YEEVLD_EX_NO_YEEV ) = "::yeevld::noyeev";
externdef ysidDecl( YEEVLD_EX_STARTUP ) = "::yeevld::startup";

STATICF void yeevldServer( yeevldcx *cx );
STATICF void yeevldPanicHdlr(CONST ysid *exid, dvoid *ptr);
STATICF void yeevldAttach( yeevldcx *cx, char *attach_to );
STATICF void yeevldAttachHandler( yeevr *rec, dvoid *usrp );
STATICF void yeevldAttachDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			       size_t argsz );


STATICF void yeevldStartLog( yeevldcx *cx );






boolean yeevldMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  noreg boolean rv;
  char      vbuf[80];
  char	    *attach_to;
  sb4	    sts;
  yeevldcx  cx;
  
  NOREG(rv);

  
  CLRSTRUCT(cx);
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, yeevldmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Event Processor ");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);

  DISCARD yseSetPanic(yeevldPanicHdlr, (dvoid*)&cx);
  
  if (!ok)
    return(FALSE);
  
  yseTry
  {
    ytInit();
    yoInit();
    
    attach_to = ysResGetLast("mnlogsrv.connect-to-producer");
    
    cx.panic_yeevldcx = FALSE;
    yoEnvInit( &cx.env_yeevldcx );
    
    
    cx.q_yeevldcx = (ysque*)yoQueCreate("yeevld queue");
    cx.rcv_yeevldcx = (yeevReceiver)0;
    cx.devt_yeevldcx = (ysevt*)0;
    
    
    cx.ev_yeevldcx = yeevInit( cx.q_yeevldcx );
    
    
    cx.lg_yeevldcx = yeevlgInit( cx.q_yeevldcx, cx.ev_yeevldcx,
				attach_to ? FALSE : TRUE );
    
    
    yeevSinkAttach( cx.ev_yeevldcx, YSLSEV_INFO );
    
    rv = TRUE;
    if( attach_to )
      yeevldAttach( &cx, attach_to );
    yeevldServer( &cx );
  }
  yseCatch( YEEVLD_EX_STARTUP )
  {
    yslError("yeevldMain: startup error, exiting\n");
    rv = FALSE;
  }
  yseCatch( YS_EX_INTERRUPT )
  {
    yslError("%s caught %s, exiting\n", ysProgName(), yseExid);
    rv = FALSE;
  }
  yseCatchAll
  {
    yslError("yeevldMain: %s caught unexpected exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
    rv = FALSE;
  }
  yseEnd;
    
  

  yseTry
  {
    if( cx.devt_yeevldcx )
      ysEvtDestroy( cx.devt_yeevldcx );
    
    cx.panic_yeevldcx = TRUE;

    yeevlgTerm( cx.lg_yeevldcx );
    yeevSinkDetach( cx.ev_yeevldcx );
    
    
    
    
    
    yeevTerm( cx.ev_yeevldcx );
    
    yoQueDestroy( cx.q_yeevldcx );
    yoEnvFree( &cx.env_yeevldcx );
  }
  yseCatchAll
  {
    yslError("%s caught unexpected exception %s while exiting\n",
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





STATICF void yeevldPanicHdlr(CONST ysid *exid, dvoid *ptr)
{
  yeevldcx *cx = (yeevldcx *)ptr;

  yslError("yeevldPanicHdlr: exception %s\n", ysidToStr(exid));
  if( cx && !cx->panic_yeevldcx )
  {
    cx->panic_yeevldcx = TRUE;
    yslError("yeevldPanicHdlr: attempting yeevlgTerm\n");
    yeevlgTerm( cx->lg_yeevldcx );
  }
}




STATICF void yeevldServer( yeevldcx *cx )
{
  
  yeevldStartLog( cx );

  
  
  

  
  ysRecord(YS_PRODUCT,YEEVLD_FAC, (ub4)300, YSLSEV_INFO, (char*)0, YSLNONE);

  yseTry
  {
    
    ysRecord(YS_PRODUCT,YEEVLD_FAC,(ub4)304,YSLSEV_NOTICE, (char*)0, YSLNONE);

    
    ysRecord(YS_PRODUCT,YEEVLD_FAC,(ub4)305,YSLSEV_DEBUG(8),(char*)0,YSLNONE);

    
    yoService( cx->q_yeevldcx );

    
    ysRecord(YS_PRODUCT,YEEVLD_FAC,(ub4)306,YSLSEV_DEBUG(8),(char*)0,YSLNONE);

    
    ysRecord(YS_PRODUCT,YEEVLD_FAC,(ub4)307,YSLSEV_NOTICE,(char*)0, YSLNONE);
  }
  yseFinally
  {
    
    ysRecord(YS_PRODUCT,YEEVLD_FAC,(ub4)308,YSLSEV_DEBUG(1),(char*)0,YSLNONE);
  }
  yseEnd;
}




STATICF void yeevldStartLog( yeevldcx *cx )
{
  yeevl_log noreg lg = (yeevl_log)0;
  yeevl_yeevld fixed;
  yeevl_yeevla var;
  char *arg;
  yeevdSeq qlist;
  yeevDiscList dl;
  boolean ronly;
  yeevl_OperationFailed exv;
  ub4 i;
  
  NOREG(lg);
  CLRSTRUCT(exv);
  CLRSTRUCT(lg);

  if( !yeevlgLType(ysResGetLast("mnlogsrv.log-type"), &fixed.type_yeevld ) )
    fixed.type_yeevld = YEEVLD_DEF_DEST_TYPE;

  if( fixed.type_yeevld != yeevl_tty_logType )
     yslDetach();			

  if( !(arg = ysResGetLast("mnlogsrv.log-file-name") ) )
     arg = (char*)YEEVLD_DEF_FILE_NAME;
  fixed.file_yeevld = arg;

  arg = ysResGetLast( "mnlogsrv.log-file-max-size-kb" );
  var.maxLogSizeKb_yeevla =
    arg ? (ub4)atol(arg) : YEEVLD_DEF_MAX_SIZE;

  arg = ysResGetLast( "mnlogsrv.log-file-alarm-size-kb" );
  var.capAlarmThresholdKb_yeevla =
    arg ? (ub4)atol(arg) : YEEVLD_DEF_ALARM_SIZE;

  arg = ysResGetLast( "mnlogsrv.log-file-keep-count" );
  var.keepCount_yeevla = (ub4)arg ? atol(arg) : YEEVLD_DEF_KEEP_COUNT;

  if( !yeevlgFType(ysResGetLast( "mnlogsrv.log-file-limit-behaviour" ),
		   &var.fullAction_yeevla))
    var.fullAction_yeevla = YEEVLD_DEF_FULL_ACTION;

  ronly = ysResGetBool("mnlogsrv.log-file-readonly");
  var.admin_yeevla = ronly ?
    yeevl_locked_adminState : yeevl_unlocked_adminState;

  yseTry
  {
    if( !ronly )
      lg = yeevl_logProc_createLog( yeevlgLogProc(cx->lg_yeevldcx),
				   &cx->env_yeevldcx,
				   &fixed, &var );
    else
      lg = yeevl_logProc_openLog( yeevlgLogProc(cx->lg_yeevldcx),
				 &cx->env_yeevldcx,
				 &fixed, &var );
  }
  yseCatchObj( YEEVL_EX_OPERATIONFAILED, yeevl_OperationFailed, exv )
  {
    
    if( ronly )
      yslError( "yeevldStartLog: failed to open readonly %s: %s\n",
	       fixed.file_yeevld, exv.reason );
    else
      yslError( "yeevldStartLog: failed to create writable %s: %s\n",
	       fixed.file_yeevld, exv.reason );
    yoFree( (dvoid*)exv.reason );
    yseThrow( YEEVLD_EX_STARTUP );
  }
  yseEnd

  if( !ronly )
  {
    

    
    dl = yeev__get_filters( cx->ev_yeevldcx, &cx->env_yeevldcx );

    
    qlist =
      yeevDiscList__get_dlist( yeevlgDefaultDiscList(cx->lg_yeevldcx),
			      &cx->env_yeevldcx );

    
    for( i = 0; i < qlist._length; i++ )
    {
      yoRelease( (dvoid*)qlist._buffer[i].dest_yeevd );
      qlist._buffer[i].dest_yeevd = (yeevReceiver)yoDuplicate((dvoid*)lg);
    }

    
    ysRecord(YS_PRODUCT,YEEVLD_FAC, (ub4)321, YSLSEV_DEBUG(8), (char*)0,
	     YSLPTR(lg), YSLEND);

    
    yeevDiscList_append_i( dl, &cx->env_yeevldcx, &qlist );

    
    yoRelease( (dvoid*)dl );
    yeevdSeq__free( &qlist, yoFree );
  }
}






STATICF void yeevldAttach( yeevldcx *cx, char *attach_to )
{
  yort_proc yort;
  yeev pev;
  yeevDiscList dlobj;
  char *oname, *host, *pid, *affinity;
  yeevDiscList ddl;
  yeevdSeq dlist;
  ub4 i;
  ydim_imr imr;

  

  oname = attach_to;


  {
    
    host = attach_to;
    while( *attach_to && *attach_to != ':' )
      attach_to++;
    if( *attach_to )
      *attach_to++ = 0;
    pid = attach_to;
    while( *attach_to && *attach_to != ':' )
      attach_to++;
    if( *attach_to )
      *attach_to++ = 0;
    affinity = attach_to;
    if( !*affinity )
      affinity = (char*)0;

    

    yslError("Attaching to host %s, pid %s, affinity %s\n",
	     yeuStr(host), yeuStr(pid), yeuStr(affinity) );

    imr = (ydim_imr)yoBind(ydim_imr__id, (char*)0,
			   (yoRefData*)0, (char*)0 );
    yort = ydim_imr_yortOfProc( imr, &cx->env_yeevldcx,
			       host, pid, affinity);
    yoRelease( (dvoid*)imr );
  }

  if( !yort )
  {
    yslError("Can't locate %s for attach\n", oname );
    yseThrow( YEEVLD_EX_STARTUP );
  }

  
  
  pev = (yeev)yoYortBind( yeev__id, (char*)0, yort );
  yoRelease( yort );

  

  dlobj = yeev__get_filters( pev, &cx->env_yeevldcx );
  yoRelease( (dvoid*)pev );

  
  ddl = yeevlgDefaultDiscList( cx->lg_yeevldcx );
  dlist = yeevDiscList__get_dlist( ddl, &cx->env_yeevldcx );

  cx->rcv_yeevldcx =
    yeevl_logProc_createReceiver( yeevlgLogProc(cx->lg_yeevldcx),
				 &cx->env_yeevldcx, oname );
  yeevReceiverSetHandler( cx->rcv_yeevldcx, yeevldAttachHandler, (dvoid*)cx );

  
  cx->devt_yeevldcx =
    ysEvtCreate( yeevldAttachDeath, (dvoid*)cx, cx->q_yeevldcx, TRUE );
  yoWatchOwner( (dvoid*)dlobj, cx->devt_yeevldcx ); 

  
  for( i = 0; i < dlist._length ; i++ )
  {
    yoRelease( (dvoid*)dlist._buffer[i].dest_yeevd );
    dlist._buffer[i].dest_yeevd =
      (yeevReceiver)yoDuplicate((dvoid*)cx->rcv_yeevldcx);
  }

  yeevDiscList_replaceDest( dlobj, &cx->env_yeevldcx, &dlist );
  yoRelease( (dvoid*)dlobj );
  yeevdSeq__free( &dlist, yoFree );
}




STATICF void yeevldAttachHandler( yeevr *rec, dvoid *usrp )
{
  yeevldcx *cx = (yeevldcx *)usrp;

  yeevl_logProc_forward( yeevlgLogProc(cx->lg_yeevldcx),
			&cx->env_yeevldcx, rec);
}





STATICF void yeevldAttachDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			       size_t argsz )
{
  yeevldcx *cx = (yeevldcx*)usrp; 

  yslError("Attached program died, exiting\n");

  yoShutdown( cx->q_yeevldcx );
}


