/* > vddevice.h

 * This file contains the extra defines required of all VD's for RISCOS use.

 */

#ifndef __VDDEVICE_H
#define __VDDEVICE_H

/* This structure is to provide access to transport and protocol specific functions */

#define VDDEVICE__OPEN	0
#define VDDEVICE__CLOSE	1
#define VDDEVICE__INFO	2
#define VDDEVICE__POLL	3
#define VDDEVICE__SETINFORMATION	4
#define VDDEVICE__QUERYINFORMATION	5
#define VDDEVICE__GETLASTERROR		6
#define VDDEVICE__COUNT	7

typedef int FNVDDEVICEOPEN( PVD, PVDOPEN );
typedef FNVDDEVICEOPEN *PFNVDDEVICEOPEN;
typedef int FNVDDEVICECLOSE( PVD, PDLLCLOSE );
typedef FNVDDEVICECLOSE *PFNVDDEVICECLOSE;

typedef int FNVDDEVICEINFO( PVD, PDLLINFO );
typedef FNVDDEVICEINFO *PFNVDDEVICEINFO;

typedef int FNVDDEVICEPOLL( PVD, PDLLPOLL );
typedef FNVDDEVICEPOLL *PFNVDDEVICEPOLL;

typedef int FNVDDEVICESETINFORMATION( PVD, PVDSETINFORMATION );
typedef FNVDDEVICESETINFORMATION *PFNVDDEVICESETINFORMATION;
typedef int FNVDDEVICEQUERYINFORMATION( PVD, PVDQUERYINFORMATION );
typedef FNVDDEVICEQUERYINFORMATION *PFNVDDEVICEQUERYINFORMATION;

typedef int FNVDDEVICEGETLASTERROR( PVD, PVDLASTERROR );
typedef FNVDDEVICEGETLASTERROR *PFNVDDEVICEGETLASTERROR;


#ifndef NO_VDDEVICE_DEFINES

#define DeviceOpen(a,b)		((PFNVDDEVICEOPEN)((a)->pDeviceProcedures[VDDEVICE__OPEN]))(a,b)
#define DeviceClose(a,b)	((PFNVDDEVICECLOSE)((a)->pDeviceProcedures[VDDEVICE__CLOSE]))(a,b)
#define DeviceInfo(a,b)		((PFNVDDEVICEINFO)((a)->pDeviceProcedures[VDDEVICE__INFO]))(a,b)

#define DevicePoll(a,b)		((PFNVDDEVICEPOLL)((a)->pDeviceProcedures[VDDEVICE__POLL]))(a,b)

#define DeviceSetInformation(a,b)	((PFNVDDEVICESETINFORMATION)((a)->pDeviceProcedures[VDDEVICE__SETINFORMATION]))(a,b)
#define DeviceQueryInformation(a,b)	((PFNVDDEVICEQUERYINFORMATION)((a)->pDeviceProcedures[VDDEVICE__QUERYINFORMATION]))(a,b)
#define DeviceGetLastError(a,b)		((PFNVDDEVICEGETLASTERROR)((a)->pDeviceProcedures[VDDEVICE__GETLASTERROR]))(a,b)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
