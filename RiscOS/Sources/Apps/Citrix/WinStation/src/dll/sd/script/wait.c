
/*************************************************************************
*
*   WAIT.C
*
*   Scripting Driver - wait command[s]
*
*   Copyright 1990-1996, Citrix Systems Inc.
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 13-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:32   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:06:06   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:45:28   kurtp
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


/*=============================================================================
==   External procedures used
=============================================================================*/

int  GetToken( PSD, PCOMMANDS );


/*=============================================================================
==   Local data
=============================================================================*/

COMMANDS WaitXCommands[] = {
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_STRING,           NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


/*******************************************************************************
 *
 *  fnWaitReceive
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
fnWaitReceive( PSD pSd )
{
    int token;
    int rc = SCRIPT_STATUS_SUCCESS;

    /*
     *  Command currently being processed?
     */
    if ( pSd->fProcessingCommand ) {

        /*
         *  Check for command timeout
         */
        if ( (Getmsec() >= pSd->ulCommandTimeout) || (pSd->ulInputTime != 0) ) {
            pSd->fProcessingCommand = FALSE;
        }
    }
    else {

        /*
         *  Get last token
         */
        switch( (token = GetToken( pSd, WaitXCommands )) ) {
    
            case TOKEN_WORD :
            case TOKEN_STRING :
                
                /*
                 *  Mark WAIT RECEIVE command being processed
                 */
                pSd->Command = COMMAND_WAIT_RECEIVE;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Set timeout
                 */
                pSd->ulInputTime = 0L;
                pSd->ulCommandTimeout = Getmsec() + 
                     (atol(pSd->szCommand + pSd->offToken) * 1000L);

                break;
    
            case TOKEN_EOL :

                /*
                 *  Mark WAIT RECEIVE command being processed
                 */
                pSd->Command = COMMAND_WAIT_RECEIVE;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Set timeout
                 */
                pSd->ulInputTime = 0L;
                pSd->ulCommandTimeout = (ULONG) -1L;

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
 *  fnWaitSilence
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
fnWaitSilence( PSD pSd )
{
    int token;
    int rc = SCRIPT_STATUS_SUCCESS;

    /*
     *  Command currently being processed?
     */
    if ( pSd->fProcessingCommand ) {

        /*
         *  Check for command timeout
         */
        if ( Getmsec() >= (pSd->ulInputTime + pSd->ulCommandTimeout) ) {
            pSd->fProcessingCommand = FALSE;
        }
    }
    else {

        /*
         *  Get last token
         */
        switch( (token = GetToken( pSd, WaitXCommands )) ) {
    
            case TOKEN_WORD :
            case TOKEN_STRING :
                
                /*
                 *  Mark WAIT SILENCE command being processed
                 */
                pSd->Command = COMMAND_WAIT_SILENCE;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Set timeouts
                 */
                pSd->ulInputTime = Getmsec();
                pSd->ulCommandTimeout = atol(pSd->szCommand + pSd->offToken) * 1000L;

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
 *  fnWaitString
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
fnWaitString( PSD pSd )
{
    int token;
    int rc = SCRIPT_STATUS_SUCCESS;

    /*
     *  Command currently being processed?
     */
    if ( pSd->fProcessingCommand ) {

        /*
         *  Check for command timeout
         */
        if ( Getmsec() >= pSd->ulCommandTimeout ) {
            pSd->fProcessingCommand = FALSE;
        }
    }
    else {

        /*
         *  Get string
         */
        switch( (token = GetToken( pSd, WaitXCommands )) ) {
    
            case TOKEN_WORD :
            case TOKEN_STRING :
                
                /*
                 *  Mark WAIT STRING command being processed
                 */
                pSd->Command = COMMAND_WAIT_STRING;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Save pointer to start of string to match
                 */
                pSd->iMatch = pSd->iCurrent = pSd->offToken;

                /*
                 *  Get last timeout
                 */
                switch( (token = GetToken( pSd, WaitXCommands )) ) {
            
                    case TOKEN_WORD :
                    case TOKEN_STRING :
                        
                        /*
                         *  Set timeout
                         */
                        pSd->ulCommandTimeout = Getmsec() + 
                             (atol(pSd->szCommand + pSd->offToken) * 1000L);
        
                        break;
            
                    default :
        
                        /*
                         *  Set timeout
                         */
                        pSd->ulCommandTimeout = (ULONG) -1L;
        
                        break;
                }
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
 *  fnWaitTime
 *
 *  ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *
 *  EXIT:
 *    SCRIPT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int  fnWaitTime( PSD pSd )
{
    int token;
    int rc = SCRIPT_STATUS_SUCCESS;

    /*
     *  Command currently being processed?
     */
    if ( pSd->fProcessingCommand ) {

        /*
         *  Check for command timeout
         */
        if ( Getmsec() >= pSd->ulCommandTimeout ) {
            pSd->fProcessingCommand = FALSE;
        }
    }
    else {

        /*
         *  Get last token
         */
        switch( (token = GetToken( pSd, WaitXCommands )) ) {
    
            case TOKEN_WORD :
            case TOKEN_STRING :
                
                /*
                 *  Mark WAIT TIME command being processed
                 */
                pSd->Command = COMMAND_WAIT_TIME;
                pSd->fProcessingCommand = TRUE;
    
                /*
                 *  Set timeout
                 */
                pSd->ulCommandTimeout = Getmsec() + 
                     (atol(pSd->szCommand + pSd->offToken) * 1000L);

                break;
    
            default :
                rc = SCRIPT_INVALID_SYNTAX;
                break;
        }
    }

    return( rc );
}
