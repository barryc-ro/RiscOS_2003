/* mx/src/yd/ydnm.c */


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
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YDNM_ORACLE
#include <ydnm.h>
#endif
#ifndef YDNMIDL_ORACLE
#include <ydnmidl.h>
#endif
#ifndef YDNMIDLI_ORACLE
#include <ydnmidlI.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif






typedef struct ydnmbc ydnmbc;
struct ydnmbc                                             
{
  ydnmNamingContext self;
  ysspTree bindings;
  yslst *iterators;
  ysle	*le;                                        
};

typedef struct ydnmob ydnmob;
struct ydnmob                                       
{
  ysspNode node;                                    
  ydnmBinding binding;
  CORBA_Object obj;
};

typedef struct ydnmi ydnmi;
struct ydnmi                                             
{
  ydnmBindingIterator self;
  ydnmNameComponent last;                             
  ydnmbc    *bc;
  boolean   first;
  ysle	    *le;                                 
};

typedef struct ydnmcx ydnmcx;
struct ydnmcx
{
  ydnmNamingContext    bc;                            
  yslst	    *contexts;
};







STATICF sword ydnmBindCmp( CONST dvoid *a, CONST dvoid *b );

STATICF ydnmbc *ydnmFindContext(ydnmbc *bc, ydnmNameComponent *ncomp);

#define ydnmFindBinding(bc,ncomp) \
  (ydnmob*)ysspLookup((dvoid*)ncomp,&bc->bindings)

STATICF void ydnmEnterBinding(ydnmbc *bc, ydnmNameComponent *ncomp,
                      ydnmBindingType type, CORBA_Object obj, boolean rebind);

STATICF CORBA_Object ydnmRemoveBinding(ydnmbc *bc, ydnmNameComponent *ncomp);

STATICF void ydnmResNmToCtx(ydnmName *n, ydnmbc **bc,
                            ydnmNameComponent **ncomp);

STATICF void ydnmFreeBindingContext(ydnmbc *bc);

STATICF void ydnmFreeBinding(ydnmob *b);

STATICF void ydnmFreeIterator(ydnmi *i);



static CONST_W_PTR struct ydnmNamingContext__tyimpl
ydnmNamingContext__impl =
 {
  ydnmNamingContext_bind_i,
  ydnmNamingContext_rebind_i,
  ydnmNamingContext_bind_context_i,
  ydnmNamingContext_resolve_i,
  ydnmNamingContext_unbind_i,
  ydnmNamingContext_new_context_i,
  ydnmNamingContext_bind_new_context_i,
  ydnmNamingContext_destroy_i,
  ydnmNamingContext_list_i
 };

static CONST_W_PTR struct ydnmBindingIterator__tyimpl
ydnmBindingIterator__impl =
 {
  ydnmBindingIterator_next_one_i,
  ydnmBindingIterator_next_n_i,
  ydnmBindingIterator_destroy_i
 };

static CONST_W_PTR struct ydnmInitialNamingContext__tyimpl
    ydnmInitialNamingContext__impl =
 {
  ydnmInitialNamingContext_get_i
 };


externdef ysmtagDecl(ydnmi_tag) = "ydnmi";
externdef ysmtagDecl(ydnmcx_tag) = "ydnmcx";





ydnmNamingContext ydnmInit(ysque *q)
{
  ydnmcx *cx;
  ydnmbc *bc;

  cx = (ydnmcx*)ysmGlbAlloc( sizeof(ydnmcx), ydnmcx_tag );
  cx->contexts = ysLstCreate();

  yoSetImpl(ydnmInitialNamingContext__id, (char*)0,
            ydnmInitialNamingContext__stubs,
	    (dvoid*)&ydnmInitialNamingContext__impl,
	    (yoload)0, TRUE, (dvoid *)cx);

  yoSetImpl(ydnmNamingContext__id, (char*)0,
            ydnmNamingContext__stubs, (dvoid*)&ydnmNamingContext__impl,
	    (yoload)0, FALSE, (dvoid *)cx);

  yoSetImpl(ydnmBindingIterator__id, (char*)0,
            ydnmBindingIterator__stubs, (dvoid*)&ydnmBindingIterator__impl,
	    (yoload)0,FALSE,(dvoid *)cx);

  cx->bc = ydnmNamingContext_new_context_i( (ydnmNamingContext)0, (yoenv*)0 );
  bc = (ydnmbc*)yoGetState((dvoid*)cx->bc);
  bc->le = ysLstEnq( cx->contexts, (dvoid*)bc );

  yoImplReady(ydnmInitialNamingContext__id, (char*)0, q);
  yoImplReady(ydnmNamingContext__id, (char*)0, q);
  yoImplReady(ydnmBindingIterator__id, (char*)0, q);

  return( (ydnmNamingContext)yoDuplicate((dvoid*)cx->bc) );
}

void ydnmTerm( ydnmNamingContext or )
{
  ydnmcx *cx = (ydnmcx*)yoGetImplState((dvoid*)or);
  ydnmbc *bc;

  yoImplDeactivate(ydnmNamingContext__id, (char*)0);
  yoImplDeactivate(ydnmBindingIterator__id, (char*)0);

  while( (bc = (ydnmbc*)ysLstDeq( cx->contexts ) ) )
    ydnmFreeBindingContext(bc);

  ysLstDestroy(cx->contexts,(ysmff)0);
  ysmGlbFree( (dvoid*)cx );
}





STATICF sword ydnmBindCmp( CONST dvoid *a, CONST dvoid *b )
{
  ydnmNameComponent *ap = (ydnmNameComponent*)a;
  ydnmNameComponent *bp = (ydnmNameComponent*)b;
  sword rv;

  if( !(rv = yduSafeStrcmp( ap->id, bp->id ) ) )
   rv = yduSafeStrcmp( ap->kind, ap->kind ); 

  return rv;
}




ydnmNamingContext
ydnmInitialNamingContext_get_i( ydnmInitialNamingContext or, yoenv* ev)
{
  ydnmcx *cx = (ydnmcx*)yoGetImplState((dvoid*)or);
  return (ydnmNamingContext)yoDuplicate((dvoid*)cx->bc);
}


void ydnmNamingContext_bind_i( ydnmNamingContext or, yoenv* ev,
			      ydnmName* n, CORBA_Object obj)
{
  ydnmbc *bc;
  ydnmNameComponent *ncomp;

  bc = (ydnmbc *) yoGetState((dvoid *)or);

  ydnmResNmToCtx(n,&bc,&ncomp);
  ydnmEnterBinding(bc,ncomp,ydnmObject,obj,FALSE);
}


void ydnmNamingContext_rebind_i( ydnmNamingContext or, yoenv* ev,
				ydnmName* n, CORBA_Object obj)
{
  ydnmbc *bc;
  ydnmNameComponent *ncomp;

  bc = (ydnmbc *) yoGetState((dvoid *)or);

  ydnmResNmToCtx(n,&bc,&ncomp);
  ydnmEnterBinding(bc,ncomp,ydnmObject,obj,TRUE);
}


void ydnmNamingContext_bind_context_i( ydnmNamingContext or,
				      yoenv* ev, ydnmName* n,
				      CORBA_Object obj)
{
  ydnmbc *bc;
  ydnmNameComponent *ncomp;

  bc = (ydnmbc *) yoGetState((dvoid *)or);

  ydnmResNmToCtx(n,&bc,&ncomp);
  ydnmEnterBinding(bc,ncomp,ydnmContext,obj,TRUE);
}


CORBA_Object ydnmNamingContext_resolve_i( ydnmNamingContext or,
					 yoenv* ev, ydnmName* n)
{
  ydnmbc *bc;
  ydnmNameComponent *ncomp;
  ydnmob *b;

  bc = (ydnmbc *) yoGetState((dvoid *)or);

  ydnmResNmToCtx(n,&bc,&ncomp);
  if(!(b = ydnmFindBinding(bc,ncomp)))
  {
    ydnmNotFound nfi;

    nfi.why_ydnmNotFound = missing_node;
    nfi.rest_ydnmNotFound._maximum = 1;
    nfi.rest_ydnmNotFound._length = 1;
    nfi.rest_ydnmNotFound._buffer = ncomp;
    yseThrowObj(EX_YDNMNOTFOUND,nfi);
  }
  return (CORBA_Object)yoDuplicate((dvoid*)b->obj);
}


CORBA_Object ydnmNamingContext_unbind_i( ydnmNamingContext or,
					yoenv* ev, ydnmName* n)
{
  ydnmbc *bc;
  ydnmNameComponent *ncomp;

  bc = (ydnmbc *) yoGetState((dvoid *)or);

  ydnmResNmToCtx(n,&bc,&ncomp);
  return ydnmRemoveBinding(bc,ncomp);
}


ydnmNamingContext
ydnmNamingContext_new_context_i( ydnmNamingContext or, yoenv* ev)
{
  ydnmcx *cx;
  ydnmbc *bc;
  ydnmNamingContext nc;

  bc = (ydnmbc *) ysmGlbAlloc(sizeof(ydnmbc),"ydnmbc");
  bc->self = (ydnmNamingContext)0;
  DISCARD ysspNewTree( &bc->bindings, ydnmBindCmp );
  bc->iterators = ysLstCreate();

  nc = (ydnmNamingContext)
    yoCreate(ydnmNamingContext__id, (char*)0,(yoRefData *)0,
             (char*)0, (dvoid *)bc);
  bc->self = (ydnmNamingContext)yoDuplicate((dvoid*)nc );

  if( or )
  {
    cx = (ydnmcx*)yoGetImplState((dvoid*)or);
    bc->le = ysLstEnq( cx->contexts, (dvoid*)bc );
  }
  return nc;
}

ydnmNamingContext
ydnmNamingContext_bind_new_context_i( ydnmNamingContext or, yoenv* ev,
                                     ydnmName* n)
{
  ydnmNamingContext nc;

  nc = ydnmNamingContext_new_context_i(or,ev);
  ydnmNamingContext_bind_context_i(or, ev, n, (CORBA_Object)nc);
  return nc;
}


void ydnmNamingContext_destroy_i( ydnmNamingContext or,
				 yoenv* ev)
{
  ydnmcx *cx;
  ydnmbc *bc;

  bc = (ydnmbc *) yoGetState((dvoid *)or);
  if( bc->bindings.root_ysspTree )
    yseThrow(EX_YDNMNOTEMPTY);

  cx = (ydnmcx*)yoGetImplState((dvoid*)or);

  DISCARD ysLstRem(cx->contexts,bc->le);
  ydnmFreeBindingContext(bc);
}


void ydnmNamingContext_list_i( ydnmNamingContext or, yoenv* ev,
			      ub4 count, ydnmBindingList* bl,
			      ydnmBindingIterator* bi)
{
  ydnmbc *bc = (ydnmbc*)yoGetState((dvoid*)or);
  ydnmi *i;

  bl->_length = bl->_maximum = 0;
  bl->_buffer = (ydnmBinding*)0;

  i = (ydnmi*)ysmGlbAlloc( sizeof(*i), ydnmi_tag );
  i->first = TRUE;
  i->last.id = (char*)0;
  i->last.kind = (char*)0;
  i->bc = bc;
  *bi = i->self =
    (ydnmBindingIterator)yoCreate( ydnmBindingIterator__id, (char*)0,
				  (yoRefData*)0, (char*)0, (dvoid*)i );
  i->le = ysLstEnq( bc->iterators, (dvoid*)i );

  if( count && (!ydnmBindingIterator_next_n_i( *bi, ev, count, bl ) ||
		bl->_length != count ) )
  {
    ydnmBindingIterator_destroy_i( *bi, ev );
    *bi = (ydnmBindingIterator)0;
  }
}



boolean ydnmBindingIterator_next_one_i( ydnmBindingIterator or,
				       yoenv* ev, ydnmBinding* b)
{
  boolean rv = FALSE;
  ydnmBindingList list;
  
  if( ydnmBindingIterator_next_n_i( or, ev, (ub4)1, &list ) && list._length )
  {
    *b = list._buffer[0];
    rv = TRUE;
    yoFree( (dvoid*)list._buffer );
  }
  return rv;
}


boolean ydnmBindingIterator_next_n_i( ydnmBindingIterator or,
				     yoenv* ev, ub4 count,
				     ydnmBindingList* bl)
{
  ydnmi	*i = (ydnmi*)yoGetState((dvoid*)or);
  ysspNode *n;
  ydnmob *ob = (ydnmob*)0;
  boolean rv;

  bl->_length = 0;
  bl->_maximum = count;
  bl->_buffer = (ydnmBinding*)yoAlloc( (size_t)count * sizeof(*bl->_buffer ) );

  if( i->first )
    n = ysspFHead( &i->bc->bindings );
  else
    n = ysspNextLookup( (dvoid*)&i->last, &i->bc->bindings );

  for( ; n && bl->_length < count ; n = ysspFNext(n) )
  {
    ob = (ydnmob*)n;
    ydnmBinding__copy( &bl->_buffer[bl->_length++], &ob->binding, yoAlloc );
    i->first = FALSE;
  }
  if( !bl->_length )
  {
    bl->_maximum = 0;
    yoFree( (dvoid*)bl->_buffer );
  }
  if( !i->first && ob )
  {
    ydnmNameComponent__free( &i->last, (ysmff)0 );
    ydnmNameComponent__copy( &i->last,
			    &ob->binding.name_ydnmBinding, (ysmaf)0 );
  }

  
  rv = !count || bl->_length;

  return rv;
}


void ydnmBindingIterator_destroy_i( ydnmBindingIterator or, yoenv* ev)
{
  ydnmi	*i = (ydnmi*)yoGetState((dvoid*)or);
  
  DISCARD ysLstRem( i->bc->iterators, i->le );
  ydnmFreeIterator(i);

}


STATICF void ydnmEnterBinding(ydnmbc *bc, ydnmNameComponent *ncomp,
                      ydnmBindingType type, CORBA_Object obj, boolean rebind)
{
  ydnmob *b;

  if((b = ydnmFindBinding(bc,ncomp)))
  {
    if(!rebind)
      yseThrow(EX_YDNMALREADYBOUND);
    yoRelease( (dvoid*)b->obj );
    b->binding.type_ydnmBinding = type;
  }
  else
  {
    b = (ydnmob *) ysmGlbAlloc(sizeof(ydnmob),"ydnmob");
    b->node.key_ysspNode = (dvoid*)&b->binding.name_ydnmBinding; 
    ydnmNameComponent__copy(&b->binding.name_ydnmBinding,ncomp, (ysmaf)0);
    b->binding.type_ydnmBinding = type;
    DISCARD ysspEnq( &b->node, &bc->bindings );
  }
  b->obj = (CORBA_Object)yoDuplicate((dvoid*)obj);
}

STATICF CORBA_Object ydnmRemoveBinding(ydnmbc *bc, ydnmNameComponent *ncomp)
{
  ydnmob *b;
  CORBA_Object obj;

  if(!(b = ydnmFindBinding(bc,ncomp)))
  {
    ydnmNotFound nfi;

    nfi.why_ydnmNotFound = missing_node;
    nfi.rest_ydnmNotFound._maximum = 1;
    nfi.rest_ydnmNotFound._length = 1;
    nfi.rest_ydnmNotFound._buffer = ncomp;
    yseThrowObj(EX_YDNMNOTFOUND,nfi);
  }

  obj = (CORBA_Object)yoDuplicate((dvoid*)b->obj);
  ysspRemove( &b->node, &bc->bindings );
  ydnmFreeBinding(b);

  return obj;
}

STATICF void ydnmResNmToCtx(ydnmName *n, ydnmbc **bc,
                            ydnmNameComponent **ncomp)
{
  ub4 i;
  ydnmbc *nbc;

  if(n->_length == 0)
    yseThrow(EX_YDNMINVALIDNAME);

  for(i=n->_length-1, *ncomp=n->_buffer; i > 0; i--, (*ncomp)++, *bc = nbc)
  {
    if(!(nbc = ydnmFindContext(*bc,*ncomp)))
    {
      ydnmCannotProceed cnp;

      cnp.ctx_ydnmCannotProceed =
	(ydnmNamingContext)yoDuplicate((dvoid*)(*bc)->self);
      cnp.rest_ydnmCannotProceed._maximum = 0;
      cnp.rest_ydnmCannotProceed._length = i+1;
      cnp.rest_ydnmCannotProceed._buffer = *ncomp;
      
      yseThrowObj(EX_YDNMCANNOTPROCEED,cnp);
    }
  }
}

STATICF ydnmbc *ydnmFindContext(ydnmbc *bc, ydnmNameComponent *ncomp)
{
  ydnmob *ob = ydnmFindBinding( bc, ncomp );
  ydnmbc *rv;

  if( ob && ob->binding.type_ydnmBinding == ydnmContext )
    rv = (ydnmbc *)yoGetState((dvoid*)ob->obj);    
  else
    rv = (ydnmbc *)0;

  return rv;
}

STATICF void ydnmFreeBindingContext(ydnmbc *bc)
{
  ydnmi	*i;
  ydnmob *ob;

  yoDispose((dvoid*)bc->self);

  
  while( (ob = (ydnmob*)ysspDeq( &bc->bindings.root_ysspTree ) ) )
    ydnmFreeBinding(ob);
  
  while( (i = (ydnmi*)ysLstDeq( bc->iterators ) ) )
    ydnmFreeIterator(i);
  ysLstDestroy(bc->iterators,(ysmff)0);
  ysmGlbFree((dvoid *)bc);
}

STATICF void ydnmFreeBinding(ydnmob *b)
{
  ydnmNameComponent__free(&b->binding.name_ydnmBinding, (ysmff)0);
  ysmGlbFree((dvoid *)b);
}

STATICF void ydnmFreeIterator(ydnmi *i)
{
  ydnmNameComponent__free( &i->last, (ysmff)0 );
  yoDispose( (dvoid*)i->self );
  ysmGlbFree( (dvoid*)i );
}

