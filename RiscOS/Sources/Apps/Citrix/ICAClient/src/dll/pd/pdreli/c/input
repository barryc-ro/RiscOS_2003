
/*************************************************************************
*
* input.c
*
* Input processing routines
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.20   15 Apr 1997 16:52:42   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.20   21 Mar 1997 16:07:32   bradp
*  update
*  
*     Rev 1.19   29 Jan 1996 18:02:56   bradp
*  update
*  
*     Rev 1.18   27 Sep 1995 13:46:30   bradp
*  update
*  
*     Rev 1.17   05 Jul 1995 14:50:34   bradp
*  update
*  
*     Rev 1.16   28 Jun 1995 22:14:34   bradp
*  update
*  
*     Rev 1.15   03 May 1995 11:47:32   butchd
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

int WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );
int ProcessData( PPD, PICAHEADER, LPBYTE, USHORT );
int ProcessAck( PPD, BYTE );
int ProcessNak( PPD, BYTE );


/*=============================================================================
==   External procedures used
=============================================================================*/

VOID OutBufFreeSequence( PPD, BYTE );
int WriteReset( PPD );
int  QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 *  assemble one reliable ica packet
 *  -- this routine is called by the frame transport PD
 *
 *  This routine is called with one reliable frame.  This frame may
 *  contain may ica packets, but we do not look at the data here.
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
DeviceProcessInput( PPD pPd,
                    LPBYTE pBuffer,
                    USHORT ByteCount )
{
    ENCRYPTIONINIT EncryptionInit;
    PICAHEADER pHeader;
    PPDRELI pPdReli;
    int fWriteReset = FALSE;
    int rc = CLIENT_STATUS_SUCCESS;

    /*
     *  Check if ica has been detected
     */
    if ( !pPd->fIcaDetected )
        return( (*pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );

    pPdReli = (PPDRELI) pPd->pPrivate;

    TRACE(( TC_RELI, TT_API4,
            "PdReli: %u [%02x %02x %02x %02x %02x %02x]",
            ByteCount, pBuffer[0], pBuffer[1], pBuffer[2],
            pBuffer[3], pBuffer[4], pBuffer[5] ));

    ASSERT( ByteCount >= sizeof(ICAHEADER), ByteCount );

    /*
     *  Get pointer to header
     */
    pHeader = (PICAHEADER) pBuffer;
    pBuffer += sizeof(ICAHEADER);
    ByteCount -= sizeof(ICAHEADER);

    /*
     *  Check for RESET
     */
    if ( pHeader->Flags & ICA_RESET ) {

        (void) QueryInfo( pPd, QueryEncryptionInit,
                          (LPBYTE)&EncryptionInit, sizeof(EncryptionInit) );

        if ( ByteCount == 0 || EncryptionInit.EncryptionLevel == 0 ) {
            TRACE(( TC_RELI, TT_API1, "Pdreli: recv [ rst]" ));
            pPdReli->ReceiveExpectedSeq = 0;
            pPd->fSendStatusConnect = TRUE;
            fWriteReset = TRUE;
        }
    }

    /*
     *  Check for DATA
     */
    if ( pHeader->Flags & ICA_DATA ) {

        /* check for piggyback ack */
        if ( pHeader->Flags & ICA_PIGGYACK ) {
            ASSERT( ByteCount > 0, 0 );

            if ( rc = ProcessAck( pPd, *pBuffer ) )
                return( rc );

            TRACE(( TC_RELI, TT_API1, "PdReli: recv [pack]       %02x (%u/%u)",
                    *pBuffer, pPdReli->OutBufAckWaitCount, pPd->OutBufCountClient ));

            pBuffer++;
            ByteCount--;
        }

        rc = ProcessData( pPd, pHeader, pBuffer, ByteCount );

    /*
     *  Check for ACK
     */
    } else if ( pHeader->Flags & ICA_ACK ) {

        ASSERT( ByteCount == 0, ByteCount );
        rc = ProcessAck( pPd, pHeader->Sequence );
        TRACE(( TC_RELI, TT_API1, "PdReli: recv [ ack]       %02x (%u/%u)",
                pHeader->Sequence, pPdReli->OutBufAckWaitCount, pPd->OutBufCountClient ));


    /*
     *  Check for NAK
     */
    } else if ( pHeader->Flags & ICA_NAK ) {

        ASSERT( ByteCount == 0, ByteCount );
        rc = ProcessNak( pPd, pHeader->Sequence );
    }

    /* 
     *  Check if we should send a reset 
     */
    if ( fWriteReset ) {
        (void) WriteReset( pPd );
    }

    return( rc );
}


/*******************************************************************************
 *
 *  ProcessData
 *
 *  Process one reliable data packet
 *
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *    pHeader (input)
 *       pointer to ica data header
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
ProcessData( PPD pPd, PICAHEADER pHeader, LPBYTE pBuffer, USHORT ByteCount )
{
    PPDRELI pPdReli;
    int DiffSeq;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Verify sequence number
     */
    DiffSeq = (int)(pPdReli->ReceiveExpectedSeq - pHeader->Sequence) & 0xff;
    if ( DiffSeq ) {

        /*
         *  Check if sequence number is too large
         *  -- occurs when a complete frame is lost (bad crc in pdframe)
         *  -- nak and bit bucket packet
         */
        if ( DiffSeq > (int)pPd->OutBufCountHost ) {

            TRACE(( TC_RELI, TT_ERROR,
                    "PdReli: recv [%4u]    %02x    ERROR - bad seq, expected %02x",
                    ByteCount, pHeader->Sequence, pPdReli->ReceiveExpectedSeq ));

            /*
             *  Arm timer to send a NAK immediately
             *  -- if we already sent a nak don't send another
             */
            if ( !pPdReli->fNakSent ) {
                pPdReli->fNakSent = TRUE;
                pPdReli->TimerNak = Getmsec();
                pPdReli->StartRecvRetryTime = pPdReli->TimerNak; // start time
            }

            return( CLIENT_STATUS_SUCCESS );
        }

        /*
         *  Duplicate sequence number
         *  -- occurs when the remote retransmit timer expires
         *  -- this handles lost acks
         *  -- bit bucket packet
         */
        TRACE(( TC_RELI, TT_ERROR,
                "PdReli: recv [%4u]    %02x    ERROR - dup seq, expected %02x",
                ByteCount, pHeader->Sequence, pPdReli->ReceiveExpectedSeq ));

        /*
         *  Arm timer to send a delayed ACK
         */
        if ( !pPdReli->TimerAck )
            pPdReli->TimerAck = Getmsec() + pPdReli->DelayedAckTime;

        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  GOOD Packet
     */
    TRACE(( TC_RELI, TT_API1, "PdReli: recv [%4u]    %02x", ByteCount, pHeader->Sequence ));

    /*
     *  Increment expected sequence number
     */
    pPdReli->ReceiveExpectedSeq++;

    /*
     *  Cancel Nak timer, reset nak sent flag, clear total receive retry time
     */
    pPdReli->fNakSent = FALSE;
    pPdReli->TimerNak = 0;
    pPdReli->StartRecvRetryTime = 0;
    pPdReli->ErrorFeedbackTime = 0;

    /*
     *  Check Ack request bit
     *  -- if it's not set, arm a delayed ack
     */
    if ( pPdReli->fAckNow )
        pPdReli->TimerAck = Getmsec();
    else if ( !pPdReli->TimerAck )
        pPdReli->TimerAck = Getmsec() + pPdReli->DelayedAckTime;

    /*
     *  Send validated data to next PD
     */
    return( (pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );
}


/*******************************************************************************
 *
 *  ProcessAck
 *
 *  Process ack packet
 *
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *    Sequence (input)
 *       ack all packets up to and including this sequence number
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ProcessAck( PPD pPd, BYTE Sequence )
{
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  ACK all outbufs up to and including sequence number
     */
    OutBufFreeSequence( pPd, Sequence );

    /*
     *  Increase congestion window  (allow more outstanding outbufs)
     */
    if ( pPdReli->CongestionWindow < (int) pPd->OutBufCountClient ) {

        if ( pPdReli->CongestionWindow < pPdReli->SlowStartThreshold ) {
            /* slow start */
            pPdReli->CongestionWindow++;
        } else if ( ++pPdReli->CongestionCount > pPdReli->CongestionWindow ) {
            /* congestion avoidance */
            pPdReli->CongestionWindow++;
            pPdReli->CongestionCount = 0;
        }
        TRACE(( TC_RELI, TT_API3,
           "PdReli: ProcessAck, congestion %u,%u, slow start %u",
           pPdReli->CongestionWindow, pPdReli->CongestionCount,
           pPdReli->SlowStartThreshold ));
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  ProcessNak
 *
 *  Process nak packet
 *
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to pd data structure
 *    Sequence (input)
 *       ack all outbufs up to but not including the bad outbuf
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ProcessNak( PPD pPd, BYTE Sequence )
{
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    TRACE(( TC_RELI, TT_API1, "PdReli: recv [nak ]       %02x", Sequence ));

    /*
     *  ACK all outbufs up to but not including the bad outbuf
     */
    OutBufFreeSequence( pPd, (BYTE)(Sequence - 1) );

    /*
     *  Set retransmit due to nak flag
     */
    pPdReli->fNakRetransmit = TRUE;

    /*
     *  Save last nak sequence
     */
    pPdReli->LastNakSequence = Sequence;

    /*
     *  Start a timer to retransmit all the outbufs that have not been ACKed
     *  -- we don't want to retransmit all these outbufs on the read thread
     *     because the write may block.
     */
    pPdReli->TimerRetransmit = Getmsec();   // arm timer to fire now
    return( CLIENT_STATUS_SUCCESS );
}


