
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
#include "printer.h"

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

/*
 * This is a filter of status bits to decide which ones we should bother the
 * HOST with an update.
 */
#define HOST_STATUS_FILTER (CPM_QUEUE_EMPTY | CPM_OUTOFPAPER | CPM_SELECT | CPM_IOERROR | CPM_TIMEOUT)

/*
 * General notes on this module:
 *
 * This is a single threaded server implementation of the CPM wire
 * protocol. Its primary intent is as a WIN95 + WIN16 server.
 *
 * It also supports DOS by having the CpmPrinter* functions handle
 * the per OS dependencies.
 *
 * This supports the newer client-server protocol on a new ICA channel,
 */

/*
 * Open printer context structure
 *
 * This contains information about the host status of a printer,
 * as well as deferred replys, etc.
 */
typedef struct _OPENCONTEXT {
    UCHAR  State;
    USHORT WindowAckSize;  // Amount that must be acked in write reply
    ULONG  HostStatus;     // Last status sent to the HOST
    int    hPrinter;
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

static POPENCONTEXT OpenPorts[MAX_PRINTERS] = { NULL };

static PDEFERRED DeferredReply = NULL;

/*
 * What level host we are talking to
 */
static USHORT HostVersionLow  = CPM_MIN_VERSION;
static USHORT HostVersionHigh = CPM_MIN_VERSION;

/*
 * This flag is set if there is any data to be processed by
 * the poll routine.
 */
static int RequestPrintData = FALSE;

/*
 * Forward references
 */
static int CpmHostStatusUpdate( USHORT, ULONG );
static CPM_BOOLEAN QueueDeferred( int, PUCHAR, UCHAR, int );
static void ProcessDeferred( void );
static USHORT CpmOpenWindow( UCHAR, USHORT );
static USHORT CpmFindContext();
static USHORT CpmWrite( PUCHAR pBuffer, USHORT Length ); // 

/*
 * External references
 */
//static USHORT SplICAWrite( PUCHAR, UCHAR, USHORT );
//static USHORT SplICAWindowOpen( UCHAR, USHORT );
extern USHORT VirtualCpm;


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
    UCHAR Channel,
    PCHAR pBuf,
    USHORT Size
    );

//
// Function prototypes for per request dispatch functions
//
static USHORT CpmSrvEnumPrinter( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvOpenPrinter( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvClosePrinter( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvWritePrinter( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvGetPrinter( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvConnect( UCHAR, PCHAR, USHORT );
static USHORT CpmSrvNoEnt( UCHAR, PCHAR, USHORT );

//
// Request dispatch table
//

#define CPM_DISPATCH_TABLE_SIZE 9

static PCPM_DISPATCH_ROUTINE CpmSrvDispatchTable [ CPM_DISPATCH_TABLE_SIZE ] = {
    CpmSrvEnumPrinter,
    CpmSrvOpenPrinter,
    CpmSrvClosePrinter,	
    CpmSrvNoEnt, // Reserved for CPM_READPRINTER_REQUEST
    CpmSrvWritePrinter,
    CpmSrvGetPrinter,
    CpmSrvNoEnt, // Reserved for CPM_SETPRINTER_REQUEST
    CpmSrvNoEnt, // Reserved for ASYNC_STATUS_REQUEST
    CpmSrvConnect
};

static UCHAR CpmSrvDispatchTableSize = CPM_DISPATCH_TABLE_SIZE;


/*****************************************************************************
 *
 *  CpmWireDataReceive
 *
 *   This is the main dispatch function for data/requests received
 *   over an ICA connection for a printer.
 *
 * ENTRY:
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
SplWireDataReceive(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    PCPM_OPENPRINTER_REQUEST r;
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
    if( Size < sizeof( CPM_CLOSEPRINTER_REQUEST ) ) {

        TRACE(( TC_CPM, TT_ERROR, "CPMSERV: Short Packet! %d bytes", Size ));

	// If theres a debugger lets catch this
        ASSERT( FALSE, 0 );

        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Use any packet type in order to get the type code.
     * All replys start with the same two bytes.
     */
    r = (PCPM_OPENPRINTER_REQUEST)pBuf;

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

    RetSize = (CpmSrvDispatchTable[Offset])( Channel, pBuf, Size );

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

static USHORT 
CpmSrvNoEnt(
    UCHAR Channel,
    PCHAR pBuf,
    USHORT Size
    )
{

    TRACE(( TC_CPM, TT_ERROR, "CPMSRV: No ent called! Channel %d, Size %d", Channel, Size));

    // If theres a debugger lets catch this
    ASSERT( FALSE, 0 );

    // Open the ICA window
    CpmOpenWindow( Channel, (USHORT)Size );

    return( 0 );
}

/*****************************************************************************
 *
 *  CpmSrvClosePrinter
 *
 *  Close an open printer
 *
 *  NOTE: There is no reply for this function, the client assumes it
 *        either always succeeds, or the transport has broken the
 *        connection.
 *
 * ENTRY:
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

static USHORT 
CpmSrvClosePrinter(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    )
{

    POPENCONTEXT p;
    PCPM_CLOSEPRINTER_REQUEST r;
    CPM_CLOSEPRINTER_REQUEST_REPLY rep;
    USHORT Result;

    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof( CPM_CLOSEPRINTER_REQUEST ) ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_CLOSEPRINTER_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CPM_TYPE_CLOSEPRINTER, 0 );

    /*
     * Check the context
     */
    if( (r->Context > MAX_PRINTERS) ||
        ((p=OpenPorts[r->Context])==NULL) ) {

	// Not open
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
        goto Done;
    }

    TRACE(( TC_CPM, TT_API1, "CpmClose: Close Successfull on Channel %d, Context %d", Channel,r->Context));

    Result = CpmClosePrinter( p->hPrinter );

    /*
     * Mark this channel as closed
     */

    OpenPorts[r->Context] = NULL;

    /*
     * Free the OPENCONTEXT resources
     */
    free( p );

    // fall through to reply

Done:
    rep.h_type = CPM_TYPE_CLOSEPRINTER_REPLY;
    rep.MpxId = r->MpxId;
    rep.Result = Result;

    /*
     * Open the ICA Window
     */
    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * Send the final result. The printer is closed regardless,
     * but the result allows us to detect if the start job
     * has failed so we can give feed back to the user.
     */
    CpmWrite( (PCHAR)&rep, sizeof( CPM_CLOSEPRINTER_REQUEST_REPLY ) );

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

static USHORT 
CpmSrvConnect(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    PCPM_CONNECT2_REQUEST rs;

    TRACE(( TC_CPM, TT_API1, "CPM: Got Connect request"));

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_CONNECT2_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    rs = (PCPM_CONNECT2_REQUEST)pBuf;

    TRACE(( TC_CPM, TT_API1, "CPM: Host Versions Low %d, High %d", rs->VersionLow,rs->VersionHigh));

    /*
     * Store the host version numbers
     */
    HostVersionHigh = rs->VersionHigh;
    HostVersionLow =  rs->VersionLow;

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
SplCloseAllContexts( )
{
    UCHAR Context;
    POPENCONTEXT p;

    TRACE(( TC_CPM, TT_API1, "CPM: Closing All Contexts (Disconnect/Unload)"));

    /*
     * Close any open printer contexts
     */

    for( Context = 0; Context < MAX_PRINTERS; Context++ ) {

        p = OpenPorts[Context];

        if( p != NULL ) {

	    CpmClosePrinter( p->hPrinter );

            // Mark the open context as invalid now
            OpenPorts[Context] = NULL;

            // Free the resources
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
SplPollAllPorts( )
{
    USHORT Context;
    int Ret;
    int Result = 0;
    ULONG Status;
    ULONG Filtered;
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
     */
    if( !RequestPrintData ) {

        PollCounter++;
        if( PollCounter < STATUS_POLL_COUNT ) {
            return( Usefull );
        }
        PollCounter = 0;
    }

    /*
     * Now Look at all active PORT contexts for a status change
     */
    for( Context = 0; Context < MAX_PRINTERS; Context++ ) {

        p = OpenPorts[Context];

        if( p == NULL ) {
            continue;
        }

        /*
	 * Poll the printer for any status changes, etc.
         *
	 * This drives an internal state machine for printing
	 * on the DOS client. On the Windows client, it does not
	 * really need to do much since printers spool to disk.
	 */
	Ret = CpmPollPrinter( p->hPrinter, &Status, &Usefull );

        if( !Ret ) {
            continue;
        }

        // At least one is active
	ActivePrinters = TRUE;

	// Filter out bits we do not bother the HOST with
        Filtered = Status & HOST_STATUS_FILTER;

        // Compare with the last status word sent to the host

        if( Filtered != (p->HostStatus & HOST_STATUS_FILTER) ) {

            Usefull = TRUE;

            // Send a Host Status Update Message

            if( CpmHostStatusUpdate( Context, Status ) == 0 ) {

                //
                // Only update the HostStatus if successfull send
                // This is because ICA may not have a buffer, so we will
                // get it next time (hopefully)
                //

                p->HostStatus = Status;
            }
        }
    }

    /*
     * If no more active printer queues, do not bother checking
     * everything on future polls.
     */

    if( ActivePrinters == 0 ) {

#if DBG
	if( RequestPrintData == TRUE ) {
            TRACE((TC_CPM, TT_API4, "PrinterPoll: No Active Printers Now"));
        }
#endif
	RequestPrintData = FALSE;
    }

    return( Usefull );
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
 *    Status
 *     New status to send to the host.
 *
 * EXIT:
 *   0 - no error
 *  !0 - An error, such as could not get an ICA buffer.
 *
 ****************************************************************************/

static int CpmHostStatusUpdate( USHORT Context, ULONG Status )
{
    int rc;
    CPM_ASYNC_STATUS r;

    TRACE(( TC_CPM, TT_API1, "CPMSERV: Sending Status Update 0x%x, Context %d", Status, Context ));

    r.h_type = CPM_TYPE_ASYNC_STATUS;
    r.Context = Context;
    r.Status = Status;

    // Now try to send it over the ICA on the cpm channel

    rc = SplICAWrite( (PUCHAR)&r, (UCHAR)VirtualCpm, sizeof( CPM_ASYNC_STATUS ) );

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
 *  CpmSrvGetPrinter
 *
 *  Handle a get printer status request from the host
 *
 * ENTRY:
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

static USHORT 
CpmSrvGetPrinter(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    USHORT Result;
    ULONG Status;
    PCPM_GETPRINTER_REQUEST r;
    CPM_GETPRINTER_REQUEST_REPLY rep;
    POPENCONTEXT p;

    ASSERT( (Channel == VirtualCpm), Channel );

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_GETPRINTER_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_GETPRINTER_REQUEST)pBuf;

    ASSERT( r->h_type == CPM_TYPE_GETPRINTER, 0 );

    /*
     * Check the context
     */
    if( (r->Context > MAX_PRINTERS) ||
        ((p=OpenPorts[r->Context])==NULL) ) {

	// Not open
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
        goto SendReply;
    }

    Status = p->HostStatus;

    Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

    // fallthrough to send reply

SendReply:

    /*
     * Open the ICA window
     */
    CpmOpenWindow( Channel, (USHORT)Size );

    rep.h_type = CPM_TYPE_GETPRINTER_REPLY;
    rep.MpxId = r->MpxId;
    rep.Status = Status;
    rep.Flags = 0;
    rep.InfoLen = 0;
    rep.Result = Result;

    /*
     * Send the reply
     */
    CpmWrite( (PCHAR)&rep, sizeof_CPM_GETPRINTER_REQUEST_REPLY );

    return( Size );
}


/*****************************************************************************
 *
 *  CpmSrvEnumPrinter
 *
 *  Handle an EnumPrinter request from the host
 *
 * ENTRY:
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

static USHORT 
CpmSrvEnumPrinter(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    PCPM_ENUMPRINTER_REQUEST r;
    CPM_ENUMPRINTER_REQUEST_REPLY rep;
    USHORT TotalSize, Count, RetVal;
    PCHAR  prep;
    PCHAR  p;

    /*
     * These new requests can only operate over
     * the client-server channel.
     */
    ASSERT( (Channel == VirtualCpm), Channel );

    /*
     * Check the size
     */
    if( Size != sizeof_CPM_ENUMPRINTER_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_ENUMPRINTER_REQUEST)pBuf;

    ASSERT( r->h_type == CPM_TYPE_ENUMPRINTER, 0 );

    /*
     * Setup main reply header body
     */
    memset( &rep, 0, sizeof(rep) );
    rep.h_type = CPM_TYPE_ENUMPRINTER_REPLY;
    rep.MpxId = r->MpxId;

    p = malloc( r->DataSize + sizeof(CPM_ENUMPRINTER_REQUEST_REPLY) );
    if( p == NULL ) {

        rep.Result = CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOFILES );
        prep = (PCHAR)&rep;
        TotalSize = sizeof( rep );
	goto SendReply;
    }

    /*
     * Call function in lptspool.lib to enumerate the printers
     * into our marshalling buffer
     */
    RetVal = CpmEnumPrinter(
                 r->Index,
                 p + sizeof(rep),
                 r->DataSize,
	         &Count,
	         &TotalSize
	         );

    //
    // No files can have valid return data
    //
    if( RetVal &&
        (CPM_DOSERROR_CODE(RetVal) != CPM_DOSERROR_NOFILES) ) {

	TRACE((TC_CPM, TT_ERROR, "Client Printer EnumPrinter: Error on Index %d", r->Index));
        rep.Result = RetVal;
        free( p );
	prep = (PCHAR)&rep;
        TotalSize = sizeof( rep );
	goto SendReply;
    }

    TRACE((TC_CPM, TT_API2, "Client Printer EnumPrinter: Returned %d printers Index %d", Count, r->Index));

    /*
     * The returned buffer after our reply header has been filled
     * in by the CpmEnumPrinter function.
     */
    rep.Count = Count;     // Number of returned printers
    rep.Size  = TotalSize; // Size of return data
    rep.Result = RetVal;

    /*
     * Copy the reply into the reserved space at the
     * begining of the marshall buffer
     */
    memcpy( p, &rep, sizeof(rep) );

    TotalSize += sizeof(rep);

    // Set the reply pointer
    prep = p;

SendReply:

    /*
     * Open the ICA window
     */
    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * Now send the reply
     */
    CpmWrite( (PUCHAR)prep, TotalSize );

    // If we allocated a marshall buffer, free it
    if( prep != (PCHAR)&rep ) {
        free( prep );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CpmSrvOpenPrinter
 *
 *  Handle an open request for a printer from a client on
 *  the Client-Server channel.
 *
 * ENTRY:
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

static USHORT 
CpmSrvOpenPrinter(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    USHORT  TotalSize;
    USHORT Result;
    USHORT Context;
    int    Handle;
    PCPM_OPENPRINTER_REQUEST r;
    CPM_OPENPRINTER_REQUEST_REPLY rep;
    POPENCONTEXT p;
    PCHAR pName;
    PCHAR pPathNameBuf;

    /*
     * Must be on the client-server channel
     */
    ASSERT( (Channel == VirtualCpm), Channel );

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpenPrinter: Called"));

    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof( CPM_OPENPRINTER_REQUEST ) ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_OPENPRINTER_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CPM_TYPE_OPENPRINTER, 0 );

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof( CPM_OPENPRINTER_REQUEST );
    TotalSize += r->NameSize;

    if( TotalSize != Size ) {

       TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpenPrinter: Bad Packet Size %d, SB %d",Size, TotalSize));

       // Send an error reply so we do not hang the client
       rep.Result = CPM_MAKE_STATUS( CPM_ERROR_BADFORMAT, CPM_DOSERROR_BADLENGTH );
       Context = (USHORT)(-1);
       goto SendReply;
    }


    /*
     * Windows printers do not need to be opened exclusive, since the
     * spooler will abritrate requests amoung the many apps.
     */
    if( !(r->AccessMode & CPM_WRITEACCESS) ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpenPrinter: BadAccess Mode 0x%x", r->AccessMode));

        // Bad open modes
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_BADACCESS );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    /*
     * Try and find a free context entry
     */
    Context = CpmFindContext();
    if( Context == (-1) ) {

	TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpenPrinter: No Free Context entries"));

        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_UNKNOWN );
        goto SendReply;
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof( CPM_OPENPRINTER_REQUEST ) );

    /*
     * Get the name into a NULL terminated buffer
     */
    pPathNameBuf = (PCHAR)malloc( r->NameSize+1 );
    if( pPathNameBuf == NULL ) {
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOMEM );
        Context = (USHORT)(-1);
        goto SendReply;
    }

    strncpy( pPathNameBuf, pName, r->NameSize );
    pPathNameBuf[r->NameSize] = 0;

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpenPrinter: Calling LptOpenPrinter on Name %s", pPathNameBuf));

    Result = CpmOpenPrinter( pPathNameBuf, &Handle );

    if( Result != 0 ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmSrvOpenPrinter: Could Not Open Printer %s, Result 0x%x", pName, Result));

        free( pPathNameBuf );

	// Map the error code and return the error
        Context = (USHORT)(-1);
        goto SendReply;
    }

    free( pPathNameBuf );

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

    p->State = OPENCONTEXT_OPEN;
    p->hPrinter = Handle;
    p->WindowAckSize = 0;
    p->HostStatus = 0;

    OpenPorts[Context] = p;

    Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

    TRACE(( TC_CPM, TT_API1, "CpmSrvOpenPrinter: Success, Context %d", Context));

SendReply:

    /*
     * Open the ICA window
     */

    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CPM_TYPE_OPENPRINTER_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context; // This context represents the open file to the OS routines
    rep.Result = Result;   // Error codes as in cpmwire.h

    /*
     * Now send the reply
     */
    CpmWrite( (PUCHAR)&rep, sizeof_CPM_OPENPRINTER_REQUEST_REPLY );

    return( Size );
}

/*****************************************************************************
 *
 *  CpmSrvWritePrinter
 *
 *  Handle a printer write request from the host on
 *  the client-server channel.
 *
 * ENTRY:
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

static USHORT 
CpmSrvWritePrinter(
    UCHAR  Channel,
    PCHAR  pBuf,
    USHORT Size
    )
{
    USHORT Result;
    POPENCONTEXT p;
    PCHAR  pWriteBuf;
    PCPM_WRITEPRINTER_REQUEST r;
    CPM_WRITEPRINTER_REQUEST_REPLY rep;
    USHORT AmountWrote = 0;

    /*
     * Make sure the data is at least the
     * size of the shorter header
     */
    if( Size < sizeof_CPM_WRITEPRINTER_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        CpmOpenWindow( Channel, (USHORT)Size );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCPM_WRITEPRINTER_REQUEST)pBuf;

    ASSERT( (r->h_type == CPM_TYPE_WRITEPRINTER), 0 );

    /*
     * Get a pointer to the write data following the packet header
     */
    pWriteBuf = (PCHAR)( pBuf + sizeof_CPM_WRITEPRINTER_REQUEST );

    /*
     * Check the context
     */
    if( (r->Context > MAX_PRINTERS) ||
        ((p=OpenPorts[r->Context])==NULL) ) {

	// Not open
        Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
        goto SendReply;
    }

    /*
     * If its a write of 0 bytes, return now
     */
    if( r->WriteSize == 0 ) {

        TRACE(( TC_CPM, TT_ERROR, "CpmWrite: 0 Length Write on Context %d", r->Context));

        Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );
        goto SendReply;
    }

    /*
     * Call the printer write function
     */
    Result = CpmWritePrinter(
                 p->hPrinter,
		 pWriteBuf,
		 r->WriteSize,
		 &AmountWrote
		 );

    // fall through to the reply

SendReply:

    TRACE(( TC_CPM, TT_API4, "CPM: Wrote %d Bytes Print Data Channel Result 0x%x", AmountWrote, Result));

    rep.h_type = CPM_TYPE_WRITEPRINTER_REPLY;
    rep.MpxId = r->MpxId;
    rep.AmountWrote = AmountWrote;
    rep.Result = Result;

    /*
     * Open the ICA window
     */
    CpmOpenWindow( Channel, (USHORT)Size );

    /*
     * Send the reply
     */
    CpmWrite( (PUCHAR)&rep, sizeof_CPM_WRITEPRINTER_REQUEST_REPLY );

    return( Size );
}

/*****************************************************************************
 *
 *  FindContext
 *
 *   Returns a free context entry
 *
 * ENTRY:
 *   Param1 (input/output)
 *     Comments
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

static USHORT 
CpmFindContext()
{
    USHORT Context;

    for( Context=0; Context < MAX_PRINTERS; Context++ ) {

        if( OpenPorts[Context] == NULL ) {
            return( Context );
	}
    }

    return( (USHORT)(-1) );
}

/*****************************************************************************
 *
 *  CpmWrite
 *
 *   This module handles writing to ICA.
 *
 *   Since we can not always get an outbuf, this will handle queueing
 *   the I/O for a deferred send if no ICA output buffer space is available.
 *
 *   Also handles queueing if a preceding send is queued, so that replys
 *   do not get re-ordered.
 *
 * ENTRY:
 *   pBuffer (input)
 *     Pointer to data buffer to write
 *
 *   Length (input)
 *     Amount of data in buffer
 *
 * EXIT:
 *   0 - no error
 *
 ****************************************************************************/

static USHORT 
CpmWrite( PUCHAR pBuffer, USHORT Length )
{
    int Ret;

    /*
     * First make sure nothing is in the deferred queue in front of us
     */
    if( DeferredReply != NULL ) {

        // We queue this right away so we do not re-order requests/replies
        if( !QueueDeferred( DEFERRED_USER_DATA, pBuffer, (UCHAR)VirtualCpm, Length) ) {
            TRACE((TC_CPM, TT_ERROR, "CPM: CpmWrite: Dropped Write, Channel %d, Length", VirtualCpm, Length));
            return( CLIENT_ERROR_NO_MEMORY );
	}
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     * Attempt the write
     */
    Ret = SplICAWrite( pBuffer, (UCHAR)VirtualCpm, Length );

    if( Ret ) {
        if( !QueueDeferred( DEFERRED_USER_DATA, pBuffer, (UCHAR)VirtualCpm, Length) ) {
            TRACE((TC_CPM, TT_ERROR, "CPM: CpmWrite: Dropped Write1, Channel %d, Length", VirtualCpm, Length));
            return( CLIENT_ERROR_NO_MEMORY );
	}
    }

    return( CLIENT_STATUS_SUCCESS );
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

static USHORT 
CpmOpenWindow( UCHAR Channel, USHORT WindowSize )
{
    USHORT rc;

    /*
     * First make sure nothing is in the deferred queue in front of us
     */
    if( DeferredReply != NULL ) {

        // We queue this right away so we do not re-order requests/replies
        if( !QueueDeferred( DEFERRED_WINDOW_ACK, NULL, Channel, WindowSize) ) {
            TRACE((TC_CPM, TT_ERROR, "CPM: CpmOpenWindow: Dropped Ack, Channel %d, WindowSize", Channel, WindowSize));
            return( CLIENT_ERROR_NO_MEMORY );
	}
        return( CLIENT_STATUS_SUCCESS );
    }

    // Try and send the ICA window update
    rc = SplICAWindowOpen( Channel, WindowSize );

    if( rc != CLIENT_STATUS_SUCCESS ) {

        // We must defer it if its CLIENT_ERROR_NO_OUTBUF
        if( rc == CLIENT_ERROR_NO_OUTBUF ) {

            if( !QueueDeferred( DEFERRED_WINDOW_ACK, NULL, Channel, WindowSize) ) {
                TRACE((TC_CPM, TT_ERROR, "CPM: CpmOpenWindow: Dropped Ack1, Channel %d, WindowSize", Channel, WindowSize));
                return( CLIENT_ERROR_NO_MEMORY );
	    }
            return( CLIENT_STATUS_SUCCESS );
        }
        else {
            TRACE(( TC_CPM, TT_ERROR, "CPMOPENWINDOW: Could Not Send Window Open rc %d",rc));
            return(rc);
        }
    }

    return( rc );
}


/*****************************************************************************
 *
 *  QueueDeferred
 *
 *   Occasionally, requests may not be able to reply since there is no
 *   ICA send buffer space.
 *
 *   When this occurs, the reply is queued into dynamically allocated memory and
 *   sent at poll() time.
 *
 *   This happens only occasionally, and the replies are typically small so
 *   this should not represent much overhead.
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

static CPM_BOOLEAN 
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


static void
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
        Ret = SplICAWrite( Tmp->Buf,
                           Tmp->Channel,
                           (USHORT)Tmp->Size
                         );

    }
    else if( Tmp->Type == DEFERRED_WINDOW_ACK ) {

        TRACE((TC_CPM, TT_API1, "Client Printer: Attempting To Send Window Ack %d", Tmp->Channel));

        // Process the WINDOW_ACK (Virtual Ack) Type
        // BUGBUG: Tmp->Size is an "int" must cast as a "USHORT"
        Ret = SplICAWindowOpen( Tmp->Channel, (USHORT)Tmp->Size );
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


