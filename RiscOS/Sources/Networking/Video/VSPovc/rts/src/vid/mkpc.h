/* GENERATED FILE
 * mkpc - public declarations
 * from mkpc.idl
 */

#ifndef MKPC_IDL
#define MKPC_IDL

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

EXTC_START

/**********  SEQUENCE DECLARATIONS *********/
#ifndef mkpc_stm_seg_DECLARED
#define mkpc_stm_seg_DECLARED
typedef struct mkpc_stm_seg mkpc_stm_seg;
#endif /* mkpc_stm_seg_DECLARED */

#ifndef YCIDL_sequence_mkpc_stm_seg_DEFINED
#define YCIDL_sequence_mkpc_stm_seg_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mkpc_stm_seg* _buffer;
} YCIDL_sequence_mkpc_stm_seg;
#ifndef YCIDL_sequence_mkpc_stm_seg_SUPP_FUNCS
#define YCIDL_sequence_mkpc_stm_seg_SUPP_FUNCS
#endif /* YCIDL_sequence_mkpc_stm_seg_SUPP_FUNCS */

#endif /* YCIDL_sequence_mkpc_stm_seg_DEFINED */

#ifndef mkpc_stm_cmd_DECLARED
#define mkpc_stm_cmd_DECLARED
typedef struct mkpc_stm_cmd mkpc_stm_cmd;
#endif /* mkpc_stm_cmd_DECLARED */

#ifndef YCIDL_sequence_mkpc_stm_cmd_DEFINED
#define YCIDL_sequence_mkpc_stm_cmd_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mkpc_stm_cmd* _buffer;
} YCIDL_sequence_mkpc_stm_cmd;
#ifndef YCIDL_sequence_mkpc_stm_cmd_SUPP_FUNCS
#define YCIDL_sequence_mkpc_stm_cmd_SUPP_FUNCS
#endif /* YCIDL_sequence_mkpc_stm_cmd_SUPP_FUNCS */

#endif /* YCIDL_sequence_mkpc_stm_cmd_DEFINED */

/******* NON-SEQUENCE DECLARATIONS *******/
#ifndef MKPC_EX_NOTCONNECT_DECLARED
#define MKPC_EX_NOTCONNECT_DECLARED
CONST ysid* mkpc_NotConnect__getId(void);
#ifndef MKPC_EX_NOTCONNECT
#define MKPC_EX_NOTCONNECT   (mkpc_NotConnect__getId())
#endif
#endif /* MKPC_EX_NOTCONNECT_DECLARED */

yotk* mkpc_NotConnect__getTC(void);
#ifndef YCTC_mkpc_NotConnect
#define YCTC_mkpc_NotConnect   (mkpc_NotConnect__getTC())
#endif
#ifndef MKPC_EX_BADPARAM_DECLARED
#define MKPC_EX_BADPARAM_DECLARED
CONST ysid* mkpc_BadParam__getId(void);
#ifndef MKPC_EX_BADPARAM
#define MKPC_EX_BADPARAM   (mkpc_BadParam__getId())
#endif
#endif /* MKPC_EX_BADPARAM_DECLARED */

yotk* mkpc_BadParam__getTC(void);
#ifndef YCTC_mkpc_BadParam
#define YCTC_mkpc_BadParam   (mkpc_BadParam__getTC())
#endif
#ifndef MKPC_EX_ALIGN_DECLARED
#define MKPC_EX_ALIGN_DECLARED
CONST ysid* mkpc_Align__getId(void);
#ifndef MKPC_EX_ALIGN
#define MKPC_EX_ALIGN   (mkpc_Align__getId())
#endif
#endif /* MKPC_EX_ALIGN_DECLARED */

yotk* mkpc_Align__getTC(void);
#ifndef YCTC_mkpc_Align
#define YCTC_mkpc_Align   (mkpc_Align__getTC())
#endif
#ifndef MKPC_EX_TIMEOUT_DECLARED
#define MKPC_EX_TIMEOUT_DECLARED
CONST ysid* mkpc_Timeout__getId(void);
#ifndef MKPC_EX_TIMEOUT
#define MKPC_EX_TIMEOUT   (mkpc_Timeout__getId())
#endif
#endif /* MKPC_EX_TIMEOUT_DECLARED */

yotk* mkpc_Timeout__getTC(void);
#ifndef YCTC_mkpc_Timeout
#define YCTC_mkpc_Timeout   (mkpc_Timeout__getTC())
#endif
#ifndef MKPC_EX_MDSOP_DECLARED
#define MKPC_EX_MDSOP_DECLARED
CONST ysid* mkpc_MdsOp__getId(void);
#ifndef MKPC_EX_MDSOP
#define MKPC_EX_MDSOP   (mkpc_MdsOp__getId())
#endif
#endif /* MKPC_EX_MDSOP_DECLARED */

yotk* mkpc_MdsOp__getTC(void);
#ifndef YCTC_mkpc_MdsOp
#define YCTC_mkpc_MdsOp   (mkpc_MdsOp__getTC())
#endif
#ifndef MKPC_EX_DSMOP_DECLARED
#define MKPC_EX_DSMOP_DECLARED
CONST ysid* mkpc_DsmOp__getId(void);
#ifndef MKPC_EX_DSMOP
#define MKPC_EX_DSMOP   (mkpc_DsmOp__getId())
#endif
#endif /* MKPC_EX_DSMOP_DECLARED */

yotk* mkpc_DsmOp__getTC(void);
#ifndef YCTC_mkpc_DsmOp
#define YCTC_mkpc_DsmOp   (mkpc_DsmOp__getTC())
#endif
#ifndef MKPC_EX_FAILURE_DECLARED
#define MKPC_EX_FAILURE_DECLARED
CONST ysid* mkpc_Failure__getId(void);
#ifndef MKPC_EX_FAILURE
#define MKPC_EX_FAILURE   (mkpc_Failure__getId())
#endif
#endif /* MKPC_EX_FAILURE_DECLARED */

yotk* mkpc_Failure__getTC(void);
#ifndef YCTC_mkpc_Failure
#define YCTC_mkpc_Failure   (mkpc_Failure__getTC())
#endif
#ifndef MKPC_EX_INTERNAL_DECLARED
#define MKPC_EX_INTERNAL_DECLARED
CONST ysid* mkpc_Internal__getId(void);
#ifndef MKPC_EX_INTERNAL
#define MKPC_EX_INTERNAL   (mkpc_Internal__getId())
#endif
#endif /* MKPC_EX_INTERNAL_DECLARED */

yotk* mkpc_Internal__getTC(void);
#ifndef YCTC_mkpc_Internal
#define YCTC_mkpc_Internal   (mkpc_Internal__getTC())
#endif

/* interface mkpc_stmCB declarations */
#ifndef mkpc_stmCB_DECLARED
#define mkpc_stmCB_DECLARED
typedef struct YCmkpc_stmCB* mkpc_stmCB;
yotk* mkpc_stmCB__getTC(void);
#ifndef YCTC_mkpc_stmCB
#define YCTC_mkpc_stmCB   (mkpc_stmCB__getTC())
#endif
#endif /* mkpc_stmCB_DECLARED */

#ifndef mkpc_stmCB_SUPP_FUNCS
#define mkpc_stmCB_SUPP_FUNCS
void mkpc_stmCB__free( mkpc_stmCB* val, ysmff ffunc);
void mkpc_stmCB__copy( mkpc_stmCB* dest, mkpc_stmCB* src, ysmaf afunc);
#endif /* mkpc_stmCB_SUPP_FUNCS */

#ifndef mkpc_stmCB_DEFINED
#define mkpc_stmCB_DEFINED
struct yostub* mkpc_stmCB__getStubs(void);
#define mkpc_stmCB__stubs (mkpc_stmCB__getStubs())
#endif /* mkpc_stmCB_DEFINED */

CONST ysid* mkpc_stmCB__getId(void);
#ifndef mkpc_stmCB__id
#define mkpc_stmCB__id   (mkpc_stmCB__getId())
#endif
#ifndef mkpc_stmCB_status_DECLARED
#define mkpc_stmCB_status_DECLARED
typedef ub4 mkpc_stmCB_status;
yotk* mkpc_stmCB_status__getTC(void);
#ifndef YCTC_mkpc_stmCB_status
#define YCTC_mkpc_stmCB_status   (mkpc_stmCB_status__getTC())
#endif
#endif /* mkpc_stmCB_status_DECLARED */

#ifndef mkpc_stmCB_status_DEFINED
#define mkpc_stmCB_status_DEFINED
#define mkpc_stmCB_statusDone ((mkpc_stmCB_status) 0)
#define mkpc_stmCB_statusBadFile ((mkpc_stmCB_status) 1)
#define mkpc_stmCB_statusBadPos ((mkpc_stmCB_status) 2)
#define mkpc_stmCB_statusBadLen ((mkpc_stmCB_status) 3)
#define mkpc_stmCB_statusErrFlush ((mkpc_stmCB_status) 4)
#define mkpc_stmCB_statusIntErr ((mkpc_stmCB_status) 5)
#endif /* mkpc_stmCB_status_DEFINED */

#ifndef mkpc_stmCB_status_SUPP_FUNCS
#define mkpc_stmCB_status_SUPP_FUNCS
void mkpc_stmCB_status__free( mkpc_stmCB_status* val, ysmff ffunc);
void mkpc_stmCB_status__copy( mkpc_stmCB_status* dest, mkpc_stmCB_status* 
  src, ysmaf afunc);
#endif /* mkpc_stmCB_status_SUPP_FUNCS */

void mkpc_stmCB_cmdStatus( mkpc_stmCB or, yoenv* ev, ub4 cmdRef, 
  mkpc_stmCB_status state);
void mkpc_stmCB_cmdStatus_nw( mkpc_stmCB or, yoenv* ev, ub4 cmdRef, 
  mkpc_stmCB_status state, ysevt* _sem);
yopar* mkpc_stmCB_cmdStatus__getPars(void);
#ifndef mkpc_stmCB_cmdStatus_pars
#define mkpc_stmCB_cmdStatus_pars (mkpc_stmCB_cmdStatus__getPars())
#endif

#ifndef mkpc_stmCB__tyimpl_DEFINED
#define mkpc_stmCB__tyimpl_DEFINED
struct mkpc_stmCB__tyimpl
{
  void (*cmdStatus)( mkpc_stmCB, yoenv*, ub4, mkpc_stmCB_status);
};
#endif /* mkpc_stmCB__tyimpl_DEFINED */


/* interface mkpc_stm declarations */
#ifndef mkpc_stm_DECLARED
#define mkpc_stm_DECLARED
typedef struct YCmkpc_stm* mkpc_stm;
yotk* mkpc_stm__getTC(void);
#ifndef YCTC_mkpc_stm
#define YCTC_mkpc_stm   (mkpc_stm__getTC())
#endif
#endif /* mkpc_stm_DECLARED */

#ifndef mkpc_stm_SUPP_FUNCS
#define mkpc_stm_SUPP_FUNCS
void mkpc_stm__free( mkpc_stm* val, ysmff ffunc);
void mkpc_stm__copy( mkpc_stm* dest, mkpc_stm* src, ysmaf afunc);
#endif /* mkpc_stm_SUPP_FUNCS */

#ifndef mkpc_stm_DEFINED
#define mkpc_stm_DEFINED
struct yostub* mkpc_stm__getStubs(void);
#define mkpc_stm__stubs (mkpc_stm__getStubs())
#endif /* mkpc_stm_DEFINED */

CONST ysid* mkpc_stm__getId(void);
#ifndef mkpc_stm__id
#define mkpc_stm__id   (mkpc_stm__getId())
#endif
#ifndef mkpc_stm_wrap_DECLARED
#define mkpc_stm_wrap_DECLARED
typedef ub4 mkpc_stm_wrap;
yotk* mkpc_stm_wrap__getTC(void);
#ifndef YCTC_mkpc_stm_wrap
#define YCTC_mkpc_stm_wrap   (mkpc_stm_wrap__getTC())
#endif
#endif /* mkpc_stm_wrap_DECLARED */

#ifndef mkpc_stm_wrap_DEFINED
#define mkpc_stm_wrap_DEFINED
#define mkpc_stm_NoWrap ((mkpc_stm_wrap) 0)
#define mkpc_stm_GenericWrap ((mkpc_stm_wrap) 1)
#define mkpc_stm_Mpeg2Wrap ((mkpc_stm_wrap) 2)
#endif /* mkpc_stm_wrap_DEFINED */

#ifndef mkpc_stm_wrap_SUPP_FUNCS
#define mkpc_stm_wrap_SUPP_FUNCS
void mkpc_stm_wrap__free( mkpc_stm_wrap* val, ysmff ffunc);
void mkpc_stm_wrap__copy( mkpc_stm_wrap* dest, mkpc_stm_wrap* src, ysmaf 
  afunc);
#endif /* mkpc_stm_wrap_SUPP_FUNCS */

#ifndef mkpc_stm_ins_DECLARED
#define mkpc_stm_ins_DECLARED
typedef struct mkpc_stm_ins mkpc_stm_ins;
#endif /* mkpc_stm_ins_DECLARED */

#ifndef mkpc_stm_ins_DEFINED
#define mkpc_stm_ins_DEFINED
struct mkpc_stm_ins
{
  mkpc_stm_wrap mkpcInsWrp;
  YCIDL_sequence_ub1 mkpcInsDat;
  sb4 mkpcInsNul;
};
yotk* mkpc_stm_ins__getTC(void);
#ifndef YCTC_mkpc_stm_ins
#define YCTC_mkpc_stm_ins   (mkpc_stm_ins__getTC())
#endif
#endif /* mkpc_stm_ins_DEFINED */

#ifndef mkpc_stm_ins_SUPP_FUNCS
#define mkpc_stm_ins_SUPP_FUNCS
void mkpc_stm_ins__free( mkpc_stm_ins* val, ysmff ffunc);
void mkpc_stm_ins__copy( mkpc_stm_ins* dest, mkpc_stm_ins* src, ysmaf afunc)
  ;
#endif /* mkpc_stm_ins_SUPP_FUNCS */

#ifndef mkpc_stm_MAXPIDS_DECLARED
#define mkpc_stm_MAXPIDS_DECLARED
#define mkpc_stm_MAXPIDS ((sb2) 3)
#endif /* mkpc_stm_MAXPIDS_DECLARED */

#ifndef mkpc_stm_seg_DECLARED
#define mkpc_stm_seg_DECLARED
typedef struct mkpc_stm_seg mkpc_stm_seg;
#endif /* mkpc_stm_seg_DECLARED */

#ifndef mkpc_stm_seg_DEFINED
#define mkpc_stm_seg_DEFINED
struct mkpc_stm_seg
{
  sysb8 mkpcSegPos;
  sysb8 mkpcSegLen;
  ub2 mkpcSegPCnt;
  ub2 mkpcSegPMsk[3];
  mkpc_stm_ins mkpcSegPpd;
  mkpc_stm_ins mkpcSegApd;
};
yotk* mkpc_stm_seg__getTC(void);
#ifndef YCTC_mkpc_stm_seg
#define YCTC_mkpc_stm_seg   (mkpc_stm_seg__getTC())
#endif
#endif /* mkpc_stm_seg_DEFINED */

#ifndef mkpc_stm_seg_SUPP_FUNCS
#define mkpc_stm_seg_SUPP_FUNCS
void mkpc_stm_seg__free( mkpc_stm_seg* val, ysmff ffunc);
void mkpc_stm_seg__copy( mkpc_stm_seg* dest, mkpc_stm_seg* src, ysmaf afunc)
  ;
#endif /* mkpc_stm_seg_SUPP_FUNCS */

#ifndef mkpc_stm_cmd_DECLARED
#define mkpc_stm_cmd_DECLARED
typedef struct mkpc_stm_cmd mkpc_stm_cmd;
#endif /* mkpc_stm_cmd_DECLARED */

#ifndef mkpc_stm_cmd_DEFINED
#define mkpc_stm_cmd_DEFINED
struct mkpc_stm_cmd
{
  char* mkpcCmdFnm;
  mkpc_stm_wrap mkpcCmdFwp;
  ub4 mkpcCmdBps;
  ub4 mkpcCmdRef;
  boolean mkpcCmdFlsh;
  boolean mkpcCmdRel;
  YCIDL_sequence_mkpc_stm_seg mkpcCmdSegs;
};
yotk* mkpc_stm_cmd__getTC(void);
#ifndef YCTC_mkpc_stm_cmd
#define YCTC_mkpc_stm_cmd   (mkpc_stm_cmd__getTC())
#endif
#endif /* mkpc_stm_cmd_DEFINED */

#ifndef mkpc_stm_cmd_SUPP_FUNCS
#define mkpc_stm_cmd_SUPP_FUNCS
void mkpc_stm_cmd__free( mkpc_stm_cmd* val, ysmff ffunc);
void mkpc_stm_cmd__copy( mkpc_stm_cmd* dest, mkpc_stm_cmd* src, ysmaf afunc)
  ;
#endif /* mkpc_stm_cmd_SUPP_FUNCS */

#ifndef mkpc_stm_cmdLst_DECLARED
#define mkpc_stm_cmdLst_DECLARED
typedef YCIDL_sequence_mkpc_stm_cmd mkpc_stm_cmdLst;
yotk* mkpc_stm_cmdLst__getTC(void);
#ifndef YCTC_mkpc_stm_cmdLst
#define YCTC_mkpc_stm_cmdLst   (mkpc_stm_cmdLst__getTC())
#endif
#endif /* mkpc_stm_cmdLst_DECLARED */

#ifndef mkpc_stm_cmdLst_SUPP_FUNCS
#define mkpc_stm_cmdLst_SUPP_FUNCS
void mkpc_stm_cmdLst__free( mkpc_stm_cmdLst* val, ysmff ffunc);
void mkpc_stm_cmdLst__copy( mkpc_stm_cmdLst* dest, mkpc_stm_cmdLst* src, 
  ysmaf afunc);
#endif /* mkpc_stm_cmdLst_SUPP_FUNCS */

void mkpc_stm_connect( mkpc_stm or, yoenv* ev, mkpc_stmCB clntHndl, ub4 
  reqBps, ub4* maxBps);
void mkpc_stm_connect_nw( mkpc_stm or, yoenv* ev, mkpc_stmCB clntHndl, ub4 
  reqBps, ub4* maxBps, ysevt* _sem);
yopar* mkpc_stm_connect__getPars(void);
#ifndef mkpc_stm_connect_pars
#define mkpc_stm_connect_pars (mkpc_stm_connect__getPars())
#endif

void mkpc_stm_newCmd( mkpc_stm or, yoenv* ev, mkpc_stm_cmdLst* cmds);
void mkpc_stm_newCmd_nw( mkpc_stm or, yoenv* ev, mkpc_stm_cmdLst* cmds, 
  ysevt* _sem);
yopar* mkpc_stm_newCmd__getPars(void);
#ifndef mkpc_stm_newCmd_pars
#define mkpc_stm_newCmd_pars (mkpc_stm_newCmd__getPars())
#endif

void mkpc_stm_pause( mkpc_stm or, yoenv* ev);
void mkpc_stm_pause_nw( mkpc_stm or, yoenv* ev, ysevt* _sem);
yopar* mkpc_stm_pause__getPars(void);
#ifndef mkpc_stm_pause_pars
#define mkpc_stm_pause_pars (mkpc_stm_pause__getPars())
#endif

void mkpc_stm_unPause( mkpc_stm or, yoenv* ev);
void mkpc_stm_unPause_nw( mkpc_stm or, yoenv* ev, ysevt* _sem);
yopar* mkpc_stm_unPause__getPars(void);
#ifndef mkpc_stm_unPause_pars
#define mkpc_stm_unPause_pars (mkpc_stm_unPause__getPars())
#endif

void mkpc_stm_dumpOn( mkpc_stm or, yoenv* ev, char* fname, ub4 mbytes);
void mkpc_stm_dumpOn_nw( mkpc_stm or, yoenv* ev, char* fname, ub4 mbytes, 
  ysevt* _sem);
yopar* mkpc_stm_dumpOn__getPars(void);
#ifndef mkpc_stm_dumpOn_pars
#define mkpc_stm_dumpOn_pars (mkpc_stm_dumpOn__getPars())
#endif

void mkpc_stm_dumpOff( mkpc_stm or, yoenv* ev);
void mkpc_stm_dumpOff_nw( mkpc_stm or, yoenv* ev, ysevt* _sem);
yopar* mkpc_stm_dumpOff__getPars(void);
#ifndef mkpc_stm_dumpOff_pars
#define mkpc_stm_dumpOff_pars (mkpc_stm_dumpOff__getPars())
#endif

void mkpc_stm_getState( mkpc_stm or, yoenv* ev, ub4* cmdRef, sysb8* segPos);
  
void mkpc_stm_getState_nw( mkpc_stm or, yoenv* ev, ub4* cmdRef, sysb8* 
  segPos, ysevt* _sem);
yopar* mkpc_stm_getState__getPars(void);
#ifndef mkpc_stm_getState_pars
#define mkpc_stm_getState_pars (mkpc_stm_getState__getPars())
#endif

void mkpc_stm_disconnect( mkpc_stm or, yoenv* ev);
void mkpc_stm_disconnect_nw( mkpc_stm or, yoenv* ev, ysevt* _sem);
yopar* mkpc_stm_disconnect__getPars(void);
#ifndef mkpc_stm_disconnect_pars
#define mkpc_stm_disconnect_pars (mkpc_stm_disconnect__getPars())
#endif

#ifndef mkpc_stm__tyimpl_DEFINED
#define mkpc_stm__tyimpl_DEFINED
struct mkpc_stm__tyimpl
{
  void (*connect)( mkpc_stm, yoenv*, mkpc_stmCB, ub4, ub4*);
  void (*newCmd)( mkpc_stm, yoenv*, mkpc_stm_cmdLst*);
  void (*pause)( mkpc_stm, yoenv*);
  void (*unPause)( mkpc_stm, yoenv*);
  void (*dumpOn)( mkpc_stm, yoenv*, char*, ub4);
  void (*dumpOff)( mkpc_stm, yoenv*);
  void (*getState)( mkpc_stm, yoenv*, ub4*, sysb8*);
  void (*disconnect)( mkpc_stm, yoenv*);
};
#endif /* mkpc_stm__tyimpl_DEFINED */


/* interface mkpc_gbl declarations */
#ifndef mkpc_gbl_DECLARED
#define mkpc_gbl_DECLARED
typedef struct YCmkpc_gbl* mkpc_gbl;
yotk* mkpc_gbl__getTC(void);
#ifndef YCTC_mkpc_gbl
#define YCTC_mkpc_gbl   (mkpc_gbl__getTC())
#endif
#endif /* mkpc_gbl_DECLARED */

#ifndef mkpc_gbl_SUPP_FUNCS
#define mkpc_gbl_SUPP_FUNCS
void mkpc_gbl__free( mkpc_gbl* val, ysmff ffunc);
void mkpc_gbl__copy( mkpc_gbl* dest, mkpc_gbl* src, ysmaf afunc);
#endif /* mkpc_gbl_SUPP_FUNCS */

#ifndef mkpc_gbl_DEFINED
#define mkpc_gbl_DEFINED
struct yostub* mkpc_gbl__getStubs(void);
#define mkpc_gbl__stubs (mkpc_gbl__getStubs())
#endif /* mkpc_gbl_DEFINED */

CONST ysid* mkpc_gbl__getId(void);
#ifndef mkpc_gbl__id
#define mkpc_gbl__id   (mkpc_gbl__getId())
#endif
void mkpc_gbl_shutdown( mkpc_gbl or, yoenv* ev);
void mkpc_gbl_shutdown_nw( mkpc_gbl or, yoenv* ev, ysevt* _sem);
yopar* mkpc_gbl_shutdown__getPars(void);
#ifndef mkpc_gbl_shutdown_pars
#define mkpc_gbl_shutdown_pars (mkpc_gbl_shutdown__getPars())
#endif

mkpc_stm mkpc_gbl_getStream( mkpc_gbl or, yoenv* ev, sb2 stmNum);
void mkpc_gbl_getStream_nw( mkpc_gbl or, yoenv* ev, sb2 stmNum, ysevt* _sem)
  ;
yopar* mkpc_gbl_getStream__getPars(void);
#ifndef mkpc_gbl_getStream_pars
#define mkpc_gbl_getStream_pars (mkpc_gbl_getStream__getPars())
#endif

#ifndef mkpc_gbl__tyimpl_DEFINED
#define mkpc_gbl__tyimpl_DEFINED
struct mkpc_gbl__tyimpl
{
  void (*shutdown)( mkpc_gbl, yoenv*);
  mkpc_stm (*getStream)( mkpc_gbl, yoenv*, sb2);
};
#endif /* mkpc_gbl__tyimpl_DEFINED */

EXTC_END
#endif /* MKPC_IDL */
