/*****************************************************************************
*
*   WENGINE.C
*
*   Windows client engine - Application Programming Interfaces (APIs)
*
*   Note - Any new windows client engine APIs or any new parameters to existing
*          API need to be thunked in DDESERV.C and ..\WFCWIN\DDECLIEN.C
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Marc Bloomfield (marcb) 27-Mar-1995
*
*   $Log$
*  
*     Rev 1.157   07 May 1998 18:52:10   terryt
*  remove IMM calls - do not exist on NT 3.51
*  
*     Rev 1.156   06 May 1998 11:38:36   anatoliyp
*  No change.
*  
*     Rev 1.155   May 04 1998 14:39:36   sumitd
*  IPX watchdog timer started only for IPX connections
*  
*     Rev 1.154   May 04 1998 17:26:44   briang
*  Fix build barning in Win16 with respect to keyboard type additions
*  
*     Rev 1.153   May 04 1998 10:28:46   sumitd
*  IPX Connection timeout increased
*  
*     Rev 1.152   May 02 1998 20:51:40   briang
*  Fix semantics error on comparison of keyboard layout
*  
*     Rev 1.151   May 02 1998 20:30:48   briang
*  use correct keyboardtype section from INI file
*  
*     Rev 1.148   Apr 30 1998 18:35:20   sumitd
*  Error when IPX connection limit is reached
*  
*     Rev 1.147   Apr 29 1998 20:21:24   BobC
*  Adding thread to thinwire makes twstack code unnecessary
*  
*     Rev 1.146   29 Apr 1998 10:32:30   DAVIDS
*  Fixed Alt-Tab and Alt-Backtab hotkeys for CE
*  
*     Rev 1.145   27 Apr 1998 21:44:14   terryt
*  unload reducer
*  
*     Rev 1.144   21 Apr 1998 16:56:32   butchd
*  CPR 10032: Ignore ALT-ESC hotkey if running in seamless mode
*  
*     Rev 1.143   Apr 20 1998 19:06:18   sumitd
*  AutoLogonAllowed flag for Secure ICA client
*  
*     Rev 1.141.1.0   20 Apr 1998 15:37:10   toma
*  Update
*
*     Rev 1.141   20 Apr 1998 15:30:34   toma
*  Update
*
*     Rev 1.140   14 Apr 1998 21:32:24   derekc
*  fixed build warning
*
*     Rev 1.139   14 Apr 1998 21:26:04   derekc
*  added SwitchUiToUnicode() support
*
*     Rev 1.138   07 Apr 1998 08:32:40   DAVIDS
*  Added client status "hotkey" for CE
*
*     Rev 1.137   07 Apr 1998 02:41:46   anatoliyp
*  TWI update
*
*     Rev 1.136   04 Apr 1998 11:55:36   DAVIDS
*  rev 1.135 was bad.
*
*     Rev 1.134   24 Mar 1998 15:35:28   toma
*  Update
*
*     Rev 1.133   24 Mar 1998 14:45:30   toma
*  Update
*
*     Rev 1.132   Mar 20 1998 15:33:34   xuanh
*  add CTRL-SHIFT-ESC hotkey.
*
*     Rev 1.131   05 Mar 1998 16:28:36   toma
*  ce merge
*
*     Rev 1.130   27 Feb 1998 18:13:00   TOMA
*  ce merge
*
*     Rev 1.129   Feb 17 1998 16:18:18   bills
*  gpszTitle was too small for the changes to the title of the client window.
*  If the description entered for a session was over 27 bytes, lstrcat would
*  spill over until it finally blew out gpfnStatusMsgProc.  The length of
*  gpszTitle was increased to 128.  It is no longer using the DESCRIPTION
*  typedef, since it was used elsewhere, I wanted to minimize the possibility
*  of introducting new problems.
*
*     Rev 1.128   Feb 14 1998 18:15:24   briang
*  Fix FindWindow(ConnectionCenter) to look for the correct class and name
*
*     Rev 1.127   Feb 11 1998 17:46:24   xuanh
*  CPR8248 Win16 title bar could not toggle, fixed.
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
#include "../../inc/cfgload.h"
#include "../../inc/miapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/timapi.h"
#include "../../inc/kbdapi.h"
// #include "../../inc/xmsapi.h"
#include "../../inc/encrypt.h"
// #include "../../inc/lptapi.h"

#include "../../inc/wfcver.h"
#include "../../inc/mouapi.h"

//#include "resource.h"

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#include "../vd/vdtwinc/twi_en.h"

#define DOS

#ifdef TWI_INTERFACE_ENABLED
#include "../vd/vdtwinc/aptwdef.h"    // TWI common data
VOID
TwiCallAgent(VOID);
VOID
TwiFullScreen(HWND hWnd, BOOLEAN flag);
#endif  //TWI_INTERFACE_ENABLED

#ifndef DOS
// change cruntime string calls to windows string calls
// Do before WINCE includes because it also remaps
// strlen
//#define strlen lstrlen
#endif

#ifdef WINCE
#include <wcecalls.h>
#include "..\vd\vdtw31ce\wcecursor.h"
#endif // WINCE


#ifdef WIN32  // IpxWatchdog
#define IPX_TIMEOUT 25000
#define IPX_PROTOCOL "IPX"
#endif


/*=============================================================================
==   Local Functions
=============================================================================*/
int WFCAPI Load( PMINIDLL );
INT WFCAPI MainLoad( PDLLLINK pLink );
INT WFCAPI UiOpen( PVOID pNotUsed, PEXEOPEN pUiOpen );
INT WFCAPI UiInfo( PVOID pNotUsed, PDLLINFO pUiInfo );
INT wWFEngPoll( VOID );
INT StopPolling( VOID );
VOID SendKeySequence( LPBYTE pszKeySequence );
INT wRegisterHotkey( PWFEHOTKEYINFO pHotkeyInfo );

#ifdef WIN32
VOID CALLBACK IpxTimeoutProc(HWND hWmd, UINT uMsg, UINT idEvent, DWORD CurrentTime);
#endif

/*=============================================================================
==   External Functions
=============================================================================*/
int WFCAPI LogStubLoad( PPLIBPROCEDURE pfnLogStubProcedures );
void FlushScanCode( void );
#ifndef DOS
void LocalPaint( HWND hWnd );
#endif


/*
 *  Script driver routines and variables
 */
BOOL   sdLoad( LPBYTE pScriptFile, LPBYTE pScriptDriver );
BOOL   sdPoll( VOID);
VOID   sdUnload( VOID);
LPBYTE gScriptFile;
LPBYTE gScriptDriver;


/*=============================================================================
==  External Vars
=============================================================================*/
#ifdef WIN32
#ifndef VDTW_THREAD
extern VDTWSTACK vTWStack;
#endif
#endif
extern HWND           g_hWndPlugin;

//HINSTANCE ghInstance = NULL; 

#ifndef DOS
extern BOOL gfIPCShutdown;
extern BOOL gbIPCEngine;
#endif

#ifdef TWI_INTERFACE_ENABLED
extern BOOL TwiModeEnableFlag = FALSE;
#endif  //TWI_INTERFACE_ENABLED

/*=============================================================================
==   Local Vars
=============================================================================*/

BOOL gbContinuePolling           = FALSE;
BOOL szClearPasswordTag          = FALSE;
#if 0
#ifdef WIN32 
BOOL gfIpxWatchdogStarted        = FALSE;
#endif
PPLIBPROCEDURE pModuleProcedures = NULL;
PPLIBPROCEDURE pLogProcedures    = NULL;
PPLIBPROCEDURE pVioProcedures    = NULL;
PPLIBPROCEDURE pMemIniProcedures   = NULL;
PPLIBPROCEDURE pKbdProcedures    = NULL;
#ifdef LPT_IN_ENGINE
PPLIBPROCEDURE pLptProcedures    = NULL;
#endif
PPLIBPROCEDURE pClibProcedures   = NULL;
#ifdef DOS

PPLIBPROCEDURE pTimerProcedures  = NULL;
PPLIBPROCEDURE pMouProcedures    = NULL;
PPLIBPROCEDURE pXmsProcedures    = NULL;
#endif
#endif
// For WinCE ghwndMain is defined
// in citrixce.c.
// This allows the PeekMessage override
// to operate.
#ifdef WINCE
extern HWND ghwndMain;
#else
static HWND ghwndMain = NULL;
#endif

static DLLLINK   gExeLink = { 0 };
static char      gpszExePath[MAXPATH] = { 0 };
static char      gpszDllPath[MAXPATH] = { 0 };
#if !defined(DOS) && !defined(RISCOS)
HICON            ghCustomIcon = NULL;
BOOL             gfRemoveTitleBar = FALSE;
#endif
HANDLE           ghWFE = NULL;
DLLLINK          gWdLink = { 0 };
DLLLINK          gRedLink = { 0 };
DLLLINK          gPdLink = { 0 };
PDLLLINK *       gpaVdLink = NULL;
USHORT           gMaxVirtualChannels = 0;
PDOPEN           gPdOpen = { 0 };
BOOL             gbVdAddrToWd = FALSE;
USHORT           gState = 0;
static BOOL      gbPollingEnabled = FALSE;
CLIENTNAME       gpszClientname = { 0 };
CLIENTSN         gpszClientSN = { 0 };
CHAR         gpszTitle[128] = { 0 };
ULONG            gVersion = 0;
static PFNSTATUSMSGPROC gpfnStatusMsgProc = NULL;
#ifdef DOS
PFNUIPOLL        gpfnUIPoll = NULL;
#endif
BOOL             gbDirectClose = FALSE;

static ULONG NoDataTime = 0;     //used to determine when to really say no data
#ifdef WIN32
static BOOL  fVersionCheck = FALSE;
static OSVERSIONINFO osvi = { 0 };
#endif

/*
 *  Define User Interface driver external procedures
 */
static PDLLPROCEDURE gEXEProcedures[] = {
    (PDLLPROCEDURE) MainLoad,
    (PDLLPROCEDURE) NULL,   // unload
    (PDLLPROCEDURE) UiOpen,
    (PDLLPROCEDURE) NULL,   // close
    (PDLLPROCEDURE) UiInfo,
};
#define EXE__COUNT (sizeof( gEXEProcedures ) / sizeof(PDLLPROCEDURE))

PUI_C2H G_pUiData = NULL;
USHORT  G_pUiDataLength = 0;
USHORT  G_Encrypted = FALSE;
USHORT  G_MouseTimer = DEF_MOUSETIMER;
#ifdef DOS
USHORT  G_MouseDoubleClickTimer = DEF_MOUSEDOUBLECLICKTIMER;
USHORT  G_MouseDoubleClickHeight = DEF_MOUSEDOUBLECLICKHEIGHT;
USHORT  G_MouseDoubleClickWidth = DEF_MOUSEDOUBLECLICKWIDTH;
#endif
USHORT  G_KeyboardTimer = DEF_KEYBOARDTIMER;
BOOL    G_fEchoTTY = FALSE;
BOOL    G_fAsync = FALSE;
BOOL    G_fIpx  = FALSE;

#ifdef DOS
USHORT G_fNoUmb = FALSE;
#endif

#if 0
/*
 *  Define WFEng driver external procedures
 */
static PDLLPROCEDURE WFEngProcedures[] = {
    (PDLLPROCEDURE) Load,
    (PDLLPROCEDURE) srvWFEngUnloadDrivers,
    (PDLLPROCEDURE) srvWFEngOpen,
    (PDLLPROCEDURE) srvWFEngClose,
    (PDLLPROCEDURE) srvWFEngLoadWd,
    (PDLLPROCEDURE) srvWFEngLoadPd,
    (PDLLPROCEDURE) srvWFEngQueryInformation,
    (PDLLPROCEDURE) srvWFEngSetInformation,
    (PDLLPROCEDURE) srvWFEngLoadVd,
    (PDLLPROCEDURE) srvWFEngLoadSession,
    (PDLLPROCEDURE) srvWFEngConnect,
    (PDLLPROCEDURE) srvWFEngDisconnect,
    (PDLLPROCEDURE) srvWFEngPoll,
    (PDLLPROCEDURE) srvWFEngMessageLoop,
    (PDLLPROCEDURE) NULL, // placeholder for WFEngLogString
};
#endif

ENCRYPTIONLEVEL g_szEncryptionLevelConnStatus;

//#define WFE__COUNT (sizeof( WFEngProcedures ) / sizeof(PDLLPROCEDURE))

static struct {
          char * pszShiftState;
          int    ShiftState;
          char * pszScanCode;
          int    ScanCode;
       } gHotkeys[] = {
{ DEF_HOTKEY1_SHIFT, DEF_HOTKEY1_SHIFTV, DEF_HOTKEY1_CHAR, DEF_HOTKEY1_CHARV },
{ DEF_HOTKEY2_SHIFT, DEF_HOTKEY2_SHIFTV, DEF_HOTKEY2_CHAR, DEF_HOTKEY2_CHARV },
{ DEF_HOTKEY3_SHIFT, DEF_HOTKEY3_SHIFTV, DEF_HOTKEY3_CHAR, DEF_HOTKEY3_CHARV },
{ DEF_HOTKEY4_SHIFT, DEF_HOTKEY4_SHIFTV, DEF_HOTKEY4_CHAR, DEF_HOTKEY4_CHARV },
{ DEF_HOTKEY5_SHIFT, DEF_HOTKEY5_SHIFTV, DEF_HOTKEY5_CHAR, DEF_HOTKEY5_CHARV },
{ DEF_HOTKEY6_SHIFT, DEF_HOTKEY6_SHIFTV, DEF_HOTKEY6_CHAR, DEF_HOTKEY6_CHARV },
{ DEF_HOTKEY7_SHIFT, DEF_HOTKEY7_SHIFTV, DEF_HOTKEY7_CHAR, DEF_HOTKEY7_CHARV },
{ DEF_HOTKEY8_SHIFT, DEF_HOTKEY8_SHIFTV, DEF_HOTKEY8_CHAR, DEF_HOTKEY8_CHARV },
{ DEF_HOTKEY9_SHIFT, DEF_HOTKEY9_SHIFTV, DEF_HOTKEY9_CHAR, DEF_HOTKEY9_CHARV },
{ NULL,              0,                  NULL,             0                 },
       };

/*******************************************************************************
 *
 *  srvWFEngLoad
 *
 *    DOS: WFEngLoad() in MINIDLL.LIB calls Load which calls this function.
 *    WIN16/WIN32: This is a normal API to initialize WENGINE
 *
 * ENTRY:
 *    pszDllPath (input)
 *       Ignored for DOS
 *    pLink (input/output)
 *       pointer to the structure MINIDLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
srvWFEngLoad( LPSTR pszDllPath, PMINIDLL pLink )
{
    int rc = CLIENT_STATUS_SUCCESS;

    ModuleInit( "WENGINE", &gExeLink, NULL );

    MouseLoad( NULL );

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoad rc(%d)", rc ));
    return( rc );
}

/*******************************************************************************
 *
 *  WFEngLoad
 *
 *    DOS: WFEngLoad() in MINIDLL.LIB calls Load which calls this function.
 *    WIN16/WIN32: This is a normal API to initialize WENGINE
 *
 * ENTRY:
 *    pszDllPath (input)
 *       Ignored for DOS
 *    pLink (input/output)
 *       pointer to the structure MINIDLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WFEngLoad( LPSTR pszDllPath, PMINIDLL pLink )
{

   return(srvWFEngLoad( pszDllPath, pLink ));

}

/*******************************************************************************
 *
 *  srvWFEngUnload
 *
 *    DOS: WFEngUnload() in MINIDLL.LIB calls MiniUnload which calls this.
 *    WIN16/WIN32: This is a normal API to terminate WENGINE
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure MINIDLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
srvWFEngUnload( PMINIDLL pLink )
{
//    VioUnload();
//    MemIniUnload();

//     TimerUnload();
//     KbdUnload();
     MouseUnload();
//     XmsUnload();

     ModuleUnload(&gExeLink);
     
    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngUnload (exit)" ));
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  WFEngUnload
 *
 *    DOS: WFEngUnload() in MINIDLL.LIB calls MiniUnload which calls this.
 *    WIN16/WIN32: This is a normal API to terminate WENGINE
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure MINIDLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WFEngUnload( PMINIDLL pLink )
{
   return(srvWFEngUnload( pLink ));
}

/*******************************************************************************
 *
 *  MainLoad
 *
 *    ModuleInit calls this routine to initialize the DLLLINK structure
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

INT WFCAPI MainLoad( PDLLLINK pLink )
{
    pLink->ProcCount       = EXE__COUNT;
    pLink->pProcedures     = gEXEProcedures;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: MainLoad (exit)" ));
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  UiOpen
 *
 *  UiOpen is called to initialize ini file parameters
 *
 * ENTRY:
 *    pNotUsed (input)
 *       not used
 *    pUiOpen (input/output)
 *       pointer to the structure UIOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

INT WFCAPI UiOpen( PVOID pNotUsed, PEXEOPEN pUiOpen )
{
    USHORT       Offset;
    ULONG        Layout;
 	 ULONG        lKeyboard;
    KEYBOARDTYPE szKeyboard;
#if defined(WIN32) || defined(DBCS)
#ifndef WINCE
#ifdef DBCS
    CHAR         szimeFileName[IMEFILENAME_LENGTH+1];
#endif
#endif
#endif
    LPBYTE       pBuf;
    USHORT       Length;
    USHORT       Len;
    ENCRYPTEDPASSWORD szEncryptedPassword;
    PASSWORD szPassword;
    KEYBOARDLAYOUT szKeyboardLayout;
    int fAutoLogonAllowed;
    
    /* initialise some static variables here */
    G_Encrypted = FALSE;
    
    /* check for script file and script driver */
    if ( ((gScriptFile   = malloc( FILEPATH_LENGTH+1 )) != NULL) &&
         ((gScriptDriver = malloc( FILEPATH_LENGTH+1 )) != NULL) ) {

        /* read profile */
        miGetPrivateProfileString( INI_WFCLIENT, INI_SCRIPT_DRIVER, INI_EMPTY,
                                   gScriptDriver, FILEPATH_LENGTH );

        miGetPrivateProfileString( INI_WFCLIENT, INI_SCRIPT_FILE, INI_EMPTY,
                                   gScriptFile,   FILEPATH_LENGTH );

        /* valid script file and path */
        if ( !strlen( gScriptFile ) || !strlen( gScriptDriver ) ) {
            free( gScriptDriver );
            free( gScriptFile );
            gScriptFile = NULL;
        }
    }

    /* get worst case byte count */
    G_pUiDataLength = sizeof(UI_C2H) + strlen(gpszExePath) +
                DOMAIN_LENGTH + USERNAME_LENGTH + PASSWORD_LENGTH +
                DIRECTORY_LENGTH + INITIALPROGRAM_LENGTH +
                CLIENTNAME_LENGTH + CLIENTSN_LENGTH + IMEFILENAME_LENGTH + 9;

    if ( G_pUiData )
        free( G_pUiData );

    if ( (G_pUiData = malloc(G_pUiDataLength)) == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    memset( G_pUiData, 0, G_pUiDataLength );
    Offset = sizeof(UI_C2H);

#ifdef UNICODESUPPORT
    /*
     *  Copy encoding type and code page
     *  We're cheating and using pNotUsed
     *  as a Unicode flag.  Once the client
     *  has received a PACKET_INIT_REQUEST
     *  from the server, it knows that it
     *  can or cannot accept Unicode.  If
     *  it cannot, it will simply function
     *  as normal.  If it can, it will call
     *  this function again with the pNotUsed
     *  flag set to Kbd_Unicode and the UI
     *  will reconstruct its information.
     */
    G_pUiData->EncodingType = (USHORT) 0;   // 1=unicode, 0=ansi
    G_pUiData->EncodingData = (USHORT) 0;
    G_pUiData->KeyboardType = (USHORT) 0;
    G_pUiData->KeyboardSubType = (BYTE) 0;
    G_pUiData->KeyboardFunctionKey = (BYTE) 0;
#endif

    /*
     * Until we connect, we don't know the encryption level
     */
    G_pUiData->EncryptionLevel = 0;

#ifdef TWI_INTERFACE_ENABLED

    TwiModeEnableFlag = miGetPrivateProfileBool( INI_SERVERSECTION,
                                                 INI_TWI_MODE,
                                                 DEF_TWI_MODE );

    if (TwiModeEnableFlag)
        G_pUiData->fTwiModeEnableFlag=1;
    else
        G_pUiData->fTwiModeEnableFlag=0;
    //  if( TwiModeEnableFlag ) MessageBox(NULL, "TwiModeFlag=1", "AP", MB_OK | MB_ICONASTERISK | MB_TASKMODAL);
    //  else                    MessageBox(NULL, "TwiModeFlag=0", "AP", MB_OK | MB_ICONASTERISK | MB_TASKMODAL);
    {
         ULONG ptru = (ULONG)GetWindowLong( ghwndMain, GWL_INSTANCEDATA );
         if( ptru ){

            ptru += sizeof(WFEINSTANCE);
            *((PULONG)ptru) = TwiModeEnableFlag;
  //  if( TwiModeEnableFlag ) MessageBox(NULL, "saved TwiModeFlag=1", "AP", MB_OK | MB_ICONASTERISK | MB_TASKMODAL);
  //  else                    MessageBox(NULL, "saved TwiModeFlag=0", "AP", MB_OK | MB_ICONASTERISK | MB_TASKMODAL);
         }
    }
#endif  //TWI_INTERFACE_ENABLED

    G_pUiData->fDisableSound = miGetPrivateProfileBool( INI_WFCLIENT, INI_SOUND,
                                                        DEF_SOUND );

    G_pUiData->fDisableCtrlAltDel = miGetPrivateProfileBool( INI_WFCLIENT, INI_CTRLALTDEL,
                                                             DEF_CTRLALTDEL );

    G_MouseTimer = miGetPrivateProfileInt( INI_WFCLIENT, INI_MOUSETIMER, DEF_MOUSETIMER );

#ifdef DOS
    G_MouseDoubleClickTimer = miGetPrivateProfileInt(INI_WFCLIENT, INI_MOUSEDOUBLECLICKTIMER, DEF_MOUSEDOUBLECLICKTIMER );
    G_MouseDoubleClickHeight = miGetPrivateProfileInt( INI_WFCLIENT, INI_MOUSEDOUBLECLICKHEIGHT, DEF_MOUSEDOUBLECLICKHEIGHT );
    G_MouseDoubleClickWidth = miGetPrivateProfileInt( INI_WFCLIENT, INI_MOUSEDOUBLECLICKWIDTH, DEF_MOUSEDOUBLECLICKWIDTH );
#endif

    G_KeyboardTimer = miGetPrivateProfileInt(INI_WFCLIENT , INI_KEYBOARDTIMER, DEF_KEYBOARDTIMER );

    G_fEchoTTY = miGetPrivateProfileBool(INI_WFCLIENT , INI_ECHO_TTY, DEF_ECHO_TTY );


    miGetPrivateProfileString( INI_WFCLIENT, INI_KEYBOARDLAYOUT, DEF_KEYBOARDLAYOUT,
			      szKeyboardLayout, sizeof(szKeyboardLayout) );

    // special new string to say pick up from local machine configuration

    TRACE((TC_WENG, TT_API1, "KeyboardLayout: '%s'\n", szKeyboardLayout));
    if (strcmpi(szKeyboardLayout, "(Auto Detect)") == 0)
    {
	KEYBOARDLAYOUT LocalLayout;
	
	if (GetLocalKeyboard(LocalLayout, sizeof(LocalLayout)) == CLIENT_STATUS_SUCCESS)
	    strcpy(szKeyboardLayout, LocalLayout);

	TRACE((TC_WENG, TT_API1, "KeyboardLayout: '%s' (local '%s')\n", szKeyboardLayout, LocalLayout));
    }

    G_pUiData->KeyboardLayout = miGetPrivateProfileLong( INI_WFCLIENT,
							szKeyboardLayout,
							(long)0 );

    TRACE((TC_WENG, TT_API1, "KeyboardLayout: %d\n", G_pUiData->KeyboardLayout));

#if 0
    G_pUiData->Lpt1  = miGetPrivateProfileInt( INI_WFCLIENT, INI_LPT1, DEF_LPT1 );
    G_pUiData->Port1 = miGetPrivateProfileInt( INI_WFCLIENT, INI_PORT1, DEF_PORT1 );

    G_pUiData->Lpt2  = miGetPrivateProfileInt( INI_WFCLIENT, INI_LPT2, DEF_LPT2 );
    G_pUiData->Port2 = miGetPrivateProfileInt( INI_WFCLIENT, INI_PORT2, DEF_PORT2 );

    G_pUiData->Lpt3  = miGetPrivateProfileInt( INI_WFCLIENT, INI_LPT3, DEF_LPT3 );
    G_pUiData->Port3 = miGetPrivateProfileInt( INI_WFCLIENT, INI_PORT3, DEF_PORT3 );
#endif

    /*
     *  Copy client build number
     */
    G_pUiData->ClientBuildNumber = VER_CLIENTBUILD;

    /*
     *  Copy domain
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    miGetPrivateProfileString( INI_SERVERSECTION, INI_DOMAIN, DEF_DOMAIN,
                               pBuf, DOMAIN_LENGTH+1 );

    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oDomain = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: Domain: %s", pBuf ));
    }

    /*
     *  Copy username
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    miGetPrivateProfileString( INI_SERVERSECTION, INI_USERNAME, DEF_USERNAME,
                               pBuf, USERNAME_LENGTH+1 );

    miGetPrivateProfileString( INI_SERVERSECTION, INI_ENCRYPTIONLEVELSESSION, DEF_ENCRYPTIONLEVEL,
                               g_szEncryptionLevelConnStatus, sizeof(g_szEncryptionLevelConnStatus) );

    if ( (Length = strlen(pBuf)) > 0 ) {
        /*
         * Test: if EncryptionLevel is not Basic, then do not allow the client to connect
         *       by returning an error. Reason: User cannot specify Username and password
         *       when EncryptionLevel is not basic
         */
#ifndef DOS
        if ( lstrcmpi( g_szEncryptionLevelConnStatus, INI_ENCRYPTION_BASIC) ) {
#else
        if ( strcmpi( g_szEncryptionLevelConnStatus, INI_ENCRYPTION_BASIC) ) {
#endif
        /* This code is added for the Banker's trust - they wanted the autologon
         *  for the Secure ICA client as well - for that they will have to add
         *  the flag AutoLogonAllowed in the INI file
         */


        fAutoLogonAllowed=miGetPrivateProfileBool(INI_SERVERSECTION,AUTOLOGON,DEF_AUTOLOGON);

        if (!fAutoLogonAllowed)
           return( CLIENT_ERROR_ENCRYPT_LEVEL_INCORRECTUSE );
        }

        G_pUiData->oUserName = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: Username: %s", pBuf ));
    }

    /*
     *  Copy password
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    miGetPrivateProfileString( INI_SERVERSECTION, INI_PASSWORD, DEF_PASSWORD,
                               szEncryptedPassword,
                               sizeof(szEncryptedPassword) );

    if ( *szEncryptedPassword ) {
        Len = DecryptFromAscii( szEncryptedPassword,
                                (USHORT)strlen( szEncryptedPassword ),
                                szPassword,
                                sizeof(szPassword) );
    } else {
    Len=miGetPrivateProfileString( INI_SERVERSECTION, INI_CLEAR_PASSWORD, DEF_CLEAR_PASSWORD,
                                   szPassword,
                                   sizeof(szPassword) );
        if (!(*szPassword)) Len = 0;
   else szClearPasswordTag=TRUE;
    }
    szPassword[Len] = 0;

    if ( (Length = strlen(szPassword)) > 0 ) {

        /*
         * Test: if EncryptionLevel is not Basic, then do not allow the client to connect
         *       by returning an error. Reason: User cannot specify Username and password
         *       when EncryptionLevel is not basic
         */

       if (szClearPasswordTag==FALSE)
   {
#ifndef DOS
        if ( lstrcmpi( g_szEncryptionLevelConnStatus, INI_ENCRYPTION_BASIC) ) {
#else
        if ( strcmpi( g_szEncryptionLevelConnStatus, INI_ENCRYPTION_BASIC) ) {
#endif
        /* This code is added for the Banker's trust - they wanted the autologon
         *  for the Secure ICA client as well - for that they will have to add
         *  the flag AutoLogonAllowed in the INI file
         */


        fAutoLogonAllowed=miGetPrivateProfileBool(INI_SERVERSECTION,AUTOLOGON,DEF_AUTOLOGON);

        if (!fAutoLogonAllowed)
           return( CLIENT_ERROR_ENCRYPT_LEVEL_INCORRECTUSE );
   }
        }

        memcpy(pBuf, szPassword, Length);
        pBuf[Length] = 0;
        G_pUiData->oPassword = Offset;
        Offset += (Length + 1);  // skip over trailing null

        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: Password: %s", pBuf ));

    }

    /*
     *  Copy client path
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    strcpy( pBuf, gpszExePath );
    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oClientDirectory = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: ClientDirectory: %s", pBuf ));
    }

    /*
     *  Copy working directory
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    miGetPrivateProfileString( INI_SERVERSECTION, INI_WORKDIRECTORY, DEF_WORKDIRECTORY,
                               pBuf, DIRECTORY_LENGTH+1 );

    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oWorkDirectory = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: WorkDirectory: %s", pBuf ));
    }

    /*
     *  Copy initial program
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    miGetPrivateProfileString( INI_SERVERSECTION, INI_INITIALPROGRAM, DEF_INITIALPROGRAM,
                               pBuf, INITIALPROGRAM_LENGTH+1 );

    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oInitialProgram = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: InitialProgram: %s", pBuf ));
    }

    /*
     *  Copy client name (not in INI buffer).
     */
    pBuf = (LPBYTE)G_pUiData + Offset;

    if ( gpszClientname &&
         (Length = strlen(gpszClientname)) ) {
       strcpy( pBuf, gpszClientname );
    } else {
        strcpy( pBuf, DEF_CLIENTNAME );
    }

    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oClientName = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: Clientname: %s", pBuf ));
    }

    /*
     *  Copy client license
     */
    pBuf = (LPBYTE)G_pUiData + Offset;

    if ( (Length = strlen(gpszClientSN)) > 0 ) {
        strcpy( pBuf, gpszClientSN );
        G_pUiData->oClientLicense = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: ClientLicense: %s", pBuf ));
    }

    /*
     *  Copy IME file name
     */
    G_pUiData->oimeFileName = 0;
#ifdef DBCS
    pBuf = (LPBYTE)G_pUiData + Offset;
    if ( (Length = strlen(szimeFileName)) > 0 ) {
        strcpy( pBuf, szimeFileName );
        G_pUiData->oimeFileName = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: imeFileName: %s", pBuf ));
    }
#endif

    /*
     *  Realloc data buffer to exact size
     */
    ASSERT( Offset <= G_pUiDataLength, Offset );
    G_pUiDataLength = Offset;
    G_pUiData = realloc( G_pUiData, G_pUiDataLength );

    ASSERT( G_pUiData, 0 );
    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: UiOpen size(%d)", Offset ));
    return( CLIENT_STATUS_SUCCESS );
}

#ifdef UNICODESUPPORT
/*******************************************************************************
 *
 * SwitchUiToUnicode
 *
 * Changes the G_pUiData packet to contain Unicode
 * data, instead of ASCII data.  We need to do this
 * because by the time the client knows that it is
 * communicating with a Unicode-enabled server, the
 * UiOpen function has already been called and the
 * packet has already been initialized.  When we
 * receive the PACKET_INIT_REQUEST from the host,
 * we need to go back and make the necessary changes
 * if the server level is 4 or greater.
 *
 * ENTRY:
 *   none
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 *******************************************************************************/

USHORT TransferAndConvert( USHORT *oString, WCHAR *szUnicodeBuffer ) {

    static USHORT oBufferOffset;
    CHAR *        lpBufferA;
    WCHAR *       lpBufferW;

    if ( *oString > 0 ) {

        // point lpBufferA to the string indicated
        // by the offset oString for G_pUiData
        lpBufferA = (LPBYTE)G_pUiData + *oString;
        lpBufferW = (WCHAR *) ( (LPBYTE)szUnicodeBuffer + oBufferOffset );

        // convert the string in G_pUiData to
        // Unicode and transfer to the new buffer
        MultiByteToWideChar( CP_ACP,
                             0,
                             lpBufferA,
                             -1,
                             lpBufferW,
                             strlen( lpBufferA ) + sizeof(CHAR)
                           );

        *oString = oBufferOffset + sizeof(UI_C2H);

        oBufferOffset += sizeof(WCHAR) * ( strlen( lpBufferA ) + 1 );

        return( oBufferOffset );

    } else {

        return( oBufferOffset );

    }

}

int SwitchUiToUnicode() {

    USHORT    oDataSectionStart,
              oDataSectionFinalSize;
   WCHAR *   szDataSectionNew;
    LPBYTE    szDataSectionFinal;
   DWORD     dwDataSectionSize;

   // set the offset to point to the
   // data section of G_pUiData
    oDataSectionStart = sizeof(UI_C2H);
   dwDataSectionSize = G_pUiDataLength - sizeof(UI_C2H);

   // create the new data section
   szDataSectionNew = (WCHAR *) HeapAlloc ( GetProcessHeap(), 0, dwDataSectionSize * sizeof(WCHAR) );

   TransferAndConvert( &G_pUiData->oDomain, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oUserName, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oPassword, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oClientDirectory, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oWorkDirectory, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oInitialProgram, szDataSectionNew );
    //TransferAndConvert( &G_pUiData->oCloudName, szDataSectionNew );
    TransferAndConvert( &G_pUiData->oClientName, szDataSectionNew );
    oDataSectionFinalSize = TransferAndConvert( &G_pUiData->oClientLicense, szDataSectionNew );

    // realloc G_pUiData to the exact size of the data structure
    G_pUiDataLength = oDataSectionStart + oDataSectionFinalSize;
    G_pUiData = realloc( G_pUiData, G_pUiDataLength );

    // now copy the data from the buffer over
    szDataSectionFinal = (LPBYTE)G_pUiData + oDataSectionStart;
    memset( szDataSectionFinal, 0, oDataSectionFinalSize );
    memcpy( szDataSectionFinal, (LPBYTE) szDataSectionNew, oDataSectionFinalSize );

   // now set the encoding type to Unicode
   G_pUiData->EncodingType = 1;

   // reclaim the heap
   HeapFree( GetProcessHeap(), 0, szDataSectionNew );
   return ( CLIENT_STATUS_SUCCESS );

}
#endif

/*******************************************************************************
 *
 *  UiInfo
 *
 *  Return dll information
 *
 * ENTRY:
 *    pNotUsed (input)
 *       not used
 *    pUiInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

INT WFCAPI UiInfo( PVOID pNotUsed, PDLLINFO pUiInfo )
{
    PMODULE_C2H pHeader;
    USHORT ByteCount;
    PUI_C2H pUiData;
    LPBYTE       pBuf;
    ENCRYPTIONINIT    EncryptionInit;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = G_pUiDataLength;

    /*
     *  Check if buffer is big enough
     */
    if ( pUiInfo->ByteCount < ByteCount ) {
        pUiInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     * Encrypt if it needs to be
     */
    if ( !G_Encrypted ) {

        G_Encrypted = TRUE;

        if ( !wdEncryptionInit( &EncryptionInit, sizeof(EncryptionInit) ) ) {

            if ( EncryptionInit.EncryptionLevel ) {

                /*
                 * For now this is the only encryption level we support.
                 */
                G_pUiData->EncryptionLevel = 1;

                G_pUiData->FixedLength = sizeof(UI_C2H);
                G_pUiData->OffsetLength = G_pUiDataLength - sizeof(UI_C2H);

                G_pUiData->EncryptionSeed = 0;  /* Seed is random */

                pBuf = (LPBYTE)G_pUiData + sizeof(UI_C2H);

                RunEncodeBuffer( &G_pUiData->EncryptionSeed, pBuf,
                                 (USHORT)(G_pUiDataLength - (USHORT)sizeof(UI_C2H)) );
            }
        }
    }

    /*
     *  Initialize default data
     */
    pUiInfo->ByteCount = ByteCount;
    pUiData = (PUI_C2H) pUiInfo->pBuffer;
    memcpy( pUiData, G_pUiData, G_pUiDataLength );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pUiData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_UserInterface;
    pHeader->VersionL = VERSION_CLIENTL_UI;
    pHeader->VersionH = VERSION_CLIENTH_UI;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: UiInfo size(%d)", ByteCount ));
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  Function: srvWFEngOpen
 *
 *  Purpose: Open windows engine (WFENG) API
 *
 *  Entry:
 *     pWFEOpen (input)
 *        pointer to WFEOPEN data
 *     phWFE (output)
 *        pointer to place to return handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngOpen( PWFEOPEN pWFEOpen, LPHANDLE phWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    CHAR *p;
    int i;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngOpen: (entered)" ));
    gVersion = pWFEOpen->Version;

      // filter out "#" for client name, it uses for icaname
    p = pWFEOpen->pszClientname;
    for ( i = 0; i < CLIENTNAME_LENGTH; i++, p++) {
          if (*p == '\0') break;
          if ( *p == '#')  *p = '~';
         }
    strcpy( gpszClientname, pWFEOpen->pszClientname );
    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngOpen, Client name %s", gpszClientname ));


    // only works on 1.5 plus
    if ( gVersion >= 2) {
        strcpy( gpszClientSN, pWFEOpen->pszClientSN );
    }
    gpfnStatusMsgProc = pWFEOpen->pfnStatusMsgProc;
#if defined(DOS) && !defined(RISCOS)
    gpfnUIPoll = pWFEOpen->pfnUIPoll;
#endif

    KbdOpen();
    
#if 0
    /*
     * Create a main window for this application instance.
     */
    if ( !(ghwndMain = CreateMainWindow( ghInstance, SW_SHOWNORMAL, pWFEOpen ))  ) {
       rc = (INT)GetLastError();
       TRACE(( TC_WENG, TT_ERROR,
           "WFEngx.Exe: srvWFEngOpen: Error (%d) creating MainWindow - aborting", rc ));
       if(rc==0)
          rc = CLIENT_ERROR_NO_MEMORY;
       goto done;
    }
#endif
done:
    if ( rc ) {
#ifndef DOS
       if ( ghwndMain ) {
          DestroyWindow( ghwndMain );
          ghwndMain = NULL;
       }
#endif
    }

    ghWFE = *phWFE = (HANDLE)ghwndMain;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngOpen: Exit rc = %d", rc ));
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngOpen
 *
 *  Purpose: Open windows engine (WFENG) API
 *
 *  Entry:
 *     pWFEOpen (input)
 *        pointer to WFEOPEN data
 *     phWFE (output)
 *        pointer to place to return handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngOpen( PWFEOPEN pWFEOpen, LPHANDLE phWFE )
{
   return(srvWFEngOpen( pWFEOpen, phWFE ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngClose
 *
 *  Purpose: Close windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngClose( HANDLE hWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    KbdClose();
    
    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngClose: %08lX rc(%d)",
            (LONG)hWFE, rc ));

    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngClose
 *
 *  Purpose: Close windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngClose( HANDLE hWFE )
{
   return(srvWFEngClose( hWFE ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngLoadSession
 *
 *  Purpose: Loads Exe (for global session information) driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngLoadSession( HANDLE hWFE, PWFELOAD pWFELoad )
{
    INT          rc = CLIENT_STATUS_SUCCESS;
    EXEOPEN      ExeOpen;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoadSession: %08lX", (LONG)hWFE ));

    /*
     * Before we use any Gets on the INI information we must allow
     * the miGet routines to initialize its Global pointer to the
     * pIniProfile structure.
     */
    if (!miSetProfilePointer( pWFELoad->pIniSection )) {
         //if the profile was not set then we have an old-styled buffered ini
         miSetSectionPointer( pWFELoad->pIniSection);
    }

    /*
     * Open the Exe
     */
    memset( &ExeOpen, 0, sizeof( ExeOpen ) );
    ExeOpen.pIniSection = pWFELoad->pIniSection;
    if ( rc = ModuleCall( &gExeLink, DLL__OPEN, &ExeOpen ) ) {
       TRACE(( TC_WENG, TT_ERROR,
            "WFEngx.Exe: srvWFEngLoadSession: Error (%d) opening Exe - aborting", rc ));
       goto done;
    }
    
    gState |= WFES_LOADEDSESSION;

    {
    char pszBuffer[20];
    char pszINIKey[20];
    UCHAR ScanCode;
    INT   ShiftState;
    INT   i;

    /*
     *  Determine if this is a Async connection
     */
    if ( miGetPrivateProfileString( INI_TRANSPORTSECTION,
                                    INI_DEVICE,              // "DeviceName"
                                    INI_EMPTY,               //
                                    pszBuffer,               //
                                    sizeof(pszBuffer) ) ) {  //
        G_fAsync = TRUE;
    }


#ifdef WIN32

    miGetPrivateProfileString(INI_SERVERSECTION,
                              INI_TRANSPORTDRIVER,
                              DEF_TRANSPORTDRIVER,
                              pszBuffer, sizeof(pszBuffer));

    if (lstrcmpi(pszBuffer,IPX_PROTOCOL)==0)
        G_fIpx=TRUE;

#endif

#ifndef DOS
#ifndef WINCE
    if(gbIPCEngine)
#endif
#endif
    for ( i = 0; gHotkeys[i].pszShiftState; i++ ) {
       /*
        * Get hotkey i shift state
        */
       sprintf( pszINIKey, INI_HOTKEY_SHIFT, i+1 );
       miGetPrivateProfileString( INI_WFCLIENT, pszINIKey,
                                  gHotkeys[i].pszShiftState,
                                  pszBuffer, sizeof(pszBuffer) );
       ShiftState = miGetPrivateProfileInt( INI_HOTKEY_SHIFTSTATES, pszBuffer,
                                            gHotkeys[i].ShiftState );

       /*
        * Get hotkey i key
        */
       sprintf( pszINIKey, INI_HOTKEY_CHAR, i+1 );
       miGetPrivateProfileString( INI_WFCLIENT, pszINIKey,
                                  gHotkeys[i].pszScanCode,
                                  pszBuffer, sizeof(pszBuffer) );
       ScanCode   = (UCHAR)miGetPrivateProfileInt( INI_HOTKEY_KEYS, pszBuffer,
                                                   gHotkeys[i].ScanCode );

       TRACE(( TC_WENG, TT_L1,
               "WFEngx.Exe: srvWFEngLoadSession reg hotkey(%d) Scan(%d) shift(%X)",
               CLIENT_STATUS_HOTKEY1+i, ScanCode, ShiftState ));
       KbdRegisterHotkey( CLIENT_STATUS_HOTKEY1+i, ScanCode, ShiftState );
    }
    }

#if 0
    {
    MOUPREFERENCES MPref;
    KBDPREFERENCES KPref;
    char szSwap[4];

    KPref.KeyboardDelay = miGetPrivateProfileInt( INI_WFCLIENT,
                                                  INI_KEYBOARDDELAY,
                                                  DEF_KEYBOARDDELAY );
    KPref.KeyboardSpeed = miGetPrivateProfileInt( INI_WFCLIENT,
                                                  INI_KEYBOARDSPEED,
                                                  DEF_KEYBOARDSPEED );

    KbdLoadPreferences( &KPref );

    MPref.HorizSpeed = miGetPrivateProfileInt( INI_WFCLIENT,
                                               INI_HORIZONTALSPEED,
                                               DEF_HORIZONTALSPEED );
    MPref.VertSpeed = miGetPrivateProfileInt( INI_WFCLIENT,
                                              INI_VERTICALSPEED,
                                              DEF_VERTICALSPEED );
    MPref.DblSpeedThreshold = miGetPrivateProfileInt( INI_WFCLIENT,
                                                      INI_DBLSPEEDTHRESHOLD,
                                                      DEF_DBLSPEEDTHRESHOLD );

    miGetPrivateProfileString( INI_WFCLIENT, INI_SWAPBUTTONS, DEF_SWAPBUTTONS,
                               szSwap, sizeof(szSwap) );
    szSwap[sizeof(szSwap)-1] = '\0';

    if ( !strcmpi( szSwap, INI_YES) ) {
        MPref.fSwapButtons = TRUE;
    } else {
        MPref.fSwapButtons = FALSE;
    }
    MouseLoadPreferences( &MPref );
    }
#endif

done:
    pWFELoad->handle = rc ? (HANDLE)1 : (HANDLE)0; // Handle not used in this version
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngLoadSession
 *
 *  Purpose: Loads Exe (for global session information) driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngLoadSession( HANDLE hWFE, PWFELOAD pWFELoad )
{
  return(srvWFEngLoadSession( hWFE, pWFELoad ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngLoadWd
 *
 *  Purpose: Loads WD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngLoadWd( HANDLE hWFE, PWFELOAD pWFELoad )
{
    INT          rc = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoadWd: %08lX", (LONG)hWFE ));

    /*
     * Before the WD does any gets for INI information we must set the
     * pointer to the Buffered INI section in case we are using an old-styled
     * top-end which uses the bINI routines still
     */
    miSetSectionPointer( pWFELoad->pIniSection );

    if ( (rc = LoadWd( (PCHAR)(pWFELoad->pIniSection),
                       (PCHAR)(pWFELoad->pszModuleName),
                       (PCHAR)(gpszDllPath),
                       &gWdLink,
                       &gRedLink,
                       &gPdLink,
                       &gPdOpen,
                       &gMaxVirtualChannels ))
                        != CLIENT_STATUS_SUCCESS )
        goto done;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoadWd: MaxVirtualChannels %u",
            gMaxVirtualChannels ));

    /*
     * Allocate array of pointers to virtual drivers.
     */
    if ( (gpaVdLink = malloc(sizeof(PDLLLINK)
                        * (gMaxVirtualChannels))) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    memset( gpaVdLink, 0, sizeof(PDLLLINK) * gMaxVirtualChannels );

    gState |= WFES_LOADEDWD;

    gbPollingEnabled = TRUE;

done:
    pWFELoad->handle = rc ? (HANDLE)1 : (HANDLE)0; // Handle not used in this version
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngLoadWd
 *
 *  Purpose: Loads WD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngLoadWd( HANDLE hWFE, PWFELOAD pWFELoad )
{
   return(srvWFEngLoadWd( hWFE, pWFELoad ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngLoadPd
 *
 *  Purpose: Loads PD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngLoadPd( HANDLE hWFE, PWFELOAD pWFELoad )
{
    INT          rc = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoadPd: %08lX", (LONG)hWFE ));

    /*
     * Before the PD does any gets for INI information we must set the
     * pointer to the Buffered INI section in case we are using an old-styled
     * top-end which uses the bINI routines still
     */
    miSetSectionPointer( pWFELoad->pIniSection );

    rc = LoadPd( (PCHAR)(pWFELoad->pIniSection),
                 (PCHAR)(pWFELoad->pszModuleName),
                 (PCHAR)(gpszDllPath),
                 &gWdLink, &gPdLink, &gPdOpen );

    gState |= WFES_LOADEDPD;

    pWFELoad->handle = rc ? (HANDLE)1 : (HANDLE)0; // Handle not used in this version
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngLoadPd
 *
 *  Purpose: Loads PD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngLoadPd( HANDLE hWFE, PWFELOAD pWFELoad )
{
   return(srvWFEngLoadPd( hWFE, pWFELoad ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngLoadVd
 *
 *  Purpose: Loads VD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngLoadVd( HANDLE hWFE, PWFELOAD pWFELoad )
{
    INT          rc = CLIENT_STATUS_SUCCESS;
    USHORT       Channel = 0;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngLoadVd: %08lX", (LONG)hWFE ));

    /*
     * Before the VD does any gets for INI information we must set the
     * pointer to the Buffered INI section in case we are using an old-styled
     * top-end which uses the bINI routines still
     */
    miSetSectionPointer( pWFELoad->pIniSection );

    rc = LoadVd( (PCHAR)(pWFELoad->pIniSection),
                 (PCHAR)(pWFELoad->pszModuleName),
                 (PCHAR)(gpszDllPath),
                 &gWdLink, gpaVdLink,
                 gMaxVirtualChannels, &Channel );

    gState |= WFES_LOADEDVD;

    pWFELoad->handle = (HANDLE)Channel;  // For Vds, the handle is the channel
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngLoadVd
 *
 *  Purpose: Loads VD driver
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     pWFELoad (input)
 *        pointer to WFELOAD data
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngLoadVd( HANDLE hWFE, PWFELOAD pWFELoad )
{
   return(srvWFEngLoadVd( hWFE, pWFELoad ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngUnloadDrivers
 *
 *  Purpose: Unload driver stack
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngUnloadDrivers( HANDLE hWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    if ( gState &
         (WFES_LOADEDWD | WFES_LOADEDPD | WFES_LOADEDVD) ) {
       wdDisconnect();
       wdDestroyWindow( (HWND)hWFE );
       UnloadDrivers( &gWdLink, &gRedLink, &gPdLink, gpaVdLink, gMaxVirtualChannels );
       gState &= ~(WFES_LOADEDWD | WFES_LOADEDPD | WFES_LOADEDVD | WFES_LOADEDSESSION);
       memset( &gWdLink, 0, sizeof(gWdLink) );
       memset( &gRedLink, 0, sizeof(gRedLink) );
       memset( &gPdLink, 0, sizeof(gPdLink) );
       memset( &gMaxVirtualChannels, 0, sizeof(gMaxVirtualChannels) );
       free( gpaVdLink );

    }
    gbVdAddrToWd = FALSE;
    gbPollingEnabled = FALSE;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngUnloadDrivers: %08lX rc(%d)",
            (LONG)hWFE, rc ));
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngUnloadDrivers
 *
 *  Purpose: Unload driver stack
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngUnloadDrivers( HANDLE hWFE )
{
   return(srvWFEngUnloadDrivers( hWFE ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngConnect
 *
 *  Purpose: Connect to application server
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngConnect( HANDLE hWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngConnect (entered)" ));

    /*
     * Let Wd know about Vds.
     */
    wdVdAddress();

#ifdef WIN32
#ifndef VDTW_THREAD
    /*
     * Tell the Wd to pass the thinwire stack to the Vd
     */
    wdThinwireStack( &vTWStack );
#endif
#endif

    /*
     *  Connect to application server
     */
    if ( !(rc = wdConnect( (HWND)hWFE )) ) {

#ifndef DOS
        /*
         * We're posting a NOP message just to break out of the GetMessage loop
         * and go into our customized PeekMessage loop
         */
        PostMessage( ghwndMain, WM_WFENG_WAKEUP, 0, 0 );
#else
    gState |= WFES_BADSTATUS;
#endif
    }

    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngConnect
 *
 *  Purpose: Connect to application server
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngConnect( HANDLE hWFE )
{
   return(srvWFEngConnect( hWFE ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngDisconnect
 *
 *  Purpose: Disconnect from application server
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngDisconnect( HANDLE hWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    miReleaseProfile();

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngDisconnect (entered)" ));

    /*
     *  Disconnect from application server
     */
    rc = wdDisconnect();

    StopPolling();
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngDisconnect
 *
 *  Purpose: Disconnect from application server
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngDisconnect( HANDLE hWFE )
{
   return(srvWFEngDisconnect( hWFE ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngSetInformation
 *
 *  Purpose: Give info to windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     type (input)
 *        type of information
 *     pData (input)
 *        type-specific parameter block
 *     cbData (input)
 *        size of pData
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngSetInformation( HANDLE hWFE, WFEINFOCLASS type,
                               LPVOID pData, UINT cbData )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    /*
     * Before we do Wd actions, make sure wd is loaded
     */
    switch ( type ) {
        case WFEAddReadHook:
            rc = wdAddReadHook( pData );
            break;

        case WFERemoveReadHook:
            rc = wdRemoveReadHook();
            break;

        case WFESetFocus:
            rc = wdSetFocus();
            break;

        case WFEKillFocus:
            rc = wdKillFocus();
            break;

        case WFEVdInfo:
            rc = vdInfo( (PWFEVDINFO)pData, TRUE );
            break;

        case WFEHotkey:
#ifndef DOS
#ifndef WINCE
            if(gbIPCEngine)
#endif
#endif
               rc = wRegisterHotkey( (PWFEHOTKEYINFO)pData );
            break;

        case WFESetDefaultMode:
            rc = wdSetDefaultMode( (PWFEDEFAULTMODE)pData, (USHORT)cbData );
            break;

        case WFESetProductID:
            rc = wdSetProductID( (PWFEPRODUCTID)pData, (USHORT)cbData );
            break;

        case WFERaiseSoftkey:
            rc = wdSetInfoPassthru( WdRaiseSoftkey );
            break;

        case WFELowerSoftkey:
            rc = wdSetInfoPassthru( WdLowerSoftkey );
            break;

        default:
            rc = CLIENT_ERROR_INVALID_CALL;
            break;
    }

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngSetInformation: %08lX rc(%d)",
            (LONG)hWFE, rc ));
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngSetInformation
 *
 *  Purpose: Give info to windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     type (input)
 *        type of information
 *     pData (input)
 *        type-specific parameter block
 *     cbData (input)
 *        size of pData
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngSetInformation( HANDLE hWFE, WFEINFOCLASS type,
                               LPVOID pData, UINT cbData )
{
   return(srvWFEngSetInformation( hWFE, type, pData, cbData ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngQueryInformation
 *
 *  Purpose: Get info from windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     type (input)
 *        type of information
 *     pData (output)
 *        type-specific parameter block
 *     cbData (input)
 *        size of pData
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngQueryInformation( HANDLE hWFE, WFEINFOCLASS type,
                                 LPVOID pData, UINT cbData )
{
    INT rc = CLIENT_STATUS_SUCCESS;

    switch ( type ) {
        case WFELastError:
            rc = wdLastError( pData, cbData );
            break;

        case WFEThinwireCache:
            rc = vdThinwireCache( pData, cbData );
            break;

        case WFEWindowHandle:
            if ( cbData < sizeof( hWFE ) ) {
                rc = CLIENT_ERROR_BUFFER_TOO_SMALL;
            } else {
                *(LPHANDLE)pData = hWFE;
            }
            break;

        case WFEVdInfo:
            rc = vdInfo( (PWFEVDINFO)pData, FALSE );
            break;

        case WFEClientDataServer:
            rc = wdGetClientDataServer( (LPSTR)pData, (USHORT)cbData );
            break;

        case WFEClientDataDomain:
            rc = wdGetClientDataDomain( (LPSTR)pData, (USHORT)cbData );
            break;

        case WFEClientDataUsername:
            rc = wdGetClientDataUsername( (LPSTR)pData, (USHORT)cbData );
            break;

        default:
            rc = CLIENT_ERROR_INVALID_CALL;
            break;
    }

    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: srvWFEngQueryInformation: %08lX rc(%d) cbData(%d)",
            (LONG)hWFE, rc, cbData ));
    TRACEBUF(( TC_WENG, TT_L1, (LPBYTE)pData, cbData ));
    return( rc );
}

/*******************************************************************************
 *
 *  Function: WFEngQueryInformation
 *
 *  Purpose: Get info from windows engine (WFENG) API
 *
 *  Entry:
 *     hWFE (input)
 *        handle
 *     type (input)
 *        type of information
 *     pData (output)
 *        type-specific parameter block
 *     cbData (input)
 *        size of pData
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngQueryInformation( HANDLE hWFE, WFEINFOCLASS type,
                                 LPVOID pData, UINT cbData )
{
   return(srvWFEngQueryInformation( hWFE, type, pData, cbData ));
}

/*******************************************************************************
 *
 *  Function: srvWFEngPoll
 *
 *  Purpose: Poll wengine
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI srvWFEngPoll( HANDLE hWFE )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    INT rc2;

    if ( gState & WFES_CONNECTED ) {
       rc = wWFEngPoll();
    }

    if ( gState & WFES_FOCUS ) {
       /*
        *  Check for input
        */
       rc2 = InputPoll();

       if ( !hWFE ) {
           hWFE = ghWFE;
       }

       StatusMsgProc( hWFE, rc2 );

       /*
        * Figure out which return code to return
        */
       if ( rc2 ) {
           switch ( rc ) {
               case CLIENT_STATUS_SUCCESS:
               case CLIENT_STATUS_NO_DATA:
                   rc = rc2;
                   break;
           }
       }
    }

    return( rc );
}

#if 0
/*******************************************************************************
 *
 *  Function: WFEngPoll
 *
 *  Purpose: Poll wengine
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT WFCAPI WFEngPoll( HANDLE hWFE )
{
   return(srvWFEngPoll( hWFE ));
}
#endif

/*******************************************************************************
 *
 *  Function: wWFEngPoll
 *
 *  Purpose: Worker for WFEngPoll
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT wWFEngPoll( VOID )
{
    INT     rc = CLIENT_STATUS_SUCCESS;
    DLLPOLL DllPoll;
    BOOL    fAbort = FALSE;
static BOOL vfFirstTime = TRUE;

    if ( vfFirstTime ) {          // Handle case if WFEngPoll is called instead
        gbContinuePolling = TRUE; // of WFEngMessageLoop
        vfFirstTime = FALSE;
    }

    if ( gbPollingEnabled && gbContinuePolling ) {
        DllPoll.CurrentTime = Getmsec();
        rc = ModuleCall( &gWdLink, DLL__POLL, &DllPoll );


        //policy on when to really say no data
        //we wait around for an extra 100ms to make sure that things
        //have really quiesced before entering the hard sleep loop
        if (rc == CLIENT_STATUS_NO_DATA) {
           //TRACE((TC_WENG,TT_L2,"Wengine.c wWFEngPoll rc==CLIENT_STATUS_NO_DATA\n"));
           if (NoDataTime) {
              if (DllPoll.CurrentTime < NoDataTime) {
                 rc = CLIENT_STATUS_SUCCESS;    //too soon for no data
              }

           }
           else {
              NoDataTime = DllPoll.CurrentTime + 100;
              rc = CLIENT_STATUS_SUCCESS;       //too soon for no data
           }
        }
        else NoDataTime = 0;


        /*
         * If we get a return code which will lead to termination,
         * disable the poll loop (which would have been called during Ipc waits)
         */
        if ( (rc == CLIENT_STATUS_CONNECTION_BROKEN_HOST) || (rc >= CLIENT_ERROR) ) {
            gbPollingEnabled = FALSE;
            fAbort = TRUE;
        }

        /*
         * Tell the UI what's going on
         */
        StatusMsgProc( ghWFE, rc );

        /*
         * If it is an abort condition, tell the UI and bail
         */
        if ( fAbort ) {
           if ( rc >= CLIENT_ERROR ) {
              StatusMsgProc( ghWFE, rc );
           }
           StopPolling();
        }
    }

    return( rc );
}

/*******************************************************************************
 *
 *  Function: StatusMsgProc
 *
 *  Purpose: Status message router
 *
 *  Entry:
 *     hWFE (input)
 *        instance handle
 *     message (input)
 *        status message returned from wdPoll
 *
 *  Exit:
 *     LRESULT - message specific return
 *
 ******************************************************************************/
LRESULT WFCAPI StatusMsgProc( HANDLE hWFE, INT message )
{
       BOOL   fProcCalled = FALSE;
       LRESULT lResult    = 0;
       LPARAM lParam      = 0;
static INT  messageHotkey = 0;

#define NotifyUI( a, b, c ) \
if ( gpfnStatusMsgProc ) \
lResult = (*(gpfnStatusMsgProc))( a, b, c ); \
fProcCalled = TRUE;

/*
 * Note - any lParam value that needs to be sent needs to be thunked
 *        in DDESERV.C and ..\WFCWIN\DDECLIEN.C
 */
    switch ( message ) {
       case CLIENT_STATUS_SUCCESS :
           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case CLIENT_STATUS_NO_DATA :
           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case CLIENT_STATUS_TTY_CONNECTED :
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: CLIENT_STATUS_TTY_CONNECTED" ));
           if ( G_fEchoTTY ) {
               NotifyUI( hWFE, CLIENT_STATUS_DELETE_CONNECT_DIALOG, 0 );
               fProcCalled = FALSE;
               wdSetFocus();
#ifndef DOS
#ifdef WINCE
               SetCursor( LoadCursorW( NULL, IDC_ARROW ) );
#else
               SetCursor( LoadCursor( NULL, IDC_ARROW ) );
#endif
#endif
           }

#ifdef WIN32
//  This code was added to make sure that if the Server maximum connection limit
//  is reached and it is no longer accepting the connections, we can timeout
            else if (G_fIpx){
                gfIpxWatchdogStarted=TRUE;
                SetTimer(hWFE,IDT_IPX_WATCHDOG,IPX_TIMEOUT, (TIMERPROC) IpxTimeoutProc);
            }
#endif
        
           break;

       case CLIENT_STATUS_CONNECTED :
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: CLIENT_STATUS_CONNECTED" ));
           if ( !G_fEchoTTY ) {
#ifdef WIN32
               if (G_fIpx && gfIpxWatchdogStarted) 
                    KillTimer(hWFE,IDT_IPX_WATCHDOG);
#endif
               NotifyUI( hWFE, CLIENT_STATUS_DELETE_CONNECT_DIALOG, 0 );
               fProcCalled = FALSE;
               wdSetFocus();
           }
#ifndef DOS
           //
           // For initial time through, show and force local paint of
           // window now (without title bar if we're fullscreen)
           //
           if ( !InitialShowWindow ) {
              ShowWindow( ghwndMain, SW_SHOWNORMAL );

#ifndef DOS
#ifndef WINCE
              if(gbIPCEngine)
#endif
#endif
                 if ( gfRemoveTitleBar )
                    ToggleWindowDressing( ghwndMain );

              //
              // Sometimes, Win95 doesn't make the window visible.
              // This is harmless and will take care of the wierd
              // Win95 scenario.
#ifndef WINCE
              if ( !IsWindowVisible(ghwndMain) )
                 ShowWindow( ghwndMain, SW_RESTORE );


#endif // WINCE

#ifdef WIN32
              //
              // BUGBUG: Win32 needs to be in foreground and on top.  NT
              // will give keyboard focus to this window even if it's
              // not active, so we make sure that it's active and on top,
              // even though this is very undesirable behavior!  This needs
              // to be figured out and fixed in order to get launcher-passed
              // showstate stuff to work (like to start minimized) once we
              // put that feature in.
              //
              if(!g_hWndPlugin) {
                 BringWindowToTop( ghwndMain );
                 SetForegroundWindow( ghwndMain );
                 }
#endif

              InvalidateRect( ghwndMain, NULL, FALSE );
              LocalPaint( ghwndMain );

              InitialShowWindow = TRUE;
           }
#endif
           break;

       case HOTKEY_EXIT:
	   wdKillFocus();
           fProcCalled = TRUE; // Don't notify UI yet
           messageHotkey = message;
           break;

       case HOTKEY_CTRL_ALT_DEL: // Ctrl+Alt+Del
           SendKeySequence( "\x1d\x38\x53\xD3\xB8\x9D" );
           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case HOTKEY_CTRL_ESC: // Ctrl+Esc
           SendKeySequence( "\x1d\x01\x81\x9D" );
           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case HOTKEY_ALT_ESC: // Alt+Esc
#ifdef TWI_INTERFACE_ENABLED
  {
       ULONG ptru = (ULONG)GetWindowLong( ghwndMain, GWL_INSTANCEDATA );
       if( ptru ){

          ptru += sizeof(WFEINSTANCE);
          if( !(*((PULONG)ptru)) ){
           SendKeySequence( "\x38\x01\x81\xB8" );
          }
       }
       else {
           SendKeySequence( "\x38\x01\x81\xB8" );
       }
  }
#else   //TWI_INTERFACE_ENABLED
           SendKeySequence( "\x38\x01\x81\xB8" );
#endif  //TWI_INTERFACE_ENABLED

           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case HOTKEY_ALT_TAB: // Alt+Tab
           SendKeySequence( "\x0F\x8F" );
           fProcCalled = TRUE; // Don't notify UI on this
           break;

       case HOTKEY_ALT_BACKTAB: // Alt+Shift+Tab
           SendKeySequence( "\x2A\x0F\x8F\xAA" );
           fProcCalled = TRUE; // Don't notify UI on this
           break;


       case HOTKEY_CTRL_SHIFT_ESC: // Ctrl+Shift+Esc
           SendKeySequence( "\x1d\x2A\x01\x81\xAA\x9D" );
           fProcCalled = TRUE; // Don't notify UI on this
           break;


       case HOTKEY_UI: // UI hotkey
       {
	   extern int cli_suspendable; /* this comes from main.c */
	   TRACE(( TC_WENG, TT_L1,
		   "WFEngx.Exe: StatusMsgProc: UI Hotkey" ));
	   if (cli_suspendable)
	   {
	       wdKillFocus();
	       fProcCalled = TRUE; // Don't notify UI yet
	       messageHotkey = message;
	   }
           break;
       }

       case CLIENT_STATUS_KILL_FOCUS :
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: StatusKillFocus messageHotkey %d", messageHotkey ));
           if ( messageHotkey ) {
               NotifyUI( hWFE, messageHotkey, lParam );
               messageHotkey = 0;
               fProcCalled = FALSE; // Notify UI with the real message
#ifndef RISCOS
               wdSetFocus();
#endif
           }
           break;

       /*
        *  Reconnect to different appserver (load-balance)
        */
       case CLIENT_STATUS_RECONNECT_TO_HOST :
           {
           LOADBALANCE LoadBalance;

           /* get name of appserver to reconnect to */
           if ( !wdLoadBalance( &LoadBalance, sizeof(LoadBalance) ) ) {
#ifdef LATER
               if ( !batch )
                   printf( "Reconnect to: %s\n", LoadBalance.AppServer );
               /* BUGBUG - reconnect to new appserver */
#endif
           }
           }
           break;

       case CLIENT_STATUS_CONNECTION_BROKEN_HOST:
       case CLIENT_STATUS_CONNECTION_BROKEN_CLIENT:
           gbPollingEnabled = FALSE;
           NotifyUI( hWFE, message, lParam );
           break;

       /*
        * If there is no status message proc, default QUERY_CLOSE to TRUE
        */
       case CLIENT_STATUS_QUERY_CLOSE:
           if ( !gpfnStatusMsgProc ) {
               lResult = (LRESULT)TRUE;
           }
           break;

       /*
        *  Feedback status
        */
       case CLIENT_STATUS_SEND_START :
       case CLIENT_STATUS_SEND_STOP :
       case CLIENT_STATUS_RECEIVE_START :
       case CLIENT_STATUS_RECEIVE_STOP :
       case CLIENT_STATUS_ERROR_RETRY :
           fProcCalled = TRUE; // Don't notify UI on these
           break;

       default:
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: message(%d)", message ));
           break;
    }

    if ( !fProcCalled ) {
       NotifyUI( hWFE, message, lParam );
    }
    return( lResult );
}

/*******************************************************************************
 *
 *  Function: StopPolling
 *
 *  Purpose: Terminate the poll loop
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 - success, or error code
 *
 ******************************************************************************/
INT StopPolling( VOID )
{
   INT rc = CLIENT_STATUS_SUCCESS;

// gState &= ~WFES_CONNECTED;
#ifdef DOS
   gbContinuePolling = FALSE;
#else
   DestroyWindow( ghwndMain );
   ghwndMain = NULL;
   ghWFE = NULL;
#endif

   TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: StopPolling (exit)" ));
   return( rc );
}


/*******************************************************************************
 *
 *  Function: SendKeySequence
 *
 *  Purpose: Send a key sequence to the host
 *
 *  Entry:
 *     pszKeySequence (input)
 *        pointer to key sequence array
 *
 *  Exit:
 *     void
 *
 ******************************************************************************/
VOID SendKeySequence( LPBYTE pszKeySequence )
{
    FlushScanCode();
    wdScanCode( pszKeySequence, (USHORT)strlen(pszKeySequence) );
}

/*******************************************************************************
 *
 *  Function: wRegisterHotkey
 *
 *  Purpose: Register hot key(s)
 *
 *  Entry:
 *     pHotkeyInfo (input)
 *        pointer to hotkey definition(s)
 *
 *  Exit:
 *     void
 *
 ******************************************************************************/
INT wRegisterHotkey( PWFEHOTKEYINFO pHotkeyInfo )
{
    int i;

    for ( i = 0; i < pHotkeyInfo->cHotkeys; i++ ) {
        KbdRegisterHotkey( pHotkeyInfo->pHotkeys[i].hotkey,
                           pHotkeyInfo->pHotkeys[i].ScanCode,
                           pHotkeyInfo->pHotkeys[i].ShiftState );
    }

    return( CLIENT_STATUS_SUCCESS );
}
