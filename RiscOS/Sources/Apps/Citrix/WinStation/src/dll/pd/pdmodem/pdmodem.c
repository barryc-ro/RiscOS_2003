
/*************************************************************************
*
*  PDMODEM.C
*
*  Modem Protocol Driver - strategy level routines
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Kurt Perry (6/2/94)
*
*  $Log$
*  
*     Rev 1.21   15 Apr 1997 16:52:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.21   21 Mar 1997 16:07:26   bradp
*  update
*  
*     Rev 1.20   30 Jan 1996 18:39:56   bradp
*  update
*  
*     Rev 1.19   13 Jul 1995 13:07:56   bradp
*  update
*  
*     Rev 1.18   28 Jun 1995 22:14:24   bradp
*  update
*  
*     Rev 1.17   10 Jun 1995 14:25:54   bradp
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
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/pd.h"

#include "pdmodem.h"

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
static int DeviceWrite( PPD, PPDWRITE );
static int DeviceCancelWrite( PPD );
static int DeviceQuery( PPD, PPDQUERYINFORMATION );
static int DeviceCallback( PPD );

/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions Used
=============================================================================*/

extern int  PdNext( PPD, USHORT, PVOID );
extern int  ModemOpen( PPDMODEM, PPDOPEN );
extern void ModemHangup( PPD, PPDMODEM );
extern void DestroyModemString( PPDMODEM );
extern int  ModemCallback( PPD );

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );

extern int DevicePoll( PPD, PDLLPOLL );

/*=============================================================================
==   Local Data
=============================================================================*/

PLIBPROCEDURE PdModemDeviceProcedures[PDDEVICE__COUNT] =
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

/*
 *  Define Modem Protocol driver data structure
 *   (this could be dynamically allocated)
 */
PDMODEM PdModemData = {0};


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
    PPDMODEM pPdModem;
    char Buffer[60];

    /*
     *  Save class of protocol driver
     */
    pPd->PdClass = PdModem;

    /*
     *  Return size of header and parameters
     *  -- since this pd allocates a new outbuf and copies the data we don't
     *     need to reserve any space.
     */
    pPdOpen->OutBufHeader     = 0;
    pPdOpen->OutBufTrailer    = 0;
    pPdOpen->OutBufParam      = 0;
    pPdOpen->fOutBufCopy      = FALSE;
    pPdOpen->fOutBufFrame     = FALSE;

    /*
     *  Initialize PDMODEM data structure
     */
    pPdModem = &PdModemData;
    pPd->pPrivate = pPdModem;
    memset( pPdModem, 0, sizeof(PDMODEM) );

    /*
     *  Get modem type
     */
    bGetPrivateProfileString( pPdOpen->pIniSection, INI_MODEMTYPE, INI_EMPTY, Buffer, sizeof(Buffer) );
    pPdModem->pModemName = strdup( Buffer );

    pPdModem->fEchoTTY = bGetPrivateProfileBool( pPdOpen->pIniSection, INI_ECHO_TTY, DEF_ECHO_TTY );

    TRACE(( TC_MODEM, TT_API2, "PdModem: DeviceOpen, %s", pPdModem->pModemName ));

    /*
     *  Initialize Modem Parameters
     */
    return( ModemOpen( pPdModem, pPdOpen ) );
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
    PPDMODEM pPdModem;

    TRACE(( TC_MODEM, TT_API1, "PdModem: DeviceClose" ));

    /*
     *  Get pointer to modem pd's data
     */
    pPdModem = (PPDMODEM) pPd->pPrivate;

    /*
     *  Free modem type string
     */
    free( pPdModem->pModemName );

    /*
     *  Free the modem strings
     */
    DestroyModemString( pPdModem );

    /*
     *  Mark private data as not valid
     */
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
    PPDMODEM_C2H pPdData;
    PMODULE_C2H pHeader;
    USHORT ModemNameLength;
    PPDMODEM pPdModem;

    /*
     *  Get pointer to modem pd's data
     */
    pPdModem = (PPDMODEM) pPd->pPrivate;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(PDMODEM_C2H);
    ModemNameLength = strlen( pPdModem->pModemName );
    if ( ModemNameLength > 0 )
        ByteCount += (ModemNameLength + 1);

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
    pPdData = (PPDMODEM_C2H) pPdInfo->pBuffer;
    memset( pPdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_ProtocolDriver;
    pHeader->VersionL = VERSION_CLIENTL_PDMODEM;
    pHeader->VersionH = VERSION_CLIENTH_PDMODEM;

    /*
     *  Initialize protocol driver data
     */
    pPdData->Header.PdClass = PdModem;

    /*
     *  Initialize modem name string
     */
    if ( ModemNameLength > 0 ) {
        pPdData->oModemName = sizeof(PDMODEM_C2H);
        strcpy( (LPBYTE)pPdData + sizeof(PDMODEM_C2H), pPdModem->pModemName );
    }

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
    PPDMODEM pPdModem;

    TRACE(( TC_MODEM, TT_API1, "PdModem: DeviceConnect" ));

    /*
     *  Get pointer to modem pd's data
     */
    pPdModem = (PPDMODEM) pPd->pPrivate;

    /*
     *  Set STATE_NO_WAIT_REDIAL to force redial if already dialing
     */
    if ( pPd->fRecvStatusConnect ) {
        pPdModem->iCurrentState = STATE_NO_WAIT_REDIAL;
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceDisconnect
 *
 *  Disconnect from citrix application server
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
    PPDMODEM pPdModem;

    TRACE(( TC_MODEM, TT_API1, "PdModem: DeviceDisconnect" ));

    /*
     *  Get pointer to modem pd's data
     */
    pPdModem = (PPDMODEM) pPd->pPrivate;

    /*
     *  Hang up active connection
     */
    ModemHangup( pPd, pPdModem );

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
    pPd->fSendStatusConnect = TRUE;
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceWrite
 *
 *  write
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
    return( PdNext( pPd, PD__WRITE, pPdWrite ) );
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
    ULONG curTime;
    PPDMODEM pPdModem;
    int rc = CLIENT_STATUS_SUCCESS;
    PMODEMSTATUS pModemStatus;

    TRACE(( TC_MODEM, TT_API4, "PdModem: DeviceQuery" ));

    /*
     *  Get pointer to modem pd's data
     */
    pPdModem = (PPDMODEM) pPd->pPrivate;

    /*
     *  Switch on function
     */
    switch ( pPdQueryInformation->PdInformationClass ) {

        /*
         *  Get current modem status
         */
        case PdModemStatus :
            curTime = (ULONG) Getmsec();
            pModemStatus = (PMODEMSTATUS) pPdQueryInformation->pPdInformation;
            pModemStatus->TimeLeft    = pPdModem->cTimeLeft;
            break;

    }

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

    return( ModemCallback( pPd ) );
}
