
/***************************************************************************
*
*  WD.H
*
*  This module contains internal WinStation Driver (WD) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  $Log$
*  
*     Rev 1.37   20 Apr 1998 17:54:28   terryt
*  dos memory fix for reducer
*  
*     Rev 1.36   15 Apr 1998 19:14:52   kurtp
*  UK fix for DOS/Win16
*  
*     Rev 1.32   15 Apr 1997 18:17:20   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.31   20 Feb 1997 14:07:46   butchd
*  update
*  
*     Rev 1.30   22 Jan 1997 16:45:54   terryt
*  client data
*  
*     Rev 1.29   08 May 1996 15:02:18   jeffm
*  update
*  
*     Rev 1.28   12 Feb 1996 09:36:40   richa
*  added a fAsync for async connections.
*
*     Rev 1.27   04 Nov 1995 15:26:44   andys
*  beep status
*
*     Rev 1.26   21 Jul 1995 18:28:12   bradp
*  update
*
*     Rev 1.25   29 Jun 1995 13:57:16   bradp
*  update
*
*     Rev 1.24   23 May 1995 18:58:04   terryt
*  Encryption
*
*     Rev 1.23   10 May 1995 18:18:16   marcb
*  update
*
*     Rev 1.22   07 Apr 1995 15:40:32   kurtp
*  update
*
*     Rev 1.21   29 Nov 1994 16:26:06   kurtp
*  update
*
*     Rev 1.20   22 Oct 1994 18:05:18   bradp
*  update
*
*
****************************************************************************/

#ifndef __WD_H__
#define __WD_H__

/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _WD * PWD;

/*=============================================================================
==   structures
=============================================================================*/

typedef struct _INFOBLOCK * PINFOBLOCK;

typedef struct _INFOBLOCK {
    PINFOBLOCK pNext;          // Next block in list (or NULL)
    BYTE       Id[8];          // ID of data block
    USHORT     Length;         // Length in bytes
    LPVOID     pData;          // Data (or NULL)
} INFOBLOCK;

/*
 *  Expansion/reduction structure
 */
typedef struct _DATA_QUEUE FAR *PDATA_QUEUE;

typedef struct _DATA_QUEUE {
    ULONG       MaxNewData;         /* maximum amount of pending new data */
        LONG                DistanceAdjustment;        /* bias for distance encoding */
        ULONG                MinimumDistanceBits;/* miniumum bits for distance encoding */
    PUCHAR      Buffer;                                /* main buffer - other data may follow at end */
    ULONG       BufferLen;          /* main buffer length */
    ULONG       BufferMask;         /* main buffer length - 1 */
    ULONG       WriteBase;          /* tail index of new data */
    ULONG       WriteReached;       /* head index of new data */
    ULONG       WriteLimit;         /* limit point for new data = WriteBase + MaxNewData */
} DATA_QUEUE;

/*
 *  WD structure
 */
typedef struct _WD {

    PDLLLINK pPdLink;             // top most protocol driver
    PDLLLINK pScriptLink;
    USHORT InputBufferLength;
    USHORT OutBufHeader;
    USHORT OutBufTrailer;
    USHORT OutBufParam;
    ULONG XmsReserve;
    ULONG LowMemReserve;

    USHORT OutBufLength;            // length of input/output buffers
    USHORT OutBufCountHost;         // number of host outbufs
    USHORT OutBufCountClient;       // number of client outbufs

    USHORT OutBufMaxByteCount;      // maximum amount of data in outbuf
    USHORT OutBufFreeCount;         // number of outbufs on free list
    POUTBUF pOutBufFree;            // pointer to outbuf free list
    POUTBUF pOutBufCurrent;         // pointer to current outbuf
    LPBYTE pInputBuffer;             // pointer to input buffer
    USHORT InputCount;              // number of bytes in buffer
    ULONG YieldTime;                // time in msecs at which to yield cpu
    LPBYTE pAppServer;               // appserver for load-balancing

    USHORT fFocus/*: 1*/;               // winstation driver has focus
    USHORT fTTYConnected/*: 1*/;        // client is connected to tty host
    USHORT fConnected/*: 1*/;           // client is connected to ica host
    USHORT fOutBufCopy/*: 1*/;          // protocol drivers copy outbuf
    USHORT fOutBufFrame/*: 1*/;         // framing protocol driver is loaded
    USHORT fReceivedStopOk/*: 1*/;      // PACKET_STOP_OK received
    USHORT fRecvStatusConnect/*: 1*/;   // recv CLIENT_STATUS_CONNECT from pd
    USHORT fSendStatusConnect/*: 1*/;   // send CLIENT_STATUS_CONNECT to engine
    USHORT fSendStatusBeeped/*: 1*/;    // send CLIENT_STATUS_BEEPED to engine
    USHORT fAsync/*: 1*/;               // This is an asynce connection

    ULONG  TimeoutStopRequest;      // PACKET_STOP_REQUEST timeout time

    PVOID pPrivate;
    BYTE  HostVersionL;             // Lowest version supported by both host and client
    BYTE  HostVersionH;             // Highest version supported by both host and client
    BYTE  EncryptionLevel;          // Encryption level from Init Request

    PINFOBLOCK pInfoBlockList;      // List of data blocks from the host

    /*  Extra data structures for input expansion/reduction */
    DATA_QUEUE expansionData;       // defines the expansion of incoming data
    BOOL expansionEnabled;
    DATA_QUEUE reductionData;       // defines the reduction of outgoing data
    BOOL reductionEnabled;
    BYTE C2H_PowerOf2Wanted;        // desired, not agreed on
    BYTE H2C_PowerOf2Wanted;        // desired, not agreed on
    USHORT H2C_MaxNewData;          // desired, not agreed on

    LPBYTE ExpanderBuffer;          // memory allocated early on
    LPBYTE ReducerBuffer;           // memory allocated early on

    PPLIBPROCEDURE pEmulProcedures;
} WD;

#endif //__WD_H__
