
/*************************************************************************
*
*   sdapi.c
*
*   Script Driver Common Code
*
*   Copyright Citrix Systems Inc. 1990-1996
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 09-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:10   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.1   21 Mar 1997 16:07:42   bradp
*  update
*  
*     Rev 1.0   13 Aug 1996 12:02:28   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:44:28   kurtp
*  Initial revision.
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

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/sdapi.h"
#include "../../../inc/logapi.h"

#include "../inc/sd.h"
#include "../script/scrpterr.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI SdLoad( PDLLLINK );
int WFCAPI SdUnload( PSD, PDLLLINK );
int WFCAPI SdOpen( PSD, PSDOPEN );
int WFCAPI SdClose( PSD, PDLLCLOSE );
int WFCAPI SdInfo( PSD, PDLLINFO );
int WFCAPI SdPoll( PSD, PDLLPOLL );
int WFCAPI SdErrorLookup( PSD, PPDLASTERROR );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions used
=============================================================================*/

int DeviceOpen( PSD, PSDOPEN );
int DeviceClose( PSD, PDLLCLOSE );
int DevicePoll( PSD, PDLLPOLL );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define Script Driver external procedures
 */
PDLLPROCEDURE SdProcedures[ SD__COUNT ] = {
    (PDLLPROCEDURE) SdLoad,
    (PDLLPROCEDURE) SdUnload,
    (PDLLPROCEDURE) SdOpen,
    (PDLLPROCEDURE) SdClose,
    (PDLLPROCEDURE) SdInfo,
    (PDLLPROCEDURE) SdPoll,
    (PDLLPROCEDURE) SdErrorLookup,
};

/*
 *  Define Script Driver data structure
 *   (this could be dynamically allocated)
 */
SD SdData = {0};


/*=============================================================================
==   Global Data
=============================================================================*/

#if 0
PPLIBPROCEDURE pClibProcedures = NULL;
PPLIBPROCEDURE pLogProcedures = NULL;
PPLIBPROCEDURE pModuleProcedures = NULL;
#endif
/*
 *  Setup in the DLL init routines and used by LoadString()
 */
//HINSTANCE G_hModule = 0;

extern LPBYTE pSdProtocolName;

/*******************************************************************************
 *
 *  Load
 *
 *    The user interface calls SdLoad to load and link a name
 *    resolver.
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdLoad( PDLLLINK pLink )
{
//   PSD pSd = malloc(sizeof(*pSd));
    PSD pSd = &SdData;
    /*
     *  Initialize DllLink structure
     */
    pLink->ProcCount   = SD__COUNT;
    pLink->pProcedures = SdProcedures;
    pLink->pData       = pSd;

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  SdUnload
 *
 *  DllUnload calls SdUnload to unlink and unload a script driver
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to script driver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdUnload( PSD pSd, PDLLLINK pLink )
{
//  free(pSd);

    pLink->ProcCount = 0;
    pLink->pProcedures = NULL;
    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  SdOpen
 *
 *  The user interface calls SdOpen to open and initialize a Sd.
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to name driver data structure
 *    pSdOpen (input/output)
 *       pointer to the structure SDOPEN
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdOpen( PSD pSd, PSDOPEN pSdOpen )
{
#if 0
    /*
     *  Initialize SD function call tables: MUST BE FIRST!
     */
    pLogProcedures = pSdOpen->pLogProcedures;
    pClibProcedures = pSdOpen->pClibProcedures;
    pModuleProcedures = pSdOpen->pModuleProcedures;
#endif
    /*
     *  Initialize SD data structure
     */
    memset( pSd, 0, sizeof(SD) );

    /*
     *  Save wd link
     */
    pSd->pWdLink = pSdOpen->pWdLink;

    /*
     *  Call script specific open
     */
    return( DeviceOpen( pSd, pSdOpen ) );
}


/*******************************************************************************
 *
 *  SdClose
 *
 *  The user interface calls SdClose to close a Sd before unloading it.
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to name driver data structure
 *    pSdClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdClose( PSD pSd, PDLLCLOSE pSdClose )
{

    return( DeviceClose( pSd, pSdClose ) );
}


/*******************************************************************************
 *
 *  SdInfo
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to name driver data structure
 *    pSdInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdInfo( PSD pSd, PDLLINFO pSdInfo )
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
    if ( pSdInfo->ByteCount < ByteCount ) {
        pSdInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize module header
     */
    pSdInfo->ByteCount = ByteCount;
    pHeader = (PMODULE_C2H) pSdInfo->pBuffer;
    memset( pHeader, 0, ByteCount );
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_Scripting;

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  SdPoll
 *
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to name driver data structure
 *    pSdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdPoll( PSD pSd, PDLLPOLL pSdPoll )
{

    return( DevicePoll( pSd, pSdPoll ) );
}


/*******************************************************************************
 *
 *  SdErrorLookup
 *
 *   SdErrorLookup returns the message string corresponding to the error
 *   code.
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to script driver data structure
 *    pLastError (input/output)
 *       pointer to the structure PDLASTERROR
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
SdErrorLookup( PSD pSd, PPDLASTERROR pErrorLookup )
{                       
    LPBYTE p;

    memset( pErrorLookup->Message, 0, sizeof(pErrorLookup->Message) );
    strcpy( pErrorLookup->ProtocolName, pSdProtocolName );

    if ( !LoadString( "SD",
                      pErrorLookup->Error,
                      pErrorLookup->Message,
                      sizeof(pErrorLookup->Message) ) ) {
       /*
        * We didn't map an error message, use the default error code.
        */
       LoadString( "SD", 
                   (UINT)(ERROR_DEFAULT), 
                   pErrorLookup->Message,
                   sizeof(pErrorLookup->Message) );
          
    }
    else if ( pErrorLookup->Error == SCRIPT_INVALID_SYNTAX )  {
        if ( (p = (LPBYTE) malloc(strlen(pErrorLookup->Message)+1)) != NULL ) {
            strcpy( p, pErrorLookup->Message );
#ifdef DOS
            sprintf(pErrorLookup->Message, p, pSd->LineNumber);
#else
            wsprintf(pErrorLookup->Message, p, pSd->LineNumber);
#endif
            free(p);
        }
    }

    return( SCRIPT_STATUS_SUCCESS );
}
