
/*************************************************************************
*
*   TWVD.C
*
*   Thin Wire Windows - Virtual driver interface routines.
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Andy Stergiades (andys) 6-Apr-1994
*
*   twvd.c,v
*   Revision 1.1  1998/01/12 11:36:09  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.37   14 Aug 1997 15:07:26   kurtp
*  fix full screen, again


*  
*     Rev 1.36   05 Aug 1997 20:26:56   kurtp
*  Add Full Screen Window support
*  
*     Rev 1.35   04 Aug 1997 19:15:12   kurtp
*  update
*  
*     Rev 1.33   14 Jul 1997 18:21:26   kurtp
*  Add LVB to transparent text ops
*  
*     Rev 1.32   06 May 1997 21:28:50   kurtp
*  update to last fix
*  
*     Rev 1.31   06 May 1997 21:13:16   kurtp
*  Fix connection/disconnect and shadow
*  
*     Rev 1.30   28 Apr 1997 14:58:18   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.29   18 Apr 1997 18:10:36   kurtp
*  update
*  
*     Rev 1.28   15 Apr 1997 18:16:34   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.28   21 Mar 1997 16:09:36   bradp
*  update
*  
*     Rev 1.27   07 Mar 1997 15:31:52   kurtp
*  update
*  
*     Rev 1.26   13 Jan 1997 16:52:06   kurtp
*  Persistent Cache
*  
*     Rev 1.25   13 Nov 1996 09:06:06   richa
*  Updated Virtual channel code.
*  
*     Rev 1.23   13 Aug 1996 13:19:12   jeffm
*  Limit values for VRES and HRES
*  
*     Rev 1.22   11 Jul 1996 10:44:56   jeffm
*  Adjusted for Win95 work area for screen percent
*  
*     Rev 1.21   27 Jun 1996 11:50:32   marcb
*  update
*  
*     Rev 1.20   26 Jun 1996 15:45:38   marcb
*  update
*  
*     Rev 1.19   13 Jun 1996 15:22:16   jeffm
*  Got rid of compiler warning for DOS with iscreenpercent
*  
*     Rev 1.18   14 May 1996 11:30:10   jeffm
*  update
*  
*     Rev 1.16   20 Jan 1996 14:28:10   kurtp
*  update
*  
*     Rev 1.15   18 Jan 1996 11:43:18   kurtp
*  update
*  
*     Rev 1.14   04 Jan 1996 16:26:40   kurtp
*  Symbol Tech Variable DOS Resolution
*  
*     Rev 1.13   03 Jan 1996 13:33:34   kurtp
*  update
*  
*************************************************************************/

#include "windows.h"
#include "fileio.h"

/*
 *  Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../app/version.h"

#include "../../../inc/client.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "twtype.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../../../inc/wengapip.h"

#include "twwin.h"
#include "twdata.h"
#include <math.h>

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


int STATIC WFCAPI TWCallSetMouseRanges( USHORT uHoriMin, USHORT uHoriMax,
                                   USHORT uVertMin, USHORT uVertMax );

int STATIC TWSetMousePosition( PVD pVd, USHORT uX, USHORT uY );
int STATIC TWCallHookMouse( PVD pVd, PVOID pMouseHook );
int STATIC TWCallUnHookMouse( PVD pVd, PVOID pMouseHook );
int STATIC TWCallResetMouse( PVD pVd );
#ifdef DOS
int STATIC TWCallHookTimer( PVD pVd, PVOID pTimerHook );
int STATIC TWCallUnHookTimer( PVD pVd, PVOID pTimerHook );
#endif
void STATIC TWHostWrite( LPBYTE pBuf, USHORT Length );


PLIBPROCEDURE VdTW31DriverProcedures[VDDRIVER__COUNT] =
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

extern USHORT far TWMoveCursor(USHORT uX, USHORT uY);
extern USHORT TWAllocCache( PVD pVd, ULONG, ULONG );
extern USHORT TWDeallocCache( PVD pVd );
extern USHORT far SaveVideoRegs( void );
extern USHORT far RestoreVideoRegs( void );
extern USHORT TWDetermineSVGA( PVD pVd );
extern USHORT TWReadCacheParameters( PVOID );
extern USHORT TWWindowsStart( PVD pVd, PTHINWIRECAPS pThinWireMode );
extern USHORT TWWindowsStop( PVD pVd );
extern USHORT TWRealizePalette( HWND, HDC, UINT *, USHORT );
int WFCAPI TWDisplayPacket(PVD pVd, USHORT uChan,
                                  LPBYTE pBuffer, USHORT Length);

#ifndef DOS
extern BOOL  TWDIMCacheInit(PVD, BOOL);
extern BOOL  vfDimContinue;
extern ULONG vTimeLastDim;
#endif

extern VD VdData;
extern ULONG TinyCacheSize;
extern ULONG LargeCacheSize;
extern VDTWCACHE vVdTWCache;

/*=============================================================================
==   Data
=============================================================================*/

#define DEFAULT_SVGACAP         "Auto"
#define VARIABLE_SVGACAP        "Variable"
#define DEFAULT_SVGAPREF        "Off"
static USHORT VirtualThinWire;

STATIC ULONG PrefHRes;
STATIC ULONG PrefVRes;
STATIC BOOL  vfFullScreen = FALSE;

extern int vwScreen, vhScreen, vSVGAmode;
static BOOL vVariableRes = FALSE;

extern HWND vhWnd;
extern HDC  vhdc;
extern COLOR_CAPS vColor;

#if 0
typedef struct _RESCLIENT {
    USHORT  hres;
    USHORT  vres;
} RESCLIENT, * PRESCLIENT;


/*
 *  Client supported resolutions
 *  Note: 640x480 is assumed!
 */

RESCLIENT aresClient[] = {
    {  800,  600 },
    { 1024,  768 },
    { 1280, 1024 },
};
#define MAX_CLIENT_RES  (sizeof(aresClient)/sizeof(aresClient[0]))
#endif

static int    cHostLevel;
static int    iColorDepth;
int    viBitsPerPixel;

ULONG  vDimCacheEnabled;
ULONG  vDimCacheSize;
ULONG  vDimBitmapMin;     
#define CCHMAXPATH 260
PCHAR  vpszDimCachePath = NULL;

#if 0
extern LPBYTE vpLVB;
void   MaskLVBToScreen( HDC, BOOL );
#endif

USHORT bClickticks;

STATIC PVOID pWd = NULL;
STATIC POUTBUFRESERVEPROC  pOutBufReserve  = NULL;
STATIC POUTBUFAPPENDPROC   pOutBufAppend   = NULL;
STATIC POUTBUFWRITEPROC    pOutBufWrite    = NULL;
STATIC PAPPENDICAPKTPROC   pAppendICAPkt   = NULL;
STATIC PAPPENDVDHEADERPROC pAppendVdHeader = NULL;

typedef struct _TWBUFFER {
    struct _TWBUFFER    *pNext;
    USHORT              BufferSize;
    BYTE                Buffer[1];
} TWBUFFER, *PTWBUFFER;

typedef struct _TWBUFFERHEAD {
    PTWBUFFER           pNext;
    PTWBUFFER           pLast;
    USHORT              BufferSizeTotal;
    USHORT              BufferCount;
} TWBUFFERHEAD, *PTWBUFFERHEAD;

TWBUFFERHEAD      ICAWriteQ = { NULL, NULL, 0, 0 };

STATIC int CacheHasBeenAllocated = FALSE;

#ifndef DOS
LPBYTE vpTWLocalStack    = NULL;
LPBYTE vpTWLocalStackRHE = NULL;
LPBYTE vpTWLocalStackNT  = NULL;
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
    WDSETINFORMATION wdsi;
    VDWRITEHOOK vdwh;
    int rc;
    WDQUERYINFORMATION wdqi;
    OPENVIRTUALCHANNEL OpenVirtualChannel;
    char string[20];
 
    /*
     *  Read Cache parameters from .ini file
     */
    if ( rc = TWReadCacheParameters(pVdOpen->pIniSection) )
       return( rc );

    /*
     *  Initizlize thinwire stuff
     */
    if ( !ThinInitOnce( 0 ) ) {
        rc = CLIENT_ERROR_VD_ERROR;
        TRACE(( TC_VD, TT_API2, "VdOpen: ThinInitOnce failed!" ));
        goto done;
    }

    /*
     * Get a virtual channel
     */
    wdqi.WdInformationClass = WdOpenVirtualChannel;
    wdqi.pWdInformation = &OpenVirtualChannel;
    wdqi.WdInformationLength = sizeof(OPENVIRTUALCHANNEL);
    OpenVirtualChannel.pVCName = VIRTUAL_THINWIRE;
    rc = WdCall( pVd, WD__QUERYINFORMATION, &wdqi );
    VirtualThinWire = OpenVirtualChannel.Channel;
    ASSERT( VirtualThinWire == Virtual_ThinWire, VirtualThinWire );
 
    /*
     *  Return virtual channel id mask
     */
    pVdOpen->ChannelMask = (1L << VirtualThinWire);
 
    /*
     *  Check for super VGA
     */
    bGetPrivateProfileString( pVdOpen->pIniSection, INI_SVGACAPABILITY, DEFAULT_SVGACAP,
                              string, sizeof(string) );

    vSVGAmode = 0;
    vVariableRes = FALSE;
    PrefHRes = 0;
    PrefVRes = 0;
    vfFullScreen = FALSE;

    if (!stricmp(string, "Off")) {
	/* Off means only 640x480 available */
    }
    else if (!stricmp(string, VARIABLE_SVGACAP)) {
	/* auto means default to the set value and send complete set */
        PrefHRes = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_DESIREDHRES, DEF_DESIREDHRES );
     
        PrefVRes = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_DESIREDVRES, DEF_DESIREDVRES );
     
        vSVGAmode = 1;
        vVariableRes = TRUE;
    }
    else {
	/* auto means default to the set value and send complete set */
        PrefHRes = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_DESIREDHRES, DEF_DESIREDHRES );
     
        PrefVRes = bGetPrivateProfileInt( pVdOpen->pIniSection,
                                          INI_DESIREDVRES, DEF_DESIREDVRES );
     
        vSVGAmode = 1;
    }

    /*
     *  Force to resonable values (defined in wfengapi.h)
     */
    if ( (((USHORT)PrefHRes & 0xffff) == 0xffff) && 
         (((USHORT)PrefVRes & 0xffff) == 0xffff) ) {
	GetModeSpec((int *)&PrefHRes, (int *)&PrefVRes);
        vfFullScreen = TRUE;
    }
    else {
        if((PrefHRes==0) || (PrefHRes > MAX_DESIREDHRES ))
            PrefHRes = DEF_DESIREDHRES;
        if((PrefVRes==0) || (PrefVRes > MAX_DESIREDVRES ))
            PrefVRes = DEF_DESIREDVRES;
    }
    
    /*
     *  Get preferred color depth
     */
    iColorDepth = bGetPrivateProfileInt( pVdOpen->pIniSection,
					 INI_DESIREDCOLOR, DEF_DESIREDCOLOR );

    TRACE((TC_TW,TT_TW_PALETTE, "VDTW: Requested ColorDepth %04x", iColorDepth));

    /*
     *  Get persistent cache values
     */
    vDimCacheEnabled = bGetPrivateProfileBool( pVdOpen->pIniSection,
                                               INI_DIMCACHEENABLED,
                                               DEF_DIMCACHEENABLED ); 

    vDimCacheSize = bGetPrivateProfileLong( pVdOpen->pIniSection,
                                            INI_DIMCACHESIZE, 
                                            DEF_DIMCACHESIZE );      

    vDimBitmapMin = bGetPrivateProfileLong( pVdOpen->pIniSection,
                                            INI_DIMMINBITMAP, 
                                            DEF_DIMMINBITMAP );

    TRACE((TC_TW,TT_TW_CACHE, "VDTW: Cache on %d size %d BitmapMin %d", vDimCacheEnabled, vDimCacheSize, vDimBitmapMin));

    if ( vDimCacheEnabled ) {

        PCHAR pszTemp;

        pszTemp = (PCHAR) malloc(CCHMAXPATH+1);
        memset( pszTemp, 0, CCHMAXPATH+1);

        bGetPrivateProfileString( pVdOpen->pIniSection, 
                                  INI_DIMCACHEPATH, 
                                  DEF_DIMCACHEPATH,
                                  pszTemp, CCHMAXPATH );

	TRACE((TC_TW,TT_TW_CACHE, "VDTW: Cache in '%s'", pszTemp));

        if ( strlen(pszTemp) ) {

            //  remove trailing backslash
            if ( pszTemp[strlen(pszTemp)-1] == '.' ) {
                pszTemp[strlen(pszTemp)-1] = '\0';
            }

            //  just to insure directory is there
            mkdir( pszTemp );

            //  restore trailing backslash
//          strcat( pszTemp, "\\" );
	    strcat( pszTemp, "." );
            vpszDimCachePath = (PCHAR) malloc(strlen(pszTemp)+1);
            strcpy( vpszDimCachePath, pszTemp );
        }


        free( pszTemp );
    }
 
    /*
     *  Get click ticks
     */
    bClickticks = (USHORT) bGetPrivateProfileInt( pVdOpen->pIniSection, INI_CLICKTICKS, DEF_CLICKTICKS );
 
    /*
     * Initialize mode
     */
    memset( &vThinWireMode, 0, sizeof(vThinWireMode) );
 
    /*
     *  Register write hooks for all virtual channels handled by this vd
     */
    vdwh.Type  = (UCHAR)VirtualThinWire;
    vdwh.pVdData = pVd;
    vdwh.pProc = (PVDWRITEPROCEDURE) TWDisplayPacket;
    wdsi.WdInformationClass  = WdVirtualWriteHook;
    wdsi.pWdInformation      = &vdwh;
    wdsi.WdInformationLength = sizeof(VDWRITEHOOK);
    rc = WdCall(pVd, WD__SETINFORMATION, &wdsi);
 
    /*
     * This returns pointers to functions to use to send data to the host
     */
    pWd             = vdwh.pWdData;
    pOutBufReserve  = vdwh.pOutBufReserveProc;
    pOutBufAppend   = vdwh.pOutBufAppendProc;
    pOutBufWrite    = vdwh.pOutBufWriteProc;
    pAppendVdHeader = vdwh.pAppendVdHeaderProc;


    TRACE(( TC_VD, TT_API2, "VdOpen: writehook ch=%u p=%lx rc=%u", vdwh.Type, vdwh.pVdData, rc ));

done:
    ASSERT( !rc, rc );
    return( rc );
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

static int 
DriverClose( PVD pVd, PDLLCLOSE pVdClose )
{
   TWDeallocCache(pVd);
   CacheHasBeenAllocated = FALSE;

   TWWindowsStop(pVd);

   ThinDestroyOnce();

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
    PVDTW_C2H pVdData;
    PMODULE_C2H pHeader;
    PTHINWIRECAPS pCaps, pPref;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(VDTW_C2H);

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
    pVdData = (PVDTW_C2H) pVdInfo->pBuffer;
    memset( pVdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pVdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_VirtualDriver;
    pHeader->VersionL = VERSION_CLIENTL_VDTW;
    pHeader->VersionH = VERSION_CLIENTH_VDTW;
    strcpy( pHeader->HostModuleName, vpszVersion );

    /*
     *  Initialize virtual driver header
     */
    pVdData->Header.ChannelMask = pVd->ChannelMask;

    /*
     *  Initialize virtual driver data
     */
    /*
     * First specify thinwire mode capabilities for this client
     */
    pCaps = &pVdData->ThinWireCaps;
    pCaps->cbSmallCache   = (USHORT) TinyCacheSize;
    pCaps->cbLargeCache   = LargeCacheSize;
    pCaps->fColorCaps     = CCAPS_4_BIT;
    pCaps->flGraphicsCaps = GCAPS_COMPLEX_CURVES;
    pCaps->ResCapsCnt     = 1;
    pCaps->ResCapsOff     = (USHORT)((LPBYTE)&pCaps->ResCaps - (LPBYTE)pCaps);
    pCaps->ResCaps.HRes   = (USHORT)PrefHRes;
    pCaps->ResCaps.VRes   = (USHORT)PrefVRes;

    /*
     * Now specify thinwire mode preference for this client
     */
    pPref = &pVdData->ThinWirePref;
    pPref->cbSmallCache   = pCaps->cbSmallCache;
    pPref->cbLargeCache   = pCaps->cbLargeCache;
    pPref->fColorCaps     = CCAPS_4_BIT;
    pPref->flGraphicsCaps = GCAPS_COMPLEX_CURVES;
    pPref->ResCapsCnt     = 1;
    pPref->ResCapsOff     = (USHORT)((LPBYTE)&pPref->ResCaps - (LPBYTE)pPref);

    viBitsPerPixel = iColorDepth == CCAPS_4_BIT ? 4 : 8;

    pCaps->ResCapsCnt = EnumerateModes(&pCaps->ResCaps,
				       sizeof(pVdData->ResCaps)/sizeof(pVdData->ResCaps[0]) + 1, &viBitsPerPixel);
    if (!vSVGAmode)
	pCaps->ResCapsCnt = 1;
    
    pPref->ResCaps.HRes   = pCaps->ResCaps.HRes;
    pPref->ResCaps.VRes   = pCaps->ResCaps.VRes;

    if (viBitsPerPixel == 8)
    {
	pCaps->fColorCaps     |= CCAPS_8_BIT;
	pCaps->flGraphicsCaps |= GCAPS_SSB_1BYTE_PP;

	pPref->fColorCaps     |= CCAPS_8_BIT;
	pPref->flGraphicsCaps |= GCAPS_SSB_1BYTE_PP;
    }

    TRACE((TC_TW,TT_TW_PALETTE, "VDTW: %d modes sent %d bpp", pCaps->ResCapsCnt, viBitsPerPixel));
    
    /*
     *  Initialize cache data
     */
    pVdData->CacheXms    = 0;
    pVdData->CacheLowMem = LargeCacheSize;
    pVdData->CacheDASD   = 0;
    pVdData->CacheTiny   = TinyCacheSize;

    /*
     *  Initialize dim data
     */
    pCaps->flGraphicsCaps &= ~GCAPS_BMPS_PRECACHED;
    if ( vDimCacheEnabled ) {
    
        pCaps->flGraphicsCaps |= GCAPS_BMPS_PRECACHED;
        pPref->flGraphicsCaps |= GCAPS_BMPS_PRECACHED;

        pVdData->DimCacheSize       = vDimCacheSize;
        pVdData->DimBitmapMin       = 
             (vDimBitmapMin < (2048 + DIM_SYSTEM_OVERHEAD + 1) ?
                              (2048 + DIM_SYSTEM_OVERHEAD + 1) :
                              vDimBitmapMin);
        pVdData->DimSignatureLevel  = DIM_SIGNATURE_LEVEL;
        pVdData->DimFilesysOverhead = DIM_SYSTEM_OVERHEAD;
    }

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
    int         rc = CLIENT_STATUS_SUCCESS;
    PTWBUFFER pTWBuffer;

    /*
     *  If no work then ship out 
     */
    if ( (vfDimContinue == TRUE) && (ICAWriteQ.pNext == NULL) ) {
        if ( (ULONG)Getmsec() > (vTimeLastDim + 1000 /*DIM_YIELD*/) ) {
            vfDimContinue = TWDIMCacheInit(pVd, vfDimContinue);
            vTimeLastDim = Getmsec();
        }
    }

#if !defined(RISCOS)
    /*
     *  Check if LVB need to be flushed
     */
    if ( vpLVB != NULL ) {
        MaskLVBToScreen( vhdc, FALSE );
    }
#endif

    /*
     *  Check if there is anything to write and get the byte Count
     */
    if ( ICAWriteQ.pNext == NULL ) {

        // Nothing to do so exit
        rc = CLIENT_STATUS_NO_DATA;
        goto Exit;
    }

    /*
     *  Dequeue the 1st entry.
     */
    pTWBuffer = ICAWriteQ.pNext;
    ICAWriteQ.pNext = pTWBuffer->pNext;
    ICAWriteQ.BufferSizeTotal -= pTWBuffer->BufferSize;
    ICAWriteQ.BufferCount--;

    TRACE(( TC_TW, TT_API2, "VDTW: Virtual Write (from queue), %u", pTWBuffer->BufferSize ));

    if ( pOutBufReserve( pWd, pTWBuffer->BufferSize ) ) {
        rc =  CLIENT_STATUS_NO_DATA;
        goto Exit;
    }

    /*
     *  Append Virtual write header
     */
    if ( rc = pAppendVdHeader( pWd, (UCHAR)VirtualThinWire, pTWBuffer->BufferSize ) ) {
        goto Exit;
    }

    /*
     *  Append data
     */
    if ( rc = pOutBufAppend( pWd, pTWBuffer->Buffer, pTWBuffer->BufferSize ) ) {
        goto Exit;
    }

    /*
     *  Write last outbuf
     */
    rc = pOutBufWrite( pWd );

    free( pTWBuffer );

Exit:
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
    PVDTWCACHE pVdTWCache;


    switch ( pVdQueryInformation->VdInformationClass ) {

       case VdThinWireCache :
          TRACE(( TC_UI, TT_API4, "DriverQueryInformation: VdThinWireCache" ));
          ASSERT( pVdQueryInformation->VdInformationLength == sizeof(VDTWCACHE), 0 );
          pVdTWCache = (PVDTWCACHE)pVdQueryInformation->pVdInformation;
          pVdTWCache->ulXms    = vVdTWCache.ulXms;
          pVdTWCache->ulLowMem = vVdTWCache.ulLowMem;
          pVdTWCache->ulDASD   = vVdTWCache.ulDASD;
          pVdTWCache->ulTiny   = vVdTWCache.ulTiny;
          strcpy( pVdTWCache->pszCachePath, vVdTWCache.pszCachePath );
          break;

       default:
          break;
    }

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
   PVDTWCACHE pCache;
   int rc = CLIENT_STATUS_SUCCESS;

   TRACE(( TC_UI, TT_API4, "DriverSetInformation: %d", pVdSetInformation->VdInformationClass ));
   
   switch ( pVdSetInformation->VdInformationClass ) {

      case VdSetFocus:
	 rc = TWWindowsStart(pVd, &vThinWireMode);
         break;

      case VdKillFocus:
	 rc = TWWindowsStop(pVd);
         break;

      case VdMousePosition:
         rc = TWSetMousePosition(pVd,
               ((PMOUPOSITION)(pVdSetInformation->pVdInformation))->X,
               ((PMOUPOSITION)(pVdSetInformation->pVdInformation))->Y );
         break;

      case VdDisableModule:
         break;

      case VdThinWireCache :
         ASSERT( pVdSetInformation->VdInformationLength == sizeof(VDTWCACHE), 0 );
         pCache = (PVDTWCACHE) pVdSetInformation->pVdInformation;
#ifdef BEYONDDEMO_CACHING
         if ( !CacheHasBeenAllocated ) {
//           rc = TWAllocCache( pVd, pCache->ulXms, pCache->ulLowMem );
             if ( rc == CLIENT_STATUS_SUCCESS )
                 CacheHasBeenAllocated = TRUE;
         }
#else
         TinyCacheSize = 0L;
         LargeCacheSize = 0L;
#endif
         break;

#if defined(DOS)

      case VdThinWireDeallocateCache :
         // Special function to steal the thinwire cache
         // This should never be called when engine has focus
         // After this is called, engine should never get focus again.
         // used to get a large amount of data for a virtual driver for
         // a flash rom update
         if(CacheHasBeenAllocated) {
//             TWDeallocCache(pVd);
             CacheHasBeenAllocated = FALSE;
         }
         break;

      case VdThinWireSaveVideoRegs :
         // call the save_hw_regs
//         SaveVideoRegs();
         break;

      case VdThinWireRestoreVideoRegs :
         // call the res_hw_regs
//         RestoreVideoRegs();
         break;

#else

      case VdInitWindow:
         rc = TWInitWindow( pVd, (HWND)(ULONG)pVdSetInformation->pVdInformation );
         break;

      case VdDestroyWindow:
	 rc = TWDestroyWindow( (HWND)(ULONG)pVdSetInformation->pVdInformation );
         break;

      case VdPaint:
//	  rc = TWPaint( pVd, (HWND)(ULONG)pVdSetInformation->pVdInformation );
         break;

      case VdThinwireStack:
         {
             PVDTWSTACK pTWStack = (PVDTWSTACK)pVdSetInformation->pVdInformation;

             vpTWLocalStack    = pTWStack->pTop;
             vpTWLocalStackNT  = pTWStack->pMiddle;
             vpTWLocalStackRHE = pTWStack->pBottom;
         }
         break;

      case VdRealizePaletteFG:
         if ( vColor == Color_Cap_256 ) {
           rc = TWRealizePalette( vhWnd, vhdc, 
                                    (UINT *)pVdSetInformation->pVdInformation, 
                                    TWREALIZEPALETTE_FG );
         }
         break;

      case VdInactivate:
         if ( vColor == Color_Cap_256 ) {
           UINT cColors;
             rc = TWRealizePalette( vhWnd, vhdc, 
                                    (UINT *)&cColors, 
                                    TWREALIZEPALETTE_FOCUS );
         }
         break;

      case VdRealizePaletteBG:
         if ( vColor == Color_Cap_256 ) {
           rc = TWRealizePalette( vhWnd, vhdc, 
                                    (UINT *)pVdSetInformation->pVdInformation, 
                                    TWREALIZEPALETTE_BG );
         }
         break;

#endif

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
 *  TWSetMousePosition
 *
 *    Calls Mouse code to set mouse position.
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWSetMousePosition( PVD pVd, USHORT uX, USHORT uY )
{
    int rc = CLIENT_STATUS_SUCCESS;
#if !defined( DOS) && !defined(RISCOS)
    ULONG vwScreen = GetWindowLong( vhWnd, GWL_WINDOWWIDTH );
    ULONG vhScreen = GetWindowLong( vhWnd, GWL_WINDOWHEIGHT );
#endif

    TRACE(( TC_VD, TT_API2, "TWSetMousePosition: pvd=%lx X=%u Y=%u",pVd, uX, uY));

#ifdef DOS
    // set mouse driver position
    rc = MousePosition( uX, uY );
#endif

#if 0
    // move our cursor
    uX = (USHORT)(((ULONG)uX * vwScreen)/0x10000);
    uY = (USHORT)(((ULONG)uY * vhScreen)/0x10000);
    rc = TWMoveCursor(uX, uY);
#endif
    
#if !defined(DOS) && !defined(RISCOS)
    //  save moved to position for wengine
    SetWindowLong( vhWnd, GWL_MOUSE_X, (LONG) uX );
    SetWindowLong( vhWnd, GWL_MOUSE_Y, (LONG) uY );
#endif

    return(rc);
}

#ifdef DOS
/*******************************************************************************
 *
 *  TWCallSetMouseRanges
 *
 *    Calls Mouse code to set ranges.  (called from old asm code)
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/


//WARNING - turn optimization off because compiler does not set up
//          pVd right

//#pragma optimize("",off)
STATIC int WFCAPI 
TWCallSetMouseRanges( USHORT uHoriMin, USHORT uHoriMax, USHORT uVertMin, USHORT uVertMax )
{
   int rc;
   PVD pVd = &VdData;      // do this for benefit of trace statement

   TRACE(( TC_VD, TT_API2, "TWSetMouseRanges: hmin=%u hmax=%u vmin=%u vmax=%u",uHoriMin,uHoriMax, uVertMin, uVertMax));
   rc = MouseSetRanges(uHoriMin, uHoriMax, uVertMin, uVertMax);

   return(rc);
}
//#pragma optimize("",on)

/*******************************************************************************
 *
 *  TWCallHookMouse
 *
 *    Calls Mouse code to do the hooking
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWCallHookMouse( PVD pVd, PVOID pMouseHook )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWCallHookMouse: pVd=%lx pHook=%lx", pVd, pMouseHook));
   rc = MouseAddHook(MouseHookInt, pMouseHook);

   return(rc);
}
/*******************************************************************************
 *
 *  TWCallUnHookMouse
 *
 *    Calls Mouse code to do the unhooking
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWCallUnHookMouse( PVD pVd, PVOID pMouseHook )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWCallUnHookMouse: pVd=%lx pHook=%lx", pVd, pMouseHook));
   rc = MouseRemoveHook(MouseHookInt, pMouseHook);

   return(rc);
}
/*******************************************************************************
 *
 *  TWCallResetMouse
 *
 *    Calls Mouse code for reset.
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWCallResetMouse( PVD pVd )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWCallResetMouse: pVd=%lx", pVd));
   rc = MouseReset();

   return(rc);
}
#endif

#ifdef DOS
/*******************************************************************************
 *
 *  TWCallHookTimer
 *
 *    Calls Timer code to do the hooking
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWCallHookTimer( PVD pVd, PVOID pTimerHook )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWCallHookTimer: pVd=%lx pHook=%lx", pVd, pTimerHook));
   rc = TimerAddHook(pTimerHook);

   Delay(100L);

   return(rc);
}
/*******************************************************************************
 *
 *  TWCallUnHookTimer
 *
 *    Calls Timer code to do the unhooking
 *
 * ENTRY:
 *
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
TWCallUnHookTimer( PVD pVd, PVOID pTimerHook )
{
   int rc;

   TRACE(( TC_VD, TT_API2, "TWCallUnHookTimer: pVd=%lx pHook=%lx", pVd, pTimerHook));
   rc = TimerRemoveHook(pTimerHook);

   return(rc);
}
#endif

/*******************************************************************************
 *
 *  TWHostWrite
 *
 * ENTRY:
 *    pBuf (input)
 *       Buffer with data packet
 *
 *    Length (input)
 *       Length of the data packet
 *
 * EXIT:
 *
 ******************************************************************************/

void STATIC TWHostWrite( LPBYTE pBuf, USHORT Length )
{
    PTWBUFFER   pTWBuffer;

    TRACE(( TC_TW, TT_API3, "TWVD: TWHostWrite Len=%d, pData=[%c][%c][%c][%c]",
            Length, pBuf[0],pBuf[1],pBuf[2],pBuf[3] ));

    if ((pTWBuffer = malloc( sizeof(TWBUFFER) + Length )) == NULL) {

        goto Exit;
    }

    /*
     * Copy data to our buffer.
     */
    memcpy( pTWBuffer->Buffer, pBuf, Length );
    pTWBuffer->BufferSize = Length;

    /*
     *  Enqueue the entry on the end of the zero terminated list
     */
    pTWBuffer->pNext = NULL;
    if ( ICAWriteQ.pNext ) {
        ICAWriteQ.pLast->pNext = pTWBuffer;
    } else {
        ICAWriteQ.pNext = pTWBuffer;
    }
    ICAWriteQ.pLast = pTWBuffer;
    ICAWriteQ.BufferSizeTotal += pTWBuffer->BufferSize;
    ICAWriteQ.BufferCount++;

Exit:
    return;
}
