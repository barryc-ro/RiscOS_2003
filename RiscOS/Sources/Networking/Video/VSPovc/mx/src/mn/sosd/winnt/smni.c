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
    dbrower   06/27/95 -  created.
*/

#include <signal.h>

#ifndef MTL_ORACLE
#include <mtl.h>
#endif

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
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SMNI_ORACLE
#include <smni.h>
#endif

#include <time.h>

STATICF boolean smniInitNios( yslst *syncList, yslst *asyncList,
			     ub4 *num, mnnpa *pav, mnnio **niov );

STATICF boolean smniNioInit( char *gw, boolean intr, mnnpa *pa, mnnio **nio );

STATICF dvoid *smniMalloc( size_t size );

STATICF void smniLogger( const char *fmt, va_list ap);

STATICF boolean smniRegGetVal(CONST char *rkey, CONST char *rname, char
			      *rval);

/* MN diagnostic message default logger */
dvoid smniLogger( CONST char *fmt, va_list ap);

/* size of buffer to hand to smniRegGetVal */
#define SMNI_REG_BUFSIZ   256

#define smniFree ysmFGlbFree
#define SMNI_DEF_MNLOGFN "mn.trc"

/* global count of calls to ssmniInit minus calls to ssmniTerm */
static int  ssmniInitCnt = 0;
static char smniRegKey[] = "SOFTWARE\\Oracle\\MediaNet";
static char smniRegOmnAddr[] = "OMN_ADDR";

static FILE *smniLogFp = NULL;			   /* pointer to MN log file */

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

  NOTES
    smniInit() calls ssmniInit() with omn_addr set to NULL, that means
    omn_addr will be looked from YSR or Windows registry. The ssmniInit()
    calls ssysiInit() to ensure YS layer is initiated.

    For win32 platforms, windows registry will be read to get OMN_ADDR if
    not set elsewhere, and for the "LOGFILE" and "trace" resources.
*/

/* ARGSUSED */
boolean smniInit( void *osdCtx, mnLogger logger )
{
  return ssmniInit( osdCtx, logger,  (CONST char *)NULL );
}

boolean ssmniInit( void *osdCtx, mnLogger logger, CONST char *omn_addr )
{
  noreg boolean rv = FALSE;		/* assume the worst */
  char	*heapRes;		/* mn.heapsize resource value */
  ub4	heapsize;		/* actual heapsize, or 0 */
  mnbv	bv;
  mnbv	*bvp;
  ub4	nbv;

  yslst	*syncList;		/* list of sync gateway addrs */
  yslst	*asyncList;		/* list of async gateway addrs  */
  ub4	reqGateways;		/* requested # gateways */
  ub4	openGateways;		/* number gateways opened OK */
  mnnpa	*pav;			/* array of physical addresses */
  mnnio	**niov;			/* array of pointers to created nios */
  ub4	mnFlags;
  char  omabuf[SMNI_REG_BUFSIZ];/* buf for registry value of OMN_ADDR */
  char  trcbuf[SMNI_REG_BUFSIZ];/* buf for registry value for mn.trace file */
  char  mnLogFn[128];		/* mn log filename */

  NOREG(rv);

  if( ssmniInitCnt > 0)
  {                     /* media net already initialized */
    if (!omn_addr)      /* not a new init request */
    {
      ssmniInitCnt++;
      return TRUE;
    }
    else
    {
      if (!strcmp(ysResGetLast( "mn.sync-gateway" ), omn_addr))
      {                 /* a new init request, yet the omn_addr is same */
        ssmniInitCnt++;
	return TRUE;
      }
      return FALSE;     /* illegal to start a new MN with an existing one */
    }
  }

  pav = (mnnpa *)0;
  niov = (mnnio**)0;
  heapsize = 0;
  if( (heapRes = ysResGetLast( "mn.heapsize" )) )
  {
    heapsize = (ub4)atol( heapRes );	/* atol is OSD... */
    if (tolower(heapRes[strlen(heapRes) - 1]) == 'k')
      heapsize *= 1024;
  }
  if (heapsize)			/* allocate if heap if requfest */
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

  if( omn_addr )              /* if an omn_addr is specified */
  {
    /* delete the prevoius sync-gateway value from YSR */
    if (syncList)
    {
      ysstr *val = NULL;
      
      val = (ysstr *) ysLstRem(syncList, syncList->head);
      if (!val) ysStrDestroy(val);
    }
  
    /* set omn_addr in YSR */
    ysResSet( "mn.sync-gateway", omn_addr );
    syncList = ysResGet( "mn.sync-gateway");
  }

  /* if no gateways in YSR, look at OMN_ADDR, and windows registry */
  if( !syncList && !asyncList )
  {
    if( !(omn_addr = ysResGetLast("OMN_ADDR")) &&
       smniRegGetVal( smniRegKey, smniRegOmnAddr, omabuf) )
        omn_addr = omabuf;

    if ( omn_addr )
      ysResSet( "mn.sync-gateway", omn_addr );
  }
  syncList = ysResGet( "mn.sync-gateway");

  /* allocate space for results */
  reqGateways =  0;
  if( syncList )
    reqGateways += ysLstCount( syncList );
  if( asyncList )
    reqGateways += ysLstCount( asyncList );

  if (!reqGateways)
  {
    yslError("no gateway specified and OMN_ADDR is not defined");
    return rv;
  }

  /* get mn.trace resource setting from registry if not in env */
  if (!ysResGet("mn.trace") && smniRegGetVal( smniRegKey, "trace", trcbuf) )
    ysResSet( "mn.trace", trcbuf);

  if( !logger )
  {
    logger = (mnLogger) yslPrint;	      /* default */

    /* if trace is on, set up mn trace log file and its default logger */
    if( ysResGetBool( "mn.trace" ) )
    {
      if (!smniRegGetVal( smniRegKey, "LOGFILE", mnLogFn))
	strcpy(mnLogFn, SMNI_DEF_MNLOGFN);

      if( (smniLogFp = fopen(mnLogFn, "w")) )
      {
	setbuf( smniLogFp, (char *)0);
	logger = (mnLogger) smniLogger;
      }
      else
      {
	yslError("could not open MN log file %s, default to yslError\n",
		 mnLogFn);
	logger = (mnLogger) yslError;
      }
    }
  }
  
  niov = (mnnio **)smniMalloc( reqGateways * sizeof(*niov) );
  pav = (mnnpa *)smniMalloc( reqGateways * sizeof(*pav) );

  /* open the gateways and fire up Media Net */
  if( niov && (rv = smniInitNios( syncList, asyncList, &openGateways,
				 pav, niov ) ) )
  {
    mnFlags = 0;
    if( ysResGetBool( "mn.trace" ) )
      mnFlags |= MNFLG_TRACE;

    yseTry
      mnInit( mnFlags, logger, niov, (ub4)openGateways, pav,
	     bvp, nbv, smniMalloc, smniFree, (mnSpin)0, (dvoid *)0, (ub4)0);
    yseCatch(YS_EX_FAILURE)
      rv = FALSE;
    yseEnd;
  }

  if (niov)
    ysmGlbFree((dvoid *) niov);
  if (pav)
    ysmGlbFree((dvoid *) pav);

  if (rv) ssmniInitCnt++;
  return( rv );
}

/* --------------------------- ssmniTerm ---------------------------- */
/*
  NAME
    ssmniTerm
  DESCRIPTION
    Function called to terminate Media Net session started via ssmniInit().
    
  PARAMETERS
    None
  RETURNS

  NOTES
*/

void ssmniTerm(void)
{
  ssmniInitCnt--;			   /* decrease mn session init count */
  if( !ssmniInitCnt )
  {
    mnTerm();

    if( smniLogFp )		       /* close mn.trc file, if it is opened */
    {
      if( !fclose(smniLogFp) )
	smniLogFp = NULL;
      else
	yslError( "ssmniTerm: fail to fclose mn trace file" );
    }
  }
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
      {
	free(*nio); *nio = (mnnio*)0;
	yslError("physical address %s is invalid", gw);
      }
    }
  else
    yslError("unsupported address family %s", gw);

  return( *nio != (mnnio*)0 );

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

/* --------------------------- smniRegGetVal -------------------------- */
/*
  NAME
    smniRegGetVal
  DESCRIPTION
    Retrieve registry value from registry key in windows registry.
  PARAMETERS
    rkey	-- registry key to query
    rname	-- name of value to be queried
    rval        -- pointer to pre-allocated string buffer
  RETURNS
    TRUE if successful or FALSE.
*/
STATICF boolean smniRegGetVal(CONST char *rkey, CONST char *rname, char *rval)
{
  HKEY hk;
  int  typ, len;
  boolean rv;

  len = SMNI_REG_BUFSIZ;
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

/* --------------------------- smniLogger -------------------------- */
/*
 * smniLogger - default media net logger
 *
 * Default Media Net diagnostic message logger. It logs all messages in a
 * log file. For FATAL errors, it also logs to a popup dialog box 
 */ 
STATICF void smniLogger( const char *fmt, va_list ap)
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
  fprintf(smniLogFp, "%s: %s\n", tmt, buf);
    
  if(!strncmp(buf, "FATAL:",6)) 
  {                              
    /* FATAL error; log to popup dialog box */
    yslError(buf);
  }
}

/* ENABLE check_proto_decl */



