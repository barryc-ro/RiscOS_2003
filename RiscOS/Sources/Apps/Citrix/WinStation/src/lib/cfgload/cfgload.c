
/*************************************************************************
*
*   CFGLOAD.C
*
*   Configuration Load library
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/12/95) [from Kurt Perry's CFG library]
*
*   Log: See VLOG
*
*
*************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../inc/client.h"
#ifdef  DOS
#include "../../inc/dos.h"
#endif
#include "../../inc/clib.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/biniapi.h"
#include "../../inc/cfgload.h"
/* #include "../../inc/wengapip.h" */
#include "../../inc/logapi.h"

#if 0
#include "..\..\inc\vioapi.h"
#include "..\..\inc\timapi.h"
#include "..\..\inc\kbdapi.h"
#include "..\..\inc\xmsapi.h"
#include "..\..\inc\mouapi.h"
#include "..\..\inc\lptapi.h"
#endif

// local includes
#include "cfgloadp.h"

/*=============================================================================
==   Local Functions Used
=============================================================================*/

int   LoadDriver( PCHAR, PCHAR, PDLLLINK );
void  UnloadVd( PDLLLINK *, USHORT );
void  UnloadWd( PDLLLINK );
void  UnloadPd( PDLLLINK );

/*=============================================================================
==   Local Variables
=============================================================================*/

/*=============================================================================
==   External Functions Used
=============================================================================*/
FNWFENGPOLL         srvWFEngPoll;
FNSTATUSMESSAGEPROC StatusMsgProc;

/*=============================================================================
==   External Data
=============================================================================*/
extern BOOL G_fAsync;

/*=============================================================================
==   Global Data
=============================================================================*/


/*******************************************************************************
 *
 *  LoadPd
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
LoadPd( PCHAR    pMergedSection,
        PCHAR    pModuleName,
        PCHAR    pDLLPath,
        PDLLLINK pWdLink,
        PDLLLINK pPdLink,
        PPDOPEN  pPdOpen )
{
    int    rc;
    PDOPEN PdOpen;

//  TRACE((TC_LIB, TT_API1, "LoadPd: pMergedSection" ));
//  TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, (ULONG)500 ));

    /*
     *  get Pd's dll and load
     */
    if ( (rc = LoadDriver( pModuleName, pDLLPath, pPdLink )) != CLIENT_STATUS_SUCCESS )
	goto done;
    
    /*
     *  Open Protocol driver
     */
    memset( &PdOpen, 0, sizeof(PDOPEN) );
#ifdef RISCOS
    ModuleLookup(pModuleName, NULL, &PdOpen.pDeviceProcedures);
#else
    PdOpen.pModuleProcedures = pModuleProcedures;
#ifdef DOS
    PdOpen.pClibProcedures   = pClibProcedures;
#else
    PdOpen.pClibProcedures   = NULL;
#endif
    PdOpen.pLogProcedures    = pLogProcedures;
    PdOpen.pBIniProcedures   = pBIniProcedures;
#endif
    PdOpen.pIniSection       = pMergedSection;
    PdOpen.pExePath          = (LPBYTE)pDLLPath;

    rc = ModuleCall( pPdLink, DLL__OPEN, &PdOpen );
    TRACE((TC_LIB, TT_API1, "LoadPd: ModuleCall DLL$OPEN, rc=%u", rc ));
    if ( rc )
        goto done;

    /*
     *  Return totals of reserve bytes
     */
    pPdOpen->OutBufHeader  += PdOpen.OutBufHeader;
    pPdOpen->OutBufTrailer += PdOpen.OutBufTrailer;
    pPdOpen->OutBufParam   += PdOpen.OutBufParam;

    if ( !pPdOpen->fOutBufCopy )
        pPdOpen->fOutBufCopy = PdOpen.fOutBufCopy;
    if ( !pPdOpen->fOutBufFrame )
        pPdOpen->fOutBufFrame = PdOpen.fOutBufFrame;

    TRACE((TC_LIB, TT_API1, "LoadPd: Header %u, Trailer %u, Param %u",
            pPdOpen->OutBufHeader, pPdOpen->OutBufTrailer,
            pPdOpen->OutBufParam ));

done:
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  LoadWd
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
LoadWd( PCHAR     pMergedSection,
        PCHAR     pModuleName,
        PCHAR     pDLLPath,
        PDLLLINK  pWdLink,
        PDLLLINK  pPdLink,
        PPDOPEN   pPdOpen,
        PUSHORT   pMaxVirtualChannels )
{
    int      rc;
    WDOPEN   WdOpen;

//  TRACE((TC_LIB, TT_API1, "LoadWd: pMergedSection" ));
//  TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, (ULONG)500 ));

    /*
     *  get Wd's dll and load
     */
    if ( (rc = LoadDriver( pModuleName, pDLLPath, pWdLink )) != CLIENT_STATUS_SUCCESS )
	goto done;

    /*
     *  Open Winstation driver
     */
    memset( &WdOpen, 0, sizeof(WDOPEN) );
#ifdef RISCOS
    ModuleLookup(pModuleName, NULL, &WdOpen.pEmulProcedures);
#else
    WdOpen.pModuleProcedures = pModuleProcedures;
#ifdef DOS
    WdOpen.pClibProcedures   = pClibProcedures;
    WdOpen.pXmsProcedures    = pXmsProcedures;
    WdOpen.pMouProcedures    = pMouProcedures;
    WdOpen.pTimerProcedures  = pTimerProcedures;
#else
    WdOpen.pClibProcedures   = NULL; 
    WdOpen.pXmsProcedures    = NULL;
    WdOpen.pMouProcedures    = NULL;
    WdOpen.pTimerProcedures  = NULL;
#endif
#ifndef RISCOS
    WdOpen.pVioProcedures    = pVioProcedures;
    WdOpen.pKbdProcedures    = pKbdProcedures;
#endif
#ifdef LPT_IN_ENGINE
    WdOpen.pLptProcedures    = pLptProcedures;
#else
    WdOpen.pLptProcedures    = NULL;
#endif
    WdOpen.pLogProcedures    = pLogProcedures;
    WdOpen.pBIniProcedures   = pBIniProcedures;
#endif
    WdOpen.pPdLink           = pPdLink;
    WdOpen.pIniSection       = pMergedSection;
    WdOpen.OutBufHeader      = pPdOpen->OutBufHeader;
    WdOpen.OutBufTrailer     = pPdOpen->OutBufTrailer;
    WdOpen.OutBufParam       = pPdOpen->OutBufParam;
    WdOpen.fOutBufCopy       = pPdOpen->fOutBufCopy;
    WdOpen.fOutBufFrame      = pPdOpen->fOutBufFrame;
    if ( G_fAsync ) {
        WdOpen.fAsync        = TRUE;
    } else {
        WdOpen.fAsync        = FALSE;
    }
    rc = ModuleCall( pWdLink, DLL__OPEN, &WdOpen );
    TRACE((TC_LIB, TT_API1, "LoadWd: ModuleCall DLL$OPEN, rc=%u", rc));

    *pMaxVirtualChannels = WdOpen.MaxVirtualChannels;

done:
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  LoadVD
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
LoadVd( PCHAR    pMergedSection,
        PCHAR    pModuleName,
        PCHAR    pDLLPath,
        PDLLLINK pWdLink,
        PDLLLINK * ppVdLink,
        USHORT   MaxVirtualChannels,
        PUSHORT  pChannel )
{
    int      rc;
    VDOPEN   VdOpen;
    PDLLLINK pVdLink = NULL;
    USHORT   Channel;


//  TRACE((TC_LIB, TT_API1, "LoadVd: pMergedSection" ));
//  TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, (ULONG)500 ));

    /*
     *  allocate space for DLLLINK
     */
    if ( (pVdLink = (PDLLLINK) malloc( sizeof(DLLLINK) )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    /*
     * get Vd's dll and load
     */
    if ( (rc = LoadDriver( pModuleName, pDLLPath, pVdLink )) != CLIENT_STATUS_SUCCESS )
        goto done;

    /*
     *  open Vd
     */
    memset( &VdOpen, 0, sizeof(VDOPEN) );
#ifdef RISCOS
    ModuleLookup(pModuleName, NULL, &VdOpen.pDeviceProcedures);
#else
    VdOpen.pModuleProcedures   = pModuleProcedures;

#ifdef DOS
    VdOpen.pXmsProcedures      = pXmsProcedures;
    VdOpen.pClibProcedures     = pClibProcedures;
    VdOpen.pMouProcedures      = pMouProcedures;
    VdOpen.pTimerProcedures    = pTimerProcedures;
#else
    VdOpen.pXmsProcedures      = NULL;
    VdOpen.pClibProcedures     = NULL;
    VdOpen.pMouProcedures      = NULL;
    VdOpen.pTimerProcedures    = NULL;
#endif

#ifdef LPT_IN_ENGINE
    VdOpen.pLptProcedures      = pLptProcedures;
#else
    VdOpen.pLptProcedures      = NULL;
#endif
#ifndef RISCOS
    VdOpen.pLogProcedures      = pLogProcedures;
    VdOpen.pBIniProcedures     = pBIniProcedures;
    VdOpen.pKbdProcedures      = pKbdProcedures;
#endif
#endif
    VdOpen.pfnWFEngPoll        = (PPLIBPROCEDURE)srvWFEngPoll;
    VdOpen.pfnStatusMsgProc    = (PPLIBPROCEDURE)StatusMsgProc;
    VdOpen.pWdLink             = pWdLink;
    VdOpen.pIniSection         = pMergedSection;
    rc = ModuleCall( pVdLink, DLL__OPEN, &VdOpen );

    TRACE(( TC_LIB, TT_API1,
            "LoadVd: ModuleCall DLL$OPEN, ChannelMask 0x%lx, rc=%u",
            VdOpen.ChannelMask, rc));

    if ( rc ) {

        //  unload on error
        rc = ModuleUnload( pVdLink );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        // free VdLink
        if ( pVdLink )
            (void) free( pVdLink );
    }
    else {

        //  return the vdlink to the caller
        for ( Channel=0; Channel < MaxVirtualChannels; Channel++ ) {
            if ( VdOpen.ChannelMask & 0x1 ) {
                ppVdLink[Channel] = pVdLink;
                *pChannel = Channel;
            }
            VdOpen.ChannelMask >>= 1;
        }
    }

done:
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  LoadDriver
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
LoadDriver( PCHAR pModuleName, PCHAR pDLLPath, PDLLLINK pPdLink )
{
    int rc;
    PCHAR pFullPath = NULL, pLoadPath = pModuleName;

#ifndef RISCOS
    /*
     * If pDLLPath not NULL, make fully qualified path
     */
    if ( pDLLPath ) {

        if ( (pLoadPath = pFullPath = (char *) malloc( strlen(pDLLPath) + strlen(pModuleName) + 1 )) == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto done;
        }
        strcpy( pFullPath, pDLLPath );
        strcat( pFullPath, pModuleName );
    }
#endif

    /*
     *  load dll into memory
     */
    rc = ModuleLoad( pLoadPath, pPdLink );
    TRACE((TC_LIB, TT_API1, "LoadDriver: ModuleName %s, DllPath %s, LoadPath %s, rc=%u",
          pModuleName, pDLLPath, pLoadPath, rc ));

done:

    //  clean up
    if ( pFullPath != NULL )
        (void) free( pFullPath );

    //  return result
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}

/*******************************************************************************
 *
 *  UnloadDrivers
 *
 *  ENTRY:
 *  EXIT:
 *
 ******************************************************************************/

void
UnloadDrivers( PDLLLINK pWdLink,
               PDLLLINK pPdLink,
               PDLLLINK * ppVdLink,
               USHORT  MaxVirtualChannels )
{

    TRACE((TC_LIB, TT_API1, "UnloadDrivers: Wd %lx, Pd %lx, pVd %lx", pWdLink, pPdLink, ppVdLink));

    /*
     *  unload virtual drivers
     */
    UnloadVd( ppVdLink, MaxVirtualChannels );

    /*
     *  unload winstation driver
     */
    UnloadWd( pWdLink );

    /*
     *  unload protocol drivers
     */
    UnloadPd( pPdLink );
}

/*******************************************************************************
 *
 *  UnloadVd
 *
 *   unload all virtual drivers
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
UnloadVd( PDLLLINK * ppVdLink, USHORT MaxVirtualChannels )
{
    USHORT i;
    USHORT j;
    int rc;

    TRACE((TC_LIB, TT_API1, "UnloadVd: pVd %lx", ppVdLink));

    if ( ppVdLink == NULL )
        return;

    //  look for each virtual driver class
    for ( i=0; i<MaxVirtualChannels; i++ ) {

        // not loaded
        if ( ppVdLink[i] == NULL || ppVdLink[i]->pProcedures == NULL )
            continue;

        // close first
        rc = ModuleCall( ppVdLink[i], DLL__CLOSE, NULL );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        // then unload
        rc = ModuleUnload( ppVdLink[i] );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        //  free the memory
        (void) free( ppVdLink[i] );

        //  scan forward for duplicates
        for ( j=i+1; j<MaxVirtualChannels; j++ ) {
            if ( ppVdLink[j] == ppVdLink[i] )
                ppVdLink[j] = NULL;
        }

        //  clear ourself
        ppVdLink[i] = NULL;
    }

    /* the array of pointers is freed in the calling procedure */
}

/*******************************************************************************
 *
 *  UnloadWd
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
UnloadWd( PDLLLINK pWdLink )
{
    int rc;

    TRACE((TC_LIB, TT_API1, "UnloadWd: pWd %lx", pWdLink));

    // wd valid?
    if ( pWdLink && pWdLink->pProcedures ) {

        // close first
        rc = ModuleCall( pWdLink, DLL__CLOSE, NULL );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        // then unload
        rc = ModuleUnload( pWdLink );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    }
}

/*******************************************************************************
 *
 *  UnloadPd
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
UnloadPd( PDLLLINK pPdLink )
{
    int rc;

    TRACE((TC_LIB, TT_API1, "UnloadPd: pPd %lx", pPdLink));

    // keep unloading till all Pds are gone
    while ( pPdLink && pPdLink->pProcedures ) {

        // close first
        rc = ModuleCall( pPdLink, DLL__CLOSE, NULL );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );

        // then unload
        rc = ModuleUnload( pPdLink );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    }
}

