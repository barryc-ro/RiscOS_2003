
/*************************************************************************
*
*   TDASYNC.C
*
*   Transport Driver - DOS Async Driver
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Kurt Perry (4/07/94)
*           Brad Pedersen (6/4/95)
*
*   $Log$
*  
*     Rev 1.17   15 Apr 1997 16:55:12   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.17   21 Mar 1997 16:07:50   bradp
*  update
*  
*     Rev 1.16   15 Mar 1996 17:33:30   KenB
*  move messages to nls\us\tdasynct.str
*  
*     Rev 1.15   19 Sep 1995 11:49:08   scottk
*  stop bits broken
*  
*     Rev 1.14   20 Jul 1995 12:08:14   bradp
*  update
*  
*     Rev 1.13   05 Jul 1995 19:32:10   bradp
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
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/td.h"

#define NO_TDDEVICE_DEFINES
#include "../../../inc/tddevice.h"
#include "../../../inc/tddevicep.h"

#include "tdasync.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int DeviceOpen( PPD, PPDOPEN );
static int DeviceClose( PPD, PDLLCLOSE );
static int DeviceInfo( PPD, PDLLINFO );
static int DeviceConnect( PPD );
static int DeviceDisconnect( PPD );
static int DeviceProcessInput( PPD );
static int DeviceWrite( PPD, POUTBUF, PUSHORT );
static int DeviceCheckWrite( PPD, POUTBUF );
static int DeviceCancelWrite( PPD, POUTBUF );
static int DeviceSendBreak( PPD );

PLIBPROCEDURE TdAsyncDeviceProcedures[TDDEVICE__COUNT] =
{
    (PLIBPROCEDURE)DeviceOpen,
    (PLIBPROCEDURE)DeviceClose,

    (PLIBPROCEDURE)DeviceInfo,

    (PLIBPROCEDURE)DeviceConnect,
    (PLIBPROCEDURE)DeviceDisconnect,

    (PLIBPROCEDURE)DeviceProcessInput,

    (PLIBPROCEDURE)DeviceWrite,
    (PLIBPROCEDURE)DeviceCheckWrite,
    (PLIBPROCEDURE)DeviceCancelWrite,

    (PLIBPROCEDURE)DeviceSendBreak
};

/*=============================================================================
==   Internal Functions
=============================================================================*/

static int SetLastError( PPD, int );
ULONG _htol( PCHAR );


/*=============================================================================
==   Defines
=============================================================================*/

#define DEVICE_HANDLE 1

/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define Async Protocol driver data structure
 *   (this could be dynamically allocated)
 */
//PDASYNC PdAsyncData = {0};

/*
 *   Async error messages
 */
static LPBYTE pProtocolName = "SERIAL";


/*******************************************************************************
 *
 *  DeviceOpen
 *
 *  initialize
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pPdOpen (input/output)
 *       pointer to td open parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceOpen( PPD pPd, PPDOPEN pPdOpen )
{
    PPDASYNC pPdAsync;
    BYTE Buf[10];
    BYTE * pBuf;
    DCBINFO * pDcb;
    LINECONTROL * pLc;
    PVOID pIni;

    TRACE((TC_TD,TT_API1, "DeviceOpen: in"));

    pPd->PdClass = PdAsync;

    /*
     *  Return size of header and parameters
     */
    pPdOpen->OutBufHeader  = 0;
    pPdOpen->OutBufTrailer = 0;
    pPdOpen->OutBufParam   = 0;

    /*
     *  Initialize async data structure
     */
    pPdAsync = malloc(sizeof(PDASYNC));
    pPd->pPrivate = pPdAsync;
    memset( pPdAsync, 0, sizeof(PDASYNC) );

    /*
     *  Allocate PD input buffer
     */
    pPdAsync->pInBuf = malloc( pPd->OutBufLength );
    if ( pPdAsync->pInBuf == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    /*
     *  Get pointers to data structures
     */
    pDcb = &pPdAsync->Dcb;
    pLc  = &pPdAsync->LineControl;
    pIni = pPdOpen->pIniSection;

    /*
     *  Get communication parameters
     */

    pPdAsync->PortNumber = bGetPrivateProfileInt( pIni, INI_PORTNUMBER, DEF_PORTNUMBER );
    pPdAsync->BaudRate   = bGetPrivateProfileLong(pIni, INI_BAUD, DEF_BAUD );

    /* get i/o address */
    pBuf = Buf;
    bGetPrivateProfileString( pIni, INI_IOADDR, INI_DEFAULT, Buf, sizeof(Buf) );
    if ( !stricmp( Buf, INI_DEFAULT ) ) {
        switch ( pPdAsync->PortNumber ) {
            case 2:  pBuf = "0x02F8"; break;
            case 3:  pBuf = "0x03E8"; break;
            case 4:  pBuf = "0x02E8"; break;
            default: pBuf = "0x03F8"; break;
        }
    }
    pPdAsync->ComIOAddr = (USHORT) _htol( pBuf );

    /* get interrupt level */
    pBuf = Buf;
    bGetPrivateProfileString( pIni, INI_IRQ, INI_DEFAULT, Buf, sizeof(Buf) );
    if ( !stricmp( Buf, INI_DEFAULT ) ) {
        switch ( pPdAsync->PortNumber ) {
            case 2:
            case 4:  pBuf = "3"; break;
            default: pBuf = "4"; break;
	}
    }
    pPdAsync->ComIRQ = (USHORT) atoi( pBuf );


    if ( bGetPrivateProfileBool( pIni, INI_DTR, DEF_DTR ) ) 
        pDcb->fbCtlHndShake |= MODE_DTR_CONTROL;

    if ( bGetPrivateProfileBool( pIni, INI_RTS, DEF_RTS ) ) 
        pDcb->fbFlowReplace |= MODE_RTS_CONTROL;

    pDcb->fbTimeout = MODE_WAIT_READ_TIMEOUT;

    pLc->bDataBits = (BYTE) bGetPrivateProfileInt(pIni, INI_DATA, DEF_DATA );

    bGetPrivateProfileString( pIni, INI_STOP, DEFSTR_STOP, Buf, sizeof(Buf));

    if ( !stricmp( Buf, "1" ) ) 
       pLc->bStopBits = DOS_STOP1;
    else if (!stricmp( Buf, "1.5" ) ) 
       pLc->bStopBits = DOS_STOP15;
    else if (!stricmp( Buf, "2" ) ) 
       pLc->bStopBits = DOS_STOP2;
    else 
       pLc->bStopBits = DOS_STOP1;

    bGetPrivateProfileString( pIni, INI_PARITY, DEF_PARITY, Buf, sizeof(Buf));
    if ( !stricmp( Buf, "ODD" ) ) 
        pLc->bParity = 1;
    else if ( !stricmp( Buf, "EVEN" ) ) 
        pLc->bParity = 2;
    else if ( !stricmp( Buf, "MARK" ) ) 
        pLc->bParity = 3;
    else if ( !stricmp( Buf, "SPACE" ) ) 
        pLc->bParity = 4;


    if ( bGetPrivateProfileBool( pIni, INI_SW, DEF_SW ) ) {
        pDcb->fbFlowReplace |= (MODE_AUTO_TRANSMIT | MODE_AUTO_RECEIVE);
        pDcb->bXONChar  = (BYTE) bGetPrivateProfileInt(pIni, INI_XON, DEF_XON);
        pDcb->bXOFFChar = (BYTE) bGetPrivateProfileInt(pIni, INI_XOFF,DEF_XOFF);
    }

    if ( bGetPrivateProfileBool( pIni, INI_HW, DEF_HW ) ) {

        bGetPrivateProfileString( pIni, INI_HW_RX, DEF_HW_RX, Buf, sizeof(Buf));
        if ( !stricmp( Buf, "DTR" ) ) {
            pDcb->fbCtlHndShake &= ~MODE_DTR_CONTROL;
            pDcb->fbCtlHndShake |= MODE_DTR_HANDSHAKE;
	} else if ( !stricmp( Buf, "RTS" ) ) {
            pDcb->fbFlowReplace &= ~(MODE_RTS_CONTROL);
            pDcb->fbFlowReplace |= MODE_RTS_HANDSHAKE;
	}

        bGetPrivateProfileString( pIni, INI_HW_TX, DEF_HW_TX, Buf, sizeof(Buf));
        if ( !stricmp( Buf, "CTS" ) ) {
            pDcb->fbCtlHndShake |= MODE_CTS_HANDSHAKE;
	} else if ( !stricmp( Buf, "DSR" ) ) {
            pDcb->fbCtlHndShake |= MODE_DSR_HANDSHAKE;
	}
    }

    TRACE((TC_TD,TT_API1, "DeviceOpen: port %u, baud %lu, i/o 0x%x, int %u",
       pPdAsync->PortNumber, pPdAsync->BaudRate, pPdAsync->ComIOAddr, pPdAsync->ComIRQ ));
    TRACE((TC_TD,TT_API1, "DeviceOpen: hndshake 0x%x, flowreplace 0x%x, data %u, stop %u",
       pDcb->fbCtlHndShake, pDcb->fbFlowReplace, pLc->bDataBits, pLc->bStopBits ));
    TRACE((TC_TD,TT_API1, "DeviceOpen: parity %u, xon 0x%x, xoff 0x%x",
        pLc->bParity, pDcb->bXONChar, pDcb->bXOFFChar ));

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceClose
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pPdClose (input/output)
 *       pointer to td close parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceClose( PPD pPd, PDLLCLOSE pPdClose )
{
    PPDASYNC pPdAsync;

    /*
     *  Get pointer to async td
     */
    pPdAsync = (PPDASYNC) pPd->pPrivate;

    if (pPdAsync)
    {
	free( pPdAsync->pInBuf );
	pPd->pPrivate = NULL;

	free(pPdAsync);
    }
    
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
    PTD_C2H pTdData;
    PMODULE_C2H pHeader;
    PPDASYNC pPdAsync;

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdInfo->pBuffer;
    pHeader->VersionL = VERSION_CLIENTL_TDASYNC;
    pHeader->VersionH = VERSION_CLIENTH_TDASYNC;

    /*
     *  Initialize client address (com port number)
     */
    pTdData = (PTD_C2H) pPdInfo->pBuffer;
    pPdAsync = (PPDASYNC) pPd->pPrivate;
    sprintf( pTdData->ClientAddress, "COM%u", pPdAsync->PortNumber );

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
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceConnect( PPD pPd )
{
    PPDASYNC pPdAsync;

    /*
     *  Get pointer to async td
     */
    pPdAsync = (PPDASYNC) pPd->pPrivate;

    /*
     *  Open com port
     */
    if ( AsyncOpen( DEVICE_HANDLE, pPdAsync->ComIRQ, pPdAsync->ComIOAddr, FALSE ) ) {
        return( SetLastError( pPd, TDASYNC_OPEN_FAILED ) );
    }

    /*
     *  set DCB info
     */
    if ( AsyncSetDCB( DEVICE_HANDLE, &pPdAsync->Dcb  ) ) 
        goto badset;

    /*
     *  Set the baud rate
     */
    TRACE((TC_TD,TT_API1, "Baud Rate: %lu", pPdAsync->BaudRate ));
    if ( AsyncSetBaud( DEVICE_HANDLE, pPdAsync->BaudRate ) ) 
        goto badset;

    /*
     *  set the line ctrl
     */
    if ( AsyncSetLineCtrl( DEVICE_HANDLE, &pPdAsync->LineControl ) ) 
        goto badset;

    return( CLIENT_STATUS_SUCCESS );

 /*=============================================================================
 ==   Error returns
 =============================================================================*/

    /*
     *  parameter set failed
     */
badset:
    return( SetLastError( pPd, TDASYNC_BAD_PARAM ) );
}


/*******************************************************************************
 *
 *  DeviceDisconnect
 *
 *  disconnect from citrix application server
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
DeviceDisconnect( PPD pPd )
{
    /*
     *  Hangup connection by dropping DTR and closing com port
     */
    (void) AsyncSetModemCtrl( DEVICE_HANDLE, DTR_OFF );
    (void) AsyncClose( DEVICE_HANDLE );

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 *  read data from serial line
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
DeviceProcessInput( PPD pPd )
{
    PPDASYNC pPdAsync;
    USHORT BytesAvail;
    int rc;

    /*
     *  Get pointer to async td
     */
    pPdAsync = (PPDASYNC) pPd->pPrivate;

    /*
     *  Read at device
     */
    rc = AsyncRead( DEVICE_HANDLE,
                    pPdAsync->pInBuf,
                    pPd->OutBufLength,
                    &BytesAvail );

    /*
     *  Nothing received
     */
    if ( BytesAvail == 0 )
       return( CLIENT_STATUS_NO_DATA );

    /*
     *  Check for error
     */
    if ( rc )
        return( SetLastError( pPd, TDASYNC_READ_FAILED ) );

    /*
     *  Process Data
     */
    LogPrintf( LOG_CLASS, LOG_RECEIVE, "RECEIVE: %u bytes", BytesAvail );
    LogBuffer( LOG_CLASS, LOG_RECEIVE, pPdAsync->pInBuf, (ULONG)BytesAvail );

    if ( pPd->pReadHook )
        (*pPd->pReadHook)( pPdAsync->pInBuf, BytesAvail );
    if ( pPd->pProcessInputProc )
         rc = (*pPd->pProcessInputProc)( pPd->pWdData, pPdAsync->pInBuf, BytesAvail );

    ASSERT( rc == CLIENT_STATUS_SUCCESS | rc == CLIENT_STATUS_NO_DATA, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  DeviceWrite
 *
 *  write data to serial line
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *    pBytesWritten (output)
 *       address to return number of bytes actually written
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceWrite( PPD pPd, POUTBUF pOutBuf, PUSHORT pBytesWritten )
{
    int rc;

    /*
     *  Write data to the uart
     */
    rc = AsyncWrite( DEVICE_HANDLE, 
                     pOutBuf->pBuffer,
                     pOutBuf->ByteCount, 
                     pBytesWritten );

    TRACE(( TC_TD, TT_API1, "AsyncWrite: bc %u, rc=0x%x", *pBytesWritten, rc ));

    /*
     *  Check for error
     */
    if ( rc ) {
        *pBytesWritten = 0;
        return( SetLastError( pPd, TDASYNC_WRITE_FAILED ) );
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceCheckWrite
 *
 *  check if previous write is complete
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceCheckWrite( PPD pPd, POUTBUF pOutBuf )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceCancelWrite
 *
 *  flush output - bit bucket all pending output data
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceCancelWrite( PPD pPd, POUTBUF pOutBuf )
{
    (void) AsyncWriteFlush( DEVICE_HANDLE );
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceSendBreak  (async only)
 *
 *  transmit break signal
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
DeviceSendBreak( PPD pPd )
{
    AsyncSendBreak(DEVICE_HANDLE, 250);
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  SetLastError
 *
 *  save async error code
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    Error (input)
 *       netbios error code
 *
 * EXIT:
 *     NT error code
 *
 *
 ******************************************************************************/

static int
SetLastError( PPD pPd, int Error )
{
    if ( Error == 0 )
        return( CLIENT_STATUS_SUCCESS );

    TRACE(( TC_TD, TT_ERROR, "SetLastError: 0x%x", Error ));
    ASSERT( FALSE, Error );

    if ( pPd->LastError == 0 )
        pPd->LastError = Error;

    return( CLIENT_ERROR_PD_ERROR );
}


/*******************************************************************************
 *
 *  _htol
 *
 *  convert hex string to long integer
 *
 * ENTRY:
 *    s (input)
 *       Pointer to hex string
 *
 * EXIT:
 *     long integer
 *
 ******************************************************************************/

ULONG
_htol( PCHAR s )
{
    CHAR c;
    PCHAR p;
    ULONG val = 0L;

    // look for '0x' prefix
    if( (p = strstr( s, "0x" )) != NULL || (p = strstr( s, "0X" )) != NULL ) {
        s = (p + 2);     // remove '0x' prefix
    }

    // look for 'h' suffix
    else if ( (p = strchr( s, 'h' )) != NULL || (p = strchr ( s, 'H' )) != NULL ) {
        *p = 0;     // remove 'h' suffix
    }

    // just plain decimal number
    else {
        return( atol( s ) );
    }

    // do the hex conversion
    while(((c = (CHAR)toupper(*s++)) >= '0' && c <= '9') ||
           (c >='A' && c <= 'F')) {
        val = (val << 4)
            + ((c >= '0' && c <= '9') ? (ULONG)(c - '0') :
                                        (ULONG)(10 + (c - 'A')));
    }

    return(val);
}
