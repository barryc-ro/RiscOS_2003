/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * s0ysmain.c - OSD main() for executables
 *
 * DESCRIPTION
 * This is a standard template for an OSD main().  To use this template,
 * include the following sequence prior to the declaration of your portable
 * entry point.
 *
 * #define ENTRY_POINT  <name-of-portable-entry-point>
 * #include <s0ysmain.c>
 *
 * MODIFICATIONS
 * dbrower   09/ 9/96 - add odsbuf to stack of main(). 
 * dbrower   10/18/96 - extensive changes for potential use as a server.
 * dbrower   10/23/96 - changes from jchangav to make shutdown work.
 */

#include <signal.h>
#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
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

#include <ctype.h>

typedef struct argarea
{
  dvoid *osdbuf;
  char  *argv0;
  sword argc;
  char  **argv;
} ARGAREA;

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

/* DISABLE check_proto_decl */

void    main(int argc, char **argv);

/* ENTRY_POINT had better be defined... */

#ifndef ENTRY_POINT
    /* Raise compile error on ANSI with #error and non/ANSI by 
       not having the #error in the first column. */
    #error "ENTRY_POINT not defined.  It must be!"
#endif 

boolean ENTRY_POINT(dvoid *osdp, char *nm, sword argc, char **argv);

/*
 * main - OSD main()
 *
 * Don't do anything about signals here.  sysiInit will set that up
 * when called.
 */
void main(int argc, char **argv)
{
  boolean sts;
  ub1 osdbuf[SYSX_OSDPTR_SIZE];
  char base[SYSFP_MAX_PATHLEN];

  sysfpExtractBase(base, argv[0]);

  hServerStopEvent = (HANDLE) NULL;

  syslOsdInit( &osdbuf, (syslPFunc)0, (syslPFunc)0 );

  /* run console app, if (1) interactive mode on winnt, or (2) on Win95 */
  if ((GetPlatformId() == VER_PLATFORM_WIN32_NT && IsInteractive()) || 
      (GetPlatformId() == VER_PLATFORM_WIN32_WINDOWS))
    sts = ENTRY_POINT((dvoid *) osdbuf, base, (sword) argc - 1, argv + 1);
  else 
    sts = win32srvMain(base, (sword) argc, argv);

  /* call main entrypoint */

  /* terminate program */
  exit((int) !sts);
}

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
  {
    ssyslSysLog( EVENTLOG_ERROR_TYPE, 0, YSLSEV_ERR + 1,
		"SetServiceStatus" );
  }
  return fResult;
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
    ReportStatusToSCMgr(SERVICE_STOPPED, NO_ERROR, 0);
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

  ReportStatusToSCMgr(ssStatus.dwCurrentState, EXIT_SUCCESS, 0);
}


STATICF void argsToArgv(char *args, int *argc, char **argv)
{
  boolean  quotemode;
  char    *p = args;
  char    *n;
  int      i = 1;
 
  while(*p)
  {
    while (*p == ' ')           /* skip the space(s) */
      p++;

    if (*p == '\"')
      quotemode = TRUE, p++;    /* skip the quote */
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

  strcpy(srvName, SSYSI_SVC_PREFIX );
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
    freopen(buf, "w", stderr);
    ssyslSysLog(EVENTLOG_ERROR_TYPE, 0, YSLSEV_ERR + 1,
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
  HANDLE hSvcEvent = NULL;
  char	*base_svcname;
  int    len = 0;
  
  /* Service initialization.  argv[0] has full service key name */
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

  syslOsdInit(&osdbuf, (syslPFunc)0, (syslPFunc)0);
  
  /* report the status to the service control manager. */
  if (!ReportStatusToSCMgr(SERVICE_START_PENDING, // service state
                           EXIT_SUCCESS,          // exit code
                           3000))                 // wait hint
    goto cleanup;
  
  // create the event object. The control handler function signals
  // this event when it receives the "stop" control code.
  hServerStopEvent = CreateEvent(NULL,    // no security attributes
				 TRUE,    // manual reset event
				 FALSE,   // not-signalled
				 NULL);   // no name

  if (hServerStopEvent == NULL)
    goto cleanup;

  /* Get service startup parameters from registry */
  strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
  strcat(subkey, argv[0]);
  if (RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS)
  {
    /* there is no parm node for the service */
    regArgBuf = malloc(1);
    regArgBuf[0] = '\0';
  }
  else if (RegQueryValueEx(hk, SSYSI_SVC_ARGVAL, NULL, &type, NULL, &argLen) 
	   != ERROR_SUCCESS)
  {
    /* there is no Arguments key */
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
  RegCloseKey(hk);
  
  DISCARD argsToArgv(regArgBuf, &regArgc, regArgv);

  /* report the status to the service control manager. */
  if (!ReportStatusToSCMgr(SERVICE_RUNNING,       // service state
      			   EXIT_SUCCESS,          // exit code
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

  if (argc > 1)
  {
    argv[1][0] = 'E';
    if (hSvcEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, argv[1]))
    {
      HANDLE hSvcPid = NULL;
      INT *pSvcPid;
  
      argv[1][0] = 'S';
      /* comes here for services spawned by other media net services */
      if (hSvcPid = OpenFileMapping( FILE_MAP_READ|FILE_MAP_WRITE, FALSE, 
				    argv[1] ))
	if (!(pSvcPid = (INT *)MapViewOfFile( hSvcPid, 
					     FILE_MAP_READ|FILE_MAP_WRITE,
					     0, 0, 0)))
	{
	  CloseHandle(hSvcEvent);
	  if (hSvcPid) CloseHandle(hSvcPid);
	  goto cleanup;
	}

      *pSvcPid = _getpid();
      SetEvent( hSvcEvent );
      UnmapViewOfFile( pSvcPid );
      CloseHandle( hSvcPid );
      CloseHandle( hSvcEvent );
    }
  }
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
    SetEvent(hServerStopEvent);
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

STATICF DWORD WINAPI EntryPointThread(LPVOID arg)
{
  ARGAREA *parg = (ARGAREA *)arg;
  boolean rv;

  rv = ENTRY_POINT((dvoid *)parg->osdbuf, parg->argv0, parg->argc, parg->argv);

  SetEvent(hServerStopEvent);
  return( rv ? 0 : 1 );
}

/* ENABLE check_proto_decl */
