
/*************************************************************************
*
* cpmserv.c
*
*  Server side of Citrix Client Printer Mapping remote access protocol.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 05/18/94
*
* Log:
*
*
*
*************************************************************************/

#include "cpmserv.h"
#include "cpmtrans.h"

/*
 * Include the client LPT support routines
 */
#include "../../../../inc/lptapi.h"

/*
 * This is the number of loops before we call the printer status poll
 */
#ifdef DOS
#define STATUS_POLL_COUNT 10
#else
/*
 * Windows uses the spooler, which should always be "ready"
 */
#define STATUS_POLL_COUNT 1
#endif

#define LPT_PORT_BASE      0
#define COM_PORT_BASE      4

/*
 * This is a filter of status bits to decide which ones we should bother the
 * HOST with an update.
 */
#define HOST_STATUS_FILTER (CPM_QUEUE_EMPTY | CPM_OUTOFPAPER | CPM_SELECT | CPM_IOERROR | CPM_TIMEOUT)

/*
 * General notes on this module:
 *
 * This is a single threaded server implementation of the CPM wire
 * protocol. Its primary intent is as a DOS 3.x targeted server.
 *
 * This supports multiple printers, each on their own ICA channel.
 *
 * A context value is passed back from a printer open request, but is
 * not sent on writes from the host to the client (this server). The
 * ICA channel is used to identify the requestor.
 *
 * No acknowledgment is made of any data that we receive for the printer.
 *
 * Printer status updates are sent to the host whenever a change to status
 * occurs that has never been sent.
 *
 * We can service host requests for status update to allow applications and
 * spoolers to set printer modes on the parallel port.
 *
 * SJM: Note that the 'pBuf' passed in is *not* word-aligned. For efficiency
 * each function willdeal with this as it sees fit. Structures that only contain
 * byte wide values are OK. Others need copying out before accessing.
 */

/*
 * Open printer context structure
 *
 * This contains information about the host status of a printer,
 * as well as deferred replys, etc.
 */
typedef struct _OPENCONTEXT {
    UCHAR  State;
    UCHAR  Port;
    USHORT WindowAckSize;  // Amount that must be acked in the Send ICA Window
    USHORT HostStatusWord; // Last status byte sent to the HOST
    int    PrintBufferSize;
    int    PrintBufferData;
    int    PrintBufferTail;
    PCHAR  PrintBuffer;
} OPENCONTEXT, *POPENCONTEXT;

#define OPENCONTEXT_INVALID 0
#define OPENCONTEXT_OPEN    1
#define OPENCONTEXT_CLOSING 2  // when Close is received, but must finish data in buffer

/*
 * Occasionally, requests such as PrinterOpen() will not be able to
 * reply since an ICA buffer was not available. This should be rare,
 * but the network timeout can be rather long until the HOST can
 * try again.
 *
 * This defines a simple structure that is dynamicly allocated when
 * this condition occurs and free when the data is sent at poll() time.
 */
typedef struct _DEFERRED {
    struct _DEFERRED *Next;
    UCHAR           Type;
    PUCHAR          Buf;
    UCHAR           Channel;
    int             Size;
} DEFERRED, *PDEFERRED;

// Types of deferred ICA messages
#define DEFERRED_USER_DATA  1
#define DEFERRED_WINDOW_ACK 2

static POPENCONTEXT OpenPorts[Virtual_Maximum] = { NULL };

static PDEFERRED DeferredReply = NULL;

/*
 * This flag is set if there is any data to be processed by
 * the poll routine.
 */
int RequestPrintData = FALSE;

/*
 * Forward references
 */
int ICAChannelToPort( UCHAR );
int CpmHostStatusUpdate( UCHAR, USHORT );
USHORT MapLptError( int );
CPM_BOOLEAN QueueDeferred( int, PUCHAR, UCHAR, int );
void ProcessDeferred( void );
USHORT CpmOpenWindow( UCHAR, USHORT );

/*
 * External references
 */
USHORT CpmICAWrite( PUCHAR, UCHAR, USHORT );
USHORT CpmICAWindowOpen( UCHAR, USHORT );

extern USHORT VirtualLPT1;
extern USHORT VirtualLPT2;
extern USHORT VirtualCOM1;
extern USHORT VirtualCOM2;


///////////////////////////////////////////////////////////////////////////////////
//
//
//              Main Request Dispatch Table and Routines
//
//    These handle a request received from a transport provider on
//    behalf of a redirector client (The WinView HOST).
//
//    The routines here are dispatched from the transport buffer by
//    CpmWireDataReceive() through the function dispatch table. These
//    routines unmarshall the request parameters, calculate any request
//    data buffer addresses, and then handle the request calling the base
//    client printer support routines.
//
//    At the current time, it is assumed that all data following a
//    header is available in the transport buffer supplied to
//    CpmWireDataReceive().
//
//    If all of the data were not available, extra buffering code would
//    need to be added to the unmarshalling routines along with any
//    additional state and data structures.
//
//    The client request routines have taken care to only make requests that
//    can be replied to in a single transport buffer.
//
//    It is assumed that the transport buffer size is the same for
//    the life of the connection.
//
///////////////////////////////////////////////////////////////////////////////////

//
// All per request dispatch functions have this prototype
// for the dispatch table
//

typedef
USHORT
(*PCPM_DISPATCH_ROUTINE)(
    PVOID pTransport,
    UCHAR Channel,
    PCHAR pBuf,
    USHORT Size
    );

//
// Function prototypes for per request dispatch functions
//

USHORT CpmSrvOpen( PVOID, UCHAR, PCHAR, USHORT );
USHORT CpmSrvClose( PVOID, UCHAR, PCHAR, USHORT );
USHORT CpmSrvWrite( PVOID, UCHAR, PCHAR, USHORT );
USHORT CpmSrvConnect( PVOID, UCHAR, PCHAR, USHORT );
USHORT CpmSrvGetStatus( PVOID, UCHAR, PCHAR, USHORT );
USHORT CpmSrvNoEnt( PVOID, UCHAR, PCHAR, USHORT );

//
// Request dispatch table
//

#define CPM_DISPATCH_TABLE_SIZE 8

PCPM_DISPATCH_ROUTINE CpmSrvDispatchTable [ CPM_DISPATCH_TABLE_SIZE ] = {
    CpmSrvOpen,
    CpmSrvClose,
    CpmSrvNoEnt, // Reserved for CPM_READ_REQUEST
    CpmSrvWrite,
    CpmSrvWrite,
    CpmSrvGetStatus, // Reserved for CPM_GETSTATUS_REQUEST
    CpmSrvNoEnt, // Reserved for CPM_SETSTATUS_REQUEST
    CpmSrvConnect
};

UCHAR CpmSrvDispatchTableSize = CPM_DISPATCH_TABLE_SIZE;


/*****************************************************************************
 *
 *  CpmWireDataReceive
 *
 *   This is the main dispatch function for data/requests received
 *   over an ICA connection for a printer.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
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
    )
{
    PCPM_OPEN_REQUEST r;
    USHORT RetSize;
    UCHAR Offset;

    if( Channel >= Virtual_Maximum ) {

        // Catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Make sure the data is at least the
     * size of our smallest request
     */
    if( Size < sizeof_CPM_WRITE1_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );

        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Use any packet type in order to get the type code.
     * All replys start with the same two bytes.
     */
    r = (PCPM_OPEN_REQUEST)pBuf;

    // Make sure a malicous/buggy client does not crash us

    Offset = r->h_type - (UCHAR)CPM_REQUEST_BASE;

    if( Offset > CpmSrvDispatchTableSize ) {
        TRACE(( TC_CPM, TT_ERROR, "CPMSERV: Type > DispTable, %d", r->h_type ));
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    TRACE(( TC_CPM, TT_API4, "CPMSERV: Dispatching Request %d, Size %d", r->h_type, Size));

    //
    // dispatch based on packet id to functions
    //
    // NOTE: Each function is responsible for opening the ICA window which
    //       includes its request header as well as any data
    //

    RetSize = (CpmSrvDispatchTable[Offset])( pTransport, Channel, pBuf, Size );

    // Look for definition/packing errors in the protocol
    ASSERT( RetSize == Size, 0 );

    return( Size );
}


/*****************************************************************************
 *
 *  CpmSrvNoEnt
 *
 *  No entry error function for empty table entries
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvNoEnt(
    PVOID pTransport,
    UCHAR Channel,
    PCHAR pBuf,
    USHORT Size
    )
{

    TRACE(( TC_CPM, TT_ERROR, "CPMSRV: No ent called! pTransport 0x%x, Channel %d, Size %d", pTransport, Channel, Size));

    // If theres a debugger lets catch this
    ASSERT( FALSE, 0 );

    // Open the ICA window
    CpmOpenWindow( Channel, (USHORT)Size );

    return( 0 );
}


/*****************************************************************************
 *
 *  CpmSrvOpen
 *
 *  Handle an open request for a printer from a client
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvOpen(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    USHORT  TotalSize;
    USHORT Result;
    USHORT Context;
    int    StatusWord;
    int    Ret, Port, BufferSize;
    PCPM_OPEN_REQUEST r;
    CPM_OPEN_REQUEST_REPLY rep;
    POPENCONTEXT p;


    TRACE(( TC_CPM, TT_API1, "CpmSrvOpen: Called"));

    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof( CPM_OPEN_REQUEST ) ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_OPEN_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CPM_TYPE_OPEN, 0 );

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof( CPM_OPEN_REQUEST );
    if( TotalSize != Size ) {

       TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Bad Packet Size %d, SB %d",Size, TotalSize));

       // Send an error reply so we do not hang the client
       rep.Result = CPM_MAKE_STATUS( CPM_ERROR_BADFORMAT, CPM_DOSERROR_BADLENGTH );
       Context = (USHORT)(-1);
       goto SendReply;
    }

    /*
     * Call the Printer request setup routine
     */

    ASSERT( r->Channel == Channel, 0 );

    if( r->Channel != Channel ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Channel Number in Request %d, SB %d", r->Channel, Channel));

        // Communication link error?
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_UNKNOWN );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    if( !(r->AccessMode & CPM_EXCLUSIVE_MODE) ||
        !(r->AccessMode & CPM_WRITEACCESS) ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: BadAccess Mode 0x%x", r->AccessMode));

        // Bad open modes
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_BADACCESS );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    /*
     * Convert the ICA channel number to an LPT port number
     */

    Port = ICAChannelToPort( Channel );
    if( Port == (UCHAR)(-1) ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Bad Port for Channel %d", Channel));

        // Invalid port
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_NOTFOUND );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    /*
     * Make sure this channel is not already open
     */

    p = OpenPorts[Channel];

    if( p != NULL ) {

        /*
         * If this is a CLOSING Channel, reverse the process and reply
         */
        if( p->State == OPENCONTEXT_CLOSING ) {

            TRACE(( TC_CPM, TT_API1, "CpmSrvOpen: Channel %d in Closing State, Now Open", Channel));

            p->State = OPENCONTEXT_OPEN;
            Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );
            Context = Port;

            /*
             * We must have a new status update sent to the host since
             * this is a "new" open and the ICB/FCB from the previous one
             * with the status was freed.
             *
	     * NOTE: This is not done anymore. See comments below.
	     */

//          p->HostStatusWord = 0;

            goto SendReply;
        }

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Channel %d Allready Open", Channel));

        // Already open
        Result = CPM_MAKE_STATUS( CPM_ERROR_LOCKED, CPM_DOSERROR_SHARE );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    BufferSize = (USHORT)CpmTransportMaxSize( pTransport );

    if( BufferSize == 0 ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Invalid Buffer Size %d", BufferSize));

        // Invalid ICA buffer size
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_BADLENGTH );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpen: Calling LptOpen on Port %d", Port));

    Ret = LptOpen( Port );

    if( Ret != CLIENT_STATUS_SUCCESS ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpen: Could Not Open LPT Port %d", Port));

        // Map the error code and return the error
        Result = MapLptError( Ret );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    /*
     * Allocate the open context and buffer memory for the printer so we
     * can spool received data out at poll() time.
     */

    p = malloc( sizeof(OPENCONTEXT) );

    if( p == NULL ) {

        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_BADLENGTH );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    p->PrintBuffer = malloc( BufferSize );

    if( p->PrintBuffer == NULL ) {

        free( p );
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_BADLENGTH );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    p->State = OPENCONTEXT_OPEN;
    p->Port = Port;
    p->WindowAckSize = 0;
    p->PrintBufferSize = BufferSize;
    p->PrintBufferData = 0;
    p->PrintBufferTail = 0;

    /*
     * We must get the status word now, since a fast host could call
     * GetStatus before we update the status from the poll() routine.
     */
    Ret = LptStatus( p->Port, &StatusWord );
    if( Ret == 0 ) {
        p->HostStatusWord = StatusWord;
    }
    else {
        // Error, set it to zero
	p->HostStatusWord = 0;
    }

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpen: HostStatusWord 0x%x", p->HostStatusWord));

    OpenPorts[Channel] = p;

    Context = Port;
    Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpen: Success, Context %d", Context));

SendReply:

    /*
     * Open the ICA window
     */

    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CPM_TYPE_OPEN_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context; // This context represents the open file to the OS routines
    rep.Result = Result;   // Error codes as in cpmwire.h

    // Catch any transport bugs
    ASSERT( CpmTransportMaxSize( pTransport ) >= sizeof_CPM_OPEN_REQUEST_REPLY, 0 );


    /*
     * Now send the reply out on the ICA
     */

    Ret = CpmICAWrite( (PUCHAR)&rep,
                       Channel,
                       sizeof_CPM_OPEN_REQUEST_REPLY
                     );

    // Maybe no buffer space on ICA
    if( Ret ) {

       // Attempt to queue it
       if( !QueueDeferred( DEFERRED_USER_DATA,
                           (PUCHAR)&rep,
                           Channel,
                           sizeof_CPM_OPEN_REQUEST_REPLY
                         ) )  {
           TRACE((TC_CPM, TT_ERROR, "Client Printer Open: Dropped Reply, Channel %d", Channel));
           return( 0 );
       }
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CpmSrvClose
 *
 *  Close an open printer
 *
 *  NOTE: There is no reply for this function, the client assumes it
 *        either always succeeds, or the transport has broken the
 *        connection.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvClose(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    )
{

    POPENCONTEXT p;
    CPM_CLOSE_REQUEST close;
    PCPM_CLOSE_REQUEST r;
    USHORT Result = CPM_ERROR_NONE;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof( CPM_CLOSE_REQUEST ) ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    memcpy(&close, pBuf, sizeof( CPM_CLOSE_REQUEST ));
    r = &close;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CPM_TYPE_CLOSE, 0 );

    /*
     * Make sure the port is open
     */

    p = OpenPorts[Channel];

    if( p == NULL ) {

        // Not open
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_NOHANDLES );
        goto Done;
    }

    ASSERT( r->Context == p->Port, 0 );

    if( r->Context != p->Port ) {

        // Communication error?
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_NOHANDLES );
        goto Done;
    }

    TRACE(( TC_CPM, TT_API1, "CpmClose: Close Successfull on Channel %d", Channel));

    /*
     * Even though we got this close from the HOST, we could still
     * be printing. So if this is the case, we just mark the port a CLOSING
     * and have the poll() routine finish the close.
     */

    if( !(p->PrintBufferTail < p->PrintBufferData) ) {

        TRACE(( TC_CPM, TT_API1, "CpmClose: Channel %d Closed right away", Channel));

        /*
         * Call the printer close routine
         */

        LptClose( p->Port );

        /*
         * Mark this channel as closed
         */

        OpenPorts[Channel] = NULL;

        /*
         * Free the OPENCONTEXT resources and the printers buffer
         */

        ASSERT( p->PrintBuffer != NULL, 0 );

        free( p->PrintBuffer );

        free( p );
     }
     else {

         /*
          * Mark it as CLOSING so that the poll() routine will
          * close this port when all of the data has been written out.
          */
         TRACE(( TC_CPM, TT_API1, "CpmClose: Marking Channel %d for deferred Close", Channel));
         p->State = OPENCONTEXT_CLOSING;
     }

Done:

    /*
     * Open the ICA Window
     */

    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * We have no way of handling an error from CpmPrinterClose, since
     * there is no reply.
     *
     * The only reason for an error would be a bad Context, so we
     * ASSERT here in order to catch any bugs during development.
     */

    ASSERT( Result == CPM_ERROR_NONE, 0 );

    return( Size );
}

/*****************************************************************************
 *
 *  CpmSrvWrite
 *
 *  Handle a printer write request from the client.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvWrite(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    PCHAR  pWriteBuf;
    USHORT CurrLength;
    USHORT CurrHead;
    USHORT NewLength;
    USHORT WriteSize;
    PCPM_WRITE1_REQUEST rs;
    POPENCONTEXT p;

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_WRITE1_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    rs = (PCPM_WRITE1_REQUEST)pBuf;

    /*
     * Get a pointer to the write data following the packet header
     */
    if( rs->h_type == CPM_TYPE_WRITE1 ) {
        pWriteBuf = (PCHAR)( pBuf + sizeof_CPM_WRITE1_REQUEST );
        WriteSize = rs->WriteSize;
    }
    else {
	CPM_WRITE2_REQUEST wr2;

        ASSERT( rs->h_type == CPM_TYPE_WRITE2, rs->h_type );

	pWriteBuf = (PCHAR)( pBuf + sizeof( CPM_WRITE2_REQUEST ) );
	memcpy(&wr2, pBuf, sizeof( CPM_WRITE2_REQUEST ));
        WriteSize = wr2.WriteSize;
    }

    p = OpenPorts[Channel];

    /*
     * Verify that we believe the port is open
     */

    if( p == NULL ) {

        // Invalid port
        CpmOpenWindow( Channel, (USHORT)Size );
        goto Done;
    }

    /*
     * If its a write of 0 bytes, return now
     */

    if( WriteSize == 0 ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmWrite: 0 Length Write on Channel %d", Channel));
        CpmOpenWindow( Channel, (USHORT)Size );
        goto Done;
    }

    //
    // Update the WindowAckSize field.
    // The Size includes the packet header and the data sent to us.
    //
    // When this data is finally printed, we will then send this to
    // the ICA so that more data can be sent.
    //

    p->WindowAckSize += (USHORT)Size;

    TRACE(( TC_CPM, TT_API4, "CPM: Got %d Bytes Print Data Channel %d", WriteSize, Channel));

    ASSERT( p->PrintBuffer != NULL, 0 );

    // fragged packet, append new data
    if ( p->PrintBufferData != p->PrintBufferTail ) {

        // if we get here we probably had a fragmented packet and
        // need to append the data to the original buffer
        // but, we do not request more data or adjust the tail ptr

        // current buffer length
        CurrLength = p->PrintBufferSize;

        // current head pointer
        CurrHead = p->PrintBufferData;

        // new buffer length
        NewLength  = CurrHead + WriteSize;

        // realloc buffer if it needs to grow
        if ( NewLength > CurrLength ) {

            TRACE(( TC_CPM, TT_ERROR, "CpmSrvWrite: Frag Packet Buffer OVERFLOW! ICA Problems! NewLength %d, CurrLength %d", NewLength, CurrLength));
            /*
	     * An overflow is an ICA bug. Truncate the write to fit within
	     * the available buffer space.
	     */
            WriteSize = CurrLength - CurrHead;
            NewLength = CurrLength;
	}

        // append new data to old data
        memcpy( &p->PrintBuffer[CurrHead], pWriteBuf, WriteSize );
        p->PrintBufferData = NewLength;
        TRACE(( TC_CPM, TT_API4, "CpmSrvWrite: copy %u bytes at %u", WriteSize, CurrHead ));
    }
    else {

       if ( (int)WriteSize > p->PrintBufferSize ) {

           TRACE(( TC_CPM, TT_ERROR, "CpmSrvWrite: NonFrag Packet Buffer OVERFLOW! ICA Problems! NewLength %d, CurrLength %d", NewLength, CurrLength));
           WriteSize = p->PrintBufferSize;
       }

       // copy data to "spool" print buffer if available and set pointers
       memcpy( p->PrintBuffer, pWriteBuf, WriteSize );
       p->PrintBufferData = WriteSize;
       p->PrintBufferTail = 0;
       TRACE(( TC_CPM, TT_API4, "CpmSrvWrite: copy %u bytes", WriteSize ));
    }

    /*
     * If we queued any data, then set the flag for
     * the poll routine
     */

    if( WriteSize ) RequestPrintData = TRUE;

    /*
     * There is no reply, so just return.
     */
Done:

    return( Size );
}

/*****************************************************************************
 *
 *  CpmSrvConnect
 *
 *  Handle a connect from the host.
 *
 *  We do not reply to this since the client to host information
 *  has already been sent.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvConnect(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    CPM_CONNECT_REQUEST req;
    PCPM_CONNECT_REQUEST rs;

    TRACE(( TC_CPM, TT_API1, "CPM: Got Connect request"));

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_CONNECT_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    memcpy(&req, pBuf, sizeof_CPM_CONNECT_REQUEST);
    rs = &req;

    /*
     * Right now we do not do anything with the version
     * info.
     */

    TRACE(( TC_CPM, TT_API1, "CPM: Host Versions Low %d, High %d", rs->VersionLow,rs->VersionHigh));

    CpmOpenWindow( Channel, (USHORT)Size );

    return( Size );
}

/*****************************************************************************
 *
 *  CpmCloseAllContexts
 *
 *   Close all active printer contexts
 *
 * ENTRY:
 *
 * EXIT:
 *
 ****************************************************************************/

void
CpmCloseAllContexts( )
{
    UCHAR Channel;
    POPENCONTEXT p;

    TRACE(( TC_CPM, TT_API1, "CPM: Closing All Contexts (Disconnect/Unload)"));

    /*
     * Close any active printer contexts
     */

    for( Channel = 0; Channel < Virtual_Maximum; Channel++ ) {

        p = OpenPorts[Channel];

        if( p != NULL ) {

            // Close the lpt device

            LptClose( p->Port );

            // Mark the open context as invalid now
            OpenPorts[Channel] = NULL;

            // Free the resources
            ASSERT( p->PrintBuffer != NULL, 0 );

            free( p->PrintBuffer );

            free( p );
        }
    }
}


/*****************************************************************************
 *
 *  CpmPollAllPorts
 *
 *    This is called to check the status on all of the valid
 *    open ports, and queue a status change packet to the host
 *    if one has changed.
 *
 * ENTRY:
 *
 * EXIT:
 *    TRUE if the function did "useful work"
 *    FALSE if it did not do useful work so DOS yield() may be called.
 *
 ****************************************************************************/

int
CpmPollAllPorts( )
{
    UCHAR Channel;
    int   Ret, AmountWrote, Count;
    int Result = 0;
    int StatusWord;
    USHORT Filtered;
    int Usefull = FALSE;
    POPENCONTEXT p;
    int ActivePrinters = 0; // To determine when last printer goes inactive
    static int PollCounter = 0;

    /*
     * First see if any replies have been queued
     */
    if( DeferredReply != NULL ) {

        // The worker routine
        ProcessDeferred();
    }

    /*
     * Since looking at all of the printer status for every
     * poll is too wastefull, we only do it so often, unless a printer
     * actually has data on a queue.
     *
     *
     * BUGBUG: We should look into average poll loops between printer
     *         byte sends and use this to dynamicly adjust this poll
     *         count. This is because fast laser printer may want the
     *         data as fast as possible, while slower dot matrix printers
     *         should not waste our time always polling.
     */

    if( !RequestPrintData ) {

        PollCounter++;
        if( PollCounter < STATUS_POLL_COUNT ) {
            return( Usefull );
        }
        PollCounter = 0;
    }

    /*
     * Now Look at all active printer contexts for a status change
     */

    for( Channel = 0; Channel < Virtual_Maximum; Channel++ ) {

        p = OpenPorts[Channel];

        if( p == NULL ) {
            continue;
        }

        // Get the port, and if valid poll the lpt device status

        Ret = LptStatus( p->Port, &StatusWord );
        if( Ret != 0 ) {
            continue;
        }

        // Filter out bits we do not bother the HOST with
        Filtered = StatusWord & HOST_STATUS_FILTER;

        // Compare with the last status word sent to the host

        if( Filtered != (p->HostStatusWord & HOST_STATUS_FILTER) ) {

            Usefull = TRUE;

            // Send a Host Status Update Message

            if( CpmHostStatusUpdate( Channel, (USHORT)StatusWord ) == 0 ) {

                //
                // Only update the HostStatusWord if successfull send
                // This is because ICA may not have a buffer, so we will
                // get it next time (hopefully)
                //

                p->HostStatusWord = StatusWord;
            }
        }

        /*
         * See if this printer has any data on its queue
         */

        if( !(p->PrintBufferTail < p->PrintBufferData) ) {

            continue;
        }

        ActivePrinters++;  // This one has data

        /*
         * Now see if the port is ready for any printer data
         */
        if( (StatusWord & LPT_SELECT) &&
            (StatusWord & LPT_NOTBUSY) ) {

            // We can send characters out to the printer
	    Count = p->PrintBufferData - p->PrintBufferTail;

//          c = p->PrintBuffer[p->PrintBufferTail];

            Ret = LptWriteBlock( p->Port, &p->PrintBuffer[p->PrintBufferTail], Count, &AmountWrote );

            if( Ret != 0 ) continue;  // Some error, get it next status time.

//          TRACE(( TC_CPM, TT_API3, "PrinterPoll: Sent char :%c: Printer %d", c, Channel));

            // advance the count by the amount written
            p->PrintBufferTail += AmountWrote;

            // We did useful work, the printer is accepting data
            Usefull = TRUE;

            /*
             * If buffer empty then notify ICA window
             */

            if ( p->PrintBufferData != 0 &&
                 p->PrintBufferTail == p->PrintBufferData ) {

                 TRACE((TC_CPM, TT_API4, "PrinterPoll: Printer %d gone idle", Channel));

                 ActivePrinters--; // Remove this one from the count.

                 //
                 // Open the ICA Send Window
                 // BUGBUG: We currently only open when whole buffers are
                 //         printed. We should open on 1/2 full/empty.
                 //

                 CpmOpenWindow( Channel, p->WindowAckSize );

                 p->WindowAckSize = 0;  // Ack has been sent

                 //
                 // If this is a CLOSING channel, do the cleanup now
                 //
                 if( p->State == OPENCONTEXT_CLOSING ) {

                     TRACE(( TC_CPM, TT_API1, "CpmPoll: Doing Deferred Close for Channel %d", Channel));
                     LptClose( p->Port );
                     OpenPorts[Channel] = NULL;
                     ASSERT( p->PrintBuffer != NULL, 0 );
                     free( p->PrintBuffer );
                     free( p );
                 }
            }
        }
    }

    /*
     * If no more active printer queues, do not bother checking
     * everything on future polls.
     */

    if( ActivePrinters == 0 ) {

        if( RequestPrintData == TRUE ) {
            TRACE((TC_CPM, TT_API4, "PrinterPoll: No Active Printers Now"));
        }
        RequestPrintData = FALSE;
    }

    return( Usefull );
}

/*****************************************************************************
 *
 *  ICAChannelToPort
 *
 *   Convert the supplied ICA channel to the low level LPT port number.
 *
 * ENTRY:
 *   Channel (input)
 *     ICA channel number.
 *
 * EXIT:
 *   (-1) - ICA channel does not have an LPT port associated with it
 *        - Else the port number is returned
 *
 ****************************************************************************/

int
ICAChannelToPort( UCHAR Channel )
{
    int Port;

    if( Channel == VirtualLPT1 ) {
        Port = LPT_PORT_BASE+0;
    } else if( Channel == VirtualLPT2 ) {
        Port = LPT_PORT_BASE+1;
//  } else if( Channel == Virtual_LPT3 ) {
//      Port = LPT_PORT_BASE+2;
//  } else if( Channel == Virtual_LPT4 ) {
//      Port = LPT_PORT_BASE+3;
    } else if( Channel == VirtualCOM1 ) {
        Port = COM_PORT_BASE+0;
    } else if( Channel == VirtualCOM2 ) {
        Port = COM_PORT_BASE+1;
//  } else if( Channel == Virtual_COM3 ) {
//      Port = COM_PORT_BASE+2;
//  } else if( Channel == Virtual_COM4 ) {
//      Port = COM_PORT_BASE+3;
    }
    else {
        Port = (UCHAR)(-1);
    }

    return( Port );
}

/*****************************************************************************
 *
 *  CpmHostStatusUpdate
 *
 *   Update the Parallel port status byte on the HOST for the
 *   given channel.
 *
 * ENTRY:
 *    Channel
 *     ICA Channel that represents the printer whose status is to be
 *     updated.
 *
 *    StatusWord
 *     New status word to send to the host.
 *
 * EXIT:
 *   0 - no error
 *  !0 - An error, such as could not get an ICA buffer.
 *
 ****************************************************************************/

int CpmHostStatusUpdate( UCHAR Channel, USHORT StatusWord )
{
    int rc;
    CPM_STATUS_UPDATE r;

    TRACE(( TC_CPM, TT_API1, "CPMSERV: Sending Status Update 0x%x, Channel %d", StatusWord, Channel ));

    r.h_type = CPM_TYPE_STATUS_UPDATE;
    r.StatusWord = StatusWord;

    // Now try to send it over the ICA on the given channel

    rc = CpmICAWrite( (PUCHAR)&r, Channel, sizeof( CPM_STATUS_UPDATE ) );

    if( rc != CLIENT_STATUS_SUCCESS ) {

        if( rc == CLIENT_ERROR_NO_OUTBUF ) {
            TRACE(( TC_CPM, TT_API1, "CPMSTATUSUPDATE: No OutBuf, Deferring"));
            return(rc);
        }
        else {
            TRACE(( TC_CPM, TT_ERROR, "CPMSTATUSUPDATE: General Error %d",rc));
            return(rc);
        }
    }

    return( rc );
}

/*****************************************************************************
 *
 *  CpmOpenWindow
 *
 *   Flow control for client printer mapping is handled by delaying the ICA
 *   flow control acknowledgement until we have buffer space for more data.
 *
 *   This function "opens" the window by sending the ICA ack.
 *
 *   Since we may not have ICA buffer space at the time we try and send
 *   the ack, we handle this condition by placing a record on the deferred
 *   processing queue to have the ack sent at poll() time.
 *
 *
 * ENTRY:
 *   Channel (input)
 *     ICA channel to open the window on
 *
 *   WindowSize (input)
 *     The count to open the window
 *
 * EXIT:
 *   CLIENT_STATUS_SUCCESS - no error
 *
 ****************************************************************************/

USHORT
CpmOpenWindow( UCHAR Channel, USHORT WindowSize )
{
    USHORT rc;

    // First make sure nothing is in the deferred queue in front of us

    if( DeferredReply != NULL ) {

        // We queue this right away so we do not re-order requests/replies
        QueueDeferred( DEFERRED_WINDOW_ACK, NULL, Channel, WindowSize);
        return( CLIENT_STATUS_SUCCESS );
    }

    // Try and send the ICA window update
    rc = CpmICAWindowOpen( Channel, WindowSize );

    if( rc != CLIENT_STATUS_SUCCESS ) {

        // We must defer it if its CLIENT_ERROR_NO_OUTBUF
        if( rc == CLIENT_ERROR_NO_OUTBUF ) {

            QueueDeferred( DEFERRED_WINDOW_ACK, NULL, Channel, WindowSize);
            return( CLIENT_STATUS_SUCCESS );
        }
        else {
            TRACE(( TC_CPM, TT_ERROR, "CPMOPENWINDOW: Could Not Send Window Open rc %d",rc));
            return(rc);
        }
    }
    return CLIENT_STATUS_SUCCESS;
}


/*****************************************************************************
 *
 *  QueueDeferred
 *
 *   Occasionally, requests such as open may not be able to reply
 *   since there is no ICA send buffer space.
 *
 *   Since the network timeout on the HOST redirector can be long, the
 *   reply is queued into dynamically allocated memory and sent at poll()
 *   time.
 *
 *   This happens only occasionally, and the replies are small so this
 *   should not represent much overhead.
 *
 *   NOTE: The most frequent data sent to the HOST, which is STATUS UPDATE
 *         does not queue. If it can not get a buffer, the STATUS update
 *         is done again at a later poll() time.
 *
 *
 * ENTRY:
 *   Type (input)
 *     Whether this is a deferred virtual write, or virtual ack
 *
 *   pBuf (input)
 *     Pointer to buffer with data to queue if Type == DEFERRED_USER_DATA
 *
 *   Channel (input)
 *     Which ICA channel to queue this for
 *
 *   Size (input)
 *     Size of the data and/or Window acknowledgement
 *
 *
 * EXIT:
 *   CLIENT_STATUS_SUCCESS - no error
 *
 ****************************************************************************/

CPM_BOOLEAN
QueueDeferred( int Type, PUCHAR pBuf, UCHAR Channel, int Size )
{
    PDEFERRED Tmp, Last;

    // Attempt to queue it
    Tmp = (PDEFERRED)malloc( sizeof( DEFERRED ) );

    if ( Tmp == NULL ) {
        TRACE(( TC_CPM, TT_ERROR, "QueueDeferred: No memory, dropping Host reply Type %d, Size %d, Channel %d", Type, Size, Channel ));
        return( FALSE );
    }

    if( (Type == DEFERRED_USER_DATA) && (pBuf != NULL) ) {

        // Allocate buffer space
        Tmp->Buf = (PUCHAR)malloc( Size );

        if( Tmp->Buf == NULL ) {
            free( Tmp );
            return( FALSE );
        }

        // Copy the data
        memcpy( Tmp->Buf, pBuf, Size );
    }
    else {
        Tmp->Buf = NULL;
    }

    Tmp->Type = Type;
    Tmp->Channel = Channel;
    Tmp->Size = Size;
    Tmp->Next = NULL;

    //
    // Now link this entry on the end of the list. (should be short)
    // If this list ever becomes large, then we should add code to maintain
    // an end-of-list pointer.
    //

    if( DeferredReply == NULL ) {

        // Begining of list
        DeferredReply = Tmp;
        return( TRUE );
    }

    // Entrie(s) already on the list, so chain forward until end

    Last = DeferredReply;

    while( Last->Next != NULL ) Last = Last->Next;

    Last->Next = Tmp;

    return( TRUE );
}


/*****************************************************************************
 *
 *  ProcessDeferred
 *
 *   Process any deferred messages to be sent to the HOST
 *
 * ENTRY:
 *
 * EXIT:
 *
 ****************************************************************************/


void
ProcessDeferred()
{
    int   Ret;
    PDEFERRED Tmp;

    Tmp = DeferredReply;

    if( Tmp == NULL ) return;

    if( Tmp->Type == DEFERRED_USER_DATA ) {

        // Process the USER_DATA (Virtual Write) Type
        TRACE((TC_CPM, TT_API1, "Client Printer: Attempting To Send deferred reply channel %d", Tmp->Channel));

        // Attempt to write it
        // BUGBUG: Tmp->Size is an "int" must cast as a "USHORT"
        Ret = CpmICAWrite( Tmp->Buf,
                           Tmp->Channel,
                           (USHORT)Tmp->Size
                         );

    }
    else if( Tmp->Type == DEFERRED_WINDOW_ACK ) {

        TRACE((TC_CPM, TT_API1, "Client Printer: Attempting To Send Window Ack %d", Tmp->Channel));

        // Process the WINDOW_ACK (Virtual Ack) Type
        // BUGBUG: Tmp->Size is an "int" must cast as a "USHORT"
        Ret = CpmICAWindowOpen( Tmp->Channel, (USHORT)Tmp->Size );
    }
    else {

        // Bad type, on debug build print it out.
        TRACE((TC_CPM, TT_ERROR, "Client Printer: Bad Deferred Reply Type %d for Channel %d", Tmp->Type, Tmp->Channel));
        Ret = 0;  // remove the entry
    }

    if( Ret == 0 ) {

        // Success this time around, so remove the entry
        TRACE((TC_CPM, TT_ERROR, "Client Printer: Sent deferred reply channel %d, Type %d", Tmp->Channel, Tmp->Type));

        DeferredReply = Tmp->Next;

        if( Tmp->Buf != NULL ) {
            free( Tmp->Buf );
        }

        free( Tmp );
    }
}


/*****************************************************************************
 *
 *  MapLptError
 *
 *   Map an LPT error to a CPM wire error (DOS type)
 *
 * ENTRY:
 *   LptError (input)
 *    The LPT error in using the Citrix Client Error codes (client.h)
 *
 * EXIT:
 *   Returns the error code as a DOS format error.
 *
 ****************************************************************************/

USHORT
MapLptError( int LptError )
{
    USHORT Result;

    switch( LptError ) {

	case CLIENT_STATUS_SUCCESS:
                Result = CPM_ERROR_NONE;
                break;

        case CLIENT_ERROR_INVALID_HANDLE:
                Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_UNKNOWN );
                break;

        case CLIENT_ERROR_OPEN_FAILED:
                Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_UNKNOWN );
                break;

        case CLIENT_ERROR_NO_MEMORY:
                Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_UNKNOWN );
                break;

	case CLIENT_ERROR_ENTRY_NOT_FOUND:
	case CLIENT_ERROR_PORT_NOT_AVAILABLE:
                Result = CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_UNKNOWN );
                break;

        default:
                Result = CPM_MAKE_STATUS( CPM_ERROR_UNKNOWN, CPM_DOSERROR_UNKNOWN );
                break;
    }

    return( Result );
}

/*****************************************************************************
 *
 *  CpmFlush
 *
 *   Flush the given printer (empty the queue, idle it)
 *
 * ENTRY:
 *   Channel (input)
 *     Printer ICA channel number
 *
 *   Mask (input)
 *     ICA mask for flushing input or output
 *
 * EXIT:
 *
 ****************************************************************************/

VOID
CpmFlush(
    UCHAR Channel,
    UCHAR Mask
    )
{
    POPENCONTEXT p;

    if( Channel > Virtual_Maximum ) {
        TRACE(( TC_CPM, TT_ERROR, "CPMSERV: Channel %d invalid", Channel ));
        return;
    }

    // Get the port context structure
    p = OpenPorts[Channel];

    if( p == NULL ) {
        // Not open
        TRACE(( TC_CPM, TT_ERROR, "CPMSERV: Channel %d not open", Channel ));
        return;
    }

    TRACE(( TC_CPM, TT_API1, "CPMSERV: Channel %d flushed", Channel ));

    //
    // Whenever we get a flush, things are bad as far as the
    // Host is concerned. Se we force the port closed since this
    // is what the host does when there are problems.
    //

    //
    // Call the printer close routine
    //
    LptClose( p->Port );

    // Mark this channel as closed
    OpenPorts[Channel] = NULL;

    // Free the OPENCONTEXT resources and the printers buffer
    ASSERT( p->PrintBuffer != NULL, 0 );

    free( p->PrintBuffer );

    free( p );

    return;
}

/*****************************************************************************
 *
 *  CpmSrvGetStatus
 *
 *  Handle a get status request from the host
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
 *
 *   Channel (input)
 *     ICA channel number the request came in on
 *
 *   pBuf (input)
 *     Buffer containing data from client
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
CpmSrvGetStatus(
    PVOID  pTransport,
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    int Ret;
    PCPM_GETSTATUS_REQUEST r;
    CPM_GETSTATUS_REQUEST_REPLY rep;
    POPENCONTEXT p;

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_GETSTATUS_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_GETSTATUS_REQUEST)pBuf;

    ASSERT( r->h_type == CPM_TYPE_GETSTATUS, 0 );

    /*
     * Setup main reply header body
     */
    rep.h_type = CPM_TYPE_GETSTATUS_REPLY;
    rep.MpxId = r->MpxId;

    p = OpenPorts[Channel];

    /*
     * Verify that we believe the port is open
     */
    if( p == NULL ) {

        // Invalid port
        CpmOpenWindow( Channel, (USHORT)Size );
        rep.Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_NOHANDLES );
        goto SendReply;
    }

    /*
     * Open the ICA window
     */
    CpmOpenWindow( Channel, (USHORT)Size );

    rep.h_type = CPM_TYPE_GETSTATUS_REPLY;
    rep.MpxId = r->MpxId;
    rep.StatusWord = p->HostStatusWord;
    rep.Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

    TRACE((TC_CPM, TT_ERROR, "CpmSrvGetStatus: Returning Status 0x%x",rep.StatusWord));

    // Catch any transport bugs
    ASSERT( CpmTransportMaxSize( pTransport ) >= sizeof( CPM_GETSTATUS_REQUEST_REPLY ), 0 );

SendReply:

    /*
     * Now send the reply out on the ICA
     */
    Ret = CpmICAWrite( (PUCHAR)&rep,
                       Channel,
                       sizeof( CPM_GETSTATUS_REQUEST_REPLY )
                     );

    // Maybe no buffer space on ICA
    if( Ret ) {

       // Attempt to queue it
       if( !QueueDeferred( DEFERRED_USER_DATA,
                           (PUCHAR)&rep,
                           Channel,
                           sizeof( CPM_GETSTATUS_REQUEST_REPLY)
                         ) )  {
           TRACE((TC_CPM, TT_ERROR, "Client Printer Status: Dropped Reply, Channel %d", Channel));
           return( 0 );
       }
    }

    return( Size );
}

