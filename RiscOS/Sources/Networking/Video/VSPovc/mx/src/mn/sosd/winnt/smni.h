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

/* PUBLIC TYPES AND CONSTANTS */

/* PUBLIC FUNCTIONS */

/* ---------------------------- smniInit ---------------------------- */
boolean smniInit( void *osdCtx, mnLogger logger );
boolean ssmniInit( void *osdCtx, mnLogger logger, CONST char *omnaddr );
void    ssmniTerm( void );
/*
  NAME
    smniInit
  DESCRIPTION
    Do media net startup; called after YSR database loaded.
  PARAMETERS
    osdCtx	-- osd context provided in the call to the entry point.
    logger	-- pointer to an mnLogger function to use, or NULL
		   to use a default logger.
    omnaddr     -- specific omn gateway address, can be NULL
  RETURNS
    TRUE if at least one gateway was sucessfully opened and MN is
    ready to use.
  NOTES (winnt version)
    smniInit() calls ssmniInit() with omn_addr set to NULL, that means
    omn_addr will be looked from YSR or Windows registry. The ssmniInit()
    calls ssysiInit() to ensure YS layer is initiated.
*/

#endif


