
/*************************************************************************
*
*   buffer.c 
*
*   Common WinStation Driver output buffer routines
*
*   Copyright Citrix Systems Inc. 1994
*
*   Author: Brad Pedersen  (3/29/94)
*
*   $Log$
*  
*     Rev 1.24   15 Apr 1997 18:17:16   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.23   08 May 1996 15:02:52   jeffm
*  update
*  
*     Rev 1.22   03 Feb 1996 23:03:54   bradp
*  update
*  
*     Rev 1.21   05 Dec 1995 10:35:34   miked
*  update
*  
*     Rev 1.20   27 Sep 1995 13:42:08   bradp
*  update
*  
*     Rev 1.19   26 Sep 1995 14:01:12   bradp
*  update
*  
*     Rev 1.18   20 Jul 1995 15:05:00   bradp
*  update
*  
*     Rev 1.17   03 May 1995 11:46:24   kurtp
*  update
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../inc/wd.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int  STATIC OutBufPoolAlloc( PWD, USHORT, USHORT );
void STATIC OutBufPoolFree( PWD );

int  STATIC WFCAPI OutBufReserve( PWD, USHORT );
int  STATIC WFCAPI OutBufAppend( PWD, LPBYTE, USHORT );
int  STATIC WFCAPI OutBufWrite( PWD );

int  STATIC OutBufAlloc( PWD, POUTBUF * );
void STATIC OutBufFree( PWD, POUTBUF );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/



/*=============================================================================
==   External Functions Used
=============================================================================*/

int PdCall( PWD, USHORT, PVOID );


/*******************************************************************************
 *
 *  OutBufPoolAlloc
 *
 *  Allocate wd buffer pool
 *
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    Count (input)
 *       number of buffers to initialize
 *    Length (input)
 *       size of buffer in bytes
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
OutBufPoolAlloc( PWD pWd, USHORT Count, USHORT Length )
{
    POUTBUF pOutBuf;
    int rc;

    ASSERT( Count > 0, Count );
    ASSERT( Length > 100, Length );

    /*
     *  Return if buffer size and count are not changing
     */
    if ( pWd->OutBufLength == Length && pWd->OutBufCountClient == Count )
        return( CLIENT_STATUS_SUCCESS );

    TRACE(( TC_WD, TT_API3, "OutBufPoolAlloc: count %u, length %u", Count, Length ));

    /*
     *  Free old buffers
     */
    OutBufPoolFree( pWd );

    /*
     *  Save new length and count
     */
    pWd->OutBufCountClient = Count;
    pWd->OutBufLength = Length;
    pWd->OutBufMaxByteCount = pWd->OutBufLength - (pWd->OutBufHeader + pWd->OutBufTrailer);
    pWd->OutBufFreeCount = Count;

    /*
     *  Allocate buffers
     */
    while ( Count-- > 0 ) {

        /*
         *  Allocate buffer header
         */
        pOutBuf = (POUTBUF) malloc( sizeof(OUTBUF) );
        if ( pOutBuf == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto badheader;
        }

        /*
         *  Initialize OUTBUF structure
         */
        memset( pOutBuf, 0, sizeof(OUTBUF) );

        /*
         *  Allocate buffer memory 
         */
        pOutBuf->pMemory = (LPBYTE) malloc( Length );
        if ( pOutBuf->pMemory == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto badbuffer;
        }
#ifdef DEBUG
        memset( pOutBuf->pMemory, 0xDD, Length );
#endif

        /*
         *  Allocate buffer parameter memory
         */
        if ( pWd->OutBufParam > 0 ) {
            pOutBuf->pParam = (PVOID) malloc(pWd->OutBufParam);
            if ( pOutBuf->pParam == NULL ) {
                rc = CLIENT_ERROR_NO_MEMORY;
                goto badparam;
            }
        }

        /*
         *  Initialize OUTBUF parameter structure
         */
        memset( pOutBuf->pParam, 0, pWd->OutBufParam );

        /*
         *  Link to free buffer pool
         */
        pOutBuf->pLink = pWd->pOutBufFree;
        pWd->pOutBufFree = pOutBuf;
    }

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad parameter buffer
     */
badparam:
    free( pOutBuf->pMemory );

    /*
     *  bad buffer
     */
badbuffer:
    free( pOutBuf );

    /*
     *  bad header
     */
badheader:
    OutBufPoolFree( pWd );
    return( rc );
}


/*******************************************************************************
 *
 *  OutBufPoolFree
 *
 *  Free wd buffer pool memory
 *
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC 
OutBufPoolFree( PWD pWd )
{
    POUTBUF pOutBuf;

    OutBufFree( pWd, pWd->pOutBufCurrent );
    pWd->pOutBufCurrent = NULL;

    while ( pOutBuf = pWd->pOutBufFree ) {
        pWd->pOutBufFree = pOutBuf->pLink;

        free( pOutBuf->pMemory );
        if ( pOutBuf->pParam )
            free( pOutBuf->pParam );
        free( pOutBuf );
    }

    pWd->OutBufCountClient = 0;
    pWd->OutBufLength      = 0;
    pWd->OutBufFreeCount   = 0;
}


/*******************************************************************************
 *
 *  OutBufReserve
 *
 *  Make sure we have enough outbufs to satisfy the requested byte count
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    ByteCount (input)
 *       number of bytes of buffer space to allocate
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
OutBufReserve( PWD pWd, USHORT ByteCount )
{
    PDQUERYINFORMATION PdQueryInfo;
    USHORT OutBufCount;
    int rc;

    ASSERT( pWd->pOutBufCurrent == NULL, 0 );

    /*
     *  Query protocol drivers to see if it's ok to allocate a buffer (pdreli)
     */
    PdQueryInfo.PdInformationClass = PdOutBufReserve;
    if ( rc = PdCall( pWd, PD__QUERYINFORMATION, &PdQueryInfo ) )
        return( rc );

    /*
     *  If the framing protocol driver is loaded double the byte count.
     *  The framing protocol driver must escape all 'special' characters
     *  in the data, causing the data length to grow (upto 2x).
     */
    if ( pWd->fOutBufFrame )
        ByteCount *= 2;

    /* 
     *  Calculate number of outbufs needed for the specified data
     *  -- round up 
     */
    OutBufCount = (ByteCount + pWd->OutBufMaxByteCount - 1) / pWd->OutBufMaxByteCount;

    /*
     *  If any protocol driver allocates a new outbuf and copies the data
     *  add 1 to the count.  The protocol driver must free the original
     *  outbuf immediately after copying it.
     */
    if ( pWd->fOutBufCopy )
        OutBufCount++;

    ASSERT( OutBufCount <= pWd->OutBufCountClient, 0 );

    TRACE(( TC_WD, TT_API3, "OutBufReserve: %u, (%u/%u)", 
            ByteCount, OutBufCount, pWd->OutBufFreeCount ));

    /*
     *  Check if enough free outbufs exist
     */
    return( OutBufCount <= pWd->OutBufFreeCount ? CLIENT_STATUS_SUCCESS : CLIENT_ERROR_NO_OUTBUF );
}


/*******************************************************************************
 *
 *  OutBufAppend
 *
 *  Append data to current output buffer. 
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    pData (input)
 *       pointer to data to append
 *    ByteCount (input)
 *       length of data to append
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
OutBufAppend( PWD pWd, LPBYTE pData, USHORT ByteCount )
{
    PDWRITE WriteData;
    POUTBUF pOutBuf;
    USHORT Count;
    int rc;

    while ( ByteCount > 0 ) {

        /*
         *  Allocate one outbuf and make it the current outbuf
         */
        if ( (pOutBuf = pWd->pOutBufCurrent) == NULL ) {
            if ( rc = OutBufAlloc( pWd, &pOutBuf ) ) {
                return( rc );
            }
            pWd->pOutBufCurrent = pOutBuf;
        }
    
        Count = pOutBuf->MaxByteCount - pOutBuf->ByteCount;
        ASSERT( (int)Count >= 0, 0 );
        if ( ByteCount < Count )
            Count = ByteCount;
 
        memcpy( pOutBuf->pBuffer + pOutBuf->ByteCount, pData, Count );
        pOutBuf->ByteCount += Count;
        pData += Count;
        ByteCount -= Count;
        ASSERT( (int)ByteCount >= 0, 0 );
#ifdef DEBUG
#ifdef DOS
        (void) _msize( pOutBuf->pMemory );
#endif
#endif
 
        if ( pOutBuf->ByteCount == pOutBuf->MaxByteCount ) {

            pWd->pOutBufCurrent = NULL;

            /* write outbuf */
            WriteData.pOutBuf = pOutBuf;
            if ( rc = PdCall( pWd, PD__WRITE, &WriteData ) )
                return( rc );
        }
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  OutBufWrite
 *
 *  Write last partial outbuf to protocol driver and return any unused outbufs
 *
 *  NOTE:  the PD must free the output buffer
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
OutBufWrite( PWD pWd )
{
    PDWRITE WriteData;
    POUTBUF pOutBuf;

    /*
     *  Get pointer to current buffer structure 
     */
    if ( (pOutBuf = pWd->pOutBufCurrent) == NULL ) 
        return( CLIENT_STATUS_SUCCESS );

    pWd->pOutBufCurrent = NULL;

    /*
     *  If byte count is zero return outbuf
     */
    if ( pOutBuf->ByteCount == 0 ) {
        OutBufFree( pWd, pOutBuf );
        return( CLIENT_STATUS_SUCCESS );
    }

    TRACE(( TC_WD, TT_API3, "OutBufWrite: %lx, mem %lx, param %lx, bc %u", 
            pOutBuf, pOutBuf->pMemory, pOutBuf->pParam, pOutBuf->ByteCount ));

    /* 
     *  Write data buffer to pd
     */
    WriteData.pOutBuf = pOutBuf;
    return( PdCall( pWd, PD__WRITE, &WriteData ) );
}


/*******************************************************************************
 *
 *  OutBufAlloc
 *
 *  Allocate one outbuf
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    ppOutBuf (output)
 *       address to return pointer to outbuf
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
OutBufAlloc( PWD pWd, POUTBUF * ppOutBuf )
{
    POUTBUF pOutBuf;

    /*
     *  If we are out of buffers - return error
     */
    if ( (pOutBuf = pWd->pOutBufFree) == NULL ) {
        TRACE(( TC_WD, TT_API3, "OutBufAlloc: none available" ));
        ASSERT( pWd->OutBufFreeCount == 0, pWd->OutBufFreeCount );
        return( CLIENT_ERROR_NO_OUTBUF );
    }

    /*
     *  Unlink buffer from head of buffer pool
     */
    pWd->pOutBufFree = pOutBuf->pLink;
    pWd->OutBufFreeCount--;

    /*
     *  Initialize output buffer
     */
    pOutBuf->pLink        = NULL;
    pOutBuf->MaxByteCount = pWd->OutBufMaxByteCount;
    pOutBuf->pBuffer      = pOutBuf->pMemory + pWd->OutBufHeader;
    pOutBuf->ByteCount    = 0;
    pOutBuf->fControl     = FALSE;      // not a control buffer (ack/nak)
    pOutBuf->fRetransmit  = FALSE;      // not a retransmit
    pOutBuf->fCompress    = TRUE;       // compress data
    pOutBuf->Sequence     = 0;
    pOutBuf->Fragment     = 0;
    pOutBuf->pSaveBuffer  = NULL;
    pOutBuf->SaveByteCount = 0;

    /*
     *  Return buffer to caller
     */
    *ppOutBuf = pOutBuf;

    TRACE(( TC_WD, TT_API3, "OutBufAlloc: %lx, mem %lx, param %lx", 
            pOutBuf, pOutBuf->pMemory, pOutBuf->pParam ));
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  OutBufFree
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC 
OutBufFree( PWD pWd, POUTBUF pOutBuf )
{

    if ( pOutBuf ) {
#ifdef DEBUG
        ASSERT( pOutBuf->pLink == NULL, 0 );
#ifdef DOS
        (void) _msize( pOutBuf->pMemory );
#endif
        memset( pOutBuf->pMemory, 0xDD, pWd->OutBufLength );
#endif
        pOutBuf->pLink = pWd->pOutBufFree;
        pWd->pOutBufFree = pOutBuf;
        pWd->OutBufFreeCount++;

        TRACE(( TC_WD, TT_API3, "OutBufFree: %lx, mem %lx, param %lx", 
                pOutBuf, pOutBuf->pMemory, pOutBuf->pParam ));
    }
}


