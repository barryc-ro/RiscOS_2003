/* GENERATED FILE
 * mzscli - client stubs
 * from /vobs/rts/pub/mzscli.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MZSCLI_IDL
#include <mzscli.h>
#endif

EXTC_START

static CONST_DATA yotk mzs_notify__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x97,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x13,'I','D','L',':','m','z','s','_','n','o'
  ,'t','i','f','y',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x0d
  ,':',':','m','z','s','_','n','o','t','i','f','y',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x14,'m','z','s','_'
  ,'n','o','t','i','f','y','_','p','l','a','y','D','o','n','e'
  ,0x00,0x00,0x00,0x00,0x12,'m','z','s','_','n','o','t','i','f'
  ,'y','_','f','i','n','i','s','h',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x13,'m','z','s','_','n','o','t','i','f','y','_','b','a','d'
  ,'F','i','l','e',0x00,0x00,0x00,0x00,0x00,0x17,'m','z','s','_'
  ,'n','o','t','i','f','y','_','s','r','v','I','n','t','e','r'
  ,'n','a','l',0x00};

yotk* mzs_notify__getTC(void)
{
  return (yotk*)mzs_notify__tc;
}

void mzs_notify__free( mzs_notify* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzs_notify, (dvoid *)val, ffunc);
}

void mzs_notify__copy( mzs_notify* dest, mzs_notify* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mzs_notify, (dvoid *)dest, (dvoid *)src, afunc);
}

/* Client stubs for interface ::mzs_clientCB */
static ysidDecl(mzs_clientCB___id) = "IDL:mzs_clientCB:1.0";

CONST ysid* mzs_clientCB__getId(void)
{
  return (CONST ysid*)mzs_clientCB___id;
}

static CONST_DATA yotk mzs_clientCB__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'3',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','z','s','_','c','l',
  'i','e','n','t','C','B',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x0f,':',':','m','z','s','_','c','l','i','e',
  'n','t','C','B',0x00};

yotk* mzs_clientCB__getTC(void)
{
  return (yotk*)mzs_clientCB__tc;
}


void mzs_clientCB__free( mzs_clientCB* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzs_clientCB, (dvoid *)val, ffunc);
}

void mzs_clientCB__copy( mzs_clientCB* dest, mzs_clientCB* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mzs_clientCB, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mzs_clientCB_endOfStream__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mzs_clientCB_endOfStream");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mzs_clientCB_endOfStream", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_mzs_notify;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mzs_clientCB_endOfStream( mzs_clientCB or, yoenv* ev, mzs_notify 
  reason)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mzs_clientCB__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mzs_clientCB__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mzs_clientCB__id);
      (*(void (*)( mzs_clientCB, yoenv*, mzs_notify))_f)(or, ev, reason);
    }
    else
      (*_impl->endOfStream)(or, ev, reason);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mzs_clientCB_endOfStream_nw( or, ev, reason, (ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mzs_clientCB_endOfStream_nw( mzs_clientCB or, yoenv* ev, mzs_notify 
  reason, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *)&reason;
  yoSendReq( (dvoid *)or, ev, "endOfStream", TRUE, _sem, (sword)1, 
    mzs_clientCB_endOfStream_pars, _parvec);
}



EXTC_END
