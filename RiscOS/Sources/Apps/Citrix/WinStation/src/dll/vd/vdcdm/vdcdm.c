/*************************************************************************
*
* vdcdm.c
*
* Client Drive Mapping Virtual Driver
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author:  JohnR 04/20/94
*
*     Rev 1.31   09 Jul 1997 16:09:30   davidp
*  Added include for ica30.h because of Hydrix surgery
*  
*     Rev 1.30   15 Apr 1997 18:02:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.30   21 Mar 1997 16:09:10   bradp
*  update
*  
*     Rev 1.29   13 Nov 1996 09:15:48   richa
*  Updated Virtual channel code.
*  
*     Rev 1.28   08 May 1996 14:09:42   jeffm
*  update
*  
*     Rev 1.27   11 Dec 1995 19:29:28   JohnR
*  update
*
*     Rev 1.26   19 Oct 1995 13:58:38   JohnR
*  update
*
*     Rev 1.25   19 Oct 1995 13:52:48   JohnR
*  CPR#1968 fix
*
*
*     Rev 1.25   14 Sep 1995 11:23:18   JohnR
*  update
*
*     Rev 1.24   20 Jul 1995 15:03:56   bradp
*  update
*
*     Rev 1.23   24 Jun 1995 18:31:54   richa
*
*     Rev 1.22   02 May 1995 13:57:10   butchd
*  update
*
*     Rev 1.21   17 Apr 1995 12:45:54   marcb
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

#include "../../../inc/clib.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/vd.h"
#include "../../wd/inc/wd.h"
#include "citrix/cdmwire.h" // Wire protocol definitions
#include "vdcdm.h"

#define NO_VDDRIVER_DEFINES
#include "../../../inc/vddriver.h"
#include "../../../inc/vddriverp.h"


/*=============================================================================
==   Functions Defined
=============================================================================*/
static int DriverOpen( PVD, PVDOPEN );
static int DriverClose( PVD, PDLLCLOSE );
static int DriverInfo( PVD, PDLLINFO );
static int DriverPoll( PVD, PDLLPOLL );
static int DriverQueryInformation( PVD, PVDQUERYINFORMATION );
static int DriverSetInformation( PVD, PVDSETINFORMATION );
static int DriverGetLastError( PVD, PVDLASTERROR );



void CdmHardError( int deverror, int errorcode );
USHORT CdmICAWrite( PUCHAR, USHORT );
BOOLEAN CdmICABufAvail( void );
int WFCAPI CdmPoll( LPVOID, LPVOID );
STATIC void WFCAPI ICADataArrival( LPVOID, USHORT, LPBYTE, USHORT );


/*=============================================================================
==   External Functions used
=============================================================================*/

extern int WdCall( PVD pVd, USHORT ProcIndex, LPVOID pParam );
int CdmDosGetDrives ( LPBYTE pBuffer, USHORT ByteCount, PUSHORT pDriveCount );

STATIC int RingBufWrite( LPBYTE, USHORT );
STATIC int RingBufEmpty( VOID );
STATIC int RingBufReadAvail( PUSHORT );
STATIC int RingBufRead( VOID );
STATIC USHORT CdmSecuritySetAccess( ULONG fAccessLimit );


/*=============================================================================
==   Data
=============================================================================*/

static LPVOID CdmTransportHandle = NULL;
static USHORT CdmMaxWindowSize = 0;
static USHORT CdmMaxRequestSize = 0;
static USHORT VirtualCdm;

// See inc\wdapi.h and dll\wd\wdica30\wdica.c
// These are returned when we register our hook

STATIC LPVOID pWd = NULL;
STATIC POUTBUFRESERVEPROC OutBufReserve   = NULL;
STATIC POUTBUFAPPENDPROC OutBufAppend     = NULL;
STATIC POUTBUFWRITEPROC OutBufWrite       = NULL;
STATIC PAPPENDVDHEADERPROC AppendVdHeader = NULL;


//
// Cache configuration parameters read from the INI file at
// startup. These are referenced by cdmserv.c whenever a
// connect request is received.
//

STATIC unsigned int CacheTimeout0 = DEF_CACHETIMEOUT0;
STATIC unsigned int CacheTimeout1 = DEF_CACHETIMEOUT1;
STATIC unsigned int CacheXferSize = DEF_CACHEXFERSIZE;
STATIC unsigned int CacheFlags = 0;


PLIBPROCEDURE VdCdmDriverProcedures[VDDRIVER__COUNT] =
{
    (PLIBPROCEDURE)DriverOpen,
    (PLIBPROCEDURE)DriverClose,
    (PLIBPROCEDURE)DriverInfo,
    (PLIBPROCEDURE)DriverPoll,
    (PLIBPROCEDURE)DriverSetInformation,
    (PLIBPROCEDURE)DriverQueryInformation,
    (PLIBPROCEDURE)DriverGetLastError   
};

/*******************************************************************************
 *
 *  DriverOpen
 *
 *    Called once to set up things.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pVdOpen (input/output)
 *       pointer to the structure VDOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DriverOpen( PVD pVd, PVDOPEN pVdOpen )
{
   WDSETINFORMATION wdsi;
   VDWRITEHOOK vdwh;
   int rc;
   WDQUERYINFORMATION wdqi;
   OPENVIRTUALCHANNEL OpenVirtualChannel;

   TRACE(( TC_CDM, TT_API3, "VDCDM: DriverOpen"));

   /*
    * Get a virtual channel
    */
   wdqi.WdInformationClass = WdOpenVirtualChannel;
   wdqi.pWdInformation = &OpenVirtualChannel;
   wdqi.WdInformationLength = sizeof_OPENVIRTUALCHANNEL;
   OpenVirtualChannel.pVCName = VIRTUAL_CDM;
   rc = WdCall( pVd, WD__QUERYINFORMATION, &wdqi );
   VirtualCdm = OpenVirtualChannel.Channel;
   ASSERT( VirtualCdm == Virtual_Cdm, VirtualCdm );

   /*
    * VD init return values
    */
   pVdOpen->ChannelMask = (1L << VirtualCdm);  // Client Drive Mapping Channel
   pVd->pPrivate   = NULL;          // Fill in ptr to our data if needed

   /*
    *  Register write hooks for all virtual channels handled by this vd
    */
   vdwh.Type  = VirtualCdm;
   vdwh.pVdData = pVd;
   vdwh.pProc = (PVDWRITEPROCEDURE) ICADataArrival;
   wdsi.WdInformationClass  = WdVirtualWriteHook;
   wdsi.pWdInformation      = &vdwh;
   wdsi.WdInformationLength = sizeof(VDWRITEHOOK);
   rc = WdCall(pVd, WD__SETINFORMATION, &wdsi);
   TRACE(( TC_CDM, TT_API2, "VDCDM: writehook ch=%u p=%lx rc=%u", vdwh.Type, vdwh.pVdData, rc ));

   if( rc ) {
        TRACE(( TC_CDM, TT_API1, "VDCDM: Could not register to the WD. rc %d", rc));
        return( rc );
   }

   // This returns pointers to functions to use to send data to the host
   pWd            = vdwh.pWdData;
   OutBufReserve  = vdwh.pOutBufReserveProc;
   OutBufAppend   = vdwh.pOutBufAppendProc;
   OutBufWrite    = vdwh.pOutBufWriteProc;
   AppendVdHeader = vdwh.pAppendVdHeaderProc;

   TRACE(( TC_CDM, TT_API3, "VDCDM: Registered"));

   CdmMaxWindowSize = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                             INI_MAXWINDOWSIZE,
                                             DEF_MAXWINDOWSIZE );

   CdmMaxRequestSize = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                              INI_MAXREQUESTSIZE,
                                              DEF_MAXREQUESTSIZE );
   if ( CdmMaxRequestSize > vdwh.MaximumWriteSize )
       CdmMaxRequestSize = vdwh.MaximumWriteSize;

   rc = SetupTransport( CdmMaxWindowSize,
                        CdmMaxRequestSize,
                        &CdmTransportHandle );

   if ( rc ) {
        // BUGBUG: Must deregister our hooks!
        TRACE(( TC_CDM, TT_ERROR, "VDCDM: Can't Setup Client Drive Mapping" ));
        return( rc );
   }

   TRACE(( TC_CDM, TT_API1, "VDCDM: Transport setup done" ));

   /*
    * Get the cache control parameters
    */

   CacheTimeout0 = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_CACHETIMEOUT0,
                                          DEF_CACHETIMEOUT0 );

   CacheTimeout1 = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_CACHETIMEOUT1,
                                          DEF_CACHETIMEOUT1 );

   CacheXferSize = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_CACHEXFERSIZE,
                                          DEF_CACHEXFERSIZE );

   if ( bGetPrivateProfileBool( pVdOpen->pIniSection,
                                INI_NOCACHE, DEF_NOCACHE ) ) {
       CacheFlags |= CONNECT_NOCACHE;
   }

   if ( bGetPrivateProfileBool( pVdOpen->pIniSection,
                                INI_NOWRITEALLOC, DEF_NOWRITEALLOC ) ) {
       CacheFlags |= CONNECT_NOWRITEALLOCATE;
   }

   /*
    * Set the error mode for the current task so that we
    * do not get pop ups due to floppy not in drive.
    * We handle this by passing the status back to the host.
    *
    * CPR#1968 Fix
    */
#ifdef WIN16
   SetErrorMode( SEM_FAILCRITICALERRORS );
#endif

   return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DriverClose
 *
 *  The user interface calls VdClose to close a Vd before unloading it.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to procotol driver data structure
 *    pVdClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

extern void CdmDosCloseAllContexts( void );

static int
DriverClose( PVD pVd, PDLLCLOSE pVdClose )
{
   pVdClose;

   if( CdmTransportHandle == NULL ) {
       TRACE(( TC_CDM, TT_API1, "CdmClose: Called Closed Handle"));
       return( CLIENT_ERROR_NOT_OPEN );
   }

   /*
    * Close the low level I/O contexts
    */
   CdmDosCloseAllContexts();

   /*
    * Delete the transport data structures
    */
   DeleteTransport( CdmTransportHandle );

   CdmTransportHandle = NULL;

   TRACE(( TC_CDM, TT_API3, "CdmClose: Done closing"));

   return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DriverInfo
 *
 *    This routine is called to get module information
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pVdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DriverInfo( PVD pVd, PDLLINFO pVdInfo )
{
    USHORT Count;
    USHORT ByteCount;
    PVDCDM_C2H pVdData;
    PMODULE_C2H pHeader;
    PVDFLOW pFlow;
    int rc;

    /*
     *  Get byte count necessary to hold data
     */
    if ( rc = CdmDosGetDrives( NULL, 0, &Count ) )
        return( rc );
    ByteCount = sizeof_VDCDM_C2H + (Count * sizeof(VDCLIENTDRIVES));

    /*
     *  Check if buffer is big enough
     */
    if ( pVdInfo->ByteCount < ByteCount ) {
        pVdInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize default data
     */
    pVdInfo->ByteCount = ByteCount;
    pVdData = (PVDCDM_C2H) pVdInfo->pBuffer;
    memset( pVdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pVdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_VirtualDriver;
    pHeader->VersionL = VERSION_CLIENTL_VDCDM;
    pHeader->VersionH = VERSION_CLIENTH_VDCDM;

    /*
     *  Initialize virtual driver header
     */
    pVdData->Header.ChannelMask = pVd->ChannelMask;
    pFlow = &pVdData->Header.Flow;
    pFlow->BandwidthQuota = 0;
    pFlow->Flow = VirtualFlow_Cdm;
    pFlow->data.Cdm.MaxWindowSize = CdmMaxWindowSize;
    pFlow->data.Cdm.MaxByteCount = CdmMaxRequestSize;

    /*
     *  Initialize virtual driver data
     */
    rc = CdmDosGetDrives( pVdInfo->pBuffer + sizeof_VDCDM_C2H,
                          (USHORT)(pVdInfo->ByteCount - sizeof_VDCDM_C2H),
                          &pVdData->ClientDriveCount );

    if ( pVdData->ClientDriveCount > 0 )
        pVdData->oClientDrives = sizeof_VDCDM_C2H;

    /*
     * Fill in the cache control parameters
     */
    pVdData->CacheFlags = CacheFlags;
    pVdData->CacheTimeout0 = CacheTimeout0;
    pVdData->CacheTimeout1 = CacheTimeout1;
    pVdData->CacheXferSize = CacheXferSize;

    return( rc );
}


/*******************************************************************************
 *
 *  DriverPoll
 *
 *  The Winstation driver calls DriverPoll
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to procotol driver data structure
 *    pVdPoll (input)
 *       pointer to the structure DLLPOLL  (not used)
 *
 * EXIT:
 *    CLIENT_STATUS_NO_DATA - nothing to do
 *    CLIENT_STATUS_SUCCESS - did something successfuly
 *
 ******************************************************************************/

static int
DriverPoll( PVD pVd, PDLLPOLL pVdPoll )
{
    USHORT ByteCount;
    int rc;

    /*
     *  Check if ring buffer is empty
     */
    if ( rc = RingBufReadAvail( &ByteCount ) )
        return( rc );

    TRACE(( TC_CDM, TT_ICOOK, "VDCDM: Virtual Write (from RINGBUF), %u", ByteCount ));

    /*
     *  Attempt to reserve enough output buffers
     */
    if ( OutBufReserve( pWd, (USHORT)(ByteCount+4) ) )
        return( CLIENT_STATUS_NO_DATA );

    /*
     *  Append Virtual write header
     */
    if ( rc = AppendVdHeader( pWd, VirtualCdm, ByteCount ) )
        return( rc );

    /*
     *  Read data from ring buffer and append to output buffer
     */
    if ( rc = RingBufRead() ) {
        TRACE(( TC_CDM, TT_ICOOK, "VDCDM: RINGBUF READ FAILURE!!! rc %d", rc ));
        return( rc );
    }

    /*
     *  Write last outbuf
     */
    return( OutBufWrite( pWd ) );
}


/*******************************************************************************
 *
 *  DriverQueryInformation
 *
 *   Queries information about the virtual driver - called from VdQueryInformation
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pVdQueryInformation (input/output)
 *       pointer to the structure VDQUERYINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DriverQueryInformation( PVD pVd, PVDQUERYINFORMATION pVdQueryInformation )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DriverSetInformation
 *
 *   Sets information for us, from VdSetInformation
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pVdSetInformation (input/output)
 *       pointer to the structure VDSETINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DriverSetInformation( PVD pVd, PVDSETINFORMATION pVdSetInformation )
{
   PVOID pVoid = pVdSetInformation->pVdInformation;

   switch ( pVdSetInformation->VdInformationClass ) {

       case VdGetSecurityAccess :
           break;

       case VdSetSecurityAccess :

           if ( pVdSetInformation->VdInformationLength != sizeof(ULONG) ) {
              (void) CdmSecuritySetAccess( CDM_SECURITY_ACCESS_NONE );
           }
           else {
              (void) CdmSecuritySetAccess( (ULONG) *((PULONG)pVoid) );
           }
           break;
   }
   return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DriverGetLastError
 *
 *   Queries error data.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pVdQueryInformation (input/output)
 *       pointer to the structure VDQUERYINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int DriverGetLastError( PVD pVd, PVDLASTERROR pLastError )
{
   // interpret last error and pass back code and string data
   pLastError->Error = pVd->LastError;
   pLastError->Message[0] = '\0';
   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  ICADataArrival
 *
 *   A data PDU arrived over one of our channels that we service.
 *
 *   Dispatch it into the main CDM transport code.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *
 *    uChan (input)
 *       ICA channel the data is for. (Should be VirtualCdm)
 *
 *    pBuf (input)
 *       Buffer with arriving data packet
 *
 *    Length (input)
 *       Length of the data packet
 *
 * EXIT:
 *
 ******************************************************************************/

STATIC void WFCAPI ICADataArrival( LPVOID pVd, USHORT uChan, LPBYTE pBuf, USHORT Length )
{

    ASSERT( uChan == VirtualCdm, uChan );
    ASSERT( CdmTransportHandle, 0 );

    /*
     * Call the Transport Data arrive function
     */
    CdmTransportReceiveData( CdmTransportHandle, pBuf, Length );
}


/*****************************************************************************
 *
 *  CdmICAWrite
 *
 *   This module writes data to the ICA.
 *
 * ENTRY:
 *   pBuffer (input)
 *     Pointer to data buffer to write
 *
 *   Length (input)
 *     Amount of data in buffer
 *
 * EXIT:
 *   0 - no error
 *
 ****************************************************************************/


STATIC USHORT
CdmICAWrite( PUCHAR pBuffer, USHORT Length )
{
    int rc;

    ASSERT( pWd, 0 );

    /*
     *  If ring buffer is NOT empty or we can not reserve enough output
     *  buffers queue the data.
     */
    if ( !RingBufEmpty() || OutBufReserve( pWd, (USHORT)(Length+4) ) ) {
        TRACE(( TC_CDM, TT_ICOOK, "VDCDM: CdmICAWrite: queue virtual write, %u", Length ));
        return( RingBufWrite( pBuffer, Length ) );
    }

    TRACE(( TC_CDM, TT_API2, "VDCDM: CdmICAWrite, %u", Length ));

    /*
     *  Append Virtual write header
     */
    if ( rc = AppendVdHeader( pWd, VirtualCdm, Length ) ) {
        TRACE(( TC_CDM, TT_ICOOK, "VDCDM: CdmICAWrite AppendVdHeader Error %u", rc ));
        return( rc );
    }

    /*
     *  Append data to output buffers
     */
    if ( rc = OutBufAppend( pWd, pBuffer, Length ) ) {
        TRACE(( TC_CDM, TT_ICOOK, "VDCDM: CdmICAWrite OutBufAppend Error %u", rc ));
        return( rc );
    }

    /*
     *  Write last outbuf
     */
    rc = OutBufWrite( pWd );

#if DBG
    if( rc ) {
        TRACE(( TC_CDM, TT_ICOOK, "VDCDM: CdmICAWrite OutBufWrite Error %u", rc ));
    }
#endif

    return( rc );
}


