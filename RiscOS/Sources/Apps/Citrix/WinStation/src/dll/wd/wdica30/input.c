
/*************************************************************************
*
*   input.c
*  
*   ICA 3.0 WinStation Driver - input state machine
*  
*   Copyright 1994, Citrix Systems Inc.
*  
*   Author: Brad Pedersen (4/8/94)
*  
*   input.c,v
*   Revision 1.1  1998/01/12 11:36:19  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.43   15 Apr 1997 18:17:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.42   04 Mar 1997 17:43:58   terryt
*  client shift states
*  
*     Rev 1.41   22 Jan 1997 16:46:52   terryt
*  client data
*  
*     Rev 1.40   22 May 1996 16:23:50   jeffm
*  update
*  
*     Rev 1.39   30 Jan 1996 18:10:56   bradp
*  update
*  
*     Rev 1.38   30 Jan 1996 11:04:52   bradp
*  update
*  
*     Rev 1.37   29 Jan 1996 18:03:06   bradp
*  update
*  
*    
*************************************************************************/

/*
 *  Includes
 */
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"
#include "citrix/ica30.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/mouapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   Defines and structures
=============================================================================*/

/*
 *  Defines for ica detection
 */
#define DETECT_LOOKING  0
#define DETECT_INSTRING 1

/*
 *  States for ICA state table
 *  -- used to transition states in ica state table
 */
typedef enum _ICASTATES {
    sTYPE,      // 0  packet type
    sLEN1,      // 1  packet length (one byte length)
    sLEN2L,     // 2  packet length (low byte)
    sLEN2H,  	// 3  packet length (high byte)
    sDATA,  	// 4  packet data
    sNUMSTATES
} ICASTATES;

/*
 *  Data types for ICA state table
 *  -- used to save incoming data in correct data structure
 */
typedef enum _ICATYPES {
    dTYPE, 	// 1 packet type
    dLEN1,	// 2 packet length (one byte length)
    dLEN2L,	// 3 packet length (low byte)
    dLEN2H,	// 4 packet length (high byte)
    dDATA,	// 5 packet data
    dNUMTYPES
} ICATYPES;

/*
 *  ICA state table structure
 *  -- used in state table array
 */
typedef struct _ICASTATE {
    ICASTATES NextState;            // next state
    ICATYPES DataType;              // data type to save
} ICASTATE, * PICASTATE;


typedef void (*PICAPROCEDURE)( PWD, LPBYTE, USHORT );

/*
 *  ICA length and procedure structure
 *  -- used in ica packet processes
 */
typedef struct _ICADATA {
    signed char Length;             // length of ica packet
    PICAPROCEDURE pProcedure;       // pointer to procedure to process packet
} ICADATA, * PICADATA;


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI WdICA30EmulProcessInput( PWD, LPBYTE, USHORT );
void _CheckForIcaDetect( PWD, LPBYTE, USHORT );
void WriteTTY( LPVOID, LPBYTE, USHORT );
void WdICA30EmulWriteTTY( PWD, LPBYTE, USHORT );


/*=============================================================================
==   Functions referenced
=============================================================================*/

int PdCall( PWD, USHORT, PVOID );
int WdKbdSetMode( PWDICA, KBDCLASS );

void IcaInitRequest( PWD, LPBYTE, USHORT );
void IcaInitConnect( PWD, LPBYTE, USHORT );
void IcaTerminate( PWD, LPBYTE, USHORT );
void IcaStopOk( PWD, LPBYTE, USHORT );
void IcaClearScreen( PWD, LPBYTE, USHORT );
void IcaClearEol( PWD, LPBYTE, USHORT );
void IcaSetGlobalAttr( PWD, LPBYTE, USHORT );
void IcaRawWrite( PWD, LPBYTE, USHORT );
void IcaWrtCharStrAttr( PWD, LPBYTE, USHORT );
void IcaWrtNCell1( PWD, LPBYTE, USHORT );
void IcaWrtNCell2( PWD, LPBYTE, USHORT );
void IcaBeep( PWD, LPBYTE, USHORT );
void IcaSetMousePosition( PWD, LPBYTE, USHORT );
void IcaSetMouseAttribute( PWD, LPBYTE, USHORT );
void IcaSetCursorPosition( PWD, LPBYTE, USHORT );
void IcaSetCursorRow( PWD, LPBYTE, USHORT );
void IcaSetCursorColumn( PWD, LPBYTE, USHORT );
void IcaSetCursorSize( PWD, LPBYTE, USHORT );
void IcaScrollScreen( PWD, LPBYTE, USHORT );
void IcaScrollUp( PWD, LPBYTE, USHORT );
void IcaScrollDown( PWD, LPBYTE, USHORT );
void IcaScrollLeft( PWD, LPBYTE, USHORT );
void IcaScrollRight( PWD, LPBYTE, USHORT );
void IcaScrollUp1( PWD, LPBYTE, USHORT );
void IcaScrollDown1( PWD, LPBYTE, USHORT );
void IcaScrollLeft1( PWD, LPBYTE, USHORT );
void IcaScrollRight1( PWD, LPBYTE, USHORT );
void IcaScrollUpN( PWD, LPBYTE, USHORT );
void IcaScrollDownN( PWD, LPBYTE, USHORT );
void IcaScrollLeftN( PWD, LPBYTE, USHORT );
void IcaScrollRightN( PWD, LPBYTE, USHORT );
void IcaVirtualWrite( PWD, LPBYTE, USHORT );
void IcaVirtualAck( PWD, LPBYTE, USHORT );
void IcaVirtualFlush( PWD, LPBYTE, USHORT );
void IcaCallback( PWD, LPBYTE, USHORT );
void IcaSetGraphics( PWD, LPBYTE, USHORT );
void IcaSetText( PWD, LPBYTE, USHORT );
void IcaSetVideoMode( PWD, LPBYTE, USHORT );
void IcaSetLed( PWD, LPBYTE, USHORT );
void IcaClientData( PWD, LPBYTE, USHORT );


/*=============================================================================
==   Local data
=============================================================================*/

/*
 *  ICA state table array
 */
ICASTATE StateTable[ sNUMSTATES ] = {

    /* 0 - sTYPE  */    sTYPE,   dTYPE,
    /* 1 - sLEN1  */    sDATA,   dLEN1,
    /* 2 - sLEN2L */    sLEN2H,  dLEN2L,
    /* 3 - sLEN2H */    sDATA,   dLEN2H,
    /* 4 - sDATA  */    sDATA,   dDATA,
};


/*
 *  This array is used by the ICA state table to get the length and
 *  the processing procedure for each ICA packet.
 *
 *  NOTE: make sure this array matches the defines in ica.h
 */
static ICADATA ICAData[] =
{
   -2, IcaInitRequest,                     // PACKET_INIT_REQUEST     0x00
   -2, NULL,                               // PACKET_INIT_RESPONSE    0x01
   -2, IcaInitConnect,                     // PACKET_INIT_CONNECT     0x02
   -1, IcaCallback,                        // PACKET_CALLBACK         0x03
   -2, NULL,                               // PACKET_INIT_CONNECT_RESPONSE 0x04

   -1, IcaTerminate,                       // PACKET_TERMINATE        0x05
   -2, NULL,                               // PACKET_REDRAW           0x06

    0, NULL,                               // PACKET_STOP_REQUEST     0x07
    0, IcaStopOk,                          // PACKET_STOP_OK          0x08
    0, NULL,                               // PACKET_REDISPLAY        0x09

    1, NULL,                               // PACKET_KEYBOARD0        0x0A
   -1, NULL,                               // PACKET_KEYBOARD1        0x0B
   -2, NULL,                               // PACKET_KEYBOARD2        0x0C

    5, NULL,                               // PACKET_MOUSE0           0x0D
   -1, NULL,                               // PACKET_MOUSE1           0x0E
   -2, NULL,                               // PACKET_MOUSE2           0x0F

    1, IcaClearScreen,                     // PACKET_CLEAR_SCREEN     0x10
    0, NULL,                               // XON   11                0x11
    3, IcaClearEol,                        // PACKET_CLEAR_EOL        0x12
    0, NULL,                               // XOFF  13                0x13
    1, IcaRawWrite,                        // PACKET_RAW_WRITE0       0x14
   -1, IcaRawWrite,                        // PACKET_RAW_WRITE1       0x15
   -2, IcaRawWrite,                        // PACKET_RAW_WRITE2       0x16
   -1, IcaWrtCharStrAttr,                  // PACKET_WRTCHARSTRATTR1  0x17
   -2, IcaWrtCharStrAttr,                  // PACKET_WRTCHARSTRATTR2  0x18
    5, IcaWrtNCell1,                       // PACKET_WRTNCELL1        0x19
    6, IcaWrtNCell2,                       // PACKET_WRTNCELL2        0x1A
    4, IcaBeep,                            // PACKET_BEEP             0x1B
    4, IcaSetMousePosition,                // PACKET_SETMOU_POSITION  0x1C
    1, IcaSetMouseAttribute,               // PACKET_SETMOU_ATTR      0x1D
    2, IcaSetCursorPosition,               // PACKET_SETCUR_POSITION  0x1E
    1, IcaSetCursorRow,                    // PACKET_SETCUR_ROW       0x1F
    1, IcaSetCursorColumn,                 // PACKET_SETCUR_COLUMN    0x20
    1, IcaSetCursorSize,                   // PACKET_SETCUR_SIZE      0x21

    1, IcaScrollScreen,                    // PACKET_SCROLL_SCREEN    0x22
    7, IcaScrollUp,                        // PACKET_SCROLLUP         0x23
    7, IcaScrollDown,                      // PACKET_SCROLLDN         0x24
    7, IcaScrollLeft,                      // PACKET_SCROLLLF         0x25
    7, IcaScrollRight,                     // PACKET_SCROLLRT         0x26
    0, IcaScrollUp1,                       // PACKET_SCROLLUP1        0x27
    0, IcaScrollDown1,                     // PACKET_SCROLLDN1        0x28
    0, IcaScrollLeft1,                     // PACKET_SCROLLLF1        0x29
    0, IcaScrollRight1,                    // PACKET_SCROLLRT1        0x2A
    1, IcaScrollUpN,                       // PACKET_SCROLLUP2        0x2B
    1, IcaScrollDownN,                     // PACKET_SCROLLDN2        0x2C
    1, IcaScrollLeftN,                     // PACKET_SCROLLLF2        0x2D
    1, IcaScrollRightN,                    // PACKET_SCROLLRT2        0x2E

    2, IcaVirtualWrite,                    // PACKET_VIRTUAL_WRITE0   0x2F
   -1, IcaVirtualWrite,                    // PACKET_VIRTUAL_WRITE1   0x30
   -2, IcaVirtualWrite,                    // PACKET_VIRTUAL_WRITE2   0x31
    3, IcaVirtualAck,                      // PACKET_VIRTUAL_ACK      0x32

   -1, IcaSetGraphics,                     // PACKET_SET_GRAPHICS     0x33
   -1, IcaSetText,                         // PACKET_SET_TEXT         0x34
    1, IcaSetGlobalAttr,                   // PACKET_SET_GLOBAL_ATTR  0x35
    3, IcaSetVideoMode,                    // PACKET_SET_VIDEO_MODE   0x36
#ifdef DOS
    1, IcaSetLed,                          // PACKET_SET_LED          0x37
#else
    1, NULL,                               // PACKET_SET_LED          0x37
#endif
    2, IcaVirtualFlush,                    // PACKET_VIRTUAL_FLUSH    0x38
   -2, NULL,                               // PACKET_MOUSE2           0x39
    0, NULL,                               // PACKET_COMMAND_CACHE    0x3A
   -2, IcaClientData,                      // PACKET_SET_CLIENT_DATA  0x3B
};
#define PACKET_COMMAND_CACHE    0x3A // nn write n bytes of caching data
#define PACKET_SET_CLIENT_DATA  0x3B // set n bytes of a data type 


/*******************************************************************************
 *
 *  EmulProcessInput
 *
 *  The Protocol driver calls EmulProcessInput to process input data
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to raw input data
 *    InputCount (input)
 *       byte count of raw input
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdICA30EmulProcessInput( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PICAPROCEDURE pProc;
    PICASTATE pState;
    PWDICA pIca;
    USHORT Count;

    pIca = (PWDICA) pWd->pPrivate;

    TRACE(( TC_WD, TT_IFRAME, "WdIca: %u [%02x %02x %02x %02x %02x %02x]",
            InputCount, pInputBuffer[0], pInputBuffer[1], pInputBuffer[2],
            pInputBuffer[3], pInputBuffer[4], pInputBuffer[5] ));

    /*
     *  Check for ICA detection string
     */
    if ( !pIca->fIcaDetected ) {
        _CheckForIcaDetect( pWd, pInputBuffer, InputCount );
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Process ICA data
     */
    while ( InputCount > 0 ) {

        /*
         *  Get pointer to correct state table entry
         */
        pState = &StateTable[ pIca->PacketState ];

        /*
         *  Update next state
         */
        pIca->PacketState = pState->NextState;

        /*
         *  Save input data
         */
        switch( pState->DataType ) {

            case dTYPE :  /* default next state is sTYPE */

                TRACE((TC_WD,TT_IFRAME2,"WdIca: dTYPE %02x", *pInputBuffer ));
                *((LPBYTE) &pIca->PacketType) = *pInputBuffer++;
                InputCount--;

                if ( (USHORT)pIca->PacketType >= PACKET_MAXIMUM ) {
                    ASSERT( FALSE, pIca->PacketType );
                    /* bad data was received - bit bucket packet */
                    break;
                }

                pIca->PacketLength = ICAData[ pIca->PacketType ].Length;
                switch ( pIca->PacketLength ) {

                    case (USHORT)-2 :
                        pIca->PacketLength = 0;
                        pIca->PacketState = sLEN2L;
                        break;

                    case (USHORT)-1 :
                        pIca->PacketLength = 0;
                        pIca->PacketState = sLEN1;
                        break;

                    case 0 :
#ifdef DOS
                        // hide mouse pointer if visible
                        if ( pIca->fMouseVisible &&
                             pIca->PacketType >= PACKET_CLEAR_SCREEN &&
                             pIca->PacketType <= PACKET_SCROLLRT2 )
                            (void) MouseShowPointer( FALSE );
#endif
                        if ( pProc = ICAData[ pIca->PacketType ].pProcedure )
                            (*pProc)( pWd, NULL, 0 );

#ifdef DOS
                        // restore mouse pointer if visible
                        if ( pIca->fMouseVisible &&
                             pIca->PacketType >= PACKET_CLEAR_SCREEN &&
                             pIca->PacketType <= PACKET_SCROLLRT2 )
                            (void) MouseShowPointer( TRUE );
#endif
                        break;

                    default :
                        pIca->PacketState = sDATA;
                        break;
                }

                /* reset input buffer count */
                pWd->InputCount = 0;
                break;

            case dLEN1 :

                TRACE((TC_WD,TT_IFRAME2,"WdIca: dLEN1 %02x", *pInputBuffer));
                *((LPBYTE) &pIca->PacketLength) = *pInputBuffer++;
                InputCount--;
                if ( pIca->PacketLength == 0 )
                    goto data;
                break;

            case dLEN2L :

                TRACE((TC_WD,TT_IFRAME2,"WdIca: dLEN2L %02x", *pInputBuffer));
                *((LPBYTE) &pIca->PacketLength) = *pInputBuffer++;
                InputCount--;
                break;

            case dLEN2H :

                TRACE((TC_WD,TT_IFRAME2,"WdIca: dLEN2H %02x", *pInputBuffer));
                *(((LPBYTE) &pIca->PacketLength) + 1) = *pInputBuffer++;
                InputCount--;

                if ( (USHORT)pIca->PacketLength > pWd->InputBufferLength ) {
                    ASSERT( FALSE, pIca->PacketLength );
                    pIca->PacketState = sTYPE;
                    /* bad data was received - bit bucket packet */
                    break;
                }

                if ( pIca->PacketLength == 0 )
                    goto data;

                break;

            case dDATA :
data:
                ASSERT( pWd->InputCount < pWd->InputBufferLength, 0 );
                Count = pIca->PacketLength - pWd->InputCount;
                ASSERT( (int) Count >= 0, Count );
                if ( Count > InputCount )
                    Count = InputCount;

                memcpy( &pWd->pInputBuffer[pWd->InputCount], pInputBuffer, Count );

                pWd->InputCount += Count;
                pInputBuffer += Count;
                InputCount -= Count;

                TRACE(( TC_WD, TT_ICOOK,  "WdIca: dDATA %02x (%02x/%02x)",
                        Count, pWd->InputCount, pIca->PacketLength ));

                if ( pWd->InputCount < pIca->PacketLength )
                    break;

#ifdef DOS
                // hide mouse pointer if visible
                if ( pIca->fMouseVisible &&
                     pIca->PacketType >= PACKET_CLEAR_SCREEN &&
                     pIca->PacketType <= PACKET_SCROLLRT2 )
                    (void) MouseShowPointer( FALSE );
#endif

                TRACE(( TC_WD, TT_ICOOK,  "WdIca: input buffer @%p count %d",
                        pWd->pInputBuffer, pWd->InputCount ));

                if ( pProc = ICAData[ pIca->PacketType ].pProcedure )
                    (*pProc)( pWd, pWd->pInputBuffer, pWd->InputCount );

#ifdef DOS
                // restore mouse pointer if visible
                if ( pIca->fMouseVisible &&
                     pIca->PacketType >= PACKET_CLEAR_SCREEN &&
                     pIca->PacketType <= PACKET_SCROLLRT2 )
                    (void) MouseShowPointer( TRUE );
#endif

                pIca->PacketState = sTYPE;
                break;
        }
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  _CheckForIcaDetect
 *
 *  Check for ica detection string and display data
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    pInputBuffer (input)
 *       pointer to raw input data
 *    InputCount (input)
 *       byte count of raw input
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
_CheckForIcaDetect( PWD pWd,
                    LPBYTE pInputBuffer,
                    USHORT InputCount )
{
    PDSETINFORMATION PdSetInfo;
    LPBYTE pDetectString = ICA_DETECT_STRING;
    PWDICA pIca;
    USHORT i;
    USHORT Index = 0;

    pIca = (PWDICA) pWd->pPrivate;

    for ( i = 0; i < InputCount; i++ ) {

        switch ( pIca->IcaDetectState ) {

            case DETECT_LOOKING :

                if ( pInputBuffer[i] == *pDetectString ) {
                    pIca->IcaDetectState = DETECT_INSTRING;
                    pIca->IcaDetectOffset++;
                    if ( (i > Index) && pWd->fFocus ) {
                        WriteTTY( (LPVOID)pIca, &pInputBuffer[Index], 
                                          (USHORT)(i - Index) );
                    }
                }
                break;

            case DETECT_INSTRING :
                if ( pInputBuffer[i] != pDetectString[ pIca->IcaDetectOffset ] ) {
                    pIca->IcaDetectState = DETECT_LOOKING;
                    if ( pWd->fFocus ) 
                        WriteTTY( (LPVOID)pIca, pDetectString, pIca->IcaDetectOffset );
                    Index = i;
                }

                pIca->IcaDetectOffset++;
                if ( pIca->IcaDetectOffset == sizeof(ICA_DETECT_STRING)-1 ) {

                    TRACE(( TC_WD, TT_API1, "WdIca: ICA DETECTED" ));
                    pIca->IcaDetectState = DETECT_LOOKING;
                    pIca->IcaDetectOffset = 0;

                    /* disable tty mode */
                    pWd->fTTYConnected = FALSE;

                    /* delay on echo */
                    if ( pIca->fEchoTTY ) {
                        Delay( pIca->ulTTYDelay );
                    }
#if !defined(DOS) && !defined(RISCOS)
                    /* destroy vio window if allocated */
                    if ( pIca->fVioInit ) {
                        VioDestroyWindow( pIca->hVio );
                    }
#endif
                    /* enable ica protocol stack */
                    pIca->fIcaDetected = TRUE;
                    PdSetInfo.PdInformationClass  = PdIcaDetected;
                    (void) PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );

                    /* enable scan mode keyboard */
                    (void) WdKbdSetMode( pIca, Kbd_Scan );
                    /* arm timer to send ica detect sequence to host */
                    pIca->TimerIcaDetect = Getmsec();

                    return;
                }
                break;
        }
    }

    if ( (i > Index) && (pIca->IcaDetectState == DETECT_LOOKING) && pWd->fFocus ) {
        WriteTTY( (LPVOID)pIca, &pInputBuffer[Index], (USHORT)(i - Index) );
    }
}


/*******************************************************************************
 *
 *  WriteTTY
 *
 *  Writes out data in a tty fashion
 *
 * ENTRY:
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
WriteTTY( LPVOID pvIca, LPBYTE pTTYData, USHORT cbTTYData )
{
    INT rc;
    PWDICA pIca = (PWDICA)pvIca;

    /* do not echo ? */
    if ( !pIca->fEchoTTY )
        return;

    TRACE(( TC_WD, TT_API2, "WriteTTY: %u bytes", cbTTYData ));
    TRACEBUF(( TC_WD, TT_ORAW, pTTYData, cbTTYData ));

#if !defined(DOS) && !defined(RISCOS)

    rc = VioWrtTTY( pTTYData, cbTTYData, pIca->hVio );
    if ( rc == CLIENT_ERROR_INVALID_HANDLE ) {

        HDC   hdc = GetDC( pIca->hVio );
        RECT  rect;

        /* 
         *  Note modification
         */
        pIca->fVioInit = TRUE;
        (void) VioInitWindow( pIca->hVio, 25, 80, FALSE );

        /*
         *  Need to show window?
         */
        if ( !IsWindowVisible( pIca->hVio ) ) {
            ShowWindow( pIca->hVio, SW_SHOW );
#ifdef WIN32
            SetForegroundWindow( pIca->hVio );
#endif
        }

        /*
         *  Clear window
         */
        GetClientRect( pIca->hVio, &rect );
        FillRect( hdc, &rect, GetStockObject(BLACK_BRUSH) );
        ReleaseDC( pIca->hVio, hdc );

        rc = VioWrtTTY( pTTYData, cbTTYData, pIca->hVio );
    }
#else

    rc = VioWrtTTY( pTTYData, cbTTYData, pIca->hVio );

#endif
}

void
WdICA30EmulWriteTTY( PWD pWd, LPBYTE pTTYData, USHORT cbTTYData )
{
    WriteTTY(pWd->pPrivate, pTTYData, cbTTYData);
}

