/* mx/src/ye/yeevf.h */


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





#ifndef YEEVF_ORACLE
#define YEEVF_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YODEFS_ORACLE
#include <yodefs.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif
#ifndef YEMSG_ORACLE
#include <yemsg.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
#endif







ysstr *yeevFormat( yemsgcx *msgcx, char *prod, char *fac, ub4 msgid,
		  yoany *ap, boolean crunch );

void yeevShowLRec( yemsgcx *msgcx, yeevl_yeevlr *lrec,
		   char *buf, size_t bsiz );

void yeevShowRec( yemsgcx *msgcx, yeevr *rec, char *buf, size_t bsiz );

void yeevShowAny( yotk *type, dvoid *value );

#endif

