
/*************************************************************************
*
* tdapi.c
*
* Protocol driver external routines
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (3/25/94)
*
*  tdapi.c,v
*  Revision 1.1  1998/01/12 11:35:52  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.48   15 Apr 1997 16:54:46   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.48   21 Mar 1997 16:07:46   bradp
*  update
*  
*     Rev 1.47   08 Feb 1997 16:04:14   jeffm
*  multiple browser support
*  
*     Rev 1.46   16 May 1996 11:04:58   marcb
*  update
*  
*     Rev 1.45   08 May 1996 15:34:42   jeffm
*  update
*  
*     Rev 1.44   12 Apr 1996 16:31:34   richa
*  Commented out WEP function.
*  
*     Rev 1.43   12 Apr 1996 11:10:26   richa
*  Added G_hModule for LoadString()
*  
*     Rev 1.42   27 Mar 1996 09:22:36   richa
*  Added GetModuleHandle.
*  
*     Rev 1.41   20 Mar 1996 17:44:14   KenB
*  Take out ErrorMessages[] (it was commented out anyway...)
*  
*     Rev 1.40   20 Mar 1996 14:14:04   KenB
*  remove a warning...
*  
*     Rev 1.39   12 Mar 1996 13:15:10   richa
*  
*     Rev 1.38   12 Mar 1996 11:46:30   KenB
*  loadstr.h includes wferror.h, no reason to include it twice...
*  
*     Rev 1.37   12 Mar 1996 11:32:12   KenB
*  Include loadstr.h to put the LoadString() function in here (rather than including it in each component)
*  
*     Rev 1.36   12 Mar 1996 11:13:02   KenB
*  Include wferror.h; take out ErrorMessages[] and use LoadString() instead
*  
*     Rev 1.35   13 Feb 1996 20:48:34   bradp
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
#include "citrix/ibrowerr.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/td.h"
//#include "../../../inc/loadstr.h"

#include "../../../inc/tddevice.h"
#include "../../../inc/tddevicep.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI TdLoad( PDLLLINK );
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
static int ClientGetLastError( PPD, PPDLASTERROR );

extern int NameToAddress( PPD, PNAMEADDRESS );


/*=============================================================================
==   External Functions used
=============================================================================*/

/*
int STATIC DeviceOpen( PPD, PPDOPEN );
int STATIC DeviceClose( PPD, PDLLCLOSE );
int STATIC DeviceInfo( PPD, PDLLINFO );
int STATIC DeviceConnect( PPD );
int STATIC DeviceDisconnect( PPD );
int STATIC DeviceProcessInput( PPD );
int STATIC DeviceWrite( PPD, POUTBUF, PUSHORT );
int STATIC DeviceCheckWrite( PPD, POUTBUF );
int STATIC DeviceCancelWrite( PPD, POUTBUF );
int STATIC DeviceSendBreak( PPD );
*/

/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define WinStation driver external procedures
 */
static PDLLPROCEDURE PdProcedures[ PD__COUNT ] = {
    (PDLLPROCEDURE) TdLoad,
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
 *  Define Protocol driver data structure
 *   (this could be dynamically allocated)
 */
// STATIC PD PdData = {0};

/*
 *  Setup in the DLL init routines and used by LoadString()
 */
//STATIC HINSTANCE G_hModule = 0;


/*=============================================================================
==   Global Data
=============================================================================*/

#if 0
STATIC PPLIBPROCEDURE pModuleProcedures = NULL;
STATIC PPLIBPROCEDURE pClibProcedures = NULL;
STATIC PPLIBPROCEDURE pLogProcedures = NULL;
STATIC PPLIBPROCEDURE pBIniProcedures = NULL;
#endif

//extern LPBYTE pProtocolName;


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
TdLoad( PDLLLINK pLink )
{
    PPD pPd = malloc(sizeof(PD));
    
    /*
     *  This td must always be loaded first.
     */
    if ( pLink->pData )
        return( CLIENT_ERROR_INVALID_DLL_LOAD );

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
    free(pPd);
    
    pLink->ProcCount = 0;
    pLink->pProcedures = NULL;
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
    NAMERESOLVER DllName;
    int rc;
    int i;
    BYTE BrowserKey[40];
    ADDRESS AddrList[MAX_BROWSERADDRESSLIST];
    LPBYTE lpAddrList;

#if 0
    /*
     *  Initialize PD function call tables: MUST BE FIRST!
     */
    pModuleProcedures = pPdOpen->pModuleProcedures;
    pLogProcedures    = pPdOpen->pLogProcedures;
    pBIniProcedures   = pPdOpen->pBIniProcedures;
    pClibProcedures   = pPdOpen->pClibProcedures;
#endif
    /*
     *  Initialize PD data structure
     */
    memset( pPd, 0, sizeof(PD) );
    pPd->pDeviceProcedures = pPdOpen->pDeviceProcedures;

    /*
     *  Get name of name resolver dll
     */
    bGetPrivateProfileString( pPdOpen->pIniSection, INI_NAMERESOLVER,
                             INI_EMPTY, DllName, sizeof(DllName) );
    if ( DllName[0] ) {
        pPd->pNrDll = (char *) malloc( strlen(pPdOpen->pExePath) + strlen(DllName) + 1 );
        if ( pPd->pNrDll == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto badmalloc;
        }
        strcpy( pPd->pNrDll, pPdOpen->pExePath );
        strcat( pPd->pNrDll, DllName );
    }

    /* 
     *  Get ICA browser parameters
     */
    bGetPrivateProfileString( pPdOpen->pIniSection, 
                              INI_TCPBROWSERADDRESS, DEF_TCPBROWSERADDRESS, 
                              pPd->TcpBrowserAddress, sizeof(pPd->TcpBrowserAddress) );

    bGetPrivateProfileString( pPdOpen->pIniSection, 
                              INI_IPXBROWSERADDRESS, DEF_IPXBROWSERADDRESS, 
                              pPd->IpxBrowserAddress, sizeof(pPd->IpxBrowserAddress) );

    bGetPrivateProfileString( pPdOpen->pIniSection, 
                              INI_NETBIOSBROWSERADDRESS, DEF_NETBIOSBROWSERADDRESS, 
                              pPd->NetBiosBrowserAddress, sizeof(pPd->NetBiosBrowserAddress) );

    /*
     *  determine if more than one browser address specified for TCP, IPX, NETBIOS
     */
    if( pPd->TcpBrowserAddress[0] ) {
       memset( AddrList, 0, sizeof(AddrList));
       memcpy( AddrList[0], pPd->TcpBrowserAddress, sizeof(pPd->TcpBrowserAddress));
       for(i=1; i<MAX_BROWSERADDRESSLIST; i++) {
           sprintf(BrowserKey,"%s%d",INI_TCPBROWSERADDRESS,i+1);
           bGetPrivateProfileString( pPdOpen->pIniSection, 
                                     BrowserKey,
                                     DEF_TCPBROWSERADDRESS, 
                                     AddrList[i], 
                                     sizeof(pPd->TcpBrowserAddress) );
       }
       lpAddrList = (char *)malloc( sizeof(AddrList) );
       if(lpAddrList == NULL) {
           rc = CLIENT_ERROR_NO_MEMORY;
           goto badopen;
       }
       memcpy( lpAddrList, AddrList, sizeof(AddrList));
       pPd->pTcpBrowserAddrList = lpAddrList;
    }

    if( pPd->IpxBrowserAddress[0] ) {
       memset( AddrList, 0, sizeof(AddrList));
       memcpy( AddrList[0], pPd->IpxBrowserAddress, sizeof(pPd->IpxBrowserAddress));
       for(i=1; i<MAX_BROWSERADDRESSLIST; i++) {
           sprintf(BrowserKey,"%s%d",INI_IPXBROWSERADDRESS,i+1);
           bGetPrivateProfileString( pPdOpen->pIniSection, 
                                     BrowserKey,
                                     DEF_IPXBROWSERADDRESS, 
                                     AddrList[i], 
                                     sizeof(pPd->IpxBrowserAddress) );
       }
       lpAddrList = (char *)malloc( sizeof(AddrList) );
       if(lpAddrList == NULL) {
           rc = CLIENT_ERROR_NO_MEMORY;
           goto badopen;
       }
       memcpy( lpAddrList, AddrList, sizeof(AddrList));
       pPd->pIpxBrowserAddrList = lpAddrList;
    }

    if( pPd->NetBiosBrowserAddress[0] ) {
       memset( AddrList, 0, sizeof(AddrList));
       memcpy( AddrList[0], pPd->NetBiosBrowserAddress, sizeof(pPd->IpxBrowserAddress));
       for(i=1; i<MAX_BROWSERADDRESSLIST; i++) {
           sprintf(BrowserKey,"%s%d",INI_NETBIOSBROWSERADDRESS,i+1);
           bGetPrivateProfileString( pPdOpen->pIniSection, 
                                     BrowserKey,
                                     DEF_NETBIOSBROWSERADDRESS, 
                                     AddrList[i], 
                                     sizeof(pPd->NetBiosBrowserAddress) );
       }
       lpAddrList = (char *)malloc( sizeof(AddrList) );
       if(lpAddrList == NULL) {
           rc = CLIENT_ERROR_NO_MEMORY;
           goto badopen;
       }
       memcpy( lpAddrList, AddrList, sizeof(AddrList));
       pPd->pNetBiosBrowserAddrList = lpAddrList;
    }

    pPd->BrowserRetry = bGetPrivateProfileInt( pPdOpen->pIniSection,
                                               INI_BROWSERRETRY,
                                               DEF_BROWSERRETRY );

    pPd->BrowserTimeout = bGetPrivateProfileInt( pPdOpen->pIniSection,
                                                 INI_BROWSERTIMEOUT,
                                                 DEF_BROWSERTIMEOUT );
    /*
     *  Get buffer lengths and counts
     */

    pPd->OutBufLength = bGetPrivateProfileInt( pPdOpen->pIniSection,
                                               INI_OUTBUFLENGTH,
                                               DEF_OUTBUFLENGTH );

    pPd->OutBufCountHost = bGetPrivateProfileInt( pPdOpen->pIniSection,
                                                  INI_OUTBUFCOUNTHOST,
                                                  DEF_OUTBUFCOUNTHOST );

    pPd->OutBufCountClient = bGetPrivateProfileInt( pPdOpen->pIniSection,
                                                    INI_OUTBUFCOUNTCLIENT,
                                                    DEF_OUTBUFCOUNTCLIENT );

    /*
     *  Initialize low level structures
     */
    if ( rc = DeviceOpen( pPd, pPdOpen ) )
        goto badopen;

    pPd->OutBufHeader  = pPdOpen->OutBufHeader;
    pPd->OutBufTrailer = pPdOpen->OutBufTrailer;

    /*
     *  Set Protocol Driver is open flag
     */
    pPd->fOpen = TRUE;

    TRACE(( TC_TD, TT_API1, "TdOpen: success" ));
    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad open
     */
badopen:
    if ( pPd->pNetBiosBrowserAddrList ) {
        free( pPd->pNetBiosBrowserAddrList);
        pPd->pNetBiosBrowserAddrList = NULL;
    }

    if ( pPd->pIpxBrowserAddrList ) {
        free( pPd->pIpxBrowserAddrList);
        pPd->pIpxBrowserAddrList = NULL;
    }

    if ( pPd->pTcpBrowserAddrList ) {
        free( pPd->pTcpBrowserAddrList);
        pPd->pTcpBrowserAddrList = NULL;
    }

    if ( pPd->pNrDll ) {
        free( pPd->pNrDll );
        pPd->pNrDll = NULL;
    }

    /*
     *  bad malloc
     */
badmalloc:
    TRACE(( TC_TD, TT_API1, "TdOpen: rc=%u", rc ));
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

    TRACE(( TC_TD, TT_API1, "TdClose: (enter)" ));

    /*
     *  Cancel all writes
     */
    (void) CancelWrite( pPd );

    /*
     *  All writes should have previously been canceled
     *  which clears the open flag
     */
    ASSERT( !pPd->fOpen, 0 );
    ASSERT( pPd->pOutBufHead == NULL && pPd->pOutBufTail == NULL, 0 );

    if ( pPd->pNrDll ) {
        free( pPd->pNrDll );
        pPd->pNrDll = NULL;
    }

    if ( pPd->pNetBiosBrowserAddrList ) {
        free( pPd->pNetBiosBrowserAddrList);
        pPd->pNetBiosBrowserAddrList = NULL;
    }

    if ( pPd->pIpxBrowserAddrList ) {
        free( pPd->pIpxBrowserAddrList);
        pPd->pIpxBrowserAddrList = NULL;
    }

    if ( pPd->pTcpBrowserAddrList ) {
        free( pPd->pTcpBrowserAddrList);
        pPd->pTcpBrowserAddrList = NULL;
    }

    /*
     *  Close PD
     */
    rc = DeviceClose( pPd, pPdClose );

    TRACE(( TC_TD, TT_API1, "TdClose: rc=%u", rc ));
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
 *    pTdInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PdInfo( PPD pPd, PDLLINFO pTdInfo )
{
    USHORT ByteCount;
    PTD_C2H pTdData;
    PMODULE_C2H pHeader;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(TD_C2H);

    /*
     *  Check if buffer is big enough
     */
    if ( pTdInfo->ByteCount < ByteCount ) {
        pTdInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize default data
     */
    pTdInfo->ByteCount = ByteCount;
    pTdData = (PTD_C2H) pTdInfo->pBuffer;
    memset( pTdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pTdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_TransportDriver;

    /*
     *  Initialize transport driver data
     */
    pTdData->OutBufLength = pPd->OutBufLength;

    return( DeviceInfo( pPd, pTdInfo ) );
}


/*******************************************************************************
 *
 *  PdPoll
 *
 *    WdPoll calls PdPoll to get status and to give control to the protocol
 *    drivers.  This function never blocks and always returns the current
 *    status of the connection.
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
    PDWRITE WriteData;
    int rc;

    /*
     *  If first pass thru return tty connected status
     *  If second pass thru return connected status
     */
    if ( !pPd->fSentConnect ) {
        if ( !pPd->fSentTTYConnect ) {
            pPd->fSentTTYConnect = TRUE;
            return( CLIENT_STATUS_TTY_CONNECTED );
        }
        pPd->fSentConnect = TRUE;
        return( CLIENT_STATUS_CONNECTED );
    }

    /*
     *  Check status of pending write
     */
    WriteData.pOutBuf = NULL;
    if ( rc = PdWrite( pPd, &WriteData ) )
        return( rc );

    /*
     *  Process input data
     */
    rc = DeviceProcessInput( pPd );

    /*
     *  Update receive status
     */
    if ( rc == CLIENT_STATUS_SUCCESS ) {

        // increment frames received field
        pPd->FramesRecv++;

        if ( !pPd->fReceiveActive ) {
            pPd->fReceiveActive = TRUE;
            return( CLIENT_STATUS_RECEIVE_START );
        }
    } else if ( rc == CLIENT_STATUS_NO_DATA ) {
        if ( pPd->fReceiveActive ) {
            pPd->fReceiveActive = FALSE;
            return( CLIENT_STATUS_RECEIVE_STOP );
        }
    } else {
        // keep track of receive errors
        pPd->FrameRecvError++;

        return( rc );
    }

    /*
     *  Transport driver send has started
     */
    if ( pPd->fSendStart ) {
        pPd->fSendStart = FALSE;
        return( CLIENT_STATUS_SEND_START );
    }

    /*
     *  Transport driver send has stopped
     */
    if ( pPd->fSendStop ) {
        pPd->fSendStop = FALSE;
        return( CLIENT_STATUS_SEND_STOP );
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
    POUTBUF pOutBuf;
    USHORT BytesWritten;
    int rc;

    /*
     *  Get pointer to outbuf  (may be null)
     */
    pOutBuf = pPdWrite->pOutBuf;

    /*
     *  Check if we are open
     */
    if ( !pPd->fOpen ) {
        if ( pOutBuf )
            (pPd->pOutBufErrorProc)( pPd->pWdData, pOutBuf );
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     *  Add new request to end of output queue
     */
    if ( pOutBuf ) {
        ASSERT( pOutBuf->pLink == NULL, 0 );
        if ( pPd->pOutBufTail ) {
            pPd->pOutBufTail->pLink = pOutBuf;
            pPd->pOutBufTail = pOutBuf;
        } else {
            pPd->pOutBufHead = pOutBuf;
            pPd->pOutBufTail = pOutBuf;
        }
    }

    /*
     *  Check status of previous write
     */
    if ( pOutBuf = pPd->pOutBufCurrent ) {

        /*
         *  Check if previous write has completed
         */
        rc = DeviceCheckWrite( pPd, pOutBuf );

        /*
         *  Write is still pending - return
         */
        if ( rc == CLIENT_ERROR_IO_PENDING )
            return( CLIENT_STATUS_SUCCESS );

        /*
         *  Restore original byte count and buffer pointer
         */
        if ( pOutBuf->SaveByteCount ) {
            pOutBuf->pBuffer       = pOutBuf->pSaveBuffer;
            pOutBuf->ByteCount     = pOutBuf->SaveByteCount;
            pOutBuf->pSaveBuffer   = NULL;
            pOutBuf->SaveByteCount = 0;
        }

        /*
         *  Write is complete - send outbuf to next td in chain
         */
        (pPd->pOutBufFreeProc)( pPd->pWdData, pOutBuf );
        pPd->pOutBufCurrent = NULL;
        pPd->fSendStop = TRUE;
    }

    /*
     *  Get pointer to outbuf at head of output queue
     */
    if ( (pOutBuf = pPd->pOutBufHead) == NULL )
        return( CLIENT_STATUS_SUCCESS );

    /*
     * If there is no data, return outbuf to free list
     */
    if ( pOutBuf->ByteCount == 0 ) {
        rc = CLIENT_STATUS_SUCCESS;
        goto badwrite;
    }

    /*
     *  Write data
     */
    pPd->fSendStart = TRUE;
    if ( rc = DeviceWrite( pPd, pOutBuf, &BytesWritten ) )
        goto badwrite;

    // update bytes sent tally
    pPd->BytesSent += BytesWritten;
    pPd->FramesSent++;

    LogPrintf( LOG_CLASS, LOG_TRANSMIT, "TRANSMIT: %u bytes", BytesWritten );
    LogBuffer( LOG_CLASS, LOG_TRANSMIT,
                 (PVOID)pOutBuf->pBuffer, (ULONG) BytesWritten );

    /*
     *  Call write hook routine
     */
    if ( pPd->pWriteHook )
        (*pPd->pWriteHook)( pOutBuf->pBuffer, BytesWritten );

    /*
     *  Check if complete outbuf was written
     */
    if ( BytesWritten == pOutBuf->ByteCount ) {

        /*
         *  Complete outbuf was written
         */
        pPd->pOutBufCurrent = pOutBuf;

        /*
         *  Unlink outbuf from head of queue
         */
        pPd->pOutBufHead = pOutBuf->pLink;
        if ( pPd->pOutBufHead == NULL )
            pPd->pOutBufTail = NULL;
        pOutBuf->pLink = NULL;

    } else {

        /*
         *  Save original byte count and buffer pointer the first time
         *  - these get reused by PdReli when a retransmit is done
         */
        if ( pOutBuf->SaveByteCount == 0 ) {
            pOutBuf->pSaveBuffer   = pOutBuf->pBuffer;
            pOutBuf->SaveByteCount = pOutBuf->ByteCount;
        }

        /*
         *  Partial outbuf was written
         *  - adjust bytes remaining
         *  - leave outbuf on write queue
         */
        pOutBuf->pBuffer += BytesWritten;
        pOutBuf->ByteCount -= BytesWritten;
    }

    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  Unlink outbuf from head of queue
     */
badwrite:
    pPd->pOutBufHead = pOutBuf->pLink;
    if ( pPd->pOutBufHead == NULL )
        pPd->pOutBufTail = NULL;
    pOutBuf->pLink = NULL;

    /*
     *  Free outbuf
     */
    (pPd->pOutBufErrorProc)( pPd->pWdData, pOutBuf );

    // keep track of send errors
    if(rc != CLIENT_STATUS_SUCCESS)
       pPd->FrameSendError++;

    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
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
    PBUFFERINFO pBufInfo;
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pPdQueryInformation->PdInformationClass ) {

        case PdBufferInfo :
            ASSERT( pPdQueryInformation->PdInformationLength == sizeof(BUFFERINFO), 0 );
            pBufInfo = (PBUFFERINFO) pPdQueryInformation->pPdInformation;
            pBufInfo->OutBufLength      = pPd->OutBufLength;
            pBufInfo->OutBufCountHost   = pPd->OutBufCountHost;
            pBufInfo->OutBufCountClient = pPd->OutBufCountClient;
            pPdQueryInformation->PdReturnLength = sizeof(BUFFERINFO);
            break;

        case PdLastError :
            ASSERT( pPdQueryInformation->PdInformationLength == sizeof(PDLASTERROR), 0 );
            rc = ClientGetLastError( pPd, (PPDLASTERROR) pPdQueryInformation->pPdInformation );
            pPdQueryInformation->PdReturnLength = sizeof(PDLASTERROR);
            break;

        case PdIOStatus:
            {
            PIOSTATUS pIOStat;

            pIOStat = (PIOSTATUS)pPdQueryInformation->pPdInformation;
            pIOStat->BytesRecv      = pPd->BytesRecv;     
            pIOStat->BytesSent      = pPd->BytesSent;     
            pIOStat->FramesRecv     = pPd->FramesRecv;    
            pIOStat->FramesSent     = pPd->FramesSent;    
            pIOStat->FrameRecvError = pPd->FrameRecvError;
            pIOStat->FrameSendError = pPd->FrameSendError;
            pPdQueryInformation->PdReturnLength = sizeof(IOSTATUS);
            }
            break;
    }

    TRACE(( TC_TD, TT_API1, "TdQueryInformation(%u): rc=0x%x",
            pPdQueryInformation->PdInformationClass, rc ));

    return( rc );
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
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pPdSetInformation->PdInformationClass ) {

        case PdWdAddress :
            ASSERT( pPdSetInformation->PdInformationLength == sizeof(WDADDRESS), 0 );
            pWdAddress = (PWDADDRESS) pPdSetInformation->pPdInformation;
            pPd->pWdData            = pWdAddress->pWdData;
            pPd->pOutBufAllocProc   = pWdAddress->pOutBufAllocProc;
            pPd->pOutBufErrorProc   = pWdAddress->pOutBufErrorProc;
            pPd->pOutBufFreeProc    = pWdAddress->pOutBufFreeProc;
            pPd->pProcessInputProc  = pWdAddress->pProcessInputProc;
            break;

        case PdConnect :
            rc = DeviceConnect( pPd );
            break;

        case PdDisconnect :
            rc = DeviceDisconnect( pPd );
            break;

        case PdCancelWrite :
            CancelWrite( pPd );
            break;

        case PdAddReadHook :
            pPd->pReadHook = (PIOHOOKPROC) pPdSetInformation->pPdInformation;
            break;

        case PdRemoveReadHook :
            pPd->pReadHook = NULL;
            break;

        case PdAddWriteHook :
            pPd->pWriteHook = (PIOHOOKPROC) pPdSetInformation->pPdInformation;
            break;

        case PdRemoveWriteHook :
            pPd->pWriteHook = NULL;
            break;

        case PdDisableModule :
            pPd->fSentConnect = FALSE;
            break;

    }

    TRACE(( TC_TD, TT_API1, "TdSetInformation(%u): rc=0x%x",
            pPdSetInformation->PdInformationClass, rc ));

    return( rc );
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
 *       Pointer to td data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

static void
CancelWrite( PPD pPd )
{
    POUTBUF pOutBuf;

    TRACE(( TC_TD, TT_API1, "TdCancelWrite (enter)" ));

    /*
     *  Bit bucket all future writes
     */
    pPd->fOpen = FALSE;

    /*
     *  Cancel queued writes
     */
    while ( pOutBuf = pPd->pOutBufHead ) {
        pPd->pOutBufHead = pOutBuf->pLink;
        pOutBuf->pLink = NULL;
        (void) DeviceCancelWrite( pPd, pOutBuf );
        (*pPd->pOutBufErrorProc)( pPd->pWdData, pOutBuf );
    }
    pPd->pOutBufTail = NULL;

    /*
     *  Cancel pending write
     */
    if ( pOutBuf = pPd->pOutBufCurrent ) {
        (void) DeviceCancelWrite( pPd, pOutBuf );
        (*pPd->pOutBufErrorProc)( pPd->pWdData, pOutBuf );
        pPd->pOutBufCurrent = NULL;
    }

    TRACE(( TC_TD, TT_API1, "TdCancelWrite (exit)" ));
}


/*******************************************************************************
 *
 *  ClientGetLastError
 *
 *   This routine returns the last protocol error code and message
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pLastError (output)
 *       address to return information on last protocol error
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
ClientGetLastError( PPD pPd, PPDLASTERROR pLastError )
{

    pLastError->Error = pPd->LastError;
    memset( pLastError->Message, 0, sizeof(pLastError->Message) );

    /*
     * Name resolver messages are gotten immediately
     */
    if ( pPd->ErrorMessage ) {
        strncpy( pLastError->Message,
                 pPd->ErrorMessage,
                 MAX_ERRORMESSAGE-1 );
        free( pPd->ErrorMessage );
        pPd->ErrorMessage = NULL;
        strcpy( pLastError->ProtocolName, pPd->ErrorProtocolName );
    }
    else {
        strcpy( pLastError->ProtocolName, pPd->pProtocolName );

        if ( !LoadString( "NET", 
                          pPd->LastError,
                          pLastError->Message,
                          sizeof(pLastError->Message) ) ) {
            /*
             * We didn't map an error message, use the default error code.
             */
            LoadString( NULL, 
                        (UINT)(ERROR_DEFAULT),
                        pLastError->Message,
                        sizeof(pLastError->Message) );
        }
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NameToAddress
 *
 *   NameToAddress loads a Name Resolver to convert an application server
 *   name to an address that can be used by the transport drvier
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to procotol driver data structure
 *    pNameAddress (input/output)
 *       pointer to the structure NAMEADDRESS
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
NameToAddress( PPD pPd, PNAMEADDRESS pNameAddress )
{
    NROPEN NrOpen;
    DLLLINK NrLink;
    int rc;
    int rc2;
    PDLASTERROR ErrorLookup;

    memset( pNameAddress->Address, 0, sizeof(pNameAddress->Address) );

    /*
     *  Return if there is no name resolver
     */
    if ( pPd->pNrDll == NULL )
        return( CLIENT_ERROR_NO_NAME_RESOLVER );

    /*
     *  Load dll into memory
     */
    rc = ModuleLoad( pPd->pNrDll, &NrLink );
    TRACE((TC_TD, TT_API2, "NameToAddress: ModuleLoad %s, rc=%u", pPd->pNrDll, rc));
    if ( rc )
        return( rc );

    /*
     *  Open name resolver
     */
    memset( &NrOpen, 0, sizeof(NROPEN) );
//  NrOpen.pClibProcedures        = pClibProcedures;
//  NrOpen.pLogProcedures         = pLogProcedures;
    NrOpen.pTcpBrowserAddress     = pPd->TcpBrowserAddress;
    NrOpen.pIpxBrowserAddress     = pPd->IpxBrowserAddress;
    NrOpen.pNetBiosBrowserAddress = pPd->NetBiosBrowserAddress;
    NrOpen.pTcpBrowserAddrList    = pPd->pTcpBrowserAddrList;
    NrOpen.pIpxBrowserAddrList    = pPd->pIpxBrowserAddrList;
    NrOpen.pNetBiosBrowserAddrList= pPd->pNetBiosBrowserAddrList;
    NrOpen.BrowserRetry           = pPd->BrowserRetry;
    NrOpen.BrowserTimeout         = pPd->BrowserTimeout;
    rc = ModuleCall( &NrLink, DLL__OPEN, &NrOpen );
    TRACE((TC_TD, TT_API2, "NameToAddress(open): tcp %s, ipx %s, netb %s, retry %u, timeout %u, rc=%u", 
           NrOpen.pTcpBrowserAddress, NrOpen.pIpxBrowserAddress,
           NrOpen.pNetBiosBrowserAddress,
           NrOpen.BrowserRetry, NrOpen.BrowserTimeout, rc));

    if ( rc == CLIENT_STATUS_SUCCESS ) {

        /*
         *  Convert name to address
         */
        rc = ModuleCall( &NrLink, NR__NAMETOADDRESS, pNameAddress );
        TRACE((TC_TD, TT_API2, "NameToAddress: ModuleCall NR__NAMETOADDRESS, rc=%u", rc));

        /*
         *  If name resolver can not detect netbios let the transport driver try
         *  -- this is needed when using netbios over ras
         */
        if ( rc == BR_ERROR_NETBIOS_NOT_AVAILABLE )
            rc = CLIENT_ERROR_NO_NAME_RESOLVER;

        /*
         * Get Name Resolver errors immediately
         */
        if ( rc != CLIENT_STATUS_SUCCESS && rc != CLIENT_ERROR_NO_NAME_RESOLVER ) {
            /*
             *  Get the string for this error number
             */
            ErrorLookup.Error = rc;

            rc = ModuleCall( &NrLink, NR__ERRORLOOKUP, &ErrorLookup );

            TRACE((TC_TD, TT_API2, "NameToAddress: ModuleCall NR__ERRORLOOKUP, rc=%u", rc));
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                pPd->ErrorMessage = strdup( ErrorLookup.Message );
                if ( pPd->ErrorMessage == NULL ) {
                   rc = CLIENT_ERROR_NO_MEMORY;
                }
                else {
                    pPd->LastError = ErrorLookup.Error;
                    strcpy( pPd->ErrorProtocolName, ErrorLookup.ProtocolName );
                    rc = CLIENT_ERROR_PD_ERROR;
                    TRACE((TC_TD, TT_API2, "NameToAddress: %s, rc=%u", pPd->ErrorMessage, pPd->LastError));
                }
            }
        }

        /*
         *  Close the name resolver
         */
        rc2 = ModuleCall( &NrLink, DLL__CLOSE, NULL );
        TRACE((TC_TD, TT_API2, "NameToAddress: ModuleCall DLL__CLOSE, rc=%u", rc2));
    }

    /*
     *  Unload the user interface
     */
    rc2 = ModuleUnload( &NrLink );
    TRACE((TC_TD, TT_API2, "NameToAddress: ModuleUnload, rc=%u", rc2));

    return( rc );
}

#ifdef WIN16
/*******************************************************************************
 * function: LibMain
 *
 * purpose:  LibMain is called by Windows (WIN16) to initialize a DLL.
 *
 * entry: (see LibMain definition in Windows 3.1 SDK)
 *
 * exit: (see LibMain definition in Windows 3.1 SDK)
 *
 ******************************************************************************/

int CALLBACK
LibMain( HINSTANCE hInst,
         WORD wDataSeg,
         WORD cbHeap,
         LPSTR lpszCmdLine )
{

    G_hModule = hInst;
    return( TRUE );
}

//
///*******************************************************************************
// * function: WEP
// *
// * purpose:  WEP is called by Windows (WIN16) to deinitialize a DLL.
// *
// * entry:
// *   nExitType (input)
// *      WEP_FREE_DLL -     This app is exiting
// *      WEP_SYSTEM_EXIT -  The system is exiting
// *
// * exit:
// *   TRUE if successful / FALSE if unsuccessful
// *
// *
// ******************************************************************************/
//int CALLBACK
//WEP( int nExitType )
//{
//
//    //  For future if needed
//     
//    return( TRUE );
//}
//
#endif
#ifdef   WIN32

/****************************************************************************
*   FUNCTION: DllMain(HANDLE, DWORD, LPVOID)
*
*  PURPOSE:  DllMain is called by Windows when
*            the DLL is initialized, Thread Attached, and other times.
*
*
*
******************************************************************************/

BOOL WINAPI
DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    switch ( dwReason ) {
        case DLL_PROCESS_ATTACH:
            G_hModule = hDLL;
            break;

        case DLL_PROCESS_DETACH:
            // For future
            break;

    }
    return( TRUE );
}
#endif
