
/*************************************************************************
*
* neapi.c
*
* Name Enumerator common code
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/11/94)
*
*  $Log$
*  
*     Rev 1.9   15 Apr 1997 16:18:46   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   21 Mar 1997 16:06:44   bradp
*  update
*  
*     Rev 1.8   05 Dec 1995 18:45:38   bradp
*  update
*  
*     Rev 1.7   10 Nov 1995 11:18:22   bradp
*  update
*  
*     Rev 1.6   25 May 1995 11:27:28   butchd
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
#include "../../../inc/logapi.h"
#include "../../../inc/wfengapi.h"

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "../../../inc/neapi.h"
#include "../inc/ne.h"


/*=============================================================================
==   External Functions Defined
=============================================================================*/

#ifdef DOS
int WFCAPI Load( PDLLLINK );
int WFCAPI NeUnload( PNE, PDLLLINK );
int WFCAPI NeOpen( PNE, PNEOPEN );
int WFCAPI NeClose( PNE, PDLLCLOSE );
int WFCAPI NeInfo( PNE, PDLLINFO );
int WFCAPI NePoll( PNE, PDLLPOLL );
#endif
int WFCAPI NeEnumerate( PNE, PNEENUMERATE );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/


/*=============================================================================
==   External Functions used
=============================================================================*/

int DeviceEnumerate( PNE, PNEENUMERATE );


/*=============================================================================
==   Local Data
=============================================================================*/

#ifdef  DOS
/*
 *  Define Name Resolver external procedures
 */
PDLLPROCEDURE NeProcedures[ NE$COUNT ] = {
    (PDLLPROCEDURE) Load,
    (PDLLPROCEDURE) NeUnload,
    (PDLLPROCEDURE) NeOpen,
    (PDLLPROCEDURE) NeClose,
    (PDLLPROCEDURE) NeInfo,
    (PDLLPROCEDURE) NePoll,
    (PDLLPROCEDURE) NeEnumerate,
};

/*
 *  Define Name resolver data structure
 *   (this could be dynamically allocated)
 */
NE NeData = {0};
#endif // DOS

/*=============================================================================
==   Global Data
=============================================================================*/

PPLIBPROCEDURE pClibProcedures = NULL;
PPLIBPROCEDURE pLogProcedures = NULL;


#ifdef DOS  
/*******************************************************************************
 *
 *  Load - (DOS only)
 *
 *    The user interface calls NeLoad to load and link a name
 *    resolver.
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
Load( PDLLLINK pLink )
{
    /*
     *  Initialize DllLink structure
     */
    pLink->ProcCount   = NE$COUNT;
    pLink->pProcedures = NeProcedures;
    pLink->pData       = &NeData;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NeUnload - (DOS only)
 *
 *  DllUnload calls NeUnload to unlink and unload a name resolver
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name resolver data structure
 *    pLink (input/output)
 *       pointer to the structure DLLLINK
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NeUnload( PNE pNe, PDLLLINK pLink )
{
    pLink->ProcCount = 0;
    pLink->pProcedures = NULL;
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NeOpen - (DOS only)
 *
 *  The user interface calls NeOpen to open and initialize a Ne.
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name driver data structure
 *    pNeOpen (input/output)
 *       pointer to the structure NEOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NeOpen( PNE pNe, PNEOPEN pNeOpen )
{
    /*
     *  Initialize NE function call tables: MUST BE FIRST!
     */
    pLogProcedures = pNeOpen->pLogProcedures;
    pClibProcedures = pNeOpen->pClibProcedures;

    /*
     *  Initialize NE data structure
     */
    memset( pNe, 0, sizeof(NE) );

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NeClose - (DOS only)
 *
 *  The user interface calls NeClose to close a Ne before unloading it.
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name driver data structure
 *    pNeClose (input/output)
 *       pointer to the structure DLLCLOSE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NeClose( PNE pNe, PDLLCLOSE pNeClose )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NeInfo - (DOS only)
 *
 *    This routine is called to get information about this module
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name driver data structure
 *    pNeInfo (input/output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NeInfo( PNE pNe, PDLLINFO pNeInfo )
{
    USHORT ByteCount;
    PMODULE_C2H pHeader;

    /*
     *  Get byte count necessary to hold data
     */
    ByteCount = sizeof(MODULE_C2H);

    /*
     *  Check if buffer is big enough
     */
    if ( pNeInfo->ByteCount < ByteCount ) {
        pNeInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize module header
     */
    pNeInfo->ByteCount = ByteCount;
    pHeader = (PMODULE_C2H) pNeInfo->pBuffer;
    memset( pHeader, 0, ByteCount );
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_NameEnumerator;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  NePoll - (DOS only)
 *
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name driver data structure
 *    pNePoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NePoll( PNE pNe, PDLLPOLL pNePoll )
{
    return( CLIENT_STATUS_SUCCESS );
}
#endif // DOS

/*******************************************************************************
 *
 *  NeEnumerate - DOS and WINDOWS
 *
 *   NeEnumerate returns an array of application servers
 *
 * ENTRY:
 *    pNe (input)
 *       pointer to name driver data structure
 *    pNeEnum (input/output)
 *       pointer to the structure NEENUMERATE
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
NeEnumerate( PNE pNe, PNEENUMERATE pNeEnum )
{
    return( DeviceEnumerate( pNe, pNeEnum ) );
}

