/* GENERATED FILE
 * ydnmidl - server stubs
 * from /vobs/mx/pub/ydnmidl.idl
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
#include <ydnmidlC.c>

EXTC_START

/* Server stubs for interface ::ydnmInitialNamingContext */
STATICF void ydnmInitialNamingContext_get_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmInitialNamingContext_get_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(ydnmNamingContext*)args[0] = (*((struct 
    ydnmInitialNamingContext__tyimpl*)impldef)->get)( (
    ydnmInitialNamingContext)or, ev);
}

yostub* ydnmInitialNamingContext__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*1), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "get";
  _stubs[0].parms = ydnmInitialNamingContext_get_pars;
  _stubs[0].oper = ydnmInitialNamingContext_get_s;
  _stubs[1].opernm = (CONST char*)0;
  _stubs[1].parms = (yopar*)0;
  _stubs[1].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::ydnmNamingContext */
STATICF void ydnmNamingContext_bind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_bind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct ydnmNamingContext__tyimpl*)impldef)->bind)( (ydnmNamingContext)
    or, ev,(ydnmName*)args[0],*(CORBA_Object*)args[1]);
}

STATICF void ydnmNamingContext_rebind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_rebind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct ydnmNamingContext__tyimpl*)impldef)->rebind)( (
    ydnmNamingContext)or, ev,(ydnmName*)args[0],*(CORBA_Object*)args[1]);
}

STATICF void ydnmNamingContext_bind_context_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args);

STATICF void ydnmNamingContext_bind_context_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args)
{
  (*((struct ydnmNamingContext__tyimpl*)impldef)->bind_context)( (
    ydnmNamingContext)or, ev,(ydnmName*)args[0],*(CORBA_Object*)args[1]);
}

STATICF void ydnmNamingContext_resolve_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_resolve_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct ydnmNamingContext__tyimpl*)impldef)
    ->resolve)( (ydnmNamingContext)or, ev,(ydnmName*)args[1]);
}

STATICF void ydnmNamingContext_unbind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_unbind_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(CORBA_Object*)args[0] = (*((struct ydnmNamingContext__tyimpl*)impldef)
    ->unbind)( (ydnmNamingContext)or, ev,(ydnmName*)args[1]);
}

STATICF void ydnmNamingContext_new_context_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args);

STATICF void ydnmNamingContext_new_context_s( dvoid * or, yoenv* ev, dvoid 
  * impldef, dvoid ** args)
{
  *(ydnmNamingContext*)args[0] = (*((struct ydnmNamingContext__tyimpl*)
    impldef)->new_context)( (ydnmNamingContext)or, ev);
}

STATICF void ydnmNamingContext_bind_new_context_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args);

STATICF void ydnmNamingContext_bind_new_context_s( dvoid * or, yoenv* ev, 
  dvoid * impldef, dvoid ** args)
{
  *(ydnmNamingContext*)args[0] = (*((struct ydnmNamingContext__tyimpl*)
    impldef)->bind_new_context)( (ydnmNamingContext)or, ev,(ydnmName*)
    args[1]);
}

STATICF void ydnmNamingContext_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct ydnmNamingContext__tyimpl*)impldef)->destroy)( (
    ydnmNamingContext)or, ev);
}

STATICF void ydnmNamingContext_list_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmNamingContext_list_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  (*((struct ydnmNamingContext__tyimpl*)impldef)->list)( (ydnmNamingContext)
    or, ev,*(ub4*)args[0],(ydnmBindingList*)args[1],(ydnmBindingIterator*)
    args[2]);
}

yostub* ydnmNamingContext__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*9), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "bind";
  _stubs[0].parms = ydnmNamingContext_bind_pars;
  _stubs[0].oper = ydnmNamingContext_bind_s;
  _stubs[1].opernm = "rebind";
  _stubs[1].parms = ydnmNamingContext_rebind_pars;
  _stubs[1].oper = ydnmNamingContext_rebind_s;
  _stubs[2].opernm = "bind_context";
  _stubs[2].parms = ydnmNamingContext_bind_context_pars;
  _stubs[2].oper = ydnmNamingContext_bind_context_s;
  _stubs[3].opernm = "resolve";
  _stubs[3].parms = ydnmNamingContext_resolve_pars;
  _stubs[3].oper = ydnmNamingContext_resolve_s;
  _stubs[4].opernm = "unbind";
  _stubs[4].parms = ydnmNamingContext_unbind_pars;
  _stubs[4].oper = ydnmNamingContext_unbind_s;
  _stubs[5].opernm = "new_context";
  _stubs[5].parms = ydnmNamingContext_new_context_pars;
  _stubs[5].oper = ydnmNamingContext_new_context_s;
  _stubs[6].opernm = "bind_new_context";
  _stubs[6].parms = ydnmNamingContext_bind_new_context_pars;
  _stubs[6].oper = ydnmNamingContext_bind_new_context_s;
  _stubs[7].opernm = "destroy";
  _stubs[7].parms = ydnmNamingContext_destroy_pars;
  _stubs[7].oper = ydnmNamingContext_destroy_s;
  _stubs[8].opernm = "list";
  _stubs[8].parms = ydnmNamingContext_list_pars;
  _stubs[8].oper = ydnmNamingContext_list_s;
  _stubs[9].opernm = (CONST char*)0;
  _stubs[9].parms = (yopar*)0;
  _stubs[9].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}

/* Server stubs for interface ::ydnmBindingIterator */
STATICF void ydnmBindingIterator_next_one_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmBindingIterator_next_one_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(boolean*)args[0] = (*((struct ydnmBindingIterator__tyimpl*)impldef)
    ->next_one)( (ydnmBindingIterator)or, ev,(ydnmBinding*)args[1]);
}

STATICF void ydnmBindingIterator_next_n_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmBindingIterator_next_n_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args)
{
  *(boolean*)args[0] = (*((struct ydnmBindingIterator__tyimpl*)impldef)
    ->next_n)( (ydnmBindingIterator)or, ev,*(ub4*)args[1],(ydnmBindingList*)
    args[2]);
}

STATICF void ydnmBindingIterator_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args);

STATICF void ydnmBindingIterator_destroy_s( dvoid * or, yoenv* ev, dvoid * 
  impldef, dvoid ** args /* ARGUSED */)
{
  (*((struct ydnmBindingIterator__tyimpl*)impldef)->destroy)( (
    ydnmBindingIterator)or, ev);
}

yostub* ydnmBindingIterator__getStubs(void)
{
  yostub* _result;
  yostbb* _stubs;

  _result = (yostub*) ysmGlbAlloc( sizeof(yostub)+(sizeof(yostbb)*3), 
    "yostubs");
  _result->widen = (yowiden)0;
  _result->bases = (const char**)0;
  _stubs = &(_result->stuba[0]);

  _stubs[0].opernm = "next_one";
  _stubs[0].parms = ydnmBindingIterator_next_one_pars;
  _stubs[0].oper = ydnmBindingIterator_next_one_s;
  _stubs[1].opernm = "next_n";
  _stubs[1].parms = ydnmBindingIterator_next_n_pars;
  _stubs[1].oper = ydnmBindingIterator_next_n_s;
  _stubs[2].opernm = "destroy";
  _stubs[2].parms = ydnmBindingIterator_destroy_pars;
  _stubs[2].oper = ydnmBindingIterator_destroy_s;
  _stubs[3].opernm = (CONST char*)0;
  _stubs[3].parms = (yopar*)0;
  _stubs[3].oper = (void (*)(dvoid *,yoenv*,dvoid *,dvoid **))0;

  return _result;
}


EXTC_END
