/*************************************************************************
*
*   misc.c
*  
*   ICA 3.0 WinStation Driver - misc packets
*  
*    PACKET_STOP_REQUEST   0  client->host stop sending screen data       
*    PACKET_STOP_OK        0  host->client stop accepted
*    PACKET_BEEP           2  DosBeep
*    PACKET_CALLBACK       n  callback (enter auto-answer)
*  
*   Copyright 1994, Citrix Systems Inc.
*  
*   Author: Brad Pedersen (4/9/94)
*  
*   $Log$
*  
*     Rev 1.16   15 Jul 1997 15:48:38   davidp
*  updated headers for hydra/picasso surgery
*  
*     Rev 1.15   27 May 1997 13:39:18   richa
*  Fix for Boundless
*  
*     Rev 1.14   15 Apr 1997 18:17:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   21 Mar 1997 16:09:50   bradp
*  update
*  
*     Rev 1.13   22 Jan 1997 16:46:54   terryt
*  client data
*  
*     Rev 1.12   04 Nov 1995 15:34:34   andys
*  beep status
*  
*     Rev 1.11   03 May 1995 11:45:56   kurtp
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
#include "citrix/hydrix.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"


/*=============================================================================
==   Functions referenced
=============================================================================*/

int PdCall( PWD, USHORT, PVOID );


/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaStopOk( PWD, LPBYTE, USHORT );            
void IcaBeep( PWD, LPBYTE, USHORT );              
void IcaCallback( PWD, LPBYTE, USHORT );          


/*******************************************************************************
 *
 *  IcaStopOk  (PACKET_STOP_OK)
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
IcaStopOk( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    TRACE(( TC_WD, TT_API1, "STOP_OK" ));

    if ( !pWd->TimeoutStopRequest ) {
        /*
         *  Set stop ok received flag
         */
        pWd->fReceivedStopOk = TRUE;
    
        /*
         *  Clear stop ok timeout
         */
        pWd->TimeoutStopRequest = 0L;
    }
}


/*******************************************************************************
 *
 *  IcaBeep 
 *
 *  PACKET_BEEP
 *  P1 - frequency (high byte)
 *  P2 - frequency (low byte)
 *  P3 - duration (high byte)
 *  P4 - duration (low byte)
 *  
 *  This command generates sound from the speaker.  P1:P2 specifies the 
 *  frequency of the sound in hertz.  P3:P4 specifies the duration of the 
 *  sound in milliseconds.
 *  
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
IcaBeep( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    USHORT Frequency;
    USHORT Duration;
    int rc;

    Frequency = (USHORT) *((PUSHORT)pInputBuffer);
    pInputBuffer += 2;

    Duration = (USHORT) *((PUSHORT)pInputBuffer);

    TRACE(( TC_WD, TT_API1, "BEEP: %u, %u", Frequency, Duration ));

    rc = VioBeep( Frequency, Duration );
    ASSERT( rc == 0, rc );

    // send CLIENT_STATUS_BEEPED to engine
    pWd->fSendStatusBeeped = TRUE;
}


/*******************************************************************************
 *
 *  IcaCallback  (PACKET_CALLBACK)
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
IcaCallback( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PDSETINFORMATION PdSetInfo;

    TRACE(( TC_WD, TT_API1, "CALLBACK: %u", InputCount ));

    /* enable callback in pd */
    PdSetInfo.PdInformationClass  = PdCallback;
    (void) PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );

    /* disable output processing */
    pWd->fConnected = FALSE;

}

/*******************************************************************************
 *
 *  IcaClientData
 *
 *  PACKET_SET_CLIENT_DATA
 *  
 *  Gives an arbitrary peice of data to the client indexed by an ID token.
 *  This in intended to be informational data from the host.
 *  Initially this is User Name, User Domain Name, and Server Name.
 *
 *  The ID token is 8 byte data.  The first 3 bytes are an oem id (CTX).
 *  The following 4 bytes are arbitrary, and the last byte is always 0 (NULL).
 *  
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
IcaClientData( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    CLIENTDATANAME ClientName;
    PINFOBLOCK pNew;
    PINFOBLOCK *pLast;
    PINFOBLOCK pOld;
    PVOID pData;

    ASSERT( InputCount >= INFODATA_ID_SIZE, InputCount );
    ASSERT( pInputBuffer[7] == '\0', pInputBuffer[7] );

    ClientName[0] = pInputBuffer[0];
    ClientName[1] = pInputBuffer[1];
    ClientName[2] = pInputBuffer[2];
    ClientName[3] = pInputBuffer[3];
    ClientName[4] = pInputBuffer[4];
    ClientName[5] = pInputBuffer[5];
    ClientName[6] = pInputBuffer[6];
    ClientName[7] = pInputBuffer[7];

    TRACE(( TC_WD, TT_API1, "SET_CLIENT_DATA: bc %u Token %s", InputCount-8, ClientName ));

    /*
     * Allocate and copy the data
     */
    if ( InputCount - INFODATA_ID_SIZE  > 0 ) {
        if ( ( pData = malloc( InputCount - INFODATA_ID_SIZE ) ) == NULL ) {
            ASSERT( FALSE, 0 );
            return;
        }
        memcpy( pData, pInputBuffer + 8, InputCount - INFODATA_ID_SIZE );
    }

    /*
     * Allocate the new data block
     */ 
    pNew = malloc( sizeof( INFOBLOCK ) );
    if ( pNew == NULL ) {
            ASSERT( FALSE, 0 );
            free( pData );
            return;
    }


    /*
     * Search for and delete any previous matching entry
     */
    for ( pOld = pWd->pInfoBlockList, pLast = &pWd->pInfoBlockList;
          pOld != NULL;
          pLast = &pOld->pNext, pOld = pOld->pNext ) {
        if ( !memcmp( pOld->Id, ClientName, sizeof( ClientName ) ) ) {
            if ( pOld->pData )
                free( pOld->pData );
            *pLast = pOld->pNext;
            free( pOld );
            break;
        }
    }

    /*
     * Initialize the new entry and put it on the list
     */
    memcpy( pNew->Id, ClientName, sizeof( ClientName ) );
    pNew->Length = InputCount - INFODATA_ID_SIZE;
    if ( pNew->Length )
        pNew->pData = pData;
    else
        pNew->pData = NULL;
    pNew->pNext = pWd->pInfoBlockList;
    pWd->pInfoBlockList = pNew;

}
