
/***************************************************************************
*
*  TDASYNC.H
*
*  This module contains private TD defines and structures
*
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author: Kurt Perry (4/07/94)
*          Brad Pedersen (6/4/95)
*
*  $Log$
*  
*     Rev 1.7   15 Apr 1997 16:55:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   15 Mar 1996 17:34:02   KenB
*  move error defines to asyncerr.h
*  
*     Rev 1.5   19 Sep 1995 11:49:30   scottk
*  stop bits problem
*  
*     Rev 1.4   05 Jul 1995 19:31:30   bradp
*  update
*
****************************************************************************/

#include "asyncerr.h"

/*=============================================================================
==   Defines
=============================================================================*/

/* fbCtlHndShake */
#define MODE_DTR_CONTROL        0x01
#define MODE_DTR_HANDSHAKE      0x02
#define MODE_CTS_HANDSHAKE      0x08
#define MODE_DSR_HANDSHAKE      0x10
#define MODE_DCD_HANDSHAKE      0x20
#define MODE_DSR_SENSITIVITY    0x40

/* fbFlowReplace */
#define MODE_AUTO_TRANSMIT      0x01
#define MODE_AUTO_RECEIVE       0x02
#define MODE_ERROR_CHAR         0x04
#define MODE_NULL_STRIPPING     0x08
#define MODE_BREAK_CHAR         0x10
#define MODE_RTS_CONTROL        0x40
#define MODE_RTS_HANDSHAKE      0x80
#define MODE_TRANSMIT_TOGGLE    0xC0

/* fbTimeout */
#define MODE_NO_WRITE_TIMEOUT           0x01
#define MODE_READ_TIMEOUT               0x02
#define MODE_WAIT_READ_TIMEOUT          0x04
#define MODE_NOWAIT_READ_TIMEOUT        0x06

/* AsyncSetModemCtrl */
#define DTR_ON          0x01
#define RTS_ON          0x02
#define MASK_ON         0xff00
#define DTR_OFF         0xfe
#define RTS_OFF         0xfd

/* Stop Bits */
#define DOS_STOP1  0
#define DOS_STOP15 1
#define DOS_STOP2  2


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Async DCB
 */
typedef struct _DCBINFO {
   unsigned int  usWriteTimeout;
   unsigned int  usReadTimeout;
   unsigned char fbCtlHndShake;
   unsigned char fbFlowReplace;
   unsigned char fbTimeout;
   unsigned char bErrorReplacementChar;
   unsigned char bBreakReplacementChar;
   unsigned char bXONChar;
   unsigned char bXOFFChar;
} DCBINFO, * PDCBINFO;

/*
 *  Async Line Control Structure
 */
typedef struct _LINECONTROL {
        unsigned char   bDataBits;
        unsigned char   bParity;
        unsigned char   bStopBits;
        unsigned char   fbTransBreak;
} LINECONTROL, * PLINECONTROL;

/*
 *  ASYNC PD structure
 */
typedef struct _PDASYNC {
    int PortNumber;
    ULONG BaudRate;
    USHORT ComIRQ;
    USHORT ComIOAddr;
    DCBINFO Dcb;
    LINECONTROL LineControl;
    BYTE * pInBuf;                 // pointer to input buffer
} PDASYNC, * PPDASYNC;


/*=============================================================================
==   ASM Functions
=============================================================================*/

int far AsyncOpen( int, unsigned int , unsigned int, int );
int far AsyncClose( int );
int far AsyncRead( int, char far *, int, USHORT * );
int far AsyncSetBaud( int, long );
int far AsyncSetDCB( int, PDCBINFO );
int far AsyncSetLineCtrl( int, PLINECONTROL );
int far AsyncSetModemCtrl( int, int );
int far AsyncWrite( int, char far *, int, PUSHORT );
int far AsyncWriteFlush( int );
int far AsyncGetDCB( int, PDCBINFO );
int AsyncSendBreak( int handle, int ms_time );
