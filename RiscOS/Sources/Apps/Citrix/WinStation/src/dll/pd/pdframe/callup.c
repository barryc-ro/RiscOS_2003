
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
*     Rev 1.9   15 Apr 1997 16:52:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   21 Mar 1997 16:07:14   bradp
*  update
*  
*     Rev 1.8   01 Jun 1995 22:28:54   terryt
*  Make encrypted clients work with SouthBeach
*  
*     Rev 1.7   03 May 1995 10:13:34   butchd
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

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdframe.h"


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
