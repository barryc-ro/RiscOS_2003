
/*************************************************************************
*
*   INPUT.C
*
*   Scripting Driver - input state machine
*
*   Copyright 1990-1996, Citrix Systems Inc.
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 10-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:22   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:05:32   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:44:56   kurtp
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
==  External procedures defined
=============================================================================*/

void WFCAPI ReadHook( LPBYTE, USHORT );
int DevicePoll( PSD, PDLLPOLL );


/*=============================================================================
==  External procedures used
=============================================================================*/

int  _DeviceClose( PSD );
int  ParseCommand( PSD );


/*=============================================================================
==  Internal routines
=============================================================================*/

void _MatchString( PSD, LPBYTE, USHORT );


/*=============================================================================
==  Global Data
=============================================================================*/

//  bugbug: ideally this would be passed into the hook ...
extern SD SdData;


/*******************************************************************************
 *
 *  ReadHook
 *
 * ENTRY:
 *
 *  pData (input)
 *      Incomming data stream
 *  cbData (input)
 *      Incomming data stream byte count
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void WFCAPI
ReadHook( LPBYTE pData, USHORT cbData )
{
    PSD    pSd = &SdData;
    LPBYTE p = pData;

    /*
     *  Validate local data
     */
    if ( !pSd->fReadHooked ) {
        return;
    }

    /*
     *  Note the input time
     */
    pSd->ulInputTime = Getmsec();

    /*
     *  Not Case Sensitive
     */
    if ( !pSd->fCaseSensitive ) {
        if ( (p = (LPBYTE) malloc(cbData+1)) != NULL ) {
            memcpy( p, pData, cbData );
            *(pData+cbData) = '\0';
#ifdef DOS
            {
                USHORT i;
                BYTE   c;
                for ( i=0; i<cbData; i++ ) {
                    c = *(p+i);
                    if ( (c >= 'a') && (c <= 'z') ) {
                        *(p+i) = 'A' + (c - 'a');
                    }
                }
            }
#else
            AnsiUpper( p );
#endif
        }
    }

    /*
     *  Call string mathing routine
     */
    if ( pSd->Command == COMMAND_WAIT_STRING ) {
        _MatchString( pSd, p, cbData );
    }

    /*
     *  Free temp
     */
    if ( (p != NULL) && (p != pData) ) {
        free( p );
    }

    return;
}


/*******************************************************************************
 *
 *  DevicePoll
 *
 *  device polling
 *
 * ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *    pSdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DevicePoll( PSD pSd, PDLLPOLL pSdPoll )
{
    int rc;

    /*
     *  Are we done (an EXIT command was processed)
     */
    if ( pSd->fExit ) {

        /*
         *  Close file and remove hook
         */
        _DeviceClose( pSd );

        /*
         *  Return error to Wengine to stop it from polling
         */
        rc = SCRIPT_STATUS_COMPLETE;
    }

    /*
     *  Check if we need to process next command
     */
    else if ( !(pSd->fProcessingCommand) ) {

        /*
         *  Zap command buffer
         */
        memset( pSd->szCommand, 0, MAX_COMMAND );

        /*
         *  Read next line from file and parse
         */
        if ( (fgets( pSd->szCommand, MAX_COMMAND, pSd->hFile )) == NULL ) {

            /*
             *  Close file and remove hook
             */
            _DeviceClose( pSd );

            /*
             *  Return error to Wengine to stop it from polling
             */
            rc = SCRIPT_STATUS_COMPLETE;
        }
        else {
    
            /*
             *  NULL terminate line
             */
            if (pSd->szCommand[strlen(pSd->szCommand) - 1] == '\n') {
                pSd->szCommand[strlen(pSd->szCommand) - 1] = '\0';
            }

            /*
             *  Update line number
             */
            ++pSd->LineNumber;

            /* 
             *  If not case sensitive
             */
            if ( !pSd->fCaseSensitive ) {
               LPBYTE p = pSd->szCommand;
#ifdef DOS
               {
                   USHORT i;
                   BYTE   c;

                   for ( i=0; i<strlen(pSd->szCommand); i++ ) {
                       c = *(p+i);
                       if ( (c >= 'a') && (c <= 'z') ) {
                           *(p+i) = 'A' + (c - 'a');
                       }
                   }
               }
#else
               AnsiUpper( p );
#endif
            }

            /*
             *  Parse the command
             */
            rc = ParseCommand( pSd );
        }
    }

    /*
     *  Processing current command
     */
    else {

        switch ( pSd->Command ) {

        case COMMAND_SEND_STRING :   
            rc = fnSendString( pSd );
            break;

        case COMMAND_WAIT_RECEIVE :
            rc = fnWaitReceive( pSd );
            break;

        case COMMAND_WAIT_SILENCE :  
            rc = fnWaitSilence( pSd );
            break;

        case COMMAND_WAIT_STRING :
            rc = fnWaitString( pSd );
            break;

        case COMMAND_WAIT_TIME :
            rc = fnWaitTime( pSd );
            break;

        default :
            rc = SCRIPT_INTERNAL_ERROR;
            break;
        }
    }

    return( rc );
}


/*******************************************************************************
 *
 *  MatchString
 *
 * ENTRY:
 *  pSd (input)
 *      Pointer to sd data structure
 *  pData (input)
 *      Incomming data stream
 *  cbData (input)
 *      Incomming data stream byte count
 *
 * EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
_MatchString( PSD pSd, LPBYTE pData, USHORT cbData )
{
    int i;
    BYTE c0;
    BYTE c1;
    BYTE c2;

    /*
     *  Walk incomming string for match
     */
    for ( i=0; i<(int)cbData; i++ ) {

        /*
         *  Get current bytes
         */
        c0 = *(pData+i);
        c1 = *(pSd->szCommand+pSd->iCurrent);

        /*
         *  End of match string?
         */
        if ( c1 == '\0' ) {
            break;
        }

        /*
         *  Get next match byte
         */
        c2 = *(pSd->szCommand+pSd->iCurrent+1);

        /*
         *  Process wildcards
         */
        if ( c1 == '*' ) {
            if ( c2 == '*' ) {
                ++pSd->iCurrent;
                pSd->iMatch = pSd->iCurrent;
            }
            else if ( c0 == c2 ) {
                pSd->iCurrent += 2;
                pSd->iMatch = pSd->iCurrent;
            }
            continue;
        }
        else if ( c1 == '?' ) {
            if ( pSd->iCurrent == pSd->iMatch ) {
                ++pSd->iCurrent;
                pSd->iMatch = pSd->iCurrent;
            }
            else {
                ++pSd->iCurrent;
            }
            continue;
        }

        /*
         *  Process normal matches
         */
        if ( c0 == c1 ) {
            ++pSd->iCurrent;
        }
        else {
            pSd->iCurrent = pSd->iMatch;
        }
    }

    /*
     *  Total match, then continue to next command
     */
    if ( *(pSd->szCommand+pSd->iCurrent) == '\0' ) {
        pSd->fProcessingCommand = FALSE;
    }
}
