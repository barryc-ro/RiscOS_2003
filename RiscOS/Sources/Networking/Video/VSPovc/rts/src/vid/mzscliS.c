/* GENERATED FILE
 * mzscli - server stubs
 * from /vobs/rts/pub/mzscli.idl
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
#include <mzscliC.c>

EXTC_START

/* Server stubs for interface ::mzs_clientCB */
STATICF void mzs_clientCB_endOfStream_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzs_clientCB_endOfStream_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct mzs_clientCB__tyimpl*)impldef)->endOfStream)( (mzs_clientCB)or,
     ev,*(mzs_notify*)args[0]);
}

yostub* mzs_clientCB__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "endOfStream";
  _stubs[0].parms = mzs_clientCB_endOfStream_pars;
  _stubs[0].oper = mzs_clientCB_endOfStream_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
