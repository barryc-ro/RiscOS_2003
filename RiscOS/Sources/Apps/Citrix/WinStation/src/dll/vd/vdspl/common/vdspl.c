
/*************************************************************************
*
* vdspl.c
*
* Client Printer Mapping Virtual Driver
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author:  JohnR 05/06/94
*
*  John Richardson 09/19/95
*    Rewrote to support client-server model printers and
*    Windows like print queues. This is to allow better integration
*    of Windows printers into WinFrame.
*
* $Log$
* Revision 1.1  1998/03/10 16:20:53  smiddle
* Redid the !Run files to allow command line options to be configured externally.
* Added cpm and spl virtual drivers - two versions of the printer spooling mechanism.
* Not properly tested but client still works if ClientPrinter is enabled. Doesn't work if
* ClientPrinter1.5 is enabled though.
* Made Module list in session.c dependant on #defines set in the Makefile.
* Split off exported defines into winframe.h which is exported to WinFrameRO and thence to
* the Export directory.
* Added a Message control interface to main.c. Not complete or tested.
* Added a loop forever option (CLI) for Xemplar.
* Added support for an ica: pseudo-URL scheme. ANT protocol only at the moment.
*
* Version 0.14. Tagged as 'WinStation-0_14'
*
* Revision 1.1  1997/11/27 16:57:04  smiddle
* Initial revision
*
*  
*     Rev 1.5   15 Apr 1997 18:05:20   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.4   13 Nov 1996 09:06:58   richa
*  Updated Virtual channel code.
*  
*     Rev 1.2   08 May 1996 14:27:22   jeffm
*  update
*  
*     Rev 1.1   14 Nov 1995 19:12:02   JohnR
*  update
*
*
*************************************************************************/

/*
 *  Includes
 */

#include "cpmserv.h"

#include "../../../../inc/biniapi.h"

#include "../../../../inc/client.h"

#define NO_VDDRIVER_DEFINES
#include "../../../../inc/vddriver.h"
#include "../../../../inc/vddriverp.h"

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

USHORT SplICAWrite( PUCHAR, UCHAR, USHORT );
USHORT SplICAWindowOpen( UCHAR, USHORT );
CPM_BOOLEAN SplICABufAvail( void );
static void WFCAPI ICADataArrival( PVOID, USHORT, LPBYTE, USHORT );
USHORT SplHookChannel( PVD, int );
VOID SplFlush( UCHAR Channel, UCHAR Mask );

PLIBPROCEDURE VdSplDriverProcedures[VDDRIVER__COUNT] =
{
    (PLIBPROCEDURE)DriverOpen,
    (PLIBPROCEDURE)DriverClose,
    (PLIBPROCEDURE)DriverInfo,
    (PLIBPROCEDURE)DriverPoll,
    (PLIBPROCEDURE)DriverSetInformation,
    (PLIBPROCEDURE)DriverQueryInformation,
    (PLIBPROCEDURE)DriverGetLastError   
};

/*=============================================================================
==   External Functions used
=============================================================================*/

extern int WdCall( PVD pVd, USHORT ProcIndex, PVOID pParam );

extern int SplPollAllPorts( );

/*=============================================================================
==   Data
=============================================================================*/

static int MaxWindowSize = 1024;
static int WindowSize    =  512;
USHORT VirtualCpm = (USHORT)-1;

char gcDefaultQueueName[128];

// See inc\wdapi.h and dll\wd\wdica30\wdica.c
// These are returned when we register our hook

static LPVOID pWd = NULL;
static POUTBUFRESERVEPROC OutBufReserve   = NULL;
static POUTBUFAPPENDPROC OutBufAppend     = NULL;
static POUTBUFWRITEPROC OutBufWrite       = NULL;
static PAPPENDVDHEADERPROC AppendVdHeader = NULL;

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
   int rc;
   WDQUERYINFORMATION wdqi;
   OPENVIRTUALCHANNEL OpenVirtualChannel;

   TRACE(( TC_CPM, TT_API1, "VDCPM: DriverOpen Called!"));

   /*
    * Get our settings from the profile file
    */
   MaxWindowSize = bGetPrivateProfileInt(
                       pVdOpen->pIniSection,
                       INI_CPMMAXWINDOWSIZE,
                       DEF_CPMMAXWINDOWSIZE
		       );

   WindowSize =    bGetPrivateProfileInt(
                       pVdOpen->pIniSection,
                       INI_CPMWINDOWSIZE,
                       DEF_CPMWINDOWSIZE
		       );

   //
   // Under windows the user can set a queue name to print to
   // Under RISCOS use this for driver override
   bGetPrivateProfileString(
       pVdOpen->pIniSection,
       INI_CPMQUEUE,
       DEF_CPMQUEUE,
       gcDefaultQueueName,
       sizeof(gcDefaultQueueName)
       );

   TRACE(( TC_CPM, TT_API1, "VDCPM: MaxWindowSize %d, WindowSize %d, Queue '%s'",MaxWindowSize,WindowSize,gcDefaultQueueName));

   /*
    *  Initialize to zero
    */
   pVdOpen->ChannelMask = 0;

   /*
    * Get a virtual channel
    */
   wdqi.WdInformationClass = WdOpenVirtualChannel;
   wdqi.pWdInformation = &OpenVirtualChannel;
   wdqi.WdInformationLength = sizeof_OPENVIRTUALCHANNEL;
   OpenVirtualChannel.pVCName = VIRTUAL_CPM;
   rc = WdCall( pVd, WD__QUERYINFORMATION, &wdqi );
   VirtualCpm = OpenVirtualChannel.Channel;
   ASSERT( VirtualCpm == Virtual_Cpm, VirtualCpm );

   /*
    * Hook our channel
    */
   SplHookChannel( pVd, VirtualCpm );
   pVdOpen->ChannelMask |= (1L << VirtualCpm);

   TRACE(( TC_CPM, TT_API1, "VDCPM: mask 0x%lx", pVdOpen->ChannelMask ));

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

extern void SplCloseAllContexts();

static int 
DriverClose( PVD pVd, PDLLCLOSE pVdClose )
{

   /*
    * Close any active printer contexts
    */

   SplCloseAllContexts();

   TRACE(( TC_CPM, TT_API1, "CpmClose: Done closing..."));

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
    USHORT ByteCount;
    PVDCPM_C2H pVdData;
    PMODULE_C2H pHeader;
    PVDFLOW pFlow;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof_VDCPM_C2H;

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
    pVdData = (PVDCPM_C2H) pVdInfo->pBuffer;
    memset( pVdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pVdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_VirtualDriver;
    pHeader->VersionL = VERSION_CLIENTL_VDSPL;
    pHeader->VersionH = VERSION_CLIENTH_VDSPL;

    /*
     *  Initialize virtual driver header
     */
    pVdData->Header.ChannelMask = pVd->ChannelMask;
    pFlow = &pVdData->Header.Flow;
    pFlow->Flow = VirtualFlow_Ack;
    pFlow->BandwidthQuota = 0;
    pFlow->data.Ack.MaxWindowSize = MaxWindowSize;
    pFlow->data.Ack.WindowSize = WindowSize;

    /*
     *  Initialize virtual driver data
     */
    pVdData->LptMask = 0;
    pVdData->ComMask = 0;

    return( CLIENT_STATUS_SUCCESS );
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

    /*
     * Poll the ports in case any processing is needed.
     * When we return CLIENT_STATUS_SUCCESS in this case, the client
     * will not yield, since it believes we are busy doing usefull work
     * and would like another poll again as soon as possible.
     */
    if( SplPollAllPorts() ) {
        return( CLIENT_STATUS_SUCCESS );
    }

    /*
     * This will allow the client to yield, since we are not "active"
     */
    return( CLIENT_STATUS_NO_DATA );
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
    return(CLIENT_STATUS_SUCCESS);
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
//    PVDFLUSH p;
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pVdSetInformation->VdInformationClass ) {

    case VdFlush:
//                p = (PVDFLUSH)pVdSetInformation->pVdInformation;
//                SplFlush( p->Channel, p->Mask );
                break;

    default:
                break;

   }

   return( rc );
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
 *   Dispatch it into the main CPM server code.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *
 *    uChan (input)
 *       ICA channel the data is for.
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

static void WFCAPI ICADataArrival( PVOID pVd, USHORT uChan, LPBYTE pBuf, USHORT Length )
{

    SplWireDataReceive( (UCHAR)uChan, pBuf, Length );
}


/*****************************************************************************
 *
 *  SplICAWrite
 *
 *   This module writes data to the ICA doing a VirtualWrite()
 *
 * ENTRY:
 *   pBuffer (input)
 *     Pointer to data buffer to write
 *
 *   Channel (input)
 *     ICA channel to write out on
 *
 *   Length (input)
 *     Amount of data in buffer
 *
 * EXIT:
 *   0 - no error
 *
 ****************************************************************************/


USHORT STATIC 
SplICAWrite( PUCHAR pBuffer, UCHAR Channel, USHORT Length )
{
    int rc;

    ASSERT( pWd, 0 );

    /*
     * Try and get the output buffer
     */
    rc = OutBufReserve( pWd, (USHORT)(Length+4) );

    if ( rc ) {
        TRACE(( TC_CPM, TT_API2, "VDCPM: queue virtual write, %u, chan %d", Length, Channel ));
        return( rc );
    }

    TRACE(( TC_CPM, TT_API2, "VDCPM: Virtual Write, %u, Channel %d", Length, Channel ));

    /*
     *  Append Virtual write header
     */
    if ( rc = AppendVdHeader( pWd, (USHORT)Channel, Length ) )
        return( rc );

    /*
     *  Append data to output buffers
     */
    if ( rc = OutBufAppend( pWd, pBuffer, Length ) )
        return( rc );

    /*
     *  Write last outbuf
     */
    return( OutBufWrite( pWd ) );
}


/*****************************************************************************
 *
 *  SplICAWindowOpen
 *
 *    This function opens the ICA send window by sending the ack on
 *    the given channel
 *
 * ENTRY:
 *
 *   Channel (input)
 *     ICA channel to write out on
 *
 *   WindowSize (input)
 *     Window Size to open
 *
 * EXIT:
 *   0 - no error
 *
 ****************************************************************************/


USHORT STATIC 
SplICAWindowOpen( UCHAR Channel, USHORT WindowSize )
{
    int rc;
    UCHAR Buf[4];

    ASSERT( pWd, 0 );

    /*
     * Try and get the output buffer
     */
    rc = OutBufReserve( pWd, 4 );

    if ( rc ) {

        /*
         * If we could not get a buffer, return the error. The higher
         * level will try later
         */
        return( rc );
    }

    /*
     * Construct the Ack packet, and send it
     *
     * NOTE: - This has ICA 3.0 protocol specifics embedded here
     */

    Buf[0] = PACKET_VIRTUAL_ACK;
    Buf[1] = Channel;
    Buf[2] = (UCHAR)(WindowSize & 0x00FF);  // Low byte first
    Buf[3] = (UCHAR)((WindowSize >> 8) & 0x00FF);

    /*
     *  Append data to output buffer
     */
    if ( rc = OutBufAppend( pWd, Buf, sizeof( Buf ) ) )
        return( rc );

    /*
     *  Write last outbuf
     */
    return( OutBufWrite( pWd ) );
}

/*****************************************************************************
 *
 *  SplHookChannel
 *
 *   Performs the work of "hooking" and ICA virtual channel
 *
 * ENTRY:
 *   pVd (input)
 *     Pointer to the Virtual Driver structure
 *
 *   Channel (input)
 *     Which channel number to hook
 *
 *   TotalCount (input)
 *     Total number of bytes we can have outstanding
 *
 *   MaxCount (input)
 *     Maximum number of bytes we can have outstanding
 *
 * EXIT:
 *   CLIENT_STATUS_SUCCESS - no error
 *
 ****************************************************************************/

USHORT STATIC 
SplHookChannel( PVD pVd, int Channel )
{
   WDSETINFORMATION wdsi;
   VDWRITEHOOK vdwh;
   int rc;

   /*
    * Now setup ICA
    */
   pVd->pPrivate   = NULL;          // Fill in ptr to our data if needed

   /*
    *  Register write hooks for all virtual channels handled by this vd
    */
   vdwh.Type  = Channel;
   vdwh.pVdData = pVd;
   vdwh.pProc = (PVDWRITEPROCEDURE) ICADataArrival;
   wdsi.WdInformationClass  = WdVirtualWriteHook;
   wdsi.pWdInformation      = &vdwh;
   wdsi.WdInformationLength = sizeof(VDWRITEHOOK);
   rc = WdCall(pVd, WD__SETINFORMATION, &wdsi);
   TRACE(( TC_CPM, TT_API2, "VDCPM: writehook ch=%u p=%lx rc=%u", vdwh.Type, vdwh.pVdData, rc ));

   if( rc ) {
        TRACE(( TC_CPM, TT_ERROR, "VDCPM: Could not register to the WD. rc %d, Channel %d", rc, Channel));
        return( rc );
   }

   // This returns pointers to functions to use to send data to the host
   pWd            = vdwh.pWdData;
   OutBufReserve  = vdwh.pOutBufReserveProc;
   OutBufAppend   = vdwh.pOutBufAppendProc;
   OutBufWrite    = vdwh.pOutBufWriteProc;
   AppendVdHeader = vdwh.pAppendVdHeaderProc;

   ASSERT( pWd != NULL, 0 );
   ASSERT( OutBufReserve != NULL, 0 );
   ASSERT( OutBufAppend != NULL, 0 );
   ASSERT( OutBufWrite != NULL, 0 );
   ASSERT( AppendVdHeader != NULL, 0 );

   return( rc );
}

