
/*************************************************************************
*
*   HELPERS.C
*
*   Common helper routines used in both 'internal' INI apis and
*   'external' INI apis.
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/10/95) [extracted from Kurt Perry's INIAPI.C]
*
*   Log: See VLOG
*
*
*************************************************************************/


/*******************************************************************************
 *
 *  _w_gpps
 *
 *  Worker routine used to search buffer for ENTRY= match
 *
 *  ENTRY:
 *
 *  EXIT:
 *      (int) number of characters written to pReturn (not including the
 *          terminating NULL).  0 if no match was made.
 *
 ******************************************************************************/

static int
_w_gpps( PCHAR pBuffer, PCHAR pEntry, int cbKey,
         PCHAR pReturn, int cbReturn, int entry )
{
    int len;
    BOOL bMatch = FALSE;
    PCHAR pBuf = pBuffer;
    PCHAR p = NULL;
    PCHAR q = NULL;
    PCHAR r = NULL;
    PCHAR s = NULL;
    PCHAR t = NULL;
    PCHAR u;
    int i;

    ASSERT( pReturn != NULL, 0 );

    /*
     *  search while there are string of any length
     */
    for ( i = 0; (len = strlen(pBuf)) &&
                 ( (entry == -1) || (i <= entry)); i++ ) {

        //  make copy so we may mess with it
        if ( (p = strdup( pBuf )) == NULL )
            return( 0 );

        //  find = sign, and use as temporary terminator
        if ( (q=strchr( p, '=' )) != NULL ) {

            //  find begining of entry
            r = p + strspn( p, " \t" );

            //  get past the = and terminate the pEntry match
            *(q++) = 0;

#ifdef NO_SPACES_ALLOWED_IN_KEY
            //  find end of entry and terminate
            if ( (s = strpbrk( r, " \t" )) != NULL )
               *s = 0;
#else
            s = q-2;
            while ( (s > p) && ((*s == ' ') || (*s == '\t')) ) *s-- = '\0';
//          TRACE(( TC_LIB, TT_API1, "w_gpps: look(%s) found(%s)",
//                                         pEntry, r ));
#endif

            //  now look for entry match
            if ( ((entry == -1) && !stricmp( r, pEntry )) ||
                 ((entry != -1) && ( i == entry ))        ) {

                // For our special case, return the key
                if ( entry != -1 ) {
                   //  make copy so we may mess with it
                   if ( (t = strdup( pBuf )) == NULL ) {
                       (void) free( p );    // free duped string (bd 5-10-95)
                       break;
                   }
                   *(u = strchr( t, '=' )) = '\0';
                   for ( u--; (u > t) && ((*u==' ') ||(*u=='\t')); )
                      *u-- = '\0';
                   for ( u = t; *u && ((*u==' ') ||(*u=='\t')); u++ );
                   strncpy( pEntry, u, cbKey );
                   pEntry[cbKey-1] = 0;     // (bd 5-11-95)
                   free( t );
                }

                //  find begining of string
                r = q + strspn( q, " \t" );

                //  find the end of the string and terminate
                s = r + strlen(r) - 1;
                while ( *s == ' ' || *s == '\t' )
                    *(s--) = 0;

                //  return matched entry string
                strncpy( pReturn, r, cbReturn );
                pReturn[cbReturn-1] = 0;    // (bd 5-11-95)
                bMatch = TRUE;

                //  free duped string
                (void) free( p );
                break;
            }
        }

        //  free duped string
        (void) free( p );

        //  next string
        pBuf += (len + 1);
    }

    //  return length
    return( bMatch ? strlen(pReturn) : 0 );
}

/*****************************************************************************
*
*   _htol:
*
*   ENTRY:
*      s     -  string to convert to ULONG
*
*   EXIT:
*      Normal:
*         USHORT value
*
*      Error:
*         NONE
*
*   ALGORITHM:
*
*
****************************************************************************/

static ULONG
_htol( PCHAR s )
{
    CHAR c;
    PCHAR p;
    ULONG val = 0L;

    // look for '0x' prefix
    if( (p = strstr( s, "0x" )) != NULL || (p = strstr( s, "0X" )) != NULL ) {
        s = (p + 2);     // remove '0x' prefix
    }

    // look for 'h' suffix
    else if ( (p = strchr( s, 'h' )) != NULL || (p = strchr ( s, 'H' )) != NULL ) {
        *p = 0;     // remove 'h' suffix
    }

    // just plain decimal number
    else {
        return( atol( s ) );
    }

    // do the hex conversion
    while(((c = (CHAR)toupper(*s++)) >= '0' && c <= '9') ||
           (c >='A' && c <= 'F')) {
        val = (val << 4)
            + ((c >= '0' && c <= '9') ? (ULONG)(c - '0') :
                                        (ULONG)(10 + (c - 'A')));
    }

    return(val);
}


