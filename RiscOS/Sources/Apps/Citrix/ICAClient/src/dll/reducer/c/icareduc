
/*************************************************************************
*
*   icareduc.c
*
*   Admin file for bandwidth reducer DLL
*
*************************************************************************/

/*
 *  Includes
 */
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/client.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#ifdef  DOS
#include "../../inc/dos.h"
#endif
#include "../../inc/clib.h"
#include "../../inc/wdapi.h"
#include "../wd/inc/wd.h"

#define REDUCERLIB
#include "../../inc/reducapi.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int ReducerLoad( PDLLLINK pLink );
static int ReducerUnload( PDLLLINK );
static int ReducerOpen( PDLLLINK, PDLLPROCEDURE FAR * );
static int ReducerClose( void );
static int ReducerInfo( PDLLLINK, PDLLINFO );


/*=============================================================================
==   External Functions Used
=============================================================================*/

ULONG WFCAPI ReducerDataQueueMemSize( ULONG, ULONG );
ULONG WFCAPI ExpanderDataQueueMemSize( ULONG );
VOID  WFCAPI InitReducerDataQueueVariables( PDATA_QUEUE, ULONG, ULONG, LPUCHAR, ULONG );
VOID  WFCAPI InitExpanderDataQueueVariables( PDATA_QUEUE, ULONG, LPUCHAR, ULONG );
ULONG WFCAPI ConvertDataQueueIntoBuffer( PDATA_QUEUE, POUTBUF, ULONG, ULONG, ULONG );
VOID  WFCAPI ConvertBufferIntoDataQueue( LPUCHAR, ULONG, PDATA_QUEUE);
ULONG WFCAPI ExtractNewDataFromQueue( PDATA_QUEUE, PUCHAR FAR *, LPULONG );


/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define WinStation driver external procedures
 */
PDLLPROCEDURE ReducDLLProc[] = {
    (PDLLPROCEDURE) ReducerLoad,
    (PDLLPROCEDURE) ReducerUnload,
    (PDLLPROCEDURE) ReducerOpen,
    (PDLLPROCEDURE) ReducerClose,
    (PDLLPROCEDURE) ReducerInfo
};

/* Dispatch table for reducer functions */
PDLLPROCEDURE ReducerProcedures[] = {
	(PDLLPROCEDURE) InitReducerDataQueueVariables,
	(PDLLPROCEDURE) InitExpanderDataQueueVariables,
	(PDLLPROCEDURE) ReducerDataQueueMemSize,
	(PDLLPROCEDURE) ExpanderDataQueueMemSize,
	(PDLLPROCEDURE) ConvertDataQueueIntoBuffer,
	(PDLLPROCEDURE) ConvertBufferIntoDataQueue,
	(PDLLPROCEDURE) ExtractNewDataFromQueue,
};

static ULONG dataThingy;

/*******************************************************************************
 *
 *  Load
 *
 *  DLL Load procedure
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
ReducerLoad( PDLLLINK pLink )
{

    pLink->ProcCount   = sizeof (ReducDLLProc) / sizeof (PDLLPROCEDURE);
    pLink->pProcedures = ReducDLLProc;
    pLink->pData       = &dataThingy;   // wot this for anyway?
    
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  ReducerUnload
 *
 *  DLL Unload procedure
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int WFCAPI ReducerUnload( PDLLLINK pLink )
{
    pLink->ProcCount   = 0;
    pLink->pProcedures = 0;
    pLink->pData       = NULL;

    return CLIENT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 *  ReducerOpen
 *
 *  Driver open routine
 *
 * ENTRY:
 *    pLink (input, ignored)
 *       pointer to the structure DLLLINK
 *    pProcs (output)
 *       pointer to pass back address of reducer API table
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int WFCAPI ReducerOpen( PDLLLINK pLink, PDLLPROCEDURE FAR *pProcs )
{
    *pProcs = (PDLLPROCEDURE)ReducerProcedures;

    return CLIENT_STATUS_SUCCESS;
}

/*******************************************************************************
 *
 *  ReducerClose
 *
 *  Driver close routine
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int WFCAPI ReducerClose( void )
{
    return CLIENT_STATUS_SUCCESS;
}


/*******************************************************************************
 *
 *  ReducerInfo
 *
 *  Driver info routine
 *
 * ENTRY:
 *    pLink (ignored)
 *       pointer to the structure DLLLINK
 *    pInfo (output)
 *       pointer to the information struct to be filled out
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int WFCAPI ReducerInfo( PDLLLINK pLink, PDLLINFO pInfo )
{
    const char fName[] = "reducerModuleInfo";
    USHORT      cbInfoSize;
    PMODULE_C2H pData;  // the only data this module sends is the header info

    if ( pInfo == NULL )
        return CLIENT_ERROR_NULL_MEM_POINTER;

    cbInfoSize = sizeof(MODULE_C2H);

    if ( pInfo->ByteCount < cbInfoSize )
    {
        // caller is asking for buffer size
        pInfo->ByteCount = cbInfoSize;
        return CLIENT_ERROR_BUFFER_TOO_SMALL;
    }

    pData = (PMODULE_C2H) (pInfo->pBuffer);
    if ( pData == NULL )
        return CLIENT_ERROR_NULL_MEM_POINTER;

    // init default data
    //
    memset( pData, 0, cbInfoSize );
    pData->ByteCount = cbInfoSize;

    // set info goodies
    //
    pData->ModuleClass = Module_SubDriver;
    pData->VersionL    = REDUCER_VERSION_LOW;
    pData->VersionH    = REDUCER_VERSION_HIGH;

    pInfo->ByteCount = cbInfoSize;

    return CLIENT_STATUS_SUCCESS;
}
