
/*************************************************************************
*
*   NEICA.C
*
*   ICA Name Enumerator
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Brad Pedersen (11/7/95)
*
*   $Log$
*  
*     Rev 1.17   03 May 1997 17:15:08   thanhl
*  update
*  
*     Rev 1.16   03 May 1997 15:57:32   thanhl
*  update
*  
*     Rev 1.15   28 Apr 1997 19:24:00   thanhl
*  update
*  
*     Rev 1.14   16 Apr 1997 13:53:16   BillG
*  update
*  
*     Rev 1.13   15 Apr 1997 16:18:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   10 Mar 1997 12:13:50   butchd
*  Changed ICA_BR_ENUM version check for WF 2.0 to version 2 (not 5 as originally planned)
*  
*     Rev 1.11   05 Mar 1997 10:52:44   butchd
*  don't return published app names from pre-WF 2.0 master browsers
*  
*     Rev 1.10   11 Feb 1997 13:57:24   jeffm
*  multiple browser support
*  
*     Rev 1.9   03 Feb 1996 20:16:34   bradp
*  update
*  
*     Rev 1.8   29 Jan 1996 15:28:18   marcb
*  update
*  
*     Rev 1.7   23 Jan 1996 19:28:04   bradp
*  update
*  
*     Rev 1.6   03 Jan 1996 18:17:28   bradp
*  update
*  
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
#include "citrix/icabrows.h"
#include "citrix/ibrowerr.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/neapi.h"
#include "../inc/ne.h"



/*=============================================================================
==   External Functions Defined
=============================================================================*/

int DeviceEnumerate( PNE, PNEENUMERATE );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

int _BrEnum( PNEENUMERATE );


/*=============================================================================
==   Functions used
=============================================================================*/

int IoOpen( void );
void IoClose( void );
int BrRequestMasterBrowser( PICA_BR_ADDRESS );
int BrRead( int, PICA_BR_ADDRESS, void *, int, int * );
int BrWrite( PICA_BR_ADDRESS, void *, int );
int BrPurgeInput( void );


/*=============================================================================
==   Global Data
=============================================================================*/

extern BYTE            G_TcpBrowserAddress[];
extern BYTE            G_IpxBrowserAddress[];
extern BYTE            G_NetBiosBrowserAddress[];
extern ADDRESS         G_TcpBrowserAddrList[];     
extern ADDRESS         G_IpxBrowserAddrList[];     
extern ADDRESS         G_NetBiosBrowserAddrList[]; 
extern ICA_BR_ADDRESS  G_BrowserAddress;
extern int             G_fBrowserAddress;
extern PICA_BR_ADDRESS G_pLocalAddr;
extern int             G_LocalAddrCount;
extern int             G_LanaNumber;
extern USHORT          G_RequestRetry;
extern USHORT          G_ReadTimeout;


/*******************************************************************************
 *
 *  DeviceEnumerate
 *
 *   DeviceEnumerate returns an array of application servers
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name resolver data structure
 *    pNeEnum (input/output)
 *       pointer to the structure NEENUMERATE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DeviceEnumerate( PNE pNe, PNEENUMERATE pNeEnum )
{
    int rc;

    /*
     *  Initialize browser parameters
     */
    memcpy( G_TcpBrowserAddress, pNeEnum->pTcpBrowserAddress, sizeof(ADDRESS) );
    memcpy( G_IpxBrowserAddress, pNeEnum->pIpxBrowserAddress, sizeof(ADDRESS) );
    memcpy( G_NetBiosBrowserAddress, pNeEnum->pNetBiosBrowserAddress, sizeof(ADDRESS) );

    if(pNeEnum->pTcpBrowserAddrList)
        memcpy( G_TcpBrowserAddrList, pNeEnum->pTcpBrowserAddrList, sizeof(ADDRESS) * MAX_BROWSERADDRESSLIST);

    if(pNeEnum->pIpxBrowserAddrList)
        memcpy( G_IpxBrowserAddrList, pNeEnum->pIpxBrowserAddrList, sizeof(ADDRESS) * MAX_BROWSERADDRESSLIST );

    if(pNeEnum->pNetBiosBrowserAddrList)
        memcpy( G_NetBiosBrowserAddrList, pNeEnum->pNetBiosBrowserAddrList, sizeof(ADDRESS) * MAX_BROWSERADDRESSLIST );

    G_RequestRetry = pNeEnum->BrowserRetry;
    G_ReadTimeout  = pNeEnum->BrowserTimeout;
    G_LanaNumber   = pNeEnum->LanaNumber;

    /*
     *  Open I/O
     */
    if ( rc = IoOpen() )
        goto badopen;

    /*
     *  Enumerate names
     */
    rc = _BrEnum( pNeEnum );

    TRACE(( TC_PD, TT_API1, "DeviceEnumerate: bc %u, rc=%u",
            pNeEnum->ByteCount, rc ));

    /*
     *  Close I/O
     */
    IoClose();

    /*
     *  Map browser error codes to client error codes
     */
badopen:
    if ( rc ) {
        switch ( rc ) {
            case BR_ERROR_NO_MASTER :
                rc = CLIENT_ERROR_NO_MASTER_BROWSER;
                break;
            case BR_ERROR_IPX_NOT_AVAILABLE :
            case BR_ERROR_TCP_NOT_AVAILABLE :
            case BR_ERROR_NETBIOS_NOT_AVAILABLE :
                rc = CLIENT_ERROR_TRANSPORT_NOT_AVAILABLE;
                break;
            case BR_ERROR_NOT_ENOUGH_MEMORY :
                rc = CLIENT_ERROR_NO_MEMORY;
                break;
            case BR_ERROR_IO_ERROR :
            case BR_ERROR_READ_TIMEOUT :
                rc = CLIENT_ERROR_IO_ERROR;
                break;
        }
        if ( rc != CLIENT_ERROR_BUFFER_TOO_SMALL ) 
            pNeEnum->ByteCount = 0;
        pNeEnum->pName[0] = '\0';
    }

    return( rc );
}


/*******************************************************************************
 *
 *  _BrEnum
 *
 *  This routine sends a packet to the browser and waits for
 *  an enumeration of ICA browsers to be returned
 *
 *
 *  ENTRY:
 *     pNeEnum (input/output)
 *        pointer to the structure NEENUMERATE
 *
 *  EXIT:
 *     BR_ERROR_SUCCESS - no error
 *
 ******************************************************************************/

int
_BrEnum( PNEENUMERATE pNeEnum )
{
    BYTE Buffer[ MAX_BR_BUFFER ];
    ICA_BR_ADDRESS RemoteAddress;
    ICA_BR_ADDRESS Address;
    ICA_BR_REQUEST_ENUM Request;
    PICA_BR_ENUM pEnum;
    USHORT Sequence;
    char * pNameBuffer;
    char * pNewNameBuffer;
    int NameBufferLength;
    int NewNameBufferLength;
    int NameOffset;
    int NamesLength;
    int BytesRead;
    int rc;

    /*
     *  Get address of browser
     */
    if ( rc = BrRequestMasterBrowser( &Address ) )
        goto badmaster;

    /*
     *  Initialize length of name buffer
     */
    NameBufferLength = 0;
    pNameBuffer = NULL;

    /*
     *  Send enumeration request to browser
     */
tryagain:
    memset( &Request, 0, sizeof(Request) );
    Request.Header.ByteCount = sizeof(Request);
    Request.Header.Version   = 1;
    Request.Header.Command   = BR_REQUEST_ENUM;
    Request.Header.Signature = BR_SIGNATURE;
    Request.DataType = pNeEnum->DataType;
    Request.EnumReqFlags = pNeEnum->EnumReqFlags;
    BrPurgeInput();
    if ( rc = BrWrite( &Address, &Request, Request.Header.ByteCount ) )
        goto badwrite;

    /*
     *  Initialize counters
     */
    NameOffset = 0;
    Sequence   = 0;

    /*
     *  Loop until all packets have been received
     */
    do {

        /*
         *  Wait for Response
         */
        if ( rc = BrRead( BR_ENUM, &RemoteAddress, Buffer, MAX_BR_BUFFER, &BytesRead ) )
            goto badread;

        /*
         *  Get pointer to enumeration data
         */
        pEnum = (PICA_BR_ENUM) Buffer;

        /*
         *  Check for correct sequence
         */
        if ( pEnum->Sequence != Sequence ) {
            TRACE(( TC_PD, TT_ERROR, "Enumeration: bad sequence, expected %u, received %u",
                    Sequence, pEnum->Sequence ));
            /* flush read data until we get an error (timeout) */
            while ( !BrRead( BR_ENUM, &RemoteAddress, Buffer, MAX_BR_BUFFER, &BytesRead ) );
            goto tryagain;
        }

        TRACE(( TC_PD, TT_API1, "Enumeration: length %u, seq %u, flags 0x%x",
                BytesRead, pEnum->Sequence, pEnum->EnumFlags ));

        /*
         *  Append names to buffer
         */
        if ( pEnum->oFirstName ) {

            /*
             *  Get length of buffer not including the header
             */
            NamesLength = BytesRead - pEnum->oFirstName;

            /*
             *  Make sure names will fit in buffer
             *  -- if not grow buffer
             */
            if ( NameOffset + NamesLength > NameBufferLength ) {

                NewNameBufferLength = NameOffset + NamesLength;
                if ( (pNewNameBuffer = malloc( NewNameBufferLength+1 )) == NULL ) {
                    rc = CLIENT_ERROR_NO_MEMORY;
                    goto badalloc;
                }

                if ( pNameBuffer ) {
                    memcpy( pNewNameBuffer, pNameBuffer, NameBufferLength );
                    free( pNameBuffer );
                }

                TRACE(( TC_PD, TT_API1, "Enumeration: grow name buffer, old %u, new %u",
                        NameBufferLength, NewNameBufferLength ));

                pNameBuffer = pNewNameBuffer;
                NameBufferLength = NewNameBufferLength;
            }

            /*
             *  Copy names to name buffer
             */
            memcpy( &pNameBuffer[NameOffset], (char *)pEnum + pEnum->oFirstName, NamesLength );
            NameOffset += NamesLength;
        }

        /*
         *  Increment expected sequence
         */
        Sequence++;

    } while ( !(pEnum->EnumFlags & ENUM_FINAL) );

    /*
     * Handle the none found case
     */
    if ( pNameBuffer == NULL ) {
        if ( (pNameBuffer = malloc( 1+1 )) == NULL ) { // Just need to fit a double NULL
            rc = CLIENT_ERROR_NO_MEMORY;
            goto badalloc;
        }
        pNameBuffer[NameOffset++] = '\0';  // add string terminator
    }

    /*
     *  Add trailing double null
     */
    pNameBuffer[NameOffset++] = '\0';  // add double null

    /*
     *  Make sure names will fit in buffer
     */
    if ( (int) pNeEnum->ByteCount < NameOffset ) {
        pNeEnum->ByteCount = NameOffset;
        rc = CLIENT_ERROR_BUFFER_TOO_SMALL;
        goto badbuffer;
    }

    /*
     * If this was a request to enumrate Published Applications, only
     * copy the returned names if the master browser is WF 2.0 or above
     * (ICA_BR_ENUM packet version 2 or greater).  Otherwise, we will
     * ignore any enumeration data that was received (it won't be what
     * the caller wants to see); just return double-null terminated buffer.
     */
    if ( (pEnum->Header.Version < 2) &&
         ((pNeEnum->DataType == DATATYPE_ADDRESS) && 
          (pNeEnum->EnumReqFlags & ENUMREQ_ONLY_APPS)) ) {

        NameOffset = 0;
        pNameBuffer[NameOffset++] = '\0';  // double null
        pNameBuffer[NameOffset++] = '\0';
    }

    /*
     *  Copy names into buffer
     */
    pNeEnum->ByteCount = NameOffset;
    memcpy( pNeEnum->pName, pNameBuffer, NameOffset );

    /*
     *  Free name buffer
     */
    if ( pNameBuffer )
        free( pNameBuffer );

    return( BR_ERROR_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

badbuffer:
badread:
badwrite:
badalloc:
    if ( pNameBuffer )
        free( pNameBuffer );

badmaster:
    TRACE(( TC_PD, TT_ERROR, "_BrEnum failed, rc=%u", rc ));
    return( rc );
}





