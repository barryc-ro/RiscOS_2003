/* mx/src/yd/ydim.c */


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
#ifndef YDYOIDL_ORACLE
#include <ydyoidl.h>
#endif
#ifndef YDIDL_IDL
#include <ydidl.h>
#endif
#ifndef YDIDLI_ORACLE
#include <ydidlI.h>
#endif
#ifndef YOSTD_ORACLE
#include <yostd.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDSP_ORACLE
#include <ydsp.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YDQ_ORACLE
#include <ydq.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif
#ifndef YDCA_ORACLE
#include <ydca.h>
#endif
#ifndef YSMSC_ORACLE
#include <ysmsc.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif




#define YDIM_CHUNK_SIZE	    ((sb4)100)

#define YDIM_FAC	    "YDIM"



typedef struct ydimcx	ydimcx;
typedef struct ydimln	ydimln;
typedef struct ydimn	ydimn;
typedef struct ydimy	ydimy;
typedef struct ydima	ydima;
typedef struct ydimso	ydimso;
typedef struct ydimsp	ydimsp;
typedef struct ydimicx	ydimicx;
typedef struct ydimcicx	ydimcicx;
typedef struct ydimyicx	ydimyicx;
typedef struct ydimghcx	ydimghcx;

externdef ysmtagDecl(ydimcx_tag) = "ydimcx";
externdef ysmtagDecl(ydimln_tag) = "ydimln";
externdef ysmtagDecl(ydimn_tag) = "ydimn";
externdef ysmtagDecl(ydimain_tag) = "ydimain";
externdef ysmtagDecl(ydimy_tag) = "ydimy";
externdef ysmtagDecl(ydima_tag) = "ydima";
externdef ysmtagDecl(ydimso_tag) = "ydimso";
externdef ysmtagDecl(ydimsp_tag) = "ydimsp";
externdef ysmtagDecl(ydimicx_tag) = "ydimicx";
externdef ysmtagDecl(ydimyicx_tag) = "ydimyicx";
externdef ysmtagDecl(ydimcicx_tag) = "ydimcicx";
externdef ysmtagDecl(ydimghcx_tag) = "ydimghcx";


struct ydimln
{
 ysspNode	ydimln_spnode;
 ydimcx	*cx_ydimln;	

 char		*host_ydimln;
 char		*path_ydimln;
 yostd_stringList args_ydimln;

 ysstr		*lock_ydimln;	

 
 yslst		    *rlist_ydimln; 

 

 yslst		    *ilist_ydimln; 
 yslst		    *alist_ydimln; 

 dvoid		    *usrp_ydimln; 
 ydimRdyFunc	    rdy_ydimln; 
 ysevt		    *evt_ydimln; 
};


struct ydimn
{
 

 ysspNode	inode_ydimn;
 ydim_implInfo  info_ydimn;	
 ydimln		*launch_ydimn;	
 ysspTree	otree_ydimn;	
 boolean	update_ydimn;	
};


struct ydimy
{
 ysspNode	    ynode_ydimy;    
 ysspNode	    pnode_ydimy;    
 yort_procInfo	    pinfo_ydimy;    
 ysspTree	    otree_ydimy;    
 ysevt		    *devt_ydimy;    
 ydimcx		    *cx_ydimy;
};


struct ydimicx
{
 ysle		*le_ydimicx;
 ydim_infoIterator self_ydimicx;
 boolean	first_ydimicx;	    
 ydim_implInfo	kinfo_ydimicx;	    
 ydim_implInfo	info_ydimicx;	    
 char		*host_ydimicx;	    
};


struct ydimyicx
{
 ysle	    *le_ydimyicx;
 ydim_yortIterator self_ydimyicx;
 boolean    first_ydimyicx;	
 yort_proc  yort_ydimyicx;	
};


struct ydimcicx
{
  ysle	    *le_ydimcicx;
  ydim_claimIterator self_ydimcicx;
  yort_claim	filter_ydimcicx; 
  ysre		*re_ydimcicx;	
  yort_claim	cursor_ydimcicx; 
  boolean	first_ydimcicx;	
};


struct ydima
{
 yort_procInfo	        *pinfo_ydima;
 ydyo_activeInfoList    *alist_ydima;
};


struct ydimso
{
  ydim_sync self_ydimso;
  ydimSyncFunc	hdlr_ydimso;
  dvoid	    *usrp_ydimso;	
  sb4	    cnt_ydimso;		
};


struct ydimsp
{
  ydim_imr  dest_ydimsp;	
  ydim_sync whenDone_ydimsp;	
  enum
  {
    sendInfo_ydimsp,		
    sendActive_ydimsp,		
    sendTry_ydimsp,		
    sendCommit_ydimsp,		
    sendRep_ydimsp,		
    repComp_ydimsp		
  } state_ydimsp;
  ysevt	    *evt_ydimsp;	

  ydim_info outobj_ydimsp;	
  ydim_implInfo ikey_ydimsp;
  yort_proc ykey_ydimsp;	
  yort_claim claim_ydimsp;	
  ysevt	    *devt_ydimsp;	
  ysevt	    *gsem_ydimsp;	
  yoenv	    env_ydimsp;		
  ydimcx    *cx_ydimsp;
};


struct ydimcx
{
 ydim_imr	self_ydimcx;
 ydmt_imtr	mt_ydimcx;	
 ydsp_spawner	sp_ydimcx;	
 yort_proc	yort_ydimcx;	

 ysque	    *q_ydimcx;		
 yoenv	    ev_ydimcx;		

 ydcacx	    *cacx_ydimcx;	

 
 ysspTree  itree_ydimcx;	

 
 ysspTree  ltree_ydimcx;

 
 ysspTree  atree_ydimcx;	

 
 ysspTree  ytree_ydimcx;	
 ysspTree  ptree_ydimcx;	

 sb4	    chunk_ydimcx;	

 yslst	    *iis_ydimcx;	
 yslst	    *yis_ydimcx;	
 yslst	    *cis_ydimcx;	
 ydimso	    *so_ydimcx;		
 ydimsp	    *asp_ydimcx;	
};

typedef void (*ydimGlobalFunc)( CONST ydim_imr place,
			      CONST dvoid *usrp, yoenv *ev, ydimghcx *gh );


struct ydimghcx
{
 ydimcx    *ydimcx_ydimghcx;	
 dvoid	    *obj_ydimghcx;	
 dvoid	    *outobj_ydimghcx;	
 CONST char *name_ydimghcx;	
 ydim_imr   dest_ydimghcx;	
 yoenv	    ev_ydimghcx;	
 ysevt	    *evt_ydimghcx;	

 ydimGlobalFunc    func_ydimghcx; 
};



STATICF void ydimObjReleaseDispose( dvoid *o );


STATICF sword ydimIImplCmp( CONST dvoid *a, CONST dvoid *b );


STATICF sword ydimAICmp( CONST dvoid *a, CONST dvoid *b );


STATICF sword ydimIIIHWCmp( CONST dvoid *a, CONST dvoid *b );


STATICF void ydimInitInfo( ydim_implInfo *info );
STATICF void ydimInitAinfo( ydyo_activeInfo *ainfo, ydim_implInfo *info );

STATICF void ydimPutGlobal( CONST ydim_imr or, CONST dvoid *obj,
			  CONST ydimGlobalFunc f, CONST char *name );

STATICF void ydimGlobalHandler(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			       size_t argsz);

STATICF void ydimYortDeath(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			   size_t argsz);


STATICF void ydimPutInfo(CONST ydim_imr dest,
			CONST dvoid *usrp,
			yoenv *ev, ydimghcx *gh);
STATICF void ydimDelInfo(CONST ydim_imr dest,
			CONST dvoid *usrp,
			yoenv *ev, ydimghcx *gh);
STATICF void ydimPutActive(CONST ydim_imr dest,
			  CONST dvoid *usrp,
			  yoenv *ev, ydimghcx *gh);
STATICF void ydimDelActive(CONST ydim_imr dest,
			  CONST dvoid *usrp,
			  yoenv *ev, ydimghcx *gh);
STATICF void ydimDelYort(CONST ydim_imr dest,
			CONST dvoid *usrp,
			yoenv *ev, ydimghcx *gh);

STATICF sword ydimYYortCmp( CONST dvoid *a, CONST dvoid *b );
STATICF sword ydimYProcCmp( CONST dvoid *a, CONST dvoid *b );

STATICF void ydimKeySet( ydim_implInfo *dest, ydim_implInfo *src );

STATICF ydimy *ydimAddYort( ydimcx *cx, yort_procInfo *pinfo );

STATICF ydimln *ydimAddLaunch( ydimcx *cx, ydim_implInfo *info );
STATICF void ydimFreeLaunch( ydimln *ln );
STATICF void  ydimLaunchActive( ydimcx *cx, ydimain *ain );
STATICF void ydimLaunchTimeout(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			      size_t argsz);
STATICF void ydimLaunchCheck( ydimln *ln );


STATICF ysspNode *ydimLookup( dvoid *key, ysspTree *t, ysspCmpFunc cmp );
STATICF ysspNode *ydimFirstLookup(dvoid *key, ysspTree *t, ysspCmpFunc cmp);

STATICF ydim_active ydimGetActive( ydimcx *cx, ydyo_activeInfo *ainfo );

STATICF void ydimAddActiveToAlist( ydim_activeList *alist, ydim_active ai );
STATICF void ydimAddObjToTree( ysspTree *otree, dvoid *or );
STATICF void ydimDelObjFromOtree(ysspTree *otree, dvoid *or );
STATICF void ydimDestroyOtree( ysspTree *ht );
STATICF void ydimOtreeToAlist( ysspTree *otree, ydim_activeList *alist,
			      ysmaf af, ysmff ff  );
STATICF void ydimAlistToOtree( ydim_activeList *alist, ysspTree *otree );
STATICF void ydimUpdateAlist( ydimn *on );

STATICF ub4 ysspTreeCount( ysspTree *h );

STATICF sword ydimStringListCmp( CONST dvoid *a, CONST dvoid *b );
STATICF sword ydimLaunchCmp( CONST dvoid *a, CONST dvoid *b );

STATICF yort_procInfo *ydimPInfoOfYort( ydimcx *cx, yort_proc y );

STATICF void ydimDestroyII( dvoid *x );
STATICF void ydimDestroyCI( dvoid *x );
STATICF void ydimDestroyYI( dvoid *x );

STATICF void ydimSyncComplete( dvoid *usrp, CONST ysid *exid,
			      dvoid *argp, size_t argsz );
STATICF void ydimSyncInfo( ydimsp *sp );
STATICF void ydimSyncActive( ydimsp *sp );
STATICF void ydimSyncTry( ydimsp *sp );
STATICF void ydimSyncReply( ydimsp *sp );
STATICF void ydimSyncWait( ydimcx *cx );
STATICF void ydimSyncDeath( dvoid *usrp, CONST ysid *exid,
			   dvoid *argp, size_t argsz );



externdef ysidDecl(YDIM_EX_INTERNAL) = "ydim::internal";

static CONST_W_PTR struct ydim_imr__tyimpl ydim_imr__impl =
 {
  ydim_imr_addInfoLocal_i,
  ydim_imr_addInfoGlobal_i,
  ydim_imr_destroyInfoLocal_i,
  ydim_imr_destroyInfoGlobal_i,
  ydim_imr_addActiveLocal_i,
  ydim_imr_destroyActiveLocal_i,
  ydim_imr_destroyYortLocal_i,
  ydim_imr_yortOfProc_i,
  ydim_imr_procOfYort_i,
  ydim_imr_activeOfYort_i,
  ydim_imr_listImpl_i,
  ydim_imr_listYort_i,
  ydim_imr_createSync_i,
  ydim_imr_startSync_i,
  ydim_imr_abandonFor_i,
  ydim_imr_transfer_i,
  ydim_imr_tryStake_i,
  ydim_imr_transferStake_i,
  ydim_imr_commitStake_i,
  ydim_imr_abortStake_i,
  ydim_imr_listClaim_i,
  ydim_imr__get_self_i,
  ydim_imr__get_mt_i,
  ydim_imr__get_plist_i,
  ydim_imr_addActiveGlobal_i,
  ydim_imr_destroyActiveGlobal_i,
  ydim_imr_destroyYortGlobal_i,
  ydim_imr_exists_i,
  ydim_imr_stake_i,
  ydim_imr_abandon_i,
  ydim_imr_listProperties_i,
  ydim_imr_propertyResolve_i
 };

static CONST_W_PTR struct ydim_info__tyimpl ydim_info__impl =
{
 ydim_info_destroyLocal_i,
 ydim_info_destroyGlobal_i,
 ydim_info__get_info_i
};

static CONST_W_PTR struct ydim_active__tyimpl ydim_active__impl =
{
 ydim_active_destroyLocal_i,
 ydim_active_destroyGlobal_i,
 ydim_active__get_ainfo_i
};

static CONST_W_PTR struct ydim_infoIterator__tyimpl ydim_infoIterator__impl =
 {
  ydim_infoIterator_next_one_i,
  ydim_infoIterator_next_n_i,
  ydim_infoIterator_destroy_i
 };

static CONST_W_PTR struct ydim_yortIterator__tyimpl ydim_yortIterator__impl =
 {
  ydim_yortIterator_next_one_i,
  ydim_yortIterator_next_n_i,
  ydim_yortIterator_destroy_i
 };


static CONST_W_PTR struct ydim_sync__tyimpl ydim_sync__impl =
 {
  ydim_sync_decrement_i,
  ydim_sync_destroy_i,
 };

static CONST_W_PTR struct ydim_claimIterator__tyimpl ydim_claimIterator__impl =
 {
  ydim_claimIterator_next_one_i,
  ydim_claimIterator_next_n_i,
  ydim_claimIterator_destroy_i
 };







void ydimInit( ysque *imq, ydim_imr *oydim_imr )
{
  ydimcx *cx = (ydimcx*)ysmGlbAlloc( sizeof(*cx), ydimcx_tag );
  char	*arg;

  cx->q_ydimcx = imq;
  cx->mt_ydimcx = (ydmt_imtr)0;
  cx->sp_ydimcx = (ydsp_spawner)0;
  yoEnvInit( &cx->ev_ydimcx );

  DISCARD ysspNewTree( &cx->itree_ydimcx, ydimIImplCmp );
  DISCARD ysspNewTree( &cx->ltree_ydimcx, ydimLaunchCmp );
  DISCARD ysspNewTree( &cx->atree_ydimcx, ydimAICmp );
  DISCARD ysspNewTree( &cx->ytree_ydimcx, ydimYYortCmp );
  DISCARD ysspNewTree( &cx->ptree_ydimcx, ydimYProcCmp );

  cx->iis_ydimcx = ysLstCreate();
  cx->cis_ydimcx = ysLstCreate();
  cx->yis_ydimcx = ysLstCreate();
  cx->so_ydimcx = (ydimso*)0;
  cx->asp_ydimcx = (ydimsp*)0;

  cx->cacx_ydimcx = ydcaInit( imq );

  
  yoSetImpl( ydim_imr__id, (char*)0, ydim_imr__stubs,
	    (dvoid*)&ydim_imr__impl, (yoload)0, TRUE, (dvoid*)cx );

  yoSetImpl( ydim_info__id, (char*)0, (yostub*)ydim_info__stubs,
	    (dvoid*)&ydim_info__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( ydim_sync__id, (char*)0, (yostub*)ydim_sync__stubs,
	    (dvoid*)&ydim_sync__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( ydim_active__id, (char*)0, (yostub*)ydim_active__stubs,
	    (dvoid*)&ydim_active__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( ydim_infoIterator__id, (char*)0,
	    ydim_infoIterator__stubs,
	    (dvoid*)&ydim_infoIterator__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( ydim_yortIterator__id, (char*)0,
	    ydim_yortIterator__stubs,
	    (dvoid*)&ydim_yortIterator__impl, (yoload)0, FALSE, (dvoid*)cx );
  yoSetImpl( ydim_claimIterator__id, (char*)0,
	    ydim_claimIterator__stubs,
	    (dvoid*)&ydim_claimIterator__impl, (yoload)0, FALSE, (dvoid*)cx );

  
  cx->self_ydimcx = (ydim_imr)yoCreate( ydim_imr__id, (char*)0,
				       (yoRefData*)0, (char *)0, (dvoid *)0);

  arg = ysResGetLast("ydim.chunksize");
  cx->chunk_ydimcx = arg ? atoi(arg) : YDIM_CHUNK_SIZE;
  if( cx->chunk_ydimcx <= 0 ) cx->chunk_ydimcx = 1;

  cx->yort_ydimcx = (yort_proc)yoYort();

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1250,YSLSEV_INFO,(char*)0, YSLNONE );

  *oydim_imr = (ydim_imr)yoDuplicate((dvoid*)cx->self_ydimcx);

  yoImplReady( ydim_imr__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_info__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_sync__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_active__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_infoIterator__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_yortIterator__id, (char*)0, cx->q_ydimcx );
  yoImplReady( ydim_claimIterator__id, (char*)0, cx->q_ydimcx );
}

STATICF void ydimDestroyII( dvoid *x )
{
  ydimicx *myii = (ydimicx*)x;

  ydim_implInfo__free( &myii->kinfo_ydimicx, yotkFreeStr);
  ydim_implInfo__free( &myii->info_ydimicx, yotkFreeStr);
  yduFreeCacheStr(myii->host_ydimicx);
  ydimObjReleaseDispose( (dvoid*)myii->self_ydimicx );
  ysmGlbFree( (dvoid*)myii );
}

STATICF void ydimDestroyCI( dvoid *x )
{
  ydimcicx *cix = (ydimcicx*)x;

  if( !cix->first_ydimcicx )
    yort_claim__free( &cix->cursor_ydimcicx, yotkFreeStr );
  yort_claim__free( &cix->filter_ydimcicx, yotkFreeStr );
  if( cix->re_ydimcicx )
    ysREFree( cix->re_ydimcicx );
  ydimObjReleaseDispose( (dvoid*)cix->self_ydimcicx );
  ysmGlbFree( (dvoid*)cix );
}

STATICF void ydimDestroyYI( dvoid *x )
{
  ydimyicx *myyi = (ydimyicx*)x;

  ydimObjReleaseDispose( (dvoid*)myyi->self_ydimyicx );
  if( myyi->yort_ydimyicx )
    yoRelease( (dvoid*)myyi->yort_ydimyicx );
  ysmGlbFree( (dvoid*)myyi );
}

void ydimTerm( ydim_imr imr )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);
  ysspNode  *n;			
  ydimy     *ytn;		
  ydimn	    *itn;		
  ydimain   *ain;		
  yoenv	    ev;

  yoEnvInit(&ev);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1251,YSLSEV_INFO,(char*)0, YSLNONE );

  if( cx->mt_ydimcx )
    yoRelease( (dvoid*)cx->mt_ydimcx );
  if( cx->sp_ydimcx )
    yoRelease( (dvoid*)cx->sp_ydimcx );
  yoRelease( (dvoid*)cx->yort_ydimcx );

  yoImplDeactivate( ydim_infoIterator__id, (char*)0 );
  yoImplDeactivate( ydim_yortIterator__id, (char*)0 );
  yoImplDeactivate( ydim_claimIterator__id, (char*)0 );
  yoImplDeactivate( ydim_info__id, (char*)0 );
  yoImplDeactivate( ydim_sync__id, (char*)0 );
  yoImplDeactivate( ydim_imr__id, (char*)0 );

  ysLstDestroy( cx->cis_ydimcx, ydimDestroyCI );
  ysLstDestroy( cx->iis_ydimcx, ydimDestroyII );
  ysLstDestroy( cx->yis_ydimcx, ydimDestroyYI );

  if( cx->so_ydimcx )
  {
    ;
    
  }
  if( cx->asp_ydimcx )
  {
    ;
    
  }

  ydcaTerm( cx->cacx_ydimcx );

  yoImplDeactivate( ydim_yortIterator__id, (char*)0 );
  yoImplDeactivate( ydim_infoIterator__id, (char*)0 );

  while( (n = ysspDeqTree( &cx->ltree_ydimcx ) ) )
    ydimFreeLaunch( (ydimln*)n->key_ysspNode );

  
  while( (n = ysspDeqTree( &cx->itree_ydimcx ) ) )
  {
    itn = (ydimn*)n;
    ydimDestroyOtree( &itn->otree_ydimn );
    ydim_implInfo__free( &itn->info_ydimn, yotkFreeStr);
    yoDispose( (dvoid*)itn->info_ydimn.self_ydim_implInfo );
    ysmGlbFree( (dvoid*)itn );
  }

  
  while( (n = ysspDeqTree( &cx->ytree_ydimcx ) ) )
  {
    ytn = (ydimy*)n->key_ysspNode;
    
    ysspRemove( &ytn->pnode_ydimy, &cx->ptree_ydimcx );

    
    ysEvtDestroy( ytn->devt_ydimy );
    yort_procInfo__free( &ytn->pinfo_ydimy, yotkFreeStr);
    ydimDestroyOtree( &ytn->otree_ydimy );
    ysmGlbFree( (dvoid*)ytn );
  }

  
  while( ( n = ysspDeqTree( &cx->atree_ydimcx ) ) )
  {
    ain = (ydimain*)n;
    ydyo_activeInfo__free( &ain->ainfo_ydimain, yotkFreeStr);
    yoDispose( (dvoid*)ain->ainfo_ydimain.self_ydyo_activeInfo );
    ysmGlbFree( (dvoid*)ain );
  }

  yoEnvFree( &cx->ev_ydimcx );
  ydimObjReleaseDispose( (dvoid*)cx->self_ydimcx );
  ysmGlbFree( (dvoid*)cx );
  yoEnvFree(&ev);
}

void ydimSetRefs( ydim_imr imr, ydmt_imtr mt, ydsp_spawner sp )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);

  cx->mt_ydimcx = (ydmt_imtr)yoDuplicate( (dvoid*)mt );
  cx->sp_ydimcx = (ydsp_spawner)yoDuplicate( (dvoid*)sp );
}








yort_proc ydimStartLaunch( ydim_imr imr, ydim_implInfo *info, sysb8 *timeout,
			  dvoid *usrp, ydimRdyFunc rdy, dvoid *element )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);
  ysspNode  *n = ysspLookup( (dvoid*)info, &cx->itree_ydimcx );
  ydimln    *ln;
  ydimn	    *in;
  ydimain   *ain;
  ydyo_activeInfo fake;
  ydim_active a;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1252,YSLSEV_INFO,(char*)0,
	   YSLSTR(yduStr(info->intf_ydim_implInfo)),
	   YSLSTR(yduStr(info->impl_ydim_implInfo)),
	   YSLSTR(yduStr(info->host_ydim_implInfo)), YSLEND );

  in = (ydimn*)n;
  info = (ydim_implInfo*)n->key_ysspNode;
  ydimInitAinfo( &fake, info );
  ln = in->launch_ydimn;
  if( ln )			
  {
    if( !ln->rlist_ydimln )	
    {
      
      ln->usrp_ydimln = usrp;
      ln->rdy_ydimln = rdy;
      ln->rlist_ydimln = ysLstCreate();
      ln->ilist_ydimln = ysLstCreate();
      ln->alist_ydimln = ysLstCreate();

      

      fake.yort_ydyo_activeInfo = (yort_proc)0;
      for( n = ysspFHead( &cx->itree_ydimcx ) ; n ; n = ysspFNext( n ) )
      {
	in = (ydimn*)n;
	if( in->launch_ydimn == ln )
	{
	  info = (ydim_implInfo*)n->key_ysspNode;
	  fake.intf_ydyo_activeInfo = info->intf_ydim_implInfo;
	  fake.impl_ydyo_activeInfo = info->impl_ydim_implInfo;
	  a = ydimGetActive( cx, &fake );
	  ydimAddActiveToAlist( &info->alist_ydim_implInfo, a );
	  ain = (ydimain*)yoGetState( (dvoid*)a );
	  
	  ysmCheck( ain, ydimain_tag );
	  yoRelease( (dvoid*)a );

	  
	  DISCARD ysLstEnq( ln->ilist_ydimln, (dvoid*)&ain->ainfo_ydimain );

	  
	  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1253,YSLSEV_INFO,(char*)0,
		   YSLSTR(yduStr(info->intf_ydim_implInfo)),
		   YSLSTR(yduStr(info->impl_ydim_implInfo)),
		   YSLSTR(yduStr(info->host_ydim_implInfo)), YSLEND);
	}
      }
      DISCARD ysLstEnq( ln->rlist_ydimln, element );

      
      ln->evt_ydimln =
	ysEvtCreate( ydimLaunchTimeout, (dvoid*)ln, (ysque*)0, TRUE );
      ysTimer( timeout, ln->evt_ydimln );
    }
  }
  else				
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1254,YSLSEV_EMERG,(char*)0,
	     YSLSTR(yduStr(info->intf_ydim_implInfo)),
	     YSLSTR(yduStr(info->impl_ydim_implInfo)),
	     YSLSTR(yduStr(info->host_ydim_implInfo)), YSLEND);
    ysePanic(YDIM_EX_INTERNAL);
  }

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1255,YSLSEV_DEBUG(3),(char*)0, YSLNONE);

  return( (yort_proc)yoDuplicate((dvoid*)cx->self_ydimcx) );
}



void ydimAppendLaunch( ydim_imr imr, ydim_implInfo *info, dvoid *element )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);
  ysspNode  *n;
  ydimln    *ln;
  ydimn	    *in;

  n = ysspLookup( (dvoid*)info, &cx->itree_ydimcx );
  in = (ydimn*)n;
  info = (ydim_implInfo*)n->key_ysspNode;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1256,YSLSEV_DEBUG(2),(char*)0,
	   YSLSTR(yduStr(info->intf_ydim_implInfo)),
	   YSLSTR(yduStr(info->impl_ydim_implInfo)),
	   YSLSTR(info->host_ydim_implInfo), YSLEND );

  
  if( (ln = in->launch_ydimn) )
  {
    if( ln->rlist_ydimln )
      DISCARD ysLstEnq( ln->rlist_ydimln, element );
    else
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1257,YSLSEV_EMERG,(char*)0, YSLNONE );
      ysePanic(YDIM_EX_INTERNAL);
    }
  }
}


boolean ydimIsLaunching( ydim_imr imr, yort_proc y )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);

  return( yoIsEq( (dvoid*)cx->self_ydimcx, (dvoid*)y ) );
}






ydimain *ydimActiveGetAinfo( CONST ydim_active or )
{
  ydimain   *ain = (ydimain*)yoGetState((dvoid *)or);
  return( ain );
}


void ydimActiveSetUsrp( CONST ydim_imr imr, yort_proc y,
		       char *intf, char *impl, dvoid *usrp )
{
  ydimcx *cx = (ydimcx*)yoGetImplState( (dvoid*)imr );
  ydyo_activeInfo    lainfo;
  ydimain *ain;

  lainfo.intf_ydyo_activeInfo = intf;
  lainfo.impl_ydyo_activeInfo = impl;
  lainfo.yort_ydyo_activeInfo = y;

  if( (ain = (ydimain*)ysspLookup( (dvoid *)&lainfo, &cx->atree_ydimcx ) ) )
    ain->usrp_ydimain = usrp;
}

yslst *ydimListImpl( yslst *lst, ydim_imr imr,
		    char *intf, char *impl, char *host )
{

  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)imr);

  ydim_implInfo  lookup;
  ysspNode  *n;

  lookup.intf_ydim_implInfo = intf;
  lookup.impl_ydim_implInfo = impl;
  lookup.host_ydim_implInfo = host;
  lookup.pathName_ydim_implInfo = (char*)0;

  n = ydimFirstLookup((dvoid*)&lookup, &cx->itree_ydimcx, ydimIIIHWCmp);
  while( n && !ydimIIIHWCmp( (dvoid*)&lookup, n->key_ysspNode ) )
  {
    ydimUpdateAlist( (ydimn*)n );
    DISCARD ysLstEnq( lst, n->key_ysspNode );
    n = ysspFNext( n );
  }
  return( lst );
}







ydim_imr ydim_imr__get_self_i( ydim_imr or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  return( (ydim_imr)yoDuplicate((dvoid*)cx->self_ydimcx) );
}


ydmt_imtr ydim_imr__get_mt_i( ydim_imr or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  return( (ydmt_imtr)yoDuplicate((dvoid*)cx->mt_ydimcx) );
}



void ydim_imr_addInfoGlobal_i( ydim_imr or, yoenv *ev,
			      ydim_implInfo* info, ydim_info* infoObj)
{
  ysspNode  *n;
  ydim_implInfo  *o;
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1258,YSLSEV_INFO,(char*)0, YSLNONE );

  

  ydimPutGlobal( or, (dvoid *)info, (CONST ydimGlobalFunc)ydimPutInfo,
		"ydimPutInfo" );

  
  if( (n = ysspLookup( (dvoid *)info, &cx->itree_ydimcx ) ) )
  {
    o = (ydim_implInfo*)n->key_ysspNode;
    *infoObj = (ydim_info)yoDuplicate((dvoid*)o->self_ydim_implInfo);
  }
  else
    *infoObj = (ydim_info)0;
}




void ydim_imr_addInfoLocal_i( ydim_imr or, yoenv *ev,
			     ydim_implInfo* info, ydim_info* infoObj)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ysspNode  *n;			
  ydimn	    *on;		
  ydim_implInfo  *infon;	
  ydyo_activeInfo ainfo;	
  boolean   isnew;
  ydim_active	a;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1259,YSLSEV_INFO,(char*)0, YSLNONE );

  ydimSyncWait( cx );		

  
  if( (n = ysspLookup( (dvoid *)info, &cx->itree_ydimcx ) ) )
  {
    on = (ydimn*)n;
    infon = (ydim_implInfo*)n->key_ysspNode; 
    isnew = FALSE;
  }
  else				
  {
    isnew = TRUE;
    on = (ydimn*) ysmGlbAlloc( sizeof(*on), ydimn_tag );
    on->inode_ydimn.key_ysspNode = (dvoid *)(infon = &on->info_ydimn);
    on->launch_ydimn = (ydimln*)0;
    on->update_ydimn = TRUE;
    DISCARD ysspNewTree( &on->otree_ydimn, yoCmp );

    
    ydim_implInfo__copy( infon, info, yotkAllocStr );
    ydimAlistToOtree( &info->alist_ydim_implInfo, &on->otree_ydimn );

    
    infon->self_ydim_implInfo =
      (ydim_info)yoCreate( ydim_info__id, (char*)0,
			  (yoRefData*)0,
			  (char *)0, (dvoid *)infon);

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1260,YSLSEV_DEBUG(4),(char*)0, YSLNONE);

    DISCARD ysspEnq( &on->inode_ydimn, &cx->itree_ydimcx );
  }

  

  infon->minInstances_ydim_implInfo = info->minInstances_ydim_implInfo;
  infon->maxInstances_ydim_implInfo = info->maxInstances_ydim_implInfo;

  
  if( !isnew && infon->pathName_ydim_implInfo && info->pathName_ydim_implInfo )
  {
    yostd_stringList__free(&infon->args_ydim_implInfo, yotkFreeStr);
  }

  
  if( !isnew && info->pathName_ydim_implInfo )
  {
    yduFreeCopyStr( &infon->pathName_ydim_implInfo,
		   info->pathName_ydim_implInfo, yotkFreeStr, yotkAllocStr );
    yostd_stringList__copy( &infon->args_ydim_implInfo,
			   &info->args_ydim_implInfo, yotkAllocStr );
  }

  
  if( info->pathName_ydim_implInfo && !on->launch_ydimn )
  {
    
    ydimInitAinfo( &ainfo, infon );
    a = ydimGetActive( cx, &ainfo );
    ydimAddObjToTree( &on->otree_ydimn, (dvoid*)a);
    on->update_ydimn = TRUE;
    on->launch_ydimn = ydimAddLaunch( cx, infon );
    yoRelease( (dvoid*)a );
  }

  

  yostd_stringList__free( &infon->args_ydim_implInfo, yotkFreeStr);
  yostd_stringList__copy( &infon->args_ydim_implInfo,
			 &info->args_ydim_implInfo, yotkAllocStr );

  *infoObj = (ydim_info)yoDuplicate((dvoid*)infon->self_ydim_implInfo);
}



void ydim_imr_destroyInfoLocal_i( ydim_imr or, yoenv *ev,
				 ydim_implInfo* info)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ysspNode  *n;
  ydim_implInfo  *ninfo;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1261,YSLSEV_INFO,(char*)0, YSLNONE );

  ydimSyncWait( cx );		

  
  if( (n = ysspLookup( (dvoid *)info, &cx->itree_ydimcx ) ) )
  {
    
    ninfo = (ydim_implInfo*)n->key_ysspNode;

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1262,YSLSEV_INFO,(char*)0, YSLEND );

    ysspRemove( n, &cx->itree_ydimcx );
    ydim_implInfo__free( ninfo, yotkFreeStr);
    yoDispose( (dvoid*)ninfo->self_ydim_implInfo );
    ysmGlbFree( (dvoid *)n );
  }
}



void ydim_imr_destroyInfoGlobal_i( ydim_imr or, yoenv *ev,
				  ydim_implInfo* info)
{
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1263,YSLSEV_INFO,(char*)0, YSLNONE);

  ydimPutGlobal(or, (dvoid *)info, (CONST ydimGlobalFunc)ydimDelInfo,
		"ydimDelInfo");
}







void ydim_imr_addActiveGlobal_i( ydim_imr or, yoenv *ev,
				yort_procInfo* pinfo,
				ydyo_activeInfoList* list)

{
  ydima	    arg;
  arg.pinfo_ydima = pinfo;
  arg.alist_ydima = list;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1264,YSLSEV_INFO,(char*)0,
	   YSLSTR(pinfo->host_yort_procInfo),
	   YSLSTR(pinfo->pid_yort_procInfo),
	   YSLSTR(yduStr(pinfo->affinity_yort_procInfo)), YSLEND);

  
  ydimPutGlobal(or,(dvoid*)&arg,(CONST ydimGlobalFunc)ydimPutActive,
		"ydimPutActive");
}



void ydim_imr_addActiveLocal_i( ydim_imr or, yoenv *ev,
			       yort_procInfo* pinfo,
			       ydyo_activeInfoList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydim_active	a;

  ub4		i;		
  ysspNode	*n;		
  ydim_implInfo	*info;		
  ydim_implInfo	linfo;		
  ydim_info	outobj;
  ydimn		*in;
  ydimy		*y;

  ydyo_activeInfo    *ainfo;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1265,YSLSEV_INFO,(char*)0,YSLNONE );

  ydimSyncWait( cx );		

  
  y = ydimAddYort( cx, pinfo );
  for( i = 0 ; i < list->_length ; i++ )
  {
    ainfo = &list->_buffer[i];

    

    if( !strcmp( ainfo->intf_ydyo_activeInfo, ysidToStr(yort_proc__id) ) )
      continue;

    ydimInitInfo( &linfo );
    linfo.intf_ydim_implInfo = ainfo->intf_ydyo_activeInfo;
    linfo.impl_ydim_implInfo = ainfo->impl_ydyo_activeInfo;
    linfo.host_ydim_implInfo = pinfo->host_yort_procInfo;

    
    if((n = ydimFirstLookup((dvoid*)&linfo, &cx->itree_ydimcx,
			    ydimIIIHWCmp)))
    {
      
      in = (ydimn*)n;
      info = (ydim_implInfo *)n->key_ysspNode;
    }
    else
    {
      
      ydim_imr_addInfoLocal_i( or, ev, &linfo, &outobj );
      info = (ydim_implInfo *)yoGetState( (dvoid *)outobj );
      in = (ydimn*)ysspLookup( (dvoid*)info, &cx->itree_ydimcx );
      yoRelease( (dvoid*)outobj );
    }

    a = ydimGetActive( cx, ainfo );
    ydimAddObjToTree( &y->otree_ydimy, (dvoid*)a );
    ydimAddObjToTree( &in->otree_ydimn, (dvoid*)a );
    in->update_ydimn = TRUE;
    yoRelease( (dvoid*)a );
  }
}



void ydim_imr_destroyActiveLocal_i( ydim_imr or, yoenv *ev,
				   ydyo_activeInfoList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ub4	    i;			
  ysspNode  *n;			
  ydimn	    *on;
  ydim_implInfo  linfo;
  ydyo_activeInfo    *ainfo;
  ydimy	    lookup;
  yort_proc y = (yort_proc)0;
  ydim_active	a;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1266,YSLSEV_INFO,(char*)0, YSLNONE );

  ydimSyncWait( cx );		

  ydimInitInfo( &linfo );
  for( i = 0 ; i < list->_length ; i++ )
  {
    ainfo = &list->_buffer[i];
    y = ainfo->yort_ydyo_activeInfo;

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1267,YSLSEV_DEBUG(3),(char*)0,
	     YSLSTR(ainfo->intf_ydyo_activeInfo),
	     YSLSTR(yduStr(ainfo->impl_ydyo_activeInfo)), YSLEND );

    
    lookup.pinfo_ydimy.self_yort_procInfo = y;
    if( !ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) )
      continue;

    yduFreeCopyStr(&linfo.intf_ydim_implInfo, ainfo->intf_ydyo_activeInfo,
		   yotkFreeStr, yotkAllocStr);
    yduFreeCopyStr(&linfo.impl_ydim_implInfo, ainfo->impl_ydyo_activeInfo,
		   yotkFreeStr, yotkAllocStr);

    
    if( !strcmp(ysidToStr(yort_proc__id), ainfo->intf_ydyo_activeInfo) )
    {
      
      yduFreeCopyStr(&linfo.host_ydim_implInfo, ainfo->impl_ydyo_activeInfo,
		     yotkFreeStr, yotkAllocStr);
      *(strchr( linfo.host_ydim_implInfo, ':')) = 0;

      
      ydim_imr_destroyYortLocal_i( or, ev, y );
      
      ydim_imr_destroyInfoLocal_i( or, ev, &linfo );
      yduFreeCacheStr( linfo.host_ydim_implInfo );
      linfo.host_ydim_implInfo = (char*)0;
    }

    
    for((n = ydimFirstLookup((dvoid*)&linfo, &cx->itree_ydimcx, ydimIIIHWCmp));
	n && !ydimIIIHWCmp( (dvoid*)&linfo, n->key_ysspNode );
	n = ysspFNext( n ) )
    {
      
      on = (ydimn*)ysspLookup( n->key_ysspNode, &cx->itree_ydimcx );
      a = ydimGetActive( cx, ainfo );
      ydimDelObjFromOtree( &on->otree_ydimn, (dvoid*)a );
      yoRelease( (dvoid*)a );
      on->update_ydimn = TRUE;
    }
  }
  ydim_implInfo__free( &linfo, yotkFreeStr);
}



void ydim_imr_destroyActiveGlobal_i( ydim_imr or, yoenv *ev,
				ydyo_activeInfoList* list)
{
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1268,YSLSEV_INFO,(char*)0, YSLNONE);

  ydimPutGlobal(or,(dvoid*)list,(CONST ydimGlobalFunc)ydimDelActive,
		"ydimDelActive");
}



yort_proc ydim_imr_yortOfProc_i( ydim_imr or, yoenv *ev,
				char* host, char* pid, char* affinity)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimy	    lookup;
  ydimy	    *y;
  ysspNode  *n;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1269,YSLSEV_INFO,(char*)0, YSLNONE);

  CLRSTRUCT(lookup);
  lookup.pinfo_ydimy.host_yort_procInfo = host;
  lookup.pinfo_ydimy.pid_yort_procInfo = pid;
  lookup.pinfo_ydimy.affinity_yort_procInfo = affinity;

  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ptree_ydimcx ) ) )
  {
    y = (ydimy*)n->key_ysspNode;
    return((yort_proc)yoDuplicate((dvoid*)y->pinfo_ydimy.self_yort_procInfo));
  }
  return( (yort_proc)0 );
}







boolean ydim_imr_procOfYort_i( ydim_imr or, yoenv *ev, yort_proc y,
			  yort_procInfo* pinfo)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  yort_procInfo	*lpinfo;

  if( (lpinfo = ydimPInfoOfYort( cx, y ) ) )
  {
    yort_procInfo__copy( pinfo, lpinfo, yoAlloc );
    return( TRUE );
  }
  return FALSE;
}




boolean ydim_imr_activeOfYort_i( ydim_imr or, yoenv *ev, yort_proc yr,
			  ydim_activeList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimy	    *y = (ydimy*)0;
  ydimy	    lookup;
  boolean   rv = FALSE;
  ysspNode  *n;

  lookup.pinfo_ydimy.self_yort_procInfo = yr;

  list->_length = list->_maximum = 0;
  list->_buffer = (ydim_active*)0;
  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) ) )
  {
    y = (ydimy*)n->key_ysspNode;
    ydimOtreeToAlist( &y->otree_ydimy, list, yoAlloc, yoFree );
    rv = TRUE;
  }
  return( rv );
}


void ydim_imr_destroyYortLocal_i( ydim_imr or, yoenv *ev, yort_proc yr)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ysspNode  *n, *an;
  ydimy	    *y;
  ydimy	    lookup;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1270,YSLSEV_INFO,(char*)0, YSLNONE );

  ydimSyncWait( cx );		

  lookup.pinfo_ydimy.self_yort_procInfo = yr;

  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) ) )
  {
    y = (ydimy*)n->key_ysspNode;

    
    while( (an = ysspDeqTree( &y->otree_ydimy ) ) )
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1271,YSLSEV_DEBUG(3),(char*)0,YSLNONE);

      ydim_active_destroyLocal_i( (ydim_active)an->key_ysspNode, ev );
      yoRelease( an->key_ysspNode );
      ysmGlbFree( (dvoid*)an );
    }

    
    ysEvtDestroy( y->devt_ydimy );

    ysspRemove( n, &cx->ytree_ydimcx );
    ysspRemove( &y->pnode_ydimy, &cx->ptree_ydimcx );
    yort_procInfo__free( &y->pinfo_ydimy, yotkFreeStr);
    ysmGlbFree( (dvoid *)y );
  }
}



void ydim_imr_destroyYortGlobal_i( ydim_imr or, yoenv *ev, yort_proc y)
{
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1272,YSLSEV_INFO,(char*)0, YSLNONE);

  ydimPutGlobal(or,(dvoid*)y,(CONST ydimGlobalFunc)ydimDelYort,
		"ydimDelYort" );
}






boolean ydim_imr_exists_i( ydim_imr or, yoenv *ev, char *intf, char *impl )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydim_infoList ilist;
  ydim_infoIterator ii;
  ydim_activeList *alist;
  ydimain *ain;
  ydyo_activeInfo *ainfo;
  ub4 i, j;
  boolean rv = FALSE;
  
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1273,YSLSEV_INFO,(char*)0,
	   YSLSTR(intf), YSLSTR(yduStr(impl)), YSLEND );

  ydim_imr_listImpl( or, ev, cx->chunk_ydimcx, intf, impl, (char*)0,
		    &ilist, &ii );

  while( !rv && ilist._length )
  {
    for( i = 0 ; !rv && i < ilist._length ; i++ )
    {
      
      alist = &ilist._buffer[i].alist_ydim_implInfo; 
      for( j = 0 ; j < alist->_length ; j++ )
      {
	ain = ydimActiveGetAinfo( alist->_buffer[j] );
	ainfo = &ain->ainfo_ydimain;
	if( !bit( ainfo->implFlags_ydyo_activeInfo, yort_suspended_implFlag))
	{
	  rv = TRUE;
	  break;
	}
      }
    }
    ydim_infoList__free( &ilist, yoFree );
    if( !rv && ii )
      DISCARD ydim_infoIterator_next_n_i(ii, ev, cx->chunk_ydimcx,&ilist);
    else
      break;
  }
  if( ii )
    ydim_infoIterator_destroy_i( ii, ev );

  return( rv );
}




void ydim_imr_listImpl_i( ydim_imr or, yoenv *ev, sb4 count,
		     char* intf, char* impl,
		     char* host,
		     ydim_infoList* list,
		     ydim_infoIterator* ii)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimicx   *myii;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1274,YSLSEV_INFO,(char*)0,
	   YSLSB4( count ), YSLEND );

  list->_length = list->_maximum = 0;
  list->_buffer = (ydim_implInfo*)0;

  myii = (ydimicx*)ysmGlbAlloc( sizeof(*myii), ydimicx_tag );
  myii->first_ydimicx = TRUE;

  ydimInitInfo( &myii->kinfo_ydimicx );
  ydimInitInfo( &myii->info_ydimicx );

  myii->kinfo_ydimicx.intf_ydim_implInfo = yduCopyCacheStr( intf );
  myii->kinfo_ydimicx.impl_ydim_implInfo = yduCopyCacheStr( impl );
  myii->host_ydimicx = yduCopyCacheStr( host );

  *ii = (ydim_infoIterator)yoCreate( ydim_infoIterator__id,
				    (char*)0,
				    (yoRefData*)0,
				    (char *)0, (dvoid *)myii );
  myii->self_ydimicx = *ii;
  myii->le_ydimicx = ysLstEnq( cx->iis_ydimcx, (dvoid*)myii );

  if( count && (!ydim_infoIterator_next_n_i( *ii, ev, count, list ) ||
		list->_length != (ub4)count) )
  {
    ydim_infoIterator_destroy_i( *ii, ev );
    *ii = (ydim_infoIterator)0;
  }
  if( *ii )
    *ii = (ydim_infoIterator)yoDuplicate((dvoid*)*ii);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1275,YSLSEV_DEBUG(3),(char*)0,
	   YSLSTR( *ii ? "TRUE" : "FALSE"), YSLEND );
}



void ydim_imr_listYort_i( ydim_imr or, yoenv *ev, sb4 count, yort_proc y,
			 ydim_yortList* ylist, ydim_yortIterator* yi)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimyicx  *myyi;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1276,YSLSEV_INFO,(char*)0,
	   YSLSB4( count ), YSLEND );

  ylist->_length = ylist->_maximum = 0;
  ylist->_buffer = (yort_proc*)0;

  myyi = (ydimyicx*)ysmGlbAlloc( sizeof(*myyi), ydimyicx_tag );
  myyi->first_ydimyicx = TRUE;
  myyi->yort_ydimyicx = (yort_proc)yoDuplicate((dvoid*)y);

  *yi = (ydim_yortIterator)yoCreate( ydim_yortIterator__id,
				    (char*)0,
				    (yoRefData*)0,
				    (char *)0, (dvoid *)myyi );
  myyi->self_ydimyicx = *yi;
  myyi->le_ydimyicx = ysLstEnq( cx->yis_ydimcx, (dvoid*)myyi );

  if( count && (!ydim_yortIterator_next_n_i( *yi, ev, count, ylist ) ||
     ylist->_length != (ub4)count) )
  {
    ydim_yortIterator_destroy_i( *yi, ev );
    *yi = (ydim_yortIterator)0;
  }
  if( *yi )
    *yi = (ydim_yortIterator)yoDuplicate((dvoid*)*yi);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1277,YSLSEV_DEBUG(3),(char*)0,
	   YSLSTR( *yi ? "TRUE" : "FALSE"), YSLEND );
}



ydim_sync ydim_imr_createSync_i( ydim_imr or, yoenv* ev, sb4 cnt)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimso    *so = (ydimso*)ysmGlbAlloc( sizeof(*so), ydimso_tag );

  if( cx->so_ydimcx )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1278,YSLSEV_EMERG,(char*)0, YSLNONE );
    ysePanic(YDIM_EX_INTERNAL);
  }

  so->self_ydimso = (ydim_sync)yoCreate( ydim_sync__id, (char*)0,
					(yoRefData*)0,  (char*)0, (dvoid*)so );
  so->cnt_ydimso = cnt;
  so->hdlr_ydimso = (ydimSyncFunc)0;
  so->usrp_ydimso = (dvoid*)0;
  cx->so_ydimcx = so;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1279,YSLSEV_INFO,(char*)0,
	   YSLSB4(so->cnt_ydimso), YSLEND );

  return( (ydim_sync)yoDuplicate( (dvoid*)so->self_ydimso ) );
}

void ydimSetSyncHandler( ydim_sync or, ydimSyncFunc hdlr, dvoid *usrp )
{
  ydimso    *so = (ydimso*)yoGetState((dvoid*)or );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1280,YSLSEV_INFO,(char*)0, YSLNONE);

  so->hdlr_ydimso = hdlr;
  so->usrp_ydimso = usrp;
}



void ydim_imr_startSync_i( ydim_imr or, yoenv* ev, ydim_imr dest,
			  ydim_sync whenDone)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimsp    *sp = (ydimsp*)ysmGlbAlloc( sizeof(*sp), ydimsp_tag );
  ysevt	*evt;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1281,YSLSEV_INFO,(char*)0, YSLNONE );

  
  if( cx->asp_ydimcx )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1282,YSLSEV_EMERG,(char*)0, YSLNONE );
    ysePanic(YDIM_EX_INTERNAL);
  }

  sp->ykey_ydimsp = (yort_proc)0;
  sp->dest_ydimsp = (ydim_imr)yoDuplicate( (dvoid*)dest );
  sp->whenDone_ydimsp = (ydim_sync)yoDuplicate( (dvoid*)whenDone );
  sp->outobj_ydimsp = (ydim_info)0;
  sp->claim_ydimsp.property = (char*)0;
  sp->claim_ydimsp.owner = (yort_proc)0;
  sp->claim_ydimsp.obj = (CORBA_Object)0;
  sp->devt_ydimsp = (ysevt*)0;
  sp->evt_ydimsp = (ysevt*)0;
  sp->gsem_ydimsp = (ysevt*)0;
  sp->cx_ydimsp = cx;
  sp->state_ydimsp = sendInfo_ydimsp;
  ydimInitInfo( &sp->ikey_ydimsp );
  yoEnvInit( &sp->env_ydimsp );

  
  cx->asp_ydimcx = sp;

  if( yoIsEq( (dvoid*)or, (dvoid*)dest ) )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1283,YSLSEV_EMERG,(char*)0, YSLNONE);
    ysePanic( YDIM_EX_INTERNAL );
  }
  else				
  {
    
    evt = ysSemCreate( (dvoid*)0 );
    ydcaSetInactiveEvt( cx->cacx_ydimcx, evt );

    while( !ysSemTest( evt ) )
    {
      ysSvcAll( cx->q_ydimcx );
      ysYield();
    }
    ysSemDestroy( evt );

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1284,YSLSEV_DEBUG(3),(char*)0, YSLNONE);

    
    sp->devt_ydimsp = ysEvtCreate(ydimSyncDeath, (dvoid*)sp,
				  cx->q_ydimcx, TRUE);
    yoWatchOwner( (dvoid*)sp->dest_ydimsp, sp->devt_ydimsp );

    sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
				 (ysque*)0, TRUE );
    ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
}



void ydim_imr_stake_i( ydim_imr or, yoenv* ev, yort_claim* what,
		      yoevt replyTo)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydimSyncWait( cx );		

  ydcaStakeFor( cx->cacx_ydimcx, what, replyTo );
}


void ydim_imr_abandon_i( ydim_imr or, yoenv* ev, yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydimSyncWait( cx );		

  ydcaAbandonFor( cx->cacx_ydimcx, what, FALSE );
}


void ydim_imr_transfer_i( ydim_imr or, yoenv* ev, yort_claim* newClaim)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydimSyncWait( cx );

  ydcaTransfer( cx->cacx_ydimcx, newClaim );
}



yostd_stringList ydim_imr_listProperties_i( ydim_imr or, yoenv* ev,
					   char* regexp )
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  yslst	    *l;
  ysre	    *re;
  boolean   first = TRUE;
  char	    *s;
  yostd_stringList list;
  yort_claim	*now;
  yort_claim	cursor;

  l = ysLstCreate();
  re = regexp ? ysRECompile( regexp, FALSE ) : (ysre*)0;
  cursor.owner = (yort_proc)0;
  cursor.obj = (CORBA_Object)0;

  
  while( (now = ydcaListNext(cx->cacx_ydimcx,first?(yort_claim*)0: &cursor)) )
  {
    cursor.property = now->property;
    first = FALSE;
    if( (!re || ysREMatch( re, now->property, TRUE )))
      DISCARD ysLstEnq( l, (dvoid*)now->property );
  }

  
  list._length = 0;
  list._maximum = (ub4)ysLstCount( l );
  list._buffer =
    (char**)yoAlloc( (size_t)list._maximum * sizeof(char*));

  while( (s = (char*)ysLstDeq( l ) ) )
    list._buffer[ list._length++ ] = ysStrDupWaf( s, yoAlloc );

  ysLstDestroy( l, (ysmff)0 );
  if( re )
    ysREFree( re );

  return( list );
}


CORBA_Object ydim_imr_propertyResolve_i(ydim_imr or, yoenv* ev, char* property)
{
  yort_claimList    clist;
  yort_claim	    what;
  ydim_claimIterator	ci;
  dvoid *obj;

  what.property = property;
  what.obj = (dvoid*)0;
  what.owner = (yort_proc)0;
  ydim_imr_listClaim_i( or, ev, (sb4)2, &what, &clist, &ci );

  if( ci )
  {
    
    ydim_claimIterator_destroy_i( ci, ev );
    yseThrow( YDYO_EX_NOTUNIQUE );
  }

  obj = clist._length ? clist._buffer[0].obj : (dvoid*)0;
  return( (CORBA_Object)yoDuplicate( obj ) );
}


void ydim_imr_abandonFor_i( ydim_imr or, yoenv* ev, yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydimSyncWait( cx );		

  ydcaAbandonFor( cx->cacx_ydimcx, what, TRUE );
}



ydim_tryResult ydim_imr_tryStake_i( ydim_imr or, yoenv* ev, yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  return ydcaTryStake( cx->cacx_ydimcx, what );
}



ydim_tryResult ydim_imr_transferStake_i( ydim_imr or, yoenv* ev,
					yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  return ydcaTransferStake( cx->cacx_ydimcx, what );
}



void ydim_imr_commitStake_i( ydim_imr or, yoenv* ev, yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydcaCommitStake( cx->cacx_ydimcx, what );
}



void ydim_imr_abortStake_i( ydim_imr or, yoenv* ev, yort_claim* what)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydcaAbortStake( cx->cacx_ydimcx, what );
}

void ydim_imr_listClaim_i( ydim_imr or, yoenv* ev, sb4 count,
			  yort_claim* what, yort_claimList* claims,
			  ydim_claimIterator* ci)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimcicx  *cix;

  claims->_length = claims->_maximum = 0;
  claims->_buffer = (yort_claim*)0;

  cix = (ydimcicx*)ysmGlbAlloc( sizeof(*cix), ydimcicx_tag );
  yort_claim__copy( &cix->filter_ydimcicx, what, yotkAllocStr );
  if( cix->filter_ydimcicx.property )
    cix->re_ydimcicx = ysRECompile( cix->filter_ydimcicx.property, FALSE );
  else
    cix->re_ydimcicx = (ysre*)0;

  *ci = (ydim_claimIterator)yoCreate( ydim_claimIterator__id, (char*)0,
				 (yoRefData*)0, (char*)0, (dvoid*)cix);

  cix->cursor_ydimcicx.property = (char*)0;
  cix->cursor_ydimcicx.obj = (dvoid*)0;
  cix->cursor_ydimcicx.owner = (yort_proc)0;
  cix->first_ydimcicx = TRUE;
  cix->self_ydimcicx = *ci;
  cix->le_ydimcicx = ysLstEnq( cx->cis_ydimcx, (dvoid*)cix );

  if( count && (!ydim_claimIterator_next_n_i( *ci, ev, count, claims ) ||
		claims->_length != (ub4)count) )
  {
    ydim_claimIterator_destroy_i( *ci, ev );
    *ci = (ydim_claimIterator)0;
  }
  if( *ci )
    *ci = (ydim_claimIterator)yoDuplicate((dvoid*)*ci);
}








ydim_implInfo ydim_info__get_info_i( ydim_info or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydim_implInfo  *info = (ydim_implInfo*)yoGetState((dvoid *)or);
  ydimn	    *on = (ydimn*)ysspLookup( (dvoid*)info, &cx->itree_ydimcx );
  ydim_implInfo  rv;

  
  ysmCheck( on, ydimn_tag );

  
  ydimUpdateAlist( on );

  
  ydim_implInfo__copy( &rv, info, yoAlloc );
  return rv;
}



void ydim_info_destroyLocal_i( ydim_info or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydim_implInfo  *info = (ydim_implInfo *)yoGetState((dvoid *)or);
  ydimn	    *on = (ydimn*)ysspLookup( (dvoid*)info, &cx->itree_ydimcx );

  
  ysmCheck( on, ydimn_tag );

  ydimSyncWait( cx );		

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1285,YSLSEV_INFO,(char*)0, YSLNONE );

  ysspRemove( &on->inode_ydimn, &cx->itree_ydimcx );
  ydim_implInfo__free( info, yotkFreeStr);
  yoDispose( (dvoid*)or );
  ysmGlbFree( (dvoid *)on );
}


void ydim_info_destroyGlobal_i( ydim_info or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydim_implInfo  *info = (ydim_implInfo *)yoGetState((dvoid *)or);
  ydimn	    *on = (ydimn*)ysspLookup( (dvoid*)info, &cx->itree_ydimcx );

  
  ysmCheck( on, ydimn_tag );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1286,YSLSEV_INFO,(char*)0, YSLNONE);

  ydim_imr_destroyInfoGlobal_i( cx->self_ydimcx, ev, info );
}







ydyo_activeInfo ydim_active__get_ainfo_i( ydim_active or, yoenv *ev)
{
  ydimain   *ain = (ydimain*)yoGetState((dvoid *)or);
  ydyo_activeInfo rv;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1287,YSLSEV_INFO,(char*)0, YSLNONE );

  
  ysmCheck( ain, ydimain_tag );
  ydyo_activeInfo__copy( &rv, &ain->ainfo_ydimain, yoAlloc );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1288,YSLSEV_DEBUG(4),(char*)0,
	   YSLSTR(yduStr(ain->ainfo_ydimain.intf_ydyo_activeInfo)),
	   YSLSTR(yduStr(ain->ainfo_ydimain.impl_ydyo_activeInfo)), YSLEND);

  return rv;
}


ydim_yortProcInfoList ydim_imr__get_plist_i( ydim_imr or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);

  ydim_yortProcInfoList rv;
  ysspNode  *n;
  ydimy	    *y;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1289,YSLSEV_INFO,(char*)0, YSLNONE);

  rv._length = rv._maximum = 0;
  for( n = ysspFHead( &cx->ptree_ydimcx); n ; n = ysspFNext( n ) )
      rv._maximum++;

  rv._buffer =
    (yort_procInfo*)yoAlloc((size_t)rv._maximum*sizeof(yort_procInfo));

  for( n = ysspFHead( &cx->ptree_ydimcx); n ; n = ysspFNext( n ) )
  {
    y = (ydimy*)n->key_ysspNode;
    yort_procInfo__copy( &rv._buffer[rv._length++], &y->pinfo_ydimy, yoAlloc );
  }
  return( rv );
}




void ydim_active_destroyLocal_i( ydim_active or, yoenv *ev)
{
  ydimcx  *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimain *ain = (ydimain *)yoGetState((dvoid *)or);

  ydyo_activeInfo    *ainfo = &ain->ainfo_ydimain;
  ydim_implInfo  *info;
  ydimy	    lookup;
  ydimy	    *y = (ydimy*)0;
  ub4	    i;
  ydim_infoIterator ii;
  ydim_infoList	ilist;
  ysspNode  *n;
  ydimn	    *on;

  
  ysmCheck( ain, ydimain_tag );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1290,YSLSEV_INFO,(char*)0, YSLNONE);

  ydimSyncWait( cx );		

  
  lookup.pinfo_ydimy.self_yort_procInfo = ainfo->yort_ydyo_activeInfo;
  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) ) )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1291,YSLSEV_DEBUG(3),(char*)0, YSLNONE);
    y = (ydimy*)n->key_ysspNode;
    ydimDelObjFromOtree( &y->otree_ydimy, (dvoid*)or );
  }

  

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1292,YSLSEV_DEBUG(3),(char*)0, YSLNONE);

  ydim_imr_listImpl_i( cx->self_ydimcx, ev, cx->chunk_ydimcx,
		      ainfo->intf_ydyo_activeInfo, ainfo->impl_ydyo_activeInfo,
		      y ? y->pinfo_ydimy.host_yort_procInfo : (char *)0,
		      &ilist, &ii );
  do
  {
    for( i = 0 ; i < ilist._length ; i++ )
    {
      info =
	(ydim_implInfo*)yoGetState((dvoid*)
				   ilist._buffer[i].self_ydim_implInfo);
      on = (ydimn*)ysspLookup( (dvoid*)info, &cx->itree_ydimcx );

      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1293,YSLSEV_DEBUG(4),(char*)0,
	       YSLSTR(yduStr(info->intf_ydim_implInfo)), YSLEND );

      ydimDelObjFromOtree( &on->otree_ydimn, (dvoid*)or );
      on->update_ydimn = TRUE;
    }
    ydim_infoList__free( &ilist, yoFree );

  } while(ii && ydim_infoIterator_next_n_i( ii, ev, cx->chunk_ydimcx, &ilist));

  if( ii )
    ydim_infoIterator_destroy_i( ii, ev );

  ysspRemove( &ain->anode_ydimain, &cx->atree_ydimcx );
  ydyo_activeInfo__free( ainfo, yotkFreeStr);
  yoDispose( (dvoid*)or );
  ysmGlbFree( (dvoid *)ain );
}


void ydim_active_destroyGlobal_i( ydim_active or, yoenv *ev)
{
  ydimcx  *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydyo_activeInfoList ailist;
  ydimain *ain = (ydimain*)yoGetState((dvoid *)or);

  
  ysmCheck( ain, ydimain_tag );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1294,YSLSEV_INFO,(char*)0, YSLNONE);

  ailist._maximum = ailist._length = 1;
  ailist._buffer = &ain->ainfo_ydimain;

  ydim_imr_destroyActiveGlobal_i( cx->self_ydimcx, ev, &ailist );
}






boolean ydim_infoIterator_next_one_i( ydim_infoIterator or, yoenv *ev,
				     ydim_implInfo* info)
{
  boolean rv = FALSE;
  ydim_infoList list;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1295,YSLSEV_INFO,(char*)0, YSLNONE);

  if( ydim_infoIterator_next_n_i( or, ev, (sb4)1, &list ) && list._length )
  {
    
    DISCARD memcpy( (dvoid*)info, (dvoid*)&list._buffer[0], sizeof(*info) );
    rv = TRUE;
    yoFree( (dvoid*)list._buffer );
  }
  return( rv );
}



boolean ydim_infoIterator_next_n_i( ydim_infoIterator or, yoenv *ev,
				   sb4 count, ydim_infoList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimicx   *myii = (ydimicx*)yoGetState( (dvoid*)or );
  ydim_implInfo  *lookup;
  ydim_implInfo  *info = (ydim_implInfo*)0;
  ysspNode  *n;
  boolean   rv;

  
  ysmCheck( myii, ydimicx_tag );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1296,YSLSEV_INFO,(char*)0,YSLNONE);

  list->_length = 0;
  list->_maximum = (ub4)count;
  list->_buffer =
    (ydim_implInfo*)yoAlloc( (size_t)count * sizeof(*list->_buffer) );

  while( list->_length < (ub4)count )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1297,YSLSEV_DEBUG(4),(char*)0,
	     YSLSTR(yduStr(myii->host_ydimicx)), YSLEND );

    lookup = &myii->kinfo_ydimicx;
    yduFreeCopyStr(&lookup->host_ydim_implInfo, myii->host_ydimicx,
		   yotkFreeStr, yotkAllocStr);

    
    if( myii->first_ydimicx )
      n = ydimFirstLookup((dvoid*)&myii->kinfo_ydimicx, &cx->itree_ydimcx,
			  ydimIIIHWCmp);
    else
      n = ysspNextLookup((dvoid*)&myii->info_ydimicx, &cx->itree_ydimcx );

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1298,YSLSEV_DEBUG(4),(char*)0,
	     YSLSTR(myii->first_ydimicx ? "FIRST" : "NEXT"), YSLEND);

    for( ; n && list->_length < (ub4)count ; n = ysspFNext(n) )
    {
      if( ydimIIIHWCmp( (dvoid *)&myii->kinfo_ydimicx, n->key_ysspNode ) )
      {
	
	ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1299,YSLSEV_DEBUG(4),
		 (char*)0,YSLNONE);
	n = (ysspNode *)0;
	break;
      }
      info = (ydim_implInfo*)n->key_ysspNode;
      ydimUpdateAlist( (ydimn*)n );
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1300,YSLSEV_DEBUG(5),(char*)0,YSLNONE);
      ydim_implInfo__copy( &list->_buffer[list->_length++], info, yoAlloc );
      myii->first_ydimicx = FALSE;
    }
    
    if( !n )
      break;
  }
  if( !list->_length )
  {
    list->_maximum = 0;
    yoFree( (dvoid*)list->_buffer );
    list->_buffer = (ydim_implInfo*)0;
  }

  if( !myii->first_ydimicx && info )
    ydimKeySet( &myii->info_ydimicx, info );

  
  rv = !count || list->_length;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1301,YSLSEV_DEBUG(4),(char*)0,
	   YSLUB4((ub4)rv), YSLUB4(list->_length), YSLEND );

  return( rv );
}




void ydim_infoIterator_destroy_i( ydim_infoIterator or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimicx  *myii = (ydimicx*)yoGetState( (dvoid*)or );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1302,YSLSEV_INFO,(char*)0, YSLNONE );

  
  ysmCheck( myii, ydimicx_tag );
  DISCARD ysLstRem( cx->iis_ydimcx, myii->le_ydimicx );
  ydimDestroyII( (dvoid*)myii );
}







boolean ydim_yortIterator_next_one_i( ydim_yortIterator or, yoenv *ev,
				     yort_proc *oyort)
{
  boolean rv = FALSE;
  ydim_yortList list;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1303,YSLSEV_INFO,(char*)0, YSLNONE );

  list._length = list._maximum = 0;
  list._buffer = (yort_proc*)0;
  if( ydim_yortIterator_next_n_i( or, ev, (sb4)1, &list ) && list._length )
  {
    rv = TRUE;
    *oyort = list._buffer[0];
    yoFree( (dvoid*)list._buffer );
  }
  return( rv );
}



boolean ydim_yortIterator_next_n_i( ydim_yortIterator or, yoenv *ev,
				   sb4 count, ydim_yortList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimyicx  *myyi = (ydimyicx*)yoGetState((dvoid*)or);
  ydimy	    lookup;
  ydimy	    *y = (ydimy*)0;
  ysspNode  *n;
  boolean   rv;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1304,YSLSEV_INFO,(char*)0, YSLNONE);

  
  ysmCheck( myyi, ydimyicx_tag );

  lookup.pinfo_ydimy.self_yort_procInfo = myyi->yort_ydimyicx;

  list->_maximum = (ub4)count;
  list->_length = 0;
  list->_buffer =
    (yort_proc*)yoAlloc( (size_t)count * sizeof(*list->_buffer) );

  if( myyi->first_ydimyicx )
    if( lookup.pinfo_ydimy.self_yort_procInfo )
      n = ysspLookup(  (dvoid *)&lookup, &cx->ytree_ydimcx );
    else
      n = ysspFHead( &cx->ytree_ydimcx );
  else
    n = ysspNextLookup( (dvoid *)&lookup, &cx->ytree_ydimcx );

  for( ; n && list->_length < (ub4)count ; n = ysspFNext( n ))
  {
    y = (ydimy *)n->key_ysspNode;
    if( myyi->yort_ydimyicx )
      yoRelease((dvoid*)myyi->yort_ydimyicx );
    myyi->yort_ydimyicx =
      (yort_proc)yoDuplicate((dvoid*)y->pinfo_ydimy.self_yort_procInfo);
    list->_buffer[list->_length++] =
      (yort_proc)yoDuplicate((dvoid*)y->pinfo_ydimy.self_yort_procInfo);
    myyi->first_ydimyicx = FALSE;
  }

  if( !list->_length )
  {
    list->_maximum = 0;
    yoFree( (dvoid *)list->_buffer );
    list->_buffer = (yort_proc*)0;
  }
  
  rv = !count || list->_length;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1305,YSLSEV_DEBUG(4),(char*)0,
	   YSLUB4((ub4)rv), YSLUB4( list->_length ), YSLEND);

  return( rv );
}



void ydim_yortIterator_destroy_i( ydim_yortIterator or, yoenv *ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimyicx  *myyi = (ydimyicx*)yoGetState((dvoid*)or);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1306,YSLSEV_INFO,(char*)0, YSLNONE);

  
  ysmCheck( myyi, ydimyicx_tag );
  DISCARD ysLstRem( cx->yis_ydimcx, myyi->le_ydimyicx );
  ydimDestroyYI( (dvoid*)myyi );
}







void ydim_sync_decrement_i( ydim_sync or, yoenv* ev)
{
  ydimcx *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimso *so = (ydimso*)yoGetState((dvoid*)or );

  ydimSyncFunc hdlr;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1308,YSLSEV_INFO,(char*)0, YSLNONE );

  if( cx->so_ydimcx != so )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1307,YSLSEV_EMERG,(char*)0,
	     YSLSTR("ydim_sync_decrement"), YSLEND );
    ysePanic(YDIM_EX_INTERNAL);
  }

  if( --so->cnt_ydimso == 0 && so->hdlr_ydimso )
  {
    hdlr = so->hdlr_ydimso;
    so->hdlr_ydimso = (ydimSyncFunc)0;
    
    (*hdlr)( or, TRUE, so->usrp_ydimso );
  }
}


void ydim_sync_destroy_i( ydim_sync or, yoenv* ev)
{
  ydimcx *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimso *so = (ydimso*)yoGetState((dvoid*)or );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1309,YSLSEV_INFO,(char*)0, YSLNONE );

  if( cx->so_ydimcx != so )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1307,YSLSEV_EMERG,(char*)0,
	     YSLSTR("ydim_sync_destroy"), YSLEND );
    ysePanic(YDIM_EX_INTERNAL);
  }

  if( so->hdlr_ydimso )
    (*so->hdlr_ydimso)( or, FALSE, so->usrp_ydimso );

  yoDispose( (dvoid*)or );
  yoRelease( (dvoid*)or );
  ysmGlbFree( (dvoid*)so );
  cx->so_ydimcx = (ydimso*)0;
}





boolean ydim_claimIterator_next_one_i( ydim_claimIterator or, yoenv* ev,
				      yort_claim* claim)
{
  yort_claimList claims;
  boolean rv;

  rv = ydim_claimIterator_next_n_i( or, ev, (sb4)1, &claims );

  if( claims._length )
    yort_claim__copy( claim, &claims._buffer[0], yoAlloc );

  yort_claimList__free( &claims, yoFree );
  return( rv );
}


boolean ydim_claimIterator_next_n_i( ydim_claimIterator or, yoenv* ev,
				    sb4 count, yort_claimList* list)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimcicx   *cix = (ydimcicx*)yoGetState( (dvoid*)or );
  yort_claim	*current = (yort_claim*)0;

  
  ysmCheck( cix, ydimcicx_tag );

  list->_length = 0;
  list->_maximum = (ub4)count;
  list->_buffer =
    (yort_claim*)yoAlloc( (size_t)count * sizeof(*list->_buffer) );

  while( list->_length < (ub4)count )
  {
    current = cix->first_ydimcicx ? (yort_claim*)0 : &cix->cursor_ydimcicx;
    if( !(current = ydcaListNext( cx->cacx_ydimcx, current ) ) )
      break;

    
    if( !cix->first_ydimcicx )
      yort_claim__free( &cix->cursor_ydimcicx, yotkFreeStr );
    yort_claim__copy( &cix->cursor_ydimcicx, current, yotkAllocStr );
    cix->first_ydimcicx = FALSE;

    
    if((!cix->re_ydimcicx ||
	ysREMatch( cix->re_ydimcicx, current->property, TRUE )) &&
       (!cix->filter_ydimcicx.obj ||
	yoIsEq(cix->filter_ydimcicx.obj, current->obj)) &&
       (!cix->filter_ydimcicx.owner ||
	yoIsEq((dvoid*)cix->filter_ydimcicx.owner, (dvoid*)current->owner)))
    {
      yort_claim__copy( &list->_buffer[ list->_length++ ], current, yoAlloc);
    }
  }
  return current ? TRUE : FALSE;
}


void ydim_claimIterator_destroy_i( ydim_claimIterator or, yoenv* ev)
{
  ydimcx    *cx = (ydimcx*)yoGetImplState((dvoid *)or);
  ydimcicx   *cix = (ydimcicx*)yoGetState( (dvoid*)or);

  DISCARD ysLstRem( cx->cis_ydimcx, cix->le_ydimcicx );
  ydimDestroyCI( (dvoid*)cix );
}













STATICF void ydimObjReleaseDispose( dvoid *o )
{
  yoRelease( o );
  yoDispose( o );
}




STATICF void ydimInitInfo( ydim_implInfo *info )
{
  info->self_ydim_implInfo = (ydim_info)0;

  info->intf_ydim_implInfo =
    info->impl_ydim_implInfo =
      info->host_ydim_implInfo =
	info->pathName_ydim_implInfo = (char *)0;

  info->level_ydim_implInfo = (char *)0;
  info->minInstances_ydim_implInfo = 0;
  info->maxInstances_ydim_implInfo = SB4MAXVAL;

  info->alist_ydim_implInfo._length = info->alist_ydim_implInfo._maximum = 0;
  info->alist_ydim_implInfo._buffer = (ydim_active*)0;
  info->args_ydim_implInfo._length = info->args_ydim_implInfo._maximum = 0;
  info->args_ydim_implInfo._buffer = (char **)0;
}




STATICF void ydimInitAinfo( ydyo_activeInfo *ainfo, ydim_implInfo *info )
{
  CLRSTRUCT( *ainfo );
  ainfo->self_ydyo_activeInfo = (ydim_active)0;
  ainfo->intf_ydyo_activeInfo = info->intf_ydim_implInfo;
  ainfo->impl_ydyo_activeInfo = info->impl_ydim_implInfo;
  ainfo->yort_ydyo_activeInfo = (yort_proc)0;
  ainfo->yortImpl_ydyo_activeInfo = (yort_impl)0;
  ainfo->implFlags_ydyo_activeInfo = 0;
}



STATICF sword ydimIImplCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv;
  ydim_implInfo  *ap = (ydim_implInfo *)a;
  ydim_implInfo  *bp = (ydim_implInfo *)b;

  if(!(rv = yduSafeStrcmp(ap->intf_ydim_implInfo, bp->intf_ydim_implInfo)) &&
     !(rv = yduSafeStrcmp(ap->impl_ydim_implInfo, bp->impl_ydim_implInfo)))
    rv = yduSafeStrcmp(ap->host_ydim_implInfo, bp->host_ydim_implInfo);

  return( rv );
}





STATICF sword ydimIIIHWCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv;
  ydim_implInfo  *ap = (ydim_implInfo *)a;
  ydim_implInfo  *bp = (ydim_implInfo *)b;

  if(!(rv = yduWildStrcmp(ap->intf_ydim_implInfo, bp->intf_ydim_implInfo)) &&
     !(rv = yduWildStrcmp(ap->impl_ydim_implInfo, bp->impl_ydim_implInfo)))
    rv = yduWildStrcmp(ap->host_ydim_implInfo, bp->host_ydim_implInfo);

  return( rv );
}


STATICF sword ydimAICmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv;
  ydyo_activeInfo  *ap = (ydyo_activeInfo *)a;
  ydyo_activeInfo  *bp = (ydyo_activeInfo *)b;

  if(!(rv = yoCmp((dvoid*)ap->yort_ydyo_activeInfo,
	       (dvoid*)bp->yort_ydyo_activeInfo)) &&
     !(rv = yduWildStrcmp(ap->intf_ydyo_activeInfo,
			  bp->intf_ydyo_activeInfo)))
     rv = yduWildStrcmp(ap->impl_ydyo_activeInfo,
			bp->impl_ydyo_activeInfo);

  return( rv );
}



STATICF void ydimPutGlobal( CONST ydim_imr or, CONST dvoid *obj,
			   CONST ydimGlobalFunc f, CONST char *name )
{
  ydimcx *cx = (ydimcx*)yoGetImplState( (dvoid*)or );
  yslst	*orbds;
  ysle	*e;
  ydimghcx  lgh;
  ydimghcx  *gh;
  ydimsp *sp;
  char *refstr;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1310,YSLSEV_DEBUG(4),(char*)0,
	   YSLSTR(name), YSLEND );

  
  orbds = yoListORBDs();

  

  
  if( (sp = cx->asp_ydimcx ) )
    DISCARD ysLstEnq( orbds, yoDuplicate( (dvoid*)sp->dest_ydimsp ) );

  
  ydimSyncWait( cx );

  
  yseTry
  {

    for( e = ysLstHead( orbds ); e ; e = ysLstNext( e ) )
    {
      if( !yoIsEq( (dvoid*)cx->self_ydimcx, ysLstVal(e) ) )
      {
	refstr = yoRefToStr( ysLstVal(e) );
	
	ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1311,YSLSEV_DEBUG(5),(char*)0,
		 YSLSTR(refstr), YSLEND );
	yoFree( (dvoid*)refstr );

	gh = (ydimghcx*)ysmGlbAlloc(sizeof(*gh),"ydimghcx");
	
	gh->ydimcx_ydimghcx = cx;
	gh->func_ydimghcx = f;
	gh->name_ydimghcx = name;
	gh->obj_ydimghcx = (dvoid*)obj;
	gh->dest_ydimghcx = (ydim_imr)yoDuplicate(ysLstVal(e));
	gh->outobj_ydimghcx = (dvoid*)0;
	gh->evt_ydimghcx = ysEvtSimple( ydimGlobalHandler, (dvoid*)gh );
	yoEnvInit( &gh->ev_ydimghcx );
	(*gh->func_ydimghcx)( gh->dest_ydimghcx, gh->obj_ydimghcx,
			     &gh->ev_ydimghcx, gh );
	ysYield();
      }
    }
    
    lgh.ydimcx_ydimghcx = cx;
    lgh.func_ydimghcx = f;
    lgh.name_ydimghcx = name;
    lgh.obj_ydimghcx = (dvoid*)obj;
    lgh.dest_ydimghcx = (ydim_imr)cx->self_ydimcx;
    lgh.outobj_ydimghcx = (dvoid*)0;
    lgh.evt_ydimghcx = (ysevt*)0;
    (*f)( cx->self_ydimcx, obj, &cx->ev_ydimcx, &lgh );
  }
  yseCatch( YS_EX_INTERRUPT )
    yseRethrow;
  yseCatchAll
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1312,YSLSEV_WARNING,(char*)0,
	     YSLSTR(ysidToStr(yseExid)), YSLEND );
  yseEnd;

  if( orbds )
    ysLstDestroy( orbds, yoRelease );

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1313,YSLSEV_DEBUG(5),(char*)0, YSLNONE );
}





STATICF void ydimGlobalHandler(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			       size_t argsz)
{
  ydimghcx *ghcx = (ydimghcx*)usrp;
  char *refstr;

  if(exid)
  {
    refstr = yoRefToStr( (dvoid*)ghcx->dest_ydimghcx );
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1314,YSLSEV_WARNING,(char*)0,
	     YSLSTR(ysidToStr(exid)),
	     YSLSTR(ghcx->name_ydimghcx),
	     YSLSTR(refstr), YSLEND);
    yoFree( (dvoid*)refstr );
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1315,YSLSEV_DEBUG(5),(char*)0,
	     YSLSTR(ghcx->name_ydimghcx), YSLEND );
  }

  if( ghcx->outobj_ydimghcx )
    yoRelease( ghcx->outobj_ydimghcx );
  yoRelease( (dvoid*)ghcx->dest_ydimghcx );
  yoEnvFree( &ghcx->ev_ydimghcx );
  ysmGlbFree( (dvoid*)ghcx );
}




STATICF void ydimPutInfo(CONST ydim_imr dest,
			 CONST dvoid *usrp, yoenv *ev, ydimghcx *gh )
{
  if( gh->evt_ydimghcx )
  {
    ydim_imr_addInfoLocal_nw( dest, ev, (ydim_implInfo *)usrp,
			  (ydim_info*)&gh->outobj_ydimghcx,
			  gh->evt_ydimghcx );
  }
  else
  {
    ydim_imr_addInfoLocal( dest, ev, (ydim_implInfo *)usrp,
			  (ydim_info*)&gh->outobj_ydimghcx );
    yoRelease( gh->outobj_ydimghcx );
  }
}




STATICF void ydimDelInfo( CONST ydim_imr dest,
			 CONST dvoid *usrp,
			 yoenv *ev, ydimghcx *gh)
{
  if( gh->evt_ydimghcx )
    ydim_imr_destroyInfoLocal_nw( dest, ev, (ydim_implInfo *)usrp,
				 gh->evt_ydimghcx );
  else
    ydim_imr_destroyInfoLocal( dest, ev, (ydim_implInfo *)usrp );
}





STATICF void ydimPutActive(CONST ydim_imr dest,
			   CONST dvoid *usrp,
			   yoenv *ev, ydimghcx *gh )
{
  ydima *arg = (ydima *)usrp;

  if( gh->evt_ydimghcx )
    ydim_imr_addActiveLocal_nw( dest, ev, arg->pinfo_ydima,
			       arg->alist_ydima,
			       gh->evt_ydimghcx);
  else
    ydim_imr_addActiveLocal( dest, ev, arg->pinfo_ydima, arg->alist_ydima );
}




STATICF void ydimDelActive(CONST ydim_imr dest,
			   CONST dvoid *usrp,
			   yoenv *ev, ydimghcx *gh )
{
  if( gh->evt_ydimghcx )
    ydim_imr_destroyActiveLocal_nw( dest, ev, (ydyo_activeInfoList*)usrp,
				   gh->evt_ydimghcx );
  else
    ydim_imr_destroyActiveLocal( dest, ev, (ydyo_activeInfoList*)usrp );
}







STATICF void ydimDelYort(CONST ydim_imr dest,
			 CONST dvoid *usrp,
			 yoenv *ev, ydimghcx *gh)
{
  if( gh->evt_ydimghcx )
    ydim_imr_destroyYortLocal_nw( dest, ev, (yort_proc)usrp,
				 gh->evt_ydimghcx );
  else
    ydim_imr_destroyYortLocal( dest, ev, (yort_proc)usrp );
}




STATICF void ydimKeySet( ydim_implInfo *dest, ydim_implInfo *src )
{
  yduFreeCopyStr(&dest->intf_ydim_implInfo, src->intf_ydim_implInfo,
		 yotkFreeStr, yotkAllocStr);
  yduFreeCopyStr(&dest->impl_ydim_implInfo, src->impl_ydim_implInfo,
		 yotkFreeStr, yotkAllocStr);
  yduFreeCopyStr(&dest->host_ydim_implInfo, src->host_ydim_implInfo,
		 yotkFreeStr, yotkAllocStr);
  yduFreeCopyStr(&dest->pathName_ydim_implInfo, src->pathName_ydim_implInfo,
		 yotkFreeStr, yotkAllocStr);
}




STATICF sword ydimYYortCmp( CONST dvoid *a, CONST dvoid *b )
{
  ydimy *ap = (ydimy*)a;
  ydimy *bp = (ydimy*)b;

  return( yoCmp((dvoid*)ap->pinfo_ydimy.self_yort_procInfo,
		(dvoid*)bp->pinfo_ydimy.self_yort_procInfo ) );
}




STATICF sword ydimYProcCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv = 0;
  ydimy *ap = (ydimy*)a;
  ydimy *bp = (ydimy*)b;

  if(!(rv = (sword)strcmp(ap->pinfo_ydimy.host_yort_procInfo,
		   bp->pinfo_ydimy.host_yort_procInfo )) &&
     !(rv = yduSafeStrcmp(ap->pinfo_ydimy.pid_yort_procInfo,
			  bp->pinfo_ydimy.pid_yort_procInfo )))
    rv = yduSafeStrcmp(ap->pinfo_ydimy.affinity_yort_procInfo,
		       bp->pinfo_ydimy.affinity_yort_procInfo );
  return( rv );
}





STATICF ydimy *ydimAddYort( ydimcx *cx, yort_procInfo *pinfo )
{
  ydsp_procInfo	spinfo;
  ydsp_proc	proc;
  CONST char	*host = ysGetHostName();
  ydimy	    lookup;
  ysspNode  *n;
  ydimy	    *y;

  
  CLRSTRUCT(lookup);
  lookup.pinfo_ydimy.self_yort_procInfo = pinfo->self_yort_procInfo;
  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) ) )
  {
    y = (ydimy*)n;
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1316,YSLSEV_DEBUG(4),(char*)0, YSLNONE );
  }
  else			
  {
    y = (ydimy*)ysmGlbAlloc( sizeof(*y), ydimy_tag );
    y->ynode_ydimy.key_ysspNode = y->pnode_ydimy.key_ysspNode = (dvoid*)y;
    DISCARD ysspNewTree( &y->otree_ydimy, yoCmp );

    
    yort_procInfo__copy( &y->pinfo_ydimy, pinfo, yotkAllocStr );

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1317,YSLSEV_DEBUG(3),(char*)0,
	     YSLSTR(y->pinfo_ydimy.host_yort_procInfo),
	     YSLSTR(y->pinfo_ydimy.pid_yort_procInfo),
	     YSLSTR(yduStr(y->pinfo_ydimy.affinity_yort_procInfo)), YSLEND);

    DISCARD ysspEnq( &y->pnode_ydimy, &cx->ptree_ydimcx );
    DISCARD ysspEnq( &y->ynode_ydimy, &cx->ytree_ydimcx );

    y->cx_ydimy = cx;
    y->devt_ydimy = ysEvtCreate(ydimYortDeath, (dvoid*)y, cx->q_ydimcx, TRUE);
    yoWatchOwner( (dvoid*)pinfo->self_yort_procInfo, y->devt_ydimy );
  }

  
  if( cx->sp_ydimcx && !strcmp( host, pinfo->host_yort_procInfo ) )
  {
    
    spinfo.self_ydsp_procInfo = (ydsp_proc)0;
    spinfo.host_ydsp_procInfo = pinfo->host_yort_procInfo;
    spinfo.pid_ydsp_procInfo = pinfo->pid_yort_procInfo;
    spinfo.affinity_ydsp_procInfo = pinfo->affinity_yort_procInfo;
    spinfo.name_ydsp_procInfo = pinfo->name_yort_procInfo;
    spinfo.parent_ydsp_procInfo = (ydsp_spawner)0;
    spinfo.state_ydsp_procInfo = ydsp_start_ok;
    ydsp_spawner_addExisting( cx->sp_ydimcx, &cx->ev_ydimcx,
			     &spinfo, &proc );
    yoRelease( (dvoid*)proc );
  }
  return y;
}






STATICF void ydimYortDeath(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			   size_t argsz)
{
  ydimy *y = (ydimy*)usrp;

  ydim_imr_destroyYortLocal_i( y->cx_ydimy->self_ydimcx,
			      &y->cx_ydimy->ev_ydimcx,
			      y->pinfo_ydimy.self_yort_procInfo );
}





STATICF ysspNode *ydimLookup( dvoid *key, ysspTree *t, ysspCmpFunc cmp )
{
  ysspNode *n;
  sb4 cmpval = 0;		

  
  n = t->root_ysspTree;
  while( n && (cmpval = (*cmp)( key, n->key_ysspNode ) ) )
    n = ( cmpval < 0 ) ? n->left_ysspNode : n->right_ysspNode;

  
  if( n )
    ysspSplay( n, t );

  return( n );
}




STATICF ysspNode *ydimFirstLookup( dvoid *key, ysspTree *t, ysspCmpFunc cmp )
{
  ysspNode *n, *pn;

  
  for(n = ydimLookup( key, t, cmp ); (pn = ysspFPrev(n)) ; n = pn )
    if( (*cmp)( key, pn->key_ysspNode) )
      break;

  if( n )
    ysspSplay( n, t );
  return( n );
}






STATICF ydim_active ydimGetActive( ydimcx *cx, ydyo_activeInfo *ainfo )
{
  ysspNode	*n;
  ydimain	*ain = (ydimain*)0;
  ydyo_activeInfo    *nainfo = (ydyo_activeInfo*)0;
  ydyo_activeInfo    lainfo;
  ydim_active	a;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1318,YSLSEV_DEBUG(4),(char*)0,
	   YSLSTR(yduStr(ainfo->intf_ydyo_activeInfo)),
	   YSLSTR(yduStr(ainfo->impl_ydyo_activeInfo)), YSLEND),

  
  DISCARD memcpy( (dvoid*)&lainfo, (dvoid*)ainfo, sizeof(lainfo));

  
  if( (n = ysspLookup( (dvoid*)ainfo, &cx->atree_ydimcx ) ) )
  {
    ain = (ydimain*)n;
    nainfo = &ain->ainfo_ydimain;

    if( ainfo->yort_ydyo_activeInfo )
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1319,YSLSEV_DEBUG(4),(char*)0,YSLNONE);

      
      nainfo->implFlags_ydyo_activeInfo = ainfo->implFlags_ydyo_activeInfo;
    }
    else			
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1320,YSLSEV_DEBUG(3),(char*)0,YSLNONE);

      
      lainfo.yort_ydyo_activeInfo = (yort_proc)cx->self_ydimcx;
      lainfo.yortImpl_ydyo_activeInfo = (yort_impl)0;
      ain = (ydimain*)yoGetState( (dvoid*)(a = ydimGetActive(cx, &lainfo)));
      
      ysmCheck( ain, ydimain_tag );
      nainfo = &ain->ainfo_ydimain;
      yoRelease( (dvoid*)a );
    }
  }

  
  if( ainfo->yort_ydyo_activeInfo && !ain )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1321,YSLSEV_DEBUG(4),(char*)0, YSLNONE);

    
    lainfo.yort_ydyo_activeInfo = (yort_proc)cx->self_ydimcx;
    if( (n = ysspLookup( (dvoid*)&lainfo, &cx->atree_ydimcx ) ) )
    {
      ain = (ydimain*)n;
      nainfo = &ain->ainfo_ydimain;

      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1322,YSLSEV_DEBUG(4),(char*)0,YSLNONE);

      
      ysspRemove( n, &cx->atree_ydimcx );
      
      nainfo->yort_ydyo_activeInfo =
	(yort_proc)yoDuplicate((dvoid*)ainfo->yort_ydyo_activeInfo);
      nainfo->yortImpl_ydyo_activeInfo = ainfo->yortImpl_ydyo_activeInfo;
      DISCARD ysspEnq( n, &cx->atree_ydimcx );

      
      ydimLaunchActive( cx, ain );
    }
  }

  if( !ain )			
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1323,YSLSEV_DEBUG(4),(char*)0, YSLNONE );

    ain = (ydimain*)ysmGlbAlloc( sizeof(*ain), ydimain_tag );
    nainfo = &ain->ainfo_ydimain;
    ain->anode_ydimain.key_ysspNode = (dvoid*)nainfo;
    ain->ucnt_ydimain = 0;
    ain->usrp_ydimain = (dvoid*)0;
    ydyo_activeInfo__copy( nainfo, ainfo, yotkAllocStr );
    nainfo->self_ydyo_activeInfo =
      (ydim_active)yoCreate( ydim_active__id, (char*)0,
			    (yoRefData*)0, (char *)0, (dvoid *)ain );

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1324,YSLSEV_DEBUG(4),(char*)0, YSLNONE );
    DISCARD ysspEnq( &ain->anode_ydimain, &cx->atree_ydimcx );
  }

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1325,YSLSEV_DEBUG(4),(char*)0,
	   YSLSTR(yduStr(nainfo->intf_ydyo_activeInfo)),
	   YSLSTR(yduStr(nainfo->impl_ydyo_activeInfo)), YSLEND );

  return( (ydim_active)yoDuplicate((dvoid*)nainfo->self_ydyo_activeInfo));
}





STATICF void ydimAddActiveToAlist( ydim_activeList *alist, ydim_active ai )
{
  ub4 i;
  ydim_active	*nbuffer;

  
  for( i = 0 ; i < alist->_length ; i++ )
    if( yoIsEq( (dvoid*)ai, (dvoid*)alist->_buffer[i] ) )
      break;
    else if( !(i % 100 ) )
      ysYield();

  
  if( i >= alist->_length )
  {
    
    if( i >= alist->_maximum )
    {
      ysYield();
      alist->_maximum += max( 5, alist->_maximum / 2 );
      nbuffer = (ydim_active*)
	ysmGlbAlloc(sizeof(*nbuffer)*(size_t)alist->_maximum,
		    "ydim_active _buffer" );
      if( alist->_buffer )
      {
	DISCARD memcpy( nbuffer, alist->_buffer, i * sizeof(*nbuffer) );
	ysmGlbFree( (dvoid*)alist->_buffer );
      }
      alist->_buffer = nbuffer;
    }
    alist->_buffer[alist->_length++] = (ydim_active)yoDuplicate((dvoid*)ai);
  }
}





STATICF void ydimAddObjToTree( ysspTree *ht, dvoid *or )
{
  ysspNode *n;

  if( !ysspLookup( or, ht ))
  {
    n = (ysspNode*)ysmGlbAlloc( sizeof(*n), "ydim ref node");
    n->key_ysspNode = yoDuplicate( or );
    DISCARD ysspEnq( n, ht );
  }
}

STATICF void ydimDelObjFromOtree( ysspTree *ht, dvoid *or )
{
  ysspNode *n;

  if( ( n = ysspLookup( (dvoid*)or, ht )))
  {
    ysspRemove( n, ht );
    yoRelease( n->key_ysspNode );
    ysmGlbFree( (dvoid*)n );
  }
}

STATICF void ydimDestroyOtree( ysspTree *ht )
{
  ysspNode *n;
  while( (n = ysspDeqTree( ht ) ) )
  {
    yoRelease( n->key_ysspNode );
    ysmGlbFree( (dvoid*)n );
  }
}





STATICF void ydimOtreeToAlist( ysspTree *otree, ydim_activeList *alist,
			      ysmaf af, ysmff ff )
{
  ub4 i;
  ysspNode *n;

  if( !ff ) ff = ysmFGlbFree;

  
  ydim_activeList__free( alist, (ysmff)ff );

  i = alist->_length = alist->_maximum = 0;
  alist->_buffer = (ydim_active*)0;

  
  if( ( i = ysspTreeCount( otree ) ) )
  {
    if( af )
      alist->_buffer =
	(ydim_active*)(*af)( (size_t)i * sizeof(ydim_active) );
    else
      alist->_buffer =
	(ydim_active*)ysmGlbAlloc( (size_t)i * sizeof(ydim_active),
				  "ydim_activeList buffer" );

    alist->_maximum = i;

    for( n = ysspFHead( otree ) ; n ; n = ysspFNext( n ) )
      alist->_buffer[ alist->_length++ ] =
	(ydim_active)yoDuplicate( n->key_ysspNode );
  }
}


STATICF void ydimUpdateAlist( ydimn *on )
{
  if( on->update_ydimn )
  {
    on->update_ydimn = FALSE;
    ydimOtreeToAlist( &on->otree_ydimn,
		     &on->info_ydimn.alist_ydim_implInfo,
		     yotkAllocStr, yotkFreeStr );
  }
}


STATICF ub4 ysspTreeCount( ysspTree *h )
{
  ub4 rv= 0;
  ysspNode *n;

  for( n = ysspFHead( h ); n ; n = ysspFNext(n) )
    rv++;

  return rv;
}






STATICF void ydimAlistToOtree( ydim_activeList *alist, ysspTree *otree )
{
  ub4 i;

  for( i = 0; i < alist->_length ; i++ )
    ydimAddObjToTree( otree, (dvoid*)alist->_buffer[i] );
}







STATICF void  ydimLaunchActive( ydimcx *cx, ydimain *ain )
{
  ydim_implInfo  lookup;
  ysspNode  *n;
  ydyo_activeInfo    *ainfo;
  ydyo_activeInfo    *lainfo;
  yort_procInfo	    *pinfo;
  ydimn	    *in;
  ysle	    *e = (ysle*)0;
  ysle	    *next;
  ydimln    *ln;

  

  ainfo = &ain->ainfo_ydimain;
  pinfo = ydimPInfoOfYort( cx, ainfo->yort_ydyo_activeInfo );

  lookup.intf_ydim_implInfo  = ainfo->intf_ydyo_activeInfo;
  lookup.impl_ydim_implInfo  = ainfo->impl_ydyo_activeInfo;
  lookup.host_ydim_implInfo  = pinfo->host_yort_procInfo;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1326,YSLSEV_INFO,(char*)0,
	   YSLSTR(yduStr(ainfo->intf_ydyo_activeInfo)),
	   YSLSTR(yduStr(ainfo->impl_ydyo_activeInfo)),
	   YSLSTR(pinfo->host_yort_procInfo), YSLEND);

  if( (n = ysspLookup( (dvoid*)&lookup, &cx->itree_ydimcx )) )
  {
    in = (ydimn*)n;
    ln = in->launch_ydimn;

    

    for( e = ysLstHead( ln->ilist_ydimln ) ; e ; e = next )
    {
      next = ysLstNext(e);
      lainfo = (ydyo_activeInfo*)ysLstVal(e);
      if( yoIsEq((dvoid*)ainfo->self_ydyo_activeInfo,
		 (dvoid*)lainfo->self_ydyo_activeInfo ))
      {
	
	ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1327,YSLSEV_DEBUG(4),
		 (char*)0,YSLNONE);
	DISCARD ysLstEnq( ln->alist_ydimln, ysLstVal(e) );
	DISCARD ysLstRem( ln->ilist_ydimln, e );
	break;
      }
    }
    if( !e )
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1328,YSLSEV_EMERG,(char*)0,YSLNONE);
      ysePanic(YDIM_EX_INTERNAL);
    }
    ydimLaunchCheck(ln);
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1329,YSLSEV_EMERG,(char*)0,YSLNONE);
    ysePanic(YDIM_EX_INTERNAL);
  }
}





STATICF void ydimLaunchTimeout(dvoid *usrp, CONST ysid *exid, dvoid *arg,
			       size_t argsz)
{
  ydimln *ln = (ydimln*)usrp;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1351,YSLSEV_EMERG,(char*)0,
	   YSLSTR(ln->host_ydimln),
	   YSLSTR(ln->path_ydimln),
	   YSLUB4(ysLstCount(ln->rlist_ydimln)),
	   YSLEND);

  
  if( ln->evt_ydimln )
  {
    ysEvtDestroy( ln->evt_ydimln );
    ln->evt_ydimln = (ysevt*)0;
  }

  
  ydimLaunchCheck( ln );
}





STATICF void ydimLaunchCheck( ydimln *ln )
{
  ydyo_activeInfo    *ainfo = (ydyo_activeInfo*)0;
  yoenv ev;

  yoEnvInit(&ev);

  

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1330,YSLSEV_INFO,(char*)0,
	   YSLUB4(ysLstCount(ln->rlist_ydimln)),
	   YSLUB4(ysLstCount(ln->alist_ydimln)), YSLEND);

  if( ln->rdy_ydimln && ln->rlist_ydimln )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1331,YSLSEV_INFO,(char*)0, YSLNONE );
    (*ln->rdy_ydimln)( ln->usrp_ydimln,
		      ln->rlist_ydimln,
		      ln->alist_ydimln,
		      ln->evt_ydimln ? (yslst*)0 : ln->ilist_ydimln );
  }

  

  if( ln->rlist_ydimln && !ysLstCount( ln->rlist_ydimln ) )
  {
    
    if( ln->evt_ydimln )
    {
      ysEvtDestroy( ln->evt_ydimln );
      ln->evt_ydimln = (ysevt*)0;
    }

    

    while( (ainfo = (ydyo_activeInfo*)ysLstDeq( ln->ilist_ydimln ) ) )
      ydim_active_destroyLocal( ainfo->self_ydyo_activeInfo, &ev );

    
    ysLstDestroy( ln->rlist_ydimln, (ysmff)0 );
    ysLstDestroy( ln->ilist_ydimln, (ysmff)0 );
    ysLstDestroy( ln->alist_ydimln, (ysmff)0 );
    ln->rlist_ydimln = (yslst *)0;
    ln->ilist_ydimln = (yslst *)0;
    ln->alist_ydimln = (yslst *)0;

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1332,YSLSEV_INFO,(char*)0, YSLNONE );
  }
  else if (!ln->evt_ydimln)
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1333,YSLSEV_EMERG,(char*)0, YSLNONE );
    ysePanic(YDIM_EX_INTERNAL);
  }
  yoEnvFree(&ev);

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1334,YSLSEV_INFO,(char*)0,
	   YSLUB4(ln->rlist_ydimln ? ysLstCount(ln->rlist_ydimln) : 0),
	   YSLUB4(ln->alist_ydimln ? ysLstCount(ln->alist_ydimln) : 0),
	   YSLEND);
}





STATICF sword ydimStringListCmp( CONST dvoid *a, CONST dvoid *b )
{
  yostd_stringList *ap = (yostd_stringList *)a;
  yostd_stringList *bp = (yostd_stringList *)b;
  sword rv = 0;
  ub4 i = 0;

  for( ; !rv && i < ap->_length && i < bp->_length ; i++ )
    rv = (sword)strcmp( ap->_buffer[i], bp->_buffer[i] );
  return( rv );
}




STATICF sword ydimLaunchCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv = 0;
  ydimln *ap = (ydimln *)a;
  ydimln *bp = (ydimln *)b;

  if(!(rv = (sword)strcmp( ap->host_ydimln, bp->host_ydimln ) ) &&
     !(rv = (sword)strcmp( ap->path_ydimln, bp->path_ydimln ) ) )
    rv = ydimStringListCmp((dvoid*)&ap->args_ydimln,
			   (dvoid*)&ap->args_ydimln );

  return( rv );
}





STATICF ydimln *ydimAddLaunch( ydimcx *cx, ydim_implInfo *info )
{
  ysspNode  *n;
  ydimln    *ln = (ydimln*)0;
  ydimln    lookup;

  lookup.host_ydimln = info->host_ydim_implInfo;
  lookup.path_ydimln = info->pathName_ydim_implInfo;
  lookup.args_ydimln._maximum = info->args_ydim_implInfo._maximum;
  lookup.args_ydimln._length = info->args_ydim_implInfo._length;
  lookup.args_ydimln._buffer = info->args_ydim_implInfo._buffer;
  lookup.host_ydimln = info->host_ydim_implInfo;

  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ltree_ydimcx ) ) )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1335,YSLSEV_DEBUG(4),(char*)0,
	     YSLSTR(info->host_ydim_implInfo),
	     YSLSTR(info->pathName_ydim_implInfo), YSLEND);

    ln = (ydimln*)n->key_ysspNode;
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1336,YSLSEV_DEBUG(4),(char*)0,
	     YSLSTR(info->host_ydim_implInfo),
	     YSLSTR(info->pathName_ydim_implInfo), YSLEND);

    ln = (ydimln*)ysmGlbAlloc( sizeof(*ln), ydimln_tag);
    ln->ydimln_spnode.key_ysspNode = (dvoid*)ln;
    ln->cx_ydimln = cx;
    ln->host_ydimln = yduCopyCacheStr(info->host_ydim_implInfo);
    ln->path_ydimln = yduCopyCacheStr(info->pathName_ydim_implInfo);
    yostd_stringList__copy( &ln->args_ydimln, &info->args_ydim_implInfo,
			   yotkAllocStr );
    ln->rlist_ydimln = (yslst*)0;
    ln->ilist_ydimln = (yslst*)0;
    ln->alist_ydimln = (yslst*)0;
    ln->usrp_ydimln = (dvoid*)0;
    ln->rdy_ydimln = (ydimRdyFunc)0;
    ln->evt_ydimln = (ysevt*)0;
    DISCARD ysspEnq( &ln->ydimln_spnode, &cx->ltree_ydimcx );
  }
  return( ln );
}





STATICF void ydimFreeLaunch( ydimln *ln )
{
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1337,YSLSEV_DEBUG(3),(char*)0, YSLNONE);

  

  if( ln->evt_ydimln )
  {
    ysEvtDestroy( ln->evt_ydimln );
    ln->evt_ydimln = (ysevt*)0;
  }
  if( ln->rlist_ydimln )
    ydimLaunchCheck( ln );

  

  
  if( ln->host_ydimln)
    yduFreeCacheStr( ln->host_ydimln );
  if( ln->path_ydimln )
    yduFreeCacheStr( ln->path_ydimln );

  yostd_stringList__free( &ln->args_ydimln, yotkFreeStr);
  ysmGlbFree( (dvoid*)ln );
}




STATICF yort_procInfo *ydimPInfoOfYort( ydimcx *cx, yort_proc y )
{
  ysspNode  *n;
  ydimy	    *yy;
  ydimy	    lookup;

  lookup.pinfo_ydimy.self_yort_procInfo = y;
  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ytree_ydimcx ) ) )
  {
    yy = (ydimy*)n->key_ysspNode;
    return( &yy->pinfo_ydimy );
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1338,YSLSEV_INFO,(char*)0, YSLNONE );
    return (yort_procInfo*)0;
  }
}






STATICF void ydimSyncComplete( dvoid *usrp, CONST ysid *exid,
			      dvoid *argp, size_t argsz )
{
  ydimsp    *sp = (ydimsp*)usrp;
  ysevt	    *sem;

  
  if( exid )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1339,YSLSEV_WARNING,(char*)0,
	     YSLUB4((ub4)sp->state_ydimsp),
	     YSLSTR(yduStr(ysidToStr( exid ))), YSLEND );
    ydimSyncDeath( usrp, exid, argp, argsz );
    return;
  }
  else
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1339,YSLSEV_INFO,(char*)0,
	     YSLUB4((ub4)sp->state_ydimsp),
	     YSLSTR(yduStr(ysidToStr( exid ))), YSLEND );

  
  ysEvtDestroy( sp->evt_ydimsp );

  switch( sp->state_ydimsp )
  {
  case sendInfo_ydimsp:
    ydimSyncInfo( sp );
    break;

  case sendActive_ydimsp:
    ydimSyncActive( sp );
    break;

  case sendTry_ydimsp:
    ydimSyncTry( sp );
    break;

  case sendCommit_ydimsp:

    sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
				 (ysque*)0, TRUE );

    if( argp && *(ydim_tryResult*)argp == ydim_success_tryResult )
    {
      
      sp->state_ydimsp = sendTry_ydimsp;
      ydim_imr_commitStake_nw(sp->dest_ydimsp, &sp->env_ydimsp,
			      &sp->claim_ydimsp, sp->evt_ydimsp);
    }
    else			
    {
      
      sp->state_ydimsp = sendRep_ydimsp;
      ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
    }
    break;

  case sendRep_ydimsp:
    ydimSyncReply( sp );
    break;

  case repComp_ydimsp:

    sem = sp->gsem_ydimsp;
    yoRelease( (dvoid*)sp->ykey_ydimsp );
    yoRelease( (dvoid*)sp->whenDone_ydimsp );
    yoRelease( (dvoid*)sp->dest_ydimsp );
    yoEnvFree( &sp->env_ydimsp );
    sp->cx_ydimsp->asp_ydimcx = (ydimsp*)0;

    
    if( sp->devt_ydimsp )
      ysEvtDestroy( sp->devt_ydimsp );

    ysmGlbFree( (dvoid*)sp );

    
    if( sem )
      ysTrigger( sem, (ysid*)0, (dvoid*)0, (size_t)0 );
    break;

  default:
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1340,YSLSEV_EMERG,(char*)0,
	     YSLUB4((ub4)(sword)sp->state_ydimsp), YSLEND );
    ysePanic(YDIM_EX_INTERNAL);
  }
}





STATICF void ydimSyncInfo( ydimsp *sp )
{
  ydimcx    *cx = sp->cx_ydimsp;
  ysspNode  *n;
  ydimn	    *on;
  ydim_implInfo	*ii;
  ydim_implInfo	lii;
  ydim_active   *obuffer;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1341,YSLSEV_DEBUG(3),(char*)0,
	   YSLSTR(yduStr( sp->ikey_ydimsp.intf_ydim_implInfo )),
	   YSLSTR(yduStr( sp->ikey_ydimsp.impl_ydim_implInfo )),
	   YSLSTR(yduStr( sp->ikey_ydimsp.host_ydim_implInfo )), YSLEND );

  sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
			       (ysque*)0, TRUE );

  if( sp->outobj_ydimsp )
    yoRelease( (dvoid*)sp->outobj_ydimsp );
  sp->outobj_ydimsp = (ydim_info)0;

  

  while( ( n = ysspNextLookup( (dvoid*)&sp->ikey_ydimsp,
			      &cx->itree_ydimcx ) ) )
  {
    on = (ydimn*)n;
    ii = (ydim_implInfo*)&on->info_ydimn;

    ydimKeySet( &sp->ikey_ydimsp, ii );

    if( ii->level_ydim_implInfo || ii->minInstances_ydim_implInfo
       || ii->maxInstances_ydim_implInfo != SB4MAXVAL ||
       ii->pathName_ydim_implInfo )
    {
      ydim_implInfo__copy( &lii, ii, yotkAllocStr );

      
      obuffer = lii.alist_ydim_implInfo._buffer;
      lii.alist_ydim_implInfo._buffer = (ydim_active*)0;
      lii.alist_ydim_implInfo._maximum =
	lii.alist_ydim_implInfo._length = 0;

      ydim_imr_addInfoLocal_nw( sp->dest_ydimsp, &sp->env_ydimsp,
			    &lii, &sp->outobj_ydimsp, sp->evt_ydimsp );

      lii.alist_ydim_implInfo._buffer = obuffer;
      lii.alist_ydim_implInfo._maximum =
	lii.alist_ydim_implInfo._length = ii->alist_ydim_implInfo._length;
      ydim_implInfo__free( &lii, yotkFreeStr );
      return;
    }
  }
  if( !n )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1342,YSLSEV_DEBUG(3),(char*)0, YSLNONE );
    ydim_implInfo__free( &sp->ikey_ydimsp, yotkFreeStr );

    
    sp->state_ydimsp = sendActive_ydimsp;
    ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
}





STATICF void ydimSyncActive( ydimsp *sp )
{
  ydimcx    *cx = sp->cx_ydimsp;
  ydimy	    *y;
  ysle	    *e;
  sb4	    i;
  ydyo_activeInfo  *aip;
  ysspNode  *n, *m;
  yslst	    *alist;
  ydimy	    lookup;

  ydyo_activeInfoList ailist;

  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1343,YSLSEV_DEBUG(3),(char*)0, YSLNONE );
  	
  sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
			       (ysque*)0, TRUE );

  
  lookup.pinfo_ydimy.self_yort_procInfo = sp->ykey_ydimsp;
  if( !(n = ysspNextLookup( (dvoid*)&lookup, &cx->ytree_ydimcx ) ) )
  {
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1344,YSLSEV_DEBUG(3),(char*)0, YSLNONE);
    yoRelease( (dvoid*)sp->ykey_ydimsp );
    sp->ykey_ydimsp = (yort_proc)0;

    
    sp->state_ydimsp = sendTry_ydimsp;
    ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
  else				
  {
    y = (ydimy*)n->key_ysspNode;
    alist = ysLstCreate();
    ailist._length = 0;

    
    for( m = ysspFHead( &cx->atree_ydimcx ); m ; m = ysspFNext( m ) )
    {
      aip = (ydyo_activeInfo *)m->key_ysspNode;
      if(yoIsEq((dvoid*)aip->yort_ydyo_activeInfo,
		(dvoid*)y->pinfo_ydimy.self_yort_procInfo))
	DISCARD ysLstEnq( alist, (dvoid *)aip );
    }

    ailist._length = ailist._maximum = ysLstCount( alist );
    yoRelease( (dvoid*)sp->ykey_ydimsp );

    sp->ykey_ydimsp =
      (yort_proc)yoDuplicate((dvoid*)y->pinfo_ydimy.self_yort_procInfo);

    
    if( ailist._length )
    {
      

      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1345,YSLSEV_DEBUG(3),(char*)0,
	       YSLUB4( ailist._length ), YSLEND );

      ailist._buffer = (ydyo_activeInfo*)
	ysmGlbAlloc( (size_t)ailist._length * sizeof(ydyo_activeInfo),
		    "active list");

      for( i = 0, e = ysLstHead( alist ); e ; e = ysLstNext( e ), i++ )
	ydyo_activeInfo__copy( &ailist._buffer[i],
			      (ydyo_activeInfo*)ysLstVal(e), yotkAllocStr );

      ysLstDestroy( alist, (ysmff)0 );

      ydim_imr_addActiveLocal_nw( sp->dest_ydimsp, &sp->env_ydimsp,
				 &y->pinfo_ydimy, &ailist, sp->evt_ydimsp );
      ydyo_activeInfoList__free( &ailist, yotkFreeStr );
    }
  }
}




STATICF void ydimSyncTry( ydimsp *sp )
{
  yort_claim *what;

  sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
			       (ysque*)0, TRUE );

  
  if((what = ydcaListNext(sp->cx_ydimsp->cacx_ydimcx,
			  sp->claim_ydimsp.property ?
			  &sp->claim_ydimsp : (yort_claim*)0)))
  {
    if( sp->claim_ydimsp.property )
      yort_claim__free( &sp->claim_ydimsp, yotkFreeStr );
    yort_claim__copy( &sp->claim_ydimsp, what, yotkAllocStr );

    sp->state_ydimsp = sendCommit_ydimsp;
    ydim_imr_tryStake_nw(sp->dest_ydimsp, &sp->env_ydimsp,
			 &sp->claim_ydimsp, sp->evt_ydimsp);
  }
  else				
  {
    sp->state_ydimsp = sendRep_ydimsp;
    ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
  }
}




STATICF void ydimSyncReply( ydimsp *sp )
{
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1346,YSLSEV_DEBUG(2),(char*)0, YSLNONE );

  sp->state_ydimsp = repComp_ydimsp;
  sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
			       (ysque*)0, TRUE );

  if( sp->whenDone_ydimsp )
    ydim_sync_decrement_nw( sp->whenDone_ydimsp,
			   &sp->cx_ydimsp->ev_ydimcx, sp->evt_ydimsp );
  else
    ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
}






STATICF void ydimSyncDeath( dvoid *usrp, CONST ysid *exid,
			   dvoid *argp, size_t argsz )
{
  ydimsp *sp = (ydimsp*)usrp;
  char *rs;

  rs = yoRefToStr((dvoid*)sp->dest_ydimsp );
  
  ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1347,YSLSEV_WARNING,(char*)0,
	   YSLSTR(rs), YSLEND ); 
  yoFree( (dvoid*)rs );

  

  switch( sp->state_ydimsp )
  {
  case sendInfo_ydimsp:
    ydim_implInfo__free( &sp->ikey_ydimsp, yotkFreeStr );
    break;

  case sendActive_ydimsp:
    yoRelease( (dvoid*)sp->ykey_ydimsp );
    sp->ykey_ydimsp = (yort_proc)0;
    break;

  case sendTry_ydimsp:
  case sendCommit_ydimsp:
    yort_claim__free( &sp->claim_ydimsp, yotkFreeStr );
    break;

  case sendRep_ydimsp:
  case repComp_ydimsp:
    break;

  default:
    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1348,YSLSEV_EMERG,(char*)0,
	     YSLUB4((ub4)sp->state_ydimsp), YSLEND );
    ysePanic( YDIM_EX_INTERNAL );
    break;
  }

  
  ysEvtDestroy( sp->evt_ydimsp );

  
  sp->state_ydimsp = repComp_ydimsp;
  sp->evt_ydimsp = ysEvtCreate( ydimSyncComplete, (dvoid*)sp,
			       (ysque*)0, TRUE );
  ysTrigger( sp->evt_ydimsp, (ysid*)0, (dvoid*)0, (size_t)0 );
}






STATICF void ydimSyncWait( ydimcx *cx )
{
  ydimsp *sp;
  ysevt *sem;

  
  if( (sp = cx->asp_ydimcx ) )
  {
    if( sp->gsem_ydimsp )
    {
      
      ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1349,YSLSEV_EMERG,(char*)0, YSLNONE );
      ysePanic(YDIM_EX_INTERNAL);
    }

    
    ysRecord(YS_PRODUCT,YDIM_FAC,(ub4)1350,YSLSEV_DEBUG(2),(char*)0, YSLNONE );

    
    sem = sp->gsem_ydimsp = ysSemCreate((dvoid*)0);
    ysSemWait(sem); 
    ysSemDestroy(sem); 
  }
}


