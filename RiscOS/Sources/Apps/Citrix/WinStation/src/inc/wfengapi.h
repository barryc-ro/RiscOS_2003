/***************************************************************************
*
*  WFENGAPI.H
*
*  This module contains all externalized WinFrame ENGINE APIs, defines,
*   and structures
*
*   (NOTE: windows.h is expected to be included before this file)
*
*  Copyright 1995, Citrix Systems Inc.
*
*  Author: Butch Davis (4/4/95)
*
*  $Log$
*  
*     Rev 1.42   10 Sep 1997 16:09:16   kalyanv
*  added the encryption level session names
*  
*     Rev 1.41   20 Aug 1997 00:12:16   tariqm
*  RSA Copyright message
*  
*     Rev 1.39   18 Jul 1997 15:40:30   tariqm
*  Encryption DLL  Load
*  
*     Rev 1.38   16 Jul 1997 18:11:40   terryt
*  add error messages
*  
*     Rev 1.37   25 Jun 1997 18:09:56   tariqm
*  Addition of encryption interface to GUI
*  
*     Rev 1.35   18 Jun 1997 11:36:54   butchd
*  Added SV_TYPE_APPSERVER bit
*  
*     Rev 1.34   11 Jun 1997 10:17:00   terryt
*  client double click support
*
*     Rev 1.33   18 Apr 1997 16:44:36   butchd
*  changed min & incr of persistant cache to 2K
*  
*     Rev 1.32   15 Apr 1997 18:46:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.31   20 Feb 1997 14:06:26   butchd
*  update
*  
*     Rev 1.30   11 Feb 1997 10:58:58   butchd
*  update
*  
*     Rev 1.29   08 Feb 1997 13:55:02   jeffm
*  multiple browser support added
*  
*     Rev 1.28   07 Feb 1997 13:52:14   butchd
*  update
*  
*     Rev 1.27   06 Feb 1997 11:12:58   bradp
*  reduce keyboard timer
*  
*     Rev 1.26   28 Jan 1997 07:25:48   butchd
*  added Server Location UI defines
*  
*     Rev 1.25   22 Jan 1997 17:49:54   kurtp
*  update
*  
*     Rev 1.24   20 Jan 1997 18:12:10   butchd
*  Added browser address list section key defines
*  
*     Rev 1.23   14 Jan 1997 17:24:14   butchd
*  update
*  
*     Rev 1.22   13 Jan 1997 14:41:50   butchd
*  update
*  
*     Rev 1.21   27 Nov 1996 19:16:52   jeffm
*  added SetInfo type of WFEFileSecurity
*  
*     Rev 1.20   13 Nov 1996 12:18:34   richa
*  
*     Rev 1.19   28 Aug 1996 10:22:32   marcb
*  update
*  
*     Rev 1.18   13 Aug 1996 13:12:54   jeffm
*  Added max for x and y resolution of client.
*  
*     Rev 1.17   13 Aug 1996 11:38:20   BillG
*  add ICAPortNumber
*  
*     Rev 1.16   27 Jul 1996 11:04:24   butchd
*  update
*  
*     Rev 1.15   26 Jun 1996 14:59:26   brucef
*  Added DOS-specific keywords for mouse and
*  keyboard user preferences.
*  
*  
*     Rev 1.14   25 Jun 1996 11:47:50   butchd
*  update
*  
*     Rev 1.13   12 Jun 1996 14:16:46   marcb
*  update
*  
*     Rev 1.12   04 Jun 1996 15:49:36   jeffm
*  update
*  
*     Rev 1.10   01 Jun 1996 12:15:42   unknown
*  
*     Rev 1.9   16 May 1996 12:25:22   marcb
*  update
*  
*     Rev 1.8   06 May 1996 19:26:38   jeffm
*  update
*  
*     Rev 1.7   27 Apr 1996 16:05:10   andys
*  soft keyboard
*
*     Rev 1.6   09 Feb 1996 16:09:00   butchd
*  added CLIENT_ERROR_WIN16_WFENGINE_ALREADY_RUNNING error
*
*     Rev 1.5   09 Feb 1996 13:39:42   butchd
*  increased MAX_ERRORMESSAGE from 100 to 256
*
*     Rev 1.4   31 Jan 1996 19:37:30   butchd
*  added WFCAPI_NOLOADDS typedef
*
*     Rev 1.3   30 Jan 1996 09:52:16   bradp
*  update
*
*     Rev 1.2   25 Jan 1996 11:40:16   butchd
*  Different INI_ECHO_TTY for DOS & WIN
*
*     Rev 1.1   24 Jan 1996 10:33:24   butchd
*  update
*
*     Rev 1.0   20 Jan 1996 14:00:26   kurtp
*  Initial revision.
*
****************************************************************************/

#ifndef __WFENGAPI_H__
#define __WFENGAPI_H__

#ifdef __cplusplus
extern "C" {
#endif


/*=============================================================================
 ==   assure consistant structure packing regardless of compiler flags
 ============================================================================*/
/* #ifdef WIN32 */
/* #pragma pack(8) */
/* #else */
/* #pragma pack(1) */
/* #endif */

/*=============================================================================
 ==   Define the Citrix/Hydrix SV_TYPE_APPSERVER bit if not defined
 ==   Future versions of lmserver.h should have this bit set (someday?)
 ============================================================================*/
#ifndef SV_TYPE_APPSERVER
#define SV_TYPE_APPSERVER           0x10000000  /* Multi-User NT and Citrix WinFrame */
#endif

/*=============================================================================
 ==   basic defines and typedefs
 ============================================================================*/
#ifdef DOS
#define WFCAPI _cdecl far _loadds
#define PWFCAPI _cdecl far *
#define WFCAPI_NOLOADDS _cdecl far
#define PWFCAPI_NOLOADDS _cdecl far *
#endif

#ifdef  WIN16
#define WFCAPI _cdecl far _loadds
#define PWFCAPI _cdecl far *
#define WFCAPI_NOLOADDS _cdecl far
#define PWFCAPI_NOLOADDS _cdecl far *
#endif

#ifdef  WIN32
#define WFCAPI _cdecl
#define PWFCAPI _cdecl *
#define WFCAPI_NOLOADDS _cdecl
#define PWFCAPI_NOLOADDS _cdecl *
#endif

#ifdef RISCOS
#define WFCAPI _cdecl
#define PWFCAPI _cdecl *
#define WFCAPI_NOLOADDS _cdecl
#define PWFCAPI_NOLOADDS _cdecl *
#endif

typedef char CHAR;
#if 0
typedef unsigned char UCHAR;
typedef int INT;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#endif
    
typedef void far * PVOID;
typedef char far * PCHAR;
#if 0
typedef UCHAR far * PUCHAR;
typedef USHORT far * PUSHORT;
typedef ULONG far * PULONG;
#endif
    
typedef PVOID  far * PPVOID;
typedef PCHAR  far * PPCHAR;
typedef PUCHAR far * PPUCHAR;

#if 0
typedef UCHAR BOOLEAN;
#endif
typedef BOOLEAN far *PBOOLEAN;         

#ifndef IN
#define IN
#endif

typedef unsigned long (PWFCAPI PLIBPROCEDURE)( );
typedef PLIBPROCEDURE far * PPLIBPROCEDURE;

/*
 *  keyboard shift states
 */
#define KSS_SYSREQ          0x8000
#define KSS_CAPSLOCK        0x4000
#define KSS_NUMLOCK         0x2000
#define KSS_SCROLLLOCK      0x1000
#define KSS_RIGHTALT        0x0800
#define KSS_RIGHTCTRL       0x0400
#define KSS_LEFTALT         0x0200
#define KSS_LEFTCTRL        0x0100
#define KSS_INSERTON        0x0080
#define KSS_CAPSLOCKON      0x0040
#define KSS_NUMLOCKON       0x0020
#define KSS_SCROLLLOCKON    0x0010
#define KSS_EITHERALT       0x0008
#define KSS_EITHERCTRL      0x0004
#define KSS_LEFTSHIFT       0x0002
#define KSS_RIGHTSHIFT      0x0001
#define KSS_EITHERSHIFT     0x0003  /* KSS_LEFTSHIFT | KSS_RIGHTSHIFT */


/*=============================================================================
 ==   Status and Error codes
 ============================================================================*/

/*
 *  Exit code delimiters
 */
#define CLIENT_STATUS_SUCCESS                        0
#define CLIENT_ERROR                              1000

/*
 *  WdPoll status codes (1-254
 */
#define CLIENT_STATUS_CONNECTED                      1
#define CLIENT_STATUS_CONNECTION_BROKEN_HOST         2
#define CLIENT_STATUS_CONNECTION_BROKEN_CLIENT       3
#define CLIENT_STATUS_MODEM_INIT                     4
#define CLIENT_STATUS_MODEM_DIALING                  5
#define CLIENT_STATUS_MODEM_WAITING                  6
#define CLIENT_STATUS_MODEM_NO_RESPONSE              7
#define CLIENT_STATUS_MODEM_ERROR                    8
#define CLIENT_STATUS_MODEM_NO_DIAL_TONE             9
#define CLIENT_STATUS_MODEM_REDIALING               10
#define CLIENT_STATUS_MODEM_VOICE                   11
#define CLIENT_STATUS_MODEM_BUSY                    12
#define CLIENT_STATUS_MODEM_TERMINATE               13
#define CLIENT_STATUS_MODEM_TIMEOUT                 14
#define CLIENT_STATUS_TTY_CONNECTED                 15
#define CLIENT_STATUS_MODEM_OUT_OF_RETRIES          16
#define CLIENT_STATUS_RECONNECT_TO_HOST             17
#define CLIENT_STATUS_KILL_FOCUS                    18
#define CLIENT_STATUS_REBOOT                        19
#define CLIENT_STATUS_CTRLBREAK                     20
#define CLIENT_STATUS_NO_DATA                       21
#define CLIENT_STATUS_MENUKEY                       22
#define CLIENT_STATUS_QUERY_CLOSE                   23   // (WIN) wait for response
#define CLIENT_STATUS_LOADING_STACK                 24
#define CLIENT_STATUS_CONNECTING                    25
#define CLIENT_STATUS_SEND_START                    26
#define CLIENT_STATUS_SEND_STOP                     27
#define CLIENT_STATUS_RECEIVE_START                 28
#define CLIENT_STATUS_RECEIVE_STOP                  29
#define CLIENT_STATUS_ERROR_RETRY                   30
#define CLIENT_STATUS_BEEPED                        31
#define CLIENT_STATUS_DELETE_CONNECT_DIALOG         32
#define CLIENT_STATUS_DISCONNECTING                 33
#define CLIENT_STATUS_QUERY_ACCESS                  34   // (WIN) wait for response


// Range of 600-899 is reserved for specific client implementations
#define CLIENT_STATUS_RESERVED_6XX_8XX              600


#define CLIENT_STATUS_HOTKEY1                       900 // All
#define CLIENT_STATUS_HOTKEY2                       901 // All
#define CLIENT_STATUS_HOTKEY3                       902 // Win only
#define CLIENT_STATUS_HOTKEY4                       903 // Win only
#define CLIENT_STATUS_HOTKEY5                       904 // Win only
#define CLIENT_STATUS_HOTKEY6                       905 // Win only
#define CLIENT_STATUS_HOTKEY7                       906 // Win only
#define CLIENT_STATUS_HOTKEY8                       907 // Win only
#ifdef DOS
#define WFENG_NUM_HOTKEYS                           2
#else
#define WFENG_NUM_HOTKEYS                           8
#endif

/*
 * Hotkeys (all)
 */
#define HOTKEY_UI               /* Shift+F1 */      CLIENT_STATUS_HOTKEY1
#define HOTKEY_EXIT             /* Shift+F3 */      CLIENT_STATUS_HOTKEY2

/*
 * Hotkeys (Win)
 */
#define HOTKEY_TITLEBAR_TOGGLE  /* Shift+F2 */      CLIENT_STATUS_HOTKEY3
#define HOTKEY_CTRL_ALT_DEL     /* Ctrl+F1  */      CLIENT_STATUS_HOTKEY4
#define HOTKEY_CTRL_ESC         /* Ctrl+F2  */      CLIENT_STATUS_HOTKEY5
#define HOTKEY_ALT_ESC          /* Alt+F2   */      CLIENT_STATUS_HOTKEY6
#define HOTKEY_ALT_TAB          /* Alt+F3   */      CLIENT_STATUS_HOTKEY7
#define HOTKEY_ALT_BACKTAB      /* Ctrl+F3  */      CLIENT_STATUS_HOTKEY8

/*
 *  General errors
 */
#define CLIENT_ERROR_DLL_NOT_FOUND                1000
#define CLIENT_ERROR_NO_MEMORY                    1001
#define CLIENT_ERROR_BAD_OVERLAY                  1002
#define CLIENT_ERROR_BAD_PROCINDEX                1003
#define CLIENT_ERROR_BUFFER_TOO_SMALL             1004
#define CLIENT_ERROR_CORRUPT_ALLOC_HEADER         1005
#define CLIENT_ERROR_CORRUPT_ALLOC_TRAILER        1006
#define CLIENT_ERROR_CORRUPT_FREE_HEADER          1007
#define CLIENT_ERROR_CORRUPT_SEG_HEADER           1008
#define CLIENT_ERROR_MEM_ALREADY_FREE             1009
#define CLIENT_ERROR_MEM_TYPE_MISMATCH            1010
#define CLIENT_ERROR_NULL_MEM_POINTER             1011
#define CLIENT_ERROR_IO_PENDING                   1012
#define CLIENT_ERROR_INVALID_BUFFER_SIZE          1013
#define CLIENT_ERROR_INVALID_MODE                 1014
#define CLIENT_ERROR_NOT_OPEN                     1015
#define CLIENT_ERROR_NO_OUTBUF                    1016
#define CLIENT_ERROR_DLL_PROCEDURE_NOT_FOUND      1017
#define CLIENT_ERROR_DLL_LARGER_THAN_256K         1018
#define CLIENT_ERROR_DLL_BAD_EXEHEADER            1019
#define CLIENT_ERROR_OPEN_FAILED                  1020
#define CLIENT_ERROR_INVALID_HANDLE               1021
#define CLIENT_ERROR_VD_NOT_FOUND                 1022
#define CLIENT_ERROR_WD_NAME_NOT_FOUND            1023
#define CLIENT_ERROR_PD_NAME_NOT_FOUND            1024
#define CLIENT_ERROR_THINWIRE_OUTOFSYNC           1025
#define CLIENT_ERROR_NO_MOUSE                     1026
#define CLIENT_ERROR_INVALID_CALL                 1027
#define CLIENT_ERROR_QUEUE_FULL                   1028
#define CLIENT_ERROR_INVALID_DLL_LOAD             1029
#define CLIENT_ERROR_PD_ERROR                     1030      // call GetLastError
#define CLIENT_ERROR_VD_ERROR                     1031      // call GetLastError
#define CLIENT_ERROR_ALREADY_OPEN                 1032
#define CLIENT_ERROR_PORT_NOT_AVAILABLE           1033
#define CLIENT_ERROR_IO_ERROR                     1034
#define CLIENT_ERROR_THINWIRE_CACHE_ERROR         1035
#define CLIENT_ERROR_BAD_FILE                     1036
#define CLIENT_ERROR_CONFIG_NOT_FOUND             1037
#define CLIENT_ERROR_SERVER_FILE_NOT_FOUND        1038
#define CLIENT_ERROR_PROTOCOL_FILE_NOT_FOUND      1039
#define CLIENT_ERROR_MODEM_FILE_NOT_FOUND         1040
#define CLIENT_ERROR_LAN_NOT_AVAILABLE            1041
#define CLIENT_ERROR_PD_TRANSPORT_UNAVAILABLE     1042      // unavail on open
#define CLIENT_ERROR_INVALID_PARAMETER            1043
#define CLIENT_ERROR_WD_NOT_LOADED                1044
#define CLIENT_ERROR_NOT_CONNECTED                1045
#define CLIENT_ERROR_VD_NOT_LOADED                1046
#define CLIENT_ERROR_ALREADY_CONNECTED            1047
#define CLIENT_ERROR_WFENGINE_NOT_FOUND           1048
#define CLIENT_ERROR_ENTRY_NOT_FOUND              1049
#define CLIENT_ERROR_PD_ENTRY_NOT_FOUND           1050
#define CLIENT_ERROR_WD_ENTRY_NOT_FOUND           1051
#define CLIENT_ERROR_PD_SECTION_NOT_FOUND         1052
#define CLIENT_ERROR_WD_SECTION_NOT_FOUND         1053
#define CLIENT_ERROR_DEVICE_SECTION_NOT_FOUND     1054
#define CLIENT_ERROR_VD_SECTION_NOT_FOUND         1055
#define CLIENT_ERROR_VD_NAME_NOT_FOUND            1056
#define CLIENT_ERROR_SERVER_CONFIG_NOT_FOUND      1057
#define CLIENT_ERROR_CONFIG_CONFIG_NOT_FOUND      1058
#define CLIENT_ERROR_HOTKEY_SHIFTSTATES_NOT_FOUND 1059
#define CLIENT_ERROR_HOTKEY_KEYS_NOT_FOUND        1060
#define CLIENT_ERROR_KEYBOARDLAYOUT_NOT_FOUND     1061
#define CLIENT_ERROR_UI_ENGINE_VER_MISMATCH       1062
#define CLIENT_ERROR_IPC_TIMEOUT                  1063
#define CLIENT_ERROR_UNENCRYPT_RECEIVED           1064
#define CLIENT_ERROR_NENUM_NOT_DEFINED            1065
#define CLIENT_ERROR_NENUM_NO_SERVERS_FOUND       1066
#define CLIENT_ERROR_NENUM_NETWORK_ERROR          1067
#define CLIENT_ERROR_NENUM_WINDOWS95_NOT_SUPPORTED 1068
#define CLIENT_ERROR_CONNECTION_TIMEOUT           1069
#define CLIENT_ERROR_DRIVER_UNSUPPORTED           1070
#define CLIENT_ERROR_NO_MASTER_BROWSER            1071
#define CLIENT_ERROR_TRANSPORT_NOT_AVAILABLE      1072
#define CLIENT_ERROR_NO_NAME_RESOLVER             1073
#define CLIENT_ERROR_SEVEN_BIT_DATA_PATH          1074
#define CLIENT_ERROR_WIN16_WFENGINE_ALREADY_RUNNING 1075
#define CLIENT_ERROR_HOST_NOT_SECURED             1076
#define CLIENT_ERROR_ENCRYPT_UNSUPPORT_BYCLIENT   1077
#define CLIENT_ERROR_ENCRYPT_UNSUPPORT_BYHOST     1078


// Range of 1600-1899 is reserved for specific client implementations
#define CLIENT_ERROR_RESERVED_16XX_18XX           1600


/*=============================================================================
 ==  String lengths and invalid character defines
 ============================================================================*/


#ifdef  VERSION1_LABEL
#define LABEL_LENGTH            12
#define LABEL_INVALID_CHARACTERS                "=<>|\"@&()^;,\\/-"
#define LABEL_INVALID_CHARACTERS_SPACED         "= < > | \" @ & ( ) ^ ; ,\\ / -"
#endif

#define DOSFILENAME_LENGTH      12
#define DESCRIPTION_INVALID_CHARACTERS          "\\/:*?\"<>|,.()[]"
#define DESCRIPTION_INVALID_CHARACTERS_SPACED   "\\ / : * ? \" < > | , . ( ) [ ]"
#define DESCRIPTION_LENGTH      40
#define PROGRAMGROUP_LENGTH     30
#define NAMEENUMERATOR_LENGTH   13
#define NAMERESOLVER_LENGTH     13
#define TRANSPORTNAME_LENGTH    40
#define ENCRYPTION_LEVEL_NAME_LENGTH 60
#define EMULATIONNAME_LENGTH    40
#define ENCRYPTIONLEVEL_LENGTH  37

#ifdef  CLIENTNAME_LENGTH
#undef  CLIENTNAME_LENGTH
#endif
#define CLIENTNAME_LENGTH       20                      // from ica30.h

// OUR client serial number length (which is less than max)
#ifdef  CLIENTSN_LENGTH
#undef  CLIENTSN_LENGTH
#endif
#define CLIENTSN_LENGTH         20

#ifdef  DOMAIN_LENGTH
#undef  DOMAIN_LENGTH
#endif
#define DOMAIN_LENGTH           17                      // from ica30.h

#ifdef  USERNAME_LENGTH
#undef  USERNAME_LENGTH
#endif
#define USERNAME_LENGTH         20                      // from ica30.h

#ifdef  PASSWORD_LENGTH
#undef  PASSWORD_LENGTH
#endif
#define PASSWORD_LENGTH         14                      // from ica30.h

#ifdef  DIRECTORY_LENGTH
#undef  DIRECTORY_LENGTH
#endif
#define DIRECTORY_LENGTH        256                     // from ica30.h

#ifdef  INITIALPROGRAM_LENGTH
#undef  INITIALPROGRAM_LENGTH
#endif
#define INITIALPROGRAM_LENGTH   256                     // from ica30.h

#define ENCRYPTEDPASSWORD_LENGTH   (2*PASSWORD_LENGTH+4+8) // 4 + 2X + pad
#define ADDRESS_LENGTH          64   // should be the same as MAX_BR_NAME-1
#define DEVICENAME_LENGTH       59
#define BAUD_LENGTH             7
#define MODEM_LENGTH            40
#define MODEMINIT_LENGTH        12
#define MODEMINITSTRING_LENGTH  80
#define KEYBOARDLAYOUT_LENGTH   40
#define HOTKEYNAME_LENGTH       20

#define DIALINGLOCATION_LENGTH  128
#define DIALINGPREFIX_LENGTH    128
#define DIALPROPENTRY_LENGTH    32
#define DIALCOUNTRY_LENGTH      64

// The following #define is in support of Application Publishing (.ICA files),
// where the 'Address' and 'Description' (the entry's section name) are 
// likely to be the same, so we'll use the smaller of the two 
// (DESCRIPTION_LENGTH) for usability sake.
//
// BUG Note (ButchD 7/27/96): Instead this #define being set to the existing 
// DESCRIPTION_LENGTH (40), the define is set to 38, for compatability with 
// early (WF 1.6 and Internet) .ICA file processing clients.  These
// clients had a bug processing the [ApplicationServers] section which would 
// truncate the 'Description' (section name) to 38 characters max instead of 
// the 40 character max that it should be allowed to have.  Therefore, to 
// maintain compatibility for all .ICA file clients, the 'Address' and 
// 'Description' will have a maximum of 38 characters.

#define ICAFILE_ADDRESS_AND_DESCRIPTION_LENGTH  38

#define MAXPATH 260
#define FILEPATH_LENGTH         MAXPATH

#ifdef MAX_INI_LINE
#undef MAX_INI_LINE
#endif
#define MAX_INI_LINE    256
#define MAXINILINE_LENGTH   MAX_INI_LINE


/*=============================================================================
 ==   String typedefs (string length + 1 for NULL terminator)
 ============================================================================*/

#ifdef  VERSION1_LABEL
typedef CHAR LABEL[ LABEL_LENGTH+1 ];
#endif

typedef CHAR DOSFILENAME[ DOSFILENAME_LENGTH+1 ];
typedef CHAR DESCRIPTION[ DESCRIPTION_LENGTH+1 ];
typedef CHAR PROGRAMGROUP[ PROGRAMGROUP_LENGTH+1 ];
typedef CHAR NAMEENUMERATOR[ NAMEENUMERATOR_LENGTH+1 ];
typedef CHAR NAMERESOLVER[ NAMERESOLVER_LENGTH+1 ];
typedef CHAR TRANSPORTNAME[ TRANSPORTNAME_LENGTH+1 ];
typedef CHAR ENCRYPTIONLEVEL[ ENCRYPTIONLEVEL_LENGTH+1 ];
typedef CHAR EMULATIONNAME[ EMULATIONNAME_LENGTH+1 ];
typedef CHAR CLIENTNAME[ CLIENTNAME_LENGTH+1 ];
typedef CHAR CLIENTSN[ CLIENTSN_LENGTH+1 ];
typedef CHAR DOMAIN[ DOMAIN_LENGTH+1 ];
typedef CHAR USERNAME[ USERNAME_LENGTH+1 ];
typedef CHAR PASSWORD[ PASSWORD_LENGTH+1 ];
typedef CHAR ENCRYPTEDPASSWORD[ ENCRYPTEDPASSWORD_LENGTH+1 ];
typedef CHAR DIRECTORY[ DIRECTORY_LENGTH+1 ];
typedef CHAR INITIALPROGRAM[ INITIALPROGRAM_LENGTH+1 ];
typedef CHAR ADDRESS[ ADDRESS_LENGTH+1 ];
typedef CHAR DEVICENAME[ DEVICENAME_LENGTH+1 ];
typedef CHAR BAUD[ BAUD_LENGTH+1 ];
typedef CHAR MODEM[ MODEM_LENGTH+1 ];
typedef CHAR MODEMINIT[ MODEMINIT_LENGTH+1 ];
typedef CHAR MODEMINITSTRING[ MODEMINITSTRING_LENGTH+1 ];
typedef CHAR KEYBOARDLAYOUT[ KEYBOARDLAYOUT_LENGTH+1 ];
typedef CHAR HOTKEYNAME[ HOTKEYNAME_LENGTH+1 ];
typedef CHAR DIALINGLOCATION[ DIALINGLOCATION_LENGTH+1 ];
typedef CHAR DIALINGPREFIX[ DIALINGPREFIX_LENGTH+1 ];
typedef CHAR DIALPROPENTRY[ DIALPROPENTRY_LENGTH+1 ];
typedef CHAR DIALCOUNTRY[ DIALCOUNTRY_LENGTH+1 ];
typedef CHAR FILEPATH[ FILEPATH_LENGTH+1 ];
typedef CHAR MAXINILINE[ MAXINILINE_LENGTH+1 ];


/*=============================================================================
 ==  Default configuration file names, client name file, and setup file
 ============================================================================*/

#define DEF_CONFIG_FILE     "WFCLIENT.INI"
#define DEF_PROTOCOL_FILE   "MODULE.INI"
#define DEF_MODEM_FILE      "MODEM.INI"
#define DEF_SERVER_FILE     "APPSRV.INI"
#define CLIENTNAME_FILE     "WFCNAME.INI"
#define SETUP_FILE          "WFCSETUP.INI"


/*=============================================================================
 ==  Common INI defines (INI_)
 ============================================================================*/

#define INI_EMPTY           ""
#define INI_OFF             "Off"
#define INI_ON              "On"
#define INI_YES             "Yes"
#define INI_NO              "No"
#define INI_DEFAULT         "Default"


/*=============================================================================
 ==  WinFrame Client SETUP INI keys (INI_) and defaults (DEF_)
 ============================================================================*/

/*
 * section name
 */
#define INI_SETUP                   "Client Setup"

#define INI_SETUP_PRODUCTNAME       "ProductName"
#define DEF_SETUP_PRODUCTNAME       ""
#define INI_SETUP_SERIALNUMREQ      "SerialNumReq"
#define DEF_SETUP_SERIALNUMREQ      0
#define INI_SETUP_NETWORKSUPPORT    "NetworkSupport"
#define DEF_SETUP_NETWORKSUPPORT    1


/*=============================================================================
 ==  Client Name INI key (INI_) and defaults (DEF_)
 ==     The ClientName is in <boot-drive>:\WFCNAME.INI for DOS and WIN16
 ==                and is in <system-drive>:\WFCNAME.INI for WIN32
 ============================================================================*/

#define INI_CLIENTNAME       "ClientName"
#define DEF_CLIENTNAME       "<Undefined>"
// client serial number is also stuck here
#define INI_CLIENTSN         "ClientSN"
#define DEF_CLIENTSN         ""
#define INI_CLIENTINIVERSION "Version"
#define DEF_CLIENTINIVERSION 1
#define LATEST_CLIENTINIVERSION 2
#define INI_CLIENTUSERNAME  "ClientUserName"
#define DEF_CLIENTUSERNAME  ""
#define INI_CLIENTCOMPANY   "ClientCompany"
#define DEF_CLIENTCOMPANY   ""


/*=============================================================================
 ==  [WFClient] section configuration INI keys (INI_) and defaults (DEF_)
 ==     (wfclient.ini and appsrv.ini)
 ============================================================================*/

/*
 * section name
 */
#define INI_WFCLIENT        "WFClient"

/*
 * wfclient.ini (and .ica files)
 */
#define INI_KEYBOARDLAYOUT  "KeyboardLayout"
#define DEF_KEYBOARDLAYOUT  "(User Profile)"      // user NT user profile code

#define MAX_BROWSERADDRESSLIST      5             // max count of browser addr

#define INI_TCPBROWSERADDRESS       "TcpBrowserAddress"
#define DEF_TCPBROWSERADDRESS       ""
#define INI_TCPBROWSERSTRING        "TCP/IP"        // for Server Location UI

#define INI_IPXBROWSERADDRESS       "IpxBrowserAddress"
#define DEF_IPXBROWSERADDRESS       ""
#define INI_IPXBROWSERSTRING        "IPX/SPX"       // for Server Location UI

#define INI_NETBIOSBROWSERADDRESS       "NetBiosBrowserAddress"
#define DEF_NETBIOSBROWSERADDRESS       ""
#define INI_NETBIOSBROWSERSTRING        "NETBIOS"   // For Server Location UI

#define INI_BROWSERRETRY      "BrowserRetry"
#define DEF_BROWSERRETRY      3

#define INI_BROWSERTIMEOUT    "BrowserTimeout"
#define DEF_BROWSERTIMEOUT    1000

#define INI_NOUMB           "NoUmb"               // disable umb memory (DOS)

#define INI_LPT1            "Lpt1"
#define DEF_LPT1            0
#define INI_PORT1           "Port1"
#define DEF_PORT1           0

#define INI_LPT2            "Lpt2"
#define DEF_LPT2            0
#define INI_PORT2           "Port2"
#define DEF_PORT2           0

#define INI_LPT3            "Lpt3"
#define DEF_LPT3            0
#define INI_PORT3           "Port3"
#define DEF_PORT3           0

#define INI_KEYBOARDDELAY   "KeyboardDelay"
#define DEF_KEYBOARDDELAY   -1

#define INI_KEYBOARDSPEED   "KeyboardSpeed"
#define DEF_KEYBOARDSPEED   -1

#define INI_VERTICALSPEED   "VerticalSpeed"
#define DEF_VERTICALSPEED   -1

#define INI_HORIZONTALSPEED "HorizontalSpeed"
#define DEF_HORIZONTALSPEED -1

#define INI_DBLSPEEDTHRESHOLD   "DoubleSpeedThreshold"
#define DEF_DBLSPEEDTHRESHOLD   -1

#define INI_SWAPBUTTONS "SwapButtons"
#define DEF_SWAPBUTTONS INI_NO

/*
 * appsrv.ini (and .ica files)
 */
#ifdef RISCOS
#define INI_LOGFILE         "LogFile"
#define DEF_LOGFILE         "wfclog"
#endif
#ifdef DOS
#define INI_LOGFILE         "LogFile"
#define DEF_LOGFILE         "wfclient.log"
#endif
#ifdef WIN16
#define INI_LOGFILE         "LogFileWin16"
#define DEF_LOGFILE         "wfcwin.log"
#endif
#ifdef WIN32
#define INI_LOGFILE         "LogFileWin32"
#define DEF_LOGFILE         "wfcwin32.log"
#endif
#define INI_LOGAPPEND       "LogAppend"
#define DEF_LOGAPPEND       FALSE

#define INI_LOGCONNECT      "LogConnect"
#define DEF_LOGCONNECT      TRUE
#define INI_LOGERRORS       "LogErrors"
#define DEF_LOGERRORS       TRUE
#define INI_LOGTRANSMIT     "LogTransmit"
#define DEF_LOGTRANSMIT     FALSE
#define INI_LOGRECEIVE      "LogReceive"
#define DEF_LOGRECEIVE      FALSE
#define INI_LOGKEYBOARD     "LogKeyboard"
#define DEF_LOGKEYBOARD     FALSE


// Define the ICA TCP Port Number

#define INI_ICAPORTNUMBER       "ICAPortNumber"
#define DEF_ICAPORTNUMBER   1494


/* HOTKEY_UI : Shift + F1 */
#define INI_HOTKEY1_SHIFT   "Hotkey1Shift"
#define DEF_HOTKEY1_SHIFT   "Shift"
#define DEF_HOTKEY1_SHIFTV  KSS_EITHERSHIFT
#define INI_HOTKEY1_CHAR    "Hotkey1Char"
#define DEF_HOTKEY1_CHAR    "F1"
#define DEF_HOTKEY1_CHARV   59

/* HOTKEY_EXIT : Shift + F3 */
#define INI_HOTKEY2_SHIFT   "Hotkey2Shift"
#define DEF_HOTKEY2_SHIFT   "Shift"
#define DEF_HOTKEY2_SHIFTV  KSS_EITHERSHIFT
#define INI_HOTKEY2_CHAR    "Hotkey2Char"
#define DEF_HOTKEY2_CHAR    "F3"
#define DEF_HOTKEY2_CHARV   61

/* HOTKEY_TITLEBAR_TOGGLE : Shift + F2 */
#define INI_HOTKEY3_SHIFT   "Hotkey3Shift"
#define DEF_HOTKEY3_SHIFT   "Shift"
#define DEF_HOTKEY3_SHIFTV  KSS_EITHERSHIFT
#define INI_HOTKEY3_CHAR    "Hotkey3Char"
#define DEF_HOTKEY3_CHAR    "F2"
#define DEF_HOTKEY3_CHARV   60

/* HOTKEY_CTRL_ALT_DEL : Ctrl + F1 */
#define INI_HOTKEY4_SHIFT   "Hotkey4Shift"
#define DEF_HOTKEY4_SHIFT   "Ctrl"
#define DEF_HOTKEY4_SHIFTV  KSS_EITHERCTRL
#define INI_HOTKEY4_CHAR    "Hotkey4Char"
#define DEF_HOTKEY4_CHAR    "F1"
#define DEF_HOTKEY4_CHARV   59

/* HOTKEY_CTRL_ESC : Ctrl + F2 */
#define INI_HOTKEY5_SHIFT   "Hotkey5Shift"
#define DEF_HOTKEY5_SHIFT   "Ctrl"
#define DEF_HOTKEY5_SHIFTV  KSS_EITHERCTRL
#define INI_HOTKEY5_CHAR    "Hotkey5Char"
#define DEF_HOTKEY5_CHAR    "F2"
#define DEF_HOTKEY5_CHARV   60

/* HOTKEY_ALT_ESC : Alt + F2 */
#define INI_HOTKEY6_SHIFT   "Hotkey6Shift"
#define DEF_HOTKEY6_SHIFT   "Alt"
#define DEF_HOTKEY6_SHIFTV  KSS_EITHERALT
#define INI_HOTKEY6_CHAR    "Hotkey6Char"
#define DEF_HOTKEY6_CHAR    "F2"
#define DEF_HOTKEY6_CHARV   60

/* HOTKEY_ALT_TAB : Alt + plus */
#define INI_HOTKEY7_SHIFT   "Hotkey7Shift"
#define DEF_HOTKEY7_SHIFT   "Alt"
#define DEF_HOTKEY7_SHIFTV  KSS_EITHERALT
#define INI_HOTKEY7_CHAR    "Hotkey7Char"
#define DEF_HOTKEY7_CHAR    "plus"
#define DEF_HOTKEY7_CHARV   78

/* HOTKEY_ALT_BACKTAB : Alt + minus */
#define INI_HOTKEY8_SHIFT   "Hotkey8Shift"
#define DEF_HOTKEY8_SHIFT   "Alt"
#define DEF_HOTKEY8_SHIFTV  KSS_EITHERALT
#define INI_HOTKEY8_CHAR    "Hotkey8Char"
#define DEF_HOTKEY8_CHAR    "minus"
#define DEF_HOTKEY8_CHARV   74

#define INI_HOTKEY_SHIFT    "Hotkey%uShift"
#define INI_HOTKEY_CHAR     "Hotkey%uChar"

#define INI_SOUND           "DisableSound"
#define DEF_SOUND           FALSE

#define INI_CTRLALTDEL      "DisableCtrlAltDel"
#define DEF_CTRLALTDEL      TRUE

#define INI_MOUSETIMER      "MouseTimer"
#define DEF_MOUSETIMER      100
#define DEFSTR_MOUSETIMER   "100"


#define INI_MOUSEDOUBLECLICKTIMER      "MouseDoubleClickTimer"
#define DEF_MOUSEDOUBLECLICKTIMER       500
#define DEFSTR_MOUSEDOUBLECLICKTIMER   "500"

#define INI_MOUSEDOUBLECLICKHEIGHT     "MouseDoubleClickHeight"
#define DEF_MOUSEDOUBLECLICKHEIGHT      4
#define DEFSTR_MOUSEDOUBLECLICKHEIGHT  "4"

#define INI_MOUSEDOUBLECLICKWIDTH     "MouseDoubleClickWidth"
#define DEF_MOUSEDOUBLECLICKWIDTH      4
#define DEFSTR_MOUSEDOUBLECLICKWIDTH  "4"

#define INI_KEYBOARDTIMER   "KeyboardTimer"
#define DEF_KEYBOARDTIMER   50
#define DEFSTR_KEYBOARDTIMER "50"

#define INI_MRU_CONNECTION  "MRU Connection"
#define INI_MRU_NETWORK     "Network Connection"
#define INI_MRU_SERIAL      "Serial Connection"

#define INI_CURRENT_DIALPREFIX  "Current Dialing Prefix"
#define DEF_DIALPREFIX      "No Prefix"

#define INI_RUNTIMEPROMPT   "RuntimePrompt"
#define DEF_RUNTIMEPROMPT   TRUE

#define INI_HAVE16_WANT256      "ColorMismatchPrompt_Have16_Want256"
#define DEF_HAVE16_WANT256      FALSE
#define INI_HAVE64K_WANT256     "ColorMismatchPrompt_Have64K_Want256"
#define DEF_HAVE64K_WANT256     FALSE
#define INI_HAVE16M_WANT256     "ColorMismatchPrompt_Have16M_Want256"
#define DEF_HAVE16M_WANT256     FALSE

#define INI_ECHO_TTY_DOS    "DosConnectTTY"
#define INI_ECHO_TTY_WIN    "ConnectTTY"
#ifdef DOS
#define INI_ECHO_TTY        INI_ECHO_TTY_DOS
#define DEF_ECHO_TTY        TRUE
#else
#define INI_ECHO_TTY        INI_ECHO_TTY_WIN
#define DEF_ECHO_TTY        FALSE
#endif
#define INI_ECHO_TTY_DELAY  "ConnectTTYDelay"
#define DEF_ECHO_TTY_DELAY  1000

#define INI_SCRIPT_FILE           "ScriptFile"
#define INI_SCRIPT_DRIVER_DOS     "ScriptDriver"
#define INI_SCRIPT_DRIVER_WIN16   "ScriptDriverWin16"
#define INI_SCRIPT_DRIVER_WIN32   "ScriptDriverWin32"
#if defined(DOS) || defined(RISCOS)
#define INI_SCRIPT_DRIVER         INI_SCRIPT_DRIVER_DOS
#endif
#ifdef WIN16
#define INI_SCRIPT_DRIVER         INI_SCRIPT_DRIVER_WIN16
#endif
#ifdef WIN32
#define INI_SCRIPT_DRIVER         INI_SCRIPT_DRIVER_WIN32
#endif


/*=============================================================================
 ==  Serial configuration keys (INI_)
 ==     (wfclient.ini)
 ============================================================================*/

/*
 * section names
 */
#define INI_COMDEVICES      "Serial Ports"
#define INI_COMPORTS        "Windows COM Port Names"

#define INI_DIRECTCONNECT_VER1 "Direct Connect"
#define INI_DIRECTCONNECT    "(Direct Connect)"


/*=============================================================================
 ==  AppServer INI keys (INI_) and defaults (DEF_)
 ==     (appsrv.ini and .ica files)
 ============================================================================*/

/*
 * section names
 */
#define INI_APPSERVERLIST   "ApplicationServers"
#define INI_DEFAULT_COMMON  "Common Default Information"
#define INI_DEFAULT_NETWORK "Default Network Connection"
#define INI_PUBLISHEDAPP    "Published Application"
#define DEF_PUBLISHEDAPP    FALSE
#define INI_DEFAULT_SERIAL  "Default Serial Connection"
#define INI_DIALPREFIX      "Dialing Prefixes"

#define INI_DESCRIPTION     "Description"
#define DEF_DESCRIPTION     ""

#define INI_PROTOCOLDRIVER  "ProtocolDriver"

#define INI_DOMAIN          "Domain"
#define DEF_DOMAIN          ""
#define INI_USERNAME        "Username"
#define DEF_USERNAME        ""
#define INI_PASSWORD        "Password"
#define DEF_PASSWORD        ""

#define INI_AUTHENCRYPTIONLEVEL "AuthEncryptionLevel"
#define INI_DATAENCRYPTIONLEVEL "DataEncryptionLevel"

#define INI_CLIENTTYPE      "Client-Type"
#define INTERNET_CLIENT     "Internet"
#define WINDOWS_CLIENT      "Windows"

#define INI_WORKDIRECTORY   "WorkDirectory"
#define DEF_WORKDIRECTORY   ""
#define INI_INITIALPROGRAM  "InitialProgram"
#define DEF_INITIALPROGRAM  ""
#define INI_PROGRAMGROUP    "ProgramGroup"
#define DEF_PROGRAMGROUP    "WinFrame Client"
#define INI_ICONPATH        "IconPath"
#define DEF_ICONPATH        ""
#define INI_ICONINDEX       "IconIndex"
#define DEF_ICONINDEX       0

#define DEF_AUTH_ENCR       "Basic"
#define DEF_DATA_ENCR       "None"
#define INI_ENCRYPTION      "Encryption Levels"

#define INI_SCREENPERCENT   "ScreenPercent"
#define DEF_SCREENPERCENT   75


/*=============================================================================
 ==  Common driver defines (INI_)
 ==     (module.ini)
 ============================================================================*/

#if defined(DOS) || defined(RISCOS)
#define INI_DRIVERNAME      "DriverName"
#define INI_NAMEENUMERATOR  "NameEnumerator"
#define INI_NAMERESOLVER    "NameResolver"
#endif

#ifdef WIN16
#define INI_DRIVERNAME      "DriverNameWin16"
#define INI_NAMEENUMERATOR  "NameEnumeratorWin16"
#define INI_NAMERESOLVER    "NameResolverWin16"
#endif

#ifdef WIN32
#define INI_DRIVERNAME      "DriverNameWin32"
#define INI_NAMEENUMERATOR  "NameEnumeratorWin32"
#define INI_NAMERESOLVER    "NameResolverWin32"
#endif
#define INI_DRIVERUNSUPPORTED "Unsupported"


/*=============================================================================
 ==  Device Settings lists (UI) INI keys (INI_)
 ==     (module.ini)
 ============================================================================*/

#define INI_HOTKEY_SHIFTSTATES "Hotkey Shift States"
#define INI_HOTKEY_KEYS        "Hotkey Keys"
#ifdef WIN16
#define INI_BAUDRATES       "Baud Rates - WIN16"
#else
#define INI_BAUDRATES       "Baud Rates"
#endif
#define INI_DATALIST        "Data Bits"
#define INI_PARITYLIST      "Parity"
#define INI_STOPLIST        "Stop Bits"
#define INI_PORTNUMLIST     "Port Numbers"
#define INI_IOADDRLIST      "IO Addresses"
#define INI_IRQLIST         "Interrupts"
#define INI_HWRXLIST        "Hardware Receive Flow Control"
#define INI_HWTXLIST        "Hardware Transmit Flow Control"


/*=============================================================================
 ==   Transport Driver INI keys (INI_) and defaults (DEF_)
 ============================================================================*/

#define INI_TRANSPORTDRIVER "TransportDriver"
#define DEF_TRANSPORTDRIVER INI_SERIAL

#define INI_OUTBUFLENGTH      "OutBufLength"
#define DEF_OUTBUFLENGTH      512

#define INI_OUTBUFCOUNTHOST   "OutBufCountHost"
#define DEF_OUTBUFCOUNTHOST   6

#define INI_OUTBUFCOUNTCLIENT "OutBufCountClient"
#define DEF_OUTBUFCOUNTCLIENT 6

#define INI_ADDRESS           "Address"
#define DEF_ADDRESS           ""

#define INI_LANANUMBER        "LanaNumber"        // netbios lana number
#define DEF_LANANUMBER        0

#define INI_COMPRESSION       "Compress"

#define INI_ENCRYPTIONLEVELAUTH "EncryptionLevelAuth"
#define INI_ENCRYPTIONLEVELSESSION "EncryptionLevelSession"
#define INI_ENCRYPTIONDLL          "EncryptionDLL"
#define DEF_ENCRYPTIONLEVEL INI_ENCRYPTION_BASIC
#define INI_DRIVERNAMEWIN32 "DriverNameWin32"
#define INI_ENCRC5_0 "EncRC5-0"
#define INI_ENCRC5_40 "EncRC5-40"
#define INI_ENCRC5_56 "EncRC5-56"
#define INI_ENCRC5_128 "EncRC5-128"


// Encryption Levels 
#define     INI_ENCRYPTION_NONE     "None"
#define     INI_ENCRYPTION_BASIC    "Basic"
#define     INI_ENCRYPTION_RC5_40   "RC5 (40 bit)"
#define     INI_ENCRYPTION_RC5_128  "RC5 (128 bit - US only)"
#define     INI_ENCRYPTION_RC5_0    "RC5 (128 bit - Login Only)"

// TransportDriver names in INI file version 1
#define     INI_SERIAL_VER1       "Serial Port"
#define     INI_BIOSINT14_VER1    "Bios Int 14 Driver"
#define     INI_DIGIINT14_VER1    "Digiboard Int 14"
#define     INI_SSTINT14_VER1     "Equinox SuperSerial Int 14"
#define     INI_WINSOCK_IPX_VER1  "Microsoft WinSock IPX"
#define     INI_WINSOCK_SPX_VER1  "Microsoft WinSock SPX"
#define     INI_WINSOCK_NETB_VER1 "Microsoft WinSock NetBIOS"
#define     INI_WINSOCK_TCP_VER1  "Microsoft WinSock TCP/IP"
#define     INI_NETBIOS_VER1      "NetBIOS"
#define     INI_IPX_VER1          "Novell IPX"
#define     INI_SPX_VER1          "Novell SPX"
#define     INI_TCP_FTP_VER1      "FTP TCP/IP"
#define     INI_TCP_NOV_VER1      "Novell Lan WorkPlace TCP/IP"
#define     INI_TCP_MS_VER1       "Microsoft TCP/IP"
#define     INI_TCP_VSL_VER1      "VSL TCP/IP"

// TransportDriver names in INI file version 2
#define     INI_SERIAL     "Standard COM Port"
#define     INI_BIOSINT14  "Int 14 - Bios"
#define     INI_DIGIINT14  "Int 14 - Digiboard"
#define     INI_SSTINT14   "Int 14 - Equinox SuperSerial"
#define     INI_NETBIOS    "NETBIOS"
#define     INI_IPX        "IPX"
#define     INI_SPX        "SPX"
#define     INI_TCP        "TCP/IP"
#define     INI_TCP_FTP    "TCP/IP - FTP"
#define     INI_TCP_NOV    "TCP/IP - Novell Lan WorkPlace"
#define     INI_TCP_MS     "TCP/IP - Microsoft"
#define     INI_TCP_VSL    "TCP/IP - VSL"

/*
 *  Serial
 */
#define     INI_PORTNUMBER "PortNumber"
#define     DEF_PORTNUMBER 1
#define     INI_PORTNAME   "PortName"
#define     DEF_PORTNAME   "COM1"
#define     INI_DEVICE     "DeviceName"
#define     DEF_DEVICE     "COM1"
#define     INI_BAUD       "BaudRate"
#define     DEF_BAUD       38400
#define     DEFSTR_BAUD    "38400"
#define     INI_STOP       "StopBits"
#define     DEF_STOP       1
#define     DEFSTR_STOP    "1"
#define     INI_PARITY     "Parity"
#define     DEF_PARITY     "None"
#define     INI_DATA       "DataBits"
#define     DEF_DATA       8
#define     DEFSTR_DATA    "8"
#define     INI_IRQ        "Interrupt"
#define     DEF_IRQ        "Default"
#define     INI_IOADDR     "IOBase"
#define     DEF_IOADDR     "Default"
#define     INI_HW_RX      "ReceiveFlowControl"
#define     DEF_HW_RX      "RTS"
#define     INI_HW_TX      "TransmitFlowControl"
#define     DEF_HW_TX      "CTS"
#define     INI_SW         "SoftwareFlowControl"
#define     DEF_SW         FALSE
#define     INI_HW         "HardwareFlowControl"
#define     DEF_HW         TRUE
#define     INI_XON        "XonChar"
#define     DEF_XON        0x11
#define     DEFSTR_XON     "0x11"
#define     INI_XOFF       "XoffChar"
#define     DEF_XOFF       0x13
#define     DEFSTR_XOFF    "0x13"
#define     INI_DTR        "DTR"
#define     DEF_DTR        TRUE
#define     INI_RTS        "RTS"
#define     DEF_RTS        FALSE
#define     INI_MODEMTYPE  "ModemType"
#define     DEF_MODEMTYPE  ""
#define     INI_MODEM      "Modem"
#define     DEF_MODEM      "Off"
#define     INI_USEDEVICE  "UseSerialDevice"
#define     DEF_USEDEVICE  "No"

#define     INI_CTS         "CTS"
#define     INI_DSR         "DSR"
#define     INI_PARITYNONE  "None"
#define     INI_PARITYEVEN  "Even"
#define     INI_PARITYMARK  "Mark"
#define     INI_PARITYODD   "Odd"
#define     INI_PARITYSPACE "Space"

#define     INI_INQUEUE     "InQueue"
#define     DEF_INQUEUE     1024
#define     INI_OUTQUEUE    "OutQueue"
#define     DEF_OUTQUEUE    1024
#define     INI_XONLIM      "XonLim"
#define     DEF_XONLIM      20          // 20% empty send xon
#define     INI_XOFFLIM     "XoffLim"
#define     DEF_XOFFLIM     80          // 80% full send xoff


/*=============================================================================
 ==   Protocol Driver INI keys (INI_) and defaults (DEF_)
 ============================================================================*/

/*
 * pdcomp.dll
 */

#define INI_MAXCOMPRESSDISABLE "MaxCompressDisable"
#define DEF_MAXCOMPRESSDISABLE  50

/*
 *  pdreli.dll
 */

#define INI_MAXRETRYTIME    "MaxRetryTime"
#define DEF_MAXRETRYTIME    60000                   // 1 minute

#define INI_RETRANSMITDELTA "RetransmitTimeDelta"

#define INI_DELAYEDACK      "DelayedAckTime"
#define DEF_DELAYEDACK      200                     // 1/5 second

/*
 *  pdmodem.ini
 */
#define INI_MODEMLIST           "Modems"

#define INI_PHONENUMBER         "PhoneNumber"
#define INI_DIALTIMEOUT         "DialTimeout"
#define DEF_DIALTIMEOUT         60
#define INI_DIALRETRY           "DialRetries"
#define DEF_DIALRETRY           15
#define INI_MODEMINIT           "COMMAND_INIT"
#define DEF_MODEMINIT           "AT&F^M"
#define INI_MODEMDIAL           "COMMAND_DIAL"
#define DEF_MODEMDIAL           "ATDT^M"
#define INI_MODEMHANGUP         "COMMAND_HANGUP"
#define DEF_MODEMHANGUP         "~~~+++~~~ATH0^M"
#define INI_MODEMLISTEN         "COMMAND_LISTEN"
#define DEF_MODEMLISTEN         "ATS0=1^M"
#define INI_MODEMCONNECT        "CONNECT"
#define DEF_MODEMCONNECT        "CONNECT"

#define     INI_MERRORCORRECT     "PROTOCOL"
#define     INI_MERRORCORRECTON   "<protocol_on>"
#define     INI_MERRORCORRECTOFF  "<protocol_off>"
#define     INI_MCOMPRESSION      "COMPRESSION"
#define     INI_MCOMPRESSIONON    "<compression_on>"
#define     INI_MCOMPRESSIONOFF   "<compression_off>"
#define     INI_MSPEAKER          "SPEAKER"
#define     INI_MSPEAKERON        "<speaker_on>"
#define     INI_MSPEAKEROFF       "<speaker_off>"
#define     INI_MHWFLOWCONTROL    "HWFLOWCONTROL"
#define     INI_MHWFLOWCONTROLON  "<hwflowcontrol_on>"
#define     INI_MHWFLOWCONTROLOFF "<hwflowcontrol_off>"
#define     INI_MAUTODIAL         "autodial"
#define     INI_MAUTODIALON       "<autodial_on>"
#define     INI_MAUTODIALOFF      "<autodial_off>"
#define     INI_MALIAS            "ALIAS"



/*=============================================================================
 ==   WinStation Driver INI keys (INI_) and defaults (DEF_)
 =============================================================================*/

#define INI_TYPE            "WinStationDriver"
#define DEF_TYPE            INI_ICA30

#define INI_ICA30           "ICA 3.0"
#define INI_ICA30TEXT       "ICA 3.0 (Text Only)"
#define INI_ICA20           "ICA 2.0"
#define INI_ICA20TEXT       "ICA 2.0 (Text Only)"
#define INI_TTY             "Generic TTY"


#define INI_XMSRESERVE      "XmsReserve"
#define DEF_XMSRESERVE      0
#define INI_LOWMEMRESERVE   "LowMemReserve"
#define DEF_LOWMEMRESERVE   51200                   // 50K   BUGBUG
#define INI_BUFFERLENGTH    "BufferLength"
#define DEF_BUFFERLENGTH    2048


/*
 *  wdica20.dll
 */

// number of consecutive error retries before aborting
#define INI_MAXRETRYCOUNT   "MaxRetryCount"
#define DEF_MAXRETRYCOUNT   20

// time host will wait for ack after needing a buffer to send more stuff
// this is sent to host in 10ths of seconds rather than msec
#define INI_ACKTIMEOUT      "Transmit Timeout"
#define DEF_ACKTIMEOUT      2000

// time host will wait for good stuff after sending a NAK
// this is sent to host in 10ths of seconds rather than msec
#define INI_NAKTIMEOUT      "NAK Timeout"
#define DEF_NAKTIMEOUT      1500

// time client will wait prior to sending an ack after receiving duplicate data
//#define INI_DELAYEDACK      "DelayedAckTime"
//#define DEF_DELAYEDACK      200

// On to turn on reliable, Off to keep it off
#define INI_RELIABLE        "ErrorCorrection"
#define DEF_RELIABLE        "Off"

// default values for things sent to host that are not gotten from .ini
#define DEF_WATCHDOG          60
#define DEF_RCVTIMEOUT        5


/*=============================================================================
 ==   Virtual Driver INI keys (INI_) and defaults (DEF_)
 ============================================================================*/

/*
 *  vdtw30.dll
 */
#define INI_VDTW30        "Thinwire3.0"
#define INI_MINSMALLCACHE "MinSpecialCache16Color"
#define INI_MAXSMALLCACHE "MaxSpecialCache16Color"
#define INI_MAXLARGECACHE "MaxCache16Color"
#define INI_MAXMEMCACHE   "MaxMemoryCache"
#define INI_MINMEMCACHE   "MinMemoryCache"
#define INI_MAXDASDCACHE  "MaxDiskCache"
#define INI_MINDASDLEFT   "MinDiskLeft"
#define INI_DIRNAME       "DiskCacheDirectory"
#define INI_SVGACAPABILITY "SVGACapability"
#define INI_SVGAPREFERENCE "SVGAPreference"
#define INI_DESIREDHRES    "DesiredHRES"
#define DEF_DESIREDHRES    640
#define MAX_DESIREDHRES    2048
#define INI_DESIREDVRES    "DesiredVRES"
#define DEF_DESIREDVRES    480
#define MAX_DESIREDVRES    2048
#define INI_AUTO           "Auto"
#define INI_LARGECACHE    "WindowsCache"
#define DEF_LARGECACHE    3072
#define INI_CLICKTICKS    "ClickTicks"
#define DEF_CLICKTICKS    5

//  256 or greater color supported on windows clients only
#define INI_DESIREDCOLOR  "DesiredColor"
#define DEF_DESIREDCOLOR  0x0001            //  16 color

//  persistent cache (DIM) 
#define INI_DIMCACHEENABLED     "PersistentCacheEnabled"
#define DEF_DIMCACHEENABLED     0
#define INI_DIMCACHESIZE        "PersistentCacheSize"
#define DEF_DIMCACHESIZE        0
#define INI_DIMCACHEPERCENT_UI  "PersistentCachePercent"
#define DEF_DIMCACHEPERCENT_UI  3       // (UI) default to 3% of disk
#define INI_DIMMINBITMAP        "PersistentCacheMinBitmap"
#define DEF_DIMMINBITMAP        0
#define RES_DIMMINBITMAP_UI     1024    // (UI) each UI tick is 1K bytes
#define MIN_DIMMINBITMAP_UI     2       // (UI) minumum 2K
#define MAX_DIMMINBITMAP_UI     64      // (UI) maximum 64K
#define INCR_DIMMINBITMAP_UI    2       // (UI) in increments of 2K
#ifdef WIN16
#define INIT_DIMMINBITMAP_UI    8       // (UI) initially 8K (WIN16)
#endif
#ifdef WIN32
#define INIT_DIMMINBITMAP_UI    8       // (UI) initially 8K (WIN32)
#endif
#define INI_DIMCACHEPATH        "PersistentCachePath"
#define DEF_DIMCACHEPATH        "Cache"


/*
 * vdcom30.dll
 */
#define INI_CCMWINDOWSIZE      "WindowSize"
#define DEF_CCMWINDOWSIZE      1024


/*
 *  vdcdm30.dll
 */

#define INI_MAXWINDOWSIZE      "MaxWindowSize"
#define DEF_MAXWINDOWSIZE      6276
#define INI_MAXREQUESTSIZE     "MaxRequestSize"
#define DEF_MAXREQUESTSIZE     1046 // This includes a 2 byte per entry ringbuf overhead

/*
 * vdcdm30.dll - cache parameters
 *
 * CacheTimeout allows setting up to an 18 hour timeout.
 * CacheTimeoutHigh is not documented to users but can allow longer
 * timeouts for special situations.
 */
#define INI_CACHETIMEOUT0      "CacheTimeout"
#define DEF_CACHETIMEOUT0      600

#define INI_CACHETIMEOUT1      "CacheTimeoutHigh"
#define DEF_CACHETIMEOUT1      0

#define INI_CACHEXFERSIZE      "CacheTransferSize"
#define DEF_CACHEXFERSIZE      0   // 0 means ICA buffer size

#define INI_NOCACHE            "CacheDisable"
#define DEF_NOCACHE            FALSE

#define INI_NOWRITEALLOC       "CacheWriteAllocateDisable"
#define DEF_NOWRITEALLOC       FALSE

/*
 *  vdcpm30.dll
 */

#define INI_CPMMAXWINDOWSIZE "MaxWindowSize"
#define DEF_CPMMAXWINDOWSIZE 1024

#define INI_CPMWINDOWSIZE   "WindowSize"
#define DEF_CPMWINDOWSIZE    512

#define INI_CPMDIRECTPORT   "UseDirectPort"
#define DEF_CPMDIRECTPORT   FALSE

#define INI_CPMQUEUE        "WindowsPrinter"
#define DEF_CPMQUEUE        ""


/*=============================================================================
 ==  Dialing Properties INI keys (INI_) and defaults (DEF_)
 ============================================================================*/

#define INI_DIALPROP            "DialingProperties"
#define INI_CURRENTLOCATION     "CurrentLocation"
#define DEF_CURRENTLOCATION     "Default Location"
#define INI_AREACODE            "AreaCode"
#define DEF_AREACODE            ""
#define INI_COUNTRY             "Country"
#define DEF_COUNTRY             "United States of America (1)"
#define INI_LOCALACCESS         "LocalAccess"
#define DEF_LOCALACCESS         ""
#define INI_LONGDISTANCEACCESS  "LongDistanceAccess"
#define DEF_LONGDISTANCEACCESS  ""
#define INI_CALLINGCARD         "CallingCard"
#define DEF_CALLINGCARD         ""
#define INI_DISABLECALLWAITING  "DisableCallWaiting"
#define DEF_DISABLECALLWAITING  ""
#define INI_TONEDIALING         "ToneDialing"
#define DEF_TONEDIALING         TRUE


/*=============================================================================
 ==   UI suppliers must use user-defined messages above WM_WFENG_USER
 ============================================================================*/
#define WM_WFENG_USER (WM_USER+1000)


/*=============================================================================
 ==   WFENG dll entry points
 ============================================================================*/
//#define WFENG_API_VERSION 1L        // WinFrame 1.54+
//#define WFENG_API_VERSION 2L        // add serial number to wf 1.5 (build 158)
//#define WFENG_API_VERSION 3L        // WinFrame Client 1.50.67+
#define WFENG_API_VERSION 4L          // WinFrame Internet Client

typedef VOID (PWFCAPI PFNUIPOLL)( VOID );
typedef LRESULT (PWFCAPI PFNSTATUSMSGPROC)( HANDLE hWFE,
					    INT message,
					    LPARAM lParam );

/*
 * Default INI file settings structures
 */
typedef struct _DEFINISECT {
   char *pSectName;
   char *pSectDefault;
} DEFINISECT, *PDEFINISECT;

typedef struct _DEFINIFILE {
   char *pFileName;
   DEFINISECT *pINISect;
} DEFINIFILE, *PDEFINIFILE;

/*
 * Data block for WFEngOpen()
 */
typedef struct _WFEOPEN {
   ULONG              Version;           // ALL API version - set to WFENG_API_VERSION
   PFNSTATUSMSGPROC   pfnStatusMsgProc;  // ALL StatusMsgProc() function
   LPSTR              pszClientname;     // ALL
#ifdef DOS
   PFNUIPOLL          pfnUIPoll;         // DOS UIPoll() function
#else
   USHORT             uInitialWidth;     // WIN (optional) Initial window width
   USHORT             uInitialHeight;    // WIN (optional) Initial window height
   LPSTR              pszLogLabel;       // WIN (optional) Connection identifier string
   LPSTR              pszTitle;          // WIN (optional) Window Title
#endif
   // below is for API Version 2 and above
   LPSTR              pszClientSN;       // can be null pointer
   // below is for API Version 3 and above
#ifndef DOS
   LPSTR              pszIconPath;       // WIN (optional) Icon Path
   USHORT             uIconIndex;        // WIN (optional) Icon Index
#endif
   // below is for API Version 4 and above
   ULONG              reserved;          // Reserved - must be 0
} WFEOPEN, FAR *PWFEOPEN;

/*
 * Data block for WFEngLoadxxx()
 */
typedef struct _WFELOAD {
   HANDLE handle;        // Output - handle to loaded module
   LPSTR  pszModuleName; // List of FQ UI modules ('\0\0' terminated for WFEngLoadSession)
   LPVOID pIniSection;
} WFELOAD, FAR *PWFELOAD;

/*
 *  WFEngSetInformation()/WFEngQueryInformation() enum
 */
typedef enum _WFEINFOCLASS {
    WFELastError,       // Query
    WFEAddReadHook,     // Set
    WFERemoveReadHook,  // Set
    WFEThinwireCache,   // Query
    WFEWindowHandle,    // Query
    WFESetFocus,        // Set
    WFEKillFocus,       // Set
    WFEVdInfo,          // Set & Query
    WFELogInfo,         // Set
    WFEHotkey,          // Set
    WFESetDefaultMode,  // Set
    WFESetProductID,    // Set
    WFERaiseSoftkey,    // Set
    WFELowerSoftkey,    // Set
    WFEFileSecurity,    // Set
    WFEClientDataServer,    // Query
    WFEClientDataDomain,    // Query
    WFEClientDataUsername  // Query
} WFEINFOCLASS;

/*
 * Data block for WFESetProductID
 */
typedef struct _WFEPRODUCTID {
    USHORT  ProductID;
} WFEPRODUCTID, * PWFEPRODUCTID;

/*
 * Data block for WFESetClientInfo
 */
typedef struct _WFEDEFAULTMODE {
#define SET_DEFAULT_TEXT        0
#define SET_DEFAULT_NOT_TEXT    1
    USHORT  DefaultMode;
} WFEDEFAULTMODE, * PWFEDEFAULTMODE;

/*
 * Data block for WFEVdInfo via WFEngSetInformation/QueryInformation
 */
typedef struct _WFEVDINFO {
    HANDLE hVd;                  // Vd handle returned from WFEngLoadVd
    ULONG  type;                 // Vd-specific info type
    UINT   cbData;               // size (in bytes) of pData
    UCHAR  pData[1];             // Info buffer - Set (input), Query (output)
} WFEVDINFO, * PWFEVDINFO;


// Thinwire VD setinfo type
#define VdThinWireDeallocateCache   201
#define VdThinWireSaveVideoRegs     202
#define VdThinWireRestoreVideoRegs  203


/*
 * Data block for WFEngSetInformation-WFEHotKey
 */
typedef struct _WFEHOTKEY {
    int    hotkey;               //Hotkey status value (CLIENT_STATUS_HOTKEYx )
    int    ShiftState;           //Hotkey shift state ( KSS_xxx flags )
    int    ScanCode;             //Number of hotkeys being set
} WFEHOTKEY, * PWFEHOTKEY;

typedef struct _WFEHOTKEYINFO {
    int       cHotkeys;         //Number of hotkeys being set
    WFEHOTKEY pHotkeys[1];      //Array of hotkeys
} WFEHOTKEYINFO, * PWFEHOTKEYINFO;

/*
 * Data block for WFEngSetInformation-WFELogInfo
 */
typedef struct _WFELOGINFO {
    LPSTR  pszLogFile;
    ULONG  ulLogClass;
    ULONG  ulLogEvent;
    ULONG  ulLogFlags;
} WFELOGINFO, * PWFELOGINFO;

/*
 *  Data Block for WFEngQueryInformation() - WFELastError
 */
#define MAX_ERRORMESSAGE    256

typedef struct _WFELASTERROR {
    DOSFILENAME DriverName;
    int Error;
    char Message[ MAX_ERRORMESSAGE ];
} WFELASTERROR, * PWFELASTERROR;


#ifndef DOS
#ifndef WFENGINE
#define WFENGINE
#endif
#endif

struct _MINIDLL;
typedef int (PWFCAPI PFNLOGPROC)( );
typedef int (PWFCAPI PFNUNLOAD)( struct _MINIDLL * );


typedef struct _MINIDLL {
#ifdef DOS
// NOTE: 1st six elements must be same as DLLLINK
    USHORT Segment;                 // starting seg of mem allocated for dll (aligned) (output)
    USHORT DllSize;                 // actual size of dll in paragraphs
    USHORT ProcCount;               // number of procedures in call table
    PVOID  pProcedures;             // pointer to procedure call table
    PVOID  pData;                   // pointer to dll data structure
    PUCHAR pMemory;                 // pointer to malloced memory (non-aligned)
    LPSTR  pszDllPath;              // for internal use only - can be ignored
    LPVOID pClibProcedures;         // Pointer to Clib functions (output)
    LPVOID pVioProcedures;          // Pointer to Vio functions (output)
    LPVOID pKbdProcedures;          // Pointer to Kbd functions (output)
    LPVOID pMouProcedures;          // Pointer to Mou functions (output)
    LPVOID pXmsProcedures;          // Pointer to Xms functions (output)
    LPVOID pBIniProcedures;         // Pointer to BIni functions (output)
    LPVOID pModuleProcedures;       // Pointer to full module functions (output)
    PFNLOGPROC * ppfnLogProcedures; // List of Logxxx() functions (input)
    PFNUNLOAD pfnUnload;            // Unload procedure (output)
#else
    ULONG  reserved;                // Reserved - must be 0
#endif
} MINIDLL, * PMINIDLL;

typedef int (PWFCAPI PFNMINILOAD  )( char *, PMINIDLL );
typedef int (PWFCAPI PFNMINIUNLOAD)( PMINIDLL );

int WFCAPI MiniLoad( char *, PMINIDLL );
int WFCAPI MiniUnload( PMINIDLL );

extern PFNMINILOAD   pfnMiniLoad;
extern PFNMINIUNLOAD pfnMiniUnload;

typedef INT (WFCAPI FNWFENGOPEN)( PWFEOPEN pWFEOpen, LPHANDLE phWFE );
typedef FNWFENGOPEN far * PFNWFENGOPEN;
typedef INT (WFCAPI FNWFENGCLOSE)( HANDLE hWFE );
typedef FNWFENGCLOSE far * PFNWFENGCLOSE;
typedef INT (WFCAPI FNWFENGCONNECT)( HANDLE hWFE );
typedef FNWFENGCONNECT far * PFNWFENGCONNECT;
typedef INT (WFCAPI FNWFENGDISCONNECT)( HANDLE hWFE );
typedef FNWFENGDISCONNECT far * PFNWFENGDISCONNECT;
typedef INT (WFCAPI FNWFENGLOADSESSION)( HANDLE hWFE, PWFELOAD pWFELoad );
typedef FNWFENGLOADSESSION far * PFNWFENGLOADSESSION;
typedef INT (WFCAPI FNWFENGLOADWD)( HANDLE hWFE, PWFELOAD pWFELoad );
typedef FNWFENGLOADWD far * PFNWFENGLOADWD;
typedef INT (WFCAPI FNWFENGLOADVD)( HANDLE hWFE, PWFELOAD pWFELoad );
typedef FNWFENGLOADVD far * PFNWFENGLOADVD;
typedef INT (WFCAPI FNWFENGLOADPD)( HANDLE hWFE, PWFELOAD pWFELoad );
typedef FNWFENGLOADPD far * PFNWFENGLOADPD;
typedef INT (WFCAPI FNWFENGUNLOADDRIVERS)( HANDLE hWFE );
typedef FNWFENGUNLOADDRIVERS far * PFNWFENGUNLOADDRIVERS;
typedef INT (WFCAPI FNWFENGSETINFORMATION)( HANDLE hWFE, WFEINFOCLASS type,
					    LPVOID pData, UINT cbData );
typedef FNWFENGSETINFORMATION far * PFNWFENGSETINFORMATION;
typedef INT (WFCAPI FNWFENGQUERYINFORMATION)( HANDLE hWFE, WFEINFOCLASS type,
					      LPVOID pData, UINT cbData );
typedef FNWFENGQUERYINFORMATION far * PFNWFENGQUERYINFORMATION;
typedef INT (WFCAPI FNWFENGPOLL)( HANDLE hWFE );
typedef FNWFENGPOLL far * PFNWFENGPOLL;
typedef LRESULT (WFCAPI FNSTATUSMESSAGEPROC)( HANDLE hWFE, int message );
typedef FNSTATUSMESSAGEPROC far * PFNSTATUSMESSAGEPROC;
typedef INT (WFCAPI FNWFENGMESSAGELOOP)( HANDLE hWFE );
typedef FNWFENGMESSAGELOOP far * PFNWFENGMESSAGELOOP;
typedef INT (WFCAPI FNWFENGLOAD)( LPSTR pszDllPath, PMINIDLL pLink );
typedef FNWFENGLOAD far * PFNWFENGLOAD;
typedef INT (WFCAPI FNWFENGUNLOAD)( PMINIDLL pLink );
typedef FNWFENGUNLOAD far * PFNWFENGUNLOAD;
typedef INT (WFCAPI FNWFENGLOGSTRING)( HANDLE hWFE, ULONG LogClass,
				       ULONG LogEvent, LPSTR pszString );
typedef FNWFENGLOGSTRING far * PFNWFENGLOGSTRING;

FNWFENGLOAD              WFEngLoad;
FNWFENGUNLOAD            WFEngUnload;
#ifdef WFENGINE
FNWFENGOPEN              WFEngOpen;
FNWFENGCLOSE             WFEngClose;
FNWFENGCONNECT           WFEngConnect;
FNWFENGDISCONNECT        WFEngDisconnect;
FNWFENGLOADSESSION       WFEngLoadSession;
FNWFENGLOADWD            WFEngLoadWd;
FNWFENGLOADVD            WFEngLoadVd;
FNWFENGLOADPD            WFEngLoadPd;
FNWFENGUNLOADDRIVERS     WFEngUnloadDrivers;
FNWFENGSETINFORMATION    WFEngSetInformation;
FNWFENGQUERYINFORMATION  WFEngQueryInformation;
FNWFENGPOLL              WFEngPoll;
FNWFENGMESSAGELOOP       WFEngMessageLoop;
FNWFENGLOGSTRING         WFEngLogString;
#else
extern PPLIBPROCEDURE pWFEngProcedures;

#define WFENGINE$LOAD             0
#define WFENGINE$UNLOAD           1
#define WFENGINE$OPEN             2
#define WFENGINE$CLOSE            3
#define WFENGINE$LOADWD           4
#define WFENGINE$LOADPD           5
#define WFENGINE$QUERYINFORMATION 6
#define WFENGINE$SETINFORMATION   7
#define WFENGINE$LOADVD           8
#define WFENGINE$LOADSESSION      9
#define WFENGINE$CONNECT         10
#define WFENGINE$DISCONNECT      11
#define WFENGINE$POLL            12
#define WFENGINE$MESSAGELOOP     13
#define WFENGINE$LOGSTRING       14

#define WFEngUnloadDrivers    ((PFNWFENGUNLOADDRIVERS)(pWFEngProcedures[WFENGINE$UNLOAD]))
#define WFEngOpen             ((PFNWFENGOPEN)(pWFEngProcedures[WFENGINE$OPEN]))
#define WFEngClose            ((PFNWFENGCLOSE)(pWFEngProcedures[WFENGINE$CLOSE]))
#define WFEngLoadWd           ((PFNWFENGLOADWD)(pWFEngProcedures[WFENGINE$LOADWD]))
#define WFEngLoadPd           ((PFNWFENGLOADPD)(pWFEngProcedures[WFENGINE$LOADPD]))
#define WFEngQueryInformation ((PFNWFENGQUERYINFORMATION)(pWFEngProcedures[WFENGINE$QUERYINFORMATION]))
#define WFEngSetInformation   ((PFNWFENGSETINFORMATION)(pWFEngProcedures[WFENGINE$SETINFORMATION]))
#define WFEngLoadVd           ((PFNWFENGLOADVD)(pWFEngProcedures[WFENGINE$LOADVD]))
#define WFEngLoadSession      ((PFNWFENGLOADSESSION)(pWFEngProcedures[WFENGINE$LOADSESSION]))
#define WFEngConnect          ((PFNWFENGCONNECT)(pWFEngProcedures[WFENGINE$CONNECT]))
#define WFEngDisconnect       ((PFNWFENGDISCONNECT)(pWFEngProcedures[WFENGINE$DISCONNECT]))
#define WFEngPoll             ((PFNWFENGPOLL)(pWFEngProcedures[WFENGINE$POLL]))
#define WFEngMessageLoop      ((PFNWFENGMESSAGELOOP)(pWFEngProcedures[WFENGINE$MESSAGELOOP]))
#define WFEngLogString        ((PFNWFENGLOGSTRING)(pWFEngProcedures[WFENGINE$LOGSTRING]))

#endif

/* #pragma pack() */

#ifdef __cplusplus
}
#endif

#endif //__WFENGAPI_H__
