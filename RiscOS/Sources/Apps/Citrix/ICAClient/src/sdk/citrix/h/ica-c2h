/******************************************************************************
*
*  ICA-C2H.H   client -> host
*
*     This file contains structures that are sent by the client to
*     the host at connection time.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen (8/27/94)
*
*  $Log$
*  
*     Rev 1.73   20 May 1998 17:20:00   terryt
*  Change license checking for old clients
*  
*     Rev 1.72   May 03 1998 16:23:36   briang
*  removed the IFDEF UNICODES
*  
*     Rev 1.71   22 Apr 1998 18:51:50   thanhl
*  UNICODE protocol & Japanese support
*  
*     Rev 1.70   14 Apr 1998 21:11:36   derekc
*  added version 4 information to UI_C2H
*  
*     Rev 1.69   09 Apr 1998 20:04:34   kurtp
*  Add versioning to reducer code
*  
*     Rev 1.67   01 Apr 1998 14:01:18   miked
*  UK Reducer fix
*  
*     Rev 1.66   Mar 30 1998 16:03:30   grega
*  Merged in UK Reducer
*  
*     Rev 1.65   30 Jan 1998 18:17:28   terryt
*  make twi compatible
*  
*     Rev 1.64   Jan 28 1998 15:57:48   sumitd
*  Seemless mode flag added in the Data transmitted to server
*  
*     Rev 1.63   28 Jan 1998 15:43:40   kalyanv
*  updated
*  
*     Rev 1.61   26 Jan 1998 11:15:26   KenB
*  add CLIENTIDs
*  
*     Rev 1.60   Jan 06 1998 14:09:54   bills
*  Added version numbers for the new pdtapi.
*  
*     Rev 1.59   15 Jul 1997 15:46:02   bradp
*  update
*  
*     Rev 1.58   12 Jun 1997 18:27:04   kurtp
*  make ica.h shared and ica-c2h.h unshared
*  
*     Rev 1.57   05 Jun 1997 19:35:36   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.56   28 May 1997 10:31:32   terryt
*  client double click support
*  
*     Rev 1.56   27 May 1997 14:29:14   terryt
*  client double click support
*  
*     Rev 1.55   21 Apr 1997 16:56:54   TOMA
*  update
*  
*     Rev 1.54   16 Apr 1997 14:51:12   bradp
*  update
*  
*     Rev 1.53   Mar 27 1997 16:17:56   scottc
*  add java clientid
*  
*     Rev 1.52   22 Jan 1997 16:50:38   terryt
*  client data
*  
*     Rev 1.51   13 Jan 1997 16:27:00   kurtp
*  update
*  
*     Rev 1.50   29 Aug 1996 17:46:34   bradp
*  update
*  
*
*******************************************************************************/

#ifndef __ICAC2H_H__
#define __ICAC2H_H__

/* #pragma pack(1) */

#define IMEFILENAME_LENGTH 32
/*=============================================================================
==   Common Module Header
=============================================================================*/

/*
 *  Common module header (36 bytes)
 */
typedef struct _MODULE_C2H {
    USHORT ByteCount;               // length of module data in bytes (<2k)
    BYTE ModuleCount;               // number of modules headers left to be sent
    BYTE ModuleClass;               // module class (MODULECLASS)
    BYTE VersionL;                  // lowest supported version
    BYTE VersionH;                  // highest supported version
    BYTE ModuleName[13];            // client module name (8.3)
    BYTE HostModuleName[9];         // optional host module name (9 characters)
    USHORT ModuleDate;              // module date in dos format
    USHORT ModuleTime;              // module time in dos format
    ULONG ModuleSize;               // module file size in bytes
} MODULE_C2H, * PMODULE_C2H;

typedef struct _IMEFILENAME {
      BYTE imeFileName[ IMEFILENAME_LENGTH + 1 ];     // IME file name (length 32)
} IMEFileName, * pIMEFileName;

/*=============================================================================
==   User Interfaces
=============================================================================*/

#define VERSION_CLIENTL_UI     1
#define VERSION_CLIENTH_UI_15  2    // WinFrame 1.5
#define VERSION_CLIENTH_UI_16  3    // WinFrame 1.6
#define VERSION_CLIENTH_UI_PICASSO 4 // picasso server
#define VERSION_CLIENTH_UI     VERSION_CLIENTH_UI_PICASSO

#define VERSION_CLIENTL_UIEXT  1
#define VERSION_CLIENTH_UIEXT  1

/*
 *  User Interface  (winlink.exe)
 */
typedef struct _UI_C2H {

    /* version 1 */
    MODULE_C2H Header;
    BULONG fDisableSound: 1;     // don't send sound to client
    BULONG fReconnect: 1;        // request to reconnect to existing session
    BULONG fDisableCtrlAltDel: 1;// disable ctrl-alt-del on host for this client
    BULONG fPromptForPassword: 1;// force server to prompt for password
    BULONG fTwiModeEnableFlag: 1;// Seemless mode or not
    ULONG KeyboardLayout;       // e.g. 409
    BYTE EncryptionLevel;       // encryption level of password (0=plain text)
    BYTE EncryptionSeed;        // seed for encryption
    USHORT oDomain;             // offset - domain of user database
    USHORT oUserName;           // offset - user's logon name
    USHORT oPassword;           // offset - user's password
    USHORT oClientDirectory;    // offset - directory containing winlink.exe
    USHORT oWorkDirectory;      // offset - user's working directory (initial dir)
    USHORT oInitialProgram;     // offset - user's initial program
    BYTE Lpt1;                  // connect this host lpt to (1=lpt1 .. 8=lpt8)
    BYTE Port1;                 // ... this client port (1=lpt1 .. 8=com4)
    BYTE Lpt2;                  // connect this host lpt to (1=lpt1 .. 8=lpt8)
    BYTE Port2;                 // ... this client port (1=lpt1 .. 8=com4)
    BYTE Lpt3;                  // connect this host lpt to (1=lpt1 .. 8=lpt8)
    BYTE Port3;                 // ... this client port (1=lpt1 .. 8=com4)

    /* version 2
     * I collapsed 158 2 and 3 down to 2 because the VERSION_CLIENTH_UI
     * was set to 2 even though the comments below included version 2 and
     * version 3 prior to my version 3.
     */
    USHORT oCloudName;          // offset - appserver cloud name for load-balance
    USHORT ClientBuildNumber;   // client build number
    USHORT oClientName;         // offset - name of client
    USHORT FixedLength;         // sizeof this structure
    USHORT OffsetLength;        // sizeof any following data

    /* version 3 */
    USHORT oClientLicense;      // offset - client license number

    /* version 4 */
    USHORT EncodingType;        // Encoding type
                                // - 0: ANSI/ASCII
                                // - 1: UNICODE
    USHORT EncodingData;        // Encoding data ( encoding type dependent )
                                // - for ANSI/ASCII - code page
                                //                    0: system default CP
                                //                    1: system OEM CP
	 USHORT KeyboardType;        // Keyboard type (PC/XT, PC/AT, Japanese...)
	 BYTE KeyboardSubType;       // Keyboard subtype (US 101, JPN 106)
	 BYTE KeyboardFunctionKey;   // Number of function keys
    USHORT oimeFileName;        // Offset to IME file name
    USHORT Reserved;            // keep the structure at multiple of 4 bytes

} UI_C2H, * PUI_C2H;

/*
 *  User Interface extension  (uicwin.dll)
 */
typedef struct _UIEXT_C2H {
    MODULE_C2H Header;
} UIEXT_C2H, * PUIEXT_C2H;


/*=============================================================================
==   Winstation Driver
=============================================================================*/

/*
 *  Version 1 - SouthBeach 1.29
 *  Version 3 - WinFrame 1.5 (build 158)
 *  Version 4 - Internet Client (new)
 *  Version 5 - WinFrame 2.0 beta
 *  Version 6 - WinFrame 2.0
 *  Version 7 - Picasso with WD reducer
 */
#define VERSION_CLIENTL_WD   1 
#define MIN_WD_REDUCER_CLIENT_VERSION  7
#define VERSION_CLIENTH_WD   7


/*
 *  Client product id
 */
#define CLIENTID_CITRIX_DOS          0x0001     // citrix dos client
// avoid 2 (andy)
#define CLIENTID_CITRIX_CONSOLE      0x0003     // citrix console
#define CLIENTID_CITRIX_TEXT_TERM    0x0004     // citrix text terminals
#define CLIENTID_CITRIX_MVGA_TERM    0x0007     // citrix MVGA terminals
#define CLIENTID_CITRIX_NEW_JAVA     0x0008     // citrix JAVA client
#define CLIENTID_CITRIX_WINCE        0x0009     // citrix WinCE client

#define CLIENTID_CITRIX_INTERNET     0x0101     // citrix internet client

#define CLIENTID_FORCESERIALIZE_FLAG 0x4000     // client requires license no.
#define CLIENTID_TERMINAL_FLAG       0x8000     // terminal based client


/*
 *  ica 3.0 (wdica30.dll)
 */
typedef struct _WD_C2H {

    /* version 1,2,3,4 */
    MODULE_C2H Header;
    ULONG SerialNumber;         // client computer unique serial number
    struct {
        BYTE day;               // 1-31
        BYTE month;             // 1-12
        USHORT year;            // 1980-2099
    } CurrentDate;              // client computer date
    struct {
        BYTE hour;              // 0-23
        BYTE minute;            // 0-59
        BYTE second;            // 0-59
        BYTE hsecond;           // 0-99
    } CurrentTime;              // client computer time
    BULONG fEnableGraphics: 1;   // graphics are supported
    BULONG fEnableMouse: 1;      // mouse is available
    BULONG fDoubleClickDetect: 1;// client will detect double clicks (ver 6)
    USHORT TextModeCount;       // number of supported text modes
    USHORT oTextMode;           // offset - array of WDTEXTMODE structures
    USHORT ICABufferLength;     // maximum size of one ica packet
    USHORT OutBufCountHost;     // number of outbufs on host
    USHORT OutBufCountClient;   // number of outbufs on client
    USHORT OutBufDelayHost;     // host outbuf write delay (1/1000 seconds)
    USHORT OutBufDelayClient;   // client outbuf write delay (1/1000 seconds)
    USHORT ClientProductId;     // client software product id
    ULONG  ClientSerialNumber;  // client software serial number

    /* version 5,6 */
    USHORT VcBindCount;         // number of WDVCBIND structures in array
    USHORT oVcBind;             // offset - array of WDVCBIND structures

    /* version 7 */
    BYTE C2H_PowerOf2Wanted;    // reduction buffer requested
    BYTE H2C_PowerOf2Wanted;    // expansion buffer requested
    USHORT H2C_MaxNewData;      // maximum expanded packet size
    USHORT ReducerVersion;      // version of reducer

} WD_C2H, * PWD_C2H;


/*=============================================================================
==   Virtual Drivers
=============================================================================*/

/*
 *  Version 1 - SouthBeach 1.29
 *  Version 2 - WinFrame 1.6    (256 color)
 *  Version 3 - WinFrame 2.0    (dim support)
 */

#define VERSION_CLIENTL_VDTW   1
#define VERSION_CLIENTH_VDTW   3

#define VERSION_CLIENTL_VDCDM  1
#define VERSION_CLIENTH_VDCDM  4 // Must track cdmwire.h CDM_VERSIONx

#define VERSION_CLIENTL_VDCPM  1
#define VERSION_CLIENTH_VDCPM  1

#define VERSION_CLIENTL_VDSPL  4 // Do not set lower, means old client
#define VERSION_CLIENTH_VDSPL  4 // Must track cpmwire.h CPM_MAX_VERSION

#define VERSION_CLIENTL_VDCOM  4 // Do not set lower, means old client
#define VERSION_CLIENTH_VDCOM  4 // Must track ccmwire.h CCM_MAX_VERSION

/*
 *  common virtual driver header
 */
typedef struct _VD_C2H {
    MODULE_C2H Header;
    ULONG ChannelMask;          // bit mask of supported vd channels (b0=0)
    VDFLOW Flow;
} VD_C2H, * PVD_C2H;

/*
 *  client drive mapping (vdcdm30.dll)
 */
typedef struct _VDCDM_C2H {

    /* version 1 */
    VD_C2H Header;
    USHORT ClientDriveCount;    // number of client drives
    USHORT oClientDrives;       // offset - array of VDCLIENTDRIVES structures

    USHORT CacheFlags;
    USHORT CacheTimeout0;
    USHORT CacheTimeout1;
    USHORT CacheXferSize;

    /* version 2 */
    USHORT oClientDrives2;      // offset - array of VDCLIENTDRIVES2 structures

#if defined(UNICODESUPPORT)
    /* version 3 */
    USHORT EncodingType;        // Encoding type 
    USHORT EncodingData;        // Encoding data ( depending on type )
#endif

} VDCDM_C2H, * PVDCDM_C2H;

#define sizeof_VDCDM_C2H	(sizeof(VD_C2H) + 14)

/*
 *  client printer mapping (vdcpm30.dll)
 */
typedef struct _VDCPM_C2H {

    /* version 1 */
    VD_C2H Header;
    BYTE LptMask;               // mask of available client lpt ports (b0=lpt1)
    BYTE ComMask;               // mask of available client com ports (b0=com1)

#if defined(UNICODESUPPORT)
    /* version 2 */
    USHORT EncodingType;        // Encoding type 
    USHORT EncodingData;        // Encoding data ( depending on type )
#endif

} VDCPM_C2H, * PVDCPM_C2H;

#define sizeof_VDCPM_C2H	(sizeof(VD_C2H) + 2)

/*=============================================================================
==   Protocol Driver Header
=============================================================================*/

#define VERSION_CLIENTL_PDFRAME     1
#define VERSION_CLIENTH_PDFRAME     1

#define VERSION_CLIENTL_PDRELIABLE  1
#define VERSION_CLIENTH_PDRELIABLE  1

#define VERSION_CLIENTL_PDCOMPRESS  1
#define VERSION_CLIENTH_PDCOMPRESS  1

#define VERSION_CLIENTL_PDCRYPT1    1
#define VERSION_CLIENTH_PDCRYPT1    1

#define VERSION_CLIENTL_PDMODEM     1
#define VERSION_CLIENTH_PDMODEM     1

#define VERSION_CLIENTL_PDTAPI		1
#define VERSION_CLIENTH_PDTAPI		1

/*
 *  common protocol driver header
 */
typedef struct _PD_C2H {
    MODULE_C2H Header;
    BYTE PdClass;                 // class of protocol driver (PDCLASS)
    BYTE Pad1[3];
} PD_C2H, * PPD_C2H;

/*
 *  framing protocol driver (pdframe.dll)
 */
typedef struct _PDFRAME_C2H {
    PD_C2H Header;
} PDFRAME_C2H, * PPDFRAME_C2H;

/*
 *  reliable protocol driver (pdreli.dll)
 */
typedef struct _PDRELIABLE_C2H {
    PD_C2H Header;
    ULONG MaxRetryTime;         // maximum time to attempt retries (msec)
    ULONG RetransmitTimeDelta;  // increment timeout by this value (msec)
    ULONG DelayedAckTime;       // delayed ack timeout (msec)
} PDRELIABLE_C2H, * PPDRELIABLE_C2H;

/*
 *  compression protocol driver (pdcomp.dll)
 */
typedef struct _PDCOMPRESS_C2H {
    PD_C2H Header;
    USHORT MaxCompressDisable;  // disable compression for this many outbufs
    USHORT Pad;
} PDCOMPRESS_C2H, * PPDCOMPRESS_C2H;

/*
 *  compression protocol driver (pdcrypt1.dll)
 */
typedef struct _PDCRYPT1_C2H {
    PD_C2H Header;
    BYTE EncryptionLevel;       // encryption level
} PDCRYPT1_C2H, * PPDCRYPT1_C2H;

#define sizeof_PDCRYPT1_C2H	(sizeof(PD_C2H) + 1)

/*
 *  modem protocol driver (pdmodem.dll and pdtapi.dll)
 */
typedef struct _PDMODEM_C2H {
    PD_C2H Header;
    USHORT oModemName;
} PDMODEM_C2H, * PPDMODEM_C2H;

#define sizeof_PDMODEM_C2H	(sizeof(PD_C2H) + 2)

/*=============================================================================
==   Transport Driver Header
=============================================================================*/

#define VERSION_CLIENTL_TDASYNC   1
#define VERSION_CLIENTH_TDASYNC   2

#define VERSION_CLIENTL_TDCOMM    1
#define VERSION_CLIENTH_TDCOMM    2

#define VERSION_CLIENTL_TDINT14   1
#define VERSION_CLIENTH_TDINT14   2

#define VERSION_CLIENTL_TDDIGI    1
#define VERSION_CLIENTH_TDDIGI    2

#define VERSION_CLIENTL_TDSST     1
#define VERSION_CLIENTH_TDSST     2

#define VERSION_CLIENTL_TDNETB    1
#define VERSION_CLIENTH_TDNETB    2

#define VERSION_CLIENTL_TDSPX     1
#define VERSION_CLIENTH_TDSPX     2

#define VERSION_CLIENTL_TDIPX     1
#define VERSION_CLIENTH_TDIPX     2

#define VERSION_CLIENTL_TDTCPFTP  1
#define VERSION_CLIENTH_TDTCPFTP  2

#define VERSION_CLIENTL_TDTCPNOV  1
#define VERSION_CLIENTH_TDTCPNOV  2

#define VERSION_CLIENTL_TDTCPMS   1
#define VERSION_CLIENTH_TDTCPMS   2

#define VERSION_CLIENTL_TDTCPVSL  1
#define VERSION_CLIENTH_TDTCPVSL  2

/*
 *  transport drivers (tdnetb.dll, tdasync.dll, tdipx.dll, .... )
 */
typedef struct _TD_C2H {

    /* version 1 */
    MODULE_C2H Header;
    USHORT OutBufLength;        // length of outbufs in bytes

    /* version 2 */
    USHORT ClientAddressFamily; // address family of ClientAddress
    BYTE ClientAddress[20];     // client network address
} TD_C2H, * PTD_C2H;

/*
 * Address families.
 */
#define AF_UNSPEC       0               /* unspecified */
#define AF_UNIX         1               /* local to host (pipes, portals) */
#define AF_INET         2               /* internetwork: UDP, TCP, etc. */
#define AF_IMPLINK      3               /* arpanet imp addresses */
#define AF_PUP          4               /* pup protocols: e.g. BSP */
#define AF_CHAOS        5               /* mit CHAOS protocols */
#define AF_IPX          6               /* IPX and SPX */
#define AF_NS           6               /* XEROX NS protocols */
#define AF_ISO          7               /* ISO protocols */
#define AF_OSI          AF_ISO          /* OSI is ISO */
#define AF_ECMA         8               /* european computer manufacturers */
#define AF_DATAKIT      9               /* datakit protocols */
#define AF_CCITT        10              /* CCITT protocols, X.25 etc */
#define AF_SNA          11              /* IBM SNA */
#define AF_DECnet       12              /* DECnet */
#define AF_DLI          13              /* Direct data link interface */
#define AF_LAT          14              /* LAT */
#define AF_HYLINK       15              /* NSC Hyperchannel */
#define AF_APPLETALK    16              /* AppleTalk */
#define AF_NETBIOS      17              /* NetBios-style addresses */


/*=============================================================================
==   Protocol Resolver Header
=============================================================================*/

typedef struct _PR_C2H {
    MODULE_C2H Header;
} PR_C2H, * PPR_C2H;


/*=============================================================================
==   Scripting Header
=============================================================================*/

typedef struct _SCRIPT_C2H {
    MODULE_C2H Header;
} SCRIPT_C2H, * PSCRIPT_C2H;


// #pragma pack()

#endif //__ICAC2H_H__
