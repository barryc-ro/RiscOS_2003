/* Copyright (c) Oracle Corporation 1995.  All Rights Reserved. */

/*
  NAME
    smnjoin.h
  DESCRIPTION
    OSD part of mnjoin operation -- knows nios and the like
  PUBLIC FUNCTIONS
    <x>
  NOTES
    <x>
  MODIFIED   (MM/DD/YY)
    dbrower   06/28/95 -  created.
*/

#ifndef SMNJOIN_ORACLE
#define SMNJOIN_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef MNI_ORACLE
#include <mni.h>
#endif

/* PUBLIC TYPES AND CONSTANTS */

/* PUBLIC FUNCTIONS */

/* ---------------------------- ssmnjLink ---------------------------- */
/*
  NAME
    smnjLink
  DESCRIPTION
    given a gtwy addr string, open the nip and fill in the pa.
  PARAMETERS
    gtwy	-- the gateway addr.
    pa		-- the physical address for it [OUT]
    nio		-- the nio opened for it [OUT]
  RETURNS
    none.
*/

void smnjLink(char *gtwy, mnnpa *pa, mnnio **nio);

#endif

