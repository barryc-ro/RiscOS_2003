
/*************************************************************************
*
*   PARSE.C
*
*   Scripting Driver - command parser
*
*   Copyright 1990-1996, Citrix Systems Inc.
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 13-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:05:38   richa
*  Initial revision.
*  
*     Rev 1.1   17 May 1996 13:48:58   kurtp
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
==   External procedures defined
=============================================================================*/

int  ParseCommand( PSD );


/*=============================================================================
==   Local procedures defined
=============================================================================*/

int  fnBeep( PSD );
int  fnExit( PSD );
int  fnSend( PSD );
int  fnSet( PSD );
int  fnWait( PSD );
int  GetToken( PSD, PCOMMANDS );


/*=============================================================================
==   External procedures used
=============================================================================*/

int  fnSendString( PSD );
int  fnSetIgnoreCase( PSD );
int  fnSetFlow( PSD );
int  fnSetParity( PSD );
int  fnSetSpeed( PSD );
int  fnSetDataBits( PSD );
int  fnSetStopBits( PSD );
int  fnSetXonXoff( PSD );
int  fnWaitReceive( PSD );
int  fnWaitSilence( PSD );
int  fnWaitString( PSD );
int  fnWaitTime( PSD );


/*=============================================================================
==   Local data
=============================================================================*/

COMMANDS Commands[] = {
    { "BEEP",         TOKEN_BEEP,             fnBeep },
    { "EXIT",         TOKEN_EXIT,             fnExit },
    { "SEND",         TOKEN_SEND,             fnSend },
    { "SET",          TOKEN_SET,              fnSet  },
    { "WAIT",         TOKEN_WAIT,             fnWait },
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


COMMANDS SendCommands[] = {
    { "STRING",       TOKEN_SEND_STRING,      fnSendString },
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


COMMANDS SetCommands[] = {
    { "IGNORECASE",   TOKEN_SET_IGNORECASE,   fnSetIgnoreCase },
#ifdef ASYNC_STUFF
    { "FLOW",         TOKEN_SET_FLOW,         fnSetFlow },
    { "PARITY",       TOKEN_SET_PARITY,       fnSetParity },
    { "SPEED",        TOKEN_SET_SPEED,        fnSetSpeed },
    { "DATABITS",     TOKEN_SET_DATABITS,     fnSetDataBits },
    { "STOPBITS",     TOKEN_SET_STOPBITS,     fnSetStopBits },
    { "XONXOFFCHARS", TOKEN_SET_XONXOFFCHARS, fnSetXonXoff },
#endif
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


COMMANDS WaitCommands[] = {
    { "RECEIVE",      TOKEN_WAIT_RECEIVE,     fnWaitReceive },
    { "SILENCE",      TOKEN_WAIT_SILENCE,     fnWaitSilence },
    { "STRING",       TOKEN_WAIT_STRING,      fnWaitString },
    { "TIME",         TOKEN_WAIT_TIME,        fnWaitTime },
    { NULL,           TOKEN_WORD,             NULL   },
    { NULL,           TOKEN_EOL,              NULL   },
};


/*******************************************************************************
 *
 *  ParseCommand
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
ParseCommand( PSD pSd )
{
    int token;
    int rc = SCRIPT_STATUS_SUCCESS;

    /*
     *  Init token pointers
     */
    pSd->offCommand = 0;
    pSd->offToken   = 0;

    /*
     *  Get first token
     */
    switch( (token = GetToken( pSd, Commands )) ) {

        case TOKEN_BEEP :
        case TOKEN_EXIT :
        case TOKEN_SEND :
        case TOKEN_SET  :
        case TOKEN_WAIT :
            rc = (*Commands[token].fnCommand)(pSd);
            break;

        case TOKEN_EOL :
            break;

        default :

            rc = SCRIPT_INVALID_SYNTAX;
            break;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  fnBeep
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
fnBeep( PSD pSd )
{

    /*
     *  Do the beep
     */
#ifdef DOS
    _asm {
        push ax
        push bx
        mov ax, 0e07h
        mov bx, 0
        int 10h
        pop bx
        pop ax
    }
#else
//  MessageBeep(MB_OK);
#endif

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnExit
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
fnExit( PSD pSd )
{

    /*
     *  Mark the script as closed, will get caught and closed by DevicePoll
     */
    pSd->fExit = TRUE;

    return( SCRIPT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  fnSend
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
fnSend( PSD pSd )
{
    int token;
    int rc;

    /*
     *  Get next token
     */
    switch( (token = GetToken( pSd, SendCommands )) ) {

        case TOKEN_SEND_STRING :
            rc = (*SendCommands[token].fnCommand)(pSd);
            break;

        default :
            rc = SCRIPT_INVALID_SYNTAX;
            break;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  fnSet
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
fnSet( PSD pSd )
{
    int token;
    int rc;

    /*
     *  Get next token
     */
    switch( (token = GetToken( pSd, SetCommands )) ) {

        case TOKEN_SET_IGNORECASE   :
#ifdef ASYNC_STUFF
        case TOKEN_SET_FLOW         :
        case TOKEN_SET_PARITY       :
        case TOKEN_SET_SPEED        :
        case TOKEN_SET_DATABITS     :
        case TOKEN_SET_STOPBITS     :
        case TOKEN_SET_XONXOFFCHARS :
#endif
            rc = (*SetCommands[token].fnCommand)(pSd);
            break;

        default :
            rc = SCRIPT_INVALID_SYNTAX;
            break;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  fnWait
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
fnWait( PSD pSd )
{
    int token;
    int rc;

    /*
     *  Get next token
     */
    switch( (token = GetToken( pSd, WaitCommands )) ) {

        case TOKEN_WAIT_RECEIVE :
        case TOKEN_WAIT_SILENCE :
        case TOKEN_WAIT_STRING  :
        case TOKEN_WAIT_TIME    :
            rc = (*WaitCommands[token].fnCommand)(pSd);
            break;

        default :
            rc = SCRIPT_INVALID_SYNTAX;
            break;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  GetToken
 *
 *  ENTRY:
 *    pSd (input)
 *       Pointer to sd data structure
 *    pCommands (input)
 *       Array of valid commands
 *
 *  EXIT:
 *
 ******************************************************************************/

int
GetToken( PSD pSd, PCOMMANDS pCommands )
{
    int    i;
    int    token = TOKEN_EOL;
    int    cbCommand = 0;
    LPBYTE p = (pSd->szCommand + pSd->offCommand);

    /*
     *  Current token starts at current command string
     */
    pSd->offToken = pSd->offCommand;

    /*
     *  Find start of token
     */
    while ( (*p != '\0') && (*p == ' ') ) {
        ++p;
        ++cbCommand;
    }
    pSd->offToken += cbCommand;

    /*
     *  Effective end of line?
     */
    if ( (*p == '\0') || (*p == ';') ) {
        *p = '\0';
        ++cbCommand;
        goto done;
    }

    /*
     *  Is this token a string?
     */
    if ( (*p == '\"') ) {
        token = TOKEN_STRING;
        *(p++) = '\0';
        ++cbCommand;
        ++pSd->offToken;
    }

    /*
     *  Build up token
     *  bugbug: fix for comments at end of line (e.g. SET SPEED 9600;fubar)
     */
    while ( (*p != '\0') ) {

        /*
         *  Handle string one way
         */
        if ( token == TOKEN_STRING ) {

            if ( (*p == '\"') ) {
                ++p;
                ++cbCommand;
                if ( (*p != '\"') ) {
                    *(p-1) = '\0';
                    break;
                }
            }
        }
        else if ( (*p == ' ') || (*p == ';') ) {
            break;
        }

        ++p;
        ++cbCommand;
    }
    *p = '\0';
    ++cbCommand;

    /*
     *  If string then done
     */
    if ( token == TOKEN_STRING ) {
        goto done;
    }

    /*
     *  Set pointer to token start
     */
    p = pSd->szCommand + pSd->offToken;

    /*
     *  Lookup token type
     */
    for( i=0; (pCommands+i)->token != TOKEN_EOL; i++ ) {

        /*
         *  Match?
         */
        if ( ((pCommands+i)->pszCommand == NULL) ||
             !stricmp(p, (pCommands+i)->pszCommand) ) {
            token = (pCommands+i)->token;
            break;
        }
    }

done:

    /*
     *  Set pointer beyond current command
     */
    pSd->offCommand += cbCommand;

    return( token );
}
