/*************************************************************************
*
* cdmserv.c
*
*  Server side of Citrix Client Drive Mapping remote file access protocol.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 01/18/94
*
* Log:
*
*
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
//#include "cdmdosio.h"

/*
 * General notes on this module:
 *
 * This is a single threaded server implementation of the CDM wire
 * protocol. Its primary intent is as a DOS 3.x targeted server.
 *
 * This server does no caching, other than buffers needed to service a
 * given request. Once the request is serviced, the buffer is re-used
 * for the next request. The CDM client (WinView Server) does all of the
 * caching.
 *
 * This server has two primary interfaces. The first is the transport
 * interface in which incoming requests arrive, and replys are sent out
 * on. The second is the operating system file and directory access
 * routines.
 *
 * Even though DOS 3.x file access semantics apply, this server is not
 * tied to DOS, and has been run as a native NT WIN32 console program
 * for CDM development and debugging. A separate module, cdmdosio.c handles
 * the actual DOS 3.x low level file access details. (int 21H calls)
 *
 * The NT version of the low level file access routines is cdmntio.c
 *
 * The operating system low level file access handling module is responsible
 * for allocating and freeing the Context (FileId) identifiers which
 * represent an open file and/or directory. These are returned from the
 * operating system specific OsOpen(), OsCreate(), and OsFindFirst() calls,
 * and released by either OsClose(), or OsFindClose(). These are unsigned
 * shorts, allowing up to 65534 open files. One place is reserved (-1) to
 * represent an invalid Context or FileId. This wire protocol server module
 * will not interpret these numbers in any way (except for (-1)), but just
 * pass them on through the request and reply packet headers on behalf of the
 * OS specific modules.
 *
 * In order to properly associate reply's with their original requests, a
 * unique 8 bit MpxId is supplied in the request packets, and must be passed
 * back to the client unmodified or interpreted when the request's particular
 * reply has been sent.
 *
 * These MpxId's allow the server to be multithreaded, but at the current
 * time it is single threaded to perform under the DOS 3.x environment.
 *
 * The client will issue multiple requests using these MpxId's in order to
 * overlap requests and get better performance from high latency networks
 * and modems.
 *
 * The Context field of the requests and replies is for use by this server
 * uninterpreted, or altered by the client. These are used internally to
 * match up the data structures that represent an open file and/or directory
 * context.
 *
 */

/*
 * Definitions of the transport interface that this module uses.
 *
 * NOTE: These are the same transport routines defined for cdmwire.c,
 *       the client side protocol.
 *
 * This module has been designed to be independent of the transport used,
 * so only a few generic transport service calls, and opaque data types are
 * needed.
 *
 * Every routine is passed a PVOID that represents the unique transport
 * a request is to be sent out on. The routine is to pass this through
 * to the proper transport to send data, and for get transfer size calls.
 *
 * The Cdm wire server routines are called by the dispatch function when
 * the transport delivers a whole data packet to this module. They handle
 * the per protocol function argument interpretation and servicing of the
 * request, and callout to the OS specific I/O routines to perform file
 * I/O on behalf of the client.
 *
 * The function CdmWireDataReceive() is called by the transport provider
 * when a request is received from the client. This contains requests and/or
 * data from the client. This function will dispatch the proper handlers for
 * each function.
 *
 * Currently, the CITRIX upper layers will always send in whole packets to
 * CdmWireDataReceive(). If this assumption changes in the future, code will
 * be required to buffer partial packets and 'build' them into whole ones.
 *
 * All send data is also done as whole packets and data. The transport send
 * routine will return an error if it can not send all of the requested
 * data, without transfering any. This is to ensure that a whole packet is
 * received by the other end of the protocol.
 *
 *
 * Transport Functions:
 *
 * int CdmTransportSendData(PVOID  pTransport,
 *                          PCHAR  Buf,
 *                          USHORT Size,
 *                          USHORT Final )
 *
 *   This routine will send data out on the tranport specified by
 *   pTransport. The assumption is that all of the requested data will
 *   be sent out, or an error will be returned.
 *
 *   This routine can block until space is free to send the data. All callers
 *   must have proper operating system context to allow blocking.
 *
 *   The caller of this routine can call CdmTransportMaxSize()
 *   in order to know what is the maximum amount that can be written.
 *   This function should be implemented as a macro by the transport provider
 *   to allow it to be called frequently without much overhead.
 *
 *   The Final flag when true indicates that this transfer should be sent out
 *   on the wire immediately, for this is the final send in a sequence.
 *   When false, the data is not to be sent since more will be coming
 *   in the next call(s) to Send. The total of all requests up to and including
 *   the one with Final == TRUE must be less than or equal to the maximum
 *   buffer size for the transport. An overflow condition during a sequence
 *   will "abort" or throw away the data written since the last time
 *   when Final == TRUE was set.
 *
 *   This routine returns 0 for success, and a non-zero error code on error.
 *
 *   The wire protocol will always send packet headers as whole units.
 *   Any data following may be in separate write requests. Since the packet
 *   headers specify the exact size of following data, the amount that
 *   needs to be read again is known. Since the protocol keeps record
 *   boundries, the next packet header will be aligned correctly in
 *   the data stream. For smaller writes, an attempt should be made to
 *   have the data sent in one transport packet by using the Final
 *   flag. Sending multiple units should only be done for larger
 *   transfers.
 *
 * USHORT CdmTransportMaxSize( PVOID pTransport )
 *
 *   This routine returns the maximum amount of data that can be
 *   sent or received on the specified transport.
 *
 *   This assumes that both directions have the same buffer size
 *   limitations and guarantees.
 *
 *   This also assumes that the same maximum send size remains true
 *   for the life of this transport connection. (IE: It never skrinks
 *   from one request to another)
 *
 *
 * This function is called by the transport when data arrives for
 * us to process.
 *
 * USHORT CdmWireDataReceive( PVOID pTransport, PCHAR pBuf, USHORT Size )
 *
 * This function returns the amount of data 'accepted'. It should
 * always accept all the data, but this allows an error to be signaled
 * by returning an amount less than the Size argument. The transport will
 * throw away unaccepted data without any re-transmission. This is because
 * all data presented to here is from a 'reliable' connection.
 *
 */

///////////////////////////////////////////////////////////////////////////////////
//
//
//              Main Request Dispatch Table and Routines
//
//    These handle a request received from a transport provider on
//    behalf of a client.
//
//    The routines here are dispatched from the transport buffer by
//    CdmWireDataReceive() through the function dispatch table. These
//    routines unmarshall the request parameters, calculate any request
//    data buffer addresses, and then handle the request calling the OS
//    specific I/O interface routines. Reply's are sent using the
//    transport routines CdmTransportSendData(), and CdmTransportMaxSize().
//
//    The client uses the MpxId to match replys with their originating
//    request. This MpxId is passed through by the server unmodified, or
//    interpreted.
//
//    The server uses the Context field of many of the requests and replies
//    for its own internal use. This is for tracking any data structures
//    to represent an open file on behalf of the client. The client passes
//    this through unmodified or interpreted.
//
//    At the current time, it is assumed that all data following a
//    header is available in the transport buffer supplied to
//    CdmWireDataReceive().
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
// ReceiveData Queue header (used to hold receive data until user answers
//                           the security popup)
//
typedef struct _RCVDATAQ {
   struct _RCVDATAQ *pNext;
   PVOID            pTransport;
   USHORT           Size;
   CHAR             Buf[1];
} RCVDATAQ, *PRCVDATAQ;

//
// All per request dispatch functions have this prototype
// for the dispatch table
//

typedef
USHORT
(*PCDM_DISPATCH_ROUTINE)(
    PVOID pTransport,
    PCHAR pBuf,
    USHORT Size
    );

//
// Function prototypes for per request dispatch functions
//

USHORT STATIC CdmSrvCreate( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvOpen( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvClose( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvRead( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvWrite( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFindFirst( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFindNext( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFindClose( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvGetAttr( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvSetAttr( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvGetDateTime( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvSetDateTime( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvDelete( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvRename( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvCreateDir( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvDeleteDir( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvReadCond( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFileLock( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFileUnLock( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFileChangeSize( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvSeek( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvVolumeInfo( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvFindFirstIndex( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvConnect( PVOID, PCHAR, USHORT );
USHORT STATIC CdmSrvNoEnt( PVOID, PCHAR, USHORT );


//
// Request dispatch table
//

#define CDM_DISPATCH_TABLE_SIZE 25

STATIC PCDM_DISPATCH_ROUTINE CdmSrvDispatchTable [ CDM_DISPATCH_TABLE_SIZE ] = {
    CdmSrvCreate,
    CdmSrvOpen,
    CdmSrvClose,
    CdmSrvRead,
    CdmSrvWrite,
    CdmSrvFindFirst,
    CdmSrvFindNext,
    CdmSrvFindClose,
    CdmSrvGetAttr,
    CdmSrvSetAttr,
    CdmSrvGetDateTime,
    CdmSrvSetDateTime,
    CdmSrvDelete,
    CdmSrvRename,
    CdmSrvCreateDir,
    CdmSrvDeleteDir,
    CdmSrvNoEnt,               // Place holder for Notify cache
    CdmSrvReadCond,
    CdmSrvFileLock,
    CdmSrvFileUnLock,
    CdmSrvFileChangeSize,
    CdmSrvSeek,
    CdmSrvVolumeInfo,
    CdmSrvConnect,
    CdmSrvFindFirstIndex
};

STATIC UCHAR CdmSrvDispatchTableSize = CDM_DISPATCH_TABLE_SIZE;

/*
 * The I/O buffer(s) contain entries which specifies which files
 * data is in the buffer along with the offset and size.
 *
 * This is to allow the implementation of a read-ahead buffer
 * that can read more blocks than requested to increase
 * performance.
 */

// Allow up to a (normal) cluster of I/O
#define IO_BUFFER_SIZE 4096

// Define the amount of read ahead to attempt
#define READ_AHEAD_SIZE IO_BUFFER_SIZE

typedef struct _IO_ENTRY {

    unsigned long  Offset;    // File Offset of data in buffer
    unsigned int   ValidSize; // Bytes of valid data in buffer
    short          Context;   // File context. (-1) means buffer invalid

    unsigned char  IoBuffer[IO_BUFFER_SIZE]; // The I/O buffer

} IO_ENTRY, *PIO_ENTRY;

static IO_ENTRY IoBuffer;

// static unsigned char IoBuffer[IO_BUFFER_SIZE];

#define CdmAllocateIOBuffer() ( &IoBuffer )

#define CdmFreeIOBuffer( p ) (p->Context = (-1))

#define CdmFreeValidIOBuffer( p )

// Version numbers from host we are communicating with.
STATIC USHORT CdmCltVersionMin = CDM_MIN_VERSION;
STATIC USHORT CdmCltVersionMax = CDM_MIN_VERSION;  // Until host tells us otherwise

/*
 *  Begin: wkp Security stuff
 */

//  return codes
#define CDM_SECURITY_ACCESS_ACCEPTED  0
#define CDM_SECURITY_ACCESS_DENIED    1
USHORT STATIC CdmSecuritySetAccess( ULONG fAccessRequired );
USHORT STATIC CdmSecurityVerifyAccess( ULONG fAccessRequested );
BOOL   STATIC QueueReceiveData(PVOID pTransport, PCHAR pBuf, USHORT Size);


//  default access
#define CDM_SECURITY_ACCESS_UNKNOWN      0xFFFFFFFFL
static ULONG fSecurityAccess = CDM_SECURITY_ACCESS_UNKNOWN;
static ULONG fSecurityPopup  = FALSE;
static PRCVDATAQ pRcvDataHead = NULL;

extern PFNSTATUSMESSAGEPROC gpfnStatusMsgProc;
#ifndef DOS
// external from the Engine
extern BOOL gbIPCEngine;
#endif

/*
 *  End: wkp Security stuff
 */



/*****************************************************************************
 *
 *  CdmWireDataReceive
 *
 *   This is the main dispatch function for data/requests received
 *   over a transport. This is called by the transport provider
 *   passing us an opaque handle, along with a virtual address buffer
 *   pointer, and size.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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
CdmWireDataReceive(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT Size
    )
{
    PCDM_CREATE_REQUEST r;
    USHORT RetSize;
#ifdef REQUEST_REORDER_OK // Kill warning
    int ret;
#endif

    /*
     * Make sure the data is at least the
     * size of our smallest request
     */
    if( Size < sizeof( CDM_CLOSE_REQUEST ) ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Use any packet type in order to get the type code.
     * All replys start with the same two bytes.
     */
    r = (PCDM_CREATE_REQUEST)pBuf;

    // Make sure a malicous/buggy client does not crash us

    if( r->h_type > CdmSrvDispatchTableSize ) {
        TRACE(( TC_CDM, TT_API1, "CDMSERV: Type > DispTable, %d", r->h_type ));
        return( 0 );
    }

    TRACE(( TC_CDM, TT_API3, "CDMSERV: Dispatching Request %d, Size %d", r->h_type, Size));

    // dispatch based on packet id to functions

    RetSize = (CdmSrvDispatchTable[r->h_type])( pTransport, pBuf, Size );

    // Look for definition/packing errors in the protocol
    ASSERT( RetSize == Size, 0 );

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvNoEnt
 *
 *  No entry error function for empty table entries
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvNoEnt(
    PVOID pTransport,
    PCHAR pBuf,
    USHORT Size
    )
{

    TRACE(( TC_CDM, TT_API1, "CDMSRV: No ent called! pTransport 0x%x, Size %d", pTransport, Size));

    // If theres a debugger lets catch this
    ASSERT( FALSE, 0 );
    return( 0 );
}


/*****************************************************************************
 *
 *  CdmSrvCreate
 *
 *  Handle a Create file request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvCreate(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    USHORT Context;
    PCDM_CREATE_REQUEST r;
    CDM_CREATE_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_CREATE_REQUEST ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_CREATE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_CREATE, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_OPEN_CREATE ) ) {

       if(!fSecurityPopup) {
          // Send an error reply so we do not hang the client
          rep.h_type = CDM_TYPE_CREATE_REPLY;
          rep.MpxId = r->MpxId;
          rep.Context = (USHORT)(-1);
          rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );
          
          // If transport problems, we can't do anything anyway
          CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_CREATE_REQUEST_REPLY ),
                                TRUE
                              );
          
          return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }

    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_CREATE_REQUEST + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_CREATE_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_CREATE_REQUEST_REPLY,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_CREATE_REQUEST );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosCreate( pName,
                  r->PathNameSize,
                  r->AccessMode,
                  r->CreateMode,
                  r->Attributes,
                  &Context,
                  &Result
                );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_CREATE_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context; // This context represents the open file to the OS routines
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_CREATE_REQUEST_REPLY, 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_CREATE_REQUEST_REPLY,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvOpen
 *
 *  Handle an open file request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvOpen(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    USHORT Context;
    PCDM_OPEN_REQUEST r;
    CDM_OPEN_REQUEST_REPLY rep;
    ULONG  fAccessMode;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_OPEN_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_OPEN_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_OPEN, 0 );

    /*
     * WKP: Security check for internet client
     */

    if( (r->AccessMode & CDM_ACCESS_MASK) == CDM_READACCESS ) {
        fAccessMode = CDM_SECURITY_ACCESS_OPEN_READ;
    }
    else {
        fAccessMode = CDM_SECURITY_ACCESS_OPEN_WRITE;
    }

    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( fAccessMode ) ) {

       if(!fSecurityPopup) {
          // Send an error reply so we do not hang the client
          rep.h_type = CDM_TYPE_OPEN_REPLY;
          rep.MpxId = r->MpxId;
          rep.Context = (USHORT)(-1);
          rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );
          
          // If transport problems, we can't do anything anyway
          CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_OPEN_REQUEST_REPLY ,
                                TRUE
                              );
          
          return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_OPEN_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_OPEN_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_OPEN_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_OPEN_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosOpen( pName,
                r->PathNameSize,
                r->AccessMode,
                &Context,
                &Result
              );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_OPEN_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context; // This context represents the open file to the OS routines
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_OPEN_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_OPEN_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvClose
 *
 *  Close an open file freeing the FileId.
 *
 *  NOTE: There is no reply for this function, the client assumes it
 *        either always succeeds, or the transport has broken the
 *        connection.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvClose(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    USHORT Result;
    PCDM_CLOSE_REQUEST r;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_CLOSE_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_CLOSE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_CLOSE, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosClose( r->Context,
                 &Result
               );

    /*
     * We have no way of handling an error from CdmDosClose, since
     * there is no reply.
     *
     * The only reason for an error would be a bad Context, so we
     * ASSERT here in order to catch any bugs during development.
     */
    ASSERT( Result == CDM_ERROR_NONE, 0 );

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvRead
 *
 *  Handle a Read file request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvRead(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PIO_ENTRY p;
    PCDM_READ_REQUEST r;
    CDM_READ_REQUEST_REPLY rep;
    USHORT Result;
    USHORT TotalRead, ToRead;
    USHORT TimeStamp, DateStamp;

    TotalRead = 0;

    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_READ_REQUEST  ) {

        // If theres a debugger lets catch this
        TRACE(( TC_CDM, TT_API1, "CdmSrvRead: Size %d != sizeof(READ_REQUEST)", Size));
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_READ_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_READ, 0 );


    /*
     * Get a read buffer
     *
     * This limits the amount we can reply to in a single read
     * request. This is because the reply header must be written
     * to the transport before the data. If an error occurs from
     * a multiple read and transport send sequence, we can not report
     * the error, or the short read size since the header has already
     * been sent.
     */
    p = CdmAllocateIOBuffer();

    ASSERT( p != NULL, 0 );

    /*
     * This assertion aids in debugging mismatches between
     * the clients view of a maximum read size, and the servers.
     *
     * In the nondebug version, a short read reply is returned, allowing
     * correct function.
     */
    TRACE(( TC_CDM, TT_API3, "CDMSERV: r->ReadSize %d, IO_BUFFER_SIZE %d", r->ReadSize, IO_BUFFER_SIZE));

    ASSERT( r->ReadSize <= IO_BUFFER_SIZE, r->ReadSize );

#ifdef DO_READ_AHEAD
    // See if we have a readahead cache hit
    //
    // Could get more complex and try and tranfer as much as we can
    // from the cache and then read. Won't do this since any I/O is costly
    // and a few more bytes does not matter.
    //
    // One case that would be good to optimize is to keep I/O on 512 byte
    // boundries, and transfer partial reads to allow this. Also round
    // Read-ahead offset down.
    //
    // This could be another big win.
    //

    if( r->Context == p->Context ) {
        // Need to validate r->Context to make sure it is still open

        // See if r->ReadOffset,r->ReadSize is within p->Offset,p->ValidSize

        // If hit bcopy data, skip over the CdmDosRead() this time around.
        if( (r->ReadOffset >= p->Offset) &&
            ( (r->ReadOffset + r->ReadSize) < (pOffset-> + p->ValidSize) ) ) {

            // Hit!
        }
        else {
            // No Hit, must read.
        }
    }
#endif

    ToRead = r->ReadSize;
    if(ToRead > IO_BUFFER_SIZE) ToRead = IO_BUFFER_SIZE;

#ifdef DO_READ_AHEAD
    if( ToRead < READ_AHEAD_SIZE ) ToRead = READ_AHEAD_SIZE;
#endif

    /*
     * Call the OS specific routine to read the data.
     */

    CdmDosRead( r->Context,
                p->IoBuffer,
                ToRead,
                r->ReadOffset,
                &TimeStamp,
                &DateStamp,
                &TotalRead,
                &Result
              );

#ifdef DO_READ_AHEAD
    // Update the Read-ahead entry cache

    if( Result == CDM_ERROR_NONE ) {
        p->Offset = r->ReadOffset;
        p->Context = r->Context;
        p->ValidSize = TotalRead;
    }
    else {
        // Any errors, invalid the buffer
        p->Context = (-1);
    }
#endif

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */
    rep.h_type = CDM_TYPE_READ_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.TimeStamp = TimeStamp;
    rep.DateStamp = DateStamp;
    rep.Result = Result;   // Error codes as in cdmwire.h
    rep.ReturnSize = TotalRead;

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >=
              (sizeof_CDM_READ_REQUEST_REPLY + TotalRead), 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_READ_REQUEST_REPLY ,
                                FALSE
                              );

    // Transports having problems
    if( Ret ) {
       TRACE(( TC_CDM, TT_API1, "CdmSrvRead: Transport Send Error %d", Ret));
#ifdef DO_READ_AHEAD
       CdmFreeValidIOBuffer( p );
#else
       CdmFreeIOBuffer( p );
#endif
       return( 0 );
    }

    // Now write out the data following the reply header

    Ret = CdmTransportSendData( pTransport,
                                p->IoBuffer,
                                (USHORT) TotalRead,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       TRACE(( TC_CDM, TT_API1, "CdmSrvRead: Transport Send Error %d", Ret));
#ifdef DO_READ_AHEAD
       CdmFreeValidIOBuffer( p );
#else
       CdmFreeIOBuffer( p );
#endif
       return( 0 );
    }

    // Free the I/O buffer
#ifdef DO_READ_AHEAD
    CdmFreeValidIOBuffer( p );
#else
    CdmFreeIOBuffer( p );
#endif

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvWrite
 *
 *  Handle a write file request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvWrite(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pWriteBuf;
    USHORT Result;
    USHORT TotalSize;
    USHORT AmountWrote;
    PCDM_WRITE_REQUEST r;
    CDM_WRITE_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_WRITE_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_WRITE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_WRITE, 0 );

    /*
     * Make sure we have the whole command request
     */

    TotalSize = sizeof_CDM_WRITE_REQUEST  + r->WriteSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_WRITE_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = r->Context;
       rep.WroteSize = 0;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_WRITE_REQUEST_REPLY ,
                             TRUE
                           );
       return( 0 );
    }

    /*
     * Get a pointer to the write data following the packet header
     */
    pWriteBuf = (PCHAR)( pBuf + sizeof_CDM_WRITE_REQUEST  );

    /*
     * Call the OS specific routine
     */

    CdmDosWrite( r->Context,
                 pWriteBuf,
                 r->WriteSize,
                 r->WriteOffset,
                 &AmountWrote,
                 &Result
               );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_WRITE_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context; // This context represents the open file to the OS routines
    rep.WroteSize = AmountWrote;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_WRITE_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_WRITE_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvFindFirst
 *
 *  Handle a FindFirst request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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
CdmSrvFindFirst(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    PIO_ENTRY p;
    PCHAR     pFindBuf;
    PCDM_FINDFIRST_REQUEST r;
    CDM_FINDFIRST_REQUEST_REPLY rep;
    USHORT TotalSize;
    USHORT TotalRead;
    USHORT Result;
    USHORT Context;
    USHORT MaxCount;
    UCHAR  EntriesRead;
#ifdef WIN32
    // Only VERSION4 hosts or new can handle long file names
    USHORT LongFileNames = (CdmCltVersionMax >= CDM_VERSION4 );
#else
    USHORT LongFileNames = FALSE;
#endif

    /*
     * Make sure the data is at least the size of the header
     */
    if( Size < sizeof_CDM_FINDFIRST_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FINDFIRST_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FINDFIRST, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_FIND_FIRST ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.NumberReturned = 0;
       rep.BytesReturned = 0;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_FINDFIRST_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.NumberReturned = 0;
       rep.BytesReturned = 0;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }


    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_FINDFIRST_REQUEST  );


    /*
     * Get an I/O buffer
     *
     * This limits the amount we can reply to in a single FindFirst
     * request. This is because the reply header must be written
     * to the transport before the data. If an error occurs from
     * a multiple read and transport send sequence, we can not report
     * the error, or the short read size since the header has already
     * been sent.
     */

    p = CdmAllocateIOBuffer();
    ASSERT( p != NULL, 0 );

    pFindBuf = p->IoBuffer;

    /*
     * This assertion aids in debugging mismatches between
     * the clients view of a maximum read size, and the servers.
     *
     * In the nondebug version, a short read reply is returned, allowing
     * correct function.
     */
    ASSERT( (r->BufferSize ) <= IO_BUFFER_SIZE, 0 );

    /*
     * Determine how many FINDSTRUCTS we can reply to with the given
     * I/O buffer size, but do not overflow our BYTE sized count.
     */
    if( LongFileNames ) {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT_LONG ;
    }
    else {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT ;
    }

    if( MaxCount > 255 ) MaxCount = 255;

    if( (USHORT)r->NumberRequested > MaxCount ) r->NumberRequested = (UCHAR)MaxCount;

    /*
     * Call the OS specific routine to open the directory
     * pathname and read the FINDSTRUCT data and entry names into
     * the buffer.
     */

    TotalRead = 0;
    EntriesRead = 0;

    CdmDosFindFirstIndex( pName,
                          r->PathNameSize,
                          0,               // Start index
                          LongFileNames,
                          pFindBuf,
			  (USHORT)r->BufferSize,
                          (PUSHORT)&TotalRead,
                          r->NumberRequested,
                          &EntriesRead,
                          &Context,
                          &Result
                        );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context;
    rep.Result = Result;   // Error codes as in cdmwire.h
    rep.BytesReturned = TotalRead;
    rep.NumberReturned = EntriesRead;

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >=
              (sizeof_CDM_FINDFIRST_REQUEST_REPLY + TotalRead), 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                                FALSE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Now write out the data following the reply header
    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)pFindBuf,
                                (USHORT)TotalRead,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Free the I/O buffer
    CdmFreeIOBuffer( p );

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvFindNext
 *
 *  Handle a FindNext request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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
CdmSrvFindNext(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PIO_ENTRY p;
    PCHAR     pFindBuf;
    PCDM_FINDNEXT_REQUEST r;
    CDM_FINDNEXT_REQUEST_REPLY rep;
    USHORT Result;
    USHORT MaxCount;
    USHORT TotalRead;
    UCHAR  EntriesRead;
#ifdef WIN32
    USHORT LongFileNames = (CdmCltVersionMax >= CDM_VERSION4 );
#else
    USHORT LongFileNames = FALSE;
#endif


    /*
     * Make sure the data the size of the request header
     */
    if( Size != sizeof_CDM_FINDNEXT_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FINDNEXT_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FINDNEXT, 0 );

    /*
     * Get an I/O buffer
     *
     * This limits the amount we can reply to in a single FindFirst
     * request. This is because the reply header must be written
     * to the transport before the data. If an error occurs from
     * a multiple read and transport send sequence, we can not report
     * the error, or the short read size since the header has already
     * been sent.
     */
    p = CdmAllocateIOBuffer();
    ASSERT( p != NULL, 0 );

    pFindBuf = p->IoBuffer;


    /*
     * This assertion aids in debugging mismatches between
     * the clients view of a maximum read size, and the servers.
     *
     * In the nondebug version, a short read reply is returned, allowing
     * correct function.
     */
    ASSERT( r->BufferSize  <= IO_BUFFER_SIZE, 0 );

    /*
     * Determine how many FINDSTRUCTS we can reply to with the given
     * I/O buffer size, but do not overflow our BYTE sized count.
     */

    if( LongFileNames ) {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT_LONG ;
    }
    else {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT ;
    }

    if( MaxCount > 255 ) MaxCount = 255;

    if( (USHORT)r->NumberRequested > MaxCount ) r->NumberRequested = (UCHAR)MaxCount;


    /*
     * Call the OS specific routine to continue searching
     * the directory and read the FINDSTRUCT data.
     */

    TotalRead = 0;

    CdmDosFindNext( r->Context,
                    0,         // FindNext index
                    LongFileNames,
                    pFindBuf,
                    (USHORT)r->BufferSize,
                    &TotalRead,
                    r->NumberRequested,
                    &EntriesRead,
                    &Result
                  );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FINDNEXT_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.Result = Result;   // Error codes as in cdmwire.h
    rep.BytesReturned = TotalRead;
    rep.NumberReturned = EntriesRead;

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >=
              (sizeof_CDM_FINDNEXT_REQUEST_REPLY + TotalRead), 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FINDNEXT_REQUEST_REPLY ,
                                FALSE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Now write out the data following the reply header

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)pFindBuf,
                                (USHORT)TotalRead,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Free the I/O buffer
    CdmFreeIOBuffer( p );

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvFindClose
 *
 *  Close the open FindFirst/FindNext context freeing the FileId.
 *
 *  NOTE: There is no reply for this function, the client assumes it
 *        either always succeeds, or the transport has broken the
 *        connection.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvFindClose(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    USHORT Result;
    PCDM_FINDCLOSE_REQUEST r;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_FINDCLOSE_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FINDCLOSE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FINDCLOSE, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosFindClose( r->Context,
                     &Result
                   );

    /*
     * We have no way of handling an error from CdmDosFindClose, since
     * there is no reply.
     *
     * The only reason for an error would be a bad Context, so we
     * ASSERT here in order to catch any bugs during development.
     */
    ASSERT( Result == CDM_ERROR_NONE, Result );

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvGetAttr
 *
 *  Handle a get file attributes request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvGetAttr(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    USHORT TotalSize;
    USHORT FileDate, FileTime;
    USHORT Attributes;
    PCHAR  pName;
    ULONG  FileSize;
    PCDM_GETATTR_REQUEST r;
    CDM_GETATTR_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_GETATTR_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_GETATTR_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_GETATTR, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_GET_FINFO ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_GETATTR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_GETATTR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_GETATTR_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_GETATTR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_GETATTR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_GETATTR_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     *
     * The CdmDosGetAttrEx function has been designed to return
     * the files size and its date and time along with the attributes.
     * This information is available through FindFirst.
     *
     * Client requestors tend to ask for most, if not all of this
     * information on a given file, so this cuts down on network
     * latency before responding to the user.
     */

    CdmDosGetAttrEx( pName,
                     r->PathNameSize,
                     &Attributes,
                     &FileSize,
                     &FileDate,
                     &FileTime,
                     &Result
                   );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_GETATTR_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Attributes = Attributes;
    rep.FileSize = FileSize;
    rep.FileDate = FileDate;
    rep.FileTime = FileTime;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_GETATTR_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_GETATTR_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvSetAttr
 *
 *  Handle a Set file attributes request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvSetAttr(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    PCDM_SETATTR_REQUEST r;
    CDM_SETATTR_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_SETATTR_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_SETATTR_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_SETATTR, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_SET_FINFO ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_SETATTR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_SETATTR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_SETATTR_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_SETATTR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_SETATTR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_SETATTR_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosSetAttr( pName,
                   r->PathNameSize,
                   r->Attributes,
                   &Result
                 );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_SETATTR_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_SETATTR_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_SETATTR_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvGetDateTime
 *
 *   Handle a get file date and time request
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvGetDateTime(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    USHORT FileDate, FileTime;
    PCDM_GETDATETIME_REQUEST r;
    CDM_GETDATETIME_REQUEST_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_GETDATETIME_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_GETDATETIME_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_GETDATETIME, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosGetDateTime( r->Context,
                       &FileDate,
                       &FileTime,
                       &Result
                     );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_GETDATETIME_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.FileDate = FileDate;
    rep.FileTime = FileTime;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_GETDATETIME_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_GETDATETIME_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvSetDateTime
 *
 *   Handle a set file date and time request
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvSetDateTime(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    PCDM_SETDATETIME_REQUEST r;
    CDM_SETDATETIME_REQUEST_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_SETDATETIME_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_SETDATETIME_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_SETDATETIME, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosSetDateTime( r->Context,
                       r->FileDate,
                       r->FileTime,
                       &Result
                     );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_SETDATETIME_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_SETDATETIME_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_SETDATETIME_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvDelete
 *
 *  Handle a file delete request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvDelete(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    PCDM_DELETE_REQUEST r;
    CDM_DELETE_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_DELETE_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_DELETE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_DELETE, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_REMOVE ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_DELETE_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_DELETE_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_DELETE_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_DELETE_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_DELETE_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_DELETE_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosDelete( pName,
                  r->PathNameSize,
                  &Result
                );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_DELETE_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_DELETE_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_DELETE_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvRename
 *
 *  Handle a file rename request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvRename(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pOldName, pNewName;
    USHORT Result;
    USHORT TotalSize;
    PCDM_RENAME_REQUEST r;
    CDM_RENAME_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_RENAME_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_RENAME_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_RENAME, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_RENAME ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_RENAME_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_RENAME_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_RENAME_REQUEST  + r->OldPathNameSize + r->NewPathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_RENAME_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_RENAME_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get pointers to the pathnames following the packet header
     */
    pOldName = (PCHAR)( pBuf + sizeof_CDM_RENAME_REQUEST  );
    pNewName = (PCHAR)( pOldName + r->OldPathNameSize );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname strings are NOT NULL terminated!
     */

    CdmDosRename( pOldName,
                  r->OldPathNameSize,
                  pNewName,
                  r->NewPathNameSize,
                  &Result
                );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_RENAME_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_RENAME_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_RENAME_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvCreateDir
 *
 *  Handle a Create directory request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvCreateDir(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    PCDM_CREATEDIR_REQUEST r;
    CDM_CREATEDIR_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_CREATEDIR_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_CREATEDIR_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_CREATEDIR, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_CREATE_DIR ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_CREATEDIR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_CREATEDIR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_CREATEDIR_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_CREATEDIR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_CREATEDIR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_CREATEDIR_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosCreateDir( pName,
                     r->PathNameSize,
                     r->AccessMode,
                     r->Attributes,
                     &Result
                   );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_CREATEDIR_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_CREATEDIR_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_CREATEDIR_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvDeleteDir
 *
 *  Handle a Delete directory request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvDeleteDir(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    USHORT Result;
    USHORT  TotalSize;
    PCDM_DELETEDIR_REQUEST r;
    CDM_DELETEDIR_REQUEST_REPLY rep;


    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_DELETEDIR_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_DELETEDIR_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_DELETEDIR, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_REMOVE_DIR ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_DELETEDIR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_DELETEDIR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_DELETEDIR_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_DELETEDIR_REPLY;
       rep.MpxId = r->MpxId;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_DELETEDIR_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_DELETEDIR_REQUEST  );

    /*
     * Call the OS specific routine
     *
     * NOTE: The pathname string is NOT NULL terminated!
     */

    CdmDosDeleteDir( pName,
                     r->PathNameSize,
                     &Result
                   );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_DELETEDIR_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_DELETEDIR_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_DELETEDIR_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvReadCond
 *
 *  Handle a Read Conditional file request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvReadCond(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int Ret;
    PIO_ENTRY  p;
    PCDM_READCOND_REQUEST r;
    CDM_READCOND_REQUEST_REPLY rep;
    USHORT TotalRead;
    USHORT Result, Valid;
    USHORT TimeStamp, DateStamp;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_READCOND_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_READCOND_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_READCOND, 0 );


    /*
     * Get a read buffer
     *
     * This limits the amount we can reply to in a single read
     * request. This is because the reply header must be written
     * to the transport before the data. If an error occurs from
     * a multiple read and transport send sequence, we can not report
     * the error, or the short read size since the header has already
     * been sent.
     */
    p = CdmAllocateIOBuffer();
    ASSERT( p != NULL, 0 );

    /*
     * This assertion aids in debugging mismatches between
     * the clients view of a maximum read size, and the servers.
     *
     * In the nondebug version, a short read reply is returned, allowing
     * correct function.
     */
    ASSERT( r->ReadSize <= IO_BUFFER_SIZE, 0 );

    /*
     * Call the OS specific routine to read the data.
     */

    TotalRead = 0;
    Valid = FALSE;

    // The routine compares the supplied time with the files
    TimeStamp = r->TimeStamp;
    DateStamp = r->DateStamp;

    CdmDosReadCond( r->Context,
                    p->IoBuffer,
                    r->ReadSize,
                    r->ReadOffset,
                    &TimeStamp,
                    &DateStamp,
                    &TotalRead,
                    &Valid,
                    &Result
              );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_READCOND_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.TimeStamp = TimeStamp;
    rep.DateStamp = DateStamp;
    rep.Result = Result;   // Error codes as in cdmwire.h
    rep.ReturnSize = TotalRead;

    if( Valid ) {
        // If valid, we do not actually transfer any data
        rep.Flags = READCOND_VALIDATED;
    }
    else {
        rep.Flags = 0;
    }

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >=
              (sizeof_CDM_READCOND_REQUEST_REPLY + TotalRead), 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_READCOND_REQUEST_REPLY ,
                                FALSE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Now write out the data following the reply header

    Ret = CdmTransportSendData( pTransport,
                                p->IoBuffer,
                                (USHORT)TotalRead,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Free the I/O buffer
    CdmFreeIOBuffer( p );

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvFileLock
 *
 *  Handle a FileLock request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvFileLock(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    PCDM_FILELOCK_REQUEST r;
    CDM_FILELOCK_REQUEST_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_FILELOCK_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FILELOCK_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FILELOCK, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosFileLock( r->Context,
                    r->LockStart,
                    r->LockSize,
                    r->Type,
                    &Result
                  );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FILELOCK_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_FILELOCK_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FILELOCK_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvFileUnLock
 *
 *  Handle a File UnLock request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvFileUnLock(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    PCDM_FILEUNLOCK_REQUEST r;
    CDM_FILEUNLOCK_REQUEST_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_FILEUNLOCK_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FILEUNLOCK_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FILEUNLOCK, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosFileUnLock( r->Context,
                      r->LockStart,
                      r->LockSize,
                      &Result
                    );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FILEUNLOCK_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_FILEUNLOCK_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FILEUNLOCK_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvFileChangeSize
 *
 *  Handle a File ChangeSize request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvFileChangeSize(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    PCDM_FILECHANGESIZE_REQUEST r;
    CDM_FILECHANGESIZE_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_FILECHANGESIZE_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FILECHANGESIZE_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FILECHANGESIZE, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosFileChangeSize( r->Context,
                          r->NewSize,
                          &Result
                        );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FILECHANGESIZE_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_FILECHANGESIZE_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FILECHANGESIZE_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvSeek
 *
 *  Handle a Seek request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvSeek(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    USHORT Result;
    ULONG  FileSize;
    PCDM_SEEK_REQUEST r;
    CDM_SEEK_REQUEST_REPLY rep;


    /*
     * Make sure the data is the size of the header
     */
    if( Size != sizeof_CDM_SEEK_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_SEEK_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_SEEK, 0 );

    /*
     * Call the OS specific routine
     */

    CdmDosSeek( r->Context,
                r->NewOffset,
                r->Whence,
                &FileSize,
                &Result
              );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_SEEK_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = r->Context;
    rep.FileSize = FileSize;
    rep.Result = Result;   // Error codes as in cdmwire.h

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_SEEK_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_SEEK_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}


/*****************************************************************************
 *
 *  CdmSrvVolumeInfo
 *
 *  Handle a Volume Information request from the client.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvVolumeInfo(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    int    VolNo;
    PCHAR  pName;
    USHORT Result;
    USHORT TotalSize;
    USHORT ResultFlags;
    ULONG  VolumeSize, BytesFree;
    ULONG  AllocationSize, SerialNumber;
    PCDM_VOLUMEINFO_REQUEST r;
    union {
        CDM_VOLUMEINFO_REQUEST_REPLY  rep;
        CDM_VOLUMEINFO2_REQUEST_REPLY rep2;
    } Buf;


    TRACE(( TC_CDM, TT_API3, "CdmServ: VolumeInformation"));

    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_VOLUMEINFO_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_VOLUMEINFO_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_VOLUMEINFO, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_VOL_INFO ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       if( CdmCltVersionMax >= CDM_VERSION2 ) {
          Buf.rep2.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
          Buf.rep2.MpxId = r->MpxId;
          Buf.rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );
          TotalSize = sizeof_CDM_VOLUMEINFO2_REQUEST_REPLY ;
       }
       else {
          Buf.rep.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
          Buf.rep.MpxId = r->MpxId;
          Buf.rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );
          TotalSize = sizeof_CDM_VOLUMEINFO_REQUEST_REPLY ;
       }


       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&Buf,
                             TotalSize,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_VOLUMEINFO_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       if( CdmCltVersionMax >= CDM_VERSION2 ) {
          Buf.rep2.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
          Buf.rep2.MpxId = r->MpxId;
          Buf.rep2.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
          TotalSize = sizeof_CDM_VOLUMEINFO2_REQUEST_REPLY ;
       }
       else {
          Buf.rep.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
          Buf.rep.MpxId = r->MpxId;
          Buf.rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
          TotalSize = sizeof_CDM_VOLUMEINFO_REQUEST_REPLY ;
       }

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&Buf,
                             TotalSize,
                             TRUE
                           );

       return( 0 );
    }

    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_OPEN_REQUEST  );

    /*
     * The name passed is "X:\.." where X is the drive letter
     * of the volume.
     *
     * The DOS volume numbers are 0 for the current drive, 1 for drive A:,
     * 2 for drive B:, 3 for C:, etc.
     *
     * So here we translate it to the proper number.
     */

    if( (r->PathNameSize < 2) || pName[1] != ':' ) {
        // Default to C:
        VolNo = 3;
        TRACE(( TC_CDM, TT_API1, "CDMSERV: Bad VolInfoPath :%s:, Size %d", pName, r->PathNameSize ));
    }
    else {
        VolNo = toupper(pName[0]) - 'A';
        VolNo++;  // 0 means current drive.
        TRACE(( TC_CDM, TT_API1, "CDMSERV: VolInfo: VolNo %d", VolNo ));
    }

    /*
     * Call the OS specific routine
     */

    CdmDosVolumeInfo( (USHORT)VolNo,
                      (USHORT)r->Flags,
                      &VolumeSize,
                      &BytesFree,
                      &AllocationSize,
                      &SerialNumber,
                      &ResultFlags,
                      &Result
                    );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    /*
     * Depending on the version numbers passed from the host, reply
     * with either a VOLUMEINFO2, or VOLUMEINFO reply.
     */
    if( CdmCltVersionMax >= CDM_VERSION2 ) {
        Buf.rep2.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
        Buf.rep2.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
        Buf.rep2.VolumeSize = VolumeSize;
        Buf.rep2.BytesFree = BytesFree;
        Buf.rep2.AllocationSize = AllocationSize;
        Buf.rep2.SerialNumber = SerialNumber;
        Buf.rep2.Flags = (ULONG)ResultFlags;
        Buf.rep2.Result = Result;   // Error codes as in cdmwire.h
        TotalSize = sizeof_CDM_VOLUMEINFO2_REQUEST_REPLY ;
    }
    else {
        Buf.rep.h_type = CDM_TYPE_VOLUMEINFO_REPLY;
        Buf.rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
        Buf.rep.VolumeSize = VolumeSize;
        Buf.rep.BytesFree = BytesFree;
        Buf.rep.AllocationSize = AllocationSize;
        Buf.rep.SerialNumber = SerialNumber;
        Buf.rep.Result = Result;   // Error codes as in cdmwire.h
        TotalSize = sizeof_CDM_VOLUMEINFO_REQUEST_REPLY ;
    }

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&Buf,
                                TotalSize,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvFindFirstIndex
 *
 *  Handle a FindFirst request from the client at a specific
 *  starting index.
 *
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvFindFirstIndex(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT  Size
    )
{
    int    Ret;
    PCHAR  pName;
    PIO_ENTRY p;
    PCHAR     pFindBuf;
    PCDM_FINDFIRSTINDEX_REQUEST r;
    CDM_FINDFIRST_REQUEST_REPLY rep;
    USHORT TotalSize;
    USHORT TotalRead;
    USHORT Result;
    USHORT Context;
    USHORT MaxCount;
    UCHAR  EntriesRead;
#ifdef WIN32
    USHORT LongFileNames = (CdmCltVersionMax >= CDM_VERSION4 );
#else
    USHORT LongFileNames = FALSE;
#endif


    /*
     * Make sure the data is at least the size of the header
     */
    if( Size < sizeof_CDM_FINDFIRSTINDEX_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_FINDFIRSTINDEX_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_FINDFIRSTINDEX, 0 );

    /*
     * WKP: Security check for internet client
     */
    if( CDM_SECURITY_ACCESS_ACCEPTED != CdmSecurityVerifyAccess( CDM_SECURITY_ACCESS_FIND_FIRST ) ) {

       if(!fSecurityPopup) {
       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.NumberReturned = 0;
       rep.BytesReturned = 0;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NOACCESS, CDM_DOSERROR_ACCESSDENIED );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
       }
       else {
          // queue the command for later dispatch
          if(QueueReceiveData(pTransport, pBuf, Size))
             return(Size);
          else
             return(0);
       }
    }

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_FINDFIRSTINDEX_REQUEST  + r->PathNameSize;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       // Send an error reply so we do not hang the client
       rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
       rep.MpxId = r->MpxId;
       rep.Context = (USHORT)(-1);
       rep.NumberReturned = 0;
       rep.BytesReturned = 0;
       rep.Result = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );

       // If transport problems, we can't do anything anyway
       CdmTransportSendData( pTransport,
                             (PCHAR)&rep,
                             sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                             TRUE
                           );

       return( 0 );
    }


    /*
     * Get a pointer to the pathname following the packet header
     */
    pName = (PCHAR)( pBuf + sizeof_CDM_FINDFIRSTINDEX_REQUEST  );


    /*
     * Get an I/O buffer
     *
     * This limits the amount we can reply to in a single FindFirst
     * request. This is because the reply header must be written
     * to the transport before the data. If an error occurs from
     * a multiple read and transport send sequence, we can not report
     * the error, or the short read size since the header has already
     * been sent.
     */

    p = CdmAllocateIOBuffer();
    ASSERT( p != NULL, 0 );

    pFindBuf = p->IoBuffer;

    /*
     * This assertion aids in debugging mismatches between
     * the clients view of a maximum read size, and the servers.
     *
     * In the nondebug version, a short read reply is returned, allowing
     * correct function.
     */
    ASSERT( (r->BufferSize ) <= IO_BUFFER_SIZE, 0 );

    /*
     * Determine how many FINDSTRUCTS we can reply to with the given
     * I/O buffer size, but do not overflow our BYTE sized count.
     */
    if( LongFileNames ) {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT_LONG ;
    }
    else {
        MaxCount = IO_BUFFER_SIZE / sizeof_FINDSTRUCT ;
    }

    if( MaxCount > 255 ) MaxCount = 255;

    if( (USHORT)r->NumberRequested > MaxCount ) r->NumberRequested = (UCHAR)MaxCount;

    /*
     * Call the OS specific routine to open the directory
     * pathname and read the FINDSTRUCT data and entry names into
     * the buffer.
     */

    TotalRead = 0;
    EntriesRead = 0;

    CdmDosFindFirstIndex( pName,
                          r->PathNameSize,
                          r->Index,           // Start index
                          LongFileNames,
                          pFindBuf,
                          (USHORT)r->BufferSize,
                          (PUSHORT)&TotalRead,
                          r->NumberRequested,
                          &EntriesRead,
                          &Context,
                          &Result
                        );

    /*
     * We pass any errors from the OS specific function
     * back to the client, regardless.
     */

    rep.h_type = CDM_TYPE_FINDFIRST_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Context = Context;
    rep.Result = Result;   // Error codes as in cdmwire.h
    rep.BytesReturned = TotalRead;
    rep.NumberReturned = EntriesRead;

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >=
              (sizeof_CDM_FINDFIRST_REQUEST_REPLY + TotalRead), 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_FINDFIRST_REQUEST_REPLY ,
                                FALSE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Now write out the data following the reply header
    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)pFindBuf,
                                (USHORT)TotalRead,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       CdmFreeIOBuffer( p );
       return( 0 );
    }

    // Free the I/O buffer
    CdmFreeIOBuffer( p );

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSrvConnect
 *
 *   Handle a connect request from the host.
 *
 * ENTRY:
 *   pTransport (input)
 *     Opaque pointer to transport that request came in on
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

USHORT STATIC 
CdmSrvConnect(
    PVOID  pTransport,
    PCHAR  pBuf,
    USHORT Size
    )
{
    int    Ret;
    USHORT TotalSize;
    PCDM_CONNECT_REQUEST r;
    CDM_CONNECT_REQUEST_REPLY rep;

    /*
     * Make sure the data is at least the
     * size of the header
     */
    if( Size < sizeof_CDM_CONNECT_REQUEST  ) {

        // If theres a debugger lets catch this
        ASSERT( FALSE, 0 );
        return( 0 );
    }

    /*
     * Get a pointer to our specific header
     */
    r = (PCDM_CONNECT_REQUEST)pBuf;

    /*
     * Check for any internal bugs, such as wrong order of
     * functions in the dispatch table
     */
    ASSERT( r->h_type == CDM_TYPE_CONNECT, 0 );

    /*
     * Make sure we have the whole command request
     */
    TotalSize = sizeof_CDM_CONNECT_REQUEST ;
    if( TotalSize != Size ) {

       // If theres a debugger, catch this
       ASSERT( FALSE, 0 );

       return( 0 );
    }

    /*
     * Set our versions passed from the client
     */
    CdmCltVersionMin = r->VersionLow;
    CdmCltVersionMax = r->VersionHigh;

    /*
     * If the host is CDM_VERSION4 or newer, it is expecting
     * a reply.
     */
    if( !(r->VersionHigh >= CDM_VERSION4) ) {
        /*
         * No reply to this packet
         */
        return( Size );
    }

    rep.h_type = CDM_TYPE_CONNECT_REPLY;
    rep.MpxId = r->MpxId;  // Pass through the clients MpxId untouched
    rep.Result = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
    rep.ReplyVersion = CDM_CONNECT_REPLY_VERSION1;
    rep.ReplySize = sizeof_CDM_CONNECT_REQUEST_REPLY;

#ifdef WIN32
    // WIN32 (95,NT) support long file names
    rep.Capabilities = CDM_LONGFILE_NAMES + CDM_ANSI_CHARS;
#endif

#ifdef WIN16
    // WIN16 does not support long file names
    rep.Capabilities = CDM_ANSI_CHARS;
#endif

#ifdef DOS
    // DOS, WIN16 do not support long file names, ANSI chars
    rep.Capabilities = 0;
#endif

    // Catch any transport bugs
    ASSERT( CdmTransportMaxSize( pTransport ) >= sizeof_CDM_CONNECT_REQUEST_REPLY , 0 );

    /*
     * Now send the reply out on the transport
     */

    Ret = CdmTransportSendData( pTransport,
                                (PCHAR)&rep,
                                sizeof_CDM_CONNECT_REQUEST_REPLY ,
                                TRUE
                              );

    // Transports having problems
    if( Ret ) {
       return( 0 );
    }

    return( Size );
}

/*****************************************************************************
 *
 *  CdmSecuritySetAccess
 *
 *  Handle a set access request from the client ui.
 *
 *  ENTRY:  fAccessLimit - Limit of host requested cdm request
 *
 *  EXIT:
 *
 ****************************************************************************/

USHORT STATIC
CdmSecuritySetAccess( ULONG fAccessLimit )
{
    PRCVDATAQ pTmp,pNext;

    /*
     *  Check for invalid access bits
     */
    if ( fAccessLimit & (~CDM_SECURITY_ACCESS_ALL) ) {
        fSecurityAccess = CDM_SECURITY_ACCESS_DENIED;
    }
    else {
        fSecurityAccess = fAccessLimit;
    }

    // turn off popup flag
    fSecurityPopup = FALSE;

    // restart the requested commands
    pTmp = pRcvDataHead;
    pRcvDataHead = NULL;
    while(pTmp!=NULL) {
       pNext = pTmp->pNext;

       CdmWireDataReceive( pTmp->pTransport,pTmp->Buf,pTmp->Size);

       free(pTmp);
       pTmp = pNext;
       }


    return( 0 );
}


/*****************************************************************************
 *
 *  CdmSecurityVerifyAccess
 *
 *   Handle a set access request from the client ui.
 *
 * ENTRY:
 *
 * EXIT:
 *
 ****************************************************************************/

USHORT STATIC
CdmSecurityVerifyAccess( ULONG fAccessRequested )
{
#ifdef DOS
    // for DOS, always allow access. Security is only meant for
    // Windows.
    return(CDM_SECURITY_ACCESS_ACCEPTED);
#else
//  BYTE buffer[128];

    //
    // if we are using IPC, we are not the Internet client
    // so don't bother with drive security
    //
    if(gbIPCEngine) {
        return(CDM_SECURITY_ACCESS_ACCEPTED);
    }

    /* 
     *  Ask first time thru
     */
    if ( CDM_SECURITY_ACCESS_UNKNOWN == fSecurityAccess ) {

        /*
         *  Set to no access and let nature take its course
         */
        fSecurityAccess = CDM_SECURITY_ACCESS_NONE;

        /*
         *  Request ui to set access rights
         */
        fSecurityPopup = TRUE;
        (void) (*gpfnStatusMsgProc)( NULL, CLIENT_STATUS_QUERY_ACCESS );
    }

//  wsprintf( buffer, "Requested %08lx vs. Required %08lx\n", fAccessRequested, fSecurityAccess);

    /*
     *  Check access rights
     */
    if ( (fAccessRequested & fSecurityAccess) ) {
//      MessageBox(NULL, buffer, "Verify Access OK", MB_OK);
        return(CDM_SECURITY_ACCESS_ACCEPTED);
    }

//  MessageBox(NULL, buffer, "Verify Access Fail", MB_OK);
    return(CDM_SECURITY_ACCESS_DENIED);
#endif
}


/*****************************************************************************
 *
 *  QueueReceiveData
 *
 *   All receive data is queued until user answers the question
 *
 * ENTRY:
 *
 * EXIT:
 *
 ****************************************************************************/
BOOL STATIC QueueReceiveData(PVOID pTransport, PCHAR pBuf, USHORT Size)
{

   PRCVDATAQ pMem;
   PRCVDATAQ pTmp;

   // allocate the memory for receive data
   pMem = (PRCVDATAQ)malloc(Size + sizeof(RCVDATAQ));
   if(pMem==NULL)
      return(FALSE);

   // fill in the information
   pMem->pTransport = pTransport;
   pMem->Size = Size;
   memcpy(pMem->Buf,pBuf,Size);
   pMem->pNext = NULL;

   // add to the linked list at end
   if(pRcvDataHead==NULL) {
      pRcvDataHead = pMem;
      }
   else {
      pTmp = pRcvDataHead;
      while(pTmp->pNext != NULL) {
         pTmp = pTmp->pNext;
         }
      pTmp->pNext = pMem;
      }
   
   return(TRUE);
}
