
/*************************************************************************
*
*  INPUT.C
*
*  Modme Protocol Driver - input state machine
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Kurt Perry (6/2/94)
*
*  $Log$
*  
*     Rev 1.14   15 Apr 1997 16:52:28   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   21 Mar 1997 16:07:24   bradp
*  update
*  
*     Rev 1.13   30 Jan 1996 18:39:52   bradp
*  update
*  
*     Rev 1.12   29 Jan 1996 20:10:38   bradp
*  update
*  
*     Rev 1.11   29 Jan 1996 18:02:46   bradp
*  update
*  
*     Rev 1.9   21 Jul 1995 18:27:34   bradp
*  update
*  
*     Rev 1.8   28 Jun 1995 22:14:16   bradp
*  update
*  
*     Rev 1.7   03 May 1995 10:20:22   butchd
*  clib.h now standard
*  
*     Rev 1.6   02 May 1995 14:23:56   butchd
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

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdmodem.h"


/*=============================================================================
==   External procedures defined
=============================================================================*/

int WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );
int DevicePoll( PPD, PDLLPOLL );


/*=============================================================================
==   External procedures used
=============================================================================*/

int PdNext( PPD, USHORT, PVOID );
int ModemProcessInput( PPD, LPBYTE, USHORT );
int ModemStateMachine( PPD, PPDMODEM, PDLLPOLL );


/*=============================================================================
==   Defines and structures
=============================================================================*/

/*=============================================================================
==   Local data
=============================================================================*/


/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to Pd data structure
 *    pBuffer (input)
 *       Points to input buffer containing data.
 *    ByteCount (input)
 *       Number of bytes of data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
DeviceProcessInput( PPD pPd, LPBYTE pBuffer, USHORT ByteCount )
{
    int Status;
    PPDMODEM pPdModem = pPd->pPrivate;

    /*
     *  Does modem PD have input focus?
     */
    if ( pPdModem != NULL && pPdModem->fFocus ) {

        /*
         *  Send data to Modem state machine
         */
        Status =  ModemProcessInput( pPd, pBuffer, ByteCount );

        /*
         *  Just return no data on callback
         */
        if ( pPdModem->fCallback ) {
            Status = CLIENT_STATUS_NO_DATA;
        }
    } 
#ifdef DOS
    else 
        Status = (pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount );
#else
    Status = (pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount );
#endif

    return( Status );
}


/*******************************************************************************
 *
 *  DevicePoll
 *
 *  device polling
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DevicePoll( PPD pPd, PDLLPOLL pPdPoll )
{
    int rc = CLIENT_STATUS_SUCCESS;
    int rc2;
    PPDMODEM pPdModem = pPd->pPrivate;

    /*
     *  Does modem PD have input focus?
     */
    if ( pPdModem->fFocus && pPd->fRecvStatusConnect ) {

        /*
         *  Drive state machine
         */
        rc = ModemStateMachine( pPd, pPdModem, pPdPoll );

        /*
         *  Just return no data on callback
         */
        if ( pPdModem->fCallback ) {
            rc = CLIENT_STATUS_NO_DATA;
        }
    }

    /*
     *  Poll next level PD
     */
    rc2 = PdNext( pPd, DLL__POLL, pPdPoll );
    if ( rc2 == CLIENT_STATUS_TTY_CONNECTED ) {
        TRACE(( TC_PD, TT_API1, "PdModem: Recv CLIENT_STATUS_TTY_CONNECTED" ));
#ifndef DOS
        if ( pPdModem->fEchoTTY ) {
            TRACE(( TC_PD, TT_API1, "PdModem: Send CLIENT_STATUS_TTY_CONNECTED" ));
            rc = rc2;
        } else 
#endif
        {
            rc2 = CLIENT_STATUS_SUCCESS;
        }
    }

    return( (rc != CLIENT_STATUS_SUCCESS) ? rc : rc2 );
}
