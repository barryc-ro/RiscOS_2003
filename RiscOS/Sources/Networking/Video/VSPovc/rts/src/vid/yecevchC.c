/* GENERATED FILE
 * yecevch - client stubs
 * from /vobs/mx/pub/yecevch.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef YECEVCH_IDL
#include <yecevch.h>
#endif

EXTC_START

static ysidDecl(yecec_Disconnected___id) = "IDL:yecec/Disconnected:1.0";

CONST ysid* yecec_Disconnected__getId(void)
{
  return (CONST ysid*)yecec_Disconnected___id;
}

static CONST_DATA yotk yecec_Disconnected__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1b,'I','D','L',':','y','e','c','e','c','/',
  'D','i','s','c','o','n','n','e','c','t','e','d',':','1','.',
  '0',0x00,0x00,0x00,0x00,0x00,0x16,':',':','y','e','c','e','c'
  ,':',':','D','i','s','c','o','n','n','e','c','t','e','d',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00};

yotk* yecec_Disconnected__getTC(void)
{
  return (yotk*)yecec_Disconnected__tc;
}

static ysidDecl(yecec_NothingToPull___id) = "IDL:yecec/NothingToPull:1.0";

CONST ysid* yecec_NothingToPull__getId(void)
{
  return (CONST ysid*)yecec_NothingToPull___id;
}

static CONST_DATA yotk yecec_NothingToPull__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1c,'I','D','L',':','y','e','c','e','c','/',
  'N','o','t','h','i','n','g','T','o','P','u','l','l',':','1',
  '.','0',0x00,0x00,0x00,0x00,0x17,':',':','y','e','c','e','c'
  ,':',':','N','o','t','h','i','n','g','T','o','P','u','l','l'
  ,0x00,0x00,0x00,0x00,0x00,0x00};

yotk* yecec_NothingToPull__getTC(void)
{
  return (yotk*)yecec_NothingToPull__tc;
}

/* Client stubs for interface ::yecec::PushCons */
static ysidDecl(yecec_PushCons___id) = "IDL:yecec/PushCons:1.0";

CONST ysid* yecec_PushCons__getId(void)
{
  return (CONST ysid*)yecec_PushCons___id;
}

static CONST_DATA yotk yecec_PushCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','c','/',
  'P','u','s','h','C','o','n','s',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','c',':',':','P','u',
  's','h','C','o','n','s',0x00};

yotk* yecec_PushCons__getTC(void)
{
  return (yotk*)yecec_PushCons__tc;
}


void yecec_PushCons__free( yecec_PushCons* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yecec_PushCons, (dvoid *)val, ffunc);
}

void yecec_PushCons__copy( yecec_PushCons* dest, yecec_PushCons* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yecec_PushCons, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yecec_PushCons_push__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PushCons_push");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PushCons_push", (ub4)2);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)yoTcAny;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yecec_Disconnected;
    _pars[2].mode = YOMODE_INVALID;
    _pars[2].tk = (yotk*)yoTcNull;
    _pars[2].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yecec_PushCons_push( yecec_PushCons or, yoenv* ev, yoany* data)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PushCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PushCons__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yecec_PushCons__id);
      (*(void (*)( yecec_PushCons, yoenv*, yoany*))_f)(or, ev, data);
    }
    else
      (*_impl->push)(or, ev, data);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PushCons_push_nw( or, ev, data, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yecec_PushCons_push_nw( yecec_PushCons or, yoenv* ev, yoany* data, 
  ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) data;
  yoSendReq( (dvoid *)or, ev, "push", TRUE, _sem, (sword)1, 
    yecec_PushCons_push_pars, _parvec);
}

yopar* yecec_PushCons_disconnect__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PushCons_disconnect");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PushCons_disconnect", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yecec_PushCons_disconnect( yecec_PushCons or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PushCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PushCons__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yecec_PushCons__id);
      (*(void (*)( yecec_PushCons, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->disconnect)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PushCons_disconnect_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yecec_PushCons_disconnect_nw( yecec_PushCons or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "disconnect", TRUE, _sem, (sword)0, 
    yecec_PushCons_disconnect_pars, (dvoid **)0);
}


/* Client stubs for interface ::yecec::PushSupp */
static ysidDecl(yecec_PushSupp___id) = "IDL:yecec/PushSupp:1.0";

CONST ysid* yecec_PushSupp__getId(void)
{
  return (CONST ysid*)yecec_PushSupp___id;
}

static CONST_DATA yotk yecec_PushSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','c','/',
  'P','u','s','h','S','u','p','p',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','c',':',':','P','u',
  's','h','S','u','p','p',0x00};

yotk* yecec_PushSupp__getTC(void)
{
  return (yotk*)yecec_PushSupp__tc;
}


void yecec_PushSupp__free( yecec_PushSupp* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yecec_PushSupp, (dvoid *)val, ffunc);
}

void yecec_PushSupp__copy( yecec_PushSupp* dest, yecec_PushSupp* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yecec_PushSupp, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yecec_PushSupp_disconnect_push_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PushSupp_disconnect_push_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PushSupp_disconnect_push_supp", (
      ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yecec_PushSupp_disconnect_push_supp( yecec_PushSupp or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PushSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PushSupp__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yecec_PushSupp__id);
      (*(void (*)( yecec_PushSupp, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->disconnect_push_supp)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PushSupp_disconnect_push_supp_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yecec_PushSupp_disconnect_push_supp_nw( yecec_PushSupp or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "disconnect_push_supp", TRUE, _sem, (sword)0, 
    yecec_PushSupp_disconnect_push_supp_pars, (dvoid **)0);
}


/* Client stubs for interface ::yecec::PullSupp */
static ysidDecl(yecec_PullSupp___id) = "IDL:yecec/PullSupp:1.0";

CONST ysid* yecec_PullSupp__getId(void)
{
  return (CONST ysid*)yecec_PullSupp___id;
}

static CONST_DATA yotk yecec_PullSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','c','/',
  'P','u','l','l','S','u','p','p',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','c',':',':','P','u',
  'l','l','S','u','p','p',0x00};

yotk* yecec_PullSupp__getTC(void)
{
  return (yotk*)yecec_PullSupp__tc;
}


void yecec_PullSupp__free( yecec_PullSupp* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yecec_PullSupp, (dvoid *)val, ffunc);
}

void yecec_PullSupp__copy( yecec_PullSupp* dest, yecec_PullSupp* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yecec_PullSupp, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yecec_PullSupp_pull__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PullSupp_pull");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PullSupp_pull", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcAny;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yecec_Disconnected;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yecec_NothingToPull;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yoany yecec_PullSupp_pull( yecec_PullSupp or, yoenv* ev)
{
  yoany _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PullSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PullSupp__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yecec_PullSupp__id);
      _result = (*(yoany (*)( yecec_PullSupp, yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->pull)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PullSupp_pull_nw( or, ev, (ysevt*)_sem);
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

void yecec_PullSupp_pull_nw( yecec_PullSupp or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "pull", TRUE, _sem, (sword)0, 
    yecec_PullSupp_pull_pars, (dvoid **)0);
}

yopar* yecec_PullSupp_try_pull__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PullSupp_try_pull");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PullSupp_try_pull", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcAny;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)yoTcBoolean;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yecec_Disconnected;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yoany yecec_PullSupp_try_pull( yecec_PullSupp or, yoenv* ev, boolean* 
  has_event)
{
  yoany _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PullSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PullSupp__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yecec_PullSupp__id);
      _result = (*(yoany (*)( yecec_PullSupp, yoenv*, boolean*))_f)(or, ev, 
        has_event);
    }
    else
      _result = (*_impl->try_pull)(or, ev, has_event);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PullSupp_try_pull_nw( or, ev, has_event, (ysevt*)_sem);
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

void yecec_PullSupp_try_pull_nw( yecec_PullSupp or, yoenv* ev, boolean* 
  has_event, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) has_event;
  yoSendReq( (dvoid *)or, ev, "try_pull", TRUE, _sem, (sword)1, 
    yecec_PullSupp_try_pull_pars, _parvec);
}

yopar* yecec_PullSupp_disconnect__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PullSupp_disconnect");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PullSupp_disconnect", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yecec_PullSupp_disconnect( yecec_PullSupp or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PullSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PullSupp__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, yecec_PullSupp__id);
      (*(void (*)( yecec_PullSupp, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->disconnect)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PullSupp_disconnect_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yecec_PullSupp_disconnect_nw( yecec_PullSupp or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "disconnect", TRUE, _sem, (sword)0, 
    yecec_PullSupp_disconnect_pars, (dvoid **)0);
}


/* Client stubs for interface ::yecec::PullCons */
static ysidDecl(yecec_PullCons___id) = "IDL:yecec/PullCons:1.0";

CONST ysid* yecec_PullCons__getId(void)
{
  return (CONST ysid*)yecec_PullCons___id;
}

static CONST_DATA yotk yecec_PullCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','c','/',
  'P','u','l','l','C','o','n','s',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','c',':',':','P','u',
  'l','l','C','o','n','s',0x00};

yotk* yecec_PullCons__getTC(void)
{
  return (yotk*)yecec_PullCons__tc;
}


void yecec_PullCons__free( yecec_PullCons* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yecec_PullCons, (dvoid *)val, ffunc);
}

void yecec_PullCons__copy( yecec_PullCons* dest, yecec_PullCons* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yecec_PullCons, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yecec_PullCons_disconnect__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yecec_PullCons_disconnect");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yecec_PullCons_disconnect", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yecec_PullCons_disconnect( yecec_PullCons or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yecec_PullCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yecec_PullCons__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yecec_PullCons__id);
      (*(void (*)( yecec_PullCons, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->disconnect)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yecec_PullCons_disconnect_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yecec_PullCons_disconnect_nw( yecec_PullCons or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "disconnect", TRUE, _sem, (sword)0, 
    yecec_PullCons_disconnect_pars, (dvoid **)0);
}


static ysidDecl(yeceCa_AlreadyConnected___id) = 
  "IDL:yeceCa/AlreadyConnected:1.0";

CONST ysid* yeceCa_AlreadyConnected__getId(void)
{
  return (CONST ysid*)yeceCa_AlreadyConnected___id;
}

static CONST_DATA yotk yeceCa_AlreadyConnected__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'L',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x20,'I','D','L',':','y','e','c','e','C','a',
  '/','A','l','r','e','a','d','y','C','o','n','n','e','c','t',
  'e','d',':','1','.','0',0x00,0x00,0x00,0x00,0x1b,':',':','y'
  ,'e','c','e','C','a',':',':','A','l','r','e','a','d','y','C'
  ,'o','n','n','e','c','t','e','d',0x00,0x00,0x00,0x00,0x00,0x00
  };

yotk* yeceCa_AlreadyConnected__getTC(void)
{
  return (yotk*)yeceCa_AlreadyConnected__tc;
}

static ysidDecl(yeceCa_TypeError___id) = "IDL:yeceCa/TypeError:1.0";

CONST ysid* yeceCa_TypeError__getId(void)
{
  return (CONST ysid*)yeceCa_TypeError___id;
}

static CONST_DATA yotk yeceCa_TypeError__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'@',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x19,'I','D','L',':','y','e','c','e','C','a',
  '/','T','y','p','e','E','r','r','o','r',':','1','.','0',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','e','c','e',
  'C','a',':',':','T','y','p','e','E','r','r','o','r',0x00,0x00
  ,0x00,0x00,0x00};

yotk* yeceCa_TypeError__getTC(void)
{
  return (yotk*)yeceCa_TypeError__tc;
}

/* Client stubs for interface ::yeceCa::ProxyPushCons */
static ysidDecl(yeceCa_ProxyPushCons___id) = "IDL:yeceCa/ProxyPushCons:1.0";
  

CONST ysid* yeceCa_ProxyPushCons__getId(void)
{
  return (CONST ysid*)yeceCa_ProxyPushCons___id;
}

static CONST_DATA yotk yeceCa_ProxyPushCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1d,'I','D','L',':','y','e','c','e','C','a',
  '/','P','r','o','x','y','P','u','s','h','C','o','n','s',':',
  '1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,':',':',
  'y','e','c','e','C','a',':',':','P','r','o','x','y','P','u',
  's','h','C','o','n','s',0x00};

yotk* yeceCa_ProxyPushCons__getTC(void)
{
  return (yotk*)yeceCa_ProxyPushCons__tc;
}


void yeceCa_ProxyPushCons__free( yeceCa_ProxyPushCons* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_ProxyPushCons, (dvoid *)val, ffunc);
}

void yeceCa_ProxyPushCons__copy( yeceCa_ProxyPushCons* dest, 
  yeceCa_ProxyPushCons* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_ProxyPushCons, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceCa_ProxyPushCons_connect_push_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ProxyPushCons_connect_push_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ProxyPushCons_connect_push_supp", 
      (ub4)2);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_yecec_PushSupp;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yeceCa_AlreadyConnected;
    _pars[2].mode = YOMODE_INVALID;
    _pars[2].tk = (yotk*)yoTcNull;
    _pars[2].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yeceCa_ProxyPushCons_connect_push_supp( yeceCa_ProxyPushCons or, 
  yoenv* ev, yecec_PushSupp push_supp)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ProxyPushCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ProxyPushCons__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceCa_ProxyPushCons__id);
      (*(void (*)( yeceCa_ProxyPushCons, yoenv*, yecec_PushSupp))_f)(or, ev,
         push_supp);
    }
    else
      (*_impl->connect_push_supp)(or, ev, push_supp);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ProxyPushCons_connect_push_supp_nw( or, ev, push_supp, (ysevt*)
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yeceCa_ProxyPushCons_connect_push_supp_nw( yeceCa_ProxyPushCons or, 
  yoenv* ev, yecec_PushSupp push_supp, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&push_supp;
  yoSendReq( (dvoid *)or, ev, "connect_push_supp", TRUE, _sem, (sword)1, 
    yeceCa_ProxyPushCons_connect_push_supp_pars, _parvec);
}


/* Client stubs for interface ::yeceCa::ProxyPullSupp */
static ysidDecl(yeceCa_ProxyPullSupp___id) = "IDL:yeceCa/ProxyPullSupp:1.0";
  

CONST ysid* yeceCa_ProxyPullSupp__getId(void)
{
  return (CONST ysid*)yeceCa_ProxyPullSupp___id;
}

static CONST_DATA yotk yeceCa_ProxyPullSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1d,'I','D','L',':','y','e','c','e','C','a',
  '/','P','r','o','x','y','P','u','l','l','S','u','p','p',':',
  '1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,':',':',
  'y','e','c','e','C','a',':',':','P','r','o','x','y','P','u',
  'l','l','S','u','p','p',0x00};

yotk* yeceCa_ProxyPullSupp__getTC(void)
{
  return (yotk*)yeceCa_ProxyPullSupp__tc;
}


void yeceCa_ProxyPullSupp__free( yeceCa_ProxyPullSupp* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_ProxyPullSupp, (dvoid *)val, ffunc);
}

void yeceCa_ProxyPullSupp__copy( yeceCa_ProxyPullSupp* dest, 
  yeceCa_ProxyPullSupp* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_ProxyPullSupp, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceCa_ProxyPullSupp_connect_pull_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ProxyPullSupp_connect_pull_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ProxyPullSupp_connect_pull_cons", 
      (ub4)2);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_yecec_PullCons;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yeceCa_AlreadyConnected;
    _pars[2].mode = YOMODE_INVALID;
    _pars[2].tk = (yotk*)yoTcNull;
    _pars[2].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yeceCa_ProxyPullSupp_connect_pull_cons( yeceCa_ProxyPullSupp or, 
  yoenv* ev, yecec_PullCons pull_cons)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ProxyPullSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ProxyPullSupp__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceCa_ProxyPullSupp__id);
      (*(void (*)( yeceCa_ProxyPullSupp, yoenv*, yecec_PullCons))_f)(or, ev,
         pull_cons);
    }
    else
      (*_impl->connect_pull_cons)(or, ev, pull_cons);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ProxyPullSupp_connect_pull_cons_nw( or, ev, pull_cons, (ysevt*)
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yeceCa_ProxyPullSupp_connect_pull_cons_nw( yeceCa_ProxyPullSupp or, 
  yoenv* ev, yecec_PullCons pull_cons, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&pull_cons;
  yoSendReq( (dvoid *)or, ev, "connect_pull_cons", TRUE, _sem, (sword)1, 
    yeceCa_ProxyPullSupp_connect_pull_cons_pars, _parvec);
}


/* Client stubs for interface ::yeceCa::ProxyPullCons */
static ysidDecl(yeceCa_ProxyPullCons___id) = "IDL:yeceCa/ProxyPullCons:1.0";
  

CONST ysid* yeceCa_ProxyPullCons__getId(void)
{
  return (CONST ysid*)yeceCa_ProxyPullCons___id;
}

static CONST_DATA yotk yeceCa_ProxyPullCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1d,'I','D','L',':','y','e','c','e','C','a',
  '/','P','r','o','x','y','P','u','l','l','C','o','n','s',':',
  '1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,':',':',
  'y','e','c','e','C','a',':',':','P','r','o','x','y','P','u',
  'l','l','C','o','n','s',0x00};

yotk* yeceCa_ProxyPullCons__getTC(void)
{
  return (yotk*)yeceCa_ProxyPullCons__tc;
}


void yeceCa_ProxyPullCons__free( yeceCa_ProxyPullCons* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_ProxyPullCons, (dvoid *)val, ffunc);
}

void yeceCa_ProxyPullCons__copy( yeceCa_ProxyPullCons* dest, 
  yeceCa_ProxyPullCons* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_ProxyPullCons, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceCa_ProxyPullCons_connect_pull_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ProxyPullCons_connect_pull_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ProxyPullCons_connect_pull_supp", 
      (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_yecec_PullSupp;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yeceCa_AlreadyConnected;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceCa_TypeError;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yeceCa_ProxyPullCons_connect_pull_supp( yeceCa_ProxyPullCons or, 
  yoenv* ev, yecec_PullSupp pull_supp)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ProxyPullCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ProxyPullCons__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceCa_ProxyPullCons__id);
      (*(void (*)( yeceCa_ProxyPullCons, yoenv*, yecec_PullSupp))_f)(or, ev,
         pull_supp);
    }
    else
      (*_impl->connect_pull_supp)(or, ev, pull_supp);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ProxyPullCons_connect_pull_supp_nw( or, ev, pull_supp, (ysevt*)
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yeceCa_ProxyPullCons_connect_pull_supp_nw( yeceCa_ProxyPullCons or, 
  yoenv* ev, yecec_PullSupp pull_supp, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&pull_supp;
  yoSendReq( (dvoid *)or, ev, "connect_pull_supp", TRUE, _sem, (sword)1, 
    yeceCa_ProxyPullCons_connect_pull_supp_pars, _parvec);
}


/* Client stubs for interface ::yeceCa::ProxyPushSupp */
static ysidDecl(yeceCa_ProxyPushSupp___id) = "IDL:yeceCa/ProxyPushSupp:1.0";
  

CONST ysid* yeceCa_ProxyPushSupp__getId(void)
{
  return (CONST ysid*)yeceCa_ProxyPushSupp___id;
}

static CONST_DATA yotk yeceCa_ProxyPushSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'D',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1d,'I','D','L',':','y','e','c','e','C','a',
  '/','P','r','o','x','y','P','u','s','h','S','u','p','p',':',
  '1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,':',':',
  'y','e','c','e','C','a',':',':','P','r','o','x','y','P','u',
  's','h','S','u','p','p',0x00};

yotk* yeceCa_ProxyPushSupp__getTC(void)
{
  return (yotk*)yeceCa_ProxyPushSupp__tc;
}


void yeceCa_ProxyPushSupp__free( yeceCa_ProxyPushSupp* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_ProxyPushSupp, (dvoid *)val, ffunc);
}

void yeceCa_ProxyPushSupp__copy( yeceCa_ProxyPushSupp* dest, 
  yeceCa_ProxyPushSupp* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_ProxyPushSupp, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceCa_ProxyPushSupp_connect_push_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ProxyPushSupp_connect_push_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ProxyPushSupp_connect_push_cons", 
      (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_yecec_PushCons;
    _pars[1].mode = YOMODE_EXCEPT;
    _pars[1].tk = (yotk*)YCTC_yeceCa_AlreadyConnected;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceCa_TypeError;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yeceCa_ProxyPushSupp_connect_push_cons( yeceCa_ProxyPushSupp or, 
  yoenv* ev, yecec_PushCons push_cons)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ProxyPushSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ProxyPushSupp__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceCa_ProxyPushSupp__id);
      (*(void (*)( yeceCa_ProxyPushSupp, yoenv*, yecec_PushCons))_f)(or, ev,
         push_cons);
    }
    else
      (*_impl->connect_push_cons)(or, ev, push_cons);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ProxyPushSupp_connect_push_cons_nw( or, ev, push_cons, (ysevt*)
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yeceCa_ProxyPushSupp_connect_push_cons_nw( yeceCa_ProxyPushSupp or, 
  yoenv* ev, yecec_PushCons push_cons, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&push_cons;
  yoSendReq( (dvoid *)or, ev, "connect_push_cons", TRUE, _sem, (sword)1, 
    yeceCa_ProxyPushSupp_connect_push_cons_pars, _parvec);
}


/* Client stubs for interface ::yeceCa::ConsAdm */
static ysidDecl(yeceCa_ConsAdm___id) = "IDL:yeceCa/ConsAdm:1.0";

CONST ysid* yeceCa_ConsAdm__getId(void)
{
  return (CONST ysid*)yeceCa_ConsAdm___id;
}

static CONST_DATA yotk yeceCa_ConsAdm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','C','a',
  '/','C','o','n','s','A','d','m',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','C','a',':',':','C',
  'o','n','s','A','d','m',0x00};

yotk* yeceCa_ConsAdm__getTC(void)
{
  return (yotk*)yeceCa_ConsAdm__tc;
}


void yeceCa_ConsAdm__free( yeceCa_ConsAdm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_ConsAdm, (dvoid *)val, ffunc);
}

void yeceCa_ConsAdm__copy( yeceCa_ConsAdm* dest, yeceCa_ConsAdm* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yeceCa_ConsAdm, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yeceCa_ConsAdm_obtain_push_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ConsAdm_obtain_push_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ConsAdm_obtain_push_supp", (ub4)1)
      ;
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPushSupp;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPushSupp yeceCa_ConsAdm_obtain_push_supp( yeceCa_ConsAdm or, 
  yoenv* ev)
{
  yeceCa_ProxyPushSupp _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ConsAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ConsAdm__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yeceCa_ConsAdm__id);
      _result = (*(yeceCa_ProxyPushSupp (*)( yeceCa_ConsAdm, yoenv*))_f)(or,
         ev);
    }
    else
      _result = (*_impl->obtain_push_supp)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ConsAdm_obtain_push_supp_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_ConsAdm_obtain_push_supp_nw( yeceCa_ConsAdm or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "obtain_push_supp", TRUE, _sem, (sword)0, 
    yeceCa_ConsAdm_obtain_push_supp_pars, (dvoid **)0);
}

yopar* yeceCa_ConsAdm_obtain_pull_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_ConsAdm_obtain_pull_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_ConsAdm_obtain_pull_supp", (ub4)1)
      ;
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPullSupp;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPullSupp yeceCa_ConsAdm_obtain_pull_supp( yeceCa_ConsAdm or, 
  yoenv* ev)
{
  yeceCa_ProxyPullSupp _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_ConsAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_ConsAdm__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yeceCa_ConsAdm__id);
      _result = (*(yeceCa_ProxyPullSupp (*)( yeceCa_ConsAdm, yoenv*))_f)(or,
         ev);
    }
    else
      _result = (*_impl->obtain_pull_supp)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_ConsAdm_obtain_pull_supp_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_ConsAdm_obtain_pull_supp_nw( yeceCa_ConsAdm or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "obtain_pull_supp", TRUE, _sem, (sword)0, 
    yeceCa_ConsAdm_obtain_pull_supp_pars, (dvoid **)0);
}


/* Client stubs for interface ::yeceCa::SuppAdm */
static ysidDecl(yeceCa_SuppAdm___id) = "IDL:yeceCa/SuppAdm:1.0";

CONST ysid* yeceCa_SuppAdm__getId(void)
{
  return (CONST ysid*)yeceCa_SuppAdm___id;
}

static CONST_DATA yotk yeceCa_SuppAdm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','C','a',
  '/','S','u','p','p','A','d','m',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','C','a',':',':','S',
  'u','p','p','A','d','m',0x00};

yotk* yeceCa_SuppAdm__getTC(void)
{
  return (yotk*)yeceCa_SuppAdm__tc;
}


void yeceCa_SuppAdm__free( yeceCa_SuppAdm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_SuppAdm, (dvoid *)val, ffunc);
}

void yeceCa_SuppAdm__copy( yeceCa_SuppAdm* dest, yeceCa_SuppAdm* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yeceCa_SuppAdm, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yeceCa_SuppAdm_obtain_push_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_SuppAdm_obtain_push_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_SuppAdm_obtain_push_cons", (ub4)1)
      ;
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPushCons;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPushCons yeceCa_SuppAdm_obtain_push_cons( yeceCa_SuppAdm or, 
  yoenv* ev)
{
  yeceCa_ProxyPushCons _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_SuppAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_SuppAdm__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yeceCa_SuppAdm__id);
      _result = (*(yeceCa_ProxyPushCons (*)( yeceCa_SuppAdm, yoenv*))_f)(or,
         ev);
    }
    else
      _result = (*_impl->obtain_push_cons)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_SuppAdm_obtain_push_cons_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_SuppAdm_obtain_push_cons_nw( yeceCa_SuppAdm or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "obtain_push_cons", TRUE, _sem, (sword)0, 
    yeceCa_SuppAdm_obtain_push_cons_pars, (dvoid **)0);
}

yopar* yeceCa_SuppAdm_obtain_pull_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_SuppAdm_obtain_pull_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_SuppAdm_obtain_pull_cons", (ub4)1)
      ;
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPullCons;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPullCons yeceCa_SuppAdm_obtain_pull_cons( yeceCa_SuppAdm or, 
  yoenv* ev)
{
  yeceCa_ProxyPullCons _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_SuppAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_SuppAdm__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yeceCa_SuppAdm__id);
      _result = (*(yeceCa_ProxyPullCons (*)( yeceCa_SuppAdm, yoenv*))_f)(or,
         ev);
    }
    else
      _result = (*_impl->obtain_pull_cons)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_SuppAdm_obtain_pull_cons_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_SuppAdm_obtain_pull_cons_nw( yeceCa_SuppAdm or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "obtain_pull_cons", TRUE, _sem, (sword)0, 
    yeceCa_SuppAdm_obtain_pull_cons_pars, (dvoid **)0);
}


/* Client stubs for interface ::yeceCa::EventChannel */
static ysidDecl(yeceCa_EventChannel___id) = "IDL:yeceCa/EventChannel:1.0";

CONST ysid* yeceCa_EventChannel__getId(void)
{
  return (CONST ysid*)yeceCa_EventChannel___id;
}

static CONST_DATA yotk yeceCa_EventChannel__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'?',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1c,'I','D','L',':','y','e','c','e','C','a',
  '/','E','v','e','n','t','C','h','a','n','n','e','l',':','1',
  '.','0',0x00,0x00,0x00,0x00,0x17,':',':','y','e','c','e','C'
  ,'a',':',':','E','v','e','n','t','C','h','a','n','n','e','l'
  ,0x00};

yotk* yeceCa_EventChannel__getTC(void)
{
  return (yotk*)yeceCa_EventChannel__tc;
}


void yeceCa_EventChannel__free( yeceCa_EventChannel* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_EventChannel, (dvoid *)val, ffunc);
}

void yeceCa_EventChannel__copy( yeceCa_EventChannel* dest, 
  yeceCa_EventChannel* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_EventChannel, (dvoid *)dest, (dvoid *)src, afunc)
    ;
}


yopar* yeceCa_EventChannel_for_consumers__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_EventChannel_for_consumers");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_EventChannel_for_consumers", (ub4)
      1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ConsAdm;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ConsAdm yeceCa_EventChannel_for_consumers( yeceCa_EventChannel or, 
  yoenv* ev)
{
  yeceCa_ConsAdm _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_EventChannel__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_EventChannel__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yeceCa_EventChannel__id)
        ;
      _result = (*(yeceCa_ConsAdm (*)( yeceCa_EventChannel, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->for_consumers)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_EventChannel_for_consumers_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_EventChannel_for_consumers_nw( yeceCa_EventChannel or, yoenv* 
  ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "for_consumers", TRUE, _sem, (sword)0, 
    yeceCa_EventChannel_for_consumers_pars, (dvoid **)0);
}

yopar* yeceCa_EventChannel_for_suppliers__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_EventChannel_for_suppliers");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_EventChannel_for_suppliers", (ub4)
      1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_SuppAdm;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_SuppAdm yeceCa_EventChannel_for_suppliers( yeceCa_EventChannel or, 
  yoenv* ev)
{
  yeceCa_SuppAdm _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_EventChannel__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_EventChannel__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yeceCa_EventChannel__id)
        ;
      _result = (*(yeceCa_SuppAdm (*)( yeceCa_EventChannel, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->for_suppliers)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_EventChannel_for_suppliers_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_EventChannel_for_suppliers_nw( yeceCa_EventChannel or, yoenv* 
  ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "for_suppliers", TRUE, _sem, (sword)0, 
    yeceCa_EventChannel_for_suppliers_pars, (dvoid **)0);
}

yopar* yeceCa_EventChannel_destroy__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_EventChannel_destroy");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_EventChannel_destroy", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void yeceCa_EventChannel_destroy( yeceCa_EventChannel or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_EventChannel__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_EventChannel__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, yeceCa_EventChannel__id)
        ;
      (*(void (*)( yeceCa_EventChannel, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->destroy)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_EventChannel_destroy_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void yeceCa_EventChannel_destroy_nw( yeceCa_EventChannel or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "destroy", TRUE, _sem, (sword)0, 
    yeceCa_EventChannel_destroy_pars, (dvoid **)0);
}


static CONST_DATA yotk yeceCa_eventChannelList__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x00,0xa0,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x20,'I','D','L',':','y','e','c','e','C','a'
  ,'/','e','v','e','n','t','C','h','a','n','n','e','l','L','i'
  ,'s','t',':','1','.','0',0x00,0x00,0x00,0x00,0x1b,':',':','y'
  ,'e','c','e','C','a',':',':','e','v','e','n','t','C','h','a'
  ,'n','n','e','l','L','i','s','t',0x00,0x00,0x00,0x00,0x00,0x13
  ,0x00,0x00,0x00,'P',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,
  0x00,0x00,0x00,'?',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,'I'
  ,'D','L',':','y','e','c','e','C','a','/','E','v','e','n','t'
  ,'C','h','a','n','n','e','l',':','1','.','0',0x00,0x00,0x00,
  0x00,0x17,':',':','y','e','c','e','C','a',':',':','E','v','e'
  ,'n','t','C','h','a','n','n','e','l',0x00,0x00,0x00,0x00,0x00
  ,0x00};

yotk* yeceCa_eventChannelList__getTC(void)
{
  return (yotk*)yeceCa_eventChannelList__tc;
}

void yeceCa_eventChannelList__free( yeceCa_eventChannelList* val, ysmff 
  ffunc)
{
  yotkFreeVal( YCTC_yeceCa_eventChannelList, (dvoid *)val, ffunc);
}

void yeceCa_eventChannelList__copy( yeceCa_eventChannelList* dest, 
  yeceCa_eventChannelList* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceCa_eventChannelList, (dvoid *)dest, (dvoid *)src, 
    afunc);
}

/* Client stubs for interface ::yeceCa::Factory */
static ysidDecl(yeceCa_Factory___id) = "IDL:yeceCa/Factory:1.0";

CONST ysid* yeceCa_Factory__getId(void)
{
  return (CONST ysid*)yeceCa_Factory___id;
}

static CONST_DATA yotk yeceCa_Factory__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'6',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x17,'I','D','L',':','y','e','c','e','C','a',
  '/','F','a','c','t','o','r','y',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','y','e','c','e','C','a',':',':','F',
  'a','c','t','o','r','y',0x00};

yotk* yeceCa_Factory__getTC(void)
{
  return (yotk*)yeceCa_Factory__tc;
}


void yeceCa_Factory__free( yeceCa_Factory* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceCa_Factory, (dvoid *)val, ffunc);
}

void yeceCa_Factory__copy( yeceCa_Factory* dest, yeceCa_Factory* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_yeceCa_Factory, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yeceCa_Factory_create__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_Factory_create");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_Factory_create", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_EventChannel;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_EventChannel yeceCa_Factory_create( yeceCa_Factory or, yoenv* ev)
{
  yeceCa_EventChannel _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yeceCa_Factory__id);
      _result = (*(yeceCa_EventChannel (*)( yeceCa_Factory, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->create)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_Factory_create_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_Factory_create_nw( yeceCa_Factory or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "create", TRUE, _sem, (sword)0, 
    yeceCa_Factory_create_pars, (dvoid **)0);
}

yopar* yeceCa_Factory__get_channels__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_Factory__get_channels");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_Factory__get_channels", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_eventChannelList;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_eventChannelList yeceCa_Factory__get_channels( yeceCa_Factory or, 
  yoenv* ev)
{
  yeceCa_eventChannelList _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yeceCa_Factory__id);
      _result = (*(yeceCa_eventChannelList (*)( yeceCa_Factory, yoenv*))_f)(
        or, ev);
    }
    else
      _result = (*_impl->_get_channels)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_Factory__get_channels_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_Factory__get_channels_nw( yeceCa_Factory or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "_get_channels", TRUE, _sem, (sword)0, 
    yeceCa_Factory__get_channels_pars, (dvoid **)0);
}

yopar* yeceCa_Factory__get_self__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceCa_Factory__get_self");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceCa_Factory__get_self", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_Factory;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_Factory yeceCa_Factory__get_self( yeceCa_Factory or, yoenv* ev)
{
  yeceCa_Factory _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceCa_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceCa_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, yeceCa_Factory__id);
      _result = (*(yeceCa_Factory (*)( yeceCa_Factory, yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->_get_self)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceCa_Factory__get_self_nw( or, ev, (ysevt*)_sem);
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

void yeceCa_Factory__get_self_nw( yeceCa_Factory or, yoenv* ev, ysevt* _sem)
  
{
  yoSendReq( (dvoid *)or, ev, "_get_self", TRUE, _sem, (sword)0, 
    yeceCa_Factory__get_self_pars, (dvoid **)0);
}


/* Client stubs for interface ::yeceTec::TypedPushCons */
static ysidDecl(yeceTec_TypedPushCons___id) = 
  "IDL:yeceTec/TypedPushCons:1.0";

CONST ysid* yeceTec_TypedPushCons__getId(void)
{
  return (CONST ysid*)yeceTec_TypedPushCons___id;
}

static CONST_DATA yotk yeceTec_TypedPushCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'E',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1e,'I','D','L',':','y','e','c','e','T','e',
  'c','/','T','y','p','e','d','P','u','s','h','C','o','n','s',
  ':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x19,':',':','y'
  ,'e','c','e','T','e','c',':',':','T','y','p','e','d','P','u'
  ,'s','h','C','o','n','s',0x00};

yotk* yeceTec_TypedPushCons__getTC(void)
{
  return (yotk*)yeceTec_TypedPushCons__tc;
}


void yeceTec_TypedPushCons__free( yeceTec_TypedPushCons* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTec_TypedPushCons, (dvoid *)val, ffunc);
}

void yeceTec_TypedPushCons__copy( yeceTec_TypedPushCons* dest, 
  yeceTec_TypedPushCons* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTec_TypedPushCons, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceTec_TypedPushCons_get_typed_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTec_TypedPushCons_get_typed_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceTec_TypedPushCons_get_typed_cons", (
      ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcObject;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

CORBA_Object yeceTec_TypedPushCons_get_typed_cons( yeceTec_TypedPushCons or,
   yoenv* ev)
{
  CORBA_Object _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTec_TypedPushCons__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTec_TypedPushCons__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceTec_TypedPushCons__id);
      _result = (*(CORBA_Object (*)( yeceTec_TypedPushCons, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->get_typed_cons)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTec_TypedPushCons_get_typed_cons_nw( or, ev, (ysevt*)_sem);
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

void yeceTec_TypedPushCons_get_typed_cons_nw( yeceTec_TypedPushCons or, 
  yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "get_typed_cons", TRUE, _sem, (sword)0, 
    yeceTec_TypedPushCons_get_typed_cons_pars, (dvoid **)0);
}


/* Client stubs for interface ::yeceTec::TypedPullSupp */
static ysidDecl(yeceTec_TypedPullSupp___id) = 
  "IDL:yeceTec/TypedPullSupp:1.0";

CONST ysid* yeceTec_TypedPullSupp__getId(void)
{
  return (CONST ysid*)yeceTec_TypedPullSupp___id;
}

static CONST_DATA yotk yeceTec_TypedPullSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'E',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1e,'I','D','L',':','y','e','c','e','T','e',
  'c','/','T','y','p','e','d','P','u','l','l','S','u','p','p',
  ':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x19,':',':','y'
  ,'e','c','e','T','e','c',':',':','T','y','p','e','d','P','u'
  ,'l','l','S','u','p','p',0x00};

yotk* yeceTec_TypedPullSupp__getTC(void)
{
  return (yotk*)yeceTec_TypedPullSupp__tc;
}


void yeceTec_TypedPullSupp__free( yeceTec_TypedPullSupp* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTec_TypedPullSupp, (dvoid *)val, ffunc);
}

void yeceTec_TypedPullSupp__copy( yeceTec_TypedPullSupp* dest, 
  yeceTec_TypedPullSupp* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTec_TypedPullSupp, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceTec_TypedPullSupp_get_typed_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTec_TypedPullSupp_get_typed_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceTec_TypedPullSupp_get_typed_supp", (
      ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcObject;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

CORBA_Object yeceTec_TypedPullSupp_get_typed_supp( yeceTec_TypedPullSupp or,
   yoenv* ev)
{
  CORBA_Object _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTec_TypedPullSupp__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTec_TypedPullSupp__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceTec_TypedPullSupp__id);
      _result = (*(CORBA_Object (*)( yeceTec_TypedPullSupp, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->get_typed_supp)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTec_TypedPullSupp_get_typed_supp_nw( or, ev, (ysevt*)_sem);
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

void yeceTec_TypedPullSupp_get_typed_supp_nw( yeceTec_TypedPullSupp or, 
  yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "get_typed_supp", TRUE, _sem, (sword)0, 
    yeceTec_TypedPullSupp_get_typed_supp_pars, (dvoid **)0);
}


static ysidDecl(yeceTeca_InterfaceNotSupported___id) = 
  "IDL:yeceTeca/InterfaceNotSupported:1.0";

CONST ysid* yeceTeca_InterfaceNotSupported__getId(void)
{
  return (CONST ysid*)yeceTeca_InterfaceNotSupported___id;
}

static CONST_DATA yotk yeceTeca_InterfaceNotSupported__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x27,'I','D','L',':','y','e','c','e','T','e'
  ,'c','a','/','I','n','t','e','r','f','a','c','e','N','o','t'
  ,'S','u','p','p','o','r','t','e','d',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x00,'"',':',':','y','e','c','e','T','e','c','a',
  ':',':','I','n','t','e','r','f','a','c','e','N','o','t','S',
  'u','p','p','o','r','t','e','d',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00};

yotk* yeceTeca_InterfaceNotSupported__getTC(void)
{
  return (yotk*)yeceTeca_InterfaceNotSupported__tc;
}

static ysidDecl(yeceTeca_NoSuchImplementation___id) = 
  "IDL:yeceTeca/NoSuchImplementation:1.0";

CONST ysid* yeceTeca_NoSuchImplementation__getId(void)
{
  return (CONST ysid*)yeceTeca_NoSuchImplementation___id;
}

static CONST_DATA yotk yeceTeca_NoSuchImplementation__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,'&','I','D','L',':','y','e','c','e','T','e',
  'c','a','/','N','o','S','u','c','h','I','m','p','l','e','m',
  'e','n','t','a','t','i','o','n',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x00,'!',':',':','y','e','c','e','T','e','c','a',
  ':',':','N','o','S','u','c','h','I','m','p','l','e','m','e',
  'n','t','a','t','i','o','n',0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00};

yotk* yeceTeca_NoSuchImplementation__getTC(void)
{
  return (yotk*)yeceTeca_NoSuchImplementation__tc;
}

/* Client stubs for interface ::yeceTeca::TypedProxyPushCons */
static ysidDecl(yeceTeca_TypedProxyPushCons___id) = 
  "IDL:yeceTeca/TypedProxyPushCons:1.0";

CONST ysid* yeceTeca_TypedProxyPushCons__getId(void)
{
  return (CONST ysid*)yeceTeca_TypedProxyPushCons___id;
}

static CONST_DATA yotk yeceTeca_TypedProxyPushCons__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'O',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,'$','I','D','L',':','y','e','c','e','T','e','c'
  ,'a','/','T','y','p','e','d','P','r','o','x','y','P','u','s'
  ,'h','C','o','n','s',':','1','.','0',0x00,0x00,0x00,0x00,0x1f
  ,':',':','y','e','c','e','T','e','c','a',':',':','T','y','p'
  ,'e','d','P','r','o','x','y','P','u','s','h','C','o','n','s'
  ,0x00};

yotk* yeceTeca_TypedProxyPushCons__getTC(void)
{
  return (yotk*)yeceTeca_TypedProxyPushCons__tc;
}


void yeceTeca_TypedProxyPushCons__free( yeceTeca_TypedProxyPushCons* val, 
  ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_TypedProxyPushCons, (dvoid *)val, ffunc);
}

void yeceTeca_TypedProxyPushCons__copy( yeceTeca_TypedProxyPushCons* dest, 
  yeceTeca_TypedProxyPushCons* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_TypedProxyPushCons, (dvoid *)dest, (dvoid *)
    src, afunc);
}



/* Client stubs for interface ::yeceTeca::TypedProxyPullSupp */
static ysidDecl(yeceTeca_TypedProxyPullSupp___id) = 
  "IDL:yeceTeca/TypedProxyPullSupp:1.0";

CONST ysid* yeceTeca_TypedProxyPullSupp__getId(void)
{
  return (CONST ysid*)yeceTeca_TypedProxyPullSupp___id;
}

static CONST_DATA yotk yeceTeca_TypedProxyPullSupp__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'O',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,'$','I','D','L',':','y','e','c','e','T','e','c'
  ,'a','/','T','y','p','e','d','P','r','o','x','y','P','u','l'
  ,'l','S','u','p','p',':','1','.','0',0x00,0x00,0x00,0x00,0x1f
  ,':',':','y','e','c','e','T','e','c','a',':',':','T','y','p'
  ,'e','d','P','r','o','x','y','P','u','l','l','S','u','p','p'
  ,0x00};

yotk* yeceTeca_TypedProxyPullSupp__getTC(void)
{
  return (yotk*)yeceTeca_TypedProxyPullSupp__tc;
}


void yeceTeca_TypedProxyPullSupp__free( yeceTeca_TypedProxyPullSupp* val, 
  ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_TypedProxyPullSupp, (dvoid *)val, ffunc);
}

void yeceTeca_TypedProxyPullSupp__copy( yeceTeca_TypedProxyPullSupp* dest, 
  yeceTeca_TypedProxyPullSupp* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_TypedProxyPullSupp, (dvoid *)dest, (dvoid *)
    src, afunc);
}



/* Client stubs for interface ::yeceTeca::TypedSuppAdm */
static ysidDecl(yeceTeca_TypedSuppAdm___id) = 
  "IDL:yeceTeca/TypedSuppAdm:1.0";

CONST ysid* yeceTeca_TypedSuppAdm__getId(void)
{
  return (CONST ysid*)yeceTeca_TypedSuppAdm___id;
}

static CONST_DATA yotk yeceTeca_TypedSuppAdm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'E',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1e,'I','D','L',':','y','e','c','e','T','e',
  'c','a','/','T','y','p','e','d','S','u','p','p','A','d','m',
  ':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x19,':',':','y'
  ,'e','c','e','T','e','c','a',':',':','T','y','p','e','d','S'
  ,'u','p','p','A','d','m',0x00};

yotk* yeceTeca_TypedSuppAdm__getTC(void)
{
  return (yotk*)yeceTeca_TypedSuppAdm__tc;
}


void yeceTeca_TypedSuppAdm__free( yeceTeca_TypedSuppAdm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_TypedSuppAdm, (dvoid *)val, ffunc);
}

void yeceTeca_TypedSuppAdm__copy( yeceTeca_TypedSuppAdm* dest, 
  yeceTeca_TypedSuppAdm* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_TypedSuppAdm, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceTeca_TypedSuppAdm_obtain_typed_push_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedSuppAdm_obtain_typed_push_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedSuppAdm_obtain_typed_push_cons", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_TypedProxyPushCons;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceTeca_InterfaceNotSupported;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_TypedProxyPushCons yeceTeca_TypedSuppAdm_obtain_typed_push_cons( 
  yeceTeca_TypedSuppAdm or, yoenv* ev, char* supported_interface)
{
  yeceTeca_TypedProxyPushCons _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedSuppAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedSuppAdm__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceTeca_TypedSuppAdm__id);
      _result = (*(yeceTeca_TypedProxyPushCons (*)( yeceTeca_TypedSuppAdm, 
        yoenv*, char*))_f)(or, ev, supported_interface);
    }
    else
      _result = (*_impl->obtain_typed_push_cons)(or, ev, 
        supported_interface);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedSuppAdm_obtain_typed_push_cons_nw( or, ev, 
        supported_interface, (ysevt*)_sem);
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

void yeceTeca_TypedSuppAdm_obtain_typed_push_cons_nw( yeceTeca_TypedSuppAdm 
  or, yoenv* ev, char* supported_interface, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&supported_interface;
  yoSendReq( (dvoid *)or, ev, "obtain_typed_push_cons", TRUE, _sem, (sword)
    1, yeceTeca_TypedSuppAdm_obtain_typed_push_cons_pars, _parvec);
}

yopar* yeceTeca_TypedSuppAdm_obtain_typed_pull_cons__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedSuppAdm_obtain_typed_pull_cons");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedSuppAdm_obtain_typed_pull_cons", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPullCons;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceTeca_NoSuchImplementation;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPullCons yeceTeca_TypedSuppAdm_obtain_typed_pull_cons( 
  yeceTeca_TypedSuppAdm or, yoenv* ev, char* uses_interface)
{
  yeceCa_ProxyPullCons _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedSuppAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedSuppAdm__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, 
        yeceTeca_TypedSuppAdm__id);
      _result = (*(yeceCa_ProxyPullCons (*)( yeceTeca_TypedSuppAdm, yoenv*, 
        char*))_f)(or, ev, uses_interface);
    }
    else
      _result = (*_impl->obtain_typed_pull_cons)(or, ev, uses_interface);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_nw( or, ev, 
        uses_interface, (ysevt*)_sem);
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

void yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_nw( yeceTeca_TypedSuppAdm 
  or, yoenv* ev, char* uses_interface, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&uses_interface;
  yoSendReq( (dvoid *)or, ev, "obtain_typed_pull_cons", TRUE, _sem, (sword)
    1, yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_pars, _parvec);
}


/* Client stubs for interface ::yeceTeca::TypedConsAdm */
static ysidDecl(yeceTeca_TypedConsAdm___id) = 
  "IDL:yeceTeca/TypedConsAdm:1.0";

CONST ysid* yeceTeca_TypedConsAdm__getId(void)
{
  return (CONST ysid*)yeceTeca_TypedConsAdm___id;
}

static CONST_DATA yotk yeceTeca_TypedConsAdm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'E',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1e,'I','D','L',':','y','e','c','e','T','e',
  'c','a','/','T','y','p','e','d','C','o','n','s','A','d','m',
  ':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x19,':',':','y'
  ,'e','c','e','T','e','c','a',':',':','T','y','p','e','d','C'
  ,'o','n','s','A','d','m',0x00};

yotk* yeceTeca_TypedConsAdm__getTC(void)
{
  return (yotk*)yeceTeca_TypedConsAdm__tc;
}


void yeceTeca_TypedConsAdm__free( yeceTeca_TypedConsAdm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_TypedConsAdm, (dvoid *)val, ffunc);
}

void yeceTeca_TypedConsAdm__copy( yeceTeca_TypedConsAdm* dest, 
  yeceTeca_TypedConsAdm* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_TypedConsAdm, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* yeceTeca_TypedConsAdm_obtain_typed_pull_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedConsAdm_obtain_typed_pull_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedConsAdm_obtain_typed_pull_supp", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_TypedProxyPullSupp;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceTeca_InterfaceNotSupported;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_TypedProxyPullSupp yeceTeca_TypedConsAdm_obtain_typed_pull_supp( 
  yeceTeca_TypedConsAdm or, yoenv* ev, char* supported_interface)
{
  yeceTeca_TypedProxyPullSupp _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedConsAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedConsAdm__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceTeca_TypedConsAdm__id);
      _result = (*(yeceTeca_TypedProxyPullSupp (*)( yeceTeca_TypedConsAdm, 
        yoenv*, char*))_f)(or, ev, supported_interface);
    }
    else
      _result = (*_impl->obtain_typed_pull_supp)(or, ev, 
        supported_interface);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedConsAdm_obtain_typed_pull_supp_nw( or, ev, 
        supported_interface, (ysevt*)_sem);
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

void yeceTeca_TypedConsAdm_obtain_typed_pull_supp_nw( yeceTeca_TypedConsAdm 
  or, yoenv* ev, char* supported_interface, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&supported_interface;
  yoSendReq( (dvoid *)or, ev, "obtain_typed_pull_supp", TRUE, _sem, (sword)
    1, yeceTeca_TypedConsAdm_obtain_typed_pull_supp_pars, _parvec);
}

yopar* yeceTeca_TypedConsAdm_obtain_typed_push_supp__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedConsAdm_obtain_typed_push_supp");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedConsAdm_obtain_typed_push_supp", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceCa_ProxyPushSupp;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_yeceTeca_NoSuchImplementation;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceCa_ProxyPushSupp yeceTeca_TypedConsAdm_obtain_typed_push_supp( 
  yeceTeca_TypedConsAdm or, yoenv* ev, char* uses_interface)
{
  yeceCa_ProxyPushSupp _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedConsAdm__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedConsAdm__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, 
        yeceTeca_TypedConsAdm__id);
      _result = (*(yeceCa_ProxyPushSupp (*)( yeceTeca_TypedConsAdm, yoenv*, 
        char*))_f)(or, ev, uses_interface);
    }
    else
      _result = (*_impl->obtain_typed_push_supp)(or, ev, uses_interface);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedConsAdm_obtain_typed_push_supp_nw( or, ev, 
        uses_interface, (ysevt*)_sem);
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

void yeceTeca_TypedConsAdm_obtain_typed_push_supp_nw( yeceTeca_TypedConsAdm 
  or, yoenv* ev, char* uses_interface, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&uses_interface;
  yoSendReq( (dvoid *)or, ev, "obtain_typed_push_supp", TRUE, _sem, (sword)
    1, yeceTeca_TypedConsAdm_obtain_typed_push_supp_pars, _parvec);
}


/* Client stubs for interface ::yeceTeca::TypedEventChannel */
static ysidDecl(yeceTeca_TypedEventChannel___id) = 
  "IDL:yeceTeca/TypedEventChannel:1.0";

CONST ysid* yeceTeca_TypedEventChannel__getId(void)
{
  return (CONST ysid*)yeceTeca_TypedEventChannel___id;
}

static CONST_DATA yotk yeceTeca_TypedEventChannel__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'N',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,'#','I','D','L',':','y','e','c','e','T','e','c'
  ,'a','/','T','y','p','e','d','E','v','e','n','t','C','h','a'
  ,'n','n','e','l',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x1e
  ,':',':','y','e','c','e','T','e','c','a',':',':','T','y','p'
  ,'e','d','E','v','e','n','t','C','h','a','n','n','e','l',0x00
  };

yotk* yeceTeca_TypedEventChannel__getTC(void)
{
  return (yotk*)yeceTeca_TypedEventChannel__tc;
}


void yeceTeca_TypedEventChannel__free( yeceTeca_TypedEventChannel* val, 
  ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_TypedEventChannel, (dvoid *)val, ffunc);
}

void yeceTeca_TypedEventChannel__copy( yeceTeca_TypedEventChannel* dest, 
  yeceTeca_TypedEventChannel* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_TypedEventChannel, (dvoid *)dest, (dvoid *)src,
     afunc);
}


yopar* yeceTeca_TypedEventChannel_for_consumers__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedEventChannel_for_consumers");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedEventChannel_for_consumers", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_TypedConsAdm;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_TypedConsAdm yeceTeca_TypedEventChannel_for_consumers( 
  yeceTeca_TypedEventChannel or, yoenv* ev)
{
  yeceTeca_TypedConsAdm _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedEventChannel__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedEventChannel__tyimpl*) yoLocalObj( (
    CORBA_Object)or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        yeceTeca_TypedEventChannel__id);
      _result = (*(yeceTeca_TypedConsAdm (*)( yeceTeca_TypedEventChannel, 
        yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->for_consumers)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedEventChannel_for_consumers_nw( or, ev, (ysevt*)_sem);
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

void yeceTeca_TypedEventChannel_for_consumers_nw( 
  yeceTeca_TypedEventChannel or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "for_consumers", TRUE, _sem, (sword)0, 
    yeceTeca_TypedEventChannel_for_consumers_pars, (dvoid **)0);
}

yopar* yeceTeca_TypedEventChannel_for_suppliers__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_TypedEventChannel_for_suppliers");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( 
      "yeceTeca_TypedEventChannel_for_suppliers", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_TypedSuppAdm;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_TypedSuppAdm yeceTeca_TypedEventChannel_for_suppliers( 
  yeceTeca_TypedEventChannel or, yoenv* ev)
{
  yeceTeca_TypedSuppAdm _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_TypedEventChannel__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_TypedEventChannel__tyimpl*) yoLocalObj( (
    CORBA_Object)or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, 
        yeceTeca_TypedEventChannel__id);
      _result = (*(yeceTeca_TypedSuppAdm (*)( yeceTeca_TypedEventChannel, 
        yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->for_suppliers)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_TypedEventChannel_for_suppliers_nw( or, ev, (ysevt*)_sem);
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

void yeceTeca_TypedEventChannel_for_suppliers_nw( 
  yeceTeca_TypedEventChannel or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "for_suppliers", TRUE, _sem, (sword)0, 
    yeceTeca_TypedEventChannel_for_suppliers_pars, (dvoid **)0);
}


static CONST_DATA yotk yeceTeca_typedEventChannelList__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x27,'I','D','L',':','y','e','c','e','T','e'
  ,'c','a','/','t','y','p','e','d','E','v','e','n','t','C','h'
  ,'a','n','n','e','l','L','i','s','t',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x00,'"',':',':','y','e','c','e','T','e','c','a',
  ':',':','t','y','p','e','d','E','v','e','n','t','C','h','a',
  'n','n','e','l','L','i','s','t',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x13,0x00,0x00,0x00,'`',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0e,0x00,0x00,0x00,'N',0x00,0x00,0x00,0x00,0x00,0x00,0x00,'#'
  ,'I','D','L',':','y','e','c','e','T','e','c','a','/','T','y'
  ,'p','e','d','E','v','e','n','t','C','h','a','n','n','e','l'
  ,':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x1e,':',':','y','e'
  ,'c','e','T','e','c','a',':',':','T','y','p','e','d','E','v'
  ,'e','n','t','C','h','a','n','n','e','l',0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00};

yotk* yeceTeca_typedEventChannelList__getTC(void)
{
  return (yotk*)yeceTeca_typedEventChannelList__tc;
}

void yeceTeca_typedEventChannelList__free( yeceTeca_typedEventChannelList* 
  val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_typedEventChannelList, (dvoid *)val, ffunc);
}

void yeceTeca_typedEventChannelList__copy( yeceTeca_typedEventChannelList* 
  dest, yeceTeca_typedEventChannelList* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_typedEventChannelList, (dvoid *)dest, (dvoid *)
    src, afunc);
}

/* Client stubs for interface ::yeceTeca::Factory */
static ysidDecl(yeceTeca_Factory___id) = "IDL:yeceTeca/Factory:1.0";

CONST ysid* yeceTeca_Factory__getId(void)
{
  return (CONST ysid*)yeceTeca_Factory___id;
}

static CONST_DATA yotk yeceTeca_Factory__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'<',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x19,'I','D','L',':','y','e','c','e','T','e',
  'c','a','/','F','a','c','t','o','r','y',':','1','.','0',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','e','c','e',
  'T','e','c','a',':',':','F','a','c','t','o','r','y',0x00};

yotk* yeceTeca_Factory__getTC(void)
{
  return (yotk*)yeceTeca_Factory__tc;
}


void yeceTeca_Factory__free( yeceTeca_Factory* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_yeceTeca_Factory, (dvoid *)val, ffunc);
}

void yeceTeca_Factory__copy( yeceTeca_Factory* dest, yeceTeca_Factory* src, 
  ysmaf afunc)
{
  yotkCopyVal( YCTC_yeceTeca_Factory, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* yeceTeca_Factory_create__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_Factory_create");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceTeca_Factory_create", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_TypedEventChannel;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_TypedEventChannel yeceTeca_Factory_create( yeceTeca_Factory or, 
  yoenv* ev)
{
  yeceTeca_TypedEventChannel _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, yeceTeca_Factory__id);
      _result = (*(yeceTeca_TypedEventChannel (*)( yeceTeca_Factory, yoenv*)
        )_f)(or, ev);
    }
    else
      _result = (*_impl->create)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_Factory_create_nw( or, ev, (ysevt*)_sem);
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

void yeceTeca_Factory_create_nw( yeceTeca_Factory or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "create", TRUE, _sem, (sword)0, 
    yeceTeca_Factory_create_pars, (dvoid **)0);
}

yopar* yeceTeca_Factory__get_channels__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_Factory__get_channels");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceTeca_Factory__get_channels", (ub4)1);
      
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_typedEventChannelList;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_typedEventChannelList yeceTeca_Factory__get_channels( 
  yeceTeca_Factory or, yoenv* ev)
{
  yeceTeca_typedEventChannelList _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, yeceTeca_Factory__id);
      _result = (*(yeceTeca_typedEventChannelList (*)( yeceTeca_Factory, 
        yoenv*))_f)(or, ev);
    }
    else
      _result = (*_impl->_get_channels)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_Factory__get_channels_nw( or, ev, (ysevt*)_sem);
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

void yeceTeca_Factory__get_channels_nw( yeceTeca_Factory or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "_get_channels", TRUE, _sem, (sword)0, 
    yeceTeca_Factory__get_channels_pars, (dvoid **)0);
}

yopar* yeceTeca_Factory__get_self__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "yeceTeca_Factory__get_self");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "yeceTeca_Factory__get_self", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_yeceTeca_Factory;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

yeceTeca_Factory yeceTeca_Factory__get_self( yeceTeca_Factory or, yoenv* ev)
  
{
  yeceTeca_Factory _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct yeceTeca_Factory__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct yeceTeca_Factory__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, yeceTeca_Factory__id);
      _result = (*(yeceTeca_Factory (*)( yeceTeca_Factory, yoenv*))_f)(or, 
        ev);
    }
    else
      _result = (*_impl->_get_self)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      yeceTeca_Factory__get_self_nw( or, ev, (ysevt*)_sem);
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

void yeceTeca_Factory__get_self_nw( yeceTeca_Factory or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "_get_self", TRUE, _sem, (sword)0, 
    yeceTeca_Factory__get_self_pars, (dvoid **)0);
}



EXTC_END
