
/*************************************************************************
*
*   SEND.C
*
*   Scripting Driver - send command[s]
*
*   Copyright 1990-1996, Citrix Systems Inc.
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 13-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:06:00   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:45:22   kurtp
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

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/sdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/sd.h"

#include "script.h"
#include "scrpterr.h"


/*=============================================================================
==   Local structures
=============================================================================*/


/*=============================================================================
==   External procedures defined
=============================================================================*/


/*=============================================================================
==   Local procedures defined
=============================================================================*/

int  _SendString( PSD );


/*=============================================================================
==   External procedures used
=============================================================================*/

int  GetToken( PSD, PCOMMANDS );


/*=============================================================================
==   Local data
=============================================================================*/

COMMANDS SendStringCommands[] = {
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_STRING,           NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


/*******************************************************************************
 *
 *  fnSendString
 *
 *  ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *
 *  EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
fnSendString( PSD pSd )
{
    int rc = SCRIPT_STATUS_SUCCESS;
    int token;

    /*
     *  Command currently being processed?
     */
    if ( pSd->fProcessingCommand ) {

        /*
         *  Check for command timeout
         */
        if ( Getmsec() >= pSd->ulCommandTimeout ) {
            (void) _SendString( pSd );
        }
    }
    else {

        /*
         *  Get last token
         */
        switch( (token = GetToken( pSd, SendStringCommands )) ) {
    
            case TOKEN_WORD :
            case TOKEN_STRING :
                
                /*
                 *  Mark SEND STRING command being processed
                 */
                pSd->Command = COMMAND_SEND_STRING;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Start sending the string
                 */
                (void) _SendString( pSd );
                break;
    
            default :
                rc = SCRIPT_INVALID_SYNTAX;
                break;
        }
    }

    return( rc );
}


/*******************************************************************************
 *
 *  _SendString
 *
 *  ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *
 *  EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int  
_SendString( PSD pSd )
{
    INT rc = SCRIPT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    CHARCODE cc;
    LPBYTE p;
    LPBYTE pIn = (pSd->szCommand + pSd->offToken);
    LPBYTE pOut = NULL;
 
    /*
     *  Reset timeout
     */
    pSd->ulCommandTimeout = 0L;

    /*
     *  Done with this command
     */
    if ( *pIn == '\0' ) {
        pSd->fProcessingCommand = FALSE;
        goto done;
    }


    /*
     *  Allocate buffer
     */
    if ( (pOut = (LPBYTE) malloc( strlen(pIn) )) == NULL ) {
        pSd->fProcessingCommand = FALSE;
        rc = SCRIPT_ERROR_NO_MEMORY;
        goto done;
    }
    memset( (p = pOut), '\0', strlen(pIn) );
 
    /*
     * Format string
     */
    while ( *pIn != '\0' ) {

        if ( *pIn == '^' ) {
            ++pIn;
            ++pSd->offToken;
            if ( (*pIn >= 'a') && (*pIn <= 'z') ) {
                *p = *(pIn++) - 'a' + 1;
                ++pSd->offToken;
            }
            else if ( (*pIn >= 'A') && (*pIn <= 'Z') ) {
                *p = *(pIn++) - 'A' + 1;
                ++pSd->offToken;
            }
            else {
                *p = *(pIn++);
                ++pSd->offToken;
            }
        }
        else if ( (*pIn == '~') ) {
            ++pIn;
            ++pSd->offToken;
            pSd->ulCommandTimeout = Getmsec() + 500L;
            break;
        }
        else {
            *p = *(pIn++);
            ++pSd->offToken;
        }

        /*
         *  Send command now?
         */
        if ( *p == 0x0d ) {
            pSd->ulCommandTimeout = Getmsec();
            break;
        }

        /*
         *  Next out byte
         */
        ++p;
    }

    /*
     * Send keyboard event
     */
    if ( strlen(pOut) ) {
        cc.pCharCodes = (LPVOID) pOut;
        cc.cCharCodes = strlen(pOut);
        memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
        WdSetInfo.WdInformationClass  = WdCharCode;
        WdSetInfo.pWdInformation      = (LPVOID)&cc;
        WdSetInfo.WdInformationLength = sizeof( cc );
        if ( rc = ModuleCall( pSd->pWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
            pSd->fProcessingCommand = FALSE;
            TRACE(( TC_MODEM, TT_ERROR,
               "SCRIPT: _SendString: Error (%d) from WdSetInfo-WdCharCode", rc ));
            goto done;
        }
    }
 
done:

    /*
     * Free local junk
     */
    if ( pOut ) {
        free( pOut );
    }

    TRACE(( TC_MODEM, TT_L1, "SCRIPT: _SendString: rc(%d)", rc ));
    return( rc );
}
