/* mx/src/ye/yeu.c */


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

#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef MN_ORACLE
#include <mn.h>
#endif





sword yeuSafeStrcmp( CONST char *a, CONST char *b )
{
  sword rv;
  if( !a && !b )
    rv = 0;
  else if ( !a && b )
    rv = -1;
  else if ( a && !b )
    rv = 1;
  else
    rv = (sword)strcmp( a, b );
  return rv;
}

