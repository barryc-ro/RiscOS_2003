/* mx/src/ye/yece.h */


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





#ifndef YECE_ORACLE
#define YECE_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YECEVCH_ORACLE
#include <yecevch.h>
#endif






yeceCa_Factory yeceInit( ysque *q );





void yeceTerm( yeceCa_Factory ev );
#endif
