/*******************************************************************************
*
*   LOGAPI.C
*
*   Log and event logging functions
*
*
*   Copyright Citrix Systems Inc. 1990
*
*   Author:   Brad Pedersen
*
*   $Log$
*  
*     Rev 1.8   15 Apr 1997 18:18:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.7   09 May 1996 11:16:18   jeffm
*  update
*  
*     Rev 1.6   06 Oct 1995 10:33:52   kurtp
*  update
*  
*     Rev 1.5   28 Jun 1995 17:46:30   marcb
*  update
*  
*     Rev 1.4   10 Jun 1995 14:02:44   marcb
*  update
*  
*     Rev 1.3   03 May 1995 12:16:52   butchd
*  clib.h now standard
*
******************************************************************************/

#include <windows.h>

/*  Get the standard C includes */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#ifndef DOS
#include <stdarg.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#endif

/*  Get CLIB includes */
#include "../../inc/client.h"
#ifdef  DOS
#include "../../inc/dos.h"
#endif
#include "../../inc/clib.h"
#include "../../inc/logapi.h"
#include "../../inc/wlogapi.h"

/*=============================================================================
==   Functions Defined
=============================================================================*/
int  WFCAPI LogStubOpen( PLOGOPEN pLogOpen );
int  WFCAPI LogStubClose( void );
void WFCAPI LogStubPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, ... );
void WFCAPI LogStubBuffer( ULONG LogClass, ULONG LogEnable,
                            LPVOID pBuffer, ULONG ByteCount );
void WFCAPI LogStubAssert( PCHAR, PCHAR, int, int );

/*=============================================================================
==   External Functions used
=============================================================================*/

/*=============================================================================
==   DATA
=============================================================================*/

PLIBPROCEDURE gpfnLogStubFunctions[]=
{
 (PLIBPROCEDURE)LogStubOpen,
 (PLIBPROCEDURE)LogStubClose,
 (PLIBPROCEDURE)LogStubPrintf,
 (PLIBPROCEDURE)LogStubBuffer,
 (PLIBPROCEDURE)LogStubAssert,
};

#ifndef DOS
extern HANDLE ghWFE;
extern ULONG gulLogClass;
extern ULONG gulLogEvent;

#ifdef DEBUG
BOOL vfLogging = TRUE;
#endif
#endif

/*******************************************************************************
 *
 *  LogStubLoad
 *
 *    Initialize and return entrypoints.
 *
 * ENTRY:
 *    pfnLogStubProcedures (input)
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI LogStubLoad( PPLIBPROCEDURE pfnLogStubProcedures )
{

   // set up entrypoint array to return
   *pfnLogStubProcedures = (PLIBPROCEDURE)gpfnLogStubFunctions;

   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  LogStubOpen
 *
 *    Open log file
 *
 * ENTRY:
 *    pLogOpen - pointer to log options and file name
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_OPEN_FAILED
 *
 * NOTE:
 *    To change log options, call LogOpen again (LogClose not needed).
 *
 ******************************************************************************/

int WFCAPI LogStubOpen( PLOGOPEN pLogOpen )
{
   return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  LogStubClose
 *
 *    Close log file
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI LogStubClose( void )
{
   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  LogStubPrintf
 *
 *  ENTRY:
 *     LogClass (input)
 *        log class mask
 *     LogEnable (input)
 *        log enable mask
 *     pFormat (input)
 *        format string
 *     ...  (input)
 *        enough arguments to satisfy format string
 *
 *  EXIT:
 *     number of bytes
 *
 ******************************************************************************/

void WFCAPI
LogStubPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, ... )
{
#ifndef DOS
    char Buffer[1024];
    va_list arg_marker;

#ifdef DEBUG
    // Bail if the logging suspended
    if ( vfLogging == FALSE )
        return;
#endif

    // Bail if the log flags don't match
    if ( (LogClass & LOG_ASSERT)  ||  // Always log asserts
         ((LogClass & gulLogClass) && (LogEnable & gulLogEvent)) ) {
        va_start( arg_marker, pFormat );
        wvsprintf( Buffer, pFormat, arg_marker );
        WFEngLogString( ghWFE, LogClass, LogEnable, Buffer ); 
    }
#endif
}

/*******************************************************************************
 *
 *  LogStubBuffer
 *
 *  This routine conditional writes a data buffer to the log file
 *
 *  ENTRY:
 *     LogClass (input)
 *        log class bit mask
 *     LogEnable (input)
 *        log type bit mask
 *     pBuffer (input)
 *        pointer to data buffer
 *     ByteCount (input)
 *        length of buffer
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

void WFCAPI
LogStubBuffer( ULONG LogClass, ULONG LogEnable,
             LPVOID pBuffer, ULONG ByteCount )
{
#ifndef DOS
    // Bail if the log flags don't match
    if ( ((LogClass & gulLogClass) && (LogEnable & gulLogEvent)) ) {
        WinLogBuffer( ghWFE, LogClass, LogEnable, pBuffer, ByteCount );
    }
#endif
}

#ifndef WIN32
#pragma optimize( "leg", off ) // Remove _asm warning
#endif
/*******************************************************************************
 *
 *  LogStubAssert
 *
 *  ENTRY:
 *     pExpr (input)
 *        assert expression
 *     pFileName (input)
 *        pointer to file name
 *     LineNumber (input)
 *        Line number of assert in file
 *     rc (input)
 *        error code
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

void WFCAPI
LogStubAssert( PCHAR pExpr, PCHAR pFileName, int LineNumber, int rc )
{
#ifndef DOS
WinLogAssert( ghWFE, (LPSTR)pExpr, (LPSTR)pFileName, LineNumber, rc );
#else
#ifdef DEBUG
   _asm int 3
#endif
#endif
}

#ifndef WIN32
#pragma optimize( "leg", on )
#endif

