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
*   wengine.c,v
*   Revision 1.1  1998/01/12 11:36:40  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.114   12 Aug 1997 22:34:06   tariqm
*  win16 fix
*  
*     Rev 1.113   08 Aug 1997 21:18:22   tariqm
*  scripting support
*  
*     Rev 1.112   21 Jul 1997 19:32:08   tariqm
*  Update...
*  
*     Rev 1.111   10 Jul 1997 22:54:00   tariqm
*  Connection Status..
*  
*     Rev 1.110   11 Jun 1997 10:29:46   terryt
*  client double click support
*  
*     Rev 1.109   27 May 1997 17:41:30   terryt
*  client double click support
*  
*     Rev 1.108   27 May 1997 14:26:24   terryt
*  client double click support
*  
*     Rev 1.107   15 Apr 1997 18:18:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.108   09 Apr 1997 16:06:38   BillG
*  update
*  
*  
*     Rev 1.107   21 Mar 1997 16:10:04   bradp
*  update
*  
*     Rev 1.106   10 Mar 1997 19:13:34   BillG
*  filter client name
*  
*     Rev 1.105   06 Mar 1997 10:49:18   kurtp
*  fix CPR 4269
*  
*     Rev 1.104   20 Feb 1997 14:10:26   butchd
*  Added client data query support
*  
*     Rev 1.103   26 Jun 1996 15:07:40   brucef
*  Add DOS-specific INI Keyword processing and calls to new Mouse
*  and Keyboard APIs to manage user preferences.
*  
*     Rev 1.102   11 Jun 1996 14:55:00   jeffm
*  update
*  
*     Rev 1.101   28 May 1996 19:57:44   jeffm
*  update
*  
*     Rev 1.100   20 May 1996 15:49:34   jeffm
*  update
*  
*     Rev 1.97   27 Apr 1996 15:50:38   andys
*  soft keyboard
*
*     Rev 1.96   14 Feb 1996 10:40:02   butchd
*  must still force on top and foreground for Win32
*  
*     Rev 1.95   12 Feb 1996 17:44:50   butchd
*  fixed initial show and focus bugs
*  
*     Rev 1.94   12 Feb 1996 09:40:06   richa
*  Create and setup the G_fAsync flag so that we can mark async connections.
*
*     Rev 1.93   31 Jan 1996 17:22:26   kurtp
*  update
*  
*     Rev 1.92   30 Jan 1996 16:26:32   bradp
*  update
*  
*     Rev 1.91   30 Jan 1996 09:52:02   bradp
*  update
*  
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
#include "../../inc/biniapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/timapi.h"
#include "../../inc/kbdapi.h"
// #include "../../inc/xmsapi.h"
#include "../../inc/encrypt.h"
// #include "../../inc/lptapi.h"

#include "../../inc/wfcver.h"
#include "../../inc/mouapi.h"

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#define DOS

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
extern VDTWSTACK vTWStack;
#endif
extern HWND           g_hWndPlugin;

//HINSTANCE ghInstance = NULL; 

#ifndef DOS
extern BOOL gfIPCShutdown;
extern BOOL gbIPCEngine;
#endif


/*=============================================================================
==   Local Vars
=============================================================================*/

BOOL gbContinuePolling           = FALSE;
#if 0
PPLIBPROCEDURE pModuleProcedures = NULL;
PPLIBPROCEDURE pLogProcedures    = NULL;
PPLIBPROCEDURE pVioProcedures    = NULL;
PPLIBPROCEDURE pBIniProcedures   = NULL;
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
static HWND      ghwndMain = NULL;
static DLLLINK   gExeLink = { 0 };
static char      gpszExePath[MAXPATH] = { 0 };
static char      gpszDllPath[MAXPATH] = { 0 };
#if !defined(DOS) && !defined(RISCOS)
HICON            ghCustomIcon = NULL;
BOOL             gfRemoveTitleBar = FALSE;
#endif
HANDLE           ghWFE = NULL;
DLLLINK          gWdLink = { 0 };
DLLLINK          gPdLink = { 0 };
PDLLLINK *       gpaVdLink = NULL;
USHORT           gMaxVirtualChannels = 0;
PDOPEN           gPdOpen = { 0 };
BOOL             gbVdAddrToWd = FALSE;
USHORT           gState = 0;
static BOOL      gbPollingEnabled = FALSE;
CLIENTNAME       gpszClientname = { 0 };
CLIENTSN         gpszClientSN = { 0 };
DESCRIPTION      gpszTitle = { 0 };
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
//    BIniUnload();

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
    PCHAR        pIni = pUiOpen->pIniSection;
    USHORT       Offset;
    LPBYTE       pBuf;
    USHORT       Length;
    USHORT       Len;
    ENCRYPTEDPASSWORD szEncryptedPassword;
    PASSWORD szPassword;
    KEYBOARDLAYOUT szKeyboardLayout;
    
    /* initialise some static variables here */
    G_Encrypted = FALSE;
    
    /* check for script file and script driver */
    if ( ((gScriptFile   = malloc( FILEPATH_LENGTH+1 )) != NULL) &&
         ((gScriptDriver = malloc( FILEPATH_LENGTH+1 )) != NULL) ) {
        
        /* read profile */
        bGetPrivateProfileString( pIni, INI_SCRIPT_DRIVER, INI_EMPTY, 
                                  gScriptDriver, FILEPATH_LENGTH );

        bGetPrivateProfileString( pIni, INI_SCRIPT_FILE, INI_EMPTY, 
                                  gScriptFile,   FILEPATH_LENGTH );

        /* valid script file and path */
        if ( !strlen( gScriptFile ) || !strlen( gScriptDriver ) ) {
            free( gScriptDriver );
            free( gScriptFile );
            gScriptFile = NULL;
        }
    }

    /* get worse case byte count */
    G_pUiDataLength = sizeof(UI_C2H) + strlen(gpszExePath) +
                DOMAIN_LENGTH + USERNAME_LENGTH + PASSWORD_LENGTH +
                DIRECTORY_LENGTH + INITIALPROGRAM_LENGTH +
                CLIENTNAME_LENGTH + CLIENTSN_LENGTH + 8;

    if ( G_pUiData )
        free( G_pUiData );

    if ( (G_pUiData = malloc(G_pUiDataLength)) == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    memset( G_pUiData, 0, G_pUiDataLength );
    Offset = sizeof(UI_C2H);

    /*
     * Until we connect, we don't know the encryption level
     */
    G_pUiData->EncryptionLevel = 0;

    G_pUiData->fDisableSound = bGetPrivateProfileBool( pIni, INI_SOUND,
                                                       DEF_SOUND );

    G_pUiData->fDisableCtrlAltDel = bGetPrivateProfileBool( pIni, INI_CTRLALTDEL,
                                                            DEF_CTRLALTDEL );

    G_MouseTimer = bGetPrivateProfileInt( pIni, INI_MOUSETIMER, DEF_MOUSETIMER );

#ifdef DOS
    G_MouseDoubleClickTimer = bGetPrivateProfileInt( pIni, INI_MOUSEDOUBLECLICKTIMER, DEF_MOUSEDOUBLECLICKTIMER );
    G_MouseDoubleClickHeight = bGetPrivateProfileInt( pIni, INI_MOUSEDOUBLECLICKHEIGHT, DEF_MOUSEDOUBLECLICKHEIGHT );
    G_MouseDoubleClickWidth = bGetPrivateProfileInt( pIni, INI_MOUSEDOUBLECLICKWIDTH, DEF_MOUSEDOUBLECLICKWIDTH );
#endif

    G_KeyboardTimer = bGetPrivateProfileInt( pIni, INI_KEYBOARDTIMER, DEF_KEYBOARDTIMER );

    G_fEchoTTY = bGetPrivateProfileBool( pIni, INI_ECHO_TTY, DEF_ECHO_TTY );


    bGetPrivateProfileString( pIni, INI_KEYBOARDLAYOUT, DEF_KEYBOARDLAYOUT,
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

    G_pUiData->KeyboardLayout = bGetPrivateProfileLong( pIni,
							szKeyboardLayout,
							(long)0 );

    TRACE((TC_WENG, TT_API1, "KeyboardLayout: %d\n", G_pUiData->KeyboardLayout));

#if 0
    G_pUiData->Lpt1  = bGetPrivateProfileInt( pIni, INI_LPT1, DEF_LPT1 );
    G_pUiData->Port1 = bGetPrivateProfileInt( pIni, INI_PORT1, DEF_PORT1 );

    G_pUiData->Lpt2  = bGetPrivateProfileInt( pIni, INI_LPT2, DEF_LPT2 );
    G_pUiData->Port2 = bGetPrivateProfileInt( pIni, INI_PORT2, DEF_PORT2 );

    G_pUiData->Lpt3  = bGetPrivateProfileInt( pIni, INI_LPT3, DEF_LPT3 );
    G_pUiData->Port3 = bGetPrivateProfileInt( pIni, INI_PORT3, DEF_PORT3 );
#endif

    /*
     *  Copy client build number
     */
    G_pUiData->ClientBuildNumber = VER_CLIENTBUILD;

    /*
     *  Copy domain
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    bGetPrivateProfileString( pIni, INI_DOMAIN, DEF_DOMAIN,
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
    bGetPrivateProfileString( pIni, INI_USERNAME, DEF_USERNAME,
                              pBuf, USERNAME_LENGTH+1 );

    bGetPrivateProfileString( pIni, INI_ENCRYPTIONLEVELSESSION, DEF_ENCRYPTIONLEVEL,
                                 g_szEncryptionLevelConnStatus, sizeof(g_szEncryptionLevelConnStatus) );


    if ( (Length = strlen(pBuf)) > 0 ) {
        G_pUiData->oUserName = Offset;
        Offset += (Length + 1);  // skip over trailing null
        TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: Username: %s", pBuf ));
    }

    /*
     *  Copy password
     */
    pBuf = (LPBYTE)G_pUiData + Offset;
    bGetPrivateProfileString( pIni, INI_PASSWORD, DEF_PASSWORD,
                              szEncryptedPassword,
                              sizeof(szEncryptedPassword) );

    if ( *szEncryptedPassword ) {
        Len = DecryptFromAscii( szEncryptedPassword,
                                (USHORT)strlen( szEncryptedPassword ),
                                szPassword,
                                sizeof(szPassword) );
    } else {
        Len = 0;
    }
    szPassword[Len] = 0;


    if ( (Length = strlen(szPassword)) > 0 ) {

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
    bGetPrivateProfileString( pIni, INI_WORKDIRECTORY, DEF_WORKDIRECTORY,
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
    bGetPrivateProfileString( pIni, INI_INITIALPROGRAM, DEF_INITIALPROGRAM,
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
     *  Realloc data buffer to exact size
     */
    ASSERT( Offset <= G_pUiDataLength, Offset );
    G_pUiDataLength = Offset;
    G_pUiData = realloc( G_pUiData, G_pUiDataLength );

    ASSERT( G_pUiData, 0 );
    TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: UiOpen size(%d)", Offset ));
    return( CLIENT_STATUS_SUCCESS );
}


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
    if ( bGetPrivateProfileString( pWFELoad->pIniSection,   //
                                   INI_DEVICE,              // "DeviceName"
                                   INI_EMPTY,               //
                                   pszBuffer,               //
                                   sizeof(pszBuffer) ) ) {  //
        G_fAsync = TRUE;
    }

#ifndef DOS
    if(gbIPCEngine) 
#endif
    for ( i = 0; gHotkeys[i].pszShiftState; i++ ) {
       /*
        * Get hotkey i shift state
        */
       sprintf( pszINIKey, INI_HOTKEY_SHIFT, i+1 );
       bGetPrivateProfileString( pWFELoad->pIniSection, pszINIKey,
                                 gHotkeys[i].pszShiftState,
                                 pszBuffer, sizeof(pszBuffer) );
       ShiftState = bGetPrivateProfileInt( pWFELoad->pIniSection, pszBuffer,
                                           gHotkeys[i].ShiftState );

       /*
        * Get hotkey i key
        */
       sprintf( pszINIKey, INI_HOTKEY_CHAR, i+1 );
       bGetPrivateProfileString( pWFELoad->pIniSection, pszINIKey,
                                 gHotkeys[i].pszScanCode,
                                 pszBuffer, sizeof(pszBuffer) );
       ScanCode   = (UCHAR)bGetPrivateProfileInt( pWFELoad->pIniSection, pszBuffer,
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

    KPref.KeyboardDelay = bGetPrivateProfileInt( pWFELoad->pIniSection,
                                                 INI_KEYBOARDDELAY,
                                                 DEF_KEYBOARDDELAY );
    KPref.KeyboardSpeed = bGetPrivateProfileInt( pWFELoad->pIniSection,
                                                 INI_KEYBOARDSPEED,
                                                 DEF_KEYBOARDSPEED );

    KbdLoadPreferences( &KPref );

    MPref.HorizSpeed = bGetPrivateProfileInt( pWFELoad->pIniSection,
                                              INI_HORIZONTALSPEED,
                                              DEF_HORIZONTALSPEED ); 
    MPref.VertSpeed = bGetPrivateProfileInt( pWFELoad->pIniSection,
                                             INI_VERTICALSPEED,
                                             DEF_VERTICALSPEED );
    MPref.DblSpeedThreshold = bGetPrivateProfileInt( pWFELoad->pIniSection,
                                                     INI_DBLSPEEDTHRESHOLD,
                                                     DEF_DBLSPEEDTHRESHOLD );

    bGetPrivateProfileString( pWFELoad->pIniSection, 
                                       INI_SWAPBUTTONS, DEF_SWAPBUTTONS,
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

    if ( (rc = LoadWd( (PCHAR)(pWFELoad->pIniSection),
                       (PCHAR)(pWFELoad->pszModuleName),
                       (PCHAR)(gpszDllPath),
                       &gWdLink, &gPdLink,
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
       UnloadDrivers( &gWdLink, &gPdLink, gpaVdLink, gMaxVirtualChannels );
       gState &= ~(WFES_LOADEDWD | WFES_LOADEDPD | WFES_LOADEDVD | WFES_LOADEDSESSION);
       memset( &gWdLink, 0, sizeof(gWdLink) );
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
    /*
     * Tell the Wd to pass the thinwire stack to the Vd
     */
    wdThinwireStack( &vTWStack );
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
            if(gbIPCEngine) 
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
           }
           break;

       case CLIENT_STATUS_CONNECTED :
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: CLIENT_STATUS_CONNECTED" ));
           if ( !G_fEchoTTY ) {
               NotifyUI( hWFE, CLIENT_STATUS_DELETE_CONNECT_DIALOG, 0 );
               fProcCalled = FALSE;
               wdSetFocus();
           }
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
           SendKeySequence( "\x38\x01\x81\xB8" );
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

       case HOTKEY_UI: // UI hotkey
           TRACE(( TC_WENG, TT_L1,
                   "WFEngx.Exe: StatusMsgProc: UI Hotkey" ));
           wdKillFocus();
           fProcCalled = TRUE; // Don't notify UI yet
           messageHotkey = message;
           break;

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
