/* Copyright (c) Oracle Corporation 1995.  All Rights Reserved. */

/*
  NAME
    smng.c
  DESCRIPTION
    OSD media-net initialization for gateway programs (mnars!)
    that need special stuff.
  PUBLIC FUNCTIONS
    smngInit
  PRIVATE FUNCTIONS
    smngInitNios
    smngNioInit
    smngAbort
    smngMalloc
  MODIFIED   (MM/DD/YY)
    dbrower   06/27/95 -  created.
*/

#include <signal.h>

#ifndef MTL_ORACLE
#include <mtl.h>
#endif

#ifndef SYSI_ORACLE
#include <sysi.h>
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

#ifndef SMNG_ORACLE
#include <smng.h>
#endif

#include <time.h>

static char smngRegKey[] = "SOFTWARE\\Oracle\\MediaNet";
static char smngRegOmnAddr[] = "OMN_ADDR";
static FILE *smngLogFp = NULL;                     /* pointer to MN log file */

STATICF boolean smngInitNios( yslst *syncList, yslst *asyncList,
			     ub4 *num, mnnpa *pav, mnnio **niov );

STATICF boolean smngNioInit( char *gw, boolean intr, mnnpa *pa, mnnio **nio );

STATICF sb4 smngAbort(void *usrp);

STATICF boolean smngRegGetVal(CONST char *rkey, CONST char *rname, char *rval);

STATICF dvoid *smngMalloc( size_t size );

STATICF void smngLogger( const char *fmt, va_list ap);

#define SMNG_REG_BUFSIZ	256

#define smngFree ysmFGlbFree
#define SMNG_DEF_MNLOGFN "mn.trc"

/* ---------------------------- smngInit ---------------------------- */
/*
  NAME
    smngInit
  DESCRIPTION
    Function called from portable main program entry point to initialize
    Media Net.  The Media Net parameters are taken from the YSR resource
    database, so it must be set up completely before this is called.

    There may be errors reported that are not returned to the caller.
    
    For win32 platforms, windows registry will be read to get OMN_ADDR if
    not set elsewhere, and for the "LOGFILE" and "trace" resources.

  PARAMETERS
    osdCtx	-- the osd context from the call to the entry point.
  RETURNS
    TRUE if at least one gateway has been opened into the
    Media Net, and FALSE if there is no gateway opened.
*/

boolean smngInit( void *osdCtx, mnLogger logger )
{
  boolean rv = FALSE;		/* assume the worst */
  char	*heapRes;		/* mn.heapsize resource value */
  ub4	heapsize;		/* actual heapsize, or 0 */
  mnbv	bv;
  mnbv	*bvp;
  sb4	nbv;

  char	*omn_addr;		/* OMN_ADDR value if no gateways speced. */
  yslst	*syncList;		/* list of sync gateway addrs */
  yslst	*asyncList;		/* list of async gateway addrs  */
  ub4	reqGateways;		/* requested # gateways */
  ub4	openGateways;		/* number gateways opened OK */
  mnnpa	*pav;			/* array of physical addresses */
  mnnio	**niov;			/* array of pointers to created nios */
  ub4	mnFlags;
  char  omabuf[SMNG_REG_BUFSIZ]; /* buf to get registry value for OMN_ADDR */
  char  trcbuf[SMNG_REG_BUFSIZ];  /* buf for registry value for mn.trace file*/
  char  mnLogFn[128];		/* mn log filename */

  pav = (mnnpa *)0;
  niov = (mnnio**)0;
  heapsize = 0;
  if( (heapRes = ysResGetLast( "mn.heapsize" )) )
  {
    heapsize = atol( heapRes );	/* atol is OSD... */
    if (tolower(heapRes[strlen(heapRes) - 1]) == 'k')
      heapsize *= 1024;
  }
  if (heapsize)			/* allocate if heap if request */
  {
    bv.len = heapsize;
    bv.buf = (ub1 *) smngMalloc(heapsize);
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
    if( !(omn_addr = ysResGetLast("OMN_ADDR")) &&
       smngRegGetVal( smngRegKey, smngRegOmnAddr, omabuf))
	omn_addr = omabuf;

    if( omn_addr )
      ysResSet( "mn.sync-gateway", omn_addr );
  }
  syncList = ysResGet( "mn.sync-gateway");

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
  
  niov = (mnnio **)smngMalloc( reqGateways * sizeof(*niov) );

  /* no pa's in gateway!? */
  /* pav = (mnnpa *)smngMalloc( reqGateways * sizeof(*bpav) ); */
  pav = (mnnpa *)0;

  if( !ysResGet("mn.trace") &&smngRegGetVal( smngRegKey, "trace", trcbuf) )
    ysResSet( "mn.trace", trcbuf);

  if( !logger )
  {
    logger = (mnLogger) mtlLog;             /* default logger */

    /* if trace is on, set up mn trace log file and its default logger */
    if( ysResGetBool( "mn.trace" ) )
    {
      if (!smngRegGetVal( smngRegKey, "LOGFILE", mnLogFn))
	strcpy(mnLogFn, SMNG_DEF_MNLOGFN);

      if( (smngLogFp = fopen(mnLogFn, "w")) )
      {
	setbuf( smngLogFp, (char *)0);
	logger = (mnLogger) smngLogger;
      }
      else
      {
	mtlLog("could not open MN log file %s, default to mtlLog\n",
	       mnLogFn);
      }
    }
  }

  /* open the gateways and fire up Media Net */
  if( niov && (rv = smngInitNios( syncList, asyncList, &openGateways,
				 pav, niov ) ) )
  {
    mnFlags = 0;
    if( ysResGetBool( "mn.trace" ) )
      mnFlags |= MNFLG_TRACE;
    
    mnInit( mnFlags | MNFLG_FORWARD, logger, niov, (ub4)openGateways, pav,
	   bvp, nbv, smngMalloc, smngFree, (mnSpin)0, (dvoid *)0, (ub4)0);

  }

  return( rv );
}


/* ---------------------------- smngInitNios ---------------------------- */
/*
  NAME
    smngInitNios
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

STATICF boolean smngInitNios( yslst *syncList, yslst *asyncList,
			     ub4 *num, mnnpa *pav, mnnio **niov )
{
  mnnpa	lpa;			/* local pa; tossed. */
  ysle	*e;
  *num = 0;

  if( syncList ) 
    for( e = ysLstHead( syncList ); e ; e = ysLstNext( e ) )
      if( smngNioInit( (char *)e->ptr, FALSE, &lpa, niov ) )
      {
	(*num)++;
	niov++;
      }
  if( asyncList )
    for( e = ysLstHead( asyncList ); e ; e = ysLstNext( e ) )
      if( smngNioInit( (char *)e->ptr, TRUE, &lpa, niov ) )
      {
	(*num)++;
	niov++;
      }
  return( *num != 0 );
}


/* ---------------------------- smngNioInit ---------------------------- */
/*
  NAME
    smngNioInit
  DESCRIPTION
    Given a gateway address string, check the address family to see if
    it is supported.  If so, create the NIO for it and connect it to
    the appropriate family adress.  Fill in both the physical address
    and the NIO.

    This is different than smniNioInit!!!  This sets the bind address
    in the Open call, where the client version does not.
    
  PARAMETERS
    gw		-- the physical address string.
    intr	-- whether to open the NIO in interrupt (async) mode.
    pa		-- the created physical address [OUT]
    nio		-- the created NIO [OUT]	
  RETURNS
    TRUE if the open succeeded, otherwise FALSE.
  NOTES
    Will report errors.
*/

STATICF boolean smngNioInit( char *gw, boolean intr, mnnpa *pa, mnnio **nio )
{
  *nio = (mnnio*)0;

  if (!strncmp(gw, "UDP:", 4))
    {
      *nio = smnudpOpen( (char *)gw, intr);
      if (!*nio)
	yslError("error opening %s", gw);
      else if (smnudpPa(pa, gw))
	yslError("physical address %s is invalid", gw);
    }
  else
    yslError("unsupported address family %s", gw);

  return( *nio != (mnnio*)0 );
}

/* ---------------------------- smngMalloc ---------------------------- */
/*
  NAME
    smngMalloc
  DESCRIPTION
    Malloc-alike routine for use with Media-net.
  PARAMETERS
    size	-- to allocate
  RETURNS
    pointer to new space, or NULL.
*/
STATICF dvoid *smngMalloc( size_t size )
{
  return( ysmGlbAlloc( size, "smngMalloc" ) );
}

/* --------------------------- smngRegGetVal -------------------------- */
/*
  NAME
    smngRegGetVal
  DESCRIPTION
    Retrieve registry value from registry key in windows registry.
  PARAMETERS
    rkey	-- registry key to query
    rname	-- name of value to be queried
    rval        -- pointer to pre-allocated string buffer
  RETURNS
    TRUE if successful or FALSE.
*/
STATICF boolean smngRegGetVal(CONST char *rkey, CONST char *rname, char *rval)
{
  HKEY hk;
  int  typ, len;
  boolean rv;

  len = SMNG_REG_BUFSIZ;
  hk = (HKEY)0;
  rv = FALSE;
  *rval = 0;

  if ((RegOpenKey(HKEY_LOCAL_MACHINE, rkey, &hk) == ERROR_SUCCESS) &&
      (RegQueryValueEx(hk, rname, NULL, &typ, rval, &len) == ERROR_SUCCESS))
    rv = TRUE;

  if( hk )
    RegCloseKey(hk);

  return rv;
}


/* --------------------------- smngLogger -------------------------- */
/*
 * smngLogger - default media net logger
 *
 * Default Media Net diagnostic message logger. It logs all messages in a
 * log file. For FATAL errors, it also logs to a popup dialog box 
 */ 
STATICF void smngLogger( const char *fmt, va_list ap)
{
  time_t     clock;
  struct tm *tm;
  char tmt[64];
  char buf[1024];

  vsprintf(buf, fmt, ap);   
    
  /* Get time stamp */
  time(&clock);
  tm = localtime(&clock);          
  sprintf(tmt, "%d:%d:%d", tm->tm_hour, tm->tm_min, tm->tm_sec);	  
  
  /* log message to log file */
  fprintf(smngLogFp, "%s: %s\n", tmt, buf);
    
  if(!strncmp(buf, "FATAL:",6)) 
  {                              
    /* FATAL error; log to popup dialog box */
    yslError(buf);
  }
}

/* ENABLE check_proto_decl */


