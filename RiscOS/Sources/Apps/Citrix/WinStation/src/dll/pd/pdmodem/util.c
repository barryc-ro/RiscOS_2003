
/*************************************************************************
 *
 *  UTIL.C
 *
 *  Modme Protocol Driver - utility file
 *
 *  Copyright 1994, Citrix Systems Inc.
 *
 *  Author: Kurt Perry (7/11/94)
 *
 *  $Log$
*  
*     Rev 1.13   15 Apr 1997 16:52:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   21 Mar 1997 16:07:26   bradp
*  update
*  
*     Rev 1.12   03 May 1995 10:20:34   butchd
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
#include "citrix/ica.h"

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/pd.h"

#include "pdmodem.h"


/*=============================================================================
==   Procedures defined
=============================================================================*/

char * BuildModemString( PPDOPEN, char *, char * );
void   DestroyModemString( PPDMODEM );

/*=============================================================================
==   External procedures used
=============================================================================*/

/*=============================================================================
==   Defines and structures
=============================================================================*/

typedef struct _INITSTRING {
    int    fDefault;
    char * pszFunction;
    char * pszString;
} INITSTRING, * PINITSTRING;

/*=============================================================================
==   Local data
=============================================================================*/

INITSTRING vaInitStrings[] = {

    //  hardcoded "canned" strings
    { TRUE, "cr",            "^M" },
    { TRUE, "lf",            "^J" },
    { TRUE, "phonenumber",   NULL },

};
#define MAXCANNEDSTRINGS (sizeof(vaInitStrings)/sizeof(vaInitStrings[0]))
#define PHONENUMBER      2

#define MAXBUFFER   255
char buffer[MAXBUFFER+1];
char buffer2[MAXBUFFER+1];

#define DEF_STRING_SIZE 128


/*******************************************************************************
 *
 *  GetModemStrings
 *
 *  ENTRY:
 *    pPdModme (input)
 *       Pointer to local Pd data structure
 *    pPdOpen (input)
 *       Pointer to Pd Open data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
GetModemStrings( PPDMODEM pPdModem, PPDOPEN pPdOpen )
{

    TRACE(( TC_MODEM, TT_API1, "PdModem: GetModemStrings" ));

    /*
     *  Build modem strings
     */
    if ( (pPdModem->pszInitString = BuildModemString( pPdOpen, INI_MODEMINIT,    DEF_MODEMINIT )) == NULL    ||
         (pPdModem->pszDialString = BuildModemString( pPdOpen, INI_MODEMDIAL,    DEF_MODEMDIAL )) == NULL    ||
         (pPdModem->pszHangString = BuildModemString( pPdOpen, INI_MODEMHANGUP,  DEF_MODEMHANGUP )) == NULL  ||
         (pPdModem->pszAnswString = BuildModemString( pPdOpen, INI_MODEMLISTEN,  DEF_MODEMLISTEN )) == NULL  ||
         (pPdModem->pszConnString = BuildModemString( pPdOpen, INI_MODEMCONNECT, DEF_MODEMCONNECT )) == NULL ) {

        ASSERT( FALSE, CLIENT_ERROR_NO_MEMORY );
        return( CLIENT_ERROR_NO_MEMORY );
    }

    /*
     *  Get times
     */
    pPdModem->cRetryTimeout  = (USHORT) bGetPrivateProfileInt( pPdOpen->pIniSection, INI_DIALTIMEOUT, DEF_DIALTIMEOUT );
    pPdModem->ulRetryTimeout = (ULONG) pPdModem->cRetryTimeout * 1000L;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  BuildModemString
 *
 *  ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

char *
BuildModemString( PPDOPEN pPdOpen, char * pszEntry, char * pszDefault )
{
    int  i;
    int  j;
    int  cbString = DEF_STRING_SIZE;
    int  iString = 0;
    int  cbSub;
    int  cbFun;
    int  cbBuf;
    int  iEntry;
    char c;
    char * p;
    char * pszString;
    char achEntry[80];

    TRACE(( TC_MODEM, TT_API1, "PdModem: BuildModemString %s", pszEntry ));

    /*
     *  Get initial modem string size
     */
    if ( (pszString = (char *) malloc( cbString )) == NULL ) {
        return( NULL );
    }
    pszString[0] = '\0';

    /*
     *  Try many variations
     */
    for ( iEntry=0; ; iEntry++ ) {

        /*
         *  Try default first, then enumerations
         */
        if ( iEntry == 0 ) {
            sprintf( achEntry, "%s", pszEntry );
        }
        else {
            sprintf( achEntry, "%s%u", pszEntry, iEntry );
        }

        /*
         *  Read the ini section, quit if not found after 0th entry
         *  In other words, we look for COMMAND_xxx1 even if we can't
         *  find COMMAND_xxx.
         */
        if ( !bGetPrivateProfileString( pPdOpen->pIniSection, achEntry,
                                        INI_EMPTY, buffer, sizeof(buffer) )
             && iEntry != 0 ) {
            break;
        }

        /*
         *  Ignore blank lines
         */
        if ( (cbBuf = strlen( buffer )) == 0 ) {
            continue;
        }

        TRACE(( TC_MODEM, TT_API2, "PdModem: parse %s=%s", achEntry, buffer ));

        /*
         *  Do substitution
         */
        for ( i=0; i<cbBuf; ) {

            //  get current char
            c = buffer[i++];

            //  begin substitution string
            if ( c == '<' ) {

                /*
                 *  Get pointer to ?> and look for match
                 */
                p = &buffer[i];

                /*
                 *  Look for known string matches
                 */
                for ( j=0; j<MAXCANNEDSTRINGS; j++ ) {

                    /*
                     *  Length of function string to match
                     */
                    cbFun = strlen(vaInitStrings[j].pszFunction);

                    /*
                     *  Match of function string?
                     */
                    if ( !strnicmp( p, vaInitStrings[j].pszFunction, cbFun ) ) {

                        /*
                         *  Is this the phone number entry
                         */
                        if ( j == PHONENUMBER ) {
                            bGetPrivateProfileString( pPdOpen->pIniSection, 
                                                      INI_PHONENUMBER, INI_EMPTY, 
                                                      buffer2, sizeof(buffer2) );
                            vaInitStrings[j].pszString = buffer2;
                        }

                        /*
                         *  Lenght of substitute string
                         */
                        cbSub = strlen(vaInitStrings[j].pszString);
                        if ( (iString+cbSub) >= cbString ) {
                            cbString += DEF_STRING_SIZE;
                            if ( (pszString = realloc( pszString, cbString )) == NULL ) {
                                return( NULL );
                            }
                        }

                        /*
                         *  Substitute and adjust pointers
                         */
                        if ( cbSub ) {
                            memcpy( &pszString[iString], vaInitStrings[j].pszString, cbSub );
                            iString += cbSub;
                        }
                        i += (cbFun + 1);
                        goto get_next;
                    }
                }

                /*
                 *  <?> not found, remove it from input string
                 */
                for ( j=i; j<cbBuf; j++ ) {
                    ++i;
                    if ( *(p++) == '>' ) {
                        break;
                    }
                }
            }
            else {
                pszString[iString++] = c;
            }

            /*
             *  Need to realloc?
             */
get_next:
            if ( iString >= cbString ) {
                cbString += DEF_STRING_SIZE;
                if ( (pszString = realloc( pszString, cbString )) == NULL ) {
                    return( NULL );
                }
            }
        }
    }

    /*
     *  Realloc string and NULL terminate
     */
    if ( iString ) {
        pszString = realloc( pszString, iString+1 );
        pszString[iString] = '\0';
    }
    else {
        pszString = pszDefault;
    }

    TRACE(( TC_MODEM, TT_API1, "PdModem: %s=%s", pszEntry, pszString ));

    return( pszString );
}


/*******************************************************************************
 *
 *  DestroyModemStrings
 *
 *  ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
DestroyModemString( PPDMODEM pPdModem )
{

    TRACE(( TC_MODEM, TT_API1, "PdModem: DestroyModemStrings" ));

    /*
     *  Free memory associated with modem strings
     */
    if ( pPdModem->pszInitString != NULL ) {
        (void) free( pPdModem->pszInitString );
        pPdModem->pszInitString = NULL;
    }

    if ( pPdModem->pszDialString != NULL ) {
        (void) free( pPdModem->pszDialString );
        pPdModem->pszDialString = NULL;
    }

    if ( pPdModem->pszHangString != NULL ) {
        (void) free( pPdModem->pszHangString );
        pPdModem->pszHangString = NULL;
    }

    if ( pPdModem->pszAnswString != NULL ) {
        (void) free( pPdModem->pszAnswString );
        pPdModem->pszAnswString = NULL;
    }

    if ( pPdModem->pszConnString != NULL ) {
        (void) free( pPdModem->pszConnString );
        pPdModem->pszConnString = NULL;
    }

    if ( pPdModem->pszModemRespBuffer != NULL ) {
        (void) free( pPdModem->pszModemRespBuffer );
        pPdModem->pszModemRespBuffer = NULL;
    }
}
