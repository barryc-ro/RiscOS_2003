
/*************************************************************************
*
*   callup.c 
*
*   called by protocol drivers
*
*   copyright notice: Copyright 1994, Citrix Systems Inc.
*
*   Author: Brad Pedersen
*
*   $Log$
*  
*     Rev 1.10   15 Apr 1997 18:18:10   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   01 Jun 1995 22:29:46   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.8   03 May 1995 11:51:08   kurtp
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"

#define NO_WDEMUL_DEFINES
#include "../inc/wd.h"

/*=============================================================================
==   External procedures defined
=============================================================================*/

int  WFCAPI WdTTYEmulOutBufAlloc( PWD, POUTBUF * );
void WFCAPI WdTTYEmulOutBufError( PWD, POUTBUF );
void WFCAPI WdTTYEmulOutBufFree( PWD, POUTBUF );
int  WFCAPI WdTTYEmulSetInfo( PWD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI WdTTYEmulQueryInfo( PWD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Internal procedures defined
=============================================================================*/

/*=============================================================================
==   External procedures used
=============================================================================*/

int OutBufAlloc( PWD, POUTBUF * );
VOID OutBufFree( PWD, POUTBUF );


/*******************************************************************************
 *
 *  WdTTYEmulOutBufAlloc
 *
 *  The protocol drvier calls this routine to allocate an output buffer
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    ppOutBuf (output)
 *       address to return pointer to buffer structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdTTYEmulOutBufAlloc( PWD pWd, POUTBUF * ppOutBuf )
{
    return( OutBufAlloc( pWd, ppOutBuf ) );
}


/*******************************************************************************
 *
 *  WdTTYEmulOutBufError
 *
 *  Return an outbuf to the wd free list (data was not written)
 *  -- this routine is called by a protocol driver
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void WFCAPI
WdTTYEmulOutBufError( PWD pWd, POUTBUF pOutBuf )
{
    OutBufFree( pWd, pOutBuf );
}


/*******************************************************************************
 *
 *  WdTTYEmulOutBufFree
 *
 *  The transport pd calls this routine when it's done writing a buffer
 *  -- this routine is called by a protocol driver
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void WFCAPI
WdTTYEmulOutBufFree( PWD pWd, POUTBUF pOutBuf )
{
    OutBufFree( pWd, pOutBuf );
}


/*******************************************************************************
 *
 *  WdTTYEmulSetInfo
 *
 *  The transport pd calls this routine when it needs access to wd data
 *  -- this routine is called by a protocol driver
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int WFCAPI
WdTTYEmulSetInfo( PWD pWd, SETINFOCLASS SetInfoClass, LPBYTE pInputBuffer, USHORT InputCount )
{
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( SetInfoClass ) {

        /*
         *  Not supported
         */
        default :

            rc = CLIENT_ERROR_BAD_PROCINDEX;
            break;

    }

    return( rc );
}

/*******************************************************************************
 *
 *  WdTTYEmulQueryInfo
 *
 *  The transport pd calls this routine when it needs access to wd data
 *  -- this routine is called by a protocol driver
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    pOutputBufffer (input-output)
 *       pointer to buffer 
 *    OutputCount
 *       length of buffer
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int WFCAPI
WdTTYEmulQueryInfo( PWD pWd, QUERYINFOCLASS QueryInfoClass, LPBYTE pOutputBuffer, USHORT OutputCount )
{
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( QueryInfoClass ) {

        /*
         *  Not supported
         */
        default :

            rc = CLIENT_ERROR_BAD_PROCINDEX;
            break;

    }

    return( rc );
}
