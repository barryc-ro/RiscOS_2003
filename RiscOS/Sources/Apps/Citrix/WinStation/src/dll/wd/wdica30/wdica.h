
/***************************************************************************
*
* wdica.h
*
* ICA 3.0 WinStation Driver
*
* This module contains private WD defines and structures
*
* Copyright 1994, Citrix Systems Inc.
*
* Author: Brad Pedersen (4/8/94)
*
*  $Log$
*  
*     Rev 1.39   11 Jun 1997 10:17:24   terryt
*  client double click support
*  
*     Rev 1.38   30 Apr 1997 14:38:12   terryt
*  shift states again
*  
*     Rev 1.37   15 Apr 1997 18:18:06   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.36   04 Mar 1997 17:43:56   terryt
*  client shift states
*  
*     Rev 1.35   13 Jan 1997 16:39:38   kurtp
*  Persistent Cache
*  
*     Rev 1.34   28 Aug 1996 15:09:26   marcb
*  update
*  
*     Rev 1.33   08 May 1996 15:16:58   jeffm
*  update
*  
*     Rev 1.32   27 Apr 1996 15:56:24   andys
*  soft keyboard
*  
*     Rev 1.31   29 Jan 1996 18:03:10   bradp
*  update
*  
*     Rev 1.30   19 Jan 1996 16:13:50   kurtp
*  update
*  
*     Rev 1.29   19 Jan 1996 15:46:10   kurtp
*  update
*  
*     Rev 1.28   07 Aug 1995 20:43:58   bradp
*  update
*  
*
****************************************************************************/

#ifndef __WDICA_H__
#define __WDICA_H__

/*
 *  Persistent cache queue structure
 */
typedef struct _WDCachePac {
    PVOID  pCache;
    USHORT cbCache;
    struct _WDCachePac * Next;
} WDCachePac, * PWDCachePac;


/*
 *  ICA WD structure
 */
typedef struct _WDICA {

    USHORT fMouse /*: 1*/;            // mouse available flag
    USHORT fGraphics/*: 1*/;          // graphics is enabled (thinwire has focus)
    USHORT fSendStopRequest/*: 1*/;   // request to send PACKET_STOP_REQUEST
    USHORT fSendRedisplay/*: 1*/;     // request to send PACKET_REDISPLAY
    USHORT fSendTerminate/*: 1*/;     // request to send PACKET_TERMINATE
    USHORT fSendTerminateAck/*: 1*/;  // request to send PACKET_TERMINATE (ACK)
    USHORT fReceivedTerminate/*: 1*/; // received PACKET_TERMINATE from host
    USHORT fIcaDetected/*: 1*/;       // ica has been detected
    USHORT fMouseVisible/*: 1*/;      // mouse pointer is presently visible
    USHORT fSendRedraw/*: 1*/;        // request to send PACKET_REDRAW
    USHORT fTerminate/*: 1*/;         // terminate host connection
    USHORT fVioInit/*:1*/;            // vio/tty window initialized (win only)
    USHORT fEchoTTY/*:1*/;            // echo tty data during tty connect phase
    USHORT fSendRaiseSoftkey/*: 1*/;  // send PACKET_SOFT_KEYBOARD
    USHORT fSendLowerSoftkey/*: 1*/;  // send PACKET_SOFT_KEYBOARD 
    USHORT fHostNotSecured/*: 1*/;    // host has not been secured
    USHORT fSendCacheCommand/*: 1*/;  // send ICA_COMMAND_CACHE
    USHORT fKbdShiftStateSet/*: 1*/;  // Shift state is set (win only)
    USHORT fDoubleClickDetect/*: 1*/; // Double click detect on client

    PDLLLINK pModuleList;         // init - pointer to array of loaded modules
    USHORT ModuleCount;           // init - number of loaded modules
    ULONG SerialNumber;           // init - client computer serial number

    USHORT PacketState;           // input - current input state
    USHORT PacketType;            // input - packet type
    USHORT PacketLength;          // input - packet length in bytes

    USHORT TextIndex;             // text row/col index G_TextModes
    BYTE GlobalAttr;              // global attribute
    KBDCLASS KbdMode;             // current kbd mode (ascii or scan)

    USHORT IcaDetectState;        // ica detect state
    USHORT IcaDetectOffset;       // offset within ica detect string
    ULONG TimerIcaDetect;         // timer - ica detect timer

    USHORT ScrollTopRow;
    USHORT ScrollLeftCol;
    USHORT ScrollBotRow;
    USHORT ScrollRightCol;
    BYTE ScrollCell[2];
    USHORT ScrollLines;

    HVIO   hVio;                  // text mode window handle (win16 & win32)

#define KBD_BUFFER_START_SIZE   64
    PCHAR  pKbdBuffer;            // locally buffered keystrokes
    USHORT cbKbdBuffer;           // count of keys in 
    USHORT cbKbdBufferSize;       // size of kbd buffer
    USHORT ShiftState;

#define MOU_BUFFER_START_SIZE   64
    PCHAR  pMouBuffer;            // locally buffered keystrokes
    USHORT cbMouBuffer;           // count of keys in 
    USHORT cbMouBufferSize;       // size of kbd buffer

    PDLLLINK * pVdLink;           // pointer to array of virtual drivers
    WDVDWRITEHOOK VdWriteHook[ Virtual_Maximum ];

    PWDREDRAW pRedraw;            // Redraw rectangles
    USHORT    cbRedraw;           // size of pRedraw

    PWDCachePac pCacheHeader;     // Cache Command
    USHORT      CachePacCount;    // size of Cache Command

    ULONG   ulTTYDelay;           // time to wait (ms) after ICA detected

} WDICA, * PWDICA;

#endif // __WDICA_H__
