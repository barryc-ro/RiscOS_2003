
/*************************************************************************
*
* callup.c
*
* Routines to handle buffers, input data and broken connections
* -- called by transport level PD
*
* copyright notice: Copyright 1993, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.9   15 Apr 1997 16:52:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   21 Mar 1997 16:07:20   bradp
*  update
*  
*     Rev 1.8   01 Jun 1995 22:29:02   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.7   03 May 1995 10:20:08   butchd
*  clib.h now standard
*  
*     Rev 1.6   02 May 1995 14:23:50   butchd
*  update
*  
*     Rev 1.5   17 Apr 1995 12:02:50   butchd
*  windows.h now standard
*  
*     Rev 1.4   07 Apr 1995 15:38:10   richa
*  Win client.
*
*     Rev 1.3   05 Apr 1995 16:39:02   butchd
*  global STATUS and ERROR code changes
*
*     Rev 1.2   28 Nov 1994 17:52:46   kurtp
*  update
*
*     Rev 1.1   01 Sep 1994 17:28:14   bradp
*  update
*
*     Rev 1.0   10 Aug 1994 15:15:16   bradp
*  Initial revision.
*
*     Rev 1.1   02 Aug 1994 17:34:58   bradp
*  update
*
*     Rev 1.0   22 Jul 1994 16:43:14   kurtp
*  Initial revision.
*
*     Rev 1.0   21 Jul 1994 17:26:54   bradp
*  Initial revision.
*
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

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdmodem.h"


/*=============================================================================
==   External procedures defined
=============================================================================*/

int  WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void WFCAPI DeviceOutBufError( PPD, POUTBUF );
void WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Internal procedures defined
=============================================================================*/

/*=============================================================================
==   External procedures used
=============================================================================*/

int OutBufAlloc( PPD, POUTBUF, POUTBUF * );
VOID OutBufError( PPD, POUTBUF );
VOID OutBufFree( PPD, POUTBUF );
int SetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


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

int WFCAPI
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

void WFCAPI
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

void WFCAPI
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

int WFCAPI
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

int WFCAPI
DeviceQueryInfo( PPD pPd, QUERYINFOCLASS QueryInfoClass, LPBYTE pBuffer, USHORT cbBuffer )
{
    return( QueryInfo( pPd, QueryInfoClass, pBuffer, cbBuffer ) );
}
