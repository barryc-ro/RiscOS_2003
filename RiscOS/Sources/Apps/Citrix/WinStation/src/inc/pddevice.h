/* > pddevice.h

 * This file contains the extra defines required of all PD's for RISCOS use.

 */

#ifndef __PDDEVICE_H
#define __PDDEVICE_H

/* This structure is to provide access to transport and protocl specific functions */

#define PDDEVICE__OPEN		0
#define PDDEVICE__CLOSE		1
#define PDDEVICE__INFO		2
#define PDDEVICE__CONNECT	3
#define PDDEVICE__DISCONNECT	4
#define PDDEVICE__INIT		5
#define PDDEVICE__ENABLE	6
#define PDDEVICE__DISABLE	7
#define PDDEVICE__PROCESSINPUT	8
#define PDDEVICE__QUERY		9
#define PDDEVICE__POLL		10
#define PDDEVICE__WRITE		11
#define PDDEVICE__CANCELWRITE	12
#define PDDEVICE__CALLBACK	13
#define PDDEVICE__SETINFO	14
#define PDDEVICE__QUERYINFO	15
#define PDDEVICE__OUTBUFALLOC	16
#define PDDEVICE__OUTBUFERROR	17
#define PDDEVICE__OUTBUFFREE	18
#define PDDEVICE__COUNT		19

typedef int FNPDDEVICEOPEN( PPD, PPDOPEN );
typedef FNPDDEVICEOPEN *PFNPDDEVICEOPEN;

typedef int FNPDDEVICECLOSE( PPD, PDLLCLOSE );
typedef FNPDDEVICECLOSE *PFNPDDEVICECLOSE;

typedef int FNPDDEVICEINFO( PPD, PDLLINFO );
typedef FNPDDEVICEINFO *PFNPDDEVICEINFO;

typedef int FNPDDEVICECONNECT( PPD );
typedef FNPDDEVICECONNECT *PFNPDDEVICECONNECT;
typedef int FNPDDEVICEDISCONNECT( PPD );
typedef FNPDDEVICEDISCONNECT *PFNPDDEVICEDISCONNECT;

typedef int FNPDDEVICEINIT( PPD, LPVOID, USHORT );
typedef FNPDDEVICEINIT *PFNPDDEVICEINIT;
    
typedef int FNPDDEVICEENABLE( PPD );
typedef FNPDDEVICEENABLE *PFNPDDEVICEENABLE;
typedef int FNPDDEVICEDISABLE( PPD );
typedef FNPDDEVICEDISABLE *PFNPDDEVICEDISABLE;

typedef int FNPDDEVICEPROCESSINPUT( PPD );
typedef FNPDDEVICEPROCESSINPUT *PFNPDDEVICEPROCESSINPUT;
typedef int FNPDDEVICEQUERY( PPD, PPDQUERYINFORMATION );
typedef FNPDDEVICEQUERY *PFNPDDEVICEQUERY;
typedef int FNPDDEVICEPOLL( PPD, PDLLPOLL );
typedef FNPDDEVICEPOLL *PFNPDDEVICEPOLL;

typedef int FNPDDEVICEWRITE( PPD, PPDWRITE );
typedef FNPDDEVICEWRITE *PFNPDDEVICEWRITE;
typedef int FNPDDEVICECHECKWRITE( PPD, POUTBUF );
typedef FNPDDEVICECHECKWRITE *PFNPDDEVICECHECKWRITE;
typedef int FNPDDEVICECANCELWRITE( PPD );
typedef FNPDDEVICECANCELWRITE *PFNPDDEVICECANCELWRITE;

typedef int FNPDDEVICECALLBACK( PPD );
typedef FNPDDEVICECALLBACK *PFNPDDEVICECALLBACK;

typedef int FNPDDEVICESETINFO( PPD, SETINFOCLASS, LPBYTE, USHORT );
typedef FNPDDEVICESETINFO *PFNPDDEVICESETINFO;
typedef int FNPDDEVICEQUERYINFO( PPD, QUERYINFOCLASS, LPBYTE, USHORT );
typedef FNPDDEVICEQUERYINFO *PFNPDDEVICEQUERYINFO;

typedef int FNPDDEVICEOUTBUFALLOC( PPD, POUTBUF * );
typedef FNPDDEVICEOUTBUFALLOC *PFNPDDEVICEOUTBUFALLOC;
typedef void FNPDDEVICEOUTBUFERROR( PPD, POUTBUF );
typedef FNPDDEVICEOUTBUFERROR *PFNPDDEVICEOUTBUFERROR;
typedef void FNPDDEVICEOUTBUFFREE( PPD, POUTBUF );
typedef FNPDDEVICEOUTBUFFREE *PFNPDDEVICEOUTBUFFREE;

#ifndef NO_PDDEVICE_DEFINES

#define DeviceOpen(a,b)		((PFNPDDEVICEOPEN)((a)->pDeviceProcedures[PDDEVICE__OPEN]))(a,b)
#define DeviceClose(a,b)	((PFNPDDEVICECLOSE)((a)->pDeviceProcedures[PDDEVICE__CLOSE]))(a,b)
#define DeviceInfo(a,b)		((PFNPDDEVICEINFO)((a)->pDeviceProcedures[PDDEVICE__INFO]))(a,b)
#define DeviceConnect(a)	((PFNPDDEVICECONNECT)((a)->pDeviceProcedures[PDDEVICE__CONNECT]))(a)
#define DeviceDisconnect(a)	((PFNPDDEVICEDISCONNECT)((a)->pDeviceProcedures[PDDEVICE__DISCONNECT]))(a)
#define DeviceInit(a,b,c)	((PFNPDDEVICEINIT)((a)->pDeviceProcedures[PDDEVICE__INIT]))(a,b,c)
#define DeviceEnable(a)		((PFNPDDEVICEENABLE)((a)->pDeviceProcedures[PDDEVICE__ENABLE]))(a)
#define DeviceDisable(a)	((PFNPDDEVICEDISABLE)((a)->pDeviceProcedures[PDDEVICE__DISABLE]))(a)

#define DeviceProcessInput(a)	((PFNPDDEVICEPROCESSINPUT)((a)->pDeviceProcedures[PDDEVICE__PROCESSINPUT]))(a)
#define DeviceQuery(a,b)	((PFNPDDEVICEQUERY)((a)->pDeviceProcedures[PDDEVICE__QUERY]))(a,b)
#define DevicePoll(a,b)		((PFNPDDEVICEPOLL)((a)->pDeviceProcedures[PDDEVICE__POLL]))(a,b)

#define DeviceWrite(a,b)	((PFNPDDEVICEWRITE)((a)->pDeviceProcedures[PDDEVICE__WRITE]))(a,b)
#define DeviceCancelWrite(a)	((PFNPDDEVICECANCELWRITE)((a)->pDeviceProcedures[PDDEVICE__CANCELWRITE]))(a)

#define DeviceCallback(a)	((PFNPDDEVICECALLBACK)((a)->pDeviceProcedures[PDDEVICE__CALLBACK]))(a)

#define DeviceSetInfo(a,b,c,d)	((PFNPDDEVICESETINFO)((a)->pDeviceProcedures[PDDEVICE__SETINFO]))(a,b,c,d)
#define DeviceQueryInfo(a,b,c,d) ((PFNPDDEVICEQUERYINFO)((a)->pDeviceProcedures[PDDEVICE__QUERYINFO]))(a,b,c,d)

#define DeviceOutBufAlloc(a,b)	((PFNPDDEVICEOUTBUFALLOC)((a)->pDeviceProcedures[PDDEVICE__OUTBUFALLOC]))(a,b)
#define DeviceOutBufError(a,b)	((PFNPDDEVICEOUTBUFERROR)((a)->pDeviceProcedures[PDDEVICE__OUTBUFERROR]))(a,b)
#define DeviceOutBufFree(a,b)	((PFNPDDEVICEOUTBUFFREE)((a)->pDeviceProcedures[PDDEVICE__OUTBUFFREE]))(a,b)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
