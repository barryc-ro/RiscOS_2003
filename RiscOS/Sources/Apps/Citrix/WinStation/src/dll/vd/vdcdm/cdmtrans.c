/*************************************************************************
*
* cdmtrans.c
*
* TRANSPORT interface for the CDM Server.
*
* This module implements a TRANSPORT for the Client Drive Mapping server
* over the CITRIX ICA protcol within the WinView client.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 03/11/94
*
* $Log$
*  
*     Rev 1.13   09 Jul 1997 16:09:26   davidp
*  Added include for ica30.h because of Hydrix surgery
*  
*     Rev 1.12   15 Apr 1997 18:02:42   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   21 Mar 1997 16:09:00   bradp
*  update
*  
*     Rev 1.11   08 May 1996 14:04:00   jeffm
*  update
*  
*     Rev 1.10   14 Sep 1995 11:24:12   JohnR
*  update
*
*     Rev 1.9   24 Jun 1995 18:31:20   richa
*
*     Rev 1.8   17 Apr 1995 12:45:50   marcb
*  update
*
*************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"

#include "../../../inc/clib.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/vd.h"
#include "../../wd/inc/wd.h"
#include "citrix/cdmwire.h" // Wire protocol definitions
#include "vdcdm.h"
//#include "cdmserv.h"
//#include "cdmtrans.h"

/*
 * Global data to this module
 */

/*
 * CdmTransport Main Data structures
 * Under single threaded DOS, we only have one transport
 */
static CDMTRANSPORT Transport = { CDMTRANSPORT_UNINITIALIZED, 0, 0, 0 };

/*
 * Forward references
 */
STATIC USHORT CdmICAWrite( PUCHAR, USHORT );
STATIC int RingBufCreate( USHORT );
STATIC void RingBufDelete( VOID );


/*****************************************************************************
 *
 *  CdmTransportSendData
 *
 *   This function transmits data out onto a transport for the Client
 *   Drive Mapping Protocol.
 *
 * ENTRY:
 *   pTransport (input)
 *     The transport handle passed in from the client
 *
 *   pBuf (input)
 *     Pointer to buffer of data to send
 *
 *   Size (input)
 *     Size of the data to be sent from buffer
 *
 *   Final (input)
 *     Flag that marks the end of a sequence and the data should be
 *     sent on the wire now as a complete packet.
 *
 *     If false, the data is put into a buffer for future sending, and
 *     not sent. If the buffer overflows, this routine will flush the unsent
 *     data and return an error to the caller. The buffer size promise we
 *     make to the caller can be retrieved  from CdmTransportMaxSize ()
 *     and is valid for the duration of the connection.
 *
 *  This routine returns an error to the caller if the requested data
 *  can not be sent as one complete network PDU. This includes any data
 *  written into the buffer since the last Final == TRUE call.
 *
 *  This routine will block during the transmitt if the underlying
 *  transport provider does not have enough buffer space. All callers
 *  of this function should have proper thread/process context and not
 *  be holding any critical sections that need to be released before a
 *  (potentially) long term block.
 *
 * EXIT:
 *   0 - no error
 *
 ****************************************************************************/

int STATIC CdmTransportSendData(PVOID  pTransport,
                         PCHAR  pBuf,
                         USHORT Size,
                         USHORT Final )
{
    int ret;
    PCHAR ptr;
    PCDMTRANSPORT pt = (PCDMTRANSPORT)pTransport;


    TRACE(( TC_CDM, TT_API3, "CdmTransportSendData, Size %u, Final %d", Size, Final ));

    /*
     * Its an error to attempt a write on a non-connected transport.
     */
    if( pt->State != CDMTRANSPORT_CONNECTED ) {
         ASSERT( FALSE, 0 );
         TRACE(( TC_CDM, TT_API3, "CdmTransportSendData Not Connected!"));
         return( CDMTRANSPORT_ERROR_NOCONNECTION );
    }

    /*
     * Make sure we will not overflow the buffer
     */
    if( (pt->BufCount + Size) > pt->BufSize ) {

        TRACE(( TC_CDM, TT_ERROR, "VDCDM: Transport buffer overflow, %u+%u > %u",
                 pt->BufCount, Size, pt->BufSize ));
        /*
         * We toss any unsent data on an overflow. We either send
         * a complete, correct packet, or not at all.
         */
        pt->BufCount = 0;
        return( CDMTRANSPORT_ERROR_OVERFLOW );
    }

    /*
     * Copy the data into the transport buffer
     */
    ptr = pt->Buf;
    ptr += pt->BufCount;
    pt->BufCount += Size;
    memcpy( ptr, pBuf, Size );

    /*
     * If the Final flag == FALSE, then we will hold the data in
     * the buffer until a future call with Final == TRUE.
     * The user can not write more than pt->BufSize data until setting
     * Final = TRUE.
     */
    if( Final == FALSE ) {

        TRACE(( TC_CDM, TT_API3, "CdmTransportSendData, delay send" ));

        return( 0 );
    }
    else {
        /*
         * Send the data out on the pipe with CdmICAWrite()
         */

        // USHORT CdmICAWrite( (PCHAR)PUCHAR, USHORT );
        ret = CdmICAWrite( pt->Buf, (USHORT)pt->BufCount );
        if( ret == 0 ) {
            TRACE(( TC_CDM, TT_API3, "CdmTransportSendData CdmIcaWrite Success, Size %u", pt->BufCount ));
            pt->BufCount = 0;
            return( 0 );
        }

        /*
         * Else this has been a general error such as a disconnect.
         * Toss the data and return an error.
         */
        TRACE(( TC_CDM, TT_ERROR, "CdmTransportSendData, %u", ret ));
        pt->BufCount = 0;
        return( CDMTRANSPORT_ERROR );
    }
    return( 0 );
}


/*****************************************************************************
 *
 *  CdmTransportMaxSize
 *
 *   This routine returns the maximum PDU (Protocol Data Unit) size that
 *   can be sent by the transport as one packet.
 *
 *   (IE: A read will return the whole write, not fragmented, without any
 *        other following packets)
 *
 *   This routine assumes that both directions of the transport connection
 *   has been set to the same buffer size.
 *
 *   This also assumes that the maximum size remains for the duration
 *   of the connection, without ever getting smaller.
 *
 * ENTRY:
 *   pTransport (input)
 *     Handle to the transport
 *
 * EXIT:
 *   size of the maximum send size. On errors, return 0.
 *
 ****************************************************************************/

USHORT STATIC 
CdmTransportMaxSize( PVOID pTransport )
{
    PCDMTRANSPORT pt = (PCDMTRANSPORT)pTransport;

    ASSERT( pt != NULL, 0 );
    ASSERT( pt->State == CDMTRANSPORT_CONNECTED, 0 );

    return( pt->BufSize );
}


/*****************************************************************************
 *
 *  CdmTransportReceiveData
 *
 *   This function is called with data that arrived for this transport.
 *   The data will be dispatched to the CDM server's main dispatch function
 *   CdmWireDataReceive().
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request is for
 *
 *   pBuf (input)
 *     Buffer containing data from requesting client
 *
 *   Size (input)
 *     The amount of data in the buffer.
 *
 * EXIT:
 *   This function returns the amount of data 'accepted'. It should
 *   always accept all the data, but this allows an error to be signaled
 *   by returning an amount less than the Size argument.
 *
 ****************************************************************************/

USHORT STATIC 
CdmTransportReceiveData( PVOID  pTransport, PCHAR  pBuf, USHORT  Size )
{
    return( CdmWireDataReceive( pTransport, pBuf, Size ) );
}


/*****************************************************************************
 *
 *  SetupTransport
 *
 *   Setup the transport connection of the given name, returning
 *   a PVOID handle to it. It is ready for reading requests and setting
 *   replies at this point.
 *
 * ENTRY:
 *
 *   TotalByteCount (input)
 *     The total number of bytes that can be sent to the host at one time.
 *     This represents the "send window".
 *     This value has to also be known to the NT and/or OS/2 host so that
 *     file requests to not exceed this value until acknowledged.
 *
 *   MaxByteCount (input)
 *     The maximum PDU size to be used by this transport and the CDM
 *     protocol. This is supplied from the real transport provider
 *     (NetBios, SPX, ASYNC, through ICA)
 *
 *   pCdmTransportHandle (output)
 *     address to return handle to transport
 *
 * EXIT:
 *   NULL means an error setting up a transport connection.
 *   If successfull, a PVOID handle is returned to the transport.
 *
 ****************************************************************************/

int STATIC 
SetupTransport( USHORT TotalByteCount,
                USHORT MaxByteCount,
                PVOID * pCdmTransportHandle )
{
    int rc;

    ASSERT( TotalByteCount < 0xFFFFL, 0 );
    ASSERT( MaxByteCount < 0xFFFFL, 0 );

    TRACE(( TC_CDM, TT_API1,
      "SetupTransport: TotalByteCount %u, MaxByteCount %u",
       TotalByteCount, MaxByteCount ));

    /*
     *  Initialize output ring buffer
     */
    if ( rc = RingBufCreate( (USHORT) TotalByteCount ) )
        goto badringbuf;

    /*
     * Setup the CDMTRANSPORT data structures
     */
    Transport.BufCount = 0;
    Transport.BufSize = (USHORT)MaxByteCount;

    if ( (Transport.Buf = malloc( Transport.BufSize )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto badalloc;
    }

    Transport.State = CDMTRANSPORT_CONNECTED;
    *pCdmTransportHandle = (PVOID) &Transport;

    TRACE(( TC_CDM, TT_API1, "SetupTransport: rc=0" ));
    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  malloc of transport buffer failed
     */
badalloc:
    RingBufDelete();

    /*
     *  create of ring buffer failed
     */
badringbuf:
    TRACE(( TC_CDM, TT_API1, "SetupTransport: rc=%u", rc ));
    return( rc );
}



/*****************************************************************************
 *
 *  DeleteTransport
 *
 *  This deletes a transport freeing up any resources for it.
 *
 * ENTRY:
 *   pTransport (input)
 *      The transport to delete.
 *
 * EXIT:
 *   TRUE  -  Everything freed with no problems
 *   FALSE -  Transport was not connected.
 *
 ****************************************************************************/

BOOLEAN STATIC 
DeleteTransport( PVOID pTransport )
{

    /*
     * Delete ring buffer
     */
    RingBufDelete();

    /*
     * Free the memory in the buffer
     */
    free(Transport.Buf);

    Transport.Buf = NULL;
    Transport.State = CDMTRANSPORT_UNINITIALIZED;
    Transport.BufCount = 0;
    Transport.BufSize = 0;

    return(TRUE);
}

