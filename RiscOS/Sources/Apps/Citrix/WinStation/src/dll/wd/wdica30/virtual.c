
/*************************************************************************
*
*   virtual.c
*
*   ICA 3.0 WinStation Driver - virtual i/o packets
*
*   PACKET_VIRTUAL_WRITE0   2  write 1 byte of virtual data
*   PACKET_VIRTUAL_WRITE1   n  write n bytes of virtual data (short)
*   PACKET_VIRTUAL_WRITE2   nn write n bytes of virtual data (long)
*   PACKET_VIRTUAL_ACK      3  ack virtual channel (slide window)
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Brad Pedersen (4/9/94)
*
*   $Log$
*  
*     Rev 1.12   15 Apr 1997 18:18:02   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   21 Mar 1997 16:09:54   bradp
*  update
*  
*     Rev 1.11   12 Jul 1995 10:06:04   bradp
*  update
*  
*     Rev 1.10   10 Jul 1995 10:07:44   JohnR
*  Put back virtual flush code that got deleted from 1.4 -> 1.5
*  
*
*     Rev 1.9   03 May 1995 11:46:42   kurtp
*  update
*
*************************************************************************/

/*
 *  Includes
 */
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaVirtualWrite( PWD, LPBYTE, USHORT );
void IcaVirtualAck( PWD, LPBYTE, USHORT );
void IcaVirtualFlush( PWD, LPBYTE, USHORT );

/*=============================================================================
==   External Functions Used
=============================================================================*/

int VdCall( PWD, USHORT, USHORT, PVOID );


/*******************************************************************************
 *
 *  IcaVirtualWrite  (PACKET_VIRTUAL_WRITE)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaVirtualWrite( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDVDWRITEHOOK pVdHook;
    VIRTUALCLASS Type;
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    Type = *pInputBuffer++;     // virtual channel id

    pVdHook = &pIca->VdWriteHook[ Type ];

    TRACE(( TC_WD, TT_API1, "VIRTUAL_WRITE(%u): bc %u", Type, InputCount-1 ));

    if ( pVdHook->pProc )
        (*pVdHook->pProc)( (LPVOID) pVdHook->pData, (USHORT) Type, pInputBuffer, (USHORT) (InputCount - 1) );
}


/*******************************************************************************
 *
 *  IcaVirtualAck  (PACKET_VIRTUAL_ACK)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaVirtualAck( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
}


/*******************************************************************************
 *
 *  IcaVirtualFlush (PACKET_VIRTUAL_FLUSH)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaVirtualFlush( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    int rc;
    VDFLUSH Param;
    VDSETINFORMATION Info;

    // Put this in the VDFLUSH structure
    Param.Channel = *pInputBuffer++;     // virtual channel id
    Param.Mask = *pInputBuffer++;     // Input/Output flags

    // Setup the paramters for the VD call
    Info.VdInformationClass  = VdFlush;
    Info.pVdInformation      = &Param;
    Info.VdInformationLength = sizeof(Param);

    TRACE(( TC_WD|TC_CPM, TT_API1, "VIRTUAL_FLUSH: Channel %d, Mask 0x%x", Param.Channel, Param.Mask));

    // Do the call
    rc = VdCall( pWd, (USHORT) Param.Channel, VD__SETINFORMATION, &Info );

    TRACE(( TC_WD|TC_CPM, TT_API1, "VIRTUAL_FLUSH: rc %d", rc));
}

