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
#include "../inc/miapi.h"
#include "../inc/wengapip.h"
#include "../inc/timapi.h"
#include "../inc/kbdapi.h"
#include "../inc/encrypt.h"
#include "../inc/dll.h"
#include "../inc/iniapi.h"

//#include "citrix/ctxver.h"
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
#include "vdu.h"

#include "session.h"
#include "sessionp.h"
#include "utils.h"
#include "version.h"

#include "resource.h"
#include "clientname.h"

/* --------------------------------------------------------------------------------------------- */

static char *gszWfclientIni = INI_PATH "WFClient";
static char *gszModuleIni = INI_PATH "Module";

typedef struct _OVERRIDELIST
{
    char *pszKey; 
    char *pszValue;
} OVERRIDELIST, * POVERRIDELIST;

/* --------------------------------------------------------------------------------------------- */

#define MAX_ERRORMSG    500                    // maximum error msg length

#define OVERRIDE_COUNT	1

static icaclient_session	global_session = NULL;
       CLIENTNAME gszClientName = { 0 };			// this must be global as cfgini.c uses it
static BOOL bPDError=FALSE;

/* --------------------------------------------------------------------------------------------- */

extern LPBYTE gScriptFile;
extern LPBYTE gScriptDriver;
extern BOOL sdLoad( LPBYTE pScriptFile, LPBYTE pScriptDriver );
extern BOOL sdPoll( VOID);
extern VOID sdUnload( VOID);

extern int gbContinuePolling;

extern FILEPATH gszLoadDllFileName;        // name of last DLL loaded

extern int cli_modem;	// from main.c

/* --------------------------------------------------------------------------------------------- */

static void session__close(icaclient_session sess);

/* --------------------------------------------------------------------------------------------- */

static void MessageBox(const char *message, const char *server)
{
    _kernel_oserror e;
    e.errnum = 0;
    strncpy(e.errmess, message, sizeof(e.errmess)-1);
    LOGERR(_swix(Wimp_ReportError, _INR(0,2), &e, 0, server));
}

static int client_to_ids(int iErrorCode)
{
    int nResourceId;

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

    return nResourceId;
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

static BOOL EMGetErrorMessage( icaclient_session sess, int iErrorCode, LPSTR chBuffer, int cbBuffSize )
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
    if ( !(*szBuffer) )
    {
	nResourceId = client_to_ids(iErrorCode);

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

static int EMErrorPopup(icaclient_session sess, int iError)
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

    FadeScreen(0);

    LOGERR(_swix(OS_RestoreCursors, 0));

    reset_char_defs();
    
    LOGERR(_swix(Wimp_ReadSysInfo, _IN(0)|_OUT(0), 1, &mode));
    LOGERR(_swix(Wimp_SetMode, _IN(0), mode));
}

static int question(icaclient_session sess, const char *message)
{
    _kernel_oserror e;
    int rval;

    e.errnum = 0;
    strncpy(e.errmess, message, sizeof(e.errmess)-1);

    LOGERR(_swix(Wimp_ReportError, _INR(0,2) | _OUT(1),
		 &e,
		 3,		// OK and cancel
		 APP_NAME,
		 &rval));
/*
    LOGERR(_swix(Wimp_ReportError, _INR(0,5) | _OUT(1),
		 &e,
		 0x100 | (4<<9),
		 sess->gszServerLabel,
		 NULL,		// spritename
		 NULL,		// spritearea
		 "OK,Cancel",
		 &rval));
		 */

    return rval == 3 || rval == 1;
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

static void WFEngineStatusCallback( icaclient_session sess, int message )
{
    int rc;
    static BOOL ignore_next_kill = FALSE;

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
	
	if (question(sess, utils_msgs_lookup("disconn")))
	    srvWFEngDisconnect(sess->hWFE);
	else
	{
	    ignore_next_kill = TRUE;
	    session_resume(sess);
	}
	break;

    case CLIENT_STATUS_DELETE_CONNECT_DIALOG:
	break;

    case CLIENT_STATUS_CONNECTED:
	TRACE(( LOG_CLASS, LOG_CONNECT, "CONNECTED to %s", sess->gszServerLabel));
	sess->HaveFocus = FALSE;
	sess->Connected = TRUE;

	connect_close(sess);
	break;

    case CLIENT_STATUS_KILL_FOCUS:  // ignore these messages
	TRACE((TC_UI, TT_API1, "client_status_kill_focus: %p", sess));
	if (ignore_next_kill)
	{
	    ignore_next_kill = FALSE;
	}
	else
	{
	    sess->HaveFocus = TRUE;
	    restore_desktop();
	}
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
static int EMEngOpen(icaclient_session sess)
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
 *  EMEngLoadwinframe_session
 *
 *  Start the session
 *
 */
static int EMEngLoadwinframe_session(icaclient_session sess)
{
    INT rc = CLIENT_STATUS_SUCCESS;
    CFGINIOVERRIDE *pOverrides;
    int i;
    arg_element *a;
    int count;
    BOOL had_address;
    char *password = NULL;

    /*
     * Set up our command line override structure key and value pointers.
     */
    
    count = 0;
    for (a = sess->arg_list; a; a = a->next)
	count++;

    pOverrides = calloc(count + 1, sizeof(*pOverrides));
    
    had_address = FALSE;
    for (a = sess->arg_list, i = 0; a; a = a->next, i++)
    {
	char *name = a->name;
	char *value = a->value;
	
	if (strcmpi(name, "PASSCLEAR") == 0)
	{
	    int len = strlen(value);
	    char *work;

	    work = strdup(value);
	    password = malloc(len + 16);
		
	    EncryptToAscii(work, len, password, len + 16);

	    name = INI_PASSWORD;
	    value = password;

	    free(work);
	}

	pOverrides[i].pszKey = name;
	pOverrides[i].pszValue = value;
    }
    
    pOverrides[i].pszKey = NULL;
    pOverrides[i].pszValue = NULL;
    
    strcpy(sess->szClientType, INTERNET_CLIENT);
    
    rc = CfgIniLoad( sess->hWFE,
		     sess->gszServerLabel,
		     pOverrides,
		     sess->gszICAFile,
		     gszModuleIni,
		     NULL,                     // MODEM file
		     gszWfclientIni);

    free(pOverrides);
    free(password);
    
    return(rc);
}

/* --------------------------------------------------------------------------------------------- */

static void session_free(icaclient_session sess)
{
    TRACE((TC_UI, TT_API1, "session_free: %p", sess));

    FlushPrivateProfileCache();

    if (global_session == sess)
    {
	global_session = NULL;

	if (cli_modem)
	    timeout_enable();
    }

    if (sess->tempICAFile)
	remove(sess->gszICAFile);

    free_elements(&sess->arg_list);
    
    free(sess->gszICAFile);
    free(sess);
}

static icaclient_session session_new(void)
{
    icaclient_session sess = calloc(sizeof(struct icaclient_session_), 1);

    sess->HaveFocus = TRUE;
    
    global_session = sess;

    if (cli_modem)
	timeout_disable();

    return sess;
}

static int session_abort(icaclient_session sess, int rc)
{
    TRACE((TC_UI, TT_API1, "session_abort: %p state %d", sess, rc));

    EMErrorPopup(sess, rc);

    session__close(sess);
//  connect_close(sess);
    
    session_free(sess);

    return rc;
}

static int session__open(icaclient_session sess)
{
    int rc;

    TRACE((TC_UI, TT_API1, "session__open: %p", sess));

    // determine the best client name to use
    clientname(gszClientName, sizeof(gszClientName));

    connect_status(sess, client_to_ids(CLIENT_STATUS_LOADING_STACK));

    rc = srvWFEngLoad( NULL, NULL );  // Call the WFEngLoad API
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // open the engine
    rc = EMEngOpen(sess);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // load the session
    rc = EMEngLoadwinframe_session(sess);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // set the product id to Internet client
    {
       WFEPRODUCTID WFEProductID;
       WFEProductID.ProductID = CLIENTID_CITRIX_RISCOS;
       wdSetProductID( &WFEProductID, sizeof(WFEProductID));
    }

    // update the status to connecting
    connect_status(sess, client_to_ids(CLIENT_STATUS_CONNECTING));

    rc = srvWFEngConnect(sess->hWFE);
    if (rc != CLIENT_STATUS_SUCCESS)
	return rc;

    // update the status to connected
    connect_status(sess, client_to_ids(CLIENT_STATUS_CONNECTED));

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

    sess->SetUp = TRUE;
    
    // initialise this here as it is statically inited in wengine
    gbContinuePolling = TRUE;
    bPDError = FALSE;

    return rc;
}

static void session__close(icaclient_session sess)
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

/* --------------------------------------------------------------------------------------------- */

icaclient_session session_open_url(const char *url, const char *bfile)
{
    icaclient_session sess;
    char *description = NULL;
    const char *s;
    arg_element *arg_list = NULL;
    BOOL unterminated_host = FALSE;

    TRACE((TC_UI, TT_API1, "session_open_url: '%s'", url));

    /* get host name (server name) */
    s = url + 4;
    if (s[0] == '/' && s[1] == '/')
    {
	const char *start = s+2;
	const char *query = strchr(start, '?');
	const char *slash = strchr(start, '/');
	const char *end;
	char *host;

	if (query && query < slash)
	{
	    end = query;
	    unterminated_host = TRUE;
	}
	else
	    end = slash;

	host = extract_and_expand(start, end ? end - start : -1);

	add_element(&arg_list, strdup(INI_ADDRESS), host);
	
	s = end;
    }

    /* get server description */
    if (s && s[0] == '/' && s[1] > ' ')
    {
	const char *start = s+1;
	const char *end = strchr(start, '?');

	description = extract_and_expand(start, end ? end - start : -1);

	s = end;
    }
    
    /* get args from query section of URL */
    if (s && s[0] == '?' && s[1] > ' ')
    {
	parse_args(&arg_list, s);

	/* patch up the case where Fresco puts a slash on the end of the URL if the host name wasn't terminated */
	if (unterminated_host && arg_list)
	{
	    char *last = arg_list->value ? arg_list->value : arg_list->name;
	    int len = strlen(last);
	    if (last[len-1] == '/')
		last[len-1] = '\0';
	}
    }

    /* get args from POST file */
    if (bfile)
    {
	FILE *f;
	if ((f = fopen(bfile, "rb")) != NULL)
	{
	    int len;
	    char *s;

	    fseek(f, 0, SEEK_END);
	    len = (int)ftell(f);
	    if ((s = malloc(len+1)) != NULL)
	    {
		/* read in post data and teeerminate */
		fread(s, len, 1, f);
		s[len] = 0;

		parse_args(&arg_list, s);

		free(s);
	    }
	    
	    fclose(f);
	}
    }

    TRACE((TC_UI, TT_API1, "session_open_url: description '%s'", strsafe(description)));

    sess = NULL;
    if (description == NULL)
    {
	arg_element *a;
	char *host;

	/* find the host name if the description isn't available */
	host = NULL;
	for (a = arg_list; a; a = a->next)
	    if (strcmpi(a->name, INI_ADDRESS) == 0)
		host = a->value;
	
	if (host)
	    sess = session_open_server(host);
    }
    else
    {
	if ((sess = session_new()) != NULL)
	{
	    strncpy(sess->gszServerLabel, description, sizeof(sess->gszServerLabel));
	}
    }

    if (sess)
	sess->arg_list = arg_list;
    else
	free_elements(&arg_list);
    
    free(description);

    return sess;
}

icaclient_session session_open_server(const char *host)
{
    icaclient_session sess = NULL;
    if (host)
    {
	char name[L_tmpnam];
	FILE *f;

	if ((sess = session_new()) != NULL)
	{
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
	    else
	    {
		MessageBox(utils_msgs_lookup("noscrap"), host);

		free(sess);
		sess = NULL;
	    }
	}
    }
    
    return sess;
}

icaclient_session session_open(const char *ica_file, int delete_after)
{
    icaclient_session sess;
    int rc;

    TRACE((TC_UI, TT_API1, "session_open: '%s'", ica_file));

    if ((sess = session_new()) == NULL)
	return NULL;

    sess->gszICAFile = strdup(ica_file);
    sess->tempICAFile = delete_after;
    
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

    return sess;
}

icaclient_session session_open_appsrv(const char *description)
{
    icaclient_session sess;

    TRACE((TC_UI, TT_API1, "session_open_appsrv: '%s'", description));

    if ((sess = session_new()) != NULL)
    {
	sess->gszICAFile = strdup(APPSRV_FILE);
    
	strncpy(sess->gszServerLabel, description, sizeof(sess->gszServerLabel));
    }

    return sess;
}

int session_connect(icaclient_session sess)
{
    int rc;
    if ((rc = session__open(sess)) != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	return FALSE;
    }
    return TRUE;
}

int session_connected(icaclient_session sess)
{
    return sess && sess->SetUp;
}

int session_poll(icaclient_session sess)
{
    int rc;
    
    TRACE((TC_UI, TT_API1, "session_poll: state %x connected %d focus %d continuepolling %d", gState, gState & WFES_CONNECTED, sess->HaveFocus, gbContinuePolling ));

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
#ifdef DEBUG
	{
	extern void *event__id_block;
	static int reported = FALSE;
	if (event__id_block == NULL && !reported)
	{
	    TRACE((TC_UI, TT_API1, "**** session_poll: id_block gone null ****" ));
	    reported = TRUE;
	}
	}
#endif
    
    }
    while (!sess->HaveFocus && gbContinuePolling && !bPDError);
    
    return gbContinuePolling;
}

void session_close(icaclient_session sess)
{
    session__close(sess);

    if (!sess->HaveFocus)
	restore_desktop();

    session_free(sess);
}

/* --------------------------------------------------------------------------------------------- */

void session_close_if_connected(icaclient_session sess)
{
    if ( !(sess->HaveFocus && !sess->Connected) )
    {
	session__close(sess);
	
	// allow cleanup
	while (session_poll(sess))
	    ;
	
	session_free(sess);
    }
}

void session_resume(icaclient_session sess)
{
    TRACE((TC_UI, TT_API1, "session_resume: %p", sess));

    // Give focus back to the engine
    if (!srvWFEngSetInformation(sess->hWFE, WFESetFocus, NULL, 0))
	sess->HaveFocus = FALSE;
}

void session_run(const char *file, int file_is_url, const char *bfile)
{
    icaclient_session sess;
#ifdef DEBUG
    extern void *event__id_block;
    TRACE((TC_UI, TT_API1, "**** session_run: id_block %p ****", event__id_block ));
#endif

    TRACE((TC_UI, TT_API1, "session_run: '%s' url %d bfile '%s'", strsafe(file), file_is_url, strsafe(bfile)));

    if (file_is_url)
	sess = session_open_url(file, bfile);
    else
	sess = session_open(file, FALSE);

    if (sess)
    {
	if (!session_connect(sess))
	    return;
    
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
}

/* --------------------------------------------------------------------------------------------- */

extern int SdLoad( PDLLLINK );
extern int ReducerLoad( PDLLLINK pLink );
extern int AudHWSnd16Load( PDLLLINK pLink );

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
	{ "tdasync",	(PLIBPROCEDURE)TdLoad/*, TdAsyncDeviceProcedures */ },
	{ "pdcrypt",	(PLIBPROCEDURE)PdLoad/*, PdCryptDeviceProcedures */ },
	{ "pdrfram",	(PLIBPROCEDURE)PdLoad/*, PdRFrameDeviceProcedures */ },
	{ "pdframe",	(PLIBPROCEDURE)PdLoad/*, PdFrameDeviceProcedures */ },
	{ "pdreli",	(PLIBPROCEDURE)PdLoad/*, PdReliDeviceProcedures */ },
	{ "pdmodem",	(PLIBPROCEDURE)PdLoad/*, PdModemDeviceProcedures */ },
	{ "wdica30",	(PLIBPROCEDURE)WdLoad/*, WdICA30EmulProcedures */ },
	{ "wdtty",	(PLIBPROCEDURE)WdLoad/*, WdTTYEmulProcedures */ },
	{ "nrtcpro",	(PLIBPROCEDURE)NrLoad/*, NrTcpRODeviceProcedures */ },
	{ "nrica",	(PLIBPROCEDURE)NrLoad/*, NrICADeviceProcedures */ },
#ifdef INCL_ENUM
	{ "neica",	(PLIBPROCEDURE)NeLoad/*, NeICADeviceProcedures */ },
#else
	{ "neica",	0 },
#endif
	{ "vdtw30",	(PLIBPROCEDURE)VdLoad/*, VdTW31DriverProcedures */ },
	{ "vdcpm30",	(PLIBPROCEDURE)VdLoad/*, VdCpmDriverProcedures */ },
	{ "vdspl30",	(PLIBPROCEDURE)VdLoad/*, VdSplDriverProcedures */ },
	{ "vdcdm",	(PLIBPROCEDURE)VdLoad/*, VdCdmDriverProcedures */ },
	{ "vdcam",	(PLIBPROCEDURE)VdLoad/*, VdCamDriverProcedures */ },
	{ "audsnd16",	(PLIBPROCEDURE)AudHWSnd16Load/*, AudSnd16DriverProcedures */ },
#ifdef INCL_SCRIPT
	{ "script",	(PLIBPROCEDURE)SdLoad, NULL },
#endif
#ifdef INCL_REDUCER
	{ "icaredu",    (PLIBPROCEDURE)ReducerLoad, NULL },
#endif
	{ NULL,		0,	NULL }
    }, *mp;

    TRACE((TC_UI, TT_API1, "ModuleLookup: '%s'", pName));

    if (!inited)
    {
#ifdef INCL_TCPIP
	modules[0].fnTable = TdTcpRODeviceProcedures;
#endif
#ifdef INCL_ASYNC
	modules[1].fnTable = TdAsyncDeviceProcedures;
#endif
	modules[2].fnTable = PdCryptDeviceProcedures;
#ifdef INCL_TCPIP
	modules[3].fnTable = PdRFrameDeviceProcedures;
#endif
#ifdef INCL_ASYNC
	modules[4].fnTable = PdFrameDeviceProcedures;
	modules[5].fnTable = PdReliDeviceProcedures;
#endif
#ifdef INCL_MODEM
	modules[6].fnTable = PdModemDeviceProcedures;
#endif
	modules[7].fnTable = WdICA30EmulProcedures;
	modules[8].fnTable = WdTTYEmulProcedures;

#ifdef INCL_TCPIP
	modules[9].fnTable = NrTcpRODeviceProcedures;
	modules[10].fnTable = NrICADeviceProcedures;
#endif
#ifdef INCL_ENUM
	modules[11].fnTable = NeICADeviceProcedures;
#endif
	modules[12].fnTable = VdTW31DriverProcedures;
#ifdef INCL_PRINTER
	modules[13].fnTable = VdCpmDriverProcedures;
	modules[14].fnTable = VdSplDriverProcedures;
#endif
#ifdef INCL_DRIVES
	modules[15].fnTable = VdCdmDriverProcedures;
#endif
#ifdef INCL_AUDIO
	modules[16].fnTable = VdCamDriverProcedures;
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
