/* GENERATED FILE
 * mzsec - server stubs
 * from /vobs/rts/pub/mzsec.idl
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
#include <mzsecC.c>

EXTC_START

/* Server stubs for interface ::mzs::ec */
STATICF void mzs_ec_sendEvent_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_ec_sendEvent_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_ec__tyimpl*)impldef)->sendEvent)( (mzs_ec)or, ev,(
    mzs_event*)args[0]);
}

yostub* mzs_ec__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "sendEvent";
  _stubs[0].parms = mzs_ec_sendEvent_pars;
  _stubs[0].oper = mzs_ec_sendEvent_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
