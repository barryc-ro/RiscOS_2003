/* > vddriver.h

 * This file contains the extra defines required of all VD's for RISCOS use.

 */

#ifndef __VDDRIVER_H
#define __VDDRIVER_H

/* This structure is to provide access to transport and protocol specific functions */

#define VDDRIVER__OPEN	0
#define VDDRIVER__CLOSE	1
#define VDDRIVER__INFO	2
#define VDDRIVER__POLL	3
#define VDDRIVER__SETINFORMATION	4
#define VDDRIVER__QUERYINFORMATION	5
#define VDDRIVER__GETLASTERROR		6
#define VDDRIVER__COUNT	7

typedef int FNVDDRIVEROPEN( PVD, PVDOPEN );
typedef FNVDDRIVEROPEN *PFNVDDRIVEROPEN;
typedef int FNVDDRIVERCLOSE( PVD, PDLLCLOSE );
typedef FNVDDRIVERCLOSE *PFNVDDRIVERCLOSE;

typedef int FNVDDRIVERINFO( PVD, PDLLINFO );
typedef FNVDDRIVERINFO *PFNVDDRIVERINFO;

typedef int FNVDDRIVERPOLL( PVD, PDLLPOLL );
typedef FNVDDRIVERPOLL *PFNVDDRIVERPOLL;

typedef int FNVDDRIVERSETINFORMATION( PVD, PVDSETINFORMATION );
typedef FNVDDRIVERSETINFORMATION *PFNVDDRIVERSETINFORMATION;
typedef int FNVDDRIVERQUERYINFORMATION( PVD, PVDQUERYINFORMATION );
typedef FNVDDRIVERQUERYINFORMATION *PFNVDDRIVERQUERYINFORMATION;

typedef int FNVDDRIVERGETLASTERROR( PVD, PVDLASTERROR );
typedef FNVDDRIVERGETLASTERROR *PFNVDDRIVERGETLASTERROR;


#ifndef NO_VDDRIVER_DEFINES

#define DriverOpen(a,b)		((PFNVDDRIVEROPEN)((a)->pDriverProcedures[VDDRIVER__OPEN]))(a,b)
#define DriverClose(a,b)	((PFNVDDRIVERCLOSE)((a)->pDriverProcedures[VDDRIVER__CLOSE]))(a,b)
#define DriverInfo(a,b)		((PFNVDDRIVERINFO)((a)->pDriverProcedures[VDDRIVER__INFO]))(a,b)

#define DriverPoll(a,b)		((PFNVDDRIVERPOLL)((a)->pDriverProcedures[VDDRIVER__POLL]))(a,b)

#define DriverSetInformation(a,b)	((PFNVDDRIVERSETINFORMATION)((a)->pDriverProcedures[VDDRIVER__SETINFORMATION]))(a,b)
#define DriverQueryInformation(a,b)	((PFNVDDRIVERQUERYINFORMATION)((a)->pDriverProcedures[VDDRIVER__QUERYINFORMATION]))(a,b)
#define DriverGetLastError(a,b)		((PFNVDDRIVERGETLASTERROR)((a)->pDriverProcedures[VDDRIVER__GETLASTERROR]))(a,b)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
