/* GENERATED FILE
 * mkpi - server stubs
 * from /vobs/rts/src/vid/mkpi.idl
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
#include <mkpiC.c>

EXTC_START

/* Server stubs for interface ::mkpi::stmCB */
STATICF void mkpi_stmCB_cmdStatus_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args);

STATICF void mkpi_stmCB_cmdStatus_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args)
{
  (*((struct mkpi_stmCB__tyimpl*)impldef)->cmdStatus)( (mkpi_stmCB)or, ev,*(
    ub4*)args[0],*(mkpc_stmCB_status*)args[1]);
}

STATICF yogfp mkpi_stmCB__widen( ub4 _idx, dvoid *_data, CONST ysid* _id);

STATICF yogfp mkpi_stmCB__widen( ub4 _idx, dvoid *_data, CONST ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( mkpc_stmCB__id, _id) )
  {
  }
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const mkpi_stmCB__bases[] =
{
  "IDL:mkpc/stmCB:1.0",
  (char*)0
};

yostub* mkpi_stmCB__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)mkpi_stmCB__widen;
  _result->bases = mkpi_stmCB__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "cmdStatus";
  _stubs[0].parms = mkpi_stmCB_cmdStatus_pars;
  _stubs[0].oper = mkpi_stmCB_cmdStatus_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mkpi::stm */
STATICF void mkpi_stm_connect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_connect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->connect)( (mkpi_stm)or, ev,*(
    mkpc_stmCB*)args[0],*(ub4*)args[1],(ub4*)args[2]);
}

STATICF void mkpi_stm_newCmd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_newCmd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->newCmd)( (mkpi_stm)or, ev,(
    mkpc_stm_cmdLst*)args[0]);
}

STATICF void mkpi_stm_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->pause)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_unPause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_unPause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->unPause)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_dumpOn_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_dumpOn_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->dumpOn)( (mkpi_stm)or, ev,*(char**)
    args[0],*(ub4*)args[1]);
}

STATICF void mkpi_stm_dumpOff_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_dumpOff_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->dumpOff)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_getState_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_getState_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->getState)( (mkpi_stm)or, ev,(ub4*)
    args[0],(sysb8*)args[1]);
}

STATICF void mkpi_stm_disconnect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_disconnect_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->disconnect)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_getMib_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_getMib_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkpmib_snmpStm_data*)args[0] = (*((struct mkpi_stm__tyimpl*)impldef)
    ->getMib)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_GetInfo_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_GetInfo_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzc_chnlInfo*)args[0] = (*((struct mkpi_stm__tyimpl*)impldef)->GetInfo)(
     (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_Rebuild_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_Rebuild_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzc_chnlInfo*)args[0] = (*((struct mkpi_stm__tyimpl*)impldef)->Rebuild)(
     (mkpi_stm)or, ev,(mzc_chnlreqx*)args[1]);
}

STATICF void mkpi_stm_Enable_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_Enable_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->Enable)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_Disable_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_Disable_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->Disable)( (mkpi_stm)or, ev);
}

STATICF void mkpi_stm_TearDown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_stm_TearDown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_stm__tyimpl*)impldef)->TearDown)( (mkpi_stm)or, ev);
}

STATICF yogfp mkpi_stm__widen( ub4 _idx, dvoid *_data, CONST ysid* _id);

STATICF yogfp mkpi_stm__widen( ub4 _idx, dvoid *_data, CONST ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( mkpc_stm__id, _id) )
  {
  }
  else if ( ysidEq( mkpmib_snmpStm__id, _id) )
    _fps += 8;
  else if ( ysidEq( mzc_chnl__id, _id) )
    _fps += 9;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const mkpi_stm__bases[] =
{
  "IDL:mkpc/stm:1.0",
  "IDL:mkpmib/snmpStm:1.0",
  "IDL:mzc/chnl:1.0",
  (char*)0
};

yostub* mkpi_stm__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*14), 
    "yostubs");
  _result->widen = (yowiden)mkpi_stm__widen;
  _result->bases = mkpi_stm__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "connect";
  _stubs[0].parms = mkpi_stm_connect_pars;
  _stubs[0].oper = mkpi_stm_connect_s;
  _stubs[1].opernm = "newCmd";
  _stubs[1].parms = mkpi_stm_newCmd_pars;
  _stubs[1].oper = mkpi_stm_newCmd_s;
  _stubs[2].opernm = "pause";
  _stubs[2].parms = mkpi_stm_pause_pars;
  _stubs[2].oper = mkpi_stm_pause_s;
  _stubs[3].opernm = "unPause";
  _stubs[3].parms = mkpi_stm_unPause_pars;
  _stubs[3].oper = mkpi_stm_unPause_s;
  _stubs[4].opernm = "dumpOn";
  _stubs[4].parms = mkpi_stm_dumpOn_pars;
  _stubs[4].oper = mkpi_stm_dumpOn_s;
  _stubs[5].opernm = "dumpOff";
  _stubs[5].parms = mkpi_stm_dumpOff_pars;
  _stubs[5].oper = mkpi_stm_dumpOff_s;
  _stubs[6].opernm = "getState";
  _stubs[6].parms = mkpi_stm_getState_pars;
  _stubs[6].oper = mkpi_stm_getState_s;
  _stubs[7].opernm = "disconnect";
  _stubs[7].parms = mkpi_stm_disconnect_pars;
  _stubs[7].oper = mkpi_stm_disconnect_s;
  _stubs[8].opernm = "getMib";
  _stubs[8].parms = mkpi_stm_getMib_pars;
  _stubs[8].oper = mkpi_stm_getMib_s;
  _stubs[9].opernm = "GetInfo";
  _stubs[9].parms = mkpi_stm_GetInfo_pars;
  _stubs[9].oper = mkpi_stm_GetInfo_s;
  _stubs[10].opernm = "Rebuild";
  _stubs[10].parms = mkpi_stm_Rebuild_pars;
  _stubs[10].oper = mkpi_stm_Rebuild_s;
  _stubs[11].opernm = "Enable";
  _stubs[11].parms = mkpi_stm_Enable_pars;
  _stubs[11].oper = mkpi_stm_Enable_s;
  _stubs[12].opernm = "Disable";
  _stubs[12].parms = mkpi_stm_Disable_pars;
  _stubs[12].oper = mkpi_stm_Disable_s;
  _stubs[13].opernm = "TearDown";
  _stubs[13].parms = mkpi_stm_TearDown_pars;
  _stubs[13].oper = mkpi_stm_TearDown_s;
  _stubs[14].opernm = (CONST char*)0;
  _stubs[14].parms = (yopar*)0;
  _stubs[14].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mkpi::gbl */
STATICF void mkpi_gbl_shutdown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_gbl_shutdown_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mkpi_gbl__tyimpl*)impldef)->shutdown)( (mkpi_gbl)or, ev);
}

STATICF void mkpi_gbl_getStream_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_gbl_getStream_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkpc_stm*)args[0] = (*((struct mkpi_gbl__tyimpl*)impldef)->getStream)( (
    mkpi_gbl)or, ev,*(sb2*)args[1]);
}

STATICF void mkpi_gbl_getMib_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_gbl_getMib_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkpmib_snmpGbl_data*)args[0] = (*((struct mkpi_gbl__tyimpl*)impldef)
    ->getMib)( (mkpi_gbl)or, ev);
}

STATICF void mkpi_gbl_GetInfo_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_gbl_GetInfo_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzc_chnlPrvInfo*)args[0] = (*((struct mkpi_gbl__tyimpl*)impldef)
    ->GetInfo)( (mkpi_gbl)or, ev);
}

STATICF void mkpi_gbl_Build_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mkpi_gbl_Build_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzc_channel*)args[0] = (*((struct mkpi_gbl__tyimpl*)impldef)->Build)( (
    mkpi_gbl)or, ev,(mzc_chnlreqx*)args[1]);
}

STATICF yogfp mkpi_gbl__widen( ub4 _idx, dvoid *_data, CONST ysid* _id);

STATICF yogfp mkpi_gbl__widen( ub4 _idx, dvoid *_data, CONST ysid* _id)
{
  yogfp* _fps = (yogfp*)_data;

  if ( ysidEq( mkpc_gbl__id, _id) )
  {
  }
  else if ( ysidEq( mkpmib_snmpGbl__id, _id) )
    _fps += 2;
  else if ( ysidEq( mzc_chnlPrv__id, _id) )
    _fps += 3;
  else 
    yseThrow(YS_EX_BADPARAM);
  return _fps[_idx];
}

static const char* const mkpi_gbl__bases[] =
{
  "IDL:mkpc/gbl:1.0",
  "IDL:mkpmib/snmpGbl:1.0",
  "IDL:mzc/chnlPrv:1.0",
  (char*)0
};

yostub* mkpi_gbl__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*5), 
    "yostubs");
  _result->widen = (yowiden)mkpi_gbl__widen;
  _result->bases = mkpi_gbl__bases;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "shutdown";
  _stubs[0].parms = mkpi_gbl_shutdown_pars;
  _stubs[0].oper = mkpi_gbl_shutdown_s;
  _stubs[1].opernm = "getStream";
  _stubs[1].parms = mkpi_gbl_getStream_pars;
  _stubs[1].oper = mkpi_gbl_getStream_s;
  _stubs[2].opernm = "getMib";
  _stubs[2].parms = mkpi_gbl_getMib_pars;
  _stubs[2].oper = mkpi_gbl_getMib_s;
  _stubs[3].opernm = "GetInfo";
  _stubs[3].parms = mkpi_gbl_GetInfo_pars;
  _stubs[3].oper = mkpi_gbl_GetInfo_s;
  _stubs[4].opernm = "Build";
  _stubs[4].parms = mkpi_gbl_Build_pars;
  _stubs[4].oper = mkpi_gbl_Build_s;
  _stubs[5].opernm = (CONST char*)0;
  _stubs[5].parms = (yopar*)0;
  _stubs[5].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
