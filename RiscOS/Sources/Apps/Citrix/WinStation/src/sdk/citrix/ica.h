/******************************************************************************
*
*  ICA.H
*     This file contains definitions for the ICA 3.0 structures
*
*     NOTE: this file is shared between the client and the server
*
*  Copyright Citrix Systems Inc. 1997
*
*  Author: Brad Pedersen
*
*  ica.h,v
*  Revision 1.1  1998/01/12 11:37:55  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.2   25 Aug 1997 12:00:20   BillG
*  update
*  
*     Rev 1.1   20 Jun 1997 20:30:04   kurtp
*  update
*  
*     Rev 1.8   16 Jun 1997 21:52:16   kurtp
*  update
*  
*     Rev 1.7   12 Jun 1997 18:26:30   kurtp
*  make ica.h shared
*  
*     Rev 1.6   11 Jun 1997 07:40:36   butchd
*  backed out last change
*  
*     Rev 1.5   10 Jun 1997 15:26:20   butchd
*  define SV_TYPE_APPSERVER for DOS and WIN16 client usage
*  
*     Rev 1.4   05 Jun 1997 19:35:20   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.3   23 May 1997 14:14:36   butchd
*  SV_TYPE_APPSERVER part of lmserver.h now
*  
*     Rev 1.2   May 06 1997 18:07:24   billm
*  added virtual_oem2
*  
*     Rev 1.1   21 Apr 1997 16:56:44   TOMA
*  update
*  
*     Rev 1.0   21 Mar 1997 15:43:06   bradp
*  Initial revision.
*  
*
*******************************************************************************/

#ifndef __ICA_H__
#define __ICA_H__

/* #pragma pack(1) */


/*=============================================================================
==   Registered Netware Bindery ID  (SAP ID)
=============================================================================*/

/*
 *  SAP ID - Citrix Application Server for NT
 */
#define CITRIX_APPLICATION_SERVER       0x083d
#define CITRIX_APPLICATION_SERVER_SWAP  0x3d08  // byte swapped

// old winview sap id
//#define CITRIX_APPLICATION_SERVER       0x052d
//#define CITRIX_APPLICATION_SERVER_SWAP  0x2d05  // byte swapped


/*=============================================================================
==   Citrix AppServer mask (ms networks NetServerEnum type field)
=============================================================================*/

#ifdef notdef   // part of lmserver.h now
#define SV_TYPE_APPSERVER   0x10000000
#endif


/*=============================================================================
==   Client Modules
=============================================================================*/

/*
 *  modules classes
 */
typedef enum _MODULECLASS {
    Module_UserInterface,
    Module_UserInterfaceExt,
    Module_WinstationDriver,
    Module_VirtualDriver,
    Module_ProtocolDriver,
    Module_TransportDriver,
    Module_NameResolver,
    Module_NameEnumerator,
    Module_Scripting
} MODULECLASS;


/*=============================================================================
==   Protocol Drivers - Common data structures
=============================================================================*/

/*
 *  protocol driver classes
 *  NOTE: don't change the order of this structure
 */
typedef enum _PDCLASS {
    PdNone,            // 0 
    PdConsole,         // 1  no dll
    PdNetwork,         // 2  tdnetb.dll, tdspx.dll, tdftp.dll tdipx.dll
    PdAsync,           // 3  tdasync.dll
    PdOemTransport,    // 4  user transport driver
    PdISDN,            // 5  not implemented
    PdX25,             // 6  not implemented
    PdModem,           // 7  pdmodem.dll
    PdOemConnect,      // 8  user protocol driver
    PdFrame,           // 9  pdframe.dll
    PdReliable,        // 10 pdreli.dll
    PdEncrypt,         // 11 pdcrypt1.dll
    PdCompress,        // 12 pdcomp.dll
    PdTelnet,          // 13 not implemented
    PdOemFilter,       // 14 user protocol driver
    PdNasi,            // 15 tdnasi.dll
    PdClass_Maximum    // 16 -- must be last
} PDCLASS;


/*=============================================================================
==   Winstation Drivers - Common data structures
=============================================================================*/

/*
 *  Valid row/column combinations
 */
typedef struct _WDTEXTMODE {
    BYTE Index;            // this value is sent by PACKET_SET_VIDEOMODE
    BYTE Flags;            // Used by the client
    USHORT Columns;
    USHORT Rows;
    USHORT ResolutionX;
    USHORT ResolutionY;
    BYTE FontSizeX;
    BYTE FontSizeY;
} WDTEXTMODE, * PWDTEXTMODE;

/*
 * Values for WDTEXTMODE Flags field. This are used by the client for non-standard video adapters
 */
#define VIDEO_ALTERNATE     1

/*
 * Values for WDTEXTMODE Index field
 * These must be common across all WDs in order to support shadow
 * and WinStation reconnect to a different WinStation (WD) type
 */
/*
 * The following are for valid VGA/SVGA text modes.
 * These MUST match the values defined by the client.
 */
#define TEXTINDEX_40x25     0
#define TEXTINDEX_80x25     1
#define TEXTINDEX_80x43     2
#define TEXTINDEX_80x50     3
#define TEXTINDEX_80x60     4
#define TEXTINDEX_132x25    5
#define TEXTINDEX_132x43    6
#define TEXTINDEX_132x50    7
#define TEXTINDEX_132x60    8

/*
 * The following are additional modes supported by Relisys, VT420, and others.
 */
#define TEXTINDEX_80x24     9
#define TEXTINDEX_80x36     10
#define TEXTINDEX_80x48     11
#define TEXTINDEX_80x72     12
#define TEXTINDEX_80x144    13
#define TEXTINDEX_132x24    14
#define TEXTINDEX_132x36    15
#define TEXTINDEX_132x48    16
#define TEXTINDEX_132x72    17
#define TEXTINDEX_132x144   18

/*=============================================================================
==   Virtual Drivers - Common data structures
=============================================================================*/

/*
 *  Virtual Channel Name
 */
#define VIRTUALNAME_LENGTH  7

typedef CHAR VIRTUALNAME[ VIRTUALNAME_LENGTH + 1 ];  // includes null
typedef CHAR * PVIRTUALNAME;

typedef LONG VIRTUALCLASS;
typedef LONG * PVIRTUALCLASS;

/*
 *  Virtual i/o channel names  (VIRTUALNAME)
 *
 *  name syntax:  xxxyyyy<null>
 *
 *    xxx    - oem id (CTX - Citrix Systems)
 *    yyyy   - virtual channel name
 *    <null> - trailing null
 */

#define VIRTUAL_LPT1      "CTXLPT1"   // old client printer mapping
#define VIRTUAL_LPT2      "CTXLPT2"   // old client printer mapping
#define VIRTUAL_COM1      "CTXCOM1"   // old client printer mapping
#define VIRTUAL_COM2      "CTXCOM2"   // old client printer mapping
#define VIRTUAL_CPM       "CTXCPM "   // new client printer mapping (spooling)
#define VIRTUAL_CCM       "CTXCCM "   // client com mapping
#define VIRTUAL_CDM       "CTXCDM "   // client drive mapping
#define VIRTUAL_CLIPBOARD "CTXCLIP"   // clipboard
#define VIRTUAL_THINWIRE  "CTXTW  "   // remote windows data
#define VIRTUAL_CAM       "CTXCAM "   // client audio mapping
#define VIRTUAL_OEM       "OEMOEM "   // used by oems
#define VIRTUAL_OEM2      "OEMOEM2"   // used by oems

#define VIRTUAL_MAXIMUM   32    // number of virtual channels

/*
 *  Types of virtual i/o flow control
 */
typedef enum _VIRTUALFLOWCLASS {
    VirtualFlow_None,
    VirtualFlow_Ack,
    VirtualFlow_Delay,
    VirtualFlow_Cdm
} VIRTUALFLOWCLASS;

/*
 *  Virtual driver flow control - ack
 */
typedef struct _VDFLOWACK {
    USHORT MaxWindowSize;           // maximum # of bytes we can write
    USHORT WindowSize;              // # of bytes we can write before blocking
} VDFLOWACK, * PVDFLOWACK;

/*
 *  Virtual driver flow control - delay
 */
typedef struct _VDFLOWDELAY {
    ULONG DelayTime;                // delay time in 1/1000 seconds
} VDFLOWDELAY, * PVDFLOWDELAY;

/*
 *  Virtual driver flow control - cdm (client drive mapping)
 */
typedef struct _VDFLOWCDM {
    USHORT MaxWindowSize;           // total # of bytes we can write
    USHORT MaxByteCount;            // maximum size of any one write
} VDFLOWCDM, * PVDFLOWCDM;

/*
 *  Virtual driver flow control structure
 */
typedef struct _VDFLOW {
    BYTE BandwidthQuota;            // percentage of total bandwidth (0-100)
    BYTE Flow;                      // type of flow control
    BYTE Pad1[2];
    union {
        VDFLOWACK Ack;
        VDFLOWDELAY Delay;
        VDFLOWCDM Cdm;
    } unnamed;			/* SJM: added a name here in case size matters */
} VDFLOW, * PVDFLOW;

/*
 *  Structure used to bind virtual channel name to number
 */
typedef struct _WD_VCBIND {
    VIRTUALNAME VirtualName;
    USHORT VirtualClass;
} WD_VCBIND, * PWD_VCBIND;


/*=============================================================================
==   Virtual Driver Header - client drive mapping
=============================================================================*/

typedef struct _VDCLIENTDRIVES {
    BYTE DriveLetter;
    BYTE Pad;
    USHORT Flags;
} VDCLIENTDRIVES, * PVDCLIENTDRIVES;

/*
 *  Client drive mapping flags (Flags)
 */
#define CDM_REMOVEABLE      0x0001
#define CDM_FIXED           0x0002
#define CDM_REMOTE          0x0004
#define CDM_CDROM           0x0008
#define CDM_RAMDISK         0x0010
#define CDM_AUTOCONNECT     0x0020

typedef struct _VDCLIENTDRIVES2 {
    BYTE DriveLetter;
    BYTE NetworkType;
    USHORT oFileServer;
    USHORT oShareName;
    USHORT oCurrentDirectory;
} VDCLIENTDRIVES2, * PVDCLIENTDRIVES2;

/*
 *  Network type (NetworkType)
 */
#define CDM_NONE            0
#define CDM_LANMAN          1
#define CDM_NETWARE_3x      2
#define CDM_NETWARE_4x      3


/*=============================================================================
==   Client Data - Common data structures
=============================================================================*/

/*
 *  Client product id
 */
#define CLIENTID_CITRIX_DOS          0x0001     // citrix dos client
// avoid 2 (andy)
#define CLIENTID_CITRIX_CONSOLE      0x0003     // citrix console
#define CLIENTID_CITRIX_TEXT_TERM    0x0004     // citrix text terminals
#define CLIENTID_CITRIX_TEXT_TERM    0x0004     // citrix text terminals

#define CLIENTID_CITRIX_INTERNET     0x0101     // citrix internet client

#define CLIENTID_CITRIX_JAVA         0x0105     // citrix java client

#define CLIENTID_CITRIX_RISCOS       0x8086     // citrix RISC OS client

#define CLIENTID_FORCESERIALIZE_FLAG 0x4000     // client requires license no.
#define CLIENTID_TERMINAL_FLAG       0x8000     // terminal based client

/* #pragma pack() */

#endif //__ICA_H__
