/* GENERATED FILE
 * mkpmib - server skeleton header
 * from /vobs/rts/inc/mkpmib.idl
 */

#ifndef MKPMIBI_H
#define MKPMIBI_H

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MKPMIB_IDL
#include <mkpmib.h>
#endif

EXTC_START

mkpmib_snmpStm_data mkpmib_snmpStm_getMib_i( mkpmib_snmpStm or, yoenv* ev);
mkpmib_snmpGbl_data mkpmib_snmpGbl_getMib_i( mkpmib_snmpGbl or, yoenv* ev);

EXTC_END
#endif /* MKPMIBI_H */
