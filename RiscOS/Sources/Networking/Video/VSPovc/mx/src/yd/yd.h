/* mx/src/yd/yd.h */


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





#ifndef YD_ORACLE
#define YD_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YDNMIDL_ORACLE
#include <ydnmidl.h>
#endif



typedef struct {

  ysque	*q_ydcx;
  boolean server_ydcx;		
  sysb8	timo_ydcx;

  ydim_imr	ydim_ydcx;	
  ydrt_router	ydrt_ydcx;	
  ydmt_imtr	ydmt_ydcx;	
  ydsp_spawner	ydsp_ydcx;	
  ydch_och	ydch_ydcx;	

  ysevt		*ssem_ydcx;	
  ysid		*sexid_ydcx;	

  yoenv		env_ydcx;
  
} ydcx;



#endif

