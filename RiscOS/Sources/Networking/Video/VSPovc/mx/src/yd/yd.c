/* mx/src/yd/yd.c */


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
#ifndef YDRT_ORACLE
#include <ydrt.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDCH_ORACLE
#include <ydch.h>
#endif
#ifndef YDSP_ORACLE
#include <ydsp.h>
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
#ifndef YD_ORACLE
#include <yd.h>
#endif
#ifndef YDNM_ORACLE
#include <ydnm.h>
#endif

#ifndef MNR_ORACLE
#include <mnr.h>
#endif
#ifndef MNRS_ORACLE
#include <mnrs.h>
#endif
#ifndef YSLOG_ORACLE
#include <yslog.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif


#define ENTRY_POINT ydMain
#include <s0ysmain.c>



#define YD_FAC	    "YD"


static struct ysargmap ydmap[] =
{
  { 's', "mnorbsrv.spawner-only=true", 0 },
  { 0, (char *) 0, 0 }
};

externdef CONST_W_PTR mnrid ydRpcRid =
  { { 2, 222, 55, 224, 232, 169, 227, 223  }, MNVERS(1, 0) };

STATICF CONST_DATA mnrcd   ydRpcComp = { 0 };

STATICF CONST_DATA mnrcd *ydRpcCompList[] = { &ydRpcComp };


typedef struct
{
  ydcx	    *cx_yds;
  yoenv	    ev_yds;
  sb4	    req_yds;
  ydim_imr  imr_yds;
} yds;


#define YD_SYNC_WAIT_MS	((ub4)1000)



STATICF void ydServer(ydcx *cx, dvoid *osdp);
STATICF void ydServerTerm( ydcx *cx );
STATICF void ydThunk(ydcx *cx);

STATICF void ydSyncOrbds(ydcx *cx);
STATICF void ydSyncHandler( ydim_sync or, boolean decr, dvoid *usrp );

externdef ysidDecl(YD_SYNC_LOCK) = "yd::sync";
externdef ysidDecl(YD_EX_INTERNAL) = "yd::internal";
externdef ysidDecl(YD_EX_SYNC_ERR) = "yd::syncError";






boolean ydMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  ydcx	    cx;
  boolean   ok;
  char       vbuf[80];
  sword      sts;
  CONST char	*f;
  sword	    l;

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Daemon");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);

  if (!ok)
    return(FALSE);

  yslDetach();			

  CLRSTRUCT(cx);

  yseTry
  {
    ytInit();
    yoInit();
    ydServer( &cx, osdp );
  }
  yseCatch(YS_EX_INTERRUPT)
  {
  }
  yseCatch(YS_EX_BADMAGIC)
  {
    yseGetLoc( &f, &l );
    yslError("ydMain: %s caught %s from %s %d, panic!\n", 
	     ysProgName(), ysidToStr(yseExid), f, l);
    ysePanic( yseExid );
  }
  yseCatchAll
  {
    yseGetLoc( &f, &l );
    yslError("ydMain: %s caught %s from %s %d, exiting\n", 
	     ysProgName(), ysidToStr(yseExid), f, l);
  }
  yseEnd;

  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n", ysProgName(), yseExid );
  yseEnd;
  ysTerm(osdp);
  return TRUE;
}





STATICF void ydServer( ydcx *cx, dvoid *osdp )
{
  boolean serve;
  yslst *orbds;
  ub4 norbds;
  sysb8 timo;
  ysevt *tsem;
  mnrs *sh;
  sb4 mnsts;
  boolean   router;

  router = !ysResGetBool("mnorbsrv.spawner-only");

  if( router )
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1100, YSLSEV_INFO, (char*)0, YSLNONE );

#ifdef LATER
  else
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)11XX, YSLSEV_INFO, (char*)0, YSLNONE );
#endif
    
  yoEnvInit( &cx->env_ydcx );
  norbds = 0;

  
  cx->q_ydcx = (ysque*)yoQueCreate("yd-queue");

  
  if( router )
  {
    ydimInit( cx->q_ydcx, &cx->ydim_ydcx);
    ydThunk(cx);
    ydmtInit( cx->q_ydcx, &cx->ydmt_ydcx, cx->ydim_ydcx);
    ydThunk(cx);
  }
  ydspInit( cx->q_ydcx, &cx->ydsp_ydcx, cx->ydim_ydcx, osdp );
  ydThunk(cx);

  if( router ) 
  {
    ydimSetRefs( cx->ydim_ydcx, cx->ydmt_ydcx, cx->ydsp_ydcx );
    ydThunk(cx);
    cx->ydch_ydcx = ydchInit( cx->q_ydcx );
    ydThunk(cx);

    
    ydrtInit( cx->q_ydcx, &cx->ydrt_ydcx, cx->ydim_ydcx);
    ydThunk(cx);

    
    sysb8ext( &timo, YD_SYNC_WAIT_MS * 1000 );
    cx->sexid_ydcx = (ysid*)0;
    sh = mnrCreateServer( &ydRpcRid, (ub4)0, ydRpcCompList, (dvoid*)0 );
    
    

    
    while( TRUE )
    {
      
      orbds = yoListORBDs();
      norbds = ysLstCount( orbds );
      ysLstDestroy( orbds, (ysmff)yoRelease );

      if( norbds )
      {
	
	ysRecord(YS_PRODUCT, YD_FAC, (ub4)1101, YSLSEV_INFO, (char*)0,
		 YSLUB4(norbds), YSLEND );

	
	ysYield();		
	if( yoClaim(YD_SYNC_LOCK, (dvoid*)cx->ydim_ydcx) )
	{
	  
	  ysRecord(YS_PRODUCT,YD_FAC,(ub4)1102,YSLSEV_INFO, (char*)0,YSLNONE);

	  ydSyncOrbds(cx);

	  
	  ysRecord(YS_PRODUCT,YD_FAC,(ub4)1103, YSLSEV_INFO,(char*)0,YSLNONE);
	  break;
	}
      }
      else			
      {
	
	ysRecord(YS_PRODUCT, YD_FAC, (ub4)1104, YSLSEV_INFO,(char*)0,YSLNONE);
      
	
	if( !(mnsts = mnrRegister( sh, "mnorbsrvInitial", FALSE )) )
	{
	  
	  ysRecord(YS_PRODUCT,YD_FAC,(ub4)1105,YSLSEV_INFO,(char*)0,YSLNONE);
	  ydrtStartRouting( cx->ydrt_ydcx );
	  DISCARD mnrUnregister( sh );
	  break;
	}
      }
      

      tsem = ysSemCreate( (dvoid*)0 );
      ysTimer( &timo, tsem );
      ysSemWait( tsem );
      ysSemDestroy( tsem );
    }
    mnrDestroyServer( sh );

  } 
  
  serve = router || !cx->sexid_ydcx;

  if( serve && router )
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1106, YSLSEV_INFO, (char*)0,YSLNONE);
  else if ( serve )
  {
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1106, YSLSEV_INFO, (char*)0,YSLNONE);
  }
  else
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1107, YSLSEV_ALERT, (char*)0,YSLNONE);
    
  
  if( serve )
  {
    if( router )
    {
      
      if( norbds )
	yoAbandon( YD_SYNC_LOCK );
    }
    cx->server_ydcx = TRUE;

    yseTry
    {
      yoService( cx->q_ydcx );
    }
    yseCatch(YS_EX_INTERRUPT)
    {
      
      ysRecord(YS_PRODUCT, YD_FAC, (ub4)1112, YSLSEV_ERR, (char*)0,YSLNONE);
      yslError("ydService: %s interrupted, exiting.\n", ysProgName() );
    }
    yseEnd;

    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1108, YSLSEV_INFO, (char*)0,YSLNONE);
  }
  else if( !router && norbds )
    yoAbandon( YD_SYNC_LOCK );

  
  ydServerTerm( cx );

  
  ysRecord(YS_PRODUCT, YD_FAC, (ub4)1109, YSLSEV_INFO, (char*)0,YSLNONE);
}

STATICF void ydServerTerm( ydcx *cx )
{
  boolean   router;

  router = !ysResGetBool("mnorbsrv.spawner-only");

  if( router && !cx->sexid_ydcx )
    yoTermRouter();

  yoEnvFree( &cx->env_ydcx );

  
  ysRecord(YS_PRODUCT, YD_FAC, (ub4)1110, YSLSEV_INFO, (char*)0,YSLNONE);

  if( cx->ydch_ydcx ) ydchTerm( cx->ydch_ydcx );
  if( cx->ydsp_ydcx ) ydspTerm( cx->ydsp_ydcx, TRUE );
  if( cx->ydrt_ydcx ) ydrtTerm( cx->ydrt_ydcx );
  if( cx->ydmt_ydcx ) ydmtTerm( cx->ydmt_ydcx );
  if( cx->ydim_ydcx ) ydimTerm( cx->ydim_ydcx );

  yoQueDestroy( cx->q_ydcx );

  if( cx->ydch_ydcx ) yoRelease( (dvoid*)cx->ydch_ydcx );
  if( cx->ydsp_ydcx ) yoRelease( (dvoid*)cx->ydsp_ydcx );
  if( cx->ydrt_ydcx ) yoRelease( (dvoid*)cx->ydrt_ydcx );
  if( cx->ydmt_ydcx ) yoRelease( (dvoid*)cx->ydmt_ydcx );
  if( cx->ydim_ydcx ) yoRelease( (dvoid*)cx->ydim_ydcx );
}





STATICF void ydThunk(ydcx *cx)
{
  ysYield();
  ysSvcAll(cx->q_ydcx);
  ysYield();
  ysSvcAll(cx->q_ydcx);
  ysYield();
  ysSvcAll(cx->q_ydcx);
  ysYield();
  ysSvcAll(cx->q_ydcx);
}



STATICF void ydSyncOrbds(ydcx *cx)
{
  yslst	*orbds;
  ysle	*e;
  ydim_imr imr;
  sb4	cnt;
  ydim_sync sync;
  ysevt *sem;
  char *rs;

  

  orbds = yoListORBDs();
  
  if( (cnt = (sb4)ysLstCount( orbds )))
  {
    sync = ydim_imr_createSync( cx->ydim_ydcx, &cx->env_ydcx, cnt );
    ydimSetSyncHandler( sync, ydSyncHandler, (dvoid*)cx );

    for( e = ysLstHead( orbds ); e ; e = ysLstNext( e ) )
    {
      imr = (ydim_imr)ysLstVal(e);
      ydim_imr_startSync( imr, &cx->env_ydcx, cx->ydim_ydcx, sync );

      rs = yoRefToStr( (dvoid*) imr );
      
      ysRecord(YS_PRODUCT, YD_FAC, (ub4)1113, YSLSEV_INFO, (char*)0,
	       YSLSTR(rs), YSLEND);
      yoFree( (dvoid*)rs );
    }

    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1114, YSLSEV_INFO, (char*)0,
	     YSLUB4(cnt), YSLEND);

    

    sem = cx->ssem_ydcx = ysSemCreate( (dvoid*)0 );
    ysSvcLoop( cx->q_ydcx, cx->ssem_ydcx );
    cx->ssem_ydcx = (ysevt*)0;
    ysSemDestroy( sem );
  }
  ysLstDestroy( orbds, (ysmff)yoRelease );
}





STATICF void ydSyncHandler( ydim_sync or, boolean decr, dvoid *usrp )
{
  ydcx	*cx = (ydcx*)usrp;

  if( decr )
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1115, YSLSEV_INFO, (char*)0, YSLNONE ); 
  else
    
    ysRecord(YS_PRODUCT, YD_FAC, (ub4)1116, YSLSEV_ERR, (char*)0, YSLNONE );
    
  cx->sexid_ydcx = decr ? (ysid*)0 : (ysid*)YD_EX_SYNC_ERR;

  
  ydimSetSyncHandler( or, (ydimSyncFunc)0, (dvoid*)cx );

  if( decr )
  {
    ydim_sync_destroy( or, &cx->env_ydcx );

    
    ydrtStartRouting( cx->ydrt_ydcx );
  }

  
  ysTrigger( cx->ssem_ydcx, cx->sexid_ydcx, (dvoid*)0, (size_t)0 );
}



