
/*************************************************************************
*
*   SCRPTERR.H
*
*   Scripting Driver Error File
*
*   Copyright Citrix Systems Inc. 1990-1996
*
*   Author: Kurt Perry (kurtp)
*
*   Date: 09-May-1996
*
*   $Log$
*  
*     Rev 1.1   15 Apr 1997 16:53:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Aug 1996 12:05:58   richa
*  Initial revision.
*  
*     Rev 1.0   17 May 1996 13:45:18   kurtp
*  Initial revision.
*  
*************************************************************************/

//      SCRIPT_STATUS_SUCCESS   0x0000    // in SDAPI.H
//      SCRIPT_STATUS_COMPLETE  0x0001    // in SDAPI.H
#define SCRIPT_OPEN_FAILED      0x0002    // file not found or error opening
#define SCRIPT_READ_HOOK_FAILED 0x0003    // read hook set failed
#define SCRIPT_ERROR_NO_MEMORY  0x0004    // malloc failed
#define SCRIPT_INVALID_SYNTAX   0x0005    // error parsing script command
#define SCRIPT_INTERNAL_ERROR   0x0006    // unknown internal error
