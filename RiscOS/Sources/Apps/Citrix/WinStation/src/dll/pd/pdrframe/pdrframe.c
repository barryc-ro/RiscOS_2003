
/*************************************************************************
*
*   pdrframe.c
*
*   Reliable Frame Protocol Driver - creates frames out of reliable data
*                                    streaming data and ensures data integrity.
*
*   Frame Syntax:
*
*    <length> <data>
*
*    1. Each frame begins with a length
*
*    2. Then the data follows
*
*   copyright notice: Copyright 1994, Citrix Systems Inc.
*
*   Author: Kurt Perry (10/4/94)
*
*   pdrframe.c,v
*   Revision 1.1  1998/01/12 11:35:50  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.13   15 Apr 1997 16:53:00   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   21 Mar 1997 16:07:42   bradp
*  update
*  
*     Rev 1.12   08 May 1996 16:50:46   jeffm
*  update
*  
*     Rev 1.11   13 Jul 1995 13:08:26   bradp
*  update
*  
*     Rev 1.10   28 Jun 1995 22:14:40   bradp
*  update
*  
*     Rev 1.9   12 Jun 1995 16:58:14   terryt
*  Disable reliable framer on old hosts
*  
*     Rev 1.8   10 Jun 1995 14:26:10   bradp
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
#include "citrix/ica-c2h.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#define NO_PDDEVICE_DEFINES
#include "../../../inc/pddevice.h"
#include "../../../inc/pddevicep.h"

#include "pdrframe.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int DeviceOpen( PPD, PPDOPEN );
static int DeviceClose( PPD, PDLLCLOSE );
static int DeviceInfo( PPD, PDLLINFO );
static int DeviceConnect( PPD );
static int DeviceDisconnect( PPD );
static int DeviceInit( PPD, LPVOID, USHORT );
static int DeviceEnable( PPD );
static int DeviceDisable( PPD );
static int DeviceQuery( PPD, PPDQUERYINFORMATION );
static int DevicePoll( PPD, PDLLPOLL );
static int DeviceWrite( PPD, PPDWRITE );
static int DeviceCancelWrite( PPD );
static int DeviceCallback( PPD );

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );

PLIBPROCEDURE PdRFrameDeviceProcedures[PDDEVICE__COUNT] =
{
    (PLIBPROCEDURE)DeviceOpen,
    (PLIBPROCEDURE)DeviceClose,

    (PLIBPROCEDURE)DeviceInfo,

    (PLIBPROCEDURE)DeviceConnect,
    (PLIBPROCEDURE)DeviceDisconnect,

    (PLIBPROCEDURE)DeviceInit,
    
    (PLIBPROCEDURE)DeviceEnable,
    (PLIBPROCEDURE)DeviceDisable,

    (PLIBPROCEDURE)DeviceProcessInput,
    (PLIBPROCEDURE)DeviceQuery,
    (PLIBPROCEDURE)DevicePoll,

    (PLIBPROCEDURE)DeviceWrite,
    (PLIBPROCEDURE)DeviceCancelWrite,

    (PLIBPROCEDURE)DeviceCallback,

    (PLIBPROCEDURE)DeviceSetInfo,
    (PLIBPROCEDURE)DeviceQueryInfo,

    (PLIBPROCEDURE)DeviceOutBufAlloc,
    (PLIBPROCEDURE)DeviceOutBufError,
    (PLIBPROCEDURE)DeviceOutBufFree
};

/*=============================================================================
==   External Functions Used
=============================================================================*/

void STATIC OutBufError( PPD, POUTBUF );
int  STATIC OutBufAppend( PPD, POUTBUF, LPBYTE, USHORT );
int  STATIC OutBufWrite( PPD );
int STATIC PdNext( PPD, USHORT, PVOID );
int  STATIC QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define Reliable Framing Protocol driver data structure
 *   (this could be dynamically allocated)
 */
STATIC PDRFRAME PdRFrameData = {0};


/*******************************************************************************
 *
 *  DeviceOpen
 *
 *  open and initialize device
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdOpen (output)
 *       pointer to pd open parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/


static int
DeviceOpen( PPD pPd, PPDOPEN pPdOpen )
{
    PPDRFRAME pPdRFrame;

    /*
     *  Save class of protocol driver
     */
    pPd->PdClass = PdFrame;

    /*
     *  This Pd does not need to handshake with host
     */
    pPd->fSendStatusConnect = TRUE;

    /*
     *  Return size of header and parameters
     */
    pPdOpen->OutBufHeader     = sizeof(USHORT);
    pPdOpen->OutBufTrailer    = 0;
    pPdOpen->OutBufParam      = 0;
    pPdOpen->fOutBufCopy      = FALSE;
    pPdOpen->fOutBufFrame     = FALSE;

    /*
     *  Initialize PDRFRAME data structure
     */
    pPdRFrame = &PdRFrameData;
    pPd->pPrivate = pPdRFrame;
    memset( pPdRFrame, 0, sizeof(PDRFRAME) );

    /*
     *  Save input buffer size
     */
    pPdRFrame->InputBufferLength = pPd->OutBufLength;

    /*
     *  Allocate PD input buffer
     */
    pPdRFrame->pInBuf = malloc( pPdRFrame->InputBufferLength );
    if ( pPdRFrame->pInBuf == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceClose
 *
 *  close physical device
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdClose (output)
 *       pointer to pd close parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceClose( PPD pPd, PDLLCLOSE pPdClose )
{
    PPDRFRAME pPdRFrame;

    /*
     *  Get pointer to pd
     */
    pPdRFrame = (PPDRFRAME) pPd->pPrivate;

    free( pPdRFrame->pInBuf );
    pPd->pPrivate = NULL;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceInfo
 *
 *    This routine is called to get module information
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to protocol driver data structure
 *    pPdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceInfo( PPD pPd, PDLLINFO pPdInfo )
{
    USHORT ByteCount;
    PPDFRAME_C2H pPdData;
    PMODULE_C2H pHeader;
    ENCRYPTIONINIT EncryptionInit;
    PPDRFRAME pPdRFrame;
    int rc;

    /*
     *  Get pointer to pd
     */
    pPdRFrame = (PPDRFRAME) pPd->pPrivate;

    /*
     * Get the host's initialization encryption level
     * This is used to detect old host versions that don't have a PDCRYPT1.DLL
     * and have the bug that closes the connection if a DLL isn't found.
     */
    rc = QueryInfo( pPd, QueryEncryptionInit,
                    (LPBYTE)&EncryptionInit, sizeof(EncryptionInit) );
    if ( rc != CLIENT_STATUS_SUCCESS )
        return ( rc );

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(PDFRAME_C2H);

    /*
     *  Check if buffer is big enough
     */
    if ( pPdInfo->ByteCount < ByteCount ) {
        pPdInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize default data
     */
    pPdInfo->ByteCount = ByteCount;
    pPdData = (PPDFRAME_C2H) pPdInfo->pBuffer;
    memset( pPdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_ProtocolDriver;
    pHeader->VersionL = VERSION_CLIENTL_PDFRAME;
    pHeader->VersionH = VERSION_CLIENTH_PDFRAME;
    /*
     * If host is old, don't try to load the DLL, and never enable the module
     */
    if ( EncryptionInit.EncryptionLevel > 0 ) {
        strcpy( pHeader->HostModuleName, "PDRFRAME" );
    }
    else {
        pPdRFrame->Disable = TRUE;
    }

    /*
     *  Initialize protocol driver data
     */
    pPdData->Header.PdClass = PdFrame;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceConnect
 *
 *  Connect to Citrix Application Server
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pParams (input)
 *       pointer to pd parameters
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceConnect( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceDisconnect
 *
 *  disconnect from citrix application server
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceDisconnect( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceInit
 *
 *  init module
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pBuffer (input)
 *       Pointer to init data from host
 *    ByteCount (input)
 *       length of init data in bytes
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceInit( PPD pPd, LPVOID pBuffer, USHORT ByteCount )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceEnable
 *
 *  enable module
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceEnable( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceDisable
 *
 *  disable module
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceDisable( PPD pPd )
{
    pPd->fSendStatusConnect = TRUE;
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  DevicePoll
 *
 *  device polling
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DevicePoll( PPD pPd, PDLLPOLL pPdPoll )
{

    return( PdNext( pPd, DLL__POLL, pPdPoll ) );
}



/*******************************************************************************
 *
 *  DeviceWrite
 *
 *  write a frame
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdWrite (input)
 *       pointer to protocol driver write structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceWrite( PPD pPd, PPDWRITE pPdWrite )
{
    POUTBUF pOutBuf;
    PPDRFRAME pPdRFrame;
    int rc;

    /*
     *  Check if protocol is enabled
     */
    if ( !pPd->fEnableModule )
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );

    /*
     *  Get pointer to current outbuf
     */
    pOutBuf = pPdWrite->pOutBuf;

    /*
     *  Check for old hosts
     */
    pPdRFrame = (PPDRFRAME) pPd->pPrivate;

    if ( pPdRFrame->Disable )
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );

    /*
     *  Just resend on retransmit, length already prepended
     */
    if ( pOutBuf->fRetransmit )
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );

    /*
     *  Make room for byte count header
     */
    pOutBuf->pBuffer -= sizeof(USHORT);

    /*
     *  Save byte count as header
     */
    *((PUSHORT)pOutBuf->pBuffer) = (USHORT) pOutBuf->ByteCount;

    /*
     *  Increment byte count to include size of header
     */
    pOutBuf->ByteCount += sizeof(USHORT);

    /*
     *  Write buffer
     */
    rc = PdNext( pPd, PD__WRITE, pPdWrite );

    TRACE(( TC_FRAME, TT_API3, "PdRFrame: DeviceWrite, bc %u, Status=0x%x",
            pOutBuf->ByteCount, rc ));

    return( rc );
}


/*******************************************************************************
 *
 *  DeviceCancelWrite
 *
 *  Cancel all pending writes
 *  -- don't let any further writes occur
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceCancelWrite( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceQuery
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
DeviceQuery( PPD pPd, PPDQUERYINFORMATION pPdQueryInformation )
{
    return( PdNext( pPd, PD__QUERYINFORMATION, pPdQueryInformation ) );
}


/*******************************************************************************
 *
 *  DeviceCallback
 *
 *  Process Callback Set Info
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int 
DeviceCallback( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}
