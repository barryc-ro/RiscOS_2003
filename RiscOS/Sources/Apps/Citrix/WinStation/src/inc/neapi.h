
/***************************************************************************
*
*  NEAPI.H  - Name Enumerator
*
*  This module contains external name enumerator API defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/11/94)
*
*  $Log$
*  
*     Rev 1.14   15 Apr 1997 18:45:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   11 Feb 1997 13:58:02   jeffm
*  multiple browser support
*  
*     Rev 1.12   06 May 1996 18:40:34   jeffm
*  update
*  
*     Rev 1.11   03 Feb 1996 20:16:22   bradp
*  update
*  
*     Rev 1.10   23 Jan 1996 19:28:36   bradp
*  update
*  
*     Rev 1.9   08 Jan 1996 15:38:54   butchd
*  update
*  
*     Rev 1.8   20 Dec 1995 13:41:50   bradp
*  update
*  
*     Rev 1.7   05 Dec 1995 18:47:00   bradp
*  update
*  
*     Rev 1.6   10 Nov 1995 11:20:18   bradp
*  update
*  
*     Rev 1.5   24 May 1995 19:31:02   butchd
*  update
*  
*     Rev 1.4   02 May 1995 13:40:14   butchd
*  update
*
****************************************************************************/

#ifndef __NEAPI_H__
#define __NEAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Index into NR procedure array
 */
//      DLL$LOAD                 0
//      DLL$UNLOAD               1
//      DLL$OPEN                 2
//      DLL$CLOSE                3
//      DLL$INFO                 4
//      DLL$POLL                 5
#define NE__ENUMERATE           	 6
#define NE__COUNT              	 7   /* number of external ne procedures */

/*
 *  NeOpen structure
 */
typedef struct _NEOPEN {
    PPLIBPROCEDURE pClibProcedures;
    PPLIBPROCEDURE pLogProcedures;
} NEOPEN, * PNEOPEN;

/*
 *  NeEnumerate structure
 */
typedef struct _NEENUMERATE {
    LPBYTE  pTcpBrowserAddress;
    LPBYTE  pIpxBrowserAddress;
    LPBYTE  pNetBiosBrowserAddress;
    USHORT  BrowserRetry;
    USHORT  BrowserTimeout;
    USHORT  DataType;
    USHORT  EnumReqFlags;
    int     LanaNumber;             // netbios lana number
    char *  pName;                  // in/out: pointer to data buffer
    USHORT  ByteCount;              // in/out: length of data buffer in bytes
    LPBYTE  pTcpBrowserAddrList;
    LPBYTE  pIpxBrowserAddrList;
    LPBYTE  pNetBiosBrowserAddrList;
} NEENUMERATE, * PNEENUMERATE;

#ifndef DOS
/*
 * DLL entry point names and typedefs for WINDOWS
 * (Note: 1st parm not used by Windows DLL).
 */
#ifdef WIN16
#define FNNEENUMERATE   "_NeEnumerate"
#else
#define FNNEENUMERATE   "NeEnumerate"
#endif
typedef int (PWFCAPI PFNNEENUMERATE)( LPVOID, PNEENUMERATE );
#endif

#ifdef __cplusplus
}
#endif

#endif //__NEAPI_H__
