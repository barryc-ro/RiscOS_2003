
/*************************************************************************
*
*   SCRIPT.C
*
*   Scripting Driver - Source
*
*   Copyright 1990-1996, Citrix Systems Inc.
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 09-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.1   21 Mar 1997 16:07:44   bradp
*  update
*  
*     Rev 1.0   13 Aug 1996 12:05:42   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:45:04   kurtp
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/sdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/sd.h"

#include "script.h"
#include "scrpterr.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int DeviceOpen( PSD, PSDOPEN );
int DeviceClose( PSD, PDLLCLOSE );
int DeviceInfo( PSD, PDLLINFO );
int DeviceConnect( PSD );
int DeviceDisconnect( PSD );

void WFCAPI ReadHook( LPBYTE, USHORT );


/*=============================================================================
==   Internal Functions
=============================================================================*/

int _DeviceClose( PSD );


/*=============================================================================
=    Functions used
=============================================================================*/


/*=============================================================================
==   Local Data
=============================================================================*/

LPBYTE pSdProtocolName = "SCRIPT";

COMMANDS WaitStringCommands[] = {
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_STRING,           NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


/*******************************************************************************
 *
 *  DeviceOpen
 *
 *  open and initialize device
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *    pSdOpen (output)
 *       pointer to sd open parameter block
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/


int
DeviceOpen( PSD pSd, PSDOPEN pSdOpen )
{
    INT rc = SCRIPT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
 
    TRACE(( TC_MODEM, TT_API2, "SCRIPT: DeviceOpen"));

    /*
     *  Open script file
     */
    if ( (pSd->hFile = fopen( pSdOpen->pScriptFile, "r" )) == NULL ) {
        rc = SCRIPT_OPEN_FAILED;
        TRACE((TC_MODEM, TT_ERROR,
               "SCRIPT: DeviceOpen: fopen(%s) failed, rc=%d", pSdOpen->pScriptFile, rc));
        goto done;
    }

    /*
     *  Add read hook
     */
    memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
    WdSetInfo.WdInformationClass  = WdAddReadHook;
    WdSetInfo.pWdInformation      = (LPVOID)ReadHook;
    WdSetInfo.WdInformationLength = sizeof( LPVOID );
    if ( rc = ModuleCall( pSd->pWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
        rc = SCRIPT_READ_HOOK_FAILED;
        TRACE(( TC_MODEM, TT_ERROR,
                "SCRIPT: DeviceOpen: WdSetInfo(WdAddReadHook) failed, rc=%d", rc ));
        goto done;
    }
 
    /*
     *  Note read hook valid
     */
    pSd->fReadHooked = TRUE;

done:

    /*
     *  Close file on error
     */
    if ( (rc != SCRIPT_STATUS_SUCCESS) && (pSd->hFile != NULL) ) {
        (void) fclose( pSd->hFile );
    }

    TRACE(( TC_MODEM, TT_L1, "SCRIPT: DeviceOpen: rc(%d)", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  DeviceClose
 *
 *  close physical device
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *    pSdClose (output)
 *       pointer to sd close parameter block
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DeviceClose( PSD pSd, PDLLCLOSE pSdClose )
{

    TRACE(( TC_MODEM, TT_API2, "SCRIPT: DeviceClose"));

    return( _DeviceClose( pSd )  );
}


/*******************************************************************************
 *
 *  DeviceInfo
 *
 *    This routine is called to get module information
 *
 * ENTRY:
 *    pSd (input)
 *       pointer to script driver data structure
 *    pSdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DeviceInfo( PSD pPd, PDLLINFO pSdInfo )
{
    TRACE(( TC_MODEM, TT_API1, "SCRIPT: DeviceInfo" ));

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceConnect
 *
 *  Connect to Citrix Application Server
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *    pParams (input)
 *       pointer to sd parameters
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DeviceConnect( PSD pSd )
{

    TRACE(( TC_MODEM, TT_API1, "SCRIPT: DeviceConnect" ));

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceDisconnect
 *
 *  Disconnect from citrix application server
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DeviceDisconnect( PSD pSd )
{

    TRACE(( TC_MODEM, TT_API1, "SCRIPT: DeviceDisconnect" ));

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  _DeviceClose
 *
 *   close physical device (worker routine)
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
_DeviceClose( PSD pSd )
{
    INT rc = SCRIPT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;

    /*
     *  Check if read hooked
     */
    if ( pSd->fReadHooked ) {

        /*
         * Remove read hook
         */
        memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
        WdSetInfo.WdInformationClass  = WdRemoveReadHook;
        if ( rc = ModuleCall( pSd->pWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
            TRACE(( TC_MODEM, TT_ERROR,
                    "SCRIPT: _DeviceClose: WdSetInfo(WdRemoveReadHook) failed, rc=%d", rc ));
            rc = SCRIPT_STATUS_SUCCESS;
        }

        /*
         *  Mark read as not hooked always
         */
        pSd->fReadHooked = FALSE;
    }

    /*
     *  Close file if open
     */
    if ( (pSd->hFile != NULL) ) {
        (void) fclose( pSd->hFile );
        pSd->hFile = NULL;
    }

    return( rc );
}
