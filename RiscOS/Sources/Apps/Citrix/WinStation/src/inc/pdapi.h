
/***************************************************************************
*
*  PDAPI.H
*
*  This module contains external protocol driver API defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  pdapi.h,v
*  Revision 1.1  1998/01/12 11:36:55  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.32   15 Apr 1997 18:45:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.31   06 May 1996 19:16:04   jeffm
*  update
*  
*     Rev 1.30   27 Sep 1995 13:41:34   bradp
*  update
*  
*     Rev 1.29   01 Jun 1995 22:29:54   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.28   08 May 1995 16:41:56   butchd
*  update
*  
*     Rev 1.27   02 May 1995 13:40:26   butchd
*  update
*
****************************************************************************/
#ifndef __PDAPI_H__
#define __PDAPI_H__

/*
 *  Index into PD procedure array
 */
//      DLL$LOAD                 0
//      DLL$UNLOAD               1
//      DLL$OPEN                 2
//      DLL$CLOSE                3
//      DLL$INFO                 4
//      DLL$POLL                 5
#define PD__WRITE              	 6
#define PD__QUERYINFORMATION   	 7
#define PD__SETINFORMATION     	 8
#define PD__COUNT              	 9   /* number of external pd procedures */

/*
 *  PdOpen structure
 */
typedef struct _PDOPEN {
    PPLIBPROCEDURE   pModuleProcedures;
    PPLIBPROCEDURE   pClibProcedures;
    PPLIBPROCEDURE   pLogProcedures;
    PPLIBPROCEDURE   pBIniProcedures;
    LPVOID           pIniSection;
    LPBYTE           pExePath;
    USHORT OutBufHeader;        // out: number of header bytes to reserve
    USHORT OutBufTrailer;       // out: number of trailer bytes to reserve
    USHORT OutBufParam;         // out: number of parameter bytes to reserve
    BUSHORT fOutBufCopy : 1;      // out: pd copies data into new outbuf
    BUSHORT fOutBufFrame : 1;     // out: framing protocol driver (2x outbufs)
    PPLIBPROCEDURE   pDeviceProcedures;
} PDOPEN, * PPDOPEN;

/*
 *  PdWrite structure
 */
typedef struct _PDWRITE {
    POUTBUF pOutBuf;
} PDWRITE, * PPDWRITE;

/*
 *  PdInformationClass enum
 */
typedef enum _PDINFOCLASS {
    PdNop,
    PdLastError,
    PdWdAddress,
    PdConnect,
    PdDisconnect,
    PdCancelWrite,
    PdKillFocus,
    PdSetFocus,
    PdSendBreak,
    PdEnablePassThrough,
    PdDisablePassThrough,
    PdCallback,
    PdAddReadHook,
    PdRemoveReadHook,
    PdAddWriteHook,
    PdRemoveWriteHook,
    PdModemStatus,
    PdBufferInfo,
    PdInitModule,
    PdDisableModule,
    PdIcaDetected,
    PdOutBufReserve,
    PdEnableModule,
    PdTimeoutStatus,
    PdIOStatus
} PDINFOCLASS;

/*
 *  PdQueryInformation structure
 */
typedef struct _PDQUERYINFORMATION {
    PDINFOCLASS PdInformationClass;
    LPVOID pPdInformation;
    USHORT PdInformationLength;
    USHORT PdReturnLength;
} PDQUERYINFORMATION, * PPDQUERYINFORMATION;

/*
 *  PdSetInformation structure
 */
typedef struct _PDSETINFORMATION {
    PDINFOCLASS PdInformationClass;
    LPVOID pPdInformation;
    USHORT PdInformationLength;
} PDSETINFORMATION, * PPDSETINFORMATION;

/*
 *  PdSetInformation - PdWdAddress
 */
typedef struct _WDADDRESS {
    LPVOID pWdData;
    POUTBUFALLOCPROC  pOutBufAllocProc;     // pointer to OutBufAlloc
    POUTBUFFREEPROC   pOutBufErrorProc;     // pointer to OutBufError
    POUTBUFFREEPROC   pOutBufFreeProc;      // pointer to OutBufFree
    PPROCESSINPUTPROC pProcessInputProc;    // pointer to EmulProcessInput
    PSETINFOPROC      pSetInfoProc;         // pointer to EmulSetInfo
    PQUERYINFOPROC    pQueryInfoProc;       // pointer to EmulQueryInfo - SJM: this was PSETINFOPROC
} WDADDRESS, * PWDADDRESS;

/*
 *  PdQueryInformation - PdModemStatus
 */
typedef struct _MODEMSTATUS {
    USHORT  TimeLeft;
    USHORT  RetriesLeft;
} MODEMSTATUS, * PMODEMSTATUS;

/*
 *  PdQueryInformation - PdBufferInfo
 */
typedef struct _BUFFERINFO {
    USHORT OutBufLength;              // length of input/output buffers
    USHORT OutBufCountHost;           // number of host outbufs
    USHORT OutBufCountClient;         // number of client outbufs
} BUFFERINFO, * PBUFFERINFO;

/*
 *  PdQueryInformation - PdLastError
 */
typedef struct _PDLASTERROR {
    char ProtocolName[10];
    int Error;
    char Message[ MAX_ERRORMESSAGE ];
} PDLASTERROR, * PPDLASTERROR;

/*
 *  PdQueryInformation - PdTimeoutStatus
 */
typedef struct _TIMEOUTSTATUS {
    ULONG MaxRetryTime;         // maximum retry time  (msec)
    ULONG SendRetryTime;        // total send retry time (msec)
    ULONG ReceiveRetryTime;     // total receive retry time (msec)
} TIMEOUTSTATUS, * PTIMEOUTSTATUS;

/*
 *  PdQueryInformation - PdIOStatus
 */
typedef struct _IOSTATUS {
    ULONG IOStatusSize;         // length of structure
    ULONG BytesRecv;            // total bytes received
    ULONG BytesSent;            // total bytes sent
    ULONG FramesRecv;           // total frames received
    ULONG FramesSent;           // total frames sent
    ULONG FrameRecvError;       // frame receive errors
    ULONG FrameSendError;       // frame send errors
} IOSTATUS, * PIOSTATUS;

#endif //__PDAPI_H__
