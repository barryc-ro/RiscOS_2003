/*****************************************************************************
* 
*  vdlic.h
* 
*  Client Token-Based Licensing Protocol
* 
*  Copyright 1998 Citrix Systems Inc.
* 
*  Author:  Bruce Fortune  2-16-98
* 
*  $Log$
*  
*     Rev 1.1   29 Apr 1998 12:50:22   wf20r
*  Update to match picasso
*  
*     Rev 1.0   Feb 24 1998 20:38:22   brucef
*  Initial revision.
*  
*     Rev 1.0   Feb 19 1998 22:02:24   briang
*  Initial revision.
* 
*****************************************************************************/

#ifndef __VDLIC_H__
#define __VDLIC_H__

#include <citrix\ica.h>
#include <citrix\ica-c2h.h>        // for VD_C2H structure

// All of the wire structures in this header must be packed
#pragma pack(1)


//////////////////////////////////////////////////////////////////////////////
// Timeout for client response to license operations
//
#define MAX_LICENSE_TIMEOUT        (20*1000)

//////////////////////////////////////////////////////////////////////////////
// virtual channel initialization packet
//

typedef struct _VDLIC_C2H
{
    VD_C2H  Header;
    ULONG   KeyExchangeAlg;
    ULONG   LicProtocolVer;        // version of MS licensing protocol
} VDLIC_C2H, * PVDLIC_C2H;

//////////////////////////////////////////////////////////////////////////////
// H2C - Send Command With Data

typedef struct _LIC_SEND_COMMAND_WITH_DATA
{
    UCHAR       iCommand;
    USHORT      cbData;
} LIC_SEND_COMMAND_WITH_DATA, * PLIC_SEND_COMMAND_WITH_DATA;

//////////////////////////////////////////////////////////////////////////////
// C2H - Response

typedef struct _LIC_COMMAND_RESPONSE
{
    UCHAR       iCommand;
    USHORT      cbData;
} LIC_COMMAND_RESPONSE, * PLIC_COMMAND_RESPONSE;

#define LIC_H2C_BASE                    0x00
#define LIC_COMMAND_REQUEST_LICENSE     (LIC_H2C_BASE)
#define LIC_COMMAND_SEND_LICENSE        (LIC_H2C_BASE + 0x1)

//// Client-to-host packet identifiers

#define LIC_C2H_BASE                    0x80
#define LIC_COMMAND_REQUEST_RESPONSE    (LIC_C2H_BASE)


//// turn off packing
#pragma pack()


#endif // __VDLIC_H__
