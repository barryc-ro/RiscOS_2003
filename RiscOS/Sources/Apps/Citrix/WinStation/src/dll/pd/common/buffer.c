
/*************************************************************************
*
* buffer.c
*
* Output buffer routines
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (4/3/94)
*
* $Log$
*  
*     Rev 1.16   15 Apr 1997 16:51:34   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.16   21 Mar 1997 16:06:56   bradp
*  update
*  
*     Rev 1.15   08 May 1996 16:40:22   jeffm
*  update
*  
*     Rev 1.14   05 Dec 1995 10:17:32   miked
*  update
*  
*     Rev 1.13   26 Sep 1995 14:00:52   bradp
*  update
*  
*     Rev 1.12   20 Jul 1995 15:02:48   bradp
*  update
*  
*     Rev 1.11   02 Jun 1995 09:47:20   bradp
*  update
*  
*     Rev 1.10   01 Jun 1995 22:28:02   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.9   03 May 1995 10:07:58   butchd
*  CLIB now standard
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
#else
#ifdef DEBUG
#include <malloc.h>
#endif
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int  STATIC OutBufAlloc( PPD, POUTBUF, POUTBUF * );
void STATIC OutBufError( PPD, POUTBUF );
void STATIC OutBufFree( PPD, POUTBUF );
int  STATIC OutBufAppend( PPD, POUTBUF, LPBYTE, USHORT );
int  STATIC OutBufWrite( PPD );
int  STATIC SetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions Used
=============================================================================*/

int STATIC PdNext( PPD, USHORT, PVOID );


/*******************************************************************************
 *
 *  OutBufAlloc
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *    pOutBufOrig (input)
 *       pointer to original outbuf (or null)
 *    ppOutBuf (output)
 *       address to return pointer to buffer structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
OutBufAlloc( PPD pPd, POUTBUF pOutBufOrig, POUTBUF * ppOutBuf )
{
    POUTBUF pOutBufNew;
    int rc;

    rc = (pPd->pOutBufAllocProc)( pPd->pWdData, &pOutBufNew );

    /*
     *  Alloc was successful
     */
    if ( rc == CLIENT_STATUS_SUCCESS ) {

        pOutBufNew->pBuffer -= pPd->OutBufHeader;
        ASSERT( pOutBufNew->pBuffer >= pOutBufNew->pMemory, pPd->OutBufHeader );
        pOutBufNew->MaxByteCount += (pPd->OutBufHeader + pPd->OutBufTrailer);

        /* copy inherited fields */
        if ( pOutBufOrig ) {
            pOutBufNew->StartTime   = pOutBufOrig->StartTime;
            pOutBufNew->Sequence    = pOutBufOrig->Sequence;
            pOutBufNew->Fragment    = pOutBufOrig->Fragment++; /* note '++' !! */
            pOutBufNew->fControl    = pOutBufOrig->fControl;
            pOutBufNew->fRetransmit = pOutBufOrig->fRetransmit;
            pOutBufNew->fCompress   = pOutBufOrig->fCompress;
        }
    }

    *ppOutBuf = pOutBufNew;
    return( rc );
}


/*******************************************************************************
 *
 *  OutBufError  (write completed with an error)
 *
 *  return outbuf to free pool in wd
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC 
OutBufError( PPD pPd, POUTBUF pOutBuf )
{
    ASSERT( pOutBuf->pLink == NULL, 0 );

    (pPd->pOutBufErrorProc)( pPd->pWdData, pOutBuf );
}


/*******************************************************************************
 *
 *  OutBufFree
 *
 *  send outbuf up chain to next pd or wd
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC 
OutBufFree( PPD pPd, POUTBUF pOutBuf )
{
    ASSERT( pOutBuf->pLink == NULL, 0 );

    (pPd->pOutBufFreeProc)( pPd->pWdData, pOutBuf );
}


/*******************************************************************************
 *
 *  OutBufAppend
 *
 *  Append data to current output buffer.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *    pOutBufOrig (input)
 *       pointer to original outbuf (or null)
 *    pData (input)
 *       pointer to data to append
 *    ByteCount (input)
 *       length of data to append
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
OutBufAppend( PPD pPd, POUTBUF pOutBufOrig, LPBYTE pData, USHORT ByteCount )
{
    PDWRITE WriteData;
    POUTBUF pOutBuf;
    USHORT Count;
    int rc;
    int rc2;

    while ( ByteCount > 0 ) {

        /*
         *  Allocate one outbuf and make it the current outbuf
         */
        if ( (pOutBuf = pPd->pOutBufCurrent) == NULL ) {
            if ( rc = OutBufAlloc( pPd, pOutBufOrig, &pOutBuf ) ) {
                /* check if previous write has completed */
                WriteData.pOutBuf = NULL;
                if ( rc2 = PdNext( pPd, PD$WRITE, &WriteData ) )
                    return( rc2 );
                return( rc );
            }
            pPd->pOutBufCurrent = pOutBuf;
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
        (void) _msize( pOutBuf->pMemory );
#endif

        if ( pOutBuf->ByteCount == pOutBuf->MaxByteCount ) {

            pPd->pOutBufCurrent = NULL;

            /* write outbuf */
            WriteData.pOutBuf = pOutBuf;
            if ( rc = PdNext( pPd, PD$WRITE, &WriteData ) )
                return( rc );
        }
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  OutBufWrite
 *
 *  Write last partial outbuf to protocol driver
 *
 *  NOTE:  the PD must free the output buffer
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
OutBufWrite( PPD pPd )
{
    PDWRITE WriteData;
    POUTBUF pOutBuf;

    /*
     *  Get pointer to current buffer structure
     */
    if ( (pOutBuf = pPd->pOutBufCurrent) == NULL )
        return( CLIENT_STATUS_SUCCESS );

    pPd->pOutBufCurrent = NULL;

    ASSERT( pOutBuf->ByteCount > 0, 0 );

    TRACE(( TC_PD, TT_API3, "OutBufWrite: %lx, mem %lx, param %lx, bc %u",
            pOutBuf, pOutBuf->pMemory, pOutBuf->pParam, pOutBuf->ByteCount ));

    /*
     *  Write data buffer to pd
     */
    WriteData.pOutBuf = pOutBuf;
    return( PdNext( pPd, PD$WRITE, &WriteData ) );
}


/*******************************************************************************
 *
 *  SetInfo
 *
 *  pass set info up to wd
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int STATIC 
SetInfo( PPD pPd, SETINFOCLASS SetInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{

    TRACE(( TC_PD, TT_API1, "OutSetInfo: SetInfoClass %u", SetInfoClass ));
    return( (pPd->pSetInfoProc)( pPd->pWdData, SetInfoClass, pBuffer, cbBuffer ) );
}


/*******************************************************************************
 *
 *  QueryInfo
 *
 *  query info from wd
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to wd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int STATIC 
QueryInfo( PPD pPd, QUERYINFOCLASS QueryInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{

    TRACE(( TC_PD, TT_API1, "OutQueryInfo: QueryInfoClass %u", QueryInfoClass ));
    return( (pPd->pQueryInfoProc)( pPd->pWdData, QueryInfoClass, pBuffer, cbBuffer ) );
}
