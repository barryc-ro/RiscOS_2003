
/***************************************************************************
*
*  PDFRAME.H
*
*  This module contains private PD defines and structures
*
*  Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.3   15 Apr 1997 16:52:18   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   19 Jan 1996 11:11:14   bradp
*  update
*  
*     Rev 1.1   07 Apr 1995 15:28:50   richa
*  Win client.
*
*     Rev 1.0   10 Aug 1994 15:15:10   bradp
*  Initial revision.
*
*     Rev 1.0   04 May 1994 11:24:20   bradp
*  Initial revision.
*
*
*************************************************************************/

/*=============================================================================
==   Defines
=============================================================================*/

#define FRAME_ESC       0x7d
#define FRAME_FLAG      0x7e


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Frame PD structure
 */
typedef struct _PDFRAME {

    LPBYTE pInBuf;             // pointer to input buffer
    POUTBUF pCurrentOutBuf;    // pointer to current output buffer
    USHORT ReceiveState;       // current receive state
    USHORT InputCount;         // number of input bytes received

    BUSHORT f8BitData : 1;      // 8 bit data path detected

} PDFRAME, * PPDFRAME;

/* SJM: rename local functions to prevent clashes */

#define DeviceOutBufAlloc	PdFrameDeviceOutBufAlloc
#define DeviceOutBufError	PdFrameDeviceOutBufError
#define DeviceOutBufFree	PdFrameDeviceOutBufFree
#define DeviceSetInfo		PdFrameDeviceSetInfo
#define DeviceQueryInfo		PdFrameDeviceQueryInfo

#define DeviceProcessInput	PdFrameDeviceProcessInput

