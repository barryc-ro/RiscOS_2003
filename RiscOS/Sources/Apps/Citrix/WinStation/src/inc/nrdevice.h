/* > nrdevice.h

 * This file contains the extra defines required of all NR's for RISCOS use.

 */

#ifndef __NRDEVICE_H
#define __NRDEVICE_H

/*---------------------------------------------------------------------------------------------------- */

typedef int FNNRDEVICENAMETOADDRESS( PNR, PNAMEADDRESS );
typedef FNNRDEVICENAMETOADDRESS *PFNNRDEVICENAMETOADDRESS;

#define NRDEVICE__COUNT 1

#ifndef NO_NRDEVICE_DEFINES

#define DeviceNameToAddress(a,b)	((PFNNRDEVICENAMETOADDRESS)((a)->pDeviceProcedures[0]))(a,b)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
