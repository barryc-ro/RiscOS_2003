
/*************************************************************************
*
* pdapi.c
*
* Common code for all non-transport level PDs  (middle of pd stack)
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (4/2/94)
*
* pdapi.c,v
* Revision 1.1  1998/01/12 11:35:34  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.34   15 Apr 1997 16:51:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.34   21 Mar 1997 16:06:58   bradp
*  update
*  
*     Rev 1.33   08 May 1996 16:39:54   jeffm
*  update
*  
*     Rev 1.32   27 Sep 1995 15:02:54   scottk
*  trap in where pOutBuf is NULL but allowed and not checked for
*  
*     Rev 1.31   14 Jul 1995 12:07:54   bradp
*  update
*  
*     Rev 1.30   13 Jul 1995 13:07:12   bradp
*  update
*  
*     Rev 1.29   28 Jun 1995 22:13:42   bradp
*  update
*  
*     Rev 1.28   26 Jun 1995 17:59:54   marcb
*  update
*  
*     Rev 1.27   12 Jun 1995 12:06:40   bradp
*  update
*  
*     Rev 1.26   10 Jun 1995 14:25:16   bradp
*  update
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
#include "citrix/ica-h2c.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "../../../inc/pddevice.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int PdLoad( PDLLLINK );
static int PdUnload( PPD, PDLLLINK );
static int PdOpen( PPD, PPDOPEN );
static int PdClose( PPD, PDLLCLOSE );
static int PdInfo( PPD, PDLLINFO );
static int PdPoll( PPD, PDLLPOLL );
static int PdWrite( PPD, PPDWRITE );
static int PdQueryInformation( PPD, PPDQUERYINFORMATION );
static int PdSetInformation( PPD, PPDSETINFORMATION );

/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

static void CancelWrite( PPD );
int PdNext( PPD, USHORT, PVOID );


/*=============================================================================
==   External Functions used
=============================================================================*/

/*
int STATIC DeviceOpen( PPD, PPDOPEN );
int STATIC DeviceClose( PPD, PDLLCLOSE );
int STATIC DeviceInfo( PPD, PDLLINFO );
int STATIC DeviceConnect( PPD );
int STATIC DeviceDisconnect( PPD );
int STATIC DeviceInit( PPD, LPVOID, USHORT );
int STATIC DeviceEnable( PPD );
int STATIC DeviceDisable( PPD );
int STATIC DeviceQuery( PPD, PPDQUERYINFORMATION );
int STATIC DevicePoll( PPD, PDLLPOLL );
int STATIC DeviceWrite( PPD, PPDWRITE );
int STATIC DeviceCancelWrite( PPD );
int STATIC DeviceCallback( PPD );

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );
int STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );
*/



void STATIC OutBufError( PPD, POUTBUF );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define WinStation driver external procedures
 */
static PDLLPROCEDURE PdProcedures[ PD__COUNT ] = {
    (PDLLPROCEDURE) PdLoad,
    (PDLLPROCEDURE) PdUnload,
    (PDLLPROCEDURE) PdOpen,
    (PDLLPROCEDURE) PdClose,
    (PDLLPROCEDURE) PdInfo,
    (PDLLPROCEDURE) PdPoll,
    (PDLLPROCEDURE) PdWrite,
    (PDLLPROCEDURE) PdQueryInformation,
    (PDLLPROCEDURE) PdSetInformation,
};

/*
 *  Define Protocol driver link structure
 */
static DLLLINK PdLink = {0};

/*
 *  Define Protocol driver data structure
 *   (this could be dynamically allocated)
 */
// STATIC PD PdData = {0};


/*=============================================================================
==   Global Data
=============================================================================*/

/*
STATIC PPLIBPROCEDURE pClibProcedures = NULL;
STATIC PPLIBPROCEDURE pLogProcedures = NULL;
STATIC PPLIBPROCEDURE pBIniProcedures = NULL;
*/

/*******************************************************************************
 *
 *  Load
 *
 *    The user interface calls PdLoad to load and link a layered protocol
 *    driver.  All Pds must be loaded before loading the Wd.  PdLoad can be
 *    called multiple times to stack several protocol drivers to form a
 *    complete protocol stack.
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
PdLoad( PDLLLINK pLink )
{
    PPD pPd = malloc(sizeof(PD));

    TRACE(( TC_PD, TT_API1, "PdLoad: pLink in: '%s' proc %p data %p", pLink->ModuleName, pLink->pProcedures, pLink->pData ));

    /*
     *  This pd must never be loaded first.
     */
    if ( pLink->pData == NULL ) {
        ASSERT( FALSE, 0 );
        return( CLIENT_ERROR_INVALID_DLL_LOAD );
    }

    /*
     *  Copy the DllLink structure from the WinStation driver
     *  into this PD.  Clear structure after copying it.
     */
    PdLink = *pLink;
//  memset( pLink, 0, sizeof(DLLLINK) );

    /*
     *  Initialize DllLink structure
     */
    pLink->ProcCount   = PD__COUNT;
    pLink->pProcedures = PdProcedures;
    pLink->pData       = pPd;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  PdUnload
 *
 *  DllUnload calls PdUnload to unlink and unload a Pd.  The Wd must be
 *  unloaded before any Pds can be unloaded.  Pds are unloaded in the
 *  reverse order in which they were loaded.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdUnload( PPD pPd, PDLLLINK pLink )
{
    /*
     *  Unlink this PD from the linked list of protocol drivers
     */
    *pLink = pPd->Link;
//  memset( &PdLink, 0, sizeof(DLLLINK) );

    free(pPd);
    
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  PdOpen
 *
 *  The user interface calls PdOpen to open and initialize a Pd.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdOpen (input/output)
 *       pointer to the structure PDOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdOpen( PPD pPd, PPDOPEN pPdOpen )
{
    PDQUERYINFORMATION PdQueryInfo;
    BUFFERINFO BufferInfo;
    int rc;

    /*
     *  Initialize PD function call tables: MUST BE FIRST!
     */
//    pLogProcedures = pPdOpen->pLogProcedures;
//    pBIniProcedures = pPdOpen->pBIniProcedures;
//    pClibProcedures = pPdOpen->pClibProcedures;

    /*
     *  Initialize PD data structure
     */
    memset( pPd, 0, sizeof(PD) );
    pPd->pDeviceProcedures = pPdOpen->pDeviceProcedures;
    pPd->Link = PdLink;

    /*
     *  Query size of input and output buffers from TD (level 0)
     */
    PdQueryInfo.PdInformationClass  = PdBufferInfo;
    PdQueryInfo.pPdInformation      = &BufferInfo;
    PdQueryInfo.PdInformationLength = sizeof(BUFFERINFO);
    if ( rc = PdNext( pPd, PD__QUERYINFORMATION, &PdQueryInfo ) )
        goto badquery;

    pPd->OutBufLength      = BufferInfo.OutBufLength;
    pPd->OutBufCountHost   = BufferInfo.OutBufCountHost;
    pPd->OutBufCountClient = BufferInfo.OutBufCountClient;

    TRACE(( TC_PD, TT_API1,
            "PdOpen: OutBufLength %u, OutBufCountHost %u, OutBufCountClient %u",
            pPd->OutBufLength, pPd->OutBufCountHost, pPd->OutBufCountClient ));

    /*
     *  Initialize low level structures
     */
    if (pPd->pDeviceProcedures == 0)
    {
	rc = CLIENT_ERROR_BAD_OVERLAY;
	goto badopen;
    }

    if ( rc = DeviceOpen( pPd, pPdOpen ) )
        goto badopen;

    pPd->OutBufHeader  = pPdOpen->OutBufHeader;
    pPd->OutBufTrailer = pPdOpen->OutBufTrailer;

    /*
     *  Set Protocol Driver is open flag
     */
    pPd->fOpen = TRUE;
    pPd->CloseCode = CLIENT_STATUS_SUCCESS;

    TRACE(( TC_PD, TT_API1, "PdOpen: success" ));
    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad open
     *  query of buffer sizes failed
     */
badopen:
badquery:
    TRACE(( TC_PD, TT_API1, "PdOpen: rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  PdClose
 *
 *  The user interface calls PdClose to close a Pd before unloading it.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdClose( PPD pPd, PDLLCLOSE pPdClose )
{
    int rc;

    TRACE(( TC_PD, TT_API1, "PdClose: (enter)" ));

    /*
     *  Cancel all writes
     */
    (void) CancelWrite( pPd );
    TRACE(( TC_PD, TT_API1, "PdClose: (after cancel)" ));

    /*
     *  All writes should have previously been canceled
     *  which clears the open flag
     */
    ASSERT( !pPd->fOpen, 0 );

    /*
     *  Close PD
     */
    rc = DeviceClose( pPd, pPdClose );

    TRACE(( TC_PD, TT_API1, "PdClose: rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  PdInfo
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdInfo( PPD pPd, PDLLINFO pPdInfo )
{
    return( DeviceInfo( pPd, pPdInfo ) );
}


/*******************************************************************************
 *
 *  PdPoll
 *
 *    WdPoll calls PdPoll to get status and to give control to the protocol
 *    drivers.  This function never blocks and always returns the current
 *    status of the connection.  It then passes thru the DevicePoll passing
 *    the return code, this allows the current level pd to modify the rc.
 *    This is needed for the modem state machine to report state changes
 *    to the UI.
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdPoll( PPD pPd, PDLLPOLL pPdPoll )
{
    int rc;

    /*
     *  make sure logical connection is still open
     */
    if ( !pPd->fOpen ) {
        return( pPd->CloseCode );
    }

    /*
     *  If Protocol Driver is connected and we received a connect
     *  status from pd below us, send status to pd above us
     */
    if ( pPd->fSendStatusConnect && pPd->fRecvStatusConnect && !pPd->fConnected ) {
        TRACE(( TC_PD, TT_API1, "PdPoll[%u]: Send CLIENT_STATUS_CONNECTED", pPd->PdClass ));
        pPd->fConnected = TRUE;
        pPd->fSendStatusConnect = FALSE;
        return( CLIENT_STATUS_CONNECTED );
    }

    /*
     *  Send poll to next protocol driver
     */
    rc = DevicePoll( pPd, pPdPoll );

    /*
     *  Check for host connection
     */
    if ( rc == CLIENT_STATUS_CONNECTED ) {
        TRACE(( TC_PD, TT_API1, "PdPoll[%u]: Recv CLIENT_STATUS_CONNECTED", pPd->PdClass ));
        pPd->fRecvStatusConnect = TRUE;
        rc = CLIENT_STATUS_SUCCESS;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  PdWrite
 *
 *   PdWrite writes transport data
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdWrite (input/output)
 *       pointer to the structure PDWRITE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdWrite( PPD pPd, PPDWRITE pPdWrite )
{
    /*
     *  Check if we are open
     */
    if ( !pPd->fOpen ) {
        OutBufError( pPd, pPdWrite->pOutBuf );
        return( pPd->CloseCode );
    }

    /*
     *  Check if ica has been detected
     *  Check for raw write (retransmit)
     */
    if ( !pPd->fIcaDetected || 
        (pPdWrite->pOutBuf && pPdWrite->pOutBuf->fRetransmit )) {
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );
    }

    /*
     *  Process write data
     */
    return( DeviceWrite( pPd, pPdWrite ) );
}


/*******************************************************************************
 *
 *  PdQueryInformation
 *
 *   Queries information about the procotol driver
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdQueryInformation (input/output)
 *       pointer to the structure PDQUERYINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdQueryInformation( PPD pPd, PPDQUERYINFORMATION pPdQueryInformation )
{
    /*
     *  Call PD specific query routine
     */
    return( DeviceQuery( pPd, pPdQueryInformation ) );
}


/*******************************************************************************
 *
 *  PdSetInformation
 *
 *   Sets information for a procotol driver
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pPdSetInformation (input/output)
 *       pointer to the structure PDSETINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdSetInformation( PPD pPd, PPDSETINFORMATION pPdSetInformation )
{
    PWDADDRESS pWdAddress;
    PPD_H2C pPdHeader;
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pPdSetInformation->PdInformationClass ) {

        case PdWdAddress :

            if ( pPdSetInformation->PdInformationLength != sizeof(WDADDRESS) )
                return( CLIENT_ERROR_INVALID_BUFFER_SIZE );

            pWdAddress = (PWDADDRESS) pPdSetInformation->pPdInformation;
            pPd->pWdData            = pWdAddress->pWdData;
            pPd->pOutBufAllocProc   = pWdAddress->pOutBufAllocProc;
            pPd->pOutBufErrorProc   = pWdAddress->pOutBufErrorProc;
            pPd->pOutBufFreeProc    = pWdAddress->pOutBufFreeProc;
            pPd->pProcessInputProc  = pWdAddress->pProcessInputProc;
            pPd->pSetInfoProc       = pWdAddress->pSetInfoProc;
            pPd->pQueryInfoProc     = pWdAddress->pQueryInfoProc;

            if ( pWdAddress->pWdData ) {
                pWdAddress->pWdData            = pPd;
#if 1
                pWdAddress->pOutBufAllocProc   = (POUTBUFALLOCPROC)   pPd->pDeviceProcedures[PDDEVICE__OUTBUFALLOC];
                pWdAddress->pOutBufErrorProc   = (POUTBUFFREEPROC)    pPd->pDeviceProcedures[PDDEVICE__OUTBUFERROR];
                pWdAddress->pOutBufFreeProc    = (POUTBUFFREEPROC)    pPd->pDeviceProcedures[PDDEVICE__OUTBUFFREE];
                pWdAddress->pProcessInputProc  = (PPROCESSINPUTPROC)  pPd->pDeviceProcedures[PDDEVICE__PROCESSINPUT];
                pWdAddress->pSetInfoProc       = (PSETINFOPROC)       pPd->pDeviceProcedures[PDDEVICE__SETINFO];
                pWdAddress->pQueryInfoProc       = (PQUERYINFOPROC)   pPd->pDeviceProcedures[PDDEVICE__QUERYINFO];
#else
                pWdAddress->pOutBufAllocProc   = (POUTBUFALLOCPROC)   DeviceOutBufAlloc;
                pWdAddress->pOutBufErrorProc   = (POUTBUFFREEPROC)    DeviceOutBufError;
                pWdAddress->pOutBufFreeProc    = (POUTBUFFREEPROC)    DeviceOutBufFree;
                pWdAddress->pProcessInputProc  = (PPROCESSINPUTPROC)  DeviceProcessInput;
                pWdAddress->pSetInfoProc       = (PSETINFOPROC)       DeviceSetInfo;
                pWdAddress->pQueryInfoProc       = (PQUERYINFOPROC)   DeviceQueryInfo;
#endif
            }
            break;

        case PdConnect :

            /* connect physical layer */
            if ( rc = PdNext( pPd, PD__SETINFORMATION, pPdSetInformation ) )
                return( rc );

            /* connect logical layer */
            return( DeviceConnect( pPd ) );

        case PdDisconnect :

            if ( rc = DeviceDisconnect( pPd ) )
                return( rc );
            break;

        case PdCancelWrite :
            CancelWrite( pPd );
            return( CLIENT_STATUS_SUCCESS );

        case PdInitModule :
            pPdHeader = (PPD_H2C) pPdSetInformation->pPdInformation;
            if ( pPdHeader->PdClass == pPd->PdClass ) {
                rc = DeviceInit( pPd, pPdHeader,
                                 pPdSetInformation->PdInformationLength );
                TRACE(( TC_PD, TT_API2, "PdInitModule[%u], rc=%u", pPd->PdClass, rc ));
                if ( rc )
                    return( rc );
                pPd->fInitModule = TRUE;
            }
            break;

        case PdEnableModule :
            rc = DeviceEnable( pPd );
            TRACE(( TC_PD, TT_API2, "PdEnableModule[%u], rc=%u", pPd->PdClass, rc ));
            if ( rc ) {
	        return( rc );
            }
            if ( pPd->fInitModule ) {
                pPd->fEnableModule = TRUE;
            }
            break;

        case PdDisableModule :
            TRACE(( TC_PD, TT_API2, "PdDisableModule[%u]", pPd->PdClass ));
            pPd->fSendStatusConnect = FALSE;
            (void) DeviceDisable( pPd );

            pPd->fInitModule        = FALSE;
            pPd->fEnableModule      = FALSE;
            pPd->fConnected         = FALSE;
            pPd->fRecvStatusConnect = FALSE;
            break;

        case PdIcaDetected :
            pPd->fIcaDetected = TRUE;
            break;

        case PdCallback :

            /* callback physical layer */
            if ( rc = PdNext( pPd, PD__SETINFORMATION, pPdSetInformation ) )
                return( rc );

            /* callback logical layer */
            return( DeviceCallback( pPd ) );


    }

    return( PdNext( pPd, PD__SETINFORMATION, pPdSetInformation ) );
}


/*******************************************************************************
 *
 *  CancelWrite
 *
 *  flush output - bit bucket all pending output data
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

static void
CancelWrite( PPD pPd )
{
    PDSETINFORMATION PdSetInfo;
    POUTBUF pOutBuf;

    TRACE(( TC_PD, TT_API1, "PdCancelWrite (enter)" ));

    /*
     *  Bit bucket all future writes
     */
    pPd->fOpen = FALSE;
    pPd->CloseCode = CLIENT_STATUS_SUCCESS;

    /*
     *  Cancel transport level writes
     */
    PdSetInfo.PdInformationClass = PdCancelWrite;
    (void) PdNext( pPd, PD__SETINFORMATION, &PdSetInfo );

    /*
     *  Cancel queued writes
     */
    DeviceCancelWrite( pPd );

    /*
     *  Cancel pending write
     */
    if ( pOutBuf = pPd->pOutBufCurrent ) {
        pPd->pOutBufCurrent = NULL;
        OutBufError( pPd, pOutBuf );
    }

    TRACE(( TC_PD, TT_API1, "PdCancelWrite (exit)" ));
}


/*******************************************************************************
 *
 *  PdNext
 *
 *    Call PD procedure
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    ProcIndex (input)
 *       pd procedure index
 *    pParm (input/output)
 *       pointer to parameter for pd api
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
PdNext( PPD pPd, USHORT ProcIndex, PVOID pParam )
{
    PDLLPROCEDURE pProcedure;

#ifdef DEBUG
    ASSERT( pPd->Link.pProcedures, 0 );
    ASSERT( pPd->Link.pData, 0 );
    if ( ProcIndex >= pPd->Link.ProcCount )
    {
	TRACE(( TC_PD, TT_API1, "PdNext: '%s'", pPd->Link.ModuleName ));
        return( CLIENT_ERROR_BAD_PROCINDEX );
    }
#endif

    pProcedure = ((PDLLPROCEDURE *) pPd->Link.pProcedures)[ ProcIndex ];
    ASSERT( pProcedure, 0 );

    return( (*pProcedure)( pPd->Link.pData, pParam ) );
}
