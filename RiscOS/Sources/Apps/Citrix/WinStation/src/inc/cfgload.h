
/*************************************************************************
*
*   CFGLOAD.H
*
*   Configuration Load library header file
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (4/12/95)
*
*   $Log$
*  
*     Rev 1.4   15 Apr 1997 18:44:46   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.3   06 May 1996 18:35:44   jeffm
*  update
*  
*     Rev 1.2   20 Jul 1995 09:41:06   marcb
*  update
*  
*     Rev 1.1   02 May 1995 13:38:50   butchd
*  update
*
*
*************************************************************************/

#ifndef __CFGLOAD_H__
#define __CFGLOAD_H__

/*
 *  Library routines
 */
int   LoadPd( PCHAR, PCHAR, PCHAR, PDLLLINK, PDLLLINK, PPDOPEN );
int   LoadWd( PCHAR, PCHAR, PCHAR, PDLLLINK, PDLLLINK, PPDOPEN, PUSHORT );
int   LoadVd( PCHAR, PCHAR, PCHAR, PDLLLINK, PDLLLINK *, USHORT, PUSHORT );
void  UnloadDrivers( PDLLLINK, PDLLLINK, PDLLLINK *, USHORT );

#endif //__CFGLOAD_H__
