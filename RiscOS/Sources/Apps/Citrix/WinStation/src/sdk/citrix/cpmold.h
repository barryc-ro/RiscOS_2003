
/*************************************************************************
*
* cpmold.h
*
* Older Wire Protocol Header for CITRIX Client Printer Mapping Protocol
*
* The new protocol level is in cpmwire.h, this is for compatibility with
* existing clients/hosts.
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
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 04/28/94
*
* Log:
*
*
*
*************************************************************************/


#ifndef _CPMOLD_
#define _CPMOLD_

/*
 * All of the structures in this header must be packed. Otherwise, they
 * would have to manually be padded to 4 byte size multiples.
 *
 * Parameters are alaigned on natural boundries, and should pose no
 * problems to RISC architectures with alignment restrictions.
 */

//#pragma pack(1)

/*
 * General structures and formats used by the printer wire protocol.
 *
 * NOTE: DL stands for "Down Level"
 */

#define CPMDL_MIN_VERSION  0x0001
#define CPMDL_MAX_VERSION  0x0001

/*
 * Open the given printer device specified by the number in the channel
 * byte of the request.
 *
 * NOTE: MpxId's are defined for open for (2) reasons:
 *
 *  1: Multiple open requests could be sent for the same port to the
 *     client, this allows which reply that succeeds to be identified.
 *
 *  2: Open is infrequent compared to write/read, so the over head of
 *     the byte is low.
 */

typedef struct _CPM_OPEN_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    UCHAR  Channel;       // This is the ICA definition. IE: Virtual_LPT1, etc.
    UCHAR  AccessMode;

} CPM_OPEN_REQUEST, *PCPM_OPEN_REQUEST;
#define CPM_TYPE_OPEN CPM_REQUEST_BASE

/*
 * Reply from a CPM_OPEN_REQUEST
 */
typedef struct _CPM_OPEN_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Handle for open device
    USHORT Result;        // Result Code(s)

} CPM_OPEN_REQUEST_REPLY, *PCPM_OPEN_REQUEST_REPLY;
#define CPM_TYPE_OPEN_REPLY CPM_REPLY_BASE

#define sizeof_CPM_OPEN_REQUEST_REPLY	6

/*
 * Close the given device releasing the Context value.
 *
 * NOTE: There is no reply to this request. It is expected that the server
 *       will always close the context, and we can free it immediately
 *       after issuing this call.
 *
 *       This also means that the re-director file client must wait until all
 *       outstanding command request packets have returned until it frees
 *       its internal data structures after issuing this packet.
 *
 */
typedef struct _CPM_CLOSE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;

} CPM_CLOSE_REQUEST, *PCPM_CLOSE_REQUEST;
#define CPM_TYPE_CLOSE CPM_TYPE_OPEN+1

/*
 * There is no CPM_CLOSE_REPLY protocol packet.
 * The number is reserved here by this definition.
 */
#define CPM_TYPE_CLOSE_REPLY CPM_TYPE_OPEN_REPLY+1

/*
 * Read from the device on the current virtual circuit.
 * At this point, OPEN must have been successful, and since there is
 * one device per connection, Context is implied.
 *
 * NOTE: This is currently not issued by the CPM redirector, but has
 *       been defined since future ECP (Enhanced Capabilities Parallel) Ports
 *       do support reading for supporting printers such as the Windows Printing
 *       System. These printers would be accessed by the Windwos Printing System
 *       Application/Driver directly without going through the NT spooler. So we
 *       need to have the same NT device API support as a native ECP port.
 */

#define CPM_TYPE_READ CPM_TYPE_CLOSE+1

/*
 * Reply from a CPM_READ_REQUEST
 *
 * The data read from the file server follows this header and is represented
 * by ReturnSize.
 */
#define CPM_TYPE_READ_REPLY CPM_TYPE_CLOSE_REPLY+1


/*
 * Write to the device on the current virtual circuit.
 *
 * The write data follows this header in the data stream.
 *
 * NOTE: Currently, there is no acknowledgment that the data
 *       was actually accepted by the printer. It is just sent
 *       over the connection. If a bug sends data over an invalid
 *       connection (not open) it is dropped.
 *
 *  We can only send data to the printer when the status is
 *  CPM_QUEUE_EMPTY.
 *
 *  This is for printer writes that are less than 255 bytes (modem configurations)
 *
 */
typedef struct _CPM_WRITE1_REQUEST {
    UCHAR  h_type;

    UCHAR  WriteSize;

} CPM_WRITE1_REQUEST, *PCPM_WRITE1_REQUEST;
#define CPM_TYPE_WRITE1 CPM_TYPE_READ+1

#define sizeof_CPM_WRITE1_REQUEST	2

/*
 * Reply from a CPM_WRITE1_REQUEST
 *
 * NOTE: Currently there is no reply, but is here for future use if needed.
 */
#define CPM_TYPE_WRITE1_REPLY CPM_TYPE_READ_REPLY+1

/*
 * Write to the device on the current virtual circuit.
 *
 * The write data follows this header in the data stream.
 *
 * NOTE: Currently, there is no acknowledgment that the data
 *       was actually accepted by the printer. It is just sent
 *       over the connection. If a bug sends data over an invalid
 *       connection (not open) it is dropped.
 *
 *  This is for printer writes that exceed 255 bytes (network configurations)
 */
typedef struct _CPM_WRITE2_REQUEST {
    UCHAR  h_type;
    UCHAR  Pad;

    USHORT  WriteSize;

} CPM_WRITE2_REQUEST, *PCPM_WRITE2_REQUEST;
#define CPM_TYPE_WRITE2 CPM_TYPE_WRITE1+1

/*
 * Reply from a CPM_WRITE2_REQUEST
 *
 * NOTE: Currently there is no reply, but is here for future use if needed.
 */
#define CPM_TYPE_WRITE2_REPLY CPM_TYPE_WRITE1_REPLY+1

/*
 * Get device status
 *
 * NOTE: Currently not sent.
 */
typedef struct _CPM_GETSTATUS_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

} CPM_GETSTATUS_REQUEST, *PCPM_GETSTATUS_REQUEST;
#define CPM_TYPE_GETSTATUS CPM_TYPE_WRITE2+1

#define sizeof_CPM_GETSTATUS_REQUEST	2

/*
 * Reply from a CPM_GETSTATUS_REQUEST
 *
 * NOTE: This is only sent as a result of a GETSTATUS_REQUEST.
 *
 *       STATUS_UPDATE is sent when the remote Citrix client sends
 *       an unsolicited status update.
 */
typedef struct _CPM_GETSTATUS_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT StatusWord;

    ULONG Result;

} CPM_GETSTATUS_REQUEST_REPLY, *PCPM_GETSTATUS_REQUEST_REPLY;
#define CPM_TYPE_GETSTATUS_REPLY CPM_TYPE_WRITE2_REPLY+1

/*
 * Set Printer Status Bits
 *
 * This sets the devices setable non-data bits.
 *
 */
typedef struct _CPM_SETSTATUS_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    UCHAR  ControlByte;
    UCHAR  Pad;

} CPM_SETSTATUS_REQUEST, *PCPM_SETSTATUS_REQUEST;
#define CPM_TYPE_SETSTATUS CPM_TYPE_GETSTATUS+1

/*
 * Reply from a CPM_SETSTATUS_REQUEST
 *
 * This reply is required since the port may not have
 * accepted the status byte set, or it changed the condition.
 * IE: Set PrinterSelect, PrinterReturns PaperOut, Error, Not ready, etc.
 */
typedef struct _CPM_SETSTATUS_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT StatusWord;

    ULONG  Result;
} CPM_SETSTATUS_REQUEST_REPLY, *PCPM_SETSTATUS_REQUEST_REPLY;
#define CPM_TYPE_SETSTATUS_REPLY CPM_TYPE_GETSTATUS_REPLY+1


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
typedef struct _CPM_STATUS_UPDATE {
    UCHAR  h_type;
    UCHAR  Pad;

    USHORT StatusWord;

} CPM_STATUS_UPDATE, *PCPM_STATUS_UPDATE;
#define CPM_TYPE_STATUS_UPDATE CPM_TYPE_SETSTATUS_REPLY+1

/*
 * This is the first packet sent by the host and supplies
 * host version information.
 *
 * There is no reply to this packet since the client sent
 * the information at ICA connect time.
 */

typedef struct _CPM_CONNECT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Size;          // Size of this request header
    USHORT VersionLow;
    USHORT VersionHigh;
    USHORT Flags;         // Flags to be defined

} CPM_CONNECT_REQUEST, *PCPM_CONNECT_REQUEST;
#define CPM_TYPE_CONNECT CPM_TYPE_SETSTATUS+1

#define sizeof_CPM_CONNECT_REQUEST	10

/*
 * This is the maximum packet size define used by transport drivers
 * for calculating buffer space and timeout parameters.
 */
#define CPMDL_MAX_PACKET_SIZE (sizeof_CPM_CONNECT_REQUEST)

/*
 * This is the defined maximum number of requests that the HOST
 * can request from the remote CPM device Server.
 */
#define CPMDL_MAXREQUEST_COUNT  4

//#pragma pack()

#endif // _CPMOLD_

