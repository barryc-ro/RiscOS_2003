
/*************************************************************************
*
* pdcrypt.c
*
* Encryption protocol driver 
*
* copyright notice: Copyright 1995, Citrix Systems Inc.
*
* smiddle
*
* pdcrypt.c,v
* Revision 1.1  1998/01/12 11:35:41  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.13   15 Apr 1997 16:52:02   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   21 Mar 1997 16:07:12   bradp
*  update
*  
*     Rev 1.12   08 May 1996 16:42:48   jeffm
*  update
*  
*     Rev 1.11   20 Jul 1995 12:08:00   bradp
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdcrypt.h"

#define NO_PDDEVICE_DEFINES
#include "../../../inc/pddevice.h"
#include "../../../inc/pddevicep.h"

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

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );

PLIBPROCEDURE PdCryptDeviceProcedures[PDDEVICE__COUNT] =
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
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions Used
=============================================================================*/

void STATIC OutBufError( PPD, POUTBUF );
int  STATIC OutBufWrite( PPD );
int  STATIC PdNext( PPD, USHORT, PVOID );
int  STATIC QueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define Encryption Protocol driver data structure
 *   (this could be dynamically allocated)
 */
STATIC PDCRYPT PdCryptData = {0};


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
    PPDCRYPT pPdCrypt;

    /*
     *  Save class of protocol driver
     */
    pPd->PdClass = PdEncrypt;

    /*
     *  Return size of header and parameters
     *  -- since this pd allocates a new outbuf and copies the data we don't
     *     need to reserve any space.
     */
    pPdOpen->OutBufHeader     = 1;   // compress header flag
    pPdOpen->OutBufTrailer    = 0;
    pPdOpen->OutBufParam      = 0;
    pPdOpen->fOutBufCopy      = TRUE;
    pPdOpen->fOutBufFrame     = FALSE;

    /*
     *  Initialize PDCOMP data structure
     */
    pPdCrypt = &PdCryptData;
    pPd->pPrivate = pPdCrypt;
    memset( pPdCrypt, 0, sizeof(PDCRYPT) );

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
    PPDCRYPT1_C2H pPdData;
    PMODULE_C2H pHeader;
    ENCRYPTIONINIT EncryptionInit;
    PPDCRYPT pPdCrypt;
    int rc;

    /*
     *  Get pointer to crypt data structure
     */
    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

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
    ByteCount = sizeof(PDCRYPT1_C2H);

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
    pPdData = (PPDCRYPT1_C2H) pPdInfo->pBuffer;
    memset( pPdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_ProtocolDriver;
    pHeader->VersionL = VERSION_CLIENTL_PDCRYPT1;
    pHeader->VersionH = VERSION_CLIENTH_PDCRYPT1;

    /*
     * If host is old, don't try to load the DLL, and never enable the module
     */
    if ( EncryptionInit.EncryptionLevel > 0 ) {
        strcpy( pHeader->HostModuleName, "PDCRYPT1" );
    }
    else {
        pPdCrypt->fOff = TRUE;
        pPd->fSendStatusConnect = TRUE;
    }

    /*
     *  Initialize protocol driver data
     */
    pPdData->Header.PdClass = PdEncrypt;
    pPdData->EncryptionLevel = 1;

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
 *  Disable module
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
    PPDCRYPT pPdCrypt;

    /*
     *  Get pointer to crypt data structure
     */
    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

    ResetSessionState( pPdCrypt );
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
 *  write a buffer
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
    int rc;
    PPDCRYPT pPdCrypt;
    POUTBUF  pOutBuf;

    /*
     *  Check if protocol is enabled
     */
    if ( !pPd->fEnableModule ) 
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );

    /*
     *  Get pointer to crypt data structure
     */
    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

    /*
     *  No header if we have been disabled
     */
    if ( pPdCrypt->fOff ) 
        return( PdNext( pPd, PD__WRITE, pPdWrite ) );

    /*
     *  Get pointer to outbuf being writen
     */
    pOutBuf = pPdWrite->pOutBuf;


    /*
     *  Put the header byte in the buffer
     */

    if ( pPdCrypt->fSessionKey ) {

         TRACE(( TC_PD, TT_ORAW, "PdCrypt: before encrypt, %d", pOutBuf->ByteCount ));
         TRACEBUF(( TC_PD, TT_ORAW, pOutBuf->pBuffer, (ULONG)pOutBuf->ByteCount ));
     
         EncryptStream( pPdCrypt, pOutBuf->pBuffer, pOutBuf->ByteCount );

         if ( !pPdCrypt->fPerm ) {
             pOutBuf->pBuffer--;
             pOutBuf->ByteCount++;
             *(pOutBuf->pBuffer) = CRYPT_ENCRYPTED;
         }
    }
    else {
        /*
	 * This should only be allowed under controled conditions.
	 * I.E. just at startup.
	 */
         pOutBuf->pBuffer--;
         pOutBuf->ByteCount++;
        *(pOutBuf->pBuffer) = CRYPT_NOT_ENCRYPTED;
    }

    /*
     *  Write buffer
     */
    rc = PdNext( pPd, PD__WRITE, pPdWrite );

    TRACE(( TC_PD, TT_API3, "PdCrypt: DeviceWrite, bc %u, Status=0x%x", 
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
