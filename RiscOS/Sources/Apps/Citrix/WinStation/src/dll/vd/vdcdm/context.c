/*************************************************************************
*
* Context.c
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author: John Richardson 01/20/94
*         Rich Andresen   06/12/95
*
* Log:
*
*************************************************************************/

#include "windows.h"
#include "fileio.h"

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

#ifdef DEBUG
VOID DumpContexts();
#else
#define DumpContexts()
#endif

STATIC POPENCONTEXT ContextTable[ MAX_OPEN_CONTEXT ] = { 0 };




/*****************************************************************************
 *
 *  AllocateContext
 *
 *   Allocate an open file or directory context.
 *
 *
 * ENTRY:
 *   Type (input)
 *     Type of open Context, file, or directory findfirst/findnext.
 *
 *
 * EXIT:
 *   Returns the Context identifier allocated, or (-1) for an
 *   allocation error.
 *
 ****************************************************************************/

USHORT STATIC
AllocateContext( int Type )
{
    USHORT index;

    // Find a free entry
    for( index=0; index < MAX_OPEN_CONTEXT; index++ ) {

        if( ContextTable[index] == 0 ) {

            ContextTable[index] = malloc( sizeof( OPENCONTEXT ) );
            if( ContextTable[index] == 0 ) {

               TRACE(( TC_CDM, TT_API1, "AllocateContext: Out of memory"));
               return( (USHORT)(-1) );
            }

            ContextTable[index]->Type = Type;

            // Now allocate the entry
            switch( Type ) {

            case OPENCONTEXT_FILE:
                       ContextTable[index]->x.pFileEnt = malloc( sizeof( FILEENT ) );
                       if( ContextTable[index]->x.pFileEnt == 0 ) {

                           TRACE(( TC_CDM, TT_API1, "AllocateContext: Out of memory"));
                           free( ContextTable[index] );
                           ContextTable[index] = 0;
                           return( (USHORT)(-1) );
                       }
                       TRACE(( TC_CDM, TT_API4, "AllocateContext File: Index=%d", index));
                       return( index );

            case OPENCONTEXT_FIND:
                       ContextTable[index]->x.pFindBuf = malloc( sizeof(FIND)  );
                       if(ContextTable[index]->x.pFindBuf == 0 ) {

                           TRACE(( TC_CDM, TT_API1, "AllocateContext: Out of memory"));
                           free( ContextTable[index] );
                           ContextTable[index] = 0;
                           return( (USHORT)(-1) );
                       }
                       TRACE(( TC_CDM, TT_API4, "AllocateContext Find: Index=%d", index));
                       ContextTable[index]->DirIndex = 0;
                       return( index );

            default:
                       TRACE(( TC_CDM, TT_API1, "AllocateContext: Default!!"));
                       free( ContextTable[index] );
                       ContextTable[index] = 0;
                       return( (USHORT)(-1) );
            }
        }
    }

    // No free entries
    TRACE(( TC_CDM, TT_API1, "AllocateContext: No Free Entries!!"));
    DumpContexts();
    return( (USHORT)(-1) );
}


/*****************************************************************************
 *
 *  ValidateContext
 *
 *    Check to see if the supplied context is valid. Returns a
 *    pointer to a context structure if valid. NULL if not.
 *
 *
 * ENTRY:
 *    FileId
 *     The context entry to lookup
 *
 *
 * EXIT:
 *  NULL if the FileId was invalid
 *  Pointer to the OPENCONTEXT structure if valid.
 *
 ****************************************************************************/

POPENCONTEXT STATIC
ValidateContext( USHORT FileId )
{
    if( FileId > MAX_OPEN_CONTEXT ) {
        DumpContexts();
	return( NULL );
    }

    if( ContextTable[ FileId ] ) {
        return( ContextTable[ FileId ] );
    }
    else {
        DumpContexts();
        return( NULL );
    }
}


/*****************************************************************************
 *
 *  FreeContext
 *
 *   Free the context entry and it associated memory. It is up
 *   the caller to have closed any open handles, etc.
 *
 *
 * ENTRY:
 *    FileId
 *     The context entry to free
 *
 *
 * EXIT:
 *
 ****************************************************************************/

void STATIC
FreeContext( USHORT FileId )
{
    if( ValidateContext( FileId ) == NULL ) {
        TRACE(( TC_CDM, TT_ERROR, "FreeContext: FileId is NULL"));
        return;
    }

    switch( ContextTable[ FileId ]->Type ) {

        case OPENCONTEXT_FILE:
            TRACE(( TC_CDM, TT_API4, "FreeContext File: Index=%d", FileId));
            free( ContextTable[ FileId ]->x.pFileEnt );
            break;

        case OPENCONTEXT_FIND:
            TRACE(( TC_CDM, TT_API4, "FreeContext Find: Index=%d", FileId));
            TRACE(( TC_CDM, TT_API1, "FreeContext: Free FIND, FindId=%d", FileId));
            free( ContextTable[ FileId ]->x.pFindBuf );
            break;

        default:
            TRACE(( TC_CDM, TT_API1, "FreeContext: Default Context Type?"));
            break;
    }

    free( ContextTable[ FileId ] );
    ContextTable[ FileId ] = 0;
}


/*****************************************************************************
 *
 *  CdmDosCloseAllContexts
 *
 *   This is called when we are being disconnected from a server
 *   communications link and all open contexts need to be freed.
 *
 * ENTRY:
 *
 * EXIT:
 *
 *
 ****************************************************************************/

void STATIC
CdmDosCloseAllContexts()
{
    USHORT index;

    /*
     * Loop through all entries closing handles and freeing context memory
     */
    for( index=0; index < MAX_OPEN_CONTEXT; index++ ) {

        if( ContextTable[index] == 0 ) {

            continue; // This entry is free
        }
        else {
            switch( ContextTable[index]->Type ) {

                case OPENCONTEXT_FILE:
                      /*
                       * Must close the Dos handle
                       */
                      CLOSE( ContextTable[index]->x.pFileEnt->DosHandle );

                      free( ContextTable[index]->x.pFileEnt );
                      break;

                case OPENCONTEXT_FIND:
                      /*
                       * DOS does not maintain any internal state
                       * from a FindFirst/FindNext. All state is contained
                       * in the FINDBUF that we allocated.
                       */

                      free( ContextTable[index]->x.pFindBuf );
                      break;

                default:
                      break;
            }

            /*
             * Now free the entry and mark it invalid
             */
            free( ContextTable[index] );
            ContextTable[index] = 0;
        }
    }
}

/*****************************************************************************
 *
 *  ContextGetAttributes( pName, PathNameSize, pAttributes, pFileSize );
 *
 *   Free the context entry and it associated memory. It is up
 *   the caller to have closed any open handles, etc.
 *
 *
 * ENTRY:
 *    FileId
 *     The context entry to free
 *
 *
 * EXIT:
 *
 ****************************************************************************/

void STATIC
ContextGetAttributes( PCHAR pName, USHORT PathNameSize, PUSHORT pAttributes, PULONG pFileSize )
{
    USHORT index;

    // Find a valid FILE entry
    for( index=0; index < MAX_OPEN_CONTEXT; index++ ) {

        // Is it a valid OPENCONTEXT_FILE type
        if ( (ContextTable[index] != 0 ) &&
             (ContextTable[index]->Type == OPENCONTEXT_FILE) &&

	     ( (ContextTable[index]->x.pFileEnt->OpenForWriting == TRUE) ||
	     (ContextTable[index]->x.pFileEnt->fAttributesChanged == TRUE) ) &&

	     (ContextTable[index]->x.pFileEnt->NameSize == PathNameSize) &&
	     (!memicmp(ContextTable[index]->x.pFileEnt->pName, pName, PathNameSize))
            ) {

	    TRACE(( TC_CDM, TT_API4, "ContextGA: It's a Hit"));

            //
	    // Get the updated attributes
	    //
	    if( ContextTable[index]->x.pFileEnt->fAttributesChanged == TRUE ) {
	        *pAttributes = ContextTable[index]->x.pFileEnt->Attributes;
	    }

            //
	    // Get the updated file size
	    //
	    if( ContextTable[index]->x.pFileEnt->OpenForWriting == TRUE ) {
	        *pFileSize = SEEK( ContextTable[index]->x.pFileEnt->DosHandle, 0L, 2 );
	    }

	    return;
        }
    }

    // Don't change anything.
    TRACE(( TC_CDM, TT_API4, "ContextGA: Not found or Attributes did not change."));
    return;
}


#ifndef WIN32

/*****************************************************************************
 *
 *  ContextSetAttributes( pName, PathNameSize, Attributes )
 *
 *   Free the context entry and it associated memory. It is up
 *   the caller to have closed any open handles, etc.
 *
 *
 * ENTRY:
 *    FileId
 *     The context entry to free
 *
 *
 * EXIT:
 *
 ****************************************************************************/

BOOL STATIC
ContextSetAttributes( PCHAR pName, USHORT PathNameSize, USHORT Attributes )
{
    USHORT index;

    // Find a valid FILE entry
    for( index=0; index < MAX_OPEN_CONTEXT; index++ ) {

        // Is it a valid OPENCONTEXT_FILE type
        if ( (ContextTable[index] != 0 ) &&
             (ContextTable[index]->Type == OPENCONTEXT_FILE) &&
             (ContextTable[index]->x.pFileEnt->NameSize == PathNameSize) &&
             (!memicmp(ContextTable[index]->x.pFileEnt->pName, pName, PathNameSize))
            ) {
            TRACE(( TC_CDM, TT_API4, "ContextSA: It's a Hit, attributes(0x%x) set", Attributes));
            if ( !ContextTable[index]->x.pFileEnt->fAttributesValid ||
                 ((ContextTable[index]->x.pFileEnt->fAttributesValid) &&
                  (ContextTable[index]->x.pFileEnt->Attributes != Attributes)) ) {
                ContextTable[index]->x.pFileEnt->fAttributesChanged = TRUE;
            }
            ContextTable[index]->x.pFileEnt->fAttributesValid = TRUE;
            ContextTable[index]->x.pFileEnt->Attributes = Attributes;
            return( TRUE );
        }
    }

    // No free entries
    TRACE(( TC_CDM, TT_API4, "ContextSA: Not found."));
    return( FALSE );
}

#endif

#ifdef DEBUG
VOID STATIC DumpContexts()
{
    USHORT Index;
    POPENCONTEXT p;

    for( Index=0; Index < MAX_OPEN_CONTEXT; Index++ ) {
        TRACE(( TC_CDM, TT_ERROR, "Context %d 0x%x\n",Index,ContextTable[Index]));
        p = ContextTable[Index];
        if( p ) {
            TRACE(( TC_CDM, TT_ERROR, "Type %d, DirIndex %d, FindHandle 0x%x \n",p->Type,p->DirIndex,p->FindHandle));
            TRACE(( TC_CDM, TT_ERROR, "FindBufferFull %d \n",p->FindBufferFull));
	}
    }
}
#endif


