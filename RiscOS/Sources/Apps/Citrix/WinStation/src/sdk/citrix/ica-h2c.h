/******************************************************************************
*
*  ICA-H2C.H   host -> client
*
*     This file contains structures that are sent by the host to
*     the client at connection time.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen (8/27/94)
*
*  $Log$
*  
*     Rev 1.12   16 Jun 1997 21:57:24   kurtp
*  update
*  
*     Rev 1.11   21 Apr 1997 16:56:58   TOMA
*  update
*  
*     Rev 1.10   29 Aug 1996 17:46:34   bradp
*  update
*  
*
*******************************************************************************/

#ifndef __ICAH2C_H__
#define __ICAH2C_H__

//#pragma pack(1)


/*=============================================================================
==   Common Module Header
=============================================================================*/

/*
 *  Common module header
 */
typedef struct _MODULE_H2C {
    USHORT ByteCount;               // length of module data in bytes (<2k)
    BYTE ModuleCount;               // number of modules headers left to be sent
    BYTE ModuleClass;               // module class (MODULECLASS)
    BYTE VersionL;                  // lowest supported version
    BYTE VersionH;                  // highest supported version
    BYTE Version;                   // connect version level
    BYTE Pad1;
} MODULE_H2C, * PMODULE_H2C;


/*=============================================================================
==   Winstation Driver Header
=============================================================================*/

/*
 *  1 - SOUTHBEACH 1.29
 *  2 - WinFrame 153 (press release/beta)
 *  3 - WinFrame 154 
 *  4 - WinFrame 158 (ship level)
 *  5 - WinFrame 2.0
 */
#define VERSION_HOSTL_WD   1
#define VERSION_HOSTH_WD   5


/*
 *  ica 3.0 (wdica.dll)
 */
typedef struct _WD_H2C {
    MODULE_H2C Header;
    USHORT ICABufferLength;     // maximum size of one ica packet
    USHORT OutBufCountHost;     // number of outbufs on host
    USHORT OutBufCountClient;   // number of outbufs on client
    USHORT OutBufDelayHost;     // host outbuf write delay (1/1000 seconds)
    USHORT OutBufDelayClient;   // client outbuf write delay (1/1000 seconds)
    USHORT oAppServerName;      // reconnect to this appserver (load-balancing)
    char   OEMId[4];            // Server OEM Id, ASCIIZ string

    /* version 5 */
    USHORT WdFlag;              // ica winstation driver flags
} WD_H2C, * PWD_H2C;

/*
 * WdFlag 
 */
#define WDFLAG_SECURED 0x0001   // server has been secured


/*=============================================================================
==   Protocol Driver Header
=============================================================================*/

#define VERSION_HOSTL_PDFRAME     1
#define VERSION_HOSTH_PDFRAME     1

#define VERSION_HOSTL_PDRFRAME    1
#define VERSION_HOSTH_PDRFRAME    1

#define VERSION_HOSTL_PDMODEM     1
#define VERSION_HOSTH_PDMODEM     1

#define VERSION_HOSTL_PDRELIABLE  1
#define VERSION_HOSTH_PDRELIABLE  1

#define VERSION_HOSTL_PDCOMPRESS  1
#define VERSION_HOSTH_PDCOMPRESS  1

#define VERSION_HOSTL_PDCRYPT1    1
#define VERSION_HOSTH_PDCRYPT1    1

/*
 *  common protocol driver header
 */
typedef struct _PD_H2C {
    MODULE_H2C Header;
    BYTE PdClass;                 // class of protocol driver (PDCLASS)
    BYTE Pad1[3];
} PD_H2C, * PPD_H2C;

/*
 *  reliable protocol driver (pdreli.dll)
 */
typedef struct _PDRELIABLE_H2C {
    PD_H2C Header;
    ULONG MaxRetryTime;         // maximum time to attempt retries (msec)
    ULONG RetransmitTimeDelta;  // increment timeout by this value (msec)
    ULONG DelayedAckTime;       // delayed ack timeout (msec)
} PDRELIABLE_H2C, * PPDRELIABLE_H2C;


/*=============================================================================
==   Transport Driver Header
=============================================================================*/

/*
 *  transport drivers (tdnetb.dll, tdasync.dll, tdipx.dll, .... )
 */
typedef struct _TD_H2C {
    MODULE_H2C Header;
} TD_H2C, * PTD_H2C;


//#pragma pack()

#endif //__ICAH2C_H__
