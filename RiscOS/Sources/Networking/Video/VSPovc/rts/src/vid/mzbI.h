/* GENERATED FILE
 * mzb - server skeleton header
 * from /vobs/rts/pub/mzb.idl
 */

#ifndef MZBI_H
#define MZBI_H

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MZB_IDL
#include <mzb.h>
#endif

EXTC_START

mzb_image_err mzb_image_setPhys_i( mzb_image or, yoenv* ev, 
  mzb_image_mnnpa* phys, char* image, ub2 howmany);
mzb_image_err mzb_image_get_i( mzb_image or, yoenv* ev, mzb_image_mnnpa* 
  phys, char** image);
mzb_image_err mzb_image_updateDB_i( mzb_image or, yoenv* ev, char* fname, 
  boolean flush);
mzb_image_err mzb_image_dumpMappings_i( mzb_image or, yoenv* ev);

EXTC_END
#endif /* MZBI_H */
