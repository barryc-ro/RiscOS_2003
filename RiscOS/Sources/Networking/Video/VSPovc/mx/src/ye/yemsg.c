/* mx/src/ye/yemsg.c */


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
#ifndef YEMSG_ORACLE
#include <yemsg.h>
#endif
#ifndef YSSP_ORACLE
#include <yssp.h>
#endif
#ifndef SYSFP_ORACLE
#include <sysfp.h>
#endif
#ifndef SYSXCD_ORACLE
#include <sysxcd.h>
#endif
#ifndef YSMSG_ORACLE
#include <ysmsg.h>
#endif



typedef struct yemsgn yemsgn;

struct yemsgn
{
  ysspNode  node_yemsgn;	
  char	    *prod_yemsgn;	
  char	    *fac_yemsgn;	
  char	    *data_yemsgn;	
  size_t    offset_yemsgn;	
  ysmsgd    *msgd_yemsgn;
};

struct yemsgcx
{
  ysspTree  files_yemsgcx;
};

externdef ysidDecl( YEMSG_EX_FILE_ERR ) = "yemsg::fileError";

externdef ysmtagDecl(yemsgcx_tag) = "yemsgcx";
externdef ysmtagDecl(yemsgn_tag) = "yemsgn";



STATICF void yemsgLoad( yemsgcx *cx, char *prod, char *fac );
STATICF void yemsgUnload( yemsgcx *cx, char *prod, char *fac );
STATICF yemsgn *yemsgFind( yemsgcx *cx, char *prod, char *fac );
STATICF void yemsgDestroyNode( yemsgn *mn );
STATICF sword yemsgCmp( CONST dvoid *a , CONST dvoid *b );



yemsgcx *yemsgInit()
{
  yemsgcx *cx;

  cx = (yemsgcx*)ysmGlbAlloc( sizeof(*cx), yemsgcx_tag );
  DISCARD ysspNewTree( &cx->files_yemsgcx, yemsgCmp );

  return( cx );
}

void yemsgTerm( yemsgcx *cx )
{
  ysspNode *n;

  
  ysmCheck( cx, yemsgcx_tag );

  while( (n = ysspDeqTree( &cx->files_yemsgcx ) ) )
    yemsgDestroyNode( (yemsgn*)n );

  ysmGlbFree( (dvoid*)cx );
}


CONST char *yemsgLookup( yemsgcx *cx, char *prod, char *fac, ub4 msgId )
{
  yemsgn *mn;
  char *msg = (char*)0;
  size_t offset;

  
  ysmCheck( cx, yemsgcx_tag );

  if( !(mn = yemsgFind( cx, prod, fac ) ) )
  {
    yemsgLoad( cx, prod, fac );
    mn = yemsgFind( cx, prod, fac );
  }
  if( mn && msgId <= mn->msgd_yemsgn->idmax )
  {
    offset =
      (size_t)ysMsgOffset( mn->msgd_yemsgn, msgId ) - mn->offset_yemsgn;
    msg = mn->data_yemsgn + offset; 
  }
  return msg;
}





STATICF void yemsgLoad( yemsgcx *cx, char *prod, char *fac )
{
  yemsgn *mn = (yemsgn*)ysmGlbAlloc(sizeof(*mn), yemsgn_tag );
  size_t fsz;
  sysb8	dataoff;

  
  ysmCheck( cx, yemsgcx_tag );

  yemsgUnload( cx, prod, fac );

  if( !(mn->msgd_yemsgn = ysMsgOpen( prod, fac ) ) )
    return;

  fsz = (size_t)ysMsgDataLength( mn->msgd_yemsgn );
  mn->data_yemsgn = (char*)ysmGlbAlloc( fsz, "data_yemsgn");
  mn->offset_yemsgn = (size_t)ysMsgDataOffset( mn->msgd_yemsgn );

  sysb8ext( &dataoff, mn->offset_yemsgn );
  DISCARD sysfpSeek( mn->msgd_yemsgn->fp, &dataoff );
  if( fsz != sysfpRead( mn->msgd_yemsgn->fp, (dvoid*)mn->data_yemsgn, fsz ) )
  {
    ysmGlbFree( (dvoid*)mn->data_yemsgn );
    yslError("yemsg: error reading file for %s/%s\n", prod, fac );
    ysePanic( YEMSG_EX_FILE_ERR );
  }
  mn->prod_yemsgn = ysStrDup( prod );
  mn->fac_yemsgn = ysStrDup( fac );
  mn->node_yemsgn.key_ysspNode = (dvoid*)mn;
  DISCARD ysspEnq( &mn->node_yemsgn, &cx->files_yemsgcx );
}





STATICF void yemsgUnload( yemsgcx *cx, char *prod, char *fac )
{
  yemsgn *mn;

  if( (mn = yemsgFind( cx, prod, fac ) ) )
  {
    ysspRemove( &mn->node_yemsgn, &cx->files_yemsgcx );
    yemsgDestroyNode( mn );
  }
}


STATICF void yemsgDestroyNode( yemsgn *mn )
{
  ysMsgClose( mn->msgd_yemsgn );
  ysmGlbFree( (dvoid*)mn->data_yemsgn );
  ysmGlbFree( (dvoid*)mn->prod_yemsgn );
  ysmGlbFree( (dvoid*)mn->fac_yemsgn );
  ysmGlbFree( (dvoid*)mn );
}




STATICF yemsgn *yemsgFind( yemsgcx *cx, char *prod, char *fac )
{
  yemsgn lookup;
  lookup.prod_yemsgn = prod;
  lookup.fac_yemsgn = fac;

  return( (yemsgn*)ysspLookup( (dvoid*)&lookup, &cx->files_yemsgcx ) );
}





STATICF sword yemsgCmp( CONST dvoid *a, CONST dvoid *b )
{
  sword rv;
  yemsgn *ap = (yemsgn*)a;
  yemsgn *bp = (yemsgn*)b;

  if( !(rv = (sword)strcmp( ap->prod_yemsgn, bp->prod_yemsgn )) )
    rv = (sword)strcmp( ap->fac_yemsgn, bp->fac_yemsgn );

  return( rv );
}


