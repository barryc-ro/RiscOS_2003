/* GENERATED FILE
 * mkpi - server skeleton header
 * from /vobs/rts/src/vid/mkpi.idl
 */

#ifndef MKPII_H
#define MKPII_H

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MKPI_IDL
#include <mkpi.h>
#endif

EXTC_START

void mkpi_stmCB_cmdStatus_i( mkpi_stmCB or, yoenv* ev, ub4 cmdRef, 
  mkpc_stmCB_status state);
void mkpi_stm_connect_i( mkpi_stm or, yoenv* ev, mkpc_stmCB clntHndl, ub4 
  reqBps, ub4* maxBps);
void mkpi_stm_newCmd_i( mkpi_stm or, yoenv* ev, mkpc_stm_cmdLst* cmds);
void mkpi_stm_pause_i( mkpi_stm or, yoenv* ev);
void mkpi_stm_unPause_i( mkpi_stm or, yoenv* ev);
void mkpi_stm_dumpOn_i( mkpi_stm or, yoenv* ev, char* fname, ub4 mbytes);
void mkpi_stm_dumpOff_i( mkpi_stm or, yoenv* ev);
void mkpi_stm_getState_i( mkpi_stm or, yoenv* ev, ub4* cmdRef, sysb8* 
  segPos);
void mkpi_stm_disconnect_i( mkpi_stm or, yoenv* ev);
mkpmib_snmpStm_data mkpi_stm_getMib_i( mkpi_stm or, yoenv* ev);
mzc_chnlInfo mkpi_stm_GetInfo_i( mkpi_stm or, yoenv* ev);
mzc_chnlInfo mkpi_stm_Rebuild_i( mkpi_stm or, yoenv* ev, mzc_chnlreqx* req);
  
void mkpi_stm_Enable_i( mkpi_stm or, yoenv* ev);
void mkpi_stm_Disable_i( mkpi_stm or, yoenv* ev);
void mkpi_stm_TearDown_i( mkpi_stm or, yoenv* ev);
void mkpi_gbl_shutdown_i( mkpi_gbl or, yoenv* ev);
mkpc_stm mkpi_gbl_getStream_i( mkpi_gbl or, yoenv* ev, sb2 stmNum);
mkpmib_snmpGbl_data mkpi_gbl_getMib_i( mkpi_gbl or, yoenv* ev);
mzc_chnlPrvInfo mkpi_gbl_GetInfo_i( mkpi_gbl or, yoenv* ev);
mzc_channel mkpi_gbl_Build_i( mkpi_gbl or, yoenv* ev, mzc_chnlreqx* req);

EXTC_END
#endif /* MKPII_H */
