/*******************************************************************************
*
*  ENCRYPT.C
*     This module contains the routines to encrypt and decrypt buffers.
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author:      Ken Beal
*
*  $Log$
*  
*     Rev 1.11   15 Apr 1997 18:52:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.10   31 Jan 1996 19:37:04   butchd
*  changed WFCAPI to WFCAPI_NOLOADDS
*  
*     Rev 1.9   11 Jul 1995 17:43:22   marcb
*  update
*  
*     Rev 1.8   30 May 1995 17:22:48   terryt
*  Zero seeds not allowed
*  
*     Rev 1.7   24 May 1995 09:40:02   butchd
*  update
*  
*     Rev 1.6   23 May 1995 18:57:24   terryt
*  Encryption
*  
*     Rev 1.5   16 May 1995 07:47:48   butchd
*  update
*  
*     Rev 1.4   03 May 1995 10:34:38   butchd
*  clib.h now standard
*  
*******************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


/*  Get CLIB includes */
#include "../../inc/client.h"
#ifdef  DOS
#include "../../inc/dos.h"
#endif
#include "../../inc/clib.h"
#include "../../inc/encrypt.h"


// local defines
//
USHORT BinToAscii2( LPSTR, USHORT, LPSTR, USHORT, LPSTR, USHORT );
USHORT AsciiToBin( LPSTR, USHORT, LPSTR, USHORT );
VOID SetupBinAscii( VOID );

CHAR BINASCII[] = {
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};
CHAR ASCIIBIN[256] = { 0 };
BOOL vfCalledSetupBinAscii = FALSE;

//
// SetupBinAscii: initialization code for ASCIIBIN[] structure.  We want to
// set each one in ASCIIBIN[] to point "back" to the value we get when we
// do BINASCII[i].  I.e.: i = ASCIIBIN[ BINASCII[ i ] ].
//

VOID SetupBinAscii( VOID )
{
   USHORT i;

   for ( i=0; i<16; i++ ) {
      ASCIIBIN[ BINASCII[i] ] = (BYTE) i;
   }
} // SetupBinAscii()

//
// BinToAscii2: takes a pointer to binary data and a length, and a pointer
// to the buffer to write it to and its length.  It returns the length of
// the ascii data written. The data is in a counted hex string.
// Note that this actually concatonates two strings
//
// Return value: length of ASCII buffer if successful;
//               0 if ASCII buffer wasn't big enough.
//


USHORT BinToAscii2( LPSTR pszBin1,   USHORT usLenBin1,
                    LPSTR pszBin2,   USHORT usLenBin2,
                    LPSTR pszAscii, USHORT usLenAscii )
{

   USHORT usBin;
   USHORT usAscii = 0;

   //
   // If not enough room ==> error
   //
   if ( usLenAscii < 4 + (usLenBin1 + usLenBin2) * 2 )
       return 0;

   pszAscii[usAscii++] = BINASCII[ ((usLenBin1 + usLenBin2) >> 12) & 0xf ];
   pszAscii[usAscii++] = BINASCII[ ((usLenBin1 + usLenBin2) >> 8) & 0xf ];
   pszAscii[usAscii++] = BINASCII[ ((usLenBin1 + usLenBin2) >> 4) & 0xf ];
   pszAscii[usAscii++] = BINASCII[ (usLenBin1 + usLenBin2) & 0xf ];

   for ( usBin = 0; usBin < usLenBin1; usBin++ ) {
       pszAscii[usAscii++] = BINASCII[((BYTE)(pszBin1[usBin]) >> 4) & 0xf ];
       pszAscii[usAscii++] = BINASCII[((BYTE)(pszBin1[usBin])     ) & 0xf ];
   };

   for ( usBin = 0; usBin < usLenBin2; usBin++ ) {
       pszAscii[usAscii++] = BINASCII[((BYTE)(pszBin2[usBin]) >> 4) & 0xf ];
       pszAscii[usAscii++] = BINASCII[((BYTE)(pszBin2[usBin])     ) & 0xf ];
   };

   pszAscii[usAscii] = 0; // Null terminate

   return( usAscii );

} // BinToAscii2()


//
// AsciiToBin: reverse of the above.
//

USHORT AsciiToBin( LPSTR pszAscii, USHORT usLenAscii,
                   LPSTR pszBin,   USHORT usLenBin   )
{
   USHORT usBin;
   USHORT usLenBuf;
   USHORT usAscii = 0;

   if ( !vfCalledSetupBinAscii ) {
      SetupBinAscii();
      vfCalledSetupBinAscii = TRUE;
   }

   //
   // If not long enough for length ==> error
   //
   if ( usLenAscii < 4 ) {
      usLenBuf = 0;
      goto Done;
   }

   usLenBuf  = ASCIIBIN[ pszAscii[usAscii++] ] << 12;
   usLenBuf += ASCIIBIN[ pszAscii[usAscii++] ] << 8;
   usLenBuf += ASCIIBIN[ pszAscii[usAscii++] ] << 4;
   usLenBuf += ASCIIBIN[ pszAscii[usAscii++] ] ;

   //
   // If not enough room for binary ==> error
   //
   if ( usLenBuf > usLenBin ) {
      usLenBuf = 0;
      goto Done;
   }

   //
   // If not enough ASCII for conversion ==> error
   //
   if ( usLenBuf > ( usLenAscii - 4 ) / 2 )  {
      usLenBuf = 0;
      goto Done;
   }


   // Now, rip through them 2 at a time, breaking them out.  We count up with
   // usBin, but we return usLenBuf -- which was determined from the first 4
   // bytes of ASCII data.  (This is necessary so we won't pad on BinToAscii()
   // and then return EXTRA bytes on AsciiToBin().)

   for ( usBin = 0; usBin < usLenBuf; usBin++ ) {
      pszBin[usBin]  = (BYTE) (ASCIIBIN[ pszAscii[usAscii++] ] << 4);
      pszBin[usBin] += (BYTE) (ASCIIBIN[ pszAscii[usAscii++] ]);
   };

Done:
   return( usLenBuf );
} // AsciiToBin()

//
// EncryptToAscii: "wrapper" around the Encrypt() and BinToAscii() routines.
// Pass in the buffer you want to encrypt (we'll destroy it), and its length;
// and the buffer you want ASCII in and its length.
//
// Return: length of resulting string, or 0 if something went wrong.
//

int WFCAPI_NOLOADDS 
EncryptToAscii( LPSTR pszOrig,  USHORT usOrigLen,
                LPSTR pszAscii, USHORT usAsciiLen )
{
   BYTE Seed = 0; /* Random seed */
   int Length;

   RunEncodeBuffer( &Seed, pszOrig, usOrigLen );

   //
   // Prefix the encrypted string with the seed
   //

   Length = BinToAscii2( (LPSTR)&Seed, 1, pszOrig, usOrigLen, pszAscii, usAsciiLen );

   
   return( Length );
} // EncryptToAscii()


//
// DecryptFromAscii: "wrapper" around AsciiToBin() and Decrypt(), as above.
// Return: length of result, or 0 if error.
//

int WFCAPI_NOLOADDS
DecryptFromAscii( LPSTR pszAscii,  USHORT usAsciiLen,
                  LPSTR pszResult, USHORT usResultLen )
{
   USHORT rc;
   USHORT i;
   BYTE Seed;

   rc = AsciiToBin( pszAscii, usAsciiLen, pszResult, usResultLen );
   if ( rc ) {
      //
      // Pick off the seed in the first byte
      //
      Seed = pszResult[0];
      for ( i = 0; i < rc; i++ )
         pszResult[i] = pszResult[i+1];
      rc--;
      RunDecodeBuffer( Seed, pszResult, rc );
   }
   return( rc );
} // DecryptFromAscii()


//
// RunDecodeBuffer
//
// A bit of simple descrambling
//
//
int WFCAPI_NOLOADDS 
RunDecodeBuffer( UCHAR Seed, PUCHAR Buffer, USHORT Length )
{
    USHORT i;

    if ( Length == 0 )
       return 0;

    for ( i = Length; i > 1; i-- ) {
          Buffer[i-1] ^= ( Buffer[i-2] ^ Seed );
    }

    if ( Length >= 1 ) 
        Buffer[0] ^= (Seed | 0X43);

    return 0;
}

//
// RunEncodeBuffer
//
// A bit of simple scrambling
//
//
int WFCAPI_NOLOADDS
RunEncodeBuffer( PUCHAR Seed, PUCHAR Buffer, USHORT Length )
{
    USHORT i;

    if ( Length == 0 )
       return 0;

    if ( *Seed == 0 ) {
        *Seed = (UCHAR)Getmsec();
	if ( *Seed == 0 )
	   *Seed = 42;
    }

    Buffer[0] ^= ((*Seed) | 0X43);

    for ( i = 1; i < Length; i++ ) {
          Buffer[i] ^= (Buffer[i-1]^(*Seed));
    }

    return 0;
}

