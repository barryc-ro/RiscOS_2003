
/*************************************************************************
*
* tdtcpro.c
*
* Protocol Driver - TCP/IP under RISC OS
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Simon Middleton from tdtcpms.c
*
*  tdtcpro.c,v
*  Revision 1.1  1998/01/12 11:36:04  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.21   15 Apr 1997 17:48:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.22   21 Mar 1997 16:08:26   bradp
*  update
*  
*     Rev 1.21   19 Mar 1997 14:08:24   richa
*  Send ClientName to the TD so that we'll be able to reconnect to disconnected session in a cluster.
*  
*     Rev 1.20   13 Aug 1996 13:03:36   BillG
*  add ICA port number
*  
*     Rev 1.21   31 Jul 1996 13:54:18   BillG
*  
*     Rev 1.19   18 Mar 1996 18:13:54   KenB
*  move error message to nls\us\tdtcpmst.str
*  
*     Rev 1.18   20 Jul 1995 12:09:44   bradp
*  update
*  
*     Rev 1.17   18 Apr 1995 11:56:10   unknown
*  update
*  
*     Rev 1.16   17 Apr 1995 14:43:22   unknown
*  update
*  
*************************************************************************/

/*
 *  Includes
 */
#include "windows.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../inc/client.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/clib.h"
#endif

#include "../../../inc/wdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/td.h"

#define NO_TDDEVICE_DEFINES
#include "../../../inc/tddevice.h"
#include "../../../inc/tddevicep.h"

#include "tdtcpro.h"

#include "sys/types.h"
#undef AF_IPX
#include "sys/socket.h"
#include "sys/errno.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int DeviceOpen( PPD, PPDOPEN );
static int DeviceClose( PPD, PDLLCLOSE );
static int DeviceInfo( PPD, PDLLINFO );
static int DeviceConnect( PPD );
static int DeviceDisconnect( PPD );
static int DeviceProcessInput( PPD );
static int DeviceWrite( PPD, POUTBUF, PUSHORT );
static int DeviceCheckWrite( PPD, POUTBUF );
static int DeviceCancelWrite( PPD, POUTBUF );
static int DeviceSendBreak( PPD );

PLIBPROCEDURE TdTcpRODeviceProcedures[TDDEVICE__COUNT] =
{
    (PLIBPROCEDURE)DeviceOpen,
    (PLIBPROCEDURE)DeviceClose,

    (PLIBPROCEDURE)DeviceInfo,

    (PLIBPROCEDURE)DeviceConnect,
    (PLIBPROCEDURE)DeviceDisconnect,

    (PLIBPROCEDURE)DeviceProcessInput,

    (PLIBPROCEDURE)DeviceWrite,
    (PLIBPROCEDURE)DeviceCheckWrite,
    (PLIBPROCEDURE)DeviceCancelWrite,

    (PLIBPROCEDURE)DeviceSendBreak
};

/*=============================================================================
==   Internal Functions
=============================================================================*/

int SetLastError( PPD, int );

/*=============================================================================
==   External Functions Used
=============================================================================*/

extern int GetTCPHostNamePort(char *szHostString, char *szHostName, USHORT *pPortNumber);
extern int NameToAddress( PPD, PNAMEADDRESS );

/*=============================================================================
==   External Data
=============================================================================*/
extern int errno;

/*=============================================================================
==   Local Data
=============================================================================*/

/*
 *  Define TCP Protocol driver data structure
 *   (this could be dynamically allocated)
 */
//static PDTCP PdTcpData = {0};

/*
 *   TCP error messages
 */
static LPBYTE pProtocolName = "TCP/IP";

/* Define the ICA TCP Port number
 * The default value is CITRIX_TCP_PORT. 
 * This value can be changed by specifying the port number within the host name 
 * (format "Hostname:Portnumber"), or specifying the port number in the ".ini" files.
 */

static USHORT gTCP_PortNumber = CITRIX_TCP_PORT;



/*******************************************************************************
 *
 *  DeviceOpen
 *
 *  initialize
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pPdOpen (input/output)
 *       pointer to td open parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceOpen( PPD pPd, PPDOPEN pPdOpen )
{
    PPDTCP pPdTcp;
    USHORT  iPortNumber;
    char szHostString[ADDRESS_LENGTH];


    pPd->PdClass = PdNetwork;
    pPd->pProtocolName = (PCHAR)pProtocolName;

    /*
     *  Return size of header and parameters
     */
    pPdOpen->OutBufHeader  = 0;
    pPdOpen->OutBufTrailer = 0;
    pPdOpen->OutBufParam   = 0;

    /*
     *  Initialize TCP data structure
     */
    pPdTcp = malloc(sizeof(*pPdTcp));
    pPd->pPrivate = pPdTcp;
    memset( pPdTcp, 0, sizeof(PDTCP) );

    /*
     *  Allocate PD input buffer
     */
    pPdTcp->pInBuf = malloc( pPd->OutBufLength );
    if ( pPdTcp->pInBuf == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    TRACE((TC_TD,TT_API3, "TDTCPRO: Calling bGetPrivateProfileString"));

    /*
     *  Get application server hostname
     */
   bGetPrivateProfileString( pPdOpen->pIniSection,      // buffered section
                              INI_ADDRESS,
                              DEF_ADDRESS,
			     szHostString,
			     sizeof(szHostString));

    /*
     *  Get client name
     */
   bGetPrivateProfileString( pPdOpen->pIniSection,      // buffered section
                              INI_CLIENTNAME,
                              "",
			     pPdTcp->NameAddress.ClientName,
			     sizeof(pPdTcp->NameAddress.ClientName));

              
    GetTCPHostNamePort( (char *) szHostString, (char *) pPdTcp->NameAddress.Name, &iPortNumber);					

    if (iPortNumber == 0)       // get the Port Number from .ini file
	iPortNumber = bGetPrivateProfileInt( pPdOpen->pIniSection, 
					     INI_ICAPORTNUMBER, 
					     DEF_ICAPORTNUMBER);

    if (iPortNumber!= 0 ) gTCP_PortNumber = iPortNumber ;	
	
    TRACE((TC_TD,TT_API3, "TDTCPRO: HostName '%s', TCP Port Number %d", pPdTcp->NameAddress.Name, gTCP_PortNumber));
    return( CLIENT_STATUS_SUCCESS );

}


/*******************************************************************************
 *
 *  DeviceClose
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pPdClose (input/output)
 *       pointer to td close parameter block
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceClose( PPD pPd, PDLLCLOSE pPdClose )
{
    PPDTCP pPdTcp;

    /*
     *  Get pointer to TCP td
     */
    pPdTcp = (PPDTCP) pPd->pPrivate;

    if ( pPdTcp )
    {
        free( pPdTcp->pInBuf );
        free( pPdTcp );
    }
    pPd->pPrivate = NULL;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceInfo
 *
 *    This routine is called to get module information
 *
 * ENTRY:
 *    pPd (input)
 *       pointer to protocol driver data structure
 *    pPdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceInfo( PPD pPd, PDLLINFO pPdInfo )
{
    PTD_C2H pTdData;
    PMODULE_C2H pHeader;
    PPDTCP pPdTcp;
    struct sockaddr saddr;
    int alen = sizeof(saddr);

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pPdInfo->pBuffer;
    pHeader->VersionL = VERSION_CLIENTL_TDTCPMS;
    pHeader->VersionH = VERSION_CLIENTH_TDTCPMS;

    /*
     *  Initialize client address (ip address)
     */
    pTdData = (PTD_C2H) pPdInfo->pBuffer;
    pPdTcp = (PPDTCP) pPd->pPrivate;
    (void) getsockname( pPdTcp->CNum, &saddr, &alen );
    pTdData->ClientAddressFamily = AF_INET;
    memcpy( pTdData->ClientAddress, saddr.sa_data, sizeof(saddr.sa_data) );

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceConnect
 *
 *  Connect to Citrix Application Server
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceConnect( PPD pPd )
{
    USHORT Port;
    ULONG InetAddr;
    PPDTCP pPdTcp;
    int rc;
    char *s;

    /*
     *  Get pointer to TCP td
     */
    pPdTcp = (PPDTCP) pPd->pPrivate;
    pPdTcp->Connect = FALSE;

    /*
     *  Get application server address
     */
    if ( rc = NameToAddress( pPd, &pPdTcp->NameAddress ) )
        goto badcall;

    /*
     * Attempt to map the HOSTNAME to an internet address
     */

    Port = gTCP_PortNumber ;

//  InetAddr = *(ULONG *)pPdTcp->NameAddress.Address;
    s = pPdTcp->NameAddress.Address;
    InetAddr = s[0] | (s[1] << 8) | (s[2] << 16) | (s[3] << 24);
    

    TRACE((TC_TD,TT_API3, "PDTCP: InetAddr is 0x%lx Port is %d", InetAddr, Port));

    /*
     *  Attempt to connect to application server
     */
    if ( rc = Call( pPd, InetAddr, Port, &pPdTcp->CNum ) )
        goto badcall;
    pPdTcp->Connect = TRUE;

    return( CLIENT_STATUS_SUCCESS );

 /*=============================================================================
 ==   Error returns
 =============================================================================*/

    /*
     *  call failed
     *  get adapter status failed
     */
 badcall:
    return( SetLastError( pPd, rc ) );
}


/*******************************************************************************
 *
 *  DeviceDisconnect
 *
 *  disconnect from citrix application server
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceDisconnect( PPD pPd )
{
    PPDTCP pPdTcp;

    /*
     *  Get pointer to TCP td
     */
    pPdTcp = (PPDTCP) pPd->pPrivate;

    /*
     *  Hangup connection
     */
    if ( pPdTcp->Connect )
        (void) Hangup( pPd, pPdTcp->CNum );
    pPdTcp->Connect = FALSE;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceProcessInput
 *
 *  process network input data
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceProcessInput( PPD pPd )
{
    PPDTCP pPdTcp;
    int AmountRead;
    int rc;

    /*
     *  Get pointer to TCP td
     */
    pPdTcp = (PPDTCP) pPd->pPrivate;

    /*
     *  Process input data
     */
    if ( rc = Receive( pPd, pPdTcp->CNum, pPdTcp->pInBuf, pPd->OutBufLength, &AmountRead ) ) {
        if( rc != CLIENT_STATUS_NO_DATA )
            rc = SetLastError( pPd, rc );
    }

    /*
     *  If successful copy data
     */
    if ( rc == CLIENT_STATUS_SUCCESS && pPd->pProcessInputProc ) {
        LogPrintf( LOG_CLASS, LOG_RECEIVE, "RECEIVE: %u bytes", AmountRead );
        LogBuffer( LOG_CLASS, LOG_RECEIVE,
                    (PVOID)pPdTcp->pInBuf, (ULONG)AmountRead );
        if ( pPd->pReadHook )
            (*pPd->pReadHook)( pPdTcp->pInBuf, AmountRead );
        rc = (*pPd->pProcessInputProc)( pPd->pWdData, pPdTcp->pInBuf, AmountRead );
        ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    }

    ASSERT( rc == CLIENT_STATUS_SUCCESS || rc == CLIENT_STATUS_NO_DATA, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  DeviceWrite
 *
 *  write data to network  (returns before write is complete)
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *    pBytesWritten (output)
 *       address to return number of bytes actually written
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceWrite( PPD pPd, POUTBUF pOutBuf, PUSHORT pBytesWritten )
{
    PPDTCP pPdTcp;
    int rc;

    /*
     *  Get pointers
     */
    pPdTcp = (PPDTCP) pPd->pPrivate;

    /*
     *  Write data to network
     */
    rc = Send( pPd, pPdTcp->CNum, pOutBuf->pBuffer, pOutBuf->ByteCount, pBytesWritten );

    TRACE((TC_TD,TT_API3, "TcpSend: CNum %u, bc=%u, bs=%u, rc=0x%x", pPdTcp->CNum, pOutBuf->ByteCount, *pBytesWritten, rc ));

    if ( rc ) {
        if ( rc == EWOULDBLOCK )
            rc = CLIENT_STATUS_SUCCESS;
        rc = SetLastError( pPd, rc );
    }

    ASSERT( rc == CLIENT_STATUS_SUCCESS, rc );
    return( rc );
}


/*******************************************************************************
 *
 *  DeviceCheckWrite
 *
 *  check if previous write is complete
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceCheckWrite( PPD pPd, POUTBUF pOutBuf )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceCancelWrite
 *
 *  flush output - bit bucket all pending output data
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    pOutBuf (input)
 *       pointer to outbuf structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceCancelWrite( PPD pPd, POUTBUF pOutBuf )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  DeviceSendBreak  (async only)
 *
 *  transmit break signal
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceSendBreak( PPD pPd )
{
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  SetLastError
 *
 *  save TCP error code
 *
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to td data structure
 *    Error (input)
 *       netbios error code
 *
 * EXIT:
 *     NT error code
 *
 *
 ******************************************************************************/

int
SetLastError( PPD pPd, int Error )
{
    if ( Error == 0 )
        return( CLIENT_STATUS_SUCCESS );

    if ( Error > 0xFF )
        return( Error );

    if ( pPd->LastError == 0 )
        pPd->LastError = Error;

    ASSERT( FALSE, Error );
    return( CLIENT_ERROR_PD_ERROR );
}

