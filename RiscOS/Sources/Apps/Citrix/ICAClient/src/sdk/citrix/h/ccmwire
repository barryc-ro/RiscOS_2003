
/*************************************************************************
*
* ccmwire.h
*
* Wire Protocol Header for CITRIX Client COM Mapping Protocol
*
* This has been designed to not conflict with cdmwire.h, or cpmwire.h
* for modules that include both.
*
* Since the on wire format is different from Client Drive Mapping, these
* requests must be transmitted over a separate virtual circuit.
*
*     (The command byte has been designed to be unique of any CDM packet TYPE
*      bytes to at least catch this, and allow possible future support
*      of mixed protocols over the same virtual circuit)
*
* copyright notice: Copyright 1995 Citrix Systems Inc.
*
* Author:  John Richardson 10/11/95
*
* Log:
*
*
*
*************************************************************************/


#ifndef _CCMWIRE_
#define _CCMWIRE_

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

#define CCM_MIN_VERSION  0x0004
#define CCM_MAX_VERSION  0x0004

/*
 * WARNING: These are the same as defined in cdmwire.h
 */

// File Sharing Modes
#define CCM_EXCLUSIVE_MODE     0x10
#define CCM_SHARE_MASK         0x70

// Access Modes
#define CCM_READACCESS         0x00
#define CCM_WRITEACCESS        0x01
#define CCM_READWRITE          0x02
#define CCM_ACCESS_MASK        0x07

/*
 * Error Classes returned from the server.
 *
 * These are the same as the DOS 3.x error classes, and are the most used by
 * clients of the wire protcol. The detailed DOS 3.x errors are usually not
 * needed, or recommended to keep the client abstract enough so that new
 * non-DOS file servers could be used.
 *
 */
#define CCM_ERROR_NONE      0x0000   // No error.
#define CCM_ERROR_RESOURCE  0x0001   // Out of open files, memory, etc.
#define CCM_ERROR_TEMPORARY 0x0002   // Can resolve if try again later
#define CCM_ERROR_NOACCESS  0x0003   // Permission problem
#define CCM_ERROR_INTERNAL  0x0004   // Internal DOS (or Server) error.
#define CCM_ERROR_HARDWARE  0x0005   // Hardware error (I/O error, offline )
#define CCM_ERROR_SYSTEM    0x0006   // General system failure (or Server)
#define CCM_ERROR_INVALID   0x0007   // Invalid request, parameter, etc.
#define CCM_ERROR_NOTFOUND  0x0008   // File, etc. not found.
#define CCM_ERROR_BADFORMAT 0x0009   // Name, or parameters in incorrect format.
#define CCM_ERROR_LOCKED    0x000A   // File/Item locked
#define CCM_ERROR_MEDIA     0x000B   // Media failure, CRC, wrong disk, etc.
#define CCM_ERROR_EXISTS    0x000C   // Collision w/existing item (file, etc.)
#define CCM_ERROR_EOF       0x000D   // End of file on file, directory
#define CCM_ERROR_UNKNOWN   0x000E   // No error class available

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
#define CCM_DOSERROR_NOERROR      0x0000  // No Error
#define CCM_DOSERROR_INVALIDFUNC  0x0001  // Invalid Function
#define CCM_DOSERROR_NOTFOUND     0x0002  // File not found
#define CCM_DOSERROR_PATHNOTFOUND 0x0003  // Path not found
#define CCM_DOSERROR_NOHANDLES    0x0004  // No more file handles left.
#define CCM_DOSERROR_ACCESSDENIED 0x0005  // No access to file, readonly, etc.
#define CCM_DOSERROR_INVALIDHANDLE 0x0006  // Invalid Handle
#define CCM_DOSERROR_NOMEM        0x0008  // Insufficient memory
#define CCM_DOSERROR_BADACCESS    0x000C  // An Invalid access code was specifed
#define CCM_DOSERROR_BADDRIVE     0x000F  // An Invalid drive was specifed
#define CCM_DOSERROR_NOFILES      0x0012  // No more files left (FILES=xx in CONFIG.SYS)
#define CCM_DOSERROR_WRITEPROTECT 0x0013  // Write Protected disk
#define CCM_DOSERROR_NOTREADY     0x0015  // Drive Not Ready
#define CCM_DOSERROR_BADLENGTH    0x0018  // Bad request structure length
#define CCM_DOSERROR_UNKNOWNMEDIA 0x001A  // Unknown Media Type
#define CCM_DOSERROR_SHARE        0x0020  // Sharing violation
#define CCM_DOSERROR_LOCK         0x0021  // Lock violation
#define CCM_DOSERROR_INVALIDDISK  0x0022  // Invalid disk change
#define CCM_DOSERROR_EXISTS       0x0050  // File exists
#define CCM_DOSERROR_CANCELLED    0x0070  // Operation canceled
#define CCM_DOSERROR_BAUDRATE     0x0071  // Bad baudrate for hardware
#define CCM_DOSERROR_WORDLEN      0x0072  // Bad wordlen for hardware
#define CCM_DOSERROR_STOPBITS     0x0073  // Bad stop bits for hardware
#define CCM_DOSERROR_PARITY       0x0074  // Bad parity for hardware
#define CCM_DOSERROR_BUFFERSIZE   0x0075  // Bad buffer size
#define CCM_DOSERROR_UNKNOWN      0x00FF  // No DOS error code available

/*
 * Format of the "Status" word returned in the CCM protocol.
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
#define CCM_ERROR_CODE( Status) ( Status & 0x00FF )
#define CCM_DOSERROR_CODE( Status ) ( ((Status >> 8) & 0x00FF) )
#define CCM_MAKE_STATUS(Error, DosError) ( (unsigned short)((Error & 0x00FF) | (DosError << 8)) )

/*
 * The GetInfo and SetInfo requests take a generic information
 * structure for the specific parameters.
 *
 * The following defines are the currently supported operations.
 */

/*
 * Line Status
 *
 * Line status is returned from read, write, and eventwait
 *
 */
typedef struct _CCM_LINESTATUS {
    USHORT LineError;     // Line error
    USHORT HoldStatus;    // Line hold status
    USHORT InputCount;    // characters in input queue
    USHORT OutputCount;   // characters in output queue
} CCM_LINESTATUS, *PCCM_LINESTATUS;

/*
 * Hold status
 */
#define CCM_HOLD_CTS      0x0001
#define CCM_HOLD_DSR      0x0002
#define CCM_HOLD_RLS      0x0004
#define CCM_HOLD_XOFF     0x0008
#define CCM_HOLD_XOFFSENT 0x0010
#define CCM_HOLD_EOF      0x0020

/*
 * Line Error status
 *
 * Line Error status is returned from read, write, and eventwait
 */
#define CCM_LINE_ERROR_BREAK        0x0001
#define CCM_LINE_ERROR_FRAMING      0x0002
#define CCM_LINE_ERROR_OVERRUN      0x0004  // SCC overrun
#define CCM_LINE_ERROR_QUEUEOVERRUN 0x0008  // Client buffer overrun
#define CCM_LINE_ERROR_PARITY       0x0010

/*
 * Set the remote ports LINEINFO structure
 */
typedef struct _CCM_LINEINFO {

    UCHAR  DataBits;

#define CCM_5_DATA       ((UCHAR)0x00)
#define CCM_6_DATA       ((UCHAR)0x01)
#define CCM_7_DATA       ((UCHAR)0x02)
#define CCM_8_DATA       ((UCHAR)0x03)

    UCHAR  StopBits;

#define CCM_1_STOP       ((UCHAR)0x00)
#define CCM_1_5_STOP     ((UCHAR)0x01)
#define CCM_2_STOP       ((UCHAR)0x02)

    UCHAR  Parity;

#define CCM_NONE_PARITY  ((UCHAR)0x00)
#define CCM_ODD_PARITY   ((UCHAR)0x01)
#define CCM_EVEN_PARITY  ((UCHAR)0x02)
#define CCM_MARK_PARITY  ((UCHAR)0x03)
#define CCM_SPACE_PARITY ((UCHAR)0x04)


} CCM_LINEINFO, *PCCM_LINEINFO;

#define CCM_INFO_TYPE_LINEINFO  1

/*
 * Special characters structure
 */
typedef struct _CCM_CHARS {
    UCHAR  EofChar;
    UCHAR  ErrorChar;
    UCHAR  BreakChar;
    UCHAR  EventChar;
    UCHAR  XonChar;
    UCHAR  XoffChar;
} CCM_CHARS, *PCCM_CHARS;

#define CCM_DEFAULT_XON     0x11
#define CCM_DEFAULT_XOFF    0x13

#define CCM_INFO_TYPE_CHARS  2

/*
 * Baudrate is a bit mask, which allows the supported rates
 * to be returned in one long word.
 *
 * When a rate is set, only one bit must be set. If more than
 * one bit is set on a set baud rate operation, the result
 * is undefined.
 */
#define CCM_BAUD_075          ((ULONG)0x00000001)
#define CCM_BAUD_110          ((ULONG)0x00000002)
#define CCM_BAUD_134_5        ((ULONG)0x00000004)
#define CCM_BAUD_150          ((ULONG)0x00000008)
#define CCM_BAUD_300          ((ULONG)0x00000010)
#define CCM_BAUD_600          ((ULONG)0x00000020)
#define CCM_BAUD_1200         ((ULONG)0x00000040)
#define CCM_BAUD_1800         ((ULONG)0x00000080)
#define CCM_BAUD_2400         ((ULONG)0x00000100)
#define CCM_BAUD_4800         ((ULONG)0x00000200)
#define CCM_BAUD_7200         ((ULONG)0x00000400)
#define CCM_BAUD_9600         ((ULONG)0x00000800)
#define CCM_BAUD_14400        ((ULONG)0x00001000)
#define CCM_BAUD_19200        ((ULONG)0x00002000)
#define CCM_BAUD_38400        ((ULONG)0x00004000)
#define CCM_BAUD_56K          ((ULONG)0x00008000)
#define CCM_BAUD_128K         ((ULONG)0x00010000)
#define CCM_BAUD_115200       ((ULONG)0x00020000)
#define CCM_BAUD_57600        ((ULONG)0x00040000)
#define CCM_BAUD_MASK         ((ULONG)0x0007FFFF)

typedef struct _CCM_BAUDRATE {
    ULONG  BaudRate;
} CCM_BAUDRATE, *PCCM_BAUDRATE;

#define CCM_INFO_TYPE_BAUDRATE  3

/*
 * Set the escape char used for certain data modes
 */
typedef struct _CCM_ESCAPECHAR {
    UCHAR EscapeChar;
} CCM_ESCAPECHAR, *PCCM_ESCAPECHAR;

#define CCM_INFO_TYPE_ESCAPECHAR  4

/*
 * Values in the data stream when Escape is set
 */
#define CCM_LSRMST_ESCAPE      0x0  // EscapeChar,0x0
#define CCM_LSRMST_LSR_DATA    0x1  // EscapeChar,0x1,LineStatus,RxDataByte
#define CCM_LSRMST_LSR_NODATA  0x2  // EscapeChar,0x2,LineStatus
#define CCM_LSRMST_MST         0x3  // EscapeChar,0x3,ModemStatus


typedef struct _CCM_CAPABILITIES {

    USHORT  MaxTxQueue;     // Maximum serial buffer sizes
    USHORT  MaxRxQueue;

    ULONG   SupportedBauds; // Mask of supported baud rates

    USHORT  Capabilities;   // Mask of capabilities

#define CCM_CAP_DTRDSR         0x0001
#define CCM_CAP_RTSCTS         0x0002
#define CCM_CAP_CD             0x0004
#define CCM_CAP_PARITY_CHECK   0x0008
#define CCM_CAP_XONXOFF        0x0010
#define CCM_CAP_SETXCHAR       0x0020
#define CCM_CAP_TOTALTIMEOUTS  0x0040
#define CCM_CAP_INTTIMEOUTS    0x0080
#define CCM_CAP_SPECIALCHARS   0x0100
#define CCM_CAP_16BITMODE      0x0200
#define CCM_CAP_MASK           0x03FF

    USHORT  SettableCaps;   // Mask of settable parameters

#define CCM_SET_PARITY         0x0001
#define CCM_SET_BAUD           0x0002
#define CCM_SET_DATABITS       0x0004
#define CCM_SET_STOPBITS       0x0008
#define CCM_SET_HANDSHAKING    0x0010
#define CCM_SET_PARITY_CHECK   0x0020
#define CCM_SET_CARRIER_DETECT 0x0040
#define CCM_SET_MASK           0x007F

    USHORT  DataBits;       // Data bit capabilities

#define CCM_DATABITS_5         0x0001
#define CCM_DATABITS_6         0x0002
#define CCM_DATABITS_7         0x0004
#define CCM_DATABITS_8         0x0008
#define CCM_DATABITS_16        0x0010
#define CCM_DATABITS_16X       0x0020

    USHORT  StopParity;     // Stop and parity bit capabilities

#define CCM_STOPBITS_10        0x0001
#define CCM_STOPBITS_15        0x0002
#define CCM_STOPBITS_20        0x0004
#define CCM_PARITY_NONE        0x0100
#define CCM_PARITY_ODD         0x0200
#define CCM_PARITY_EVEN        0x0400
#define CCM_PARITY_MARK        0x0800
#define CCM_PARITY_SPACE       0x1000

} CCM_CAPABILITIES, *PCCM_CAPABILITIES;

#define CCM_INFO_TYPE_CAPABILITIES  5


/*
 * Set various communication signal states
 *
 * The SignalState bits are in parallel with the signal mask bits.
 *
 * IE: CCM_SIGNAL_DTR (bit 0) state is set to SignalState bit 0
 *
 */
typedef struct _CCM_SETSIGNAL {
    ULONG SignalMask;    // Mask of signals to set

    UCHAR SignalState;   // Signal state to set
} CCM_SETSIGNAL, *PCCM_SETSIGNAL;

#define CCM_INFO_TYPE_SETSIGNAL 6

/*
 * Signal bits
 */
#define CCM_SIGNAL_DTR   0x0001
#define CCM_SIGNAL_RTS   0x0002
#define CCM_SIGNAL_XON   0x0004  // Virtual signal depends on handshake mode
#define CCM_SIGNAL_XOFF  0x0008  //  "" ""
#define CCM_SIGNAL_BREAK 0x0010

/*
 * Set the input and output buffer sizes.
 *
 * The caller can not request a buffer size greater than
 * the maximum size returned in the capabilities info.
 *
 * This call can still fail if there is not enough dynamic
 * client memory to allocate for the buffer.
 *
 * Some clients may have fixed buffer sizes.
 *
 * A failure of this call does not change a buffer size in effect.
 */
typedef struct _CCM_BUFFERSIZE {
    USHORT TxQueueSize;
    USHORT RxQueueSize;
} CCM_BUFFERSIZE, *PCCM_BUFFERSIZE;

#define CCM_INFO_TYPE_BUFFERSIZE 7

/*
 * Wait for an event to occur
 *
 * EventWait can also return read data if no reads are outstanding
 * at the client when a receive character event occurs.
 *
 * This is because some single threaded applications such as
 * Windows Terminal poll the event status, and when they see
 * a receive char event, expect to do a non-blocking driver read and
 * get the data. If the data must be transferred from the client
 * with a READPORT_REQUEST, the thread end up not waiting for the
 * data, and ignore any data that arrives later.
 */
#define CCM_EV_RXCHAR           0x0001  // Any Character received
#define CCM_EV_RXFLAG           0x0002  // Received certain character
#define CCM_EV_TXEMPTY          0x0004  // Transmitt Queue Empty
#define CCM_EV_CTS              0x0008  // CTS toggle
#define CCM_EV_DSR              0x0010  // DSR toggle
#define CCM_EV_DCD              0x0020  // DCD toggle
#define CCM_EV_BREAK            0x0040  // BREAK
#define CCM_EV_ERR              0x0080  // Status error reported
#define CCM_EV_RING             0x0100
#define CCM_EV_PERR             0x0200  // A printer error occurred
#define CCM_EV_RX80FULL         0x0400  // Receive buffer is 80 percent full
#define CCM_EV_USER1            0x0800
#define CCM_EV_USER2            0x1000


typedef struct _CCM_EVENTWAIT {
    ULONG      EventMask;
    USHORT     ReadData;    // Optional Read data returned
    CCM_LINESTATUS Stat;        // Line status
} CCM_EVENTWAIT, *PCCM_EVENTWAIT;

#define CCM_INFO_TYPE_EVENTWAIT 8

/*
 * Purge the data in the serial port
 */
typedef struct _CCM_PURGE {
    ULONG Flags;
} CCM_PURGE, *PCCM_PURGE;

#define CCM_INFO_TYPE_PURGE 9

#define CCM_PURGE_INPUT  0x0001
#define CCM_PURGE_OUTPUT 0x0002

/*
 * Set handshake and flow control modes on the port
 */
typedef struct _CCM_HANDFLOW {

    ULONG  ControlHandShake;

#define CCM_DTR_CONTROL        ((ULONG)0x01)
#define CCM_DTR_HANDSHAKE      ((ULONG)0x02)
#define CCM_DTR_MASK           ((ULONG)0x03)
#define CCM_CTS_HANDSHAKE      ((ULONG)0x08)
#define CCM_DSR_HANDSHAKE      ((ULONG)0x10)
#define CCM_DCD_HANDSHAKE      ((ULONG)0x20)
#define CCM_OUT_HANDSHAKEMASK  ((ULONG)0x38)
#define CCM_DSR_SENSITIVITY    ((ULONG)0x40)
#define CCM_ERROR_ABORT        ((ULONG)0x80000000L)

    ULONG  FlowReplace;

#define CCM_AUTO_TRANSMIT      ((ULONG)0x01)
#define CCM_AUTO_RECEIVE       ((ULONG)0x02)
#define CCM_ERROR_CHAR         ((ULONG)0x04)
#define CCM_RTS_CONTROL        ((ULONG)0x40)
#define CCM_RTS_HANDSHAKE      ((ULONG)0x80)
#define CCM_RTS_MASK           ((ULONG)0xc0)
#define CCM_TRANSMIT_TOGGLE    ((ULONG)0xc0)

    USHORT XonLimit;
    USHORT XoffLimit;
} CCM_HANDFLOW, *PCCM_HANDFLOW;

#define CCM_INFO_TYPE_HANDFLOW 10

/*
 * Get the current modem status register values.
 *
 * The delta values are required since a signal such as
 * DCD could have toggled between status updates.
 *
 * Most app's will treat a DCD toggle as DCD going low, and
 * hang up the connection.
 *
 * These values map to the original COM port values, since these
 * are the values returned in the Windows NT get modem status IOCTL's.
 */
typedef struct _CCM_MODEMSTATUS {

    ULONG State;

#define CCM_MODEM_DCTS     0x01
#define CCM_MODEM_DDSR     0x02
#define CCM_MODEM_TERI     0x04
#define CCM_MODEM_DDCD     0x08
#define CCM_MODEM_CTS      0x10
#define CCM_MODEM_DSR      0x20
#define CCM_MODEM_RI       0x40
#define CCM_MODEM_DCD      0x80

} CCM_MODEMSTATUS, *PCCM_MODEMSTATUS;

#define CCM_INFO_TYPE_MODEMSTATUS 11

//
// Read/write operation timeouts
//
// These are in milliseconds
//

typedef struct _CCM_TIMEOUTS {

    ULONG ReadIntervalTimeout;
    ULONG ReadTotalTimeoutMultiplier;
    ULONG ReadTotalTimeoutConstant;

    ULONG WriteTotalTimeoutMultiplier;
    ULONG WriteTotalTimeoutConstant;

} CCM_TIMEOUTS, *PCCM_TIMEOUTS;

#define CCM_INFO_TYPE_TIMEOUTS 12

/*
 * The numbering of the following CCM protocol requests and replies are
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
 * CDM Request: 0-63
 * CDM Reply:   128-191
 *
 * CPM Request: 64-95
 * CPM Reply:   192-223
 *
 * CCM Request: 96-127
 * CCM Reply:   224-255
 */
#define CCM_REQUEST_BASE 96
#define CCM_REPLY_BASE   224

/*
 * Do an Enumerate Ports request.
 *
 * Ports on the remote client are enumerated starting at
 * the specified index. If the error returns CCM_DOSERROR_NOFILES,
 * there are no more ports at that index or later on the client.
 *
 * To enumerate all of the ports on a remote client, start with
 * index == 0, and enumerate sequential indexes until CCM_DOSERROR_NOFILES
 * is returned.
 *
 */

/*
 * This structure describes a printer entry and allows
 * multiple to be enumerated at one time.
 *
 * The packing is:
 *
 *  CCM_ENUMPRINTER_REQUEST_REPLY
 *  ENUMSTRUCT 0
 *  <Strings for entry 0>
 *  ENUMSTRUCT 1
 *  <Strings for entry 1>
 *  ENUMSTRUCT 2
 *  <Strings for entry 2>
 *  ...
 *
 */
typedef struct _ENUMPORTSTRUCT {
    UCHAR  NameSize;       // Size of the port name following this header
    UCHAR  Pad;
    USHORT ExtraSize;      // Size of extra information if supplied following Comment
    ULONG  Flags;          // Flags
} ENUMPORTSTRUCT, *PENUMPORTSTRUCT;


typedef struct _CCM_ENUMPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests

    USHORT Index;            // Index of printer to return info on
    USHORT DataSize;         // Maximum amount of data to return

} CCM_ENUMPORT_REQUEST, *PCCM_ENUMPORT_REQUEST;
#define CCM_TYPE_ENUMPORT CCM_REQUEST_BASE // 96

/*
 * Reply from a CCM_ENUMPORT_REQUEST
 */
typedef struct _CCM_ENUMPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)

    USHORT Count;          // Count returned
    USHORT Size;           // Size
} CCM_ENUMPORT_REQUEST_REPLY, *PCCM_ENUMPORT_REQUEST_REPLY;
#define CCM_TYPE_ENUMPORT_REPLY CCM_REPLY_BASE // 224

/*
 * Open the given port device specified by name.
 */
typedef struct _CCM_OPENPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    UCHAR  NameSize;      // Size of the name following this header
    UCHAR  AccessMode;

} CCM_OPENPORT_REQUEST, *PCCM_OPENPORT_REQUEST;
#define CCM_TYPE_OPENPORT CCM_TYPE_ENUMPORT+1 // 97

/*
 * Reply from a CCM_OPENPORT_REQUEST
 */
typedef struct _CCM_OPENPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)
    USHORT Context;       // Handle for open port

    //
    // The following return the current settings
    // on the port. The application can issue IOCTL's to
    // change these modes if desired.
    //
    CCM_LINEINFO     LineInfo;
    CCM_BAUDRATE     CurrentBaud;
    CCM_CAPABILITIES Capabilities;
    CCM_BUFFERSIZE   QueueSizes;

} CCM_OPENPORT_REQUEST_REPLY, *PCCM_OPENPORT_REQUEST_REPLY;
#define CCM_TYPE_OPENPORT_REPLY CCM_TYPE_ENUMPORT_REPLY+1 // 225

/*
 * Close the given device releasing the Context value.
 */
typedef struct _CCM_CLOSEPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;

} CCM_CLOSEPORT_REQUEST, *PCCM_CLOSEPORT_REQUEST;
#define CCM_TYPE_CLOSEPORT CCM_TYPE_OPENPORT+1 // 98

/*
 * Reply from a CCM_CLOSEPPORT_REQUEST
 */
typedef struct _CCM_CLOSEPPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)

} CCM_CLOSEPORT_REQUEST_REPLY, *PCCM_CLOSEPORT_REQUEST_REPLY;
#define CCM_TYPE_CLOSEPORT_REPLY CCM_TYPE_OPENPORT_REPLY+1 // 226

/*
 * Read from the port on the current Context.
 *
 * NOTE: This can be a long term operation in which the
 *       reply may take some time.
 *
 *       It can be canceled by either closing the port or
 *       issueing CCM_CANCEL_REQUEST with the mpx id on the
 *       context value.
 */
typedef struct _CCM_READPPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Context;     // Handle for open printer
    USHORT ReadSize;    // Read data size

} CCM_READPORT_REQUEST, *PCCM_READPORT_REQUEST;
#define CCM_TYPE_READPORT CCM_TYPE_CLOSEPORT+1 // 99


/*
 * Reply from a CCM_READPORT_REQUEST
 *
 * The data read from the file port follows this header and is represented
 * by ReturnSize.
 */
typedef struct _CCM_READPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Result;      // Result
    USHORT AmountRead;  // Amount read
    CCM_LINESTATUS Stat;
} CCM_READPORT_REQUEST_REPLY, *PCCM_READPORT_REQUEST_REPLY;
#define CCM_TYPE_READPORT_REPLY CCM_TYPE_CLOSEPORT_REPLY+1 // 227

/*
 * Client-Server port write request.
 *
 * NOTE: This can be a long term operation in which the
 *       reply may take some time.
 *
 *       It can be canceled by either closing the port or
 *       issueing CCM_CANCEL_REQUEST with the mpx id on the
 *       context value.
 */
typedef struct _CCM_WRITEPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Context;       // Handle for open printer
    USHORT WriteSize;     // Write data size

} CCM_WRITEPORT_REQUEST, *PCCM_WRITEPORT_REQUEST;
#define CCM_TYPE_WRITEPORT CCM_TYPE_READPORT+1 // 100

/*
 * Reply from a CCM_WRITE_PORT_REQUEST
 */
typedef struct _CCM_WRITEPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;

    USHORT Result;        // Result
    USHORT AmountWrote;   // Amount wrote
    CCM_LINESTATUS Stat;
} CCM_WRITEPORT_REQUEST_REPLY, *PCCM_WRITEPORT_REQUEST_REPLY;
#define CCM_TYPE_WRITEPORT_REPLY CCM_TYPE_READPORT_REPLY+1 // 228

/*
 * Get port status
 */
typedef struct _CCM_GETPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  InfoType;       // Info type
    USHORT MaxInfoSize;    // Maximum amount of port specific data

    ULONG  Flags;          // Type specific flags

} CCM_GETPORT_REQUEST, *PCCM_GETPORT_REQUEST;
#define CCM_TYPE_GETPORT CCM_TYPE_WRITEPORT+1 // 101

/*
 * Reply from a CCM_GETPORT_REQUEST
 *
 * NOTE: This is only sent as a result of a GETPORT_REQUEST.
 *
 *       CCM_ASYNC_STATUS is sent when the remote Citrix client sends
 *       an unsolicited status update.
 */
typedef struct _CCM_GETPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;
    USHORT InfoSize;       // Length of extra data returned

} CCM_GETPORT_REQUEST_REPLY, *PCCM_GETPORT_REQUEST_REPLY;
#define CCM_TYPE_GETPORT_REPLY CCM_TYPE_WRITEPORT_REPLY+1 // 229

/*
 * Set Port Status
 */
typedef struct _CCM_SETPORT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  InfoType;
    USHORT InfoSize;

} CCM_SETPORT_REQUEST, *PCCM_SETPORT_REQUEST;
#define CCM_TYPE_SETPORT CCM_TYPE_GETPORT+1 // 102

/*
 * Reply from a CCM_SETPORT_REQUEST
 *
 * This reply is required since the port may not have
 * accepted the status byte set, or it changed the condition.
 */
typedef struct _CCM_SETPORT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;
    ULONG  Flags;          // Per InfoType result data

} CCM_SETPORT_REQUEST_REPLY, *PCCM_SETPORT_REQUEST_REPLY;
#define CCM_TYPE_SETPORT_REPLY CCM_TYPE_GETPORT_REPLY+1

/*
 *  Packet that allows the remote device server on the Citrix Client
 *  to send status changes to the redirector.
 *
 * NOTE: This status update packet may be sent by the remote device
 *       server on the Citrix Client without a GETSTATUS request any
 *       time after a given port has been opened. This allows async
 *       updates of notable events such as DCD drop, or PaperOut on
 *       the without having to poll. The conditions that generate
 *       a status update are set by the status mask(s) supplied to
 *       SetPort.
 *
 *       At the current time, this is the only way Status is updated
 *       without the host ever asking for it.
 */

// Reserve this entry
#define CCM_TYPE_ASYNC_STATUS_REQUEST CCM_TYPE_SETPORT+1 // 71

typedef struct _CCM_ASYNC_STATUS {
    UCHAR  h_type;
    UCHAR  Pad;

    USHORT Context;      // Context the status is for
    ULONG  Status;

} CCM_ASYNC_STATUS, *PCCM_ASYNC_STATUS;
#define CCM_TYPE_ASYNC_STATUS CCM_TYPE_SETPORT_REPLY+1

/*
 * This is the first packet sent by the host and supplies
 * host version information.
 *
 * There is no reply to this packet since the client sent
 * the information at ICA connect time.
 */

typedef struct _CCM_CONNECT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Size;          // Size of this request header
    USHORT VersionLow;
    USHORT VersionHigh;
    USHORT Flags;         // Flags to be defined

} CCM_CONNECT_REQUEST, *PCCM_CONNECT_REQUEST;
#define CCM_TYPE_CONNECT CCM_TYPE_ASYNC_STATUS_REQUEST+1

// Reserve this entry
#define CCM_TYPE_CONNECT_REPLY CCM_TYPE_ASYNC_STATUS+1 // 72


/*
 * Cancel I/O
 */
typedef struct _CCM_CANCEL_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;        // Context of open port
    UCHAR  CancelId;       // MpxId of request to cancel
    ULONG  Mask;

} CCM_CANCEL_REQUEST, *PCCM_CANCEL_REQUEST;
#define CCM_TYPE_CANCEL CCM_TYPE_CONNECT+1 // 70

/*
 * Masks for cancel requests
 */
#define CANCEL_READ  0x0001
#define CANCEL_WRITE 0x0002
#define CANCEL_EVENT 0x0004
#define CANCEL_ALL   0x8000   // CancelId is ignored

/*
 * Reply from a CCM_CANCEL_REQUEST
 */
typedef struct _CCM_CANCEL_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;
    ULONG  CanceledMask;   // Ones canceled

} CCM_CANCEL_REQUEST_REPLY, *PCCM_CANCEL_REQUEST_REPLY;
#define CCM_TYPE_CANCEL_REPLY CCM_TYPE_CONNECT_REPLY+1

/*
 * This is the maximum packet size define used by transport drivers
 * for calculating buffer space and timeout parameters.
 */
#define CCM_MAX_PACKET_SIZE (sizeof(CCM_OPENPORT_REQUEST_REPLY))

#pragma pack()

#endif // _CCMWIRE_

