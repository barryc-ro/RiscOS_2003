
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
*     Rev 1.14   15 Apr 1997 18:17:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   11 Nov 1996 15:50:40   richa
*  Put the virtual stuff in common\wdapi.c
*  
*  
*     Rev 1.11   26 Sep 1995 14:01:26   bradp
*  update
*  
*     Rev 1.10   01 Jun 1995 22:29:40   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.9   03 May 1995 11:45:32   kurtp
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
#include "../../../inc/xmsapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../inc/wd.h"


/*=============================================================================
==   External procedures defined
=============================================================================*/

int  WFCAPI WdICA30EmulOutBufAlloc( PWD, POUTBUF * );
void WFCAPI WdICA30EmulOutBufError( PWD, POUTBUF );
void WFCAPI WdICA30EmulOutBufFree( PWD, POUTBUF );
int  WFCAPI WdICA30EmulSetInfo( PWD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI WdICA30EmulQueryInfo( PWD, QUERYINFOCLASS, LPBYTE, USHORT );


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
 *  WdICA30EmulOutBufAlloc
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
WdICA30EmulOutBufAlloc( PWD pWd, POUTBUF * ppOutBuf )
{
    return( OutBufAlloc( pWd, ppOutBuf ) );
}


/*******************************************************************************
 *
 *  WdICA30EmulOutBufError
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
WdICA30EmulOutBufError( PWD pWd, POUTBUF pOutBuf )
{
    OutBufFree( pWd, pOutBuf );
}


/*******************************************************************************
 *
 *  WdICA30EmulOutBufFree
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
WdICA30EmulOutBufFree( PWD pWd, POUTBUF pOutBuf )
{
    OutBufFree( pWd, pOutBuf );
}


/*******************************************************************************
 *
 *  WdICA30EmulSetInfo
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
WdICA30EmulSetInfo( PWD pWd, SETINFOCLASS SetInfoClass, LPBYTE pInputBuffer, USHORT InputCount )
{
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( SetInfoClass ) {

        /*
         *  Callback is now complete, we can resume processing data
         */
        case CallbackComplete :

            /* set wd to connected state */
            pWd->fConnected = TRUE;
            break;

    }

    return( rc );
}

/*******************************************************************************
 *
 *  WdICA30EmulQueryInfo
 *
 *  The transport pd calls this routine when it needs access to wd data
 *  -- this routine is called by a protocol driver
 *
 * ENTRY:
 *    pWD (input)
 *       Pointer to Pd data structure
 *    pOutputBuffer (input-output)
 *       pointer to buffer structure
 *    InputCount Input
 *       size of buffer
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int WFCAPI
WdICA30EmulQueryInfo( PWD pWd, QUERYINFOCLASS QueryInfoClass, LPBYTE pOutputBuffer, USHORT OutputCount )
{
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( QueryInfoClass ) {

        /*
         *  Query Encryption initialization parameters
         */
        case QueryEncryptionInit :

	    if ( OutputCount >= sizeof( ENCRYPTIONINIT ) ) {
	        
                ((PENCRYPTIONINIT)pOutputBuffer)->EncryptionLevel =
		            pWd->EncryptionLevel;
            }
            break;

    }

    return( rc );
}
