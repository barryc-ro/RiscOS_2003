/*************************************************************************
*
* encrypt.c
*
* Encryption module.
*
* IF YOU MAKE A CHANGE IN THIS FILE, PLEASE MAKE THE SAME CHANGE IN THE
* CLIENT SOURCE SO WE CAN KEEP THINGS IN SYNC.
*
* copyright notice: Copyright 1995, Citrix Systems Inc.
*
* smiddle
*
* encrypt.c,v
* Revision 1.1  1998/01/12 11:35:39  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.6   15 Apr 1997 16:51:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   21 Mar 1997 16:07:10   bradp
*  update
*  
*     Rev 1.5   08 May 1996 16:48:38   jeffm
*  update
*  
*     Rev 1.4   31 May 1995 15:11:02   terryt
*  Change Encryption algorithm
*  
*     Rev 1.3   09 May 1995 11:43:08   terryt
*  Reconnect redo and fix types
*  
*     Rev 1.2   03 May 1995 11:27:18   butchd
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

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdcrypt.h"

/******************* Common code ****************************/

void STATIC
GetTimeStamp( LPBYTE Stamp, ULONG Size )
{
    int i;
#ifdef DOS
    ULONG SystemTime;

    _dos_gettime(&SystemTime);
#else
    DWORD SystemTime = 0;	// SJM: fixme

//  SystemTime = GetCurrentTime();
#endif

    for ( i = 0; i < sizeof(SystemTime); i++)
        Stamp[i%Size] += ((LPBYTE)(&SystemTime))[i];
}


/*
 * Don't pay too much attention to this, it's just expanding small
 * bits of random data into larger sizes.  The real way to do this is
 * to use DES to really shuffle data around.
 *
 * At the moment we really only use this for 1 byte session keys,
 * so it's overkill;
 */
void STATIC 
CreateRand( LPBYTE RandData, ULONG Size )
{
    ULONG i;
    ULONG j;
    BYTE  Rand[4];
    BYTE  Mess = 0x42;

    for ( i = 0; i < Size; i++ ) {
        if ( i%4 == 0 )
            GetTimeStamp( Rand, 4 );
        RandData[i] = Rand[i%4];
    }
    for ( i = 0; i < 4; i++ ) {
        for ( j = 0; j < Size; j++ ) {
	    RandData[j] += Mess;
	    Mess = RandData[j] ^ (BYTE)j + (BYTE)i;
	}
    }
}


/*
 *  The algorithm is XOR the previously encrypted byte with the seed, and then
 *  XOR the current unencrypted byte to get the encrypted byte to send.
 *
 *  The assumption here is that Size > 0.
 *
 *  The USHORT's and the do-while are here because this was the best
 *  code generated for the Client.  The C optimizers aren't very good,
 *  in the main loop, there should only be 1 read and 1 write memory,
 *  unfortuneately there are normally more.
 */
void STATIC 
EncryptStream( PPDCRYPT pPdCrypt,
               LPBYTE Buffer,
	       USHORT Size )
{
    USHORT Cur;
    USHORT Seed;

    Seed = pPdCrypt->SessionKey;
    Cur = pPdCrypt->OutputQueue;

    do {
        Cur = *Buffer ^ ( Cur ^ Seed );
	*Buffer++ = (BYTE)Cur;
    } while ( --Size );

    pPdCrypt->OutputQueue = (BYTE)Cur;
}

/*
 *  The algorithm is XOR the previously encrypted byte with the seed, and then
 *  XOR the current encrypted byte to get the unencrypted byte.
 *
 *  The assumption here is that Size > 0.
 *
 *  The USHORT's, the for loop, and the backwards scan are here because this
 *  was the best code generated for the Client.  The C optimizers aren't
 *  very good.  In the main loop there should only be 1 read and 1 write
 *  memory, unfortuneately there are normally more.
 */
void STATIC 
DecryptStream( PPDCRYPT pPdCrypt,
               LPBYTE Buffer,
	       USHORT Size )
{
    USHORT i;
    USHORT Last;
    USHORT Seed;

    Seed = pPdCrypt->SessionKey;
    Last = Buffer[Size-1];

    for ( i = Size-1; i > 0; i-- ) {
        Buffer[i] ^= ( Buffer[i-1] ^ Seed );
    }

    Buffer[0] ^= (pPdCrypt->InputQueue ^ Seed);

    pPdCrypt->InputQueue = (BYTE)Last;

}

/*
 *  This should be RSA. 
 */
void STATIC 
CreatePublicKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, PUSHORT Size )
{
    *Size = 0;
}

/*
 *  This should be RSA. 
 */
void STATIC 
InitPublicKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, USHORT Size )
{

}

/*
 * Initialize the queues with agreed upon values.
 */
void STATIC 
InitSessionState( PPDCRYPT pPdCrypt )
{
   pPdCrypt->InputQueue = pPdCrypt->SessionKey | 0x43;
   pPdCrypt->OutputQueue = pPdCrypt->SessionKey | 0x43;
}

/*
 * Initialize the queues with agreed upon values.
 *
 * This is done on reconnects.
 *
 * If this were real encryption, we wouldn't want to use the same values
 * that we started with.
 */
void STATIC 
InitSessionState2( PPDCRYPT pPdCrypt )
{

   pPdCrypt->InputQueue = pPdCrypt->SessionKey | 0x43;
   pPdCrypt->OutputQueue = pPdCrypt->SessionKey | 0x43;

}

/*
 * Using Private key, unencrypt the buffer and use it to initialize the
 * session.
 *
 */
void STATIC 
InitSessionKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, USHORT Size ) 
{
   if ( Size < 1 )
       return;

   pPdCrypt->SessionKey = *Buffer;

   InitSessionState( pPdCrypt );
}

/*
 * This is called by reconnected sessions.  No public/private needed.
 */
void STATIC 
ResetSessionKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, USHORT Size ) 
{
   if ( Size < 1 )
       return;

   pPdCrypt->SessionKey = *Buffer;

   InitSessionState2( pPdCrypt );
}

/*
 * This is called by reconnected sessions.  No public/private needed.
 */
void STATIC 
ResetSessionState( PPDCRYPT pPdCrypt ) 
{
   InitSessionState2( pPdCrypt );
}


/*
 * Create a session key and then encrypt it using the public key.
 */
void STATIC 
CreateSessionKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, PUSHORT Size )
{
    CreateRand( &pPdCrypt->SessionKey, 1 );

    if ( pPdCrypt->SessionKey == 0 )
        pPdCrypt->SessionKey = 0x42;

    *Buffer = pPdCrypt->SessionKey;
    *Size = 1;

    InitSessionState( pPdCrypt );
}

/*
 * Get session key, for use by reconnected sessions
 */
void STATIC 
GetSessionKey( PPDCRYPT pPdCrypt, LPBYTE Buffer, PUSHORT Size ) 
{
   if ( *Size > 0 ) {
       *Buffer = pPdCrypt->SessionKey;
       *Size = 1;
   }
}
