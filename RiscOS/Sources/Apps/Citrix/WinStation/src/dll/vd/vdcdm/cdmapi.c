/*************************************************************************
*
* cdmapi.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"

#include "fileio.h"
#include <time.h>

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

/*
 * Forward references
 */
BOOLEAN
CdmMarshallFind(
    PFIND   pFind,
    USHORT  LongFileNames,
    PCHAR   pBuf,
    USHORT  FindBufSize,
    PUSHORT pFindBufRead
    );


/*****************************************************************************
 *
 *  CdmDosCreate
 *
 *  Handle a file create call. This is not a straight DOS create, but
 *  a combination of create new file, create file, depending on the
 *  create mode(s).
 *
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of file to create
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   AccessMode (input)
 *     The access mode(s) to have the file open under after creating
 *
 *   CreateMode (input)
 *     Flags defining whether to truncate, fail if exists, etc.
 *
 *   Attributes (input)
 *     DOS 3.x file attributes to set on a new file if created
 *
 *   pFileId (output)
 *     Pointer to variable to place the new FileId if the file is opened
 *     for I/O after creation
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC
CdmDosCreate(
    PCHAR   pName,
    USHORT  PathNameSize,
    USHORT  AccessMode,
    USHORT  CreateMode,
    USHORT  Attributes,
    PUSHORT pFileId,
    PUSHORT pResult
    )
{
    int ret;
    unsigned int DosAttributes, DosAccess;
    USHORT Context;
    USHORT ErrCode, ErrClass;
    POPENCONTEXT p;
    PCHAR pPathNameBuf;

    // Convert to 16 bit ints for DOS

    ASSERT( (AccessMode & 0xFFFF0000L) == 0, 0 );
    ASSERT( (Attributes & 0xFFFF0000L) == 0, 0 );

    DosAccess = (unsigned int)AccessMode;
    DosAttributes = (unsigned int)Attributes;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

    TRACE(( TC_CDM, TT_API4, "CdmDosCreate: Name :%s:, Access 0x%x, Attr 0x%x", pPathNameBuf, DosAccess, DosAttributes));

    /*
     * Combinations of CreateMode:
     *
     * Create exclusive (CDM_CREATE_NEWFILE no truncate or open existing)
     *
     * Create empty file always (CDM_CREATE_NEWFILE CDM_TRUNCATE )
     *
     * Open, or Create no data clobber (CDM_CREATE_NEWFILE CDM_OPEN_EXISTING )
     */

    /*
     * Check for invalid CreateMode combinations
     */
    if( (CreateMode & CDM_CREATE_NEWFILE) &&
        (CreateMode & CDM_OPEN_EXISTING)  &&
        (CreateMode & CDM_TRUNCATE ) ) {

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: Bad mode combination NEW|EXIST|TRUNC"));
        free( pPathNameBuf );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_BADACCESS );
        return;
    }

    /*
     * If the caller specified truncate, make sure they
     * have specified write access, and its not to be a
     * read only file.
     *
     */
    if( (CreateMode & CDM_TRUNCATE) &&
          ( ((AccessMode != CDM_WRITEACCESS ) && (AccessMode != CDM_READWRITE ))
                    ||
            (Attributes & CDM_ATTR_READONLY)
          ) ) {

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: Bad mode combination TRUNC & !WRITE_ACCESS"));
        free( pPathNameBuf );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_BADACCESS );
        return;
    }

    /*
     * Allocate a file context entry
     */
    Context = AllocateContext( OPENCONTEXT_FILE );

    if( Context == (USHORT)(-1) ) {
        // Out of context entries
        free( pPathNameBuf );
        TRACE(( TC_CDM, TT_ERROR, "CdmDosCreate: AllocateContext failed."));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Attempt to get a pointer to the file context entry
     * by validating the returned Context
     */
    p = ValidateContext( Context );
    if( p == NULL ) {
        free( pPathNameBuf );
        TRACE(( TC_CDM, TT_ERROR, "CdmDosCreate: ValidateContext failed."));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate
    strncpy( p->x.pFileEnt->pName, pName, PathNameSize );
    p->x.pFileEnt->pName[PathNameSize] = 0;
    p->x.pFileEnt->NameSize = PathNameSize;
    p->x.pFileEnt->Attributes = DosAttributes;
    p->x.pFileEnt->fAttributesChanged = FALSE;
    p->x.pFileEnt->fAttributesValid = TRUE;
#ifdef PERF_PROFILE
    p->IsNullFile = FALSE;
#endif

    //
    // Mark that we are open for writing. This tells
    // GetAttributes to use the lseek() on the open handle,
    // and not FindFirst(). This is because FindFirst() returns
    // zero for file size if a file is open for writing.
    //
    p->x.pFileEnt->OpenForWriting = TRUE;

    if( (CreateMode & CDM_CREATE_NEWFILE) &&
        !(CreateMode & CDM_OPEN_EXISTING) &&
        !(CreateMode & CDM_TRUNCATE) ) {

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: _dos_creatnew exclusive (NEWFILE & !EXIST & !TRUNC"));

        /*
         * This is an exclusive create
         */
        ret = 0;
        p->x.pFileEnt->DosHandle = -1;
#ifdef  WIN32
        /*
         *  If the file exists then don't do the create, assume an error
         *  indacates "file not found"
         */
        if ( GetFileAttributes(pPathNameBuf) == -1L )
#endif
        CREATNEW( pPathNameBuf, DosAttributes, p->x.pFileEnt->DosHandle, ret );

        if ( (ret != 0) || (p->x.pFileEnt->DosHandle == 0xffff) ) {
            // Get the error code and class
            CdmDosError( ret, &ErrClass, &ErrCode );
            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CREATNEW failed, ret=%d, ErrCode=%d", ret, ErrCode));
            FreeContext( Context );
            free( pPathNameBuf );
            *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
            *pFileId = (USHORT)(-1);
            return;
        } else {
#ifdef REOPENFILE  // We leave it RW, let the client handle it
                   // since RO create makes no sense anyway.
            //
            // We check to see if the created file should be
            // readonly access and reopen it for readonly if
            // so. We can not close and reopen if the file is
            // read/write since a file can be created as an READONLY
            // attribute file, but it can be written on the handle
            // returned on the create.
            // New opens for writing will be rejected.
            //

            if( (AccessMode & CDM_ACCESS_MASK) == CDM_READACCESS ) {
                TRACE(( TC_CDM, TT_API4, "CdmDosCreate: _dos_creatnew exclusive RO... reopen now"));
                CLOSE( p->x.pFileEnt->DosHandle );

                ret = 0;
                p->x.pFileEnt->DosHandle = 0;
                OPEN( pPathNameBuf, DosAccess, p->x.pFileEnt->DosHandle, ret );

                if ( (ret != 0) || (p->x.pFileEnt->DosHandle == 0xffff) ) {
#ifdef  WIN32
                    ret = GetLastError();
#endif
                    TRACE(( TC_CDM, TT_API4, "CdmDosCreate: open after creatnew failed ret 0x%x",ret));
                    CdmDosError( ret, &ErrClass, &ErrCode );
                    FreeContext( Context );
                    free( pPathNameBuf );
                    *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
                    *pFileId = (USHORT)(-1);
                    return;
                }

                TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CreatenewExclusiveRO, successfull return"));
                // Everythings OK now
                *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                *pFileId = Context;
                free( pPathNameBuf );
                return;
            }
            else {

                TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CreatenewExclusiveRW, successfull return"));
                *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                *pFileId = Context;
#ifdef PERF_PROFILE
                if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
                    p->IsNullFile = TRUE;
		}
#endif
		free( pPathNameBuf );
                return;
            }
#else

            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CreatenewExclusiveRW, successful return"));
            *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
            *pFileId = Context;
            free( pPathNameBuf );
            return;
        }
#endif //REOPEN

    } else if( (CreateMode & CDM_CREATE_NEWFILE) &&
             (CreateMode & CDM_TRUNCATE) ) {
        /*
         * We want to create a newfile, clobbering
         * the existing one if it exists
         */

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: _dos_creat with TRUNCATE"));

        ret = 0;
        p->x.pFileEnt->DosHandle = 0;
        CREAT( pPathNameBuf, DosAttributes, p->x.pFileEnt->DosHandle, ret );

        if ( (ret != 0) || (p->x.pFileEnt->DosHandle == -1) ) {
            // Get the error code and class
#ifdef  WIN32
            ret = GetLastError();
#endif
            CdmDosError( ret, &ErrClass, &ErrCode );
            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CREAT failed, ret=%d, EC=%d", ret, ErrCode));
            FreeContext( Context );
            free( pPathNameBuf );
            *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
            *pFileId = (USHORT)(-1);
            return;
        }
#ifdef REOPENFILE
        // Now close the handle and reopen it with the proper modes

        CLOSE( p->x.pFileEnt->DosHandle );

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: _dos_creat truncate... reopen now"));

        ret = 0;
        p->x.pFileEnt->DosHandle = 0;
        OPEN( pPathNameBuf, DosAccess, p->x.pFileEnt->DosHandle, ret );

        if ( (ret != 0) || (p->x.pFileEnt->DosHandle == -1) ) {
#ifdef  WIN32
            ret = GetLastError();
#endif
            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: open after creat failed ret 0x%x",ret));
            CdmDosError( ret, &ErrClass, &ErrCode );
            FreeContext( Context );
            free( pPathNameBuf );
            *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
            *pFileId = (USHORT)(-1);
            return;
        }

#endif //REOPENFILE

        TRACE(( TC_CDM, TT_API4, "CdmDosCreate: Create with truncate success, returning"));
        // Everythings OK now
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        *pFileId = Context;
#ifdef PERF_PROFILE
        if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
            p->IsNullFile = TRUE;
	}
#endif
        free( pPathNameBuf );
        return;
    } else if( CreateMode & CDM_OPEN_EXISTING ) {

       TRACE(( TC_CDM, TT_API4, "CdmDosCreate: Open existing"));
        /*
         * Try to open an existing file.
         */
        ret = 0;
        p->x.pFileEnt->DosHandle = 0;
        OPEN( pPathNameBuf, DosAccess, p->x.pFileEnt->DosHandle, ret );

        if ( (ret == 0) && (p->x.pFileEnt->DosHandle != -1) ) {
            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: open existing successfull"));
            // Successfull open
            *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
            *pFileId = Context;
#ifdef PERF_PROFILE
            if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
                p->IsNullFile = TRUE;
	    }
#endif
            free( pPathNameBuf );
            return;
        } else {
#ifdef  WIN32
            ret = GetLastError();
#endif
            TRACE(( TC_CDM, TT_API4, "CdmDosCreate: _dos_open failed, but CDM_CREATE_NEWFILE"));
            // If File not found and CDM_CREATE_NEWFILE, try to create it
            if( (ret == 2 ) && (CreateMode & CDM_CREATE_NEWFILE) ) {
                ret = 0;
                p->x.pFileEnt->DosHandle = 0;
                CREAT( pPathNameBuf, DosAttributes, p->x.pFileEnt->DosHandle, ret );
                if ( (ret != 0) || (p->x.pFileEnt->DosHandle == -1) ) {
                    CdmDosError( ret, &ErrClass, &ErrCode );
                    TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CREATNEW failed. ret=%d, EC=%d", ret, ErrCode));
                    FreeContext( Context );
                    free( pPathNameBuf );
                    *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
                    *pFileId = (USHORT)(-1);
                    return;
                } else {
                    *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                    TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CREATNEW Success. Index=%d, Handle=%d", Context, p->x.pFileEnt->DosHandle));
                    *pFileId = Context;
#ifdef PERF_PROFILE
                    if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
                        p->IsNullFile = TRUE;
	            }
#endif
                    free( pPathNameBuf );
                    return;
                }
#ifdef REOPENFILE
                // We have created a new file, close it and reopen
                // it with the proper access modes

                CLOSE( p->x.pFileEnt->DosHandle );

                TRACE(( TC_CDM, TT_API4, "CdmDosCreate: File created, now reopening"));
                ret = 0;
                p->x.pFileEnt->DosHandle = 0;
                OPEN( pPathNameBuf, DosAccess, p->x.pFileEnt->DosHandle, ret );

                if ( (ret == 0) && (p->x.pFileEnt->DosHandle != -1) ) {
                    TRACE(( TC_CDM, TT_API4, "CdmDosCreate: File opened successfully"));
                    // Successfull open
                    *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                    *pFileId = Context;
#ifdef PERF_PROFILE
                    if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
                        p->IsNullFile = TRUE;
	            }
#endif
                    free( pPathNameBuf );
                    return;
                } else {
#ifdef  WIN32
                    ret = GetLastError();
#endif
                    CdmDosError( ret, &ErrClass, &ErrCode );
                    TRACE(( TC_CDM, TT_API4, "Error reopening file after OPEN, ret=%d, EC=%d", ret, ErrCode));
                    FreeContext( Context );
                    *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
                    *pFileId = (USHORT)(-1);
                    free( pPathNameBuf );
                    return;
                }
#else
                // Successfull open
                *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                *pFileId = Context;
#ifdef PERF_PROFILE
                if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
                    p->IsNullFile = TRUE;
	        }
#endif
                free( pPathNameBuf );
                return;

#endif // REOPENFILE
            } else {
                // CDM_CREATE_NEWFILE not set
                CdmDosError( ret, &ErrClass, &ErrCode );
                TRACE(( TC_CDM, TT_API4, "CdmDosCreate: CDM_CREATE_NEWFILE not set, no file found, ret=%d", ret));
                FreeContext( Context );
                *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
                *pFileId = (USHORT)(-1);
                free( pPathNameBuf );
                return;
            }
        }
    } else {
        TRACE(( TC_CDM, TT_API4, "CDM DOS IO: Create, Bad mode combination Mode 0x%x, Access 0x%x, Attr 0x%x",
               CreateMode, AccessMode, Attributes ));
        FreeContext( Context );
        free( pPathNameBuf );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosOpen
 *
 *  Handle a file open call.
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of file to create
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   AccessMode (input)
 *     The access mode(s) to have the file open under after creating
 *
 *   Attributes (input)
 *     DOS 3.x file attributes to set on a new file if created
 *
 *   pFileId (output)
 *     Pointer to variable to place the new FileId if the file is opened
 *     for I/O after creation
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosOpen(
    PCHAR   pName,
    USHORT  PathNameSize,
    USHORT  AccessMode,
    PUSHORT pFileId,
    PUSHORT  pResult
    )
{
    int ret;
    unsigned int DosAccess;
    USHORT Context;
    USHORT ErrCode, ErrClass;
    POPENCONTEXT p;
    PCHAR pPathNameBuf;


    // Convert to 16 bit ints for DOS

    ASSERT( (AccessMode & 0xFFFF0000L) == 0, 0 );

    DosAccess = (unsigned int)AccessMode;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosOpen: Pathname to long"));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosOpen: Could not allocate pathname buffer %d bytes", PathNameSize));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;


    /*
     * Allocate a file context entry
     */
    Context = AllocateContext( OPENCONTEXT_FILE );

    if( Context == (USHORT)(-1) ) {
        // Out of context entries
        free( pPathNameBuf );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Attempt to get a pointer to the file context entry
     * by validating the returned Context
     */
    p = ValidateContext( Context );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        free( pPathNameBuf );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate
    strncpy( p->x.pFileEnt->pName, pName, PathNameSize );
    p->x.pFileEnt->pName[PathNameSize] = 0;
    p->x.pFileEnt->NameSize = PathNameSize;
    p->x.pFileEnt->Attributes = 0xffff;
    p->x.pFileEnt->fAttributesChanged = FALSE;
    p->x.pFileEnt->fAttributesValid = FALSE;
#ifdef PERF_PROFILE
    p->IsNullFile = FALSE;
#endif

    //
    // Mark that we are open for writing. This tells
    // GetAttributes to use the lseek() on the open handle,
    // and not FindFirst(). This is because FindFirst() returns
    // zero for file size if a file is open for writing.
    //
    if( (DosAccess & CDM_ACCESS_MASK) == CDM_READACCESS ) {
        p->x.pFileEnt->OpenForWriting = FALSE;
    }
    else {
	p->x.pFileEnt->OpenForWriting = TRUE;
    }

    TRACE(( TC_CDM, TT_API4, "CdmDosOpen: Name :%s: DosAccess 0x%x", pPathNameBuf, DosAccess));

    /*
     * Try to open an existing file.
     */
    ret = 0;
    p->x.pFileEnt->DosHandle = 0;
    OPEN( pPathNameBuf, DosAccess, p->x.pFileEnt->DosHandle, ret );

    if ( (ret == 0) && (p->x.pFileEnt->DosHandle != -1) ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosOpen: Successfull, returning"));
        // Successfull open
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        *pFileId = Context;
#ifdef PERF_PROFILE
	if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
            p->IsNullFile = TRUE;
	}
#endif
	free( pPathNameBuf );
        return;
    } else {
        // Error opening the file
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_API4, "CdmDosOpen: Error ret=%d, EC=%d", ret, ErrCode));
        FreeContext( Context );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        *pFileId = (USHORT)(-1);
        free( pPathNameBuf );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosClose
 *
 *  Handle a file close call.
 *
 * ENTRY:
 *
 *   FileId (output)
 *     The file context to close
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosClose(
    USHORT FileId,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrClass, ErrCode;
    POPENCONTEXT p;

    /*
     * Attempt to get a pointer to the file context entry
     * by validating the returned Context
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosClose: Bad Context %d", FileId));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    TRACE(( TC_CDM, TT_API4, "CdmDosClose: Close Index=%d, handle=%x", FileId, p->x.pFileEnt->DosHandle ));

    /*
     * close the existing file context
     */
    ret = CLOSE( p->x.pFileEnt->DosHandle );

#ifndef WIN32
    /*
     *  If a set attribute occurred while we had the file open then
     *  we delay that call til now.  (Only if the attributes actually changed)
     */
    if ( p->x.pFileEnt->fAttributesChanged == TRUE ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosClose: SetFileAttr after close, Index=%d.", FileId ));
        _dos_setfileattr( p->x.pFileEnt->pName, p->x.pFileEnt->Attributes );
    }
#endif

    if ( ret == 0 ) {
        // Successfull close
        TRACE(( TC_CDM, TT_API4, "CdmDosClose: Close successful, handle=%x", p->x.pFileEnt->DosHandle ));
        FreeContext( FileId );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    } else {
        // Error closing the file
        FreeContext( FileId );
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        TRACE(( TC_CDM, TT_API4, "CdmDosClose: FileId %d, Error Closing Dos Handle ErrClass 0x%x, ErrCode 0x%x", FileId, ErrClass, ErrCode ));
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosRead
 *
 *  Handle a file Read call.
 *
 *  This will read up to Size data into the buffer. Short reads other than
 *  0 are not an indication of end of file, but could be due to buffer space
 *  available.
 *
 *  A read of 0 with the status return of CDM_ERROR_EOF is an actual EOF
 *  indication.
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to read
 *
 *   pBuf (output)
 *     Buffer to read the requested data into
 *
 *   Size (input)
 *     Amount of data to read
 *
 *   Offset (input)
 *     Offset in file to start reading from
 *
 *   pTimeStamp (output)
 *     Current DOS time stamp on the file as defined cdmwire.h
 *
 *   pDateStamp (output)
 *     Current DOS date stamp on the file as defined in cdmwire.h
 *
 *   pTotalRead (output)
 *     Pointer to variable to place the amount read from the file.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosRead(
    USHORT  FileId,
    PCHAR   pBuf,
    USHORT  Size,
    ULONG   Offset,
    PUSHORT pTimeStamp,
    PUSHORT pDateStamp,
    PUSHORT pTotalRead,
    PUSHORT pResult
    )
{
    int             ret;
    POPENCONTEXT    p;
    USHORT          DosSize, DosTotalRead, ErrClass, ErrCode;
    LONG            Temp;

    DosSize = (USHORT)Size;
    DosTotalRead = 0;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        TRACE(( TC_CDM, TT_ERROR, "CdmDosRead: Invalid Server FileId 0x%x", FileId));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

#ifdef PERF_PROFILE
    /*
     * Return any garbage fast on this special file
     * for profiling clientdrive/ICA performance on
     * a given CLIENT/HOST configuration.
     */
    if( p->IsNullFile ) {
        *pTotalRead = DosSize;
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
#endif

    /*
     * Seek to the offset we are going to read from
     */
    Temp = SEEK( p->x.pFileEnt->DosHandle, Offset, 0 );

    if( Temp == -1L ) {
        // Problems with handle
        *pTotalRead = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        TRACE(( TC_CDM, TT_ERROR, "CdmDosRead: Seek Error! FileId 0x%x", FileId));
        return;
    }

    /*
     * Now do the DOS read
     */
    ret = 0;
    FSREAD( p->x.pFileEnt->DosHandle, pBuf, DosSize, DosTotalRead, ret );

    if( ret || (DosTotalRead == -1) ) {
        *pTotalRead = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_ERROR, "CdmDosRead: Read Error, Class %d, Code %d", ErrClass, ErrCode));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    } else {
        // No errors
        *pTotalRead = DosTotalRead;

        /*
         * We now return the current DOS file time and
         * date stamps for client cache control
         */
        ret = _dos_getftime( p->x.pFileEnt->DosHandle,
                             (UINT *)pDateStamp,
                             (UINT *)pTimeStamp
                           );
        // If read handle was valid, getftime handle is valid
        ASSERT( ret == 0, 0 );

        TRACE(( TC_CDM, TT_API4, "CdmDosRead: Req %d, Got %d", DosSize, DosTotalRead));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosWrite
 *
 *  Handle a file write call.
 *
 *  This will write up to Size data from the buffer into the file.
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to write
 *
 *   pBuf (output)
 *     Buffer to write the requested data from
 *
 *   Size (input)
 *     Amount of data to write
 *
 *   Offset (input)
 *     Offset in file to start writing from
 *
 *   pAmountWrote (output)
 *     Pointer to variable to place the amount of data written to the file.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosWrite(
    USHORT  FileId,
    PCHAR   pBuf,
    USHORT  Size,
    ULONG   Offset,
    PUSHORT pAmountWrote,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT DosSize, DosAmountWrote, ErrClass, ErrCode;
    LONG    Temp;

    DosSize = (USHORT)Size;
    DosAmountWrote = 0;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    TRACE(( TC_CDM, TT_API4, "CdmDosWrite: Attempt write, index=%d, BytesToWrite=%d", FileId, Size));
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

#ifdef PERF_PROFILE
    /*
     * Return any garbage fast on this special file
     * for profiling clientdrive/ICA performance on
     * a given CLIENT/HOST configuration.
     */
    if( p->IsNullFile ) {
        *pAmountWrote = DosSize;
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
#endif

    /*
     * Seek to the offset we are going to start the write from
     */
    Temp = SEEK( p->x.pFileEnt->DosHandle, Offset, 0 );

    if( Temp == -1L ) {
        // Problems with handle
        *pAmountWrote = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        TRACE(( TC_CDM, TT_ERROR, "CdmDosWrite: Error Seeking for write"));
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }

    TRACE(( TC_CDM, TT_API4, "CdmDosWrite: Write to Handle %d", p->x.pFileEnt->DosHandle));

    /*
     * Now do the DOS write
     */
    ret = 0;
    FSWRITE( p->x.pFileEnt->DosHandle, pBuf, DosSize, DosAmountWrote, ret );

    if ( ret || (DosAmountWrote == -1) ) {
        *pAmountWrote = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_ERROR, "CdmDosWrite: Write failed, ret=%d, EC=%d", ErrCode, ErrClass));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    } else {
        // No errors
        *pAmountWrote = DosAmountWrote;
        TRACE(( TC_CDM, TT_API4, "CdmDosWrite: Write successful"));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosFindFirstIndex
 *
 *  Find the first entries in a directory at a specific starting
 *  index by pathname.
 *
 *  If all of the found entries fit into the supplied buffer, the count is
 *  returned and the open file context is closed.
 *
 *  If not all of the entries fit into the buffer, and there were no errors,
 *  a valid open file context is returned to allow a FindNext call by the
 *  client. FindClose must be called to clear this context, or FindNext
 *  must be called until it reaches the end of the directory.
 *
 *  The current index is maintained as part of the open file state.
 *
 *  The pathname may contain wild card specifiers, and these will be passed
 *  through to DOS to export DOS file name expansion.
 *
 *  If this routine returns any errors, the context is not valid.
 *
 *  This routine will continue searching until either the total buffer
 *  size is filled, the number of entries has been found, or the end of
 *  the directory has been reached by calling FindNext in order to reduce
 *  the number of average network requests. IE: most searchs can be done
 *  in one request reply, rather than two in this case.
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of the directory to
 *     enumerate. The last entry may be a file name with or without wild
 *     card specifiers
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   StartIndex (input)
 *     Starting index to look for entrys at
 *
 *   LongFileNames (input)
 *     If TRUE, the host we are communicating with can handle
 *     LongFileNames. We return them only if we have them on
 *     a per file basis.
 *
 *   pFindBuf (output)
 *     Pointer to buffer to place the find data.
 *
 *   FindBufSize (input)
 *     Size in bytes of find buffer. This limits CountRequested to what
 *     fits into this buffer.
 *
 *   pFindBufRead (output)
 *     Pointer to variable to return the number of bytes actually read
 *     into the find buf.
 *
 *   CountRequested (input)
 *     Maximum number of entries that the caller can handle. This is limited
 *     by what will fit into the buffer.
 *
 *   pCountRead (output)
 *     Total number of entries placed into the buffer, may be less than
 *     CountRequested due to FindBufSize being reached, OR end of directory.
 *
 *   pFileId (output)
 *     Pointer to variable to place the new FileId if more entries remain
 *     in the directory so that FindNext may be called.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFindFirstIndex(
    PCHAR       pName,
    USHORT      PathNameSize,
    USHORT      StartIndex,
    USHORT      LongFileNames,
    PCHAR       pBuf,
    USHORT      FindBufSize,
    PUSHORT     pFindBufRead,
    UCHAR       CountRequested,
    PCHAR       pCountRead,
    PUSHORT     pFileId,
    PUSHORT     pResult
    )
{
    int         ret;
    USHORT      Context, AmountCopied;
    USHORT      ErrCode, ErrClass;
    POPENCONTEXT pOC;
    PFIND       pFind;
    PCHAR       pPathNameBuf;
    USHORT      NewReadBytes, NewResult;
    UCHAR       NewReadCount;
    BOOLEAN     Result;

    TRACE(( TC_CDM, TT_API3, "CdmDosFindFirst: pName 0x%lx, PathNmSz %d, pBuf 0x%lx", pName, PathNameSize, pBuf));
    TRACE(( TC_CDM, TT_API3, "FindBufSize %d, pFindBufRead 0x%lx, *pFindBufRead %ld", FindBufSize, pFindBufRead, *pFindBufRead));
    TRACE(( TC_CDM, TT_API3, "CountReq %d, pCountRead 0x%lx, *pCountRead %d", CountRequested, pCountRead, *pCountRead));
    TRACE(( TC_CDM, TT_API3, "pFileId 0x%lx, pResult 0x%lx", pFileId, pResult));
    TRACE(( TC_CDM, TT_API3, "sizeof(FINDSTRUCT) is %d", sizeof_FINDSTRUCT));

    /*
     * Validate the users basic parameters
     */
    if( CountRequested == 0 ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    // Setup the initial return counts
    *pFindBufRead = 0L;
    *pCountRead = 0;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

#ifdef  WIN32
    /*
     *  If he sends us a ????????.???, we'll switch it to *.*
     */
    {
     PCHAR pTmp;
     pTmp = strchr( pPathNameBuf, '?' );
     if ( pTmp != NULL) {
         if ( !strcmp( pTmp, "????????.???" ) ) {
             strcpy( pTmp, "*.*" );
         }
     }
    }
#endif

    TRACE(( TC_CDM, TT_API3, "FindFirst on Name :%s:", pPathNameBuf));

    /*
     * Allocate a file context entry
     */
    Context = AllocateContext( OPENCONTEXT_FIND );

    if( Context == (USHORT)(-1) ) {
        // Out of context entries
        free( pPathNameBuf );
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Attempt to get a pointer to the FIND context entry
     * by validating the returned Context
     */
    pOC = ValidateContext( Context );
    if( pOC == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        free( pPathNameBuf );
        return;
    }

    /*
     * Try the DOS Find first on the directory
     *
     * NOTE: This returns all files of all attributes and lets the client
     *       sort out the files according to its display rules.
     *
     */
    ret = 0;
    pOC->FindHandle = 0;
    pOC->FindBufferFull = FALSE;

    // Get a find entry, and mark the buffer as having one
    FINDFIRST( pPathNameBuf, pOC->x.pFindBuf, pOC->FindHandle, ret );

    TRACE(( TC_CDM, TT_API3, "CDMAPI: FindFirst Handle=%x", pOC->FindHandle ));

    if ( ret || (pOC->FindHandle == (HANDLE)-1L) ) {
        // Error on FindFirst, no entries found, no Context to return
#ifdef  WIN32
        ret = GetLastError();
        TRACE(( TC_CDM, TT_API3, "FINDFIRST: GetLastError=%d", ret ));
        //
	// Win32 FindFirst can return no error, but a
	// FindHandle of (-1) when no files on a floppy
        //
	if( ret == 0 ) {
	    ErrClass = CDM_ERROR_NOTFOUND;
            ErrCode = CDM_DOSERROR_NOFILES;
	}
	else {
  	    CdmDosError( ret, &ErrClass, &ErrCode );
	}
#else
        CdmDosError( ret, &ErrClass, &ErrCode );
#endif
        TRACE(( TC_CDM, TT_API3, "CDMAPI: FindFirst Error ret: ErrClass %d, ErrCode %d", ErrClass, ErrCode ));
        FreeContext( Context );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        *pFileId = (USHORT)(-1);
        free( pPathNameBuf );
        return;
    } else {

        TRACE(( TC_CDM, TT_API3, "CDMAPI: Entries returned from FINDFIRST"));

	// If we are at the callers index, we can begin transfering entries
        if( pOC->DirIndex >= StartIndex ) {

	    AmountCopied = 0;

	    pFind = pOC->x.pFindBuf;

	    /*
	     * Marshall the data out into the callers buffer
	     */
	    Result = CdmMarshallFind(
	                 pFind,
			 LongFileNames,
			 pBuf,
			 FindBufSize,
			 &AmountCopied
			 );

            if( !Result ) {

                // Buffer overflow on first entry
                *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_BADLENGTH );
                *pFileId = (USHORT)(-1);
                free( pPathNameBuf );
                FreeContext( Context );
                return;
	    }

	    pBuf += AmountCopied;
            FindBufSize -= AmountCopied;
            (*pFindBufRead) += AmountCopied;
	    (*pCountRead)++;
            CountRequested--;
        }

	// Increment the current dir position
        pOC->DirIndex++;

        /*
         * See if there is still room for more
         */
        if( CountRequested == 0 ) {
            // The Find Context is Valid, the count and size have been updated
            *pFileId = Context;
            *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
            free( pPathNameBuf );
            return;
        }

        /*
         * Now call CdmDosFindNext to fill in more
         * entries. Remember that the Context gets closed if
         * we scan till the end of the directory.
         */
        CdmDosFindNext( Context,
                        StartIndex,
                        LongFileNames,
			pBuf,
                        FindBufSize,
                        &NewReadBytes, // Bytes
                        CountRequested,
                        &NewReadCount,
                        &NewResult
                      );

        if( (NewResult == CDM_ERROR_NONE) ||
            (CDM_DOSERROR_CODE(NewResult) == CDM_DOSERROR_BADLENGTH) ) {

	    // The Find Context is still Valid, update the count and size
	    *pFileId = Context;
            *pCountRead += NewReadCount;
            *pFindBufRead += NewReadBytes;
            *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
            free( pPathNameBuf );
            return;
        } else {

	    // A FindNext error occurred.
            //
            // The Find Context is no longer Valid, update the count and size
            // since a NO_MORE_FILES error still has a valid count and size,
            // and return the error status from the FindNext. If it was an
            // error other than NO_MORE_FILES, count and size is set to 0
            // properly.
            // The Context has been freed by the FindNext so we do not have to
            //
            *pFileId = (USHORT)(-1);
            *pCountRead += NewReadCount;
            *pFindBufRead += NewReadBytes;
            TRACE(( TC_CDM, TT_API3, "FindFirst: pCountRead=%d, pFindBufRead=%d", *pCountRead, *pFindBufRead ));
            *pResult = NewResult;
            free( pPathNameBuf );
            return;
        }
    }
}

/*****************************************************************************
 *
 *  CdmDosFindNext
 *
 *  Find the next entries in a directory represented by the Context
 *  opened by FindFirst.
 *
 *  If all of the found entries fit into the supplied buffer, the count is
 *  returned and the open file context is closed.
 *
 *  If not all of the entries fit into the buffer, and there were no errors,
 *  the open file context is NOT closed.
 *
 *  This routine will continue searching until either the total buffer
 *  size is filled, the number of entries has been found, or the end of
 *  the directory has been reached.
 *
 * ENTRY:
 *
 *   FileId (input)
 *     Open FindFirst Context to use.
 *
 *   StartIndex (input)
 *     Starting index to use for find
 *
 *   pFindBuf (output)
 *     Pointer to buffer to place the find data.
 *
 *   FindBufSize (input)
 *     Size in bytes of find buffer. This limits CountRequested to what
 *     fits into this buffer.
 *
 *   pFindBufRead (output)
 *     Pointer to variable to return the number of bytes actually read
 *     into the find buf.
 *
 *   CountRequested (input)
 *     Maximum number of entries that the caller can handle. This is limited
 *     by what will fit into the buffer.
 *
 *   pCountRead (output)
 *     Total number of entries placed into the buffer, may be less than
 *     CountRequested due to FindBufSize being reached, OR end of directory.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFindNext(
    USHORT      FileId,
    USHORT      StartIndex,
    USHORT      LongFileNames,
    PCHAR       pBuf,
    USHORT      FindBufSize,
    PUSHORT     pFindBufRead,
    UCHAR       CountRequested,
    PUCHAR      pCountRead,
    PUSHORT     pResult
    )
{
    int ret;
    USHORT ErrCode, ErrClass, AmountCopied, Tmp;
    POPENCONTEXT pOC;
    PFIND   pFind;
    BOOLEAN Result;

    /*
     * Its an error to ask for either no entries, or to supply a buffer
     * size that can not fit even one entry.
     */

    if( CountRequested == 0 ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    // Setup the initial return counts
    *pFindBufRead = 0L;
    *pCountRead = 0;

    /*
     * Attempt to get a pointer to the FIND context entry
     * by validating the returned Context
     */
    pOC = ValidateContext( FileId );
    if( pOC == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * To support long file names, we are no longer sure whether the
     * entry will fit into the buffer to pass back to the host. This
     * is because we do not know our name length until we get the
     * FIND entry. Once we get an entry, we can find that there is no
     * space in the buffer. None of the operating systems support a
     * "pushback" on the find operation. So we implement one here with
     * the flag FindBufferFull. This flag is cleared when we actually
     * copy the entry to the host or scan over it, and if its set, we
     * do not need do to a findnext. This way we do not lose entries.
     */

    while( 1 ) {

        // See if we have satisfied the caller maximum count
        if( *pCountRead == CountRequested ) {
            *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
            return;
        }

        /*
         * Do the DOS Find Next on the directory
         *
         * NOTE: The previous FindFirst search attributes apply to
         *       this search.
         */
        if( !pOC->FindBufferFull ) {
	    FINDNEXT( pOC->FindHandle, pOC->x.pFindBuf, ret );
	}
	else {
	    ret = 0;
	}

        if ( ret ) {
            // Error on FindNext, no entries found, close the Context to return
#ifdef  WIN32
            ret = GetLastError();
            TRACE(( TC_CDM, TT_API3, "FINDNEXT: GetLastError=%d", ret ));
#endif
            CdmDosError( ret, &ErrClass, &ErrCode );
            if( ErrCode == CDM_DOSERROR_NOTFOUND ) ErrCode = CDM_DOSERROR_NOFILES;
            TRACE(( TC_CDM, TT_API3, "CDMAPI: FindNext Error ret: ErrClass %d, ErrCode %d", ErrClass, ErrCode ));
            *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
            if( ErrCode == CDM_DOSERROR_NOFILES ) {
                // We autoclose the context when we return CDM_DOSERROR_NOFILES
		CdmDosFindClose( FileId, &Tmp );
	    }
	    return;
        } else {

            pOC->FindBufferFull = TRUE;

	    // See if we are at the callers requested index
            if( pOC->DirIndex >= StartIndex ) {

	        AmountCopied = 0;

		pFind = pOC->x.pFindBuf;

  	        /*
	         * Marshall the data out into the callers buffer
	         */
	        Result = CdmMarshallFind(
	                     pFind,
			     LongFileNames,
			     pBuf,
			     FindBufSize,
			     &AmountCopied
			     );

                if( !Result ) {

                    if( *pCountRead == 0 ) {
  		        // Buffer overflow on first entry
                        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_BADLENGTH );
                        return;
		    }
		    else {
   	                *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                        return;
		    }
		}

		pBuf += AmountCopied;
                FindBufSize -= AmountCopied;
		(*pFindBufRead) += AmountCopied;
                (*pCountRead)++;
            }

            // Update the current dir index
            pOC->DirIndex++;

	    // continue reading more entries
	    pOC->FindBufferFull = FALSE;
	    continue;
        }

    } // End while( 1 )
}

/*****************************************************************************
 *
 *  CdmMarshallFind
 *
 *   Handle marshalling of find data structs for CDM wire
 *
 * ENTRY:
 *   Param1 (input/output)
 *     Comments
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

BOOLEAN STATIC 
CdmMarshallFind(
    PFIND   pFind,
    USHORT  LongFileNames,
    PCHAR   pBuf,
    USHORT  FindBufSize,
    PUSHORT pFindBufRead
    )
{
    PCHAR       pTmp;
    PFINDSTRUCT pFindBuf;
    USHORT ShortNameSize = FIND_NAMESIZE_MIN;

    // WIN32 supports long file names
#ifdef WIN32
    USHORT LongNameSize = 0;
    PFINDSTRUCT_LONG pFindBufL;

    /*
     * If we have an alternate file name on WIN32, the main
     * file name is long.
     */
    if ( pFind->cAlternateFileName[0] != 0 ) {
        LongNameSize  = strlen( pFind->cFileName );
        if( LongNameSize > CDM_MAX_PATH_LEN ) LongNameSize = CDM_MAX_PATH_LEN;
    }

    // Only send Long File Names to a host that can handle it
    if( LongFileNames ) {

	// We must marshall into a FINDSTRUCT_LONG

	if( FindBufSize < (sizeof( FINDSTRUCT_LONG ) + ShortNameSize + LongNameSize) ) {
            return(FALSE);
        }

        pFindBufL = (PFINDSTRUCT_LONG)pBuf;

        pFindBufL->NameSize = (UCHAR)ShortNameSize;
	pFindBufL->LongNameSize = (UCHAR)LongNameSize;

        pFindBufL->Attributes = (USHORT)pFind->dwFileAttributes;
        pFindBufL->FileSize = (ULONG)pFind->nFileSizeLow;
        FileTimeToDosDateTime( &pFind->ftLastWriteTime,
                               &pFindBufL->WriteDate,
                               &pFindBufL->WriteTime);

        // Increment to the next entry
        pFindBufL++;
        *pFindBufRead += (USHORT)sizeof( FINDSTRUCT_LONG );

        // Copy the short name after the find struct, without the NULL byte
        pTmp = (PUCHAR)pFindBufL;

	if ( LongNameSize ) {

            // Copy the short name
	    memcpy( pTmp, pFind->cAlternateFileName, ShortNameSize );
            pTmp += ShortNameSize;
            *pFindBufRead += ShortNameSize;

            // Now copy the long name
            memcpy( pTmp, pFind->cFileName, LongNameSize );
            pTmp += LongNameSize;
            *pFindBufRead += LongNameSize;

	} else {

            // We just have a short name in the normal file name place
	    memcpy( pTmp, pFind->cFileName, ShortNameSize );
            pTmp += ShortNameSize;
            *pFindBufRead += ShortNameSize;
        }
    }
    else {
#endif // WIN32

	// We must marshall into a FINDSTRUCT

	if( FindBufSize < (sizeof_FINDSTRUCT + ShortNameSize ) ) {
            return(FALSE);
        }
        pFindBuf = (PFINDSTRUCT)pBuf;

        pFindBuf = (PFINDSTRUCT)pBuf;

        pFindBuf->NameSize = (UCHAR)ShortNameSize;

#ifdef  WIN32
        pFindBuf->Attributes = (USHORT)pFind->dwFileAttributes;
        pFindBuf->FileSize = (ULONG)pFind->nFileSizeLow;
        FileTimeToDosDateTime( &pFind->ftLastWriteTime,
                               &pFindBuf->WriteDate,
                               &pFindBuf->WriteTime);
#else
        pFindBuf->Attributes = (USHORT)pFind->attrib;
        pFindBuf->FileSize = (ULONG)pFind->size;
        pFindBuf->WriteTime = pFind->wr_time;
        pFindBuf->WriteDate = pFind->wr_date;
#endif

        // Increment to the next entry
        pFindBuf++;
        *pFindBufRead += (USHORT)sizeof_FINDSTRUCT;

        // Copy the short name after the find struct, without the NULL byte
        pTmp = (PUCHAR)pFindBuf;
#ifdef  WIN32

	if ( LongNameSize ) {

            // Copy the short name field, ignore the long file name
	    memcpy( pTmp, pFind->cAlternateFileName, ShortNameSize );
            pTmp += ShortNameSize;
            *pFindBufRead += ShortNameSize;

	} else {

            // We just have a short name in the normal file name place
	    memcpy( pTmp, pFind->cFileName, ShortNameSize );
            pTmp += ShortNameSize;
            *pFindBufRead += ShortNameSize;
        }
    }
#else
        // DOS and WIN16 clients only have the short name
	memcpy( pTmp, pFind->name, ShortNameSize );
        pTmp += ShortNameSize;
        *pFindBufRead += ShortNameSize;
#endif

    return( TRUE );
}

/*****************************************************************************
 *
 *  CdmDosFindClose
 *
 *  Handle a Find Close call.
 *
 * ENTRY:
 *
 *   FileId (output)
 *     The Find context to close
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFindClose(
    USHORT  FileId,
    PUSHORT pResult
    )
{
    POPENCONTEXT pOC;

    /*
     * Attempt to get a pointer to the FIND context entry
     * by validating the returned Context
     */
    pOC = ValidateContext( FileId );
    if( pOC == NULL ) {
        TRACE(( TC_CDM, TT_API4, "CdmDosFindClose: FileId 0x%x INVALID!", FileId));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Free the context, DOS does not maintain any internal
     * handles for Find contexts
     */

    /*
     *  Close the find handle (Win32 only)
     */
    FINDCLOSE( pOC->FindHandle );

    FreeContext( FileId );
    *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
    return;
}

/*****************************************************************************
 *
 *  CdmDosGetAttrEx
 *
 *   Handle a Get file attributes call
 *
 *   This is an extended get attributes that uses the FindFirst command
 *   to return a files size and its date/time along with its attributes.
 *
 *   This reduces traffic while querying files on a WAN.
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of file to get attributes on
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   pAttributes (output)
 *     Pointer to variable to place DOS 3.x file attributes from file name.
 *
 *   pFileSize (output)
 *     Pointer to variable to place file size.
 *
 *   pFileDate (output)
 *     Pointer to variable to place file date.
 *
 *   pFileTime (output)
 *     Pointer to variable to place file time.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosGetAttrEx(
    PCHAR   pName,
    USHORT  PathNameSize,
    PUSHORT pAttributes,
    PULONG  pFileSize,
    PUSHORT pFileDate,
    PUSHORT pFileTime,
    PUSHORT pResult
    )
{
    int     ret;
    FIND    FindBuf;
    USHORT  ErrCode, ErrClass;
    PCHAR   pPathNameBuf;
    HANDLE  FindHandle;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

    /*
     * Try the DOS Find first on the directory
     *
     * NOTE: This returns all files of all attributes and lets the client
     *       sort out the files according to its display rules.
     */
    ret = 0;
    FindHandle = 0;
    FINDFIRST( pPathNameBuf, &FindBuf, FindHandle, ret );

    if ( ret || (FindHandle == (HANDLE)-1L) ) {
        // Get the error from DOS
#ifdef  WIN32
        ret = GetLastError();
        TRACE(( TC_CDM, TT_API3, "CDMAPI: (CdmDosGetAttr) GetLastError=%d", ret ));
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );

        // Make a NO_MORE_FILES (18) into a NOT_FOUND (2) to be
        // compatible with getattr
        if( ErrCode == CDM_DOSERROR_NOFILES ) ErrCode = CDM_DOSERROR_NOTFOUND;

        /*
         * Find first always fails on the root directory, so if we get
         * a file not found error, see if its a name of "X:\" and fake
         * a valid stat if so using the current time, filesize of 0.
         *
         * Note that NETX returns PATHNOTFOUND
         */
        if( ( ErrCode == CDM_DOSERROR_NOTFOUND ) ||
            ( ErrCode == CDM_DOSERROR_PATHNOTFOUND ) ) {
            if( (PathNameSize == 3) &&
                (pPathNameBuf[1] == ':') &&
                (pPathNameBuf[2] == '\\' )) {

                *pAttributes = (USHORT)CDM_ATTR_DIRECTORY;
                *pFileSize = (ULONG)0L;

                // This routines converts current date/time to ftime format
                _dos_rootftime( (LPUINT)pFileDate,
                                (LPUINT)pFileTime
                              );
                TRACE(( TC_CDM, TT_API3, "CdmDosGetAttr: Success on ROOT! Name :%s:, NmSz %d, Attr 0x%x, size %ld", pPathNameBuf, PathNameSize, *pAttributes, *pFileSize));
                *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
                free( pPathNameBuf );
                return;
            }
        }

        TRACE(( TC_CDM, TT_API3, "CdmDosGetAttr: Error! Name :%s:, NmSz %d, ret %d", pPathNameBuf, PathNameSize, ErrCode));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pPathNameBuf );
        return;
    } else {
        // Successful FindFirst, return the files attributes
#ifdef  WIN32
        *pAttributes = (USHORT)FindBuf.dwFileAttributes;
        *pFileSize = (ULONG)FindBuf.nFileSizeLow;
        FileTimeToDosDateTime( &FindBuf.ftLastWriteTime,
                               pFileDate,
                               pFileTime );
#else
        *pAttributes = (USHORT)FindBuf.attrib;
        *pFileSize = (ULONG)FindBuf.size;
        *pFileDate = FindBuf.wr_date;
        *pFileTime = FindBuf.wr_time;
#endif
        TRACE(( TC_CDM, TT_API3, "CdmDosGetAttr: Success! Name :%s:, NmSz %d, Attr 0x%x, size %ld, WT 0x%x, WD 0x%x",
              pPathNameBuf, PathNameSize, *pAttributes, *pFileSize,
              *pFileTime, *pFileDate));
        /*
         *  Close the find handle (Win32 only)
         */
        FINDCLOSE( FindHandle );

#ifdef PERF_PROFILE
        if( stricmp( "C:\\NULL", pPathNameBuf ) == 0 ) {
            *pFileSize = (ULONG)(1024L*1024L*10L);
	}
#endif

	/*
         *  If a set attribute occurred while we had the file open then
         *  we store the information.  We return this info on this call.
         *
	 *  Also if the file has a handle open for writing, the file size
	 *  will be incorrect since FindFirst() returns 0 for file size
	 *  on files that have exclusive write opens.
	 */
        ContextGetAttributes( pName, PathNameSize, pAttributes, pFileSize );

	*pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        free( pPathNameBuf );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosSetAttr
 *
 *   Handle a Set file attributes call
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of file to set attributes on
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   Attributes (input)
 *     Pointer to variable to place DOS 3.x file attributes from file name.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosSetAttr(
    PCHAR   pName,
    USHORT  PathNameSize,
    USHORT  Attributes,
    PUSHORT pResult
    )
{
    int ret;
    unsigned int DosAttributes;
    USHORT ErrCode, ErrClass;
    PCHAR pPathNameBuf;

#ifndef  WIN32
    /*
     *  If the file is open then we'll change it in our context table and
     *  then we'll change it on the close file.
     */
    if ( ContextSetAttributes( pName, PathNameSize, Attributes ) ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        TRACE(( TC_CDM, TT_API4, "CdmDosSetAttr: Success! Changed local!!"));
        return;
    }
#endif

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }

    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;


    /*
     * Try to Set the files attributes.
     */
    DosAttributes = (unsigned int)Attributes;

    TRACE(( TC_CDM, TT_API3, "CdmDosSetAttr: Name :%s:, attr 0x%x", pPathNameBuf, DosAttributes));

    ret = _dos_setfileattr( pPathNameBuf,
                            DosAttributes
                          );

    if( ret == 0 ) {
        // Successful setattr
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        TRACE(( TC_CDM, TT_API4, "CdmDosSetAttr: Success! Name :%s", pPathNameBuf));
        free( pPathNameBuf );
        return;
    } else {
        // Error doing the setattr
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_API4, "CdmDosSetAttr: Failed! Name :%s, rc=%d", pPathNameBuf, ErrCode));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pPathNameBuf );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosGetDateTime
 *
 *  Handle a get date and time call
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to get the date and time on
 *
 *   pFileDate (output)
 *     Pointer to variable to place the files date
 *
 *   pFileTime (output)
 *     Pointer to variable to place the files time
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosGetDateTime(
    USHORT  FileId,
    PUSHORT pFileDate,
    PUSHORT pFileTime,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT ErrClass, ErrCode;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Now do the DOS Get ftime
     */
    ret = _dos_getftime( p->x.pFileEnt->DosHandle,
                         (UINT *)pFileDate,
                         (UINT *)pFileTime
                       );

    if( ret == 0 ) {
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    } else {
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosSetDateTime
 *
 *  Handle a Set date and time call
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to set the date and time on
 *
 *   FileDate (input)
 *     The new file date to set
 *
 *   FileTime (input)
 *     The new file time to set
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosSetDateTime(
    USHORT  FileId,
    USHORT  FileDate,
    USHORT  FileTime,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT ErrClass, ErrCode;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        TRACE(( TC_CDM, TT_ERROR, "CdmSDT: DosSetDateTime Failed, Invalid Context, Index= %d", FileId));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    TRACE(( TC_CDM, TT_API4, "CdmSDT: Attempt SetDate Time, Index=%d, Handle=%d", FileId, p->x.pFileEnt->DosHandle));
    TRACE(( TC_CDM, TT_API4, "CdmSDT: Date=0x%x, Time=0x%x", FileDate, FileTime));

    /*
     * Now do the DOS Set ftime
     */
    ret = _dos_setftime( p->x.pFileEnt->DosHandle,
                         (UINT)FileDate,
                         (UINT)FileTime
                       );

    if( ret == 0 ) {
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        TRACE(( TC_CDM, TT_API4, "CdmSDT: Successfull SetDateTime"));
        return;
    } else {
        CdmDosError( ret, &ErrClass, &ErrCode );
        // BUGBUG - Temporary
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        //*pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        //BUGBUG
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosDelete
 *
 *   Handle a file delete call
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of file to delete
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosDelete(
    PCHAR   pName,
    USHORT  PathNameSize,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrCode, ErrClass;
    PCHAR pPathNameBuf;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

    TRACE(( TC_CDM, TT_API3, "CdmDosDelete: file name :%s:", pPathNameBuf));

    /*
     * Try to delete the file name
     */

    ret = DELETEFILE( pPathNameBuf );

    if( ret == 0 ) {
        // Successful delete
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        free( pPathNameBuf );
        return;
    } else {
        // Error doing the delete
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pPathNameBuf );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosRename
 *
 *   Handle a file rename call
 *
 * ENTRY:
 *   pOldName (input)
 *     Pointer to NON NULL terminated pathname of file to rename
 *
 *   OldPathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   pNewName (input)
 *     Pointer to NON NULL terminated pathname of new name for the file
 *
 *   NewPathNameSize (input)
 *     Size of the new pathname for the file
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosRename(
    PCHAR   pOldName,
    USHORT  OldPathNameSize,
    PCHAR   pNewName,
    USHORT  NewPathNameSize,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrCode, ErrClass;
    PCHAR pOldPathNameBuf;
    PCHAR pNewPathNameBuf;

    /*
     * Copy the path name strings into our buffers so we can NULL
     * terminate them.
     */
    if( OldPathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    if( NewPathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }

    pOldPathNameBuf = (PCHAR)malloc( OldPathNameSize+1 );
    if( pOldPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }
    pNewPathNameBuf = (PCHAR)malloc( NewPathNameSize+1 );
    if( pNewPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        free( pOldPathNameBuf );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pOldPathNameBuf, pOldName, OldPathNameSize );
    pOldPathNameBuf[OldPathNameSize] = 0;

    strncpy( pNewPathNameBuf, pNewName, NewPathNameSize );
    pNewPathNameBuf[NewPathNameSize] = 0;

    TRACE(( TC_CDM, TT_API3, "CdmDosRename: from :%s: to :%s:", pOldPathNameBuf, pNewPathNameBuf));

    /*
     * Try to rename the file
     */

    ret = RENAMEFILE( pOldPathNameBuf, pNewPathNameBuf );

    if( ret == 0 ) {
        // Successful rename
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        free( pOldPathNameBuf );
        free( pNewPathNameBuf );
        return;
    } else {
        // Error doing the rename
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pOldPathNameBuf );
        free( pNewPathNameBuf );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosCreateDir
 *
 *   Handle a Create Directory call
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of the directory to create
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosCreateDir(
    PCHAR   pName,
    USHORT  PathNameSize,
    USHORT  AccessMode,
    USHORT  Attributes,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrCode, ErrClass;
    PCHAR pPathNameBuf;
    unsigned int DosAttributes;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

    /*
     *  NOTE: If an attempt is made to do mkdir of a file name that already
     *        exists as either a normal data file or a directory, an error
     *        class of 0 with an extended error code of 183 is returned by
     *        the NTVDM Dos box on NT (July 1993 build). This is not
     *        documented, so I fix this by doing a GetAttr first, and if the
     *        target file exists I fail properly.
     */

    /*
     * Try to get the files attributes.
     */
    ret = _dos_getfileattr( pPathNameBuf,
                            &DosAttributes
                          );

    if( ret == 0 ) {
        // Successful getattr means a file exists!
        TRACE(( TC_CDM, TT_API3, "DosCreateDir File exists error"));
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_EXISTS, CDM_DOSERROR_EXISTS );
        free( pPathNameBuf );
        return;
    }

    /*
     * Try to create the directory
     */

    TRACE(( TC_CDM, TT_API3, "DosCreateDir: Attempting to make directory :%s:", pPathNameBuf));

    ret = _mkdir( pPathNameBuf );

    if( ret == 0 ) {
        // Successful create dir
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        TRACE(( TC_CDM, TT_API3, "DosCreateDir: Success, *pResult 0x%lx", *pResult));
        free( pPathNameBuf );
        return;
    } else {
        // Error doing the create dir
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_API3, "DosCreateDir Error: ErrClass 0x%x, ErrCode 0x%x",
                 ErrClass, ErrCode));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pPathNameBuf );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosDeleteDir
 *
 *   Handle a Delete Directory call
 *
 * ENTRY:
 *   pName (input)
 *     Pointer to NON NULL terminated pathname of the directory to delete
 *
 *   PathNameSize (input)
 *     Length of the non null terminated pathname
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosDeleteDir(
    PCHAR   pName,
    USHORT  PathNameSize,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrCode, ErrClass;
    PCHAR pPathNameBuf;

    /*
     * Copy the path name string into our buffer so we can NULL
     * terminate it.
     */
    if( PathNameSize > MAX_PATH_NAME ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_BADFORMAT, CDM_DOSERROR_BADLENGTH );
        return;
    }
    pPathNameBuf = (PCHAR)malloc( PathNameSize+1 );

    if( pPathNameBuf == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_RESOURCE, CDM_DOSERROR_NOMEM );
        return;
    }

    // Copy the pName string into the local buffer and NULL terminate it
    strncpy( pPathNameBuf, pName, PathNameSize );
    pPathNameBuf[PathNameSize] = 0;

    TRACE(( TC_CDM, TT_API3, "CdmDosDeleteDir: Dir name :%s:", pPathNameBuf));

    /*
     * Try to delete the directory
     */

    ret = _rmdir( pPathNameBuf );

    if( ret == 0 ) {
        // Successful delete dir
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        free( pPathNameBuf );
        return;
    } else {
        // Error doing the delete dir
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        free( pPathNameBuf );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosReadCond
 *
 *  Handle a file Read conditional call.
 *
 *  If the supplied Date and Time do not match the ones on the file,
 *  the read is done. Otherwise status CDM_ERROR_NONE,
 *  CDM_DOSERROR_NOERROR is returned with pTotalRead == 0 to signify a
 *  match up of the date and time.
 *
 *  Clients use this call to implement time based cache consistency, and
 *  already have the requested data in their cache. This call is to test
 *  if that cache is still valid. The date and time supplied were previously
 *  returned to the client from the CdmDosRead call.
 *
 *  This will read up to Size data into the buffer. Short reads other than
 *  0 are not an indication of end of file, but could be due to buffer space
 *  available.
 *
 *  A read of 0 with the status return of CDM_ERROR_EOF is an actual EOF
 *  indication.
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to read
 *
 *   pBuf (output)
 *     Buffer to read the requested data into
 *
 *   Size (input)
 *     Amount of data to read
 *
 *   Offset (input)
 *     Offset in file to start reading from
 *
 *   pTimeStamp (input/output)
 *     Current DOS time stamp on the file as defined cdmwire.h
 *
 *   pDateStamp (output/output)
 *     Current DOS date stamp on the file as defined in cdmwire.h
 *
 *   pTotalRead (output)
 *     Pointer to variable to place the amount read from the file.
 *
 *   pValid (output)
 *     Set to TRUE if the data was still valid and no actual
 *     data was transfered. Success is returned in this case.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosReadCond(
    USHORT  FileId,
    PCHAR   pBuf,
    USHORT  Size,
    ULONG   Offset,
    PUSHORT pTimeStamp,
    PUSHORT pDateStamp,
    PUSHORT pTotalRead,
    PUSHORT pValid,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT CurDate, CurTime;
    USHORT DosSize, DosTotalRead, ErrClass, ErrCode;
    LONG    Temp;

    DosSize = (USHORT)Size;
    DosTotalRead = 0;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

#ifdef PERF_PROFILE
    /*
     * Return any garbage fast on this special file
     * for profiling clientdrive/ICA performance on
     * a given CLIENT/HOST configuration.
     */
    if( p->IsNullFile ) {
        *pTotalRead = DosSize;
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        *pValid = FALSE;
        return;
    }
#endif

    /*
     * Get the files current date and time and compare it
     */
    ret = _dos_getftime( p->x.pFileEnt->DosHandle,
                         (UINT *)&CurDate,
                         (UINT *)&CurTime
                       );

    if( ret != 0 ) {
        // Error on file handle
        *pTotalRead = 0L;
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }

    if( (CurDate == *pDateStamp) && (CurTime == *pTimeStamp) ) {
        // The clients time is up to date with the files
        *pTotalRead = 0L;
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        *pValid = TRUE;
        return;
    }

    /*
     * Here we have to actually return the new(er) data
     * to the host.
     */
    *pValid = FALSE;

    /*
     * Seek to the offset we are going to read from
     */
    Temp = SEEK( p->x.pFileEnt->DosHandle, Offset, 0 );

    if( Temp == -1L ) {
        // Problems with handle
        *pTotalRead = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }

    /*
     * Now do the DOS read, since the clients time is out of date
     */
    ret = 0;  // Is already set
    FSREAD( p->x.pFileEnt->DosHandle, pBuf, DosSize, DosTotalRead, ret );

    if( ret || (DosTotalRead == -1) ) {
        *pTotalRead = 0L;
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    } else {
        // No errors
        *pTotalRead = (USHORT)DosTotalRead;

        /*
         * We now return the new DOS file time and
         * date stamps to the client.
         */
        ret = _dos_getftime( p->x.pFileEnt->DosHandle,
                             (UINT *)pDateStamp,
                             (UINT *)pTimeStamp
                           );
        // If read handle was valid, getftime handle is valid
        ASSERT( ret == 0, 0 );

        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosFileLock
 *
 *  Handle a DOS file lock request
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to lock
 *
 *   LockStart (input)
 *     The starting offset in bytes to set the DOS file lock
 *
 *   LockSize (input)
 *     The size in bytes for the file lock.
 *
 *   Type (input)
 *     The type of lock to set.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFileLock(
    USHORT  FileId,
    ULONG   LockStart,
    ULONG   LockSize,
    UCHAR   Type,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT ErrClass, ErrCode;

    /*
     * We can only handle DOS type locks (single reader/writer)
     */
    if( Type != LOCKTYPE_DOS ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_SHARE );
        return;
    }

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Now do the DOS file lock
     */

    ret = _dos_lock( p->x.pFileEnt->DosHandle,
                     0,                   // lock operation
                     LockStart,
                     LockSize
                   );

    if( ret == 0 ) {
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    } else {
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosFileUnLock
 *
 *  Handle a DOS file unlock request
 *
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to unlock
 *
 *   LockStart (input)
 *     The starting offset in bytes to set the DOS file lock
 *
 *   LockSize (input)
 *     The size in bytes for the file lock.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFileUnLock(
    USHORT  FileId,
    ULONG   LockStart,
    ULONG   LockSize,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT ErrClass, ErrCode;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Now do the DOS file unlock
     */

    ret = _dos_lock( p->x.pFileEnt->DosHandle,
                     1,                   // unlock operation
                     LockStart,
                     LockSize
                   );

    if( ret == 0 ) {
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    } else {
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }
}


/*****************************************************************************
 *
 *  CdmDosChangeSize
 *
 *  Handle a change file size request
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to set the new size on
 *
 *   NewSize (input)
 *     The new size for the file.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosFileChangeSize( USHORT  FileId, ULONG   NewSize, PUSHORT pResult )
{
    int ret;
    POPENCONTEXT p;
    ULONG FileSize;
    CHAR Buf;
    USHORT DosAmountWrote, ErrClass, ErrCode;

    DosAmountWrote = 0;

    TRACE(( TC_CDM, TT_API3, "CdmDosFileChangeSize: NewSize %ld", NewSize));

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Seek to the offset we are going to start the write from
     * and get the files current size
     */
    FileSize = SEEK( p->x.pFileEnt->DosHandle, NewSize, 0 );

    if( FileSize == -1L ) {
        // Problems with handle
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_API3, "CdmDosFileChangeSize: Error doing seek %d", ret));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }

    /*
     * See if the requested size is less than the current size
     *
     * The Dos Write (int 0x21, 40H is documented to shrink the file as well
     *                as grow)
     */
#ifdef DEBUG
    if ( NewSize < FileSize ) {
        TRACE(( TC_CDM, TT_API3, "CdmDosFileChangeSize: Newsize (%ld) < CurrSize (%ld)", NewSize, FileSize));
    }
#endif

    /*
     * Now do the DOS write of 0 bytes to attempt to
     * grow or shrink the file
     */
    ret = 0;
    FSWRITE( p->x.pFileEnt->DosHandle, &Buf, 0, DosAmountWrote, ret );

    if ( ret || (DosAmountWrote == -1) ) {
#ifdef  WIN32
        ret = GetLastError();
#endif
        CdmDosError( ret, &ErrClass, &ErrCode );
        TRACE(( TC_CDM, TT_API3, "CdmDosFileChangeSize: Error doing write %d", ret));
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    } else {
        TRACE(( TC_CDM, TT_API3, "CdmDosFileChangeSize: Success, FileSize=0x%x", FileSize));
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
}

/*****************************************************************************
 *
 *  CdmDosSeek
 *
 *  Handle a DOS file seek request.
 *
 * ENTRY:
 *
 *   FileId (input)
 *     The file context to get the size from
 *
 *   NewOffset (input)
 *     The new file offset to be set.
 *
 *   Whence (input)
 *     The whence (relative) value to calculate final offset
 *
 *   pFileSize (output)
 *     Variable to place the file size returned from DOS seek.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void STATIC 
CdmDosSeek(
    USHORT  FileId,
    ULONG   NewOffset,
    UCHAR   Whence,
    PULONG  pFileSize,
    PUSHORT pResult
    )
{
    int ret;
    POPENCONTEXT p;
    USHORT ErrClass, ErrCode;

    /*
     * Validate the open file context and attempt to get a pointer to it
     */
    p = ValidateContext( FileId );
    if( p == NULL ) {
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_INVALID, CDM_DOSERROR_UNKNOWN );
        return;
    }

    /*
     * Now do the DOS file seek
     */
    TRACE(( TC_CDM, TT_API3, "CdmDosSeek: NewOffset=0x%lx, Whence=%d", NewOffset, Whence));
    *pFileSize = SEEK( p->x.pFileEnt->DosHandle, NewOffset, Whence );

    if( *pFileSize != -1L ) {
#ifdef PERF_PROFILE
        if( p->IsNullFile ) {
            *pFileSize = (ULONG)(1024L*1024L*10L);
	}
#endif
	TRACE(( TC_CDM, TT_API3, "CdmDosSeek: Returning success, file size %ld", *pFileSize));
        // No errors
        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    } else {
#ifdef  WIN32
        ret = GetLastError();
#endif
        TRACE(( TC_CDM, TT_API3, "CdmDosSeek: Returning error %d", ret));
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        return;
    }
}
