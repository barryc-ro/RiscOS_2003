
/*************************************************************************
*
* buffer.c
*
* Buffer routines
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.16   15 Apr 1997 16:52:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.16   21 Mar 1997 16:07:28   bradp
*  update
*  
*     Rev 1.15   03 Feb 1996 23:04:14   bradp
*  xcopy fix
*  
*     Rev 1.14   05 Dec 1995 10:33:06   miked
*  update
*  
*     Rev 1.13   20 Jul 1995 15:02:54   bradp
*  update
*  
*     Rev 1.12   03 May 1995 11:47:14   butchd
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
==   External procedures defined
=============================================================================*/

VOID OutBufLinkAck( PPD, POUTBUF );
VOID OutBufUnlinkAck( PPD, POUTBUF * );
VOID OutBufFreeSequence( PPD, BYTE );


/*=============================================================================
==   External procedures used
=============================================================================*/

VOID UpdateRoundTripTime( PPD, ULONG );
VOID OutBufError( PPD, POUTBUF );
VOID OutBufFree( PPD, POUTBUF );


/*******************************************************************************
 *
 *  OutBufLinkAck
 *
 *  td has sent the data (data is on the wire)
 *  -- link outbuf into proper place in ACK output buffers
 *  -- these buffers are waiting to be ACKed
 *  -- start retransmit timer
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pOutBuf (input)
 *       pointer to output buffer to link
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

VOID
OutBufLinkAck( PPD pPd, POUTBUF pOutBuf )
{
    POUTBUF pPrevOb;
    POUTBUF pOb;
    PPDRELI pPdReli;
    int DiffSeq;

    pPdReli = (PPDRELI) pPd->pPrivate;

    ASSERT( pOutBuf->pLink == NULL, 0 );

    /*
     *  Locate correct location to insert outbuf in list
     *  -- sequence numbers must be kept in order
     */
    pPrevOb = NULL;
    for ( pOb = pPdReli->pOutBufHead; pOb != NULL; pOb = pOb->pLink ) {

        /* if ( pOb->Sequence > pOutBuf->Sequence ) break; */
        DiffSeq = (int)(pOutBuf->Sequence - pOb->Sequence) & 0xff;
        if ( DiffSeq > (int)pPd->OutBufCountClient )
            break;

        /*
         * If sequence numbers match, make sure fragment
         * numbers are in correct order.
         */
        if ( pOutBuf->Sequence == pOb->Sequence &&
             pOutBuf->Fragment < pOb->Fragment )
            break;

        pPrevOb = pOb;
    }

    /*
     *  Insert outbuf into ack queue
     */
    if ( pPrevOb ) {
        pPrevOb->pLink = pOutBuf;
        pOutBuf->pLink = pOb;
    } else {
        pOutBuf->pLink = pPdReli->pOutBufHead;
        pPdReli->pOutBufHead = pOutBuf;
    }

    /*
     *  Increment outbufs wait for ack
     */
    if ( !pOutBuf->fRetransmit ) {
        pPdReli->OutBufAckWaitCount++;
        ASSERT( pPdReli->OutBufAckWaitCount <= (int) pPd->OutBufCountClient, 0 );
    }

    /*
     *  Get timestamp
     */
    pOutBuf->StartTime = Getmsec();

    TRACE(( TC_RELI, TT_API2,
            "PdReli: OutBufLinkAck: %lx, seq %02x,%u, AckWait %u, rx %u",
            pOutBuf, pOutBuf->Sequence, pOutBuf->Fragment,
            pPdReli->OutBufAckWaitCount, pOutBuf->fRetransmit ));

    /*
     *  Start retransmit timer
     */
    pPdReli->TimerRetransmit = pOutBuf->StartTime + pPdReli->RetransmitTimeout;
}


/*******************************************************************************
 *
 *  OutBufUnlinkAck
 *
 *  We received an ACK (data has been successfully processed)
 *  -- unlink first outbuf from list of ACK output buffers
 *  -- calculate roundtrip time (used for retransmit timeout)
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    ppOutBuf (output)
 *       address to return pointer to unlinked outbuf
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

VOID
OutBufUnlinkAck( PPD pPd, POUTBUF * ppOutBuf )
{
    PPDRELI pPdReli;
    POUTBUF pOutBuf;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Unlink first outbuf in list
     */
    pOutBuf = pPdReli->pOutBufHead;
    ASSERT( pOutBuf, 0 );
    pPdReli->pOutBufHead = pOutBuf->pLink;
    pOutBuf->pLink = NULL;

    /*
     *  Decrement outbufs waiting for ack
     */
    pPdReli->OutBufAckWaitCount--;
    ASSERT( pPdReli->OutBufAckWaitCount >= 0, 0 );

    /*
     *  If no outbufs waiting for ack - cancel retransmit timer
     */
    if ( pPdReli->OutBufAckWaitCount == 0 ) {
        pPdReli->TimerRetransmit = 0;
        TRACE(( TC_RELI, TT_API2, "PdReli: OutBufUnlinkAck, cancel retransmit timer" ));
    }

    TRACE(( TC_RELI, TT_API2,
            "PdReli: OutBufUnlinkAck: %lx, seq %02x,%u,  AckWait %u, rx %u",
            pOutBuf, pOutBuf->Sequence, pOutBuf->Fragment,
            pPdReli->OutBufAckWaitCount, pOutBuf->fRetransmit ));

    *ppOutBuf = pOutBuf;
}


/*******************************************************************************
 *
 *  OutBufFreeSequence
 *
 *  Return all output buffers that have a sequence number less than or
 *  equal to specified sequence number.  This routine is called when we
 *  receive an ACK.
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    Sequence (input)
 *       sequence number
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

VOID
OutBufFreeSequence( PPD pPd, BYTE Sequence )
{
    POUTBUF pNext;
    POUTBUF pMatching;
    POUTBUF pOutBuf;
    PPDRELI pPdReli;
    int DiffSeq;
    int fUpdateTime = TRUE;

    pPdReli = (PPDRELI) pPd->pPrivate;

    TRACE(( TC_RELI, TT_API2, "PdReli: OutBufFreeSequence: %02x", Sequence ));

    /*
     *  Save last ack sequence
     */
    pPdReli->LastAckSequence = Sequence;

    /*
     *  Find output buffer with specified sequence number on
     *  unacknowledged buffer list.  Multiple occurances of the
     *  same sequence number may exist.  We need to find the last
     *  occurance.
     */
    pMatching = NULL;
    for ( pNext = pPdReli->pOutBufHead; pNext != NULL; pNext = pNext->pLink ) {

        /* if ( pNext->Sequence > Sequence ) break; */
        DiffSeq = (int)(Sequence - pNext->Sequence) & 0xff;
        if ( DiffSeq > (int)pPd->OutBufCountClient )
            break;

        pMatching = pNext;
    }

    /*
     *  If sequence number didn't exist - return
     */
    if ( pMatching == NULL )
       return;

    /*
     *  pMatching = outbuf with matching sequence number
     *
     *  Unlink and free all buffers up to and including the outbuf
     *  that we just located (pMatching).
     *  -- all of these outbufs should have sequence numbers less
     *     than or equal to the matching sequence number.
     */
    do {

        /*
         * unlink first outbuf
         */
        OutBufUnlinkAck( pPd, &pOutBuf );

        /*
         *  Calculate round trip time (skip retransmits)
         *  -- received ack for this outbuf
         */
        if ( fUpdateTime && !pOutBuf->fRetransmit ) {
            fUpdateTime = FALSE;
            ASSERT( pOutBuf->StartTime, 0 );
            UpdateRoundTripTime( pPd, Getmsec() - pOutBuf->StartTime );
            pOutBuf->StartTime = 0;
        } else {
            UpdateRoundTripTime( pPd, 0L );
        }

        /*
         * free buffer
         */
        OutBufFree( pPd, pOutBuf );

    } while ( pOutBuf != pMatching );
}

