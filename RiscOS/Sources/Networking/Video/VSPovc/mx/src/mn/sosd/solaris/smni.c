/* Copyright (c) Oracle Corporation 1995.  All Rights Reserved. */

/*
  NAME
    smni.c
  DESCRIPTION
    OSD media-net initialization for regular "host" programs; not
    for gateways that need special stuff.
  PUBLIC FUNCTIONS
    smniInit
  PRIVATE FUNCTIONS
    smniInitNios
    smniNioInit
    smniPostIntr
    smniIntrHandler
    smniAbort
    smniMalloc
  MODIFIED   (MM/DD/YY)
    tsepez    02/05/95 -  remove dependence on mtpa.h
    dbrower   06/27/95 -  created.
*/

#include <signal.h>

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSLST_ORACLE
#include <yslst.h>
#endif

#ifndef MN_ORACLE
#include <mn.h>
#endif
#ifndef MNNIO_ORACLE
#include <mnnio.h>
#endif

#ifndef SMNUDP_ORACLE
#include <smnudp.h>
#endif

#ifndef SMNI_ORACLE
#include <smni.h>
#endif

STATICF boolean smniInitNios( yslst *syncList, yslst *asyncList,
			     ub4 *num, mnnpa *pav, mnnio **niov );

STATICF boolean smniNioInit( char *gw, boolean intr, mnnpa *pa, mnnio **nio );

STATICF void smniPostIntr(int sig);
STATICF void smniIntrHandler(dvoid *usrp, sb4 val);
STATICF sb4 smniAbort(dvoid *usrp);

STATICF dvoid *smniMalloc( size_t size );
STATICF void smniLog(CONST char *msg);

#define smniFree ysmFGlbFree

/* maintain refcnt for smniInit */
static ub4 smniInitCnt = 0;

/* ---------------------------- smniInit ---------------------------- */
/*
  NAME
    smniInit
  DESCRIPTION
    Function called from portable main program entry point to initialize
    Media Net.  The Media Net parameters are taken from the YSR resource
    database, so it must be set up completely before this is called.

    There may be errors reported that are not returned to the caller.
    
  PARAMETERS
    osdCtx	-- the osd context from the call to the entry point.
  RETURNS
    TRUE if at least one gateway has been opened into the
    Media Net, and FALSE if there is no gateway opened.
*/

/* ARGSUSED */
boolean smniInit( void *osdCtx, mnLogger logger )
{
  boolean rv = FALSE;		/* assume the worst */
  char	*traceRes;		/* mn.trace resource value */
  char	*heapRes;		/* mn.heapsize resource value */
  ub4	heapsize;		/* actual heapsize, or 0 */
  mnbv	bv;
  mnbv	*bvp;
  ub4	nbv;

  char	*omn_addr;		/* OMN_ADDR value if no gateways speced. */
  yslst	*syncList;		/* list of sync gateway addrs */
  yslst	*asyncList;		/* list of async gateway addrs  */
  ub4	reqGateways;		/* requested # gateways */
  ub4	openGateways;		/* number gateways opened OK */
  mnnpa	*pav;			/* array of physical addresses */
  mnnio	**niov;			/* array of pointers to created nios */
  ub4	mnFlags;

  if (smniInitCnt++ > 0)
    return TRUE;
  
  if( !logger )
    logger = (mnLogger) smniLog;
  pav = (mnnpa *)0;
  niov = (mnnio**)0;
  heapsize = 0;
  if( (heapRes = ysResGetLast( "mn.heapsize" )) )
  {
    heapsize = (ub4)atol( heapRes );	/* atol is OSD... */
    if (tolower(heapRes[strlen(heapRes) - 1]) == 'k')
      heapsize *= 1024;
  }
  if (heapsize)			/* allocate if heap if request */
  {
    bv.len = heapsize;
    bv.buf = (ub1 *) smniMalloc(heapsize);
    bvp = &bv;
    nbv = 1;
  }
  else				/* use unlimited heap. */
  {
    bvp = (mnbv *) 0;
    nbv = 0;
  }

  /* get lists of gateways */
  syncList = ysResGet( "mn.sync-gateway");
  asyncList = ysResGet( "mn.async-gateway");

  /* if no gateways in YSR, look at OMN_ADDR */
  if( !syncList && !asyncList )
  {
    if( (omn_addr = ysResGetLast("OMN_ADDR")) )
    {
      ysResSet( "mn.sync-gateway", omn_addr );
      syncList = ysResGet( "mn.sync-gateway");
    }
  }

  /* allocate space for results */
  reqGateways =  0;
  if(syncList)
    reqGateways += ysLstCount( syncList );
  if( asyncList )
    reqGateways += ysLstCount( asyncList );

  if (!reqGateways)
    {
      yslError("no gateway specified and OMN_ADDR is not defined");
      return rv;
    }
  
  niov = (mnnio **)smniMalloc( reqGateways * sizeof(*niov) );
  pav = (mnnpa *)smniMalloc( reqGateways * sizeof(*pav) );

  /* open the gateways and fire up Media Net */
  if( niov && (rv = smniInitNios( syncList, asyncList, &openGateways,
				 pav, niov ) ) )
  {
    mnFlags = 0;
    if( (traceRes = ysResGetLast( "mn.trace" )) && !strcmp( traceRes, "true"))
      mnFlags |= MNFLG_TRACE;

    yseTry
      mnInit( mnFlags, logger, niov, (ub4)openGateways, pav,
	     bvp, nbv, smniMalloc, smniFree, (mnSpin)0, (dvoid *)0, (ub4)0);
    yseCatch(YS_EX_FAILURE)
      rv = FALSE;
    yseEnd
  }

  if (niov)
    ysmGlbFree((dvoid *) niov);
  if (pav)
    ysmGlbFree((dvoid *) pav);
  return( rv );
}


/* ---------------------------- smniInitNios ---------------------------- */
/*
  NAME
    smniInitNios
  DESCRIPTION
    Initialize all the NIOS and map them to the appropriate gateway
    addresses.
  PARAMETERS
    syncList	    -- list of gateways to be opened in synchronous mode.
    asyncList	    -- list of gateways to be opened in interrupt mode.
    num		    -- filled in with number of opened gateways [OUT]
    pav		    -- array of opened physical addresses [OUT]
    niov	    -- array of opened NIOs [OUT]
  RETURNS
    TRUE if at least one gateway was sucessfully opened, FALSE if none were.

  NOTES
    Errors may be reported under the covers.
*/

STATICF boolean smniInitNios( yslst *syncList, yslst *asyncList,
			     ub4 *num, mnnpa *pav, mnnio **niov )
{
  char buf[ 100 ];
  ysle	*e;
  *num = 0;
  if( syncList ) 
    for( e = ysLstHead( syncList ); e ; e = ysLstNext( e ) )
      if( smniNioInit( (char *)e->ptr, FALSE, pav, niov ) )
      {
	(*num)++;
	pav++;
	niov++;
      }
  if( asyncList )
    for( e = ysLstHead( asyncList ); e ; e = ysLstNext( e ) )
      if( smniNioInit( (char *)e->ptr, TRUE, pav, niov ) )
      {
	(*num)++;
	pav++;
	niov++;
      }
  return( *num != 0 );
}


/* ---------------------------- smniNioInit ---------------------------- */
/*
  NAME
    smniNioInit
  DESCRIPTION
    Given a gateway address string, check the address family to see if
    it is supported.  If so, create the NIO for it and connect it to
    the appropriate family adress.  Fill in both the physical address
    and the NIO.
  PARAMETERS
    gw	-- the physical address string.
    intr	-- whether to open the NIO in interrupt (async) mode.
    pa		-- the created physical address [OUT]
    nio		-- the created NIO [OUT]	
  RETURNS
    TRUE if the open succeeded, otherwise FALSE.
  NOTES
    Will report errors.
*/

STATICF boolean smniNioInit( char *gw, boolean intr, mnnpa *pa, mnnio **nio )
{
  *nio = (mnnio*)0;

  if (!strncmp(gw, "UDP:", 4))
    {
      *nio = smnudpOpen( (char *) 0, intr);
      if (!*nio)
	yslError("error opening %s", gw);
      else if (smnudpPa(pa, gw))
	yslError("physical address %s is invalid", gw);
    }
  else
    yslError("unsupported address family %s", gw);

  return( *nio != (mnnio*)0 );
}

/* ---------------------------- smniLog -------------------------------- */
/*
  NAME
    smniLog
  DESCRIPTION
    default logging routine for Media Net messages
  PARAMETERS
    msg         -- to record
  RETURNS
    nothing
*/
STATICF void   smniLog( CONST char  *msg )
{
  #define MNMSG(n, l) "OMN", "MN", (n), (l), (char *)0
  if (!strncmp(msg, "FATAL", 5))
    ysRecord(MNMSG(2001, YSLSEV_ALERT), YSLSTR(msg+7), YSLEND);
  else if (!strncmp(msg, "ERROR", 5))
    ysRecord(MNMSG(2002, YSLSEV_ERR), YSLSTR(msg+7), YSLEND);
  else if (!strncmp(msg, "WARNING", 7))
    ysRecord(MNMSG(2003, YSLSEV_WARNING), YSLSTR(msg+9), YSLEND);
  else
    ysRecord(MNMSG(2004, YSLSEV_DEBUG(2)), YSLSTR(msg), YSLEND);
#undef MNMSG
}

/* ---------------------------- smniMalloc ---------------------------- */
/*
  NAME
    smniMalloc
  DESCRIPTION
    Malloc-alike routine for use with Media-net.
  PARAMETERS
    size	-- to allocate
  RETURNS
    pointer to new space, or NULL.
*/
STATICF dvoid *smniMalloc( size_t size )
{
  return( ysmGlbAlloc( size, "smniMalloc" ) );
}

/* ENABLE check_proto_decl */


