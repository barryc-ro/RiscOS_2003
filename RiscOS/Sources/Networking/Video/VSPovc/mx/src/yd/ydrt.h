/* mx/src/yd/ydrt.h */


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





#ifndef YDRT_ORACLE
#define YDRT_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDMT_ORACLE
#include <ydmt.h>
#endif






void ydrtInit( ysque *q, ydrt_router *oydrt_router, ydim_imr imr );




void ydrtStartRouting( ydrt_router ydrt );




void ydrtTerm( ydrt_router rt );



#endif

