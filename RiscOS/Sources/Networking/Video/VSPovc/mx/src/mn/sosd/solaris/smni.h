/* Copyright (c) Oracle Corporation 1995.  All Rights Reserved. */

/*
  NAME
    smni.h
  DESCRIPTION
    OSD media-net initialization for regular "host" programs; not
    for gateways that need special stuff.
  PUBLIC FUNCTIONS
    smniInit
  MODIFIED   (MM/DD/YY)
    dbrower   06/27/95 -  created.
*/

#ifndef SMNI_ORACLE
#define SMNI_ORACLE

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif

#ifndef MN_ORACLE
#include <mn.h>
#endif

EXTC_START

/* PUBLIC TYPES AND CONSTANTS */

/* PUBLIC FUNCTIONS */

/* ---------------------------- smniInit ---------------------------- */
boolean smniInit( void *osdCtx, mnLogger logger );
/*
  NAME
    smniInit
  DESCRIPTION
    Do media net startup; called after YSR database loaded.
  PARAMETERS
    osdCtx	-- osd context provided in the call to the entry point.
    logger	-- pointer to an mnLogger function to use, or NULL
		   to use a default logger.
  RETURNS
    TRUE if at least one gateway was sucessfully opened and MN is
    ready to use.
*/

EXTC_END
#endif


