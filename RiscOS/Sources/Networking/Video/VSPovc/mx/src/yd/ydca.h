/* mx/src/yd/ydca.h */


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





#ifndef YDCA_ORACLE
#define YDCA_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif



typedef struct ydcacx ydcacx;









ydcacx *ydcaInit( ysque *q );




void ydcaTerm( ydcacx *cx );





void ydcaSetInactiveEvt( ydcacx *cx, ysevt *evt );







void ydcaStakeFor( ydcacx *cx, yort_claim *claim, yoevt reply );




void ydcaAbandonFor( ydcacx *cx, yort_claim *claim, boolean proxy );



void ydcaTransfer( ydcacx *cx, yort_claim* newClaim);








ydim_tryResult ydcaTryStake( ydcacx *cx, yort_claim *what );



ydim_tryResult ydcaTransferStake( ydcacx *cx, yort_claim *what );



void ydcaCommitStake( ydcacx *cx, yort_claim *what );



void ydcaAbortStake( ydcacx *cx, yort_claim *what );




yort_claim *ydcaListNext( ydcacx *cx, yort_claim *this );


#endif

