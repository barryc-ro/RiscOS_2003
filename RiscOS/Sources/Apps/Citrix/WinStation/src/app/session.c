/* session.c

 *

 */

#include "windows.h"

#include <limits.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/client.h"
#include "../inc/clib.h"
#include "../inc/wdapi.h"
#include "../inc/pdapi.h"
#include "../inc/vdapi.h"
#include "../inc/vioapi.h"
#include "../inc/logapi.h"
#include "../inc/cfgini.h"
#include "../inc/cfgload.h"
#include "../inc/biniapi.h"
#include "../inc/wengapip.h"
#include "../inc/timapi.h"
#include "../inc/kbdapi.h"
#include "../inc/encrypt.h"
#include "../inc/dll.h"
#include "../inc/iniapi.h"

#include "citrix/ctxver.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#include "../inc/tddevicep.h"
#include "../inc/pddevicep.h"
#include "../inc/wdemulp.h"
#include "../inc/vddriverp.h"
#include "../inc/nrdevicep.h"
#include "../inc/debug.h"

#include "swis.h"
#include "tboxlibs/toolbox.h"

#include "session.h"
#include "sessionp.h"
#include "utils.h"
#include "version.h"

#include "resource.h"

/* --------------------------------------------------------------------------------------------- */

static char *gszWfclientIni = INI_PATH "WFClient";
static char *gszModuleIni = INI_PATH "Module";

typedef struct _OVERRIDELIST
{
    char *pszKey; 
    char *pszValue;
} OVERRIDELIST, * POVERRIDELIST;

/* --------------------------------------------------------------------------------------------- */

#define ENV_CLIENTNAME    APP_NAME "$ClientName"           // CLIENTNAME environment name
#define INET_HOSTNAME	  "Inet$HostName"

#define MAX_ERRORMSG      500                    // maximum error msg length

#define OVERRIDE_COUNT 1

static OVERRIDELIST g_pOverrides[OVERRIDE_COUNT] = {
#if 0
    { INI_USERNAME,       g_szUserName     },
                    { INI_DOMAIN,         g_szDomain       },
                    { INI_PASSWORD,       g_szPassword     },
                    { INI_INITIALPROGRAM, g_szCmdLine      },
                    { INI_WORKDIRECTORY,  g_szWorkDir      },
                    { INI_ENCRYPTIONDLL,  szEncryptionDLL },
                    { INI_CLIENTTYPE,     szClientType},
                    { INI_ENCRYPTIONLEVELSESSION, g_szEncryptionDLL},
                    { INI_DESIREDHRES,    g_szHRes         },
                    { INI_DESIREDVRES,    g_szVRes         },
                    { INI_SCREENPERCENT,  g_szScreenPercent},
#endif
                    { NULL,               NULL,            }
                  };

static Session		global_session = NULL;
       CLIENTNAME	gszClientName = { 0 };			// this must be global as cfgini.c uses it
static BOOL		bPDError=FALSE;

/* --------------------------------------------------------------------------------------------- */

extern LPBYTE gScriptFile;
extern LPBYTE gScriptDriver;
extern BOOL   sdLoad( LPBYTE pScriptFile, LPBYTE pScriptDriver );
extern BOOL   sdPoll( VOID);
extern VOID   sdUnload( VOID);

extern int gbContinuePolling;

extern FILEPATH       gszLoadDllFileName;        // name of last DLL loaded

/* --------------------------------------------------------------------------------------------- */

static void MessageBox(const char *message, const char *server)
{
    _kernel_oserror e;
    e.errnum = 0;
    strncpy(e.errmess, message, sizeof(e.errmess)-1);
    LOGERR(_swix(Wimp_ReportError, _INR(0,2), &e, 0, server));
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

static BOOL EMGetErrorMessage( Session sess, int iErrorCode, LPSTR chBuffer, int cbBuffSize )
{
    INT rc, nResourceId;
    WFELASTERROR LastError;
    TRANSPORTNAME pszTransport;
    CHAR szBuffer[MAX_ERRORMSG];
    CHAR strFormatString[MAX_ERRORMSG];
    int bErrorCodeInMessage = FALSE;

    /*
     *  Empty string for default.
     */
    *szBuffer = '\0';

    /*
     * Special case certain error codes.
     */
    switch( (LastError.Error = iErrorCode) ) {

    case CLIENT_ERROR_PD_ERROR:
	/*
	 *  Query engine for true error message text
	 */
	if ( (rc = srvWFEngQueryInformation( 
	    sess->hWFE,
	    WFELastError,
	    &LastError, 
	    sizeof(LastError) ))
	     == CLIENT_STATUS_SUCCESS ) {

	    lstrcpy( szBuffer, LastError.Message );
	    bErrorCodeInMessage = TRUE;
	}
	break;

    case CLIENT_ERROR_PD_TRANSPORT_UNAVAILABLE:
	LoadString("IDS", IDS_PDTRANSPORTUNAVAILABLE_FORMAT,
		   strFormatString,
		   sizeof(strFormatString));
	/*
	 * First find out what transport we're dealing with
	 */
	GetPrivateProfileString( sess->gszServerLabel,
				 INI_TRANSPORTDRIVER,
				 DEF_TRANSPORTDRIVER,
				 pszTransport, sizeof(pszTransport),
				 sess->gszICAFile );

	wsprintf( szBuffer, strFormatString, pszTransport );
	break;
    }

    /*
     *  If we did not get a specific error message; return the generic one
     *  associated with the error code.
     */
    if ( !(*szBuffer) ) {
#if 0
	char buf[16], *s;
	sprintf(buf, "msg%d:", iErrorCode);
	s = utils_msgs_lookup(buf);
	if (s && s[0])
	    strcpy(szBuffer, s);
	else
	    sprintf(szBuffer, "Error code %d", iErrorCode);
#endif
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

        if ( LoadString("IDS",
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
        if ( LoadString( "IDS",
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

    return (bErrorCodeInMessage);
}

static int EMErrorPopup(Session sess, int iError)
{
   char szErrorMsg[MAX_ERRORMSG];

   EMGetErrorMessage( sess, iError, szErrorMsg, sizeof(szErrorMsg));
   MessageBox(szErrorMsg, sess->gszServerLabel);

   LogPrintf( LOG_CLASS, LOG_ERRORS, szErrorMsg);

   return(0);
}

// restore desktop (should work with mode numbers and selectors)
static void restore_desktop(void)
{
    int mode;

    LOGERR(_swix(OS_RestoreCursors, 0));

    LOGERR(_swix(Wimp_ReadSysInfo, _IN(0)|_OUT(0), 1, &mode));
    LOGERR(_swix(Wimp_SetMode, _IN(0), mode));
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

static void WFEngineStatusCallback( Session sess, int message )
{
    int rc;
    LRESULT lResult = 0;

#ifdef DEBUG
    if (message != CLIENT_STATUS_SUCCESS && message != CLIENT_STATUS_NO_DATA)
	LogPrintf( LOG_CLASS, LOG_CONNECT, "callback: %d", message);
#endif
   
    switch ( message )
    {
    case CLIENT_STATUS_CONNECTION_BROKEN_HOST:
    case CLIENT_STATUS_CONNECTION_BROKEN_CLIENT:

	if ( (rc = srvWFEngUnloadDrivers( sess->hWFE )) != CLIENT_STATUS_SUCCESS ) {
    
            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGUNLOADDRIVERS);
	}
	if ( (rc = srvWFEngClose( sess->hWFE )) != CLIENT_STATUS_SUCCESS ) {

            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGCLOSE);
	}
	TRACE(( LOG_CLASS, LOG_CONNECT, "DISCONNECTED from %s", sess->gszServerLabel));
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
//          lResult = EMStatusMessage(message);
	break;


    case HOTKEY_UI: // CLIENT_STATUS_HOTKEY1:     // suspend
	TRACE(( LOG_CLASS, LOG_CONNECT, "HOTKEY_UI"));
	break;

    case HOTKEY_EXIT: // CLIENT_STATUS_HOTKEY2:     // exit
	TRACE(( LOG_CLASS, LOG_CONNECT, "HOTKEY_EXIT"));
	srvWFEngDisconnect(sess->hWFE);
	break;

    case CLIENT_STATUS_DELETE_CONNECT_DIALOG:
	connect_close(sess);
	break;

    case CLIENT_STATUS_CONNECTED:
	TRACE(( LOG_CLASS, LOG_CONNECT, "CONNECTED to %s", sess->gszServerLabel));
	sess->HaveFocus = FALSE;
	sess->Connected = TRUE;
	break;

    case CLIENT_STATUS_KILL_FOCUS:  // ignore these messages
	sess->HaveFocus = TRUE;

	restore_desktop();
	break;

    case CLIENT_STATUS_CONNECTING:
	break;

    case CLIENT_STATUS_TTY_CONNECTED:
	break;

    case CLIENT_STATUS_BEEPED:
	break;

    case CLIENT_STATUS_QUERY_ACCESS:
//          EMFileSecurityPopup();
	break;


    case CLIENT_ERROR_HOST_NOT_SECURED: 
    case CLIENT_ERROR_VD_ERROR:
    case CLIENT_ERROR_PD_ERROR:
	if(bPDError==FALSE)
	{
	    connect_close(sess);
	    EMErrorPopup(sess, message);
	    bPDError=TRUE;
	}
	break;

    default:
#if 0
    {
	char szStatus[80];
	sprintf(szStatus,"Unknown status message (%d)",message);
	MessageBox(szStatus, sess->gszServerLabel);
    }
#endif
	break;
    }
}  // end WFEngineStatusCallback

/* --------------------------------------------------------------------------------------------- */

/*
 *  EMEngOpen
 *
 *  Start the engine window
 *
 */
static int EMEngOpen(Session sess)
{
    WFEOPEN  WFEOpen;
    INT rc = CLIENT_STATUS_SUCCESS;
    ULONG iHRes,iVRes;

    // determine the desired size of client window
    iHRes = GetPrivateProfileInt(sess->gszServerLabel, INI_DESIREDHRES,
				 0, sess->gszICAFile);
    if(iHRes == 0)
	iHRes = GetPrivateProfileInt(INI_VDTW30, INI_DESIREDHRES,
				     DEF_DESIREDHRES, gszWfclientIni);
         
    iVRes = GetPrivateProfileInt(sess->gszServerLabel, INI_DESIREDVRES,
				 0, sess->gszICAFile);
    if(iVRes == 0)
	iVRes = GetPrivateProfileInt(INI_VDTW30, INI_DESIREDVRES,
				     DEF_DESIREDVRES, gszWfclientIni);

    WFEOpen.Version          = WFENG_API_VERSION;
    WFEOpen.pfnStatusMsgProc = NULL; // &WFEngineStatusCallback;
    WFEOpen.pszClientname    = gszClientName;   
    WFEOpen.uInitialWidth    = (int)iHRes;   
    WFEOpen.uInitialHeight   = (int)iVRes;  
    WFEOpen.pszLogLabel      = "IWFC";     
    WFEOpen.pszTitle         = sess->gszServerLabel;
    WFEOpen.pszClientSN      = "";                 
    WFEOpen.pszIconPath      = APP_DIR;
    WFEOpen.uIconIndex       = 0;      
    WFEOpen.reserved         = 0; //(ULONG)&WFEOpenp;
    // WFEOpenp.hWndPlugin      = 

    rc = srvWFEngOpen(&WFEOpen, &sess->hWFE);

    return(rc);
}                          

/*
 *  EMEngLoadSession
 *
 *  Start the session
 *
 */
static int EMEngLoadSession(Session sess)
{
    INT rc = CLIENT_STATUS_SUCCESS;
    CFGINIOVERRIDE pOverrides[OVERRIDE_COUNT];
    int            i, j;

    /*
     * Set up our command line override structure key and value pointers.
     */
    
    //g_pOverrides[6].pszValue = szEncryptionDLL;
    //g_pOverrides[7].pszValue = INTERNET_CLIENT;
    strcpy(sess->szClientType, INTERNET_CLIENT);

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
    
    rc = CfgIniLoad( sess->hWFE,
		     sess->gszServerLabel,
		     pOverrides,
		     sess->gszICAFile,
		     gszModuleIni,
		     NULL,                     // MODEM file
		     gszWfclientIni);

    return(rc);
}

/* --------------------------------------------------------------------------------------------- */

static void session_free(Session sess)
{
    TRACE((TC_UI, TT_API1, "session_free: %p", sess));

    FlushPrivateProfileCache();

    if (global_session == sess)
	global_session = NULL;

    if (sess->tempICAFile)
	remove(sess->gszICAFile);
   
    free(sess->gszICAFile);
    free(sess);
}

static int session_abort(Session sess, int rc)
{
    TRACE((TC_UI, TT_API1, "session_abort: %p state %d", sess, rc));

    connect_close(sess);
    EMErrorPopup(sess, rc);

    session_free(sess);

    return rc;
}

static int session__open(Session sess)
{
    int rc;
    char *pEnvVar;

    TRACE((TC_UI, TT_API1, "session__open: %p", sess));

    // determine the client name from environment variable first
    // then use Win32 computername and then the default from the
    // resource file
    pEnvVar = getenv(ENV_CLIENTNAME);
    if (pEnvVar == NULL)
	pEnvVar = getenv(INET_HOSTNAME);

    // use time stamp to creat an unique client name
    if (pEnvVar == NULL)
	sprintf(gszClientName, "Internet%08u", clock() & 0xffffff);
    else
	lstrcpy(gszClientName, pEnvVar);

    // start the connect status dialog 
    connect_open(sess);
    connect_status(sess, CLIENT_STATUS_LOADING_STACK);

    rc = srvWFEngLoad( NULL, NULL );  // Call the WFEngLoad API
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // open the engine
    rc = EMEngOpen(sess);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // load the session
    rc = EMEngLoadSession(sess);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // set the product id to Internet client
    {
       WFEPRODUCTID WFEProductID;
       WFEProductID.ProductID = CLIENTID_CITRIX_RISCOS;
       wdSetProductID( &WFEProductID, sizeof(WFEProductID));
    }

    // update the status to connecting
    connect_status(sess, CLIENT_STATUS_CONNECTING);

    rc = srvWFEngConnect(sess->hWFE);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    /*
     *  Load script driver
     */
    if ( gScriptFile ) {
        sess->fSdPoll = sess->fSdLoaded = sdLoad( gScriptFile, gScriptDriver );
        free( gScriptDriver );
        free( gScriptFile );
        gScriptFile = NULL;
	gScriptDriver = NULL;
    }

    connect_close(sess);
    
    // initialise this here as it is statically inited in wengine
    gbContinuePolling = TRUE;

    return rc;
}

static void session__close(Session sess)
{
    TRACE((TC_UI, TT_API1, "session_close: %p", sess));

    /*
     *  Unload script driver
     */
    if ( sess->fSdLoaded ) {
        sdUnload();
	sess->fSdLoaded = FALSE;
    }

    srvWFEngDisconnect(sess->hWFE);
    
    /*
     * These APIs are called automatically on exit
     * (The APIs called by the UI are handled in WFCWINx.DLL)
     */
    TRACE((TC_UI, TT_API1, "session_close: unload drivers"));
    srvWFEngUnloadDrivers( sess->hWFE );

    TRACE((TC_UI, TT_API1, "session_close: close"));
    srvWFEngClose( sess->hWFE );

    TRACE((TC_UI, TT_API1, "session_close: unload engine"));
    srvWFEngUnload( (PMINIDLL)NULL );  // Call the WFEngUnload API

    connect_close(sess);
}

Session session_open_url(const char *url)
{
    char *host = NULL;
    char *path = NULL;
    char *args = NULL;
    const char *s;
    int rc;

    TRACE((TC_UI, TT_API1, "session_open_url: '%s'", url));

    s = url + 4;
    if (s[0] == '/' && s[1] == '/')
    {
	const char *start = s+2;
	const char *end = strchr(start, '/');
	host = strndup(start, end ? end - start : INT_MAX);
	s = end;
    }

    if (s && s[0] == '/' && s[1] > ' ')
    {
	const char *start = s+1;
	const char *end = strchr(start, '?');
	path = strndup(start, end ? end - start : INT_MAX);
	s = end;
    }
    
    if (s && s[0] == '?' && s[1] > ' ')
	args = strdup(s+1);

    TRACE((TC_UI, TT_API1, "session_open_url: host '%s' path '%s' args '%s'",
	   strsafe(host), strsafe(path), strsafe(args)));

    return session_open_server(host);
}

Session session_open_server(const char *host)
{
    Session sess = NULL;
    int rc;
    if (host)
    {
	char name[L_tmpnam];
	FILE *f;

	global_session = sess = calloc(sizeof(struct session_), 1);
	sess->HaveFocus = TRUE;

	strncpy(sess->gszServerLabel, host, sizeof(sess->gszServerLabel));

	if ((f = fopen(tmpnam(name), "w")) != NULL)
	{
	    fprintf(f, "[%s]\n", host);
	    fprintf(f, "Address=%s\n", host);
	    fprintf(f, "[ApplicationServers]\n");
	    fprintf(f, "%s=\n", host);
	    fclose(f);

	    sess->gszICAFile = strdup(name);
	    sess->tempICAFile = TRUE;
	}
	
	if ((rc = session__open(sess)) != CLIENT_STATUS_SUCCESS)
	{
	    session_abort(sess, rc);
	    global_session = sess = NULL;
	}
    }
    
    return sess;
}

Session session_open(const char *ica_file)
{
    Session sess;
    int rc;

    TRACE((TC_UI, TT_API1, "session_open: '%s'", ica_file));

    global_session = sess = calloc(sizeof(struct session_), 1);

    sess->gszICAFile = strdup(ica_file);
    sess->HaveFocus = TRUE;
    
    // get the first server listing in the ICA file
    sess->gszServerLabel[0] = '\0';
    rc = GetPrivateProfileString( INI_APPSERVERLIST,
                            NULL, 
                            "",
                            sess->gszServerLabel,
                            sizeof(sess->gszServerLabel),
                            sess->gszICAFile );

    // strip '=' and any spaces before it from the name
    {
	char *s = strchr(sess->gszServerLabel, '=');
	if (s)
	{
	    *s-- = 0;
	    while (s > sess->gszServerLabel && isspace(*s))
		*s-- = 0;
	}
    }
    
    TRACE((TC_UI, TT_API1, "got server: '%s'", sess->gszServerLabel));

    GetPrivateProfileString(sess->gszServerLabel, INI_ENCRYPTIONLEVELSESSION, 
			    "",
			    sess->szEncryptionDLL, sizeof(sess->szEncryptionDLL),
			    sess->gszICAFile);

    
    if (sess->gszServerLabel[0] == '\0')
    {
	char msg[128];

	LoadString("IDS", IDS_CLIENT_ERROR_SERVER_SECTION_NOT_FOUND,
		   msg, sizeof(msg));
	msg_report(msg);

	session_free(sess);

	return NULL;
    }

    if ((rc = session__open(sess)) != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	global_session = sess = NULL;
    }

    return sess;
}

int session_poll(Session sess)
{
    int rc;
    
    DTRACE((TC_UI, TT_API1, "session_poll: state %x connected %d focus %d continuepolling %d", gState, gState & WFES_CONNECTED, sess->HaveFocus, gbContinuePolling ));

//  if ( gState & WFES_CONNECTED )
//	srvWFEngMessageLoop( sess->hWFE );

    do
    {
	rc = srvWFEngPoll( sess->hWFE );

	/*
	 *  Poll SD
	 */
	if ( sess->fSdPoll ) {
	    sess->fSdPoll = sdPoll();
	}

	WFEngineStatusCallback(sess, rc);

#ifdef DEBUG
	LogPoll();
#endif
    }
    while (!sess->HaveFocus && gbContinuePolling);
    
    return gbContinuePolling;
}

void session_close(Session sess)
{
    session__close(sess);

    if (!sess->HaveFocus)
	restore_desktop();

    session_free(sess);
}

/* --------------------------------------------------------------------------------------------- */

void session_resume(Session sess)
{
    TRACE((TC_UI, TT_API1, "session_resume: %p", sess));

    // Give focus back to the engine
    if (!srvWFEngSetInformation(sess->hWFE, WFESetFocus, NULL, 0))
	sess->HaveFocus = FALSE;
}

void session_run(const char *file, int file_is_url)
{
    Session sess;

    TRACE((TC_UI, TT_API1, "session_run: %s url %d", file, file_is_url));

    if (file_is_url)
	sess = session_open_url(file);
    else
	sess = session_open(file);

    TRACE((TC_UI, TT_API1, "session_run: entering poll"));

    while (sess->HaveFocus && !sess->Connected)
	session_poll(sess);

    TRACE((TC_UI, TT_API1, "session_run: entering close"));

    session__close(sess);

    if (!sess->HaveFocus)
	restore_desktop();

    TRACE((TC_UI, TT_API1, "session_run: going for cleanup Focus %d", sess->HaveFocus));

    // allow cleanup
    while (session_poll(sess))
	;

    session_free(sess);
}

/* --------------------------------------------------------------------------------------------- */

extern int SdLoad( PDLLLINK );

int ModuleLookup( PCHAR pName, PLIBPROCEDURE *pfnLoad, PPLIBPROCEDURE *pfnTable )
{
    static BOOL inited = FALSE;
    static struct
    {
	PCHAR pName;
	PLIBPROCEDURE fnLoad;
	PPLIBPROCEDURE fnTable;
    } modules[] =
    {
	{ "tdtcpro",	(PLIBPROCEDURE)TdLoad/*, TdTcpRODeviceProcedures */ },
	{ "pdcrypt",	(PLIBPROCEDURE)PdLoad/*, PdCryptDeviceProcedures */ },
	{ "pdrfram",	(PLIBPROCEDURE)PdLoad/*, PdRFrameDeviceProcedures */ },
	{ "pdmodem",	(PLIBPROCEDURE)PdLoad/*, PdModemDeviceProcedures */ },
	{ "wdica30",	(PLIBPROCEDURE)WdLoad/*, WdICA30EmulProcedures */ },
	{ "wdtty",	(PLIBPROCEDURE)WdLoad/*, WdTTYEmulProcedures */ },
	{ "nrtcpro",	(PLIBPROCEDURE)NrLoad, NULL },
#ifdef INCL_ENUM
	{ "neica",	(PLIBPROCEDURE)NeLoad/*, NeICADeviceProcedures */ },
#else
	{ "neica",	0 },
#endif
	{ "vdtw30",	(PLIBPROCEDURE)VdLoad/*, VdTW31DriverProcedures */ },
	{ "vdcpm30",	(PLIBPROCEDURE)VdLoad/*, VdCpmDriverProcedures */ },
	{ "vdspl",	(PLIBPROCEDURE)VdLoad/*, VdSplDriverProcedures */ },
#ifdef INCL_SCRIPT
	{ "script",	(PLIBPROCEDURE)SdLoad, NULL },
#endif
	{ NULL,		0,	NULL }
    }, *mp;

    TRACE((TC_UI, TT_API1, "ModuleLookup: '%s'", pName));

    if (!inited)
    {
	modules[0].fnTable = TdTcpRODeviceProcedures;
	modules[1].fnTable = PdCryptDeviceProcedures;
	modules[2].fnTable = PdRFrameDeviceProcedures;
#ifdef INCL_MODEM
	modules[3].fnTable = PdModemDeviceProcedures;
#endif
	modules[4].fnTable = WdICA30EmulProcedures;
	modules[5].fnTable = WdTTYEmulProcedures;
#ifdef INCL_ENUM
	modules[7].fnTable = NeICADeviceProcedures;
#endif
	modules[8].fnTable = VdTW31DriverProcedures;
#ifdef INCL_PRINTER
	modules[9].fnTable = VdCpmDriverProcedures;
	modules[10].fnTable = VdSplDriverProcedures;
#endif
	inited = TRUE;
    }
    
    for (mp = modules; mp->pName; mp++)
    {
	if (mp->fnLoad && strcmpi(mp->pName, pName) == 0)
	{
	    if (pfnLoad)
		*pfnLoad = mp->fnLoad;
	    if (pfnTable)
		*pfnTable = mp->fnTable;

	    return CLIENT_STATUS_SUCCESS;
	}
    }

    return CLIENT_ERROR_DLL_NOT_FOUND;
}

/* --------------------------------------------------------------------------------------------- */
