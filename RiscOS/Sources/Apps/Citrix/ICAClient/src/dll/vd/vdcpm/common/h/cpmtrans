
/*************************************************************************
*
* cpmtrans.h
*
* Header file for the Client Printer Mapping Internal Transport Routines
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 05/06/94
*
* Log:
*
*
*************************************************************************/


/*
 * Definition of our internal transport structure. This is passed
 * as a PVOID to the clients, and they will pass it in on all calls
 * to identify the unique transport.
 */
typedef struct _CPMTRANSPORT {
    int             State;     // The state of the connection.
    unsigned int    BufSize;   // The size of the output buffer
    UCHAR           Channel;   // ICA channel
} CPMTRANSPORT, *PCPMTRANSPORT;

// Define connection states

#define CPMTRANSPORT_UNINITIALIZED 0
#define CPMTRANSPORT_CONNECTED     1
#define CPMTRANSPORT_DISCONNECTED  2
#define CPMTRANSPORT_ERROR         3

// Define Error codes

#define CPMTRANSPORT_ERROR_NOCONNECTION 1
#define CPMTRANSPORT_ERROR_OVERFLOW     2
#define CPMTRANSPORT_ERROR_UNKNOWN      3

/*****************************************************************************
 *
 *  SetupTransport
 *
 *   Setup the transport connection for the given channel, returning
 *   a PVOID handle to it. It is ready for reading requests and sending
 *   replies at this point.
 *
 * ENTRY:
 *
 *   Channel (input)
 *     ICA channel to create this transport for
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
SetupTransport( UCHAR Channel,
                USHORT TotalByteCount,
                USHORT MaxByteCount,
                PVOID * pCpmTransportHandle );


/*****************************************************************************
 *
 *  DeleteTransport
 *
 *  This deletes a transport freeing up any resources for it.
 *
 *  Any sends in progress, or deferred requests are thrown away.
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
DeleteTransport(
    PVOID pTransport
    );


/*****************************************************************************
 *
 *  CpmTransportSendData
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
 *   pSent (output)
 *     Pointer to variable to place the actual amount of data sent.
 *     This routine should send all of the data, but this provides the
 *     ability to signify an error to the caller.
 *
 *   Final (input)
 *     Flag that marks the end of a sequence and the data should be
 *     sent on the wire now as a complete packet.
 *
 *     If false, the data is put into a buffer for future sending, and
 *     not sent. If the buffer overflows, this routine will flush the unsent
 *     data and return an error to the caller. The buffer size promise we
 *     make to the caller can be retrieved  from CpmTransportMaxSize ()
 *     and is valid for the duration of the connection.
 *
 *  This routine returns an error to the caller if the requested data
 *  can not be sent as one complete network PDU. This includes any data
 *  written into the buffer since the last Final == TRUE call.
 *
 *  If a send can not be done due to the outgoing ICA send window, the
 *  request will be queued to the deferred queue for sending at
 *  CpmTransportPoll() time. If there are no available queued slots, the
 *  data will be dropped and it will be handled as a data buffer overrun.
 *
 *  It is up to the higher level to make sure no more requests are queued
 *  than can be handled by deferred queue setup at start time.
 *
 * EXIT:
 *   0  - no error
 *   !0 - Error code
 *
 ****************************************************************************/

int
CpmTransportSendData(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size,
    PUSHORT pSent,
    USHORT  Final
    );


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
 *   size of the maximum send size. On errors, returns 0.
 *
 ****************************************************************************/

USHORT
CpmTransportMaxSize(
    PVOID pTransport
    );


/*****************************************************************************
 *
 *  CpmWireDataReceive
 *
 *   This is the main dispatch function for data/requests received
 *   over a transport. This is called by the transport provider
 *   passing an opaque handle, along with a virtual address buffer
 *   pointer, and size.
 *
 *   NOTE: This function is called by the ICA arrival function
 *         in vdcpm.c using the transport handle opened at VD init time.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel the data arrived on
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

USHORT
CpmWireDataReceive(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    );

