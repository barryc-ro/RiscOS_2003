/* > tddevice.h

 * This file contains the extra defines required of all TD's for RISCOS use.

 */

#ifndef __TDDEVICE_H
#define __TDDEVICE_H

/* This structure is to provide access to transport and protocl specific functions */

#define TDDEVICE__OPEN		0
#define TDDEVICE__CLOSE		1
#define TDDEVICE__INFO		2
#define TDDEVICE__CONNECT	3
#define TDDEVICE__DISCONNECT	4
#define TDDEVICE__PROCESSINPUT	5
#define TDDEVICE__WRITE		6
#define TDDEVICE__CHECKWRITE	7
#define TDDEVICE__CANCELWRITE	8
#define TDDEVICE__SENDBREAK	9
#define TDDEVICE__COUNT		10

typedef int FNTDDEVICEOPEN( PPD, PPDOPEN );
typedef FNTDDEVICEOPEN *PFNTDDEVICEOPEN;

typedef int FNTDDEVICECLOSE( PPD, PDLLCLOSE );
typedef FNTDDEVICECLOSE *PFNTDDEVICECLOSE;

typedef int FNTDDEVICEINFO( PPD, PDLLINFO );
typedef FNTDDEVICEINFO *PFNTDDEVICEINFO;

typedef int FNTDDEVICECONNECT( PPD );
typedef FNTDDEVICECONNECT *PFNTDDEVICECONNECT;
typedef int FNTDDEVICEDISCONNECT( PPD );
typedef FNTDDEVICEDISCONNECT *PFNTDDEVICEDISCONNECT;

typedef int FNTDDEVICEPROCESSINPUT( PPD );
typedef FNTDDEVICEPROCESSINPUT *PFNTDDEVICEPROCESSINPUT;

typedef int FNTDDEVICEWRITE( PPD, POUTBUF, PUSHORT );
typedef FNTDDEVICEWRITE *PFNTDDEVICEWRITE;
typedef int FNTDDEVICECHECKWRITE( PPD, POUTBUF );
typedef FNTDDEVICECHECKWRITE *PFNTDDEVICECHECKWRITE;
typedef int FNTDDEVICECANCELWRITE( PPD, POUTBUF );
typedef FNTDDEVICECANCELWRITE *PFNTDDEVICECANCELWRITE;

typedef int FNTDDEVICESENDBREAK( PPD );
typedef FNTDDEVICESENDBREAK *PFNTDDEVICESENDBREAK;

#ifndef NO_TDDEVICE_DEFINES

#define DeviceOpen(a,b)		((PFNTDDEVICEOPEN)((a)->pDeviceProcedures[TDDEVICE__OPEN]))(a,b)
#define DeviceClose(a,b)	((PFNTDDEVICECLOSE)((a)->pDeviceProcedures[TDDEVICE__CLOSE]))(a,b)
#define DeviceInfo(a,b)		((PFNTDDEVICEINFO)((a)->pDeviceProcedures[TDDEVICE__INFO]))(a,b)
#define DeviceConnect(a)	((PFNTDDEVICECONNECT)((a)->pDeviceProcedures[TDDEVICE__CONNECT]))(a)
#define DeviceDisconnect(a)	((PFNTDDEVICEDISCONNECT)((a)->pDeviceProcedures[TDDEVICE__DISCONNECT]))(a)

#define DeviceProcessInput(a)	((PFNTDDEVICEPROCESSINPUT)((a)->pDeviceProcedures[TDDEVICE__PROCESSINPUT]))(a)

#define DeviceWrite(a,b,c)	((PFNTDDEVICEWRITE)((a)->pDeviceProcedures[TDDEVICE__WRITE]))(a,b,c)
#define DeviceCheckWrite(a,b)	((PFNTDDEVICECHECKWRITE)((a)->pDeviceProcedures[TDDEVICE__CHECKWRITE]))(a,b)
#define DeviceCancelWrite(a,b)	((PFNTDDEVICECANCELWRITE)((a)->pDeviceProcedures[TDDEVICE__CANCELWRITE]))(a,b)

#define DeviceSendBreak(a)	((PFNTDDEVICESENDBREAK)((a)->pDeviceProcedures[TDDEVICE__SENDBREAK]))(a)

#endif

/*---------------------------------------------------------------------------------------------------- */

#endif
