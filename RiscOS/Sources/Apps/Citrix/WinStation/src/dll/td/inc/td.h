
/***************************************************************************
*
*  TD.H
*
*  This module contains internal Transport Driver (TD) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  $Log$
*  
*     Rev 1.15   15 Apr 1997 16:54:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   08 Feb 1997 13:56:16   jeffm
*  multiple browser support added
*  
*     Rev 1.13   08 May 1996 16:21:26   richa
*  update
*  
*     Rev 1.12   03 Feb 1996 23:02:36   bradp
*  update
*  
*     Rev 1.11   03 Feb 1996 20:16:44   bradp
*  update
*  
*     Rev 1.10   20 Dec 1995 13:45:46   bradp
*  update
*  
*     Rev 1.9   05 Dec 1995 18:46:28   bradp
*  update
*  
*
****************************************************************************/

#ifndef __TD_H__
#define __TD_H__

/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _PD * PPD;


/*=============================================================================
==   structures
=============================================================================*/

 /*
  *  PD error message structure
  */
typedef struct _PDERRORMESSAGE {
    int Error;
    char * pMessage;
} PDERRORMESSAGE, * PPDERRORMESSAGE;

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
    POUTBUFALLOCPROC pOutBufAllocProc;  // pointer to OutBufAlloc
    POUTBUFFREEPROC pOutBufErrorProc;   // pointer to OutBufError
    POUTBUFFREEPROC pOutBufFreeProc;    // pointer to OutBufFree
    PPROCESSINPUTPROC pProcessInputProc;// pointer to EmulProcessInput
    PIOHOOKPROC pReadHook;              // pointer to read hook procedure
    PIOHOOKPROC pWriteHook;             // pointer to write hook procedure

    POUTBUF pOutBufHead;                // head of output queue
    POUTBUF pOutBufTail;                // tail of output queue
    POUTBUF pOutBufCurrent;             // pointer to current outbuf

    int LastError;                      // transport level error code
    char * ErrorMessage;                // transport error message
    char ErrorProtocolName[10];         // Protocol error source

    int fOpen/* : 1 */;                       // open flag
    int fSentConnect/* : 1 */;                // returned STATUS_CONNECTED on poll
    int fSentTTYConnect/* : 1 */;             // returned STATUS_TTY_CONNECTED on poll
    int fReceiveActive/* : 1 */;              // receive is currently active
    int fSendStart/* : 1 */;                  // returned STATUS_SEND_START on poll
    int fSendStop/* : 1 */;                   // returned STATUS_SEND_STOP on poll

    LPBYTE pNrDll;                      // pointer to name resolver dll
    BYTE TcpBrowserAddress[ ADDRESS_LENGTH+1 ];  // addr of tcp ica browser
    BYTE IpxBrowserAddress[ ADDRESS_LENGTH+1 ];  // addr of ipx ica browser
    BYTE NetBiosBrowserAddress[ ADDRESS_LENGTH+1 ]; // addr of netbios ica browser
    USHORT BrowserRetry;                // browser i/o retry
    USHORT BrowserTimeout;              // browser i/o timeout

    PVOID pPrivate;                     // pointer to private data structure

    ULONG BytesRecv;                    // total bytes received
    ULONG BytesSent;                    // total bytes sent
    ULONG FramesRecv;                   // total frames received
    ULONG FramesSent;                   // total frames sent
    ULONG FrameRecvError;               // frame receive errors
    ULONG FrameSendError;               // frame send errors

    LPBYTE pTcpBrowserAddrList;         // addr list of tcp ica browser
    LPBYTE pIpxBrowserAddrList;         // addr list of ipx ica browser
    LPBYTE pNetBiosBrowserAddrList;     // addr list of netbios ica browser

    PPLIBPROCEDURE pDeviceProcedures;	// Pointer to device function structure
    
} PD;

/*---------------------------------------------------------------------------------------------------- */

#endif //__TD_H__
