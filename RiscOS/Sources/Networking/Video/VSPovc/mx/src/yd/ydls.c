/* mx/src/yd/ydls.c */


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
#ifndef YDQ_ORACLE
#include <ydq.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif

#define ENTRY_POINT ydlsMain
#include <s0ysmain.c>




static struct ysargmap ydlsmap[] =
{
  { 'a', "yd.show-active=true", 0 },
  { 's', "yd.show-spawn=true", 0 },
  { 0, (char *) 0, 0 }
};








boolean ydlsMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  sword     sts;
  boolean   ok;
  char       vbuf[80];
  ydim_imr  imr;

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, ydlsmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Daemon Query Utility");
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

    imr = (ydim_imr)yoBind(ydim_imr__id, (char *)0, (yoRefData*)0,(char*)0);
    ydqListAll( imr );
    yoRelease( (dvoid*)imr );
  };
  yseCatch( YS_EX_INTERRUPT )
  {
    yslError("interrupted\n");
    
  }
  yseCatchAll
  {
    yslPrint("%s caught exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  }
  yseEnd;

  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
  {    
    yslPrint("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  }
  yseEnd;
  ysTerm(osdp);
  return TRUE;
}

