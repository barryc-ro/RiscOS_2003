/******************************************************************************
*
*   ENCRYPT.H
*
*   Encrypt/Decrypt header file
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/12/95)
*
*   $Log$
*  
*     Rev 1.8   15 Apr 1997 18:45:08   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.7   06 May 1996 18:38:12   jeffm
*  update
*  
*     Rev 1.6   31 Jan 1996 19:37:40   butchd
*  changed WFCAPI to WFCAPI_NOLOADDS
*  
*     Rev 1.5   24 May 1995 09:39:16   butchd
*  update
*  
*     Rev 1.4   23 May 1995 18:54:48   terryt
*  update
*  
*     Rev 1.3   16 May 1995 06:24:08   butchd
*  update
*  
*     Rev 1.2   02 May 1995 13:39:28   butchd
*  update
*
*******************************************************************************/

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
==   Macros
=============================================================================*/

int WFCAPI_NOLOADDS EncryptToAscii( LPSTR, USHORT, LPSTR, USHORT );
int WFCAPI_NOLOADDS DecryptFromAscii( LPSTR, USHORT, LPSTR, USHORT );
int WFCAPI_NOLOADDS RunEncodeBuffer( PUCHAR, PUCHAR, USHORT );
int WFCAPI_NOLOADDS RunDecodeBuffer( UCHAR, PUCHAR, USHORT );

#ifdef __cplusplus
}
#endif

#endif //__ENCRYPT_H__
