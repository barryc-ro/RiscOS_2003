
/*************************************************************************
*
* nrapi.c
*
* Name Resolver common code
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/3/94)
*
*  nrapi.c,v
*  Revision 1.1  1998/01/12 11:35:21  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.17   15 Apr 1997 16:19:36   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.17   21 Mar 1997 16:06:48   bradp
*  update
*  
*     Rev 1.16   08 Feb 1997 16:05:24   jeffm
*  multiple browser support
*  
*     Rev 1.15   16 May 1996 11:04:12   marcb
*  update
*  
*     Rev 1.14   08 May 1996 16:26:20   jeffm
*  update
*  
*     Rev 1.13   23 Apr 1996 09:03:44   richa
*  Added LibMain and DllMain to get the module instance handle.
*  
*     Rev 1.12   27 Mar 1996 11:57:06   KenB
*  LOADSTR.H should be included for everyone
*  
*     Rev 1.11   27 Mar 1996 10:04:50   richa
*  
*     Rev 1.10   20 Mar 1996 14:15:10   KenB
*  Use LoadString(), not ErrorMessages[]...
*  
*     Rev 1.9   20 Dec 1995 19:02:30   bradp
*  update
*  
*     Rev 1.8   05 Dec 1995 18:46:00   bradp
*  update
*  
*     Rev 1.7   10 Nov 1995 11:18:46   bradp
*  update
*  
*     Rev 1.6   02 May 1995 14:04:22   butchd
*  update
*  
*     Rev 1.5   17 Apr 1995 08:30:46   butchd
*  removed winlink.h, changed pLogProcedures type
*  
*     Rev 1.4   07 Apr 1995 11:49:36   richa
*  Windows client.
*
*     Rev 1.3   05 Apr 1995 15:50:52   butchd
*  global STATUS and ERROR code changes
*
*     Rev 1.2   16 Feb 1995 17:31:12   terryt
*  Spruce up name resolvers
*
*     Rev 1.1   11 Nov 1994 18:26:54   bradp
*  update
*
*     Rev 1.0   07 Nov 1994 19:19:38   bradp
*  Initial revision.
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
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#include "../../../inc/clib.h"
#endif
//#include "../../../inc/loadstr.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/logapi.h"
#include "../inc/nr.h"

#include "../../../inc/clib.h"

#include "../../../inc/nrdevice.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI NrLoad( PDLLLINK );
int WFCAPI NrUnload( PNR, PDLLLINK );
int WFCAPI NrOpen( PNR, PNROPEN );
int WFCAPI NrClose( PNR, PDLLCLOSE );
int WFCAPI NrInfo( PNR, PDLLINFO );
int WFCAPI NrPoll( PNR, PDLLPOLL );
int WFCAPI NrNameToAddress( PNR, PNAMEADDRESS );
int WFCAPI NrErrorLookup( PNR, PPDLASTERROR );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions used
=============================================================================*/

//STATIC int DeviceNameToAddress( PNR, PNAMEADDRESS );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define Name Resolver external procedures
 */
PDLLPROCEDURE NrProcedures[ NR__COUNT ] = {
    (PDLLPROCEDURE) NrLoad,
    (PDLLPROCEDURE) NrUnload,
    (PDLLPROCEDURE) NrOpen,
    (PDLLPROCEDURE) NrClose,
    (PDLLPROCEDURE) NrInfo,
    (PDLLPROCEDURE) NrPoll,
    (PDLLPROCEDURE) NrNameToAddress,
    (PDLLPROCEDURE) NrErrorLookup,
};

/*
 *  Define Name resolver data structure
 *   (this could be dynamically allocated)
 */
NR NrData = {0};


/*=============================================================================
==   Global Data
=============================================================================*/

//extern char * pNrProtocolName;
//STATIC PPLIBPROCEDURE pClibProcedures = NULL;
//STATIC PPLIBPROCEDURE pLogProcedures = NULL;

BYTE   G_TcpBrowserAddress[ ADDRESS_LENGTH+1 ];
BYTE   G_IpxBrowserAddress[ ADDRESS_LENGTH+1 ];
BYTE   G_NetBiosBrowserAddress[ ADDRESS_LENGTH+1 ];
USHORT G_RequestRetry;
USHORT G_ReadTimeout;
ADDRESS G_TcpBrowserAddrList[MAX_BROWSERADDRESSLIST];
ADDRESS G_IpxBrowserAddrList[MAX_BROWSERADDRESSLIST];
ADDRESS G_NetBiosBrowserAddrList[MAX_BROWSERADDRESSLIST];

/*
 *  Setup in the DLL init routines and used by LoadString()
 */
//STATIC HINSTANCE G_hModule = 0;


/*******************************************************************************
 *
 *  NrLoad
 *
 *    The user interface calls NrLoad to load and link a name
 *    resolver.
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrLoad( PDLLLINK pLink )
{
    /*
     *  Initialize DllLink structure
     */
    pLink->ProcCount   = NR__COUNT;
    pLink->pProcedures = NrProcedures;
    pLink->pData       = &NrData;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrUnload
 *
 *  DllUnload calls NrUnload to unlink and unload a name resolver
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name resolver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrUnload( PNR pNr, PDLLLINK pLink )
{
    pLink->ProcCount = 0;
    pLink->pProcedures = NULL;
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrOpen
 *
 *  The user interface calls NrOpen to open and initialize a Nr.
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name driver data structure
 *    pNrOpen (input/output)
 *       pointer to the structure NROPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrOpen( PNR pNr, PNROPEN pNrOpen )
{
    /*
     *  Initialize NR function call tables: MUST BE FIRST!
     */
//    pLogProcedures = pNrOpen->pLogProcedures;
//    pClibProcedures = pNrOpen->pClibProcedures;
    
    /*
     *  Initialize browser addresses
     */
    memcpy( G_TcpBrowserAddress, pNrOpen->pTcpBrowserAddress, sizeof(ADDRESS) );
    memcpy( G_IpxBrowserAddress, pNrOpen->pIpxBrowserAddress, sizeof(ADDRESS) );
    memcpy( G_NetBiosBrowserAddress, pNrOpen->pNetBiosBrowserAddress, sizeof(ADDRESS) );

    if(pNrOpen->pTcpBrowserAddrList != NULL)
        memcpy( G_TcpBrowserAddrList, pNrOpen->pTcpBrowserAddrList, sizeof(G_TcpBrowserAddrList));
    if(pNrOpen->pIpxBrowserAddrList)
        memcpy( G_IpxBrowserAddrList, pNrOpen->pIpxBrowserAddrList, sizeof(G_IpxBrowserAddrList));
    if(pNrOpen->pNetBiosBrowserAddrList)
        memcpy( G_NetBiosBrowserAddrList, pNrOpen->pNetBiosBrowserAddrList, sizeof(G_NetBiosBrowserAddrList));

    G_RequestRetry = pNrOpen->BrowserRetry;
    G_ReadTimeout  = pNrOpen->BrowserTimeout;

    /*
     *  Initialize NR data structure
     */
    memset( pNr, 0, sizeof(NR) );

    pNr->pDeviceProcedures = pNrOpen->pDeviceProcedures;

    TRACE((TC_TD, TT_API1, "NrOpen: DeviceProcedures %p", pNr->pDeviceProcedures));

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrClose
 *
 *  The user interface calls NrClose to close a Nr before unloading it.
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name driver data structure
 *    pNrClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrClose( PNR pNr, PDLLCLOSE pNrClose )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrInfo
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name driver data structure
 *    pNrInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrInfo( PNR pNr, PDLLINFO pNrInfo )
{
    USHORT ByteCount;
    PMODULE_C2H pHeader;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(MODULE_C2H);

    /*
     *  Check if buffer is big enough
     */
    if ( pNrInfo->ByteCount < ByteCount ) {
        pNrInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize module header
     */
    pNrInfo->ByteCount = ByteCount;
    pHeader = (PMODULE_C2H) pNrInfo->pBuffer;
    memset( pHeader, 0, ByteCount );
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_NameResolver;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrPoll
 *
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name driver data structure
 *    pNrPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrPoll( PNR pNr, PDLLPOLL pNrPoll )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NrNameToAddress
 *
 *   NrNameToAddress converts a application server name to an address that
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

int WFCAPI
NrNameToAddress( PNR pNr, PNAMEADDRESS pNameAddress )
{
    TRACE(( TC_TD, TT_API1, "NrNameToAddress: DeviceProcedures %p [0] = %p", pNr->pDeviceProcedures, pNr->pDeviceProcedures[0] ));
    return( DeviceNameToAddress( pNr, pNameAddress ) );
}

/*******************************************************************************
 *
 *  NrErrorLookup
 *
 *   NrErrorLookup returns the message string corresponding to the error
 *   code.
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name resolver data structure
 *    pLastError (input/output)
 *       pointer to the structure PDLASTERROR
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NrErrorLookup( PNR pNr, PPDLASTERROR pErrorLookup )
{
    TRACE(( TC_TD, TT_API1, "NrErrorLookup: ProtocolName '%s' rc %d", pNr->pProtocolName, pErrorLookup->Error ));

    memset( pErrorLookup->Message, 0, sizeof(pErrorLookup->Message) );
    strcpy( pErrorLookup->ProtocolName, pNr->pProtocolName );

    if ( !LoadString( pErrorLookup->ProtocolName, //"NET",
                      pErrorLookup->Error == 0xFFFF ? 0 : pErrorLookup->Error,
                      pErrorLookup->Message,
                      sizeof(pErrorLookup->Message) ) ) {
       /*
        * We didn't map an error message, use the default error code.
        */
       LoadString( NULL, 
                   (UINT)(ERROR_DEFAULT), 
                   pErrorLookup->Message,
                   sizeof(pErrorLookup->Message) );
          
    }
    
    return( CLIENT_STATUS_SUCCESS );
}

#ifdef WIN16
/*******************************************************************************
 * function: LibMain
 *
 * purpose:  LibMain is called by Windows (WIN16) to initialize a DLL.
 *
 * entry: (see LibMain definition in Windows 3.1 SDK)
 *
 * exit: (see LibMain definition in Windows 3.1 SDK)
 *
 ******************************************************************************/

int CALLBACK
LibMain( HINSTANCE hInst,
         WORD wDataSeg,
         WORD cbHeap,
         LPSTR lpszCmdLine )
{

    G_hModule = hInst;
    return( TRUE );
}

//
///*******************************************************************************
// * function: WEP
// *
// * purpose:  WEP is called by Windows (WIN16) to deinitialize a DLL.
// *
// * entry:
// *   nExitType (input)
// *      WEP_FREE_DLL -     This app is exiting
// *      WEP_SYSTEM_EXIT -  The system is exiting
// *
// * exit:
// *   TRUE if successful / FALSE if unsuccessful
// *
// *
// ******************************************************************************/
//int CALLBACK
//WEP( int nExitType )
//{
//
//    //  For future if needed
//     
//    return( TRUE );
//}
//
#endif
#ifdef   WIN32

/****************************************************************************
*   FUNCTION: DllMain(HANDLE, DWORD, LPVOID)
*
*  PURPOSE:  DllMain is called by Windows when
*            the DLL is initialized, Thread Attached, and other times.
*
*
*
******************************************************************************/

BOOL WINAPI
DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    switch ( dwReason ) {
        case DLL_PROCESS_ATTACH:
            G_hModule = hDLL;
            break;

        case DLL_PROCESS_DETACH:
            // For future
            break;

    }
    return( TRUE );
}
#endif
