/* GENERATED FILE
 * mtcr - server stubs
 * from /vobs/rts/pub/mtcr.idl
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
#include <mtcrC.c>

EXTC_START

/* Server stubs for interface ::mtcr::resolve */
STATICF void mtcr_resolve_name_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mtcr_resolve_name_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mkd_segmentList*)args[0] = (*((struct mtcr_resolve__tyimpl*)impldef)
    ->name)( (mtcr_resolve)or, ev,*(char**)args[1],*(CORBA_Object*)args[2],(
    yoany*)args[3]);
}

yostub* mtcr_resolve__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "name";
  _stubs[0].parms = mtcr_resolve_name_pars;
  _stubs[0].oper = mtcr_resolve_name_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
