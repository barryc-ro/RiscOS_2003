/*************************************************************************
*
*  THINWIN.H
*     Structures and definitions for Thinwire windows protocols.
*
*  Copyright Citrix Systems Inc. 1992
*
*  $Log$
*  
*     Rev 1.2   21 Apr 1997 16:57:40   TOMA
*  update
*  
*     Rev 1.1   06 May 1996 17:02:06   jeffm
*  allow include recursion
*  
*     Rev 1.0   21 Jul 1994 15:31:52   andys
*  Initial revision.
*  
*     Rev 1.7   01 Jul 1994 14:11:30   andys
*  add changes for distant images
*
*     Rev 1.6   05 Apr 1993 12:10:52   andys
*  add not focused error code
*
*     Rev 1.5   06 Mar 1993 15:07:40   andys
*  add new error codes
*
*     Rev 1.4   16 Feb 1993 11:05:04   andys
*  new error
*
*     Rev 1.3   09 Feb 1993 17:47:22   andys
*  fix an .inc translate problem
*
*     Rev 1.2   07 Feb 1993 17:14:50   andys
*  make receive packet changes for the receive buffer accumulation stuff
*
*     Rev 1.1   02 Feb 1993 13:47:20   andys
*  add system input things
*
*     Rev 1.0   15 Dec 1992 18:27:42   andys
*  Initial revision.
*
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

#ifndef INCL_THINWIN
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
typedef struct _TWINPACKETHDR FAR *PTWINPACKETHDR;
typedef struct _TWINPACKETHDR FAR * FAR *PPTWINPACKETHDR;

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
typedef struct _TWINSYSSENDPACKET FAR *PTWINSYSSENDPACKET;
typedef struct _TWINSYSSENDPACKET FAR * FAR *PPTWINSYSSENDPACKET;

/*------------------------------------------------------------------------
   RECEIVE Packet Header
      This header is placed on all received data to identify it
      to the Windows code.
------------------------------------------------------------------------*/

typedef struct _TWINRCVPACKETHDR {
   UCHAR twRcvType;           // type of data (see below)
};
typedef struct _TWINRCVPACKETHDR TWINRCVPACKETHDR;
typedef struct _TWINRCVPACKETHDR FAR *PTWINRCVPACKETHDR;
typedef struct _TWINRCVPACKETHDR FAR * FAR *PPTWINRCVPACKETHDR;

#define  TWIN_RCVPACKET_MOUSE    1
#define  TWIN_RCVPACKET_SYSTEM   2

/*------------------------------------------------------------------------
   RECEIVED System Data
      Used to send System commands from the REMOTE to the HOST
------------------------------------------------------------------------*/

// commands used in TWINSYSRCVPACKET,  twSysRcvCmd
#define  TWIN_SYSCMD_SUSPEND        1     // suspend output to VCALL
#define  TWIN_SYSCMD_RESUME         2     // resume output to VCALL
#define  TWIN_SYSCMD_INITRESPONSE   3     // return from init request
#define  TWIN_SYSCMD_DOBID          4     // distant object IDs

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
   USHORT twInitRespFlags;       // some flags
   ULONG  twInitRespCache;       // cache size
   USHORT twInitDOCount;         // count of DO entries coming
};
typedef struct _TWINSYSCMDINITRESP TWINSYSCMDINITRESP;
typedef struct _TWINSYSCMDINITRESP FAR *PTWINSYSCMDINITRESP;
typedef struct _TWINSYSCMDINITRESP FAR * FAR *PPTWINSYSCMDINITRESP;

#define TWIN_INITFLAGS_RESETDO   0x0001

// TWIN_SYSCMD_DOBID
//  used to pass the distant object ids back to host
typedef struct _TWINSYSCMDDOBID {
   USHORT twDOBHandle;           // handle for host to use
   ULONG  twDOBID;               // ID in packet
   ULONG  twDOBUnused;           // unused
};
typedef struct _TWINSYSCMDDOBID TWINSYSCMDDOBID;
typedef struct _TWINSYSCMDDOBID FAR *PTWINSYSCMDDOBID;
typedef struct _TWINSYSCMDDOBID FAR * FAR *PPTWINSYSCMDDOBID;

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
      TWINSYSCMDDOBID twDOB;
      char Max[TWIN_MAX_SYSRCVDATA];   // force == to TWIN_SIZEOF_SYSCMD
   } twSC;
};
typedef struct _TWINSYSRCVPACKET TWINSYSRCVPACKET;
typedef struct _TWINSYSRCVPACKET FAR *PTWINSYSRCVPACKET;
typedef struct _TWINSYSRCVPACKET FAR * FAR *PPTWINSYSRCVPACKET;
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
typedef struct _TWINMOUSEPACKET FAR *PTWINMOUSEPACKET;
typedef struct _TWINMOUSEPACKET FAR * FAR *PPTWINMOUSEPACKET;

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
typedef struct _TWINRCVPACKET FAR *PTWINRCVPACKET;
typedef struct _TWINRCVPACKET FAR * FAR *PPTWINRCVPACKET;
/* XLATON */

#ifndef INCL_NO_VCALL_MSG

/*************************************************************************
*    VCALL Message Structures
*
**************************************************************************/

/*------------------------------------------------------------------------
   SEND Message Header used for DISPLAY and SYSTEM commands sent
        from HOST to REMOTE
        This is only a header tacked onto the display/system buffer
------------------------------------------------------------------------*/

typedef struct _TWINMSGHDR {
   VCPACKETHDR twVCHdr;       // vcall header
   TWINPACKETHDR twPktHdr;    // thin wire windows header
};
typedef struct _TWINMSGHDR TWINMSGHDR;
typedef struct _TWINMSGHDR FAR *PTWINMSGHDR;
typedef struct _TWINMSGHDR FAR *FAR *PPTWINMSGHDR;

/*------------------------------------------------------------------------
   SEND Message used to send SYSTEM commands to the REMOTE
        Structure represents the entire message, not just a header.
------------------------------------------------------------------------*/

typedef struct _TWINSYSSENDMSG {
   TWINMSGHDR twSysSendHdr;    // system send header
   TWINSYSSENDPACKET twSysSendPkt;
};
typedef struct _TWINSYSSENDMSG TWINSYSSENDMSG;
typedef struct _TWINSYSSENDMSG FAR *PTWINSYSSENDMSG;
typedef struct _TWINSYSSENDMSG FAR *FAR *PPTWINSYSSENDMSG;

/*------------------------------------------------------------------------
   RECEIVE Message used to receive MOUSE and SYSTEM commands sent
           from REMOTE to HOST
           Structure represents the entire message, not just a header.
------------------------------------------------------------------------*/
/* XLATOFF */
typedef struct _TWINRCVMSG {
   VCPACKETHDR twRVCHdr;    // vcall header
   TWINRCVPACKET twRPkt;
};
typedef struct _TWINRCVMSG TWINRCVMSG;
typedef struct _TWINRCVMSG FAR *PTWINRCVMSG;
typedef struct _TWINRCVMSG FAR *FAR *PPTWINRCVMSG;
/* XLATON */
#endif
#endif //INCL_THINWIN
