
/***************************************************************************
*
*  NRAPI.H  - Name Resolver
*
*  This module contains external name resolver API defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/3/94)
*
*  nrapi.h,v
*  Revision 1.1  1998/01/12 11:36:54  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.11   15 Apr 1997 18:45:44   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.11   19 Mar 1997 14:03:14   richa
*  Send ClientName to the TD so that we'll be able to reconnect to disconnected session in a cluster.
*  
*     Rev 1.10   08 Feb 1997 13:52:06   jeffm
*  added multiple browser support
*  
*     Rev 1.9   06 May 1996 18:40:52   jeffm
*  update
*  
*     Rev 1.8   20 Dec 1995 13:41:54   bradp
*  update
*  
*     Rev 1.7   05 Dec 1995 18:47:04   bradp
*  update
*  
*     Rev 1.6   10 Nov 1995 11:20:28   bradp
*  update
*  
*     Rev 1.5   02 May 1995 13:40:20   butchd
*  update
*
****************************************************************************/
#ifndef __NRAPI_H__
#define __NRAPI_H__

/*
 *  Index into NR procedure array
 */
//      DLL__LOAD                 0
//      DLL__UNLOAD               1
//      DLL__OPEN                 2
//      DLL__CLOSE                3
//      DLL__INFO                 4
//      DLL__POLL                 5
#define NR__NAMETOADDRESS   	 6
#define NR__ERRORLOOKUP          7
#define NR__COUNT              	 8   /* number of external nr procedures */

/*
 *  NrOpen structure
 */
typedef struct _NROPEN {
    PPLIBPROCEDURE pClibProcedures;
    PPLIBPROCEDURE pLogProcedures;
    LPBYTE pTcpBrowserAddress;
    LPBYTE pIpxBrowserAddress;
    LPBYTE pNetBiosBrowserAddress;
    USHORT BrowserRetry;
    USHORT BrowserTimeout;
    LPBYTE pTcpBrowserAddrList;
    LPBYTE pIpxBrowserAddrList;
    LPBYTE pNetBiosBrowserAddrList;

    PPLIBPROCEDURE pDeviceProcedures;
} NROPEN, * PNROPEN;

/*
 *  NrNameToAddress structure
 */
typedef struct _NAMEADDRESS {
    ADDRESS Name;
    ADDRESS Address;
    CLIENTNAME  ClientName;
    int LanaNumber;             // netbios lana number
} NAMEADDRESS, * PNAMEADDRESS;


#endif //__NRAPI_H__

