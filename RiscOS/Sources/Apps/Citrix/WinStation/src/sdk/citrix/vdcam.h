/*****************************************************************************
* 
*  vdcam.h
* 
*  Client Audio Mapping "on-the-wire" protocol definitions
* 
*  Copyright 1997 Citrix Systems Inc.
* 
*  Author:  David Pope  8-6-97
* 
*  $Log$
*  Revision 1.1  1998/06/19 17:13:21  smiddle
*  Merged in Beta2 code. A few redundant header files removed, various new ones
*  added. It all compiles and sometimes it runs. Mostly it crashes in the new
*  ini code though.
*  Added a check for the temporary ICA file being created OK. If not then it gives
*  a warning that the scrap directory might need to be set up.
*  Upped version number to 0.40 so that there is room for some bug fixes to the
*  WF 1.7 code.
*
*  Version 0.40. Tagged as 'WinStation-0_40'
*
*  
*     Rev 1.4   17 Dec 1997 13:08:48   davidp
*  added Extra field to OPEN packet for wider format support
*  
*     Rev 1.3   13 Dec 1997 19:49:32   davidp
*  added Citrix ADPCM format
*  
*     Rev 1.2   26 Sep 1997 17:46:18   davidp
*  cosmetic
*  
*     Rev 1.1   26 Sep 1997 17:44:48   davidp
*  Removed PURGE, adjusted pack() pragma
*   
*      Rev 1.0   19 Sep 1997 13:17:56   BillG
*  Initial revision.
* 
*****************************************************************************/

#ifndef __VDCAM_H__
#define __VDCAM_H__

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"        // for VD_C2H structure

// All of the wire structures in this header must be packed
//#pragma pack(1)

#define VERSION_CLIENTL_VDCAM   1
#define VERSION_CLIENTH_VDCAM   1 

#define VERSION_HOST_CAM        1
#define VERSION_FLOW_CONTROL    1

//////////////////////////////////////////////////////////////////////////////
// format IDs are of the following form:
//

typedef struct _FORMAT_ID
{
    USHORT  iFormat;
    USHORT  wSubFormat;
} FORMAT_ID, * PFORMAT_ID;

// values for iFormat field above
//
#define FORMAT_NONE             0x0000  // for error & other n/a conditions
#define FORMAT_LINEAR_PCM       0x0001
#define FORMAT_CTX_ADPCM        0x0002

// values for wSubFormat field above
//

// for FORMAT_NONE
#define SUBFMT_NONE             0x0000  // for error & other n/a conditions

// for FORMAT_LINEAR_PCM
//      Bits    0-1     Sample rate
//          00 = 8 kHz
//          01 = 11.025 kHz
//          10 = 22.05 kHz
//          11 = 44.1 kHz
//      Bits    2-3     RESERVED, set to zero
//      Bit     4       Channels
//          0 = Mono
//          1 = Stereo
//      Bits    5-7     RESERVED, set to zero
//      Bit     8       Bit depth
//          0 = 8-bit
//          1 = 16-bit
//      Bits    9-15    RESERVED, set to zero

#define SUBFMT_LINEAR_PCM_8KHZ              0x0000
#define SUBFMT_LINEAR_PCM_11KHZ             0x0001
#define SUBFMT_LINEAR_PCM_22KHZ             0x0002
#define SUBFMT_LINEAR_PCM_44KHZ             0x0003
#define SUBFMT_LINEAR_PCM_MASK_RATE         0x0003

#define SUBFMT_LINEAR_PCM_MONO              0x0000
#define SUBFMT_LINEAR_PCM_STEREO            0x0010
#define SUBFMT_LINEAR_PCM_MASK_CHANNELS     0x0010

#define SUBFMT_LINEAR_PCM_8BIT              0x0000
#define SUBFMT_LINEAR_PCM_16BIT             0x0100
#define SUBFMT_LINEAR_PCM_MASK_DEPTH        0x0100


// for FORMAT_CTX_ADPCM
//      Bits    0-1     Sample rate
//          00 = 8 kHz
//          01 = 11.025 kHz
//          10 = 22.05 kHz
//          11 = 44.1 kHz
//      Bits    2-3     RESERVED, set to zero
//      Bit     4       Channels
//          0 = Mono
//          1 = Stereo
//      Bits    5-7     RESERVED, set to zero
//      Bit     8       Bit depth
//          0 = 2-bit
//      Bits    9-15    RESERVED, set to zero

#define SUBFMT_CTX_ADPCM_8KHZ              0x0000
#define SUBFMT_CTX_ADPCM_11KHZ             0x0001
#define SUBFMT_CTX_ADPCM_22KHZ             0x0002
#define SUBFMT_CTX_ADPCM_44KHZ             0x0003
#define SUBFMT_CTX_ADPCM_MASK_RATE         0x0003

#define SUBFMT_CTX_ADPCM_MONO              0x0000
#define SUBFMT_CTX_ADPCM_STEREO            0x0010
#define SUBFMT_CTX_ADPCM_MASK_CHANNELS     0x0010

#define SUBFMT_CTX_ADPCM_2BIT              0x0000
#define SUBFMT_CTX_ADPCM_MASK_DEPTH        0x0100


//////////////////////////////////////////////////////////////////////////////
//  Device identifier struct for the packets that refer to a particular device

typedef unsigned char DEVICE_ID;

// values for DEVICE_ID (this is an enum, not a bitmask)
//
#define CAM_DEVICE_NONE             0x00    // no device type (for error cond.)
#define CAM_DEVICE_PCM_OUT          0x01    // PCM output


//////////////////////////////////////////////////////////////////////////////
// virtual channel initialization packet
//

typedef struct _VDCAM_C2H
{
    VD_C2H  Header;
    USHORT  cbMaxDataSize;      // maximum data block size (could be less
                                //   than VD packet size max)
    UCHAR   iFCVersionH;        // highest version of flow control supported
                                //   by the client
} VDCAM_C2H, * PVDCAM_C2H;

#define sizeof_VDCAM_C2H	(sizeof(VD_C2H) + 3)

//////////////////////////////////////////////////////////////////////////////
// H2C - Initialize connection to signify host's readiness

typedef struct _CAM_INIT
{
    UCHAR       iCommand;       // set to CAM_COMMAND_INIT
    UCHAR       iCAMVersionH;   // highest version of CAM supported by host
    UCHAR       iFCVersionH;    // highest version of CAM flow control
                                //   supported by the host
} CAM_INIT, * PCAM_INIT;

#define sizeof_CAM_INIT	3

//////////////////////////////////////////////////////////////////////////////
// H2C - Open device

typedef struct _CAM_OPEN_REQUEST
{
    UCHAR       iCommand;       // set to CAM_COMMAND_OPEN
    DEVICE_ID   DeviceID;       // unique device identifier
				// Citrix-assigned format ID
    USHORT      FormatID_iFormat;
    USHORT      FormatID_wSubFormat;
    UCHAR       Extra[4];       // format-specific extra information
} CAM_OPEN_REQUEST, * PCAM_OPEN_REQUEST;

#define sizeof_CAM_OPEN_REQUEST	10

//////////////////////////////////////////////////////////////////////////////
// H2C - Close device

typedef struct _CAM_CLOSE_REQUEST
{
    UCHAR       iCommand;       // set to CAM_COMMAND_CLOSE
    DEVICE_ID   DeviceID;       // which device to close
} CAM_CLOSE_REQUEST, * PCAM_CLOSE_REQUEST;

#define sizeof_CAM_CLOSE_REQUEST	2

//////////////////////////////////////////////////////////////////////////////
// H2C - Write to device

typedef struct _CAM_WRITE_REQUEST
{
    UCHAR       iCommand;       // set to CAM_COMMAND_WRITE
    DEVICE_ID   DeviceID;       // device to write to
    USHORT      oData;          // offset to data to be written
    USHORT      cbData;         // how much data
    UCHAR       reserved[2];
} CAM_WRITE_REQUEST, * PCAM_WRITE_REQUEST;

//////////////////////////////////////////////////////////////////////////////
// H2C - Request capability packet

typedef struct _CAM_REQUEST_CAPABILITY
{
    UCHAR       iCommand;       // set to CAM_COMMAND_REQUEST_CAPINFO
    DEVICE_ID   DeviceID;       // device whose capability to query
    USHORT      oFormatIDs;     // offset to FORMAT_ID structs
    USHORT      nFormatIDs;     // number of FORMAT_ID structs to follow
} CAM_REQUEST_CAPABILITY, * PCAM_REQUEST_CAPABILITY;

#define sizeof_CAM_REQUEST_CAPABILITY	6

//////////////////////////////////////////////////////////////////////////////
// C2H - Capability information packet

typedef struct _CAM_CAPABILITY_INFO
{
    UCHAR       iCommand;           // set to CAM_COMMAND_CAPABILITY_INFO
    DEVICE_ID   DeviceID;           // which device's capability info this is
    USHORT      oFormatIDs;         // offset to format IDs
    USHORT      nFormatIDs;         // number of format IDs
} CAM_CAPABILITY_INFO, * PCAM_CAPABILITY_INFO;

#define sizeof_CAM_CAPABILITY_INFO	6

//////////////////////////////////////////////////////////////////////////////
// C2H - Reset

typedef struct _CAM_RESET
{
    UCHAR       iCommand;       // set to CAM_COMMAND_RESET
    DEVICE_ID   DeviceID;       // device to reset
    UCHAR       iReason;        // why this device needed resetting
} CAM_RESET, * PCAM_RESET;

#define sizeof_CAM_RESET	3

// values for iReason field above

// the host sent the client something before sending an INIT packet
#define CAM_RESET_NOT_INITIALIZED       0x01
// the host attempted to operate on an invalid device (not open when it should
//   have been, or something just plain weird)
#define CAM_RESET_BAD_DEVICE_ID         0x02
// this host tried to access an unknown device type (unknown iDevType entry
//     in a DEVICE_ID)
#define CAM_RESET_BAD_DEVICE_TYPE       0x03
// the host sent more commands than the client had storage for (i.e. flow
//   control failed)
#define CAM_RESET_COMMAND_OVERFLOW      0x04
// the host sent more data than the client had storage for (i.e. flow control
//   failed)
#define CAM_RESET_DATA_OVERFLOW         0x05
// the host sent a packet with an unknown iCommand field
#define CAM_RESET_UNKNOWN_PACKET        0x06
// the host sent a command packet that was invalid for the device or protocol
// state (e.g. a WRITE packet to an unopened device)
#define CAM_RESET_INVALID_COMMAND       0x07
// the host sent a command with an unknown format ID (i.e. iFormat or
//   iSubFormat was unknown)
#define CAM_RESET_UNKNOWN_FORMAT_ID     0x08
// there was an internal error on the client that requires a protocol reset
#define CAM_RESET_INTERNAL_ERROR        0x09

// there was a hardware-generated error executing a command for a given device
//   (e.g. hardware device was already opened by a client-side app on a Windows
//   client)
#define CAM_RESET_DEVICE_ERROR          0xFF


//////////////////////////////////////////////////////////////////////////////
// H2C - Acknowledge client RESET

typedef struct _CAM_RESET_ACK
{
    UCHAR       iCommand;       // set to CAM_COMMAND_RESET_ACK
    DEVICE_ID   DeviceID;       // device whose reset to acknowledge
} CAM_RESET_ACK, * PCAM_RESET_ACK;

#define sizeof_CAM_RESET_ACK	2

//////////////////////////////////////////////////////////////////////////////
// C2H - Resource Ack

typedef struct _CAM_RESOURCE_ACK
{
    UCHAR   iCommand;           // set to CAM_COMMAND_RESOURCE_ACK
    UCHAR   iResource[2];       // which resource pool we're talking about
    UCHAR   nReleased[2];       // how many units to release
} CAM_RESOURCE_ACK, * PCAM_RESOURCE_ACK;

#define sizeof_CAM_RESOURCE_ACK	5

// values defined for the iResource field above
#define CAM_RESOURCE_COMMANDS   0x0001
#define CAM_RESOURCE_DATA       0x0002


//////////////////////////////////////////////////////////////////////////////
// packet IDs for iCommand structures above

#define CAM_H2C_BASE        0x00
#define CAM_C2H_BASE        0x80

// Host-to-client packet identifiers

// These are the numbers to be used in the iCommand fields of the headers of
// the above structures.  
#define CAM_COMMAND_INIT                (CAM_H2C_BASE)
#define CAM_COMMAND_OPEN                (CAM_H2C_BASE + 0x1)
#define CAM_COMMAND_CLOSE               (CAM_H2C_BASE + 0x2)
#define CAM_COMMAND_WRITE               (CAM_H2C_BASE + 0x3)
#define CAM_COMMAND_RESET_ACK           (CAM_H2C_BASE + 0x4)
#define CAM_COMMAND_REQUEST_CAPINFO     (CAM_H2C_BASE + 0x5)

//// Client-to-host packet identifiers

#define CAM_COMMAND_RESOURCE_ACK        (CAM_C2H_BASE)
#define CAM_COMMAND_RESET               (CAM_C2H_BASE + 0x1)
#define CAM_COMMAND_CAPABILITY_INFO     (CAM_C2H_BASE + 0x2)


//// turn off packing
//#pragma pack()


#endif // __VDCAM_H__
