
/*************************************************************************
*
*   wdtty.c
*
*   WinStation driver - TTY
*
*   Copyright Citrix Systems Inc. 1994
*
*   Author: Brad Pedersen  (3/29/94)
*
*   $Log$
*  
*     Rev 1.23   15 Apr 1997 18:18:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.22   12 Feb 1996 09:38:02   richa
*  added a nop WriteTTY.
*
*     Rev 1.21   29 Jan 1996 20:12:32   bradp
*  update
*
*     Rev 1.20   03 May 1995 17:46:18   marcb
*  update
*
*     Rev 1.19   03 May 1995 12:54:46   marcb
*  update
*
*     Rev 1.18   03 May 1995 11:51:14   kurtp
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
#include "../../../inc/vioapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"

#define NO_WDEMUL_DEFINES
#include "../inc/wd.h"
#include "../../../inc/wdemul.h"

/*=============================================================================
==   Defines
=============================================================================*/


/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int EmulOpen( PWD, PWDOPEN );
static int EmulClose( PWD );
static int EmulInfo( PWD, PDLLINFO );
static int EmulPoll( PWD, PDLLPOLL, int );
static int EmulSetInformation( PWD, PWDSETINFORMATION );
static int WFCAPI EmulProcessInput( PWD, CHAR FAR *, USHORT );
static void WriteTTY( LPVOID pIca, LPBYTE pTTYData, USHORT cbTTYData );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

static int KeyWrite( PWD, PCHARCODE );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int  WFCAPI OutBufReserve( PWD, USHORT );
int  WFCAPI OutBufAppend( PWD, CHAR FAR *, USHORT );
int  WFCAPI OutBufWrite( PWD );

int  WFCAPI WdTTYEmulOutBufAlloc( PWD, POUTBUF * );
void WFCAPI WdTTYEmulOutBufError( PWD, POUTBUF );
void WFCAPI WdTTYEmulOutBufFree( PWD, POUTBUF );
int  WFCAPI WdTTYEmulSetInfo( PWD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI WdTTYEmulQueryInfo( PWD, QUERYINFOCLASS, LPBYTE, USHORT );


/*******************************************************************************/

PLIBPROCEDURE WdTTYEmulProcedures[WDEMUL__COUNT] =
{
    (PLIBPROCEDURE)EmulOpen,
    (PLIBPROCEDURE)EmulClose,
    (PLIBPROCEDURE)EmulInfo,
    (PLIBPROCEDURE)EmulProcessInput,
    (PLIBPROCEDURE)EmulPoll,
    (PLIBPROCEDURE)EmulSetInformation,
    (PLIBPROCEDURE)WdTTYEmulSetInfo,
    (PLIBPROCEDURE)WdTTYEmulQueryInfo,
    (PLIBPROCEDURE)WdTTYEmulOutBufAlloc,
    (PLIBPROCEDURE)WdTTYEmulOutBufError,
    (PLIBPROCEDURE)WdTTYEmulOutBufFree,
    (PLIBPROCEDURE)WriteTTY
};

/*******************************************************************************
 *
 *  EmulOpen
 *
 *  WdOpen calls EmulOpen
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdOpen (input/output)
 *       pointer to the structure WDOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
EmulOpen( PWD pWd, PWDOPEN pWdOpen )
{

    /*
     *  Return the number of supported virtual channels
     */
    pWdOpen->MaxVirtualChannels = 0;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  EmulClose
 *
 *  WdClose calls EmulClose
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
EmulClose( PWD pWd )
{

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  EmulInfo
 *
 *    This routine is called to get ICA 3.0 information
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to procotol driver data structure
 *    pWdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
EmulInfo( PWD pWd, PDLLINFO pWdInfo )
{

    pWdInfo->ByteCount = 0;
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  EmulPoll
 *
 *  WdPoll calls EmulPoll to process keyboard and mouse input
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdPoll (input)
 *       pointer to poll structure
 *    PdStatus (input)
 *       protocol driver status
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
EmulPoll( PWD pWd, PDLLPOLL pWdPoll, int PdStatus )
{
    int rc = CLIENT_STATUS_SUCCESS;

    /*
     *  Ack kill focus
     */
    if ( pWd->fReceivedStopOk ) {
        pWd->fReceivedStopOk = FALSE;
        return( CLIENT_STATUS_KILL_FOCUS );
    }

    return( rc );
}


/*******************************************************************************
 *
 *  EmulSetInformation
 *
 *   Sets information for winstation driver
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdSetInformation (input/output)
 *       pointer to the structure WDSETINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
EmulSetInformation( PWD pWd, PWDSETINFORMATION pWdSetInformation )
{
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pWdSetInformation->WdInformationClass ) {

        case WdKillFocus :
            pWd->fReceivedStopOk = TRUE;
            break;

        case WdCharCode :

            /*
             *  Send keystrokes down the wire
             */
            ASSERT( pWdSetInformation->WdInformationLength == sizeof(CHARCODE), 0 );
            rc = KeyWrite( pWd, (PCHARCODE) pWdSetInformation->pWdInformation );

            break;

#if !defined(DOS) && !defined(RISCOS)
        case WdInitWindow :
            rc = VioInitWindow( (HWND)(ULONG)pWdSetInformation->pWdInformation,
                          25, 80, TRUE ); //BUGBUG: Don't hardcode row/columns
            break;

        case WdDestroyWindow :
            rc = VioDestroyWindow( (HWND)(ULONG)pWdSetInformation->pWdInformation );
            break;

        case WdPaint :
            rc = VioPaint( (HWND)(ULONG)pWdSetInformation->pWdInformation );
            break;
#endif
    }

    return( rc );
}


/*******************************************************************************
 *
 *  EmulProcessInput
 *
 *  Protocol Driver calls EmulProcessInput to process input data
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pBuffer (input)
 *       pointer to data buffer
 *    ByteCount (input)
 *       number of bytes of data in buffer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int WFCAPI
EmulProcessInput( PWD pWd, CHAR FAR * pBuffer, USHORT ByteCount )
{

    return( VioWrtTTY( pBuffer, ByteCount, 0 ) );
}

/*******************************************************************************
 *
 *  KeyWrite
 *
 *  send keyboard data to host
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_STATUS_NO_DATA - no keyboard data to send
 *
 ******************************************************************************/

static int
KeyWrite( PWD pWd, PCHARCODE pCC )
{
    int i;
    int rc;
    int rc2;
    CHAR Ch;
    PUSHORT pChar = pCC->pCharCodes;

    /*
     *  Allocate output buffer
     */
    if ( rc = OutBufReserve( pWd, pCC->cCharCodes ) ) {
        if ( rc == CLIENT_ERROR_NO_OUTBUF )
            rc = CLIENT_STATUS_SUCCESS;
        return( rc );
    }

    /*
     *  Append keyboard data to output buffer
     */
    for ( i=0; i< (int)pCC->cCharCodes; i++ ) {

        Ch = (CHAR) (UCHAR) *(pChar+i);
        if ( rc = OutBufAppend( pWd, (CHAR FAR *) &Ch, 1 ) )
            break;
    }

    /*
     *  Write outbuf to protocol driver
     *  -- this frees the allocated outbufs
     */
    if ( rc2 = OutBufWrite( pWd ) )
        rc = rc2;

    return( rc );
}


/*******************************************************************************
 *
 *  WriteTTY
 *
 *  does nothing.
 *
 * ENTRY:
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

static void
WriteTTY( LPVOID pICA, LPBYTE pTTYData, USHORT cbTTYData )
{
    return;
}
