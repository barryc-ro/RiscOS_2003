
/*************************************************************************
*
* timers.c
*
* Timer routines
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.22   15 Apr 1997 16:52:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.22   21 Mar 1997 16:07:36   bradp
*  update
*  
*     Rev 1.21   27 Jan 1996 19:22:46   bradp
*  update
*  
*     Rev 1.20   05 Dec 1995 10:33:24   miked
*  update
*  
*     Rev 1.19   27 Sep 1995 13:46:46   bradp
*  update
*  
*     Rev 1.18   26 Sep 1995 14:06:56   bradp
*  update
*  
*     Rev 1.17   05 Jul 1995 14:50:44   bradp
*  update
*  
*     Rev 1.16   23 May 1995 15:32:06   bradp
*  update
*  
*     Rev 1.15   03 May 1995 11:47:58   butchd
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

int RetransmitTimer( PPD );
int NakTimer( PPD );
int AckTimer( PPD );

VOID CancelAllTimers( PPD );
VOID UpdateRoundTripTime( PPD, ULONG );


/*=============================================================================
==   Internal procedures defined
=============================================================================*/

/*=============================================================================
==   External procedures used
=============================================================================*/

int WriteData( PPD, PPDWRITE, BYTE );
int PdNext( PPD, USHORT, PVOID );
int OutBufAlloc( PPD, POUTBUF, POUTBUF * );


/*******************************************************************************
 *
 *  RetransmitTimer
 *
 *  This timer fires when a outbuf is sent with an ack request and no
 *  ack is received within the timeout period.
 *
 *  Retransmit all outbufs that are on the unacknowledged buffer list.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - successful
 *
 ******************************************************************************/

int
RetransmitTimer( PPD pPd )
{
    PDWRITE WriteParams;
    PPDRELI pPdReli;
    POUTBUF pOutBuf;
    POUTBUF pOutBufNext;
    int rc;

    if ( !pPd->fOpen )
        return( pPd->CloseCode );

    pPdReli = (PPDRELI) pPd->pPrivate;

    TRACE(( TC_RELI, TT_API1,
            "PdReli: RETRANSMIT (%s), timeout %lu (%lu/%lu)",
            pPdReli->fNakRetransmit ? "nak" : "timeout",
            pPdReli->RetransmitTimeout, pPdReli->TotalRetransmitTime,
            pPdReli->MaxRetryTime ));

    /*
     *  Check how long we have been doing retries
     */
    if ( pPdReli->TotalRetransmitTime >= pPdReli->MaxRetryTime ) {
        TRACE(( TC_RELI, TT_ERROR, "PdReli: CRITICAL ERROR, retry time exceeded" ));

        /* terminate host connection, this will report an error on WdPoll */
        pPd->fOpen = FALSE;
        pPd->CloseCode = CLIENT_ERROR_CONNECTION_TIMEOUT;
        CancelAllTimers( pPd );
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Adjust retransmit timeout (exponential backoff)
     */
    if ( !pPdReli->fNakRetransmit ) {
        pPdReli->TotalRetransmitTime += pPdReli->RetransmitTimeout;
        pPdReli->RetransmitTimeout *= 2;
        if ( pPdReli->RetransmitTimeout > MAXIMUM_RETRANSMIT_TIMEOUT )
            pPdReli->RetransmitTimeout = MAXIMUM_RETRANSMIT_TIMEOUT;
    }

    /*
     *  Reset congestion window to it's smallest value
     */
    pPdReli->SlowStartThreshold = max( pPdReli->CongestionWindow / 2, 2 );
    pPdReli->CongestionWindow = DEFAULT_CONGESTION_WINDOW;
    pPdReli->CongestionCount = 0;

    /*
     *  Get pointer to head of ack list
     */
    pOutBuf = pPdReli->pOutBufHead;

    /*
     *  Make sure the sequence number being naked is on the list
     *  -- it must be at the head since the list is sorted by sequence number
     *  -- if it's not, the frame is currently being retransmited
     */
    if ( pPdReli->fNakRetransmit && pOutBuf ) {
        if ( pOutBuf->Sequence != pPdReli->LastNakSequence )
            goto armtimer;
    }

    /*
     *  Unlink all outbufs from list
     *  -- they will get put back as the writes complete
     */
    pPdReli->pOutBufHead = NULL;

    /*
     *  Retransmit all the outbufs that have not been acked
     */
    while ( pOutBuf ) {

        pOutBufNext = pOutBuf->pLink;
        pOutBuf->pLink = NULL;

        /* set retransmition flag */
        pOutBuf->fRetransmit = TRUE;

        TRACE(( TC_RELI, TT_API1, "PdReli: snd2 [%4u] %02x,%u",
                pOutBuf->ByteCount, pOutBuf->Sequence, pOutBuf->Fragment ));

        /* retransmit output buffer */
        WriteParams.pOutBuf = pOutBuf;
        rc = PdNext( pPd, PD__WRITE, &WriteParams );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        pOutBuf = pOutBufNext;
    }

    /*
     *  Arm retransmit timer again
     */
armtimer:
    pPdReli->TimerRetransmit = Getmsec() + pPdReli->RetransmitTimeout;

    /*
     *  Clear retransmit due to nak flag
     */
    pPdReli->fNakRetransmit = FALSE;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NakTimer
 *
 *  This timer fires when no response is received after sending a NAK
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - successful
 *
 ******************************************************************************/

int
NakTimer( PPD pPd )
{
    PPDRELI pPdReli;
    PDWRITE WriteParams;
    POUTBUF pOutBuf;
    PICAHEADER pHeader;
    int rc;

    if ( !pPd->fOpen )
        return( CLIENT_STATUS_SUCCESS );

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Cancel pending Ack
     */
    pPdReli->TimerAck = 0;

    /*
     *  Allocate outbuf
     */
    if ( rc = OutBufAlloc( pPd, NULL, &pOutBuf ) ) {
        TRACE(( TC_RELI, TT_API1, "PdReli: NakTimer pending, not enough outbufs" ));
        return( rc );
    }

    /*
     *  Initialize output buffer
     */
    pOutBuf->ByteCount = sizeof(ICAHEADER);
    pOutBuf->fControl  = TRUE;

    /*
     *  Initialize nak header
     */
    pHeader = (PICAHEADER) pOutBuf->pBuffer;
    pHeader->Flags    = ICA_NAK;
    pHeader->Sequence = pPdReli->ReceiveExpectedSeq;

    TRACE(( TC_RELI, TT_API1, "PdReli: send [ nak]       %02x", pHeader->Sequence ));

    /*
     *  Write Nak
     */
    WriteParams.pOutBuf = pOutBuf;
    return( PdNext( pPd, PD__WRITE, &WriteParams ) );
}


/*******************************************************************************
 *
 *  AckTimer
 *
 *  This timer is armed whenever there is an ACK to be sent.  If we send
 *  a data packet before this timer expires we send a piggyback ack and
 *  cancel the timer.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - successful
 *
 ******************************************************************************/

int
AckTimer( PPD pPd )
{
    PPDRELI pPdReli;
    PDWRITE WriteParams;
    POUTBUF pOutBuf;
    PICAHEADER pHeader;
    int rc;

    if ( !pPd->fOpen )
        return( CLIENT_STATUS_SUCCESS );

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Allocate outbuf
     */
    if ( rc = OutBufAlloc( pPd, NULL, &pOutBuf ) ) {
        TRACE(( TC_RELI, TT_API1, "PdReli: AckTimer pending, not enough outbufs" ));
        return( rc );
    }

    /*
     *  Initialize output buffer
     */
    pOutBuf->ByteCount = sizeof(ICAHEADER);
    pOutBuf->fControl  = TRUE;

    /*
     *  Initialize ack header
     */
    pHeader = (PICAHEADER) pOutBuf->pBuffer;
    pHeader->Flags    = ICA_ACK;
    pHeader->Sequence = pPdReli->ReceiveExpectedSeq - 1;

    TRACE(( TC_RELI, TT_API1, "PdReli: send [ack ]       %02x", pHeader->Sequence ));

    /*
     *  Turn off delayed acks
     */
    pPdReli->fAckNow = TRUE;

    /*
     *  Write ACK
     */
    WriteParams.pOutBuf = pOutBuf;
    return( PdNext( pPd, PD__WRITE, &WriteParams ) );
}


/*******************************************************************************
 *
 *  CancelAllTimers
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

VOID
CancelAllTimers( PPD pPd )
{
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    pPdReli->fNakSent = FALSE;

    pPdReli->TimerRetransmit = 0;
    pPdReli->TimerNak = 0;
    pPdReli->TimerAck = 0;
}


/*******************************************************************************
 *
 *  UpdateRoundTripTime
 *
 *  Calculate a "running average" round trip time
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *    TimeSpent (input)
 *       time spent in last round-trip  (1/1000 seconds)
 *
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

VOID
UpdateRoundTripTime( PPD pPd, ULONG TimeSpent )
{
    PPDRELI pPdReli;
    LONG Delta;

    pPdReli = (PPDRELI) pPd->pPrivate;

    if ( TimeSpent > 0 ) {

        /*
         *  Prime the pump
         */
        if ( pPdReli->RoundTripTime == 0 )
            pPdReli->RoundTripTime = pPdReli->RetransmitTimeout << 3;

        /*
         *  Smoothed round trip time is scaled x 8 here, so this is really:
         *     Delta = TimeSpent - RoundTripTime
         */
        Delta = TimeSpent - (pPdReli->RoundTripTime >> 3);
        pPdReli->RoundTripTime += Delta;
        if ( Delta < 0 )
            Delta = -Delta;

        /*
         *  Round trip deviation is scaled x 4 here, so this is really:
         *    RoundTripDeviation += (|Delta| - RoundTripDeviation) / 4
         */
        pPdReli->RoundTripDeviation += Delta - (pPdReli->RoundTripDeviation>>2);
    }

    /*
     *  Smoothed round trip time is scaled x 8 and
     *  Round trip deviation is scaled x 4 here, so this is really:
     *     RetransmitTimeout = 2 * (RoundTripTime + RoundTripDeviation)
     */
    pPdReli->RetransmitTimeout = ((pPdReli->RoundTripTime>>2) +
                                     pPdReli->RoundTripDeviation)>>1;

    Delta = (pPdReli->RetransmitTimeout / 4) + pPdReli->RetransmitTimeDelta;

    TRACE(( TC_RELI, TT_API2, "PdReli: UpdateRoundTripTime: %4lu,%4lu,%4lu (%lu)",
            TimeSpent, pPdReli->RetransmitTimeout,
            pPdReli->RetransmitTimeout + Delta, Delta ));

    // incrment by timeout delta (fudge factor)
    pPdReli->RetransmitTimeout += Delta;

    // reset total retransmit time
    pPdReli->TotalRetransmitTime = 0;
    pPdReli->ErrorFeedbackTime = 0;
}

