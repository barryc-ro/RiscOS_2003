
/*************************************************************************
*
* callup.c 
*
* Routines to handle buffers, input data and broken connections
* -- called by transport level PD
*
* copyright notice: Copyright 1993, Citrix Systems Inc.
*
* smiddle 
*
* callup.c,v
* Revision 1.1  1998/01/12 11:35:45  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.6   15 Apr 1997 16:51:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   21 Mar 1997 16:07:08   bradp
*  update
*  
*     Rev 1.5   08 May 1996 16:45:46   jeffm
*  update
*  
*     Rev 1.4   01 Jun 1995 22:28:38   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.3   09 May 1995 11:43:00   terryt
*  Reconnect redo and fix types
*  
*     Rev 1.2   03 May 1995 11:27:10   butchd
*  clib.h now standard
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
#include "citrix/ica.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdrframe.h"

/*=============================================================================
==   External procedures defined
=============================================================================*/

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Internal procedures defined
=============================================================================*/

/*=============================================================================
==   External procedures used
=============================================================================*/

int STATIC OutBufAlloc( PPD, POUTBUF, POUTBUF * );
VOID STATIC OutBufError( PPD, POUTBUF );
VOID STATIC OutBufFree( PPD, POUTBUF );
int STATIC SetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int STATIC QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*******************************************************************************
 *
 *  DeviceOutBufAlloc
 *
 *  The transport pd calls this routine to allocate an output buffer
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    ppOutBuf (output)
 *       address to return pointer to buffer structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
DeviceOutBufAlloc( PPD pPd, POUTBUF * ppOutBuf )
{
    return( OutBufAlloc( pPd, NULL, ppOutBuf ) );
}


/*******************************************************************************
 *
 *  DeviceOutBufError
 *
 *  Return an outbuf to the wd free list
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC WFCAPI
DeviceOutBufError( PPD pPd, POUTBUF pOutBuf )
{
    OutBufError( pPd, pOutBuf );
}


/*******************************************************************************
 *
 *  DeviceOutBufFree
 *
 *  The transport pd calls this routine when it's done writing a buffer
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *    pOutBuf (input)
 *       pointer to buffer structure to free
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void STATIC WFCAPI
DeviceOutBufFree( PPD pPd, POUTBUF pOutBuf )
{
    OutBufFree( pPd, pOutBuf );
}
           

/*******************************************************************************
 *
 *  DeviceSetInfo
 *
 *  The transport pd calls this routine when wants to modify wd data
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int STATIC WFCAPI
DeviceSetInfo( PPD pPd, SETINFOCLASS SetInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{
    return( SetInfo( pPd, SetInfoClass, pBuffer, cbBuffer ) );
}

/*******************************************************************************
 *
 *  DeviceQueryInfo
 *
 *  The transport pd calls this routine when wants to query wd data
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPD (input)
 *       Pointer to Pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int STATIC WFCAPI
DeviceQueryInfo( PPD pPd, QUERYINFOCLASS QueryInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{
    return( QueryInfo( pPd, QueryInfoClass, pBuffer, cbBuffer ) );
}
