
/*************************************************************************
*
* vdapi.c
*
* Virtual driver external routines
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Andy 4/6/94
*
* $Log$
*  
*     Rev 1.18   15 Apr 1997 18:02:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.17   08 May 1996 13:56:00   jeffm
*  update
*  
*     Rev 1.16   20 Dec 1995 15:11:50   bradp
*  update
*  
*     Rev 1.15   08 Nov 1995 09:55:00   marcb
*  update
*  
*     Rev 1.14   03 May 1995 09:52:02   richa
*
*     Rev 1.13   02 May 1995 13:56:46   butchd
*  update
*
*     Rev 1.12   28 Apr 1995 13:49:38   richa
*
*     Rev 1.11   17 Apr 1995 12:42:20   marcb
*  update
*
*************************************************************************/

#include <windows.h>

/*
 *  Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../../../inc/client.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/vd.h"

/*=============================================================================
==   Functions Defined
=============================================================================*/

extern int WFCAPI VdLoad( PDLLLINK );
int static WFCAPI VdUnload( PVD, PDLLLINK );
int static WFCAPI VdOpen( PVD, PVDOPEN );
int static WFCAPI VdClose( PVD, PDLLCLOSE );
int static WFCAPI VdInfo( PVD, PDLLINFO );
int static WFCAPI VdPoll( PVD, PDLLPOLL );
int static WFCAPI VdQueryInformation( PVD, PVDQUERYINFORMATION );
int static WFCAPI VdSetInformation( PVD, PVDSETINFORMATION );

int STATIC WdCall( PVD pVd, USHORT ProcIndex, PVOID pParam );


/*=============================================================================
==   External Functions used
=============================================================================*/
int STATIC DriverOpen( PVD, PVDOPEN );
int STATIC DriverClose( PVD, PDLLCLOSE );
int STATIC DriverInfo( PVD, PDLLINFO );
int STATIC DriverPoll( PVD, PDLLPOLL );
int STATIC DriverQueryInformation( PVD, PVDQUERYINFORMATION );
int STATIC DriverSetInformation( PVD, PVDSETINFORMATION );
int STATIC DriverGetLastError( PVD, PVDLASTERROR );


/*=============================================================================
==   Data
=============================================================================*/
/*
 *  Define Virtual driver external procedures
 */
STATIC PDLLPROCEDURE VdProcedures[ VD$COUNT ] = {
    (PDLLPROCEDURE) VdLoad,
    (PDLLPROCEDURE) VdUnload,
    (PDLLPROCEDURE) VdOpen,
    (PDLLPROCEDURE) VdClose,
    (PDLLPROCEDURE) VdInfo,
    (PDLLPROCEDURE) VdPoll,
    (PDLLPROCEDURE) VdQueryInformation,
    (PDLLPROCEDURE) VdSetInformation,
};

/*
 *  Define Protocol driver data structure
 *   (this could be dynamically allocated)
 */
// STATIC VD VdData = {0};


/*=============================================================================
==   Global Data
=============================================================================*/

STATIC PPLIBPROCEDURE pClibProcedures  = NULL;
STATIC PPLIBPROCEDURE pMouProcedures   = NULL;
STATIC PPLIBPROCEDURE pTimerProcedures = NULL;
STATIC PPLIBPROCEDURE pLptProcedures   = NULL;
STATIC PPLIBPROCEDURE pXmsProcedures   = NULL;
STATIC PPLIBPROCEDURE pLogProcedures   = NULL;
STATIC PPLIBPROCEDURE pBIniProcedures  = NULL;
STATIC PPLIBPROCEDURE pKbdProcedures   = NULL;
STATIC PFNWFENGPOLL   gpfnWFEngPoll    = NULL;
STATIC PFNSTATUSMESSAGEPROC gpfnStatusMsgProc = NULL;

/*******************************************************************************
 *
 *  Load
 *
 *    The user interface calls VdLoad to load and link a virtual
 *    driver.
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

extern int WFCAPI
VdLoad( PDLLLINK pLink )
{
    PWD pVd = malloc(sizeof(VD));

    pLink->ProcCount   = VD$COUNT;
    pLink->pProcedures = VdProcedures;
    pLink->pData       = pvd;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  VdUnload
 *
 *  DllUnload calls VdUnload to unlink and unload a Vd.
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to virtual driver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
VdUnload( PVD pPd, PDLLLINK pLink )
{
    free(pPd);

    pLink->ProcCount   = 0;
    pLink->pProcedures = 0;
    pLink->pData       = NULL;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  VdOpen
 *
 *  The user interface calls VdOpen to open and initialize a Vd.
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
VdOpen( PVD pVd, PVDOPEN pVdOpen )
{
    int rc;

    /*
     *  Initialize global data
     */
    pClibProcedures   = pVdOpen->pClibProcedures;
    pMouProcedures    = pVdOpen->pMouProcedures;
    pTimerProcedures  = pVdOpen->pTimerProcedures;
    pLptProcedures    = pVdOpen->pLptProcedures;
    pXmsProcedures    = pVdOpen->pXmsProcedures;
    pLogProcedures    = pVdOpen->pLogProcedures;
    pBIniProcedures   = pVdOpen->pBIniProcedures;
    pKbdProcedures    = pVdOpen->pKbdProcedures;
    gpfnWFEngPoll     = (PFNWFENGPOLL)pVdOpen->pfnWFEngPoll;
    gpfnStatusMsgProc = (PFNSTATUSMESSAGEPROC)pVdOpen->pfnStatusMsgProc;

   /*
    *  Initialize VD data structure
    */
    memset( pVd, 0, sizeof(VD) );
    pVd->pDriverProcedures = pVdOpen->pDriverProcedures;
    
    pVd->pWdLink = pVdOpen->pWdLink;

   /*
    *  Initialize specific virtual driver
    */
    rc = DriverOpen( pVd, pVdOpen );

    /*
     *  Save channel mask
     */
    pVd->ChannelMask = pVdOpen->ChannelMask;

    TRACE(( TC_VD, TT_API1, "VdOpen: rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  VdClose
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
VdClose( PVD pVd, PDLLCLOSE pVdClose )
{
    int rc;

    rc = DriverClose( pVd, pVdClose );

    TRACE(( TC_VD, TT_API1, "VdClose: rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  VdInfo
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to procotol driver data structure
 *    pVdInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
VdInfo( PVD pVd, PDLLINFO pVdInfo )
{
    return( DriverInfo( pVd, pVdInfo ) );
}


/*******************************************************************************
 *
 *  VdPoll
 *
 *  WdPoll calls VdPoll
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to winstation driver data structure
 *    pVdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
VdPoll( PVD pVd, PDLLPOLL pVdPoll )
{
    return( DriverPoll( pVd, pVdPoll ) );
}


/*******************************************************************************
 *
 *  VdQueryInformation
 *
 *   Queries information about the procotol driver
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to procotol driver data structure
 *    pVdQueryInformation (input/output)
 *       pointer to the structure VDQUERYINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
VdQueryInformation( PVD pVd, PVDQUERYINFORMATION pVdQueryInformation )
{
   int rc = CLIENT_STATUS_SUCCESS;

   switch ( pVdQueryInformation->VdInformationClass ) {

      case VdLastError :
         if ( pVdQueryInformation->VdInformationLength < sizeof(VDLASTERROR) )
            return( CLIENT_ERROR_BUFFER_TOO_SMALL );
         rc = DriverGetLastError( pVd, (PVDLASTERROR) pVdQueryInformation->pVdInformation );
         pVdQueryInformation->VdReturnLength = sizeof(VDLASTERROR);
         break;

      default:
         rc = DriverQueryInformation( pVd, pVdQueryInformation );
         break;

    }

    TRACE(( TC_VD, TT_API1, "VdQueryInformation(%u): rc=0x%x",
            pVdQueryInformation->VdInformationClass, rc ));

    return( rc );
}


/*******************************************************************************
 *
 *  VdSetInformation
 *
 *   Sets information for a procotol driver
 *
 * ENTRY:
 *    pVd (input)
 *       pointer to procotol driver data structure
 *    pVdSetInformation (input/output)
 *       pointer to the structure VDSETINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
VdSetInformation( PVD pVd, PVDSETINFORMATION pVdSetInformation )
{
    int rc = CLIENT_STATUS_SUCCESS;

    rc = DriverSetInformation( pVd, pVdSetInformation );

    TRACE(( TC_VD, TT_API1, "VdSetInformation(%u): rc=0x%x",
            pVdSetInformation->VdInformationClass, rc ));

    return( rc );
}


/*******************************************************************************
 *
 *  WdCall
 *
 *    WdCall is used to call the winstation driver
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to virtual driver data structure
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
WdCall( PVD pVd, USHORT ProcIndex, PVOID pParam )
{
    PDLLLINK pLink;
    PDLLPROCEDURE pProcedure;

    pLink = pVd->pWdLink;

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

