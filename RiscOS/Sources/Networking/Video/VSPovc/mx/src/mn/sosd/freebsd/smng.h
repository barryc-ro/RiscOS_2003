/* Copyright (c) Oracle Corporation 1995.  All Rights Reserved. */

/*
  NAME
    smng.h
  DESCRIPTION
    OSD media-net initialization for gateways.  Initializes
    special NIOs.
  PUBLIC FUNCTIONS
    smngInit
  MODIFIED   (MM/DD/YY)
    dbrower   06/27/95 -  created.
*/

#ifndef SMNG_ORACLE
#define SMNG_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef MN_ORACLE
#include <mn.h>
#endif

/* PUBLIC TYPES AND CONSTANTS */

/* PUBLIC FUNCTIONS */

/* ---------------------------- smngInit ---------------------------- */
boolean smngInit( void *osdCtx, mnLogger logger );
/*
  NAME
    smngInit
  DESCRIPTION
    Do media net startup; called after YSR database loaded.
  PARAMETERS
    osdCtx	-- osd context provided in the call to the entry point.
    logger	-- pointer to an mnLogger function to use, or NULL
		   to use a default logger.
  RETURNS
    TRUE if all gateways were sucessfully opened and MN is
    ready to use.
*/

#endif


