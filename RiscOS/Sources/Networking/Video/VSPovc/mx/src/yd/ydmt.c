/* mx/src/yd/ydmt.c */


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
#ifndef YDMT_ORACLE
#include <ydmt.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDIDLI_ORACLE
#include <ydidlI.h>
#endif
#ifndef YOIDL_ORACLE
#include <yoidl.h>
#endif

#define YDMT_FAC	"YDMT"

externdef ysidDecl(YDMT_EX_INTERNAL) = "ydmt::internal";
externdef ysidDecl(YDMT_EX_UNEXPECTED_NULL) = "ydmt::unexpected_null";

static CONST_W_PTR struct ydmt_imtr__tyimpl ydmt_imtr__impl =
 {
  ydmt_imtr_addInfo_i,
  ydmt_imtr_addYortAll_i,
  ydmt_imtr_getInfo_i,
  ydmt_imtr__get_self_i,
  ydmt_imtr__get_imr_i,
  ydmt_imtr__get_mtinfo_i
 };

static CONST_W_PTR struct ydmt_info__tyimpl ydmt_info__impl =
 {
  ydmt_info_destroy_i,
  ydmt_info__get_mtinfo_i,
  ydmt_info__set_mtinfo_i
 };




#define YDMT_DEFAULT_REAP_MS	((sb4)(30*60*1000))


typedef struct
{
  ysspNode  node_ydmtn;		
  ydmt_minfo  info_ydmtn;	
  sysb8	    expire_ydmtn;

} ydmtn;

typedef struct
{
  ysque	    *q_ydmtcx;
  ydmt_imtr ydmtcx_self;
  ydim_imr  imr_ydmtcx;
  ysspTree  mtree_ydmtcx;
  sb4	    nmtree_ydmtcx;
  sysb8	    reap_ydmtcx;	

} ydmtcx;



STATICF void ydmtReap( ydmtcx *cx );
STATICF ydmt_info ydmtAddAll( ydmtcx *cx, yort_proc y, yort_implAll *all );
STATICF sword ydmtMCmp(CONST dvoid *a, CONST dvoid *b);
STATICF ydmt_minfo* ydmtGetInfo( CONST ydmt_imtr or, yort_proc y,
				char *intf, char *impl  );






void ydmtInit( ysque *q, ydmt_imtr *oydmt, ydim_imr imr )
{
  ydmtcx    *cx = (ydmtcx*)ysmGlbAlloc(sizeof(*cx), "ydmtcx");
  char	    *arg;
  sb4	    val;

  cx->q_ydmtcx = q;
  cx->imr_ydmtcx = (ydim_imr)yoDuplicate((dvoid*)imr);

  DISCARD ysspNewTree( &cx->mtree_ydmtcx, ydmtMCmp );
  cx->nmtree_ydmtcx= 0;

  arg = ysResGetLast("ydmt.reap-ms");
  val = (sb4)(arg ? atol( arg ) : YDMT_DEFAULT_REAP_MS);
  sysb8ext( &cx->reap_ydmtcx, val * 1000 );
  
  yoSetImpl( ydmt_imtr__id, (char*)0, ydmt_imtr__stubs,
	    (dvoid*)&ydmt_imtr__impl, (yoload)0, TRUE, (dvoid*)cx );
  yoSetImpl( ydmt_info__id, (char*)0, ydmt_info__stubs,
	    (dvoid*)&ydmt_info__impl, (yoload)0, FALSE, (dvoid*)cx );

  
  cx->ydmtcx_self = (ydmt_imtr)yoCreate( ydmt_imtr__id, (char*)0,
				    (yoRefData*)0,
				    (char *)0, (dvoid *)0);

  *oydmt = (ydmt_imtr)yoDuplicate((dvoid*)cx->ydmtcx_self);

  yoImplReady( ydmt_imtr__id, (char*)0, cx->q_ydmtcx );
  yoImplReady( ydmt_info__id, (char*)0, cx->q_ydmtcx );

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1450, YSLSEV_INFO, (char*)0, YSLNONE );  
}





void ydmtTerm( ydmt_imtr mt )
{
  ydmtcx    *cx = (ydmtcx*)yoGetImplState((dvoid *)mt);
  ysspNode  *n;
  ydmtn	    *mn;
  ydmt_minfo *mi;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1451, YSLSEV_INFO, (char*)0, YSLNONE );

  yoImplDeactivate( ydmt_info__id, (char*)0 );
  yoImplDeactivate( ydmt_imtr__id, (char*)0 );

  while( (n = ysspDeqTree( &cx->mtree_ydmtcx ) ) )
  {
    
    mn = (ydmtn*)n;
    mi = &mn->info_ydmtn;
    ydimActiveSetUsrp( cx->imr_ydmtcx, 
		      mn->info_ydmtn.yort_ydmt_minfo,
		      mi->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo,
		      mi->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo,
		      (dvoid*)0 );
    yoDispose( (dvoid*)mn->info_ydmtn.self_ydmt_minfo );
    ydmt_minfo__free( &mn->info_ydmtn, yotkFreeStr );
    ysmGlbFree( (dvoid*)mn );
  }
  yoRelease( (dvoid*)cx->imr_ydmtcx );

  yoRelease( (dvoid*)cx->ydmtcx_self );
  yoDispose( (dvoid*)cx->ydmtcx_self );
  ysmGlbFree( (dvoid*)cx );
}











void ydmt_imtr_addInfo_i( ydmt_imtr or, yoenv *ev,
			 ydmt_minfo* minfo, ydmt_info* mtobj)
{

  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1452, YSLSEV_INFO, (char*)0, YSLNONE );

  *mtobj = ydmtAddAll( cx, minfo->yort_ydmt_minfo, &minfo->all_ydmt_minfo );
}



void ydmt_imtr_addYortAll_i( ydmt_imtr or, yoenv *ev,
			    yort_proc y, yort_implAllList* alist)
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  ub4		i;
  ydmt_info	outobj;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1453, YSLSEV_INFO, (char*)0, YSLNONE );

  for( i = 0; i < alist->_length ; i++ )
  {
    outobj = ydmtAddAll( cx, y, &alist->_buffer[ i ] );
    yoRelease( (dvoid*)outobj );
  }
  ydmtReap( cx );
}






ydmt_minfo ydmt_imtr_getInfo_i( ydmt_imtr or, yoenv *ev,
			       yort_proc y, char *intf, char *impl )
{
  ydmt_minfo minfo;
  ydmt_minfo *mi;

  mi = ydmtGetInfo( or, y, intf, impl );
  if( mi )
    ydmt_minfo__copy( &minfo, mi, yoAlloc );
  else
  {
    CLRSTRUCT(minfo);
    yseThrow( EX_YDUNKNOWN );
  }
  return minfo;
}


ydmt_imtr ydmt_imtr__get_self_i( ydmt_imtr or, yoenv *ev )
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  return( (ydmt_imtr)yoDuplicate((dvoid*)cx->ydmtcx_self) );
}


ydim_imr ydmt_imtr__get_imr_i( ydmt_imtr or, yoenv *ev )
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  return( (ydim_imr)yoDuplicate((dvoid*)cx->imr_ydmtcx) );
}





ydmt_minfoList ydmt_imtr__get_mtinfo_i( ydmt_imtr or, yoenv *ev )
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  ydmt_minfoList list;

  ysspNode  *n;
  ydmt_minfo  *mi;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1454, YSLSEV_INFO, (char*)0,
	   YSLUB4(cx->nmtree_ydmtcx ), YSLEND );

  list._length = 0;
  list._maximum = (ub4)cx->nmtree_ydmtcx;
  list._buffer =
    (ydmt_minfo*)yoAlloc( (size_t)list._maximum * sizeof(ydmt_minfo) );

  for( n = ysspFHead( &cx->mtree_ydmtcx ) ; n ; n = ysspFNext( n ) )
  {
    mi = &((ydmtn*)n->key_ysspNode)->info_ydmtn;
    
    ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1455, YSLSEV_DEBUG(4), (char*)0,
     YSLSTR(yduStr(mi->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo)),
     YSLSTR(yduStr(mi->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo)),
	     YSLEND );

    ydmt_minfo__copy( &list._buffer[list._length++], mi, yoAlloc );
  }

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1456, YSLSEV_DEBUG(4), (char*)0,
	   YSLUB4(list._length), YSLEND );

  return( list );
}





void ydmt_info_destroy_i( ydmt_info or, yoenv *ev)
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  ydmtn *mn = (ydmtn*)yoGetState( (dvoid*)or );
  ydmt_minfo  *minfo = &mn->info_ydmtn;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1457, YSLSEV_INFO, (char*)0,
   YSLSTR(yduStr(minfo->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo)),
   YSLSTR(yduStr(minfo->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo)),
	   YSLEND);
	   
  cx->nmtree_ydmtcx--;
  ysspRemove( &mn->node_ydmtn, &cx->mtree_ydmtcx );
  ydmt_minfo__free( minfo, yotkFreeStr );
  yoDispose( (dvoid*)mn->info_ydmtn.self_ydmt_minfo );

  ydimActiveSetUsrp( cx->imr_ydmtcx, 
	    minfo->yort_ydmt_minfo,
	    minfo->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo,
	    minfo->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo,
	    (dvoid*)0 );

  ysmGlbFree( (dvoid*)mn );
}





ydmt_minfo ydmt_info__get_mtinfo_i( ydmt_info or, yoenv* ev)
{
  ydmtn *mn = (ydmtn*)yoGetState( (dvoid*)or );
  ydmt_minfo  *minfo = &mn->info_ydmtn;
  ydmt_minfo  mtinfo;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1458, YSLSEV_INFO, (char*)0,
   YSLSTR(yduStr(minfo->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo)),
   YSLSTR(yduStr(minfo->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo)),
	   YSLEND );

  ydmt_minfo__copy( &mtinfo, minfo, yoAlloc );
  return mtinfo;
}





void ydmt_info__set_mtinfo_i( ydmt_info or, yoenv *ev, ydmt_minfo* val)
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  ydmtn *mn = (ydmtn*)yoGetState( (dvoid*)or );
  ydmt_minfo  *minfo = &mn->info_ydmtn;
  ysspNode *n;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1459, YSLSEV_INFO, (char*)0,
   YSLSTR(yduStr(val->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo)),
   YSLSTR(yduStr(val->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo)),
	     YSLEND );

  
  cx->nmtree_ydmtcx--;
  ysspRemove( &mn->node_ydmtn, &cx->mtree_ydmtcx );
  ydimActiveSetUsrp( cx->imr_ydmtcx, 
		    minfo->yort_ydmt_minfo,
		    minfo->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo,
		    minfo->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo,
		    (dvoid*)0 );
  ydmt_minfo__free( &mn->info_ydmtn, yotkFreeStr );
  ydmt_minfo__copy( &mn->info_ydmtn, val, yotkAllocStr );
  if( (n = ysspLookup( (dvoid*)mn, &cx->mtree_ydmtcx ) ) )
  {
    
    cx->nmtree_ydmtcx--;
    ysspRemove( n, &cx->mtree_ydmtcx );
    
    ydmt_minfo__free( &((ydmtn*)n)->info_ydmtn, yotkFreeStr );
    
    yoDispose( (dvoid*)((ydmtn*)n)->info_ydmtn.self_ydmt_minfo );
    ysmGlbFree( (dvoid*)n );
  }
  yoRelease( (dvoid*)mn->info_ydmtn.self_ydmt_minfo );
  mn->info_ydmtn.self_ydmt_minfo = (ydmt_info)yoDuplicate((dvoid*)or);
  ysClock(&mn->expire_ydmtn);
  sysb8add(&mn->expire_ydmtn,&mn->expire_ydmtn,&cx->reap_ydmtcx);
  DISCARD ysspEnq( &mn->node_ydmtn, &cx->mtree_ydmtcx ); 
  cx->nmtree_ydmtcx++;
  ydimActiveSetUsrp( cx->imr_ydmtcx,
		    minfo->yort_ydmt_minfo,
		    minfo->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo,
		    minfo->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo,
		    (dvoid*)minfo );
}









STATICF ydmt_minfo* ydmtGetInfo( CONST ydmt_imtr or, yort_proc y,
				char *intf, char *impl  )
{
  ydmtcx *cx = (ydmtcx*)yoGetImplState( (dvoid*)or );
  ysspNode  *n;
  ydmtn	    *mn = (ydmtn*)0;

  ydmtn	    lookup;
  ydmt_minfo  *mi = &lookup.info_ydmtn;
  yort_implInfo *yi = &mi->all_ydmt_minfo.info_yort_implAll;

  
  ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1460, YSLSEV_INFO, (char*)0,
	   YSLSTR(yduStr(intf)), YSLSTR(yduStr(impl)), YSLEND ); 

  mi->yort_ydmt_minfo = y;
  yi->intf_yort_implInfo = intf;
  yi->impl_yort_implInfo = impl;

  if( (n = ysspLookup( (dvoid*)&lookup, &cx->mtree_ydmtcx) ) )
  {
    mn = (ydmtn*)n->key_ysspNode;
    mi = &mn->info_ydmtn;

    
    ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)146, YSLSEV_DEBUG(2), (char*)0,
       YSLSTR(yduStr(mi->all_ydmt_minfo.info_yort_implAll.intf_yort_implInfo)),
       YSLSTR(yduStr(mi->all_ydmt_minfo.info_yort_implAll.impl_yort_implInfo)),
	     YSLEND );
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)146, YSLSEV_DEBUG(2), (char*)0,YSLNONE);
    mi = (ydmt_minfo*)0;
  }
  return mi;
}





STATICF sword ydmtMCmp(CONST dvoid *a, CONST dvoid *b)
{
  ydmt_minfo   *ap = &((ydmtn*)a)->info_ydmtn;
  ydmt_minfo   *bp = &((ydmtn*)b)->info_ydmtn;
  yort_implAll	*aip = &ap->all_ydmt_minfo;
  yort_implAll	*bip = &bp->all_ydmt_minfo;
  sword	rv;
  
  if(!(rv = yoCmp( (dvoid*)ap->yort_ydmt_minfo,
		  (dvoid*)bp->yort_ydmt_minfo))
     && !(rv = (sword)strcmp(aip->info_yort_implAll.intf_yort_implInfo,
			     bip->info_yort_implAll.intf_yort_implInfo)))
    rv = yduSafeStrcmp(aip->info_yort_implAll.impl_yort_implInfo, 
		       bip->info_yort_implAll.impl_yort_implInfo);

  return( rv );
}





STATICF ydmt_info ydmtAddAll( ydmtcx *cx, yort_proc y, yort_implAll *all )
{
  ydmt_info rv;
  ydmtn *mn;
  ysspNode *n;
  
  ydmtn	    lookup;
  yort_implInfo *ii = &lookup.info_ydmtn.all_ydmt_minfo.info_yort_implAll;

  lookup.info_ydmtn.yort_ydmt_minfo = y;
  ii->intf_yort_implInfo = all->info_yort_implAll.intf_yort_implInfo;
  ii->impl_yort_implInfo = all->info_yort_implAll.impl_yort_implInfo;

  if( (n = ysspLookup( (dvoid*)&lookup, &cx->mtree_ydmtcx) ) )
  {
    
    
    mn = (ydmtn*)n;
    cx->nmtree_ydmtcx--;
    ysspRemove( n, &cx->mtree_ydmtcx );
    yort_implAll__free( &mn->info_ydmtn.all_ydmt_minfo, yotkFreeStr );
  }
  else				
  {
    mn = (ydmtn*)ysmGlbAlloc( sizeof(*mn), "ydmtn" );
    mn->node_ydmtn.key_ysspNode = (dvoid*)mn;
    mn->info_ydmtn.self_ydmt_minfo =
      (ydmt_info) yoCreate(ydmt_info__id, (char*)0,
			   (yoRefData*)0, (char*)0, (dvoid*)mn);
    mn->info_ydmtn.yort_ydmt_minfo = (yort_proc)yoDuplicate((dvoid*)y);
  }

  yort_implAll__copy( &mn->info_ydmtn.all_ydmt_minfo, all, yotkAllocStr );
  cx->nmtree_ydmtcx++;
  DISCARD ysspEnq( &mn->node_ydmtn, &cx->mtree_ydmtcx );
  ydimActiveSetUsrp( cx->imr_ydmtcx, 
		    mn->info_ydmtn.yort_ydmt_minfo,
		    all->info_yort_implAll.intf_yort_implInfo,
		    all->info_yort_implAll.impl_yort_implInfo,
		    (dvoid*)&mn->info_ydmtn );
  ysClock(&mn->expire_ydmtn);
  sysb8add(&mn->expire_ydmtn, &mn->expire_ydmtn, &cx->reap_ydmtcx);

  rv = (ydmt_info)yoDuplicate((dvoid*)mn->info_ydmtn.self_ydmt_minfo);

  return( rv );
}





STATICF void ydmtReap( ydmtcx *cx )
{
  sysb8	now;
  ysspNode  *n, *next;
  ydmtn *mn;
  yort_implInfo *ip;

  ysClock(&now);
  for( n = ysspFHead( &cx->mtree_ydmtcx ) ; n ; n = next )
  {
    next  = ysspFNext(n);
    mn = (ydmtn*)n->key_ysspNode;
    if( sysb8cmp( &now, >=, &mn->expire_ydmtn ) )
    {
      ip = &mn->info_ydmtn.all_ydmt_minfo.info_yort_implAll;

      
      ysRecord(YS_PRODUCT,YDMT_FAC,(ub4)1463, YSLSEV_INFO, (char*)0,
	       YSLSTR(yduStr(ip->intf_yort_implInfo)),
	       YSLSTR(yduStr(ip->impl_yort_implInfo)), YSLEND );

      ysspRemove( n,  &cx->mtree_ydmtcx );
      cx->nmtree_ydmtcx--;
      ydmt_minfo__free( &mn->info_ydmtn, yotkFreeStr );

      

      yoDispose( (dvoid*)mn->info_ydmtn.self_ydmt_minfo );
      ysmGlbFree( (dvoid*)mn );
    }
  }
}

