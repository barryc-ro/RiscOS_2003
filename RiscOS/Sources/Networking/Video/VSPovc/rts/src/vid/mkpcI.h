/* GENERATED FILE
 * mkpc - server skeleton header
 * from mkpc.idl
 */

#ifndef MKPCI_H
#define MKPCI_H

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MKPC_IDL
#include <mkpc.h>
#endif

EXTC_START

void mkpc_stmCB_cmdStatus_i( mkpc_stmCB or, yoenv* ev, ub4 cmdRef, 
  mkpc_stmCB_status state);
void mkpc_stm_connect_i( mkpc_stm or, yoenv* ev, mkpc_stmCB clntHndl, ub4 
  reqBps, ub4* maxBps);
void mkpc_stm_newCmd_i( mkpc_stm or, yoenv* ev, mkpc_stm_cmdLst* cmds);
void mkpc_stm_pause_i( mkpc_stm or, yoenv* ev);
void mkpc_stm_unPause_i( mkpc_stm or, yoenv* ev);
void mkpc_stm_dumpOn_i( mkpc_stm or, yoenv* ev, char* fname, ub4 mbytes);
void mkpc_stm_dumpOff_i( mkpc_stm or, yoenv* ev);
void mkpc_stm_getState_i( mkpc_stm or, yoenv* ev, ub4* cmdRef, sysb8* 
  segPos);
void mkpc_stm_disconnect_i( mkpc_stm or, yoenv* ev);
void mkpc_gbl_shutdown_i( mkpc_gbl or, yoenv* ev);
mkpc_stm mkpc_gbl_getStream_i( mkpc_gbl or, yoenv* ev, sb2 stmNum);

EXTC_END
#endif /* MKPCI_H */
