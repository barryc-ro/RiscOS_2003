
/***************************************************************************
*
*  PD.H
*
*  This module contains internal Protocol Driver (PD) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  pd.h,v
*  Revision 1.1  1998/01/12 11:35:36  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.12   15 Apr 1997 16:51:40   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.11   08 May 1996 16:40:48   jeffm
*  update
*  
*     Rev 1.10   28 Jun 1995 22:13:48   bradp
*  update
*  
*     Rev 1.9   10 Jun 1995 14:25:24   bradp
*  update
*  
*
****************************************************************************/

#ifndef __PD_H__
#define __PD_H__


/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _PD * PPD;



/*=============================================================================
==   structures
=============================================================================*/

/*
 *  PD structure
 */
typedef struct _PD {

    PDCLASS PdClass;

    USHORT OutBufHeader;                // number of header bytes to reserve
    USHORT OutBufTrailer;               // number of trailer bytes to reserve

    USHORT OutBufLength;                // length of input/output buffers
    USHORT OutBufCountHost;             // number of host outbufs
    USHORT OutBufCountClient;           // number of client outbufs

    PVOID pWdData;                      // pointer to stacked pd/wd
    POUTBUFALLOCPROC  pOutBufAllocProc; // pointer to OutBufAlloc
    POUTBUFFREEPROC   pOutBufErrorProc; // pointer to OutBufError
    POUTBUFFREEPROC   pOutBufFreeProc;  // pointer to OutBufFree
    PPROCESSINPUTPROC pProcessInputProc;// pointer to EmulProcessInput
    PSETINFOPROC      pSetInfoProc;     // pointer to EmulSetInfo
    PQUERYINFOPROC    pQueryInfoProc;   // pointer to EmulQueryInfo

    POUTBUF pOutBufCurrent;             // pointer to current outbuf

    int    fOpen: 1;                    // open flag
    int    fInitModule: 1;              // host has sent module an init packet
    int    fEnableModule: 1;            // enable functionality of this module
    int    fIcaDetected: 1;             // ica host detected
    int    fRecvStatusConnect: 1;       // recv CLIENT_STATUS_CONNECT from pd
    int    fSendStatusConnect: 1;       // send CLIENT_STATUS_CONNECT to next pd
    int    fConnected: 1;               // connected with host

    USHORT CloseCode;                   // set by entity that clears fOpen flag
    PVOID  pPrivate;                    // pointer to private data structure

    PPLIBPROCEDURE pDeviceProcedures;	// Pointer to device function structure
    DLLLINK Link;
} PD;

#define OutBufAlloc	PdOutBufAlloc
#define OutBufError	PdOutBufError
#define OutBufFree	PdOutBufFree
#define OutBufAppend	PdOutBufAppend
#define OutBufWrite	PdOutBufWrite

#endif //__PD_H__
