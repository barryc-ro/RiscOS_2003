/* mx/src/ye/yece.c */


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
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif
#ifndef YECE_ORACLE
#include <yece.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YECEVCH_ORACLE
#include <yecevch.h>
#endif
#ifndef YECEVCHI_ORACLE
#include <yecevchI.h>
#endif
#ifndef YOTK_ORACLE
#include <yotk.h>
#endif
#ifndef YSLOG_ORACLE
#include <yslog.h>
#endif
#ifndef YEMSG_ORACLE
#include <yemsg.h>
#endif
#ifndef YEEVF_ORACLE
#include <yeevf.h>
#endif

#define YECE_FAC    "YECE"





typedef struct yececx yececx;	
typedef struct yecech yecech;	
typedef struct yecesc yecesc;	
typedef struct yecep yecep;	
typedef struct yecepc yecepc;	

static CONST_W_PTR struct yeceCa_ProxyPushCons__tyimpl
yeceCa_ProxyPushCons__impl =
 {
  yeceCa_ProxyPushCons_connect_push_supp_i,
  yeceCa_ProxyPushCons_push_i,
  yeceCa_ProxyPushCons_disconnect_i
 };

static CONST_W_PTR struct yeceCa_ProxyPullSupp__tyimpl
yeceCa_ProxyPullSupp__impl =
 {
  yeceCa_ProxyPullSupp_connect_pull_cons_i,
  yeceCa_ProxyPullSupp_pull_i,
  yeceCa_ProxyPullSupp_try_pull_i,
  yeceCa_ProxyPullSupp_disconnect_i
 };

static CONST_W_PTR struct yeceCa_ProxyPullCons__tyimpl
yeceCa_ProxyPullCons__impl =
 {
  yeceCa_ProxyPullCons_connect_pull_supp_i,
  yeceCa_ProxyPullCons_disconnect_i
 };

static CONST_W_PTR struct yeceCa_ProxyPushSupp__tyimpl
yeceCa_ProxyPushSupp__impl =
 {
  yeceCa_ProxyPushSupp_connect_push_cons_i,
  yeceCa_ProxyPushSupp_disconnect_push_supp_i
 };

static CONST_W_PTR struct yeceCa_ConsAdm__tyimpl yeceCa_ConsAdm__impl =
 {
  yeceCa_ConsAdm_obtain_push_supp_i,
  yeceCa_ConsAdm_obtain_pull_supp_i
 };

static CONST_W_PTR struct yeceCa_SuppAdm__tyimpl yeceCa_SuppAdm__impl =
 {
  yeceCa_SuppAdm_obtain_push_cons_i,
  yeceCa_SuppAdm_obtain_pull_cons_i
 };

static CONST_W_PTR struct yeceCa_EventChannel__tyimpl
yeceCa_EventChannel__impl =
 {
  yeceCa_EventChannel_for_consumers_i,
  yeceCa_EventChannel_for_suppliers_i,
  yeceCa_EventChannel_destroy_i
 };

static CONST_W_PTR struct yeceCa_Factory__tyimpl yeceCa_Factory__impl =
 {
  yeceCa_Factory_create_i,
  yeceCa_Factory__get_channels_i,
  yeceCa_Factory__get_self_i
 };

static CONST_W_PTR struct yeceTeca_TypedProxyPushCons__tyimpl
yeceTeca_TypedProxyPushCons__impl =
 {
  yeceTeca_TypedProxyPushCons_connect_push_supp_i,
  yeceTeca_TypedProxyPushCons_push_i,
  yeceTeca_TypedProxyPushCons_disconnect_i,
  yeceTeca_TypedProxyPushCons_get_typed_cons_i
 };

static CONST_W_PTR struct yeceTeca_TypedProxyPullSupp__tyimpl
yeceTeca_TypedProxyPullSupp__impl =
 {
  yeceTeca_TypedProxyPullSupp_connect_pull_cons_i,
  yeceTeca_TypedProxyPullSupp_pull_i,
  yeceTeca_TypedProxyPullSupp_try_pull_i,
  yeceTeca_TypedProxyPullSupp_disconnect_i,
  yeceTeca_TypedProxyPullSupp_get_typed_supp_i
 };

static CONST_W_PTR struct yeceTeca_TypedSuppAdm__tyimpl
yeceTeca_TypedSuppAdm__impl =
 {
  yeceTeca_TypedSuppAdm_obtain_typed_push_cons_i,
  yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_i,
  yeceTeca_TypedSuppAdm_obtain_push_cons_i,
  yeceTeca_TypedSuppAdm_obtain_pull_cons_i
 };

static CONST_W_PTR struct yeceTeca_TypedConsAdm__tyimpl
yeceTeca_TypedConsAdm__impl =
 {
  yeceTeca_TypedConsAdm_obtain_typed_pull_supp_i,
  yeceTeca_TypedConsAdm_obtain_typed_push_supp_i,
  yeceTeca_TypedConsAdm_obtain_push_supp_i,
  yeceTeca_TypedConsAdm_obtain_pull_supp_i
 };

static CONST_W_PTR struct yeceTeca_TypedEventChannel__tyimpl
yeceTeca_TypedEventChannel__impl =
 {
  yeceTeca_TypedEventChannel_for_consumers_i,
  yeceTeca_TypedEventChannel_for_suppliers_i
 };

static CONST_W_PTR struct yeceTeca_Factory__tyimpl yeceTeca_Factory__impl =
 {
  yeceTeca_Factory_create_i,
  yeceTeca_Factory__get_channels_i,
  yeceTeca_Factory__get_self_i
 };



struct yecepc
{
  CORBA_Object    self_yecepc;	
  boolean   pull_yecepc;	
  boolean   connected_yecepc;	
  CORBA_Object    supp_yecepc;	
  ysevt		  *sevt_yecepc;	
  yoenv	    ev_yecep;		
  CORBA_Object    proxy_yecepc;	
  yecesc    *sc_yecepc;		
  ysle	    *le_yecepc;		
};


struct yecep
{
  CORBA_Object	self_yecep;	
  yslst		*q_yecep;	
  boolean	connected_yecep;
  CORBA_Object	dest_yecep;	
  ysevt		*devt_yecep;	
  yoenv		ev_yecep;	
  yecesc	*sc_yecep;	
  ysle		*le_yecep;	
  ub4		ucnt_yecep;	
  boolean	removed_yecep;	
  boolean	dead_yecep;	
};


struct yecesc
{
  ysspNode  node_yecesc;	
  sb4	    nevts_yecesc;	
  yecech    *ch_yecesc;		
  yslst	    *sups_yecesc;	
  yslst	    *cons_yecesc;	
};


struct yecech
{
  boolean   typed_yecech;
  yececx    *cx_yecech;

  yeceCa_EventChannel		self_yecech;
  yeceTeca_TypedEventChannel	tself_yecech;
  yeceCa_ConsAdm		cself_yecech;
  yeceTeca_TypedConsAdm		tcself_yecech;
  yeceCa_SuppAdm		sself_yecech;
  yeceTeca_TypedSuppAdm		tsself_yecech;

  ysspTree  sch_yecech;		
  sb4	    nevts_yecech;	
  ysle	    *le_yecech;		
};


struct yececx
{
  ysque	    *q_yececx;		
  yeceCa_Factory self_yececx;	
  yeceTeca_Factory tself_yececx; 
  yslst	    *ch_yececx;		
};

externdef ysmtagDecl(yececx_tag) = "yececx";
externdef ysmtagDecl(yecech_tag) = "yecech";
externdef ysmtagDecl(yecesc_tag) = "yecesc";

externdef ysmtagDecl(yecep_tag) = "yecep";
externdef ysmtagDecl(yecepc_tag) = "yecepc";
externdef ysmtagDecl(yecers_tag) = "yecers";
externdef ysmtagDecl(yecec_tag) = "yecec";

externdef ysidDecl( YECE_EX_UNIMPL ) = "yece::unimplemented";

STATICF void yeceSendComplete( dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz );

STATICF yecech *yeceNewChannel( yececx *cx, boolean typed );
STATICF void yeceDestroyChannel( yecech *ch );

STATICF yecesc *yeceGetSubChannel( yecech *ch, char *eventIntf );
STATICF void yeceDestroySubChannel( yecesc *sc );

STATICF yecepc *yeceNewProxyConsumer( yecesc *sc );
STATICF void yeceDestroyProxyConsumer( yecepc *pc );

STATICF yecep *yeceNewParty( yecesc *sc );
STATICF void yeceDestroyParty( yecep *p );

STATICF void yeceDestroyAny(dvoid *x );

STATICF sword yeceChCmp( CONST dvoid *a, CONST dvoid *b );

STATICF void yecePollSuppliers( yecesc *sc, yoenv *ev );

STATICF void yeceShowAny(char *msg, yoany *ap );

STATICF void yecePCDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			 size_t argsz );

STATICF void yecePDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			size_t argsz );






yeceCa_Factory yeceInit( ysque *q )
{
  yececx   *cx = (yececx*)ysmGlbAlloc( sizeof(*cx), yececx_tag );

  cx->q_yececx = q;
  cx->ch_yececx = ysLstCreate();

  yoSetImpl( yeceCa_ProxyPushCons__id, (char*)0,
	    yeceCa_ProxyPushCons__stubs, (dvoid*)&yeceCa_ProxyPushCons__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( yeceCa_ProxyPullSupp__id, (char*)0,
	    yeceCa_ProxyPullSupp__stubs, (dvoid*)&yeceCa_ProxyPullSupp__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceCa_ProxyPullCons__id, (char*)0,
	    yeceCa_ProxyPullCons__stubs, (dvoid*)&yeceCa_ProxyPullCons__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( yeceCa_ProxyPushSupp__id, (char*)0,
	    yeceCa_ProxyPushSupp__stubs, (dvoid*)&yeceCa_ProxyPushSupp__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceCa_ConsAdm__id, (char*)0,
	    yeceCa_ConsAdm__stubs, (dvoid*)&yeceCa_ConsAdm__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( yeceCa_SuppAdm__id, (char*)0,
	    yeceCa_SuppAdm__stubs, (dvoid*)&yeceCa_SuppAdm__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceCa_EventChannel__id, (char*)0,
	    yeceCa_EventChannel__stubs, (dvoid*)&yeceCa_EventChannel__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceCa_Factory__id, (char*)0,
	    yeceCa_Factory__stubs, (dvoid*)&yeceCa_Factory__impl,
	    (yoload)0, TRUE, (dvoid*)cx );

  yoSetImpl( yeceTeca_TypedProxyPushCons__id, (char*)0,
	    yeceTeca_TypedProxyPushCons__stubs,
	    (dvoid*)&yeceTeca_TypedProxyPushCons__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( yeceTeca_TypedProxyPullSupp__id, (char*)0,
	    yeceTeca_TypedProxyPullSupp__stubs,
	    (dvoid*)&yeceTeca_TypedProxyPullSupp__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceTeca_TypedSuppAdm__id, (char*)0,
	    yeceTeca_TypedSuppAdm__stubs,
	    (dvoid*)&yeceTeca_TypedSuppAdm__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( yeceTeca_TypedConsAdm__id, (char*)0,
	    yeceTeca_TypedConsAdm__stubs,
	    (dvoid*)&yeceTeca_TypedConsAdm__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceTeca_TypedEventChannel__id, (char*)0,
	    yeceTeca_TypedEventChannel__stubs,
	    (dvoid*)&yeceTeca_TypedEventChannel__impl,
	    (yoload)0, FALSE, (dvoid*)cx );
	
  yoSetImpl( yeceTeca_Factory__id, (char*)0,
	    yeceTeca_Factory__stubs, (dvoid*)&yeceTeca_Factory__impl,
	    (yoload)0, TRUE, (dvoid*)cx );

  cx->self_yececx =
    (yeceCa_Factory)yoCreate( yeceCa_Factory__id, (char*)0,
			     (yoRefData*)0, (char*)0, (dvoid*)0 );

  cx->tself_yececx =
    (yeceTeca_Factory)yoCreate( yeceTeca_Factory__id, (char*)0,
			       (yoRefData*)0, (char*)0, (dvoid*)0 );

  yoImplReady( yeceCa_ProxyPushCons__id, (char*)0, q );
  yoImplReady( yeceCa_ProxyPullSupp__id, (char*)0, q );
  yoImplReady( yeceCa_ProxyPullCons__id, (char*)0, q );
  yoImplReady( yeceCa_ProxyPushSupp__id, (char*)0, q );
  yoImplReady( yeceCa_ConsAdm__id, (char*)0, q );
  yoImplReady( yeceCa_SuppAdm__id, (char*)0, q );
  yoImplReady( yeceCa_EventChannel__id, (char*)0, q );
  yoImplReady( yeceCa_Factory__id, (char*)0, q );

  yoImplReady( yeceTeca_TypedProxyPushCons__id, (char*)0, q);
  yoImplReady( yeceTeca_TypedProxyPullSupp__id, (char*)0, q);
  yoImplReady( yeceTeca_TypedSuppAdm__id, (char*)0, q );
  yoImplReady( yeceTeca_TypedConsAdm__id, (char*)0, q );
  yoImplReady( yeceTeca_TypedEventChannel__id, (char*)0, q );
  yoImplReady( yeceTeca_Factory__id, (char*)0, q );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)501, YSLSEV_INFO, (char*)0, YSLNONE);

  return( (yeceCa_Factory)yoDuplicate((dvoid*)cx->self_yececx ));
}




void yeceTerm( yeceCa_Factory or )
{
  yececx *cx = (yececx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)502, YSLSEV_INFO, (char*)0, YSLNONE );

  yoImplDeactivate( yeceCa_ProxyPushCons__id, (char*)0 );
  yoImplDeactivate( yeceCa_ProxyPullSupp__id, (char*)0 );
  yoImplDeactivate( yeceCa_ProxyPullCons__id, (char*)0 );
  yoImplDeactivate( yeceCa_ProxyPushSupp__id, (char*)0 );
  yoImplDeactivate( yeceCa_ConsAdm__id, (char*)0 );
  yoImplDeactivate( yeceCa_SuppAdm__id, (char*)0 );
  yoImplDeactivate( yeceCa_EventChannel__id, (char*)0 );
  yoImplDeactivate( yeceCa_Factory__id, (char*)0 );

  yoImplDeactivate( yeceTeca_TypedProxyPushCons__id, (char*)0 );
  yoImplDeactivate( yeceTeca_TypedProxyPullSupp__id, (char*)0);
  yoImplDeactivate( yeceTeca_TypedSuppAdm__id, (char*)0 );
  yoImplDeactivate( yeceTeca_TypedConsAdm__id, (char*)0 );
  yoImplDeactivate( yeceTeca_TypedEventChannel__id, (char*)0 );
  yoImplDeactivate( yeceTeca_Factory__id, (char*)0 );

  yoDispose( (dvoid*)cx->self_yececx );
  yoDispose( (dvoid*)cx->tself_yececx );
  yoRelease( (dvoid*)cx->self_yececx );
  yoRelease( (dvoid*)cx->tself_yececx );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)503, YSLSEV_INFO, (char*)0, YSLNONE );

  ysLstDestroy( cx->ch_yececx, (ysmff)yeceDestroyChannel );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)504, YSLSEV_INFO, (char*)0, YSLNONE );

  ysmGlbFree( (dvoid*)cx );
}






STATICF void yeceProxyConsumer( CORBA_Object or, yoenv *ev,
			       char *opernm, yosreq *rh )
{
  yecech    *ch;		
  ysle	    *e = (ysle*)0;	
  yecep	    *p;			
  yecesc    *sc;		
  yecepc    *pc;
  
  pc = (yecepc*)yoGetState((dvoid*)or);
  sc = pc->sc_yecepc;
  ch = sc->ch_yecesc;

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)505, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or),
	   YSLSTR( yeuStr(opernm) ), YSLEND );

  ch->nevts_yecech++;
  sc->nevts_yecesc++;
  for( e = ysLstHead( sc->cons_yecesc ); e ; e = ysLstNext(e) )
  {
    p = (yecep*)ysLstVal(e);
    if( p->dead_yecep )
      continue;

    
    ysRecord(YS_PRODUCT,YECE_FAC,(ub4)506, YSLSEV_DEBUG(3), (char*)0,
	     YSLANY(yoTcObject,&p->dest_yecep), YSLEND );

    p->ucnt_yecep++;
    yodsReSendReq( rh, p->dest_yecep, &p->ev_yecep,
		  ysEvtCreate( yeceSendComplete, (dvoid*)p,
			      ch->cx_yecech->q_yececx, FALSE )); 
  }
}





STATICF void yeceSendComplete( dvoid *usrp, CONST ysid *exid,
			      dvoid *arg, size_t argsz )
{
  yecep *p;

  p = (yecep*)usrp;
   
  ysmCheck(p, yecep_tag); 
  p->ucnt_yecep--;

  if( exid )
    p->dead_yecep = TRUE;
  else if( arg )
    yodsFreeRep( *(yodsrep**)arg );

  if( p->dead_yecep && !p->ucnt_yecep )
  {
    yslError("yeceSendComplete: exid %s, removing party %p\n",
	     ysidToStr(exid), p );

    
    if( !p->removed_yecep )
      DISCARD ysLstRem( p->sc_yecep->cons_yecesc, p->le_yecep );

    
    ysRecord(YS_PRODUCT,YECE_FAC,(ub4)507, YSLSEV_WARNING, (char*)0,
	     YSLSTR(ysidToStr(exid)),
	     YSLANY(yoTcObject,&p->dest_yecep),
	     YSLEND );

    yeceDestroyParty( p );
  }
}




STATICF yecech *yeceNewChannel( yececx *cx, boolean typed )
{
  yecech *ch;

  ch = (yecech*)ysmGlbAlloc( sizeof(*ch), yecech_tag );
  CLRSTRUCT(*ch);
  
  if( (ch->typed_yecech = typed) )
  {
    ch->tself_yecech =
      (yeceTeca_TypedEventChannel) yoCreate( yeceTeca_TypedEventChannel__id,
					    (char*)0, (yoRefData*)0, 
					    (char*)0, (dvoid*)ch );
    ch->tcself_yecech =
      (yeceTeca_TypedConsAdm)yoCreate( yeceTeca_TypedConsAdm__id, (char*)0,
				      (yoRefData*)0, (char*)0, (dvoid*)ch );
    ch->tsself_yecech =
      (yeceTeca_TypedSuppAdm)yoCreate( yeceTeca_TypedSuppAdm__id, (char*)0,
				      (yoRefData*)0, (char*)0, (dvoid*)ch );
  }
  else				
  {
    ch->self_yecech =
      (yeceCa_EventChannel) yoCreate( yeceCa_EventChannel__id,
				     (char*)0, (yoRefData*)0, 
				     (char*)0, (dvoid*)ch );
    ch->cself_yecech =
      (yeceCa_ConsAdm)yoCreate( yeceCa_ConsAdm__id, (char*)0, (yoRefData*)0,
			       (char*)0, (dvoid*)ch );
    ch->sself_yecech =
      (yeceCa_SuppAdm)yoCreate( yeceCa_SuppAdm__id, (char*)0, (yoRefData*)0,
			       (char*)0, (dvoid*)ch );
  }
  ch->cx_yecech = cx;
  DISCARD ysspNewTree( &ch->sch_yecech, yeceChCmp );
  ch->nevts_yecech = 0;
  ch->le_yecech = ysLstEnq( cx->ch_yececx, (dvoid*)ch );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)508, YSLSEV_DEBUG(2), (char*)0,
	   YSLSTR( typed ? "typed" : "untyped" ),
	   YSLANY(yoTcObject,ch->typed_yecech ?
		  (dvoid*)&ch->tself_yecech : (dvoid*)&ch->self_yecech),
	   YSLEND );

  return( ch );
}





STATICF void yeceDestroyChannel( yecech *ch )
{
  ysspNode *n;

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)509, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,
		  ch->typed_yecech ?
		  (dvoid**)&ch->tself_yecech : (dvoid**)&ch->self_yecech),
	   YSLEND );

  while( (n = ysspDeq( &ch->sch_yecech.root_ysspTree ) ) )
    yeceDestroySubChannel( (yecesc*)n );

  if( ch->self_yecech )
  {
    yoDispose((dvoid*)ch->self_yecech);
    yoRelease((dvoid*)ch->self_yecech);
  }
  if( ch->tself_yecech )
  {
    yoDispose((dvoid*)ch->tself_yecech);
    yoRelease((dvoid*)ch->tself_yecech);
  }
  ysmGlbFree( (dvoid*)ch );
}




STATICF yecesc *yeceGetSubChannel( yecech *ch, char *eventIntf )
{
  yececx *cx;
  yecesc *noreg sc;
  ysspNode *n;
  char	    *intf;

  NOREG(sc);

  intf = eventIntf;

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)510, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject, ch->typed_yecech ?
		  (dvoid**)&ch->tself_yecech : (dvoid**)&ch->self_yecech),
	   YSLSTR(yeuStr(intf)), YSLEND ); 

  if( (n = ysspLookup( (dvoid*)intf, &ch->sch_yecech ) ) )
  {
    sc = (yecesc*)n;
    
    ysRecord(YS_PRODUCT,YECE_FAC,(ub4)511, YSLSEV_DEBUG(2), (char*)0, YSLNONE);
  }
  else
  {
    sc = (yecesc*)ysmGlbAlloc(sizeof(*sc), yecesc_tag );
    CLRSTRUCT(*sc);
    sc->node_yecesc.key_ysspNode = (dvoid*)ysStrDup(intf);
    sc->ch_yecesc = ch;
    sc->nevts_yecesc = 0;
    sc->sups_yecesc = ysLstCreate();
    sc->cons_yecesc = ysLstCreate();
    DISCARD ysspEnq( &sc->node_yecesc, &ch->sch_yecech );
    cx = ch->cx_yecech;
	
    
    ysRecord(YS_PRODUCT,YECE_FAC,(ub4)512, YSLSEV_DEBUG(2), (char*)0, YSLNONE);

    if( intf )
    {
      yseTry
      {
	yodsSetImpl( eventIntf, (char*)0, yeceProxyConsumer,
		    (yoload)0, FALSE, (dvoid*)cx );
	yoImplReady( eventIntf, (char*)0, cx->q_yececx );
      }
      yseCatch(YO_EX_DUPLICATE)
      {
	
      }
      yseEnd;
    }
  }
  return( sc );
}




STATICF void yeceDestroySubChannel( yecesc *sc )
{
  
  ysmCheck( sc, yecesc_tag );
  
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)513, YSLSEV_DEBUG(2), (char*)0,
	   YSLSTR(yeuStr(sc->node_yecesc.key_ysspNode)), YSLEND );
      
  if( sc->node_yecesc.key_ysspNode )
  {
    yseTry
    {
      
      ysRecord(YS_PRODUCT,YECE_FAC,(ub4)514, YSLSEV_DEBUG(2), (char*)0,
	       YSLSTR(sc->node_yecesc.key_ysspNode), YSLEND );
      yoImplDeactivate( (char*)sc->node_yecesc.key_ysspNode, (char*)0 );

      
      ysRecord(YS_PRODUCT,YECE_FAC,(ub4)515,YSLSEV_DEBUG(2),(char*)0,YSLNONE);
    }
    yseCatchAll
    {
      
      ysRecord(YS_PRODUCT,YECE_FAC,(ub4)515,YSLSEV_WARNING,(char*)0,
	       YSLSTR(ysidToStr(yseExid)),
	       YSLSTR(sc->node_yecesc.key_ysspNode), YSLEND );
      yslError("yeceDestroySubChannel: exid %s\n", ysidToStr(yseExid) );
    }
    yseEnd;
  }

  ysLstDestroy( sc->sups_yecesc, (ysmff)yeceDestroyProxyConsumer );
  ysLstDestroy( sc->cons_yecesc, (ysmff)yeceDestroyParty );

  
  ysmGlbFree( sc->node_yecesc.key_ysspNode );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)517, YSLSEV_DEBUG(2), (char*)0, YSLNONE);

  ysmGlbFree( (dvoid*)sc );
}



STATICF yecep *yeceNewParty( yecesc *sc )
{
  yecep *p;

  p = (yecep*)ysmGlbAlloc( sizeof(*p), yecep_tag );
  CLRSTRUCT(*p);
  p->connected_yecep = FALSE;
  p->dest_yecep = (CORBA_Object)0;
  p->sc_yecep = sc;
  p->q_yecep = (yslst*)0;
  yoEnvInit(&p->ev_yecep);
  p->removed_yecep = FALSE;
  p->dead_yecep = FALSE;
  p->ucnt_yecep = 0;
  p->le_yecep = ysLstEnq( sc->cons_yecesc, (dvoid*)p );

  

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)518, YSLSEV_DEBUG(2), (char*)0, YSLNONE );
  return p;
}




STATICF void yeceDestroyParty( yecep *p )
{
  
  ysmCheck( p, yecep_tag );

  
  p->removed_yecep = TRUE;
  if( p->ucnt_yecep )
    return;


  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)519, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&p->self_yecep), YSLEND );
  
  yoDispose((dvoid*)p->self_yecep);
  yoRelease((dvoid*)p->self_yecep);
  if( p->dest_yecep )
  {
    yoRelease((dvoid*)p->dest_yecep);
    ysEvtDestroy( p->devt_yecep );
  }
  if( p->q_yecep )
    ysLstDestroy( p->q_yecep, yeceDestroyAny );
  yoEnvFree(&p->ev_yecep);
  ysmGlbFree( (dvoid*)p );
}




STATICF void yeceDestroyAny(dvoid *x)
{
  yotkFreeVal( yoTcAny, x, (ysmff)0 );
  ysmGlbFree( x );
}






STATICF yecepc *yeceNewProxyConsumer( yecesc *sc )
{
  yecepc *pc = (yecepc*)ysmGlbAlloc(sizeof(*pc), yecepc_tag); 

  pc->pull_yecepc = FALSE;
  pc->supp_yecepc = (CORBA_Object)0;
  pc->proxy_yecepc = (CORBA_Object)0;
  pc->connected_yecepc = FALSE;
  pc->sc_yecepc = sc;
  pc->le_yecepc = ysLstEnq( pc->sc_yecepc->sups_yecesc, (dvoid*)pc );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)520, YSLSEV_DEBUG(2), (char*)0, YSLNONE);

  return pc;
}




STATICF void yeceDestroyProxyConsumer( yecepc *pc )
{
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)521, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&pc->self_yecepc), YSLEND );

  yoDispose((dvoid*)pc->self_yecepc); 
  yoRelease((dvoid*)pc->self_yecepc); 
  if( pc->supp_yecepc )
  {
    yoRelease( (dvoid*)pc->supp_yecepc ); 
    ysEvtDestroy( pc->sevt_yecepc );
  }
  if( pc->proxy_yecepc )
  {
    yoDispose( pc->proxy_yecepc ); 
    yoRelease( pc->proxy_yecepc ); 
  }
  ysmGlbFree( (dvoid*)pc );
}




STATICF sword yeceChCmp( CONST dvoid *a, CONST dvoid *b )
{
  return( yeuSafeStrcmp( (char*)a, (char *)b ) );
}




STATICF void yecePollSuppliers( yecesc *sc, yoenv *ev )
{
  yoany	data;
  yecepc *pc;
  ysle *noreg e;
  boolean has = FALSE;

  NOREG(e);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)522, YSLSEV_DEBUG(2), (char*)0, YSLNONE );

  for( e = ysLstHead( sc->sups_yecesc ); e ; e = ysLstNext( e ) )
  {
    pc = (yecepc*)ysLstVal(e);
    if( !pc->pull_yecepc )
      continue;

    yseTry
    {
      do
      {
	data =
	  yecec_PullSupp_try_pull( (yecec_PullSupp)pc->supp_yecepc, ev, &has );
	if( has )
	{
	  yeceShowAny((char*)"yecePollSuppliers: pushing", &data );
	  yeceCa_ProxyPushCons_push_i((yeceCa_ProxyPushCons)pc->self_yecepc,
				      ev, &data );
	}
      } while( has );
      yotkFreeVal( yoTcAny, (dvoid*)&data, yoFree );
    }
    yseCatchAll
    {
      
      
      ysRecord(YS_PRODUCT,YECE_FAC,(ub4)533, YSLSEV_WARNING, (char*)0,
	       YSLSTR(ysidToStr(yseExid)),
	       YSLANY(yoTcObject,&pc->supp_yecepc),
	       YSLEND );
    }
    yseEnd;
  }

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)523, YSLSEV_DEBUG(2), (char*)0, YSLNONE);
}

STATICF void yeceShowAny(char *msg, yoany *ap )
{
  ysstr	*anystr;
  
  anystr = yeevFormat( (yemsgcx*)0, (char*)0, (char*)0, (ub4)0, ap, TRUE );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)524, YSLSEV_DEBUG(3), (char*)0,
	   YSLSTR(msg),
	   YSLSTR(ysStrToText(anystr)), YSLEND );
  
  ysStrDestroy( anystr );
}


STATICF void yecePCDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			 size_t argsz )
{
  yecepc *pc = (yecepc*)usrp;

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)546, YSLSEV_WARNING, (char*)0,
	   YSLANY(yoTcObject, (dvoid**)&pc->supp_yecepc), YSLEND );

  DISCARD ysLstRem( pc->sc_yecepc->sups_yecesc, pc->le_yecepc );

  yeceDestroyProxyConsumer( pc );
}


STATICF void yecePDeath( dvoid *usrp, CONST ysid *exid, dvoid *arg,
			size_t argsz )
{
  yecep *p = (yecep*)usrp;

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)547, YSLSEV_WARNING, (char*)0,
	   YSLANY(yoTcObject, (dvoid**)&p->dest_yecep), YSLEND );

  DISCARD ysLstRem( p->sc_yecep->cons_yecesc, p->le_yecep ); 
  yeceDestroyParty( p );
}
















void yeceCa_ProxyPushCons_push_i( yeceCa_ProxyPushCons or, yoenv* ev,
				 yoany* data)
{
  yecepc    *pc = (yecepc*)yoGetState((dvoid*)or);
  yecesc    *sc = pc->sc_yecepc;
  yecech    *ch = sc->ch_yecesc;
  ysle	    *e = (ysle*)0;	
  yecep	    *p;			
  yoany	    *qd;

  

  yeceShowAny((char*)"yeceCa_ProxyPushCons_push_i:", data );

  ch->nevts_yecech++;
  sc->nevts_yecesc++;
  for( e = ysLstHead( sc->cons_yecesc ); e ; e = ysLstNext(e) )
  {
    p = (yecep*)ysLstVal(e);

    if( p->dead_yecep )
      continue;	

    if( p->q_yecep )		
    {
      qd = (yoany*)ysmGlbAlloc(sizeof(*qd), "queued any");
      yotkCopyVal( yoTcAny, (dvoid*)qd, (dvoid*)data, (ysmaf)0 );

      
      ysRecord(YS_PRODUCT,YECE_FAC,(ub4)525, YSLSEV_DEBUG(2), (char*)0,
	       YSLNONE);
    
      DISCARD ysLstEnq( p->q_yecep, (dvoid*)qd );
    }
    else			
    {
      p->ucnt_yecep++;
      yecec_PushCons_push_nw( (yecec_PushCons)p->dest_yecep,
			     &p->ev_yecep, data,
			     ysEvtCreate( yeceSendComplete, (dvoid*)p,
					 ch->cx_yecech->q_yececx, FALSE ));
    }
  }
}


void yeceCa_ProxyPushCons_disconnect_i( yeceCa_ProxyPushCons or, yoenv* ev)
{
  yecepc    *pc = (yecepc*)yoGetState((dvoid*)or);
  
  if( !pc->supp_yecepc )
    yseThrow( YS_EX_BADPARAM );
  if( !pc->connected_yecepc )
    yseThrow(YECEC_EX_DISCONNECTED);

  DISCARD ysLstRem( pc->sc_yecepc->sups_yecesc, pc->le_yecepc );
  yeceDestroyProxyConsumer( pc );
}







void yeceCa_ProxyPushCons_connect_push_supp_i( yeceCa_ProxyPushCons or,
					      yoenv* ev,
					      yecec_PushSupp push_supp)
{
  yecepc *pc = (yecepc*)yoGetState((dvoid*)or );
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  if( pc->connected_yecepc )
    yseThrow( YECECA_EX_ALREADYCONNECTED );

  pc->supp_yecepc = (CORBA_Object)yoDuplicate(push_supp); 
  pc->connected_yecepc = TRUE;

  if( push_supp )
  {
    pc->sevt_yecepc = ysEvtCreate(yecePCDeath, (dvoid*)pc, cx->q_yececx, TRUE);
    yoWatchOwner( push_supp, pc->sevt_yecepc );
  }
}





yoany yeceCa_ProxyPullSupp_pull_i( yeceCa_ProxyPullSupp or, yoenv* ev)
{
  

  yecep *p = (yecep*)yoGetState((dvoid*)or);
  yoany	*data;
  yoany rv;
  
  if( !p->q_yecep )
    yseThrow( YS_EX_BADPARAM ); 

  
  yecePollSuppliers( p->sc_yecep, ev );

  if( !ysLstCount( p->q_yecep ) )
    yseThrow( YECEC_EX_NOTHINGTOPULL );

  data = (yoany*)ysLstDeq( p->q_yecep );
  yotkCopyVal( yoTcAny, (dvoid*)&rv, (dvoid*)data, yoAlloc );
  yeceDestroyAny((dvoid*)data );
  return rv;
}



yoany yeceCa_ProxyPullSupp_try_pull_i( yeceCa_ProxyPullSupp or, yoenv* ev,
				      boolean* has_event)
{
  yecep *p = (yecep*)yoGetState((dvoid*)or);
  yoany	*data;
  yoany rv;
  
  if( !p->q_yecep )
    yseThrow( YS_EX_BADPARAM ); 

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)526, YSLSEV_DEBUG(2), (char*)0, YSLNONE );

  
  yecePollSuppliers( p->sc_yecep, ev );

  if( ysLstCount( p->q_yecep ) )
  {
    *has_event = TRUE;
    data = (yoany*)ysLstDeq( p->q_yecep );
    yotkCopyVal( yoTcAny, (dvoid*)&rv, (dvoid*)data, yoAlloc );
    yeceDestroyAny((dvoid*)data );
  }
  else
  {
    *has_event = FALSE;
    rv._type = (yotk*)yoTcNull;
    rv._value = (dvoid*)0;
  }

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)527, YSLSEV_DEBUG(2), (char*)0,
	   YSLSTR( *has_event ? "TRUE" : "FALSE"), YSLEND );

  yeceShowAny((char*)"yeceCa_ProxyPullSupp_try_pull_i: returning", &rv );
  return rv;
}




void yeceCa_ProxyPullSupp_disconnect_i( yeceCa_ProxyPullSupp or, yoenv* ev)
{
  yecep  *p = (yecep*)yoGetState((dvoid*)or);
  
  

  DISCARD ysLstRem( p->sc_yecep->cons_yecesc, p->le_yecep ); 
  yeceDestroyParty( p );
}


void yeceCa_ProxyPullSupp_connect_pull_cons_i( yeceCa_ProxyPullSupp or,
					      yoenv* ev,
					      yecec_PullCons pull_cons)
{
  yecep *p = (yecep*)yoGetState((dvoid*)or);
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  if( !p->q_yecep || !pull_cons )
    yseThrow( YS_EX_BADPARAM ); 

  if( p->connected_yecep )
    yseThrow( YECECA_EX_ALREADYCONNECTED );
    
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)528, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&pull_cons), YSLNONE );

  p->connected_yecep = TRUE;
  p->dest_yecep = (CORBA_Object)yoDuplicate((dvoid*)pull_cons);
  p->devt_yecep = ysEvtCreate( yecePDeath, (dvoid*)p, cx->q_yececx, TRUE );
  yoWatchOwner( pull_cons, p->devt_yecep );

  yoEnvInit(&p->ev_yecep);
  p->sc_yecep = p->sc_yecep;
  p->q_yecep = ysLstCreate();
}







void yeceCa_ProxyPullCons_disconnect_i( yeceCa_ProxyPullCons or, yoenv* ev)
{
  yecepc    *pc = (yecepc*)yoGetState((dvoid*)or);
  
  DISCARD ysLstRem( pc->sc_yecepc->sups_yecesc, pc->le_yecepc );
  yeceDestroyProxyConsumer( pc );

  
}




void yeceCa_ProxyPullCons_connect_pull_supp_i( yeceCa_ProxyPullCons or,
					      yoenv* ev,
					      yecec_PullSupp pull_supp)
{
  yecepc *pc = (yecepc*)yoGetState((dvoid*)or);
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  if( !pull_supp || !pc->pull_yecepc )
    yseThrow( YS_EX_BADPARAM ); 

  if( pc->connected_yecepc )
    yseThrow( YECECA_EX_ALREADYCONNECTED );

  pc->supp_yecepc = (CORBA_Object)yoDuplicate(pull_supp); 
  pc->connected_yecepc = TRUE;
  pc->sevt_yecepc = ysEvtCreate( yecePCDeath, (dvoid*)pc, cx->q_yececx, TRUE);
  yoWatchOwner( pull_supp, pc->sevt_yecepc );
}







void yeceCa_ProxyPushSupp_disconnect_push_supp_i( yeceCa_ProxyPushSupp or,
						 yoenv* ev)
{
  yecep *p = (yecep*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)529, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or), YSLNONE );
  
  

  DISCARD ysLstRem( p->sc_yecep->cons_yecesc, p->le_yecep ); 
  yeceDestroyParty( p );
}


void yeceCa_ProxyPushSupp_connect_push_cons_i( yeceCa_ProxyPushSupp or,
					      yoenv* ev,
					      yecec_PushCons push_cons)
{
  yecep *p = (yecep*)yoGetState((dvoid*)or);
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)530, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&push_cons), YSLEND );

  if( p->connected_yecep )
    yseThrow( YECECA_EX_ALREADYCONNECTED );

  if( !push_cons )
    yseThrow( YS_EX_BADPARAM ); 

  p->connected_yecep = TRUE;
  p->dest_yecep = yoDuplicate( (dvoid*)push_cons );
  p->devt_yecep = ysEvtCreate( yecePDeath, (dvoid*)p, cx->q_yececx, TRUE );
  yoWatchOwner( push_cons, p->devt_yecep );
}







yeceCa_ProxyPushSupp yeceCa_ConsAdm_obtain_push_supp_i( yeceCa_ConsAdm or,
						       yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, (char*)0 );
  yecep *p = yeceNewParty( sc );

  p->self_yecep = (CORBA_Object)yoCreate(yeceCa_ProxyPushSupp__id,
				   (char*)0, (yoRefData*)0,
				   (char*)0, (dvoid*)p );
  return( (yeceCa_ProxyPushSupp)yoDuplicate((dvoid*)p->self_yecep) );
}


yeceCa_ProxyPullSupp yeceCa_ConsAdm_obtain_pull_supp_i( yeceCa_ConsAdm or,
						       yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, (char*)0 );
  yecep *p = yeceNewParty( sc );

  p->q_yecep = ysLstCreate();
  p->self_yecep = (CORBA_Object)yoCreate(yeceCa_ProxyPullSupp__id,
				   (char*)0, (yoRefData*)0,
				   (char*)0, (dvoid*)p );
  return( (yeceCa_ProxyPullSupp)yoDuplicate((dvoid*)p->self_yecep) );
}







yeceCa_ProxyPushCons yeceCa_SuppAdm_obtain_push_cons_i( yeceCa_SuppAdm or,
						       yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, (char*)0 );
  yecepc *pc = yeceNewProxyConsumer(sc); 

  pc->self_yecepc = (CORBA_Object)yoCreate( yeceCa_ProxyPushCons__id,
				      (char*)0, (yoRefData*)0,
				      (char*)0, (dvoid*)pc );
  return( (yeceCa_ProxyPushCons)yoDuplicate((dvoid*)pc->self_yecepc) );
}


yeceCa_ProxyPullCons yeceCa_SuppAdm_obtain_pull_cons_i( yeceCa_SuppAdm or,
						       yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, (char*)0 );
  yecepc *pc = yeceNewProxyConsumer(sc); 

  pc->pull_yecepc = TRUE;
  pc->self_yecepc = (CORBA_Object)yoCreate( yeceCa_ProxyPullCons__id,
				      (char*)0, (yoRefData*)0,
				      (char*)0, (dvoid*)pc );
  return( (yeceCa_ProxyPullCons)yoDuplicate((dvoid*)pc->self_yecepc) );
}





yeceCa_ConsAdm yeceCa_EventChannel_for_consumers_i( yeceCa_EventChannel or,
						   yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  return( (yeceCa_ConsAdm)yoDuplicate((dvoid*)ch->cself_yecech) );
}


yeceCa_SuppAdm yeceCa_EventChannel_for_suppliers_i( yeceCa_EventChannel or,
						   yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);

  return( (yeceCa_SuppAdm)yoDuplicate((dvoid*)ch->sself_yecech) );
}


void yeceCa_EventChannel_destroy_i( yeceCa_EventChannel or, yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)531, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or), YSLEND );

  DISCARD ysLstRem( ch->cx_yecech->ch_yececx, ch->le_yecech );
  yeceDestroyChannel( ch );
}





yeceCa_EventChannel yeceCa_Factory_create_i( yeceCa_Factory or, yoenv* ev)
{
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);
  yecech *ch = (yecech*)0; 

  ch = yeceNewChannel( cx, FALSE );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)532, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,ch->typed_yecech ?
		  (dvoid*)&ch->tself_yecech : (dvoid*)&ch->self_yecech),
	   YSLEND);

  return( (yeceCa_EventChannel)yoDuplicate((dvoid*)ch->self_yecech) );
}



yeceCa_eventChannelList
yeceCa_Factory__get_channels_i( yeceCa_Factory or, yoenv* ev)
{
  yeceCa_eventChannelList olist;

  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);
  yecech *ch;
  ysle	*e;
  
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)534, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or), YSLEND );

  olist._maximum = ysLstCount( cx->ch_yececx );
  olist._length = 0;
  olist._buffer = (yeceCa_EventChannel*)yoAlloc(olist._maximum *
						sizeof(yeceCa_EventChannel));
  for( e = ysLstHead(cx->ch_yececx) ; e ; e = ysLstNext( e ) )
  {
    ch = (yecech*)ysLstVal(e);
    if( !ch->typed_yecech ) 
      olist._buffer[olist._length++] =
	(yeceCa_EventChannel)yoDuplicate( (dvoid*)ch->self_yecech);
  }
  return( olist );
}


yeceCa_Factory yeceCa_Factory__get_self_i( yeceCa_Factory or, yoenv* ev)
{
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)535, YSLSEV_DEBUG(2), (char*)0,
	   "yeceCa_Factory__get_self: or %s, returning %s\n",
	   YSLANY(yoTcObject,&or),
	   YSLANY(yoTcObject,&cx->self_yececx), YSLEND );

  return( (yeceCa_Factory)yoDuplicate((dvoid*)cx->self_yececx) );
}








void yeceTeca_TypedProxyPushCons_push_i( yeceTeca_TypedProxyPushCons or,
					yoenv* ev, yoany* data)
{
  yeceCa_ProxyPushCons_push_i( (yeceCa_ProxyPushCons)or, ev, data );
}

void yeceTeca_TypedProxyPushCons_disconnect_i( yeceTeca_TypedProxyPushCons or,
					      yoenv* ev)
{
  yeceCa_ProxyPushCons_disconnect_i( (yeceCa_ProxyPushCons)or, ev );
}

void
yeceTeca_TypedProxyPushCons_connect_push_supp_i(yeceTeca_TypedProxyPushCons or,
						yoenv* ev,
						yecec_PushSupp push_supp)
{
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)536, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or),
	   YSLANY(yoTcObject,&push_supp), YSLEND );

  yeceCa_ProxyPushCons_connect_push_supp_i( (yeceCa_ProxyPushCons)or,
					   ev, push_supp );
}


CORBA_Object
yeceTeca_TypedProxyPushCons_get_typed_cons_i( yeceTeca_TypedProxyPushCons or,
					     yoenv* ev)
{
  yecepc *pc = (yecepc*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)537, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or),
	   YSLSTR(yeuStr((char*)pc->sc_yecepc->node_yecesc.key_ysspNode)),
	   YSLEND);

  

  pc->proxy_yecepc = yoCreate( (char*)pc->sc_yecepc->node_yecesc.key_ysspNode,
			      (char*)0, (yoRefData*)0, (char*)0, (dvoid*)pc );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)538, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&pc->proxy_yecepc), YSLEND);

  return( (CORBA_Object)yoDuplicate((dvoid*)pc->proxy_yecepc) );
}





yoany yeceTeca_TypedProxyPullSupp_pull_i( yeceTeca_TypedProxyPullSupp or,
					 yoenv* ev)
{
  yoany rv;
  CLRSTRUCT(rv);

  yseThrow( YECE_EX_UNIMPL );
  return(rv);
}


yoany yeceTeca_TypedProxyPullSupp_try_pull_i( yeceTeca_TypedProxyPullSupp or,
					     yoenv* ev, boolean* has_event)
{
  yoany rv;
  CLRSTRUCT(rv);

  yseThrow( YECE_EX_UNIMPL );
  return(rv);
}


void yeceTeca_TypedProxyPullSupp_disconnect_i(yeceTeca_TypedProxyPullSupp or,
					      yoenv* ev)
{
  yseThrow( YECE_EX_UNIMPL );
}


void
yeceTeca_TypedProxyPullSupp_connect_pull_cons_i(yeceTeca_TypedProxyPullSupp or,
						yoenv* ev,
						yecec_PullCons pull_cons)
{
  yseThrow( YECE_EX_UNIMPL );
}


CORBA_Object
yeceTeca_TypedProxyPullSupp_get_typed_supp_i( yeceTeca_TypedProxyPullSupp or,
					     yoenv* ev)
{
  yseThrow( YECE_EX_UNIMPL );
  return( (CORBA_Object)0 );
}




yeceCa_ProxyPushCons
yeceTeca_TypedSuppAdm_obtain_push_cons_i( yeceTeca_TypedSuppAdm or, yoenv* ev)
{
  return( yeceCa_SuppAdm_obtain_push_cons_i( (yeceCa_SuppAdm)or, ev ) );
}

yeceCa_ProxyPullCons
yeceTeca_TypedSuppAdm_obtain_pull_cons_i( yeceTeca_TypedSuppAdm or, yoenv* ev)
{
  return( yeceCa_SuppAdm_obtain_pull_cons_i( (yeceCa_SuppAdm)or, ev ) );
}


yeceTeca_TypedProxyPushCons
yeceTeca_TypedSuppAdm_obtain_typed_push_cons_i( yeceTeca_TypedSuppAdm or,
					       yoenv* ev,
					       char* supported_interface)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, supported_interface );
  yecepc *pc = yeceNewProxyConsumer(sc); 

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)539, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or), YSLSTR(supported_interface), YSLEND );

  pc->self_yecepc = (CORBA_Object)yoCreate( yeceTeca_TypedProxyPushCons__id,
				      (char*)0, (yoRefData*)0,
				      (char*)0, (dvoid*)pc );
  return( (yeceTeca_TypedProxyPushCons)yoDuplicate((dvoid*)pc->self_yecepc ));
}


yeceCa_ProxyPullCons
yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_i( yeceTeca_TypedSuppAdm or,
					       yoenv* ev,
					       char* uses_interface)
{
  yseThrow( YECE_EX_UNIMPL );
  return( (yeceCa_ProxyPullCons)0 );
}



	
yeceCa_ProxyPushSupp
yeceTeca_TypedConsAdm_obtain_push_supp_i( yeceTeca_TypedConsAdm or, yoenv* ev)
{
  return( yeceCa_ConsAdm_obtain_push_supp_i( (yeceCa_ConsAdm)or, ev ) );
}

yeceCa_ProxyPullSupp
yeceTeca_TypedConsAdm_obtain_pull_supp_i( yeceTeca_TypedConsAdm or, yoenv* ev)
{
  return( yeceCa_ConsAdm_obtain_pull_supp_i( (yeceCa_ConsAdm)or, ev ) );
}


yeceTeca_TypedProxyPullSupp
yeceTeca_TypedConsAdm_obtain_typed_pull_supp_i( yeceTeca_TypedConsAdm or,
					       yoenv* ev,
					       char* supported_interface)
{
  yseThrow( YECE_EX_UNIMPL );
  return((yeceTeca_TypedProxyPullSupp)0);
}


yeceCa_ProxyPushSupp
yeceTeca_TypedConsAdm_obtain_typed_push_supp_i( yeceTeca_TypedConsAdm or,
					       yoenv* ev,
					       char* uses_interface)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);
  yecesc *sc = yeceGetSubChannel( ch, uses_interface );
  yecep *p = yeceNewParty( sc );

  p->self_yecep = (CORBA_Object)yoCreate(yeceCa_ProxyPushSupp__id,
				   (char*)0, (yoRefData*)0,
				   (char*)0, (dvoid*)p );
  return( (yeceCa_ProxyPushSupp)yoDuplicate((dvoid*)p->self_yecep) );
}



yeceTeca_TypedConsAdm
yeceTeca_TypedEventChannel_for_consumers_i( yeceTeca_TypedEventChannel or,
					   yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)540, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&ch->tcself_yecech), YSLEND );

  return( (yeceTeca_TypedConsAdm)yoDuplicate((dvoid*)ch->tcself_yecech) );
}


yeceTeca_TypedSuppAdm
yeceTeca_TypedEventChannel_for_suppliers_i( yeceTeca_TypedEventChannel or,
					   yoenv* ev)
{
  yecech *ch = (yecech*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)541, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&ch->tsself_yecech), YSLEND );

  return( (yeceTeca_TypedSuppAdm)yoDuplicate((dvoid*)ch->tsself_yecech ) );
}






yeceTeca_TypedEventChannel
yeceTeca_Factory_create_i( yeceTeca_Factory or, yoenv* ev)
{
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);
  yecech *ch = (yecech*)0; 
 
  ch = yeceNewChannel( cx, TRUE );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)542, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&ch->tself_yecech), YSLEND );

  return( (yeceTeca_TypedEventChannel)yoDuplicate((dvoid*)ch->tself_yecech ));
}


yeceTeca_typedEventChannelList
yeceTeca_Factory__get_channels_i( yeceTeca_Factory or, yoenv* ev)
{
  yeceTeca_typedEventChannelList olist;

  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);
  yecech *ch;
  ysle	*e;
  
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)544, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or), YSLEND);

  olist._maximum = ysLstCount( cx->ch_yececx );
  olist._length = 0;
  olist._buffer =
    (yeceTeca_TypedEventChannel*)yoAlloc(olist._maximum *
					 sizeof(yeceCa_EventChannel));
  for( e = ysLstHead(cx->ch_yececx) ; e ; e = ysLstNext( e ) )
  {
    ch = (yecech*)ysLstVal(e);
    if( ch->typed_yecech ) 
      olist._buffer[olist._length++] =
	(yeceTeca_TypedEventChannel)yoDuplicate( ch->tself_yecech );
  }
  return( olist );
}


yeceTeca_Factory yeceTeca_Factory__get_self_i( yeceTeca_Factory or, yoenv* ev)
{
  yececx *cx = (yececx*)yoGetImplState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)545, YSLSEV_DEBUG(2), (char*)0,
	   YSLANY(yoTcObject,&or),
	   YSLANY(yoTcObject,&cx->tself_yececx), YSLEND);

  return( (yeceTeca_Factory)yoDuplicate((dvoid*)cx->tself_yececx) );
}



