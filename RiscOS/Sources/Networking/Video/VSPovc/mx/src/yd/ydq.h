/* mx/src/yd/ydq.h */


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





#ifndef YDQ_ORACLE
#define YDQ_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YDMTDIDL_ORACLE
#include <ydmtdidl.h>
#endif






void ydqListAll( ydim_imr imr );




void ydqInfoHdr(void);



void ydqActiveHdr(void);



void ydqProcHdr(void);




void ydqShowInfoList( ydim_infoList *list );
void ydqShowInfo( ydim_implInfo *info );




void ydqShowActiveInfo( ydyo_activeInfo* ainfo );



void ydqShowProc( yort_procInfo *pinfo );



void ydqShowSProcHdr(void);




void ydqShowSProc( ydsp_procInfo *spinfo );




void ydqShowMtdList( ydmtdMetricsList *mlist );




void ydqShowDispInfoList( yort_dispInfoList *dlist );

void ydqDispInfoHdr( ub4 n );
void ydqDispInfo( yort_dispInfo *dinfo, char *queueName );



void ydqShowImplAllList( yort_implAllList *ialist, boolean terse );


void ydqQInfoHdr(void);
void ydqQInfo( yort_queueInfo *info );

void ydqShowYortQueueList( yort_queueList *qlist );

CONST char *ydqSvcState( yort_svcState state );
CONST char *ydqProcState( yort_procState state );

#endif


