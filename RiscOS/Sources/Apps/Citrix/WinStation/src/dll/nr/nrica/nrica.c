
/*************************************************************************
*
*   NRICA.C
*
*   ICA Name Resolution
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Brad Pedersen (11/7/95)
*
*   nrica.c,v
*   Revision 1.1  1998/01/12 11:35:29  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.14   28 Apr 1997 19:25:10   thanhl
*  update
*  
*     Rev 1.13   15 Apr 1997 16:19:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   19 Mar 1997 14:11:52   richa
*  Send ClientName to the TD so that we'll be able to reconnect to disconnected session in a cluster.
*  
*     Rev 1.12   06 Mar 1997 18:02:50   richa
*  Fixed CPR 4535.
*  
*     Rev 1.11   08 May 1996 16:28:02   jeffm
*  update
*  
*     Rev 1.10   20 Mar 1996 12:17:56   KenB
*  moved error messages to nls\us\nricat.str
*  
*     Rev 1.9   09 Feb 1996 13:42:02   butchd
*  tweaked error messages
*  
*     Rev 1.8   09 Jan 1996 16:37:00   bradp
*  update
*  
*     Rev 1.7   03 Jan 1996 18:17:34   bradp
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
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../../../inc/nrapi.h"
#include "../inc/nr.h"

#define NO_NRDEVICE_DEFINES
#include "../../../inc/nrdevice.h"
#include "../../../inc/nrdevicep.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int DeviceNameToAddress( PNR, PNAMEADDRESS );

PLIBPROCEDURE NrICADeviceProcedures[NRDEVICE__COUNT] =
{
    (PLIBPROCEDURE)DeviceNameToAddress
};

/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

int _GetBrowserAddress( PNR, char *, char *, PICA_BR_ADDRESS );
int _GetBrowserData( PNR, char *, int, void *, int, void *, int * );


/*=============================================================================
==   Functions used
=============================================================================*/

int IoOpen( void );
void IoClose( void );
int IoAddressCheck( char *, PICA_BR_ADDRESS, int );
int IoSameAddress( PICA_BR_ADDRESS, PICA_BR_ADDRESS );
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
extern ICA_BR_ADDRESS  G_BrowserAddress;
extern int             G_fBrowserAddress;
extern PICA_BR_ADDRESS G_pLocalAddr;
extern int             G_LocalAddrCount;
extern int             G_LanaNumber;
extern USHORT          G_RequestRetry;
extern USHORT          G_ReadTimeout;


/*=============================================================================
==   Local Data
=============================================================================*/

static char  *pNrProtocolName = "Browser";






/*******************************************************************************
 *
 *  DeviceNameToAddress
 *
 *   DeviceNameToAddress converts an application server name to an address that
 *   can be used by the transport drvier
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name resolver data structure
 *    pNameAddress (input/output)
 *       pointer to the structure NAMEADDRESS
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceNameToAddress( PNR pNr, PNAMEADDRESS pNameAddress )
{
    ICA_BR_ADDRESS Address;
    USHORT AddressFamily;
    int rc;

    memset( pNameAddress->Address, 0, sizeof(pNameAddress->Address) );
    G_LanaNumber = pNameAddress->LanaNumber;

    pNr->pProtocolName = pNrProtocolName;
    
    /*
     *  Open I/O
     */
    if ( rc = IoOpen() ) 
        return( rc );
    
    /*
     *  Resolve name
     */
    rc =_GetBrowserAddress( pNr, pNameAddress->Name, pNameAddress->ClientName,
                            &Address );
    if ( rc == BR_ERROR_SUCCESS ) {
        memcpy( &AddressFamily, &Address.Address[0], 2 );
        switch ( AddressFamily ) {
            case AF_INET :
                memcpy( pNameAddress->Address, &Address.Address[4], 4 );
                break;
            case AF_IPX :
                memcpy( pNameAddress->Address, &Address.Address[2], MAX_BR_ADDRESS-2 );
                break;
            case AF_NETBIOS :
                memcpy( pNameAddress->Address, &Address.Address[4], MAX_BR_ADDRESS-4 );
                break;
        }
    }

    /*
     *  Close I/O
     */
    IoClose();
    
    pNameAddress->LanaNumber = G_LanaNumber;

    return( rc );
}


/*******************************************************************************
 *
 *  _GetBrowserAddress
 *
 *  get address and state of browser
 *
 *
 *  ENTRY:
 *     pNr (input)
 *         pointer to name resolver data structure
 *     pName (input)
 *         pointer to browser name
 *     pClientName (input)
 *         pointer to client name
 *     pAddress (output)
 *         address to return address of browser
 *
 *  EXIT:
 *     BR_ERROR_SUCCESS - no error
 *
 ******************************************************************************/

int
_GetBrowserAddress( PNR pNr,
                    char * pName,
                    char * pClientName,
                    PICA_BR_ADDRESS pAddress )
{
    ICA_BR_DATA_ADDRESS DataAddress;
    PICA_BR_ADDRESS_PARAMS pParams = NULL;
    int ParamsLength = 0;
    int ClientNameLength;
    int DataLength;
    int Offset;
    int i;
    int rc;

    /*
     *  Make sure a name was specified
     */
    if ( pName[0] == '\0' ) {
        rc = BR_ERROR_INVALID_PARAMETER;
        goto baddata1;
    }

    /* 
     *  Make sure name is uppercase
     */
    AnsiUpper( pName );

    /*
     *  Check for numeric address  ( e.g. cc:43434dfd324, 128.1.4.3 )
     */
    if ( IoAddressCheck( pName, pAddress, TRUE ) ) {
        return( BR_ERROR_SUCCESS );
    }

    /*
     *  Initialize address request parameters
     */
    if ( pClientName ) {

        ClientNameLength = strlen(pClientName) + 1;

        ParamsLength = sizeof(ICA_BR_ADDRESS_PARAMS) + ClientNameLength;
        if ( (pParams = malloc( ParamsLength )) == NULL )
	{
	    rc = BR_ERROR_NOT_ENOUGH_MEMORY;
            goto baddata1;
	}
        memset( pParams, 0, ParamsLength );

        Offset = sizeof(ICA_BR_ADDRESS_PARAMS);
        pParams->Version = 1;

        pParams->oClientName = Offset;
        memcpy( (char*)pParams + Offset, pClientName, ClientNameLength );
    }

    /*
     *  Get address data from master browser
     */
    for ( i=0; i < (int)G_RequestRetry; i++ ) {
        rc = _GetBrowserData( pNr, pName, DATATYPE_ADDRESS, pParams, ParamsLength, &DataAddress, &DataLength );
        if ( rc != BR_ERROR_READ_TIMEOUT )
            break;
    }

    /* 
     *  Check for browser error
     *  -- check if name is already an address
     *  -- check dns or bindery
     */
    if ( rc ) {
        if ( !IoAddressCheck( pName, pAddress, FALSE ) )
            goto baddata1;

        return( BR_ERROR_SUCCESS );
    }

    /*
     *  Return address
     */
    *pAddress = DataAddress.Address;

    return( BR_ERROR_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

baddata:
    rc = BR_ERROR_NO_MASTER;
baddata1:
    TRACE(( TC_PD, TT_ERROR, "_GetBrowserAddress %s failed, rc=%u", pName, rc ));
    return( rc );
}




/*******************************************************************************
 *
 *  _GetBrowserData
 *
 *  get browser data
 *
 *  NOTE: either the browser name or address must be specified
 *
 *  ENTRY:
 *     pNr (input)
 *         pointer to name resolver data structure
 *     pName (input)
 *         pointer to browser name (optional)
 *     DataType (input)
 *         browser data type to query
 *     pParams (input)
 *         points to data request parameters
 *     ParamsLength (input)
 *         length of data request parameters
 *     pData (output)
 *         address to return browser data
 *     pDataLength (output)
 *         address to return length of data
 *
 *  EXIT:
 *     BR_ERROR_SUCCESS - no error
 *
 ******************************************************************************/

int
_GetBrowserData( PNR pNr,
                 char * pName,
                 int DataType,
                 void * pParams,
                 int ParamsLength,
                 void * pData,
                 int * pDataLength )
{
    BYTE Buffer[ MAX_BR_BUFFER ];
    ICA_BR_ADDRESS Address;
    ICA_BR_ADDRESS RemoteAddress;
    PICA_BR_REQUEST_DATA pRequest;
    PICA_BR_DATA pBrowserData;
    int BytesRead;
    int NameLength;
    int Length;
    int Offset;
    int rc;

    /*
     *  Get address of master browser
     */
    if ( rc = BrRequestMasterBrowser( &Address ) )
        goto badmaster;

    /*
     *  Get length of name (including null)
     */
    NameLength = strlen(pName) + 1;
    Length = sizeof_ICA_BR_REQUEST_DATA + NameLength + ParamsLength;

    /*
     *  Send request for browser data
     */
    pRequest = (PICA_BR_REQUEST_DATA) Buffer;
    memset( pRequest, 0, Length );
    pRequest->Header.ByteCount = Length;
    pRequest->Header.Version   = 2;
    pRequest->Header.Command   = BR_REQUEST_DATA;
    pRequest->Header.Signature = BR_SIGNATURE;
    pRequest->DataType         = DataType;
    Offset = sizeof_ICA_BR_REQUEST_DATA;
    // Set up pName
    pRequest->oBrowserName = Offset;
    strcpy( Buffer + Offset, pName );

    if ( pParams && ParamsLength ) {
        Offset += (strlen(pName) + 1);
        pRequest->oParams = Offset;
        memcpy( Buffer + Offset, pParams, ParamsLength );
    }

    BrPurgeInput();
    if ( rc = BrWrite( &Address, pRequest, pRequest->Header.ByteCount ) )
        goto badwrite;

    /*
     *  Wait for Response
     */
    if ( rc = BrRead( BR_DATA, &RemoteAddress, Buffer, MAX_BR_BUFFER, &BytesRead ) )
        goto badread;

    /*
     *  Return browser data to caller
     */
    pBrowserData = (PICA_BR_DATA) Buffer;
    memcpy( pData, Buffer + pBrowserData->oData, pBrowserData->DataLength );
    *pDataLength = pBrowserData->DataLength;

    TRACE(( TC_PD, TT_API1, "_GetBrowserData: %s, %u, length %u",
            pName, pBrowserData->DataType, pBrowserData->DataLength ));

    return( BR_ERROR_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

badread:
badwrite:
badmaster:
    TRACE(( TC_PD, TT_ERROR, "_GetBrowserData %s failed, rc=%u", pName, rc ));
    return( rc );
}


