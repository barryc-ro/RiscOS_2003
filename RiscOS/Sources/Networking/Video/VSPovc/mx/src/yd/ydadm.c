/* mx/src/yd/ydadm.c */


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
#ifndef YDYOIDL_ORACLE
#include <ydyoidl.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
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

#define ENTRY_POINT ydadmMain
#include <s0ysmain.c>



#define YDADM_FAC   "YDUT"


static struct ysargmap ydadmmap[] =
{
  { 'k', "mnorbadm.kill-process=TRUE", 0 },
  { 'D', "mnorbadm.dispatcher-ref", 1 },
  { 'i', "mnorbadm.interface-name", 1 },
  { 'I', "mnorbadm.implementation-name", 1 },
  { 'l', "mnorbadm.do-listing=TRUE", 0 },
  { 'n', "mnorbadm.limit-to-proc-name", 1 },
  { 'p', "mnorbadm.proc-host-pid-affinity", 1 },
  { 'q', "mnorbadm.queue-name", 1 },
  { 'Q', "mnorbadm.queue-ref", 1 },
  { 's', "mnorbadm.new-admin-state", 1 },
  { 'Y', "mnorbadm.yort-ref", 1 },
  { 0, (char *) 0, 0 }
};

# define YDADM_CHUNK  100

externdef ysidDecl(YDADM_EX_UNKNOWN_SVCSTATE) = "::ydadm:unknownSvcState";
externdef ysidDecl(YDADM_EX_UNKNOWN_PROCSTATE) = "::ydadm:unknownProcState";
externdef ysidDecl(YDADM_EX_UNKNOWN_QUEUESTATE) = "::ydadm:unknownQueueState";
externdef ysidDecl(YDADM_EX_UNKNOWN_INTF) = "::ydadm:unknownInterface";



STATICF void ydadmImpl( yort_proc y, yoenv *ev, char *impl, char *intf,
		       char *newstate );

STATICF void ydadmQueue( yoenv *ev, yort_queue q, char *newstate);

STATICF void ydadmDispatcher( yoenv *ev, yort_dispatcher d, char *newstate );

STATICF void ydadmAllDispatchers( yort_proc y, yoenv *ev, char *newstate );

STATICF void ydadmProc( yort_proc y, yoenv *ev, char *newstate );

STATICF yort_queue ydadmQLookup( yort_proc y, yoenv *ev, char *name );

STATICF void ydadmSplit( char *str, char **fields, sword nf );

STATICF yort_svcState ydadmSvcState( CONST char *name );

STATICF yort_procState ydadmProcState( CONST char *name );






boolean ydadmMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  sword     sts;
  boolean   ok;
  char      vbuf[80];
  ydim_imr  imr;
  yslst	    *ylst;
  char	    *newstate;
  char	    *impl, *intf;
  char	    *qntxt, *qrtxt, *drtxt;
  noreg boolean   dispall;
  boolean   more, killproc;
  char	    *ptxt, *pntxt;
  char	    *ytxt;
  sword	    tcnt;
  noreg sword	    pcnt;
  yort_proc noreg y;
  ydim_yortIterator yi;
  ydim_yortList	yortList;
  yoenv	    ev;
  char	    *words[3];
  ub4	    i;
  yort_queue	q;
  yort_procInfo pinfo;

  NOREG(y);
  NOREG(dispall);
  NOREG(pcnt);

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydadmmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Daemon Admin Utility");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  
  
  if( !(newstate = ysResGetLast("mnorbadm.new-admin-state")) &&
     !ysResGetBool("mnorbadm.do-listing"))
  {
    
    ysRecord(YS_PRODUCT, YDADM_FAC, (ub4)1510, YSLSEV_ERR, (char*)0, YSLNONE);
    return( FALSE );
  }

  tcnt = 0;
  dispall = TRUE;
  if(( killproc = ysResGetBool("mnorbadm.kill-process"))) tcnt++;
  if(( qntxt = ysResGetLast("mnorbadm.queue-name"))) tcnt++;
  if(( qrtxt = ysResGetLast("mnorbadm.queue-ref"))) tcnt++;
  if(( drtxt = ysResGetLast("mnorbadm.dispatcher-ref" ))) tcnt++;
  if(( intf = ysResGetLast( "mnorbadm.interface-name" ))) tcnt++;

  
  if( killproc || qntxt || qrtxt || drtxt || intf )
    dispall = FALSE;

  impl = ysResGetLast( "mnorbadm.implementation-name" );

  if( tcnt > 1 )
  {
    
    ysRecord(YS_PRODUCT, YDADM_FAC, (ub4)1511, YSLSEV_ERR, (char*)0, YSLNONE);
    return( FALSE );
  }

  pntxt = ysResGetLast("mnorbadm.limit-to-proc-name" );

  pcnt = 0;
  if(( ptxt = ysResGetLast("mnorbadm.proc-host-pid-affinity" ))) pcnt++;
  if(( ytxt = ysResGetLast( "mnorbadm.yort-ref" ))) pcnt++;
  if( pcnt > 1 )
  {
    
    ysRecord(YS_PRODUCT, YDADM_FAC, (ub4)1512, YSLSEV_ERR, (char*)0, YSLNONE);
    return FALSE;
  }

  yseTry
  {
    ytInit();
    yoInit();

    imr = (ydim_imr)yoBind(ydim_imr__id, (char *)0, (yoRefData*)0,(char*)0);
    yoEnvInit( &ev );

    

    
    ylst = ysLstCreate();

    

    if( pcnt )
    {
      ydadmSplit( ptxt, words, 3 );
      if( ytxt )		
	DISCARD ysLstEnq( ylst, yoStrToRef( ytxt ) );
      else if( (y = ydim_imr_yortOfProc(imr,&ev,words[0],words[1],words[2])))
	DISCARD ysLstEnq( ylst, (dvoid*)y );
      else
	
	ysRecord(YS_PRODUCT, YDADM_FAC, (ub4)1512, YSLSEV_ERR, (char*)0,
		 YSLSTR( ptxt ), YSLEND );
    }
    else			
    {
      ydim_imr_listYort(imr, &ev,(sb4)YDADM_CHUNK, (yort_proc)0,
			&yortList, &yi);
      for( more = FALSE; yortList._length; )
      {
	for( i = 0 ; i < yortList._length ; i++ )
	{
	  y = yortList._buffer[i];

	  if( !pntxt )		
	  {
	    DISCARD ysLstEnq( ylst, yoDuplicate( (dvoid*)y ) );
	  }
	  else			
	  {
	    yseTry		
	    {
	      pinfo = yort_proc__get_pInfo( y, &ev );
	      if( !strcmp( pntxt, pinfo.name_yort_procInfo ) )
		DISCARD ysLstEnq( ylst, yoDuplicate( (dvoid*)y ) );
	      yort_procInfo__free( &pinfo, yoFree );
	    }
	    yseCatch( YS_EX_INTERRUPT )
	      yseRethrow;
	    yseCatchAll;	
	    yseEnd;
	  }
	}

	ydim_yortList__free( &yortList, yoFree );
	yortList._length = 0;
	if( yi || more )
	  more = ydim_yortIterator_next_n(yi,&ev,(sb4)YDADM_CHUNK,&yortList);
      }
      if( yi )
	ydim_yortIterator_destroy( yi, &ev );
    }

    

    while( (y = (yort_proc)ysLstDeq( ylst ) ) )
    {
      yseTry
      {
	if( intf )
	{
	  ydadmImpl( y, &ev, impl, intf, newstate );
	}
	else if( qntxt || qrtxt )
	{
	  q = qrtxt ? (yort_queue)yoStrToRef(qrtxt) : ydadmQLookup(y,&ev,qntxt);
	  ydadmQueue( &ev, q, newstate );
	}
	else if( dispall )
	{
	  ydadmAllDispatchers( y, &ev, newstate ); 
	}
	else if( drtxt )
	{
	  ydadmDispatcher( &ev, (yort_dispatcher)yoStrToRef(drtxt), newstate);
	}
	else 
	{
	  ydadmProc( y, &ev, newstate );
	}
      }
      yseCatch( YS_EX_INTERRUPT )
	yseRethrow;
      yseCatchAll
	yslError("%s caught exception %s, continuing\n",
		 ysProgName(), yseExid );
      yseEnd;
    }
    ysLstDestroy( ylst, (ysmff)0 );
  }
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
    yslPrint("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;

  yseTry
  {
    yoEnvFree( &ev );
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n", ysProgName(), yseExid );
  yseEnd;

  ysTerm(osdp);
  return TRUE;
}



STATICF void ydadmImpl( yort_proc y, yoenv *ev, char *impl, char *intf,
		       char *newstate )
{
  yort_impl yimpl;
  yort_implInfo *iinfo = (yort_implInfo*)0;
  yort_procInfo pinfo;
  yort_dispInfoList dlist;
  yort_implAllList ilist;
  ub4	i;

  yort_proc_getImplAll( y, ev, (yort_implFlags)0, &pinfo, &dlist, &ilist );
  if( !impl )
    impl = pinfo.name_yort_procInfo;

  yimpl = (yort_impl)0;
  for( i = 0 ; i < ilist._length ; i++ )
  {
    iinfo = &ilist._buffer[i].info_yort_implAll;
    
    if( !strcmp( intf, iinfo->intf_yort_implInfo ) &&
       !yduSafeStrcmp( impl, iinfo->impl_yort_implInfo ) )
    {
      yimpl = iinfo->self_yort_implInfo;
      break;
    }
  }

  if( !yimpl )
    yseThrow( YDADM_EX_UNKNOWN_INTF );

  if( newstate )		
  {
    yort_impl_setAdminState( yimpl, ev, ydadmSvcState( newstate ) );
  }
  else				
  {
    yslPrint( "\nIntf/Impl %s/%s\n", intf, impl );
    yslPrint( "upTimeCs: %10d\n", iinfo->upTimeCs_yort_implInfo ); 
    yslPrint( "%-12s %-14s        Changed at: %10d\n", "operStatus:",
  	     ydqSvcState(iinfo->operStatus_yort_implInfo),
	     iinfo->operStatusChanged_yort_implInfo);
    yslPrint( "%-12s %-14s        Changed at: %10d\n", "adminStatus:",
	     ydqSvcState(iinfo->adminStatus_yort_implInfo),
	     iinfo->adminStatusChanged_yort_implInfo);
    yslPrint( "%-12s %8d  tot: %8d  last at: %10d  rej:    %8d\n", "inbound:",
	     iinfo->inboundAssocs_yort_implInfo,
	     iinfo->totalInboundAssocs_yort_implInfo,
	     iinfo->lastInbound_yort_implInfo,
	     iinfo->rejectedInbounds_yort_implInfo );
    yslPrint( "%-12s %8d  tot: %8d  last at: %10d  failed: %8d\n", "outbound:",
	     iinfo->outboundAssocs_yort_implInfo,
	     iinfo->totalOutboundAssocs_yort_implInfo,
	     iinfo->lastOutbound_yort_implInfo,
	     iinfo->failedOutbounds_yort_implInfo );
    yslPrint("%-12s %s%s%s\n", "flags:",
	     (iinfo->implFlags_yort_implInfo & yort_stateless_implFlag) ?
	     "Stateless " : "",
	     (iinfo->implFlags_yort_implInfo & yort_psf_implFlag) ?
	     "PSF " : "",
	     (iinfo->implFlags_yort_implInfo & yort_suspended_implFlag) ?
	     "Suspended " : "" );
  }

  yort_procInfo__free( &pinfo, yoFree );
  yort_dispInfoList__free( &dlist, yoFree );
  yort_implAllList__free( &ilist, yoFree );
}


STATICF void ydadmQueue(yoenv *ev, yort_queue q, char *newstate)
{
  yort_queueInfo qinfo;

  if( newstate )
  {
    if( !strcmp(newstate, "down"))
      yort_queue_shutdownDispatchers( q, ev );
    else
      yseThrow( YDADM_EX_UNKNOWN_QUEUESTATE );
  }
  else
  {
    qinfo = yort_queue__get_info( q, ev );
    ydqQInfoHdr();
    ydqQInfo( &qinfo );
    yort_queueInfo__free( &qinfo, yoFree );
  }
}

STATICF void ydadmAllDispatchers( yort_proc y, yoenv *ev, char *newstate )
{
  yort_dispatcherList	dolist;
  ub4 i;

  dolist = yort_proc__get_dispatchers( y, ev );
  for( i = 0; i < dolist._length ; i++ )
  {
    yseTry
      ydadmDispatcher(ev, dolist._buffer[i], newstate );
    yseCatch( YS_EX_INTERRUPT )
      yseRethrow;
    yseCatchAll;
    yseEnd;
  }

  yort_dispatcherList__free( &dolist, yoFree );
}

STATICF void ydadmDispatcher( yoenv *ev, yort_dispatcher d, char *newstate )
{
  yort_dispInfo dinfo;
  yort_queueInfo qi;

  if( newstate )
  {
    yort_dispatcher_setAdminState( d, ev, ydadmSvcState( newstate ));
  }
  else
  {
    ydqDispInfoHdr( (ub4)1 );
    dinfo = yort_dispatcher__get_dinfo( d, ev );
    qi = yort_queue__get_info( dinfo.queue_yort_dispInfo, ev );
    ydqDispInfo( &dinfo, qi.name_yort_queueInfo );
    yort_queueInfo__free( &qi, yoFree );
    yort_dispInfo__free( &dinfo, yoFree );
  }
}


STATICF void ydadmProc( yort_proc y, yoenv *ev, char *newstate )
{
  yort_procInfo pinfo;
  yort_dispInfoList dlist;
  yort_implAllList ilist;
  yort_queueList qlist;
  ysevt *evt;

  if( newstate )
  {
    evt = ysSemCreate((dvoid*)0);
    yort_proc_setAdminState_nw( y, ev, ydadmProcState( newstate ), evt );
    ysEvtDestroy( evt );
  }
  else
  {
    yort_proc_getImplAll( y, ev, (yort_implFlags)0, &pinfo, &dlist, &ilist );

    yslPrint("Proc %s:%s:%s %s %dk %dCs  opSt: %s  adSt: %s\n\n", 
	     pinfo.host_yort_procInfo,
	     pinfo.pid_yort_procInfo,
	     yduStr(pinfo.affinity_yort_procInfo),
	     pinfo.name_yort_procInfo,
	     pinfo.memKb_yort_procInfo,
	     pinfo.cpuCs_yort_procInfo,
	     ydqProcState(pinfo.operState_yort_procInfo),
	     ydqProcState(pinfo.adminState_yort_procInfo));
    qlist = yort_proc__get_queues( y, ev );
    ydqShowYortQueueList( &qlist );
    yslPrint("\n");
    ydqShowDispInfoList( &dlist );
    yslPrint("\n");
    ydqShowImplAllList( &ilist, TRUE );

    yort_procInfo__free( &pinfo, yoFree );
    yort_dispInfoList__free( &dlist, yoFree );
    yort_implAllList__free( &ilist, yoFree );
    yort_queueList__free( &qlist, yoFree );
  }
}


STATICF yort_queue ydadmQLookup( yort_proc y, yoenv *ev, char *name )
{
  yort_queueList qlist;
  ub4 i;
  yort_queueInfo    qi;
  yort_queue	    q;

  qlist = yort_proc__get_queues( y, ev );
  for( q = (yort_queue)0, i = 0; !q && i < qlist._length ; i++ )
  {
    qi = yort_queue__get_info( qlist._buffer[i], ev );
    if( !yduSafeStrcmp( qi.name_yort_queueInfo, name ) )
      q = (yort_queue)yoDuplicate( (dvoid*)qlist._buffer[i] );
    yort_queueInfo__free( &qi, yoFree);
  }
  yort_queueList__free( &qlist, yoFree );
  return( (yort_queue)q );
}

STATICF void ydadmSplit( char *str, char **fields, sword nf )
{
  while( *str && nf-- )
  {
    *fields = str;
    while(*str && *str != ':' )
    {
      if( *str && *str++ == '=' )
      {
	*fields = str;
	if( !nf )
	  break;
      }
    }
    if( *str && *str == ':' )
      *str++ = 0;
    fields++;
  }
  while( nf-- )
    *fields++ = (char*)0;
}


STATICF yort_svcState ydadmSvcState( CONST char *name )
{
  yort_svcState rv = 0;
  if( !strcmp( name, "down" ) )
    rv = yort_SvcStDown;
  else if( !strcmp( name, "run" ) )
    rv = yort_SvcStRun;
  else if( !strcmp( name, "halt" ) )
    rv = yort_SvcStHalted;
  else if( !strcmp( name, "congested" ) )
    rv = yort_SvcStCongested;
  else if( !strcmp( name, "restart" ) )
    rv = yort_SvcStRestart;
  else if( !strcmp( name, "quiesce" ) )
    rv = yort_SvcStQuiesce;
  else
    yseThrow( YDADM_EX_UNKNOWN_SVCSTATE ); 
  return( rv );
}


STATICF yort_procState ydadmProcState( CONST char *name )
{
  yort_procState rv = 0;
  if( !strcmp( name, "running" ) )
    rv = yort_ProcStRunning;
  else if( !strcmp( name, "runnable" ) )
    rv = yort_ProcStRunnable;
  else if( !strcmp( name, "notrunnable" ) )
    rv = yort_ProcStNotRunnable;
  else if( !strcmp( name, "invalid" ) )
    rv = yort_ProcStInvalid;
  else
    yseThrow( YDADM_EX_UNKNOWN_PROCSTATE ); 
  return( rv );
}

