/* GENERATED FILE
 * mzb - server stubs
 * from /vobs/rts/pub/mzb.idl
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
#include <mzbC.c>

EXTC_START

/* Server stubs for interface ::mzb::image */
STATICF void mzb_image_setPhys_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzb_image_setPhys_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzb_image_err*)args[0] = (*((struct mzb_image__tyimpl*)impldef)
    ->setPhys)( (mzb_image)or, ev,(mzb_image_mnnpa*)args[1],*(char**)
    args[2],*(ub2*)args[3]);
}

STATICF void mzb_image_get_s( dvoid * or, yoenv* ev, dvoid * impldef, dvoid 
  ** args);

STATICF void mzb_image_get_s( dvoid * or, yoenv* ev, dvoid * impldef, dvoid 
  ** args)
{
  *(mzb_image_err*)args[0] = (*((struct mzb_image__tyimpl*)impldef)->get)( (
    mzb_image)or, ev,(mzb_image_mnnpa*)args[1],(char**)args[2]);
}

STATICF void mzb_image_updateDB_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args);

STATICF void mzb_image_updateDB_s( dvoid * or, yoenv* ev, dvoid * impldef, 
  dvoid ** args)
{
  *(mzb_image_err*)args[0] = (*((struct mzb_image__tyimpl*)impldef)
    ->updateDB)( (mzb_image)or, ev,*(char**)args[1],*(boolean*)args[2]);
}

STATICF void mzb_image_dumpMappings_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void mzb_image_dumpMappings_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(mzb_image_err*)args[0] = (*((struct mzb_image__tyimpl*)impldef)
    ->dumpMappings)( (mzb_image)or, ev);
}

yostub* mzb_image__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*4), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "setPhys";
  _stubs[0].parms = mzb_image_setPhys_pars;
  _stubs[0].oper = mzb_image_setPhys_s;
  _stubs[1].opernm = "get";
  _stubs[1].parms = mzb_image_get_pars;
  _stubs[1].oper = mzb_image_get_s;
  _stubs[2].opernm = "updateDB";
  _stubs[2].parms = mzb_image_updateDB_pars;
  _stubs[2].oper = mzb_image_updateDB_s;
  _stubs[3].opernm = "dumpMappings";
  _stubs[3].parms = mzb_image_dumpMappings_pars;
  _stubs[3].oper = mzb_image_dumpMappings_s;
  _stubs[4].opernm = (CONST char*)0;
  _stubs[4].parms = (yopar*)0;
  _stubs[4].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
