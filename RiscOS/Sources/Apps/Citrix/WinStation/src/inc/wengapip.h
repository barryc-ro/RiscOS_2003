/******************************************************************************
*
*   WENGAPIP.H
*
*   Thin Wire Windows - wengine private header
*
*   Copyright Citrix Systems Inc. 1995-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*  
*     Rev 1.50   09 Jul 1997 15:50:10   davidp
*  Added support for setting clip region
*  
*     Rev 1.49   15 Apr 1997 18:46:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.49   09 Apr 1997 15:56:26   BillG
*  update
*  
*  
*     Rev 1.48   04 Mar 1997 17:41:14   terryt
*  client shift states
*  
*     Rev 1.47   20 Feb 1997 14:06:30   butchd
*  update
*  
*     Rev 1.46   05 Nov 1996 17:28:12   jeffm
*  ICA open to version 4
*  
*     Rev 1.45   16 Jul 1996 10:29:38   marcb
*  update
*  
*     Rev 1.44   12 Jun 1996 14:16:48   marcb
*  update
*  
*     Rev 1.43   01 Jun 1996 12:15:26   unknown
*  
*     Rev 1.42   06 May 1996 19:19:18   jeffm
*  update
*  
*     Rev 1.41   27 Apr 1996 16:05:32   andys
*  soft keyboard
*  
*     Rev 1.40   04 Jan 1996 10:55:22   kurtp
*  update
*  
*******************************************************************************/
#ifndef __WENGAPIP_H__
#define __WENGAPIP_H__

#ifdef WIN16

#define WFCCALLBACK CALLBACK _loadds
#endif
#ifdef WIN32
#define WFCCALLBACK CALLBACK
#endif

/*
 * Internal wengine states
 */
#define WFES_LOADEDSESSION 0x0001
#define WFES_LOADEDWD      0x0002
#define WFES_LOADEDPD      0x0004
#define WFES_LOADEDVD      0x0008
#define WFES_CONNECTED     0x0010
#define WFES_FOCUS         0x0020
#define WFES_WINDOW        0x0040
#define WFES_BADSTATUS     0x0100

typedef struct _WFEINSTANCE {
    HWND hWndPlugin;  // NULL for standalone window, browser window for plugin/OCX
} WFEINSTANCE, FAR *PWFEINSTANCE;

extern DLLLINK        gWdLink;
extern DLLLINK        gPdLink;
extern PDLLLINK *     gpaVdLink;
extern USHORT         gMaxVirtualChannels;
extern PDOPEN         gPdOpen;
extern BOOL           gbVdAddrToWd;
extern USHORT         gState;
#ifdef DOS
extern PFNUIPOLL      gpfnUIPoll;
#endif
extern CLIENTNAME     gpszClientname;
extern DESCRIPTION    gpszTitle;
extern ULONG          gVersion;


/*
 *  Window extra data defines
 */
#define     GWL_INSTANCEDATA    0
#define     GWL_FSTEXTBUFFER    4
#define     GWL_WINDOWWIDTH     8
#define     GWL_WINDOWHEIGHT   12
#define     GWL_CXBORDER       16
#define     GWL_CYBORDER       20
#define     GWL_TITLEPRESENT   24
#define     GWL_CYTITLE        28
#define     GWL_MOUSE_X        32
#define     GWL_MOUSE_Y        36
#define     GWL_RESERVEDBYTES  40       //  must be last GWL_ plus 4


/*
 * Functions in window.c
 */
HWND CreateMainWindow( HINSTANCE hInstance, INT nCmdShow,
                       PWFEOPEN pWFEOpen );
VOID FAR *WinAlloc( UINT cb );
VOID FAR *WinReAlloc( VOID FAR *pMem, UINT cb );
VOID WinFree( VOID FAR *pMem );
#ifndef WIN32
DWORD GetLastError( VOID );
#endif
#ifdef WIN16
BOOL Beep( DWORD dwFreq, DWORD dwDuration );
#endif

/*
 * Functions in dosonly.c/winonly.c
 */
INT InputPoll( VOID );
LRESULT Vio_Init( HWND hWnd, INT *pheight, INT *pwidth, BOOL fTTY );

/*
 * Functions in wfckey
 */
typedef BOOL (PWFCAPI PFNWKEYBOARDHOOKPROC)( INT code, WPARAM wParam, LPARAM lParam );
BOOL WFCAPI WFCKeyHook( PPLIBPROCEDURE pLogProcs,
                        PFNWKEYBOARDHOOKPROC pfnwKeyboardHookProc );
VOID WFCAPI WFCKeyUnhook( VOID );

/*
 * Functions in worker.c
 */
INT wdSetDefaultMode( PWFEDEFAULTMODE pDefaultMode, USHORT cbDefaultMode );
INT wdSetProductID( PWFEPRODUCTID pProductID, USHORT cbProductID );
INT wdSetInfoPassthru( USHORT wdInfoClass );
INT wdSetFocus( VOID );
INT wdKillFocus( VOID );
INT wdWindowSwitch( VOID );
INT wdVdAddress( VOID );
INT wdConnect( HWND hWnd  );
INT wdDisconnect( VOID );
INT wdCharCode( PUCHAR pCharCodes, USHORT cCharCodes );
INT wdScanCode( PUCHAR pScanCodes, USHORT cScanCodes );
INT wdMouseData( LPVOID pMouseData, USHORT cMouseData );
INT wdAddReadHook( PVOID ModemReadHook );
INT wdRemoveReadHook( VOID );
INT wdLastError( PWFELASTERROR pLastError, UINT cb );
INT wdLoadBalance( PLOADBALANCE pLoadBalance, UINT cb );
INT vdThinwireCache( PVDTWCACHE pCache, UINT cb );
INT wdInitWindow( HWND hWnd );
INT wdDestroyWindow( HWND hWnd );
INT wdPaint( HWND hWnd );
INT wdThinwireStack( PVDTWSTACK pTWStack  );
INT wdEncryptionInit( PENCRYPTIONINIT pEncryptionInit, UINT cb );
INT vdInfo( PWFEVDINFO pVdInfo, BOOL fSet );
INT wdGetClientDataServer(LPSTR pServer, USHORT Length);
INT wdGetClientDataDomain(LPSTR pDomain, USHORT Length);
INT wdGetClientDataUsername(LPSTR pUsername, USHORT Length);

/*
 * Functions in wengine.c
 */
#define WFENG_API_VERSION_LOW  1L
#define WFENG_API_VERSION_HIGH 4L        // WinFrame Client 2.0

FNWFENGOPEN             srvWFEngOpen;
FNWFENGCLOSE            srvWFEngClose;
FNWFENGCONNECT          srvWFEngConnect;
FNWFENGDISCONNECT       srvWFEngDisconnect;
FNWFENGLOADSESSION      srvWFEngLoadSession;
FNWFENGLOADWD           srvWFEngLoadWd;
FNWFENGLOADVD           srvWFEngLoadVd;
FNWFENGLOADPD           srvWFEngLoadPd;
FNWFENGUNLOADDRIVERS    srvWFEngUnloadDrivers;
FNWFENGSETINFORMATION   srvWFEngSetInformation;
FNWFENGQUERYINFORMATION srvWFEngQueryInformation;
FNWFENGPOLL             srvWFEngPoll;
FNWFENGMESSAGELOOP      srvWFEngMessageLoop;
FNWFENGLOAD             srvWFEngLoad;
FNWFENGUNLOAD           srvWFEngUnload;
FNWFENGLOGSTRING        srvWFEngLogString;
LRESULT WFCAPI StatusMsgProc( HANDLE hWFE, INT message );

#if !defined(DOS) && !defined(RISCOS)
VOID ToggleWindowDressing( HWND hWnd );


/*
 * Ipc stuff
 */
typedef ULONG HIPCSZ;
typedef struct _IPCINST {
   PFNUIPOLL  pfnPoll;
   WNDPROC    pfnWndProc;
   HWND       hwndService; // For now only support one service per instance
#ifdef WIN32
   HANDLE     hProcess;    // this process handle
#endif
#ifdef DEBUG
   char       pszService[50];
#endif
} IPCINST, FAR *HIPCINST;

typedef struct _IPCCONV {
   HIPCINST   idInst;
   HWND       hwndService; // service ipc window handle
   HIPCSZ     hszTopic;
#ifdef WIN32
   HANDLE     hProcess;    // other service process handle
#endif
} IPCCONV, FAR *HIPCCONV;

typedef struct _IPCDATA {
   HIPCINST   idInst;
#ifdef WIN32
   HANDLE     hGlobal;
   HANDLE     hGlobalDup;
   LPVOID     pData;
#else
   HGLOBAL    hGlobal;
#define hGlobalDup hGlobal
#endif
} IPCDATA, FAR *HIPCDATA;

/* #pragma pack(1) */
typedef struct _IPCDATAPTR {
    HWND  hwndTransmitter;
#ifdef WIN32
    HANDLE hProcessTransmitter;
#endif
    ULONG cbData;
    BYTE data[1];                     // must be last in structure
} IPCDATAPTR, FAR *PIPCDATAPTR;
/* #pragma pack() */

/*=============================================================================
 ==  Registered message names for plugin messages from engine
 ============================================================================*/
#define WFICA_MSGNAME_CALLBACK_STATUS "WFEngineCallBackStatus" // engine status   name
#define WFICA_MSGNAME_ENGINE_TEXT     "WFEngineText"           // error/status message
#define WFICA_MSGNAME_ENGINE_INIT     "WFEngineInit"           // init: engine to plugin
#define WFICA_MSGNAME_ENGINE_UNINIT   "WFEngineUninit"         // uninit: plugin to engine
#define WFICA_MSGNAME_NEW_HWND        "WFEngineNewhWnd"        // plugin to engine: new hWnd
#define WFICA_MSGNAME_SET_CLIP_LR     "WFEngineNewhWnd"        // plugin to engine: new cliprect
#define WFICA_MSGNAME_SET_CLIP_TB     "WFEngineNewhWnd"        // plugin to engine: new cliprect

#define WFENG_SERVICEUI                  "WFServiceUI"
#define WFENG_NONIPCCLASS                "WFIcaClient"
#define WFENG_SERVICECONNECTION          "WFService%08lX%03u"
#define LENGTH_SERVICECONNECTION         20           // WFService00000000xxx
#define LENGTH_SERVICECONNECTIONBASE     9            // WFService
#define LENGTH_SERVICECONNECTIONBASEINST 17           // WFService00000000
#define WFENG_TOPIC                      "APIThunk"
#define WFENG_OPEN                       "WFEngOpen"
#define WFENG_CLOSE                      "WFEngClose"
#define WFENG_CONNECT                    "WFEngConnect"
#define WFENG_DISCONNECT                 "WFEngDisconnect"
#define WFENG_LOADSESSION                "WFEngLoadSession"
#define WFENG_LOADWD                     "WFEngLoadWd"
#define WFENG_LOADVD                     "WFEngLoadVd"
#define WFENG_LOADPD                     "WFEngLoadPd"
#define WFENG_UNLOADDRIVERS              "WFEngUnloadDrivers"
#define WFENG_SETINFORMATION             "WFEngSetInformation"
#define WFENG_QUERYINFORMATION           "WFEngQueryInformation"
#define WFENG_LOGSTRING                  "WFEngLogString"
#define WFENG_STATUSMSGPROC              "StatusMsgProc"

#define WFENG_PROCESS_MSG_COUNT 10 // Process up to 10 posted messages at a time

#define WFENG_IPC_VERSION                0xAC01  // Used as wParam in IPC messages
                                                 // as both a signiature and a version
                                                 // identifier.
#define WM_WFENG_WAKEUP                  (WM_WFENG_USER-1)
#define WM_WFENG_IPC_CONNECT             (WM_WFENG_USER-2)
#define WM_WFENG_IPC_CONNECT_REPLY       (WM_WFENG_USER-3)
#define WM_WFENG_IPC_DISCONNECT          (WM_WFENG_USER-4)
#define WM_WFENG_IPC_DISCONNECT_REPLY    (WM_WFENG_USER-5)
#define WM_WFENG_IPC_POKE                (WM_WFENG_USER-6)
#define WM_WFENG_IPC_POKE_REPLY          (WM_WFENG_USER-7)
#define WM_WFENG_IPC_POKE_ACK            (WM_WFENG_USER-8)
#define WM_WFENG_DESTROY                 (WM_WFENG_USER-9)

/*
 * Generic thunk parameter block
 */
typedef struct _WFEIPC {
    ULONG  rc;                  // Returned by Ipc server - Must be first in structure
    HANDLE hWFE;                // Passed to Ipc server (returned via WFEngOpen)
    ULONG  cbData;              // Total size of data block
} WFEIPC, FAR *PWFEIPC;

/*
 *
 * WM_WFENG_IPC_CONNECT lParam data block
 */
typedef struct _IPCCONNECT {
   WFEIPC x;                          // Must be first in structure
   HIPCSZ hszService;
   HIPCSZ hszTopic;
} IPCCONNECT, FAR *PIPCCONNECT;

/*
 *
 * WM_WFENG_IPC_POKE lParam data block
 */
typedef struct _IPCPOKE {
   WFEIPC   x;                          // Must be first in structure
   HIPCSZ   hszTopic;
   HIPCSZ   hszItem;
   HANDLE   hGlobal;
} IPCPOKE, FAR *PIPCPOKE;


/*
 * WFEngOpen thunk parameter block
 */
typedef struct _WFEIPCOPEN {
   WFEIPC x;                          // Must be first in structure
   ULONG          Version;
   BOOL           fStatusMsgProc;
   CLIENTNAME     pszClientname;
   DESCRIPTION    pszTitle;
   USHORT         uInitialWidth;
   USHORT         uInitialHeight;
   ULONG          ulLogClass;
   ULONG          ulLogEvent;
   CLIENTSN       pszClientSN;
   FILEPATH       pszIconPath;
   USHORT         uIconIndex;
} WFEIPCOPEN, FAR *PWFEIPCOPEN;

/*
 * WFEngLoadxxx thunk parameter block
 */
typedef struct _WFEIPCLOADMOD {
   WFEIPC   x;                          // Must be first in structure
   HANDLE   handle;
   FILEPATH pszModuleName;
   BYTE     IniSection[1];              // Must be last in structure
} WFEIPCLOADMOD, FAR *PWFEIPCLOADMOD;

/*
 * WFEngSet/QueryInformation thunk parameter block
 */
typedef struct _WFEIPCINFORMATION {
   WFEIPC       x;                    // Must be first in structure
   WFEINFOCLASS type;
   UINT         cbData;
   BYTE         Data[1];              // Must be last in structure
} WFEIPCINFORMATION, FAR *PWFEIPCINFORMATION;

/*
 * StatusMsgProc thunk parameter block
 */
typedef enum _WFELPARAMTYPE {
    NoParam,
    ByteParam,
    WordParam,
    DWordParam,
    DataParam,
} WFELPARAMTYPE;

typedef struct _WFEIPCSTATUSMSG {
   WFEIPC        x;                   // Must be first in structure
   INT           message;
   WFELPARAMTYPE typelParam;
   UINT          cblParam;
   BYTE          lParam[1];    // Must be last in structure
} WFEIPCSTATUSMSG, FAR *PWFEIPCSTATUSMSG;

/*
 * WFEngLogString thunk parameter block
 */
typedef struct _WFEIPCLOGSTRING {
   WFEIPC       x;                    // Must be first in structure
   ULONG        ulLogClass;
   ULONG        ulLogEvent;
   BYTE         Data[1];              // Must be last in structure
} WFEIPCLOGSTRING, FAR *PWFEIPCLOGSTRING;

/*
 * WFEngOpen private parameter block
 */
typedef struct _WFEOPENP {
   HWND         hWndPlugin;
} WFEOPENP, FAR *PWFEOPENP;

/*
 * Functions in IPCCLIEN.C
 */
#define WFIPC_NORMAL       0x0000
#define WFIPC_WAITFORREPLY 0x0001
ULONG IpcThunkBegin( HIPCINST idInst, HIPCCONV hConv, LPSTR pszItem,
                     PWFEIPC * ppData, UINT cbData, HIPCDATA * phData, USHORT Flags );
void IpcThunkEnd( HIPCDATA hData );
void IpcInit( VOID );
UINT IniSize( LPSTR pIniSection );
typedef ULONG (*PFNSERVICEROUTINE)( PWFEIPC pData, BOOL * pfReply );
PFNSERVICEROUTINE GetServiceRoutine( HIPCSZ hszTopic, HIPCSZ hszItem );
VOID IpcCacheReply( HIPCDATA hDataReply );
HIPCDATA IpcGetReply( HIPCINST idInst, HIPCSZ hszItem );
UINT IpcInitialize( HIPCINST FAR* pidInst, WNDPROC pfnWndProc, PFNUIPOLL pfnPoll );
BOOL IpcUninitialize( HIPCINST idInst );
HIPCCONV IpcConnect( HIPCINST idInst, LPSTR pszService, LPSTR pszTopic );
BOOL IpcDisconnect( HIPCCONV hConv );
BOOL IpcClientTransaction( HIPCDATA pData, HIPCCONV hConv, LPSTR pszItem, BOOL fReply );
HIPCDATA IpcCreateDataHandle( HIPCCONV hConv, void FAR* pSrc, DWORD cb );
HIPCDATA IpcDupDataHandle( HIPCINST idInst, HANDLE hGlobal );
BOOL IpcCloseDupDataHandle( HIPCDATA hData );
BOOL IpcFreeDataHandle( HIPCDATA hData );
LPBYTE IpcAccessData( HIPCDATA hData );
BOOL IpcUnaccessData( HIPCDATA hData );
HIPCSZ IpcCreateStringHandle( HIPCINST idInst, LPCSTR psz );
DWORD IpcQueryString( HIPCINST idInst, HIPCSZ hsz, LPSTR psz, DWORD cchMax );
BOOL IpcFreeStringHandle( HIPCINST idInst, HIPCSZ hsz );
LRESULT WFCCALLBACK IpcWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
void IpcCleanup( HIPCINST *pidInst, HIPCCONV *phConv );
BOOL IpcNameService( HIPCINST idInst, LPSTR pszService );
void IpcReply( HIPCINST idInst, PWFEIPC pData, UINT message, ULONG rc );

ULONG WmWFEngIpcConnect( HIPCINST idInst, PIPCCONNECT pData, LPSTR pszService,
                         LPSTR pszTopic, INT cbTopic );
ULONG WmWFEngIpcPoke( HIPCINST idInst, PIPCPOKE pPokeData, BOOL * pfReply );
VOID ipcDestroyWindow( VOID );
BOOL  IpcPostMessage( HIPCCONV hConv, UINT message, HIPCDATA hData );
#ifdef DEBUG
#define SERVICE_NAME( p ) (p ? (p->pszService) : "")
#endif
#endif /* #ifndef DOS */

#endif // __WENGAPIP_H__
