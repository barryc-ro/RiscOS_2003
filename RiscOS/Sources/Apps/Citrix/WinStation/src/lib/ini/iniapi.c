
/*************************************************************************
*
*   INIAPI.C
*
*   DOS INI Library API routines.
*
*   Copyright Citrix Systems Inc. 1994
*
*   Author: Kurt Perry (4/08/1994)
*
*   $Log$
*  
*     Rev 1.35   15 Apr 1997 18:52:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.34   16 Jan 1997 15:18:28   kurtp
*  Make GetPrivateProfileString handle > 32K (int ot UINT)
*  
*     Rev 1.33   08 Nov 1995 15:55:06   butchd
*  update
*  
*     Rev 1.32   08 Nov 1995 10:20:48   KenB
*  don't use fungets(), WIN doesn't have it; use BOOL to determine whether to read or use last string
*  
*     Rev 1.31   19 Jul 1995 12:17:36   kurtp
*  update
*  
*     Rev 1.30   19 Jul 1995 12:15:26   kurtp
*  update
*  
*     Rev 1.29   02 May 1995 14:06:46   butchd
*  update
*
*************************************************************************/

#include "windows.h"
#include "fcntl.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  Get CLIB includes */
#include "../../inc/client.h"
#include "../../inc/logapi.h"
#include "../../inc/clib.h"
#include "../../inc/iniapi.h"

/*=============================================================================
==   Structures
=============================================================================*/

#define START_BUFFER_SIZE  1024     //  default buffer size

typedef struct _INICACHE {
    PCHAR pFilename;
    PCHAR pSection;
    PCHAR pBuffer;
    UINT  cbBufMax;
    UINT  cbBufCur;
    int   offBegin;
    int   offEnd;
    struct _INICACHE * pNext;
} INICACHE, * PINICACHE;

typedef struct _FILECACHE {
    PCHAR pFilename;
    FILE *fp;
    struct _FILECACHE * pNext;
} FILECACHE, * PFILECACHE;

typedef struct _EXTRAINFO {
    PUSHORT poffBegin;
    PUSHORT poffEnd;
    int     entry;
    int     cbKey;
} EXTRAINFO, * PEXTRAINFO;

/*=============================================================================
==   External Data
=============================================================================*/

/*=============================================================================
==   Local Data
=============================================================================*/

static PINICACHE  pSectionRoot = NULL;
static PFILECACHE pFileRoot = NULL;

/*=============================================================================
==   Local Functions Used
=============================================================================*/

#include "helpers.c"    // common helper routines used in both 'internal'
                        // INI apis and 'external' INI apis.

FILE * inifopen( PCHAR );
void   iniflush( void );
int
static wGetPrivateProfileString( PCHAR lpszSection,
                                 PCHAR lpszEntry,
                                 PCHAR lpszDefault,
                                 PCHAR lpszReturnBuffer,
                                 UINT  cbReturnBuffer,
                                 PCHAR lpszFilename,
                                 PEXTRAINFO pExtraInfo );

static char *myfgets(char *buf, int n, FILE *f);

/*******************************************************************************
 *
 *  dosGetPrivateProfileString
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int  WFCAPI
dosGetPrivateProfileString( PCHAR lpszSection,
                            PCHAR lpszEntry,
                            PCHAR lpszDefault,
                            PCHAR lpszReturnBuffer,
                            int   cbReturnBuffer,
                            PCHAR lpszFilename )
{
    UINT rc, i;
    
    rc = wGetPrivateProfileString( lpszSection,
                                   lpszEntry,
                                   lpszDefault,
                                   lpszReturnBuffer,
                                   (UINT) cbReturnBuffer,
                                   lpszFilename,
                                   NULL );

#if 0
    /*
     * SJM: Scan from start since there can be no newlines in the middle of an entry.
     * This also then always returns the correct length
     */
    for (i = 0; i < rc; i++)
	switch (lpszReturnBuffer[i])
	{
	case '\r':
	case '\n':
	    lpszReturnBuffer[i] = '\0';
	    rc = i;
	    break;
	case '\0':
	    rc = i;
	    break;
	}
#endif
    
    TRACE((TC_LIB, TT_API1, "dosGetPrivateProfileString: rc=%d s='%s'\n", rc, lpszReturnBuffer));

    return((int)rc);
}

/*******************************************************************************
 *
 *  wGetPrivateProfileString (local function)
 *
 *  Worker routine for dosGetPrivateProfileString
 *
 *  ENTRY:
 *
 *  EXIT:
 *      (int) number of characters written to lpszReturnBuffer (not including
 *          the terminating NULL).
 *
 ******************************************************************************/

static int
wGetPrivateProfileString( PCHAR lpszSection,
                          PCHAR lpszEntry,
                          PCHAR lpszDefault,
                          PCHAR lpszReturnBuffer,
                          UINT  cbReturnBuffer,
                          PCHAR lpszFilename,
                          PEXTRAINFO pExtraInfo )
{
    CHAR  ReadBuf[ MAX_INI_LINE ];
    UINT  cb;
    PCHAR pCacheBuf  = NULL;
    PCHAR p          = NULL;
    PINICACHE pTemp  = NULL;
    int   fFirstPass = TRUE;
    int   fInSection = FALSE;
    FILE *infile     = NULL;
    UINT  cbReturn   = strlen(lpszDefault);
    UINT  cbRemain   = cbReturnBuffer;
    UINT  cbRead;
    int   entry;
    UINT  cbKey;
    int   fDoScan    = TRUE;
    int   fDontRead  = FALSE;

    //  init return value
    ASSERT( lpszReturnBuffer != NULL, 0 );
    strncpy( lpszReturnBuffer, lpszDefault, cbReturnBuffer );
    lpszReturnBuffer[cbReturnBuffer-1] = 0;

    /*
     * If no section name was specified, just return the default
     */
    if ( lpszSection && !*lpszSection ) {
       goto done;
    }

    /*
     *  if filename is empty then section points to an array of entries
     *  already.  we just need to call search routine to locate entry.
     */
    if ( lpszFilename == NULL ) {

        //  validate parms
        if ( lpszEntry == NULL )
            goto invalid_parm;

        //  set cache buffer to section passed in
        pCacheBuf = lpszSection;

        //  go search buffer
        goto search_buffer;
    }

    //  search the cache first on Section/Entry search
    for ( pTemp=pSectionRoot; pTemp!=NULL; pTemp=pTemp->pNext ) {

        //  check to see if Filename:Section has been cached
        if ( !stricmp( pTemp->pFilename, lpszFilename ) &&
             !stricmp( pTemp->pSection, lpszSection ) ) {

            //  set cache buffer to cache entry buffer
            pCacheBuf = pTemp->pBuffer;

            //  go search buffer
            goto search_buffer;
        }
    }

    /*
     *  Section is not cached, let's cache it now/
     */
    if ( (infile = inifopen( lpszFilename )) == NULL )
        goto file_not_found;

scanagain:
    //  read each line until section header
    while ( fDontRead ? TRUE :
                        (myfgets( ReadBuf, MAX_INI_LINE, infile ) != NULL) ) {

        fDontRead = FALSE;
        cbRead = strlen( ReadBuf ) + 1;

        //  comment ?
        if ( ReadBuf[0] == ';' )
            continue;

        //  section header?
        if ( (ReadBuf[0] == '[') && ((p = strchr( ReadBuf, ']' )) != NULL) ) {

            // replace ] with null
            *p = 0;

//          TRACE((TC_LIB, TT_ERROR, "wGetPrivateProfileString: Section [%s]", (&ReadBuf[1])));

            //  the section header?
            if ( !stricmp( (&ReadBuf[1]), lpszSection ) ) {
                fInSection = TRUE;
                break;
            }
        }
        else {
//          TRACE((TC_LIB, TT_ERROR, "wGetPrivateProfileString: Line {%s}", ReadBuf));
        }
    }

    //  section not found?
    if ( !fInSection ) {
//      ASSERT( FALSE, 0 );  /* file is not optimally ordered */
        TRACE(( TC_LIB, TT_ERROR, "looking for section |%s|", lpszSection ));
        if ( fFirstPass ) {
            /*
             *  The first pass didn't necessarily start at the beginning of
             *  the file.  So we will seek to the beginning and try again
             */
            fseek( infile, 0L, SEEK_SET );
            fFirstPass = FALSE;
            goto scanagain;
        }
        TRACE((TC_LIB, TT_ERROR, "wGetPrivateProfileString: Section [%s] not found", lpszSection));
        goto sectionnotfound;
    }

    //  create cache entry and link at head
    if ( (pTemp = (PINICACHE) malloc( sizeof(INICACHE) )) != NULL ) {
        pTemp->pFilename = strdup(lpszFilename);
        pTemp->pSection  = strdup(lpszSection);
        pTemp->pBuffer   = (char *) malloc(START_BUFFER_SIZE + 1);
        ASSERT( pTemp->pBuffer != NULL, 0 );
        pTemp->cbBufMax  = START_BUFFER_SIZE;
        pTemp->cbBufCur  = 0;
        pTemp->pNext     = pSectionRoot;
        pTemp->offBegin  = max( 0, (UINT)ftell( infile ) - cbRead);
        pSectionRoot     = pTemp;
    }
    else {
        cbReturn =  0;
        goto outofmemory;
    }

    //  read each line until next section header
    while ( fDontRead ? TRUE :
                        (myfgets( ReadBuf, MAX_INI_LINE, infile ) != NULL) ) {

        fDontRead = FALSE;

        //  comment ?
        if ( ReadBuf[0] == ';' )
            continue;

        //  section header?
        if ( ReadBuf[0] == '[' ) {
            pTemp->offEnd  = (UINT)ftell( infile ) -
                             strlen(ReadBuf) - 1;
            /*
             * undo last fgets
             *
             * NOTE: this used to be done with fungets(), but that function is
             *       only in the CLIB which we didn't port to Windows.  Rather
             *       than porting the whole thing, we'll just use a boolean to
             *       tell ourselves whether to actually read the next line, or
             *       just fall through and use the line we read last time.
             *                                                     KLB 11-08-95
             */
            fDontRead = TRUE;
            break;
        }

        //  find = sign, signal of a valid entry syntax
        if ( (strchr( ReadBuf, '=' )) != NULL ) {

            //  get current strlen with NULL terminator
            cb = strlen( ReadBuf ) + 1;

            //  space left in buffer? no, then grow
            if ( (pTemp->cbBufCur + cb) >= pTemp->cbBufMax ) {
                pTemp->cbBufMax += START_BUFFER_SIZE;
                pTemp->pBuffer = (char *) realloc( pTemp->pBuffer,
                                                   pTemp->cbBufMax );
                if ( pTemp->pBuffer == NULL ) {
                    cbReturn =  0;
                    goto outofmemory;
                }
            }

            //  copy into buffer
            strncpy( &(pTemp->pBuffer[pTemp->cbBufCur]), ReadBuf, cb );

            //  index to next avail location in buffer
            pTemp->cbBufCur += cb;

            //  double NULL terminate
            pTemp->pBuffer[pTemp->cbBufCur] = 0;
        }
    }

    // If we reached the file end, calculate the end
    if ( ReadBuf[0] != '[' ) {
        pTemp->offEnd  = (UINT)ftell( infile );
    }

    //  realloc to true size
    pTemp->cbBufMax = (++pTemp->cbBufCur);
    pTemp->pBuffer = (char *) realloc( pTemp->pBuffer, pTemp->cbBufMax );
    if ( pTemp->pBuffer == NULL ) {
        cbReturn = 0;
        goto outofmemory;
    }

    //  point to search buffer
    pCacheBuf = pTemp->pBuffer;

search_buffer:

    /*
     *  if entry is NULL then return all strings in this section
     */
    if ( lpszEntry == NULL ) {
        cbReturn = pTemp->cbBufMax;
        if ( cbReturn >= cbReturnBuffer ) {
            cbReturn = cbReturnBuffer-2;
            memcpy( lpszReturnBuffer, pTemp->pBuffer, cbReturnBuffer );
            lpszReturnBuffer[cbReturn-2] = 0;
            lpszReturnBuffer[cbReturn-1] = 0;
        }
        else {
            memcpy( lpszReturnBuffer, pTemp->pBuffer, cbReturn );
        }
    }

    /*
     *  else search for string in buffer
     */
    else {
       if ( pExtraInfo ) {
          if ( pExtraInfo->poffBegin ) {
            fDoScan = FALSE; // no need to read the buffer in this case
          } else {
            entry = pExtraInfo->entry;
            cbKey = pExtraInfo->cbKey;
          }
       } else {
          entry    = -1;
       }

       if ( fDoScan ) {
          //  use worker routine to search buffer for entry
          if ( (cb = _w_gpps( pCacheBuf, lpszEntry, cbKey,
                              lpszReturnBuffer, cbReturnBuffer, entry )) )
            cbReturn = cb;
       }
    }

    // Special file-offset request.
    if ( pExtraInfo && pExtraInfo->poffBegin ) {

        *pExtraInfo->poffBegin = pTemp->offBegin;
        *pExtraInfo->poffEnd = pTemp->offEnd;
        cbReturn = 4;  // indicate that we found stuff
    }

//  TRACE((TC_LIB, TT_API1,
//         "GetProfileString: sec begin(%d) end(%d) size(%d)",
//         pTemp->offBegin, pTemp->offEnd, pTemp->offEnd - pTemp->offBegin ));


sectionnotfound:
file_not_found:
invalid_parm:
outofmemory:

//    TRACE((TC_LIB, TT_API1,
//           "GetProfileString: %s, [%s], %s -> '%s', %u, doscan %d",
//           lpszFilename, lpszSection ? lpszSection : "<sec>",
//           lpszEntry ? lpszEntry : "<entry>", lpszReturnBuffer, cbReturn, fDoScan));

done:
    return( cbReturn );
}


/*******************************************************************************
 *
 *  dosGetPrivateProfileInt
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
dosGetPrivateProfileInt( PCHAR lpszSection,
                         PCHAR lpszEntry,
                         int   iDefault,
                         PCHAR lpszFilename )
{
    char pDefault[32];
    char pBuffer[32];

    //  make default value a string
    sprintf( pDefault, "%u", iDefault );

    //  use string routine to get ...
    dosGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                                pBuffer, 32, lpszFilename );

//  TRACE((TC_LIB, TT_API1, "dosGetPrivateProfileInt: %s", pBuffer));
    return( (int) _htol( pBuffer ) );
}

/*******************************************************************************
 *
 *  GetPrivateProfileLong
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

long WFCAPI
GetPrivateProfileLong( PCHAR lpszSection,
                       PCHAR lpszEntry,
                       long  lDefault,
                       PCHAR lpszFilename )
{
    char pDefault[32];
    char pBuffer[32];

    //  make default value a string
    sprintf( pDefault, "%lu", lDefault );

    //  use string routine to get ...
    dosGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                                pBuffer, 32, lpszFilename );

//  TRACE((TC_LIB, TT_API1, "GetPrivateProfileLong: %s", pBuffer));
    return( _htol( pBuffer ) );
}

/*******************************************************************************
 *
 *  GetPrivateProfileBool
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

BOOL WFCAPI
GetPrivateProfileBool( PCHAR lpszSection,
                       PCHAR lpszEntry,
                       BOOL  fDefault,
                       PCHAR lpszFilename )
{
    char pDefault[10];
    char pBuffer[10];

    //  make default value a string
    strcpy( pDefault, fDefault ? "On" : "Off" );

    //  use string routine to get ...
    dosGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                                pBuffer, 10, lpszFilename );

    if ( !stricmp(pBuffer,"on") || !stricmp(pBuffer,"yes") ||
         !stricmp(pBuffer,"true") ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}

/*******************************************************************************
 *
 *  FlushPrivateProfileCache
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void WFCAPI
FlushPrivateProfileCache()
{
    PINICACHE pTemp;
    PINICACHE pNext;

    /*
     *  walk cache list freeing items
     */
    for ( pTemp=pSectionRoot; pTemp!=NULL; pTemp=pNext ) {

        TRACE((TC_LIB, TT_API1, "FlushPrivateProfileCache: F=%s, S=%s", pTemp->pFilename, pTemp->pSection));

        // save next pointer
        pNext = pTemp->pNext;

        // dealloc this node
        free( pTemp->pFilename );
        free( pTemp->pSection );
        free( pTemp->pBuffer );
        free( pTemp );
    }

    /*
     *  null list
     */
    pSectionRoot = NULL;

    /*
     *  flush ini file handle cache
     */
    iniflush();
}

/*****************************************************************************
*
*   inifopen
*
*   thunk into fopen for locally cached file handles
*
*   ENTRY:
*
*   EXIT:
*
****************************************************************************/

FILE *
inifopen( PCHAR filename )
{
    FILE * fp = NULL;
    PFILECACHE pTemp;

    /*
     *  walk cache list looking for file
     */
    for ( pTemp=pFileRoot; pTemp!=NULL; pTemp=pTemp->pNext ) {

        /*
         *  match? return cached handle
         */
        if ( !stricmp( pTemp->pFilename, filename) )
            return( pTemp->fp );
    }

    /*
     *  not found, open now
     */
    if ( (pTemp = (PFILECACHE) malloc( sizeof( FILECACHE ) )) != NULL ) {
        if ( (fp = fopen( filename, "rb" )) != NULL ) {
            pTemp->pFilename = strdup( filename );
            pTemp->fp = fp;
            pTemp->pNext = pFileRoot;
            pFileRoot = pTemp;
        }
        else {
            (void) free( pTemp );
        }
    }

    return( fp );
}

/*****************************************************************************
*
*   iniflush
*
*   flush ini file handle cache
*
*   ENTRY:
*
*   EXIT:
*
****************************************************************************/

void
iniflush()
{
    PFILECACHE pTemp;
    PFILECACHE pNext;

    /*
     *  walk cache list freeing cache file handles
     */
    for ( pTemp=pFileRoot; pTemp!=NULL; pTemp=pNext ) {

        // save next pointer
        pNext = pTemp->pNext;

        // dealloc this node
        (void) fclose( pTemp->fp );
        (void) free( pTemp->pFilename );
        (void) free( pTemp );
    }

    /*
     *  null list
     */
    pFileRoot = NULL;
}

/******************************************************************************
 *
 *  GetPrivateProfileEntry
 *
 *  Find the nth key in the specified section
 *
 *  ENTRY:
 *     pszSection (input)
 *        pointer to section name
 *     entry (input)
 *        0-based index of key in the section
 *     pszKey (output)
 *        pointer to place to put key
 *     cbKey (input)
 *        length of pszKey buffer
 *     pszValue (output)
 *        pointer to place to put value
 *     cbValue (input)
 *        length of pszValue buffer
 *     pszFile (input)
 *        pointer to profile file name
 *
 *  EXIT:
 *      TRUE if successful / FALSE if entry not found
 *
 *****************************************************************************/
int WFCAPI GetPrivateProfileEntry( PCHAR pszSection, int entry,
                                     PCHAR pszKey,     int cbKey,
                                     PCHAR pszValue,   int cbValue,
                                     PCHAR pszFile )
{
    int fSuccess = FALSE;
    EXTRAINFO ExtraInfo;

    TRACE(( TC_LIB, TT_API1, "GetPrivateProfileEntry: sec(%s) entry(%d)",
                                   pszSection, entry ));

    if ( !pszSection || !pszKey || !pszValue ) {
       TRACE(( TC_LIB, TT_API1, "GetPrivateProfileEntry: NULL pointer!" ));

       ASSERT( 0, 0 );
       goto done;
    }

    memset( &ExtraInfo, 0, sizeof(ExtraInfo) );
    ExtraInfo.entry = entry;
    ExtraInfo.cbKey = cbKey;
    *pszKey = 0;
    wGetPrivateProfileString( pszSection,
                              pszKey,
                              "",
                              pszValue,
                              cbValue,
                              pszFile,
                              &ExtraInfo );
    if ( *pszKey ) {

       TRACE(( TC_LIB, TT_API1,
               "GetPrivateProfileEntry: found key(%s) val(%s)",
                                     pszKey, pszValue ));
       fSuccess = TRUE;
    }


done:
    return( fSuccess );
}

/******************************************************************************
 *
 *  GetPrivatePrivateProfileSection
 *
 *  Find the position of the section in the profile
 *
 *  ENTRY:
 *     pszSection (input)
 *        pointer to section name
 *     poffBegin (output)
 *        pointer to place to file offset of section beginning
 *     poffEnd (output)
 *        pointer to place to file offset of section end
 *     pszFile (input)
 *        pointer to profile file name
 *
 *  EXIT:
 *      TRUE if successful / FALSE if section not found
 *
 *****************************************************************************/
int  WFCAPI GetPrivatePrivateProfileSection( PCHAR   pszSection,
                                               PUSHORT poffBegin,
                                               PUSHORT poffEnd,
                                               PCHAR   pszFile )
{
    int fSuccess = FALSE;
    EXTRAINFO ExtraInfo;

    TRACE(( TC_LIB, TT_API1, "GetPrivatePrivateProfileSection: sec(%s)",
                                   pszSection ));

    if ( !pszSection || !poffBegin || !poffEnd ) {
       TRACE(( TC_LIB, TT_API1, "GetPrivatePrivateProfileSection: NULL pointer!" ));

       ASSERT( 0, 0 );
       goto done;
    }

    memset( &ExtraInfo, 0, sizeof(ExtraInfo) );
    ExtraInfo.poffBegin = poffBegin;
    ExtraInfo.poffEnd   = poffEnd;
    if ( wGetPrivateProfileString( pszSection,
                                   "",
                                   "",
                                   "",
                                   1,
                                   pszFile,
                                   &ExtraInfo ) ) {
       TRACE(( TC_LIB, TT_API1,
       "GetPrivatePrivateProfileSection: found begin(%d) end(%d)",
                                        *poffBegin, *poffEnd ));
       fSuccess = TRUE;
    }

done:
    return( fSuccess );
}

/******************************************************************************
 *
 *  RefreshProfile
 *
 *  Close the specified profile and dump it's cache
 *  As normal, itt will be reopened and re-cached when it is read next
 *
 *  ENTRY:
 *     pszFile (input)
 *        pointer to profile name
 *
 *  EXIT:
 *      TRUE if successful / FALSE if error occurred
 *
 *****************************************************************************/
int  WFCAPI RefreshProfile( PCHAR pszFile )
{
    int fSuccess = TRUE;
    PINICACHE pTemp;
    PINICACHE pPrev;
    PINICACHE pNext;
    PFILECACHE pFTemp;
    PFILECACHE pFPrev;
    PFILECACHE pFNext;

    TRACE(( TC_LIB, TT_API1, "RefreshProfile: file(%s)",
                                   pszFile ));

    if ( !pszFile ) {
       TRACE(( TC_LIB, TT_API1, "RefreshProfile: NULL pointer!" ));
       ASSERT( 0, 0 );
       fSuccess = FALSE;
       goto done;
    }

    /*
     *  walk cache list freeing items that belong to this file
     */
    for ( pTemp=pSectionRoot; pTemp; pTemp=pNext ) {

        pNext = pTemp->pNext; // save this before we free it

        if ( stricmp( pszFile, pTemp->pFilename ) ) {
           pPrev = pTemp;
        } else {
           TRACE(( TC_LIB, TT_API1,
           "RefreshProfile: freeing sec(%s) file(%s)",
                                       pTemp->pSection, pTemp->pFilename ));
           // Unlink this node
           if ( pTemp == pSectionRoot ) {
              pSectionRoot = pTemp->pNext;
           } else {
              pPrev->pNext = pTemp->pNext;
           }

           // dealloc this node
           free( pTemp->pFilename );
           free( pTemp->pSection );
           free( pTemp->pBuffer );
           free( pTemp );
        }
    }

    /*
     *  walk cache list freeing specified file handle
     */
    for ( pFTemp=pFileRoot; pFTemp; pFTemp=pFNext ) {

        pFNext = pFTemp->pNext; // save this before we free it

        if ( stricmp( pszFile, pFTemp->pFilename ) ) {
           pFPrev = pFTemp;
        } else {
           TRACE(( TC_LIB, TT_API1,
           "RefreshProfile: freeing/closing file(%s)",
                                        pFTemp->pFilename ));

           // Unlink this node
           if ( pFTemp == pFileRoot ) {
              pFileRoot = pFTemp->pNext;
           } else {
              pFPrev->pNext = pFTemp->pNext;
           }

           // dealloc this node
           (void) fclose( pFTemp->fp );
           (void) free( pFTemp->pFilename );
           (void) free( pFTemp );
           break;
        }
    }

done:
    return( fSuccess );
}

/*
 * Like fgets() but strips leading and trailing NL and CR
 */

static char *myfgets(char *buf, int n, FILE *f)
{
    int c, i;

    do
	c = getc(f);
    while (c == '\n' || c == '\r');

    if (c == EOF)
	return NULL;

    i = 0;
    do
    {
	buf[i++] = c;
	c = getc(f);
    }
    while (c != '\n' && c != '\r' && c != EOF && i < n-1);

    buf[i] = '\0';
    
    return ferror(f) ? NULL : buf;
}
