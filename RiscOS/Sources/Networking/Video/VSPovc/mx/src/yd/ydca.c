/* mx/src/yd/ydca.c */


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
#ifndef YDCA_ORACLE
#include <ydca.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YOTK_ORACLE
#include <yotk.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSLOG_ORACLE
#include <yslog.h>
#endif




#define YDCA_FAC    "YDCA"

externdef ysidDecl( YDCA_EX_BADOP ) = "ydca::badOp"; 
externdef ysidDecl( YDCA_EX_INTERNAL ) = "ydca::internal";

externdef ysmtagDecl(ydcacx_tag) = "ydcacx";
externdef ysmtagDecl(ydcap_tag) = "ydcap";
externdef ysmtagDecl(ydcar_tag) = "ydcar";


typedef struct
{
  ysspNode	node_ydcap;	
  yort_claim	claim_ydcap;
  enum { try_ydcap, granted_ydcap } state_ydcap;
  ysevt		*evt_ydcap;	
  ydcacx	*cx_ydcap;	
} ydcap;


typedef enum
{
  tryStart_ydcar,		
  try_ydcar,			
  commit_ydcar,			
  abandonStart_ydcar,		
  abandon_ydcar,		
  transferStart_ydcar,		
  transfer_ydcar,		
  exit_ydcar			
} optype_ydcar;


typedef struct
{
  optype_ydcar	op_ydcar;	
  optype_ydcar	sop_ydcar;	
  yort_claim	claim_ydcar;	
  boolean	retry_ydcar;	
  yslst	    *orbds_ydcar;	
  ysle	    *e_ydcar;		
  ysle	    *lte_ydcar;		
  yoevt	    reply_ydcar;	
  yoenv	    ev_ydcar;		
  ysle	    *le_ydcar;		
  ydcacx    *cx_ydcar;
  ysevt	    *evt_ydcar;		
    
} ydcar ;



struct ydcacx
{
  ysspTree  t_ydcacx;		
  yslst	    *l_ydcacx;		
  ysque	    *q_ydcacx;		
  ysevt	    *ievt_ydcacx;	
};



STATICF CONST char *ydcaPropState( ydcap *cap );
STATICF CONST char *ydcaOper( optype_ydcar op );
STATICF CONST char *ydcaTryResult( ydim_tryResult try );

STATICF void ydcaStartMachine( ydcacx *cx, optype_ydcar op,
			      yort_claim *claim, yoevt reply );

STATICF void ydcaKillMachine( dvoid *x );

STATICF void ydcaMachine( dvoid *usrp, CONST ysid *exid,
			 dvoid *arg, size_t argsz );

STATICF void ydcaDeath( dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz );



ydcacx *ydcaInit( ysque *q )
{
  ydcacx *cx = (ydcacx*)ysmGlbAlloc( sizeof( *cx ), ydcacx_tag );

  DISCARD ysspNewTree( &cx->t_ydcacx, (ysspCmpFunc)strcmp );
  cx->l_ydcacx = ysLstCreate();
  cx->q_ydcacx = q;
  cx->ievt_ydcacx = (ysevt*)0;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1202, YSLSEV_INFO, (char*)0, YSLNONE);

  return( cx );
}

void ydcaTerm( ydcacx *cx )
{
  ydcap *cap;
  ysspNode *n;

  
  ysLstDestroy( cx->l_ydcacx, ydcaKillMachine );

  
  while( (n = ysspDeqTree( &cx->t_ydcacx ) ) )
  {
    cap = (ydcap*)n;
    yort_claim__free( &cap->claim_ydcap, yotkFreeStr );

    
    ysEvtDestroy( cap->evt_ydcap );

    ysmGlbFree( (dvoid*)n );
  }
  ysmGlbFree( (dvoid*)cx );

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1203, YSLSEV_INFO, (char*)0, YSLNONE);

}

void ydcaSetInactiveEvt( ydcacx *cx, ysevt *evt )
{
  if( !ysLstCount( cx->l_ydcacx ) )
    ysTrigger( evt, (ysid*)0, (dvoid*)0, (size_t)0 );
  else
  {
    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1201, YSLSEV_INFO, (char*)0,
	     YSLSB4( ysLstCount( cx->l_ydcacx )), YSLEND);
    cx->ievt_ydcacx = evt;
  }
}





void ydcaStakeFor( ydcacx *cx, yort_claim *claim, yoevt reply )
{
  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1204, YSLSEV_INFO, (char*)0,
	     YSLSTR( claim->property ), YSLEND);

  ydcaStartMachine( cx, tryStart_ydcar, claim, reply ); 
}



void ydcaAbandonFor( ydcacx *cx, yort_claim *claim, boolean proxy )
{
  ysspNode *n;
  ydcap *cap;
  ydyo_notOwner exv;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1205, YSLSEV_INFO, (char*)0,
	     YSLSTR( claim->property ), YSLEND);

  if( !proxy && (n = ysspLookup( (dvoid*)claim->property, &cx->t_ydcacx ) ) )
  {
    cap = (ydcap*)n;
    if( !yoIsEq( (dvoid*)cap->claim_ydcap.owner, (dvoid*)claim->owner ) )
    {
      exv.realOwner = (yort_proc)yoDuplicate((dvoid*)cap->claim_ydcap.owner);
      yseThrowObj( YDYO_EX_NOTOWNER, exv );
    }
  }

  ydcaStartMachine( cx, abandonStart_ydcar, claim, (yoevt)0 );
}


void ydcaTransfer( ydcacx *cx, yort_claim* newClaim)
{
  ysspNode *n;
  ydcap *cap;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1206, YSLSEV_INFO, (char*)0,
	     YSLSTR( newClaim->property ), YSLEND);

  if( (n = ysspLookup( (dvoid*)newClaim->property, &cx->t_ydcacx ) ) )
  {
    cap = (ydcap*)n;
    
    if(!yoIsEq( (dvoid*)cap->claim_ydcap.obj, (dvoid*)newClaim->obj ) ||
       !yoIsEq( (dvoid*)cap->claim_ydcap.owner, (dvoid*)newClaim->owner ))
      ydcaStartMachine( cx, transferStart_ydcar, newClaim, (yoevt)0);
  }
}






ydim_tryResult ydcaTryStake( ydcacx *cx, yort_claim *what )
{
  ysspNode *n;
  ydcap *cap;
  char *r1, *r2;
  ydim_tryResult rv;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1207, YSLSEV_INFO, (char*)0,
	     YSLSTR( what->property ), YSLEND);

  if( !(n = ysspLookup( (dvoid*)what->property, &cx->t_ydcacx ) ) )
  {
    

    cap = (ydcap*)ysmGlbAlloc( sizeof( *cap ), ydcap_tag );
    yort_claim__copy( &cap->claim_ydcap, what, yotkAllocStr );
    cap->state_ydcap = try_ydcap;
    cap->cx_ydcap = cx;
    cap->node_ydcap.key_ysspNode = (dvoid*)cap->claim_ydcap.property;
    DISCARD ysspEnq( &cap->node_ydcap, &cx->t_ydcacx );

    
    cap->evt_ydcap = ysEvtCreate( ydcaDeath, (dvoid*)cap, cx->q_ydcacx, TRUE );
    yoWatchOwner( (dvoid*)what->owner, cap->evt_ydcap );
    rv = ydim_success_tryResult;
  }
  else				
  {
    cap = (ydcap*)n;
    rv = ydim_fail_tryResult;

    
    if( cap->state_ydcap == try_ydcap )
    {
      
      r1 = yoRefToStr( (dvoid*)what->owner );
      r2 = yoRefToStr( (dvoid*)cap->claim_ydcap.owner );
      if( strcmp( r1, r2 ) < 0 )
	rv = ydim_retry_tryResult;
      yoFree( (dvoid*)r1 );
      yoFree( (dvoid*)r2 );
    }
  }
  return rv;
}

ydim_tryResult ydcaTransferStake( ydcacx *cx, yort_claim *what )
{
  ydim_tryResult rv = ydim_fail_tryResult;
  ydcap *cap;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1208, YSLSEV_INFO, (char*)0,
	     YSLSTR( what->property ), YSLEND);

  
  if( (cap = (ydcap*)ysspLookup( (dvoid*)what->property, &cx->t_ydcacx ) ) )
  {
    
    if( cap->state_ydcap == granted_ydcap )
    {
      
      ysEvtDestroy( cap->evt_ydcap );

      
      yoRelease( (dvoid*)cap->claim_ydcap.owner );
      yoRelease( (dvoid*)cap->claim_ydcap.obj );

      
      cap->claim_ydcap.owner = (yort_proc)yoDuplicate((dvoid*)what->owner); 
      cap->claim_ydcap.obj = (CORBA_Object)yoDuplicate((dvoid*)what->obj); 
      cap->state_ydcap = try_ydcap;
      cap->evt_ydcap = ysEvtCreate( ydcaDeath, (dvoid*)cap,
				   cx->q_ydcacx, TRUE );
      yoWatchOwner( (dvoid*)cap->claim_ydcap.owner, cap->evt_ydcap );
      rv = ydim_success_tryResult;
    }
  }
  return( rv );
}



void ydcaAbortStake( ydcacx *cx, yort_claim *what )
{
  ysspNode *n;
  ydcap *cap;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1209, YSLSEV_INFO, (char*)0,
	     YSLSTR( what->property ), YSLEND);

  if( (n = ysspLookup( (dvoid*)what->property, &cx->t_ydcacx ) ) )
  {
    cap = (ydcap*)n;
    ysEvtDestroy( cap->evt_ydcap );

    ysspRemove( n, &cx->t_ydcacx );
    yort_claim__free( &cap->claim_ydcap, yotkFreeStr );
    ysmGlbFree( (dvoid*)n );
  }
}


void ydcaCommitStake( ydcacx *cx, yort_claim *what )
{
  ysspNode *n;
  ydcap *cap;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1210, YSLSEV_INFO, (char*)0,
	     YSLSTR( what->property ), YSLEND);

  if( (n = ysspLookup( (dvoid*)what->property, &cx->t_ydcacx ) ) )
  {
    cap = (ydcap*)n;
    if( cap->state_ydcap == try_ydcap )
    {
      cap->state_ydcap = granted_ydcap;
    }
    else
    {
      
      ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1220, YSLSEV_ERR, (char*)0,
	       YSLSB4( (sb4)cap->state_ydcap ), YSLEND);
      ysePanic( YDCA_EX_INTERNAL );
    }
  }
  else
  {
    ;
     
  }
}

yort_claim *ydcaListNext( ydcacx *cx, yort_claim *this )
{
  ysspNode *n;
  ydcap *cap;

  if( this ) n = ysspNextLookup( (dvoid*)this->property, &cx->t_ydcacx );
  else n = ysspFHead( &cx->t_ydcacx );

  
  
  while( (cap = (ydcap*)n) && cap->state_ydcap != granted_ydcap )
    n = ysspFNext( n );

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1211, YSLSEV_INFO, (char*)0,
	   YSLSTR( this ? this->property : "<null>" ),
	   YSLSTR( cap ? cap->claim_ydcap.property : "<null>"),
	   YSLEND);

  return( cap ? &cap->claim_ydcap : (yort_claim*)0 );
}






STATICF CONST char *ydcaPropState( ydcap *cap )
{
  CONST char *rv;
  switch( cap->state_ydcap )
  {
  case try_ydcap:   rv = "try"; break;
  case granted_ydcap:   rv = "granted"; break;
  default: rv = "unknown"; break;
  }
  return rv;
}

STATICF CONST char *ydcaOper( optype_ydcar op )
{
  CONST char *rv;
  switch( op )
  {
  case tryStart_ydcar:	    rv = "tryStart"; break;
  case try_ydcar:	    rv = "try"; break;
  case commit_ydcar:	    rv = "commit"; break;
  case abandonStart_ydcar:  rv = "abandonStart"; break;
  case abandon_ydcar:	    rv = "abandon"; break;
  case transferStart_ydcar: rv = "transferStart"; break;
  case transfer_ydcar:	    rv = "transfer"; break;
  case exit_ydcar:	    rv = "exit"; break;
  default: rv = "unknown"; break;
  }
  return rv;
}

STATICF CONST char *ydcaTryResult( ydim_tryResult try )
{
  CONST char *rv;
  switch( try )
  {
  case ydim_success_tryResult:	    rv = "success"; break;
  case ydim_fail_tryResult:	    rv = "fail"; break;
  case ydim_retry_tryResult:	    rv = "retry"; break;
  default: rv = "unknown"; break;
  }
  return rv;
}




STATICF void ydcaStartMachine( ydcacx *cx, optype_ydcar op,
			      yort_claim *claim, yoevt reply )
{
  ydcar *car;

  if( commit_ydcar == op )
  {
    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1212, YSLSEV_ERR, (char*)0,
	     YSLSTR( ydcaOper( op )), YSLEND);
    ysePanic( YDCA_EX_BADOP );
  }

  car = (ydcar*)ysmGlbAlloc( sizeof(*car), ydcar_tag );
  car->cx_ydcar = cx;
  car->sop_ydcar = car->op_ydcar = op;
  car->orbds_ydcar = (yslst*)0;
  car->e_ydcar = car->lte_ydcar = (ysle*)0;
  car->retry_ydcar = FALSE;
  yort_claim__copy( &car->claim_ydcar, claim, yotkAllocStr );
  car->reply_ydcar = (yoevt)yoDuplicate((dvoid*)reply);
  yoEnvInit( &car->ev_ydcar );
  car->le_ydcar = ysLstEnq( cx->l_ydcacx, (dvoid*)car );
  car->evt_ydcar = ysSemCreate((dvoid*)0);
  
  
  ydcaMachine( (dvoid*)car, (ysid*)0, (dvoid*)0, (size_t)0 );
}





STATICF void ydcaKillMachine( dvoid *x )
{
  ydcar	*car = (ydcar*)x;
  yoany	any;
  boolean rv = FALSE;
    
  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1213, YSLSEV_INFO, (char*)0, YSLNONE );

  
  car->op_ydcar = exit_ydcar;
  ysEvtDestroy( car->evt_ydcar );
    
  
  if( car->reply_ydcar )
  {
    any._type = (yotk*)yoTcBoolean;
    any._value = (dvoid*)&rv;
    ysTrigger(yoToLocalEvt(car->reply_ydcar), (ysid*)0,
	      (dvoid*)&any, sizeof(any));
  }

  car->evt_ydcar = ysEvtCreate(ydcaMachine, (dvoid*)car,
			       car->cx_ydcar->q_ydcacx, TRUE);

  ysTrigger( car->evt_ydcar, (ysid*)0, (dvoid*)0, (size_t)0 );
}





STATICF void ydcaMachine( dvoid *usrp, CONST ysid *exid,
			 dvoid *arg, size_t argsz )
{
  ydcar *car = (ydcar*)usrp;
  ydim_tryResult try; 
  ysle	    *le;
  ysevt	    *evt;
  ydim_imr  imr;
  boolean   rv;
  yoany	    any;

  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1215, YSLSEV_INFO, (char*)0,
	   YSLSTR(ydcaOper(car->op_ydcar)), YSLSTR(yduStr(exid)), YSLEND );

 top:

  if( !car->orbds_ydcar )
  {
    car->orbds_ydcar = yoListORBDs();
    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1214, YSLSEV_INFO, (char*)0,
	     YSLSB4((sb4)ysLstCount(car->orbds_ydcar)),YSLSB4((sb4)1),YSLEND);
  }

  ysEvtDestroy( car->evt_ydcar );

  
  le = car->e_ydcar;
  car->e_ydcar = le ? ysLstNext( le ) : le;

  
  if( exid && car->op_ydcar != exit_ydcar ) 
  {
    
    ysLstDestroy( car->orbds_ydcar, yoRelease );
    car->orbds_ydcar = yoListORBDs();
    
    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1214, YSLSEV_INFO, (char*)0,
	     YSLSB4((sb4)ysLstCount(car->orbds_ydcar)),YSLSB4((sb4)2),YSLEND);

    car->retry_ydcar = FALSE;
    if( car->op_ydcar != transfer_ydcar )
    {
      
      car->retry_ydcar = TRUE;
      car->op_ydcar = abandon_ydcar;
    }
    car->e_ydcar = ysLstHead(car->orbds_ydcar);
  }

  switch( car->op_ydcar )	
  {
  case tryStart_ydcar:

    car->sop_ydcar = car->op_ydcar = try_ydcar;
    car->e_ydcar = ysLstHead( car->orbds_ydcar );
    break;

  case try_ydcar:

    try = *(ydim_tryResult*)arg;

    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1216, YSLSEV_INFO, (char*)0,
	     YSLSTR("try"),
	     YSLSTR( ydcaTryResult( try )), YSLEND);

    if( try == ydim_success_tryResult )
    {
      car->lte_ydcar = le;	
      if( arg && !car->e_ydcar ) 
      {
	
	car->op_ydcar = commit_ydcar;
	car->e_ydcar = ysLstHead( car->orbds_ydcar );
      }
    }
    else if( try == ydim_fail_tryResult )
    {
      
      car->op_ydcar = abandon_ydcar;
      if( car->lte_ydcar )
      {
	car->e_ydcar = ysLstHead( car->orbds_ydcar );
      }
      else			
      {
	car->e_ydcar = (ysle*)0;
      }
    }
    else if( try == ydim_retry_tryResult )
    {
      
      car->retry_ydcar = TRUE;
      car->op_ydcar = abandon_ydcar;
      car->e_ydcar = ysLstHead( car->orbds_ydcar );
    }
    break;

  case transferStart_ydcar:

    car->sop_ydcar = car->op_ydcar = transfer_ydcar;
    car->e_ydcar = ysLstHead( car->orbds_ydcar );
    break;

  case transfer_ydcar:

    try = *(ydim_tryResult*)arg;

    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1216, YSLSEV_INFO, (char*)0,
	     YSLSTR("transfer"),
	     YSLSTR( ydcaTryResult( try )), YSLEND);

    if( try == ydim_success_tryResult )
    {
      if( !car->e_ydcar )	
      {
	
	car->op_ydcar = commit_ydcar;
	car->e_ydcar = ysLstHead( car->orbds_ydcar );
      }
    }
    else			
    {
      
      ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1217, YSLSEV_INFO, (char*)0,
	       YSLSTR("transfer"),
	       YSLSTR( car->claim_ydcar.property ), YSLEND);

      
      car->op_ydcar = abandon_ydcar;
      car->e_ydcar = ysLstHead( car->orbds_ydcar );
    }
    break;

  case abandonStart_ydcar:
  
    car->op_ydcar = car->sop_ydcar = abandon_ydcar;
    car->e_ydcar = ysLstHead( car->orbds_ydcar );
    break;

  case abandon_ydcar:

    
    if( !car->e_ydcar || (car->lte_ydcar && le == car->lte_ydcar ))
    {
      
      if( car->retry_ydcar )
      {
	
	car->retry_ydcar = FALSE;
	if( car->sop_ydcar != abandon_ydcar )
	{
	  car->op_ydcar = car->sop_ydcar;
	  car->e_ydcar = ysLstHead( car->orbds_ydcar );
	  car->lte_ydcar = (ysle*)0;

	  
	}
      }
    }
    break;

  case commit_ydcar:		

    break;

  case exit_ydcar:

    
    if( car->orbds_ydcar )
      ysLstDestroy( car->orbds_ydcar, (ysmff)yoRelease );
    yort_claim__free( &car->claim_ydcar, (ysmff)yotkFreeStr );
    yoRelease( (dvoid*)car->reply_ydcar );
    yoEnvFree( &car->ev_ydcar );
    DISCARD ysLstRem( car->cx_ydcar->l_ydcacx, car->le_ydcar );

    
    if( !ysLstCount(car->cx_ydcar->l_ydcacx) && car->cx_ydcar->ievt_ydcacx )
    {
      evt = car->cx_ydcar->ievt_ydcacx;
      car->cx_ydcar->ievt_ydcacx = (ysevt*)0;
      ysTrigger( evt, (ysid*)0, (dvoid*)0, (size_t)0 );
    }

    ysmGlbFree( (dvoid*)car );

    
    return;

  default:

    
    ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1218, YSLSEV_ERR, (char*)0,
	     YSLSB4(car->op_ydcar), YSLSB4((sb4)1), YSLEND );
    ysePanic( YDCA_EX_INTERNAL );
    break;
  }

  
  car->evt_ydcar = ysEvtCreate(ydcaMachine, (dvoid*)car,
			       car->cx_ydcar->q_ydcacx, TRUE);
  if( car->e_ydcar )		
  {
    imr = (ydim_imr)ysLstVal( car->e_ydcar );
    switch( car->op_ydcar )
    {
    case try_ydcar:
      ydim_imr_tryStake_nw( imr, &car->ev_ydcar, &car->claim_ydcar,
			   car->evt_ydcar );
      break;
    case commit_ydcar:
      ydim_imr_commitStake_nw( imr, &car->ev_ydcar, &car->claim_ydcar,
			      car->evt_ydcar );
      break;
    case transfer_ydcar:
      ydim_imr_transferStake_nw( imr, &car->ev_ydcar, &car->claim_ydcar,
				car->evt_ydcar );
      break;
    case abandon_ydcar:
      ydim_imr_abortStake_nw( imr, &car->ev_ydcar, &car->claim_ydcar,
			     car->evt_ydcar );
      break;
    default:

      
      ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1218, YSLSEV_ERR, (char*)0,
	       YSLSB4(car->op_ydcar), YSLSB4((sb4)2), YSLEND );
      ysePanic( YDCA_EX_INTERNAL );
    }
  }
  else				
  {
    if( car->reply_ydcar )	
    {
      rv = car->op_ydcar == commit_ydcar;
      any._type = (yotk*)yoTcBoolean;
      any._value = (dvoid*)&rv;
      ysTrigger(yoToLocalEvt(car->reply_ydcar), (ysid*)0,
		(dvoid*)&any, sizeof(any));
    }
    car->op_ydcar = exit_ydcar;
    goto top;
  }
}






STATICF void ydcaDeath( dvoid *usrp, CONST ysid *exid,
		       dvoid *arg, size_t argsz )
{
  ydcap *cap = (ydcap*)usrp;
  
  
  ysRecord(YS_PRODUCT, YDCA_FAC, (ub4)1219, YSLSEV_WARNING, (char*)0,
	   YSLSTR(cap->claim_ydcap.property), YSLEND);

  ydcaAbandonFor( cap->cx_ydcap, &cap->claim_ydcap, TRUE );
}

