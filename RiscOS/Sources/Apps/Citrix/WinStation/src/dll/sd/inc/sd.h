
/***************************************************************************
*
*   SD.H
*
*   This module contains internal Scrip Driver (SD) defines and structures
*
*   Copyright Citrix Systems Inc. 1990-1996
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 09-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:04:28   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:44:46   kurtp
*  Initial revision.
*  
****************************************************************************/


/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _SD * PSD;


/*=============================================================================
==   structures
=============================================================================*/

/*
 *  SD structure
 */
typedef struct _SD {

    PDLLLINK pWdLink;                   //  DLL link strucutre pointer
    FILE   * hFile;                     //  script file handle

    BOOL     fExit;                     //  flag - script exiting
    BOOL     fReadHooked;               //  flag - real currently hooked
    BOOL     fProcessingCommand;        //  flag - currently processing command
    BOOL     fCaseSensitive;            //  flag - SET IGNORECASE OFF


#define MAX_COMMAND 128
    CHAR     szCommand[MAX_COMMAND];    //  current script command line
    int      offCommand;                //  current parsing offset in szCommand
    int      offToken;                  //  current token offset in szCommand
    int      iMatch;                    //  current wildcard matching offset
    int      iCurrent;                  //  anchor for wildcard matching offset

    int      LineNumber;                //  current script command line number
    int      Command;                   //  ordinal of current command
    ULONG    ulCommandTimeout;          //  current command's expiration time
    ULONG    ulInputTime;               //  time of last input data

} SD;
