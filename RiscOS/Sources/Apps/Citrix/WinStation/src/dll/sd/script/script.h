
/*************************************************************************
*
*   SCRIPT.H
*
*   Scripting Driver - include file
*
*   Copyright Citrix Systems Inc. 1990-1996
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 13-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:05:44   richa
*  Initial revision.
*  
*     Rev 1.1   17 May 1996 13:49:52   kurtp
*  update
*  
*************************************************************************/


/*=============================================================================
==   Local structures
=============================================================================*/

typedef struct _COMMANDS {
    LPBYTE  pszCommand;         //  command name
    int     token;              //  command type
    int     (*fnCommand)(PSD);  //  command function
} COMMANDS, * PCOMMANDS;


/*=============================================================================
==   Macros and defines
=============================================================================*/

/*
 *  Token types
 */

#define TOKEN_WORD               -1
#define TOKEN_STRING             -2
#define TOKEN_EOL                -3
                                 
#define TOKEN_BEEP                0
#define TOKEN_EXIT                1
#define TOKEN_SEND                2
#define TOKEN_SET                 3
#define TOKEN_WAIT                4
                                 
#define TOKEN_SEND_STRING         0
                                 
#define TOKEN_SET_IGNORECASE      0
#ifdef ASYNC_STUFF
#define TOKEN_SET_FLOW            1
#define TOKEN_SET_PARITY          2
#define TOKEN_SET_SPEED           3
#define TOKEN_SET_DATABITS        4
#define TOKEN_SET_STOPBITS        5
#define TOKEN_SET_XONXOFFCHARS    6
#endif
                                 
#define TOKEN_WAIT_RECEIVE        0
#define TOKEN_WAIT_SILENCE        1
#define TOKEN_WAIT_STRING         2
#define TOKEN_WAIT_TIME           3


/*
 *  Current command state
 */

#define COMMAND_SEND_STRING       1
#define COMMAND_SET_IGNORECASE    2
#define COMMAND_SET_FLOW          3
#define COMMAND_SET_PARITY        4
#define COMMAND_SET_SPEED         5
#define COMMAND_SET_DATABITS      6
#define COMMAND_SET_STOPBITS      7
#define COMMAND_SET_XONXOFFCHARS  8
#define COMMAND_WAIT_RECEIVE      9
#define COMMAND_WAIT_SILENCE     10
#define COMMAND_WAIT_STRING      11
#define COMMAND_WAIT_TIME        12


/*=============================================================================
==  Global Functions
=============================================================================*/

int  fnSendString( PSD );
int  fnSetIgnoreCase( PSD );
#ifdef ASYNC_STUFF
int  fnSetFlow( PSD );
int  fnSetParity( PSD );
int  fnSetSpeed( PSD );
int  fnSetDataBits( PSD );
int  fnSetStopBits( PSD );
int  fnSetXonXoff( PSD );
#endif
int  fnWaitReceive( PSD );
int  fnWaitSilence( PSD );
int  fnWaitString( PSD );
int  fnWaitTime( PSD );
