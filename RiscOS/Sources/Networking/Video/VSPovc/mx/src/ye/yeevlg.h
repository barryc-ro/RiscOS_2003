/* mx/src/ye/yeevlg.h */


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





#ifndef YEEVLG_ORACLE
#define YEEVLG_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
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
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YEEVLOGI_H
#include <yeevlogI.h>
#endif
#ifndef YEEVENTI_ORACLE
#include <yeeventI.h>
#endif
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif



typedef struct yeevlgcx yeevlgcx;



yeevlgcx *yeevlgInit( ysque *q, yeev ev, boolean public );

void yeevlgTerm( yeevlgcx *cx );

yeevl_logProc yeevlgLogProc( yeevlgcx *cx );

yeevDiscList	yeevlgDefaultDiscList( yeevlgcx *cx );


boolean yeevlgFType( CONST char *s, yeevl_fullType *type );
boolean yeevlgLType( CONST char *s, yeevl_logType *type );

#endif

