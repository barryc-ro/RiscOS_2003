/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysfp.c - OMX Host File System I/O
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

extern char *sys_errlist[];

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif

/*
 * SYSFPKIND_TEXT
 * We declare this is a constant because we need to be able to distinguish
 * it from all other values passed to sysfpOpen() because it gets special
 * handling.
 */
externdef CONST_DATA char SYSFPKIND_TEXT[] = "";

/*
 * sysfpOpen - open a file
 */
sysfp *sysfpOpen(CONST char *fn, CONST char *type, CONST char *kind,
		 CONST char **errtxt)
{
  FILE *fp;
  char  ftyp[32];

  if (kind != SYSFPKIND_TEXT)
    {
      DISCARD sprintf(ftyp, "%sb", type);             /* open in binary mode */
      fp = fopen(fn, ftyp);
    }
  else
    fp = fopen(fn, type);

  if (!fp)
    *errtxt = sys_errlist[errno];

  return (sysfp *) fp;
}

/*
 * sysfpClose - close a file
 */
void sysfpClose(sysfp *fp)
{
  fclose((FILE *) fp);
}

/*
 * sysfpEof - returns true if at eof
 */
boolean sysfpEof(sysfp *fp)
{
  return (feof((FILE *) fp) ? TRUE : FALSE);
}

/*
 * sysfpSeek - seek to position
 */
boolean sysfpSeek(sysfp *fp, CONST sysb8 *offset)
{
  return (fseek((FILE *) fp, sysb8msk(offset), 0) ? FALSE : TRUE);
}

/*
 * sysfpTell - tell position
 */
void sysfpTell(sysfp *fp, sysb8 *offset)
{
  sb4 off = ftell((FILE *) fp);
  sysb8ext(offset, off);
}

/*
 * sysfpRead - read from file
 */
size_t sysfpRead(sysfp *fp, dvoid *buf, size_t len)
{
  return fread(buf, 1, len, (FILE *) fp);
}

/*
 * sysfpWrite - write to file
 */
size_t sysfpWrite(sysfp *fp, dvoid *buf, size_t len)
{
  return fwrite(buf, 1, len, (FILE *) fp);
}

/*
 * sysfpFlush - flush buffer
 */
boolean sysfpFlush(sysfp *fp)
{
  return (fflush((FILE *) fp) == 0 ? TRUE : FALSE);
}

/*
 * sysfpPrint - print to file
 */
boolean sysfpPrint(sysfp *fp, CONST char *fmt, ...)
{
  va_list ap;
  boolean ok;

  va_start(ap, fmt);
  ok = (vfprintf((FILE *) fp, fmt, ap) != EOF);
  va_end(ap);

  return ok;
} 

/*
 * sysfpAccess - tests for access to a file
 */
boolean sysfpAccess(CONST char *fn, CONST char *type)
{
  boolean readon;
  char    path[SYSFP_MAX_PATHLEN];

  readon = !strcmp(type, "r");

  if (!access(fn, (readon ? R_OK : W_OK)))
    return TRUE;

  if (errno == ENOENT && (*type == 'w' || *type == 'a'))
    {
      sysfpExtractPath(path, fn);
      if (!access(path, W_OK))
	return TRUE;
    }

  return FALSE;
}

/*
 * sysfpIsDir - tests for a directory
 */
boolean sysfpIsDir(CONST char *fn)
{
  struct stat buf;
  if (stat(fn, &buf))
    return FALSE;
  else
    return S_ISDIR(buf.st_mode);
}

/*
 * sysfpGetDir - get directory
 */
yslst *sysfpGetDir(CONST char *fn)
{
  DIR *dp;
  struct dirent *dep;
  yslst *noreg lst;

  dp = opendir(fn);
  if (!dp)
    return (yslst *) 0;

  lst = (yslst *) 0;
  yseTry
    {
      lst = ysLstCreate();

      while (dep = readdir(dp))
	ysLstEnq(lst, (dvoid *) ysStrDup(dep->d_name));

      DISCARD closedir(dp);
    }
  yseCatch(YS_EX_OUTMEM)
    if (lst)
      {
	ysLstDestroy(lst, ysmFGlbFree);
	lst = (yslst *) 0;
      }
  yseEnd

  return lst;
}

/*
 * sysfpSize - return the length of a file
 */
CONST char *sysfpSize(CONST char *fn, sysb8 *sz)
{
  struct stat buf;
  if (stat(fn, &buf))
    return sys_errlist[errno];
  else
    {
      sysb8ext(sz, buf.st_size);
      return (char *) 0;
    }
}

/*
 * sysfpLast - return last modification time of a file
 */
CONST char *sysfpLast(CONST char *fn, sysb8 *mtime)
{
  struct stat buf;
  if (stat(fn, &buf))
    return sys_errlist[errno];
  else
    {
      sysb8ext(mtime, buf.st_mtime);
      return (char *) 0;
    }
}

/*
 * sysfpRemove - remove a file
 */
CONST char *sysfpRemove(CONST char *fn)
{
  if (unlink(fn))
    return sys_errlist[errno];
  else
    return (char *) 0;
}

/*
 * sysfpRename - rename a file
 */
CONST char *sysfpRename(CONST char *from, CONST char *to)
{
  if (rename(from, to))
    return sys_errlist[errno];
  else
    return (char *) 0;
}

/*
 * sysfpForm - construct a filename
 */
void sysfpForm(char *result, CONST char *path, CONST char *base,
	       CONST char *sfx)
{
  CONST char *s;
  size_t      len;

  /* prepend the path */
  if (*base == '/' || !path)
    DISCARD strcpy(result, base);
  else
    {
      len = strlen(path);
      DISCARD strcpy(result, path);

      if (path[len-1] != '/')
	{
	  result[len++] = '/';
	  result[len] = '\0';
	}

      DISCARD strcat(result, base);
    }

  /* append the suffix */
  if (sfx && strlen(sfx) > (size_t) 0)
    {
      /* check for existing suffix */
      for (s = base + strlen(base); s > base && *s != '.'; s--) ;

      if (*s != '.' || strcmp(s + 1, sfx))
	{
	  DISCARD strcat(result, ".");
	  DISCARD strcat(result, sfx);
	}
    }
}

/*
 * sysfpExtractPath - extract pathname
 */
void sysfpExtractPath(char *path, CONST char *fn)
{
  CONST  char *s;
  size_t len;

  for (s = fn + strlen(fn); s > fn && *s != '/'; s--) ;
  if (s == fn)
    DISCARD strcpy(path, ".");
  else
    {
      len = min(s - fn, SYSFP_MAX_PATHLEN - 1);
      DISCARD memcpy(path, fn, len);
      path[len] = '\0';
    }
}

/*
 * sysfpExtractFile - extract filename
 */
void sysfpExtractFile(char *file, CONST char *fn)
{
  CONST  char *s;
  size_t len;

  for (s = fn + strlen(fn); s > fn && *s != '/'; s--) ;
  DISCARD strcpy(file, (*s == '/' ? s + 1 : s));
}

/*
 * sysfpExtractBase - extract basename
 */
void sysfpExtractBase(char *base, CONST char *fn)
{
  char *s;

  sysfpExtractFile(base, fn);

  for (s = base + strlen(base); s > base && *s != '.'; s--) ;
  if (*s == '.')
    *s = '\0';
}

/*
 * sysfpExtractSuffix - extract suffix
 */
void sysfpExtractSuffix(char *sfx, CONST char *fn)
{
  CONST char *s;

  for (s = fn + strlen(fn); s > fn && *s != '.'; s--) ;
  if (*s == '.')
    DISCARD strcpy(sfx, s + 1);
  else
    DISCARD strcpy(sfx, "");
}

/* 
 * sysfpTemp - make temporary file name (including path and suffix).
 * NOTE: OSD file, so writeable static data permitted.
 */
static ub2 sysfpSeq = (ub2)0;

void sysfpTemp(char *result, CONST char *path, CONST char *sfx)
{
  char base[16];
  
  DISCARD sprintf(base, "tmp%04x%04x", getpid(), sysfpSeq++);
  sysfpForm(result, path, base, sfx);
}

/*
 * sysfpGetCwd - get current working directory
 */
CONST char *sysfpGetCwd(char *result)
{
  if (!getcwd(result, SYSFP_MAX_PATHLEN))
    return sys_errlist[errno];
  else
    return (char *)0;
}
