
/*************************************************************************
*
*   mouse.c
*  
*   ICA 3.0 WinStation Driver - mouse packets
*  
*    PACKET_SETMOU_POSITION  4  set mouse position (x,y)
*  
*  
*   Copyright 1994, Citrix Systems Inc.
*  
*   Author: Brad Pedersen (4/9/94)
*  
*   mouse.c,v
*   Revision 1.1  1998/01/12 11:36:23  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.18   15 Apr 1997 18:17:58   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.17   26 Sep 1995 10:52:58   bradp
*  update
*  
*     Rev 1.16   28 Jul 1995 18:30:12   kurtp
*  update
*  
*     Rev 1.15   27 Jul 1995 19:51:46   kurtp
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
#include "../../../inc/vdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaSetMousePosition( PWD, LPBYTE, USHORT );  
void IcaSetMouseAttribute( PWD, LPBYTE, USHORT );  
int MouWrite( PWD, PMOUSEDATA, USHORT );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int VdCall( PWD, USHORT, USHORT, PVOID );
int AppendICAPacket( PWD, USHORT, LPBYTE, USHORT );
int WFCAPI OutBufReserve( PWD, USHORT );
int WFCAPI OutBufAppend( PWD, LPBYTE, USHORT );
int WFCAPI OutBufWrite( PWD );


/*******************************************************************************
 *
 *  IcaSetMousePosition 
 *
 *  PACKET_SETMOU_POSITION
 *  P1 - X coordinate (low byte)
 *  P2 - X coordinate (high byte)
 *  P3 - Y coordinate (low byte)
 *  P4 - Y coordinate (high byte)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetMousePosition( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    VDSETINFORMATION Info;
    MOUPOSITION Position;
    PWDICA pIca;
    USHORT X;
    USHORT Y;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    X = (USHORT) *((PUSHORT)pInputBuffer);
    pInputBuffer += 2;

    Y = (USHORT) *((PUSHORT)pInputBuffer);

#if defined(DOS) || defined(RISCOS)
    rc = MousePosition( X, Y );
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
#endif

    /*
     *  Notify Thinwire so it can update it's mouse pointer
     */
    if ( pIca->fGraphics ) {
        Position.X = X;
        Position.Y = Y;
        Info.VdInformationClass  = VdMousePosition;
        Info.pVdInformation      = &Position;
        Info.VdInformationLength = sizeof(MOUPOSITION);
        rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    }

    TRACE(( TC_WD, TT_API1, "SETMOU_POSITION, x=%u, y=%u, rc=%u", X, Y, rc ));
}


/*******************************************************************************
 *
 *  IcaSetMouseAttribute 
 *
 *  PACKET_SETMOU_ATTR
 *  P1 - attribute  (0=pointer off, 1=pointer on)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaSetMouseAttribute( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
#if defined( DOS ) || defined (RISCOS)
    int    rc;
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    //  no change?
    if ( pIca->fMouseVisible == (USHORT) (BOOL)*pInputBuffer )
        return;

    // save fact that mouse is visible or not
    pIca->fMouseVisible = (USHORT) (BOOL)*pInputBuffer;

    rc = MouseShowPointer( (BOOL)*pInputBuffer );
    ASSERT( rc == 0, rc );
#endif

    TRACE(( TC_WD, TT_API1, "SETMOU_ATTR: %u", *pInputBuffer ));
}


/*******************************************************************************
 *
 *  MouWrite
 *
 *  send mouse data to host
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pMouData (input)
 *       available mouse packets
 *    MouCount (input)
 *       number of available mouse packets
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_STATUS_NO_DATA - no mouse data to send
 *
 ******************************************************************************/

int
MouWrite( PWD pWd, PMOUSEDATA pMouData, USHORT MouCount )
{
    PWDICA pIca;
    USHORT ByteCount;
    BYTE str[2];
    USHORT OutBufMouCount;
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  Allocate output buffer
     */
    ByteCount = (USHORT) (MouCount * sizeof(MOUSEDATA));

    /*
     *  How many keys
     */
    OutBufMouCount = pIca->cbMouBuffer + ByteCount;

    /*
     *  Check for nothing to send
     */
    if ( !OutBufMouCount ) {
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Allocate output buffer
     */
    if ( rc = OutBufReserve( pWd, (USHORT) (OutBufMouCount + 2) ) ) {

        /*
         *  If no buffer available then write the mouse data into 
         *  a connection local buffer for processing when
         *  buffers become available.
         */ 
        if ( rc == CLIENT_ERROR_NO_OUTBUF ) {

            /*
             *  Check for something to append
             */
            if ( !ByteCount ) {
                return( CLIENT_STATUS_SUCCESS );
            }

            /*
             *  check for buffer space
             */
            if ( pIca->pMouBuffer == NULL ) {

                /*
                 *  Initial buffer allocation
                 */
                pIca->cbMouBuffer     = 0;
                pIca->cbMouBufferSize = MOU_BUFFER_START_SIZE;
                pIca->pMouBuffer      = (PCHAR) malloc( pIca->cbMouBufferSize );

                /*
                 *  Trap out of memory
                 */
                if ( pIca->pMouBuffer == NULL ) 
                    return( CLIENT_ERROR_NO_MEMORY );
            }

            /*
             *  Need to grow buffer
             */
            if ( pIca->cbMouBufferSize < (pIca->cbMouBuffer + ByteCount) ) {

                PCHAR pTemp;

                /*
                 *  Buffer realloc (grow)
                 */
                pTemp = (PCHAR) malloc( (pIca->cbMouBufferSize + MOU_BUFFER_START_SIZE + ByteCount) );

                /*
                 *  Trap outof memory 
                 */
                if ( pTemp == NULL ) 
                    return( CLIENT_ERROR_NO_MEMORY );

                memcpy( pTemp, pIca->pMouBuffer, pIca->cbMouBufferSize );
                pIca->cbMouBufferSize += (MOU_BUFFER_START_SIZE + ByteCount);
                free( pIca->pMouBuffer );
                pIca->pMouBuffer = pTemp;
            }

            /*
             *  Append mouse data to ica local buffer
             */
            memcpy( &pIca->pMouBuffer[pIca->cbMouBuffer], pMouData, ByteCount );

            /*
             *  Adjust current buffer length
             */
            pIca->cbMouBuffer += ByteCount;
        }

        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Append mouse packet header
     */
    if ( (OutBufMouCount / sizeof(MOUSEDATA)) == 1 ) {
        rc = AppendICAPacket( pWd, PACKET_MOUSE0, NULL, 0 );
    } else if ( OutBufMouCount < 256 ) {
        str[0] = (BYTE) OutBufMouCount;
        rc = AppendICAPacket( pWd, PACKET_MOUSE1, str, 1 );
    } else {
        str[0] = LSB(OutBufMouCount);
        str[1] = MSB(OutBufMouCount);
        rc = AppendICAPacket( pWd, PACKET_MOUSE2, str, 2 );
    }
    if ( rc )
        goto badappend;

    /*
     *  Append buffered mouse data to output buffer
     */
    if ( pIca->pMouBuffer && pIca->cbMouBuffer ) {

        if ( rc = OutBufAppend( pWd, (LPBYTE) pIca->pMouBuffer, pIca->cbMouBuffer ) ) 
            goto badappend;

        pIca->cbMouBuffer = 0;
    }

    /*
     *  Append new mouse data to output buffer
     */
    if ( pMouData && MouCount ) {

        if ( rc = OutBufAppend( pWd, (LPBYTE) pMouData, ByteCount ) )
            goto badappend;
    }

    /*
     *  Don't compress mouse data
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
    ASSERT( rc==CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}


