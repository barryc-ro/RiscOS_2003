/* mx/src/ye/yeced.c */


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
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YECEVCH_ORACLE
#include <yecevch.h>
#endif
#ifndef YECE_ORACLE
#include <yece.h>
#endif

#define ENTRY_POINT yecedMain
#include <s0ysmain.c>



#define YECE_FAC    "YECE"


static struct ysargmap map[] =
{
   { 0, (char *) 0, 0 }
};


STATICF void yecedServer( void );
STATICF void yecedPanicHdlr(CONST ysid *exid, dvoid *ptr);







boolean yecedMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  sword     sts;
  char      vbuf[80];
  
  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, map);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB COSS Event Channel Daemon");
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  if (!ok)
    return(FALSE);
  
  yseSetPanic(yecedPanicHdlr, (dvoid*)0);
  
  ytInit();
  yoInit();
  yslDetach();			

  yseTry
  {
    yecedServer();
  }
  yseCatch( YS_EX_INTERRUPT )
  {
  }
  yseCatchAll
    yslError("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;

  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;
  ysTerm(osdp);
  return TRUE;
}








STATICF void yecedServer( void )
{
  yeceCa_Factory cef;
  ysque	    *q = (ysque*)0;
  yeev	    ev;
  
  ev = yeevRemoteLog( (yeev)0, q );

  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)560, YSLSEV_WARNING, (char*)0, YSLNONE);

  cef = yeceInit( q ); 

  yseSetPanic(yecedPanicHdlr, (dvoid*)cef);
  
  
  
  yseTry
    yoService( q );
  yseFinally
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)561, YSLSEV_WARNING, (char*)0, YSLNONE);


  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)562, YSLSEV_INFO, (char*)0, YSLNONE);
  
  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)563, YSLSEV_INFO, (char*)0, YSLNONE);


  
  ysRecord(YS_PRODUCT,YECE_FAC,(ub4)564, YSLSEV_WARNING, (char*)0, YSLNONE);
    yeceTerm( cef );
  yseEnd;

  yeevTerm( ev );
}







STATICF void yecedPanicHdlr(CONST ysid *exid, dvoid *ptr)
{
  yeceCa_Factory cef = (yeceCa_Factory)ptr;
  
  yslError("yecdPanicHdlr: exception %s, ptr %x\n", ysidToStr(exid), ptr);
  if( cef )
    yeceTerm( cef );
}

