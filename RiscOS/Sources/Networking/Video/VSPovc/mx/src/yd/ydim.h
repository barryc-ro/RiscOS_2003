/* mx/src/yd/ydim.h */


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





#ifndef YDIM_ORACLE
#define YDIM_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif




externref ysidDecl( YDIM_EX_TOO_MANY );

typedef struct ydimain	ydimain;


struct ydimain
{
 ysspNode	    anode_ydimain;  
 ydyo_activeInfo    ainfo_ydimain;
 ub4		    ucnt_ydimain;
 dvoid		    *usrp_ydimain; 
};







void ydimInit( ysque *imq, ydim_imr *oydim_imr );
void ydimTerm( ydim_imr imr );


void ydimSetRefs( ydim_imr imr, ydmt_imtr mt, ydsp_spawner sp );









typedef void (*ydimSyncFunc)( ydim_sync or, boolean decr, dvoid *usrp);



void ydimSetSyncHandler( ydim_sync or, ydimSyncFunc hdlr, dvoid *usrp );








ydimain *ydimActiveGetAinfo( CONST ydim_active or );



void ydimActiveSetUsrp( CONST ydim_imr imr, yort_proc y,
		       char *intf, char *impl, dvoid *usrp );




yslst *ydimListImpl( yslst *lst, ydim_imr imr,
		    char *intf, char *impl, char *host );









typedef void (*ydimRdyFunc)( dvoid *usrp, yslst *reqs,
			    yslst *active, yslst *timedout );




yort_proc ydimStartLaunch( ydim_imr imr, ydim_implInfo *info, sysb8 *timeout,
			  dvoid *usrp, ydimRdyFunc rdy, dvoid *element );




boolean ydimIsLaunching( ydim_imr imr, yort_proc y );



void ydimAppendLaunch( ydim_imr imr, ydim_implInfo *info, dvoid *element );

#endif


