
/*************************************************************************
*
*   output.c
*
*   ICA 3.0 WinStation Driver - output routines
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Brad Pedersen (4/8/94)
*
*   $Log$
*  
*     Rev 1.33   15 Apr 1997 18:18:00   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.32   26 Sep 1995 10:53:02   bradp
*  update
*  
*     Rev 1.31   21 Jul 1995 19:16:26   bradp
*  update
*  
*     Rev 1.30   21 Jul 1995 16:14:18   kurtp
*  update
*  
*     Rev 1.29   03 May 1995 11:46:14   kurtp
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
#include "citrix/ica30.h"

#ifdef DOS
#include "../../../inc/dos.h"
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
==   External Functions Defined
=============================================================================*/

int AppendICAPacket( PWD, USHORT, LPBYTE, USHORT );
int IcaWrite( PWD, USHORT, LPBYTE, USHORT );
int IcaDetectWrite( PWD );
int KbdWrite( PWD, PUCHAR, USHORT );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int WFCAPI OutBufReserve( PWD, USHORT );
int WFCAPI OutBufAppend( PWD, LPBYTE, USHORT );
int WFCAPI OutBufWrite( PWD );


/*******************************************************************************
 *
 *  AppendICAPacket
 *
 *  append an ica packet
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to wd data structure
 *    Type (input)
 *       ica packet type
 *    pData (input)
 *       pointer to ica packet data
 *    Length (input)
 *       length of ica packet data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
AppendICAPacket( PWD pWd, USHORT Type, LPBYTE pData, USHORT Length )
{
    int rc;
    BYTE Ch;

    /*
     *  Append ICA packet header
     */
    Ch = (BYTE) Type;
    if ( rc = OutBufAppend( pWd, &Ch, 1 ) )
        return( rc );

    /*
     *  Append ICA packet data
     */
    return( OutBufAppend( pWd, pData, Length ) );
}


/*******************************************************************************
 *
 *  KbdWrite
 *
 *  send keyboard data to host
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pKbdCodes (input)
 *       character or scan codes to send
 *    cKbdCodes (input)
 *       number of codes
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_         - other error
 *
 ******************************************************************************/

int
KbdWrite( PWD pWd, PUCHAR pKbdCodes, USHORT cKbdCodes )
{
    BYTE str[1];
    PWDICA pIca;
    USHORT OutBufKbdCount;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  How many keys
     */
    OutBufKbdCount = cKbdCodes + pIca->cbKbdBuffer;
    ASSERT( OutBufKbdCount < 256, OutBufKbdCount );

    /*
     *  Check for nothing to send
     */
    if ( !OutBufKbdCount ) {
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Allocate output buffer
     */
    if ( rc = OutBufReserve( pWd, (USHORT) (OutBufKbdCount + 2) ) ) {

        /*
         *  If no buffer available then write the keys into 
         *  a connection local buffer for processing when
         *  buffers become available.
         */ 
        if ( rc == CLIENT_ERROR_NO_OUTBUF ) {

            /*
             *  Check for something to append
             */
            if ( !cKbdCodes ) {
                return( CLIENT_STATUS_SUCCESS );
            }

            /*
             *  check for buffer space
             */
            if ( pIca->pKbdBuffer == NULL ) {

                /*
                 *  Initial buffer allocation
                 */
                pIca->cbKbdBuffer     = 0;
                pIca->cbKbdBufferSize = KBD_BUFFER_START_SIZE;
                pIca->pKbdBuffer      = (PCHAR) malloc( pIca->cbKbdBufferSize );

                /*
                 *  Trap out of memory
                 */
                if ( pIca->pKbdBuffer == NULL ) 
                    return( CLIENT_ERROR_NO_MEMORY );
            }

            /*
             *  Need to grow buffer
             */
            if ( pIca->cbKbdBufferSize < (pIca->cbKbdBuffer + cKbdCodes) ) {

                PCHAR pTemp;

                /*
                 *  Buffer realloc (grow)
                 */
                pTemp = (PCHAR) malloc( (pIca->cbKbdBufferSize + KBD_BUFFER_START_SIZE + cKbdCodes) );

                /*
                 *  Trap outof memory 
                 */
                if ( pTemp == NULL ) 
                    return( CLIENT_ERROR_NO_MEMORY );

                memcpy( pTemp, pIca->pKbdBuffer, pIca->cbKbdBufferSize );
                pIca->cbKbdBufferSize += (KBD_BUFFER_START_SIZE + cKbdCodes);
                free( pIca->pKbdBuffer );
                pIca->pKbdBuffer = pTemp;
            }

            /*
             *  Append keyboard data to ica local buffer
             */
            memcpy( &pIca->pKbdBuffer[pIca->cbKbdBuffer], pKbdCodes, cKbdCodes );

            /*
             *  Adjust current buffer length
             */
            pIca->cbKbdBuffer += cKbdCodes;
        }

        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Append keyboard packet header only in Kbd_Scan Mode
     */
    if ( pIca->KbdMode == Kbd_Scan ) {

        if ( OutBufKbdCount == 1 ) {
            rc = AppendICAPacket( pWd, PACKET_KEYBOARD0, NULL, 0 );
        } else {
            str[0] = (BYTE) OutBufKbdCount;
            rc = AppendICAPacket( pWd, PACKET_KEYBOARD1, str, 1 );
        }

        if ( rc )
            goto badappend;
    }

    /*
     *  Append buffered keyboard data to output buffer
     */
    if ( pIca->pKbdBuffer && pIca->cbKbdBuffer ) {

        if ( rc = OutBufAppend( pWd, (LPBYTE) pIca->pKbdBuffer, pIca->cbKbdBuffer ) ) 
            goto badappend;

        pIca->cbKbdBuffer = 0;
    }

    /*
     *  Append new keyboard data to output buffer
     */
    if ( pKbdCodes && cKbdCodes ) {

        if ( rc = OutBufAppend( pWd, (LPBYTE) pKbdCodes, cKbdCodes ) ) 
            goto badappend;
    }

    /*
     *  Don't compress keyboard data
     */
    if ( pWd->pOutBufCurrent ) 
        pWd->pOutBufCurrent->fCompress = FALSE;

    /*
     *  Write outbuf to protocol driver
     *  -- this frees the allocated outbufs
     */
    if ( rc = OutBufWrite( pWd ) )
        goto badwrite;

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad write
     *  bad data append
     */
badwrite:
badappend:
    if ( pWd->pOutBufCurrent )
        pWd->pOutBufCurrent->ByteCount = 0;
    (void) OutBufWrite( pWd );              // return outbufs to free pool
    ASSERT( rc==CLIENT_STATUS_SUCCESS || 
            rc==CLIENT_STATUS_HOTKEY1 || rc==CLIENT_STATUS_HOTKEY2 || rc==CLIENT_STATUS_HOTKEY3, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  IcaWrite
 *
 *  send ICA packet to host
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    Type (input)
 *       ica packet type
 *    pData (input)
 *       pointer to ica packet data
 *    Length (input)
 *       length of ica packet data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS   - no error
 *    CLIENT_ERROR_NO_OUTBUF - no output buffers (command not sent)
 *
 ******************************************************************************/

int
IcaWrite( PWD pWd, USHORT Type, LPBYTE pData, USHORT Length )
{
    int rc;

    /*
     *  Allocate output buffer
     */
    if ( rc = OutBufReserve( pWd, (USHORT) (Length + 3) ) ) 
        goto badreserve;

    /*
     *  Append ICA header 
     */
    if ( rc = AppendICAPacket( pWd, Type, pData, Length ) )
        goto badappend;

    /*
     *  Write outbuf to protocol driver
     *  -- this frees the allocated outbufs
     */
    if ( rc = OutBufWrite( pWd ) )
        goto badwrite;

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad write
     *  bad data append
     */
badwrite:
badappend:
    if ( pWd->pOutBufCurrent )
        pWd->pOutBufCurrent->ByteCount = 0;
    (void) OutBufWrite( pWd );              // return outbufs to free pool

    /*
     *  outbuf reserve failed
     */
badreserve:
    ASSERT( rc==CLIENT_STATUS_SUCCESS || rc==CLIENT_ERROR_NO_OUTBUF, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  IcaDetectWrite
 *
 *  send the "ica detection" packet to the host
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS   - no error
 *
 ******************************************************************************/

int
IcaDetectWrite( PWD pWd )
{
    POUTBUF pOutBuf;
    int rc;

    /*
     *  Allocate output buffer
     */
    rc = OutBufReserve( pWd, sizeof(ICA_DETECT_STRING)-1 );

    /*
     *  Append and write ica detect string
     */
    if ( rc == CLIENT_STATUS_SUCCESS ) {
        (void) OutBufAppend( pWd, ICA_DETECT_STRING, sizeof(ICA_DETECT_STRING)-1 );
        if ( pOutBuf = pWd->pOutBufCurrent ) {
            pOutBuf->fRetransmit = TRUE;
            pOutBuf->fControl = TRUE;
        }
        (void) OutBufWrite( pWd );
    }

    return( CLIENT_STATUS_SUCCESS );
}

