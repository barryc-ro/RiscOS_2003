
/*************************************************************************
*
* pdreli.c
*
* Reliable Protocol Driver - error correction
*
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen (7/19/94)
*
* $Log$
*  
*     Rev 1.26   15 Apr 1997 16:52:46   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.26   21 Mar 1997 16:07:34   bradp
*  update
*  
*     Rev 1.25   27 Sep 1995 13:46:36   bradp
*  update
*  
*     Rev 1.24   26 Jul 1995 16:54:46   bradp
*  update
*  
*     Rev 1.23   17 Jul 1995 16:01:40   bradp
*  update
*  
*     Rev 1.22   10 Jun 1995 14:26:02   bradp
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
#include "citrix/icareli.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/pd.h"

#define NO_PDDEVICE_DEFINES
#include "../../../inc/pddevice.h"
#include "../../../inc/pddevicep.h"

#include "pdreli.h"

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



/*=============================================================================
==   External Functions Used
=============================================================================*/

int  STATIC WFCAPI DeviceOutBufAlloc( PPD, POUTBUF * );
void STATIC WFCAPI DeviceOutBufError( PPD, POUTBUF );
void STATIC WFCAPI DeviceOutBufFree( PPD, POUTBUF );
int  STATIC WFCAPI DeviceSetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );
int  STATIC WFCAPI DeviceQueryInfo( PPD, QUERYINFOCLASS, LPBYTE, USHORT );

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );

int PdNext( PPD, USHORT, PVOID );
int WriteData( PPD, PPDWRITE, BYTE );
int RetransmitTimer( PPD );
int NakTimer( PPD );
int AckTimer( PPD );
VOID CancelAllTimers( PPD );
int CheckSlowStart( PPD );
int OutBufAlloc( PPD, POUTBUF, POUTBUF * );
void OutBufError( PPD, POUTBUF );


/*=============================================================================
==   Local Data
=============================================================================*/


PLIBPROCEDURE PdReliDeviceProcedures[PDDEVICE__COUNT] =
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
 *  Define Reliable Protocol driver data structure
 *   (this could be dynamically allocated)
 */
PDRELI PdReliData = {0};


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
    PPDRELI pPdReli;

    TRACE(( TC_RELI, TT_API1, "PdReli: DeviceOpen" ));

    /*
     *  Save class of protocol driver
     */
    pPd->PdClass = PdReliable;

    /*
     *  Return size of header and parameters
     *  -- since this pd allocates a new outbuf and copies the data we don't
     *     need to reserve any space.
     */
    pPdOpen->OutBufHeader     = sizeof(ICAHEADER) + 1;
    pPdOpen->OutBufTrailer    = 0;
    pPdOpen->OutBufParam      = 0;
    pPdOpen->fOutBufCopy      = FALSE;
    pPdOpen->fOutBufFrame     = FALSE;

    /*
     *  Initialize PDRELI data structure
     */
    pPdReli = &PdReliData;
    pPd->pPrivate = pPdReli;
    memset( pPdReli, 0, sizeof(PDRELI) );

    /*
     *  Initialize private PD structure
     */
    pPdReli->RetransmitTimeout   = DEFAULT_RETRANSMIT_TIMEOUT;
    pPdReli->CongestionWindow    = DEFAULT_CONGESTION_WINDOW;
    pPdReli->SlowStartThreshold  = pPd->OutBufCountClient;
    pPdReli->LastAckSequence     = 0xff;

    pPdReli->MaxRetryTime      = bGetPrivateProfileLong( pPdOpen->pIniSection,
                                  INI_MAXRETRYTIME, DEF_MAXRETRYTIME );

    pPdReli->RetransmitTimeDelta = bGetPrivateProfileLong( pPdOpen->pIniSection,
                                  INI_RETRANSMITDELTA, DEFAULT_RETRANSMIT_DELTA );

    pPdReli->DelayedAckTime    = bGetPrivateProfileLong( pPdOpen->pIniSection,
                                  INI_DELAYEDACK, DEF_DELAYEDACK );

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
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  cancel retransmit, ack and nak timers
     */
    CancelAllTimers( pPd );

    pPd->pPrivate = NULL;

    TRACE(( TC_RELI, TT_API1, "PdReli: DeviceClose" ));
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
    PPDRELIABLE_C2H pPdData;
    PMODULE_C2H pHeader;
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(PDRELIABLE_C2H);

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
    pPdData = (PPDRELIABLE_C2H) pPdInfo->pBuffer;
    memset( pPdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_ProtocolDriver;
    pHeader->VersionL = VERSION_CLIENTL_PDRELIABLE;
    pHeader->VersionH = VERSION_CLIENTH_PDRELIABLE;

    /*
     *  Initialize protocol driver data
     */
    pPdData->Header.PdClass      = PdReliable;
    pPdData->MaxRetryTime        = pPdReli->MaxRetryTime;
    pPdData->RetransmitTimeDelta = pPdReli->RetransmitTimeDelta;
    pPdData->DelayedAckTime      = pPdReli->DelayedAckTime;

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

int
DeviceDisable( PPD pPd )
{
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Send ACK to host
     *  -- this will ack the PACKET_TERMINATE we just received
     */
    (void) AckTimer( pPd );

    /*
     *  Cancel all timers
     */
    CancelAllTimers( pPd );

    /*
     *  Reset Sequence numbers
     */
    pPdReli->ReceiveExpectedSeq = 0;
    pPdReli->TransmitSequence   = 0;
    pPdReli->LastAckSequence    = 0xff;
    pPdReli->LastNakSequence    = 0;

    /*
     *  Reset timeouts
     */
    pPdReli->RetransmitTimeout   = DEFAULT_RETRANSMIT_TIMEOUT;
    pPdReli->CongestionWindow    = DEFAULT_CONGESTION_WINDOW;
    pPdReli->SlowStartThreshold  = pPd->OutBufCountClient;

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
    PPDRELI pPdReli;
    ULONG CurrentTime;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Get current time
     */
    CurrentTime = pPdPoll->CurrentTime;

    /*
     *  Check retransmit timer
     */
    if ( pPdReli->TimerRetransmit && (CurrentTime >= pPdReli->TimerRetransmit) ) {
        pPdReli->TimerRetransmit = 0;
        if ( RetransmitTimer( pPd ) )
            pPdReli->TimerRetransmit = CurrentTime;
    }

    /*
     *  Check Nak timer
     */
    if ( pPdReli->TimerNak && (CurrentTime >= pPdReli->TimerNak) ) {
        pPdReli->TimerNak = 0;
        if ( NakTimer( pPd ) )
            pPdReli->TimerNak = CurrentTime;
    }

    /*
     *  Check Ack timer
     */
    if ( pPdReli->TimerAck && (CurrentTime >= pPdReli->TimerAck) ) {
        pPdReli->TimerAck = 0;
        if ( AckTimer( pPd ) )
            pPdReli->TimerAck = CurrentTime;
    }

    /*
     *  Check Error Feedback status
     */
    if ( pPdReli->StartRecvRetryTime || pPdReli->TotalRetransmitTime ) {
        if ( CurrentTime >= pPdReli->ErrorFeedbackTime ) {
            pPdReli->ErrorFeedbackTime = CurrentTime + 5000;
            return( CLIENT_STATUS_ERROR_RETRY );
        }
    }

    return( PdNext( pPd, DLL__POLL, pPdPoll ) );
}


/*******************************************************************************
 *
 *  DeviceWrite
 *
 *  write a reliable packet
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
    /*
     *  Append error correction header and write data
     */
    return( WriteData( pPd, pPdWrite, ICA_DATA ) );
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
    PPDRELI pPdReli;
    POUTBUF pOutBuf;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Free all outbufs that are waiting to be acked
     */
    while ( pOutBuf = pPdReli->pOutBufHead ) {
        pPdReli->pOutBufHead = pOutBuf->pLink;
        pOutBuf->pLink = NULL;
        OutBufError( pPd, pOutBuf );
    }
    pPdReli->OutBufAckWaitCount = 0;
    pPdReli->TimerRetransmit = 0;

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
    PTIMEOUTSTATUS pTimeoutStatus;
    PPDRELI pPdReli;

    pPdReli = (PPDRELI) pPd->pPrivate;

    /*
     *  Switch on function
     */
    switch ( pPdQueryInformation->PdInformationClass ) {

        /*
         *  Check if it's ok to allocate an output buffer
         */
        case PdOutBufReserve :
            return( CheckSlowStart( pPd ) );

        /*
         *  Current Retry Timeout (time left before connection timeout)
         */
        case PdTimeoutStatus :
            pTimeoutStatus = (PTIMEOUTSTATUS) pPdQueryInformation->pPdInformation;
            pTimeoutStatus->MaxRetryTime  = pPdReli->MaxRetryTime;
            pTimeoutStatus->SendRetryTime = pPdReli->TotalRetransmitTime;
            if ( pPdReli->StartRecvRetryTime > 0 ) {
                pTimeoutStatus->ReceiveRetryTime = Getmsec() - pPdReli->StartRecvRetryTime;
            } else {
                pTimeoutStatus->ReceiveRetryTime = 0;
            }

            return( CLIENT_STATUS_SUCCESS );
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
    return( CLIENT_STATUS_SUCCESS );
}
