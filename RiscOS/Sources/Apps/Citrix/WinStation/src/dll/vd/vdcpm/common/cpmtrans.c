
/*************************************************************************
*
* cpmtrans.c
*
* TRANSPORT interface for the CPM Server.
*
* This module implements a TRANSPORT for the Client Printer Mapping server
* over the CITRIX ICA protcol within the WinView client.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 05/06/94
*
* Log:
*
*
*
*************************************************************************/

#include "cpmserv.h"
#include "cpmtrans.h"

/*
 * Global data to this module
 */

/*
 * External references
 */
USHORT CpmICAWrite( PUCHAR, UCHAR, USHORT );


/*****************************************************************************
 *
 *  CpmTransportMaxSize
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

USHORT
CpmTransportMaxSize( PVOID pTransport )
{
    PCPMTRANSPORT pt = (PCPMTRANSPORT)pTransport;

    ASSERT( pt != NULL, 0 );

    if( pt->State != CPMTRANSPORT_CONNECTED ) {
        return( 0 );
    }
    else {
        return( (USHORT)pt->BufSize );
    }
}

/*****************************************************************************
 *
 *  SetupTransport
 *
 *   Setup the transport connection for the given Channel, returning
 *   a PVOID handle to it. It is ready for reading requests and sending
 *   replies at this point.
 *
 * ENTRY:
 *
 *   Channel (input)
 *     ICA channel to setup the connection for.
 *
 *   TotalByteCount (input)
 *     The total number of bytes that can be sent to the host at one time.
 *     This represents the "send window".
 *     This value has to also be known to the NT and/or OS/2 host so that
 *     file requests to not exceed this value until acknowledged.
 *
 *   MaxByteCount (input)
 *     The maximum PDU size to be used by this transport and the CPM
 *     protocol. This is supplied from the real transport provider
 *     (NetBios, SPX, ASYNC, through ICA)
 *
 *   pCpmTransportHandle (output)
 *     address to return handle to transport
 *
 * EXIT:
 *   NULL means an error setting up a transport connection.
 *   If successfull, a PVOID handle is returned to the transport.
 *
 ****************************************************************************/

int
SetupTransport( UCHAR  Channel,
                USHORT  TotalByteCount,
                USHORT MaxByteCount,
                PVOID  *pCpmTransportHandle )
{
    PCPMTRANSPORT pt = NULL;
    int rc;

    ASSERT( TotalByteCount < 0xFFFF, 0 );
    ASSERT( MaxByteCount < 0xFFFF, 0 );

    TRACE(( TC_CPM, TT_API1,
      "SetupTransport: TotalByteCount %u, MaxByteCount %u",
       TotalByteCount, MaxByteCount ));


    /*
     * Allocate memory for the transport structure
     */
    pt = (PCPMTRANSPORT)malloc( sizeof (CPMTRANSPORT) );
    if( pt == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto badalloc;
    }

    /*
     * Setup the CPMTRANSPORT data structures
     */

    pt->BufSize = MaxByteCount;
    pt->Channel = Channel;

    pt->State = CPMTRANSPORT_CONNECTED;
    *pCpmTransportHandle = (PVOID) pt;

    TRACE(( TC_CPM, TT_API1, "SetupTransport: rc=0" ));
    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  malloc of transport header failed
     */
badalloc:


    TRACE(( TC_CPM, TT_API1, "SetupTransport: rc=%u", rc ));
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

CPM_BOOLEAN
DeleteTransport( PVOID pTransport )
{

    ASSERT( pTransport != NULL, 0 );

    /*
     * Free the memory used by the transport header
     */

    free( pTransport );

    return(TRUE);
}


