/* mx/src/yd/ydchadm.c */


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
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif

#define ENTRY_POINT ydchadmMain
#include <s0ysmain.c>




static struct ysargmap ydchadmmap[] =
{
  { 'a', "mnobjadm.show-active=true", 0 },
  { 'd', "mnobjadm.deactivate-object", YSARG_MANY },
  { 'l', "mnobjadm.lock-object", 1 },
  { 's', "mnobjadm.show-seed-objs=true", 0 },
  { 't', "mnobjadm.new-owner-object", 1 },
  { 'u', "mnobjadm.unlock-object", 1 },
  { 'x', "mnobjadm.test=true", 0 },
  { 0, (char *) 0, 0 }
};

#define YDCH_BATCH_SIZE 100



STATICF void ydchadmDeactivate(yslst *lst);
STATICF void ydchadmLock( dvoid *unbound );
STATICF void ydchadmUnlock( dvoid *unbound );
STATICF void ydchadmTransfer( dvoid *unbound, dvoid *place );
STATICF void ydchadmList(void);
STATICF void ydchadmSeed(void);
STATICF void ydchadmTest(void);






boolean ydchadmMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  sword     sts;
  boolean   ok;
  char       vbuf[80];
  char	    *arg;
  dvoid	    *noreg lunbound;
  dvoid	    *noreg uunbound;
  dvoid	    *noreg place;
  yslst	    *lst;
  boolean seed;

  NOREG(lunbound);
  NOREG(uunbound);
  NOREG(place);

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydchadmmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange Object Clearinghouse Admin Utility");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  seed = ysResGetBool("mnobjadm.show-seed-objs");

  if( (arg = ysResGetLast("mnobjadm.lock-object")))
    lunbound = yoStrToRef(arg);
  else
    lunbound = (dvoid*)0;

  if( (arg = ysResGetLast("mnobjadm.unlock-object")))
    uunbound = yoStrToRef(arg);
  else
    uunbound = (dvoid*)0;

  if( (arg = ysResGetLast("mnobjadm.new-owner-object")))
    place = yoStrToRef(arg);
  else
    place = (dvoid*)0;

  yseTry
  {
    ytInit();
    yoInit();

    if( ysResGetBool("mnobjadm.test") )
      ydchadmTest();
    else if( seed )
      ydchadmSeed(); 
    else if( lunbound && place )
      ydchadmTransfer( lunbound, place );
    else if( lunbound )
      ydchadmLock( lunbound );
    else if( uunbound )
      ydchadmUnlock( uunbound );
    else if( (lst = ysResGet("mnobjadm.deactivate-object")) )
    {
      ydchadmDeactivate( lst );
    }
    else			
    {
      ydchadmList();
    }
  };
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
    yslPrint("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;
    
  yseTry
  {
    yoRelease( lunbound );
    yoRelease( uunbound );
    yoRelease( place );

    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n", ysProgName(), yseExid );
  yseEnd;

  ysTerm(osdp);
  return TRUE;
}



STATICF void ydchadmDeactivate( yslst *lst )
{
  yslError("Deactivate unimplemented\n");
}

STATICF void ydchadmLock( dvoid *unbound )
{
  ydch_och och;
  yoenv	ev;
  yort_proc y;

  yoEnvInit( &ev );
  och = yoBind( ydch_och__id, (char*)0, (yoRefData*)0, (char*)0 );
  ydch_och_lock( och, &ev, unbound, &y );
  yoRelease( y );
  yoRelease( och );
  yoEnvFree( &ev );
}

STATICF void ydchadmUnlock( dvoid *unbound )
{
  ydch_och och;
  yoenv	ev;

  yoEnvInit( &ev );
  och = yoBind( ydch_och__id, (char*)0, (yoRefData*)0, (char*)0 );

  ydch_och_remove( och, &ev, unbound );

  yoRelease( och );
  yoEnvFree( &ev );
}


STATICF void ydchadmTransfer( dvoid *unbound, dvoid *place )
{
  ydch_och och;
  yoenv	ev;

  yoEnvInit( &ev );
  och = yoBind( ydch_och__id, (char*)0, (yoRefData*)0, (char*)0 );
  ydch_och_setPlace( och, &ev, unbound, place );
  yoRelease( och );
  yoEnvFree( &ev );
}

STATICF void ydchadmList(void)
{
  yostd_objList objs;
  CORBA_Object ref;
  char	*rs1;
  char	*rs2;
  ydch_objIterator oi;
  ydch_och och;
  yoenv	ev;
  ub4 i;

  yoEnvInit( &ev );
  och = yoBind( ydch_och__id, (char*)0, (yoRefData*)0, (char*)0 );
  ydch_och_listObjs( och, &ev, YDCH_BATCH_SIZE, &objs, &oi );
  
  yslPrint("Clearinghouse Known Object - Activated Location\n");
  yslPrint("------------------------------------------------------------\n");
  for( i = 0; i < objs._length ; i++ )
  {
    rs1 = yoRefToStr( objs._buffer[ i ] );
    ydch_och_whereActive( och, &ev, objs._buffer[ i ], (yort_proc*)&ref );
    rs2 = yoRefToStr( ref );
    yslPrint("%s - %s\n", rs1, rs2 );
    yoFree( (dvoid*)rs2 );
    yoRelease( ref );
    yoFree( (dvoid*)rs1 ); 
    yostd_objList__free( &objs, yoFree );
    if( !oi || !ydch_objIterator_next_n( oi, &ev, YDCH_BATCH_SIZE, &objs ) )
      break;
  }
  if( oi )
    ydch_objIterator_destroy( oi, &ev );
}



STATICF void ydchadmSeed(void)
{
  ydim_imr imr;
  yort_proc y;
  char *rs;

  imr = yoBind( ydim_imr__id, (char*)0, (yoRefData*)0, (char*)0 );
  y = yoYort();

  rs = yoRefToStr( imr );
  yslPrint("loose ref: %s\n", rs );
  yoFree((dvoid*)rs );
  
  rs = yoRefToStr( y );
  yslPrint("place ref: %s\n", rs );
  yoFree((dvoid*)rs );

  yoRelease(y);
  yoRelease(imr);
}


STATICF void ydchadmTest()
{
  dvoid *lunbound;
  dvoid *r;
  sysb8 dly;
  ysevt *sem;

  lunbound = yoBind( ydim_imr__id, (char*)0, (yoRefData*)0, (char*)0 );
  sysb8ext( &dly, 3 * 1000000 );

  
  yslPrint("initial state:\n");
  ydchadmList();

  ydchadmUnlock( lunbound );

  yslPrint("(delay for unlock propagation)\n");
  sem = ysSemCreate((dvoid*)0);
  ysTimer( &dly, sem );
  ysSemWait( sem );
  ysSemDestroy( sem );

  yslPrint("after initial unlock\n");
  ydchadmList();

  
  ydchadmLock( lunbound );

  yslPrint("after initial lock:\n");
  ydchadmList();

  r = yoYort();
  ydchadmTransfer( lunbound, r );
  yoRelease( r );
  yslPrint("(delay for transfer propagation)\n");
  sem = ysSemCreate((dvoid*)0);
  ysTimer( &dly, sem );
  ysSemWait( sem );
  ysSemDestroy( sem );

  yslPrint("after transfer\n");
  ydchadmList();

  ydchadmUnlock( lunbound );
  yslPrint("(delay for unlock propagation)\n");
  sem = ysSemCreate((dvoid*)0);
  ysTimer( &dly, sem );
  ysSemWait( sem );
  ysSemDestroy( sem );

  yslPrint("after final unlock\n");
  ydchadmList();

  yoRelease( lunbound );
}


