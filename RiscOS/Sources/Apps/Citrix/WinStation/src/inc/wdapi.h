
/***************************************************************************
*
*  WDAPI.H
*
*  This module contains external winstation driver API defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  $Log$
*  
*     Rev 1.53   15 Apr 1997 18:46:08   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.52   04 Mar 1997 17:40:56   terryt
*  client shift states
*  
*     Rev 1.51   22 Jan 1997 16:18:20   terryt
*  client data
*  
*     Rev 1.50   13 Jan 1997 16:28:10   kurtp
*  Persistent Cache
*  
*     Rev 1.49   13 Nov 1996 09:21:44   richa
*  Updated Virtual channel code.
*  
*     Rev 1.48   04 Nov 1996 09:42:54   richa
*  added open virtual channel structures.
*  
*     Rev 1.47   08 May 1996 13:51:32   jeffm
*  update
*  
*     Rev 1.46   06 May 1996 19:17:36   jeffm
*  update
*  
*     Rev 1.45   27 Apr 1996 16:05:58   andys
*  soft keyboard
*
*     Rev 1.44   12 Feb 1996 09:38:54   richa
*  added a fAsync for async connections.
*
*     Rev 1.43   03 Feb 1996 23:03:30   bradp
*  xcopy fix
*
*     Rev 1.42   11 Dec 1995 10:57:10   richa
*  xxx changes.
*
*     Rev 1.41   05 Dec 1995 10:16:20   miked
*  update
*
*     Rev 1.40   25 Sep 1995 16:31:46   kurtp
*  update
*
*     Rev 1.39   25 Sep 1995 12:33:06   kurtp
*  update
*
*     Rev 1.38   01 Jun 1995 22:30:02   terryt
*  Make encrypted clients work with SouthBeach
*
*     Rev 1.37   23 May 1995 18:51:42   terryt
*  Encryption
*
*     Rev 1.36   17 May 1995 15:47:30   marcb
*  update
*
*     Rev 1.35   13 May 1995 15:24:22   marcb
*  update
*
*     Rev 1.34   10 May 1995 18:17:28   marcb
*  update
*
*     Rev 1.33   03 May 1995 12:52:02   marcb
*  update
*
*     Rev 1.32   02 May 1995 13:48:14   kurtp
*  update
*
*     Rev 1.31   02 May 1995 13:41:14   butchd
*  update
*
****************************************************************************/

#ifndef __WDAPI_H__
#define __WDAPI_H__

/*
 *  Index into WD procedure array
 */
//      DLL$LOAD                 0
//      DLL$UNLOAD               1
//      DLL$OPEN                 2
//      DLL$CLOSE                3
//      DLL$INFO                 4
//      DLL$POLL                 5
#define WD__QUERYINFORMATION      6
#define WD__SETINFORMATION        7
#define WD__COUNT                 8   /* number of external wd procedures */

/*
 *  WdOpen structure
 */
typedef struct _WDOPEN {
    PPLIBPROCEDURE pModuleProcedures;
    PPLIBPROCEDURE pClibProcedures;
    PPLIBPROCEDURE pVioProcedures;
    PPLIBPROCEDURE pKbdProcedures;
    PPLIBPROCEDURE pMouProcedures;
    PPLIBPROCEDURE pTimerProcedures;
    PPLIBPROCEDURE pLptProcedures;
    PPLIBPROCEDURE pXmsProcedures;
    PPLIBPROCEDURE pLogProcedures;
    PPLIBPROCEDURE pBIniProcedures;
    LPVOID pIniSection;         // in: pointer to ini file section buffer
    PDLLLINK pPdLink;           // in: top most protocol driver
    PDLLLINK pScriptLink;       // in: pointer to scripting dll
    PDLLLINK pDll;              // in: pointer to a list of all loaded dlls
    USHORT OutBufHeader;        // in: number of header bytes to reserve
    USHORT OutBufTrailer;       // in: number of trailer bytes to reserve
    USHORT OutBufParam;         // in: number of parameter bytes to reserve
    USHORT fOutBufCopy/* : 1 */;      // in: pd copies data into new outbuf
    USHORT fOutBufFrame/* : 1 */;     // in: framing protocol driver is loaded
    USHORT fAsync/* : 1 */;           // in: Async connection or not
    USHORT MaxVirtualChannels;  // out: maximum virtual channels supported
    PPLIBPROCEDURE pEmulProcedures;
} WDOPEN, * PWDOPEN;

/*
 *  WdConnect structure
 */
typedef struct _WDCONNECT {
    USHORT NotUsed;
} WDCONNECT, * PWDCONNECT;

/*
 *  WdInformationClass enum
 */
typedef enum _WDINFOCLASS {
    WdClientData,
    WdStatistics,
    WdLastError,
    WdConnect,
    WdDisconnect,
    WdKillFocus,
    WdSetFocus,
    WdEnablePassThrough,
    WdDisablePassThrough,
    WdVdAddress,
    WdVirtualWriteHook,
    WdAddReadHook,
    WdRemoveReadHook,
    WdAddWriteHook,
    WdRemoveWriteHook,
    WdModemStatus,
    WdLoadBalance,        // get - appserver to reconnect to for load balancing
    WdCharCode,
    WdScanCode,
    WdMouseInfo,
    WdInitWindow,
    WdDestroyWindow,
    WdPaint,              // Tell the client to paint
    WdRedraw,             // Tell the host to redraw
    WdThinwireStack,      // Pass the thinwire stack to the thinwire vd
    WdEncryptionInit,     // get - Encryption initialization parameters
    WdRealizePaletteFG,   // inform client to realize it's foreground palette
    WdRealizePaletteBG,   // inform client to realize it's background palette
    WdInactivate,         // client is about to lose input focus
    WdSetProductID,       // client is about to lose input focus
    WdSetDefaultMode,     // client is about to lose input focus
    WdRaiseSoftkey,       // raise the soft keyboard
    WdLowerSoftkey,       // lower the soft keyboard
    WdIOStatus,           // IO status
    WdOpenVirtualChannel, // get - Open a virtual channel
    WdCache,              // persistent caching command set
    WdGetInfoData,        // Information from host to client
    WdWindowSwitch       // window has switched back, check keyboard state
} WDINFOCLASS;

/*
 *  WdQueryInformation structure
 */
typedef struct _WDQUERYINFORMATION {
    WDINFOCLASS WdInformationClass;
    LPVOID pWdInformation;
    USHORT WdInformationLength;
    USHORT WdReturnLength;
} WDQUERYINFORMATION, * PWDQUERYINFORMATION;

/*
 *  WdSetInformation structure
 */
typedef struct _WDSETINFORMATION {
    WDINFOCLASS WdInformationClass;
    LPVOID pWdInformation;
    USHORT WdInformationLength;
} WDSETINFORMATION, * PWDSETINFORMATION;

/*
 *  WdLoadBalance structure
 */
typedef struct _LOADBALANCE {
    ADDRESS AppServer;
} LOADBALANCE, * PLOADBALANCE;

/*
 *  WdEncryptionInit structure
 */
typedef struct _EncryptionInit {
    UCHAR EncryptionLevel;
} ENCRYPTIONINIT, * PENCRYPTIONINIT;

/*
 *  WdGetInfoData structure
 */
#define INFODATA_ID_SIZE 8
typedef struct _INFODATA {
    UCHAR Id[INFODATA_ID_SIZE];
    /* PUCHAR pData; */
} INFODATA, * PINFODATA;

/*
 *  WdOpenVirtualChannel structure
 */
typedef struct _OPENVIRTUALCHANNEL {
    LPVOID  pVCName;
    USHORT  Channel;
} OPENVIRTUALCHANNEL, * POPENVIRTUALCHANNEL;

/*
 *  Virtual i/o channel ids
 *  NOTE: don't change the order of this structure
 *  These are here for compatibility reasons (taken from ica-c2h.h)
 */
typedef enum _VIRTUALCLASS_COMPATIBILITY {
    Virtual_Screen    = 0,
    Virtual_LPT1      = 1,                // also AUX1
    Virtual_LPT2      = 2,                // also AUX2
    Virtual_Reserved3 = 3,
    Virtual_Cpm       = 4,                // client print spooling
    Virtual_COM1      = 5,
    Virtual_COM2      = 6,
    Virtual_Reserved4 = 7,
    Virtual_Ccm       = 8,                // client COMM mapping
    Virtual_ThinWire  = 9,                // remote windows data
    Virtual_Cdm       = 10,               // client drive mapping
    Virtual_PassThru  = 11,               // passthrough for shadow
    Virtual_User1     = 12,               // user defined
    Virtual_User2     = 13,               // user defined
    Virtual_User3     = 14,               // user defined
    Virtual_Reserved1 = 15,               // reserved
    Virtual_Reserved2 = 16,               // reserved
    Virtual_Clipboard = 17,               // clipboard
    Virtual_Maximum   = 32                // TOTAL NUMBER OF VIRTUAL CHANNELS
} VIRTUALCLASS__COMPATIBILITY;

/*
 *  WdScanCode structure
 */
typedef struct _SCANCODE {
    USHORT  cScanCodes;
    LPVOID  pScanCodes;
} SCANCODE, * PSCANCODE;

/*
 *  WdCharCode structure
 */
typedef struct _CHARCODE {
    USHORT  cCharCodes;
    LPVOID  pCharCodes;
} CHARCODE, * PCHARCODE;

/*
 *  WdMouseInfo structure
 */
typedef struct _MOUSEINFO {
    USHORT  cMouseData;
    LPVOID  pMouseData;
} MOUSEINFO, * PMOUSEINFO;

/* #pragma pack(1) */
/*
 * WdRedraw structure
 */
typedef struct _WDRCL {         // SetFocusProcedure parameter (via PACKET_REDRAW)
   ULONG x/* :12 */, // X coordinate
         y/* :12 */; // Y coordinate
} WDRCL, *PWDRCL;

/*
 * WdRedraw structure
 */
typedef struct _WDREDRAW {
   USHORT  cb;     // Number of rectangles
   WDRCL   rcl[1]; // Compressed rectangles
} WDREDRAW, *PWDREDRAW;
/* #pragma pack() */

/*
 *  Set Info Class enum
 */
typedef enum _SETINFOCLASS {
    CallbackComplete
} SETINFOCLASS, * PSETINFOCLASS;

/*
 *  Query Info Class enum
 */
typedef enum _QUERYINFOCLASS {
    QueryEncryptionInit,
    OpenVirtualChannel
} QUERYINFOCLASS, * PQUERYINFOCLASS;

/*
 *  Outbuf Buffer data structure
 */
typedef struct _OUTBUF {

    /*
     *  Non-inherited fields
     */
    struct _OUTBUF * pLink;     // wd/td outbuf linked list
    LPVOID pParam;               // pointer to allocated parameter memory
    LPBYTE pMemory;              // pointer to allocated buffer memory
    LPBYTE pBuffer;              // pointer within buffer memory
    USHORT MaxByteCount;        // maximum buffer byte count (static)
    USHORT ByteCount;           // byte count pointed to by pBuffer

    LPBYTE pSaveBuffer;         // saved outbuf buffer pointer
    USHORT SaveByteCount;       // saved outbuf byte count

    /*
     *  Inherited fields (when pd allocates new outbuf and copies the data)
     */
    ULONG StartTime;            // pdreli - transmit time (used to measure roundtrip)
    USHORT fControl/* : 1 */;         // pdreli - control buffer (ack/nak)
    USHORT fRetransmit/* : 1 */;      // pdreli - outbuf retransmit
    USHORT fCompress/* : 1 */;        // pdcomp - compress data
    BYTE Sequence;              // pdreli - sequence number
    BYTE Fragment;              // pdreli - fragment number

} OUTBUF, * POUTBUF;


typedef int  (PWFCAPI POUTBUFALLOCPROC)( LPVOID, POUTBUF * );
typedef void (PWFCAPI POUTBUFFREEPROC)( LPVOID, POUTBUF );
typedef int  (PWFCAPI PPROCESSINPUTPROC)( LPVOID, LPBYTE, USHORT );
typedef int  (PWFCAPI POUTBUFRESERVEPROC)( LPVOID, USHORT );
typedef int  (PWFCAPI POUTBUFAPPENDPROC)( LPVOID, LPBYTE, USHORT );
typedef int  (PWFCAPI POUTBUFWRITEPROC)( LPVOID );
typedef int  (PWFCAPI PAPPENDVDHEADERPROC)( LPVOID, USHORT, USHORT );
typedef int  (PWFCAPI PAPPENDICAPKTPROC)( LPVOID, USHORT, LPBYTE, USHORT );
typedef void (PWFCAPI PIOHOOKPROC)( LPBYTE, USHORT );
typedef int  (PWFCAPI PSETINFOPROC)( LPVOID, SETINFOCLASS, LPBYTE, USHORT );
typedef int  (PWFCAPI PQUERYINFOPROC)( LPVOID, QUERYINFOCLASS, LPBYTE, USHORT );

/*=============================================================================
 ==   Virtual driver hook structures
 ============================================================================*/

/*
 *  Virtual driver write hook structure
 */
typedef void (PWFCAPI PVDWRITEPROCEDURE)( LPVOID, USHORT, LPBYTE, USHORT );

/*
 *  Used when registering virtual write hook  (WdVirtualWriteHook)
 */
typedef struct _VDWRITEHOOK {
    USHORT Type;                  // in: virtual channel id
    LPVOID pVdData;                // in: pointer to virtual driver data
    PVDWRITEPROCEDURE pProc;      // in: pointer to vd write procedure (wd->vd)
    LPVOID pWdData;                           // out: pointer to wd structure
    POUTBUFRESERVEPROC pOutBufReserveProc;   // out: pointer to OutBufReserve
    POUTBUFAPPENDPROC pOutBufAppendProc;     // out: pointer to OutBufAppend
    POUTBUFWRITEPROC pOutBufWriteProc;       // out: pointer to OutBufWrite
    PAPPENDVDHEADERPROC pAppendVdHeaderProc; // out: pointer to AppendVdHeader
    USHORT MaximumWriteSize;                 // out: maximum ica write size
} VDWRITEHOOK, * PVDWRITEHOOK;

/*
 *  Used as an internal winstation structure
 */
typedef struct _WDVDWRITEHOOK {
    LPVOID pData;                  // pointer to virtual driver data
    PVDWRITEPROCEDURE pProc;      // pointer to virtual driver write procedure
} WDVDWRITEHOOK, * PWDVDWRITEHOOK;

/*
 *  Used when registering virtual write hook  (WdVirtualWriteHook)
 */
typedef struct _VDWRITEHOOK20 {
    USHORT Type;                  // in: virtual channel id
    LPVOID pVdData;                // in: pointer to virtual driver data
    PVDWRITEPROCEDURE pProc;      // in: pointer to vd write procedure (wd->vd)
    LPVOID pWdData;                           // out: pointer to wd structure
    POUTBUFRESERVEPROC pOutBufReserveProc;   // out: pointer to OutBufReserve
    POUTBUFAPPENDPROC pOutBufAppendProc;     // out: pointer to OutBufAppend
    POUTBUFWRITEPROC pOutBufWriteProc;       // out: pointer to OutBufWrite
    PAPPENDVDHEADERPROC pAppendVdHeaderProc; // out: pointer to AppendVdHeader
    PAPPENDICAPKTPROC pAppendICAPktProc;     // out: pointer to AppendICAPacket
    USHORT MaximumWriteSize;                 // out: maximum ica write size
} VDWRITEHOOK20, * PVDWRITEHOOK20;



#endif // __WDAPI_H__
