/* mx/src/ye/yemsg.h */


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





#ifndef YEMSG_ORACLE
#define YEMSG_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YSLST_ORACLE
#include <yslst.h>
#endif
#ifndef YSSTR_ORACLE
#include <ysstr.h>
#endif



typedef struct yemsgcx yemsgcx;






yemsgcx *yemsgInit(void);



void yemsgTerm( yemsgcx *cx );




CONST char *yemsgLookup( yemsgcx *cx, char *proc, char *fac, ub4 msgId );

#endif

