
/* Copyright (c) 1996 by Oracle Corporation.  All rights reserved. */

/*
NAME
  syssvc.c - functions for creating/starting... services
DESCRIPTIONS

PUBLIC FUNCTIONS
INT sysCreateServ(schSCManager, lpszExeName, lpszArg, lpszServName)
INT sysDestroyServ(schSCManager, lpszServName)
INT sysStartServ(schSCManager, lpszServName, argc, argv[])
INT sysStopServ(schSCManager, lpszServName)

NOTES

MODIFIED

*/
#include <sys/types.h>

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#if 0
#ifndef SYSSVCH_ORACLE
#include <syssvc.h>
#endif
#endif

INT sysCreateServ(SC_HANDLE schSCManager, LPCSTR lpszExeName, 
		       LPCSTR lpszArg, LPCSTR lpszServName)
{
  SC_HANDLE schService;
  LPCSTR    lpszServCmdLine;
  char	    lpszServCmdLineBuf[256];

  (void) strncpy(lpszServCmdLineBuf, lpszExeName, 256);

  if (lpszArg) {
    (void) strncat(lpszServCmdLineBuf, " ", 256);
    (void) strncat(lpszServCmdLineBuf, lpszArg, 256);
  }
  lpszServCmdLine = lpszServCmdLineBuf;

  schService = CreateService(schSCManager, lpszServName, lpszServName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			lpszServCmdLine, NULL, NULL, NULL, NULL, NULL);

  if (schService == NULL) 
    return -1;

  return 0;
}

INT sysDestroyServ(SC_HANDLE schSCManager, LPCSTR lpszServName)
{
  SC_HANDLE schService;
  
  schService = OpenService(schSCManager, lpszServName, DELETE);

  if (schService == NULL) 
    return -1;

  if ( ! DeleteService(schService) ) 
    return -1;

  return 0;
}

INT sysStartServ(SC_HANDLE schSCManager, LPCSTR lpszServName, 
	      INT argc, LPTSTR argv[])
{
  SERVICE_STATUS ssStatus;
  SC_HANDLE  	 schService;
  DWORD 	 dwOldCheckPoint;

  schService = OpenService(schSCManager, lpszServName, SERVICE_ALL_ACCESS);
  if (schService == NULL) 
    return -1;

  if ( !StartService(schService, argc, argv) ) 
    return -1;

  if (!QueryServiceStatus(schService, &ssStatus)) 
    return -1;
  
  while (ssStatus.dwCurrentState != SERVICE_RUNNING) {
    dwOldCheckPoint = ssStatus.dwCheckPoint;
    Sleep(ssStatus.dwWaitHint);
    if (!QueryServiceStatus(schService, &ssStatus)) 
      break;
    if (dwOldCheckPoint >= ssStatus.dwCheckPoint)
      break;
  }
    
  if (ssStatus.dwCurrentState != SERVICE_RUNNING)
    return -1;

  return 0;
}

INT sysStopServ(SC_HANDLE schSCManager, LPCSTR lpszServName)
{
  SERVICE_STATUS ssStatus;
  SC_HANDLE      schService;

  schService = OpenService(schSCManager, lpszServName, SERVICE_STOP);
  if (schService == NULL) 
    return -1;

  if ( !ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus) ) 
    return -1;

  return 0;
}
