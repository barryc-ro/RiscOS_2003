
/*************************************************************************
*
* pdframe.c
*
* Frame Protocol Driver - creates frames out of streaming data and
*                         ensures data integrity.
*
*  Frame Syntax:
*
*    <flag> <data> <crc> <flag>
*
*    1. Each frame begins and ends with a flag byte whose value is 0x7e.
*    2. The byte 0x7e is transmitted as the 2-byte sequence 0x7d, 0x5e.
*    3. The byte 0x7d is transmitted as the 2-byte sequence 0x7d, 0x5d.
*    4. The byte 0x11 is transmitted as the 2-byte sequence 0x7d, 0x31.
*    5. The byte 0x13 is transmitted as the 2-byte sequence 0x7d, 0x33.
*    6. The byte 0x91 is transmitted as the 2-byte sequence 0x7d, 0x61.
*    7. The byte 0x93 is transmitted as the 2-byte sequence 0x7d, 0x63.
*
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen (4/8/94)
*
* $Log$
*  
*     Rev 1.20   15 Apr 1997 16:52:18   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.20   21 Mar 1997 16:07:18   bradp
*  update
*  
*     Rev 1.19   09 Feb 1996 18:42:40   bradp
*  update
*  
*     Rev 1.18   13 Jul 1995 13:07:46   bradp
*  update
*  
*     Rev 1.17   28 Jun 1995 22:14:04   bradp
*  update
*  
*     Rev 1.16   10 Jun 1995 14:25:46   bradp
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
#include "pdframe.h"


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
static int DevicePoll( PPD, PDLLPOLL );
static int DeviceWrite( PPD, PPDWRITE );
static int DeviceCancelWrite( PPD );
static int DeviceQuery( PPD, PPDQUERYINFORMATION );
static int DeviceCallback( PPD );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

static int AppendEscData( PPD, POUTBUF, LPBYTE, USHORT );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );

USHORT CrcBuffer( LPBYTE, USHORT );

void OutBufError( PPD, POUTBUF );
int  OutBufAppend( PPD, POUTBUF, LPBYTE, USHORT );
int  OutBufWrite( PPD );
int PdNext( PPD, USHORT, PVOID );


/*=============================================================================
==   Local Data
=============================================================================*/

PLIBPROCEDURE PdFrameDeviceProcedures[PDDEVICE__COUNT] =
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


#define FRAME_HEADER_SIZE   1
#define FRAME_TRAILER_SIZE  3

/*
 *  Characters that need to be escaped
 *
 *  NOTE: keep this in sync with "FCLASSES" in input.c
 */
BYTE SpecialChar[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x00
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x20
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x30
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x40
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x50
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x60
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0,  // 0x70
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x80
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0x90
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xa0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xb0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xc0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xd0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xe0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0xf0
};

/*
 *  Define Framing Protocol driver data structure
 *   (this could be dynamically allocated)
 */
PDFRAME PdFrameData = {0};


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
    PPDFRAME pPdFrame;

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
     *  -- reserve an extra 5 bytes for character escaping
     *  -- if more than 5 characters need escaping an extra outbuf
     *     will be allocated.
     */
    pPdOpen->OutBufHeader     = FRAME_HEADER_SIZE + 5;  // frame byte
    pPdOpen->OutBufTrailer    = FRAME_TRAILER_SIZE; // 2 CRC bytes, frame byte
    pPdOpen->OutBufParam      = 0;
    pPdOpen->fOutBufCopy      = TRUE;
    pPdOpen->fOutBufFrame     = TRUE;

    /*
     *  Initialize PDFRAME data structure
     */
    pPdFrame = &PdFrameData;
    pPd->pPrivate = pPdFrame;
    memset( pPdFrame, 0, sizeof(PDFRAME) );

    /*
     *  Allocate PD input buffer
     */
    pPdFrame->pInBuf = malloc( pPd->OutBufLength );
    if ( pPdFrame->pInBuf == NULL )
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
    PPDFRAME pPdFrame;

    /*
     *  Get pointer to netbios pd
     */
    pPdFrame = (PPDFRAME) pPd->pPrivate;

    free( pPdFrame->pInBuf );
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
    BYTE FrameFlag = FRAME_FLAG;
    PPDFRAME pPdFrame;
    POUTBUF pOutBuf;
    USHORT Crc;
    int rc;

    /*
     *  Get pointer to outbuf being writen
     */
    pOutBuf = pPdWrite->pOutBuf;

    /*
     *  Get pointer to frame data structure
     */
    pPdFrame = (PPDFRAME) pPd->pPrivate;

    /*
     *  Append frame flag
     */
    if ( rc = OutBufAppend( pPd, pOutBuf, &FrameFlag, 1 ) )
        goto badappend;

    /*
     *  Append outbuf data
     */
    if ( rc = AppendEscData( pPd, pOutBuf, pOutBuf->pBuffer, pOutBuf->ByteCount ) )
        goto badappend;

    /*
     *  Append CRC
     */
    Crc = CrcBuffer( pOutBuf->pBuffer, pOutBuf->ByteCount );
    if ( rc = AppendEscData( pPd, pOutBuf, (LPBYTE) &Crc, sizeof(USHORT) ) )
        goto badappend;

    /*
     *  Append frame flag
     */
    if ( rc = OutBufAppend( pPd, pOutBuf, &FrameFlag, 1 ) )
        goto badappend;

    /*
     *  Free original outbuf
     */
    OutBufError( pPd, pOutBuf );

    /*
     *  Write any remaining data in last outbuf
     */
    if ( rc = OutBufWrite( pPd ) )
        goto badappend;

    TRACE(( TC_FRAME, TT_OFRAME,  "PdFrame: DeviceWrite, %u bytes", 
            pPdWrite->pOutBuf->ByteCount ));

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad append of data
     */
badappend:

    TRACE(( TC_FRAME, TT_ERROR,  "PdFrame: DeviceWrite, %u bytes, rc=%u", 
            pPdWrite->pOutBuf->ByteCount, rc ));

    OutBufError( pPd, pPdWrite->pOutBuf );
    if ( pPd->pOutBufCurrent ) {
        OutBufError( pPd, pPd->pOutBufCurrent );
        pPd->pOutBufCurrent = NULL;
    }

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
 *  AppendEscData
 *
 *  append data escaping all special characters
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pOutBufOrig (input)
 *       pointer to original outbuf (or null)
 *    pData (input)
 *       pointer to data buffer
 *    Length (input)
 *       length of data buffer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
AppendEscData( PPD pPd, POUTBUF pOutBufOrig, LPBYTE pData, USHORT Length )
{
    int rc;
    LPBYTE pCh;
    USHORT Len;
    BYTE str[2];

    while ( Length > 0 ) {

        /*
         *  Search for special character in buffer
         */
        pCh = pData;
        while ( Length > 0 && !SpecialChar[*pCh] ) {
            pCh++;
            Length--;
        }

        /*
         *  Append data preceeding special character or end of buffer
         */
        Len = pCh - pData;
        if ( Len > 0 ) {
            TRACE(( TC_FRAME, TT_OCOOK, "PdFrame: AppendEscData, %u", Len ));
            if ( rc = OutBufAppend( pPd, pOutBufOrig, pData, Len ) )
                return( rc );
            pData = pCh;
        }

        /*
         *  Append escaped character
         */
        if ( Length > 0 ) {
            TRACE(( TC_FRAME, TT_OCOOK,
                    "PdFrame: AppendEscData, %02x->7d %02x", *pCh, *pCh^0x20 ));
            str[0] = FRAME_ESC;
            str[1] = (*pCh ^ 0x20);
            if ( rc = OutBufAppend( pPd, pOutBufOrig, str, 2 ) )
                return( rc );
            pData++;
            Length--;
        }
    }

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

    /*
     *  Pass request to next PD
     */
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
