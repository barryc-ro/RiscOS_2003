/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysb8.c - Portable 64-but Signed Integer Data Type Implementation
 *
 * DESCRIPTION
 * This file need exist only for NATIVE implemenations.  It's sole purpose
 * is to define the useful sysb8zero value.  It may also define other routines
 * that the native port doesn't handle.
 */

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif
#ifndef YSB8_ORACLE
#include <ysb8.h>
#endif

#ifdef SYSB8_NATIVE

/*
 * Definition of zero
 */
static    CONST sysb8 sysb8zeroval = 0;
externdef CONST sysb8 *CONST_W_PTR sysb8zero = &sysb8zeroval;

/*
 * sysb8fmt - format a 64-bit value
 *   not supported by printf(), so we convert to a ysb8 structure and invoke
 *   ysb8fmt() explicitly.
 */
char *sysb8fmt(char *buf, CONST char *fmt, sysb8 *val)
{
  ysb8 x;

  x.lo = (ub4) *val;
  x.hi = (sb4) (*val >> 32);
  return ysb8fmt(buf, fmt, &x);
}

/*
 * sysb8fromstr - scan a string into a 64-bit value
 *   not supported by scanf(), so we invoke ysb8fromstr() explicitly and
 *   convert from a ysb8 structure.
 */
boolean sysb8fromstr(sysb8 *out, CONST char *str)
{
  ysb8 x;
  boolean ok;

  if (ok = ysb8fromstr(&x, str))
    *out = (((sb8) x.hi) << 32) + x.lo;

  return ok;
}

#endif /* SYSB8_NATIVE */
