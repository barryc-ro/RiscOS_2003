/* mx/src/yd/ydspps.c */


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
#ifndef YDSP_ORACLE
#include <ydsp.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif

#define ENTRY_POINT ydsppsMain
#include <s0ysmain.c>




static struct ysargmap ydsppsmap[] =
{
  { 0, (char *) 0, 0 }
};




STATICF sword ydsppsCmp( CONST dvoid *a, CONST dvoid *b );
STATICF void ydsppsGetProcs(ysspTree *t);
STATICF void ydsppsShowProcs(ysspTree *t);






boolean ydsppsMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  sword     sts;
  char       vbuf[80];
  ysspTree  t;

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydsppsmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Daemon Process Query Utility");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  yseTry
  {
    ytInit();
    yoInit();
    DISCARD ysspNewTree( &t, ydsppsCmp );
    ydsppsGetProcs(&t);
    ydsppsShowProcs(&t);
  };
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
    yslPrint("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;
  ysTerm(osdp);
    
  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n", ysProgName(), yseExid );
  yseEnd;
  return TRUE;
}



STATICF void ydsppsGetProcs(ysspTree *t)
{
  ydsp_procInfoList plist;
  ub4		    i;
  ydsp_procInfo	    *pinfo;
  ydsp_procInfo	    *tinfo;
  ysspNode	    *n;
  yoenv		    ev;
  ydsp_spawner sp = (ydsp_spawner)yoBind( ydsp_spawner__id, (char*)0,
					 (yoRefData*)0, (char*)0 );

  yoEnvInit(&ev);
  plist = ydsp_spawner__get_procs( sp, &ev );

  
  for( i = 0; i < plist._length ; i++ )
  {
    tinfo = (ydsp_procInfo*)0;
    pinfo = &plist._buffer[i];
    
    if( (n = ysspLookup( (dvoid*)pinfo, t)) &&
       plist._buffer[i].parent_ydsp_procInfo )
    {
      tinfo = (ydsp_procInfo*)n->key_ysspNode;
      ysspRemove( n, t );
      ydsp_procInfo__free( tinfo, (ysmff)0 );
      ysmGlbFree( (dvoid*)tinfo );
      ysmGlbFree( (dvoid*)n );
      tinfo = (ydsp_procInfo*)0;
    }

    if( !tinfo )
    {
      n = (ysspNode*)ysmGlbAlloc(sizeof(*n), "ysspNode");
      tinfo = (ydsp_procInfo*)ysmGlbAlloc( sizeof(*tinfo), "ydsp_procInfo" );
      ydsp_procInfo__copy( tinfo, pinfo, (ysmaf)0 );
      n->key_ysspNode = (dvoid*)tinfo;
      DISCARD ysspEnq( n, t );
    }
  }
  ydsp_procInfoList__free( &plist, yoFree);
  yoEnvFree(&ev);
}

STATICF void ydsppsShowProcs(ysspTree *t)
{
  ysspNode *n;

  ydqShowSProcHdr();
  while( (n = ysspDeq( &t->root_ysspTree ) ) )
  {
    ydqShowSProc( (ydsp_procInfo*)n->key_ysspNode );
    ydsp_procInfo__free( (ydsp_procInfo*)n->key_ysspNode, (ysmff)0);
    ysmGlbFree( n->key_ysspNode );
    ysmGlbFree( (dvoid*)n );
  }
}


STATICF sword ydsppsCmp( CONST dvoid *a, CONST dvoid *b )
{
  ydsp_procInfo *ap = (ydsp_procInfo *)a;
  ydsp_procInfo *bp = (ydsp_procInfo *)b;
  sword rv;

  if(!(rv = (sword)strcmp( ap->host_ydsp_procInfo, bp->host_ydsp_procInfo)) &&
     !(rv = (sword)strcmp( ap->pid_ydsp_procInfo, bp->pid_ydsp_procInfo)))
    rv = yduSafeStrcmp( ap->affinity_ydsp_procInfo,
		       bp->affinity_ydsp_procInfo);

  return( rv );
}


