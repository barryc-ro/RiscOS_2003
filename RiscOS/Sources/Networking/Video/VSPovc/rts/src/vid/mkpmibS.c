/* GENERATED FILE
 * mkpmib - server stubs
 * from /vobs/rts/inc/mkpmib.idl
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
#include <mkpmibC.c>

EXTC_START

/* Server stubs for interface ::mkpmib::snmpStm */
STATICF void mkpmib_snmpStm_getMib_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mkpmib_snmpStm_getMib_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mkpmib_snmpStm_data*)args[0] = (*((struct mkpmib_snmpStm__tyimpl*)
    impldef)->getMib)( (mkpmib_snmpStm)or, ev);
}

yostub* mkpmib_snmpStm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "getMib";
  _stubs[0].parms = mkpmib_snmpStm_getMib_pars;
  _stubs[0].oper = mkpmib_snmpStm_getMib_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mkpmib::snmpGbl */
STATICF void mkpmib_snmpGbl_getMib_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mkpmib_snmpGbl_getMib_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mkpmib_snmpGbl_data*)args[0] = (*((struct mkpmib_snmpGbl__tyimpl*)
    impldef)->getMib)( (mkpmib_snmpGbl)or, ev);
}

yostub* mkpmib_snmpGbl__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "getMib";
  _stubs[0].parms = mkpmib_snmpGbl_getMib_pars;
  _stubs[0].oper = mkpmib_snmpGbl_getMib_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
