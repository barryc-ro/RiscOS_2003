/* GENERATED FILE
 * mkpc - server stubs
 * from mkpc.idl
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
#include <mkpcC.c>

EXTC_START

/* Server stubs for interface ::mkpc::stmCB */
STATICF void mkpc_stmCB_cmdStatus_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args);

STATICF void mkpc_stmCB_cmdStatus_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args)
{
  (*((struct mkpc_stmCB__tyimpl*)impldef)->cmdStatus)( (mkpc_stmCB)or, ev,*(
    ub4*)args[0],*(mkpc_stmCB_status*)args[1]);
}

yostub* mkpc_stmCB__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "cmdStatus";
  _stubs[0].parms = mkpc_stmCB_cmdStatus_pars;
  _stubs[0].oper = mkpc_stmCB_cmdStatus_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mkpc::stm */
STATICF void mkpc_stm_connect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_connect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->connect)( (mkpc_stm)or, ev,*(
    mkpc_stmCB*)args[0],*(ub4*)args[1],(ub4*)args[2]);
}

STATICF void mkpc_stm_newCmd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_newCmd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->newCmd)( (mkpc_stm)or, ev,(
    mkpc_stm_cmdLst*)args[0]);
}

STATICF void mkpc_stm_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->pause)( (mkpc_stm)or, ev);
}

STATICF void mkpc_stm_unPause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_unPause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->unPause)( (mkpc_stm)or, ev);
}

STATICF void mkpc_stm_dumpOn_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_dumpOn_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->dumpOn)( (mkpc_stm)or, ev,*(char**)
    args[0],*(ub4*)args[1]);
}

STATICF void mkpc_stm_dumpOff_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_dumpOff_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->dumpOff)( (mkpc_stm)or, ev);
}

STATICF void mkpc_stm_getState_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_getState_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->getState)( (mkpc_stm)or, ev,(ub4*)
    args[0],(sysb8*)args[1]);
}

STATICF void mkpc_stm_disconnect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_stm_disconnect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpc_stm__tyimpl*)impldef)->disconnect)( (mkpc_stm)or, ev);
}

yostub* mkpc_stm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*8), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect";
  _stubs[0].parms = mkpc_stm_connect_pars;
  _stubs[0].oper = mkpc_stm_connect_s;
  _stubs[1].opernm = "newCmd";
  _stubs[1].parms = mkpc_stm_newCmd_pars;
  _stubs[1].oper = mkpc_stm_newCmd_s;
  _stubs[2].opernm = "pause";
  _stubs[2].parms = mkpc_stm_pause_pars;
  _stubs[2].oper = mkpc_stm_pause_s;
  _stubs[3].opernm = "unPause";
  _stubs[3].parms = mkpc_stm_unPause_pars;
  _stubs[3].oper = mkpc_stm_unPause_s;
  _stubs[4].opernm = "dumpOn";
  _stubs[4].parms = mkpc_stm_dumpOn_pars;
  _stubs[4].oper = mkpc_stm_dumpOn_s;
  _stubs[5].opernm = "dumpOff";
  _stubs[5].parms = mkpc_stm_dumpOff_pars;
  _stubs[5].oper = mkpc_stm_dumpOff_s;
  _stubs[6].opernm = "getState";
  _stubs[6].parms = mkpc_stm_getState_pars;
  _stubs[6].oper = mkpc_stm_getState_s;
  _stubs[7].opernm = "disconnect";
  _stubs[7].parms = mkpc_stm_disconnect_pars;
  _stubs[7].oper = mkpc_stm_disconnect_s;
  _stubs[8].opernm = (CONST char*)0;
  _stubs[8].parms = (yopar*)0;
  _stubs[8].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mkpc::gbl */
STATICF void mkpc_gbl_shutdown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_gbl_shutdown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpc_gbl__tyimpl*)impldef)->shutdown)( (mkpc_gbl)or, ev);
}

STATICF void mkpc_gbl_getStream_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpc_gbl_getStream_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkpc_stm*)args[0] = (*((struct mkpc_gbl__tyimpl*)impldef)->getStream)( (
    mkpc_gbl)or, ev,*(sb2*)args[1]);
}

yostub* mkpc_gbl__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "shutdown";
  _stubs[0].parms = mkpc_gbl_shutdown_pars;
  _stubs[0].oper = mkpc_gbl_shutdown_s;
  _stubs[1].opernm = "getStream";
  _stubs[1].parms = mkpc_gbl_getStream_pars;
  _stubs[1].oper = mkpc_gbl_getStream_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
