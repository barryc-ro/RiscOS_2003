/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * syssp.c - OMX Spawning Package
 */

#include <sys/types.h>
#include <io.h>
#include <time.h>

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSSP_ORACLE
#include <syssp.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSHSH_ORACLE
#include <yshsh.h>
#endif
#ifndef SYSSVCH_ORACLE
#include <syssvc.h>
#endif
#ifndef SSYSIH_ORACLE
#include <ssysi.h>
#endif


/*
 * Private Declarations
 */
struct sysspProcSts
{
  HANDLE phd;					     /* win32 process handle */
  int    pid;						/* win32 process pid */
  sword  sts;
};
typedef struct sysspProcSts sysspProcSts;

/*
 * Declarations for signal handler
 */

static yshsh *sysspcx = (yshsh *) 0;

/*
 * Private Functions
 */
STATICF void sysspArgv2Argl(int argc, char **argv, char *argl);
STATICF CONST char *sysspMapErr( sword sts );
STATICF BOOL IsInteractive( void );
STATICF int GetPlatformId( void );

/*
 * sysspInit - initialize spawning package
 */

sysspctx *sysspInit(dvoid *osdp)
{
  if (sysspcx != (yshsh *) 0)
    return ((sysspctx *) sysspcx);

  /* create hash table */
  sysspcx = ysHshCreate(16, (yshash) 0, (yshsheq) 0, ysmFGlbFree);

  srand((unsigned)time( NULL ));

  return (sysspctx *) sysspcx;
}

/*
 * sysspTerm - terminate spawning package
 */
void sysspTerm(sysspctx *ctx)
{

  if (!sysspcx)
    return;

  /* destroy hash tree */
  ysHshDestroy(sysspcx);
}

/*
 * sysspSpawn - single-process spawn
 */
sword sysspSpawn(sysspctx *ctx, char *strpid, size_t len,
		 CONST char *path, sword argc, char **argv)
{
  sword sts;
  sysspProcSts *psts;
  char  argl[256];					    /* argument line */
  char  cmdl[256];					     /* command line */
  STARTUPINFO         startInfo;
  PROCESS_INFORMATION procInfo;
  SC_HANDLE schSCManager = NULL;
  HANDLE hSvcEvent = NULL, hSvcPid = NULL;
  int *pSvcPid = NULL;
  char  svcPidName[256];
  
  if (!sysspcx)
    return SYSSP_STS_FAILURE;

  sts = SYSSP_STS_STARTED;

  /* allocate space for descriptor */
  psts = (sysspProcSts *) ysmGlbAlloc(sizeof(sysspProcSts), "procsts");

  if (GetPlatformId() == VER_PLATFORM_WIN32_NT && !IsInteractive())
  {
    /* this process is a service, so start up the child process also as a
       service */
    char  svcName[256];           /* service name */
    char  subkey[MAX_PATH];
    char  fname[_MAX_FNAME+_MAX_EXT];
    char  fext[_MAX_EXT];
    HKEY hk;
    DWORD disp;

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
      goto spawnerr;

    /* form service name */
    strcpy(svcName, SSYSI_SVC_PREFIX);
    strcat(svcName, "_");
    _splitpath( path, NULL, NULL, fname, fext );
    strcat(svcName, fname);

    /* test for file existence */
    if (fext[0])
      strcat(fname, fext);
    else
      strcat(fname, ".exe"); 
    if (!SearchPath( NULL, fname, NULL, MAX_PATH, subkey, NULL ))
      goto spawnerr;

    sprintf(svcPidName, "E_SvcPid%d", rand());

    /* create the status area and event */
    hSvcEvent = CreateEvent( NULL, TRUE, FALSE, svcPidName);
    if (hSvcEvent == NULL)
      goto spawnerr;
    
    svcPidName[0] = 'S';

    hSvcPid = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE,
                                   0, sizeof(INT), svcPidName);
    if (hSvcPid != NULL)
      pSvcPid = (INT *)MapViewOfFile( hSvcPid, FILE_MAP_READ|FILE_MAP_WRITE,
                                                  0, 0, 0);

    if (pSvcPid == NULL)
      goto spawnerr;
 
    /* now update the 'Arguments' key in the registry */
    sysspArgv2Argl(argc, argv, argl);
    strcpy(subkey, SSYSI_SVC_PARMS_PREFIX);
    strcat(subkey, svcName);

    if ((RegCreateKeyEx( HKEY_LOCAL_MACHINE, subkey, 0, NULL, 0, KEY_ALL_ACCESS,
                        NULL, &hk, &disp) == ERROR_SUCCESS) ||
        (disp == REG_OPENED_EXISTING_KEY))
    {
      /* we do not care for the return value here */
      RegSetValueEx(hk, SSYSI_SVC_ARGVAL, 0, REG_SZ, argl, strlen(argl)+1);
      RegFlushKey(hk);
      RegCloseKey(hk);
    }

    argc = 2;
    strcpy(cmdl, fname);
    argv[0] = cmdl;
    argv[1] = svcPidName;

    if (sysStartServ(schSCManager, svcName, argc, argv) <0)
    {
      if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) 
      {
        if (sysCreateServ(schSCManager, argv[0], NULL, svcName) < 0 ||
            sysStartServ(schSCManager, svcName, argc, argv) < 0) 
          goto spawnerr;
      }
      else
        goto spawnerr;
    }

    /* Open status area */
    if (WaitForSingleObject(hSvcEvent, 10000) != WAIT_OBJECT_0)
      goto spawnerr;

    CLRSTRUCT(*psts);
    psts->phd = 0;       /* do we need this */
    psts->pid = *pSvcPid;
    psts->sts = SYSSP_STS_RUNNING;
  }
  else
  {
    sysspArgv2Argl(argc, argv, argl);
    strcpy(cmdl, path);
    strcat(cmdl, " ");
    strcat(cmdl, argl);

    GetStartupInfo(&startInfo);
    if (TRUE == CreateProcess(NULL, cmdl, NULL, NULL, TRUE,
			    DETACHED_PROCESS | NORMAL_PRIORITY_CLASS,
			    (LPVOID) NULL, NULL, &startInfo, &procInfo) == TRUE)
    {
      DISCARD sprintf(strpid, "%d", procInfo.dwProcessId);
      psts->phd = procInfo.hProcess;
      psts->pid = procInfo.dwProcessId;
      psts->sts = SYSSP_STS_RUNNING;
      ysHshIns(sysspcx, &psts->pid, sizeof(HANDLE), (dvoid *) psts);
    }
    else
    {
      goto spawnerr;
    }
  }
  UnmapViewOfFile(pSvcPid);
  CloseHandle(hSvcPid);
  CloseHandle(hSvcEvent);
  CloseHandle(schSCManager);
  return sts;

spawnerr:
  sts = GetLastError();
  yslError("sysspSpawn: path %s: error %d %s\n",
           path, sts, sysspMapErr(sts)  );
  sts = SYSSP_STS_FAILURE;

  if (pSvcPid) UnmapViewOfFile(pSvcPid);
  if (hSvcPid) CloseHandle(hSvcPid);
  if (hSvcEvent) CloseHandle(hSvcEvent);
  if (schSCManager) CloseHandle(schSCManager);

  ysmGlbFree( psts );
  return sts;
}

/*
 * sysspPSpawn - parallel spawn
 *   This implementation assumes the affinity set is simply an integer
 *   specifying the number of children to start.
 */
sword sysspPSpawn(sysspctx *ctx, yslst **pids,
		  CONST char *path, CONST char *affinity_set,
		  sword argc, char **argv)
{
  sword cnt, sts;
  char *spid, strpid[64];

  cnt = atoi(affinity_set);
  if (!cnt)
    return SYSSP_STS_FAILURE;

  /* spawn all the processes */
  *pids = ysLstCreate();
  sts = SYSSP_STS_STARTED;
  while (cnt && sts == SYSSP_STS_STARTED)
    {
      sts = sysspSpawn(ctx, strpid, sizeof(strpid), path, argc, argv);
      if (sts == SYSSP_STS_STARTED)
	DISCARD ysLstEnq(*pids, (dvoid *) ysStrDup(strpid));
      cnt--;
    }

  /* if failure occurred, kill any started processes */
  if (sts == SYSSP_STS_FAILURE)
    {
      while (spid = ysLstDeq(*pids))
	{
	  DISCARD sysspKill(ctx, spid, TRUE);
	  sysspForget(ctx, spid);
	  ysmGlbFree((dvoid *) spid);
	}

      ysLstDestroy(*pids, (ysmff) 0);
    }

  return sts;
}

/*
 * sysspMakeSet - make an affinity set
 */
boolean sysspMakeSet(sysspctx *ctx, sword cnt, char *buf, size_t len)
{
  DISCARD sprintf(buf, "%d", (int)(cnt ? cnt : (sword)4));
  return TRUE;
}

/*
 * sysspCountSet - count an affinity set
 */
sword sysspCountSet(sysspctx *ctx, CONST char *affinity_set)
{
  sword ncnt;

  ncnt = -1;
  sscanf(affinity_set, "%d", &ncnt);
  return ncnt;
}

/*
 * sysspGetStatus - get process status
 */
sword sysspGetStatus(sysspctx *ctx, CONST char *strpid)
{
  int pid;
  sysspProcSts *psts;

  pid = (int) atol(strpid);
  psts = (sysspProcSts *) ysHshFind((yshsh *) ctx, &pid, sizeof(int));
  if (psts)
    return psts->sts;
  else
    {
      if (OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid) != NULL)
	return SYSSP_STS_ALIVE;
      else
	return SYSSP_STS_NOTFOUND;
    }
}

/*
 * sysspForget - forget the status of a process
 */
void sysspForget(sysspctx *ctx, CONST char *strpid)
{
  int    pid;
  dvoid *psts;

  pid = (int) atol(strpid);
  psts = ysHshRem((yshsh *) ctx, &pid, sizeof(int));
  if (psts)
    ysmGlbFree(psts);
}

/*
 * sysspKill - kill a process
 */
sword sysspKill(sysspctx *ctx, CONST char *strpid, boolean nice)
{
  sb4   pid;
  sword sts;
  sysspProcSts *psts;
  HANDLE phd = 0;
  
  pid = atol(strpid);
  psts = (sysspProcSts *) ysHshFind((yshsh *) ctx, &pid, sizeof(int));

  if ((phd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)) != NULL)
    {
      if (phd && (TerminateProcess(phd, 1) == TRUE))
        sts = SYSSP_STS_EXITOK;
      else
        sts = SYSSP_STS_EXITNOTOK;
    }
  else
    sts = SYSSP_STS_NOTFOUND;
  
  if (psts)
    psts->sts = sts;

  return sts;
}


/*
 * sysspArgv2Argl - convert argument vector to argument line
 */
STATICF void sysspArgv2Argl(int argc, char **argv, char *argl)
{
  int i = 0;

  argl[0] = '\0';
  while ((i < argc) && argv[i])
    {
      strcat(argl, argv[i++]);
      strcat(argl, " ");
    }
}


STATICF CONST char *sysspMapErr( sword sts )
{
  CONST char *rv;
  switch( sts )
  {
  case ERROR_SUCCESS:		rv = "SUCCESS"; break;
  case ERROR_INVALID_FUNCTION:	rv = "INVALID_FUNCTION"; break;
  case ERROR_FILE_NOT_FOUND:	rv = "FILE_NOT_FOUND"; break;
  case ERROR_PATH_NOT_FOUND:	rv = "PATH_NOT_FOUND"; break;
  case ERROR_ACCESS_DENIED:	rv = "ACCESS_DENIED"; break;
  case ERROR_NOT_ENOUGH_MEMORY:	rv = "NOT_ENOUGH_MEMORY"; break;
  default:  rv = "<unknown>";
  }
  return rv;
}

/*
  BOOL IsInteractive(void)

  returns TRUE if user is interactive
	  FALSE if user is not interactive
*/
STATICF BOOL IsInteractive( void )
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
STATICF int GetPlatformId( void )
{
  OSVERSIONINFO osverinfo;

  osverinfo.dwOSVersionInfoSize = sizeof (osverinfo);
  if (GetVersionEx(&osverinfo))
    return (osverinfo.dwPlatformId);
  else
    return 0;
}
