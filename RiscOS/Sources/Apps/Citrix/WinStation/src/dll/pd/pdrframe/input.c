
/*************************************************************************
*
*   input.c
*
*   Reliable Frame Protocol Driver - input state machine
*
*   copyright notice: Copyright 1994, Citrix Systems Inc.
*
*   Author: Kurt Perry
*
* $Log$
*  
*     Rev 1.13   15 Apr 1997 16:52:54   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   21 Mar 1997 16:07:40   bradp
*  update
*  
*     Rev 1.13   18 Mar 1997 21:02:30   chrisc
*  Prevent GPF on bad packet length
*  
*     Rev 1.12   08 May 1996 16:51:06   jeffm
*  update
*  
*     Rev 1.11   14 Jul 1995 12:08:28   bradp
*  update
*  
*     Rev 1.10   12 Jun 1995 16:58:26   terryt
*  Disable reliable framer on old hosts
*  
*     Rev 1.9   03 May 1995 11:50:08   butchd
*  clib.h now standard
*
*************************************************************************/

/*
 *  Includes
 */
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"
#include <citrix/ica.h>

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdrframe.h"


/*=============================================================================
==   External procedures defined
=============================================================================*/

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );


/*=============================================================================
==   External procedures used
=============================================================================*/


/*=============================================================================
==   Defines and structures
=============================================================================*/


/*=============================================================================
==   Local data
=============================================================================*/


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
 *    pInputBuffer (input)
 *       Points to input buffer containing data.
 *    InputCount (input)
 *       Number of bytes of data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
DeviceProcessInput( PPD pPd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PPDRFRAME  pPdRFrame;
    USHORT     Count;

    /*
     *  Get private data
     */
    pPdRFrame = (PPDRFRAME) pPd->pPrivate;

    /*
     *  Process all input available
     */
    while ( InputCount > 0 ) {

        /*
         *  Check if ica has been detected
         *  Check if framing is enabled
         *  Check for old hosts
         */
        if ( !pPd->fIcaDetected || !pPd->fEnableModule || pPdRFrame->Disable )
            return( (*pPd->pProcessInputProc)( pPd->pWdData, pInputBuffer, InputCount ) );
    
        /*
         *  State transistion
         */
        switch( pPdRFrame->ReceiveState ) {

            case sLEN1L :

                TRACE(( TC_PD, TT_IFRAME2, "PdRFrame: dLEN1L %02x", *pInputBuffer ));
                *((LPBYTE) &pPdRFrame->PacketLength) = *pInputBuffer++;
                InputCount--;

                /*
                 *  Next state: length hi-byte
                 */
                pPdRFrame->ReceiveState = sLEN1H;
                break;

            case sLEN1H :

                TRACE(( TC_WD, TT_IFRAME2, "PdRFrame: dLEN1H %02x", *pInputBuffer ));
                *(((LPBYTE) &pPdRFrame->PacketLength) + 1) = *pInputBuffer++;
                InputCount--;

                if ( (USHORT)pPdRFrame->PacketLength > pPdRFrame->InputBufferLength ) {
                    ASSERT( FALSE, pPdRFrame->PacketLength );

                    /* bad data was received - bit bucket packet */
                    pPdRFrame->ReceiveState = sLEN1L;
                    return( CLIENT_STATUS_SUCCESS );
                    break;
                }

                /*
                 *  Next state: data
                 */
                pPdRFrame->ReceiveState = sDATA;
                break;

            case sDATA :

                /*
                 *  No data buffered and full frame available
                 */
                if ( (pPdRFrame->InputCount == 0) && (InputCount >= pPdRFrame->PacketLength) ) {

                    /*
                     *  Pass along full frame
                     */
                    Count = pPdRFrame->PacketLength;
                    TRACE(( TC_FRAME, TT_API2, "PdFrame: non-buffered: bc %u", Count ));
                    (pPd->pProcessInputProc)( pPd->pWdData, pInputBuffer, Count );

                    /*
                     *  Adjust counts and pointers
                     */
                    InputCount -= Count;
                    pInputBuffer += Count;

                    /*
                     *  Next state: length lo-byte
                     */
                    pPdRFrame->ReceiveState = sLEN1L;
                }

                /*
                 *  Partial packet
                 */
                else {

                    ASSERT( pPdRFrame->InputCount < pPdRFrame->InputBufferLength, 0 );
                    Count = pPdRFrame->PacketLength - pPdRFrame->InputCount;
                    ASSERT( (int)Count >= 0, Count );
                    if ( Count > InputCount )
                        Count = InputCount;

                    /*
                     *  Buffer data
                     */
                    memcpy( &pPdRFrame->pInBuf[pPdRFrame->InputCount], pInputBuffer, Count );

                    /*
                     *  Adjust counts and pointers
                     */
                    pPdRFrame->InputCount += Count;
                    pInputBuffer += Count;
                    InputCount -= Count;

                    /*
                     *  More data to follow
                     */
                    if ( pPdRFrame->InputCount < pPdRFrame->PacketLength )
                        break;

                    /*
                     *  Pass along full frame
                     */
                    TRACE(( TC_FRAME, TT_API2, "PdFrame: buffered: bc %u", pPdRFrame->InputCount ));
                    (pPd->pProcessInputProc)( pPd->pWdData, pPdRFrame->pInBuf, pPdRFrame->InputCount );

                    /*
                     *  Next state: length lo-byte
                     */
                    pPdRFrame->ReceiveState = sLEN1L;

                    /*
                     *  Reset buffered count
                     */
                    pPdRFrame->InputCount = 0;
                }
                break;
        }
    }

    return( CLIENT_STATUS_SUCCESS );
}

