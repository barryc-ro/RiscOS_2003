/*************************************************************************
*
*   BINIGUTS.C
*
*   The main 'guts' of the Buffered INI file routines.  This file is to
*   be included by another C file which will compile for static linked
*   (ie: CFGINI) or proctable called (ie: everything else).
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/10/95)
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:48:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   30 May 1995 12:03:16   butchd
*  update
*  
*     Rev 1.1   30 May 1995 08:31:26   butchd
*  update
*  
*     Rev 1.0   25 May 1995 13:20:16   butchd
*  Initial revision.
*
*************************************************************************/

#include "../ini/helpers.c"     // common helper routines

/*******************************************************************************
 *
 *  bGetPrivateProfileString
 *
 *  Internal GetPrivateProfileString() function  which acts upon a memory buffer
 *  in the "INI" file format (entries of "Key=Value").
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to search.
 *      lpszEntry (input)
 *          Key name to search for associated entry.
 *      lpszDefault (input)
 *          String to return if lpszEntry not found.
 *      lpszDefault (input)
 *          String to return if lpszEntry not found.
 *      cbReturnBuffer (input)
 *          Maximum number of characters that can be written to lpszReturnBuffer.
 *
 *  EXIT:
 *      (int) number of characters written to lpszReturnBuffer (not including
 *          the terminating NULL).
 *
 ******************************************************************************/

int WFCAPI
bGetPrivateProfileString( PCHAR lpszSection,
                          PCHAR lpszEntry,
                          PCHAR lpszDefault,
                          PCHAR lpszReturnBuffer,
                          int   cbReturnBuffer )
{
    int   cb, i;

    /*
     * Validate parameters.
     */
    ASSERT(lpszSection != NULL, 0);
    ASSERT(lpszEntry != NULL, 0);
    ASSERT(lpszDefault != NULL, 0);
    ASSERT(lpszReturnBuffer != NULL, 0);

    /*
     * Search for string in buffer
     */
    if ( !(cb = _w_gpps( lpszSection, lpszEntry, -1,
                         lpszReturnBuffer, cbReturnBuffer, -1 )) ) {

        /*
         * Not found - use default.
         */
        strncpy(lpszReturnBuffer, lpszDefault, cbReturnBuffer);
        lpszReturnBuffer[cbReturnBuffer-1] = 0;
        cb = strlen(lpszReturnBuffer);
    }

#if 0
    /*
     * SJM: Scan from start since there can be no newlines in the middle of an entry.
     * This also then always returns the correct length
     */
    for (i = 0; i < cb; i++)
	switch (lpszReturnBuffer[i])
	{
	case '\r':
	case '\n':
	    lpszReturnBuffer[i] = '\0';
	    cb = i;
	    break;
	case '\0':
	    cb = i;
	    break;
	}
#endif

    TRACE((TC_LIB, TT_API1,
           "bGetPrivateProfileString: %s -> '%s', %u (default=%s)",
           lpszEntry, lpszReturnBuffer, cb, lpszDefault));

    return( cb );

}  // end bGetPrivateProfileString


/*******************************************************************************
 *
 *  bGetPrivateProfileInt
 *
 *  Internal GetPrivateProfileInt() function which acts upon a memory buffer
 *  in the "INI" file format (entries of "Key=Value").
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to search.
 *      lpszEntry (input)
 *          Key name to search for associated entry.
 *      iDefault (input)
 *          Default return value if lpszEntry not found.
 *
 *  EXIT:
 *      (int) Value of entry; iDefault if lpszEntry not found; 0 if error.
 *
 ******************************************************************************/

int WFCAPI
bGetPrivateProfileInt( PCHAR lpszSection,
                       PCHAR lpszEntry,
                       int   iDefault )
{
    char pDefault[32];
    char pBuffer[32];

    TRACE((TC_LIB, TT_API1, "bGetPrivateProfileInt:"));

    //  make default value a string
    sprintf( pDefault, "%u", iDefault );

    //  use string routine to get ...
    bGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                              pBuffer, sizeof(pBuffer) );

    return( (int) _htol( pBuffer ) );
}

/*******************************************************************************
 *
 *  bGetPrivateProfileLong
 *
 *  Internal GetPrivateProfileLong() function which acts upon a memory buffer
 *  in the "INI" file format (entries of "Key=Value").
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to search.
 *      lpszEntry (input)
 *          Key name to search for associated entry.
 *      lDefault (input)
 *          Default return value if lpszEntry not found.
 *
 *  EXIT:
 *      (long) Value of entry; lDefault if lpszEntry not found; 0 if error.
 *
 ******************************************************************************/

long WFCAPI
bGetPrivateProfileLong( PCHAR lpszSection,
                        PCHAR lpszEntry,
                        long  lDefault )
{
    char pDefault[32];
    char pBuffer[32];

    TRACE((TC_LIB, TT_API1, "bGetPrivateProfileLong:"));

    //  make default value a string
    sprintf( pDefault, "%lu", lDefault );

    //  use string routine to get ...
    bGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                              pBuffer, sizeof(pBuffer) );

    return( _htol( pBuffer ) );
}

/*******************************************************************************
 *
 *  bGetPrivateProfileBool
 *
 *  Internal GetPrivateProfileBool() function which acts upon a memory buffer
 *  in the "INI" file format (entries of "Key=Value").
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to search.
 *      lpszEntry (input)
 *          Key name to search for associated entry.
 *      fDefault (input)
 *          Default return value if lpszEntry not found.
 *
 *  EXIT:
 *      (BOOL) Value of entry; fDefault if lpszEntry not found; 0 if error.
 *
 ******************************************************************************/

BOOL WFCAPI
bGetPrivateProfileBool( PCHAR lpszSection,
                        PCHAR lpszEntry,
                        BOOL  fDefault )
{
    char pDefault[10];
    char pBuffer[10];

    TRACE((TC_LIB, TT_API1, "bGetPrivateProfileBool:"));

    //  make default value a string
    strcpy( pDefault, fDefault ? "On" : "Off" );

    //  use string routine to get ...
    bGetPrivateProfileString( lpszSection, lpszEntry, pDefault,
                              pBuffer, sizeof(pBuffer) );

    if ( !stricmp(pBuffer,"on") || !stricmp(pBuffer,"yes") ||
         !stricmp(pBuffer,"true") ) {
        return( TRUE );
    } else {
        return( FALSE );
    }
}

/*******************************************************************************
 *
 *  bGetSectionLength
 *
 *  Determines the length of the buffered INI section, including the final
 *  NULL character.
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to obtain length of.
 *
 *  EXIT:
 *      length in characters of the section
 *
 ******************************************************************************/

int WFCAPI
bGetSectionLength( PCHAR lpszSection )
{
    PCHAR p;
    int length = 0, cb;
#ifdef DEBUG
    char szSectionStub[21];
    strncpy(szSectionStub, lpszSection, 20);
    szSectionStub[20] = '\0';
#endif    

    for ( p = lpszSection; *p; p += cb  )
        length += (cb = strlen(p) + 1);
    length++;   // for final NULL

    TRACE((TC_LIB, TT_API1, "bGetSectionLength: %s..., length=%d", szSectionStub, length));

    return( length );
}

