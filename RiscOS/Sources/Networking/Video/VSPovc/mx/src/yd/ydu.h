/* mx/src/yd/ydu.h */


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





#ifndef YDU_ORACLE
#define YDU_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef YSID_ORACLE
#include <ysid.h>
#endif





#define yduImplDecl(nm) CONST_DATA char nm[]

#define yduStr( s ) (s ? s : (char *)"<null>")

#define yduIdStr( ysid ) ( ysid ? ysidToStr(ysid) : (char *)"<null>")

void yduFreeCopyStr( char **dp, CONST char *s, ysmff ff, ysmaf af );

char *yduCopyCacheStr( CONST char *s );
void yduFreeCacheStr( char *s );


sword yduSafeStrcmp( CONST char *a, CONST char *b );


sword yduWildStrcmp( CONST char *a, CONST char *b );

#endif


