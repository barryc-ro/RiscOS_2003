/* GENERATED FILE
 * mzs - server stubs
 * from /vobs/rts/pub/mzs.idl
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
#include <mzsC.c>

EXTC_START

/* Server stubs for interface ::mzs::stream */
STATICF void mzs_stream_prepare_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_prepare_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzs_stream_instance*)args[0] = (*((struct mzs_stream__tyimpl*)impldef)
    ->prepare)( (mzs_stream)or, ev,*(mkd_assetCookie*)args[1],(mkd_pos*)
    args[2],(mkd_pos*)args[3],*(ub4*)args[4],*(mzs_stream_playFlags*)
    args[5],(mkd_segInfoList*)args[6],*(CORBA_Object*)args[7]);
}

STATICF void mzs_stream_prepareSequence_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzs_stream_prepareSequence_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mzs_stream_instance*)args[0] = (*((struct mzs_stream__tyimpl*)impldef)
    ->prepareSequence)( (mzs_stream)or, ev,(mkd_assetCookieList*)args[1],(
    mkd_segInfoList*)args[2],(mkd_pos*)args[3],(mkd_pos*)args[4],*(ub4*)
    args[5],*(sb4*)args[6],*(mzs_stream_playFlags*)args[7],*(mkd_prohib*)
    args[8],*(CORBA_Object*)args[9]);
}

STATICF void mzs_stream_play_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_play_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->play)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1],(mkd_pos*)args[2],(
    mkd_pos*)args[3],*(sb4*)args[4],*(ub4*)args[5]);
}

STATICF void mzs_stream_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_pause_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->pause)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1]);
}

STATICF void mzs_stream_playFwd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_playFwd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->playFwd)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1],(mkd_pos*)args[2]);
}

STATICF void mzs_stream_playRev_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_playRev_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->playRev)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1],(mkd_pos*)args[2]);
}

STATICF void mzs_stream_frameFwd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_frameFwd_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->frameFwd)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1]);
}

STATICF void mzs_stream_frameRev_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_frameRev_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->frameRev)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],(mkd_pos*)args[1]);
}

STATICF void mzs_stream_finish_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_finish_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->finish)( (mzs_stream)or, ev,*(
    mzs_stream_instance*)args[0],*(mzs_stream_finishFlags*)args[1]);
}

STATICF void mzs_stream_bootMore_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_bootMore_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->bootMore)( (mzs_stream)or, ev,(
    mkd_pos*)args[0],(mkd_pos*)args[1],*(mzs_bootMask*)args[2]);
}

STATICF void mzs_stream_bootCancel_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzs_stream_bootCancel_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->bootCancel)( (mzs_stream)or, ev,(
    mkd_pos*)args[0],(mkd_pos*)args[1],*(ub2*)args[2]);
}

STATICF void mzs_stream_setCallback_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzs_stream_setCallback_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mzs_clientCB*)args[0] = (*((struct mzs_stream__tyimpl*)impldef)
    ->setCallback)( (mzs_stream)or, ev,*(mzs_clientCB*)args[1]);
}

STATICF void mzs_stream_removeCallback_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzs_stream_removeCallback_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mzs_clientCB*)args[0] = (*((struct mzs_stream__tyimpl*)impldef)
    ->removeCallback)( (mzs_stream)or, ev);
}

STATICF void mzs_stream_query_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_query_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mzs_stream__tyimpl*)impldef)->query)( (mzs_stream)or, ev,(
    mzs_internals*)args[0]);
}

STATICF void mzs_stream_getPos_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_getPos_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkd_pos*)args[0] = (*((struct mzs_stream__tyimpl*)impldef)->getPos)( (
    mzs_stream)or, ev,*(mzs_stream_instance*)args[1],(mzs_state*)args[2],(
    sb4*)args[3]);
}

STATICF void mzs_stream_dealloc_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_stream_dealloc_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mzs_stream__tyimpl*)impldef)->dealloc)( (mzs_stream)or, ev);
}

yostub* mzs_stream__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*16), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "prepare";
  _stubs[0].parms = mzs_stream_prepare_pars;
  _stubs[0].oper = mzs_stream_prepare_s;
  _stubs[1].opernm = "prepareSequence";
  _stubs[1].parms = mzs_stream_prepareSequence_pars;
  _stubs[1].oper = mzs_stream_prepareSequence_s;
  _stubs[2].opernm = "play";
  _stubs[2].parms = mzs_stream_play_pars;
  _stubs[2].oper = mzs_stream_play_s;
  _stubs[3].opernm = "pause";
  _stubs[3].parms = mzs_stream_pause_pars;
  _stubs[3].oper = mzs_stream_pause_s;
  _stubs[4].opernm = "playFwd";
  _stubs[4].parms = mzs_stream_playFwd_pars;
  _stubs[4].oper = mzs_stream_playFwd_s;
  _stubs[5].opernm = "playRev";
  _stubs[5].parms = mzs_stream_playRev_pars;
  _stubs[5].oper = mzs_stream_playRev_s;
  _stubs[6].opernm = "frameFwd";
  _stubs[6].parms = mzs_stream_frameFwd_pars;
  _stubs[6].oper = mzs_stream_frameFwd_s;
  _stubs[7].opernm = "frameRev";
  _stubs[7].parms = mzs_stream_frameRev_pars;
  _stubs[7].oper = mzs_stream_frameRev_s;
  _stubs[8].opernm = "finish";
  _stubs[8].parms = mzs_stream_finish_pars;
  _stubs[8].oper = mzs_stream_finish_s;
  _stubs[9].opernm = "bootMore";
  _stubs[9].parms = mzs_stream_bootMore_pars;
  _stubs[9].oper = mzs_stream_bootMore_s;
  _stubs[10].opernm = "bootCancel";
  _stubs[10].parms = mzs_stream_bootCancel_pars;
  _stubs[10].oper = mzs_stream_bootCancel_s;
  _stubs[11].opernm = "setCallback";
  _stubs[11].parms = mzs_stream_setCallback_pars;
  _stubs[11].oper = mzs_stream_setCallback_s;
  _stubs[12].opernm = "removeCallback";
  _stubs[12].parms = mzs_stream_removeCallback_pars;
  _stubs[12].oper = mzs_stream_removeCallback_s;
  _stubs[13].opernm = "query";
  _stubs[13].parms = mzs_stream_query_pars;
  _stubs[13].oper = mzs_stream_query_s;
  _stubs[14].opernm = "getPos";
  _stubs[14].parms = mzs_stream_getPos_pars;
  _stubs[14].oper = mzs_stream_getPos_s;
  _stubs[15].opernm = "dealloc";
  _stubs[15].parms = mzs_stream_dealloc_pars;
  _stubs[15].oper = mzs_stream_dealloc_s;
  _stubs[16].opernm = (CONST char*)0;
  _stubs[16].parms = (yopar*)0;
  _stubs[16].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mzs::factory */
STATICF void mzs_factory_alloc_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_factory_alloc_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzs_stream*)args[0] = (*((struct mzs_factory__tyimpl*)impldef)->alloc)( 
    (mzs_factory)or, ev,(mzc_circuit*)args[1],*(mzs_capMask*)args[2],*(ub4*)
    args[3]);
}

STATICF void mzs_factory_boot_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzs_factory_boot_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzs_stream*)args[0] = (*((struct mzs_factory__tyimpl*)impldef)->boot)( (
    mzs_factory)or, ev,(mzc_circuit*)args[1],(mkd_pos*)args[2],(mkd_pos*)
    args[3],*(mzs_bootMask*)args[4],*(ub2*)args[5],(
    mzs_factory_bootRespInfo*)args[6]);
}

STATICF void mzs_factory_getStats_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args);

STATICF void mzs_factory_getStats_s( dvoid * or, yoenv* ev, dvoid * impldef,
   dvoid ** args)
{
  (*((struct mzs_factory__tyimpl*)impldef)->getStats)( (mzs_factory)or, ev,(
    mzs_stats*)args[0]);
}

yostub* mzs_factory__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "alloc";
  _stubs[0].parms = mzs_factory_alloc_pars;
  _stubs[0].oper = mzs_factory_alloc_s;
  _stubs[1].opernm = "boot";
  _stubs[1].parms = mzs_factory_boot_pars;
  _stubs[1].oper = mzs_factory_boot_s;
  _stubs[2].opernm = "getStats";
  _stubs[2].parms = mzs_factory_getStats_pars;
  _stubs[2].oper = mzs_factory_getStats_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
