/* GENERATED FILE
 * mtcr - client stubs
 * from /vobs/rts/pub/mtcr.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MTCR_IDL
#include <mtcr.h>
#endif

EXTC_START

static ysidDecl(mtcr_noImpl___id) = "IDL:mtcr/noImpl:1.0";

CONST ysid* mtcr_noImpl__getId(void)
{
  return (CONST ysid*)mtcr_noImpl___id;
}

static CONST_DATA yotk mtcr_noImpl__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'4',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x14,'I','D','L',':','m','t','c','r','/','n',
  'o','I','m','p','l',':','1','.','0',0x00,0x00,0x00,0x00,0x0f
  ,':',':','m','t','c','r',':',':','n','o','I','m','p','l',0x00
  ,0x00,0x00,0x00,0x00,0x00};

yotk* mtcr_noImpl__getTC(void)
{
  return (yotk*)mtcr_noImpl__tc;
}

static ysidDecl(mtcr_badImpl___id) = "IDL:mtcr/badImpl:1.0";

CONST ysid* mtcr_badImpl__getId(void)
{
  return (CONST ysid*)mtcr_badImpl___id;
}

static CONST_DATA yotk mtcr_badImpl__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'P',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','c','r','/','b',
  'a','d','I','m','p','l',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','c','r',':',':','b','a',
  'd','I','m','p','l',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
  0x09,'i','m','p','l','N','a','m','e',0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x12,0x00,0x00,0x00,0x00};

yotk* mtcr_badImpl__getTC(void)
{
  return (yotk*)mtcr_badImpl__tc;
}

void mtcr_badImpl__free( mtcr_badImpl* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtcr_badImpl, (dvoid *)val, ffunc);
}

void mtcr_badImpl__copy( mtcr_badImpl* dest, mtcr_badImpl* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtcr_badImpl, (dvoid *)dest, (dvoid *)src, afunc);
}

static ysidDecl(mtcr_badName___id) = "IDL:mtcr/badName:1.0";

CONST ysid* mtcr_badName__getId(void)
{
  return (CONST ysid*)mtcr_badName___id;
}

static CONST_DATA yotk mtcr_badName__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'L',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','c','r','/','b',
  'a','d','N','a','m','e',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','c','r',':',':','b','a',
  'd','N','a','m','e',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
  0x08,'t','h','e','N','a','m','e',0x00,0x00,0x00,0x00,0x12,0x00
  ,0x00,0x00,0x00};

yotk* mtcr_badName__getTC(void)
{
  return (yotk*)mtcr_badName__tc;
}

void mtcr_badName__free( mtcr_badName* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtcr_badName, (dvoid *)val, ffunc);
}

void mtcr_badName__copy( mtcr_badName* dest, mtcr_badName* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtcr_badName, (dvoid *)dest, (dvoid *)src, afunc);
}

static ysidDecl(mtcr_authFailed___id) = "IDL:mtcr/authFailed:1.0";

CONST ysid* mtcr_authFailed__getId(void)
{
  return (CONST ysid*)mtcr_authFailed___id;
}

static CONST_DATA yotk mtcr_authFailed__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'<',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x18,'I','D','L',':','m','t','c','r','/','a',
  'u','t','h','F','a','i','l','e','d',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x13,':',':','m','t','c','r',':',':','a','u','t',
  'h','F','a','i','l','e','d',0x00,0x00,0x00,0x00,0x00,0x00};

yotk* mtcr_authFailed__getTC(void)
{
  return (yotk*)mtcr_authFailed__tc;
}

/* Client stubs for interface ::mtcr::resolve */
static ysidDecl(mtcr_resolve___id) = "IDL:mtcr/resolve:1.0";

CONST ysid* mtcr_resolve__getId(void)
{
  return (CONST ysid*)mtcr_resolve___id;
}

static CONST_DATA yotk mtcr_resolve__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'4',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','c','r','/','r',
  'e','s','o','l','v','e',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','c','r',':',':','r','e',
  's','o','l','v','e',0x00};

yotk* mtcr_resolve__getTC(void)
{
  return (yotk*)mtcr_resolve__tc;
}


void mtcr_resolve__free( mtcr_resolve* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtcr_resolve, (dvoid *)val, ffunc);
}

void mtcr_resolve__copy( mtcr_resolve* dest, mtcr_resolve* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtcr_resolve, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mtcr_resolve_name__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtcr_resolve_name");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtcr_resolve_name", (ub4)6);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mkd_segmentList;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_IN;
    _pars[2].tk = (yotk*)yoTcObject;
    _pars[3].mode = YOMODE_IN;
    _pars[3].tk = (yotk*)yoTcAny;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_mtcr_badName;
    _pars[5].mode = YOMODE_EXCEPT;
    _pars[5].tk = (yotk*)YCTC_mtcr_authFailed;
    _pars[6].mode = YOMODE_INVALID;
    _pars[6].tk = (yotk*)yoTcNull;
    _pars[6].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mkd_segmentList mtcr_resolve_name( mtcr_resolve or, yoenv* ev, char* 
  resName, CORBA_Object authRef, yoany* destination)
{
  mkd_segmentList _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mtcr_resolve__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtcr_resolve__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mtcr_resolve__id);
      _result = (*(mkd_segmentList (*)( mtcr_resolve, yoenv*, char*, 
        CORBA_Object, yoany*))_f)(or, ev, resName, authRef, destination);
    }
    else
      _result = (*_impl->name)(or, ev, resName, authRef, destination);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtcr_resolve_name_nw( or, ev, resName, authRef, destination, (ysevt*)
        _sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void mtcr_resolve_name_nw( mtcr_resolve or, yoenv* ev, char* resName, 
  CORBA_Object authRef, yoany* destination, ysevt* _sem)
{
  dvoid * _parvec[3];

  _parvec[0] = (dvoid *)&resName;
  _parvec[1] = (dvoid *)&authRef;
  _parvec[2] = (dvoid *) destination;
  yoSendReq( (dvoid *)or, ev, "name", TRUE, _sem, (sword)3, 
    mtcr_resolve_name_pars, _parvec);
}



EXTC_END
