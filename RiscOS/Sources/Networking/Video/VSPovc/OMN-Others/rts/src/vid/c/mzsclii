/* GENERATED FILE
 * mzscli - server skeleton
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

#ifndef MZSCL_H
#include <mzscl.h>
#endif

#ifndef MZSCLII_H
#include <mzsclii.h>
#endif

externdef ysidDecl(mzs_clientCB_implid) = "MZS_ClientCallback";

/***************************************************************************/
void mzs_clientCB_EndOfStream_i( mzs_clientCB or, /* ARGUSED */yoenv* ev,
                                mzs_notify reason)
{
  mzsclCliCtx  *pCC = NULLP(mzsclCliCtx);
  ysevt        *evtCB = NULL;

  pCC = (mzsclCliCtx *)yoGetState((dvoid *)or);
  pCC->reason_mzsclCliCtx = reason;

  evtCB = ysEvtCreate(mzs_clientCB_CompletionHdlr,
                      (dvoid *) pCC, 
                      pCC->ImplCliQueCB_mzsclCliCtx, FALSE);
  ysTrigger(evtCB, NULLP(ysid), NULLP(dvoid), (size_t) 0);
}
/***************************************************************************/
 void 
 mzs_clientCB_CompletionHdlr(dvoid *usrp, CONST ysid *exid, 
             /* ARGUSED */dvoid *arg, /* ARGUSED */size_t argsz)
 {
    mzsclCliCtx  *pCC = (mzsclCliCtx  *) usrp;
    char          tmp[100];

    if (exid)
    {
       DISCARD sprintf(tmp, "mzs_clientCB_CompletionHdlr: exception=%s\n",
                       exid);
       /*MzsDebugStr(YSLSEV_ERR, tmp);*/
    }
    else
    {
       if (pCC->fup_mzsclCliCtx)
       {
          (*(pCC->fup_mzsclCliCtx)) 
                  (pCC->argv_mzsclCliCtx, pCC->reason_mzsclCliCtx);
       }
    }
 }
/***************************************************************************/
/*
 * Suggested definition only. It need not be
 * const or static, or even defined at compile time.
 */
externdef CONST_W_PTR struct mzs_clientCB__tyimpl mzs_clientCB__impl =
{
  mzs_clientCB_EndOfStream_i
};

