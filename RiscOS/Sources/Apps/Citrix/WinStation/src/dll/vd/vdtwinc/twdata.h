
/*****************************************************************************\
*
*   TWDATA.H
*
*   Thin Wire Windows - references to global variables
*
*   Copyright (c) Citrix Systems Inc. 1992-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.0   03 Dec 1997 17:47:30   terryt
*  Initial revision.
*  
*     Rev 1.3   15 Apr 1997 18:16:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 14:46:52   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:35:10   kurtp
*  update
*  
\*****************************************************************************/

#ifndef __TWDATA_H__
#define __TWDATA_H__


// Note: This file must be modified in sync with TWDATA.C

extern jmp_buf3  vjmpSuspend;   
extern jmp_buf3  vjmpResume;   
extern jmp_buf3  vjmpComplete;   

extern THINWIRECAPS vThinWireMode;

extern char * vpszVersion;
extern BYTE vVersionL;
extern BYTE vVersionH;

#endif //__TWDATA_H__
