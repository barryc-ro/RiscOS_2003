
/*************************************************************************
*
* callup.c
*
* Routines to handle buffers, input data and broken connections
* -- called by transport level PD
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.17   15 Apr 1997 16:52:40   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.17   21 Mar 1997 16:07:30   bradp
*  update
*  
*     Rev 1.16   05 Dec 1995 10:33:12   miked
*  update
*  
*     Rev 1.15   20 Jul 1995 15:03:00   bradp
*  update
*  
*     Rev 1.14   01 Jun 1995 22:29:10   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.13   03 May 1995 11:47:24   butchd
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

int  WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void WFCAPI DeviceOutBufFree( PPD, POUTBUF );
void WFCAPI DeviceOutBufError( PPD, POUTBUF );
int  WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Internal procedures defined
=============================================================================*/

/*=============================================================================
==   External procedures used
=============================================================================*/

VOID OutBufLinkAck( PPD, POUTBUF );
int OutBufAlloc( PPD, POUTBUF, POUTBUF * );
VOID OutBufFree( PPD, POUTBUF );
VOID OutBufError( PPD, POUTBUF );
int SetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*******************************************************************************
 *
 *  DeviceOutBufAlloc
 *
 *  The transport pd calls this routine to allocate an output buffer
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    ppOutBuf (output)
 *       address to return pointer to buffer structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
DeviceOutBufAlloc( PPD pPd, POUTBUF * ppOutBuf )
{
    return( OutBufAlloc( pPd, NULL, ppOutBuf ) );
}


/*******************************************************************************
 *
 *  DeviceOutBufFree
 *
 *  The transport pd calls this routine when it's done writing a buffer
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void WFCAPI
DeviceOutBufFree( PPD pPd, POUTBUF pOutBuf )
{
    PPDRELI pPdReli;
    int DiffSeq;

    pPdReli = (PPDRELI) pPd->pPrivate;

    if ( pOutBuf->fControl || !pPd->fIcaDetected ) {

        /*
         *  Return control buffer to free list  (ACK/NAK)
         */
        OutBufFree( pPd, pOutBuf );

    } else {

        /*
         *  Check if this sequence number is greater than the last ack
         *  -- the outbuf may have been retransmiting when the ack came in
         */
        DiffSeq = (int)(pPdReli->LastAckSequence - pOutBuf->Sequence) & 0xff;
        if ( DiffSeq < (int)pPd->OutBufCountClient ) {

            /* decrement outbufs waiting for ack */
            if ( pOutBuf->fRetransmit ) {
                pPdReli->OutBufAckWaitCount--;
                ASSERT( pPdReli->OutBufAckWaitCount >= 0, 0 );
            }

            TRACE(( TC_RELI, TT_API2,
                    "PdReli: OutBufFree: %lx, seq %02x,%u, AckWait %u, rx %u",
                    pOutBuf, pOutBuf->Sequence, pOutBuf->Fragment,
                    pPdReli->OutBufAckWaitCount, pOutBuf->fRetransmit ));

            /* free the buffer */
            OutBufFree( pPd, pOutBuf );

        } else {

            /*
             *  Add buffer to list of buffers waiting to be ACKed
             */
            OutBufLinkAck( pPd, pOutBuf );
        }
    }
}


/*******************************************************************************
 *
 *  DeviceOutBufError
 *
 *  Return an outbuf to the wd free list
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void WFCAPI
DeviceOutBufError( PPD pPd, POUTBUF pOutBuf )
{
    OutBufError( pPd, pOutBuf );
}


/*******************************************************************************
 *
 *  DeviceSetInfo
 *
 *  The transport pd calls this routine when wants to modify wd data
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int WFCAPI
DeviceSetInfo( PPD pPd, SETINFOCLASS SetInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{
    return( SetInfo( pPd, SetInfoClass, pBuffer, cbBuffer ) );
}

/*******************************************************************************
 *
 *  DeviceQueryInfo
 *
 *  The transport pd calls this routine when wants to query wd data
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int WFCAPI
DeviceQueryInfo( PPD pPd, QUERYINFOCLASS QueryInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{
    return( QueryInfo( pPd, QueryInfoClass, pBuffer, cbBuffer ) );
}
