/* session.c

 *

 */

#include "windows.h"

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

#include "session.h"

#include "utils.h"

#include "../inc/pddevicep.h"
#include "../inc/wdemulp.h"
#include "../inc/vddevicep.h"
#include "../inc/nrdevicep.h"

#include "rdebug.h"
#include "swis.h"

#include "tboxlibs/toolbox.h"
#include "sessionp.h"

/* --------------------------------------------------------------------------------------------- */

static char *gszWfclientIni = APP_PATH "WFClient";
static char *gszModuleIni = APP_PATH "Module";

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

static int EMLogInit(void);

/* --------------------------------------------------------------------------------------------- */

static void MessageBox(const char *message, const char *server)
{
    _kernel_oserror e;
    e.errnum = 0;
    strncpy(e.errmess, message, sizeof(e.errmess)-1);
    _swix(Wimp_ReportError, _INR(0,2), &e, 0, server);
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
            if ( sess->hWFE ) {
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
               }
            break;

        case CLIENT_ERROR_PD_TRANSPORT_UNAVAILABLE:
            {
//                 LoadString(ghInstance,
//                            IDS_PDTRANSPORTUNAVAILABLE_FORMAT,
//                            strFormatString,
//                            sizeof(strFormatString));
                /*
                 * First find out what transport we're dealing with
                 */
                GetPrivateProfileString( sess->gszServerLabel,
                                         INI_TRANSPORTDRIVER,
                                         DEF_TRANSPORTDRIVER,
                                         pszTransport, sizeof(pszTransport),
                                         sess->gszICAFile );

		strcpy(strFormatString, "Transport %s unavailable");
                wsprintf( szBuffer, strFormatString, pszTransport );
            }
            break;
#if 0
        case CLIENT_ERROR_WFENGINE_NOT_FOUND:
//             LoadString(ghInstance,
//                        IDS_CLIENT_ERROR_WFENGINE_NOT_FOUND,
//                        strFormatString,
//                        sizeof(strFormatString));

            wsprintf( szBuffer, strFormatString,
#ifdef WIN16
                      "WFENGW.EXE" 
#else
                      "WFENGN.EXE"
#endif
                    );
            break;
#endif
	    
        default:
            break;
    }

    /*
     *  If we did not get a specific error message; return the generic one
     *  associated with the error code.
     */
    if ( !(*szBuffer) ) {
	char buf[16], *s;
	sprintf(buf, "msg%d:", iErrorCode);
	s = utils_msgs_lookup(buf);
	if (s && s[0])
	    strcpy(szBuffer, s);
	else
	    sprintf(szBuffer, "Error code %d", iErrorCode);

#if 0
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
#endif
    }
    
    /*
     * If we still don't have an error message, fetch the 'no error text
     * available' message and return that.
     */
#if 0
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
#endif
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
    Session sess = global_session;

#if 0
    // send to plug in if available
    if(g_hWndPlugin && g_MsgPlugin) {
       SendMessage( g_hWndPlugin, g_MsgPlugin, message, lParam);
       }
#endif
    
    switch ( message ) {

        case CLIENT_STATUS_CONNECTION_BROKEN_HOST:
        case CLIENT_STATUS_CONNECTION_BROKEN_CLIENT:

            if ( (rc = srvWFEngUnloadDrivers( hWFE )) != CLIENT_STATUS_SUCCESS ) {
    
            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGUNLOADDRIVERS);
            }
            if ( (rc = srvWFEngClose( sess->hWFE )) != CLIENT_STATUS_SUCCESS ) {

            // LogStandardErrorMessage(FALSE, rc, IDP_ERROR_WFENGCLOSE);
            }
#ifdef DEBUG
            LogPrintf( LOG_CLASS, LOG_CONNECT, "DISCONNECTED from %s", sess->gszServerLabel);
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
//          lResult = EMStatusMessage(message);
            break;


        case CLIENT_STATUS_HOTKEY1:     // invoke local task list
            break;

        case CLIENT_STATUS_DELETE_CONNECT_DIALOG:
            connect_close(sess);
            break;

        case CLIENT_STATUS_CONNECTED:
#ifdef DEBUG
            LogPrintf( LOG_CLASS, LOG_CONNECT, "CONNECTED to %s", sess->gszServerLabel);
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
#ifdef DEBUG
	{
            char szStatus[80];
            sprintf(szStatus,"Unknown status message (%d)",message);
	    MessageBox(szStatus, sess->gszServerLabel);
	}
#endif
            break;
    }

    return(lResult);

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
//    WFEOPENP WFEOpenp;
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
    WFEOpen.pfnStatusMsgProc = &WFEngineStatusCallback;
    WFEOpen.pszClientname    = gszClientName;   
    WFEOpen.uInitialWidth    = (int)iHRes;   
    WFEOpen.uInitialHeight   = (int)iVRes;  
    WFEOpen.pszLogLabel      = "IWFC";     
    WFEOpen.pszTitle         = sess->gszServerLabel;
    WFEOpen.pszClientSN      = "";                 
    WFEOpen.pszIconPath      = "<WSClient$Dir>";     
    WFEOpen.uIconIndex       = 0;      
    WFEOpen.reserved         = 0; //(ULONG)&WFEOpenp;
//    WFEOpenp.hWndPlugin      = 

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
    DBG(("session_free: %p\n", sess));

    if (global_session == sess)
	global_session = NULL;

    free(sess->gszICAFile);
    free(sess);
}

static int session_abort(Session sess, int rc)
{
    DBG(("session_abort: %p state %d\n", sess, rc));

    connect_close(sess);
    EMErrorPopup(sess, rc);

    session_free(sess);

    return rc;
}

Session session_open(const char *ica_file)
{
    int rc;
    char *pEnvVar;
    Session sess;

    DBG(("session_open: '%s'\n", ica_file));

    sess = global_session = calloc(sizeof(struct session_), 1);

    rc = EMLogInit();
    DBG(("LogInit: returns %d\n", rc));
    
    sess->gszICAFile = strdup(ica_file);
    
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
    
    DBG(("got server: '%s'\n", sess->gszServerLabel));

    GetPrivateProfileString(sess->gszServerLabel, INI_ENCRYPTIONLEVELSESSION, 
			    "",
			    sess->szEncryptionDLL, sizeof(sess->szEncryptionDLL),
			    sess->gszICAFile);

    
    if (sess->gszServerLabel[0] == '\0')
    {
	session_free(sess);

	msg_report("Can't find server"/* IDS_CLIENT_ERROR_SERVER_SECTION_NOT_FOUND */);

	return NULL;
    }

    // determine the client name from environment variable first
    // then use Win32 computername and then the default from the
    // resource file
    pEnvVar = getenv(ENV_CLIENTNAME);
    if (pEnvVar == NULL)
	pEnvVar = getenv(INET_HOSTNAME);

    // use time stampe to creat an unique client name
    if (pEnvVar == NULL)
	sprintf(gszClientName, "Internet%08u", clock() & 0xffffff);
    else
	lstrcpy(gszClientName, pEnvVar);

    // start the connect status dialog 
    connect_open(sess);
    connect_status(sess, CLIENT_STATUS_LOADING_STACK);

    rc = srvWFEngLoad( NULL, NULL );  // Call the WFEngLoad API
    if (rc != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	return NULL;
    }

    // open the engine
    rc = EMEngOpen(sess);
    if(rc != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	return NULL;
    }

    // load the session
    rc = EMEngLoadSession(sess);
    if(rc != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	return NULL;
    }

    // set the product id to Internet client
    {
       WFEPRODUCTID WFEProductID;

       // If the module.ini file exists then it's not the internet client
       WFEProductID.ProductID = CLIENTID_CITRIX_INTERNET;
       wdSetProductID( &WFEProductID, sizeof(WFEProductID));
    }

    // update the status to connecting
    connect_status(sess, CLIENT_STATUS_CONNECTING);

    rc = srvWFEngConnect(sess->hWFE);
    if (rc != CLIENT_STATUS_SUCCESS)
    {
	session_abort(sess, rc);
	return NULL;
    }

    connect_close(sess);
    
    return sess;
}

void session_poll(Session sess)
{
    DBG(("session_poll: state %x connected %d\n", gState, gState & WFES_CONNECTED ));

    if ( gState & WFES_CONNECTED )
	srvWFEngMessageLoop( sess->hWFE );
}

void session_close(Session sess)
{
    DBG(("session_close: %p\n", sess));

    /*
     * These APIs are called automatically on exit
     * (The APIs called by the UI are handled in WFCWINx.DLL)
     */
    srvWFEngUnloadDrivers( sess->hWFE );
    srvWFEngClose( sess->hWFE );
    srvWFEngUnload( (PMINIDLL)NULL );  // Call the WFEngUnload API

    connect_close(sess);

    session_free(sess);
}

void session_close_all(void)
{
    if (global_session)
	session_close(global_session);
}

/* --------------------------------------------------------------------------------------------- */

void session_run(const char *ica_file)
{
    Session sess = session_open(ica_file);
    session_poll(sess);
    session_close(sess);
}

/* --------------------------------------------------------------------------------------------- */

int ModuleLookup( PCHAR pName, PLIBPROCEDURE *pfnLoad, PPLIBPROCEDURE *pfnTable )
{
    static struct
    {
	PCHAR pName;
	PLIBPROCEDURE fnLoad;
	PPLIBPROCEDURE fnTable;
    } modules[] =
    {
	{ "tdtcpro",	(PLIBPROCEDURE)TdLoad, TdTcpRODeviceProcedures },
//	{ "pdrframe",	(PLIBPROCEDURE)PdLoad, PdRFrameDeviceProcedures },
//	{ "pdcrypt1",	(PLIBPROCEDURE)PdLoad, PdCrypt1DeviceProcedures },
	{ "wdtty",	(PLIBPROCEDURE)WdLoad, WdTTYEmulProcedures },
	{ "wdica30",	(PLIBPROCEDURE)WdLoad, WdICA30EmulProcedures },
//	{ "vdtw31",	(PLIBPROCEDURE)VdLoad, VdTW31DeviceProcedures },
	{ "nrtcpro",	(PLIBPROCEDURE)NrLoad, NULL },
//	{ "neica",	(PLIBPROCEDURE)NeLoad, NeICADeviceProcedures },
	{ NULL,		0,	NULL }
    }, *mp;

    DBG(("ModuleLookup: '%s'\n", pName));
    
    for (mp = modules; mp->pName; mp++)
    {
	if (strcmpi(mp->pName, pName) == 0)
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

#ifdef DEBUG
/*
 * EMLogInit
 *
 *
 * Initialize the important stuff for the Log file
 *
 */
int  EMLogInit(void)
{
   LOGOPEN EMLogInfo;
   int rc = CLIENT_STATUS_SUCCESS;

   LogClose();
   
   EMLogInfo.LogFlags   = LOG_APPEND;
   EMLogInfo.LogClass   = TC_ALL;
   EMLogInfo.LogEnable  = TT_ERROR;
   lstrcpy(EMLogInfo.LogFile, "<Wimp$ScrapDir>." APP_NAME);

   rc = LogOpen(&EMLogInfo);

   return(rc);
}
#endif //DEBUG

