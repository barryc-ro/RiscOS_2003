/*************************************************************************
*
*  TW20.H   ( ICA 2.0 thinwire )
*
*     Structures and definitions for Thinwire windows protocols.
*
*  Copyright Citrix Systems Inc. 1992
*
*  $Log$
*  
*     Rev 1.2   21 Apr 1997 16:57:42   TOMA
*  update
*  
*     Rev 1.1   06 May 1996 17:02:04   jeffm
*  allow include recursion
*  
*     Rev 1.0   27 Sep 1994 21:23:30   bradp
*  Initial revision.
*  
*     Rev 1.0   12 May 1994 17:03:56   bradp
*  Initial revision.
*  
*
*  Nomenclature:
*        Packet - data that will go across the wire
*        PacketHdr - stuff on the front of the packet to keep current
*                    twhost code happy and to communicate with tpss on
*                    what is to be done with the packet
*        vcallhdr - the header needed by VCALL.SYS vdd to get the
*                   data from twhost to tpss
*        message - all of the data needed; vcall headers, packet headers,
*                  packet data
*        messagehdr - a combination of vcallhdr and packethdr
*
*
*     General
*       |VCPACKETHDR..|.packet hdr..............|...packet data.....|
*       |.....message header....................|...packet data.....|
*       |............message........................................|
*
*     Display Messages
*
*       |VCPACKETHDR..|....TWINPACKETHDR........|...packet data.....|
*       |...........TWINMSGHDR..................|...packet data.....|
*       |...........message.........................................|
*
*     System Messages Sent from HOST to REMOTE
*
*       |VCPACKETHDR..|....TWINPACKETHDR........|.TWINSYSSENDPACKET.|
*       |...........TWINMSGHDR..................|.TWINSYSSENDPACKET.|
*       |...........TWINSYSSENDMSG..................................|
*
*     Messages Received by HOST from REMOTE
*
*       |VCPACKETHDR..|....TWINRCVPACKETHDR.....|..TWINRCVPACKET....|
*       |VCPACKETHDR..|...............TWINRCVPACKET.................|
*       |...........TWINRCVMSG......................................|
*
*       |VCPACKETHDR..|....TWINRCVPACKETHDR.....|..TWINSYSRCVPACKET.|
*       |VCPACKETHDR..|...............TWINRCVPACKET.................|
*       |...........TWINRCVMSG......................................|
*
*       |VCPACKETHDR..|....TWINRCVPACKETHDR.....|..TWINMOUSEPACKET..|
*       |VCPACKETHDR..|...............TWINRCVPACKET.................|
*       |...........TWINRCVMSG......................................|
*
*
**************************************************************************/

#ifndef __TW20_H__
#define __TW20_H__

#pragma pack(1)

#define INCL_THINWIN


// size big enough to hold TWINMSGHDR and windows command data
#define TWIN_MAXIMUM_BUFFER   4500
#define TWIN_MAX_SYSSENDDATA     64       // not including command byte
#define TWIN_MAX_MOUSE_PACKETS  5

// Error codes
#define  TWERROR_TIMEOUT            0x0E01
#define  TWERROR_INVALID_VIDEO      0x0E02
#define  TWERROR_MEMORY_ERROR_BUF   0x0E03
#define  TWERROR_MEMORY_ERROR_SHADOW  0x0E04
#define  TWERROR_INVALID_MODE       0x0E05
#define  TWERROR_VCALL_OPENFAIL     0x0E06
#define  TWERROR_DRVLOADFAIL        0x0E07
#define  TWERROR_INVALID_LINK       0x0EE1
#define  TWERROR_INPROG_AT_LINK     0x0EE2
#define  TWERROR_NO_LINK_FOCUS      0x0EE3

/*************************************************************************
*     Packet structures
*        These structures represent the thinwire packet data
*        sent from Host to Remote, and are encapsulated in VCALL
*        messages to get from Windows to TPSS (see below)
**************************************************************************/

/*------------------------------------------------------------------------
   HOST ONLY SEND Data Packet Header
   this is the header that the thinwire host code always presumes
   exists.  we carry this through TPSS so that the LTD can use it
   as a work area and not have to move the data.
------------------------------------------------------------------------*/

typedef struct _TWINPACKETHDR {
   USHORT   twCommand;        // Host: type (sys, disp), LTD: ICA command
   USHORT   twLen;            // length of viewer data Not incl. this hdr
};
typedef struct _TWINPACKETHDR TWINPACKETHDR;
typedef struct _TWINPACKETHDR far *PTWINPACKETHDR;
typedef struct _TWINPACKETHDR far * far *PPTWINPACKETHDR;

#define  TWIN_PACKET_DISPLAY  1
#define  TWIN_PACKET_SYSTEM   2

/*------------------------------------------------------------------------
   SEND SYSTEM Command Data
      Used to send System commands from the HOST to the REMOTE
------------------------------------------------------------------------*/
typedef struct _TWINSYSSENDPACKET {
   UCHAR twSysSendCmd;                 // system command
   //BUGBUG eventually turn this into a union of all packets?
   UCHAR twSysSendData[TWIN_MAX_SYSSENDDATA];  // system data
};
typedef struct _TWINSYSSENDPACKET TWINSYSSENDPACKET;
typedef struct _TWINSYSSENDPACKET far *PTWINSYSSENDPACKET;
typedef struct _TWINSYSSENDPACKET far * far *PPTWINSYSSENDPACKET;

/*------------------------------------------------------------------------
   RECEIVE Packet Header
      This header is placed on all received data to identify it
      to the Windows code.
------------------------------------------------------------------------*/

typedef struct _TWINRCVPACKETHDR {
   UCHAR twRcvType;           // type of data (see below)
};
typedef struct _TWINRCVPACKETHDR TWINRCVPACKETHDR;
typedef struct _TWINRCVPACKETHDR far *PTWINRCVPACKETHDR;
typedef struct _TWINRCVPACKETHDR far * far *PPTWINRCVPACKETHDR;

#define  TWIN_RCVPACKET_MOUSE    1
#define  TWIN_RCVPACKET_SYSTEM   2

/*------------------------------------------------------------------------
   RECEIVED System Data
      Used to send System commands from the REMOTE to the HOST
------------------------------------------------------------------------*/

// commands used in TWINSYSRCVPACKET,  twSysRcvCmd
#define  TWIN_SYSCMD_SUSPEND        1     // suspend output to VCALL
#define  TWIN_SYSCMD_RESUME         2     // resume output to VCALL
#define  TWIN_SYSCMD_INITRESPONSE   3     // return code from init request

#define TWIN_MAX_SYSRCVDATA        10     // not including command byte

// this is the FIXED length of a system command received by HOST
// it does not fluctuate based on structure size in order to prevent
// a surprise incompatibility with an old version of link
#define TWIN_SIZEOF_SYSCMD       11

// TWIN_SYSCMD_INITRESPONSE
//  used to synchronize the initialization of windows
//  this can also pass back some data about the client
typedef struct _TWINSYSCMDINITRESP {
   USHORT twInitRespRC;          // results of init call (0 means it worked)
   USHORT twInitRespFlags;       // response flags (unused)
   ULONG  twInitRespCache;       // cache size
};
typedef struct _TWINSYSCMDINITRESP TWINSYSCMDINITRESP;
typedef struct _TWINSYSCMDINITRESP far *PTWINSYSCMDINITRESP;
typedef struct _TWINSYSCMDINITRESP far * far *PPTWINSYSCMDINITRESP;

// TWIN_SYSCMD_SUSPEND
//  used by client to tell host that client is no longer displaying
//  windows data, and host should stop sending it - this occurs when
//  the client acts on a local hot key causing a focus change
//  No data is associated with this command
//  This is also used by TPM to switch to background

// TWIN_SYSCMD_RESUME
//  used by client to tell host that client now ready to take data
//  and host should execute a repaint
//  this occurs when the client acts on a local hot key causing a focus change
//  No data is associated with this command
//  This is also used by TPM to switch to foreground

/* XLATOFF */
typedef struct _TWINSYSRCVPACKET {
   UCHAR twSysRcvCmd;                  // system command
   union {
      TWINSYSCMDINITRESP twIR;
      char Max[TWIN_MAX_SYSRCVDATA];   // force == to TWIN_SIZEOF_SYSCMD
   } twSC;
};
typedef struct _TWINSYSRCVPACKET TWINSYSRCVPACKET;
typedef struct _TWINSYSRCVPACKET far *PTWINSYSRCVPACKET;
typedef struct _TWINSYSRCVPACKET far * far *PPTWINSYSRCVPACKET;
/* XLATON */


/*------------------------------------------------------------------------
   RECEIVED MOUSE data packet
      Used to send Mouse data from Remote to Host
------------------------------------------------------------------------*/

typedef struct _TWINMOUSEPACKET {
   UCHAR  twMouState;               // button state
   USHORT twMouAbsX;                // absolute X location
   USHORT twMouAbsY;                // absolute Y location
};
typedef struct _TWINMOUSEPACKET TWINMOUSEPACKET;
typedef struct _TWINMOUSEPACKET far *PTWINMOUSEPACKET;
typedef struct _TWINMOUSEPACKET far * far *PPTWINMOUSEPACKET;

#define  TWIN_MOUSESTATE_MOVED   0x01

/* XLATOFF */
/*------------------------------------------------------------------------
   RECEIVED System Data
      Used to send System commands from the REMOTE to the HOST
------------------------------------------------------------------------*/

typedef struct _TWINRCVPACKET {
   TWINRCVPACKETHDR twRPktHdr;
   union {
      TWINSYSRCVPACKET Sys;
      TWINMOUSEPACKET Mouse;
   } twRData;
};
typedef struct _TWINRCVPACKET TWINRCVPACKET;
typedef struct _TWINRCVPACKET far *PTWINRCVPACKET;
typedef struct _TWINRCVPACKET far * far *PPTWINRCVPACKET;
/* XLATON */

#pragma pack()

#endif //__TW20_H__
