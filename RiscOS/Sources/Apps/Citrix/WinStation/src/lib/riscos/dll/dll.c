
/*************************************************************************
*
* dll.c
*
* DLL emulation routines for RISC OS
*
* Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen  (3/15/94)
*
* $Log$
*  
*     Rev 1.36   15 Apr 1997 18:50:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.35   28 May 1996 19:47:48   jeffm
*  update
*  
*     Rev 1.34   20 May 1996 15:48:24   jeffm
*  update
*  
*     Rev 1.32   14 Jul 1995 13:30:24   butchd
*  generic CLIENT_ERROR_DLL_NOT_FOUND for _ReadHeader error
*  
*     Rev 1.31   07 Jul 1995 15:42:16   kurtp
*  update
*  
*     Rev 1.30   26 Jun 1995 17:59:12   marcb
*  update
*  
*     Rev 1.29   10 May 1995 11:58:30   butchd
*  update
*  
*     Rev 1.28   04 May 1995 20:02:36   butchd
*  update
*  
*     Rev 1.27   03 May 1995 11:06:14   butchd
*  clib.h now standard
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
#include "../../../inc/logapi.h"

/*=============================================================================
==   Data Structures
=============================================================================*/

PDLLLINK pFirstModule = NULL;

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int WFCAPI ModuleLoad( char *, PDLLLINK );
int WFCAPI ModuleUnload( PDLLLINK );
// int WFCAPI ModuleEnum( LPBYTE, USHORT, PUSHORT );
int WFCAPI ModuleCall( PDLLLINK, USHORT, PVOID );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   Functions Used
=============================================================================*/


/*=============================================================================
==   Global Data
=============================================================================*/

/*******************************************************************************
 *
 *  ModuleInit
 *
 *    ModuleInit is called to get the entry point into the module routines
 *
 * ENTRY:
 *    pName (input)
 *       pointer to path of executable
 *    pDllLink (input)
 *       pointer to executable dlllink
 *    ppModuleProcedures (output)
 *       address to return pointer to array of external procedures
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
ModuleInit( PCHAR pName,
            PDLLLINK pDllLink,
            PPLIBPROCEDURE ppModuleProcedures )
{
//  EXEHEADER ExeHeader;
    PDLLLINK pNewLink;

    /*
     *  Return pointer to array of module procedures
     */
//  *ppModuleProcedures = (PLIBPROCEDURE) ModuleProcedures;

    /*
     *  Create an entry for the executable (winlink.exe)
     */
    if ( (pNewLink = (PDLLLINK) malloc( sizeof(DLLLINK) )) == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    /*
     * Init the DLLLINK structure to zero
     */
    memset( pNewLink, '\0', sizeof(DLLLINK));

    /*
     *  Call exe load routine
     */
    (void) MainLoad( pDllLink );

    /*
     *  Initialize data
     */
    strncpy( pDllLink->ModuleName, pName, sizeof(pDllLink->ModuleName) );

//    (void) _ReadHeader( pName, &ExeHeader, &pDllLink->ModuleDate,
//                        &pDllLink->ModuleTime, &pDllLink->ModuleSize );

    /* save the engine file data for embedded DLLs */
//    EngModDate = pDllLink->ModuleDate;
//    EngModTime = pDllLink->ModuleTime; 
//    EngModSize = pDllLink->ModuleSize; 

    /*
     *  Link exe module
     */
    pDllLink->pNext = pFirstModule;
    memcpy( pNewLink, pDllLink, sizeof(DLLLINK) );
    pFirstModule = pNewLink;

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  ModuleLoad
 *
 *    ModuleLoad is used to dynamically load a dll.  After loading a dll the
 *    open routine within the loaded dll must be called to initialize it.
 *
 * ENTRY:
 *    pName (input)
 *       pointer to name of dll
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error; CLIENT_ERROR_xxx code if error.
 *
 ******************************************************************************/

int ModuleLoad( char * pName, PDLLLINK pLink )
{
    PDLLLINK pNewLink;
    int (*fnLoad)(PDLLLINK);
    int rc;

    if ((rc = ModuleLookup( pName, (PPLIBPROCEDURE)&fnLoad, NULL )) != CLIENT_STATUS_SUCCESS)
	return rc;

    // init the pLink fields
    memset(pLink, 0, sizeof(*pLink));
    strncpy(pLink->ModuleName, pName, sizeof(pLink->ModuleName));

    if (fnLoad)
	rc = fnLoad(pLink);

    /*
     *  Keep a linked list of all loaded dlls
     */
    if ( (pNewLink = (PDLLLINK) malloc( sizeof(DLLLINK) )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;

	ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
	TRACE(( TC_LIB, TT_API1, "ModuleLoad: %s, rc=%u", pName, rc ));
        return rc;
    }
    memcpy( pNewLink, pLink, sizeof(DLLLINK) );
    pNewLink->pNext = pFirstModule;
    pFirstModule = pNewLink;

    return rc;
}

/*******************************************************************************
 *
 *  ModuleUnload
 *
 *    ModuleUnload is used to unload a dll loaded by ModuleLoad.  The dll close
 *    routine should be called before using this routine.
 *
 * ENTRY:
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int ModuleUnload( PDLLLINK pLink )
{
    PDLLLINK pDllLink;
    PDLLLINK pPrevLink;
    char ModuleName[ sizeof(pLink->ModuleName) ];
    int rc;

    TRACE(( TC_LIB, TT_API1, "ModuleUnload: %s, seg 0x%04x",
            pLink->ModuleName, pLink->Segment ));

    strcpy( ModuleName, pLink->ModuleName );

    rc = ModuleCall( pLink, DLL__UNLOAD, pLink );
    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    
    /*
     *  Locate entry being unloaded and free memory
     */
    pPrevLink = NULL;
    pDllLink = pFirstModule;
    while ( pDllLink ) {
        TRACE(( TC_LIB, TT_API3, "-- |%s| == |%s|",
                ModuleName, pDllLink->ModuleName ));
        if ( !strcmp( ModuleName, pDllLink->ModuleName ) )
            break;
        pPrevLink = pDllLink;
        pDllLink = pDllLink->pNext;
    }
    ASSERT( pDllLink, 0 );

    if ( pDllLink ) {
        if ( pPrevLink )
            pPrevLink->pNext = pDllLink->pNext;
        else
            pFirstModule = pDllLink->pNext;
        free( pDllLink );
    }

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  ModuleEnum
 *
 *    ModuleEnum is used to enumerate all the loaded dlls.
 *
 * ENTRY:
 *    pBuffer (output)
 *       pointer to buffer to return enum results (may be null)
 *    ByteCount (input)
 *       length of buffer
 *    pBytesAvail (output)
 *       size of buffer necessary to return enumeration
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
ModuleEnum( LPBYTE pBuffer, USHORT ByteCount, PUSHORT pBytesAvail )
{
    PDLLLINK pDllLink;
    USHORT BytesAvail;

    /*
     *  Calculate necessary buffer length
     */
    BytesAvail = 0;
    pDllLink = pFirstModule;
    while ( pDllLink ) {
        BytesAvail += ENUM_DLLLINK_SIZE;
        pDllLink = pDllLink->pNext;
    }

    if ( pBytesAvail )
        *pBytesAvail = BytesAvail;

    /*
     *  Check if user's buffer is big enough
     */
    if ( (ByteCount < BytesAvail) || (pBuffer == NULL) )
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );

    /*
     *  Copy enum data into user's buffer
     */
    pDllLink = pFirstModule;
    while ( pDllLink ) {
        memcpy( pBuffer, pDllLink, ENUM_DLLLINK_SIZE );
        pBuffer += ENUM_DLLLINK_SIZE;
        ByteCount -= ENUM_DLLLINK_SIZE;

        pDllLink = pDllLink->pNext;
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  ModuleCall
 *
 *    ModuleCall is used to call an external procedure in a loaded dll.
 *
 * ENTRY:
 *    pLink (input)
 *       pointer to the structure DLLLINK
 *    ProcIndex (input)
 *       index of dll function to call
 *    pParam (input/output)
 *       pointer to parameter for function
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
ModuleCall( PDLLLINK pLink, USHORT ProcIndex, PVOID pParam )
{
    PDLLPROCEDURE pProcedure;

#ifdef DEBUG
    ASSERT( pLink->pProcedures, 0 );
    if ( ProcIndex >= pLink->ProcCount )
	return( CLIENT_ERROR_BAD_PROCINDEX );
#endif
    pProcedure = ((PDLLPROCEDURE *) pLink->pProcedures)[ ProcIndex ];
    ASSERT( pProcedure, 0 );

    return( (*pProcedure)( pLink->pData, pParam ) );
}


