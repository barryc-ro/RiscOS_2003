
/*************************************************************************
*
* input.c
*
* Encryption Protocol Driver - input side
*
* copyright notice: Copyright 1995, Citrix Systems Inc.
*
* smiddle
*
* input.c,v
* Revision 1.1  1998/01/12 11:35:40  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.14   15 Apr 1997 16:51:58   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.14   21 Mar 1997 16:07:10   bradp
*  update
*  
*     Rev 1.13   08 May 1996 16:48:04   jeffm
*  update
*  
*     Rev 1.13   08 May 1996 16:47:30   jeffm
*  update
*  
*     Rev 1.13   08 May 1996 16:45:58   jeffm
*  update
*  
*     Rev 1.12   21 Jul 1995 08:40:54   bradp
*  update
*  
*     Rev 1.10   20 Jul 1995 12:07:54   bradp
*  update
*  
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdcrypt.h"

/*=============================================================================
==   External procedures defined
=============================================================================*/

int STATIC WFCAPI DeviceProcessInput( PPD, LPBYTE, USHORT );


/*=============================================================================
==   External procedures used
=============================================================================*/
int STATIC DeviceCancelWrite( PPD );
int STATIC OutBufAlloc( PPD, POUTBUF, POUTBUF * );
int STATIC OutBufAppend( PPD, POUTBUF, LPBYTE, USHORT );
int STATIC PdNext( PPD, USHORT, PVOID );

/*=============================================================================
==   Defines and structures
=============================================================================*/

/*=============================================================================
==   Local data
=============================================================================*/

/*******************************************************************************
 *
 *  WriteReset
 *
 *  Write a RESET packet
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
WriteReset( PPD pPd )
{
    PPDCRYPT pPdCrypt;
    PDWRITE WriteParams;
    POUTBUF pOutBuf;
    USHORT  KeySize;
    int rc;

    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

    /*
     *  Allocate outbuf
     */
    if ( rc = OutBufAlloc( pPd, NULL, &pOutBuf ) ) {
        ASSERT( FALSE, rc );
        return( rc );
    }

    /*
     *  Initialize RESET header
     */
    pOutBuf->ByteCount = 2;
    *(pOutBuf->pBuffer) = CRYPT_SESSIONKEY;
    *(pOutBuf->pBuffer + 1) = 0;   /* Version level */

    /*
     * This resets the input and output streams.
     *
     * The sender better not be sending any encrypted data at this
     * point.  It must wait for this key.
     *
     * We will be sending any new data with this key.  So the
     * Session key message must go out now, before any other buffers.
     * 
     */
    CreateSessionKey( pPdCrypt, pOutBuf->pBuffer + 2, &KeySize );

    pOutBuf->ByteCount += KeySize;

    pPdCrypt->fSessionKey = TRUE;

    TRACE(( TC_PD, TT_API1, "PdCrypt: send [ rst]" ));

    /*
     *  Write RESET
     */
    WriteParams.pOutBuf = pOutBuf;
    (void) PdNext( pPd, PD__WRITE, &WriteParams );

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  WriteCryptMode
 *
 *  Write a mode packet
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
WriteCryptMode( BYTE CryptMode, PPD pPd )
{
    PPDCRYPT pPdCrypt;
    PDWRITE WriteParams;
    POUTBUF pOutBuf;
    int rc;

    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

    /*
     *  Allocate outbuf
     */
    if ( rc = OutBufAlloc( pPd, NULL, &pOutBuf ) ) {
        ASSERT( FALSE, rc );
        return( rc );
    }

    /*
     *  Initialize header
     */
    pOutBuf->ByteCount = 1;
    *(pOutBuf->pBuffer) = CryptMode;

    CreateRand( pOutBuf->pBuffer + pOutBuf->ByteCount,
                CRYPT_MAGIC_RANDOM_SIZE );
    pOutBuf->ByteCount += CRYPT_MAGIC_RANDOM_SIZE;

    if ( CryptMode == CRYPT_OFF ) {
        memcpy( pOutBuf->pBuffer + pOutBuf->ByteCount,
                CRYPT_MAGIC_KEY_OFF,
                CRYPT_MAGIC_KEY_OFF_SIZE );
        pOutBuf->ByteCount += CRYPT_MAGIC_KEY_OFF_SIZE;
    }
    else {
        memcpy( pOutBuf->pBuffer + pOutBuf->ByteCount,
                CRYPT_MAGIC_KEY_PERM,
                CRYPT_MAGIC_KEY_PERM_SIZE );
        pOutBuf->ByteCount += CRYPT_MAGIC_KEY_PERM_SIZE;
    }

    EncryptStream( pPdCrypt, pOutBuf->pBuffer + 1, (USHORT)(pOutBuf->ByteCount - 1) );

    TRACE(( TC_PD, TT_API1, "PdCrypt: send mode %d", CryptMode ));

    /*
     *  Write mode
     */
    WriteParams.pOutBuf = pOutBuf;
    (void) PdNext( pPd, PD__WRITE, &WriteParams );

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 *  Process a new possibly compressed input buffer
 *
 *  -- this routine is called by the transport PD
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to Pd data structure
 *    pBuffer (input)
 *       Points to input buffer containing data.
 *    ByteCount (input)
 *       Number of bytes of data
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC WFCAPI
DeviceProcessInput( PPD pPd, LPBYTE pBuffer, USHORT ByteCount )
{
    PPDCRYPT pPdCrypt;
    BYTE  CryptHeader;

    /*
     *  Check if ica has been detected
     *  Check if compression is enabled
     */
    if ( !pPd->fIcaDetected || !pPd->fEnableModule )
        return( (*pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );

    pPdCrypt = (PPDCRYPT) pPd->pPrivate;

    /*
     * If we are disabled, it becomes passthrough
     */
    if ( pPdCrypt->fOff )
        return( (*pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );

    /*
     *  Extract the encryption header 
     */
    if ( pPdCrypt->fPerm ) {
        CryptHeader = CRYPT_ENCRYPTED;
    }
    else {
        CryptHeader = *pBuffer++;
        ByteCount--;
    }

    switch ( CryptHeader ) {

    case CRYPT_ENCRYPTED:

        /* 
         *  Check for encrypted data
         */

        ASSERT( pPdCrypt->fSessionKey, pPdCrypt->fSessionKey );

        DecryptStream( pPdCrypt, pBuffer, ByteCount );

        TRACE(( TC_PD, TT_IRAW, "PdCrypt: after decrypt, %d", ByteCount ));
        TRACEBUF(( TC_PD, TT_IRAW, pBuffer, (ULONG)ByteCount ));

	break;


    case CRYPT_PUBLICKEY:

        /*
         *  Check for Public Key
         */

        TRACE(( TC_PD, TT_API3, "PdCrypt: recv [ rst]" ));

        if ( pPdCrypt->fSessionKey ) {
            TRACE(( TC_PD, TT_API3, "PdCrypt: Second key?"));
	    /* 
	     * Someone trying to break in?
	     */
	    return ( CLIENT_STATUS_SUCCESS );
	}

        /*
	 * First byte is version level, just in case.
	 *
	 * Get the public key from the buffer.  
	 */
	InitPublicKey( pPdCrypt, pBuffer + 1, (USHORT)(ByteCount-1) );

        /*
	 * Then setup the encrypted session.
	 */
        WriteReset( pPd );
        pPd->fSendStatusConnect = TRUE;

	return ( CLIENT_STATUS_SUCCESS );

    case CRYPT_NOT_ENCRYPTED:
        /* 
	 * This really shouldn't be allowed.  It maybe OK if you haven't
	 * been setup, but alot of controls must be put in.  I.E. make
	 * sure someone can't sneak in on a reconnect.
	 */
        TRACE(( TC_PD, TT_API3, "PdCrypt: recv unencrypted data bc=%d", ByteCount ));
#ifdef notdef
        if ( pPdCrypt->fSessionKey ) {
            TRACE(( TC_PD, TT_API3, "PdCrypt: received unencrypted data!" ));
            pPd->fOpen = FALSE;
            pPd->CloseCode = CLIENT_ERROR_UNENCRYPT_RECEIVED;
	    return ( CLIENT_STATUS_SUCCESS );
        }
#endif
	break;

    case CRYPT_OFF:

        /*
         *  Check for turning encryption off
         */

        TRACE(( TC_PD, TT_API3, "PdCrypt: recv Off" ));

        if ( !pPdCrypt->fSessionKey || pPdCrypt->fOff || pPdCrypt->fPerm ) {
	    ASSERT( FALSE, CryptHeader );
	    /* 
	     * Something is very wrong
	     */
	    return ( CLIENT_STATUS_SUCCESS );
	}
	if ( ByteCount != CRYPT_MAGIC_KEY_OFF_SIZE+CRYPT_MAGIC_RANDOM_SIZE ) {
	    /* 
	     * Something is very wrong
	     */
	    ASSERT( FALSE, ByteCount );
	    return ( CLIENT_STATUS_SUCCESS );
	}
        DecryptStream( pPdCrypt, pBuffer, ByteCount );

        if ( memcmp( pBuffer+CRYPT_MAGIC_RANDOM_SIZE, CRYPT_MAGIC_KEY_OFF,
	             CRYPT_MAGIC_KEY_OFF_SIZE ) ) {
	    /* 
	     * Something is very wrong
	     */
	    ASSERT( FALSE, CryptHeader );
	    return ( CLIENT_STATUS_SUCCESS );
        }

	pPdCrypt->fOff = TRUE;

        /*
	 * Tell host we are turning off output encryption
	 */
        WriteCryptMode( CRYPT_OFF, pPd );

	return ( CLIENT_STATUS_SUCCESS );

    case CRYPT_PERM:

        /*
         *  Check for making encryption permanant
         */

        TRACE(( TC_PD, TT_API3, "PdCrypt: recv Perm" ));

        if ( !pPdCrypt->fSessionKey || pPdCrypt->fOff || pPdCrypt->fPerm ) {
	    ASSERT( FALSE, CryptHeader );
	    /* 
	     * Something is very wrong
	     */
	    return ( CLIENT_STATUS_SUCCESS );
	}
	if ( ByteCount != CRYPT_MAGIC_KEY_PERM_SIZE+CRYPT_MAGIC_RANDOM_SIZE ) {
	    /* 
	     * Something is very wrong
	     */
	    ASSERT( FALSE, ByteCount );
	    return ( CLIENT_STATUS_SUCCESS );
	}
        DecryptStream( pPdCrypt, pBuffer, ByteCount );

        if ( memcmp( pBuffer+CRYPT_MAGIC_RANDOM_SIZE, CRYPT_MAGIC_KEY_PERM,
	             CRYPT_MAGIC_KEY_PERM_SIZE ) ) {
	    /* 
	     * Something is very wrong
	     */
	    ASSERT( FALSE, CryptHeader );
	    return ( CLIENT_STATUS_SUCCESS );
        }
	pPdCrypt->fPerm = TRUE;

        /*
	 * Tell host we are making encryption permanant
	 */
        WriteCryptMode( CRYPT_PERM, pPd );

	return ( CLIENT_STATUS_SUCCESS );

    default:

        /*
	 *  Errors in a reliable stream???
	 */
	ASSERT( FALSE, CryptHeader );
	return ( CLIENT_STATUS_SUCCESS );

    }

    return( (pPd->pProcessInputProc)( pPd->pWdData, pBuffer, ByteCount ) );
    
}

