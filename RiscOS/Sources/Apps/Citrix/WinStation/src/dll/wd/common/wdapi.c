
/*************************************************************************
*
*   wdapi.c
*
*   WinStation driver external routines
*
*   Copyright Citrix Systems Inc. 1994
*
*   Author: Brad Pedersen  (3/25/94)
*
*   wdapi.c,v
*   Revision 1.1  1998/01/12 11:36:13  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.53   15 Apr 1997 18:17:18   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.53   21 Mar 1997 16:09:46   bradp
*  update
*  
*     Rev 1.52   20 Feb 1997 14:08:02   butchd
*  update
*  
*     Rev 1.51   07 Feb 1997 17:47:26   kurtp
*  update
*  
*     Rev 1.50   29 Jan 1997 21:05:14   jeffm
*  fixed up free of INFOBLOCK data
*  
*     Rev 1.49   22 Jan 1997 16:48:06   terryt
*  client data
*  
*     Rev 1.48   13 Nov 1996 09:04:50   richa
*  Updated Virtual channel code.
*  
*     Rev 1.47   08 May 1996 15:03:16   jeffm
*  update
*  
*     Rev 1.46   12 Feb 1996 09:36:00   richa
*  added a WriteTTY (\r\n) so that the terminal window is brought up sooner.
*
*     Rev 1.45   29 Jan 1996 20:10:46   bradp
*  update
*
*     Rev 1.44   28 Sep 1995 13:59:10   bradp
*  update
*
*     Rev 1.43   21 Jul 1995 18:28:06   bradp
*  update
*
*     Rev 1.42   10 Jul 1995 21:37:02   jeffk
*  part of fix for 100% cpu
*
*     Rev 1.41   29 Jun 1995 13:57:12   bradp
*  update
*
*     Rev 1.40   28 Jun 1995 22:14:46   bradp
*  update
*
*     Rev 1.39   01 Jun 1995 22:29:24   terryt
*  Make encrypted clients work with SouthBeach
*
*     Rev 1.38   23 May 1995 18:59:06   terryt
*  Encryption
*
*     Rev 1.37   03 May 1995 11:46:32   kurtp
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
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"
#include "citrix/ica-h2c.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/xmsapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/wd.h"

#include "../../../inc/wdemul.h"
#include "../../../inc/wdemulp.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI WdLoad( PDLLLINK );
int WFCAPI WdUnload( PWD, PDLLLINK );
int WFCAPI WdOpen( PWD, PWDOPEN );
int WFCAPI WdClose( PWD, PDLLCLOSE );
int WFCAPI WdInfo( PWD, PDLLINFO );
int WFCAPI WdPoll( PWD, PDLLPOLL );
int WFCAPI WdQueryInformation( PWD, PWDQUERYINFORMATION );
int WFCAPI WdSetInformation( PWD, PWDSETINFORMATION );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

int  PdCall( PWD, USHORT, PVOID );
BOOL ReservedVirtualChannel( POPENVIRTUALCHANNEL pOVC );
VOID GetFirstAvailableChannel( POPENVIRTUALCHANNEL pOVC );
BOOL HandleIsNotReserved( USHORT Channel );


/*=============================================================================
==   Functions used
=============================================================================*/

int  OutBufPoolAlloc( PWD, USHORT, USHORT );
void OutBufPoolFree( PWD );

#if 0
int EmulOpen( PWD, PWDOPEN );
int EmulClose( PWD );
int EmulInfo( PWD, PDLLINFO );
int EmulPoll( PWD, PDLLPOLL, int );
int EmulSetInformation( PWD, PWDSETINFORMATION );

int  WFCAPI EmulOutBufAlloc( PWD, POUTBUF * );
void WFCAPI EmulOutBufFree( PWD, POUTBUF );
void WFCAPI EmulOutBufError( PWD, POUTBUF );
int  WFCAPI EmulProcessInput( PWD, LPBYTE, USHORT );
int  WFCAPI EmulSetInfo( PWD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI EmulQueryInfo( PWD, QUERYINFOCLASS, LPBYTE, USHORT );
void WriteTTY( LPVOID pIca, LPBYTE pTTYData, USHORT cbTTYData );
#endif

/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Array of virtual channels built during init.
 * This array isn't packed, when it is sent to the server it is though.
 */
PWD_VCBIND  paWD_VcBind = NULL;

USHORT iVc_Map = 0;
typedef struct _VC_MAP {
    VIRTUALNAME VirtualName;
    USHORT      Channel;
} VC_MAP, * PVC_MAP;

VC_MAP VcCompatablityMap[]={

    VIRTUAL_LPT1     , Virtual_LPT1     ,
    VIRTUAL_LPT2     , Virtual_LPT2     ,
    VIRTUAL_COM1     , Virtual_COM1     ,
    VIRTUAL_COM2     , Virtual_COM2     ,
    VIRTUAL_CPM      , Virtual_Cpm      ,
    VIRTUAL_CCM      , Virtual_Ccm      ,
    VIRTUAL_CDM      , Virtual_Cdm      ,
    VIRTUAL_CLIPBOARD, Virtual_Clipboard,
    VIRTUAL_THINWIRE , Virtual_ThinWire ,
    ""               , 0                ,
};

/*
 *  Define WinStation driver external procedures
 */
PDLLPROCEDURE WdProcedures[ WD__COUNT ] = {
    (PDLLPROCEDURE) WdLoad,
    (PDLLPROCEDURE) WdUnload,
    (PDLLPROCEDURE) WdOpen,
    (PDLLPROCEDURE) WdClose,
    (PDLLPROCEDURE) WdInfo,
    (PDLLPROCEDURE) WdPoll,
    (PDLLPROCEDURE) WdQueryInformation,
    (PDLLPROCEDURE) WdSetInformation,
};

/*
 *  Define WinStation driver data structure
 *   (this could be dynamically allocated)
 */
// WD WdData = {0};


/*=============================================================================
==   Global Data
=============================================================================*/

#if 0
STATIC PPLIBPROCEDURE pModuleProcedures = NULL;
STATIC PPLIBPROCEDURE pClibProcedures = NULL;
STATIC PPLIBPROCEDURE pVioProcedures = NULL;
STATIC PPLIBPROCEDURE pKbdProcedures = NULL;
STATIC PPLIBPROCEDURE pMouProcedures = NULL;
STATIC PPLIBPROCEDURE pTimerProcedures = NULL;
STATIC PPLIBPROCEDURE pLptProcedures = NULL;
STATIC PPLIBPROCEDURE pXmsProcedures = NULL;
STATIC PPLIBPROCEDURE pLogProcedures = NULL;
STATIC PPLIBPROCEDURE pBIniProcedures = NULL;
#endif

/*******************************************************************************
 *
 *  Load
 *
 *    The dll init code calls Load to load and link a winstation driver.
 *    All Pds must be loaded and opened before a Wd can be loaded.  Only one
 *    Wd can be loaded at a time.  It is possible to unload and load a new
 *    Wd without dropping an application server connection.
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
WdLoad( PDLLLINK pLink )
{
    PWD pWd = malloc(sizeof(WD));
    
    pLink->ProcCount   = WD__COUNT;
    pLink->pProcedures = WdProcedures;
    pLink->pData       = pWd;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  WdUnload
 *
 *  DllUnload calls WdUnload to unlink and unload a Wd.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdUnload( PWD pWd, PDLLLINK pLink )
{
    free(pWd);
    
    pLink->ProcCount   = 0;
    pLink->pProcedures = 0;
    pLink->pData       = NULL;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  WdOpen
 *
 *  The user interface calls WdOpen to open and initialize a Wd.
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

int WFCAPI
WdOpen( PWD pWd, PWDOPEN pWdOpen )
{
    int rc;
    PDQUERYINFORMATION PdQueryInfo;
    PDSETINFORMATION PdSetInfo;
    WDADDRESS WdAddress;
    BUFFERINFO BufferInfo;
    int OutBufCount;
    int OutBufLength;

#if 0
    /*
     *  Initialize global data
     */
    pModuleProcedures     = pWdOpen->pModuleProcedures;
    pClibProcedures       = pWdOpen->pClibProcedures;
    pVioProcedures        = pWdOpen->pVioProcedures;
    pKbdProcedures        = pWdOpen->pKbdProcedures;
    pMouProcedures        = pWdOpen->pMouProcedures;
    pTimerProcedures      = pWdOpen->pTimerProcedures;
    pLptProcedures        = pWdOpen->pLptProcedures;
    pXmsProcedures        = pWdOpen->pXmsProcedures;
    pLogProcedures        = pWdOpen->pLogProcedures;
    pBIniProcedures       = pWdOpen->pBIniProcedures;
#endif
    
    /*
     *  Initialize WD data structure
     */
    memset( pWd, 0, sizeof(WD) );
    pWd->pEmulProcedures   = pWdOpen->pEmulProcedures;
    

    pWd->pPdLink           = pWdOpen->pPdLink;
    pWd->pScriptLink       = pWdOpen->pScriptLink;
    pWd->OutBufHeader      = pWdOpen->OutBufHeader;
    pWd->OutBufTrailer     = pWdOpen->OutBufTrailer;
    pWd->OutBufParam       = pWdOpen->OutBufParam;
    pWd->fOutBufCopy       = pWdOpen->fOutBufCopy;
    pWd->fOutBufFrame      = pWdOpen->fOutBufFrame;
    pWd->fAsync            = pWdOpen->fAsync;

    pWd->XmsReserve = bGetPrivateProfileLong( pWdOpen->pIniSection,
                                              INI_XMSRESERVE,
                                              (long) DEF_XMSRESERVE );

    pWd->LowMemReserve = bGetPrivateProfileLong( pWdOpen->pIniSection,
                                                 INI_LOWMEMRESERVE,
                                                 (long) DEF_LOWMEMRESERVE );

    pWd->InputBufferLength = bGetPrivateProfileInt( pWdOpen->pIniSection,
                                                    INI_BUFFERLENGTH,
                                                    DEF_BUFFERLENGTH );

    /*
     *  Query size of input and output buffers from PD
     */
    PdQueryInfo.PdInformationClass  = PdBufferInfo;
    PdQueryInfo.pPdInformation      = &BufferInfo;
    PdQueryInfo.PdInformationLength = sizeof(BUFFERINFO);
    if ( rc = PdCall( pWd, PD__QUERYINFORMATION, &PdQueryInfo ) )
        goto badquery;

    OutBufLength         = BufferInfo.OutBufLength;
    OutBufCount          = BufferInfo.OutBufCountClient;
    pWd->OutBufCountHost = BufferInfo.OutBufCountHost;

    TRACE(( TC_WD, TT_API3,
            "WdOpen: xms %lu, low %lu, ica %u, len %u, host %u, client %u",
            pWd->XmsReserve, pWd->LowMemReserve, pWd->InputBufferLength,
            OutBufLength, pWd->OutBufCountHost, OutBufCount ));

    /*
     *  Allocate WD output buffer pool
     */
    pWd->OutBufCountClient = 0;
    if (rc = OutBufPoolAlloc(pWd, (USHORT) OutBufCount, (USHORT) OutBufLength))
        goto badoutbuf;

    /*
     *  Open winstation (init terminal dependant part of wd)
     */
    if ( rc = EmulOpen( pWd, pWdOpen ) )
        goto badopen;

    /*
     *  Allocate WD input buffer
     */
    pWd->pInputBuffer = (LPBYTE) malloc( pWd->InputBufferLength );
    if ( pWd->pInputBuffer == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto badinputbuffer;
    }

    /*
     *  Register address of OutBuf routines with PD
     */
    WdAddress.pWdData             = pWd;
#if RISCOS
    WdAddress.pOutBufAllocProc    = (POUTBUFALLOCPROC)   pWd->pEmulProcedures[WDEMUL__OUTBUFALLOC];
    WdAddress.pOutBufErrorProc    = (POUTBUFFREEPROC)    pWd->pEmulProcedures[WDEMUL__OUTBUFERROR];
    WdAddress.pOutBufFreeProc     = (POUTBUFFREEPROC)    pWd->pEmulProcedures[WDEMUL__OUTBUFFREE];
    WdAddress.pProcessInputProc   = (PPROCESSINPUTPROC)  pWd->pEmulProcedures[WDEMUL__PROCESSINPUT];
    WdAddress.pSetInfoProc        = (PSETINFOPROC)       pWd->pEmulProcedures[WDEMUL__SETINFO];
    WdAddress.pQueryInfoProc      = (PQUERYINFOPROC)     pWd->pEmulProcedures[WDEMUL__QUERYINFO];
#else
    WdAddress.pOutBufAllocProc    = (POUTBUFALLOCPROC)   EmulOutBufAlloc;
    WdAddress.pOutBufErrorProc    = (POUTBUFFREEPROC)    EmulOutBufError;
    WdAddress.pOutBufFreeProc     = (POUTBUFFREEPROC)    EmulOutBufFree;
    WdAddress.pProcessInputProc   = (PPROCESSINPUTPROC)  EmulProcessInput;
    WdAddress.pSetInfoProc        = (PSETINFOPROC)       EmulSetInfo;
    WdAddress.pQueryInfoProc      = (PQUERYINFOPROC)     EmulQueryInfo;
#endif
    PdSetInfo.PdInformationClass  = PdWdAddress;
    PdSetInfo.pPdInformation      = &WdAddress;
    PdSetInfo.PdInformationLength = sizeof(WDADDRESS);

    if ( rc = PdCall( pWd, PD__SETINFORMATION, &PdSetInfo ) )
        goto badregister;

    TRACE(( TC_WD, TT_API1, "WdOpen: success" ));

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad register
     */
badregister:
    free( pWd->pInputBuffer );

    /*
     *  bad input buffer alloc
     */
badinputbuffer:
    OutBufPoolFree( pWd );

    /*
     *  bad output buffer alloc
     *  bad open
     *  buffer query failed
     */
badoutbuf:
badopen:
badquery:
    TRACE(( TC_WD, TT_API1, "WdOpen: rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  WdClose
 *
 *  The user interface calls WdClose to close a Wd before unloading it.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdClose( PWD pWd, PDLLCLOSE pWdClose )
{
    PDSETINFORMATION PdSetInfo;
    WDADDRESS WdAddress;
    PINFOBLOCK p, pNext;

    TRACE(( TC_WD, TT_API1, "WdClose (enter)" ));

    /*
     *  Cancel all write operations  (free all outbufs)
     */
    PdSetInfo.PdInformationClass  = PdCancelWrite;
    (VOID) PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );

    /*
     *  Unregister address of OutBuf routines
     */
    memset( &WdAddress, 0, sizeof(WDADDRESS) );
    PdSetInfo.PdInformationClass  = PdWdAddress;
    PdSetInfo.pPdInformation      = &WdAddress;
    PdSetInfo.PdInformationLength = sizeof(WDADDRESS);
    (VOID) PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );

    /*
     *  Close device
     */
    (VOID) EmulClose( pWd );

    /*
     *  Free WD data structures
     */
    if ( pWd->pAppServer )
    {
        free( pWd->pAppServer );
    }
 
    p = pWd->pInfoBlockList;

    while ( p != NULL) {

        pNext = p->pNext;

        if ( p->pData )
            free( p->pData );
        free( p );

        p = pNext;
    }

    if ( paWD_VcBind != NULL ) {
        free( paWD_VcBind );
	paWD_VcBind = NULL;
    }

    iVc_Map = 0;
    
    OutBufPoolFree( pWd );
    free( pWd->pInputBuffer );

    TRACE(( TC_WD, TT_API1, "WdClose (exit)" ));

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  WdInfo
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdInfo( PWD pWd, PDLLINFO pWdInfo )
{

    return( EmulInfo( pWd, pWdInfo ) );
}


/*******************************************************************************
 *
 *  WdPoll
 *
 *  The user interface calls WdPoll to get status and to give control to
 *  winstation, virtual and protocol drivers.  This function never blocks
 *  and always returns the current status of the connection.  Winstation,
 *  virtual and protocol drivers only get to execute when this routine is
 *  called. Therefore it is necessary that this routine gets called frequently.
 *
 *      call PdPoll and process return status
 *      call VdPoll and process return status
 *      call KeyboardReadScan or KeyboardReadChar and process data
 *      call MouseRead and process data
 *
 *  This function returns the current status of the connection. The user
 *  interface uses the current status to display status and error panels.
 *  The status returned is divided into the following categories:
 *
 *      establishing connection
 *      modem status
 *      callback
 *      connected
 *      terminating connection
 *      disconnected
 *      protocol errors
 *      script status
 *      hotkeys
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdPoll( PWD pWd, PDLLPOLL pWdPoll )
{
    int rc1,rc2;

    /*
     *  If WinStation Driver is connected and we received a connect
     *  status from pd below us, send status to the winframe engine
     */
    if ( pWd->fSendStatusConnect && pWd->fRecvStatusConnect && !pWd->fConnected ) {
        TRACE(( TC_WD, TT_API1, "WdPoll: Send CLIENT_STATUS_CONNECTED" ));
        pWd->fConnected = TRUE;
        pWd->fSendStatusConnect = FALSE;
        return( CLIENT_STATUS_CONNECTED );
    }

    /*
     *  Process input data
     */
    if ( rc1 = PdCall( pWd, DLL__POLL, pWdPoll ) ) {

        if ( rc1 != CLIENT_STATUS_NO_DATA ) {

            /* check for tty connection */
            if ( rc1 == CLIENT_STATUS_TTY_CONNECTED ) {
                TRACE(( TC_WD, TT_API1, "WdPoll: Recv/Send CLIENT_STATUS_TTY_CONNECTED" ));
                if (pWd->fAsync) {
                    EmulWriteTTY( pWd, "\r\n", 2 );
                }
                pWd->fTTYConnected = TRUE;
            }

            /* check for winframe server connection */
            if ( rc1 == CLIENT_STATUS_CONNECTED ) {
                TRACE(( TC_WD, TT_API1, "WdPoll: Recv CLIENT_STATUS_CONNECTED" ));
                pWd->fRecvStatusConnect = TRUE;
                rc1 = CLIENT_STATUS_SUCCESS;
            }
        }
    }

    /*
     *  Process any Wd Info
     */
    if ( rc2 = EmulPoll( pWd, pWdPoll, rc1 ) ) {
        if ( rc2 != CLIENT_STATUS_NO_DATA )
            return( rc2 );
    }

    if ( (rc1 == CLIENT_STATUS_NO_DATA) && (rc2 == CLIENT_STATUS_NO_DATA) )
        return( CLIENT_STATUS_NO_DATA );
    else if ( rc1 != CLIENT_STATUS_NO_DATA )
        return( rc1 );
    else
        return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  WdQueryInformation
 *
 *   Queries information about the winstation driver or a protocol driver.
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdQueryInformation (input/output)
 *       pointer to the structure WDQUERYINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
WdQueryInformation( PWD pWd, PWDQUERYINFORMATION pWdQueryInformation )
{
    PDQUERYINFORMATION QueryInfo;
    PLOADBALANCE pLoadBalance;
    PENCRYPTIONINIT pEncryptionInit;
    PINFOBLOCK pBlock;
    int rc = CLIENT_STATUS_SUCCESS;

    QueryInfo.pPdInformation      = pWdQueryInformation->pWdInformation;
    QueryInfo.PdInformationLength = pWdQueryInformation->WdInformationLength;

    switch ( pWdQueryInformation->WdInformationClass ) {
        
        case WdIOStatus :
             QueryInfo.PdInformationClass = PdIOStatus;
             break;

        case WdLastError :
            QueryInfo.PdInformationClass = PdLastError;
            break;

        case WdClientData :
            pWdQueryInformation->WdReturnLength = 0;
            return( CLIENT_STATUS_SUCCESS );

        case WdStatistics :
            pWdQueryInformation->WdReturnLength = 0;
            return( CLIENT_STATUS_SUCCESS );

        case WdModemStatus :
            QueryInfo.PdInformationClass = PdModemStatus;
            break;

        case WdLoadBalance :
            if ( pWd->pAppServer ) {
                pLoadBalance = (PLOADBALANCE) pWdQueryInformation->pWdInformation;
                strncpy( pLoadBalance->AppServer, pWd->pAppServer, sizeof(LOADBALANCE) );
                pWdQueryInformation->WdReturnLength = sizeof(LOADBALANCE);
            } else {
                pWdQueryInformation->WdReturnLength = 0;
            }
            return( CLIENT_STATUS_SUCCESS );

        case WdEncryptionInit :
            pEncryptionInit = (PENCRYPTIONINIT) pWdQueryInformation->pWdInformation;
            pEncryptionInit->EncryptionLevel = pWd->EncryptionLevel;
            pWdQueryInformation->WdReturnLength = sizeof(ENCRYPTIONINIT);
            return( CLIENT_STATUS_SUCCESS );

            /*
             *  Open a virtual channel
             */
        case WdOpenVirtualChannel :

            if ( paWD_VcBind == NULL ) {
                paWD_VcBind = (PWD_VCBIND)malloc (VIRTUAL_MAXIMUM*sizeof(WD_VCBIND));
            }
            if ( !ReservedVirtualChannel( (POPENVIRTUALCHANNEL)pWdQueryInformation->pWdInformation )) {
                GetFirstAvailableChannel( (POPENVIRTUALCHANNEL)pWdQueryInformation->pWdInformation );
            }
            pWdQueryInformation->WdReturnLength = sizeof( OPENVIRTUALCHANNEL );
            return( CLIENT_STATUS_SUCCESS );

        case WdGetInfoData :
            /*
             * The ID must be in the front of the buffer, so the buffer must be at least
             * that big.
             */
            if ( QueryInfo.PdInformationLength < 8 ) {
                pWdQueryInformation->WdReturnLength = 8;
                return( CLIENT_ERROR_BUFFER_TOO_SMALL );
            }

            /*
             * Search for the ID and copy it.  If the buffer is too small, return the correct
             * length with an error.
             */
            for ( pBlock = pWd->pInfoBlockList; pBlock != NULL; pBlock = pBlock->pNext ) {
                if ( !memcmp( pBlock->Id, QueryInfo.pPdInformation, 8 ) ) {
                    pWdQueryInformation->WdReturnLength = (USHORT)(8 + pBlock->Length);
                    if ( (ULONG)( QueryInfo.PdInformationLength - 8 ) < pBlock->Length )  {
                        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
                    }
                    memcpy( (LPBYTE)QueryInfo.pPdInformation + 8, pBlock->pData, pBlock->Length );
                    return( CLIENT_STATUS_SUCCESS );
                }
            }

            /*
             * If the data was not found, return an error.
             */
            pWdQueryInformation->WdReturnLength = 8;
            return( CLIENT_ERROR );
            break;
    }

    rc = PdCall( pWd, PD__QUERYINFORMATION, &QueryInfo );
    pWdQueryInformation->WdReturnLength = QueryInfo.PdReturnLength;

    TRACE(( TC_WD, TT_API1, "WdQueryInformation(%u): rc=0x%x",
            pWdQueryInformation->WdInformationClass, rc ));

    return( rc );
}


/*******************************************************************************
 *
 *  WdSetInformation
 *
 *   Sets information for a winstation driver or a protocol driver.
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

int WFCAPI
WdSetInformation( PWD pWd, PWDSETINFORMATION pWdSetInformation )
{
    PDSETINFORMATION SetInfo;
    int rc;

    SetInfo.PdInformationClass  = PdNop;
    SetInfo.pPdInformation      = pWdSetInformation->pWdInformation;
    SetInfo.PdInformationLength = pWdSetInformation->WdInformationLength;

    switch ( pWdSetInformation->WdInformationClass ) {

        case WdConnect :
            SetInfo.PdInformationClass = PdConnect;
            break;

        case WdDisconnect :
            SetInfo.PdInformationClass = PdDisconnect;
            break;

        case WdKillFocus :
            pWd->fFocus = FALSE;
            SetInfo.PdInformationClass = PdKillFocus;
            break;

        case WdSetFocus :
            pWd->fFocus = TRUE;
            SetInfo.PdInformationClass = PdSetFocus;
            break;

        case WdEnablePassThrough :
            SetInfo.PdInformationClass = PdEnablePassThrough;
            break;

        case WdDisablePassThrough :
            SetInfo.PdInformationClass = PdDisablePassThrough;
            break;

        case WdAddReadHook :
            SetInfo.PdInformationClass = PdAddReadHook;
            break;

        case WdRemoveReadHook :
            SetInfo.PdInformationClass = PdRemoveReadHook;
            break;

        case WdAddWriteHook :
            SetInfo.PdInformationClass = PdAddWriteHook;
            break;

        case WdRemoveWriteHook :
            SetInfo.PdInformationClass = PdRemoveWriteHook;
            break;
    }

    /*
     *  Send to winstation driver
     */
    if ( rc = EmulSetInformation( pWd, pWdSetInformation ) )
        goto done;

    /*
     *  Send to protocol driver
     */
    if ( SetInfo.PdInformationClass != PdNop )
        rc = PdCall( pWd, PD__SETINFORMATION, &SetInfo );

done:
    DTRACE(( TC_WD, TT_API1, "WdSetInformation(%u): rc=0x%x",
            pWdSetInformation->WdInformationClass, rc ));

    return( rc );
}


/*******************************************************************************
 *
 *  PdCall
 *
 *    PdCall is used to call the protocol driver
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    ProcIndex (input)
 *       index of dll function to call
 *    pParam (input/output)
 *       pointer to parameter for function
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
PdCall( PWD pWd, USHORT ProcIndex, PVOID pParam )
{
    PDLLLINK pLink;
    PDLLPROCEDURE pProcedure;

    pLink = pWd->pPdLink;

#ifdef DEBUG
    ASSERT( pLink->pProcedures, 0 );
    ASSERT( pLink->pData, 0 );
    if ( ProcIndex >= pLink->ProcCount )
        return( CLIENT_ERROR_BAD_PROCINDEX );
#endif

    pProcedure = ((PDLLPROCEDURE *) pLink->pProcedures)[ ProcIndex ];
    ASSERT( pProcedure, 0 );

    return( (*pProcedure)( pLink->pData, pParam ) );
}


/*******************************************************************************
 *
 *  ReservedVirtualChannel
 *
 *  This is called to determine if a given Channel is one from the
 *  compatability list.  
 *
 * ENTRY:
 *    pOVC
 *       Pointer to OPENVIRTUALCHANNEL structure
 *
 * EXIT:
 *    return TRUE if it is a compatability channel, set up list
 *    return FALSE if it is NOT a compatability channel, do nothing
 *
 ******************************************************************************/

BOOL
ReservedVirtualChannel( POPENVIRTUALCHANNEL pOVC )
{
    int         i;

    // Map table is NULL terminated
    for ( i=0; VcCompatablityMap[i].VirtualName[0] != '\0'; i++ ) {
        if ( !(stricmp( pOVC->pVCName, VcCompatablityMap[i].VirtualName )) ) {
            pOVC->Channel = VcCompatablityMap[i].Channel;
            paWD_VcBind[iVc_Map].VirtualClass = VcCompatablityMap[i].Channel;
            strcpy( paWD_VcBind[iVc_Map].VirtualName, pOVC->pVCName );
            iVc_Map++;
            return( TRUE );
        }
    }

    return( FALSE );
}

/*******************************************************************************
 *
 *  GetFirstAvailableChannel
 *
 *  This is called to get a Virtual Channel handle. If a handle of -1 is
 *  returned then there are no more handle available.
 *
 * ENTRY:
 *    pOVC
 *       Pointer to OPENVIRTUALCHANNEL structure
 *
 * EXIT:
 *    return with pOVC-Channel set to either the channel # -1 if something
 *    is wrong.
 *
 ******************************************************************************/

VOID
GetFirstAvailableChannel( POPENVIRTUALCHANNEL pOVC )
{
    USHORT i;

    // Initialize with failure
    pOVC->Channel = (USHORT)-1;

    if ( strlen( pOVC->pVCName ) <= VIRTUALNAME_LENGTH  ) {
        for ( i=0; i <= VIRTUAL_MAXIMUM; i++ ) {
            if ( HandleIsNotReserved( (USHORT)(i) ) ) {
                strcpy( paWD_VcBind[iVc_Map].VirtualName, pOVC->pVCName );
                paWD_VcBind[iVc_Map].VirtualClass = i;
                pOVC->Channel = i;
                iVc_Map++;
                break;
            }
        }
    }

    return;
}

/*******************************************************************************
 *
 *  HandleIsNotReserved
 *
 * ENTRY:
 *    Channel
 *       Virtual Channel handle
 *
 * EXIT:
 *    TRUE  - It is a reserved handle
 *    FALSE - It is not a reserved handle
 *
 ******************************************************************************/

BOOL
HandleIsNotReserved( USHORT Channel )
{
    int i;

    for ( i=0; VcCompatablityMap[i].VirtualName[0] != '\0'; i++ ) {
        if ( Channel == VcCompatablityMap[i].Channel ) {
            return( FALSE );
        }
    }

    return( TRUE );
}

