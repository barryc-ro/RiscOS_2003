/* mx/src/yd/ydu.c */


/*
ORACLE, Copyright (c) 1982, 1983, 1986, 1990 ORACLE Corporation
ORACLE Utilities, Copyright (c) 1981, 1982, 1983, 1986, 1990, 1991 ORACLE Corp

Restricted Rights
This program is an unpublished work under the Copyright Act of the
United States and is subject to the terms and conditions stated in
your  license  agreement  with  ORACORP  including  retrictions on
use, duplication, and disclosure.

Certain uncopyrighted ideas and concepts are also contained herein.
These are trade secrets of ORACORP and cannot be  used  except  in
accordance with the written permission of ORACLE Corporation.
*/




#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YOTK_ORACLE
#include <yotk.h>
#endif







sword yduSafeStrcmp( CONST char *a, CONST char *b )
{
  sword rv;
  if( !a )
  {
    rv = b ? -1 : 0;
  }
  else if ( !b )
  {
    rv = 1;
  }
  else				
  {
    rv = (*a == *b) ? (sword)strcmp( a, b ) : (*a - *b); 
  }
  return rv;
}

sword yduWildStrcmp( register CONST char *a, register CONST char *b )
{
  sword rv;

  if( !a || !b )
    rv = 0;
  else if (*a == *b)
    rv = (sword)strcmp( a, b );
  else
    rv = *a - *b; 
  return rv;
}

void yduFreeCopyStr( char **dp, CONST char *s, ysmff ff, ysmaf af )
{
  if( *dp )
  {
    if(ff)
    {
      if( ff != yotkFreeStr )
	(*ff)((dvoid*)*dp);
      else
	yotkFreeVal( yoTcString, (dvoid*)dp, yotkFreeStr );
    }
    else ysmGlbFree((dvoid*)*dp); 
    *dp = (char*)0;
  }
  if( s )
  {
    if( af != yotkAllocStr )
      *dp = ysStrDupWaf( s, af );
    else
      yotkCopyVal( yoTcString, (dvoid*)dp, (dvoid*)&s, yotkAllocStr );
  }
}


char *yduCopyCacheStr( CONST char *s )
{
  char *rv = (char*)s;

  if( s )
    yotkCopyVal( yoTcString, (dvoid*)&rv, (dvoid*)&s, yotkAllocStr );  

  return rv;
}

void yduFreeCacheStr( char *s )
{
  yotkFreeVal( yoTcString, (dvoid*)&s, yotkFreeStr );
}



