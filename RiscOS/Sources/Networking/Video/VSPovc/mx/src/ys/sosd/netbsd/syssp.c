/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * syssp.c - OMX Spawning Package
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

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

/*
 * Private Declarations
 */
struct sysspProcSts
{
  pid_t pid;
  sword sts;
};
typedef struct sysspProcSts sysspProcSts;

/*
 * Declarations for signal handler
 */
STATICF void sysspChildHandler();
static yshsh *sysspcx = (yshsh *) 0;

/*
 * sysspInit - initialize spawning package
 */
sysspctx *sysspInit(dvoid *osdp)
{
  struct sigaction act;

  if (sysspcx != (yshsh *) 0)
    return ((sysspctx *) sysspcx);

  /* create hash table */
  sysspcx = ysHshCreate(16, (yshash) 0, (yshsheq) 0, ysmFGlbFree);

  /* install signal handler */
  DISCARD memset(&act, 0, sizeof(act));
  act.sa_handler = sysspChildHandler;
  act.sa_flags = SA_NOCLDSTOP;
  DISCARD sigaction(SIGCHLD, &act, (struct sigaction *) 0);

  return (sysspctx *) sysspcx;
}

/*
 * sysspTerm - terminate spawning package
 */
void sysspTerm(sysspctx *ctx)
{
  struct sigaction act;

  if (!sysspcx)
    return;

  /* de-install signal handler */
  DISCARD memset(&act, 0, sizeof(act));
  act.sa_handler = SIG_DFL;
  DISCARD sigaction(SIGCHLD, &act, (struct sigaction *) 0);

  /* destroy hash tree */
  ysHshDestroy(sysspcx);
}

/*
 * sysspSpawn - single-process spawn
 */
sword sysspSpawn(sysspctx *ctx, char *strpid, size_t len,
		 CONST char *path, sword argc, char **argv)
{
  pid_t pid;
  sword sts;
  char **nargv;
  sysspProcSts *psts;

  if (!sysspcx)
    return SYSSP_STS_FAILURE;

  sts = SYSSP_STS_STARTED;

  /* allocate space for descriptor */
  psts = (sysspProcSts *) ysmGlbAlloc(sizeof(sysspProcSts), "procsts");

  /* build proper argument list */
  nargv = (char **) ysmGlbAlloc(sizeof(char *) * (argc + 2), "temp");
  nargv[0] = (char *) path;
  DISCARD memcpy(nargv + 1, argv, sizeof(char *) * argc);
  nargv[argc + 1] = (char *) 0;
      
  /* fork the process */
  pid = fork();
  if (pid == (pid_t) -1)                                          /* failure */
    sts = SYSSP_STS_FAILURE;
  else if (pid == (pid_t) 0)                                /* child process */
    {
      execvp(path, nargv);

      /* if execvp fails, we end up here; make termination ugly so that the
       * parent detects the exit status of the child is "not ok"
       */
      kill(getpid(), SIGKILL);
    }
  else                                                     /* parent process */
    {
      DISCARD sprintf(strpid, "%d", pid);
      psts->pid = pid;
      psts->sts = SYSSP_STS_RUNNING;
      ysHshIns(sysspcx, &psts->pid, sizeof(pid_t), (dvoid *) psts);
    }

  ysmGlbFree((dvoid *) nargv);
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
      while ((spid = ysLstDeq(*pids)))
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
  if (!cnt)
    cnt = 2;

  DISCARD sprintf(buf, "%d", cnt);
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
  pid_t pid;
  sysspProcSts *psts;

  pid = (pid_t) atol(strpid);
  psts = (sysspProcSts *) ysHshFind((yshsh *) ctx, &pid, sizeof(pid_t));
  if (psts)
    return psts->sts;
  else
    {
      if (kill(pid, 0) == 0)
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
  pid_t pid;
  dvoid *psts;

  pid = (pid_t) atol(strpid);
  psts = ysHshRem((yshsh *) ctx, &pid, sizeof(pid_t));
  if (psts)
    ysmGlbFree(psts);
}

/*
 * sysspKill - kill a process
 */
sword sysspKill(sysspctx *ctx, CONST char *strpid, boolean nice)
{
  pid_t pid;
  sword sts;
  sysspProcSts *psts;

  pid = atol(strpid);
  psts = (sysspProcSts *) ysHshFind((yshsh *) ctx, &pid, sizeof(pid_t));
  if (kill(pid, (nice ? SIGTERM : SIGKILL)) == 0)
    sts = (nice ? SYSSP_STS_EXITOK : SYSSP_STS_EXITNOTOK);
  else
    sts = (errno == ESRCH ? SYSSP_STS_NOTFOUND : SYSSP_STS_ALIVE);

  if (psts)
    psts->sts = sts;

  return sts;
}

/*
 * sysspChildHandler - signal handling function for child death
 */
STATICF void sysspChildHandler()
{
  sysspProcSts *psts;
  pid_t pid;
  int stat;

  /* If we can successfully get the child's status, look for it in the hash
   * table.  If we find it, update the status.
   */
  if ((pid = wait(&stat)) > 0 && 
      (psts = (sysspProcSts *)ysHshFind(sysspcx, &pid, sizeof(pid_t))))
    psts->sts = (stat ? SYSSP_STS_EXITNOTOK : SYSSP_STS_EXITOK);
}
