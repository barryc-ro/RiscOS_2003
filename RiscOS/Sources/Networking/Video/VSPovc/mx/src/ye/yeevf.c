/* mx/src/ye/yeevf.c */


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
#ifndef YEEVF_ORACLE
#include <yeevf.h>
#endif
#ifndef YEEVENT_ORACLE
#include <yeevent.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif
#ifndef YSMSG_ORACLE
#include <ysmsg.h>
#endif
#ifndef CTYPE_ORACLE
#include <ctype.h>
#endif
#ifndef YSTM_ORACLE
#include <ystm.h>
#endif



#define YEEVF_TIME_DIGS	((sword)3)



STATICF void yeevfCrunch( char *s );

externdef ysidDecl( YEEVF_EX_BAD_TYPECODE ) = "yeevf:badTypecode";






ysstr *yeevFormat( yemsgcx *msgcx, char *prod, char *fac, ub4 msgid,
		  yoany *ap, boolean crunch )
{

  ysstr *rv = (ysstr*)0;	
  CONST char *tn = (char*)0;	
  YCIDL_sequence_yoany *seqany;	
  char *fmt = (char*)0;		
  yeevYsLogEvent *yevt;		

  char	lbuf[ 1024 ]; 

  seqany = (YCIDL_sequence_yoany*)0;

  if( msgid )
    fmt = (char*)yemsgLookup( msgcx, prod, fac, msgid );

  yseTry
  {
    if( !yotkCmp( ap->_type, YCTC_yeevSeqAny ) )
      seqany = (YCIDL_sequence_yoany*)ap->_value;
    else if((tn = yotkGetName( ap->_type )) && !strcmp( tn, "::yeevYsLogEvent"))
    {
      
      tn = (char*)0;
      yevt = (yeevYsLogEvent*)ap->_value;
      seqany = (YCIDL_sequence_yoany*)&yevt->vals;
    }
  }
  yseCatch( YO_EX_BADCODE )  ;
  yseEnd;

  if( seqany )
  {
    ysMsgFmt( lbuf, sizeof(lbuf), (sword)seqany->_length,
	     (yslarg*)seqany->_buffer, fmt );
  }
  else if( !yotkCmp(ap->_type, yoTcString ) )
  {
    DISCARD strcpy( lbuf, *(char**)ap->_value );
  }
  else
    ysMsgFmt( lbuf, sizeof(lbuf), 1, (yslarg*)ap, fmt );

  
  rv = ysStrCreate( "{ " );
  rv = ysStrCat( rv, lbuf ); 
  rv = ysStrCat( rv, " }, " ); 

  if( crunch )
    yeevfCrunch( ysStrToText(rv) );

  return( rv );
}


void yeevShowAny( yotk *type, dvoid *value )
{
  char *s;

  s = yotkFormat( type, value );
  yslPrint("%s\n", s );
  ysmGlbFree( (dvoid*)s );
}


void yeevShowRec( yemsgcx *msgcx, yeevr *rec, char *buf, size_t bsiz )
{
  boolean dofree = FALSE;
  ysstr	*sp;
  ystm	tm;
  char *assoc;
  char tbuf[ YSTM_BUFLEN ];
  if( !buf )
  {
    buf = (char *)ysmGlbAlloc( 32*1024, "showrec buf" );
    bsiz = 32*1024;
    dofree = TRUE;
  }

  ysConvClock( (sysb8*)&rec->origtime_yeevr, &tm );
  DISCARD ysStrClock( tbuf, &tm, FALSE, YEEVF_TIME_DIGS );
  sp = yeevFormat( msgcx, rec->prod_yeevr, rec->fac_yeevr,
		  rec->msgid_yeevr, &rec->val_yeevr, TRUE);
  assoc = rec->assoc_yeevr && *rec->assoc_yeevr ? rec->assoc_yeevr : (char*)0;

  sp = yeevFormat( msgcx, rec->prod_yeevr, rec->fac_yeevr,
		  rec->msgid_yeevr, &rec->val_yeevr, TRUE);
  assoc = rec->assoc_yeevr && *rec->assoc_yeevr ? rec->assoc_yeevr : (char*)0;
  ysFmtStrl(buf, bsiz, "%s %d %s:%s%s%s %s %s-%d %d %s%s%s%s",
	    tbuf, 
	    rec->origseq_yeevr,
	    rec->orighost_yeevr,
	    rec->origpid_yeevr,
	    rec->origaff_yeevr ? ":" : "",
	    rec->origaff_yeevr ? rec->origaff_yeevr : "",
	    rec->origprog_yeevr,
	    rec->prod_yeevr,
	    rec->msgid_yeevr,
	    rec->sev_yeevr,
	    assoc ? "[" : "",
	    assoc ? assoc : "",
	    assoc ? "] " : "",
	    ysStrToText(sp) );
  ysStrDestroy( sp );
  yslPrint("%s\n", buf );

  if( dofree )
    ysmGlbFree( (dvoid*)buf );
}

void yeevShowLRec( yemsgcx *msgcx, yeevl_yeevlr *lrec,
		  char *buf, size_t bsiz )
{

  boolean dofree = FALSE;
  yeevr *rec = &lrec->record;
  ysstr	*sp;
  char *assoc;
  ystm	tm;
  char tbuf[ YSTM_BUFLEN ];

  if( !buf )
  {
    buf = (char*)ysmGlbAlloc( 32*1024, "showrec buf" );
    bsiz = 32*1024;
    dofree = TRUE;
  }

  ysConvClock( (sysb8*)&rec->origtime_yeevr, &tm );
  DISCARD ysStrClock( tbuf, &tm, FALSE, YEEVF_TIME_DIGS );
  sp = yeevFormat( msgcx, rec->prod_yeevr, rec->fac_yeevr,
		  rec->msgid_yeevr, &rec->val_yeevr, TRUE);
  assoc = rec->assoc_yeevr && *rec->assoc_yeevr ? rec->assoc_yeevr : (char*)0;
  ysFmtStrl(buf, bsiz, "%s %d %s:%s%s%s %s %s-%d %d %s%s%s%d %s",
	    tbuf,
	    rec->origseq_yeevr,
	    rec->orighost_yeevr,
	    rec->origpid_yeevr,
	    rec->origaff_yeevr ? ":" : "",
	    rec->origaff_yeevr ? rec->origaff_yeevr : "",
	    rec->origprog_yeevr,
	    rec->prod_yeevr,
	    rec->msgid_yeevr,
	    rec->sev_yeevr,
	    assoc ? "[" : "",
	    assoc ? assoc : "",
	    assoc ? "] " : "",
	    lrec->record_id,
	    ysStrToText(sp) );
  ysStrDestroy( sp );
  yslPrint("%s\n", buf );

  if( dofree )
    ysmGlbFree( (dvoid*)buf );
}







STATICF void yeevfCrunch( char *s )
{
  char *p, *beg;
  boolean wasspace;

  beg = p = s;
  while( *s )
  {
    if( *s == '\n')
      *s = ' ';

    wasspace = isspace(*s);
    *p++ = *s++;

    
    while( wasspace && *s && (isspace(*s) || *s == '\n') )
      s++;
  }

  
  while( p > beg && (p[-1] == ' ' || p[-1] ==',')  )
    p--;

  *p = 0;
}


