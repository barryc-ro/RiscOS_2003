/* GENERATED FILE
 * mkpmib - client stubs
 * from /vobs/rts/inc/mkpmib.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MKPMIB_IDL
#include <mkpmib.h>
#endif

EXTC_START

/* Client stubs for interface ::mkpmib::snmpStm */
static ysidDecl(mkpmib_snmpStm___id) = "IDL:mkpmib/snmpStm:1.0";

CONST ysid* mkpmib_snmpStm__getId(void)
{
  return (CONST ysid*)mkpmib_snmpStm___id;
}

static CONST_DATA yotk mkpmib_snmpStm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','m','k','p','m','i','b',
  '/','s','n','m','p','S','t','m',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','m','k','p','m','i','b',':',':','s',
  'n','m','p','S','t','m',0x00};

yotk* mkpmib_snmpStm__getTC(void)
{
  return (yotk*)mkpmib_snmpStm__tc;
}


void mkpmib_snmpStm__free( mkpmib_snmpStm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpmib_snmpStm, (dvoid *)val, ffunc);
}

void mkpmib_snmpStm__copy( mkpmib_snmpStm* dest, mkpmib_snmpStm* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mkpmib_snmpStm, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mkpmib_snmpStm_data__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0xac,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x1c,'I','D','L',':','m','k','p','m','i','b'
  ,'/','s','n','m','p','S','t','m','/','d','a','t','a',':','1'
  ,'.','0',0x00,0x00,0x00,0x00,0x18,':',':','m','k','p','m','i'
  ,'b',':',':','s','n','m','p','S','t','m',':',':','d','a','t'
  ,'a',0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0a,'s','n','m'
  ,'p','S','t','m','N','o',0x00,0x00,0x00,0x00,0x00,0x00,0x05,
  0x00,0x00,0x00,0x0b,'s','n','m','p','S','t','m','S','t','a',
  0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0b,'s','n','m'
  ,'p','S','t','m','N','a','m',0x00,0x00,0x00,0x00,0x00,0x12,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,'s','n','m','p','S','t',
  'm','L','e','n',0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x00,0x00
  ,0x0b,'s','n','m','p','S','t','m','P','o','s',0x00,0x00,0x00
  ,0x00,0x00,0x17};

yotk* mkpmib_snmpStm_data__getTC(void)
{
  return (yotk*)mkpmib_snmpStm_data__tc;
}

void mkpmib_snmpStm_data__free( mkpmib_snmpStm_data* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpmib_snmpStm_data, (dvoid *)val, ffunc);
}

void mkpmib_snmpStm_data__copy( mkpmib_snmpStm_data* dest, 
  mkpmib_snmpStm_data* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mkpmib_snmpStm_data, (dvoid *)dest, (dvoid *)src, afunc)
    ;
}


yopar* mkpmib_snmpStm_getMib__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mkpmib_snmpStm_getMib");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mkpmib_snmpStm_getMib", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mkpmib_snmpStm_data;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mkpmib_snmpStm_data mkpmib_snmpStm_getMib( mkpmib_snmpStm or, yoenv* ev)
{
  mkpmib_snmpStm_data _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mkpmib_snmpStm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mkpmib_snmpStm__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mkpmib_snmpStm__id);
      _result = (*(mkpmib_snmpStm_data (*)( mkpmib_snmpStm, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->getMib)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mkpmib_snmpStm_getMib_nw( or, ev, (ysevt*)_sem);
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

void mkpmib_snmpStm_getMib_nw( mkpmib_snmpStm or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "getMib", TRUE, _sem, (sword)0, 
    mkpmib_snmpStm_getMib_pars, (dvoid **)0);
}


/* Client stubs for interface ::mkpmib::snmpGbl */
static ysidDecl(mkpmib_snmpGbl___id) = "IDL:mkpmib/snmpGbl:1.0";

CONST ysid* mkpmib_snmpGbl__getId(void)
{
  return (CONST ysid*)mkpmib_snmpGbl___id;
}

static CONST_DATA yotk mkpmib_snmpGbl__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','m','k','p','m','i','b',
  '/','s','n','m','p','G','b','l',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','m','k','p','m','i','b',':',':','s',
  'n','m','p','G','b','l',0x00};

yotk* mkpmib_snmpGbl__getTC(void)
{
  return (yotk*)mkpmib_snmpGbl__tc;
}


void mkpmib_snmpGbl__free( mkpmib_snmpGbl* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpmib_snmpGbl, (dvoid *)val, ffunc);
}

void mkpmib_snmpGbl__copy( mkpmib_snmpGbl* dest, mkpmib_snmpGbl* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mkpmib_snmpGbl, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mkpmib_snmpGbl_data__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0xa8,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x1c,'I','D','L',':','m','k','p','m','i','b'
  ,'/','s','n','m','p','G','b','l','/','d','a','t','a',':','1'
  ,'.','0',0x00,0x00,0x00,0x00,0x18,':',':','m','k','p','m','i'
  ,'b',':',':','s','n','m','p','G','b','l',':',':','d','a','t'
  ,'a',0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0b,'s','n','m'
  ,'p','G','b','l','T','t','l',0x00,0x00,0x00,0x00,0x00,0x05,0x00
  ,0x00,0x00,0x0b,'s','n','m','p','G','b','l','P','l','y',0x00
  ,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0b,'s','n','m','p'
  ,'G','b','l','D','b','l',0x00,0x00,0x00,0x00,0x00,0x05,0x00,
  0x00,0x00,0x0b,'s','n','m','p','G','b','l','F','m','t',0x00,
  0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x0c,'s','n','m','p'
  ,'G','b','l','F','l','g','s',0x00,0x00,0x00,0x00,0x05};

yotk* mkpmib_snmpGbl_data__getTC(void)
{
  return (yotk*)mkpmib_snmpGbl_data__tc;
}

void mkpmib_snmpGbl_data__free( mkpmib_snmpGbl_data* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpmib_snmpGbl_data, (dvoid *)val, ffunc);
}

void mkpmib_snmpGbl_data__copy( mkpmib_snmpGbl_data* dest, 
  mkpmib_snmpGbl_data* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mkpmib_snmpGbl_data, (dvoid *)dest, (dvoid *)src, afunc)
    ;
}


yopar* mkpmib_snmpGbl_getMib__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mkpmib_snmpGbl_getMib");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mkpmib_snmpGbl_getMib", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mkpmib_snmpGbl_data;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mkpmib_snmpGbl_data mkpmib_snmpGbl_getMib( mkpmib_snmpGbl or, yoenv* ev)
{
  mkpmib_snmpGbl_data _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mkpmib_snmpGbl__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mkpmib_snmpGbl__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mkpmib_snmpGbl__id);
      _result = (*(mkpmib_snmpGbl_data (*)( mkpmib_snmpGbl, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->getMib)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mkpmib_snmpGbl_getMib_nw( or, ev, (ysevt*)_sem);
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

void mkpmib_snmpGbl_getMib_nw( mkpmib_snmpGbl or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "getMib", TRUE, _sem, (sword)0, 
    mkpmib_snmpGbl_getMib_pars, (dvoid **)0);
}



EXTC_END
