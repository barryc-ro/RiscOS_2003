/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * NAME
 * mnjoin.c - Media Net Address Server Join Utility
 */

#ifndef SMNJ_ORACLE
#include <smnj.h>
#endif

#ifndef SMNUDP_ORACLE
#include <smnudp.h>
#endif

#ifndef MTL_ORACLE
#include <mtl.h>
#endif

void smnjLink(char *gtwy, mnnpa *pa, mnnio **nio)
{
  if (!strncmp(gtwy, "UDP:", 4))
    {
      if (smnudpPa(pa, gtwy))
	mtlExit("physical address %s is invalid", gtwy);

      if (nio)
	{
	  *nio = smnudpOpen((char *) 0, FALSE);
	  if (!*nio)
	    mtlExit("error opening %s", gtwy);
	}
    }
}
