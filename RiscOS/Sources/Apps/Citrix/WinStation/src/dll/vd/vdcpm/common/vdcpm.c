
/*************************************************************************
*
* vdcpm.c
*
* Client Printer Mapping Virtual Driver
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author:  JohnR 05/06/94
*
* $Log$
* Revision 1.1  1998/03/10 16:20:43  smiddle
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
*  
*     Rev 1.8   15 Apr 1997 18:04:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.7   13 Nov 1996 09:18:32   richa
*  Updated Virtual channel code.
*  
*     Rev 1.6   04 Nov 1996 10:21:52   richa
*  Change to dynamic allocation of virtual channel.
*  
*     Rev 1.5   19 Dec 1995 18:20:38   JohnR
*  update
*
*     Rev 1.4   10 Nov 1995 12:35:16   JohnR
*  update
*
*     Rev 1.3   20 Jul 1995 15:24:40   bradp
*  update
*
*     Rev 1.2   18 Jul 1995 13:51:46   JohnR
*  update
*
*     Rev 1.1   11 Jul 1995 14:03:16   JohnR
*  update
*
*     Rev 1.0   10 Jul 1995 11:34:00   JohnR
*  Initial revision.
*
*     Rev 1.11   02 May 1995 13:57:48   butchd
*  update
*
*     Rev 1.10   28 Apr 1995 14:18:58   richa
*  Do Win16 & Win32.
*
*     Rev 1.9   06 Apr 1995 17:43:18   butchd
*  support for DOS, WIN16, and WIN32 builds
*
*     Rev 1.8   06 Apr 1995 11:42:32   butchd
*  global STATUS and ERROR code changes
*
*     Rev 1.7   31 Mar 1995 14:53:40   JohnR
*  update
*
*     Rev 1.6   25 Jan 1995 11:25:04   marcb
*  update
*
*     Rev 1.5   03 Oct 1994 09:49:36   bradp
*  update
*
*     Rev 1.4   01 Sep 1994 17:37:46   bradp
*  update
*
*
*************************************************************************/

/*
 *  Includes
 */

#include "cpmserv.h"
#include "cpmtrans.h"

#include "../../../../inc/biniapi.h"

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

USHORT CpmICAWrite( PUCHAR, UCHAR, USHORT );
USHORT CpmICAWindowOpen( UCHAR, USHORT );
CPM_BOOLEAN CpmICABufAvail( void );
int WFCAPI CpmPoll( PVOID, PVOID );
static void WFCAPI ICADataArrival( PVOID, USHORT, LPBYTE, USHORT );
static USHORT CpmHookLpt( PVD, int, int, int );
VOID CpmFlush( UCHAR Channel, UCHAR Mask );

PLIBPROCEDURE VdCpmDriverProcedures[VDDRIVER__COUNT] =
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

extern int WdCall( PVD pVd, int ProcIndex, PVOID pParam );
int CpmPollAllPorts( void );
void CpmGetLptMask( PUCHAR pLptMask );
void CpmGetComMask( PUCHAR pLptMask );

/*=============================================================================
==   Data
=============================================================================*/

static PVOID CpmTransportHandles[Virtual_Maximum] = { NULL };
static int MaxWindowSize = 100;
static char gcDefaultQueueName[128];
USHORT VirtualLPT1 = (USHORT)-1;
USHORT VirtualLPT2 = (USHORT)-1;
USHORT VirtualCOM1 = (USHORT)-1;
USHORT VirtualCOM2 = (USHORT)-1;

// See inc\wdapi.h and dll\wd\wdica30\wdica.c
// These are returned when we register our hook

static LPVOID pWd = NULL;
static POUTBUFRESERVEPROC OutBufReserve   = NULL;
static POUTBUFAPPENDPROC OutBufAppend     = NULL;
static POUTBUFWRITEPROC OutBufWrite       = NULL;
static PAPPENDVDHEADERPROC AppendVdHeader = NULL;

BYTE LptMask = 0;
BYTE ComMask = 0;


#if 0
/*
 *  
 */
typedef struct _CHANNELLIST {
    VIRTUALNAME VirtualName;
    USHORT      MaskValue;
    PUSHORT     pVChannel;
} CHANNELLIST, * PCHANNELLIST;

CHANNELLIST aLPTChannelList[] = {

//  Channel name,   Channel mask identifier, Address to store channel #
    VIRTUAL_LPT1,   1,  &VirtualLPT1,
    VIRTUAL_LPT2,   2,  &VirtualLPT2,
    "",             0,  NULL,
};

CHANNELLIST aCOMChannelList[] = {

//  Channel name,   Channel mask identifier, Address to store channel #
    VIRTUAL_COM1,   1,  &VirtualCOM1,
    VIRTUAL_COM2,   2,  &VirtualCOM2,
    "",             0,  NULL,
};
#endif


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
    USHORT Channel;
    WDQUERYINFORMATION wdqi;
    OPENVIRTUALCHANNEL OpenVirtualChannel;
    int i;
    int rc;

    TRACE(( TC_CPM, TT_API1, "VDCPM: DriverOpen Called!"));

    /*
     * Get our settings from the profile file
     */
    MaxWindowSize = bGetPrivateProfileInt(
                       pVdOpen->pIniSection,
                       INI_CPMWINDOWSIZE,
                       DEF_CPMWINDOWSIZE
		       );

#ifdef DOS
#else
    //
    // Under windows the user can set a queue name to print to
    //
    bGetPrivateProfileString(
        pVdOpen->pIniSection,
        INI_CPMQUEUE,
        DEF_CPMQUEUE,
        gcDefaultQueueName,
        sizeof(gcDefaultQueueName)
        );
#endif

    TRACE(( TC_CPM, TT_API1, "VDCPM: MaxWindowSize %d",MaxWindowSize));

    LptMask = 1;
    ComMask = 0;
    
    /*
     *  Initialize to zero
     */
    pVdOpen->ChannelMask = 0;
   
    /*
     * Now register all LPT/COM channels that are valid
     */
    wdqi.WdInformationClass = WdOpenVirtualChannel;
    wdqi.pWdInformation = &OpenVirtualChannel;
    wdqi.WdInformationLength = sizeof_OPENVIRTUALCHANNEL;

    OpenVirtualChannel.pVCName = VIRTUAL_LPT1;
    rc = WdCall( pVd, WD__QUERYINFORMATION, &wdqi );
    Channel = OpenVirtualChannel.Channel;

    VirtualLPT1 = Channel;
    CpmHookLpt( pVd, Channel, MaxWindowSize, MaxWindowSize );
    pVdOpen->ChannelMask |= ( 1L << Channel );
   
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

extern void CpmCloseAllContexts();

static int
DriverClose( PVD pVd, PDLLCLOSE pVdClose )
{
   int i;
   pVdClose;

   /*
    * Close any active printer contexts
    */

   CpmCloseAllContexts();

   /*
    * Close all transport handles
    */

   for( i=0; i < Virtual_Maximum; i++ ) {

       if( CpmTransportHandles[i] != NULL ) {
           DeleteTransport( CpmTransportHandles[i] );
           CpmTransportHandles[i] = NULL;
       }
   }

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
    pHeader->VersionL = VERSION_CLIENTL_VDCPM;
    pHeader->VersionH = VERSION_CLIENTH_VDCPM;

    /*
     *  Initialize virtual driver header
     */
    pVdData->Header.ChannelMask = pVd->ChannelMask;
    pFlow = &pVdData->Header.Flow;
    pFlow->Flow = VirtualFlow_Ack;
    pFlow->BandwidthQuota = 0;
    pFlow->data.Ack.MaxWindowSize = MaxWindowSize;
    pFlow->data.Ack.WindowSize = MaxWindowSize;

    /*
     *  Initialize virtual driver data
     */
    pVdData->LptMask = LptMask;
    pVdData->ComMask = ComMask;

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
    int rc = CLIENT_STATUS_NO_DATA;

    if( CpmPollAllPorts() == TRUE ) {
        rc = CLIENT_STATUS_SUCCESS;
    }

    return( rc );
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
    PVDFLUSH p;
    int rc = CLIENT_STATUS_SUCCESS;

    switch ( pVdSetInformation->VdInformationClass ) {

    case VdFlush:
                p = (PVDFLUSH)pVdSetInformation->pVdInformation;
                CpmFlush( p->Channel, p->Mask );
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

    ASSERT( CpmTransportHandles[uChan], 0 );

    CpmWireDataReceive( CpmTransportHandles[uChan], (UCHAR)uChan, pBuf, Length );
}


/*****************************************************************************
 *
 *  CpmICAWrite
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


USHORT
CpmICAWrite( PUCHAR pBuffer, UCHAR Channel, USHORT Length )
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
 *  CpmICAWindowOpen
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


USHORT
CpmICAWindowOpen( UCHAR Channel, USHORT WindowSize )
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
 *  CpmHookLpt
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

static USHORT
CpmHookLpt( PVD pVd, int Channel, int TotalCount, int MaxCount )
{
   WDSETINFORMATION wdsi;
   VDWRITEHOOK vdwh;
   int rc;

   /*
    * Setup our internal structures first
    */

   rc = SetupTransport( (UCHAR)Channel,
                        (USHORT)TotalCount,
                        (USHORT)MaxCount,
                        &CpmTransportHandles[Channel] );

   if( rc ) {
       TRACE(( TC_CPM, TT_ERROR, "VDCPM: Can't Setup Client Printer Mapping" ));
       return( rc );
   }

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
