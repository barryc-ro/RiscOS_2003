/* mx/src/yd/ydch.c */


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
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDCH_ORACLE
#include <ydch.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YDIDLI_ORACLE
#include <ydidlI.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YSLST_ORACLE
#include <yslst.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif



#define YDCH_LOCK_PREFIX "::ydch::lock::"
#define YDCH_LOCK_REGEXP "^::ydch::lock::"

typedef struct ydchcx ydchcx;
typedef struct ydchoi ydchoi;

static CONST_W_PTR struct ydch_och__tyimpl ydch_och__impl =
 {
  ydch_och_whereActive_i,
  ydch_och_lock_i,
  ydch_och_setPlace_i,
  ydch_och_remove_i,
  ydch_och_listObjs_i
 };

static CONST_W_PTR struct ydch_objIterator__tyimpl ydch_objIterator__impl =
 {
  ydch_objIterator_next_one_i,
  ydch_objIterator_next_n_i,
  ydch_objIterator_destroy_i
 };



struct ydchoi
{
  ydch_objIterator self_ydchoi;
  ysle	*le_ydchoi;
  yostd_stringList properties_ydchoi;
};

struct ydchcx
{
  ysque	    *q_ydchcx;
  ydch_och  self_ydchcx;
  ydim_imr  imr_ydchcx;
  yslst	    *ois_ydchcx;	
};



STATICF ysstr *ydchMakeLock( CORBA_Object unbound );	
	STATICF void ydchObjIterDestroy( dvoid *x );

STATICF boolean ydchClaim( ydchcx *cx, yoenv *ev, char *property, dvoid *obj );
STATICF void ydchAbandon( ydchcx *cx, yoenv *ev, char *property ); 
STATICF void ydchTransfer( ydchcx *cx, yoenv *ev, yort_claim *claim );

externdef ysidDecl(YDCH_EX_INTERNAL) = "ydch::internal";
externdef ysmtagDecl(ydchoi_tag) = "ydchoi_tag";

	


ydch_och ydchInit( ysque *q )
{
  ydchcx    *cx = (ydchcx*)ysmGlbAlloc(sizeof(*cx), "ydchcx");

  cx->q_ydchcx = q;

  cx->imr_ydchcx = (ydim_imr)yoBind(ydim_imr__id, (char *)0,
				    (yoRefData*)0,(char*)0);

  cx->ois_ydchcx = ysLstCreate();

  yoSetImpl( ydch_och__id, (char*)0, ydch_och__stubs,
	    (dvoid*)&ydch_och__impl, (yoload)0, TRUE, (dvoid*)cx );

  yoSetImpl( ydch_objIterator__id, (char*)0, ydch_objIterator__stubs,
	    (dvoid*)&ydch_objIterator__impl, (yoload)0, FALSE, (dvoid*)cx );

  cx->self_ydchcx = (ydch_och)yoCreate( ydch_och__id, (char*)0,
				       (yoRefData*)0, (char *)0, (dvoid *)0);
  
  yoImplReady( ydch_och__id, (char*)0, cx->q_ydchcx );
  yoImplReady( ydch_objIterator__id, (char*)0, cx->q_ydchcx );

  return( (ydch_och)yoDuplicate((dvoid*)cx->self_ydchcx) );
}

void ydchTerm( ydch_och ch )
{
  ydchcx    *cx = (ydchcx*)yoGetImplState((dvoid *)ch);

  yoImplDeactivate( ydch_och__id, (char*)0 );
  yoImplDeactivate( ydch_objIterator__id, (char*)0 );

  ysLstDestroy( cx->ois_ydchcx, ydchObjIterDestroy );  

  yoRelease((dvoid*)cx->self_ydchcx);
  yoDispose((dvoid*)cx->self_ydchcx);

  yoRelease( (dvoid*)cx->imr_ydchcx );
  ysmGlbFree((dvoid*)cx );
}



STATICF ysstr *ydchMakeLock( CORBA_Object unbound )
{
  ysstr *s;
  char *rs;

  rs = yoRefToStr( (dvoid*)unbound );
  s = ysStrCreate( YDCH_LOCK_PREFIX );
  s = ysStrCat( s, rs );
  yoFree( (dvoid*)rs );
  return( s );
}

STATICF void ydchObjIterDestroy( dvoid *x )
{
  ydchoi *oicx = (ydchoi*)x;

  yostd_stringList__free( &oicx->properties_ydchoi, (ysmff)0 ); 
  yoRelease( (dvoid*)oicx->self_ydchoi );
  yoDispose( (dvoid*)oicx->self_ydchoi );
  ysmGlbFree( (dvoid*)oicx );
}


STATICF boolean ydchClaim( ydchcx *cx, yoenv *ev, char *property, dvoid *obj )
{
  ysevt	  *noreg sem;
  boolean  rv;
  yoevt	  revt;
  yoany	  any;
  yort_claim thing;

  NOREG(sem);
  sem = (ysevt *) 0;

  thing.owner = (yort_proc)0;
  thing.obj = (dvoid *)0;
  sem = ysSemCreate((dvoid*)0);

  yseTry
  {
    
    thing.property = (char *) property;
    thing.obj = yoDuplicate(obj);
    thing.owner = (yort_proc)yoDuplicate(yoYort());
    revt = yoToRmtEvt(sem);

    yseTry
    {
      ydyo_imr_stake((ydyo_imr)cx->imr_ydchcx,ev,&thing,revt);
    }
    yseEnd;
  }
  yseFinally
  {     
    if(sem && yseExid)
      ysSemDestroy(sem);
    if(thing.obj)
      yoRelease((dvoid*)thing.obj);
    if(thing.owner)
      yoRelease((dvoid*)thing.owner);
  }
  yseEnd;

  
  while( !ysSemTest( sem ) )
  {
    ysYield();
    ysSvcAll( cx->q_ydchcx );
  }

  ysSemSynch(sem,(dvoid *)&any); 
  rv = *(boolean*)any._value;

  yotkFreeVal(yoTcAny,(dvoid*)&any,(ysmff)0);

  return rv;
}


STATICF void ydchAbandon( ydchcx *cx, yoenv *ev, char *property )
{
  yort_claim what;

  what.property = property;
  what.obj = (dvoid*)0;
  what.owner = (yort_proc)0;
 
  ydim_imr_abandonFor( cx->imr_ydchcx, ev, &what);
}


STATICF void ydchTransfer( ydchcx *cx, yoenv *ev, yort_claim *claim )
{
  ydim_imr_transfer( cx->imr_ydchcx, ev, claim );
}



void ydch_och_whereActive_i( ydch_och or, yoenv* ev, CORBA_Object unbound, 
			    yort_proc* place)
{
  ydchcx *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ysstr *claimstr;

  claimstr = ydchMakeLock( unbound );

  *place = (yort_proc)0;
  yseTry
  {
    *place = (yort_proc)ydim_imr_propertyResolve(cx->imr_ydchcx,ev,
						 ysStrToText(claimstr));
  }
  yseCatchAll
  {
    yslError("whereActive_i caught exception %s\n", yseExid );
  }
  yseEnd;

  ysStrDestroy( claimstr );
}

void ydch_och_lock_i( ydch_och or, yoenv* ev, CORBA_Object unbound, 
		     yort_proc* place)
{
  ydchcx *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ysstr *claimstr;
  yort_claim what;

  claimstr = ydchMakeLock( unbound );
  what.property = ysStrToText(claimstr);
  what.obj = (dvoid*)yoDuplicate(yoYort());
  what.owner = (yort_proc)0;

  *place = (yort_proc)0;
  yseTry
  {
    if( ydchClaim( cx, ev, (char*)ysStrToText(claimstr), what.obj ) )
    {
      *place = (yort_proc)yoDuplicate( what.obj );
    }
    else
    {
      
      ydch_och_whereActive( or, ev, unbound, place );
    }
  }
  yseCatchAll
  {
    yslError("lock_i caught exception %s\n", yseExid );
  }
  yseEnd;
  yoRelease( what.obj );
  ysStrDestroy( claimstr );
}

void ydch_och_setPlace_i( ydch_och or, yoenv* ev, CORBA_Object unbound, 
			 yort_proc place)
{
  ydchcx *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ysstr *claimstr;
  yort_claim newClaim;

  claimstr = ydchMakeLock( unbound );
  yseTry
  {
    newClaim.property = ysStrToText( claimstr );
    newClaim.obj = (CORBA_Object)place;
    newClaim.owner = place;
    ydchTransfer( cx, ev, &newClaim );
  }
  yseCatchAll
  {
    yslError("setPlace_i caught exception %s\n", yseExid );
  }
  yseEnd;
  ysStrDestroy( claimstr );
}

void ydch_och_remove_i( ydch_och or, yoenv* ev, CORBA_Object unbound )
{
  ydchcx *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ysstr *claimstr;
  
  claimstr = ydchMakeLock( unbound );
  yseTry
  {
    ydchAbandon( cx, ev, (char*)ysStrToText(claimstr));
  }
  yseCatchAll
  {
    yslError("remove_i caught exception %s\n", yseExid );
  }
  yseEnd;
  ysStrDestroy( claimstr );
}  


void ydch_och_listObjs_i( ydch_och or, yoenv* ev, ub4 count,
			 yostd_objList* objs, ydch_objIterator* oi)
{
  ydchcx    *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ydchoi    *oicx;
  yostd_stringList strings;
 
  oicx = (ydchoi*)ysmGlbAlloc(sizeof(*oicx), ydchoi_tag );
  strings = ydim_imr_listProperties( cx->imr_ydchcx, ev,
				    (char*)YDCH_LOCK_REGEXP );
  yostd_stringList__copy( &oicx->properties_ydchoi, &strings, (ysmaf)0 );
  yostd_stringList__free( &strings, yoFree );
  objs->_length = 0;
  objs->_maximum = 0;
  objs->_buffer = (dvoid**)0;
  *oi = (ydch_objIterator)0;
  if( oicx->properties_ydchoi._length )
  {
    *oi = (ydch_objIterator)yoCreate( ydch_objIterator__id, (char*)0,
				     (yoRefData*)0, (char*)0, (dvoid*)oicx);
    oicx->self_ydchoi = (ydch_objIterator)yoDuplicate( (dvoid*)*oi );
    oicx->le_ydchoi = ysLstEnq( cx->ois_ydchcx, (dvoid*)oicx );
    if( !ydch_objIterator_next_n_i( *oi, ev, count, objs ) )
      *oi = (ydch_objIterator)0;
  }
  if( !oi )
    ydch_objIterator_destroy_i( *oi, ev );
}


boolean ydch_objIterator_next_one_i( ydch_objIterator or, yoenv* ev, 
				    CORBA_Object* obj)
{
  yostd_objList objs;
  boolean rv;
  
  rv = ydch_objIterator_next_n_i( or, ev, (ub4)1, &objs );
  if( objs._length )
    *obj = objs._buffer[0];
  else
    *obj = (CORBA_Object)0;
  yoFree( (dvoid*)objs._buffer );
  return rv;
}


boolean ydch_objIterator_next_n_i( ydch_objIterator or, yoenv* ev,
				  ub4  count, yostd_objList* objs)
{
  ydchoi    *oicx = (ydchoi*)yoGetState( (dvoid*)or );
  ub4 i;
  ub4 left;

  objs->_length = 0;
  objs->_maximum = count;
  objs->_buffer = (dvoid**)yoAlloc( (size_t)count * sizeof(dvoid**) );

  for( i = 0; i < objs->_maximum && i < oicx->properties_ydchoi._length ; i++ )
  {
    objs->_buffer[ i ] =
      yoStrToRef(oicx->properties_ydchoi._buffer[i] +
		 sizeof(YDCH_LOCK_PREFIX)-1);
    ysmGlbFree( (dvoid*)oicx->properties_ydchoi._buffer[i] );
  }
 
  
  if( (objs->_length = i) )
  {
    oicx->properties_ydchoi._length -= i;
    
    if( (left = oicx->properties_ydchoi._length - i) )
      DISCARD memcpy( (dvoid*)objs->_buffer,
		     (dvoid*)&oicx->properties_ydchoi._buffer[i],
		     sizeof(dvoid*) * left);
  }
  return( oicx->properties_ydchoi._length ? TRUE : FALSE );
}


void ydch_objIterator_destroy_i( ydch_objIterator or, yoenv* ev)
{
  ydchcx    *cx = (ydchcx*)yoGetImplState( (dvoid*)or );
  ydchoi    *oicx = (ydchoi*)yoGetState( (dvoid*)or );

  DISCARD ysLstRem( cx->ois_ydchcx, oicx->le_ydchoi );
  ydchObjIterDestroy( (dvoid*)oicx );
}

