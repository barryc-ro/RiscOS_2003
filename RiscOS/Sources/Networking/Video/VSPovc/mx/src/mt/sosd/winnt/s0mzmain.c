/* Copyright (c) 1994, 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * s0mzmain.c - OSD main() for executables
 *
 * DESCRIPTION
 * This is a standard template for an OSD main().  To use this template,
 * you could include the following sequence in your file mzyour.c:
 *
 *	#define ENTRY_POINT	mzyourMain
 *	#define DEFAULT_MTL_TOOL MtlScreenTool | MtlLogTool | MtlConsoleTool
 *	#ifndef MZYOUR_ORACLE
 *	#include <mzyour.h>
 *	#endif
 *	#include <s0mzmain.c>
 *
 * Or, you could create s0mzyour.o directly by having your Makefile do:
 *
 *  $(OBJ)/s0mzyour.o : $(OSD)/s0mzmain.c
 *    $(CC) $(CFLAGS) -o $@ -c  $(OSD)/s0mzmain.c -DENTRY_POINT=mzyourMain
 *
 * Either approach will produce an appropriate OSD that
 * will do the right thing:  It will set up YS, start mtl, and invoke your
 * portable entry point, passing it the following parameters:
 *
 *   dvoid *osdCtx - OSD pointer, not for general use.
 *   char *progName - however derived.
 *   argc - argument count (does not include program name!!)
 *   argv - argument list array (does not include program name!!)
 *
 * Your main program should load the resource database using ysArgParse()
 * It should respond to the common flag:
 *
 *	    -V		version (mtvBanner-like information) 
 *
 * (ysArgParse handles the help/usage message itself).
 * 
 * If it wishes to interpret any arguments for media-net, it must map them
 * to the resources:
 *
 *	    mn.trace		    -- "true" turns tracing on.
 *	    mn.heapsize		    -- in bytes
 *	    mn.sync-gateway	    -- polled gateway addr
 *	    mn.async-gateway	    -- interrupt driven gateway addr
 *
 * We suggest using the old "-g sync-gateway" and "-i async-gateway"
 * convention for these arguments, as in the following arg map fragment:
 *
 *	    { 'g', "mn.sync-gateway", 1 },
 *	    { 'h', "mn.heapsize", 1 },
 *	    { 'i', "mn.async-gateway", 1 },
 *
 * If your program wishes to interpret arguments that affect the mtl
 * destination, it may issue a new mtlInit.  If you wish to have an
 * argument that directs the program to different mtl destinations,
 * we suggest using the convention
 *
 *	    { 'e', "<progname>.logtool", 1 },
 *
 * and recognizing "screen", "log" and "console" as values for
 * MtlScreenTool, MtlLogTool and MtlConsoleTool respectively.
 *
 * After you have done argument parsing, you should then start media
 * net as necessary.  Most non-gateway programs can use the entry point 
 *
 *	boolean	smniInit( dvoid *osdCtx, mnLogger logger );
 *
 * When smniInit() returns sucessfully.
 *
 *    - The Resource mn.physical-address will be set to mtpaPrint()ed
 *      strings of each opened gateway.
 *
 * MODIFIED
 * dbrower   10/18/96 -  extensive changes for potential use as a server.
 */


#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSLST_ORACLE
#include <yslst.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif

#ifndef MTL_ORACLE
#include <mtl.h>
#endif

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif

#include <ctype.h>
#include <sys/types.h>		                           /* For pid_t type */
#include <process.h>		                 /* For getpid() system call */


typedef struct argarea
{
  dvoid *osdbuf;
  char  *argv0;
  sword argc;
  char  **argv;
} ARGAREA;

/* DISABLE check_proto_decl */
void    main(int argc, char **argv);

/* will default mtl tool to screen */

#ifndef DEFAULT_MTL_TOOL
#define DEFAULT_MTL_TOOL    MtlScreenTool
#endif

/* ENTRY_POINT had better be defined... */

#ifndef ENTRY_POINT
    /* Raise compile error on ANSI with #error and non/ANSI by 
       not having the #error in the first column. */
    #error "ENTRY_POINT not defined.  It must be!"
#endif 

boolean ENTRY_POINT(dvoid *osdCtx, CONST char *progName,
 		    sword argc, char **argv );

externdef HANDLE      hServerStopEvent;

/* globals for win32 service */
SERVICE_STATUS        ssStatus;
SERVICE_STATUS_HANDLE sshStatusHandle;
char                  srvName[SYSFP_MAX_PATHLEN];
DWORD                 dwErr;
ARGAREA               argarea;

STATICF void ServiceStart(int argc, char **argv);
STATICF void ServiceStop(void);
STATICF boolean ReportStatusToSCMgr(DWORD dwCurrentState,
				    DWORD dwWin32ExitCode, 
				    DWORD dwWaitHint);
STATICF boolean win32srvMain(char *base, sword argc, char **argv);
STATICF void WINAPI service_main(int argc, char **argv);
STATICF void WINAPI service_ctrl(int ctrlCode);
STATICF void argsToArgv(char *args, int *argc, char **argv);
STATICF void ServiceStart(int argc, char **argv);
STATICF void ServiceStop();
STATICF BOOL IsInteractive(void);
STATICF int GetPlatformId (void);
STATICF DWORD WINAPI EntryPointThread(LPVOID arg);

STATICF boolean ReportStatusToSCMgr(DWORD dwCurrentState,
				    DWORD dwWin32ExitCode, 
				    DWORD dwWaitHint)
{
  static DWORD dwCheckPoint = 1;
  boolean fResult = TRUE;

  if (dwCurrentState == SERVICE_START_PENDING)
    ssStatus.dwControlsAccepted = 0;
  else
    ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

  ssStatus.dwCurrentState = dwCurrentState;
  ssStatus.dwWin32ExitCode = dwWin32ExitCode;
  ssStatus.dwWaitHint = dwWaitHint;

  if ( ( dwCurrentState == SERVICE_RUNNING ) ||
       ( dwCurrentState == SERVICE_STOPPED ) )
    ssStatus.dwCheckPoint = 0;
  else
    ssStatus.dwCheckPoint = dwCheckPoint++;

  /* Report the status of the service to the service control manager. */
  if (!(fResult = SetServiceStatus( sshStatusHandle, &ssStatus)))
    ssyslSysLog( EVENTLOG_ERROR_TYPE, 0, YSLSEV_ERR + 1, "SetServiceStatus");
  return fResult;
}

STATICF void WINAPI service_ctrl(int ctrlCode)
{
  /* handle the requested control code */
  switch(ctrlCode)
  {
  case SERVICE_CONTROL_STOP:
    ssStatus.dwCurrentState = SERVICE_STOP_PENDING;
    ServiceStop();
    break;

  default:	/* un-handled or invalid control code */
    break;
  }

  ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
}

STATICF void WINAPI service_main(int argc, char **argv)
{
  /* register our service control handler */
  sshStatusHandle = RegisterServiceCtrlHandler(srvName,
                                      (LPHANDLER_FUNCTION)service_ctrl);

  ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  ssStatus.dwServiceSpecificExitCode = 0;
  
  if (!sshStatusHandle)
    goto cleanup;

  ServiceStart(argc, argv);

cleanup:

  if (sshStatusHandle)
    ReportStatusToSCMgr(SERVICE_STOPPED, 0, 0);
  return;
}

STATICF void argsToArgv(char *args, int *argc, char **argv)
{
  boolean  quotemode;
  char    *p = args;
  char    *n;
  int      i = 1;
 
  while(*p)
  {
    while (*p == ' ')          /* skip the space(s) */
      p++;

    if (*p == '\"')
      quotemode = TRUE, p++;   /* skip the quote */
    else
      quotemode = FALSE;
    
    if (*p)
      argv[i++] = p;

    n = strchr(p, (quotemode ? '\"' : ' '));
	
    if (n)
      *n = '\0';
    else
      break;
    
    p = n + 1;
  }
  *argc = i;
}

STATICF boolean win32srvMain(char *base, sword argc, char **argv)
{
  char  buf[SYSFP_MAX_PATHLEN];
  char  subkey[SYSFP_MAX_PATHLEN];
  char  pidbuf[5];
  int   len, type;
  HKEY  hk;
  
  SERVICE_TABLE_ENTRY dispatchTable[] =
    {
      { NULL, (LPSERVICE_MAIN_FUNCTION)service_main },
      { NULL, NULL }
    };

  strcpy(srvName, SSYSI_SVC_PREFIX);
  strcat(srvName, "_");
  /* base is not truly unique since the service name may be different */
  strcat(srvName, base); 

  /* syslError/yslError output can be lost without this. */
  /* Get home directory from registry */
  strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
  subkey[strlen(SSYSI_SVC_PARMS_PREFIX)-1] = '\0';
  len = sizeof(buf);

  if ((RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS) ||
      (RegQueryValueEx(hk, SSYSI_PRODUCT_HOME_KEY, NULL, &type, buf, &len)
	   != ERROR_SUCCESS))
  {
    /* take the default */
    strcpy( buf, SSYSI_DEF_PRODUCT_HOME );
  }
  RegCloseKey(hk);
  strcat( buf, SSYSI_YSL_ERR_DIR );
  strcat( buf, base );
  /* append process id for unique log file */
  strcat( buf, itoa(getpid(),pidbuf,10)); 

  dispatchTable[0].lpServiceName = (char *)srvName;

  if (!StartServiceCtrlDispatcher(dispatchTable))
  {
    freopen( buf, "w", stderr );
    ssyslSysLog( EVENTLOG_ERROR_TYPE, 0, YSLSEV_ERR + 1,
		"StartServiceCtrlDispatcher failed.");
    return FALSE;
  }

  return TRUE;
}

STATICF void ServiceStart(int argc, char **argv)
{
  ub1	osdbuf[SYSX_OSDPTR_SIZE];
  char  buf[SYSFP_MAX_PATHLEN];
  char *regArgBuf = NULL;	      /* pointer to registry arguments buf */
  int   regArgc = 0;  
  char *regArgv[128];		      /* array pointer to reg arguments */
  char  subkey[SYSFP_MAX_PATHLEN];
  HKEY  hk;
  int   argLen, type;
  DWORD thrId;
  char *base_svcname;
  int   len = 0;
  
  /* Service initialization */

  /* argv[0] has full service key name now */
  base_svcname = argv[0]; 
  /* skip past the SSYSI_SVC_PREFIX value to get to the base service name */
  base_svcname += sizeof(SSYSI_SVC_PREFIX);

  /* syslError/yslError output can be lost without this. */
  /* get home directory from registry */
  strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
  subkey[strlen(SSYSI_SVC_PARMS_PREFIX)-1] = '\0';
  len = sizeof(buf);

  if ((RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS) 
      || (RegQueryValueEx(hk, SSYSI_PRODUCT_HOME_KEY, NULL, &type, buf, &len) 
	  != ERROR_SUCCESS))
  {
    /* take the default */
    strcpy(buf, SSYSI_DEF_PRODUCT_HOME);
  }

  RegCloseKey(hk);
  strcat(buf, SSYSI_YSL_ERR_DIR);
  strcat(buf, base_svcname);				
  strcat(buf, SSYSI_YSL_ERR_SFX); 
  
  /* redirect stderr to named file based on service name */
  freopen(buf, "w", stderr); 

  /* Service initialization */
  syslOsdInit(&osdbuf, (syslPFunc)0, (syslPFunc)0);
  
  /* report the status to the service control manager. */
  if (!ReportStatusToSCMgr(SERVICE_START_PENDING, /* service state */
                           EXIT_SUCCESS,          /* exit code */
                           3000))                 /* wait hint */
    goto cleanup;

  /* create the event object. The control handler function signals  */
  /* this event when it receives the "stop" control code.           */
  hServerStopEvent = CreateEvent(NULL,    // no security attributes
                                 TRUE,    // manual reset event 
				 FALSE,   // not-signalled
      				 NULL);   // no name

  if ( hServerStopEvent == NULL)
      goto cleanup;

  /* Get service startup parameters from registry */
  strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
  strcat(subkey, argv[0]);		/* argv[0] is service name */
  if (RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS)
  {
    /* there is no key */
    regArgBuf = malloc(1);
    regArgBuf[0] = '\0';
  }
  else if (RegQueryValueEx(hk, SSYSI_SVC_ARGVAL, NULL, &type, NULL, &argLen)
	   != ERROR_SUCCESS)
  {
    /* there is no StartUp key */
    regArgBuf = malloc(1);
    regArgBuf[0] = '\0';
  }
  else
  {
    regArgBuf = malloc(argLen+1);
    if (RegQueryValueEx(hk, SSYSI_SVC_ARGVAL, NULL, &type, regArgBuf, &argLen)
                                                  != ERROR_SUCCESS)
      goto cleanup;
  }
  
  DISCARD argsToArgv(regArgBuf, &regArgc, regArgv);
  
  /* report the status to the service control manager. */
  if (!ReportStatusToSCMgr(SERVICE_RUNNING,       // service state
      			   NO_ERROR,              // exit code
      			   3000))                 // wait hint
    goto cleanup;

  /* End of initialization */
  argarea.osdbuf = (dvoid *)osdbuf;
  argarea.argv0 = base_svcname;
  argarea.argc = regArgc - 1;
  argarea.argv = regArgv + 1;
  
  if (!CreateThread( NULL, 0, EntryPointThread, &argarea, 0, &thrId))
    goto cleanup;

  /* report the status to the service control manager. */
  if (!ReportStatusToSCMgr(SERVICE_RUNNING,       // service state
      			   EXIT_SUCCESS,          // exit code
      			   3000))                 // wait hint
    goto cleanup;

  WaitForSingleObject(hServerStopEvent, INFINITE);

  /* add code to gracefully shutdown the thread !!! */

cleanup:

  if (hServerStopEvent)
      CloseHandle(hServerStopEvent);
  if (regArgBuf)
    free(regArgBuf);
}

/* FIXME Sadly, this blindly exits the process w/o shutting anything down. */

STATICF void ServiceStop()
{
  if ( hServerStopEvent )
  {
    SetEvent(hServerStopEvent);
    raise( SIGINT );
  }
}


STATICF DWORD WINAPI EntryPointThread(LPVOID arg)
{
  ARGAREA *parg = (ARGAREA *)arg;
  boolean rv;

  rv = ENTRY_POINT((dvoid *)parg->osdbuf, parg->argv0, parg->argc, parg->argv);

  SetEvent(hServerStopEvent);
  return( rv ? 0 : 1 );
}


/*
  BOOL IsInteractive(void)

  returns TRUE if user is interactive
	  FALSE if user is not interactive
*/
STATICF BOOL IsInteractive(void)
{
  HANDLE hAccessToken;
  UCHAR InfoBuffer[1024];
  PTOKEN_GROUPS ptgGroups = (PTOKEN_GROUPS)InfoBuffer;
  DWORD dwInfoBufferSize;
  PSID psidInteractive;
  SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;
  UINT x;
  BOOL bSuccess;

  if(!OpenProcessToken(GetCurrentProcess(),TOKEN_READ,&hAccessToken))
    return(FALSE);

  bSuccess = GetTokenInformation(
  				hAccessToken,
  				TokenGroups,
  				InfoBuffer,
  				1024,
  				&dwInfoBufferSize
  				);

  CloseHandle(hAccessToken);

  if( !bSuccess )
    return FALSE;

  if(!AllocateAndInitializeSid(&siaNtAuthority, 1,
  		SECURITY_INTERACTIVE_RID,
  		0, 0, 0, 0, 0, 0, 0,
  		&psidInteractive))
    return FALSE;

  // assume that we don't find the Interactive SID.
  bSuccess = FALSE;

  for(x = 0; x < ptgGroups->GroupCount; x++) 
  {
    if( EqualSid(psidInteractive, ptgGroups->Groups[x].Sid) ) 
    {
      bSuccess = TRUE;
      break;
    }
  }

  FreeSid(psidInteractive);
  return bSuccess;
}  

/* GetPlatformId() - get platform id from windows version information
 *
 * returns: VER_PLATFORM_WIN32_NT (winnt) or VER_PLATFORM_WIN32_WINDOWS (win95)
 */
STATICF int GetPlatformId (void)
{
  OSVERSIONINFO osverinfo;

  osverinfo.dwOSVersionInfoSize = sizeof (osverinfo);
  if (GetVersionEx(&osverinfo))
    return (osverinfo.dwPlatformId);
  else
    return 0;
}

/* ---------------------------- main ---------------------------- */
/*
  NAME
    main
  DESCRIPTION
    OSD Main program.
  PARAMETERS
    argc	-- standard argc, always >= 1
    argv	-- standard argv, with program name in argv[0].
  RETURNS
    Doesn't.
*/
void main(int argc, char **argv)
{
  boolean sts;
  ub1 osdbuf[SYSX_OSDPTR_SIZE];
  char *cl, buf[128], base[SYSFP_MAX_PATHLEN];

  sysfpExtractBase(base, argv[0]);

  syslOsdInit( &osdbuf, (syslPFunc)0, (syslPFunc)0 );
  
  /* standardized initialization */
  ysInit((dvoid *) osdbuf, base);

  /* Set a resource which uniquely identifies us */
  DISCARD sprintf(buf, "%d", getpid());
  ysResSet("uniqueID", buf);
  
  /* append OSD argmap(s) here */

  if (cl = (char *) getenv("CONSOLE_LOG"))
    {
      sprintf(buf, "%s\\%s", cl, base);
      mtlInit( DEFAULT_MTL_TOOL, buf );
    }
  else
    mtlInit( DEFAULT_MTL_TOOL, base );

  hServerStopEvent = (HANDLE) NULL;

  /* run console app, if (1) interactive mode on winnt, or (2) on Win95 */
  if ((GetPlatformId() == VER_PLATFORM_WIN32_NT && IsInteractive()) || 
      (GetPlatformId() == VER_PLATFORM_WIN32_WINDOWS))
    sts = ENTRY_POINT((dvoid *)osdbuf, base, (sword) argc - 1, argv + 1);
  else 
    win32srvMain(base, (sword) argc, argv);

  ysTerm((dvoid *) osdbuf);

  /* terminate program */
  exit((int) !sts);
}

