/* mx/src/yd/ydsp.c */


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
#ifndef YDIDLI_ORACLE
#include <ydidlI.h>
#endif
#ifndef YDSP_ORACLE
#include <ydsp.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef SYSSP_ORACLE
#include <syssp.h>
#endif
#ifndef YDQ_ORACLE
#include <ydq.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif



#define YDSP_FAC    "YDSP"


typedef struct
{
  ysspNode	pnode_ydspn;	
  ydsp_procInfo	spinfo_ydspn;	
  boolean	alive_ydspn;	
} ydspn;

typedef struct
{			
  ydsp_spawner	self_ydspcx;
  ydim_imr	imr_ydspcx;
  sysspctx	*syssp_ydspcx;
  ysque		*q_ydspcx;
  ysspTree	ptree_ydspcx;	
  ub4		nptree_ydspcx;
} ydspcx;

externdef ysidDecl(YDSP_EX_INTERNAL) = "ydim::internal";

static CONST_W_PTR struct ydsp_spawner__tyimpl ydsp_spawner__impl =
 {
  ydsp_spawner_Launch_i,
  ydsp_spawner_addExisting_i,
  ydsp_spawner__get_self_i,
  ydsp_spawner__get_imr_i,
  ydsp_spawner__get_procs_i
 };

static CONST_W_PTR struct ydsp_proc__tyimpl ydsp_proc_impl =
 {
  ydsp_proc_isAlive_i,
  ydsp_proc_killProc_i,
  ydsp_proc_destroyObj_i,
  ydsp_proc__get_spinfo_i
 };



STATICF sword ydsp_procCmp( CONST dvoid *a, CONST dvoid *b ); 
STATICF boolean ydspIsAlive( ydspcx *cx, ydsp_procInfo *pi );



void ydspInit( ysque *q, ydsp_spawner *oydsp, ydim_imr imr, dvoid *osdp )
{
  ydspcx *cx = (ydspcx*)ysmGlbAlloc( sizeof(*cx), "ydspcx" );
  yoenv ev;

  yoEnvInit(&ev);

  cx->q_ydspcx = q;
  cx->imr_ydspcx = (ydim_imr)yoDuplicate((dvoid*)imr);
  
  DISCARD ysspNewTree( &cx->ptree_ydspcx, ydsp_procCmp );
  cx->nptree_ydspcx = 0;

  yoSetImpl( ydsp_spawner__id, (char*)0, ydsp_spawner__stubs,
	    (dvoid*)&ydsp_spawner__impl, (yoload)0, TRUE, (dvoid*)cx );

  yoSetImpl( ydsp_proc__id, (char*)0, ydsp_proc__stubs,
	    (dvoid*)&ydsp_proc_impl, (yoload)0, FALSE, (dvoid*)cx );

  
  cx->self_ydspcx =
    (ydsp_spawner)yoCreate( ydsp_spawner__id, (char*)0,
			   (yoRefData*)0, (char*)0, (dvoid *)cx);

  *oydsp = (ydsp_spawner)yoDuplicate((dvoid*)cx->self_ydspcx);

  cx->syssp_ydspcx = sysspInit( osdp );

  yoImplReady( ydsp_spawner__id, (char*)0, cx->q_ydspcx );
  yoImplReady( ydsp_proc__id, (char*)0, cx->q_ydspcx );

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1400,YSLSEV_INFO, (char*)0, YSLNONE );

  yoEnvFree(&ev);
}

void ydspTerm( ydsp_spawner or, boolean killChildren )
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ysspNode  *n;
  ydspn	    *pn;

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1401,YSLSEV_INFO, (char*)0, YSLNONE );
  
  yoImplDeactivate( ydsp_spawner__id, (char*)0 );

  while( (n = ysspDeq( &cx->ptree_ydspcx.root_ysspTree ) ) )
  {
    cx->nptree_ydspcx--;
    pn = (ydspn*)n->key_ysspNode;
    if( killChildren && pn->alive_ydspn &&
       yoIsEq((dvoid*)pn->spinfo_ydspn.parent_ydsp_procInfo,
	      (dvoid*)cx->self_ydspcx ) &&
       ydspIsAlive( cx, &pn->spinfo_ydspn ) )
      DISCARD sysspKill( cx->syssp_ydspcx,
			pn->spinfo_ydspn.pid_ydsp_procInfo, FALSE );

    yoDispose( (dvoid*)pn->spinfo_ydspn.self_ydsp_procInfo );
    ydsp_procInfo__free( &pn->spinfo_ydspn, yotkFreeStr );
    ysmGlbFree( (dvoid*) pn );
  }

  sysspTerm( cx->syssp_ydspcx );
  yoRelease( (dvoid*)cx->imr_ydspcx );
  yoRelease( (dvoid*)cx->self_ydspcx );
  yoDispose( (dvoid*)cx->self_ydspcx );
  ysmGlbFree( (dvoid*)cx );
}


ydsp_spawner ydsp_spawner__get_self_i( ydsp_spawner or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  return( (ydsp_spawner)yoDuplicate((dvoid*)cx->self_ydspcx) );
}


ydim_imr ydsp_spawner__get_imr_i( ydsp_spawner or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  return( (ydim_imr)yoDuplicate((dvoid*)cx->imr_ydspcx) );
}



ydsp_procInfoList ydsp_spawner__get_procs_i( ydsp_spawner or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ysspNode  *n;
  ydspn	    *sn;
  sb4	    i;

  ydsp_procInfoList l;

  l._maximum = l._length = cx->nptree_ydspcx;
  l._buffer =
    (ydsp_procInfo*)yoAlloc( (size_t)l._length * sizeof(ydsp_procInfo) );
  
  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1410,YSLSEV_DEBUG(1), (char*)0, YSLNONE );

  
  i = 0;
  for( n = ysspFHead( &cx->ptree_ydspcx ); n ; n = ysspFNext(n), i++ )
  {
    sn = (ydspn*)n->key_ysspNode;
    if(sn->alive_ydspn)
      sn->alive_ydspn = ydspIsAlive(cx, &sn->spinfo_ydspn);
    ydsp_procInfo__copy( &l._buffer[i], &sn->spinfo_ydspn, yoAlloc );
  }

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1411,YSLSEV_DEBUG(1), (char*)0,
	   YSLUB4(l._length), YSLEND );

  return l;
}



void ydsp_spawner_Launch_i( ydsp_spawner or, yoenv *ev,
			   char* path, yostd_stringList* args,
			   ydsp_proc* proc)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ydsp_procInfo spinfo;
  char	    pidbuf[ 64 ];
  sword	    sts;
  
  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1412,YSLSEV_INFO, (char*)0,
	   YSLSTR( path ), YSLEND );
	   
  
  CLRSTRUCT(spinfo);
  spinfo.self_ydsp_procInfo = (ydsp_proc)0;
  spinfo.pid_ydsp_procInfo = (char*)0;

  sts = sysspSpawn( cx->syssp_ydspcx, pidbuf, sizeof(pidbuf), 
		   path, (sword)args->_length, args->_buffer );

  if( sts == SYSSP_STS_STARTED || sts == SYSSP_STS_RUNNING ||
     sts == SYSSP_STS_ALIVE || sts == SYSSP_STS_STOPPED )
  {
    spinfo.parent_ydsp_procInfo = cx->self_ydspcx;
    spinfo.pid_ydsp_procInfo = pidbuf;
    spinfo.host_ydsp_procInfo = (char*)ysGetHostName();
    spinfo.affinity_ydsp_procInfo = (char *)0;
    
    spinfo.name_ydsp_procInfo = (char *)0;
    spinfo.state_ydsp_procInfo = ydsp_start_ok;
    ydsp_spawner_addExisting_i( or, ev, &spinfo, proc ); 

    
    ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1413,YSLSEV_INFO, (char*)0, YSLNONE);
  }
  else
  {
    yduFreeCacheStr( spinfo.pid_ydsp_procInfo );

    
    ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1414,YSLSEV_WARNING, (char*)0,
	     YSLSTR(path), YSLEND );
    yseThrow(YDSP_SPAWNER_EX_LAUNCHFAIL);
  }
}




void ydsp_spawner_addExisting_i( ydsp_spawner or, yoenv *ev,
				ydsp_procInfo* spinfo,
				ydsp_proc* proc)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);

  ysspNode  *n;
  ydspn	    *pn;		
  ydsp_procInfo  *pi;		
  ydspn	    lookup;

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1415,YSLSEV_INFO, (char*)0,
	   YSLSTR(yduStr(spinfo->host_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->pid_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->affinity_ydsp_procInfo)), YSLEND );

  
  CLRSTRUCT(lookup);
  lookup.spinfo_ydspn.host_ydsp_procInfo = spinfo->host_ydsp_procInfo;
  lookup.spinfo_ydspn.pid_ydsp_procInfo = spinfo->pid_ydsp_procInfo;
  lookup.spinfo_ydspn.affinity_ydsp_procInfo = spinfo->affinity_ydsp_procInfo;
  if( (n = ysspLookup( (dvoid *)&lookup, &cx->ptree_ydspcx ) ) )
  {
    
    pn = (ydspn*)n->key_ysspNode;
    pi = &pn->spinfo_ydspn;
    ysspRemove( n, &cx->ptree_ydspcx );
    yduFreeCopyStr( &pi->affinity_ydsp_procInfo,
		   spinfo->affinity_ydsp_procInfo,
		   yotkFreeStr, yotkAllocStr );
    yduFreeCopyStr( &pi->name_ydsp_procInfo,
		   spinfo->name_ydsp_procInfo,
		   yotkFreeStr, yotkAllocStr );
    DISCARD ysspEnq( n, &cx->ptree_ydspcx );

    
    ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1416,YSLSEV_DEBUG(1),(char*)0,YSLNONE );
  }
  else				
  {
    pn = (ydspn*) ysmGlbAlloc( sizeof(*pn), "ydspn" );
    pn->pnode_ydspn.key_ysspNode = (dvoid *)pn;
    pn->alive_ydspn = TRUE;	
    pi = &pn->spinfo_ydspn;
    ydsp_procInfo__copy( pi, spinfo, yotkAllocStr ); 
    pi->self_ydsp_procInfo =
      (ydsp_proc)yoCreate( ydsp_proc__id, (char*)0,
			  (yoRefData*)0, (char *)0, (dvoid *)pn);

    DISCARD ysspEnq( &pn->pnode_ydspn, &cx->ptree_ydspcx );
    cx->nptree_ydspcx++;

    
    ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1417,YSLSEV_DEBUG(1),(char*)0, YSLNONE);
  }
  *proc = (ydsp_proc)yoDuplicate((dvoid*)pi->self_ydsp_procInfo);

  if( pn->alive_ydspn )
    pn->alive_ydspn = ydspIsAlive( cx, &pn->spinfo_ydspn );
}



ydsp_procInfo ydsp_proc__get_spinfo_i( ydsp_proc or, yoenv *ev)
{
  ydspcx *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ydspn  *pn = (ydspn *)yoGetState((dvoid*)or);
  ydsp_procInfo rv;

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1418,YSLSEV_DEBUG(1),(char*)0, YSLNONE);
  
  if(pn->alive_ydspn)
    pn->alive_ydspn = ydspIsAlive( cx, &pn->spinfo_ydspn );

  ydsp_procInfo__copy( &rv, &pn->spinfo_ydspn, yoAlloc );
  return rv;
}




boolean ydsp_proc_isAlive_i( ydsp_proc or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ydspn	    *pn = (ydspn *)yoGetState((dvoid*)or);
  ydsp_procInfo	*spinfo = &pn->spinfo_ydspn;

  

  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1419,YSLSEV_DEBUG(1),(char*)0, 
	   YSLSTR(yduStr(spinfo->host_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->pid_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->affinity_ydsp_procInfo)), YSLEND );

  if( pn->alive_ydspn )
    pn->alive_ydspn = ydspIsAlive( cx, &pn->spinfo_ydspn );
  
  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1420,YSLSEV_DEBUG(1),(char*)0,
	   YSLSTR( pn->alive_ydspn ? "alive" : "DEAD" ), YSLEND );

  return pn->alive_ydspn;
}




void ydsp_proc_killProc_i( ydsp_proc or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ydspn	    *pn = (ydspn *)yoGetState((dvoid*)or);
  ydsp_procInfo	*spinfo = &pn->spinfo_ydspn;

  
  ysRecord(YS_PRODUCT, YDSP_FAC, (ub4)1421, YSLSEV_INFO, (char*)0,
	   YSLSTR(yduStr(spinfo->host_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->pid_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->affinity_ydsp_procInfo)), YSLEND );

   if( pn->alive_ydspn && 
     (pn->alive_ydspn = ydspIsAlive(cx, &pn->spinfo_ydspn)))
     DISCARD sysspKill( cx->syssp_ydspcx,
		       pn->spinfo_ydspn.pid_ydsp_procInfo, FALSE );
  
  

  pn->alive_ydspn = ydspIsAlive(cx, &pn->spinfo_ydspn);
				
  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1422,YSLSEV_INFO, (char*)0,
	   YSLSTR( pn->alive_ydspn ? "alive" : "DEAD" ), YSLEND );
}



void ydsp_proc_destroyObj_i( ydsp_proc or, yoenv *ev)
{
  ydspcx    *cx = (ydspcx*)yoGetImplState((dvoid *)or);
  ydspn	    *pn = (ydspn *)yoGetState((dvoid*)or);
  ydsp_procInfo	*spinfo = &pn->spinfo_ydspn;
  
  
  ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1423,YSLSEV_INFO, (char*)0,
	   YSLSTR(yduStr(spinfo->host_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->pid_ydsp_procInfo)),
	   YSLSTR(yduStr(spinfo->affinity_ydsp_procInfo)), YSLEND );

  sysspForget( cx->syssp_ydspcx, spinfo->pid_ydsp_procInfo );
  ysspRemove( (ysspNode *)pn, &cx->ptree_ydspcx );
  cx->nptree_ydspcx--;
  ydsp_procInfo__free( spinfo, yotkFreeStr );
  yoDispose( (dvoid*)or );
  ysmGlbFree( (dvoid*)pn );
}





  
STATICF sword ydsp_procCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv;
  ydsp_procInfo  *ap = &((ydspn *)a)->spinfo_ydspn;
  ydsp_procInfo  *bp = &((ydspn *)b)->spinfo_ydspn;
  
  if(!(rv = yduSafeStrcmp(ap->host_ydsp_procInfo, bp->host_ydsp_procInfo)) &&
     !(rv = (sword)strcmp(ap->pid_ydsp_procInfo, bp->pid_ydsp_procInfo)) )
    rv = yduWildStrcmp(ap->affinity_ydsp_procInfo, bp->affinity_ydsp_procInfo);
  
  return( rv );
}




STATICF boolean ydspIsAlive( ydspcx *cx, ydsp_procInfo *pi )
{
  boolean rv = FALSE;
  sword sts;

  switch( (sts = sysspGetStatus( cx->syssp_ydspcx, pi->pid_ydsp_procInfo ) ) )
  {
  case SYSSP_STS_NOTFOUND:
    pi->state_ydsp_procInfo = ydsp_exit_unknown;
    rv = FALSE;
    break;
  case SYSSP_STS_EXITOK:
    pi->state_ydsp_procInfo = ydsp_exit_ok;
    rv = FALSE;
    break;
  case SYSSP_STS_EXITNOTOK:
    pi->state_ydsp_procInfo = ydsp_exit_bad;
    rv = FALSE;
    break;

  case SYSSP_STS_ALIVE:
  case SYSSP_STS_RUNNING:
  case SYSSP_STS_STOPPED:
    pi->state_ydsp_procInfo = ydsp_start_ok;
    rv = TRUE;
    break;

  default:
    
    ysRecord(YS_PRODUCT,YDSP_FAC,(ub4)1424,YSLSEV_EMERG, (char*)0,
	     YSLUB4((ub4)sts), YSLEND );
    ysePanic(YDSP_EX_INTERNAL);
  }
  return rv;
}
