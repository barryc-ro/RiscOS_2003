
/***************************************************************************
*
*   PDRFRAME.H
*
*   This module contains private PD defines and structures
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Kurt Perry (10/4/94)
*
*   Log: See VLOG
*
*************************************************************************/

#ifndef __PDRFRAME_H__
#define __PDRFRAME_H__

/*=============================================================================
==   Defines
=============================================================================*/


/*=============================================================================
==   Enumerations
=============================================================================*/

/*
 *  States for Reliable Frame state table
 */
typedef enum _RFSTATES {
    sLEN1L,     // 0  packet length (low byte)
    sLEN1H,  	// 1  packet length (high byte)
    sDATA,  	// 2  packet data
    sNUMSTATES
} RFSTATES;


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Reliable Frame PD structure
 */
typedef struct _PDRFRAME {

    LPBYTE    pInBuf;           // pointer to input buffer
    USHORT   InputBufferLength; // length of input buffer
    RFSTATES ReceiveState;      // current receive state
    USHORT   InputCount;        // number of input bytes received so far
    USHORT   PacketLength;      // total length of frame being received
    USHORT   Disable;           // Disable flag

} PDRFRAME, * PPDRFRAME;

/* SJM: rename local functions to prevent clashes */

#define DeviceOutBufAlloc	PdRFrameDeviceOutBufAlloc
#define DeviceOutBufError	PdRFrameDeviceOutBufError
#define DeviceOutBufFree	PdRFrameDeviceOutBufFree
#define DeviceSetInfo		PdRFrameDeviceSetInfo
#define DeviceQueryInfo		PdRFrameDeviceQueryInfo

#define DeviceProcessInput	PdRFrameDeviceProcessInput

#endif //__PDRFRAME_H__
