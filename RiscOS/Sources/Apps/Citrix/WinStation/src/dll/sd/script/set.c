
/*************************************************************************
*
*   SET.C
*
*   Scripting Driver - set command[s]
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
*     Rev 1.0   13 Aug 1996 12:06:04   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:45:24   kurtp
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

COMMANDS SetXCommands[] = {
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_STRING,           NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


/*******************************************************************************
 *
 *  fnSetIgnoreCase
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
fnSetIgnoreCase( PSD pSd )
{
    int rc = SCRIPT_STATUS_SUCCESS;
    int token;

    /*
     *  Get last token
     */
    switch( (token = GetToken( pSd, SetXCommands )) ) {

        case TOKEN_WORD :
        case TOKEN_STRING :
            
            if ( !stricmp( (pSd->szCommand + pSd->offToken), "ON" ) ) {
                pSd->fProcessingCommand = FALSE;
                pSd->fCaseSensitive = FALSE;
                break;
            }
            else if ( !stricmp( (pSd->szCommand + pSd->offToken), "OFF" ) ) {
                pSd->fProcessingCommand = FALSE;
                pSd->fCaseSensitive = TRUE;
                break;
            }

        default :
            rc = SCRIPT_INVALID_SYNTAX;
            break;
    }

    return( rc );
}

#ifdef ASYNC_STUFF
/*******************************************************************************
 *
 *  fnSetFlow
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
fnSetFlow( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSetParity
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
fnSetParity( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSetSpeed
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
fnSetSpeed( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSetDataBits
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
fnSetDataBits( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSetStopBits
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
fnSetStopBits( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSetXonXoff
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
fnSetXonXoff( PSD pSd )
{

    return( SCRIPT_STATUS_SUCCESS );
}
#endif
