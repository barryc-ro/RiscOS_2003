/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysx.c - MOOSE system-dependent eXternal implementation
 */
 
#ifndef SYSX_ORACLE
#include <sysx.h>
#endif

dvoid *syscGetPG()
{
  return syscPG;
}

