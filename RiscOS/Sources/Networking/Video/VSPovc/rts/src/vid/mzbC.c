/* GENERATED FILE
 * mzb - client stubs
 * from /vobs/rts/pub/mzb.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MZB_IDL
#include <mzb.h>
#endif

EXTC_START

/* Client stubs for interface ::mzb::image */
static ysidDecl(mzb_image___id) = "IDL:mzb/image:1.0";

CONST ysid* mzb_image__getId(void)
{
  return (CONST ysid*)mzb_image___id;
}

static CONST_DATA yotk mzb_image__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'-',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x12,'I','D','L',':','m','z','b','/','i','m',
  'a','g','e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x0d
  ,':',':','m','z','b',':',':','i','m','a','g','e',0x00};

yotk* mzb_image__getTC(void)
{
  return (yotk*)mzb_image__tc;
}


void mzb_image__free( mzb_image* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzb_image, (dvoid *)val, ffunc);
}

void mzb_image__copy( mzb_image* dest, mzb_image* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mzb_image, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mzb_image_err__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,0xc4,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x16,'I','D','L',':','m','z','b','/','i','m'
  ,'a','g','e','/','e','r','r',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x12,':',':','m','z','b',':',':','i','m','a',
  'g','e',':',':','e','r','r',0x00,0x00,0x00,0x00,0x00,0x00,0x09
  ,0x00,0x00,0x00,0x08,'e','r','r','S','u','c','c',0x00,0x00,0x00
  ,0x00,0x0d,'e','r','r','B','a','d','C','l','i','e','n','t',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,'e','r','r','B','u','f',
  'S','m','a','l','l',0x00,0x00,0x00,0x00,0x0b,'e','r','r','B'
  ,'a','d','F','i','l','e',0x00,0x00,0x00,0x00,0x00,0x09,'e','r'
  ,'r','N','o','M','e','m',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0b,'e','r','r','B','a','d','P','h','y','s',0x00,0x00,0x00,
  0x00,0x00,0x09,'e','r','r','R','c','I','n','t',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x08,'e','r','r','I','n','f','o',0x00,0x00
  ,0x00,0x00,0x08,'e','r','r','L','a','s','t',0x00};

yotk* mzb_image_err__getTC(void)
{
  return (yotk*)mzb_image_err__tc;
}

void mzb_image_err__free( mzb_image_err* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzb_image_err, (dvoid *)val, ffunc);
}

void mzb_image_err__copy( mzb_image_err* dest, mzb_image_err* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mzb_image_err, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mzb_image_mnnpa__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'|',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x18,'I','D','L',':','m','z','b','/','i','m',
  'a','g','e','/','m','n','n','p','a',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x14,':',':','m','z','b',':',':','i','m','a','g',
  'e',':',':','m','n','n','p','a',0x00,0x00,0x00,0x00,0x02,0x00
  ,0x00,0x00,0x07,'f','a','m','i','l','y',0x00,0x00,0x00,0x00,
  0x00,0x14,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x0a,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x05,'a','d','d'
  ,'r',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
  0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,
  0x14};

yotk* mzb_image_mnnpa__getTC(void)
{
  return (yotk*)mzb_image_mnnpa__tc;
}

void mzb_image_mnnpa__free( mzb_image_mnnpa* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzb_image_mnnpa, (dvoid *)val, ffunc);
}

void mzb_image_mnnpa__copy( mzb_image_mnnpa* dest, mzb_image_mnnpa* src, 
  ysmaf afunc)
{
  yotkCopyVal( YCTC_mzb_image_mnnpa, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mzb_image_setPhys__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mzb_image_setPhys");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mzb_image_setPhys", (ub4)4);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mzb_image_err;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)YCTC_mzb_image_mnnpa;
    _pars[2].mode = YOMODE_IN;
    _pars[2].tk = (yotk*)yoTcString;
    _pars[3].mode = YOMODE_IN;
    _pars[3].tk = (yotk*)yoTcUshort;
    _pars[4].mode = YOMODE_INVALID;
    _pars[4].tk = (yotk*)yoTcNull;
    _pars[4].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mzb_image_err mzb_image_setPhys( mzb_image or, yoenv* ev, mzb_image_mnnpa* 
  phys, char* image, ub2 howmany)
{
  mzb_image_err _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mzb_image__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mzb_image__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mzb_image__id);
      _result = (*(mzb_image_err (*)( mzb_image, yoenv*, mzb_image_mnnpa*, 
        char*, ub2))_f)(or, ev, phys, image, howmany);
    }
    else
      _result = (*_impl->setPhys)(or, ev, phys, image, howmany);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mzb_image_setPhys_nw( or, ev, phys, image, howmany, (ysevt*)_sem);
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

void mzb_image_setPhys_nw( mzb_image or, yoenv* ev, mzb_image_mnnpa* phys, 
  char* image, ub2 howmany, ysevt* _sem)
{
  dvoid * _parvec[3];

  _parvec[0] = (dvoid *) phys;
  _parvec[1] = (dvoid *)&image;
  _parvec[2] = (dvoid *)&howmany;
  yoSendReq( (dvoid *)or, ev, "setPhys", TRUE, _sem, (sword)3, 
    mzb_image_setPhys_pars, _parvec);
}

yopar* mzb_image_get__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mzb_image_get");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mzb_image_get", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mzb_image_err;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)YCTC_mzb_image_mnnpa;
    _pars[2].mode = YOMODE_OUT;
    _pars[2].tk = (yotk*)yoTcString;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mzb_image_err mzb_image_get( mzb_image or, yoenv* ev, mzb_image_mnnpa* phys,
   char** image)
{
  mzb_image_err _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mzb_image__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mzb_image__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, mzb_image__id);
      _result = (*(mzb_image_err (*)( mzb_image, yoenv*, mzb_image_mnnpa*, 
        char**))_f)(or, ev, phys, image);
    }
    else
      _result = (*_impl->get)(or, ev, phys, image);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mzb_image_get_nw( or, ev, phys, image, (ysevt*)_sem);
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

void mzb_image_get_nw( mzb_image or, yoenv* ev, mzb_image_mnnpa* phys, 
  char** image, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *) phys;
  _parvec[1] = (dvoid *) image;
  yoSendReq( (dvoid *)or, ev, "get", TRUE, _sem, (sword)2, 
    mzb_image_get_pars, _parvec);
}

yopar* mzb_image_updateDB__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mzb_image_updateDB");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mzb_image_updateDB", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mzb_image_err;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_IN;
    _pars[2].tk = (yotk*)yoTcBoolean;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mzb_image_err mzb_image_updateDB( mzb_image or, yoenv* ev, char* fname, 
  boolean flush)
{
  mzb_image_err _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mzb_image__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mzb_image__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, mzb_image__id);
      _result = (*(mzb_image_err (*)( mzb_image, yoenv*, char*, boolean))_f)
        (or, ev, fname, flush);
    }
    else
      _result = (*_impl->updateDB)(or, ev, fname, flush);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mzb_image_updateDB_nw( or, ev, fname, flush, (ysevt*)_sem);
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

void mzb_image_updateDB_nw( mzb_image or, yoenv* ev, char* fname, boolean 
  flush, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *)&fname;
  _parvec[1] = (dvoid *)&flush;
  yoSendReq( (dvoid *)or, ev, "updateDB", TRUE, _sem, (sword)2, 
    mzb_image_updateDB_pars, _parvec);
}

yopar* mzb_image_dumpMappings__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mzb_image_dumpMappings");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mzb_image_dumpMappings", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_mzb_image_err;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

mzb_image_err mzb_image_dumpMappings( mzb_image or, yoenv* ev)
{
  mzb_image_err _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct mzb_image__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mzb_image__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)3, (dvoid *)_impl, mzb_image__id);
      _result = (*(mzb_image_err (*)( mzb_image, yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->dumpMappings)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mzb_image_dumpMappings_nw( or, ev, (ysevt*)_sem);
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

void mzb_image_dumpMappings_nw( mzb_image or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "dumpMappings", TRUE, _sem, (sword)0, 
    mzb_image_dumpMappings_pars, (dvoid **)0);
}



EXTC_END
