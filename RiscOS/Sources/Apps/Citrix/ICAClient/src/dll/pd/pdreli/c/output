
/*************************************************************************
*
* output.c
*
* Output processing routines
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.19   15 Apr 1997 16:52:44   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.19   21 Mar 1997 16:07:32   bradp
*  update
*  
*     Rev 1.18   09 Feb 1996 17:25:24   bradp
*  update
*  
*     Rev 1.17   29 Jan 1996 18:03:00   bradp
*  update
*  
*     Rev 1.16   05 Dec 1995 10:33:18   miked
*  update
*  
*     Rev 1.15   15 Aug 1995 09:57:44   bradp
*  update
*  
*     Rev 1.14   03 May 1995 11:47:40   butchd
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
#include "citrix/icareli.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdreli.h"


/*=============================================================================
==   Defines and structures
=============================================================================*/

/*=============================================================================
==   External procedures defined
=============================================================================*/

int CheckSlowStart( PPD );
int WriteData( PPD, PPDWRITE, BYTE );
int WriteReset( PPD );


/*=============================================================================
==   External procedures used
=============================================================================*/

int PdNext( PPD, USHORT, PVOID );
int OutBufAlloc( PPD, POUTBUF, POUTBUF * );
void OutBufError( PPD, POUTBUF );
int DeviceCancelWrite( PPD );
VOID OutBufFree( PPD, POUTBUF );
VOID OutBufUnlinkAck( PPD, POUTBUF * );


/*******************************************************************************
 *
 *  CheckSlowStart
 *
 *  return an error if we are doing slow start or congestion avoidance
 *  - this routine is called by WD OutBufReserve
 *
 *  BUGBUG -- this routine is not being called
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS   - no error
 *    CLIENT_ERROR_NO_OUTBUF - we are waiting for an ack (slow start)
 *
 ******************************************************************************/

int
CheckSlowStart( PPD pPd )
{
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Check if we should do slow start / congestion avoidance
     *  -- this slows down transmits, reducing congestion
     */
    if ( pPdReli->OutBufAckWaitCount >= pPdReli->CongestionWindow ) {

        TRACE(( TC_RELI, TT_API4, "PdReli: CheckSlowStart %u/%u (waiting)",
               pPdReli->OutBufAckWaitCount, pPdReli->CongestionWindow ));

        return( CLIENT_ERROR_NO_OUTBUF );
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  WriteData
 *
 *  Write a data packet
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdWrite (input)
 *       pointer to protocol driver write structure
 *    Flags (input)
 *       packet flags
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
WriteData( PPD pPd, PPDWRITE pPdWrite, BYTE Flags )
{
    PPDRELI pPdReli;
    POUTBUF pOutBuf;
    PICAHEADER pHeader;

    pPdReli = (PPDRELI) pPd->pPrivate;

    pOutBuf = pPdWrite->pOutBuf;

    TRACE(( TC_RELI, TT_API1, "PdReli: send [%4u] %02x       (%u/%u)",
            pOutBuf->ByteCount, pPdReli->TransmitSequence,
            pPdReli->OutBufAckWaitCount, pPd->OutBufCountClient ));

    /*
     *  Check if a delayed ack is pending
     */
    if ( pPdReli->TimerAck ) {

        /*
         *  Cancel ack timer
         */
        pPdReli->TimerAck = 0;

        /*
         *  ACK pending - piggyback ack in the data packet
         */
        pOutBuf->pBuffer -= (sizeof(ICAHEADER) + 1);
        pOutBuf->ByteCount += (sizeof(ICAHEADER) + 1);
        pHeader = (PICAHEADER) pOutBuf->pBuffer;
        pHeader->Flags = Flags | ICA_PIGGYACK;

        pOutBuf->pBuffer[ sizeof(ICAHEADER) ] = pPdReli->ReceiveExpectedSeq - 1;

        TRACE(( TC_RELI, TT_API1, "PdReli: send [pack]       %02x",
                pPdReli->ReceiveExpectedSeq - 1 ));

    } else {

        /*
         *  no ACK pending
         */
        pOutBuf->pBuffer -= sizeof(ICAHEADER);
        pOutBuf->ByteCount += sizeof(ICAHEADER);
        pHeader = (PICAHEADER) pOutBuf->pBuffer;
        pHeader->Flags = Flags;
    }

    /*
     *  Initialize packet sequence number
     */
    pHeader->Sequence = pPdReli->TransmitSequence++;
    pOutBuf->Sequence = pHeader->Sequence;
    pOutBuf->Fragment = 0;

    /*
     *  Turn on delayed acks
     */
    pPdReli->fAckNow = FALSE;

    /*
     *  Write Data
     *  -- PdFreeProcedure (callup.c) will be called when the outbuf is
     *     actually written.
     */
    return( PdNext( pPd, PD__WRITE, pPdWrite ) );
}


/*******************************************************************************
 *
 *  WriteReset
 *
 *  Write a RESET packet
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
WriteReset( PPD pPd )
{
    PDWRITE WriteParams;
    PPDRELI pPdReli;
    POUTBUF pOutBuf;
    int rc;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  If there are any buffers - free them
     */
    while ( pPdReli->pOutBufHead ) {
        OutBufUnlinkAck( pPd, &pOutBuf );
        TRACE(( TC_RELI, TT_API2,"PdReli: WriteReset: OutBufFree %lx", pOutBuf ));
        OutBufFree( pPd, pOutBuf );
    }

    /*
     *  Allocate outbuf
     *  -- don't use common routine
     */
    if ( rc = (pPd->pOutBufAllocProc)( pPd->pWdData, &pOutBuf ) ) {
        ASSERT( FALSE, rc );
        return( rc );
    }
    pOutBuf->ByteCount = 0;

    /*
     *  Reset sequence numbers
     */
    pPdReli->TransmitSequence = 0;
    pPdReli->LastAckSequence  = 0xff;
    pPdReli->LastNakSequence  = 0;

    /*
     *  Write RESET
     */
    TRACE(( TC_RELI, TT_API1, "PdReli: send [ rst]" ));
    WriteParams.pOutBuf = pOutBuf;
    (void) WriteData( pPd, &WriteParams, ICA_RESET | ICA_DATA );

    return( CLIENT_STATUS_SUCCESS );
}
