/*************************************************************************
*
*   init.c
*  
*   ICA 3.0 WinStation Driver - init and terminate packets
*  
*    PACKET_INIT_REQUEST   nn host->client init packet
*    PACKET_INIT_RESPONSE  nn client->host init packet
*    PACKET_INIT_CONNECT   nn host->client connect packet
*  
*    PACKET_TERMINATE      n  terminate request
*  
*  
*   Copyright 1995, Citrix Systems Inc.
*  
*   Author: Brad Pedersen (4/9/94)
*
*   $Log$
*  
*     Rev 1.65   27 May 1998 17:40:06   terryt
*  Change unicode version support to 6
*  
*     Rev 1.64   06 May 1998 13:20:36   terryt
*  try smaller buffer if low on memory for reducer
*  
*     Rev 1.63   28 Apr 1998 11:34:40   terryt
*  adjust check for memory space to include header-trailer
*  
*     Rev 1.62   26 Apr 1998 15:08:24   terryt
*  Free PDCOMP buffers for reducer
*  
*     Rev 1.61   25 Apr 1998 17:55:28   terryt
*  Only allocate reducer buffers if free space is available
*  
*     Rev 1.60   21 Apr 1998 17:30:50   terryt
*  add trace for memory
*  
*     Rev 1.59   20 Apr 1998 17:54:58   terryt
*  dos memory fix for reducer
*  
*     Rev 1.58   14 Apr 1998 21:19:04   derekc
*  added SwitchUiToUnicode() function
*  
*     Rev 1.57   13 Apr 1998 18:28:38   kurtp
*  add UK reducer code
*  
*     Rev 1.54   15 Apr 1997 18:17:50   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.54   21 Mar 1997 16:09:48   bradp
*  update
*  
*     Rev 1.53   28 Aug 1996 15:09:24   marcb
*  update
*  
*     Rev 1.52   08 May 1996 15:20:24   jeffm
*  update
*  
*     Rev 1.51   07 Aug 1995 20:44:02   bradp
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
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"
#include "citrix/ica-h2c.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/xmsapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/logapi.h"
#include "../inc/wd.h"
#include "wdica.h"

#include "../../../inc/reducapi.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

void IcaInitRequest( PWD, LPBYTE, USHORT );
int SendNextInitResponse( PWD );
void IcaInitConnect( PWD, LPBYTE, USHORT );
void IcaTerminate( PWD, LPBYTE, USHORT );
void TerminateAck( PWD );
void ActivateExpansion(PWD, ULONG);
void ActivateReduction(PWD, ULONG, USHORT);

/*=============================================================================
==   External Functions Used
=============================================================================*/

int PdCall( PWD, USHORT, PVOID );
int DllCall( PDLLLINK, USHORT, PVOID );
int AppendICAPacket( PWD, USHORT, LPBYTE, USHORT );
int WFCAPI OutBufReserve( PWD, USHORT );
int WFCAPI OutBufAppend( PWD, LPBYTE, USHORT );
int WFCAPI OutBufWrite( PWD );
int VdCall( PWD, USHORT, USHORT, PVOID );
void GetExpIniData(BYTE*, BYTE*);

#ifdef UNICODESUPPORT
int SwitchUiToUnicode();
#endif

/*******************************************************************************
 *
 *  IcaInitRequest  (PACKET_INIT_REQUEST)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaInitRequest( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;
    USHORT ByteCount;
    int rc;
    VDTWCACHE ThinWireCache;
    VDSETINFORMATION Info;
    BYTE locC2H_PowerOf2Wanted;
    BYTE locH2C_PowerOf2Wanted;
    USHORT locH2C_MaxNewData;
    ULONG halfBufferSize;
    ULONG bufferSize;
    LPBYTE buffer;
    PDSETINFORMATION PdSetInfo;
#ifdef DOS
    ULONG LowHole;
#endif

    pIca = (PWDICA) pWd->pPrivate;

    if ( pIca->pModuleList != NULL ) {
        TRACE(( TC_WD, TT_API1, "IcaInitRequest: INIT_REQUEST duplicate received" ));
        return;
    }

    /*
     * Level information
     *
     * For now the first byte is all we define
     */
    if ( InputCount >= 1 ) {
        pWd->EncryptionLevel = pInputBuffer[0];

#ifdef UNICODESUPPORT

        /*
         * Only MetaFrame Servers level 6 and up
         * support Unicode
         */
        if ( pInputBuffer[0] >= 6 ) {
            
            WdKbdSetMode ( pIca, Kbd_Unicode );

            // at this point, we finally know that
            // we are using Unicode.  however, the
            // UI has ASCII information.  we need
            // to tell wengine.c to change the
            // G_pUiData structure somehow.
            SwitchUiToUnicode();

        }
#endif

    }

    TRACE(( TC_WD, TT_API1, "IcaInitRequest: encryption level %d (%sspecified)", pWd->EncryptionLevel, InputCount >= 1 ? "" : "un" ));
   
    /*
     * Initialize memory for reducer
     */

    if (
	(pReduceProcedures == NULL) ||
	(pWd->EncryptionLevel < 5)) 
    {
        /* dll not there or host will not support it */
        locC2H_PowerOf2Wanted = 0;
        locH2C_PowerOf2Wanted = 0;
    }
    else {

        /*
         *  Read out the expansion/reduction data from the ini file
         */
        GetExpIniData(&locH2C_PowerOf2Wanted, &locC2H_PowerOf2Wanted);

        /*
         *  Tell PDCOMP that reducer is enabled so it can free memory
         */
        if ( locH2C_PowerOf2Wanted && locC2H_PowerOf2Wanted ) {
            PdSetInfo.PdInformationClass  = PdReducerEnabled;
            PdSetInfo.pPdInformation      = NULL;
            PdSetInfo.PdInformationLength = 0;
            rc = PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );
        }
    }

    locH2C_MaxNewData = DEFAULT_MAX_NEW_DATA;
    buffer = NULL;
    if (locH2C_PowerOf2Wanted > 0) 
    {
        if (locH2C_PowerOf2Wanted > MAX_H2C_POW2_SUPPORTED)
            locH2C_PowerOf2Wanted = MAX_H2C_POW2_SUPPORTED;
        if (locH2C_PowerOf2Wanted < MIN_H2C_POW2_SUPPORTED )
            locH2C_PowerOf2Wanted = MIN_H2C_POW2_SUPPORTED;

        /* claim memory now to make sure it will be there when needed */
        bufferSize = raExpanderDataQueueMemSize(locH2C_PowerOf2Wanted);

#ifdef DOS
        /*
         * DOS must be careful about memory.
         */
        QueryHeap( NULL, &ThinWireCache.ulLowMem, &LowHole );

        /*
         * The QueryHeap function does not compensate for the header/trailer.
         */
        LowHole = ( LowHole & 0xfffffffc ) - 12;

        TRACE((TC_WD, TT_API1, "bufferSize %ld ulLowMem %ld LowMemReserve %ld LowMemHole %ld", bufferSize, ThinWireCache.ulLowMem, pWd->LowMemReserve, LowHole));

        if ( ( bufferSize > ThinWireCache.ulLowMem ) ||
             ( ThinWireCache.ulLowMem - bufferSize < pWd->LowMemReserve ) ||
             ( bufferSize > LowHole ) ) {

             TRACE((TC_WD, TT_API1, "buffersize %ld (pow2 %d) too big for avail memory", bufferSize, locH2C_PowerOf2Wanted));

            /* if there wasn't enough mem for expander, there certainly
             * won't be enough for reducer
             */
            locC2H_PowerOf2Wanted = 0;

            /* We'd like to have the expander working if possible. Try reducing
             * the buffer size wanted
             */
            if (locH2C_PowerOf2Wanted > MIN_H2C_POW2_SUPPORTED) {
                locH2C_PowerOf2Wanted --;
                bufferSize = raExpanderDataQueueMemSize(locH2C_PowerOf2Wanted);
                if ( ( bufferSize > ThinWireCache.ulLowMem ) ||
                     ( ThinWireCache.ulLowMem - bufferSize < pWd->LowMemReserve ) ||
                     ( bufferSize > LowHole ) ) {

                     TRACE((TC_WD, TT_API1, "buffersize %ld (pow2 %d) still too big", bufferSize, locH2C_PowerOf2Wanted));
    
                    if (locH2C_PowerOf2Wanted > MIN_H2C_POW2_SUPPORTED) {
                        /* one last try... */
                        locH2C_PowerOf2Wanted --;
                        bufferSize = raExpanderDataQueueMemSize(locH2C_PowerOf2Wanted);
                        if ( ( bufferSize > ThinWireCache.ulLowMem ) ||
                             ( ThinWireCache.ulLowMem - bufferSize < pWd->LowMemReserve ) ||
                             ( bufferSize > LowHole ) ) {
                             TRACE((TC_WD, TT_API1, "buffersize %ld (pow2 %d) yet still too big, giving up on expander", bufferSize, locH2C_PowerOf2Wanted));
                             bufferSize = 0;     /* forget it */
                        }
                    }
                    else        /* below supported min size */
                        bufferSize = 0;
                }
            }
            else        /* below supported min size */
                bufferSize = 0;

        }
#endif  /*DOS*/

        halfBufferSize = ((ULONG)1 << (locH2C_PowerOf2Wanted - 1));
        if (locH2C_MaxNewData > halfBufferSize)
            locH2C_MaxNewData = (USHORT) halfBufferSize;

        /*
         * Limit DOS and WIN16 to less than 64K.
         */
#if defined(DOS) || defined(WIN16)
        if ( (bufferSize != 0) && ( bufferSize < 64L*1024L ) )
        {
            buffer = (LPBYTE) malloc((USHORT)bufferSize);
        }
#else
        if (bufferSize != 0) 
        {
            buffer = (LPBYTE) malloc(bufferSize);
        }
#endif
        if (buffer) 
        {
#if defined(DOS) || defined(WIN16)
            memset(buffer, 0, (USHORT)bufferSize);
#else
            memset(buffer, 0, bufferSize);
#endif
        }
        else
        {
            /* no memory - turn everything off */
            TRACE((TC_WD, TT_API1, "Client expansion/reduction disabled as no memory for exp buffer pow2 %d", locH2C_PowerOf2Wanted));
            locC2H_PowerOf2Wanted = 0;
            locH2C_PowerOf2Wanted = 0;
            locH2C_MaxNewData = DEFAULT_MAX_NEW_DATA;
        }
    }
    pWd->ExpanderBuffer = buffer;
       
    buffer = NULL;
    if (locC2H_PowerOf2Wanted > 0) 
    {
        if (locC2H_PowerOf2Wanted > MAX_C2H_POW2_SUPPORTED)
            locC2H_PowerOf2Wanted = MAX_C2H_POW2_SUPPORTED;
        if (locC2H_PowerOf2Wanted < MIN_C2H_POW2_SUPPORTED )
            locC2H_PowerOf2Wanted = MIN_C2H_POW2_SUPPORTED;

        /* claim memory now to make sure it will be there when needed */

        bufferSize = raReducerDataQueueMemSize(locC2H_PowerOf2Wanted, REDUCER_RING_BUF_POW2);

#ifdef DOS
        /*
         * DOS must be careful about memory.
         */
        QueryHeap( NULL, &ThinWireCache.ulLowMem, &LowHole );

        /*
         * The QueryHeap function does not compensate for the header/trailer.
         */
        LowHole = ( LowHole & 0xfffffffc ) - 12;

        TRACE((TC_WD, TT_API1, "bufferSize %ld ulLowMem %ld LowMemReserve %ld LowMemHole %ld", bufferSize, ThinWireCache.ulLowMem, pWd->LowMemReserve, LowHole));

        if ( ( bufferSize > ThinWireCache.ulLowMem ) ||
             ( ThinWireCache.ulLowMem - bufferSize < pWd->LowMemReserve ) ||
             ( bufferSize > LowHole ) )
            bufferSize = 0;
#endif

        /*
         * Limit DOS and WIN16 to less than 64K.
         */
#if defined(DOS) || defined(WIN16)
        if ( (bufferSize != 0) && ( bufferSize < 64L*1024L ) )
        {
            buffer = (LPBYTE) malloc((USHORT)bufferSize);
        }
#else
        if (bufferSize != 0) 
        {
            buffer = (LPBYTE) malloc(bufferSize);
        }
#endif
        if (buffer) 
        {
#if defined(DOS) || defined(WIN16)
            memset(buffer, 0, (USHORT)bufferSize);
#else
            memset(buffer, 0, bufferSize);
#endif
        }
        else        
        {
            /* no memory - turn reducer off */
            TRACE((TC_WD, TT_API1, "Client reducer disabled as no memory for buffer, pow2 %d", locC2H_PowerOf2Wanted));
            locC2H_PowerOf2Wanted = 0;
        }
    }
    pWd->ReducerBuffer = buffer;

    pWd->H2C_PowerOf2Wanted = locH2C_PowerOf2Wanted;
    pWd->C2H_PowerOf2Wanted = locC2H_PowerOf2Wanted;
    pWd->H2C_MaxNewData = locH2C_MaxNewData;

#ifdef DOS
    /*
     *  Query available xms memory
     */
    (void) XmsQuery( &ThinWireCache.ulXms );
    if ( ThinWireCache.ulXms > pWd->XmsReserve )
        ThinWireCache.ulXms -= pWd->XmsReserve;
    else
        ThinWireCache.ulXms = 0;

    /*
     *  Query available low memory
     */
    QueryHeap( NULL, &ThinWireCache.ulLowMem, NULL );
    if ( ThinWireCache.ulLowMem > pWd->LowMemReserve )
        ThinWireCache.ulLowMem -= pWd->LowMemReserve;
    else
        ThinWireCache.ulLowMem = 0;
#else
    ThinWireCache.ulLowMem = 0;
    ThinWireCache.ulXms = 0;
#endif


    /*
     *  Initialize thinwire caches
     */
    TRACE(( TC_WD, TT_API1, "ThinWireCache: xms %lu, low %lu",
            ThinWireCache.ulXms, ThinWireCache.ulLowMem ));
    Info.VdInformationClass  = VdThinWireCache,
    Info.pVdInformation      = &ThinWireCache;
    Info.VdInformationLength = sizeof(VDTWCACHE);
    rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
    if ( rc == CLIENT_ERROR_VD_NOT_FOUND ) {
        rc = CLIENT_STATUS_SUCCESS;
    } else if ( rc != CLIENT_STATUS_SUCCESS ) {
        /*
         *  It would be nice to do more here
         */
        ASSERT( FALSE, 0 );
        TRACE(( TC_WD, TT_API1, "ThinWireCache: rc = %d", rc ));
    }


    /*
     *  Cancel ica detect timer
     */
    pIca->TimerIcaDetect = 0;

    /*
     *  Get the size of the necessary buffer
     */
    rc = ModuleEnum( NULL, 0, &ByteCount );
    ASSERT( rc == CLIENT_ERROR_BUFFER_TOO_SMALL, rc );
    ASSERT( ByteCount >= sizeof(DLLLINK), 0 );

    /*
     *  Allocate buffer for dll enumeration
     */
    ASSERT( pIca->pModuleList == NULL, 0 );
    if ( (pIca->pModuleList = malloc(ByteCount)) == NULL ) {
        ASSERT( FALSE, 0 );
        return;
    }

    /*
     *  Enumerate all the loaded dlls
     */
    if ( rc = ModuleEnum( (LPBYTE) pIca->pModuleList, ByteCount, NULL ) ) {
        ASSERT( FALSE, rc );
        free( pIca->pModuleList );
        pIca->pModuleList = NULL;
        return;
    }

    pIca->ModuleCount = ByteCount / ENUM_DLLLINK_SIZE;
    ASSERT( pIca->ModuleCount > 0, 0 );

    TRACE(( TC_WD, TT_API1, "INIT_REQUEST: %u modules", pIca->ModuleCount ));
}


/*******************************************************************************
 *
 *  SendNextInitResponse  (PACKET_INIT_RESPONSE)
 *
 *  -- This routine is called by the WdPoll routine
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

int
SendNextInitResponse( PWD pWd )
{
    PWDICA pIca;
    PDLLLINK pDllLink;
    DLLINFO DllInfo;
    PMODULE_C2H pHeader;
    BYTE str[2];
    int rc;

    pIca = (PWDICA) pWd->pPrivate;

    ASSERT( pIca->ModuleCount > 0, 0 );
    ASSERT( pIca->pModuleList != NULL, 0 );

    /*
     *  Get pointer to next module
     *  we use this strange calculation to allow DLLLINK to grow
     *  but the ENUM size stays the same
     */
    pDllLink = (PDLLLINK) (((ULONG)pIca->pModuleList)
               + ((pIca->ModuleCount - 1) * ENUM_DLLLINK_SIZE));


    TRACE(( TC_WD, TT_API2, "Module[%u]: %s, size %lu (enter)",
            pIca->ModuleCount-1, pDllLink->ModuleName, pDllLink->ModuleSize ));

    /*
     *  Query Dll - length of module data
     */
    DllInfo.pBuffer = NULL;
    DllInfo.ByteCount = 0;
    rc = DllCall( pDllLink, DLL__INFO, &DllInfo );
    if ( rc != CLIENT_ERROR_BUFFER_TOO_SMALL ) {
        ASSERT( FALSE, rc );
        goto skipmodule;
    }

    /*
     *  Check for zero module data
     */
    if ( DllInfo.ByteCount == 0 ) {
        ASSERT( FALSE, 0 );
        goto skipmodule;
    }

    /*
     *  Allocate buffer for module data
     */
    if ( (DllInfo.pBuffer = malloc( DllInfo.ByteCount )) == NULL ) {
        ASSERT( FALSE, DllInfo.ByteCount );
        rc = CLIENT_ERROR_NO_MEMORY;
        goto badmalloc;
    }

    /*
     *  Query Dll - module data
     */
    if ( rc = DllCall( pDllLink, DLL__INFO, &DllInfo ) ) {
        ASSERT( FALSE, rc );
        goto badquery;
    }

    /*
     *  Finish initializing module header
     */
    pHeader = (PMODULE_C2H) DllInfo.pBuffer;
    pHeader->ModuleCount = (BYTE) (pIca->ModuleCount - 1);
    strcpy( pHeader->ModuleName, pDllLink->ModuleName );
    pHeader->ModuleDate = pDllLink->ModuleDate;
    pHeader->ModuleTime = pDllLink->ModuleTime;
    pHeader->ModuleSize = pDllLink->ModuleSize;
    ASSERT( pHeader->ByteCount == DllInfo.ByteCount, 0 );

    /*
     *  Now reserve the total send size we need
     *  -- if reserve fails try module again later
     */
    if ( rc = OutBufReserve( pWd, (USHORT) (DllInfo.ByteCount + 3) ) ) {
        free( DllInfo.pBuffer );
        TRACE(( TC_WD, TT_API2, "Module[%u]: OutBufReserve failed, rc=%u",
                pIca->ModuleCount-1, rc ));
        goto badreserve;
    }

    /*
     *  Append module data
     */
    str[0] = LSB( DllInfo.ByteCount );
    str[1] = MSB( DllInfo.ByteCount );
    rc = AppendICAPacket( pWd, PACKET_INIT_RESPONSE, str, 2 );
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    rc = OutBufAppend( pWd, DllInfo.pBuffer, DllInfo.ByteCount );
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

    /*
     *  Now write the buffer
     */
    if ( rc = OutBufWrite( pWd ) ) {
        ASSERT( FALSE, rc );
        goto badwrite;
    }

    TRACE(( TC_WD, TT_API2,
            "Module[%u]: %s, class %u, ver %u-%u, bc %u",
            pIca->ModuleCount-1, pHeader->ModuleName, pHeader->ModuleClass,
            pHeader->VersionL, pHeader->VersionH, pHeader->ByteCount ));

    /*
     *  Free data buffer
     */
    free( DllInfo.pBuffer );

    /*
     *  advance to next module
     */
skipmodule:
    pIca->ModuleCount--;

    /*
     *  If this was the last module free module list
     */
    if ( pIca->ModuleCount == 0 ) {
        TRACE(( TC_WD, TT_API2, "Free module list" ));
        free( pIca->pModuleList );
        pIca->pModuleList = NULL;
    }

badreserve:
    return( CLIENT_STATUS_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  bad write
     *  bad module query
     */
badwrite:
badquery:
    free( DllInfo.pBuffer );

    /*
     *  bad memory alloc
     */
badmalloc:
    pIca->ModuleCount--;
    ASSERT( FALSE, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  IcaInitConnect  (PACKET_INIT_CONNECT)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaInitConnect( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PDSETINFORMATION PdSetInfo;
    PMODULE_H2C pHeader;
    PWD_H2C pWdHost;
    BYTE str[2];
    PWDICA pIca;
    int rc;
    static int expansionActivationBits = 0;
    static int reductionActivationBits = 0;
        static USHORT maxHeadroom = 0;

    TRACE(( TC_WD, TT_API1, "INIT_CONNECT: bc %u", InputCount ));

    pIca = (PWDICA) pWd->pPrivate;

    pHeader = (PMODULE_H2C) pInputBuffer;
    ASSERT( InputCount >= sizeof(MODULE_H2C), InputCount );

    switch ( pHeader->ModuleClass ) {

        case Module_ProtocolDriver :

            /*
             *  Initialize protocol drivers
             */
            PdSetInfo.PdInformationClass  = PdInitModule;
            PdSetInfo.pPdInformation      = pHeader;
            PdSetInfo.PdInformationLength = InputCount;
            rc = PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );
            ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
            break;

        case Module_WinstationDriver :

            pWdHost = (PWD_H2C) pHeader;
//          ASSERT( InputCount == sizeof(WD_H2C), InputCount ); SJM: remove this assert as structure size is not word-aligned
            if ( pWdHost->oAppServerName ) {
                if ( pWd->pAppServer )
                    free( pWd->pAppServer );
                pWd->pAppServer = strdup( (LPBYTE)pWdHost + pWdHost->oAppServerName );
                TRACE(( TC_WD, TT_API1, "RECONNECT to: %s", pWd->pAppServer ));
            }
//#ifdef INTERNET_CLIENT
            if ( (pWdHost->Header.Version < 5) ||
                 !(pWdHost->WdFlag & WDFLAG_SECURED) ) {
                pIca->fHostNotSecured = TRUE;
            }
//#endif 
            pWd->HostVersionL = 1;                        // Lowest connect version
            pWd->HostVersionH = pWdHost->Header.Version;  // Highest connect version  
            TRACE(( TC_WD, TT_API1, "WD: VersionL(%d) VersionH(%d)",
                    pWd->HostVersionL, pWd->HostVersionH ));

                        // Setup the expansion/reduction as per info from host
                        if ( pWdHost->Header.Version >= MIN_WD_REDUCER_SERVER_VERSION){  //DEBUG
                                if (pWdHost->H2C_PowerOf2Agreed > 0){
                                        expansionActivationBits = pWdHost->H2C_PowerOf2Agreed;
                                }
                                if ( pWdHost->C2H_PowerOf2Agreed > 0){
                                        reductionActivationBits = pWdHost->C2H_PowerOf2Agreed;
                                        maxHeadroom = pWdHost->C2H_MaxNewData;
                                }
                        } 
            break;
    }

    /*
     *  If this is the last init packet from host
     *  - send an acknowlegement packet
     *  - enable all the optional protocol drivers (e.g. compression) 
     *  - set the connected flag
     */
    if ( pHeader->ModuleCount == 0 ) {

        TRACE(( TC_WD, TT_API2, "Sending PACKET_INIT_CONNECT_RESPONSE"));

        /*
         *  Reserve the total send size we need
         */
        rc = OutBufReserve( pWd, (USHORT) (3) );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        /*
         *  Send response
         */
        str[0] = LSB( 0 );
        str[1] = MSB( 0 );
        rc = AppendICAPacket( pWd, PACKET_INIT_CONNECT_RESPONSE, str, 2 );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        /*
         *  Now write the buffer
         */
        if ( rc = OutBufWrite( pWd ) ) {
            ASSERT( FALSE, rc );
        }

    /*
         * Now enable all modules, after the acknowlege has been sent.
         * Since we aren't multi-threaded, we don't have to worry about
         * input data not being processed by these modules.  The response
         * must not be processed by the optional PD's, but any input must be.
         */
        PdSetInfo.PdInformationClass = PdEnableModule;
        rc = PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
        pWd->fSendStatusConnect = TRUE;

        // Setup the expansion/reduction as per info from host
        if (
	    pReduceProcedures &&
	    expansionActivationBits != 0 ){
            ActivateExpansion(pWd, expansionActivationBits);
                }
        if (
	    pReduceProcedures &&
	    reductionActivationBits != 0 ){
                        ActivateReduction(pWd, 
                                        reductionActivationBits, maxHeadroom);
                }
    }

}


/*******************************************************************************
 *
 *  IcaTerminate  (PACKET_TERMINATE)
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pInputBuffer (input)
 *       pointer to input data
 *    InputCount (input)
 *       byte count of input data
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
IcaTerminate( PWD pWd, LPBYTE pInputBuffer, USHORT InputCount )
{
    PWDICA pIca;

    TRACE(( TC_WD, TT_API1, "TERMINATE" ));
    ASSERT( InputCount == 1, InputCount );

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  If terminate ack (ignore for now)
     */
    if ( *pInputBuffer == TERMINATE_ACK ) 
        return;

    /*
     *  If logoff request (not disconnect)
     */
    if ( *pInputBuffer == TERMINATE_LOGOFF ) 
        pIca->fReceivedTerminate = TRUE;

    /*
     *  Check host version level
     */
    if ( pWd->HostVersionH < 4 ) {

        TerminateAck( pWd );

    } else {
        /*
         *  Set Send Terminate ACK Flag
         */
        pIca->fSendTerminateAck = TRUE;
        /* EmulPoll calls TerminateAck after it sends an ack to the host */
    }
}


/*******************************************************************************
 *
 *  TerminateAck
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    nothing
 *
 ******************************************************************************/

void
TerminateAck( PWD pWd )
{
    PDSETINFORMATION PdSetInfo;
    VDSETINFORMATION VdSetInfo;
    USHORT Channel;
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  If terminated request (not disconnect)
     */
    if ( pIca->fReceivedTerminate )
        pIca->fTerminate = TRUE;

    /*
     *  Clear connected to host flags
     */
    pWd->fTTYConnected = FALSE;
    pWd->fConnected = FALSE;
    pWd->fRecvStatusConnect = FALSE;
    pWd->fSendStatusConnect = FALSE;

    /*
     *  Disable optional protocol modules (e.g. compression)
     */
    PdSetInfo.PdInformationClass  = PdDisableModule;
    (void) PdCall( pWd, PD__SETINFORMATION, &PdSetInfo );

    /*
     *  Send disable command to all virtual drivers
     */
    VdSetInfo.VdInformationClass = VdDisableModule;
    for ( Channel = 0; Channel < Virtual_Maximum; Channel++ ) {
        (void) VdCall( pWd, Channel, VD__SETINFORMATION, &VdSetInfo );
    }
}


