/* mx/src/yd/ydrt.c */


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
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YDRT_ORACLE
#include <ydrt.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDMT_ORACLE
#include <ydmt.h>
#endif
#ifndef YDSP_ORACLE
#include <ydsp.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif
#ifndef YDCH_ORACLE
#include <ydch.h>
#endif
#ifndef YDIDLI_ORACLE
#include <ydidlI.h>
#endif
#ifndef YDYOIDL_ORACLE
#include <ydyoidl.h>
#endif



#define YDRT_FAC    "YDRT"


#define YDRT_CHUNK_SIZE	    ((sb4)1000)


#define YDRT_OFF_PROC_COST  ((sb4)20)

#define YDRT_SPAWNING_COST  ((sb4)100)

#define YDRT_SPAWN_COST	    ((sb4)10*1000)


#define YDRT_LAUNCH_TIMEOUT_MS ((sb4)60*1000)

typedef	sb4 ydrtCost;		

typedef struct
{
  ydrt_router	self_ydrtcx;
  ysque		*q_ydrtcx;	
  yoenv		ev_ydrtcx;	

  yort_proc	yort_ydrtcx;	
  yort_queueInfo qinfo_ydrtcx;
  sb4		tottms_ydrtcx;	
  sb4		totl_ydrtcx;	

  sb4		lockTimeMs_ydrtcx;

  ydim_imr	imr_ydrtcx;	
  ydmt_imtr	ydmt_ydrtcx;	
  ydsp_spawner	ydsp_ydrtcx;	
  ydch_och	ydch_ydrtcx;	
  sb4		chunksize_ydrtcx; 
  char		*host_ydrtcx;	
  sysb8		ltimeout_ydrtcx; 

  yslst		*rtelst_ydrtcx;	

} ydrtcx;




typedef struct
{
  ydrtCost	cost_ydrte;	
  ydim_implInfo	*info_ydrte;	
  ydim_active	active_ydrte;	
  ydimain	*ain_ydrte;	
  ydyo_activeInfo *ainfo_ydrte;	
} ydrte;


typedef struct
{
  enum {
    start_ydrtax,
    lockWait_ydrtax,
    done_ydrtax,
    exit_ydrtax
  }		state_ydrtax;
  yore		*req_ydrtax;	
  ysevt		*evt_ydrtax;
  yoenv		ev_ydrtax;

} ydrtax;




STATICF void ydrtRoute( ydrtcx *cx, yore *r );

STATICF void ydrtHandler( dvoid *usrp, CONST ysid *exid,
			 dvoid *arg, size_t argsz);


STATICF void ydrtForwardHandler(dvoid *usrp, CONST ysid *exid,
				dvoid *arg, size_t argsz);


STATICF yort_proc ydrtGoodSpot( ydrtcx *cx, yore *r, boolean *ready );


STATICF void ydrtActivateForward( yore *r );


STATICF void ydrtActivation( dvoid *usrp, CONST ysid *exid,
			    dvoid *arg, size_t argsz );

STATICF void ydrtCopyRte( ydrte *dest, ydrte *src );





#define ydrtPersistant( r ) (FALSE)


#define ydrtPlaceOf( r ) (r->hdr.ref)


#define ydrtTightlyBound( r ) (r->hdr.ref ? TRUE : FALSE )


#define ydrtLooselyBound( r ) ( r->hdr.ref ? FALSE : TRUE )

STATICF ydrtCost ydrtSimpleCost( ydrtcx *cx, ydimain *ain );
STATICF ydrtCost ydrtMetricCost( ydrtcx *cx, ydimain *ain, yore *r );

STATICF void ydrtLaunchForward( dvoid *usrp, yslst *reqs,
			       yslst *active, yslst *timedout );

static CONST_W_PTR struct ydrt_router__tyimpl ydrt_router__impl =
 {
  ydrt_router__get_self_i,
  ydrt_router__get_imr_i,
  ydrt_router__get_qinfo_i,
 };

externdef ysidDecl(YDRT_EX_ACTIVATE_FAILED) = "ydrt::activateFailed";
externdef ysidDecl(YDRT_EX_INTERNAL) = "ydrt::internal";

externdef ysmtagDecl(ydrtax_tag) = "ydrtax_tag";



void ydrtInit( ysque *q, ydrt_router *oydrt, ydim_imr imr )
{
  ydrtcx *cx;
  char	*arg;
  sb4	val;

  cx = (ydrtcx *)ysmGlbAlloc( sizeof(*cx), "ydrtcx" );
  CLRSTRUCT( *cx );
  yoEnvInit(&cx->ev_ydrtcx);
  cx->yort_ydrtcx = (yort_proc)yoYort();

  cx->q_ydrtcx = q;
  cx->imr_ydrtcx = imr;
  cx->lockTimeMs_ydrtcx = 0;	
  cx->qinfo_ydrtcx.self_yort_queueInfo = (yort_queue)0;
  cx->qinfo_ydrtcx.name_yort_queueInfo = ysStrDupWaf("ydrt queue", (ysmaf)0);

  arg = ysResGetLast("ydrt.chunksize");
  cx->chunksize_ydrtcx = arg ? atoi(arg) : YDRT_CHUNK_SIZE;
  if( cx->chunksize_ydrtcx < 1 ) cx->chunksize_ydrtcx = 1;

  cx->host_ydrtcx = (char*)ysResGetLast("ys.hostname");

  arg = ysResGetLast("ydrt.launch-timeout-ms");
  val = arg ? atoi(arg) : YDRT_LAUNCH_TIMEOUT_MS;
  sysb8ext( &cx->ltimeout_ydrtcx, val * 1000 );

  
  yoSetImpl( ydrt_router__id, (char*)0, ydrt_router__stubs,
	    (dvoid*)&ydrt_router__impl, (yoload)0, TRUE, (dvoid*)cx );

  
  cx->self_ydrtcx =
    (ydrt_router)yoCreate( ydrt_router__id, (char*)0,
			  (yoRefData*)0,(char *)0, (dvoid *)cx);

  cx->rtelst_ydrtcx = ysLstCreate();

  
  cx->ydmt_ydrtcx = (ydmt_imtr)yoBind( ydmt_imtr__id, (char*)0,
				      (yoRefData*)0, (char*)0 );
  cx->ydsp_ydrtcx = (ydsp_spawner)yoBind( ydsp_spawner__id, (char*)0,
					 (yoRefData*)0, (char*)0 );
  cx->ydch_ydrtcx = (ydch_och)yoBind( ydch_och__id, (char*)0,
				     (yoRefData*)0, (char*)0 );

  *oydrt = (ydrt_router)yoDuplicate((dvoid*)cx->self_ydrtcx);

  yoImplReady( ydrt_router__id, (char*)0, cx->q_ydrtcx );

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1150, YSLSEV_DEBUG(0), (char*)0, YSLNONE);
}

void ydrtStartRouting( ydrt_router ydrt )
{
  ydrtcx    *cx = (ydrtcx*)yoGetImplState((dvoid *)ydrt);

  yoInitRouter( (dvoid*)cx->imr_ydrtcx, cx->q_ydrtcx,
	       ydrtHandler, (dvoid *)cx );
}

void ydrtTerm( ydrt_router rt )
{
  ydrtcx    *cx = (ydrtcx*)yoGetImplState((dvoid *)rt);

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1151, YSLSEV_DEBUG(0), (char*)0, YSLNONE);

  yoImplDeactivate( ydrt_router__id, (char*)0 );

  if( cx->qinfo_ydrtcx.name_yort_queueInfo )
    ysmGlbFree( (dvoid*)cx->qinfo_ydrtcx.name_yort_queueInfo );

  
  ysLstDestroy( cx->rtelst_ydrtcx, ysmFGlbFree );

  yoRelease( (dvoid*)cx->ydmt_ydrtcx );
  yoRelease( (dvoid*)cx->ydsp_ydrtcx );
  yoRelease( (dvoid*)cx->ydch_ydrtcx );

  yoRelease( (dvoid*)cx->self_ydrtcx );
  yoDispose( (dvoid*)cx->self_ydrtcx );

  yoRelease( (dvoid*)cx->yort_ydrtcx );
  yoEnvFree( &cx->ev_ydrtcx );
  ysmGlbFree( (dvoid*)cx );
}








ydrt_router ydrt_router__get_self_i( ydrt_router or, yoenv *ev)
{
  ydrtcx *cx = (ydrtcx*)yoGetImplState( (dvoid*)or );
  return( (ydrt_router)yoDuplicate((dvoid*)cx->self_ydrtcx) );
}


ydim_imr ydrt_router__get_imr_i( ydrt_router or, yoenv *ev)
{
  ydrtcx *cx = (ydrtcx*)yoGetImplState( (dvoid*)or );
  return( (ydim_imr)yoDuplicate((dvoid*)cx->imr_ydrtcx) );
}



yort_queueInfo ydrt_router__get_qinfo_i( ydrt_router or, yoenv *ev)
{
  ydrtcx *cx = (ydrtcx*)yoGetImplState( (dvoid*)or );
  yort_queueInfo qi;

  
  if( cx->qinfo_ydrtcx.totEnq_yort_queueInfo )
  {
    cx->qinfo_ydrtcx.avgLen_yort_queueInfo =
      cx->totl_ydrtcx / cx->qinfo_ydrtcx.totEnq_yort_queueInfo;

    cx->qinfo_ydrtcx.avgDelayMs_yort_queueInfo = (yort_timeMs)
      (cx->tottms_ydrtcx / cx->qinfo_ydrtcx.totEnq_yort_queueInfo + 5) / 10;
  }
  yort_queueInfo__copy( &qi, &cx->qinfo_ydrtcx, yoAlloc);
  return( qi );
}










STATICF void ydrtHandler( dvoid *usrp, CONST ysid *exid,
			 dvoid *arg, size_t argsz)
{
  yore	*r = (yore*)arg;
  yore	*fr = (yore*)ysmGlbAlloc( sizeof(*fr), "yore");

  DISCARD memcpy( (dvoid*)fr, (dvoid*)r, sizeof(*fr));
  ydrtRoute( (ydrtcx*)usrp, fr );
}

STATICF void ydrtRoute( ydrtcx *cx, yore *r )
{
  yort_proc	place = (yort_proc)0;
  boolean ready = TRUE;
  boolean activate = FALSE;

  sysb8	start, mid, finish;
  ub4	qlen;
  sb4   tim;

  
  ysClock( &start );
  qlen = ysSvcPending( cx->q_ydrtcx );
  cx->qinfo_ydrtcx.curLen_yort_queueInfo = (yort_gauge)qlen;
  cx->totl_ydrtcx += qlen;
  if( qlen > cx->qinfo_ydrtcx.maxLen_yort_queueInfo )
    cx->qinfo_ydrtcx.maxLen_yort_queueInfo = (yort_gauge)qlen;

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1152, YSLSEV_INFO, (char*)0,
	   YSLSTR(yduIdStr(r->hdr.ref->intf)),
	   YSLSTR(yduIdStr(r->hdr.ref->impl)),
	   YSLSTR(yduStr(r->hdr.op)),
	   YSLSB4( qlen ), YSLEND);

  if( !ydrtPersistant( r ) )	
  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1153, YSLSEV_DEBUG(3),(char*)0,YSLNONE);

    place = ydrtGoodSpot( cx, r, &ready );
  }
  else				

  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1154, YSLSEV_DEBUG(3),(char*)0,YSLNONE);

    if( ydrtTightlyBound( r ) )
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1155, YSLSEV_EMERG,(char*)0,YSLNONE);
      ysePanic(YDRT_EX_INTERNAL);
    }

    ydch_och_whereActive( cx->ydch_ydrtcx, &cx->ev_ydrtcx,
			 (CORBA_Object)r->hdr.ref, (yort_proc*)&place );
    if( !place )		
    {
      place = ydrtGoodSpot( cx, r, &ready );
      activate = TRUE;
    }
  }

  ysClock(&mid);

  if( place )			
  {
    if( ready )
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1156, YSLSEV_INFO,(char*)0,YSLNONE);

      r->dstref = yoDuplicate((dvoid*)place);
      r->usrp = (dvoid*)cx;
      if( activate )
	ydrtActivateForward( r );
      else
	yoFwdRequest( r, ysEvtSimple(ydrtForwardHandler, (dvoid*)r) );
    }
    else
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1157, YSLSEV_INFO,(char*)0,YSLNONE);
    }
  }
  else				
  {
    
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1158, YSLSEV_INFO,(char*)0,
	       YSLSTR(yduIdStr(r->hdr.ref->intf)),
	       YSLSTR(yduIdStr(r->hdr.ref->impl)),
	       YSLSTR(yduStr(r->hdr.op)), YSLEND);
    yoRejRequest( r );
    yoFreeRouteReq( r );
    ysmGlbFree( (dvoid*)r );
  }

  
  ysClock( &finish );
  sysb8sub( &finish, &finish, &start );
  sysb8sub( &mid, &mid, &start );
  tim = (sysb8msk(&finish) + 500) / 1000;
  cx->tottms_ydrtcx += tim;
  tim /= 10;
  cx->qinfo_ydrtcx.curDelayMs_yort_queueInfo = tim;
  cx->qinfo_ydrtcx.totEnq_yort_queueInfo++;
  if( tim > cx->qinfo_ydrtcx.maxDelayMs_yort_queueInfo )
    cx->qinfo_ydrtcx.maxDelayMs_yort_queueInfo = tim;
}





STATICF void ydrtForwardHandler(dvoid *usrp, CONST ysid *exid, dvoid *arg,
				size_t argsz)
{
  yore	    *r = (yore*)usrp;
  ydrtcx    *cx = (ydrtcx*)r->usrp;
  char	    *refstr = (char*)0;
  yoenv	    ev;			

  refstr = yoRefToStr( r->dstref );
  if(!exid)
  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1159, YSLSEV_INFO,(char*)0,
	     YSLSTR(yduIdStr(r->hdr.ref->intf)),
	     YSLSTR(yduIdStr(r->hdr.ref->impl)),
	     YSLSTR(refstr), YSLEND);
    yoFreeRouteReq( r );
    yoRelease( r->dstref );
    ysmGlbFree( (dvoid*)r );
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1160, YSLSEV_WARNING,(char*)0,
	     YSLSTR(yduStr(exid)), YSLSTR(refstr), YSLEND );

    

    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1161, YSLSEV_WARNING,(char*)0,
	     YSLSTR(refstr), YSLEND );

    yoEnvInit(&ev);
    ydim_imr_destroyYortGlobal( cx->imr_ydrtcx, &ev, (yort_proc)r->dstref );
    yoEnvFree(&ev);

    
    yoRelease( r->dstref );
    r->dstref = (dvoid*)0;

    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1162, YSLSEV_WARNING,(char*)0,YSLNONE);

    ydrtRoute( cx, r );
  }
  if( refstr )
    yoFree( (dvoid*)refstr );
}





STATICF yort_proc ydrtGoodSpot( ydrtcx *cx, yore *r, boolean *ready )
{
  yort_proc	spot = (yort_proc)0;
  ub4		i, j;

  ydrte		best;
  ydrte		current;

  ydim_activeList    *alist = (ydim_activeList*)0;

  ub4		ecnt;		
  ysle		*ecur;		

  ydsp_proc	sproc;		
  sb4		rnd;		
  sysb8		tim;		
  char		*arg;
  sb4		nshift;
  ydrte		*rte;		
  ydsp_spawner	sp;
  yslst		*ilst;
  ysle		*ie;
  ydim_implInfo	*ii;

  CLRSTRUCT(best);
  CLRSTRUCT(current);
  best.cost_ydrte = SB4MAXVAL;
  current.active_ydrte = best.active_ydrte = (ydim_active)0;
  current.ain_ydrte = best.ain_ydrte = (ydimain*)0;

  

  nshift = (arg = ysResGetLast("ydrt.shiftbits")) ? (sb4)atol(arg) : (sb4)13;

  


  ilst = ysLstCreate();
  DISCARD ydimListImpl( ilst, cx->imr_ydrtcx,
		       (char*)ysidToStr(r->hdr.ref->intf),
                       (char*)r->hdr.ref->impl, (char*)0 );

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1163, YSLSEV_DEBUG(2),(char*)0,
	   YSLUB4(ysLstCount(ilst)), YSLEND );

  

  ecnt = 0;
  ecur = ysLstHead( cx->rtelst_ydrtcx );
  for( ie = ysLstHead( ilst ); ie ; ie = ysLstNext(ie) )
  {
    ii = (ydim_implInfo*)ysLstVal(ie);

    alist = &ii->alist_ydim_implInfo;

    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1164, YSLSEV_DEBUG(3),(char*)0,
	     YSLUB4(alist->_length), YSLEND );

    for( j = 0; j < alist->_length; j++ )
    {
      current.info_ydrte = ii;
      current.active_ydrte = alist->_buffer[j];
      current.ain_ydrte = ydimActiveGetAinfo( current.active_ydrte );
      current.ainfo_ydrte = &current.ain_ydrte->ainfo_ydimain;

      

      if( (current.ainfo_ydrte->implFlags_ydyo_activeInfo &
	   yort_suspended_implFlag)
	 || (!current.ainfo_ydrte->yort_ydyo_activeInfo
	 && alist->_length > current.info_ydrte->maxInstances_ydim_implInfo) )
      {
	continue;
      }

      current.cost_ydrte = ydrtSimpleCost( cx, current.ain_ydrte );
      current.cost_ydrte += ydrtMetricCost( cx, current.ain_ydrte, r );

      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1165, YSLSEV_DEBUG(3),(char*)0,
	       YSLSTR(yduIdStr(current.ainfo_ydrte->intf_ydyo_activeInfo)),
	       YSLSTR(yduIdStr(current.ainfo_ydrte->impl_ydyo_activeInfo)),
	       YSLUB4((ub4)current.cost_ydrte), YSLEND );

      
      if( current.cost_ydrte < best.cost_ydrte )
      {
	
	ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1166, YSLSEV_DEBUG(0),(char*)0,
		 YSLUB4((ub4)current.cost_ydrte),
		 YSLUB4((ub4)best.cost_ydrte), YSLEND );

	best.cost_ydrte = current.cost_ydrte;

	
	ecur = ysLstHead( cx->rtelst_ydrtcx );
	ecnt = 0;
      }

      
      if( current.cost_ydrte == best.cost_ydrte )
      {
	
	ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1167, YSLSEV_DEBUG(0),(char*)0,
		 YSLUB4((ub4)best.cost_ydrte), YSLEND);

	
	if( !ecur )
	  ecur = ysLstEnq(cx->rtelst_ydrtcx,ysmGlbAlloc(sizeof(*rte),"ydrte"));

	rte = (ydrte*)ysLstVal( ecur );
	ydrtCopyRte( rte, &current );
	ecur = ysLstNext( ecur );
	ecnt++;
      }
    } 

  } 

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1168, YSLSEV_DEBUG(0),(char*)0,
	   YSLUB4((ub4) ecnt), YSLEND);

  if( !ecnt )
  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1169, YSLSEV_INFO,(char*)0, YSLNONE);
  }
  else				
  {
    

    
    ysClock( &tim );
    rnd = sysb8msk( &tim ) >> nshift;
    if( rnd < 0 ) rnd = -rnd;
    j = (ub4)(rnd % ecnt);

    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1170, YSLSEV_DEBUG(0),(char*)0,
	     YSLUB4((ub4)j), YSLUB4((ub4)ecnt), YSLEND );

    
    for( i = 0, ecur = ysLstHead( cx->rtelst_ydrtcx ); i <= j ; i++ )
      continue;

    
    ydrtCopyRte( &best, (ydrte*)ysLstVal(ecur) );

    

    spot = best.ainfo_ydrte->yort_ydyo_activeInfo;

    *ready = TRUE;
    if( !spot ) 
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1171,YSLSEV_DEBUG(0),(char*)0,YSLNONE);

      
      spot = ydimStartLaunch( cx->imr_ydrtcx, best.info_ydrte,
			     &cx->ltimeout_ydrtcx,
			     (dvoid*)cx, ydrtLaunchForward, (dvoid*)r );

      
      if( !strcmp( best.info_ydrte->host_ydim_implInfo, cx->host_ydrtcx ) )
      {
	yseTry
	{
	  ydsp_spawner_Launch( cx->ydsp_ydrtcx, &cx->ev_ydrtcx,
			      best.info_ydrte->pathName_ydim_implInfo,
			      &best.info_ydrte->args_ydim_implInfo, &sproc );
	}
	yseCatch( YDSP_SPAWNER_EX_LAUNCHFAIL )
	{
	  yslError("ydrt: launch of %s failed\n");
	}
	yseEnd;
      }
      else
      {
	sp = (ydsp_spawner)
	  yoBind(ydsp_spawner__id, (char*)0, (yoRefData*)0,
		 best.info_ydrte->host_ydim_implInfo );
	ydsp_spawner_Launch_nw( sp,
			       &cx->ev_ydrtcx,
			       best.info_ydrte->pathName_ydim_implInfo,
			       &best.info_ydrte->args_ydim_implInfo, &sproc,
			       ysEvtDummy() );
	yoRelease( (dvoid*)sp );
      }
      *ready = FALSE;
    }
    else if( ydimIsLaunching( cx->imr_ydrtcx, spot) )
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1172, YSLSEV_INFO,(char*)0,YSLNONE);

      ydimAppendLaunch( cx->imr_ydrtcx, best.info_ydrte, (dvoid*)r );
      *ready = FALSE;
    }

    
    if( *ready )
    {
      if( ++best.ain_ydrte->ucnt_ydimain >= YDRT_OFF_PROC_COST )
      {
	for( ie = ysLstHead( ilst ); ie ; ie = ysLstNext(ie) )
	{
	  current.info_ydrte = (ydim_implInfo*)ysLstVal(ie);
	  alist = &current.info_ydrte->alist_ydim_implInfo;
	  for( j = 0 ; j < alist->_length ; j++ )
	  {
	    current.ain_ydrte = ydimActiveGetAinfo( alist->_buffer[j] );
	    current.ain_ydrte->ucnt_ydimain = 0;
	  }
	}
	best.ain_ydrte->ucnt_ydimain = 1;
      }
    }

  } 

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1173, YSLSEV_DEBUG(0),(char*)0,
	   YSLSTR( spot ? "TRUE" : "FALSE" ),
	   YSLSTR( ready ? "TRUE" : "FALSE" ), YSLEND );

  
  ysLstDestroy( ilst, (ysmff)0 );

  return( spot );
}



STATICF void ydrtActivateForward( yore *r )
{
  ydrtax *ax = (ydrtax*)ysmGlbAlloc(sizeof(*ax), ydrtax_tag );

  yoEnvInit(&ax->ev_ydrtax);
  ax->req_ydrtax = r;
  ax->state_ydrtax = start_ydrtax;
  ax->evt_ydrtax = ysSemCreate( (dvoid*)ax );
  ydrtActivation( (dvoid*)ax, (ysid*)0, (dvoid*)0, (size_t)0 );
}



STATICF void ydrtActivation( dvoid *usrp, CONST ysid *exid,
			    dvoid *arg, size_t argsz )
{
  ydrtax *ax = (ydrtax*)usrp;
  ydrtcx *cx;
  yore *r;

  ysmCheck( ax, ydrtax_tag );
  r = ax->req_ydrtax;
  cx = (ydrtcx*)r->usrp;

  
  ysEvtDestroy( ax->evt_ydrtax );

  
  while( ax->state_ydrtax != exit_ydrtax )
  {
    switch( ax->state_ydrtax )
    {
    case start_ydrtax:

      ax->state_ydrtax = lockWait_ydrtax;
      ax->evt_ydrtax = ysEvtCreate( ydrtActivation, (dvoid*)ax,
				   cx->q_ydrtcx, TRUE );
      ydch_och_lock_nw( cx->ydch_ydrtcx, &ax->ev_ydrtax,
		       (CORBA_Object)r->hdr.ref, (yort_proc*)&r->dstref,
		       ax->evt_ydrtax );
      break;

    case lockWait_ydrtax:

      ysEvtDestroy( ax->evt_ydrtax );

      ax->evt_ydrtax = ysEvtCreate( ydrtActivation, (dvoid*)ax,
				   cx->q_ydrtcx, TRUE );

      
      if( r->dstref )
      {
	ax->state_ydrtax = done_ydrtax;
	if( yoIsEq( (dvoid*)yoYort(), (dvoid*)r->dstref ) )
	{
#ifdef NEVER
	  yoFwdActivateRequest( r, ax->evt_ydrtax ); 
#endif
	}
	else
	  yoFwdRequest( r, ax->evt_ydrtax ); 
      }
      else			
      {
	ydch_och_lock_nw( cx->ydch_ydrtcx, &ax->ev_ydrtax,
			 (CORBA_Object)r->hdr.ref, (yort_proc*)&r->dstref,
			 ax->evt_ydrtax );
      }
      break;

    case done_ydrtax:		

      if( exid || !r->dstref )
	exid = YDRT_EX_ACTIVATE_FAILED;

      ax->state_ydrtax = exit_ydrtax;
      ydrtForwardHandler( (dvoid*)r, exid, arg, argsz );
      break;

    case exit_ydrtax:

      yoEnvFree(&ax->ev_ydrtax);
      ysmGlbFree( (dvoid*)ax );
      break;

    default:
      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1174, YSLSEV_EMERG,(char*)0,
	       YSLUB4((ub4)ax->state_ydrtax), YSLEND );
      ysePanic(YDRT_EX_INTERNAL);
      break;
    }
  }
}




STATICF ydrtCost ydrtSimpleCost( ydrtcx *cx, ydimain *ain )
{
  ydrtCost  rv;
  yort_proc y = ain->ainfo_ydimain.yort_ydyo_activeInfo;

  if( !y )
    rv = YDRT_SPAWN_COST;	
  else if( yoIsEq( (dvoid*)y, (dvoid*)cx->imr_ydrtcx ) )
    rv = YDRT_SPAWNING_COST;	
  else if( !yoIsEq( (dvoid*)y, (dvoid*)cx->yort_ydrtcx ) )
    rv = YDRT_OFF_PROC_COST;	
  else
    rv = 0;			

  
  rv += (ydrtCost)ain->ucnt_ydimain;

  return( rv );
}





STATICF void ydrtLaunchForward( dvoid *usrp, yslst *reqs, yslst *active,
			       yslst *timedout )
{
  ydrtcx    *cx = (ydrtcx*)usrp;
  yore	    *r = (yore*)0;
  ysle	    *e = (ysle*)0;
  ysle	    *f;
  ysle	    *nextf;
  ydyo_activeInfo    *ainfo;
  char	    *refstr = (char*)0;

  

  
  ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1175, YSLSEV_DEBUG(0),(char*)0,
	   YSLUB4(ysLstCount( reqs )), YSLUB4(ysLstCount( active )),
	   YSLUB4( timedout ? ysLstCount(timedout) : 0), YSLEND );

  
  for( f = ysLstHead( reqs ) ; f ; f = nextf )
  {
    nextf = ysLstNext( f );
    r = (yore*)f->ptr;
    r->usrp = usrp;

    
    ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1176, YSLSEV_DEBUG(3),(char*)0,
	     YSLSTR(yduIdStr(r->hdr.ref->intf)),
	     YSLSTR(yduIdStr(r->hdr.ref->impl)), YSLEND );

    for( e = ysLstHead(active); e ; e = ysLstNext( e ) )
    {
      ainfo = (ydyo_activeInfo*)ysLstVal(e);

      
      ysRecord(YS_PRODUCT,YDRT_FAC, (ub4)1177, YSLSEV_DEBUG(4),(char*)0,
	       YSLSTR(yduIdStr(ainfo->intf_ydyo_activeInfo)),
	       YSLSTR(yduIdStr(ainfo->impl_ydyo_activeInfo)), YSLEND );

      if( !strcmp( ysidToStr(r->hdr.ref->intf), ainfo->intf_ydyo_activeInfo )
         && !yduWildStrcmp( r->hdr.ref->impl, ainfo->impl_ydyo_activeInfo))
      {
	DISCARD ysLstRem( reqs, f );
	r->dstref = (dvoid*)yoDuplicate((dvoid*)ainfo->yort_ydyo_activeInfo);


	refstr = yoRefToStr(r->dstref);
	
	ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1185,YSLSEV_DEBUG(0), (char*)0,
		 YSLSTR(yduIdStr(r->hdr.ref->intf)),
		 YSLSTR(yduIdStr(r->hdr.ref->impl)),
		 YSLSTR(refstr), YSLEND );
	yoFree((dvoid*)refstr);

	yoFwdRequest( r, ysEvtSimple(ydrtForwardHandler, (dvoid*)r) );

	

	
	ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1178,YSLSEV_DEBUG(4),
		 (char*)0,YSLNONE);
      }
    }
  }

  
  if( timedout )
  {
    while( (r = (yore *)ysLstDeq( reqs ) ) )
    {
      
      ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1179,YSLSEV_DEBUG(4), (char*)0,
	       YSLSTR(yduIdStr(r->hdr.ref->intf)),
	       YSLSTR(yduIdStr(r->hdr.ref->impl)), YSLEND);
      yoRejRequest( r );
      yoFreeRouteReq( r );
      ysmGlbFree( (dvoid*)r );
    }
  }

  

  
  ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1180,YSLSEV_DEBUG(0), (char*)0,
	   YSLUB4((ub4)ysLstCount(reqs)), YSLEND );
}






STATICF ydrtCost ydrtMetricCost( ydrtcx *cx, ydimain *ain, yore *r )
{
  yort_methodInfo   *m;
  ydmt_minfo  *minfo;
  ub4	    i;
  ydrtCost  rv = 0;
  ydyo_activeInfo *ainfo;

  ainfo = &ain->ainfo_ydimain;
  minfo = (ydmt_minfo*)ain->usrp_ydimain;
  if( !minfo )
  {
    
    ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1181,YSLSEV_DEBUG(4), (char*)0, YSLNONE);
    rv = (ydrtCost)0;
  }
  else
  {
#ifdef NEVER

    

    qinfo = &minfo->all_ydmt_minfo.queue_yort_implAll;

    
    ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1182,YSLSEV_DEBUG(3), (char*)0,
	     YSLUB4(qinfo->avgDelayMs_yort_queueInfo),
	     YSLUB4(qinfo->curDelayMs_yort_queueInfo),
	     YSLUB4(qinfo->curLen_yort_queueInfo),
	     YSLUB4(cx->qscale_ydrtcx), YSLEND );

    

    rv = (qinfo->avgDelayMs_yort_queueInfo +
	  qinfo->curDelayMs_yort_queueInfo +
	  qinfo->curLen_yort_queueInfo * cx->qscale_ydrtcx );
#endif

    
    for( i = 0 ; i < minfo->all_ydmt_minfo.methods_yort_implAll._length ; i++ )
    {
      m = &minfo->all_ydmt_minfo.methods_yort_implAll._buffer[i];
      if( !strcmp( m->name_yort_methodInfo, r->hdr.op ) )
      {
	
	ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1183,YSLSEV_DEBUG(3), (char*)0,
		 YSLSTR(m->name_yort_methodInfo),
		 YSLUB4(m->avgTimeMs_yort_methodInfo),
		 YSLUB4(m->lastTimeMs_yort_methodInfo), YSLEND );

	rv += (m->avgTimeMs_yort_methodInfo + m->lastTimeMs_yort_methodInfo);
	break;
      }
    }
  }

  
  ysRecord(YS_PRODUCT,YDRT_FAC,(ub4)1184,YSLSEV_DEBUG(0), (char*)0,
	   YSLUB4((ub4) rv ), YSLEND );

  return( rv );
}





STATICF void ydrtCopyRte( ydrte *dest, ydrte *src )
{
  dest->cost_ydrte = src->cost_ydrte;
  dest->info_ydrte = src->info_ydrte;
  dest->ain_ydrte = src->ain_ydrte;
  if( src->active_ydrte )
  {
    dest->active_ydrte = src->active_ydrte;
    dest->ainfo_ydrte = src->ainfo_ydrte;
  }
  else
    dest->active_ydrte = (ydim_active)0;

  dest->ain_ydrte->usrp_ydimain = src->ain_ydrte->usrp_ydimain;
}


