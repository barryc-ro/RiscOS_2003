
/***************************************************************************
*
*  PDCRYPT.H
*
*  This module contains private PD defines and structures
*
*  Copyright 1995, Citrix Systems Inc.
*
* smiddle
*
* pdcrypt.h,v
* Revision 1.1  1998/01/12 11:35:44  smiddle
* Newly added.#
*
* Version 0.01. Not tagged
*
*  
*     Rev 1.7   15 Apr 1997 16:52:04   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   08 May 1996 16:43:38   jeffm
*  update
*  
*     Rev 1.5   31 May 1995 15:10:48   terryt
*  Change Encryption algorithm
*  
*     Rev 1.4   30 May 1995 17:07:14   terryt
*  Encryption off or force on
*  
*     Rev 1.3   26 May 1995 17:25:12   terryt
*  Simplify Reconnects with encryption
*  
*     Rev 1.2   09 May 1995 11:43:38   terryt
*  Reconnect redo and fix types
*  
*     Rev 1.1   03 May 1995 11:28:30   butchd
*  update
*  
*     Rev 1.0   07 Apr 1995 16:13:24   terryt
*  Initial revision.
*  
*************************************************************************/

#ifndef __PDCRYPT_H__
#define __PDCRYPT_H__


/*=============================================================================
==   Defines
=============================================================================*/

/*
 *  Define flags in the encryption header byte
 *  NOTE: this flags must be kept in sync with the client
 */
#define CRYPT_NOT_ENCRYPTED    0x00    // packet is not encrypted
#define CRYPT_ENCRYPTED        0x01    // packet is encrypted
#define CRYPT_PUBLICKEY        0x02    // sending public key
#define CRYPT_SESSIONKEY       0x04    // sending session key
#define CRYPT_OFF              0x06    // turn encryption off
#define CRYPT_PERM             0x07    // make encryption permanant

#define CRYPT_MAGIC_KEY_OFF            "OffCrypt "
#define CRYPT_MAGIC_KEY_OFF_SIZE       strlen(CRYPT_MAGIC_KEY_OFF)
#define CRYPT_MAGIC_KEY_PERM           "PermCrypt"
#define CRYPT_MAGIC_KEY_PERM_SIZE      strlen(CRYPT_MAGIC_KEY_PERM)
#define CRYPT_MAGIC_RANDOM_SIZE        64-CRYPT_MAGIC_KEY_PERM_SIZE 

/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Comp PD structure
 */
typedef struct _PDCRYPT {
    /*
     * "External" data, non-encryption specific
     */
    USHORT fSessionKey;
    USHORT fOff;
    USHORT fPerm;

    /*
     * The keys and state information.
     */

    BYTE   SessionKey;
    BYTE   InputQueue;
    BYTE   OutputQueue;

} PDCRYPT, * PPDCRYPT;


void EncryptStream( PPDCRYPT, LPBYTE, USHORT );
void DecryptStream( PPDCRYPT, LPBYTE, USHORT );

void CreatePublicKey( PPDCRYPT, LPBYTE, PUSHORT );
void InitPublicKey( PPDCRYPT, LPBYTE, USHORT );

void CreateSessionKey( PPDCRYPT, LPBYTE, PUSHORT );
void InitSessionKey( PPDCRYPT, LPBYTE, USHORT );
void ResetSessionKey( PPDCRYPT, LPBYTE, USHORT );
void ResetSessionState( PPDCRYPT );
void GetSessionKey( PPDCRYPT, LPBYTE, PUSHORT );

void CreateRand( LPBYTE, ULONG );

#define DeviceOutBufAlloc	PdCryptDeviceOutBufAlloc
#define DeviceOutBufError	PdCryptDeviceOutBufError
#define DeviceOutBufFree	PdCryptDeviceOutBufFree
#define DeviceSetInfo		PdCryptDeviceSetInfo
#define DeviceQueryInfo		PdCryptDeviceQueryInfo

#define DeviceProcessInput	PdCryptDeviceProcessInput

#endif //__PDCRYPT_H__
