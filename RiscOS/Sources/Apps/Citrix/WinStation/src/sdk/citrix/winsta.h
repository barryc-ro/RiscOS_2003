
/***************************************************************************
*
*  WINSTA.H
*
*  This module contains external window station defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen
*
*  $Log$
*  
*     Rev 1.275   22 Aug 1997 20:15:08   BillG
*  update
*  
*     Rev 1.273   21 Aug 1997 16:22:10   miked
*  Change get/set ini mapping from WinStation API to NT Base API
*  
*     Rev 1.272   21 Aug 1997 15:29:02   BillG
*  update
*  
*     Rev 1.271   31 Jul 1997 15:20:00   butchd
*  Added fDisableCam
*  
*     Rev 1.270   17 Jul 1997 12:32:54   JohnR
*  sp3merge
*  
*     Rev 1.269   17 Jul 1997 12:28:58   JohnR
*  sp3merge
*  
*     Rev 1.268   15 Jul 1997 16:11:04   miked
*  Add support for WinStation Extension DLLs
*  
*     Rev 1.267   15 Jul 1997 15:43:44   bradp
*  update
*  
*     Rev 1.266   11 Jul 1997 17:11:26   TOMA
*  videodriver added
*
*     Rev 1.265   09 Jul 1997 13:58:12   bradp
*  MS requested Changes
*
*     Rev 1.264   27 Jun 1997 15:41:38   butchd
*  New Wds/Pds/Tds registry tree structure
*
*     Rev 1.263   26 Jun 1997 20:02:20   BillG
*  add audio
*
*     Rev 1.261   20 Jun 1997 11:20:40   butchd
*  update
*
*     Rev 1.260   20 Jun 1997 10:14:40   butchd
*  update
*
*     Rev 1.259   19 Jun 1997 16:34:06   butchd
*  update
*
*     Rev 1.258   19 Jun 1997 15:11:56   butchd
*  update
*
*     Rev 1.257   19 Jun 1997 14:31:46   butchd
*  update
*
****************************************************************************/

#ifndef _INC_WINSTAH
#define _INC_WINSTAH

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINAPI
#define WINAPI      __stdcall
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#include <citrix\hydrix.h>
#include <citrix\ctxdef.h>

/***********
 *  Defines
 ***********/

#define PROTOCOL_ICA             1    // ICA Protocol
#define PROTOCOL_TSHARE          2    // T.Share Protocol


#define PDNAME_LENGTH            32
#define WDNAME_LENGTH            32
#define CDNAME_LENGTH            32
#define DEVICENAME_LENGTH        128
#define MODEMNAME_LENGTH         DEVICENAME_LENGTH
#define CALLBACK_LENGTH          50
#define LICENSE_PASSWORD_LENGTH  16
#define DLLNAME_LENGTH           32
#define PRINTERNAME_LENGTH       32
#define WINSTATIONCOMMENT_LENGTH 60
#define APPSERVERNAME_LENGTH     17

#define NASISPECIFICNAME_LENGTH    14
#define NASIUSERNAME_LENGTH        47
#define NASIPASSWORD_LENGTH        24
#define NASISESSIONNAME_LENGTH     16
#define NASIFILESERVER_LENGTH      47

#define LOGONID_CURRENT     ((ULONG)-1)
#define LOGONID_NONE        ((ULONG)-2)
#define SERVERNAME_CURRENT  ((HANDLE)NULL)

#define MAX_PDCONFIG             10  // maximum number of PDs per WinStation
#define MAX_UI_MODULES           5   // maximum client user interface modules
#define PSZ_ANONYMOUS           TEXT("Anonymous")

// BUGBUG: Should we still be using these #defines and related code in Hydrix?
/*
 *  Registry keys and versions for Citrix Icons
 *  -- if "windows\setup\bom\citrix\lminfs\upgrade.inf" changes increment version
 */
#define UPGRADE_KEY           TEXT("Software\\Citrix\\UPGRADE")
#define UPGRADE_ICON_VERSION  TEXT("IconVersion")
#define CURRENT_ICON_VERSION  2

#define WINFRAME_SOFTKEY_CLASS L"WinFrameSoftkey"
#define WINFRAME_SOFTKEY_APPLICATION L"wfskey.exe"

#define OEM_ID_LENGTH                        3
// End BUGBUG ----------------------------------------------------------------

/*********************************
 *   WinStationOpen access values
 *********************************/
#define WINSTATION_QUERY        0x00000001  // WinStationQueryInformation()
#define WINSTATION_SET          0x00000002  // WinStationSetInformation()
#define WINSTATION_RESET        0x00000004  // WinStationReset()
#define WINSTATION_VIRTUAL      0x00000008  // read/write direct data
#define WINSTATION_SHADOW       0x00000010  // WinStationShadow()
#define WINSTATION_LOGON        0x00000020  // logon to WinStation
#define WINSTATION_LOGOFF       0x00000040  // WinStationLogoff()
#define WINSTATION_MSG          0x00000080  // WinStationMsg()
#define WINSTATION_CONNECT      0x00000100  // WinStationConnect()
#define WINSTATION_DISCONNECT   0x00000200  // WinStationDisconnect()

#define WINSTATION_GUEST_ACCESS (WINSTATION_LOGON)

#define WINSTATION_CURRENT_GUEST_ACCESS (WINSTATION_VIRTUAL | WINSTATION_LOGOFF)

#define WINSTATION_USER_ACCESS (WINSTATION_GUEST_ACCESS |                      \
                                WINSTATION_QUERY | WINSTATION_MSG |            \
                                WINSTATION_CONNECT )

#define WINSTATION_CURRENT_USER_ACCESS (WINSTATION_SET | WINSTATION_RESET      \
                                        WINSTATION_VIRTUAL | WINSTATION_LOGOFF \
                                        WINSTATION_DISCONNECT)

#define WINSTATION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | WINSTATION_QUERY |   \
                                WINSTATION_SET | WINSTATION_RESET |            \
                                WINSTATION_VIRTUAL |    WINSTATION_SHADOW |    \
                                WINSTATION_LOGON | WINSTATION_LOGOFF |         \
                                WINSTATION_MSG |                               \
                                WINSTATION_CONNECT | WINSTATION_DISCONNECT)


/************
 *  Typedefs
 ************/

#define LOGONIDW SESSIONIDW      //externalized LogonID as SessionID in ctxapi.h
#define PLOGONIDW PSESSIONIDW
#define LOGONIDA SESSIONIDA
#define PLOGONIDA PSESSIONIDA
#ifdef UNICODE
#define LOGONID LOGONIDW
#define PLOGONID PLOGONIDW
#else
#define LOGONID LOGONIDA
#define PLOGONID PLOGONIDA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR PDNAMEW[ PDNAME_LENGTH + 1 ];
typedef WCHAR * PPDNAMEW;

typedef CHAR PDNAMEA[ PDNAME_LENGTH + 1 ];
typedef CHAR * PPDNAMEA;

#ifdef UNICODE
#define PDNAME PDNAMEW
#define PPDNAME PPDNAMEW
#else
#define PDNAME PDNAMEA
#define PPDNAME PPDNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR WDNAMEW[ WDNAME_LENGTH + 1 ];
typedef WCHAR * PWDNAMEW;

typedef CHAR WDNAMEA[ WDNAME_LENGTH + 1 ];
typedef CHAR * PWDNAMEA;

#ifdef UNICODE
#define WDNAME WDNAMEW
#define PWDNAME PWDNAMEW
#else
#define WDNAME WDNAMEA
#define PWDNAME PWDNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR CDNAMEW[ CDNAME_LENGTH + 1 ];
typedef WCHAR * PCDNAMEW;

typedef CHAR CDNAMEA[ CDNAME_LENGTH + 1 ];
typedef CHAR * PCDNAMEA;

#ifdef UNICODE
#define CDNAME CDNAMEW
#define PCDNAME PCDNAMEW
#else
#define CDNAME CDNAMEA
#define PCDNAME PCDNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR DEVICENAMEW[ DEVICENAME_LENGTH + 1 ];
typedef WCHAR * PDEVICENAMEW;

typedef CHAR DEVICENAMEA[ DEVICENAME_LENGTH + 1 ];
typedef CHAR * PDEVICENAMEA;

#ifdef UNICODE
#define DEVICENAME DEVICENAMEW
#define PDEVICENAME PDEVICENAMEW
#else
#define DEVICENAME DEVICENAMEA
#define PDEVICENAME PDEVICENAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR MODEMNAMEW[ MODEMNAME_LENGTH + 1 ];
typedef WCHAR * PMODEMNAMEW;

typedef CHAR MODEMNAMEA[ MODEMNAME_LENGTH + 1 ];
typedef CHAR * PMODEMNAMEA;

#ifdef UNICODE
#define MODEMNAME MODEMNAMEW
#define PMODEMNAME PMODEMNAMEW
#else
#define MODEMNAME MODEMNAMEA
#define PMODEMNAME PMODEMNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR DLLNAMEW[ DLLNAME_LENGTH + 1 ];
typedef WCHAR * PDLLNAMEW;

typedef CHAR DLLNAMEA[ DLLNAME_LENGTH + 1 ];
typedef CHAR * PDLLNAMEA;

#ifdef UNICODE
#define DLLNAME DLLNAMEW
#define PDLLNAME PDLLNAMEW
#else
#define DLLNAME DLLNAMEA
#define PDLLNAME PDLLNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef WCHAR PRINTERNAMEW[ PRINTERNAME_LENGTH + 1 ];
typedef WCHAR * PPRINTERNAMEW;

typedef CHAR PRINTERNAMEA[ PRINTERNAME_LENGTH + 1 ];
typedef CHAR * PPRINTERNAMEA;

#ifdef UNICODE
#define PRINTERNAME PRINTERNAMEW
#define PPRINTERNAME PPRINTERNAMEW
#else
#define PRINTERNAME PRINTERNAMEA
#define PPRINTERNAME PPRINTERNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

#ifdef UNICODE
#define NASISPECIFICNAME NASISPECIFICNAMEW
#define PNASISPECIFICNAME PNASISPECIFICNAMEW
#define NASIUSERNAME NASIUSERNAMEW
#define PNASIUSERNAME PNASIUSERNAMEW
#define NASIPASSWORD NASIPASSWORDW
#define PNASIPASSWORD PNASIPASSWORDW
#define NASISESSIONNAME NASISESSIONNAMEW
#define PNASISESSIONNAME PNASISESSIONNAMEW
#define NASIFILESERVER NASIFILESERVERW
#define PNASIFILESERVER PNASIFILESERVERW
#else
#define NASISPECIFICNAME NASISPECIFICNAMEA
#define PNASISPECIFICNAME PNASISPECIFICNAMEA
#define NASIUSERNAME NASIUSERNAMEA
#define PNASIUSERNAME PNASIUSERNAMEA
#define NASIPASSWORD NASIPASSWORDA
#define PNASIPASSWORD PNASIPASSWORDA
#define NASISESSIONNAME NASISESSIONNAMEA
#define PNASISESSIONNAME PNASISESSIONNAMEA
#define NASIFILESERVER NASIFILESERVERA
#define PNASIFILESERVER PNASIFILESERVERA
#endif /* UNICODE */

typedef WCHAR   NASISPECIFICNAMEW[ NASISPECIFICNAME_LENGTH + 1 ];
typedef WCHAR   NASIUSERNAMEW[ NASIUSERNAME_LENGTH + 1 ];
typedef WCHAR   NASIPASSWORDW[ NASIPASSWORD_LENGTH + 1 ];
typedef WCHAR   NASISESIONNAMEW[ NASISESSIONNAME_LENGTH + 1 ];
typedef WCHAR   NASIFILESERVERW[ NASIFILESERVER_LENGTH + 1 ];

typedef CHAR    NASISPECIFICNAMEA[ NASISPECIFICNAME_LENGTH + 1 ];
typedef CHAR    NASIUSERNAMEA[ NASIUSERNAME_LENGTH + 1 ];
typedef CHAR    NASIPASSWORDA[ NASIPASSWORD_LENGTH + 1 ];
typedef CHAR    NASISESIONNAMEA[ NASISESSIONNAME_LENGTH + 1 ];
typedef CHAR    NASIFILESERVERA[ NASIFILESERVER_LENGTH + 1 ];

/*------------------------------------------------*/

#define STACK_ADDRESS_LENGTH 128

/*
 *  Stack address structure
 */
typedef struct _ICA_STACK_ADDRESS {
    BYTE Address[ STACK_ADDRESS_LENGTH ];   // bytes 0,1 family, 2-n address
} ICA_STACK_ADDRESS, *PICA_STACK_ADDRESS;


/*********************************
 *  User Configuration structures
 *********************************/

/*------------------------------------------------*/

typedef WCHAR APPLICATIONNAMEW[ MAX_BR_NAME ];
typedef WCHAR *PAPPLICATIONNAMEW;

typedef CHAR APPLICATIONNAMEA[ MAX_BR_NAME ];
typedef CHAR *PAPPLICATIONNAMEA;

#ifdef UNICODE
#define APPLICATIONNAME APPLICATIONNAMEW
#define PAPPLICATIONNAME PAPPLICATIONNAMEW
#else
#define APPLICATIONNAME APPLICATIONNAMEA
#define PAPPLICATIONNAME PAPPLICATIONNAMEA
#endif /* UNICODE */

/*------------------------------------------------*/

/*
 *  Shadow options
 */
typedef enum _SHADOWCLASS {
    Shadow_Disable,
    Shadow_EnableInputNotify,
    Shadow_EnableInputNoNotify,
    Shadow_EnableNoInputNotify,
    Shadow_EnableNoInputNoNotify,
} SHADOWCLASS;

/*------------------------------------------------*/

/*
 *  Callback options
 */
typedef enum _CALLBACKCLASS {
    Callback_Disable,
    Callback_Roving,
    Callback_Fixed,
} CALLBACKCLASS;

/*------------------------------------------------*/

/*
 *  User Configuration data
 */
typedef struct _USERCONFIGW {

    /* if flag is set inherit parameter from user or client configuration */
    ULONG fInheritAutoLogon : 1;
    ULONG fInheritResetBroken : 1;
    ULONG fInheritReconnectSame : 1;
    ULONG fInheritInitialProgram : 1;
    ULONG fInheritCallback : 1;
    ULONG fInheritCallbackNumber : 1;
    ULONG fInheritShadow : 1;
    ULONG fInheritMaxSessionTime : 1;
    ULONG fInheritMaxDisconnectionTime : 1;
    ULONG fInheritMaxIdleTime : 1;
    ULONG fInheritAutoClient : 1;
    ULONG fInheritSecurity : 1;

    ULONG fPromptForPassword : 1;      // fInheritAutoLogon
    ULONG fResetBroken : 1;
    ULONG fReconnectSame : 1;
    ULONG fLogonDisabled : 1;
    ULONG fWallPaperDisabled : 1;
    ULONG fAutoClientDrives : 1;
    ULONG fAutoClientLpts : 1;
    ULONG fForceClientLptDef : 1;
    ULONG fRequireEncryption : 1;
    ULONG fDisableEncryption : 1;
    ULONG fUnused1 : 1;                 // old fDisableIniFileMapping
    ULONG fHomeDirectoryMapRoot : 1;
    ULONG fUseDefaultGina : 1;

    ULONG fPublishedApp : 1;
    ULONG fHideTitleBar : 1;

    ULONG fDisableCpm : 1;
    ULONG fDisableCdm : 1;
    ULONG fDisableCcm : 1;
    ULONG fDisableLPT : 1;
    ULONG fDisableClip : 1;
    ULONG fDisableExe : 1;
    ULONG fDisableCam : 1;

    /* fInheritAutoLogon */
    WCHAR UserName[ USERNAME_LENGTH + 1 ];
    WCHAR Domain[ DOMAIN_LENGTH + 1 ];
    WCHAR Password[ PASSWORD_LENGTH + 1 ];

    /* fInheritInitialProgram */
    WCHAR WorkDirectory[ DIRECTORY_LENGTH + 1 ];
    WCHAR InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];

    /* fInheritCallback */
    WCHAR CallbackNumber[ CALLBACK_LENGTH + 1 ];
    CALLBACKCLASS Callback;

    /* fInheritShadow */
    SHADOWCLASS Shadow;

    ULONG MaxConnectionTime;
    ULONG MaxDisconnectionTime;
    ULONG MaxIdleTime;

    ULONG KeyboardLayout;               // 0 = inherit

    /* fInheritSecurity */
    BYTE MinEncryptionLevel;

    WCHAR NWLogonServer[ NASIFILESERVER_LENGTH + 1 ];

    APPLICATIONNAMEW PublishedName;

    /* WinFrame Profile Path - Overrides standard profile path */
    WCHAR WFProfilePath[ DIRECTORY_LENGTH + 1 ];

} USERCONFIGW, * PUSERCONFIGW;

typedef struct _USERCONFIGA {

    /* if flag is set inherit parameter from user or client configuration */
    ULONG fInheritAutoLogon : 1;
    ULONG fInheritResetBroken : 1;
    ULONG fInheritReconnectSame : 1;
    ULONG fInheritInitialProgram : 1;
    ULONG fInheritCallback : 1;
    ULONG fInheritCallbackNumber : 1;
    ULONG fInheritShadow : 1;
    ULONG fInheritMaxSessionTime : 1;
    ULONG fInheritMaxDisconnectionTime : 1;
    ULONG fInheritMaxIdleTime : 1;
    ULONG fInheritAutoClient : 1;
    ULONG fInheritSecurity : 1;

    ULONG fPromptForPassword : 1;      // fInheritAutoLogon
    ULONG fResetBroken : 1;
    ULONG fReconnectSame : 1;
    ULONG fLogonDisabled : 1;
    ULONG fWallPaperDisabled : 1;
    ULONG fAutoClientDrives : 1;
    ULONG fAutoClientLpts : 1;
    ULONG fForceClientLptDef : 1;
    ULONG fRequireEncryption : 1;
    ULONG fDisableEncryption : 1;
    ULONG fUnused1 : 1;                 // old fDisableIniFileMapping
    ULONG fHomeDirectoryMapRoot : 1;
    ULONG fUseDefaultGina : 1;

    ULONG fPublishedApp : 1;
    ULONG fHideTitleBar : 1;

    ULONG fDisableCpm : 1;
    ULONG fDisableCdm : 1;
    ULONG fDisableCcm : 1;
    ULONG fDisableLPT : 1;
    ULONG fDisableClip : 1;
    ULONG fDisableExe : 1;
    ULONG fDisableCam : 1;

    /* fInheritAutoLogon */
    CHAR UserName[ USERNAME_LENGTH + 1 ];
    CHAR Domain[ DOMAIN_LENGTH + 1 ];
    CHAR Password[ PASSWORD_LENGTH + 1 ];

    /* fInheritInitialProgram */
    CHAR WorkDirectory[ DIRECTORY_LENGTH + 1 ];
    CHAR InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];

    /* fInheritCallback */
    CHAR CallbackNumber[ CALLBACK_LENGTH + 1 ];
    CALLBACKCLASS Callback;

    /* fInheritShadow */
    SHADOWCLASS Shadow;

    ULONG MaxConnectionTime;
    ULONG MaxDisconnectionTime;
    ULONG MaxIdleTime;

    ULONG KeyboardLayout;               // 0 = inherit

    /* fInheritSecurity */
    BYTE MinEncryptionLevel;

    CHAR NWLogonServer[ NASIFILESERVER_LENGTH + 1 ];

    APPLICATIONNAMEA PublishedName;

    /* WinFrame Profile Path - Overrides standard profile path */
    CHAR WFProfilePath[ DIRECTORY_LENGTH + 1 ];

} USERCONFIGA, * PUSERCONFIGA;

#ifdef UNICODE
#define USERCONFIG USERCONFIGW
#define PUSERCONFIG PUSERCONFIGW
#else
#define USERCONFIG USERCONFIGA
#define PUSERCONFIG PUSERCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

/******************
 *  PD structures
 ******************/

typedef struct _PDCONFIG2W{
    PDNAMEW PdName;                     // descriptive name of PD
    SDCLASS SdClass;                    // type of PD
    DLLNAMEW PdDLL;                     // name of PD dll
    ULONG    PdFlag;                    // PD flags
    ULONG OutBufLength;                 // optimal output buffer length
    ULONG OutBufCount;                  // optimal number of output buffers
    ULONG OutBufDelay;                  // write delay in msecs
    ULONG InteractiveDelay;             // write delay during active input
    ULONG PortNumber;                   // network listen port number
    ULONG KeepAliveTimeout;             // network watchdog frequence
} PDCONFIG2W, * PPDCONFIG2W;

typedef struct _PDCONFIG2A {
    PDNAMEA PdName;
    SDCLASS SdClass;
    DLLNAMEA PdDLL;
    ULONG    PdFlag;
    ULONG OutBufLength;
    ULONG OutBufCount;
    ULONG OutBufDelay;
    ULONG InteractiveDelay;
    ULONG PortNumber;
    ULONG KeepAliveTimeout;
} PDCONFIG2A, * PPDCONFIG2A;

/*
 *  PdFlag defines
 */
#define PD_UNUSED      0x00000001       // <unused>
#define PD_RELIABLE    0x00000002       // error free protocol already
#define PD_FRAME       0x00000004       // frame orientated protocol
#define PD_CONNECTION  0x00000008       // connection orientated protocol
#define PD_CONSOLE     0x00000010       // directly connected console
#define PD_LANA        0x00000020       // Network class uses LANAs (ie NetBIOS)
#define PD_TRANSPORT   0x00000040       // transport driver (lowest level)
#define PD_SINGLE_INST 0x00000080       // single instance only (async)


#ifdef UNICODE
#define PDCONFIG2 PDCONFIG2W
#define PPDCONFIG2 PPDCONFIG2W
#else
#define PDCONFIG2 PDCONFIG2A
#define PPDCONFIG2 PPDCONFIG2A
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _PDCONFIG3W {
    PDCONFIG2W Data;
    PDNAMEW ServiceName;               // Needed for non-LANA PdNetwork enum
    DLLNAMEW ConfigDLL;                // helper dll for WinStation configuration
    ULONG RequiredPdCount;
    PDNAMEW RequiredPds[ MAX_PDCONFIG ];
} PDCONFIG3W, * PPDCONFIG3W;

typedef struct _PDCONFIG3A {
    PDCONFIG2A Data;
    PDNAMEA ServiceName;               // Needed for non-LANA PdNetwork enum
    DLLNAMEA ConfigDLL;                // helper dll for WinStation configuration
    ULONG RequiredPdCount;
    PDNAMEA RequiredPds[ MAX_PDCONFIG ];
} PDCONFIG3A, * PPDCONFIG3A;

#ifdef UNICODE
#define PDCONFIG3 PDCONFIG3W
#define PPDCONFIG3 PPDCONFIG3W
#else
#define PDCONFIG3 PDCONFIG3A
#define PPDCONFIG3 PPDCONFIG3A
#endif /* UNICODE */

/*------------------------------------------------*/

typedef enum _FLOWCONTROLCLASS {
    FlowControl_None,
    FlowControl_Hardware,
    FlowControl_Software,
} FLOWCONTROLCLASS;

typedef enum _RECEIVEFLOWCONTROLCLASS {
    ReceiveFlowControl_None,
    ReceiveFlowControl_RTS,
    ReceiveFlowControl_DTR,
} RECEIVEFLOWCONTROLCLASS;

typedef enum _TRANSMITFLOWCONTROLCLASS {
    TransmitFlowControl_None,
    TransmitFlowControl_CTS,
    TransmitFlowControl_DSR,
} TRANSMITFLOWCONTROLCLASS;

typedef struct _FLOWCONTROLCONFIG {
    ULONG fEnableSoftwareTx: 1;
    ULONG fEnableSoftwareRx: 1;
    ULONG fEnableDTR: 1;
    ULONG fEnableRTS: 1;
    CHAR XonChar;
    CHAR XoffChar;
    FLOWCONTROLCLASS Type;
    RECEIVEFLOWCONTROLCLASS HardwareReceive;
    TRANSMITFLOWCONTROLCLASS HardwareTransmit;
} FLOWCONTROLCONFIG, * PFLOWCONTROLCONFIG;

typedef enum _ASYNCCONNECTCLASS {
    Connect_CTS,
    Connect_DSR,
    Connect_RI,
    Connect_DCD,
    Connect_FirstChar,
    Connect_Perm,
} ASYNCCONNECTCLASS;

typedef struct _CONNECTCONFIG {
    ASYNCCONNECTCLASS Type;
    ULONG fEnableBreakDisconnect: 1;
} CONNECTCONFIG, * PCONNECTCONFIG;

/*------------------------------------------------*/

typedef struct _ASYNCCONFIGW {
    DEVICENAMEW DeviceName;
    MODEMNAMEW ModemName;
    ULONG BaudRate;
    ULONG Parity;
    ULONG StopBits;
    ULONG ByteSize;
    ULONG fEnableDsrSensitivity: 1;
    ULONG fConnectionDriver: 1;
    FLOWCONTROLCONFIG FlowControl;
    CONNECTCONFIG Connect;
} ASYNCCONFIGW, * PASYNCCONFIGW;

typedef struct _ASYNCCONFIGA {
    DEVICENAMEA DeviceName;
    MODEMNAMEA ModemName;
    ULONG BaudRate;
    ULONG Parity;
    ULONG StopBits;
    ULONG ByteSize;
    ULONG fEnableDsrSensitivity: 1;
    ULONG fConnectionDriver: 1;
    FLOWCONTROLCONFIG FlowControl;
    CONNECTCONFIG Connect;
} ASYNCCONFIGA, * PASYNCCONFIGA;

#ifdef UNICODE
#define ASYNCCONFIG ASYNCCONFIGW
#define PASYNCCONFIG PASYNCCONFIGW
#else
#define ASYNCCONFIG ASYNCCONFIGA
#define PASYNCCONFIG PASYNCCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _NETWORKCONFIGW {
    LONG LanAdapter;
    DEVICENAMEW NetworkName;
    ULONG Flags;
} NETWORKCONFIGW, * PNETWORKCONFIGW;

typedef struct _NETWORKCONFIGA {
    LONG LanAdapter;
    DEVICENAMEA NetworkName;
    ULONG Flags;
} NETWORKCONFIGA, * PNETWORKCONFIGA;

#define NETWORK_CLIENT  0x00000001

#ifdef UNICODE
#define NETWORKCONFIG NETWORKCONFIGW
#define PNETWORKCONFIG PNETWORKCONFIGW
#else
#define NETWORKCONFIG NETWORKCONFIGA
#define PNETWORKCONFIG PNETWORKCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

#ifdef UNICODE
#define NASICONFIG NASICONFIGW
#define PNASICONFIG PNASICONFIGW
#else
#define NASICONFIG NASICONFIGA
#define PNASICONFIG PNASICONFIGA
#endif /* UNICODE */
typedef struct _NASICONFIGW {
    NASISPECIFICNAMEW    SpecificName;
    NASIUSERNAMEW        UserName;
    NASIPASSWORDW        PassWord;
    NASISESIONNAMEW      SessionName;
    NASIFILESERVERW      FileServer;
    BOOLEAN              GlobalSession;
} NASICONFIGW, * PNASICONFIGW;

typedef struct _NASICONFIGA {
    NASISPECIFICNAMEA    SpecificName;
    NASIUSERNAMEA        UserName;
    NASIPASSWORDA        PassWord;
    NASISESIONNAMEA      SessionName;
    NASIFILESERVERA      FileServer;
    BOOLEAN              GlobalSession;
} NASICONFIGA, * PNASICONFIGA;

/*------------------------------------------------*/

typedef struct _OEMTDCONFIGW {
    LONG Adapter;
    DEVICENAMEW DeviceName;
    ULONG Flags;
} OEMTDCONFIGW, * POEMTDCONFIGW;

typedef struct _OEMTDCONFIGA {
    LONG Adapter;
    DEVICENAMEA DeviceName;
    ULONG Flags;
} OEMTDCONFIGA, * POEMTDCONFIGA;

#ifdef UNICODE
#define OEMTDCONFIG OEMTDCONFIGW
#define POEMTDCONFIG POEMTDCONFIGW
#else
#define OEMTDCONFIG OEMTDCONFIGA
#define POEMTDCONFIG POEMTDCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _PDPARAMSW {
    SDCLASS SdClass;
    union {
        NETWORKCONFIGW Network;
        ASYNCCONFIGW Async;
        NASICONFIGW Nasi;
        OEMTDCONFIGW OemTd;
    };
} PDPARAMSW, * PPDPARAMSW;

typedef struct _PDPARAMSA {
    SDCLASS SdClass;
    union {
        NETWORKCONFIGA Network;
        ASYNCCONFIGA Async;
        NASICONFIGA Nasi;
        OEMTDCONFIGA OemTd;
    };
} PDPARAMSA, * PPDPARAMSA;

#ifdef UNICODE
#define PDPARAMS PDPARAMSW
#define PPDPARAMS PPDPARAMSW
#else
#define PDPARAMS PDPARAMSA
#define PPDPARAMS PPDPARAMSA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _PDCONFIGW {
    PDCONFIG2W Create;
    PDPARAMSW Params;
} PDCONFIGW, * PPDCONFIGW;

typedef struct _PDCONFIGA {
    PDCONFIG2A Create;
    PDPARAMSA Params;
} PDCONFIGA, * PPDCONFIGA;

#ifdef UNICODE
#define PDCONFIG PDCONFIGW
#define PPDCONFIG PPDCONFIGW
#else
#define PDCONFIG PDCONFIGA
#define PPDCONFIG PPDCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/


 /***********************
  *  Wd structures
  ***********************/


typedef struct _WDCONFIGW {
    WDNAMEW WdName;
    DLLNAMEW WdDLL;
    DLLNAMEW WsxDLL;
    ULONG WdFlag;
    ULONG WdInputBufferLength;
} WDCONFIGW, * PWDCONFIGW;

typedef struct _WDCONFIGA {
    WDNAMEA WdName;
    DLLNAMEA WdDLL;
    DLLNAMEA WsxDLL;
    ULONG WdFlag;
    ULONG WdInputBufferLength;
} WDCONFIGA, * PWDCONFIGA;

/*
 *  WdFlag defines
 */
#define WDF_UNUSED         0x00000001   // <unused>
#define WDF_SHADOW_SOURCE  0x00000002   // valid shadow source
#define WDF_SHADOW_TARGET  0x00000004   // valid shadow target
#define WDF_ICA            0x00000008   // WD is Citrix ICA class
#define WDF_TSHARE         0x00000010   // WD is Microsoft TSHARE class

#ifdef UNICODE
#define WDCONFIG WDCONFIGW
#define PWDCONFIG PWDCONFIGW
#else
#define WDCONFIG WDCONFIGA
#define PWDCONFIG PWDCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _WDCONFIG2W {
    WDCONFIGW Wd;
    ASYNCCONFIGW Async;
    USERCONFIGW User;
} WDCONFIG2W, * PWDCONFIG2W;

typedef struct _WDCONFIG2A {
    WDCONFIGA Wd;
    ASYNCCONFIGA Async;
    USERCONFIGA User;
} WDCONFIG2A, * PWDCONFIG2A;

#ifdef UNICODE
#define WDCONFIG2 WDCONFIG2W
#define PWDCONFIG2 PWDCONFIG2W
#else
#define WDCONFIG2 WDCONFIG2A
#define PWDCONFIG2 PWDCONFIG2A
#endif /* UNICODE */

/*------------------------------------------------*/

 /**************************************
  *  Connection Driver structures (CD)
  **************************************/

/*
 *  connection driver classes
 */
typedef enum _CDCLASS {
    CdNone,            // 0
    CdModem,           // 1  cdmodem.dll
    CdClass_Maximum,   // 2  -- must be last
} CDCLASS;

/*------------------------------------------------*/


typedef struct _CDCONFIGW {
    CDCLASS CdClass;
    CDNAMEW CdName;
    DLLNAMEW CdDLL;
    ULONG CdFlag;
} CDCONFIGW, * PCDCONFIGW;

typedef struct _CDCONFIGA {
    CDCLASS CdClass;
    CDNAMEA CdName;
    DLLNAMEA CdDLL;
    ULONG CdFlag;
} CDCONFIGA, * PCDCONFIGA;

#ifdef UNICODE
#define CDCONFIG CDCONFIGW
#define PCDCONFIG PCDCONFIGW
#else
#define CDCONFIG CDCONFIGA
#define PCDCONFIG PCDCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/


/*****************************
 *  Window Station structures
 *****************************/

typedef struct _WINSTATIONCREATEW {
    ULONG fEnableWinStation : 1;
    ULONG MaxInstanceCount;
} WINSTATIONCREATEW, * PWINSTATIONCREATEW;

typedef struct _WINSTATIONCREATEA {
    ULONG fEnableWinStation : 1;
    ULONG MaxInstanceCount;
} WINSTATIONCREATEA, * PWINSTATIONCREATEA;

#ifdef UNICODE
#define WINSTATIONCREATE WINSTATIONCREATEW
#define PWINSTATIONCREATE PWINSTATIONCREATEW
#else
#define WINSTATIONCREATE WINSTATIONCREATEA
#define PWINSTATIONCREATE PWINSTATIONCREATEA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _WINSTATIONCONFIGW {
    WCHAR Comment[ WINSTATIONCOMMENT_LENGTH + 1 ];
    USERCONFIGW User;
    char OEMId[4];                // WinFrame Server OEM Id
} WINSTATIONCONFIGW, * PWINSTATIONCONFIGW;

typedef struct _WINSTATIONCONFIGA {
    CHAR Comment[ WINSTATIONCOMMENT_LENGTH + 1 ];
    USERCONFIGA User;
    char OEMId[4];                // WinFrame Server OEM Id
} WINSTATIONCONFIGA, * PWINSTATIONCONFIGA;

#ifdef UNICODE
#define WINSTATIONCONFIG WINSTATIONCONFIGW
#define PWINSTATIONCONFIG PWINSTATIONCONFIGW
#else
#define WINSTATIONCONFIG WINSTATIONCONFIGA
#define PWINSTATIONCONFIG PWINSTATIONCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef enum _WINSTATIONINFOCLASS {
    WinStationCreateData,         // query WinStation create data
    WinStationConfiguration,      // query/set WinStation parameters
    WinStationPdParams,           // query/set PD parameters
    WinStationWd,                 // query WD config (only one can be loaded)
    WinStationPd,                 // query PD config (many can be loaded)
    WinStationPrinter,            // query/set LPT mapping to printer queues
    WinStationClient,             // query information about client
    WinStationModules,            // query information about all client modules
    WinStationInformation,        // query information about WinStation
    WinStationTrace,              // enable/disable winstation tracing
    WinStationBeep,               // beep the WinStation
    WinStationEncryptionOff,      // turn off encryption
    WinStationEncryptionPerm,     // encryption is permanent on
    WinStationNtSecurity,         // select winlogon security desktop
    WinStationUserToken,          // User token
    WinStationUnused1,            // *** AVAILABLE *** (old IniMapping)
    WinStationVideoData,          // query hres, vres, color depth
    WinStationInitialProgram,     // Identify Initial Program
    WinStationCd,                 // query CD config (only one can be loaded)
    WinStationSystemTrace,        // enable/disable system tracing
    WinStationVirtualData,        // query client virtual data
    WinStationClientData,         // send data to client
    WinStationSecureDesktopEnter, // turn encryption on, if enabled
    WinStationSecureDesktopExit,  // turn encryption off, if enabled
} WINSTATIONINFOCLASS;

/*------------------------------------------------*/

typedef struct _WINSTATIONCLIENTDATA {
    CLIENTDATANAME DataName;
    BOOLEAN fUnicodeData;
    /* BYTE   Data[1]; Variable length data follows */
} WINSTATIONCLIENTDATA, * PWINSTATIONCLIENTDATA;

/*------------------------------------------------*/

typedef struct _WINSTATIONUSERTOKEN {
    HANDLE ProcessId;
    HANDLE ThreadId;
    HANDLE UserToken;
} WINSTATIONUSERTOKEN, * PWINSTATIONUSERTOKEN;

/*------------------------------------------------*/

typedef struct _WINSTATIONVIDEODATA {
    USHORT  HResolution;
    USHORT  VResolution;
    USHORT  fColorDepth;
} WINSTATIONVIDEODATA, *PWINSTATIONVIDEODATA;

/*----------------------------------------------*/

typedef struct _WINSTATIONCONFIG2W {
    WINSTATIONCREATEW Create;
    PDCONFIGW Pd[ MAX_PDCONFIG ];
    WDCONFIGW Wd;
    CDCONFIGW Cd;
    WINSTATIONCONFIGW Config;
} WINSTATIONCONFIG2W, * PWINSTATIONCONFIG2W;

typedef struct _WINSTATIONCONFIG2A {
    WINSTATIONCREATEA Create;
    PDCONFIGA Pd[ MAX_PDCONFIG ];
    WDCONFIGA Wd;
    CDCONFIGA Cd;
    WINSTATIONCONFIGA Config;
} WINSTATIONCONFIG2A, * PWINSTATIONCONFIG2A;

#ifdef UNICODE
#define WINSTATIONCONFIG2 WINSTATIONCONFIG2W
#define PWINSTATIONCONFIG2 PWINSTATIONCONFIG2W
#else
#define WINSTATIONCONFIG2 WINSTATIONCONFIG2A
#define PWINSTATIONCONFIG2 PWINSTATIONCONFIG2A
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _WINSTATIONPRINTERW {
    PRINTERNAMEW Lpt1;
    PRINTERNAMEW Lpt2;
    PRINTERNAMEW Lpt3;
    PRINTERNAMEW Lpt4;
} WINSTATIONPRINTERW, * PWINSTATIONPRINTERW;

typedef struct _WINSTATIONPRINTERA {
    PRINTERNAMEA Lpt1;
    PRINTERNAMEA Lpt2;
    PRINTERNAMEA Lpt3;
    PRINTERNAMEA Lpt4;
} WINSTATIONPRINTERA, * PWINSTATIONPRINTERA;

#ifdef UNICODE
#define WINSTATIONPRINTER WINSTATIONPRINTERW
#define PWINSTATIONPRINTER PWINSTATIONPRINTERW
#else
#define WINSTATIONPRINTER WINSTATIONPRINTERA
#define PWINSTATIONPRINTER PWINSTATIONPRINTERA
#endif /* UNICODE */

/*------------------------------------------------*/

/*
 *  WinStation client data structure
 */
typedef struct _WINSTATIONCLIENTW {
    ULONG fTextOnly: 1;
    ULONG fDisableCtrlAltDel: 1;
    WCHAR ClientName[ CLIENTNAME_LENGTH + 1 ];
    WCHAR Domain[ DOMAIN_LENGTH + 1 ];
    WCHAR UserName[ USERNAME_LENGTH + 1 ];
    WCHAR Password[ PASSWORD_LENGTH + 1 ];
    WCHAR WorkDirectory[ DIRECTORY_LENGTH + 1 ];
    WCHAR InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];
    ULONG SerialNumber;         // client computer unique serial number
    BYTE EncryptionLevel;       // security level of encryption pd
    ULONG ClientAddressFamily;
    WCHAR ClientAddress[ CLIENTADDRESS_LENGTH + 1 ];
    USHORT HRes;
    USHORT VRes;
    USHORT ColorDepth;
    USHORT ProtocolType;   // PROTOCOL_ICA or PROTOCOL_TSHARE
    ULONG KeyboardLayout;
    WCHAR ClientDirectory[ DIRECTORY_LENGTH + 1 ];
    WCHAR ClientLicense[ CLIENTLICENSE_LENGTH + 1 ];
    WCHAR ClientModem[ CLIENTMODEM_LENGTH + 1 ];
    ULONG ClientBuildNumber;
    ULONG ClientHardwareId;    // client software serial number
    USHORT ClientProductId;     // client software product id
    USHORT OutBufCountHost;     // number of outbufs on host
    USHORT OutBufCountClient;   // number of outbufs on client
    USHORT OutBufLength;        // length of outbufs in bytes
} WINSTATIONCLIENTW, * PWINSTATIONCLIENTW;

/*
 *  WinStation client data structure
 */
typedef struct _WINSTATIONCLIENTA {
    ULONG fTextOnly: 1;
    ULONG fDisableCtrlAltDel: 1;
    char ClientName[ CLIENTNAME_LENGTH + 1 ];
    char Domain[ DOMAIN_LENGTH + 1 ];
    char UserName[ USERNAME_LENGTH + 1 ];
    char Password[ PASSWORD_LENGTH + 1 ];
    char WorkDirectory[ DIRECTORY_LENGTH + 1 ];
    char InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];
    ULONG SerialNumber;         // client computer unique serial number
    BYTE EncryptionLevel;       // security level of encryption pd
    ULONG ClientAddressFamily;
    char ClientAddress[ CLIENTADDRESS_LENGTH + 1 ];
    USHORT HRes;
    USHORT VRes;
    USHORT ColorDepth;
    USHORT ProtocolType;   // PROTOCOL_ICA or PROTOCOL_TSHARE
    ULONG KeyboardLayout;
    char ClientDirectory[ DIRECTORY_LENGTH + 1 ];
    char ClientLicense[ CLIENTLICENSE_LENGTH + 1 ];
    char ClientModem[ CLIENTMODEM_LENGTH + 1 ];
    ULONG ClientBuildNumber;
    ULONG ClientHardwareId;    // client software serial number
    USHORT ClientProductId;     // client software product id
    USHORT OutBufCountHost;     // number of outbufs on host
    USHORT OutBufCountClient;   // number of outbufs on client
    USHORT OutBufLength;        // length of outbufs in bytes
} WINSTATIONCLIENTA, * PWINSTATIONCLIENTA;

#ifdef UNICODE
#define WINSTATIONCLIENT WINSTATIONCLIENTW
#define PWINSTATIONCLIENT PWINSTATIONCLIENTW
#else
#define WINSTATIONCLIENT WINSTATIONCLIENTA
#define PWINSTATIONCLIENT PWINSTATIONCLIENTA
#endif /* UNICODE */

/*------------------------------------------------*/

/*
 *  ICA specific protocol performance counters
 */
typedef struct _ICA_COUNTERS {
    ULONG Reserved;
} ICA_COUNTERS, * PICA_COUNTERS;

/*
 *  T.Share specific protocol performance counters
 */
typedef struct _TSHARE_COUNTERS {
    ULONG Reserved;
} TSHARE_COUNTERS, * PTSHARE_COUNTERS;

/*
 *  WinStation protocol performance counters
 */
typedef struct _PROTOCOLCOUNTERS {
    ULONG WdBytes;              // wd common
    ULONG WdFrames;             // wd common
    ULONG WaitForOutBuf;        // wd common
    ULONG Frames;               // td common
    ULONG Bytes;                // td common
    ULONG CompressedBytes;      // pdcomp
    ULONG CompressFlushes;      // pdcomp
    ULONG Errors;               // pdreli
    ULONG Timeouts;             // pdreli
    ULONG AsyncFramingError;    // pdasync
    ULONG AsyncOverrunError;    // pdasync
    ULONG AsyncOverflowError;   // pdasync
    ULONG AsyncParityError;     // pdasync
    ULONG TdErrors;             // td common
    USHORT ProtocolType;        // PROTOCOL_ICA or PROTOCOL_TSHARE
    USHORT Length;              // length of data in protocol-specific space
    union {
        ICA_COUNTERS    IcaCounters;
        TSHARE_COUNTERS TShareCounters;
        ULONG           Reserved[100];
    } Specific;
} PROTOCOLCOUNTERS, * PPROTOCOLCOUNTERS;


/*
 * ThinWire cache statistics
 */
typedef struct _THINWIRECACHE {
    ULONG CacheReads;
    ULONG CacheHits;
} THINWIRECACHE, * PTHINWIRECACHE;

#define MAX_THINWIRECACHE   4

/*
 *  ICA specific cache statistics
 */
typedef struct _ICA_CACHE {
    THINWIRECACHE ThinWireCache[ MAX_THINWIRECACHE ];
} ICA_CACHE, * PICA_CACHE;

/*
 *  T.Share specific cache statistics
 */
typedef struct _TSHARE_CACHE {
    ULONG Reserved;
} TSHARE_CACHE, * PTSHARE_CACHE;

/*
 *  WinStation cache statistics
 */
typedef struct CACHE_STATISTICS {
    USHORT ProtocolType;        // PROTOCOL_ICA or PROTOCOL_TSHARE
    USHORT Length;              // length of data in protocol-specific space
    union {
        ICA_CACHE    IcaCacheStats;
        TSHARE_CACHE TShareCacheStats;
        ULONG        Reserved[20];
    } Specific;
} CACHE_STATISTICS, * PCACHE_STATISTICS;

/*
 *  WinStation protocol status
 */
typedef struct _PROTOCOLSTATUS {
    PROTOCOLCOUNTERS Output;
    PROTOCOLCOUNTERS Input;
    CACHE_STATISTICS Cache;
    ULONG AsyncSignal;                  // MS_CTS_ON, MS_DSR_ON, etc...
    ULONG AsyncSignalMask;              // EV_CTS, EV_DSR, etc...
} PROTOCOLSTATUS, * PPROTOCOLSTATUS;

/*
 *  AsyncSignal defines
 */
// #define MS_CTS_ON            0x0010
// #define MS_DSR_ON            0x0020
// #define MS_RING_ON           0x0040
// #define MS_RLSD_ON           0x0080
#define MS_DTR_ON           0x00010000
#define MS_RTS_ON           0x00020000

/*
 *  AsyncSignalMask defines
 */
// #define EV_CTS              0x0008  // CTS changed state
// #define EV_DSR              0x0010  // DSR changed state
// #define EV_RLSD             0x0020  // RLSD changed state
// #define EV_RING             0x0100  // Ring signal detected
#define EV_DTR             0x00010000  // DTR changed state
#define EV_RTS             0x00020000  // DTR changed state

/*------------------------------------------------*/

/*
 *  WinStation query information
 */
typedef struct _WINSTATIONINFORMATIONW {
    WINSTATIONSTATECLASS ConnectState;
    WINSTATIONNAMEW WinStationName;
    ULONG LogonId;
    LARGE_INTEGER ConnectTime;
    LARGE_INTEGER DisconnectTime;
    LARGE_INTEGER LastInputTime;
    LARGE_INTEGER LogonTime;
    PROTOCOLSTATUS Status;
    WCHAR Domain[ DOMAIN_LENGTH + 1 ];
    WCHAR UserName[USERNAME_LENGTH + 1];
    LARGE_INTEGER CurrentTime;
} WINSTATIONINFORMATIONW, * PWINSTATIONINFORMATIONW;

typedef struct _WINSTATIONINFORMATIONA {
    WINSTATIONSTATECLASS ConnectState;
    WINSTATIONNAMEA WinStationName;
    ULONG LogonId;
    LARGE_INTEGER ConnectTime;
    LARGE_INTEGER DisconnectTime;
    LARGE_INTEGER LastInputTime;
    LARGE_INTEGER LogonTime;
    PROTOCOLSTATUS Status;
    CHAR Domain[ DOMAIN_LENGTH + 1 ];
    CHAR UserName[USERNAME_LENGTH + 1];
    LARGE_INTEGER CurrentTime;
} WINSTATIONINFORMATIONA, * PWINSTATIONINFORMATIONA;

#ifdef UNICODE
#define WINSTATIONINFORMATION WINSTATIONINFORMATIONW
#define PWINSTATIONINFORMATION PWINSTATIONINFORMATIONW
#else
#define WINSTATIONINFORMATION WINSTATIONINFORMATIONA
#define PWINSTATIONINFORMATION PWINSTATIONINFORMATIONA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _BEEPINPUT {
    ULONG uType;
} BEEPINPUT, * PBEEPINPUT;

/*------------------------------------------------*/


/**********************
 *  NWLogon Structure
 **********************/


typedef struct _NWLOGONADMIN {
    WCHAR Username[ USERNAME_LENGTH + 1 ];
    WCHAR Password[ PASSWORD_LENGTH + 1 ];
    WCHAR Domain[ DOMAIN_LENGTH + 1 ];
} NWLOGONADMIN, * PNWLOGONADMIN;


/*------------------------------------------------*/


/**********************************************
 *  Registry APIs for Connection Drivers (Cds)
 **********************************************/


LONG WINAPI
RegCdEnumerateW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    PULONG  pIndex,
    PULONG  pEntries,
    PCDNAMEW pCdName,
    PULONG  pByteCount
    );

LONG WINAPI
RegCdEnumerateA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    PULONG  pIndex,
    PULONG  pEntries,
    PCDNAMEA pCdName,
    PULONG  pByteCount
    );

#ifdef UNICODE
#define RegCdEnumerate RegCdEnumerateW
#else
#define RegCdEnumerate RegCdEnumerateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegCdCreateW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    PCDNAMEW pCdName,
    BOOLEAN bCreate,
    PCDCONFIGW pCdConfig,
    ULONG CdConfigLength
    );

LONG WINAPI
RegCdCreateA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    PCDNAMEA pCdName,
    BOOLEAN bCreate,
    PCDCONFIGA pCdConfig,
    ULONG CdConfigLength
    );

#ifdef UNICODE
#define RegCdCreate RegCdCreateW
#else
#define RegCdCreate RegCdCreateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegCdQueryW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    PCDNAMEW pCdName,
    PCDCONFIGW pCdConfig,
    ULONG CdConfigLength,
    PULONG pReturnLength
    );

LONG WINAPI
RegCdQueryA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    PCDNAMEA pCdName,
    PCDCONFIGA pCdConfig,
    ULONG CdConfigLength,
    PULONG pReturnLength
    );

#ifdef UNICODE
#define RegCdQuery RegCdQueryW
#else
#define RegCdQuery RegCdQueryA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegCdDeleteW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    PCDNAMEW pCdName
    );

LONG WINAPI
RegCdDeleteA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    PCDNAMEA pCdName
    );

#ifdef UNICODE
#define RegCdDelete RegCdDeleteW
#else
#define RegCdDelete RegCdDeleteA
#endif /* UNICODE */

/*------------------------------------------------*/


/**********************************************
 *  Registry APIs for WinStation Drivers (Wds)
 **********************************************/


LONG WINAPI
RegWdEnumerateW(
    HANDLE hServer,
    PULONG  pIndex,
    PULONG  pEntries,
    PWDNAMEW pWdName,
    PULONG  pByteCount
    );

LONG WINAPI
RegWdEnumerateA(
    HANDLE hServer,
    PULONG  pIndex,
    PULONG  pEntries,
    PWDNAMEA pWdName,
    PULONG  pByteCount
    );

#ifdef UNICODE
#define RegWdEnumerate RegWdEnumerateW
#else
#define RegWdEnumerate RegWdEnumerateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWdCreateW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    BOOLEAN bCreate,
    PWDCONFIG2W pWdConfig,
    ULONG WdConfigLength
    );

LONG WINAPI
RegWdCreateA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    BOOLEAN bCreate,
    PWDCONFIG2A pWdConfig,
    ULONG WdConfigLength
    );

#ifdef UNICODE
#define RegWdCreate RegWdCreateW
#else
#define RegWdCreate RegWdCreateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWdQueryW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    PWDCONFIG2W pWdConfig,
    ULONG WdConfigLength,
    PULONG pReturnLength
    );

LONG WINAPI
RegWdQueryA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    PWDCONFIG2A pWdConfig,
    ULONG WdConfigLength,
    PULONG pReturnLength
    );

#ifdef UNICODE
#define RegWdQuery RegWdQueryW
#else
#define RegWdQuery RegWdQueryA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWdDeleteW(
    HANDLE hServer,
    PWDNAMEW pWdName
    );

LONG WINAPI
RegWdDeleteA(
    HANDLE hServer,
    PWDNAMEA pWdName
    );

#ifdef UNICODE
#define RegWdDelete RegWdDeleteW
#else
#define RegWdDelete RegWdDeleteA
#endif /* UNICODE */

/*------------------------------------------------*/


/******************************************************************
 *  Registry APIs for Transport and Protocol Drivers (Tds and Pds)
 ******************************************************************/


HANDLE WINAPI
RegOpenServerW(
    LPWSTR hServerName
    );

HANDLE WINAPI
RegOpenServerA(
    LPSTR hServerName
    );

#ifdef UNICODE
#define RegOpenServer RegOpenServerW
#else
#define RegOpenServer RegOpenServerA
#endif /* UNICODE */

/*------------------------------------------------*/
LONG WINAPI
RegCloseServer(
        HANDLE hServer
        );

/*------------------------------------------------*/
LONG WINAPI
RegPdEnumerateW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    BOOLEAN bTd,
    PULONG  pIndex,
    PULONG  pEntries,
    PPDNAMEW  pPdName,
    PULONG  pByteCount
    );

LONG WINAPI
RegPdEnumerateA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    BOOLEAN bTd,
    PULONG  pIndex,
    PULONG  pEntries,
    PPDNAMEA  pPdName,
    PULONG  pByteCount
    );

#ifdef UNICODE
#define RegPdEnumerate RegPdEnumerateW
#else
#define RegPdEnumerate RegPdEnumerateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegPdCreateW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    BOOLEAN bTd,
    PPDNAMEW pPdName,
    BOOLEAN bCreate,
    PPDCONFIG3W pPdConfig,
    ULONG PdConfigLength
    );

LONG WINAPI
RegPdCreateA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    BOOLEAN bTd,
    PPDNAMEA pPdName,
    BOOLEAN bCreate,
    PPDCONFIG3A pPdConfig,
    ULONG PdConfigLength
    );

#ifdef UNICODE
#define RegPdCreate RegPdCreateW
#else
#define RegPdCreate RegPdCreateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegPdQueryW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    BOOLEAN bTd,
    PPDNAMEW pPdName,
    PPDCONFIG3W pPdConfig,
    ULONG PdConfigLength,
    PULONG pReturnLength
    );

LONG WINAPI
RegPdQueryA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    BOOLEAN bTd,
    PPDNAMEA pPdName,
    PPDCONFIG3A pPdConfig,
    ULONG PdConfigLength,
    PULONG pReturnLength
    );

#ifdef UNICODE
#define RegPdQuery RegPdQueryW
#else
#define RegPdQuery RegPdQueryA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegPdDeleteW(
    HANDLE hServer,
    PWDNAMEW pWdName,
    BOOLEAN bTd,
    PPDNAMEW pPdName
    );

LONG WINAPI
RegPdDeleteA(
    HANDLE hServer,
    PWDNAMEA pWdName,
    BOOLEAN bTd,
    PPDNAMEA pPdName
    );

#ifdef UNICODE
#define RegPdDelete RegPdDeleteW
#else
#define RegPdDelete RegPdDeleteA
#endif /* UNICODE */

/*------------------------------------------------*/


/*************************************
 *  Registry APIs for Window Stations
 *************************************/


LONG WINAPI
RegWinStationAccessCheck(
    HANDLE hServer,
    ACCESS_MASK samDesired
    );

/*------------------------------------------------*/

LONG WINAPI
RegWinStationEnumerateW(
    HANDLE hServer,
    PULONG  pIndex,
    PULONG  pEntries,
    PWINSTATIONNAMEW pWinStationName,
    PULONG  pByteCount
    );

/*------------------------------------------------*/

LONG WINAPI
RegWinStationEnumerateA(
    HANDLE hServer,
    PULONG  pIndex,
    PULONG  pEntries,
    PWINSTATIONNAMEA pWinStationName,
    PULONG  pByteCount
    );

#ifdef UNICODE
#define RegWinStationEnumerate RegWinStationEnumerateW
#else
#define RegWinStationEnumerate RegWinStationEnumerateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationCreateW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    BOOLEAN bCreate,
    PWINSTATIONCONFIG2W pWinStationConfig,
    ULONG WinStationConfigLength
    );

LONG WINAPI
RegWinStationCreateA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName,
    BOOLEAN bCreate,
    PWINSTATIONCONFIG2A pWinStationConfig,
    ULONG WinStationConfigLength
    );

#ifdef UNICODE
#define RegWinStationCreate RegWinStationCreateW
#else
#define RegWinStationCreate RegWinStationCreateA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationQueryW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    PWINSTATIONCONFIG2W pWinStationConfig,
    ULONG WinStationConfigLength,
    PULONG pReturnLength
    );

LONG WINAPI
RegWinStationQueryA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName,
    PWINSTATIONCONFIG2A pWinStationConfig,
    ULONG WinStationConfigLength,
    PULONG pReturnLength
    );

#ifdef UNICODE
#define RegWinStationQuery RegWinStationQueryW
#else
#define RegWinStationQuery RegWinStationQueryA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationDeleteW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName
    );

LONG WINAPI
RegWinStationDeleteA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName
    );

#ifdef UNICODE
#define RegWinStationDelete RegWinStationDeleteW
#else
#define RegWinStationDelete RegWinStationDeleteA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationSetSecurityW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    PSECURITY_DESCRIPTOR pSecurityDescriptor,
    ULONG SecurityDescriptorLength
    );

LONG WINAPI
RegWinStationSetSecurityA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName,
    PSECURITY_DESCRIPTOR pSecurityDescriptor,
    ULONG SecurityDescriptorLength
    );

#ifdef UNICODE
#define RegWinStationSetSecurity RegWinStationSetSecurityW
#else
#define RegWinStationSetSecurity RegWinStationSetSecurityA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationQuerySecurityW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    PSECURITY_DESCRIPTOR pSecurityDescriptor,
    ULONG SecurityDescriptorLength,
    PULONG SecurityDescriptorLengthRequired
    );

LONG WINAPI
RegWinStationQuerySecurityA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName,
    PSECURITY_DESCRIPTOR pSecurityDescriptor,
    ULONG SecurityDescriptorLength,
    PULONG SecurityDescriptorLengthRequired
    );

#ifdef UNICODE
#define RegWinStationQuerySecurity RegWinStationQuerySecurityW
#else
#define RegWinStationQuerySecurity RegWinStationQuerySecurityA
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationQueryDefaultSecurity(
    HANDLE hServer,
    PSECURITY_DESCRIPTOR pSecurityDescriptor,
    ULONG SecurityDescriptorLength,
    PULONG SecurityDescriptorLengthRequired
    );

/*------------------------------------------------*/

LONG WINAPI
RegWinStationSetNumValueW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    LPWSTR pValueName,
    ULONG ValueData
    );

#ifdef UNICODE
#define RegWinStationSetNumValue RegWinStationSetNumValueW
#else
#define RegWinStationSetNumValue
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationQueryNumValueW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    LPWSTR pValueName,
    PULONG pValueData );

#ifdef UNICODE
#define RegWinStationQueryNumValue RegWinStationQueryNumValueW
#else
#define RegWinStationQueryNumValue
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegWinStationQueryValueW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    LPWSTR pValueName,
    PVOID pValueData,
    ULONG ValueSize,
    PULONG pValueSize );

#ifdef UNICODE
#define RegWinStationQueryValue RegWinStationQueryValueW
#else
#define RegWinStationQueryValue
#endif /* UNICODE */

/*------------------------------------------------*/


/*********************************************
 *  Registry APIs for User Configuration Data
 *********************************************/

/*------------------------------------------------*/

LONG WINAPI
RegUserConfigSet(
    WCHAR * pServerName,
    WCHAR * pUserName,
    PUSERCONFIGW pUserConfig,
    ULONG UserConfigLength
    );

/*------------------------------------------------*/

LONG WINAPI
RegUserConfigQuery(
    WCHAR * pServerName,
    WCHAR * pUserName,
    PUSERCONFIGW pUserConfig,
    ULONG UserConfigLength,
    PULONG pReturnLength
    );

/*------------------------------------------------*/

LONG WINAPI
RegUserConfigDelete(
    WCHAR * pServerName,
    WCHAR * pUserName
    );

/*------------------------------------------------*/

LONG WINAPI
RegUserConfigRename(
    WCHAR * pServerName,
    WCHAR * pUserOldName,
    WCHAR * pUserNewName
    );

/*------------------------------------------------*/

LONG WINAPI
RegDefaultUserConfigQueryW(
    WCHAR * pServerName,
    PUSERCONFIGW pUserConfig,
    ULONG UserConfigLength,
    PULONG pReturnLength
    );

LONG WINAPI
RegDefaultUserConfigQueryA(
    CHAR * pServerName,
    PUSERCONFIGA pUserConfig,
    ULONG UserConfigLength,
    PULONG pReturnLength
    );

#ifdef UNICODE
#define RegDefaultUserConfigQuery RegDefaultUserConfigQueryW
#else
#define RegDefaultUserConfigQuery RegDefaultUserConfigQueryA
#endif /* UNICODE */

/*------------------------------------------------*/


/*********************************************
 *  Additional Support Registry APIs
 *********************************************/

BOOLEAN WINAPI
RegIsCitrixAppServer(
    WCHAR * pServerName
    );

BOOLEAN WINAPI
RegSetupIsCitrixAppServer(
    WCHAR * pServerName
    );

BOOLEAN WINAPI
RegBuildNumberQuery(
    PULONG pBuildNum
    );

BOOLEAN WINAPI
RegGetCitrixVersion(
    WCHAR * pServerName,
    PULONG  pVersionNumber
    );


BOOLEAN WINAPI
RegQueryOEMId(
    BYTE *,
    ULONG
    );

/*------------------------------------------------*/


/***********************************
 *  APIs for Window Station Objects
 ***********************************/

/*------------------------------------------------*/

HANDLE WINAPI
WinStationOpenServerW(
    LPWSTR  pServerName
    );

HANDLE WINAPI
WinStationOpenServerA(
    LPSTR  pServerName
    );

#ifdef UNICODE
#define WinStationOpenServer WinStationOpenServerW
#else
#define WinStationOpenServer WinStationOpenServerA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationCloseServer(
    HANDLE  hServer
    );

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationServerPing(
    HANDLE  hServer
    );

/*------------------------------------------------*/

typedef BOOLEAN (WINAPI * PWINSTATIONENUMERATEW)(HANDLE,PLOGONIDW *,PULONG);
typedef BOOLEAN (WINAPI * PWINSTATIONENUMERATEA)(HANDLE,PLOGONIDA *,PULONG);

BOOLEAN WINAPI
WinStationEnumerateW(
    HANDLE  hServer,
    PLOGONIDW *ppLogonId,
    PULONG  pEntries
    );

BOOLEAN WINAPI
WinStationEnumerateA(
    HANDLE  hServer,
    PLOGONIDA *ppLogonId,
    PULONG  pEntries
    );

#ifdef UNICODE
#define WinStationEnumerate WinStationEnumerateW
#else
#define WinStationEnumerate WinStationEnumerateA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef BOOLEAN (WINAPI * PWINSTATIONENUMERATE_INDEXEDW)(HANDLE,PULONG,PLOGONIDW,PULONG,PULONG);
typedef BOOLEAN (WINAPI * PWINSTATIONENUMERATE_INDEXEDA)(HANDLE,PULONG,PLOGONIDA,PULONG,PULONG);

BOOLEAN WINAPI
WinStationEnumerate_IndexedW(
    HANDLE  hServer,
    PULONG  pEntries,
    PLOGONIDW pLogonId,
    PULONG  pByteCount,
    PULONG  pIndex
    );

BOOLEAN WINAPI
WinStationEnumerate_IndexedA(
    HANDLE  hServer,
    PULONG  pEntries,
    PLOGONIDA pLogonId,
    PULONG  pByteCount,
    PULONG  pIndex
    );

#ifdef UNICODE
#define WinStationEnumerate_Indexed WinStationEnumerate_IndexedW
#else
#define WinStationEnumerate_Indexed WinStationEnumerate_IndexedA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationEnumerateProcesses(
    HANDLE  hServer,
    PVOID *ppProcessBuffer
    );

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationRenameW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationNameOld,
    PWINSTATIONNAMEW pWinStationNameNew
    );

BOOLEAN WINAPI
WinStationRenameA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationNameOld,
    PWINSTATIONNAMEA pWinStationNameNew
    );

#ifdef UNICODE
#define WinStationRename WinStationRenameW
#else
#define WinStationRename WinStationRenameA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef BOOLEAN (WINAPI * PWINSTATIONQUERYINFORMATIONW)(HANDLE,ULONG,WINSTATIONINFOCLASS,PVOID,ULONG,PULONG);
typedef BOOLEAN (WINAPI * PWINSTATIONQUERYINFORMATIONA)(HANDLE,ULONG,WINSTATIONINFOCLASS,PVOID,ULONG,PULONG);

BOOLEAN WINAPI
WinStationQueryInformationW(
    HANDLE hServer,
    ULONG LogonId,
    WINSTATIONINFOCLASS WinStationInformationClass,
    PVOID  pWinStationInformation,
    ULONG WinStationInformationLength,
    PULONG  pReturnLength
    );

BOOLEAN WINAPI
WinStationQueryInformationA(
    HANDLE hServer,
    ULONG LogonId,
    WINSTATIONINFOCLASS WinStationInformationClass,
    PVOID  pWinStationInformation,
    ULONG WinStationInformationLength,
    PULONG  pReturnLength
    );

#ifdef UNICODE
#define WinStationQueryInformation WinStationQueryInformationW
#else
#define WinStationQueryInformation WinStationQueryInformationA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationSetInformationW(
    HANDLE hServer,
    ULONG LogonId,
    WINSTATIONINFOCLASS WinStationInformationClass,
    PVOID pWinStationInformation,
    ULONG WinStationInformationLength
    );

BOOLEAN WINAPI
WinStationSetInformationA(
    HANDLE hServer,
    ULONG LogonId,
    WINSTATIONINFOCLASS WinStationInformationClass,
    PVOID pWinStationInformation,
    ULONG WinStationInformationLength
    );

#ifdef UNICODE
#define WinStationSetInformation WinStationSetInformationW
#else
#define WinStationSetInformation WinStationSetInformationA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationSendMessageW(
    HANDLE hServer,
    ULONG LogonId,
    LPWSTR  pTitle,
    ULONG TitleLength,
    LPWSTR  pMessage,
    ULONG MessageLength,
    ULONG Style,
    ULONG Timeout,
    PULONG pResponse,
    BOOLEAN DoNotWait
    );

BOOLEAN WINAPI
WinStationSendMessageA(
    HANDLE hServer,
    ULONG LogonId,
    LPSTR  pTitle,
    ULONG TitleLength,
    LPSTR  pMessage,
    ULONG MessageLength,
    ULONG Style,
    ULONG Timeout,
    PULONG pResponse,
    BOOLEAN DoNotWait
    );

#ifdef UNICODE
#define WinStationSendMessage WinStationSendMessageW
#else
#define WinStationSendMessage WinStationSendMessageA
#endif /* UNICODE */


/*
 *  These are new pResponse values
 */
#define IDTIMEOUT        32000  // The MsgBox timed out before a user response
#define IDASYNC          32001  // The request was for an Async message box, no return
#define IDERROR          32002  // an error occured that resulted in not displaying
#define IDCOUNTEXCEEDED  32003  // to many in queue for winstation already
#define IDDESKTOPERROR   32004  // current desktop is not default


/*------------------------------------------------*/

BOOLEAN WINAPI
LogonIdFromWinStationNameW(
    HANDLE hServer,
    PWINSTATIONNAMEW pWinStationName,
    PULONG pLogonId
    );

BOOLEAN WINAPI
LogonIdFromWinStationNameA(
    HANDLE hServer,
    PWINSTATIONNAMEA pWinStationName,
    PULONG pLogonId
    );

#ifdef UNICODE
#define LogonIdFromWinStationName LogonIdFromWinStationNameW
#else
#define LogonIdFromWinStationName LogonIdFromWinStationNameA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationNameFromLogonIdW(
    HANDLE hServer,
    ULONG LogonId,
    PWINSTATIONNAMEW pWinStationName
    );

BOOLEAN WINAPI
WinStationNameFromLogonIdA(
    HANDLE hServer,
    ULONG LogonId,
    PWINSTATIONNAMEA pWinStationName
    );

#ifdef UNICODE
#define WinStationNameFromLogonId WinStationNameFromLogonIdW
#else
#define WinStationNameFromLogonId WinStationNameFromLogonIdA
#endif /* UNICODE */

/*------------------------------------------------*/

BOOLEAN WINAPI
WinStationConnectW(
    HANDLE hServer,
    ULONG LogonId,
    ULONG TargetLogonId,
    PWCHAR pPassword,
    BOOLEAN bWait
    );

BOOLEAN WINAPI
WinStationConnectA(
    HANDLE hServer,
    ULONG LogonId,
    ULONG TargetLogonId,
    PCHAR pPassword,
    BOOLEAN bWait
    );

#ifdef UNICODE
#define WinStationConnect WinStationConnectW
#else
#define WinStationConnect WinStationConnectA
#endif /* UNICODE */

/*------------------------------------------------*/


HANDLE WINAPI
WinStationVirtualOpen(
    HANDLE hServer,
    ULONG LogonId,
    PVIRTUALCHANNELNAME pVirtualChannelName    /* ascii name */
    );

typedef HANDLE (WINAPI * PWINSTATIONBEEPOPEN)(ULONG);
HANDLE WINAPI
_WinStationBeepOpen(
    ULONG LogonId
    );

BOOLEAN WINAPI
WinStationDisconnect(
    HANDLE hServer,
    ULONG LogonId,
    BOOLEAN bWait
    );


BOOLEAN WINAPI
WinStationReset(
    HANDLE hServer,
    ULONG LogonId,
    BOOLEAN bWait
    );


BOOLEAN WINAPI
WinStationShutdownSystem(
    HANDLE hServer,
    ULONG ShutdownFlags
    );

#define WSD_LOGOFF      0x00000001  // force WinStations to logoff
#define WSD_SHUTDOWN    0x00000002  // shutdown system
#define WSD_REBOOT      0x00000004  // reboot after shutdown
#define WSD_POWEROFF    0x00000008  // reboot after shutdown
#define WSD_FASTREBOOT  0x00000010  // CTRL-SHIFT-ALT-DEL fast reboot


typedef BOOLEAN (WINAPI * PWINSTATIONWAITSYSTEMEVENT)(HANDLE,ULONG,PULONG);

BOOLEAN WINAPI
WinStationWaitSystemEvent(
    HANDLE hServer,
    ULONG EventMask,
    PULONG pEventFlags
    );


BOOLEAN WINAPI
WinStationShadow(
    HANDLE hServer,
    PWSTR pTargetServerName,
    ULONG TargetLogonId,
    BYTE HotkeyVk,
    USHORT HotkeyModifiers
    );


typedef BOOLEAN (WINAPI * PWINSTATIONFREEMEMORY)( PVOID );

BOOLEAN WINAPI
WinStationFreeMemory(
    PVOID  pBuffer
    );

BOOLEAN WINAPI
WinStationTerminateProcess(
    HANDLE hServer,
    ULONG ProcessId,
    ULONG ExitCode
    );


BOOLEAN WINAPI
_WinStationCallback(
    HANDLE hServer,
    ULONG LogonId,
    LPWSTR pPhoneNumber
    );

BOOLEAN WINAPI
_WinStationBreakPoint(
    HANDLE hServer,
    ULONG LogonId,
    BOOLEAN KernelFlag
    );

BOOLEAN WINAPI
_WinStationReadRegistry(
    HANDLE  hServer
    );


BOOLEAN WINAPI
_WinStationWaitForConnect(
    VOID
    );


BOOLEAN WINAPI
_WinStationNotifyLogon(
    BOOLEAN fUserIsAdmin,
    HANDLE UserToken,
    PWCHAR pDomain,
    PWCHAR pUserName,
    PWCHAR pPassword,
    UCHAR Seed,
    PUSERCONFIGW pUserConfig
    );


BOOLEAN WINAPI
_WinStationNotifyLogoff(
    VOID
    );

BOOLEAN WINAPI
_WinStationGetApplicationInfo(
    HANDLE hServer,
    ULONG  LogonId,
    PBOOLEAN pfPublished,
    PBOOLEAN pfAnonymous
    );

BOOLEAN WINAPI
_WinStationCheckForApplicationName(
    HANDLE hServer,
    ULONG  LogonId,
    PWCHAR pUserName,
    ULONG  UserNameSize,
    PWCHAR pDomain,
    ULONG  DomainSize,
    PWCHAR pPassword,
    ULONG  *pPasswordSize,
    ULONG  MaxPasswordSize,
    PCHAR  pSeed,
    PBOOLEAN pfPublished,
    PBOOLEAN pfAnonymous
    );


/*******************************************************************
 *  WinStation APIs for Generic Licensing
 *  (The WinStation extension DLL determines actual implementation)
 *******************************************************************/
// BUGBUG: need to add a license type parameter to each of these APIs
//         so that ICASRV knows which extension DLL to call

BOOLEAN WINAPI
WinStationGenerateLicense(
    HANDLE hServer,
    PWCHAR pSerialNumberString,
    PVOID pLicense,
    ULONG LicenseSize
    );

BOOLEAN WINAPI
WinStationInstallLicense(
    HANDLE hServer,
    PVOID pLicense,
    ULONG LicenseSize
    );

typedef BOOLEAN (WINAPI * PWINSTATIONENUMERATELICENSES)( HANDLE, PCHAR *, ULONG * );

BOOLEAN WINAPI
WinStationEnumerateLicenses(
    HANDLE hServer,
    PVOID *ppLicense,
    ULONG  *pEntries
    );

BOOLEAN WINAPI
WinStationActivateLicense(
    HANDLE hServer,
    PVOID pLicense,
    ULONG  LicenseSize,
    PWCHAR pActivationCode
    );

BOOLEAN WINAPI
WinStationRemoveLicense(
    HANDLE hServer,
    PVOID pLicense,
    ULONG  LicenseSize
    );

BOOLEAN WINAPI
WinStationSetPoolCount(
    HANDLE hServer,
    PVOID pLicense,
    ULONG  LicenseSize
    );


BOOLEAN WINAPI
WinStationQueryLicense(
    HANDLE hServer,
    PVOID pLicenseCounts,
    ULONG ByteCount
    );

BOOLEAN WINAPI
WinStationQueryUpdateRequired(
    HANDLE hServer,
    PULONG pUpdateFlag
    );

BOOLEAN WINAPI
_WinStationAnnoyancePopup(
    HANDLE hServer,
    ULONG LogonId
    );



#ifdef __cplusplus
}
#endif

#endif  /* !_INC_WINSTAH */
