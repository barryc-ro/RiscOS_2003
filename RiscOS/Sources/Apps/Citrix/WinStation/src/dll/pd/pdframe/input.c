
/*************************************************************************
*
* input.c
*
* Frame Protocol Driver - input state machine
*
*    An input state machine assembles frames from streaming input data.
*    After a frame is assembled it is checked using the crc.  If the crc
*    is ok the frame is sent to the next pd, otherwise the data is bit
*    bucketed.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen (5/3/94)
*
* $Log$
*  
*     Rev 1.14   15 Apr 1997 16:52:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   21 Mar 1997 16:07:16   bradp
*  update
*  
*     Rev 1.13   02 Feb 1996 16:29:22   bradp
*  update
*  
*     Rev 1.12   19 Jan 1996 11:11:10   bradp
*  update
*  
*     Rev 1.11   03 May 1995 10:13:48   butchd
*  clib.h now standard
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
#include "citrix/ica.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdframe.h"


/*=============================================================================
==   External procedures defined
=============================================================================*/

int WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );
int ProcessFrame( PPD, LPBYTE, USHORT );


/*=============================================================================
==   External procedures used
=============================================================================*/

USHORT CrcBuffer( LPBYTE, USHORT );


/*=============================================================================
==   Defines and structures
=============================================================================*/

/*
 *  States for FRAME input state table  (ReceiveState)
 */
typedef enum _FSTATES {
    sSYNC,      // 0  starting state - looking for frame
    sDATA,      // 1  data
    sDATAX,     // 2  escaped data
    sNUMSTATES
} FISTATES;

/*
 *  Data types for FRAME state table
 *  -- used to save incoming data
 */
typedef enum _FTYPES {
    dNONE,	// 0 no data to save
    dSFRAME,    // 1 start of frame - reset input buffer length to zero
    dDATA,	// 2 data
    dDATAX,	// 3 escaped data
    dEFRAME,    // 4 end of frame - complete frame has been received
    dNUMTYPES
} FTYPES;

/*
 *  Character data class for FRAME state table
 *  -- used to clasify incoming data for use with state machine
 *
 *  NOTE: keep this in sync with "SpecialChar" in pdframe.c
 */
typedef enum _FCLASSES {
    cDATA,		// 0 character data
    cDATAX,             // 1 escaped character data (0x11, 0x13)
    cESC,               // 2 0x7d
    cFLAG,              // 3 0x7e
    cNUMCLASSES
} FCLASSES;

/*
 *  FRAME state table structure
 *  -- used in state table array
 */
typedef struct _FSTATE {
    FTYPES DataType;              // data type to save
    FISTATES NextState;           // next state
} FSTATE, * PFSTATE;


/*=============================================================================
==   Local data
=============================================================================*/

extern BYTE SpecialChar[256];

/*
 *  FRAME state table array
 */
static FSTATE StateTable[ sNUMSTATES ][ cNUMCLASSES ] = {

    /* sSYNC   */  dSFRAME, sSYNC,    // 0 cDATA
    /*  0      */  dSFRAME, sSYNC,    // 1 cDATAX
                   dSFRAME, sSYNC,    // 2 cESC
                   dSFRAME, sDATA,    // 3 cFLAG

    /* sDATA   */  dDATA,   sDATA,    // 0 cDATA
    /*  1      */  dDATA,   sDATA,    // 1 cDATAX
                   dNONE,   sDATAX,   // 2 cESC
                   dEFRAME, sDATA,    // 3 cFLAG

    /* sDATAX  */  dDATAX,  sDATA,    // 0 cDATA
    /*  2      */  dDATAX,  sDATA,    // 1 cDATAX
                   dSFRAME, sSYNC,    // 2 cESC
                   dSFRAME, sSYNC,    // 3 cFLAG
};


/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 *  assemble one frame
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to Pd data structure
 *    pBuffer (input)
 *       Points to input buffer containing data.
 *    ByteCount (input)
 *       Number of bytes of data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
DeviceProcessInput( PPD pPd, LPBYTE pBuffer, USHORT ByteCount )
{
    PFSTATE pState;
    PPDFRAME pPdFrame;
    USHORT Count;
    int rc;
    BYTE Ch;

    /*
     *  Check if ica has been detected
     */
    if ( !pPd->fIcaDetected )
        return( (*pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );

    ASSERT( SpecialChar[FRAME_FLAG] == cFLAG, 0 );
    ASSERT( SpecialChar[FRAME_ESC] == cESC, 0 );

    pPdFrame = (PPDFRAME) pPd->pPrivate;

    TRACE(( TC_FRAME, TT_IFRAME,
            "PdFrame: %u [%02x %02x %02x %02x %02x %02x]",
            ByteCount, pBuffer[0], pBuffer[1], pBuffer[2],
            pBuffer[3], pBuffer[4], pBuffer[5] ));

    for ( Count = 0; Count < ByteCount; Count++ ) {

        /*
         *  Get next input data character
         */
        Ch = *pBuffer++;

        /*
         *  Get pointer to correct state table entry
         */
        pState = &StateTable[ pPdFrame->ReceiveState ][ SpecialChar[Ch] ];

//      TRACE(( TC_FRAME, TT_IFRAME2, "PdFrame: StateTable[%u][%u](%02x) -> %u %u",
//                 pPdFrame->ReceiveState, SpecialChar[Ch], Ch,
//                 pState->NextState, pState->DataType ));

        /*
         *  Update next state
         */
        pPdFrame->ReceiveState = pState->NextState;

        /*
         *  Save input data
         */
        switch( pState->DataType ) {

            /*
             *  Process input data
             */
            case dDATA :
                if ( pPdFrame->InputCount >= pPd->OutBufLength ) {
                    TRACE(( TC_FRAME, TT_ERROR,  "PdFrame: dDATA   %02x (ERROR)", Ch ));
                    pPdFrame->ReceiveState = sSYNC;
                    break;
                }
                pPdFrame->pInBuf[ pPdFrame->InputCount++ ] = Ch;
//              TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dDATA   %02x (%u)",
//                      Ch, pPdFrame->InputCount ));
                break;

            /*
             *  Process escaped input data
             */
            case dDATAX :
                if ( pPdFrame->InputCount >= pPd->OutBufLength ) {
                    TRACE(( TC_FRAME, TT_ERROR,  "PdFrame: dDATAX  %02x (ERROR)", Ch ));
                    pPdFrame->ReceiveState = sSYNC;
                    break;
                }
                pPdFrame->pInBuf[ pPdFrame->InputCount++ ] = Ch ^ 0x20;
                TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dDATAX  %02x->%02x (%u)",
                        Ch, Ch^0x20, pPdFrame->InputCount ));
                break;

            /*
             *  Reset input buffer count to zero
             */
            case dSFRAME :
                pPdFrame->InputCount = 0;
                TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dSFRAME %02x", Ch ));
                break;

            /*
             *  Process received input frame
             */
            case dEFRAME :
                if ( pPdFrame->InputCount == 0 ) {
                    TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dSFRAME2 %02x", Ch ));
                    break;
                }
                TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dEFRAME %02x (%u)",
                        Ch, pPdFrame->InputCount ));
                rc = ProcessFrame( pPd, pPdFrame->pInBuf, pPdFrame->InputCount );
                pPdFrame->InputCount = 0;
                if ( rc )
                    return( rc );
                break;

            /*
             *  Do nothing state
             */
            case dNONE :
//              TRACE(( TC_FRAME, TT_ICOOK,  "PdFrame: dNONE   %02x", Ch ));
                break;

        }
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  ProcessFrame
 *
 *  Process one frame
 *
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *    pBuffer (input)
 *       pointer to packet input data
 *    ByteCount (input)
 *       byte count of packet data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ProcessFrame( PPD pPd, LPBYTE pBuffer, USHORT ByteCount )
{
    PPDFRAME pPdFrame;
    USHORT CrcCalculated;
    USHORT CrcReceived;
    USHORT i;

    pPdFrame = (PPDFRAME) pPd->pPrivate;

    if ( ByteCount <= 2 ) {
        /* bit bucket bad frame */
        TRACE(( TC_FRAME, TT_ERROR, "PdFrame: ERROR: short" ));
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Get the buffer CRC
     *  -- the last two bytes in the buffer are crc
     */
    ByteCount -= 2;
    CrcReceived = (USHORT) *((PUSHORT)&pBuffer[ByteCount]);

    /*
     *  Verify CRC
     */
    CrcCalculated = CrcBuffer( pBuffer, ByteCount );
    if( CrcReceived != CrcCalculated ) {
        TRACE(( TC_FRAME, TT_ERROR,
                "PdFrame: ERROR: bad crc, received %04x, calculated %04x",
                CrcReceived, CrcCalculated ));

        /*
         *  Check for 7 bit data path 
         *
         *  WARNING: this works today but might have to change if the ica
         *           init packet changes.  
         *  
         *  PACKET_INIT_REQUEST
         *     8 bit:    04 01 00 01 00 01 E1 ED
         *     7 bit:    04 01 00 01 00 01 61 6D
         */
        if ( !pPdFrame->f8BitData ) {
            if ( (CrcReceived | 0x8080) == CrcCalculated ) 
                return( CLIENT_ERROR_SEVEN_BIT_DATA_PATH );
        }

        /* bit bucket bad frame */
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  If eight bit data - set eight bit data flag
     */
    if ( !pPdFrame->f8BitData ) {
        for ( i=0; i < ByteCount+2; i++ ) {
            if ( pBuffer[i] & 0x80 ) {
                pPdFrame->f8BitData = TRUE;
                break;
            }
        }
    }

    /*
     *  Send validated frame to WinStation driver
     */
    TRACE(( TC_FRAME, TT_API2, "PdFrame: SUCCESS: bc %u", ByteCount ));
    return( (pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );
}

