
/***************************************************************************
*
*  TDTCPRO.H
*
*  This module contains private PD defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: John Richardsion from tdtcpnov.h
*
*  $Log$
*  
*     Rev 1.4   15 Apr 1997 17:48:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.3   07 Apr 1995 11:39:48   butchd
*  support for DOS, WIN16, and WIN32 builds
*  
*     Rev 1.2   20 Feb 1995 20:34:22   terryt
*  Change PD Open failure message
*  
*     Rev 1.1   16 Feb 1995 17:40:24   terryt
*  Spruce up name resolvers
*  
*     Rev 1.0   16 Sep 1994 17:48:38   JohnR
*  Initial revision.
*
*
****************************************************************************/


/*=============================================================================
==   Defines
=============================================================================*/

#define CITRIX_TCP_PORT  1494 // Citrix ICA Port, same as WinView 2.x

#define TCPNAMSZ      16

#define TCP_TIMEOUT   30

//#define EHOSTNOTFOUND 70        // Host not found error

/*
 *  Citrix defined errors
 */
//#define NRC_NO_NETBIOS  0xfe    /* netbios not available */


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  TCP PD structure
 */
typedef struct _PDTCP {

    LPBYTE pInBuf;              // pointer to input buffer
    int   CNum;                // Connection (socket) handle
    int   Connect;             // Connect flag

    NAMEADDRESS NameAddress;   // Name and address of connection

} PDTCP, * PPDTCP;


/*=============================================================================
==   Prototypes
=============================================================================*/

extern int  Call( PPD, ULONG, UINT, int * );
extern int  Send( PPD, int, char *, int, PUSHORT);
extern int  Receive( PPD, int, char *, int, int * );
extern void Hangup( PPD, int);
extern int  Check( PPD, int);
