/*****************************************************************************
*
*   ICAWIN.C
*
*   Embeded server API for Windows client engine
*
****************************************************************************/

#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/client.h"
#include "../../inc/clib.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/vioapi.h"
#include "../../inc/logapi.h"
#include "../../inc/cfgini.h"
#include "../../inc/cfgload.h"
#include "../../inc/biniapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/timapi.h"
#include "../../inc/kbdapi.h"
// #include "../../inc/xmsapi.h"
#include "../../inc/encrypt.h"
#include "../../inc/dll.h"
// #include "../../inc/setarg.h"
// #include "resource.h"

#include "citrix/ctxver.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#include <math.h>

/*=============================================================================
==  External Vars
=============================================================================*/
extern USHORT         gState;
extern FILEPATH       gszLoadDllFileName;        // name of last DLL loaded
extern int            argc;
extern char         **argv;
extern FILEPATH       g_szICAFileOrLabel;
extern USERNAME       g_szUserName;
extern char           g_szDomain[];
extern PASSWORD       g_szPassword;
extern INITIALPROGRAM g_szCmdLine;
extern DIRECTORY      g_szWorkDir;
extern HWND           g_hWndPlugin;
extern ULONG          g_HRes;
extern ULONG          g_VRes;
extern ULONG          gulLogClass;
extern ULONG          gulLogEvent;
extern HWND           ghWFE;
#ifdef WIN32
extern BOOL           fDragOn;
#endif

z/*=============================================================================
==   Local defines
=============================================================================*/
#define NUM_BLIPS           4                    // number of flag icons
#define BLIP_MSEC         150                    // time between waving flag icons
#define MAX_ERRORMSG      500                    // maximum error msg length
#define MAX_TITLE         100                    // maximum title length
#define ENV_CLIENTNAME    "CLIENTNAME"           // CLIENTNAME environment name

/*=============================================================================
==   Local Vars
=============================================================================*/

static HWND  hDlg=NULL;
HWND hSecDlg=NULL;              // security modeless dialog
static int gBlipTimer=0;        // blip timer handler for waving flags

int gnBlip=0;       // starting blip number

#ifdef WIN16
static FARPROC lpfnStatDlgProc=NULL;
static FARPROC lpfnFileSecProc=NULL;
#endif

static BOOL         bPDError=FALSE;
static FILEPATH     gszExeDir;
static FILEPATH     gszModuleIni;
extern FILEPATH     gszExeName;
static FILEPATH     gszWfclientIni;
static FILEPATH     gszICAFile;
       CLIENTNAME   gszClientName;
DESCRIPTION  gszServerLabel;
static HANDLE hWFE = NULL;

ENCRYPTIONLEVEL g_szEncryptionDLL = "";

static char         g_szHRes[5] = "";
static char         g_szVRes[5] = "";
static char         g_szScreenPercent[5] = "";

typedef struct _OVERRIDELIST {
    char *pszKey; 
    char *pszValue;
} OVERRIDELIST, * POVERRIDELIST;


static ENCRYPTIONLEVEL szEncryptionDLL;
static char szClientType[15];

#define OVERRIDE_COUNT 12
static OVERRIDELIST g_pOverrides[OVERRIDE_COUNT] = {
                    { INI_USERNAME,       g_szUserName     },
                    { INI_DOMAIN,         g_szDomain       },
                    { INI_PASSWORD,       g_szPassword     },
                    { INI_INITIALPROGRAM, g_szCmdLine      },
                    { INI_WORKDIRECTORY,  g_szWorkDir      },
// BUGBUG TBD       { INI_PHONENUMBER,    g_szPhoneNumber  },  requires input dlg
                    { INI_ENCRYPTIONDLL,  szEncryptionDLL },
                    { INI_CLIENTTYPE,     szClientType},
                    { INI_ENCRYPTIONLEVELSESSION, g_szEncryptionDLL},
                    { INI_DESIREDHRES,    g_szHRes         },
                    { INI_DESIREDVRES,    g_szVRes         },
                    { INI_SCREENPERCENT,  g_szScreenPercent},
                    { NULL,               NULL,            }
                  };

/*=============================================================================
==   Global Vars
=============================================================================*/
HINSTANCE ghInstance;
HINSTANCE ghPrevInstance;
UINT      g_MsgPlugin=0;
UINT      g_MsgEngineText=0;
UINT      g_MsgEngineInit=0;
UINT      g_MsgEngineUninit=0;
UINT      g_MsgNewhWnd=0;

/*=============================================================================
==   Local Functions
=============================================================================*/
static int  EMEngOpen(LPHANDLE phWFE);
static int  EMEngLoadSession(HANDLE hWFE);
static int  EMErrorPopup(int iError);
int  EMErrorResIDPopup(int iResID, UINT MsgType);
int  EMErrorResIDPopup2(int iResID, int iResID2, UINT MsgType);
static BOOL EMGetErrorMessage( int iErrorCode, LPSTR chBuffer,int cbBuffSize);
static int  EMStatusMessage(int iStatus);
static int  EMStatusDialog(int iStatus);
static int  EMKillStatusDlg(void);
#ifdef DEBUG
static int  EMLogInit(void);
#endif
static void EMFileSecurityPopup(void);
static void EMConnectStatusPopup(void);
void EMSetFileSecurity( WPARAM wFileSec);

/*=============================================================================
==   Global Functions
=============================================================================*/
BOOL CenterPopup( HWND, HWND );
int  EMGetStringResource(int iResID, LPSTR szString, int cbStr);
BOOL CALLBACK StatDialogProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK StatDialogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime);
BOOL CALLBACK FileSecDlgProc(HWND, UINT, WPARAM, LPARAM);

/*=============================================================================
==   External Functions
=============================================================================*/
INT wWFEngPoll( VOID );


/*
 * EMWinMain
 *
 * Main entry point into our stand-alone EXE for ICA file processing.
 * This gets called from WinMain depending on if we are doing ICA files
 * or the old WFENG?.EXE thing.  WinMain lives in IPCENG.C.
 *
 */
INT WINAPI EMWinMain(LPSTR lpCmdLine,
                     INT nCmdShow)
{
    MSG msg;                                 /* message                      */
    INT rcMsg = FALSE;
    INT rc;
    int cbLen;
    char *pEnvVar;

#ifdef DEBUG
    // initialize the LOG file
    EMLogInit();
#endif

  // register messages for plugin
  if(g_hWndPlugin) {
     g_MsgPlugin = RegisterWindowMessage(WFICA_MSGNAME_CALLBACK_STATUS);
     g_MsgEngineText = RegisterWindowMessage(WFICA_MSGNAME_ENGINE_TEXT);
     g_MsgEngineInit = RegisterWindowMessage(WFICA_MSGNAME_ENGINE_INIT);
     g_MsgEngineUninit = RegisterWindowMessage(WFICA_MSGNAME_ENGINE_UNINIT);
     g_MsgNewhWnd = RegisterWindowMessage(WFICA_MSGNAME_NEW_HWND);
     }

   {
   char *p;
   lstrcpy( gszExeDir, gszExeName);
   p = strrchr( gszExeDir, '\\');
   if(p) {
      *(p+1) = '\0';
      lstrcpy( gszModuleIni,gszExeDir);
      lstrcpy( gszWfclientIni,gszExeDir);
      lstrcat( gszModuleIni,DEF_PROTOCOL_FILE);
      lstrcat( gszWfclientIni,DEF_CONFIG_FILE);
      }
   else {
      EMErrorResIDPopup(IDS_CLIENT_ERROR_NOEXEDIR, MB_ICONSTOP);
#ifdef DEBUG
      LogClose();
#endif
      return(0);
      }
   }

   // get name of ICA file
   _fullpath(gszICAFile, g_szICAFileOrLabel, sizeof(gszICAFile));
   if(_access(gszICAFile,0)!=0) {
      EMErrorResIDPopup(IDS_CLIENT_ERROR_ICA_FILE_NOT_FOUND, MB_ICONSTOP);
      return(0);
      }

   // get the first server listing in the ICA file
   gszServerLabel[0] = '\0';
   GetPrivateProfileString( INI_APPSERVERLIST,
                            NULL, 
                            "",
                            gszServerLabel,
                            sizeof(gszServerLabel),
                            gszICAFile );

   GetPrivateProfileString(gszServerLabel, INI_ENCRYPTIONLEVELSESSION, 
                          "",
                          szEncryptionDLL, sizeof(szEncryptionDLL),
                          gszICAFile);
   if(gszServerLabel[0] == '\0') {
      EMErrorResIDPopup(IDS_CLIENT_ERROR_SERVER_SECTION_NOT_FOUND,MB_ICONSTOP);
#ifdef DEBUG
      LogClose();
#endif
      return(1);
      }

    // determine the client name from environment variable first
    // then use Win32 computername and then the default from the
    // resource file
    pEnvVar = getenv(ENV_CLIENTNAME);
    if(pEnvVar == NULL) {
       cbLen = sizeof(gszClientName);
#ifdef WIN32
       if(GetComputerName(gszClientName,&cbLen)==FALSE)
#endif   
        {    
          // use time stampe to creat an unique client name

          ULONG timer;
          timer=Getmsec();
          wsprintf(gszClientName,"Internet%08lu", (ULONG) (timer & 0xffffff));
          //  EMGetStringResource(IDS_INTERNET_CLIENT_NAME,gszClientName,sizeof(gszClientName));
        }
       }
    else
       lstrcpy(gszClientName,pEnvVar);

    // start the connect status dialog 
    if(!g_hWndPlugin)
       EMConnectStatusPopup();
   
    EMStatusDialog(CLIENT_STATUS_LOADING_STACK);
    rc = srvWFEngLoad( NULL, NULL );  // Call the WFEngLoad API
    if(rc != CLIENT_STATUS_SUCCESS) {
       EMKillStatusDlg();
       EMErrorPopup(rc);
#ifdef DEBUG
       LogClose();
#endif
       return(1);
       }

    // open the engine
    rc = EMEngOpen(&hWFE);
    if(rc != CLIENT_STATUS_SUCCESS) {
       EMKillStatusDlg();
       EMErrorPopup(rc);
#ifdef DEBUG
       LogClose();
#endif
       return(1);
       }

    // load the session
    rc = EMEngLoadSession(hWFE);
    if(rc != CLIENT_STATUS_SUCCESS) {
       EMKillStatusDlg();
       EMErrorPopup(rc);
#ifdef DEBUG
       LogClose();
#endif
       return(1);
       }

    // set the product id to Internet client
    {
       WFEPRODUCTID WFEProductID;

       // If the module.ini file exists then it's not the internet client
       if (_access(gszModuleIni,0) == 0) {
           WFEProductID.ProductID = CLIENTID_CITRIX_DOS;
       } else {
           WFEProductID.ProductID = CLIENTID_CITRIX_INTERNET;
       }

       wdSetProductID( &WFEProductID, sizeof(WFEProductID));
    }


    // update the status to connecting
    EMStatusDialog(CLIENT_STATUS_CONNECTING);

    rc = srvWFEngConnect(hWFE);
    if(rc != CLIENT_STATUS_SUCCESS) {
       EMKillStatusDlg();
       EMErrorPopup(rc);
#ifdef DEBUG
       LogClose();
#endif
       return(1);
       }


    while( GetMessage( &msg, NULL, 0, 0 ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
        if ( gState & WFES_CONNECTED ) {
           srvWFEngMessageLoop( hWFE );
           break;
        }
    }

    rcMsg = msg.wParam;

    /*
     * These APIs are called automatically on exit
     * (The APIs called by the UI are handled in WFCWINx.DLL)
     */
    srvWFEngUnloadDrivers( hWFE );
    srvWFEngClose( hWFE );
    srvWFEngUnload( (PMINIDLL)NULL );  // Call the WFEngUnload API
#ifdef DEBUG
    LogClose();
#endif
    EMKillStatusDlg();

    return( rcMsg );
}

/*******************************************************************************
 *
 *  WFEngineStatusCallback - helper function
 *
 *      Callback procedure for WFEngine to return dynamic status.
 *
 *  ENTRY:
 *      hWFE (input)
 *          WFEngine instance handle who is calling back.
 *      message (input)
 *          Callback message.
 *      lParam (input)
 *          Optional LPARAM associated with message.
 *  EXIT:
 *      LRESULT -- message specific return code
 *
 ******************************************************************************/

LRESULT WFCAPI
WFEngineStatusCallback( HANDLE hWFE, INT message, LPARAM lParam )
{
    int rc;
    LRESULT lResult = 0;

    // send to plug in if available
    if(g_hWndPlugin && g_MsgPlugin) {
       SendMessage( g_hWndPlugin, g_MsgPlugin, message, lParam);
       }

    switch ( message ) {

        case CLIENT_STATUS_CONNECTION_BROKEN_HOST:
        case CLIENT_STATUS_CONNECTION_BROKEN_CLIENT:

            if ( (rc = srvWFEngUnloadDrivers( hWFE )) != CLIENT_STATUS_SUCCESS ) {
    
            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGUNLOADDRIVERS);
            }
            if ( (rc = srvWFEngClose( hWFE )) != CLIENT_STATUS_SUCCESS ) {

            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGCLOSE);
            }
#ifdef DEBUG
            LogPrintf( LOG_CLASS, LOG_CONNECT, "DISCONNECTED from %s", gszServerLabel);
#endif
            break;

        case CLIENT_STATUS_MODEM_INIT:
        case CLIENT_STATUS_MODEM_DIALING:
        case CLIENT_STATUS_MODEM_WAITING:
        case CLIENT_STATUS_MODEM_NO_RESPONSE:
        case CLIENT_STATUS_MODEM_ERROR:
        case CLIENT_STATUS_MODEM_NO_DIAL_TONE:
        case CLIENT_STATUS_MODEM_REDIALING:
        case CLIENT_STATUS_MODEM_VOICE:
        case CLIENT_STATUS_MODEM_BUSY:
        case CLIENT_STATUS_MODEM_TERMINATE:
        case CLIENT_STATUS_MODEM_TIMEOUT:
        case CLIENT_STATUS_MODEM_OUT_OF_RETRIES:
            break;

        case CLIENT_STATUS_QUERY_CLOSE: // ask user to close or not
            lResult = EMStatusMessage(message);
            break;


        case CLIENT_STATUS_HOTKEY1:     // invoke local task list
            break;

        case CLIENT_STATUS_DELETE_CONNECT_DIALOG:
            EMKillStatusDlg();
            break;

        case CLIENT_STATUS_CONNECTED:
#ifdef DEBUG
            LogPrintf( LOG_CLASS, LOG_CONNECT, "CONNECTED to %s", gszServerLabel);
#endif
            break;

        case CLIENT_STATUS_KILL_FOCUS:  // ignore these messages
            break;

        case CLIENT_STATUS_CONNECTING:
            break;

        case CLIENT_STATUS_TTY_CONNECTED:
            break;

        case CLIENT_STATUS_BEEPED:
            break;

        case CLIENT_STATUS_QUERY_ACCESS:
            EMFileSecurityPopup();
            break;


        case CLIENT_ERROR_HOST_NOT_SECURED: 
        case CLIENT_ERROR_VD_ERROR:
        case CLIENT_ERROR_PD_ERROR:
            if(bPDError==FALSE) {
               EMKillStatusDlg();
               EMErrorPopup(message);
               bPDError=TRUE;
               }
            break;

        default:
#ifdef DEBUG
            {
            char szStatus[80];
            wsprintf(szStatus,"Unknown status message (%d)",message);
            MessageBox(hWFE, szStatus, gszServerLabel, MB_OK);
            }
#endif
            break;
    }

    return(lResult);

}  // end WFEngineStatusCallback

/*
 *  EMEngOpen
 *
 *  Start the engine window
 *
 */
static int EMEngOpen(LPHANDLE phWFE)
{
  WFEOPEN  WFEOpen;
  WFEOPENP WFEOpenp;
  INT rc = CLIENT_STATUS_SUCCESS;
  ULONG iHRes,iVRes;
  ULONG iScreenPercent;
#ifdef WIN32
  RECT rWorkArea;
#endif

  if ( g_hWndPlugin ) {
      iHRes = g_HRes;
      iVRes = g_VRes;
      wsprintf( g_szHRes, "%d", iHRes );
      wsprintf( g_szVRes, "%d", iVRes );
      lstrcpy ( g_szScreenPercent,"0");
      TRACE(( TC_WENG, TT_L1,
              "WFEngx.Exe EMEngOpen: hwnd(%X) HRes(%ld) VRes(%ld) szHRes(%s) szVRes(%s)",
               g_hWndPlugin, iHRes, iVRes, g_szHRes, g_szVRes ));
  } else {
      // use Screen Percentage first (only from ICA file)
      iScreenPercent = GetPrivateProfileInt(gszServerLabel,
                                            INI_SCREENPERCENT,
                                            0, 
                                            gszICAFile); 
      
      // validate the screen percentage
      if((iScreenPercent > 100) || (iScreenPercent <1))
         iScreenPercent = 0;
      
      // if using screen percent, calculate based on local screen
      if(iScreenPercent != 0) {
         double xRes, yRes, xyPercent;
//         ULONG xMin,yMin;
//         char szBuf[100];
      
         xyPercent = (double)iScreenPercent / (double)100;
         xyPercent = sqrt(xyPercent);
         xRes = (double)GetSystemMetrics(SM_CXSCREEN);
         yRes = (double)GetSystemMetrics(SM_CYSCREEN);
#ifdef WIN32
         // Get the limits of the "work area".
         if ( SystemParametersInfo( SPI_GETWORKAREA, sizeof(RECT), &rWorkArea, 0)) {
            xRes = (double)rWorkArea.right - rWorkArea.left;
            yRes = (double)rWorkArea.bottom - rWorkArea.top;
            }
#endif
         xRes = xRes * xyPercent;
         yRes = yRes * xyPercent;
      
         // convert back to ULONG HRes and VRes
         iHRes = (ULONG)xRes;
         iVRes = (ULONG)yRes;
      
         // adjust vertical resolution to include title bar
         iVRes = iVRes - (GetSystemMetrics(SM_CYCAPTION)-1);
      
//         wsprintf(szBuf,"Req window x=%u y=%u\n",iHRes, iVRes);
//         MessageBox(NULL, szBuf, "Req Win", MB_OK);
      
         }
      
      // else use the old fashioned DesiredHRes and DesiredVRes
      else {
         // determine the desired size of client window
         iHRes = GetPrivateProfileInt(gszServerLabel, INI_DESIREDHRES,
                                      0, gszICAFile);
         if(iHRes == 0)
             iHRes = GetPrivateProfileInt(INI_VDTW30, INI_DESIREDHRES,
                                          DEF_DESIREDHRES, gszWfclientIni);
         
         iVRes = GetPrivateProfileInt(gszServerLabel, INI_DESIREDVRES,
                                      0, gszICAFile);
         if(iVRes == 0)
             iVRes = GetPrivateProfileInt(INI_VDTW30, INI_DESIREDVRES,
                                          DEF_DESIREDVRES, gszWfclientIni);
         }
  }


  WFEOpen.Version          = WFENG_API_VERSION;
  WFEOpen.pfnStatusMsgProc = &WFEngineStatusCallback;
  WFEOpen.pszClientname    = gszClientName;   
  WFEOpen.uInitialWidth    = (int)iHRes;   
  WFEOpen.uInitialHeight   = (int)iVRes;  
  WFEOpen.pszLogLabel      = "IWFC";     
  WFEOpen.pszTitle         = gszServerLabel;        
  WFEOpen.pszClientSN      = "";                 
  WFEOpen.pszIconPath      = gszExeName;     
  WFEOpen.uIconIndex       = 0;      
  WFEOpen.reserved         = (ULONG)&WFEOpenp;
  WFEOpenp.hWndPlugin      = g_hWndPlugin;

  rc = srvWFEngOpen(&WFEOpen, phWFE);
  return(rc);
}                          

/*
 *  EMEngLoadSession
 *
 *  Start the session
 *
 */
static int EMEngLoadSession(HANDLE hWFE)
{
  INT rc = CLIENT_STATUS_SUCCESS;
  CFGINIOVERRIDE pOverrides[OVERRIDE_COUNT];
  int            i, j;

  /*
   * Set up our command line override structure key and value pointers.
   */

  //g_pOverrides[6].pszValue = szEncryptionDLL;
  //g_pOverrides[7].pszValue = INTERNET_CLIENT;
  strcpy(szClientType, INTERNET_CLIENT);

  for ( i = 0, j = 0; g_pOverrides[i].pszKey; i++ ) {
      if ( *g_pOverrides[i].pszValue ) {
          pOverrides[j].pszValue = g_pOverrides[i].pszValue;
          pOverrides[j++].pszKey = g_pOverrides[i].pszKey;
          TRACE(( TC_WENG, TT_L1,
                  "WFEngx.Exe EMEngLoadSession: Override key(%s) value(%s)",
                   g_pOverrides[i].pszKey,  g_pOverrides[i].pszValue ));
      }
  }
  pOverrides[j].pszValue = NULL;
  pOverrides[j].pszKey   = NULL;


  rc = CfgIniLoad( hWFE,
              gszServerLabel,
              pOverrides,
              gszICAFile,
              gszModuleIni,
              NULL,                     // MODEM file
              gszWfclientIni);

  return(rc);
}

#if 0
/*
 * EMErrorPopup
 *
 * Use the string resources to determine what the error really is.
 * This makes it easy to translate to other languages.
 *
 */
static int EMErrorPopup(int iError)
{
   char szErrorMsg[MAX_ERRORMSG];

   EMGetErrorMessage( iError, szErrorMsg, sizeof(szErrorMsg));
#ifdef WIN32
   if( !g_MsgEngineText)
      MessageBox(hWFE, szErrorMsg, gszServerLabel, MB_OK| MB_ICONSTOP | MB_SETFOREGROUND);
   else {
      ATOM atom;

      // notify the plugin
      if(g_hWndPlugin) {
         atom = GlobalAddAtom(szErrorMsg);
         SendMessage(g_hWndPlugin, g_MsgEngineText, iError, (LPARAM)atom);
         }
      }
#else
   MessageBox(hWFE, szErrorMsg, gszServerLabel, MB_OK| MB_ICONSTOP);
#endif

   LogPrintf( LOG_CLASS, LOG_ERRORS, szErrorMsg);

   return(0);
}

/*
 * EMErrorResPopup
 *
 * Use the string resources to determine what the error really is.
 * This makes it easy to translate to other languages.  This function
 * uses the raw resource ID instead of the WinFrame client return code.
 *
 * It's good for things that don't have a corresponding error code.
 *
 */
int EMErrorResIDPopup(int iResID, UINT MsgType)
{
  char szErrorMsg[MAX_ERRORMSG];

  if(!LoadString(ghInstance, iResID, szErrorMsg, sizeof(szErrorMsg))) {
    /*
     * If we still don't have an error message, fetch the 'no error text
     * available' message and return that.
     */
     if ( LoadString( ghInstance,
                      IDS_NO_ERROR_TEXT_AVAILABLE,
                      szErrorMsg,
                      sizeof(szErrorMsg)) ) {
        }
     else  {  // something is REALLY broken if must hard-code!
        lstrcpy(szErrorMsg, "(no error text available)");
        }
     }

   if(!g_MsgEngineText)
      MessageBox(hWFE, szErrorMsg, gszServerLabel, MB_OK| MsgType);
   else {
      ATOM atom;

      // notify the plugin
      if(g_hWndPlugin) {
         atom = GlobalAddAtom(szErrorMsg);
         SendMessage(g_hWndPlugin, g_MsgEngineText, iResID, (LPARAM)atom);
         }
      }

  LogPrintf( LOG_CLASS, LOG_ERRORS, szErrorMsg);

  return(0);
}

/*
 * EMErrorResPopup2
 *
 * Use the string resources to determine what the error really is.
 * This makes it easy to translate to other languages.  This function
 * uses the raw resource ID instead of the WinFrame client return code.
 *
 * It's good for things that don't have a corresponding error code.
 *
 * This function merges two messages into one.  Used to solve the
 * translation problem with strings which are too long which are split
 * into two.
 *
 */
int EMErrorResIDPopup2(int iResID, int iResID2, UINT MsgType)
{
  char szErrorMsg[MAX_ERRORMSG];
  char szErrorBuf[2*MAX_ERRORMSG];

  if(!LoadString(ghInstance, iResID, szErrorMsg, sizeof(szErrorMsg))) {
    /*
     * If we still don't have an error message, fetch the 'no error text
     * available' message and return that.
     */
     if ( LoadString( ghInstance,
                      IDS_NO_ERROR_TEXT_AVAILABLE,
                      szErrorMsg,
                      sizeof(szErrorMsg)) ) {
        }
     else  {  // something is REALLY broken if must hard-code!
        lstrcpy(szErrorMsg, "(no error text available)");
        }
     }

  lstrcpy(szErrorBuf,szErrorMsg);

  if(!LoadString(ghInstance, iResID2, szErrorMsg, sizeof(szErrorMsg))) {
    /*
     * If we still don't have an error message, fetch the 'no error text
     * available' message and return that.
     */
     if ( LoadString( ghInstance,
                      IDS_NO_ERROR_TEXT_AVAILABLE,
                      szErrorMsg,
                      sizeof(szErrorMsg)) ) {
        }
     else  {  // something is REALLY broken if must hard-code!
        lstrcpy(szErrorMsg, "(no error text available)");
        }
     }

   lstrcat(szErrorBuf,szErrorMsg);

   if(!g_MsgEngineText)
      MessageBox(hWFE, szErrorBuf, gszServerLabel, MB_OK| MsgType);
   else {
      ATOM atom;

      // notify the plugin
      if(g_hWndPlugin) {
         atom = GlobalAddAtom(szErrorMsg);
         SendMessage(g_hWndPlugin, g_MsgEngineText, iResID, (LPARAM)atom);
         }
      }

  LogPrintf( LOG_CLASS, LOG_ERRORS, szErrorMsg);

  return(0);
}

/*******************************************************************************
 *
 *  EMGetErrorMessage - helper function
 *
 *      Return the string associated with the specified error message code.
 *
 *  ENTRY:
 *      iErrorCode (input)
 *          System message code to get string for.
 *      chBuffer (input)
 *          Points to buffer to fill with system message string.
 *      cbBuffSize (input)
 *          Maximum number of characters that can be placed in chBuffer.
 *      hWFEngine (input/optional)
 *          If not NULL, specifies a WFEngine handle for extended runtime
 *          error messages.
 *      lpfnWFEngQueryInformation(input/optional)
 *          If not NULL, specifies pointer (PFNWFENGQUERYINFORMATION) to
 *          WFEngQueryInformation() function for extended runtime error
 *          messages.  Only valid if hWFEngine not NULL.
 *  EXIT:
 *      (BOOL) TRUE if the returned error message contains it's own private
 *              error code (like "IPX Error 1234 ...").  FALSE to instruct
 *              caller to output WFClient error code (if desired).
 *
 *      Note: the total length of chBuffer (including terminating NULL) will
 *      not exceed the size of the internal temporary buffer (Buffer).
 *
 ******************************************************************************/

static BOOL EMGetErrorMessage( int iErrorCode, LPSTR chBuffer,int cbBuffSize)
{
    INT rc, nResourceId;
    WFELASTERROR LastError;
    TRANSPORTNAME pszTransport;
    CHAR szBuffer[MAX_ERRORMSG];
    CHAR strFormatString[MAX_ERRORMSG];
    BOOL bErrorCodeInMessage = FALSE;

    /*
     *  Empty string for default.
     */
    *szBuffer = '\0';

    /*
     * Special case certain error codes.
     */
    switch( (LastError.Error = iErrorCode) ) {

        case CLIENT_ERROR_PD_ERROR:
            if ( hWFE ) {
               /*
                *  Query engine for true error message text
                */
               if ( (rc = srvWFEngQueryInformation( 
                                       hWFE,
                                       WFELastError,
                                       &LastError, 
                                       sizeof(LastError) ))
                                               == CLIENT_STATUS_SUCCESS ) {

                   lstrcpy( szBuffer, LastError.Message );
                   bErrorCodeInMessage = TRUE;
                   }
               }
            break;

        case CLIENT_ERROR_PD_TRANSPORT_UNAVAILABLE:
            {
                LoadString(ghInstance,
                           IDS_PDTRANSPORTUNAVAILABLE_FORMAT,
                           strFormatString,
                           sizeof(strFormatString));
                /*
                 * First find out what transport we're dealing with
                 */
                GetPrivateProfileString( gszServerLabel,
                                         INI_TRANSPORTDRIVER,
                                         DEF_TRANSPORTDRIVER,
                                         pszTransport, sizeof(pszTransport),
                                         gszICAFile );

                wsprintf( szBuffer, strFormatString, pszTransport );
            }
            break;

        case CLIENT_ERROR_WFENGINE_NOT_FOUND:
            LoadString(ghInstance,
                       IDS_CLIENT_ERROR_WFENGINE_NOT_FOUND,
                       strFormatString,
                       sizeof(strFormatString));

            wsprintf( szBuffer, strFormatString,
#ifdef WIN16
                      "WFENGW.EXE" 
#else
                      "WFENGN.EXE"
#endif
                    );
            break;

        default:
            break;
    }

    /*
     *  If we did not get a specific error message; return the generic one
     *  associated with the error code.
     */
    if ( !(*szBuffer) ) {
        if ( iErrorCode >= CLIENT_ERROR ) {
            nResourceId = IDS_CLIENT_ERROR_DLL_NOT_FOUND +
                            (iErrorCode - CLIENT_ERROR);
        } else if ( iErrorCode >= CLIENT_STATUS_HOTKEY1 ) {
            nResourceId = IDS_CLIENT_STATUS_HOTKEY1 +
                            (iErrorCode - CLIENT_STATUS_HOTKEY1);
        } else {
            nResourceId = IDS_CLIENT_STATUS_CONNECTED +
                            (iErrorCode - CLIENT_STATUS_CONNECTED);
        }

        if ( LoadString(ghInstance,
                        nResourceId,
                        strFormatString,
                        sizeof(strFormatString)) ) {
            if( iErrorCode == CLIENT_ERROR_DLL_NOT_FOUND) {
               wsprintf(szBuffer, strFormatString, gszLoadDllFileName);
            }
            else
               lstrcpy(szBuffer, strFormatString);

        }
    }

    /*
     * If we still don't have an error message, fetch the 'no error text
     * available' message and return that.
     */
    if ( !(*szBuffer) ) {
        if ( LoadString( ghInstance,
                         IDS_NO_ERROR_TEXT_AVAILABLE,
                         strFormatString,
                         sizeof(strFormatString)) ) {
            lstrcpy(szBuffer, strFormatString);
        } else  {  // something is REALLY broken if must hard-code!
            lstrcpy(szBuffer, "(no error text available)");
        }
    }

    /*
     * Copy the message to caller's buffer and return.
     */
    if((cbBuffSize-1) < lstrlen(szBuffer)) {
       memcpy(chBuffer, szBuffer, cbBuffSize);
       chBuffer[cbBuffSize-1] = '\0';
       }
    else
       lstrcpy(chBuffer, szBuffer);

    return(bErrorCodeInMessage);

}  // end GetErrorMessage

/*
 * EMStatusMessage
 *
 * Incoming status message should be displayed
 *
 */
static int EMStatusMessage(int iStatus)
{
   int iResId;
   char szBuffer[MAX_ERRORMSG];
   int cbLen;
   int iResp;
   BOOL bResp = FALSE;

   iResId = (iStatus - CLIENT_STATUS_CONNECTED) + IDS_CLIENT_STATUS_CONNECTED;
   cbLen = LoadString(ghInstance,iResId,szBuffer,sizeof(szBuffer));
   if(cbLen == 0) {
      char szBuf2[MAX_ERRORMSG];

      EMGetStringResource( IDS_CLIENT_ERROR_UNKNOWN_STATUS, szBuf2, sizeof(szBuf2));
      wsprintf(szBuffer,szBuf2,iStatus);
      }

   if(!g_MsgEngineText) {
      if(iStatus == CLIENT_STATUS_QUERY_CLOSE) 
         iResp = MessageBox( hWFE, szBuffer, gszServerLabel, MB_OKCANCEL);
      else
         iResp = MessageBox( hWFE, szBuffer, gszServerLabel, MB_OK);
      }
   else {
      ATOM atom;

      // notify the plugin
      if(g_hWndPlugin) {
         atom = GlobalAddAtom(szBuffer);
         SendMessage(g_hWndPlugin, g_MsgEngineText, iStatus, (LPARAM)atom);
         }
      iResp = IDOK;
      }

   // return code based on message box return
   if(iResp==IDOK)
      bResp = TRUE;
   else 
      bResp = FALSE;

   // invalidate parent to force redraw
   InvalidateRgn( ghWFE, NULL, TRUE);

   return(bResp);
}

/*
 * EMStatusDialog
 *
 * Incoming status should be reflected in dialog
 *
 */
static int EMStatusDialog(int iStatus)
{
   int iResId;
   char szBuffer[MAX_ERRORMSG];
   char szBuf2[MAX_ERRORMSG];
   int cbLen;


   iResId = (iStatus - CLIENT_STATUS_CONNECTED) + IDS_CLIENT_STATUS_CONNECTED;
   cbLen = EMGetStringResource(iResId,szBuffer,sizeof(szBuffer));
   if(cbLen == 0) {
      EMGetStringResource(IDS_CLIENT_ERROR_UNKNOWN_STATUS,szBuf2,sizeof(szBuf2));
      wsprintf(szBuffer,szBuf2,iStatus);
      }

   if(!g_MsgEngineText) {
      // if no dialog, do nothing
      if(hDlg==NULL)
         return(FALSE);
      SetDlgItemText( hDlg, IDC_STATDLG_STATUS, szBuffer);
      UpdateWindow(hDlg);
      }
   else {
      ATOM atom;

      if(g_hWndPlugin) {
         atom = GlobalAddAtom(szBuffer);
         SendMessage(g_hWndPlugin, g_MsgEngineText, iStatus, (LPARAM)atom);
         }
      }

   return(TRUE);
}


/*
 * StatDialogProc
 *
 * Status dialog procedure
 */
BOOL CALLBACK StatDialogProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   BOOL bOurMsg=FALSE;

   switch(msg) {
       case WM_INITDIALOG:
           {
           CHAR szTitle[MAX_TITLE];

           // center dialog box
           CenterPopup(hdlg,NULL);

           // change title to include server label
           GetWindowText(hdlg, szTitle, sizeof(szTitle));
           lstrcat(szTitle,gszServerLabel);
           SetWindowText(hdlg,szTitle);

           // make our dialog top most
           SetWindowPos(hdlg, HWND_TOP, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
           bOurMsg = TRUE;
           }
           break;

       case WM_COMMAND:
           switch(wParam) {
               case IDCANCEL:
                  if(hWFE)
                     PostMessage(hWFE, WM_CLOSE, (WPARAM)0, (LPARAM)0);
                  EMStatusDialog(CLIENT_STATUS_DISCONNECTING);
                  bOurMsg = TRUE;
                  break;
               default:
                   break;
               }
           break;

       default:
           break;
       }

   return(bOurMsg);
}

void CALLBACK StatDialogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime)
{
   if(iEvent == IDD_STATDLG) {

      // animate the icon for the waving flag
      gnBlip = (++gnBlip % NUM_BLIPS);
      SendDlgItemMessage(hDlg, 
                         IDC_STATDLG_ICON,
                         STM_SETICON, 
                         (WPARAM)hIcons[gnBlip],0);
      }
}

/*
 * EMKillStatusDlg
 *
 * get rid of status dialog box
 *
 */
static int  EMKillStatusDlg(void)
{
    // stop the timer
    if(gBlipTimer) {
       KillTimer(hDlg, IDD_STATDLG);
       gBlipTimer = 0;
       }

    // destroy the window
    if(hDlg) {
       DestroyWindow(hDlg);
       hDlg = NULL;
       }

#ifdef WIN16
    if(lpfnStatDlgProc) {
       FreeProcInstance(lpfnStatDlgProc);
       lpfnStatDlgProc = NULL;
       }
#endif

    return(TRUE);
}

/*
 * EMConnectStatusPopup
 *
 * Connection Status popup
 */
void static EMConnectStatusPopup()
{
    // create the status dialog box
#ifdef WIN16
    lpfnStatDlgProc = MakeProcInstance(StatDialogProc, ghInstance);
#endif
#ifdef WIN32
    hDlg = CreateDialog(ghInstance, 
                        MAKEINTRESOURCE(IDD_STATDLG),
                        NULL,
                        StatDialogProc);
#else
    hDlg = CreateDialog(ghInstance, 
                        MAKEINTRESOURCE(IDD_STATDLG),
                        NULL,
                        lpfnStatDlgProc);
#endif
    if(hDlg) {
       int i;

       // start timer
       gBlipTimer = SetTimer(hDlg, IDD_STATDLG, BLIP_MSEC, (TIMERPROC)StatDialogTimer);

       // init the icons
       for(i=0;i<NUM_BLIPS;i++) {
          hIcons[i] = LoadIcon(ghInstance, MAKEINTRESOURCE(IconIds[i]));
          }

       // show the status dialog
       ShowWindow(hDlg,SW_SHOW);
       UpdateWindow(hDlg);
       }
}

/*
 * EMFileSecurityPopup
 *
 * File Security Popup
 */
static void EMFileSecurityPopup(void)
{
#ifdef WIN16
   lpfnFileSecProc = MakeProcInstance(FileSecDlgProc, ghInstance);
#endif

   // give the audio warning
   MessageBeep(MB_OK);

#ifdef WIN32
   hSecDlg = CreateDialog(ghInstance,
                          MAKEINTRESOURCE(IDD_CLIENT_FILE_SECURITY),
                          hWFE,
                          FileSecDlgProc);
#else
   hSecDlg = CreateDialog(ghInstance,
                          MAKEINTRESOURCE(IDD_CLIENT_FILE_SECURITY),
                          hWFE,
                          lpfnFileSecProc);
#endif

}

/*
 * FileSecDlgProc
 *
 * File Security Dialog procedure
 */
BOOL CALLBACK FileSecDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   BOOL bOurMsg=FALSE;
   static WPARAM wFileSec;

   switch(msg) {
       case WM_INITDIALOG:
           // center dialog box
           CenterPopup(hdlg,NULL);
           wFileSec = IDD_SECURITY_NO_ACCESS;
           CheckRadioButton (hdlg, 
                             IDD_SECURITY_NO_ACCESS,
                             IDD_SECURITY_FULL_ACCESS,
                             wFileSec);
           SetFocus(hdlg);
           bOurMsg = TRUE;
           break;

       case WM_NCLBUTTONDOWN:
#ifdef WIN32
           fDragOn = TRUE;
#endif
           DefWindowProc( hdlg, msg, wParam, lParam );
           bOurMsg = TRUE;
#ifdef WIN32
           fDragOn = FALSE;
#endif
           break;

       case WM_COMMAND:
           switch(wParam) {
               case IDOK:
                  // set the file security
                  EMSetFileSecurity(wFileSec);

                  // invalidate parent to force redraw
                  InvalidateRgn( ghWFE, NULL, FALSE);
                  UpdateWindow(ghWFE);

                  // dialog box ending
                  DestroyWindow(hdlg);

                  bOurMsg = TRUE;
#ifdef WIN16
                  if(lpfnFileSecProc) {
                     FreeProcInstance(lpfnFileSecProc);
                     lpfnFileSecProc = NULL;
                     }
#endif

                  break;

               case IDD_SECURITY_NO_ACCESS:
               case IDD_SECURITY_READ_ACCESS:
               case IDD_SECURITY_FULL_ACCESS:
                    CheckRadioButton (hdlg, 
                                      IDD_SECURITY_NO_ACCESS,
                                      IDD_SECURITY_FULL_ACCESS,
                                      wParam);
                    wFileSec = wParam;
                    bOurMsg = TRUE;
                    break;



               default:
                  break;
               }
           break;

       default:
           break;
       }

   return(bOurMsg);
}

/*
 * EMSetFileSecurity
 *
 * Based on Dialog ID, set the file security for client drive mapping
 *
 */
void EMSetFileSecurity( WPARAM wFileSec)
{
   VDSETINFORMATION VdSetInfo;
   ULONG ulSecurity;

   // match dialog id code to cdm security
   if(wFileSec == IDD_SECURITY_NO_ACCESS)
      ulSecurity = CDM_SECURITY_ACCESS_NONE;
   else if(wFileSec == IDD_SECURITY_READ_ACCESS)
      ulSecurity = CDM_SECURITY_ACCESS_READ;
   else if(wFileSec == IDD_SECURITY_FULL_ACCESS)
      ulSecurity = CDM_SECURITY_ACCESS_ALL;
   else
      ulSecurity = CDM_SECURITY_ACCESS_NONE;

   /*
    * Set the security access in CDM
    */
   memset( &VdSetInfo, 0, sizeof(VdSetInfo) );
   VdSetInfo.pVdInformation      = (PVOID)&ulSecurity;
   VdSetInfo.VdInformationLength = sizeof(ULONG);
   VdSetInfo.VdInformationClass  = VdSetSecurityAccess;
   ModuleCall( gpaVdLink[Virtual_Cdm], VD__SETINFORMATION, &VdSetInfo );
}

/*
 *  EMGetStringResource
 *
 *  just a wrapper around LoadString
 *
 */
int EMGetStringResource(int iResID, LPSTR szString, int cbStr)
{
    return( LoadString( ghInstance,iResID,szString,cbStr) );
}

/*
 *  CenterPopup
 */
BOOL CenterPopup( HWND hWnd, HWND hParentWnd )
{
   int   xPopup;
   int   yPopup;
   int   cxPopup;
   int   cyPopup;
   int   cxScreen;
   int   cyScreen;
   int   cxParent;
   int   cyParent;
   RECT  rcWindow;

   cxScreen = GetSystemMetrics( SM_CXSCREEN );
   cyScreen = GetSystemMetrics( SM_CYSCREEN );

   GetWindowRect( hWnd, (LPRECT)&rcWindow );

   cxPopup = rcWindow.right - rcWindow.left;
   cyPopup = rcWindow.bottom - rcWindow.top;

   if( hParentWnd ){
      GetWindowRect( hParentWnd, (LPRECT)&rcWindow );
      cxParent = rcWindow.right - rcWindow.left;
      cyParent = rcWindow.bottom - rcWindow.top;
      xPopup = rcWindow.left + ((cxParent - cxPopup)/2);
      yPopup = rcWindow.top + ((cyParent - cyPopup)/2);
      if( xPopup+cxPopup > cxScreen )
	 xPopup = cxScreen - cxPopup;
      if( yPopup+cyPopup > cyScreen )
	 yPopup = cyScreen - cyPopup;
   }else{
      xPopup = ((cxScreen - cxPopup) / 2 + 4) & ~7;
      yPopup = (cyScreen - cyPopup) / 2;
   }
   MoveWindow( hWnd,
      ( xPopup > 0 ) ? xPopup : 0,
      ( yPopup > 0 ) ? yPopup : 0,
      cxPopup,
      cyPopup,
      TRUE );
   return( TRUE );
}
#endif

#ifdef DEBUG
/*
 * EMLogInit
 *
 *
 * Initialize the important stuff for the Log file
 *
 */
int  EMLogInit()
{
   WFELOGINFO WFEEngLogInfo;
   LOGOPEN EMLogInfo;
   int rc = CLIENT_STATUS_SUCCESS;

   WFEEngLogInfo.pszLogFile = "ICAWIN.LOG";
#ifdef DEBUG
   WFEEngLogInfo.ulLogClass = TC_ALL;
   WFEEngLogInfo.ulLogEvent = TT_ERROR;
   gulLogClass = TC_ALL;
   gulLogEvent = TT_ERROR;
#else
   WFEEngLogInfo.ulLogClass = LOG_CLASS;
   WFEEngLogInfo.ulLogEvent = LOG_CONNECT|LOG_ERRORS;
#endif
   WFEEngLogInfo.ulLogFlags = 0;

   EMLogInfo.LogFlags   = WFEEngLogInfo.ulLogFlags;
   EMLogInfo.LogClass   = WFEEngLogInfo.ulLogClass;
   EMLogInfo.LogEnable  = WFEEngLogInfo.ulLogEvent;
   lstrcpy(EMLogInfo.LogFile,WFEEngLogInfo.pszLogFile);

   // rc = LogOpen(&EMLogInfo);

   //rc = srvWFEngSetInformation( NULL, 
   //                             WFELogInfo,
   //                             &WFEEngLogInfo,
   //                             sizeof(WFEEngLogInfo));
   return(rc);
}
#endif //DEBUG

