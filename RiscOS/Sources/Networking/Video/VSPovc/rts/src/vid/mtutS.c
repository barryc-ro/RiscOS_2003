/* GENERATED FILE
 * mtut - server stubs
 * from mtut.idl
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
#include <mtutC.c>

EXTC_START

/* Server stubs for interface ::mtut::grab */
STATICF void mtut_grab_porQua_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_grab_porQua_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mtut_grab__tyimpl*)impldef)->porQua)( (mtut_grab)or, ev,(
    mtut_state*)args[0]);
}

STATICF void mtut_grab_tags_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_grab_tags_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mtut_grab__tyimpl*)impldef)->tags)( (mtut_grab)or, ev,(
    mtut_header*)args[0],(mtut_tagList*)args[1]);
}

yostub* mtut_grab__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*2), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "porQua";
  _stubs[0].parms = mtut_grab_porQua_pars;
  _stubs[0].oper = mtut_grab_porQua_s;
  _stubs[1].opernm = "tags";
  _stubs[1].parms = mtut_grab_tags_pars;
  _stubs[1].oper = mtut_grab_tags_s;
  _stubs[2].opernm = (CONST char*)0;
  _stubs[2].parms = (yopar*)0;
  _stubs[2].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mtut::parse */
STATICF void mtut_parse_doIt_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_parse_doIt_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mtut_parse__tyimpl*)impldef)->doIt)( (mtut_parse)or, ev,*(
    char**)args[0],*(char**)args[1],*(ub4*)args[2],*(boolean*)args[3],*(
    sysb8*)args[4],*(sysb8*)args[5],*(yoevt*)args[6],(mtut_grab*)args[7]);
}

STATICF void mtut_parse_whazzup_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_parse_whazzup_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mtut_parse__tyimpl*)impldef)->whazzup)( (mtut_parse)or, ev,(
    mtut_state*)args[0],(mtut_grabList*)args[1],(mtut_grabList*)args[2]);
}

STATICF void mtut_parse_quit_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_parse_quit_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args /* ARGUSED */)
{
  (*((struct mtut_parse__tyimpl*)impldef)->quit)( (mtut_parse)or, ev);
}

yostub* mtut_parse__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "doIt";
  _stubs[0].parms = mtut_parse_doIt_pars;
  _stubs[0].oper = mtut_parse_doIt_s;
  _stubs[1].opernm = "whazzup";
  _stubs[1].parms = mtut_parse_whazzup_pars;
  _stubs[1].oper = mtut_parse_whazzup_s;
  _stubs[2].opernm = "quit";
  _stubs[2].parms = mtut_parse_quit_pars;
  _stubs[2].oper = mtut_parse_quit_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::mtut::mstr */
STATICF void mtut_mstr_hello_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtut_mstr_hello_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  (*((struct mtut_mstr__tyimpl*)impldef)->hello)( (mtut_mstr)or, ev,*(
    mtut_parse*)args[0],(yoevt*)args[1]);
}

yostub* mtut_mstr__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "hello";
  _stubs[0].parms = mtut_mstr_hello_pars;
  _stubs[0].oper = mtut_mstr_hello_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
