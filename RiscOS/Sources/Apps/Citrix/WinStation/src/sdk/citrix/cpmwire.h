
/*************************************************************************
*
* cpmwire.h
*
* Wire Protocol Header for CITRIX Client Printer Mapping Protocol
*
* This header describes major version 2. Version 1 is in cpmold.h
*
* Since printers do not share the same on wire request blocks as the client
* drive mapping protocol, this header has been defined independently of the
* need to include cdmwire.h
*
* It has been designed to not conflict with cdmwire.h for modules that include
* both.
*
* Even though the wire protocol is different, the higher level interface to
* the printer ports has the same DOS file handle based access semantics as
* the Client drive mapping protocol.
*
* Since the on wire format is different from Client Drive Mapping, these
* requests must be transmitted over a separate virtual circuit.
*
* IE: The protocol "cracker" will get confused if request blocks for both
*     protocols are passed across the wire.
*
*     (The command byte has been designed to be unique of any CDM packet TYPE
*      bytes to at least catch this, and allow possible future support
*      of mixed protocols over the same virtual circuit)
*
* copyright notice: Copyright 1994,1995 Citrix Systems Inc.
*
* Author:  John Richardson 04/28/94
*          New Version     09/19/95
*
* Log:
*
*
*
*************************************************************************/


#ifndef _CPMWIRE_
#define _CPMWIRE_

/*
 * All of the structures in this header must be packed. Otherwise, they
 * would have to manually be padded to 4 byte size multiples.
 *
 * Parameters are alaigned on natural boundries, and should pose no
 * problems to RISC architectures with alignment restrictions.
 */

#pragma pack(1)

/*
 * General structures and formats used by the printer wire protocol.
 */

#define CPM_MIN_VERSION  0x0004
#define CPM_MAX_VERSION  0x0004

/*
 * WARNING: These are the same as defined in cdmwire.h
 */

// File Sharing Modes
#define CPM_EXCLUSIVE_MODE     0x10
#define CPM_SHARE_MASK         0x70

// Access Modes
#define CPM_READACCESS         0x00
#define CPM_WRITEACCESS        0x01
#define CPM_READWRITE          0x02
#define CPM_ACCESS_MASK        0x07

/*
 * Error Classes returned from the server.
 *
 * These are the same as the DOS 3.x error classes, and are the most used by
 * clients of the wire protcol. The detailed DOS 3.x errors are usually not
 * needed, or recommended to keep the client abstract enough so that new
 * non-DOS file servers could be used.
 *
 */
#define CPM_ERROR_NONE      0x0000   // No error.
#define CPM_ERROR_RESOURCE  0x0001   // Out of open files, memory, etc.
#define CPM_ERROR_TEMPORARY 0x0002   // Can resolve if try again later
#define CPM_ERROR_NOACCESS  0x0003   // Permission problem
#define CPM_ERROR_INTERNAL  0x0004   // Internal DOS (or Server) error.
#define CPM_ERROR_HARDWARE  0x0005   // Hardware error (I/O error, offline )
#define CPM_ERROR_SYSTEM    0x0006   // General system failure (or Server)
#define CPM_ERROR_INVALID   0x0007   // Invalid request, parameter, etc.
#define CPM_ERROR_NOTFOUND  0x0008   // File, etc. not found.
#define CPM_ERROR_BADFORMAT 0x0009   // Name, or parameters in incorrect format.
#define CPM_ERROR_LOCKED    0x000A   // File/Item locked
#define CPM_ERROR_MEDIA     0x000B   // Media failure, CRC, wrong disk, etc.
#define CPM_ERROR_EXISTS    0x000C   // Collision w/existing item (file, etc.)
#define CPM_ERROR_EOF       0x000D   // End of file on file, directory
#define CPM_ERROR_UNKNOWN   0x000E   // No error class available

/*
 * These are detailed DOS 3.x error codes which may be useful to clients of the
 * wire protocol. All error codes are passed through directly from DOS, but
 * only 'interesting' ones are defined here. Additional ones may be defined as
 * needed by a specific situation.
 *
 * It is recommened that the above Error Class be used for error handling,
 * since these are generic enough to be provided by any given file server. The
 * described detailed error codes below will attempt to be supplied by all
 * servers as well.
 *
 * All possible DOS error codes may not be returned by all possible WinView CDM
 * servers. IE: MacIntosh. The important one CDM_DOSERROR_UNKNOWN means that
 * the Error Class is the only information available, and it is legal for a
 * server to always return when there is an error.
 *
 */
#define CPM_DOSERROR_NOERROR      0x0000  // No Error
#define CPM_DOSERROR_INVALIDFUNC  0x0001  // Invalid Function
#define CPM_DOSERROR_NOTFOUND     0x0002  // File not found
#define CPM_DOSERROR_PATHNOTFOUND 0x0003  // Path not found
#define CPM_DOSERROR_NOHANDLES    0x0004  // No more file handles left.
#define CPM_DOSERROR_ACCESSDENIED 0x0005  // No access to file, readonly, etc.
#define CPM_DOSERROR_INVALIDHANDLE 0x0006  // Invalid Handle
#define CPM_DOSERROR_NOMEM        0x0008  // Insufficient memory
#define CPM_DOSERROR_BADACCESS    0x000C  // An Invalid access code was specifed
#define CPM_DOSERROR_BADDRIVE     0x000F  // An Invalid drive was specifed
#define CPM_DOSERROR_NOFILES      0x0012  // No more files left (FILES=xx in CONFIG.SYS)
#define CPM_DOSERROR_WRITEPROTECT 0x0013  // Write Protected disk
#define CPM_DOSERROR_NOTREADY     0x0015  // Drive Not Ready
#define CPM_DOSERROR_BADLENGTH    0x0018  // Bad request structure length
#define CPM_DOSERROR_UNKNOWNMEDIA 0x001A  // Unknown Media Type
#define CPM_DOSERROR_OUTOFPAPER   0x001C  // (28) Out of paper
#define CPM_DOSERROR_SHARE        0x0020  // Sharing violation
#define CPM_DOSERROR_LOCK         0x0021  // Lock violation
#define CPM_DOSERROR_INVALIDDISK  0x0022  // Invalid disk change
#define CPM_DOSERROR_PRINTQUEFULL 0x003D  // (61) Print queue full
#define CPM_DOSERROR_PRINTNOSPACE 0x003E  // (62) Not enough Print file space
#define CPM_DOSERROR_PRINTFILEDEL 0x003F  // (63) Print file deleted
#define CPM_DOSERROR_EXISTS       0x0050  // File exists
#define CPM_DOSERROR_UNKNOWN      0x00FF  // No DOS error code available

/*
 * Format of the "Status" word returned in the CPM protocol.
 *
 * The "Status" word is returned from all operations that have a reply. This
 * status word contains both the DOS error class (byte 0), and the DOS error
 * code (byte 1).
 *
 * As noted above, the DOS error code could be 0xFF at any time, so if DOS
 * error codes are desired, the following code will ensure compatibility with
 * future WinView Clients (such as MacIntosh)
 *
 * if( CDM_DOSERROR_CODE( Status ) == CDM_DOSERROR_UNKNOWN ) {
 *     // Handle error based on CDM_ERROR_CODE( Status )
 * }
 * else {
 *     // Handle specific error from CDM_DOSERROR_CODE( Status )
 * }
 *
 * This allows a client re-director to provide more detailed information or
 * actions if the DOS error code is available. IE: CDM_DOSERROR_NOFILES is
 * something the user can correct for future sessions by changing the FILES=xx
 * value in their CONFIG.SYS file. This may not apply to a MacIntosh client, so
 * these systems would just return the CDM_ERROR_RESOURCE error with a DOS
 * error code of CDM_DOSERROR_UNKNOWN. The re-director could provide NT error
 * codes based on the detailed information, or the general information, to the
 * user.
 *
 */
#define CPM_ERROR_CODE( Status) ( Status & 0x00FF )
#define CPM_DOSERROR_CODE( Status ) ( ((Status >> 8) & 0x00FF) )
#define CPM_MAKE_STATUS(Error, DosError) ( (unsigned short)((Error & 0x00FF) | (DosError << 8)) )

/*
 * These defines are for the printer status word
 * in the status packets. (Request, Set, Update)
 */

// BIOS defined
#define CPM_TIMEOUT      0x0001
#define CPM_RESERVED1    0x0002
#define CPM_RESERVED2    0x0004
#define CPM_IOERROR      0x0008
#define CPM_SELECT       0x0010
#define CPM_OUTOFPAPER   0x0020
#define CPM_ACK          0x0040
#define CPM_NOTBUSY      0x0080

// Software defined
#define CPM_QUEUE_EMPTY  0x0100    // No more buffered data in queue
#define CPM_QUEUE_FULL   0x0200    // No more buffered data can fit in queue


/*
 * The numbering of the following CPM protocol requests and replies are
 * done in the following way:
 *
 * Requests number from 64-74, replies number from 128-138.
 *
 * This has been done to allow the use of dispatch tables from the on
 * both the client and the server sides without having to have every other
 * entry be empty.
 *
 * Other numbers are reserved for Client Drive, and COM port mapping.
 *
 */
#define CPM_REQUEST_BASE 64
#define CPM_REPLY_BASE   192

/*
 *                 NEW CLIENT-SERVER CLIENT PRINTER REQUESTS
 *
 *                 09/18/95 John Richardson
 *
 * In order to support the new class of Windows Clients that have
 * their own spoolers, with printers already configured, a new set of
 * client-server Client Printer operations have been defined.
 *
 * The older "port" based printer protocol supports lightweight clients
 * that do not do any spooling. The hardware flow control of the printer,
 * (IE: how quickly the printer can accept data), is exposed to the WinFrame
 * Host computer through back-pressure on the ICA channels Virtual_LPT1-
 * Virtual_LPT4, and Virtual_COM1 - Virtual_COM4. Asynchronous status messages
 * are sent from the client to the host on each independent channel when
 * printer status conditions change. No requests can be sent to a printer
 * who is blocked on flow control, but its channel can be forced closed
 * by an ICA Flush command. This prevents the effective multiplexing of
 * commands over one channel.
 *
 * The ports that are available for connecting to are passed at client
 * connect time, which is OK for DOS clients that can not dynamicly
 * define new ports at runtime.
 *
 * With the introduction of the Windows Clients, clients now can have
 * many pre-defined printers already setup by the user. It is also common
 * to have many different printers connecting to the same port be defined,
 * with the user pointing the application to the proper virtual Windows
 * printer for the print mode desired. The user can define new printers
 * on their Windows client after connecting to the host, and with WIN 95
 * Plug+Play, even ports can dynamicly show up after a connection is
 * made to the host. In order to support new user interfaces on the
 * application server that allow users to browse and connect to printers
 * at any time, a new set of protocols is desired to allow printer
 * enumeration, returning detailed information such as printer device
 * driver name, so that a host driver may be automaticly matched, and
 * the ability to open a printer by its Windows name, not its port name,
 * due to the common many to one mappings of a Windows printer configuration.
 *
 * IE: A different Windows printer can be created for each paper
 *     tray in a laser printer, with the user selecting the Windows
 *     printer that selects the desired paper size. Each Windows
 *     printer actually points to the same physical printer on LPT1:.
 *
 * This operates on the new Virtual_Cpm channel, which is
 * used for all client/server (CDM like) operations for printers.
 * The printer port virtual channels such as Virtual_LPT1 -
 * Virtual_LPT4 are flow controlled by the hardware printers, and
 * can block. The client server channel is always "open" like CDM.
 * (Except for request buffers, etc.)
 *
 * It is expected that all writes on the client-server channels
 * complete before returning. This is due to remote windows machines
 * spooling to the local hard disks of the client. If they do not,
 * they must delay their write reply until buffer space is available
 * on the client. The host will not put a timeout on these operations.
 *
 * For the time being, LPT4 will become the client-server Virtual_Cpm
 * channel. A new ICA channel will be added in the future. BUGBUG:
 *
 * These operations are only supported on CPM version 2 or later.
 * The older Port operations still exist and function for light weight
 * clients. X-Terminals, Windows Terminals, DOS Clients, etc.
 * These are in cpmold.h, which must be included after this header.
 */

/*
 * Do an Enumerate Printers request.
 *
 * Printers on the remote client are enumerated starting at
 * the specified index. If the error returns CPM_DOSERROR_NOFILES,
 * there are no more printers at that index or later on the client.
 *
 * To enumerate all of the printers on a remote client, start with
 * index == 0, and enumerate sequential indexes until CPM_DOSERROR_NOFILES
 * is returned.
 *
 */

/*
 * This structure describes a printer entry and allows
 * multiple to be enumerated at one time.
 *
 * The packing is:
 *
 *  CPM_ENUMPRINTER_REQUEST_REPLY
 *  ENUMSTRUCT 0
 *  <Strings for entry 0>
 *  ENUMSTRUCT 1
 *  <Strings for entry 1>
 *  ENUMSTRUCT 2
 *  <Strings for entry 2>
 *  ...
 *
 */
typedef struct _ENUMSTRUCT {
    UCHAR  NameSize;       // Size of the printer name following this header
    UCHAR  DriverSize;     // Size of the driver name following the printer name
    UCHAR  PortSize;       // Size of the port name following the driver name
    UCHAR  CommentSize;    // Size of the comment field following the port name

    ULONG  Flags;          // Flags

    USHORT ExtraSize;      // Size of extra information if supplied following Comment
} ENUMSTRUCT, *PENUMSTRUCT;


typedef struct _CPM_ENUMPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests

    USHORT Index;            // Index of printer to return info on
    USHORT DataSize;         // Maximum amount of data to return

} CPM_ENUMPRINTER_REQUEST, *PCPM_ENUMPRINTER_REQUEST;
#define CPM_TYPE_ENUMPRINTER CPM_REQUEST_BASE // 64

/*
 * Reply from a CPM_ENUMPRINTER_REQUEST
 */
typedef struct _CPM_ENUMPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)

    USHORT Count;          // Count returned
    USHORT Size;           // Size
} CPM_ENUMPRINTER_REQUEST_REPLY, *PCPM_ENUMPRINTER_REQUEST_REPLY;
#define CPM_TYPE_ENUMPRINTER_REPLY CPM_REPLY_BASE // 192


/*
 * Open the given printer device specified by name.
 *
 * This is designed for talking to remote client-server spoolers such as
 * newer Windows clients.
 *
 * DOS, and lightweight clients continue to use the port level open
 * CPM_OPEN_REQUEST.
 */

typedef struct _CPM_OPENPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    UCHAR  NameSize;      // Size of the name following this header
    UCHAR  AccessMode;

} CPM_OPENPRINTER_REQUEST, *PCPM_OPENPRINTER_REQUEST;
#define CPM_TYPE_OPENPRINTER CPM_TYPE_ENUMPRINTER+1 // 65

/*
 * Reply from a CPM_OPENPRINTER_REQUEST
 */
typedef struct _CPM_OPENPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)
    USHORT Context;       // Handle for open printer

} CPM_OPENPRINTER_REQUEST_REPLY, *PCPM_OPENPRINTER_REQUEST_REPLY;
#define CPM_TYPE_OPENPRINTER_REPLY CPM_TYPE_ENUMPRINTER_REPLY+1 // 193


/*
 * Close the given device releasing the Context value.
 */
typedef struct _CPM_CLOSEPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;

} CPM_CLOSEPRINTER_REQUEST, *PCPM_CLOSEPRINTER_REQUEST;
#define CPM_TYPE_CLOSEPRINTER CPM_TYPE_OPENPRINTER+1 // 66

/*
 * Reply from a CPM_CLOSEPRINTER_REQUEST
 */
typedef struct _CPM_CLOSEPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)

} CPM_CLOSEPRINTER_REQUEST_REPLY, *PCPM_CLOSEPRINTER_REQUEST_REPLY;
#define CPM_TYPE_CLOSEPRINTER_REPLY CPM_TYPE_OPENPRINTER_REPLY+1 // 194

/*
 * Read from the printer on the current Context.
 *
 * NOTE: This is currently not issued by the CPM redirector, but has
 *       been defined since future ECP (Enhanced Capabilities Parallel) Ports
 *       do support reading for supporting printers such as the Windows Printing
 *       System. These printers would be accessed by the Windows Printing System
 *       Application/Driver directly without going through the NT spooler. So we
 *       need to have the same NT device API support as a native ECP port.
 */

typedef struct _CPM_READPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Context;     // Handle for open printer
    USHORT ReadSize;    // Write data size

} CPM_READPRINTER_REQUEST, *PCPM_READPRINTER_REQUEST;
#define CPM_TYPE_READPRINTER CPM_TYPE_CLOSEPRINTER+1 // 67


/*
 * Reply from a CPM_READPRINTER_REQUEST
 *
 * The data read from the file server follows this header and is represented
 * by ReturnSize.
 */
typedef struct _CPM_READPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Result;      // Result
    USHORT AmountRead;  // Amount read
} CPM_READPRINTER_REQUEST_REPLY, *PCPM_READPRINTER_REQUEST_REPLY;
#define CPM_TYPE_READPRINTER_REPLY CPM_TYPE_CLOSEPRINTER_REPLY+1 // 195

/*
 * Client-Server printer write request.
 *
 * This is used when the remote client can spool the requested data,
 * and return within a reasonable time. IE: does not wait for hardware
 * printer flow control, such as paper out, etc.
 */
typedef struct _CPM_WRITEPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Context;       // Handle for open printer
    USHORT WriteSize;     // Write data size

} CPM_WRITEPRINTER_REQUEST, *PCPM_WRITEPRINTER_REQUEST;
#define CPM_TYPE_WRITEPRINTER CPM_TYPE_READPRINTER+1 // 68

/*
 * Reply from a CPM_WRITE_PRINTER_REQUEST
 */
typedef struct _CPM_WRITEPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Result;        // Result
    USHORT AmountWrote;   // Amount wrote
} CPM_WRITEPRINTER_REQUEST_REPLY, *PCPM_WRITEPRINTER_REQUEST_REPLY;
#define CPM_TYPE_WRITEPRINTER_REPLY CPM_TYPE_READPRINTER_REPLY+1

/*
 * Get printer status
 */
typedef struct _CPM_GETPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  Flags;          // Flags to be defined with printer info
    USHORT DataSize;       // Maximum amount of printer specific data

} CPM_GETPRINTER_REQUEST, *PCPM_GETPRINTER_REQUEST;
#define CPM_TYPE_GETPRINTER CPM_TYPE_WRITEPRINTER+1 // 69

/*
 * Reply from a CPM_GETPRINTER_REQUEST
 *
 * NOTE: This is only sent as a result of a GETPRINTER_REQUEST.
 *
 *       STATUS_UPDATE is sent when the remote Citrix client sends
 *       an unsolicited status update.
 */
typedef struct _CPM_GETPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;
    ULONG  Status;
    ULONG  Flags;          // Flags to be defined for info following structure
    USHORT InfoLen;        // Length of extra data returned

} CPM_GETPRINTER_REQUEST_REPLY, *PCPM_GETPRINTER_REQUEST_REPLY;
#define CPM_TYPE_GETPRINTER_REPLY CPM_TYPE_WRITEPRINTER_REPLY+1

/*
 * Set Printer Status
 */
typedef struct _CPM_SETPRINTER_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  Status;
    ULONG  Flags;
    USHORT InfoLen;

} CPM_SETPRINTER_REQUEST, *PCPM_SETPRINTER_REQUEST;
#define CPM_TYPE_SETPRINTER CPM_TYPE_GETPRINTER+1 // 70

/*
 * Reply from a CPM_SETPRINTER_REQUEST
 *
 * This reply is required since the port may not have
 * accepted the status byte set, or it changed the condition.
 * IE: Set PrinterSelect, PrinterReturns PaperOut, Error, Not ready, etc.
 */
typedef struct _CPM_SETPRINTER_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;
    ULONG  Status;         // Status may have updated
    ULONG  Flags;

} CPM_SETPRINTER_REQUEST_REPLY, *PCPM_SETPRINTER_REQUEST_REPLY;
#define CPM_TYPE_SETPRINTER_REPLY CPM_TYPE_GETPRINTER_REPLY+1

/*
 *  Packet that allows the remote device server on the Citrix Client
 *  to send status changes to the redirector.
 *
 * NOTE: This status update packet may be sent by the remote device
 *       server on the Citrix Client without a GETSTATUS request any
 *       time after a given printer has been opened. This allows async
 *       updates of notable events such as PaperOut on the printer
 *       without having to poll.
 *
 *       At the current time, this is the only way Status is updated
 *       without the host ever asking for it.
 *
 */

// Reserve this entry
#define CPM_TYPE_ASYNC_STATUS_REQUEST CPM_TYPE_SETPRINTER+1 // 71

typedef struct _CPM_ASYNC_STATUS {
    UCHAR  h_type;
    UCHAR  Pad;

    USHORT Context;      // Context the status is for
    ULONG  Status;

} CPM_ASYNC_STATUS, *PCPM_ASYNC_STATUS;
#define CPM_TYPE_ASYNC_STATUS CPM_TYPE_SETPRINTER_REPLY+1

/*
 * This is the first packet sent by the host and supplies
 * host version information.
 *
 * There is no reply to this packet since the client sent
 * the information at ICA connect time.
 */

typedef struct _CPM_CONNECT2_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Size;          // Size of this request header
    USHORT VersionLow;
    USHORT VersionHigh;
    USHORT Flags;         // Flags to be defined

} CPM_CONNECT2_REQUEST, *PCPM_CONNECT2_REQUEST;
#define CPM_TYPE_CONNECT2 CPM_TYPE_ASYNC_STATUS_REQUEST+1

// Reserve this entry
#define CPM_TYPE_CONNECT2_REPLY CPM_TYPE_ASYNC_STATUS+1 // 72

/*
 * This is the maximum packet size define used by transport drivers
 * for calculating buffer space and timeout parameters.
 */
#define CPM_MAX_PACKET_SIZE (sizeof(CPM_CONNECT_REQUEST))

/*
 * This is the defined maximum number of requests that the HOST
 * can request from the remote CPM device Server.
 */
#define CPM_MAXREQUEST_COUNT  4

#pragma pack()

#endif // _CPMWIRE_

