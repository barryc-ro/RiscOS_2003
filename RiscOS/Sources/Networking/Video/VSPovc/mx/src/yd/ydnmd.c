/* mx/src/yd/ydnmd.c */


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
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YDNM_ORACLE
#include <ydnm.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif

#define ENTRY_POINT ydnmdMain
#include <s0ysmain.c>



#define YDNMD_FAC   "YDUT"


static struct ysargmap ydnmdmap[] =
{
   { 0, (char *) 0, 0 }
};


STATICF void ydnmdServer(void);






boolean ydnmdMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  sword     sts;
  char      vbuf[80];

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydnmdmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Name Server");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);
  
  
  if (!ok)
    return(FALSE);
  
  yslDetach();			
  
  yseTry
  {
    ytInit();
    yoInit();
    ydnmdServer();
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
    yslError("%s caught exception %s while exiting\n", ysProgName(), yseExid );
  yseEnd;
  ysTerm(osdp);
  return TRUE;
}








STATICF void ydnmdServer(void)
{
  ydnmNamingContext nm;
  ysque	    *q = (ysque*)0;
  
  
  ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1530,YSLSEV_INFO,(char*)0, YSLNONE);

  

  nm = ydnmInit( q );

  
  
  if( yoClaim( "yoir:NameService", (dvoid*)nm ) )
  {
    
    ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1531,YSLSEV_INFO,(char*)0,YSLNONE);

    yoService( q );

    
    ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1532,YSLSEV_INFO,(char*)0,YSLNONE);
    
    yoAbandon( "yoir:NameService" );
  
    
    ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1533,YSLSEV_INFO,(char*)0, YSLNONE );
    
  }
  else
  {
    
    ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1534,YSLSEV_INFO,(char*)0, YSLNONE );
  }
  ydnmTerm( nm );

  
  ysRecord(YS_PRODUCT,YDNMD_FAC,(ub4)1535,YSLSEV_INFO,(char*)0, YSLNONE );
}

