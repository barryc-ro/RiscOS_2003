/* mx/src/ye/yeevctl.c */


/*
ORACLE, Copyright (c) 1982, 1983, 1986, 1990 ORACLE Corporation
ORACLE Utilities, Copyright (c) 1981, 1982, 1983, 1986, 1990, 1991 ORACLE Corp

Restricted Rights
This program is an unpublished work under the Copyright Act of the
United States and is subject to the terms and conditions stated in
your  license  agreement  with  ORACORP  including  retrictions on
use, duplication, and disclosure.

Certain uncopyrighted ideas and concepts are also contained herein.
These are trade secrets of ORACORP and cannot be  used  except  in
accordance with the written permission of ORACLE Corporation.
*/





#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef SYSB8_ORACLE
#include <sysb8.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef YSV_ORACLE
#include <ysv.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef YT_ORACLE
#include <yt.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YEEVLOG_ORACLE
#include <yeevlog.h>
#endif
#ifndef YEEV_ORACLE
#include <yeev.h>
#endif
#ifndef YEU_ORACLE
#include <yeu.h>
#endif
#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YOYD_ORACLE
#include <yoyd.h>
#endif
#ifndef CTYPE_ORACLE
#include <ctype.h>
#endif
#ifndef YEEVF_ORACLE
#include <yeevf.h>
#endif
#ifndef YEMSG_ORACLE
#include <yemsg.h>
#endif

#define ENTRY_POINT yeevctlMain
#include <s0ysmain.c>



#define yeevctlStr( s ) (s ? s : (char *)"<null>")
#define yeevctlStrx( s ) (s ? s : (char *)"")

#define yeevctlNarrow( or, intf ) \
    { if( or ) DISCARD yoNarrow( or, intf ); else yseThrow(YS_EX_BADPARAM); }


static struct ysargmap yeevctlmap[] =
{
    { YSARG_OPTPARAM, "yeevctl.verb", 1},
    { YSARG_OPTPARAM, "yeevctl.producer-id", 1},
    { YSARG_OPTPARAM, "yeevctl.consumer-id", 1},
    { YSARG_OPTPARAM, "yeevctl.optargs", YSARG_MANY },
    { 0, (char *) 0, 0 }
};

    
typedef struct yeevctlcx yeevctlcx;
typedef struct yeevctln yeevctln;


struct yeevctln
{
  ysspNode  onode_yeevctln;
  ysspNode  inode_yeevctln;
  sb4	    idx_yeevctln;	
  yeevctlcx *cx_yeevctln;
};


struct yeevctlcx
{
  ysque	    *q_yeevctlcx;
  yeev	    ev_yeevctlcx;
  yoenv	    env_yeevctlcx;
  sb4	    trace_yeevctlcx;
  ydim_imr  imr_yeevctlcx;
  boolean   inter_yeevctlcx;	
  ysspTree  ot_yeevctlcx;	
  ysspTree  it_yeevctlcx;	
  sb4	    nt_yeevctlcx;	
  boolean   exiting_yeevctlcx;
} ;


externdef ysmtagDecl(yeevctls_tag) = "yeevctls";


externdef ysidDecl( YEEVCTL_EX_BAD_FTYPE ) = "::yeevctl::badFtype";

externdef ysidDecl( YEEVCTL_EX_CAUGHT ) = "::yeevctl::caught";

STATICF void yeevctlPanicHdlr(CONST ysid *exid, dvoid *ptr);

STATICF dvoid *yeevctlProdLookup( yeevctlcx *cx, char *procid, boolean log );
STATICF yeevReceiver yeevctlConsLookup( yeevctlcx *cx, char *procid );

STATICF boolean yeevctlInteractive( yeevctlcx *cx );
STATICF char *yeevctlField( char **cursor  );

STATICF boolean yeevctlVerbs(void);
STATICF boolean yeevctlInfo(yeevctlcx *cx, char *prod);
STATICF boolean yeevctlFilters(yeevctlcx *cx, char *prod, char *cons);

STATICF boolean yeevctlProcs(yeevctlcx *cx, char *regexp );
STATICF boolean yeevctlLogs(yeevctlcx *cx, char *logid);
STATICF boolean yeevctlReceivers( yeevctlcx *cx, char *prod, char *regexp );

STATICF boolean yeevctlDefs(yeevctlcx *cx, char *logid);

STATICF boolean yeevctlSet(yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters);
STATICF boolean yeevctlSetDef(yeevctlcx *cx, char *prod, char *cons,
			      yslst *filters);
STATICF boolean yeevctlAdd(yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters);
STATICF boolean yeevctlDel(yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters);

STATICF void yeevctlShowFilters( yeevctlcx *cx, yeevdSeq *dlvals );

STATICF sword yeevctlObjNodeCmp( CONST dvoid *a, CONST dvoid *b );
STATICF sb4 yeevctlObjIdx( yeevctlcx *cx, dvoid *obj );
STATICF dvoid *yeevctlObjLookup( yeevctlcx *cx, ub4 idx );
STATICF void yeevctlDeath( dvoid *usrp, CONST ysid *exit, dvoid *arg,
			  size_t argsz );






boolean yeevctlMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  boolean   ok;
  noreg boolean rv;
  char	    *arg;
  char      vbuf[80];
  sb4	    sts;
  yeevctlcx  cx;
  yslst	    *optargs;
  char	    *verb;
  char	    *prod;
  char	    *consumer;
  yeevctln  *on;
  ysspNode  *n;
  
  NOREG(rv);

  
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, yeevctlmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ORB Event Control Program");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);

  if (!ok)
    return(FALSE);
  
  yseTry
  {
    cx.ev_yeevctlcx = (yeev)0;
    cx.exiting_yeevctlcx = FALSE;

    ytInit();
    yoInit();

    DISCARD yseSetPanic(yeevctlPanicHdlr, (dvoid*)&cx);

    cx.trace_yeevctlcx =
      (arg=ysResGetLast("yeevctl.trace-level"))?(sb4)atol(arg):0;

    
    DISCARD ysspNewTree( &cx.ot_yeevctlcx, yoCmp );
    DISCARD ysspNewTree( &cx.it_yeevctlcx, yeevctlObjNodeCmp );
    cx.nt_yeevctlcx = 0;

    verb = ysResGetLast("yeevctl.verb");
    prod = ysResGetLast("yeevctl.producer-id");
    consumer = ysResGetLast("yeevctl.consumer-id");
    optargs = ysResGet("yeevctl.optargs");
  
    yoEnvInit( &cx.env_yeevctlcx );

    
    cx.q_yeevctlcx = (ysque*)0;
    rv = TRUE;

    cx.imr_yeevctlcx = (ydim_imr)yoBind(ydim_imr__id, (char*)0,
					(yoRefData*)0, (char*)0 );

    cx.ev_yeevctlcx = yeevInit( cx.q_yeevctlcx );
    yeevSinkAttach( cx.ev_yeevctlcx, YSLSEV_DEBUG(7) );
    cx.inter_yeevctlcx = FALSE;
    if( !verb )
    {
      cx.inter_yeevctlcx = TRUE;
      DISCARD yeevctlInteractive( &cx );
    }
    else
    {
      if( !strncmp(verb, "help", 1) )
	rv = yeevctlVerbs();
      else if( !strncmp( verb, "info", 1 ) )
	rv = yeevctlInfo( &cx, prod );
      else if( !strncmp( verb, "logs", 1 ) )
	rv = yeevctlLogs( &cx, prod );
      else if( !strncmp( verb, "defs", 3 ) )
	rv = yeevctlDefs( &cx, prod );
      else if( !strncmp( verb, "filters", 1 ) )
	rv = yeevctlFilters( &cx, prod, consumer );
      else if( !strncmp( verb, "procs", 1 ) )
	rv = yeevctlProcs( &cx, prod );
      else if( !strncmp( verb, "setdef", 4 ) )
	rv = yeevctlSetDef( &cx, prod, consumer, optargs );
      else if( !strncmp( verb, "set", 1 ) )
	rv = yeevctlSet( &cx, prod, consumer, optargs );
      else if( !strncmp( verb, "receivers", 1 ) )
	rv = yeevctlReceivers( &cx, prod, consumer );
      else if( !strncmp( verb, "add", 1 ) )
	rv = yeevctlAdd( &cx, prod, consumer, optargs );
      else if( !strncmp( verb, "delete", 3 ) )
	rv = yeevctlDel( &cx, prod, consumer, optargs );
      else
      {
	yslError( "%s: unrecognized verb \"%s\"\n",
		 ysProgName(), verb );
	rv = FALSE;
      }
    }
  }
  yseCatch( YS_EX_INTERRUPT )
  {
    
  }
  yseCatch( YEEVCTL_EX_CAUGHT )
  {
    
  }
  yseCatchAll
  {
    yslError("%s caught unexpected exception %s, exiting\n",
	     ysProgName(), ysidToStr(yseExid));
    rv = FALSE;
  }
  yseEnd;
    
  

  yseTry
  {
    cx.exiting_yeevctlcx = TRUE;
    yeevTerm( cx.ev_yeevctlcx );

    while( (n = ysspDeqTree( &cx.it_yeevctlcx ) ) )
    {
      on = (yeevctln*)n->key_ysspNode;
      ysspRemove( &on->onode_yeevctln, &cx.ot_yeevctlcx );
      yoRelease( on->onode_yeevctln.key_ysspNode );
      ysmGlbFree( (dvoid*)on );
    }

    yoEnvFree( &cx.env_yeevctlcx );
  }
  yseCatchAll
  {
    yslError("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
    rv = FALSE;
  }
  yseEnd;

  yseTry
  {
    yoTerm();
    ytTerm();
  }
  yseCatchAll
    yslError("%s caught exception %s while exiting\n",
	     ysProgName(), ysidToStr(yseExid));
  yseEnd;
  ysTerm(osdp);
  return rv;
}





STATICF void yeevctlPanicHdlr(CONST ysid *exid, dvoid *ptr)
{
  yeevctlcx *cx = (yeevctlcx*)ptr;

  if( exid )
    yslError("yeevctlPanicHdlr: exception %s, ptr %x\n",
	     ysidToStr(exid), ptr);

  if( cx && cx->ev_yeevctlcx && !cx->exiting_yeevctlcx )
  {
    yslError("yeevctlPanicHdlr: cleaning up\n");
    yeevTerm( cx->ev_yeevctlcx );
  }
}




STATICF dvoid *yeevctlProdLookup( yeevctlcx *cx, char *procid, boolean log )
{
  dvoid *rv;
  yort_proc y;
  char	*host;
  char	*pid;
  char	*affinity;
  sb4	idx;
  char buf[1024];

  if( procid )
  {
    DISCARD strcpy(buf, procid);
    procid = buf;
  }

  if( cx->trace_yeevctlcx )
    yslError("yeevctlProdLookup: procid %s\n", yeevctlStr(procid));

  rv = (dvoid*)0;
  if( procid )
  {
    
    idx = (sb4)atol( procid );
    rv = (dvoid*)yeevctlObjLookup( cx, (ub4)idx );
  }
    
  if( !rv && procid && *procid == '~' )
  {
    rv = yoStrToRef( procid );
  }
  else if( !rv && procid )
  {
    
    host = procid;
    while( *procid && *procid != ':' )
      procid++;
    if( *procid )
      *procid++ = 0;
    pid = procid;
    while( *procid && *procid != ':' )
      procid++;
    if( *procid )
      *procid++ = 0;
    affinity = procid;
    if( !*affinity )
      affinity = (char*)0;

    if( cx->trace_yeevctlcx )
      yslError("yeevctlProdLookup: host %s, pid %s, affinity %s\n",
	       yeevctlStr(host), yeevctlStr(pid), yeevctlStr(affinity) );

    y = ydim_imr_yortOfProc( cx->imr_yeevctlcx, &cx->env_yeevctlcx,
			    host, pid, affinity);
    if( y )
    {
      rv = (dvoid*)yoYortBind( log ? yeevl_logProc__id : yeev__id,
			    (char*)0, y );
      yoRelease( (dvoid*)y );
    }
    else
    {
      yslError("yeevctlProdLookup: didn't find %s:%s:%s\n",
	       yeevctlStr(host), yeevctlStr(pid), yeevctlStr(affinity) );

    }
  }
  idx = yeevctlObjIdx(cx, (dvoid*)rv );
  if( cx->trace_yeevctlcx )
    yslError("yeevctlProdLookup: returning yeev idx %d\n", idx );
    
  return rv;
}






STATICF yeevReceiver yeevctlConsLookup( yeevctlcx *cx, char *consid )
{
  yeevReceiver rv;
  yort_proc y;
  char	*host;
  char	*pid;
  char	*affinity;
  char	*recname;
  yeev	ev;
  yeevReceiverList rlist;
  char	*hisname;
  ub4	i;
  sb4	idx;
  
  if( cx->trace_yeevctlcx )
    yslError("yeevctlConsLookup: consid %s\n", yeevctlStr(consid));

  rv = (yeevReceiver)0;
  if( consid )
  {
    
    idx = (sb4)atol( consid );
    rv = (yeevReceiver)yeevctlObjLookup( cx, (ub4)idx );
  }

  if( !rv && consid && *consid == '~' )
  {
    rv = (yeevReceiver)yoStrToRef( consid );
  }
  else if( !rv && consid )
  {
    
    host = consid;
    while( *consid && *consid != ':' )
      consid++;
    if( *consid )
      *consid++ = 0;
    pid = consid;
	    
    while( *consid && *consid != ':' )
      consid++;
    if( *consid )
      *consid++ = 0;
    affinity = consid;
	    
    while( *consid && *consid != ':' )
      consid++;
    if( *consid )
      *consid++ = 0;
    recname = consid;

    if( !*affinity )
      affinity = (char*)0;

    if( cx->trace_yeevctlcx )
      yslError("yeevctlConsLookup: host %s, pid %s, affinity %s\n",
	       yeevctlStr(host), yeevctlStr(pid), yeevctlStr(affinity) );

    y = ydim_imr_yortOfProc( cx->imr_yeevctlcx, &cx->env_yeevctlcx,
			    host, pid, affinity);
    if( y )
    {
      ev = (yeev)yoYortBind( yeev__id, (char*)0, y );
      yoRelease( (dvoid*)y );

      rlist = yeev__get_receivers( ev, &cx->env_yeevctlcx );
      for( i = 0; !rv && i < rlist._length ; i++ )
      {
	hisname = yeevReceiver__get_name(rlist._buffer[i], &cx->env_yeevctlcx);
	if( !strcmp(hisname, recname) )
	  rv = (yeevReceiver)yoDuplicate( (dvoid*)rlist._buffer[i] );
	yoFree( (dvoid*)hisname );
      }
      yoRelease( (dvoid*)ev );
      yeevReceiverList__free( &rlist, yoFree );
    }
    if( !rv )
    {
      yslError("yeevctlConsLookup: didn't find %s:%s:%s:%s\n",
	       yeevctlStr(host),
	       yeevctlStr(pid),
	       yeevctlStr(affinity),
	       yeevctlStr(recname) );
    }
  }

  idx = yeevctlObjIdx( cx, (dvoid*)rv );
  if( cx->trace_yeevctlcx )
    yslError("yeevctlConsLookup: returning yeevReceiver idx %d\n", idx );
  return rv;
}


STATICF boolean yeevctlInteractive( yeevctlcx *cx )
{
  noreg boolean   stop = FALSE;
  noreg boolean   rv = TRUE;
  char buf[ 1024 ];
  char *verb;
  char *noreg consumer;
  char *noreg prod;
  char	*cursor;
  yslst *noreg optargs;
  
  NOREG(optargs);
  NOREG(stop);
  NOREG(rv);
  NOREG(prod);
  NOREG(consumer);
  
  optargs = (yslst*)0;

  while( !stop )
  {
    yslPrint("%s> ", ysProgName() );
    if( !yslGets( buf, sizeof(buf) ) )
      break;
    
    yseTry
    {
      cursor = buf;
      optargs = ysLstCreate();

      verb = yeevctlField( &cursor );
      prod = yeevctlField( &cursor );
      consumer = yeevctlField( &cursor );
    
      
      if( *cursor )
	DISCARD ysLstEnq( (yslst*)optargs, (dvoid*)cursor );

      if( !strncmp(verb, "help", 1) )
	rv = yeevctlVerbs();
      else if( !strncmp( verb, "info", 1 ) )
	rv = yeevctlInfo( cx, prod );
      else if( !strncmp( verb, "l", 1 ) )
	rv = yeevctlLogs( cx, prod );
      else if( !strncmp( verb, "defs", 3 ) )
	rv = yeevctlDefs( cx, prod );
      else if( !strncmp( verb, "filters", 1  ) )
	rv = yeevctlFilters( cx, prod, consumer );
      else if( !strncmp( verb, "procs", 1  ) )
	rv = yeevctlProcs( cx, prod );
      else if( !strncmp( verb, "receivers", 1 ) )
	rv = yeevctlReceivers( cx, prod, consumer );
      else if( !strncmp( verb, "setdef", 4 ) )
	rv = yeevctlSetDef( cx, prod, consumer, (yslst*)optargs );
      else if( !strncmp( verb, "set", 1 ) )
	rv = yeevctlSet( cx, prod, consumer, (yslst*)optargs );
      else if( !strncmp( verb, "add", 1 ) )
	rv = yeevctlAdd( cx, prod, consumer, (yslst*)optargs );
      else if( !strncmp( verb, "delete", 3 ) )
	rv = yeevctlDel( cx, prod, consumer, (yslst*)optargs );
      else if( !strncmp( verb, "quit", 1 ) )
	stop = TRUE;
      else
	yslError("unrecognized verb '%s' (help for help)\n", yeevctlStr(verb));
    }
    yseCatch( YEEVCTL_EX_CAUGHT );
    yseCatchAll
    {
      yslError("%s caught exception %s, continuing.\n",
	       ysProgName(), ysidToStr(yseExid) );
    }
    yseEnd;
    
    if( optargs )
    {
      ysLstDestroy( (yslst*)optargs, (ysmff)0 );
      optargs = (yslst*)0;
    }
  }
  return rv;
}




STATICF char *yeevctlField( char **cursor )
{
  char *p;			
  char *q;			

  p = *cursor;
  
  
  while( *p && isspace(*p) )
    p++;

  q = p;
  while( *q && !isspace(*q) )
    q++;

  if( *q )
    *q++ = 0;

  
  while(*q && isspace(*q) )
    q++;

  *cursor = q;
  return p;
}


STATICF boolean yeevctlVerbs(void)
{
  yslError("Verb      Arguments    Effect\n"); 
  yslError("--------- ------------ --------------------------------------\n");
  yslError("(none)                 enter interactive mode\n");
  yslError("help                   show this list of verbs\n");
  yslError("quit                   exit interactive mode\n");
  yslError("filters   prod-id      list filters in prod-id\n");
  yslError("filters   prod-id cons-id\n");
  yslError("                       list filters in prod-id to cons-id\n");
  yslError("procs                  list all processes\n");
  yslError("procs     regexp       list processes matching regexp\n");
  yslError("receivers prod-id      show receivers in prod-id\n");
  yslError("receivers prod-id regexp\n");
  yslError("                       show receivers limited to regexp\n");
  yslError("info      prod-id      show info from prod-id\n");
  yslError("set       prod-id cons-id filter\n");
  yslError("                       set filter in prod-id to cons-id\n");
  yslError("add       prod-id cons-id filter\n");
  yslError("                       add filter in prod-id to cons-id\n");
  yslError("delete    prod-id cons-id\n");
  yslError("                       delete all filter to cons-id\n");
  yslError("delete    prod-id cons-id filter\n");
  yslError("                       delete filter in prod-id to cons-id\n");
  yslError("logs      log-id       show logs in logger log-id\n");
  yslError("defs      log-id       show default filters in logger log-id\n");
  yslError("setdef    log-id cons-id filter\n");
  yslError("                       set default filter; cons-id ignored\n");
  return( TRUE );
}






STATICF boolean yeevctlInfo( yeevctlcx *cx, char *prod )
{
  yeevInfo info;
  char *rs;
  yeev pev;

  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  yeevctlNarrow( (dvoid*)pev, yeev__id );
  info = yeev__get_info( pev, &cx->env_yeevctlcx );

  if( cx->inter_yeevctlcx )
  {
    yslPrint("Info from producer idx %d, %s:%s:%s\n",
	     yeevctlObjIdx(cx, (dvoid*)pev),
	     yeuStr(info.host_yeevInfo),
	     yeuStr(info.pid_yeevInfo),
	     yeuStr(info.affinity_yeevInfo));
  }
  else
  {
    rs = yoRefToStr( (dvoid*)pev );
    yslPrint("Info from producer %s, %s:%s:%s\n",
	     rs, 
	     yeuStr(info.host_yeevInfo),
	     yeuStr(info.pid_yeevInfo),
	     yeuStr(info.affinity_yeevInfo));
    yoFree( (dvoid*)rs );
  }
  yslPrint("%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n",
	   "nevent", "npush", "npushm", "npull",
	   "ntpull", "ntpullm", "nlimits", "ndropped" );
  yslPrint("%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n",
	   "--------", "--------", "--------", "--------", 
	   "--------", "--------", "--------", "--------" );

  yslPrint("%8d %8d %8d %8d %8d %8d %8d %8d\n",
	   info.nevent_yeevInfo,
	   info.npush_yeevInfo,
	   info.npushm_yeevInfo,
	   info.npull_yeevInfo,
	   info.ntpull_yeevInfo,
	   info.ntpullm_yeevInfo,
	   info.nlimits_yeevInfo,
	   info.ndropped_yeevInfo );

  yeevInfo__free( &info, yoFree );
  return TRUE;
}





STATICF boolean yeevctlFilters( yeevctlcx *cx, char *prod, char *cons )
{
  yeevDiscList dlobj;
  yeevdSeq  dlvals;
  yeev pev;
  yeevReceiver noreg cev;
  char *rs, *ds;
  
  NOREG(cev);
  
  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  cev = cons && *cons ? yeevctlConsLookup( cx, cons ) : (yeevReceiver)0;

  yseTry
    yeevctlNarrow( (dvoid*)pev, yeev__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Prod %s doesn't denote a known yeev object\n",
	     yeevctlStr(prod));
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  if( cev )
  {
    yseTry
      yeevctlNarrow( (dvoid*)cev, yeevReceiver__id );
    yseCatch( YS_EX_BADPARAM )
    {
      yslError("Cons %s doesn't denote a known yeevReceiver object\n",
	       yeevctlStr(cons) );
      yseThrow( YEEVCTL_EX_CAUGHT );
    }
    yseEnd;
  }
  
  if( pev )
  {
    
    dlobj = yeev__get_filters( pev, &cx->env_yeevctlcx );

    
    if( cev )
      yeevDiscList_listDest( dlobj, &cx->env_yeevctlcx, cev, &dlvals );
    else
      dlvals = yeevDiscList__get_dlist( dlobj, &cx->env_yeevctlcx );

    if( cx->inter_yeevctlcx )
    {
      yslPrint("Filter list from producer idx %d, destination idx %d\n", 
	       yeevctlObjIdx(cx, (dvoid*)pev),
	       yeevctlObjIdx(cx, (dvoid*)cev));
    }
    else
    {
      rs = yoRefToStr( (dvoid*)pev );
      if( cev )
	ds = yoRefToStr( (dvoid*)cev );
      else
	ds = (char*)0;
      yslPrint("Filter list from producer %s, destination %s\n", rs,
	       yeevctlStr(ds) );
      yoFree( (dvoid*)rs );
      if( ds )
	yoFree( (dvoid*)ds );
    }

    yeevctlShowFilters( cx, &dlvals );
    yeevdSeq__free( &dlvals, yoFree );
    yoRelease( (dvoid*)dlobj );
  }
  return TRUE;
}





STATICF boolean yeevctlProcs( yeevctlcx *cx, char *regexp )
{
  ydim_yortIterator yi;
  ydim_yortList	yortList;
  yort_proc 	y;
  yort_procInfo	pinfo;
  char buf[ 1024 ]; 
  char obuf[ sizeof(buf) *2 ]; 
  dvoid	    *obj;
  ub4	    idx;
  boolean   log;
  ysre	    *cre;
  char	    *rs;

  if( cx->inter_yeevctlcx )
  {
    yslPrint("%-8s %-32s %-12s\n",
	     "Yeev Idx", "Host:Pid:Affinity", "Name" );
    yslPrint("%-8s %-32s %-12s\n",
	     "--------",
	     "--------------------------------",
	     "------------" );
  }
  else
  {
    yslPrint("YeevRef Host:Pid:Aff Name\n");
  }

  cre = regexp && *regexp ? ysRECompile(regexp, FALSE) : (ysre*)0;

  ydim_imr_listYort(cx->imr_yeevctlcx, &cx->env_yeevctlcx,
		    (sb4)0, (yort_proc)0, &yortList, &yi);

  while( yi && ydim_yortIterator_next_one(yi, &cx->env_yeevctlcx, &y))
  {
    DISCARD ydim_imr_procOfYort(cx->imr_yeevctlcx, &cx->env_yeevctlcx,
				y, &pinfo);
    ysFmtStr(buf, "%s:%s%s%s", 
	     yeevctlStrx(pinfo.host_yort_procInfo),
	     yeevctlStrx(pinfo.pid_yort_procInfo),
	     pinfo.affinity_yort_procInfo ? ":" : "",
	     pinfo.affinity_yort_procInfo ?
	     pinfo.affinity_yort_procInfo : "" );

    log = !strcmp( pinfo.name_yort_procInfo, "yeevld" );
    obj = yeevctlProdLookup( cx, buf, log );
    idx = (ub4)yeevctlObjIdx( cx, obj );

    if( cx->inter_yeevctlcx )
    {
      ysFmtStr( obuf, "%8d %-32.32s %s",
	       idx, buf, yeevctlStr(pinfo.name_yort_procInfo) );
    }
    else
    {
      rs = yoRefToStr( obj );
      ysFmtStr( obuf, "%s %s %s", 
	       rs, buf, yeevctlStr(pinfo.name_yort_procInfo) );
      yoFree( (dvoid*)rs );
    }

    if( cre ? ysREMatch( cre, obuf, TRUE ) : TRUE )
      yslPrint( "%s\n", obuf );

    yort_procInfo__free(&pinfo, yoFree);
    yoRelease( (dvoid*)y );
  }

  if(yi)
    ydim_yortIterator_destroy(yi, &cx->env_yeevctlcx);

  if( cre )
    ysREFree( cre );

  return TRUE;
}




	    
STATICF boolean yeevctlReceivers( yeevctlcx *cx, char *prod, char *regexp )
{
  yeevReceiverList rlist;
  ub4	i;
  char *recname;
  yeev pev;
  ysre *noreg cre;
  char *rs;

  NOREG( cre );

  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  cre = regexp && *regexp ? ysRECompile(regexp, FALSE) : (ysre*)0;

  yseTry
      yeevctlNarrow( (dvoid*)pev, yeev__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Prod %s doesn't denote a known yeev object\n",
	     yeevctlStr(prod));
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  rlist = yeev__get_receivers( pev, &cx->env_yeevctlcx );
  if( cx->inter_yeevctlcx )
  {
    yslPrint("Receivers in producer idx %d\n",
	     yeevctlObjIdx(cx, (dvoid*)pev));
    yslPrint("%-8s %s\n", "Receiver", "Name" );
    yslPrint("%-8s %s\n", "--------", "--------------------------------" );
  }
  else
  {
    rs = yoRefToStr( (dvoid*)pev );
    yslPrint("Receivers in producer %s\n", rs );
    yoFree( (dvoid*)rs );
    yslPrint("ReceiverRef Name\n");
  }
  

  for( i = 0; i < rlist._length ; i++ )
  {
    yeevctlNarrow( (dvoid*)rlist._buffer[i], yeevReceiver__id );
    recname = yeevReceiver__get_name(rlist._buffer[i], &cx->env_yeevctlcx);
    if( cre ? ysREMatch( cre, recname, TRUE ) : TRUE )
    {
      if( cx->inter_yeevctlcx )
      {
	yslPrint("%8d %s\n",
		 yeevctlObjIdx(cx, (dvoid*)rlist._buffer[i]), recname);
      }
      else
      {
	rs = yoRefToStr( (dvoid*)rlist._buffer[i] );
	yslPrint("%s %s\n", rs, recname );
	yoFree( (dvoid*)rs );
      }
    }
    yoFree( (dvoid*)recname );
  }
  yeevReceiverList__free( &rlist, yoFree );
  if( cre )
    ysREFree( cre );
  return TRUE;
}

STATICF boolean yeevctlLogs( yeevctlcx *cx, char *logid )
{
  yeevl_yeevlList llist;
  ub4	i;
  char *recname;
  yeevl_logProc log;
  char *rs;

  log = (yeevl_logProc)yeevctlProdLookup( cx, logid, TRUE );
  if( !log )
    return FALSE;


  
  llist = yeevl_logProc__get_logs( log, &cx->env_yeevctlcx );
  if( cx->inter_yeevctlcx )
  {
    yslPrint("Logs in log idx %d\n", yeevctlObjIdx(cx, (dvoid*)log));
    yslPrint("%-8s %s\n", "Log Idx", "Name" );
    yslPrint("%-8s %s\n", "--------", "--------------------------------" );
  }
  else
  {
    rs = yoRefToStr( (dvoid*)log );
    yslPrint("Logs in log %s\n", rs );
    yoFree( (dvoid*)rs ); 
    yslPrint("%s %s\n", "LogRef", "Name" );
  }

  for( i = 0; i < llist._length ; i++ )
  {
    recname = yeevl_log__get_name(llist._buffer[i], &cx->env_yeevctlcx);
    if( cx->inter_yeevctlcx )
    {
      yslPrint("%8d %s\n", yeevctlObjIdx(cx, (dvoid*)llist._buffer[i]),
	       recname);
    }
    else
    {
      rs = yoRefToStr( (dvoid*)llist._buffer[i] );
      yslPrint("%s %s\n", rs, recname );
      yoFree( (dvoid*)rs );
    }
    yoFree( (dvoid*)recname );
  }
  yeevl_yeevlList__free( &llist, yoFree );
  return TRUE;
}


STATICF boolean yeevctlDefs( yeevctlcx *cx, char *logid )
{
  yeevl_logProc log;
  yeevdSeq dlvals;
  yeevDiscList dl;
  char *rs;

  log = (yeevl_logProc)yeevctlProdLookup( cx, logid, TRUE );
  yseTry
  {
    if( !log )
      yseThrow(YS_EX_BADPARAM);
    yeevctlNarrow( (dvoid*)log, yeevl_logProc__id );
  }
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Log %s doesn't denote a known yeevl_logProc object\n",
	     yeevctlStr(logid) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;


  dl = yeevl_logProc__get_defQuals( log, &cx->env_yeevctlcx );
  dlvals = yeevDiscList__get_dlist( dl, &cx->env_yeevctlcx );


  if( cx->inter_yeevctlcx )
  {
    yslPrint("Default filters from logger idx %d\n",
	     yeevctlObjIdx(cx, (dvoid*)log));
  }
  else
  {
    rs = yoRefToStr( (dvoid*)log );
    yslPrint("Default filters from logger %s\n", rs );
    yoFree( (dvoid*)rs );
  }
  yeevctlShowFilters( cx, &dlvals );
  yoRelease( (dvoid*)dl );
  yeevdSeq__free( &dlvals, yoFree );
  return TRUE;
}




STATICF boolean yeevctlSetDef( yeevctlcx *cx, char *logid, char *consid,
			      yslst *filters )
{
  yeevl_logProc log;
  yeevdSeq dlvals;
  yeevDiscList dl;
  ub4 i;
  dvoid *e;

  log = (yeevl_logProc)yeevctlProdLookup( cx, logid, TRUE );

  if( !log || !filters || !ysLstCount(filters) )
  {
    yslError("Incomplete args for setdef operation\n");
    return( FALSE );
  }

  yseTry
  {
    if( !log )
      yseThrow(YS_EX_BADPARAM);
    yeevctlNarrow( (dvoid*)log, yeevl_logProc__id );
  }
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Log %s doesn't denote a known yeevl_logProc object\n",
	     yeevctlStr(logid) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;


  dl = yeevl_logProc__get_defQuals( log, &cx->env_yeevctlcx );

  dlvals._maximum = dlvals._length = 1;
  dlvals._buffer = (yeevd*)ysmGlbAlloc( sizeof(yeevd) *
				       (size_t)ysLstCount(filters),
				       "setdefs buf" );
  for( i = 0; (e = ysLstDeq( filters ) ); i++ )
  {
    dlvals._buffer[ i ].qual_yeevd = (char*)e;
    dlvals._buffer[ i ].dest_yeevd = (yeevReceiver)log;
  }
    
  yeevDiscList_replace( dl, &cx->env_yeevctlcx, &dlvals );

  ysmGlbFree( (dvoid*)dlvals._buffer );
  yoRelease( (dvoid*)dl );
  return TRUE;
}






STATICF boolean yeevctlSet( yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters )
{
  yeevDiscList dlobj;
  yeevdSeq nseq;
  yeevReceiver noreg cev;
  yeev pev;

  NOREG(cev);

  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  cev = cons && *cons ? yeevctlConsLookup( cx, cons ) : (yeevReceiver)0;

  if( !pev || !cev || !filters || !ysLstCount(filters) )
  {
    yslError("Incomplete args for set operation\n");
    return( FALSE );
  }

  yseTry
    yeevctlNarrow( (dvoid*)pev, yeev__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Prod %s doesn't denote a known yeev object\n",
	     yeevctlStr(prod) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  yseTry
      yeevctlNarrow( (dvoid*)cev, yeevReceiver__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Cons %s doesn't denote a known yeevReceiver object\n",
	     yeevctlStr(cons) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  
  dlobj = yeev__get_filters( pev, &cx->env_yeevctlcx );

  nseq._maximum = ysLstCount(filters);
  nseq._buffer =
    (yeevd*)ysmGlbAlloc((size_t)nseq._maximum * sizeof(yeevd), "nseq");
  for( nseq._length = 0; nseq._length < nseq._maximum; nseq._length++ )
  {
    nseq._buffer[nseq._length].qual_yeevd = (char*)ysLstDeq( filters );
    nseq._buffer[nseq._length].dest_yeevd = cev;      
  }

  yeevDiscList_replaceDest( dlobj, &cx->env_yeevctlcx, &nseq );
  yoRelease( (dvoid*)dlobj );
  ysmGlbFree( (dvoid*)nseq._buffer );

  return TRUE;
}





STATICF boolean yeevctlAdd( yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters )
{
  yeevDiscList dlobj;
  yeevdSeq nseq;
  yeevReceiver noreg cev;
  yeev pev;

  NOREG(cev);

  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  cev = cons && *cons ? yeevctlConsLookup( cx, cons ) : (yeevReceiver)0;

  if( !pev || !cev || !filters || !ysLstCount(filters) )
  {
    yslError("incomplete arguments to add.\n");
    return FALSE;
  }

  yseTry
    yeevctlNarrow( (dvoid*)pev, yeev__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Prod %s doesn't denote a known yeev object\n",
	     yeevctlStr(prod) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  yseTry
      yeevctlNarrow( (dvoid*)cev, yeevReceiver__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Cons %s doesn't denote a known yeevReceiver object\n",
	     yeevctlStr(cons) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;
  
  
  dlobj = yeev__get_filters( pev, &cx->env_yeevctlcx );

  nseq._maximum = ysLstCount(filters);
  nseq._buffer =
    (yeevd*)ysmGlbAlloc((size_t)nseq._maximum * sizeof(yeevd*), "nseq");
  for( nseq._length = 0; nseq._length < nseq._maximum; nseq._length++ )
  {
    nseq._buffer[nseq._length].qual_yeevd = (char*)ysLstDeq( filters );
    nseq._buffer[nseq._length].dest_yeevd = cev;      
  }

  yeevDiscList_append( dlobj, &cx->env_yeevctlcx, &nseq );
  yoRelease( (dvoid*)dlobj );
  ysmGlbFree( (dvoid*)nseq._buffer );

  return TRUE;
}





STATICF boolean yeevctlDel( yeevctlcx *cx, char *prod, char *cons,
			   yslst *filters )
{
  yeevDiscList dlobj;
  yeevd	evd;
  yeevReceiver noreg cev;
  yeev pev;

  NOREG(cev);

  pev = (yeev)yeevctlProdLookup( cx, prod, FALSE );
  cev = cons && *cons ? yeevctlConsLookup( cx, cons ) : (yeevReceiver)0;

  if( !pev || !cev )
  {
    yslError("incomplete arguments to delete.\n");
    return FALSE;
  }

  yseTry
    yeevctlNarrow( (dvoid*)pev, yeev__id );
  yseCatch( YS_EX_BADPARAM )
  {
    yslError("Prod %s doesn't denote a known yeev object\n",
	     yeevctlStr(prod) );
    yseThrow( YEEVCTL_EX_CAUGHT );
  }
  yseEnd;

  if( cons )
  {
    yseTry
      yeevctlNarrow( (dvoid*)cev, yeevReceiver__id );
    yseCatch( YS_EX_BADPARAM )
    {
      yslError("Cons %s doesn't denote a known yeevReceiver object\n",
	       yeevctlStr(cons) );
      yseThrow( YEEVCTL_EX_CAUGHT );
    }
    yseEnd;
  }

  
  dlobj = yeev__get_filters( pev, &cx->env_yeevctlcx );

  if( filters && ysLstCount( filters ) )
  {
    while( ysLstCount( filters ) )
    {
      evd.qual_yeevd = (char*)ysLstDeq( filters );
      evd.dest_yeevd = cev;
      yeevDiscList_remove( dlobj, &cx->env_yeevctlcx, &evd );
    }
  }
  else
  {
    yeevDiscList_destroyDest( dlobj, &cx->env_yeevctlcx, cev );
  }
  
  yoRelease( (dvoid*)dlobj );
  return TRUE;
}



STATICF void yeevctlShowFilters( yeevctlcx *cx, yeevdSeq *dlvals )
{
  char *rs;
  noreg ub4 i;
  char *noreg recname = (char*)0;
  noreg boolean logger = FALSE;

  NOREG(logger);
  NOREG(recname);
  NOREG(i);

  if( cx->inter_yeevctlcx )
  {
    yslPrint("%-52s %-8s %s\n",
	     "Filter String",  "Dest Idx",	   "Dest Name" );
    yslPrint("%-52s %-8s %s\n",
	     "----------------------------------------------------",
	     "--------", "------------" );
  }
  else
  {
    yslPrint("FilterString DestRef DestName\n");
  }
  for( i = 0; i < dlvals->_length ; i++ )
  {
    yseTry
    {
      DISCARD yoNarrow( (dvoid*)dlvals->_buffer[i].dest_yeevd,
		       yeevReceiver__id );
      recname = yeevReceiver__get_name( dlvals->_buffer[i].dest_yeevd,
				       &cx->env_yeevctlcx );
    }
    yseCatch( YS_EX_BADPARAM )
    {
      recname = (char*)0;
      logger = TRUE;
    }
    yseEnd;
    if( cx->inter_yeevctlcx )
    {
      yslPrint("%-52s %8d %s\n",
	       yeevctlStr(dlvals->_buffer[i].qual_yeevd),
	       yeevctlObjIdx(cx, (dvoid*)dlvals->_buffer[i].dest_yeevd),
	       logger ? "<logger>" : yeevctlStr(recname));
    }
    else
    {
      rs = yoRefToStr( (dvoid*)dlvals->_buffer[i].dest_yeevd );
      yslPrint("%s %s %s\n",
	       yeevctlStr(dlvals->_buffer[i].qual_yeevd),
	       rs, 
	       logger ? "<logger>" : yeevctlStr(recname));
      yoFree( (dvoid*)rs );
    }
    if( recname )
      yoFree( (dvoid*)recname );
  }
}



STATICF sb4 yeevctlObjIdx( yeevctlcx *cx, dvoid *obj )
{
  ysspNode *n;
  yeevctln *on;
  ysevt	*evt;
    
  
  if( !obj )
    return 0;

  if( (n = ysspLookup( (dvoid*)obj, &cx->ot_yeevctlcx ) ) )
    on = (yeevctln*)n;
  else
  {
    on = (yeevctln*)ysmGlbAlloc(sizeof(*on), "yeevctln");
    on->onode_yeevctln.key_ysspNode = yoDuplicate((dvoid*)obj);
    on->inode_yeevctln.key_ysspNode = (dvoid*)on;
    on->idx_yeevctln = ++cx->nt_yeevctlcx;
    on->cx_yeevctln = cx;
    DISCARD ysspEnq( &on->onode_yeevctln, &cx->ot_yeevctlcx );
    DISCARD ysspEnq( &on->inode_yeevctln, &cx->it_yeevctlcx );

    evt = ysEvtCreate( yeevctlDeath, (dvoid*)on, cx->q_yeevctlcx, FALSE );
    yoWatchOwner( obj, evt );
  }
  return( on->idx_yeevctln );
}

    
STATICF dvoid *yeevctlObjLookup( yeevctlcx *cx, ub4 idx )
{
  ysspNode *n;
  yeevctln *on;
  dvoid *rv = (dvoid*)0;
  yeevctln ln;

  ln.idx_yeevctln = (sb4)idx;

  if( (n = ysspLookup( (dvoid*)&ln, &cx->it_yeevctlcx ) ) )
  {
    on = (yeevctln*)n->key_ysspNode;
    rv = on->onode_yeevctln.key_ysspNode;
  }
  return( rv );
}


STATICF sword yeevctlObjNodeCmp( CONST dvoid *a, CONST dvoid *b )
{
  yeevctln *ap = (yeevctln*)a;
  yeevctln *bp = (yeevctln*)b;
    
  return( ap->idx_yeevctln < bp->idx_yeevctln ? -1 :
	 ap->idx_yeevctln == bp->idx_yeevctln ? 0 : 1 );
}



STATICF void yeevctlDeath( dvoid *usrp, CONST ysid *exit, dvoid *arg,
			  size_t argsz )
{
  yeevctln *on = (yeevctln*)usrp;
  yeevctlcx *cx = on->cx_yeevctln;
  
  ysspRemove( &on->onode_yeevctln, &cx->ot_yeevctlcx );
  ysspRemove( &on->inode_yeevctln, &cx->it_yeevctlcx );
  yoRelease( on->onode_yeevctln.key_ysspNode );
  ysmGlbFree( (dvoid*)on );
}


