/* GENERATED FILE
 * yecevch - server stubs
 * from /vobs/mx/pub/yecevch.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef YOCOA_ORACLE
#include <yocoa.h>
#endif
#include <yecevchC.c>

EXTC_START

/* Server stubs for interface ::yecec::PushCons */
STATICF void yecec_PushCons_push_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void yecec_PushCons_push_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct yecec_PushCons__tyimpl*)impldef)->push)( (yecec_PushCons)or, 
    ev,(yoany*)args[0]);
}

STATICF void yecec_PushCons_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yecec_PushCons_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yecec_PushCons__tyimpl*)impldef)->disconnect)( (yecec_PushCons)
    or, ev);
}

yostub* yecec_PushCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "push";
  _stubs[0].parms = yecec_PushCons_push_pars;
  _stubs[0].oper = yecec_PushCons_push_s;
  _stubs[1].opernm = "disconnect";
  _stubs[1].parms = yecec_PushCons_disconnect_pars;
  _stubs[1].oper = yecec_PushCons_disconnect_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yecec::PushSupp */
STATICF void yecec_PushSupp_disconnect_push_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yecec_PushSupp_disconnect_push_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yecec_PushSupp__tyimpl*)impldef)->disconnect_push_supp)( (
    yecec_PushSupp)or, ev);
}

yostub* yecec_PushSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "disconnect_push_supp";
  _stubs[0].parms = yecec_PushSupp_disconnect_push_supp_pars;
  _stubs[0].oper = yecec_PushSupp_disconnect_push_supp_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yecec::PullSupp */
STATICF void yecec_PullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void yecec_PullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yecec_PullSupp__tyimpl*)impldef)->pull)( (
    yecec_PullSupp)or, ev);
}

STATICF void yecec_PullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yecec_PullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yecec_PullSupp__tyimpl*)impldef)->try_pull)(
     (yecec_PullSupp)or, ev,(boolean*)args[1]);
}

STATICF void yecec_PullSupp_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yecec_PullSupp_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yecec_PullSupp__tyimpl*)impldef)->disconnect)( (yecec_PullSupp)
    or, ev);
}

yostub* yecec_PullSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "pull";
  _stubs[0].parms = yecec_PullSupp_pull_pars;
  _stubs[0].oper = yecec_PullSupp_pull_s;
  _stubs[1].opernm = "try_pull";
  _stubs[1].parms = yecec_PullSupp_try_pull_pars;
  _stubs[1].oper = yecec_PullSupp_try_pull_s;
  _stubs[2].opernm = "disconnect";
  _stubs[2].parms = yecec_PullSupp_disconnect_pars;
  _stubs[2].oper = yecec_PullSupp_disconnect_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yecec::PullCons */
STATICF void yecec_PullCons_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yecec_PullCons_disconnect_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yecec_PullCons__tyimpl*)impldef)->disconnect)( (yecec_PullCons)
    or, ev);
}

yostub* yecec_PullCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "disconnect";
  _stubs[0].parms = yecec_PullCons_disconnect_pars;
  _stubs[0].oper = yecec_PullCons_disconnect_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::ProxyPushCons */
STATICF void yeceCa_ProxyPushCons_connect_push_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPushCons_connect_push_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceCa_ProxyPushCons__tyimpl*)impldef)->connect_push_supp)( (
    yeceCa_ProxyPushCons)or, ev,*(yecec_PushSupp*)args[0]);
}

STATICF void yeceCa_ProxyPushCons_push_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_ProxyPushCons_push_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct yeceCa_ProxyPushCons__tyimpl*)impldef)->push)( (
    yeceCa_ProxyPushCons)or, ev,(yoany*)args[0]);
}

STATICF void yeceCa_ProxyPushCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPushCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceCa_ProxyPushCons__tyimpl*)impldef)->disconnect)( (
    yeceCa_ProxyPushCons)or, ev);
}

STATICF yogfp yeceCa_ProxyPushCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceCa_ProxyPushCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPushCons__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PushCons__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceCa_ProxyPushCons__bases[] =
{
  "IDL:yecec/PushCons:1.0",
  (char*)0
};

yostub* yeceCa_ProxyPushCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)yeceCa_ProxyPushCons__widen;
  _result->bases = yeceCa_ProxyPushCons__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_push_supp";
  _stubs[0].parms = yeceCa_ProxyPushCons_connect_push_supp_pars;
  _stubs[0].oper = yeceCa_ProxyPushCons_connect_push_supp_s;
  _stubs[1].opernm = "push";
  _stubs[1].parms = yeceCa_ProxyPushCons_push_pars;
  _stubs[1].oper = yeceCa_ProxyPushCons_push_s;
  _stubs[2].opernm = "disconnect";
  _stubs[2].parms = yeceCa_ProxyPushCons_disconnect_pars;
  _stubs[2].oper = yeceCa_ProxyPushCons_disconnect_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::ProxyPullSupp */
STATICF void yeceCa_ProxyPullSupp_connect_pull_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullSupp_connect_pull_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceCa_ProxyPullSupp__tyimpl*)impldef)->connect_pull_cons)( (
    yeceCa_ProxyPullSupp)or, ev,*(yecec_PullCons*)args[0]);
}

STATICF void yeceCa_ProxyPullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceCa_ProxyPullSupp__tyimpl*)impldef)
    ->pull)( (yeceCa_ProxyPullSupp)or, ev);
}

STATICF void yeceCa_ProxyPullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceCa_ProxyPullSupp__tyimpl*)impldef)
    ->try_pull)( (yeceCa_ProxyPullSupp)or, ev,(boolean*)args[1]);
}

STATICF void yeceCa_ProxyPullSupp_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullSupp_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceCa_ProxyPullSupp__tyimpl*)impldef)->disconnect)( (
    yeceCa_ProxyPullSupp)or, ev);
}

STATICF yogfp yeceCa_ProxyPullSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceCa_ProxyPullSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPullSupp__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PullSupp__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceCa_ProxyPullSupp__bases[] =
{
  "IDL:yecec/PullSupp:1.0",
  (char*)0
};

yostub* yeceCa_ProxyPullSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)yeceCa_ProxyPullSupp__widen;
  _result->bases = yeceCa_ProxyPullSupp__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_pull_cons";
  _stubs[0].parms = yeceCa_ProxyPullSupp_connect_pull_cons_pars;
  _stubs[0].oper = yeceCa_ProxyPullSupp_connect_pull_cons_s;
  _stubs[1].opernm = "pull";
  _stubs[1].parms = yeceCa_ProxyPullSupp_pull_pars;
  _stubs[1].oper = yeceCa_ProxyPullSupp_pull_s;
  _stubs[2].opernm = "try_pull";
  _stubs[2].parms = yeceCa_ProxyPullSupp_try_pull_pars;
  _stubs[2].oper = yeceCa_ProxyPullSupp_try_pull_s;
  _stubs[3].opernm = "disconnect";
  _stubs[3].parms = yeceCa_ProxyPullSupp_disconnect_pars;
  _stubs[3].oper = yeceCa_ProxyPullSupp_disconnect_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::ProxyPullCons */
STATICF void yeceCa_ProxyPullCons_connect_pull_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullCons_connect_pull_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceCa_ProxyPullCons__tyimpl*)impldef)->connect_pull_supp)( (
    yeceCa_ProxyPullCons)or, ev,*(yecec_PullSupp*)args[0]);
}

STATICF void yeceCa_ProxyPullCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPullCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceCa_ProxyPullCons__tyimpl*)impldef)->disconnect)( (
    yeceCa_ProxyPullCons)or, ev);
}

STATICF yogfp yeceCa_ProxyPullCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceCa_ProxyPullCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPullCons__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PullCons__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceCa_ProxyPullCons__bases[] =
{
  "IDL:yecec/PullCons:1.0",
  (char*)0
};

yostub* yeceCa_ProxyPullCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)yeceCa_ProxyPullCons__widen;
  _result->bases = yeceCa_ProxyPullCons__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_pull_supp";
  _stubs[0].parms = yeceCa_ProxyPullCons_connect_pull_supp_pars;
  _stubs[0].oper = yeceCa_ProxyPullCons_connect_pull_supp_s;
  _stubs[1].opernm = "disconnect";
  _stubs[1].parms = yeceCa_ProxyPullCons_disconnect_pars;
  _stubs[1].oper = yeceCa_ProxyPullCons_disconnect_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::ProxyPushSupp */
STATICF void yeceCa_ProxyPushSupp_connect_push_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPushSupp_connect_push_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceCa_ProxyPushSupp__tyimpl*)impldef)->connect_push_cons)( (
    yeceCa_ProxyPushSupp)or, ev,*(yecec_PushCons*)args[0]);
}

STATICF void yeceCa_ProxyPushSupp_disconnect_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ProxyPushSupp_disconnect_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceCa_ProxyPushSupp__tyimpl*)impldef)->disconnect_push_supp)( 
    (yeceCa_ProxyPushSupp)or, ev);
}

STATICF yogfp yeceCa_ProxyPushSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceCa_ProxyPushSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPushSupp__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PushSupp__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceCa_ProxyPushSupp__bases[] =
{
  "IDL:yecec/PushSupp:1.0",
  (char*)0
};

yostub* yeceCa_ProxyPushSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)yeceCa_ProxyPushSupp__widen;
  _result->bases = yeceCa_ProxyPushSupp__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_push_cons";
  _stubs[0].parms = yeceCa_ProxyPushSupp_connect_push_cons_pars;
  _stubs[0].oper = yeceCa_ProxyPushSupp_connect_push_cons_s;
  _stubs[1].opernm = "disconnect_push_supp";
  _stubs[1].parms = yeceCa_ProxyPushSupp_disconnect_push_supp_pars;
  _stubs[1].oper = yeceCa_ProxyPushSupp_disconnect_push_supp_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::ConsAdm */
STATICF void yeceCa_ConsAdm_obtain_push_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ConsAdm_obtain_push_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPushSupp*)args[0] = (*((struct yeceCa_ConsAdm__tyimpl*)
    impldef)->obtain_push_supp)( (yeceCa_ConsAdm)or, ev);
}

STATICF void yeceCa_ConsAdm_obtain_pull_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_ConsAdm_obtain_pull_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPullSupp*)args[0] = (*((struct yeceCa_ConsAdm__tyimpl*)
    impldef)->obtain_pull_supp)( (yeceCa_ConsAdm)or, ev);
}

yostub* yeceCa_ConsAdm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "obtain_push_supp";
  _stubs[0].parms = yeceCa_ConsAdm_obtain_push_supp_pars;
  _stubs[0].oper = yeceCa_ConsAdm_obtain_push_supp_s;
  _stubs[1].opernm = "obtain_pull_supp";
  _stubs[1].parms = yeceCa_ConsAdm_obtain_pull_supp_pars;
  _stubs[1].oper = yeceCa_ConsAdm_obtain_pull_supp_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::SuppAdm */
STATICF void yeceCa_SuppAdm_obtain_push_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_SuppAdm_obtain_push_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPushCons*)args[0] = (*((struct yeceCa_SuppAdm__tyimpl*)
    impldef)->obtain_push_cons)( (yeceCa_SuppAdm)or, ev);
}

STATICF void yeceCa_SuppAdm_obtain_pull_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_SuppAdm_obtain_pull_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPullCons*)args[0] = (*((struct yeceCa_SuppAdm__tyimpl*)
    impldef)->obtain_pull_cons)( (yeceCa_SuppAdm)or, ev);
}

yostub* yeceCa_SuppAdm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "obtain_push_cons";
  _stubs[0].parms = yeceCa_SuppAdm_obtain_push_cons_pars;
  _stubs[0].oper = yeceCa_SuppAdm_obtain_push_cons_s;
  _stubs[1].opernm = "obtain_pull_cons";
  _stubs[1].parms = yeceCa_SuppAdm_obtain_pull_cons_pars;
  _stubs[1].oper = yeceCa_SuppAdm_obtain_pull_cons_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::EventChannel */
STATICF void yeceCa_EventChannel_for_consumers_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_EventChannel_for_consumers_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ConsAdm*)args[0] = (*((struct yeceCa_EventChannel__tyimpl*)
    impldef)->for_consumers)( (yeceCa_EventChannel)or, ev);
}

STATICF void yeceCa_EventChannel_for_suppliers_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceCa_EventChannel_for_suppliers_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yeceCa_SuppAdm*)args[0] = (*((struct yeceCa_EventChannel__tyimpl*)
    impldef)->for_suppliers)( (yeceCa_EventChannel)or, ev);
}

STATICF void yeceCa_EventChannel_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_EventChannel_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceCa_EventChannel__tyimpl*)impldef)->destroy)( (
    yeceCa_EventChannel)or, ev);
}

yostub* yeceCa_EventChannel__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "for_consumers";
  _stubs[0].parms = yeceCa_EventChannel_for_consumers_pars;
  _stubs[0].oper = yeceCa_EventChannel_for_consumers_s;
  _stubs[1].opernm = "for_suppliers";
  _stubs[1].parms = yeceCa_EventChannel_for_suppliers_pars;
  _stubs[1].oper = yeceCa_EventChannel_for_suppliers_s;
  _stubs[2].opernm = "destroy";
  _stubs[2].parms = yeceCa_EventChannel_destroy_pars;
  _stubs[2].oper = yeceCa_EventChannel_destroy_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceCa::Factory */
STATICF void yeceCa_Factory_create_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_Factory_create_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yeceCa_EventChannel*)args[0] = (*((struct yeceCa_Factory__tyimpl*)
    impldef)->create)( (yeceCa_Factory)or, ev);
}

STATICF void yeceCa_Factory__get_channels_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_Factory__get_channels_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yeceCa_eventChannelList*)args[0] = (*((struct yeceCa_Factory__tyimpl*)
    impldef)->_get_channels)( (yeceCa_Factory)or, ev);
}

STATICF void yeceCa_Factory__get_self_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceCa_Factory__get_self_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yeceCa_Factory*)args[0] = (*((struct yeceCa_Factory__tyimpl*)impldef)
    ->_get_self)( (yeceCa_Factory)or, ev);
}

yostub* yeceCa_Factory__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "create";
  _stubs[0].parms = yeceCa_Factory_create_pars;
  _stubs[0].oper = yeceCa_Factory_create_s;
  _stubs[1].opernm = "_get_channels";
  _stubs[1].parms = yeceCa_Factory__get_channels_pars;
  _stubs[1].oper = yeceCa_Factory__get_channels_s;
  _stubs[2].opernm = "_get_self";
  _stubs[2].parms = yeceCa_Factory__get_self_pars;
  _stubs[2].oper = yeceCa_Factory__get_self_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTec::TypedPushCons */
STATICF void yeceTec_TypedPushCons_get_typed_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTec_TypedPushCons_get_typed_cons_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct yeceTec_TypedPushCons__tyimpl*)
    impldef)->get_typed_cons)( (yeceTec_TypedPushCons)or, ev);
}

STATICF void yeceTec_TypedPushCons_push_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceTec_TypedPushCons_push_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct yeceTec_TypedPushCons__tyimpl*)impldef)->push)( (
    yeceTec_TypedPushCons)or, ev,(yoany*)args[0]);
}

STATICF void yeceTec_TypedPushCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTec_TypedPushCons_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceTec_TypedPushCons__tyimpl*)impldef)->disconnect)( (
    yeceTec_TypedPushCons)or, ev);
}

STATICF yogfp yeceTec_TypedPushCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceTec_TypedPushCons__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceTec_TypedPushCons__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PushCons__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTec_TypedPushCons__bases[] =
{
  "IDL:yecec/PushCons:1.0",
  (char*)0
};

yostub* yeceTec_TypedPushCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)yeceTec_TypedPushCons__widen;
  _result->bases = yeceTec_TypedPushCons__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "get_typed_cons";
  _stubs[0].parms = yeceTec_TypedPushCons_get_typed_cons_pars;
  _stubs[0].oper = yeceTec_TypedPushCons_get_typed_cons_s;
  _stubs[1].opernm = "push";
  _stubs[1].parms = yeceTec_TypedPushCons_push_pars;
  _stubs[1].oper = yeceTec_TypedPushCons_push_s;
  _stubs[2].opernm = "disconnect";
  _stubs[2].parms = yeceTec_TypedPushCons_disconnect_pars;
  _stubs[2].oper = yeceTec_TypedPushCons_disconnect_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTec::TypedPullSupp */
STATICF void yeceTec_TypedPullSupp_get_typed_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTec_TypedPullSupp_get_typed_supp_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct yeceTec_TypedPullSupp__tyimpl*)
    impldef)->get_typed_supp)( (yeceTec_TypedPullSupp)or, ev);
}

STATICF void yeceTec_TypedPullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceTec_TypedPullSupp_pull_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceTec_TypedPullSupp__tyimpl*)impldef)
    ->pull)( (yeceTec_TypedPullSupp)or, ev);
}

STATICF void yeceTec_TypedPullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args);

STATICF void yeceTec_TypedPullSupp_try_pull_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceTec_TypedPullSupp__tyimpl*)impldef)
    ->try_pull)( (yeceTec_TypedPullSupp)or, ev,(boolean*)args[1]);
}

STATICF void yeceTec_TypedPullSupp_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTec_TypedPullSupp_disconnect_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceTec_TypedPullSupp__tyimpl*)impldef)->disconnect)( (
    yeceTec_TypedPullSupp)or, ev);
}

STATICF yogfp yeceTec_TypedPullSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceTec_TypedPullSupp__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceTec_TypedPullSupp__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PullSupp__id, _id) )
    _fps += 1;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTec_TypedPullSupp__bases[] =
{
  "IDL:yecec/PullSupp:1.0",
  (char*)0
};

yostub* yeceTec_TypedPullSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)yeceTec_TypedPullSupp__widen;
  _result->bases = yeceTec_TypedPullSupp__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "get_typed_supp";
  _stubs[0].parms = yeceTec_TypedPullSupp_get_typed_supp_pars;
  _stubs[0].oper = yeceTec_TypedPullSupp_get_typed_supp_s;
  _stubs[1].opernm = "pull";
  _stubs[1].parms = yeceTec_TypedPullSupp_pull_pars;
  _stubs[1].oper = yeceTec_TypedPullSupp_pull_s;
  _stubs[2].opernm = "try_pull";
  _stubs[2].parms = yeceTec_TypedPullSupp_try_pull_pars;
  _stubs[2].oper = yeceTec_TypedPullSupp_try_pull_s;
  _stubs[3].opernm = "disconnect";
  _stubs[3].parms = yeceTec_TypedPullSupp_disconnect_pars;
  _stubs[3].oper = yeceTec_TypedPullSupp_disconnect_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::TypedProxyPushCons */
STATICF void yeceTeca_TypedProxyPushCons_connect_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPushCons_connect_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceTeca_TypedProxyPushCons__tyimpl*)impldef)
    ->connect_push_supp)( (yeceTeca_TypedProxyPushCons)or, ev,*(
    yecec_PushSupp*)args[0]);
}

STATICF void yeceTeca_TypedProxyPushCons_push_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPushCons_push_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  (*((struct yeceTeca_TypedProxyPushCons__tyimpl*)impldef)->push)( (
    yeceTeca_TypedProxyPushCons)or, ev,(yoany*)args[0]);
}

STATICF void yeceTeca_TypedProxyPushCons_disconnect_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPushCons_disconnect_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceTeca_TypedProxyPushCons__tyimpl*)impldef)->disconnect)( (
    yeceTeca_TypedProxyPushCons)or, ev);
}

STATICF void yeceTeca_TypedProxyPushCons_get_typed_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPushCons_get_typed_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct yeceTeca_TypedProxyPushCons__tyimpl*)
    impldef)->get_typed_cons)( (yeceTeca_TypedProxyPushCons)or, ev);
}

STATICF yogfp yeceTeca_TypedProxyPushCons__widen( ub4 _idx, dvoid *_data, 
  CONST ysid* _id);

STATICF yogfp yeceTeca_TypedProxyPushCons__widen( ub4 _idx, dvoid *_data, 
  CONST ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPushCons__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PushCons__id, _id) )
    _fps += 1;
  else if ( ysidEq( yeceTec_TypedPushCons__id, _id) )
    _fps += 3;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTeca_TypedProxyPushCons__bases[] =
{
  "IDL:yeceCa/ProxyPushCons:1.0",
  "IDL:yecec/PushCons:1.0",
  "IDL:yeceTec/TypedPushCons:1.0",
  (char*)0
};

yostub* yeceTeca_TypedProxyPushCons__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)yeceTeca_TypedProxyPushCons__widen;
  _result->bases = yeceTeca_TypedProxyPushCons__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_push_supp";
  _stubs[0].parms = yeceTeca_TypedProxyPushCons_connect_push_supp_pars;
  _stubs[0].oper = yeceTeca_TypedProxyPushCons_connect_push_supp_s;
  _stubs[1].opernm = "push";
  _stubs[1].parms = yeceTeca_TypedProxyPushCons_push_pars;
  _stubs[1].oper = yeceTeca_TypedProxyPushCons_push_s;
  _stubs[2].opernm = "disconnect";
  _stubs[2].parms = yeceTeca_TypedProxyPushCons_disconnect_pars;
  _stubs[2].oper = yeceTeca_TypedProxyPushCons_disconnect_s;
  _stubs[3].opernm = "get_typed_cons";
  _stubs[3].parms = yeceTeca_TypedProxyPushCons_get_typed_cons_pars;
  _stubs[3].oper = yeceTeca_TypedProxyPushCons_get_typed_cons_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::TypedProxyPullSupp */
STATICF void yeceTeca_TypedProxyPullSupp_connect_pull_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPullSupp_connect_pull_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  (*((struct yeceTeca_TypedProxyPullSupp__tyimpl*)impldef)
    ->connect_pull_cons)( (yeceTeca_TypedProxyPullSupp)or, ev,*(
    yecec_PullCons*)args[0]);
}

STATICF void yeceTeca_TypedProxyPullSupp_pull_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPullSupp_pull_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceTeca_TypedProxyPullSupp__tyimpl*)
    impldef)->pull)( (yeceTeca_TypedProxyPullSupp)or, ev);
}

STATICF void yeceTeca_TypedProxyPullSupp_try_pull_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPullSupp_try_pull_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(yoany*)args[0] = (*((struct yeceTeca_TypedProxyPullSupp__tyimpl*)
    impldef)->try_pull)( (yeceTeca_TypedProxyPullSupp)or, ev,(boolean*)
    args[1]);
}

STATICF void yeceTeca_TypedProxyPullSupp_disconnect_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPullSupp_disconnect_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct yeceTeca_TypedProxyPullSupp__tyimpl*)impldef)->disconnect)( (
    yeceTeca_TypedProxyPullSupp)or, ev);
}

STATICF void yeceTeca_TypedProxyPullSupp_get_typed_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedProxyPullSupp_get_typed_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct yeceTeca_TypedProxyPullSupp__tyimpl*)
    impldef)->get_typed_supp)( (yeceTeca_TypedProxyPullSupp)or, ev);
}

STATICF yogfp yeceTeca_TypedProxyPullSupp__widen( ub4 _idx, dvoid *_data, 
  CONST ysid* _id);

STATICF yogfp yeceTeca_TypedProxyPullSupp__widen( ub4 _idx, dvoid *_data, 
  CONST ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceCa_ProxyPullSupp__id, _id) )
  {
  }
  else if ( ysidEq( yecec_PullSupp__id, _id) )
    _fps += 1;
  else if ( ysidEq( yeceTec_TypedPullSupp__id, _id) )
    _fps += 4;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTeca_TypedProxyPullSupp__bases[] =
{
  "IDL:yeceCa/ProxyPullSupp:1.0",
  "IDL:yecec/PullSupp:1.0",
  "IDL:yeceTec/TypedPullSupp:1.0",
  (char*)0
};

yostub* yeceTeca_TypedProxyPullSupp__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*5), 
    "yostubs");
  _result->widen = (yowiden)yeceTeca_TypedProxyPullSupp__widen;
  _result->bases = yeceTeca_TypedProxyPullSupp__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect_pull_cons";
  _stubs[0].parms = yeceTeca_TypedProxyPullSupp_connect_pull_cons_pars;
  _stubs[0].oper = yeceTeca_TypedProxyPullSupp_connect_pull_cons_s;
  _stubs[1].opernm = "pull";
  _stubs[1].parms = yeceTeca_TypedProxyPullSupp_pull_pars;
  _stubs[1].oper = yeceTeca_TypedProxyPullSupp_pull_s;
  _stubs[2].opernm = "try_pull";
  _stubs[2].parms = yeceTeca_TypedProxyPullSupp_try_pull_pars;
  _stubs[2].oper = yeceTeca_TypedProxyPullSupp_try_pull_s;
  _stubs[3].opernm = "disconnect";
  _stubs[3].parms = yeceTeca_TypedProxyPullSupp_disconnect_pars;
  _stubs[3].oper = yeceTeca_TypedProxyPullSupp_disconnect_s;
  _stubs[4].opernm = "get_typed_supp";
  _stubs[4].parms = yeceTeca_TypedProxyPullSupp_get_typed_supp_pars;
  _stubs[4].oper = yeceTeca_TypedProxyPullSupp_get_typed_supp_s;
  _stubs[5].opernm = (CONST char*)0;
  _stubs[5].parms = (yopar*)0;
  _stubs[5].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::TypedSuppAdm */
STATICF void yeceTeca_TypedSuppAdm_obtain_typed_push_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedSuppAdm_obtain_typed_push_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(yeceTeca_TypedProxyPushCons*)args[0] = (*((struct 
    yeceTeca_TypedSuppAdm__tyimpl*)impldef)->obtain_typed_push_cons)( (
    yeceTeca_TypedSuppAdm)or, ev,*(char**)args[1]);
}

STATICF void yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPullCons*)args[0] = (*((struct 
    yeceTeca_TypedSuppAdm__tyimpl*)impldef)->obtain_typed_pull_cons)( (
    yeceTeca_TypedSuppAdm)or, ev,*(char**)args[1]);
}

STATICF void yeceTeca_TypedSuppAdm_obtain_push_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedSuppAdm_obtain_push_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPushCons*)args[0] = (*((struct 
    yeceTeca_TypedSuppAdm__tyimpl*)impldef)->obtain_push_cons)( (
    yeceTeca_TypedSuppAdm)or, ev);
}

STATICF void yeceTeca_TypedSuppAdm_obtain_pull_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedSuppAdm_obtain_pull_cons_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPullCons*)args[0] = (*((struct 
    yeceTeca_TypedSuppAdm__tyimpl*)impldef)->obtain_pull_cons)( (
    yeceTeca_TypedSuppAdm)or, ev);
}

STATICF yogfp yeceTeca_TypedSuppAdm__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceTeca_TypedSuppAdm__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceTeca_TypedSuppAdm__id, _id) )
  {
  }
  else if ( ysidEq( yeceCa_SuppAdm__id, _id) )
    _fps += 2;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTeca_TypedSuppAdm__bases[] =
{
  "IDL:yeceCa/SuppAdm:1.0",
  (char*)0
};

yostub* yeceTeca_TypedSuppAdm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)yeceTeca_TypedSuppAdm__widen;
  _result->bases = yeceTeca_TypedSuppAdm__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "obtain_typed_push_cons";
  _stubs[0].parms = yeceTeca_TypedSuppAdm_obtain_typed_push_cons_pars;
  _stubs[0].oper = yeceTeca_TypedSuppAdm_obtain_typed_push_cons_s;
  _stubs[1].opernm = "obtain_typed_pull_cons";
  _stubs[1].parms = yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_pars;
  _stubs[1].oper = yeceTeca_TypedSuppAdm_obtain_typed_pull_cons_s;
  _stubs[2].opernm = "obtain_push_cons";
  _stubs[2].parms = yeceTeca_TypedSuppAdm_obtain_push_cons_pars;
  _stubs[2].oper = yeceTeca_TypedSuppAdm_obtain_push_cons_s;
  _stubs[3].opernm = "obtain_pull_cons";
  _stubs[3].parms = yeceTeca_TypedSuppAdm_obtain_pull_cons_pars;
  _stubs[3].oper = yeceTeca_TypedSuppAdm_obtain_pull_cons_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::TypedConsAdm */
STATICF void yeceTeca_TypedConsAdm_obtain_typed_pull_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedConsAdm_obtain_typed_pull_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(yeceTeca_TypedProxyPullSupp*)args[0] = (*((struct 
    yeceTeca_TypedConsAdm__tyimpl*)impldef)->obtain_typed_pull_supp)( (
    yeceTeca_TypedConsAdm)or, ev,*(char**)args[1]);
}

STATICF void yeceTeca_TypedConsAdm_obtain_typed_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedConsAdm_obtain_typed_push_supp_s( dvoid * or, 
  yoenv* ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPushSupp*)args[0] = (*((struct 
    yeceTeca_TypedConsAdm__tyimpl*)impldef)->obtain_typed_push_supp)( (
    yeceTeca_TypedConsAdm)or, ev,*(char**)args[1]);
}

STATICF void yeceTeca_TypedConsAdm_obtain_push_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedConsAdm_obtain_push_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPushSupp*)args[0] = (*((struct 
    yeceTeca_TypedConsAdm__tyimpl*)impldef)->obtain_push_supp)( (
    yeceTeca_TypedConsAdm)or, ev);
}

STATICF void yeceTeca_TypedConsAdm_obtain_pull_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedConsAdm_obtain_pull_supp_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceCa_ProxyPullSupp*)args[0] = (*((struct 
    yeceTeca_TypedConsAdm__tyimpl*)impldef)->obtain_pull_supp)( (
    yeceTeca_TypedConsAdm)or, ev);
}

STATICF yogfp yeceTeca_TypedConsAdm__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id);

STATICF yogfp yeceTeca_TypedConsAdm__widen( ub4 _idx, dvoid *_data, CONST 
  ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( yeceTeca_TypedConsAdm__id, _id) )
  {
  }
  else if ( ysidEq( yeceCa_ConsAdm__id, _id) )
    _fps += 2;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const yeceTeca_TypedConsAdm__bases[] =
{
  "IDL:yeceCa/ConsAdm:1.0",
  (char*)0
};

yostub* yeceTeca_TypedConsAdm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)yeceTeca_TypedConsAdm__widen;
  _result->bases = yeceTeca_TypedConsAdm__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "obtain_typed_pull_supp";
  _stubs[0].parms = yeceTeca_TypedConsAdm_obtain_typed_pull_supp_pars;
  _stubs[0].oper = yeceTeca_TypedConsAdm_obtain_typed_pull_supp_s;
  _stubs[1].opernm = "obtain_typed_push_supp";
  _stubs[1].parms = yeceTeca_TypedConsAdm_obtain_typed_push_supp_pars;
  _stubs[1].oper = yeceTeca_TypedConsAdm_obtain_typed_push_supp_s;
  _stubs[2].opernm = "obtain_push_supp";
  _stubs[2].parms = yeceTeca_TypedConsAdm_obtain_push_supp_pars;
  _stubs[2].oper = yeceTeca_TypedConsAdm_obtain_push_supp_s;
  _stubs[3].opernm = "obtain_pull_supp";
  _stubs[3].parms = yeceTeca_TypedConsAdm_obtain_pull_supp_pars;
  _stubs[3].oper = yeceTeca_TypedConsAdm_obtain_pull_supp_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::TypedEventChannel */
STATICF void yeceTeca_TypedEventChannel_for_consumers_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedEventChannel_for_consumers_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceTeca_TypedConsAdm*)args[0] = (*((struct 
    yeceTeca_TypedEventChannel__tyimpl*)impldef)->for_consumers)( (
    yeceTeca_TypedEventChannel)or, ev);
}

STATICF void yeceTeca_TypedEventChannel_for_suppliers_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args);

STATICF void yeceTeca_TypedEventChannel_for_suppliers_s( dvoid * or, yoenv* 
  ev, dvoid * impldef, dvoid ** args)
{
  *(yeceTeca_TypedSuppAdm*)args[0] = (*((struct 
    yeceTeca_TypedEventChannel__tyimpl*)impldef)->for_suppliers)( (
    yeceTeca_TypedEventChannel)or, ev);
}

yostub* yeceTeca_TypedEventChannel__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "for_consumers";
  _stubs[0].parms = yeceTeca_TypedEventChannel_for_consumers_pars;
  _stubs[0].oper = yeceTeca_TypedEventChannel_for_consumers_s;
  _stubs[1].opernm = "for_suppliers";
  _stubs[1].parms = yeceTeca_TypedEventChannel_for_suppliers_pars;
  _stubs[1].oper = yeceTeca_TypedEventChannel_for_suppliers_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::yeceTeca::Factory */
STATICF void yeceTeca_Factory_create_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceTeca_Factory_create_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yeceTeca_TypedEventChannel*)args[0] = (*((struct 
    yeceTeca_Factory__tyimpl*)impldef)->create)( (yeceTeca_Factory)or, ev);
}

STATICF void yeceTeca_Factory__get_channels_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args);

STATICF void yeceTeca_Factory__get_channels_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args)
{
  *(yeceTeca_typedEventChannelList*)args[0] = (*((struct 
    yeceTeca_Factory__tyimpl*)impldef)->_get_channels)( (yeceTeca_Factory)
    or, ev);
}

STATICF void yeceTeca_Factory__get_self_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void yeceTeca_Factory__get_self_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(yeceTeca_Factory*)args[0] = (*((struct yeceTeca_Factory__tyimpl*)
    impldef)->_get_self)( (yeceTeca_Factory)or, ev);
}

yostub* yeceTeca_Factory__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "create";
  _stubs[0].parms = yeceTeca_Factory_create_pars;
  _stubs[0].oper = yeceTeca_Factory_create_s;
  _stubs[1].opernm = "_get_channels";
  _stubs[1].parms = yeceTeca_Factory__get_channels_pars;
  _stubs[1].oper = yeceTeca_Factory__get_channels_s;
  _stubs[2].opernm = "_get_self";
  _stubs[2].parms = yeceTeca_Factory__get_self_pars;
  _stubs[2].oper = yeceTeca_Factory__get_self_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
