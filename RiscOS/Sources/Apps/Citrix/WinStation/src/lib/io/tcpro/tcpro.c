
/*************************************************************************
*
*   TCPRO.C
*
*   Microsoft TCP Library
*
*   Copyright Citrix Systems Inc. 1997
*
*   Author: $
*
*   $Log$
*  
*     Rev 1.2   13 Aug 1997 10:52:54   terryt
*  fix for browser address
*  
*     Rev 1.1   05 Aug 1997 17:12:22   scottn
*  Forgot to close macro.
*
*     Rev 1.0   04 Aug 1997 13:34:04   scottn
*  Initial revision.
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
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"
#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"

#include "citrix/icabrows.h"
#include "citrix/ibrowerr.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/biniapi.h"
#include "../../../dll/td/inc/td.h"

#include "sys/types.h"
#undef AF_IPX			// to prevent redefinition error - this can never be used anyway
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "sys/errno.h"
#include "sys/ioctl.h"

#include "socklib.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

int IoOpen( void );
void IoClose( void );
int IoRead( PICA_BR_ADDRESS, void *, int, int * );
int IoWrite( PICA_BR_ADDRESS, void *, int );
int IoBroadcast( void *, int );
int IoAddressCheck( char *, PICA_BR_ADDRESS, int );
int IoSameAddress( PICA_BR_ADDRESS, PICA_BR_ADDRESS );
int IoLocateNearestServer( PICA_BR_ADDRESS );
int IoLocateNextNearestServer( PICA_BR_ADDRESS );
void IoLocalAddress( PICA_BR_ADDRESS *, int * );


/*=============================================================================
==   Private Functions
=============================================================================*/
int closeTcpdrv(int);

/*=============================================================================
==   Functions used
=============================================================================*/

/*=============================================================================
==   Global Data
=============================================================================*/

extern BYTE            G_TcpBrowserAddress[];
extern BYTE            G_IpxBrowserAddress[];
extern BYTE            G_NetBiosBrowserAddress[];

extern ADDRESS         G_TcpBrowserAddrList[];
extern ADDRESS         G_IpxBrowserAddrList[];
extern ADDRESS         G_NetBiosBrowserAddrList[];

extern ICA_BR_ADDRESS  G_BrowserAddress;
extern ICA_BR_ADDRESS  G_BrowserAddrList[];
extern int             G_BrowserAddrListIndex;
extern int             G_fBrowserAddress;
extern int             G_fBrowserAddrList[];

extern PICA_BR_ADDRESS G_pLocalAddr;
extern int             G_LocalAddrCount;
extern int             G_LanaNumber;
extern USHORT          G_RequestRetry;
extern USHORT          G_ReadTimeout;

/*
 * #defines to make this winsock code work without winsock
 */
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define SOCKADDR_IN	struct sockaddr_in
#define INADDR_NONE     0xffffffff
typedef LPSTR		CHARPTR;

#define close_socket(a) socketclose(a)

/*=============================================================================
==   Local Data
=============================================================================*/

int hSocket = INVALID_SOCKET;
int TcpPort = 0;


/*******************************************************************************
 *
 *  IoOpen
 *
 *  Initialize TCP/IP i/o
 *
 *
 *  ENTRY:
 *     nothing
 *
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - open was sucessfully
 *    BR_ERROR_IO_ERROR - open failed
 *
 *
 ******************************************************************************/

int
IoOpen()
{
    SOCKADDR_IN LocalAddr;
    int lMode = 1;      // set to Non-Blocking mode
    int arg;
    int rc;
    int i;

    TRACE(( TC_PD, TT_API1, "TcpOpen: enter"));

    /*
     *  Open a DATAGRAM socket for UDP
     */
    hSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( hSocket < 0 ) {
        hSocket = INVALID_SOCKET;
        TRACE(( TC_PD, TT_ERROR, "TcpOpen: socket failed, rc=%u", errno ));
        return( BR_ERROR_TCP_NOT_AVAILABLE );
    }

    /*
     *  Bind to the socket
     */
    memset( &LocalAddr, 0, sizeof(SOCKADDR_IN) );
    LocalAddr.sin_family = AF_INET;
    rc = bind( hSocket,
               (struct sockaddr *) &LocalAddr,
               sizeof(SOCKADDR_IN) );

    if ( rc < 0 ) {
        TRACE(( TC_PD, TT_ERROR, "TcpOpen: bind failed, rc=%u", errno ));
        close_socket( hSocket );
        hSocket = INVALID_SOCKET;
        return( BR_ERROR_IO_ERROR );
    }

    /*
     *  Get TCP dynamic port number
     */
    arg = sizeof(LocalAddr);
    rc = getsockname( hSocket, (struct sockaddr *)&LocalAddr, &arg );
    if ( rc == SOCKET_ERROR ) {
        TRACE(( TC_PD, TT_ERROR, "TcpOpen: getsockname failed, rc=%u", errno ));
        close_socket( hSocket );
        hSocket = INVALID_SOCKET;
        return( BR_ERROR_IO_ERROR );
    }
    TcpPort = LocalAddr.sin_port;

    /*
     *  Set non-blocking
     */
    rc = socketioctl( hSocket, FIONBIO, (CHAR *)&lMode );
    if ( rc == SOCKET_ERROR ) {
        TRACE(( TC_PD, TT_ERROR, "TcpOpen: ioctl failed, rc=%u", errno ));
        LogPrintf( 0xffffffff, 0xffffffff, "TcpOpen: ioctl failed, rc=%u", errno );
        close_socket( hSocket );
        hSocket = INVALID_SOCKET;
        return( BR_ERROR_IO_ERROR );
    }

#if 0
    /*
     *  Allow Broadcasts
     */
    arg = 1;
    rc = setsockopt( hSocket, SOL_SOCKET, /*no broadcast bit available*/, (char *)&arg, sizeof(arg) );
    if ( rc ) {
        TRACE(( TC_PD, TT_ERROR, "TcpOpen: setsockopt(SO_BROADCAST) failed, rc=%u", errno ));
        close_socket( hSocket );
        return( BR_ERROR_IO_ERROR );
    }
#endif
    /*
     *  Get address of ICA Browser (optional)
     */
    memset( G_BrowserAddrList, 0, sizeof(ADDRESS)*MAX_BROWSERADDRESSLIST);
    if ( G_TcpBrowserAddress[0] ) {
        if ( !IoAddressCheck( G_TcpBrowserAddress, &G_BrowserAddress, FALSE ) ) {
            TRACE(( TC_PD, TT_ERROR, "TcpOpen: IoAddressCheck of '%s' failed", G_TcpBrowserAddress ));
            return(BR_ERROR_NO_MASTER);
        }
        memcpy( &G_BrowserAddrList[0], &G_BrowserAddress, sizeof(G_BrowserAddress));
        G_fBrowserAddrList[0] = TRUE;
        for(i=1;i<MAX_BROWSERADDRESSLIST;i++) {
            if(!IoAddressCheck( (char *)G_TcpBrowserAddrList[i], &G_BrowserAddrList[i], FALSE ))
                G_fBrowserAddrList[i] = FALSE;
            else
                G_fBrowserAddrList[i] = TRUE;
        }

        G_fBrowserAddress = TRUE;
    }

    /*
     *  Get local addresses
     */
    IoLocalAddress( &G_pLocalAddr, &G_LocalAddrCount );

    TRACE(( TC_PD, TT_API1, "TcpOpen: returning success"));
    TRACE(( TC_PD, TT_API3, "IoOpen: opened socket %u", hSocket));
    return( BR_ERROR_SUCCESS );
}


/*******************************************************************************
 *
 *  IoClose
 *
 *  close TCP/IO
 *
 *
 *  ENTRY:
 *     nothing
 *
 *
 *  EXIT:
 *    nothing
 *
 *
 ******************************************************************************/

void
IoClose()
{
    int rc;
    int i;
    TRACE(( TC_PD, TT_API1, "TcpClose: enter"));

    /*
     *  Free local address list
     */
    if ( G_pLocalAddr )
        free( G_pLocalAddr );

    /*
     *  Terminate connection and close socket
     */
    if ( hSocket != INVALID_SOCKET ) {
        TRACE((TC_PD, TT_API2, "IoClose: closing socket %u", hSocket));
        rc = close_socket( hSocket );
	if (rc != 0)
	   TRACE((TC_PD, TT_ERROR, "IoClose: close failed!! errno == %d", errno));
        hSocket = INVALID_SOCKET;

#if 0
	/*
	 * Since this is our only socket, close the TCPDRV$ file, if it's still open.
	 * Since we've removed the MS int handlers (by overriding _so_set_int_handlers
	 * in the ne or nr), the driver may not get closed when we're done, so we have
	 * to make sure, or else we lose file handle resources.
	 */
	for (i = 19; i > 0; i--) {
	   if (closeTcpdrv(i) == CLIENT_STATUS_SUCCESS)
	      break;
	}
#endif
    }
}


/*******************************************************************************
 *
 *  IoRead
 *
 *
 *  ENTRY:
 *     pAddress (output)
 *        pointer to buffer to return addresses
 *     pBuffer (output)
 *        pointer to buffer to return data
 *     ByteCount (input)
 *        length of buffer in bytes
 *     pBytesRead (output)
 *        address to return number of bytes read
 *
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - no error
 *    BR_ERROR_IO_ERROR - read failed
 *
 *
 ******************************************************************************/

int
IoRead( PICA_BR_ADDRESS pAddress,
        void * pBuffer,
        int ByteCount,
        int * pBytesRead )
{
    int rc;
    int AddrLen;

    TRACE(( TC_PD, TT_API1, "TcpRead: enter"));

    /*
     *  Read data
     */
    AddrLen = sizeof(SOCKADDR_IN);
//    AddrLen = sizeof(struct sockaddr);
    rc = recv( hSocket,
                   (CHARPTR)pBuffer,
                   ByteCount,
                   0);
/*    rc = recvfrom( hSocket,
                   pBuffer,
                   ByteCount,
                   0,
                   (struct sockaddr far *) &pAddress->Address,
                   &AddrLen );*/

    if ( rc == SOCKET_ERROR ) {
        rc = errno;
        if ( rc != EWOULDBLOCK ) {
            TRACE(( TC_PD, TT_ERROR, "TcpRead failed, rc=%u", rc ));
            return( BR_ERROR_IO_ERROR );
        }
        rc = 0;
    }

    /*
     *  Return read data
     */
    *pBytesRead = rc;
    return( BR_ERROR_SUCCESS );
}



/*******************************************************************************
 *
 *  IoWrite
 *
 *  Write data to remote client
 *
 *
 *  ENTRY:
 *     pAddress (input)
 *        pointer to address
 *     pBuffer (input)
 *        pointer to buffer to write
 *     ByteCount (input)
 *        length of buffer in bytes
 *
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - write completed successfully
 *
 *
 *
 ******************************************************************************/

int
IoWrite( PICA_BR_ADDRESS pAddress,
         void * pBuffer,
         int ByteCount )
{
    int rc;
    SOCKADDR_IN *pSockAddr = (SOCKADDR_IN *)pAddress->Address;

    TRACE(( TC_PD, TT_API1, "TcpWrite: enter with sin_port = 0x%04x, sin_addr = 0x%08lx",
	   pSockAddr->sin_port,
	   pSockAddr->sin_addr.s_addr ));

    /*
     *  Write data
     */
    rc = sendto( hSocket,
                 pBuffer,
                 ByteCount,
                 0,
                 (struct sockaddr *) &pAddress->Address,
                 sizeof(struct sockaddr) );

    if ( rc == SOCKET_ERROR ) {
	rc = errno;
        TRACE(( TC_PD, TT_ERROR, "TcpWrite: sendto failed, rc=%u", rc ));
        return( BR_ERROR_IO_ERROR );
    }

    return( BR_ERROR_SUCCESS );
}


/*******************************************************************************
 *
 *  IoBroadcast
 *
 *  Write broadcast data
 *
 *
 *  ENTRY:
 *     pBuffer (input)
 *        pointer to buffer to write
 *     ByteCount (input)
 *        length of buffer in bytes
 *
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - write completed successfully
 *
 *
 *
 ******************************************************************************/

int
IoBroadcast( void * pBuffer,
             int ByteCount )
{
    ICA_BR_ADDRESS Address;
    SOCKADDR_IN * pSockAddr;

    TRACE(( TC_PD, TT_API1, "TcpBroadcast: enter"));

    /*
     *  Initialize broadcast address
     */
    memset( &Address, 0, sizeof(Address) );

    pSockAddr = (SOCKADDR_IN *) Address.Address;
    pSockAddr->sin_family      = AF_INET;
    pSockAddr->sin_port        = htons(CITRIX_BR_UDP_PORT);
    pSockAddr->sin_addr.s_addr = INADDR_BROADCAST;

    /*
     *  Broadcast packet
     */
    return( IoWrite( &Address, pBuffer, ByteCount ) );
}


/*******************************************************************************
 *
 *  IoAddressCheck
 *
 *  check if name represents an IP address
 *
 *  ENTRY:
 *     pName (input)
 *        pointer to possible address string
 *     pAddress (output)
 *        address to return address
 *
 *
 *  EXIT:
 *    TRUE - name is an address
 *    FALSE - not an address
 *
 *
 ******************************************************************************/

int
IoAddressCheck( char * pName, PICA_BR_ADDRESS pAddress, int fNumOnly )
{
    char Name[ MAX_BR_FORMATTED_ADDRESS + 15 ];
    ULONG IPAddr;
    struct hostent * pHost;
    SOCKADDR_IN * pSockAddr;

    TRACE(( TC_PD, TT_API1, "TcpAddressCheck: enter"));

    if ( ( pName == NULL ) || ( !*pName ) )
        return( FALSE );

    /* skip leading spaces and convert to uppercase */
    TRACE(( TC_PD, TT_API1, "TcpAddressCheck: pName is >%s<", pName));
    while ( *pName && *pName == ' ' )
        pName++;
    strcpy( Name, pName );
    AnsiUpper( Name );
    TRACE(( TC_PD, TT_API1, "TcpAddressCheck: Name is >%s<", Name));

    pSockAddr = (SOCKADDR_IN *) pAddress->Address;
    memset( pSockAddr, 0, sizeof(SOCKADDR_IN) );
    pSockAddr->sin_family = AF_INET;
    pSockAddr->sin_port = htons(CITRIX_BR_UDP_PORT);

    if ( (IPAddr = inet_addr(Name)) != INADDR_NONE ) {
        pSockAddr->sin_addr.s_addr = IPAddr;
	TRACE(( TC_PD, TT_API1, "TcpAddressCheck: returning 0x%08lx", IPAddr));
        return(TRUE);
    }

    if ( !fNumOnly && (pHost = gethostbyname( Name )) ) {
        memcpy( &IPAddr, pHost->h_addr_list[0], sizeof(ULONG) );
        pSockAddr->sin_addr.s_addr = IPAddr;
	TRACE(( TC_PD, TT_API1, "TcpAddressCheck: returning 0x%08lx", IPAddr));
        return(TRUE);
    }

    TRACE(( TC_PD, TT_API1, "TcpAddressCheck: returning FALSE"));

    return( FALSE );
}


/*******************************************************************************
 *
 *  IoSameAddress
 *
 *  Compare two addresses
 *
 *  ENTRY:
 *     pAddress1 (input)
 *        pointer to first address
 *     pAddress2 (input)
 *        pointer to second address
 *
 *
 *  EXIT:
 *    TRUE  - same address
 *    FALSE - different addresses
 *
 *
 ******************************************************************************/

int
IoSameAddress( PICA_BR_ADDRESS pAddress1,
               PICA_BR_ADDRESS pAddress2 )
{
    SOCKADDR_IN * pSockAddr1;
    SOCKADDR_IN * pSockAddr2;

    pSockAddr1 = (SOCKADDR_IN *) pAddress1->Address;
    pSockAddr2 = (SOCKADDR_IN *) pAddress2->Address;

    return( pSockAddr1->sin_addr.s_addr == pSockAddr2->sin_addr.s_addr );
}


/*******************************************************************************
 *
 *  IoLocateNearestServer
 *
 *  return the address of the nearest WinFrame Server
 *
 *  ENTRY:
 *     pAddress (output)
 *         pointer to address to return server address
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - write completed successfully
 *
 *
 ******************************************************************************/

int
IoLocateNearestServer( PICA_BR_ADDRESS pAddress )
{
    if ( G_fBrowserAddress ) {
        *pAddress = G_BrowserAddress;
        return( BR_ERROR_SUCCESS );
    }

    return( BR_ERROR_NO_MASTER );
}


/*******************************************************************************
 *
 *  IoLocateNextNearestServer
 *
 *  return the address of the next nearest WinFrame Server
 *
 *  ENTRY:
 *     pAddress (output)
 *         pointer to address to return server address
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - write completed successfully
 *
 *
 ******************************************************************************/

int
IoLocateNextNearestServer( PICA_BR_ADDRESS pAddress )
{
#if 0
    if ( G_fBrowserAddress ) {
        if(G_BrowserAddrListIndex == MAX_BROWSERADDRESSLIST)
            return(BR_ERROR_NO_MASTER);

        G_BrowserAddrListIndex++;
        while((G_BrowserAddrListIndex < MAX_BROWSERADDRESSLIST ) &&
              (G_fBrowserAddrList[G_BrowserAddrListIndex]==FALSE)) {
            G_BrowserAddrListIndex++;
        }
        if(G_BrowserAddrListIndex == MAX_BROWSERADDRESSLIST)
            return(BR_ERROR_NO_MASTER);

        *pAddress = G_BrowserAddrList[G_BrowserAddrListIndex];

        return( BR_ERROR_SUCCESS );
    }
#endif
    return( BR_ERROR_NO_MASTER );
}


/*******************************************************************************
 *
 *  IoLocalAddress
 *
 *  return the addresses on the current machine
 *
 *  ENTRY:
 *     ppAddress (output)
 *         pointer to address of pointer to address
 *     pAddrCount (output)
 *         pointer to address to return count of addresses
 *
 *  EXIT:
 *    nothing
 *
 *
 ******************************************************************************/

void
IoLocalAddress( PICA_BR_ADDRESS * ppAddress,
                int * pAddrCount )
{
#if 0
//    char HostName[50];
    ULONG IPAddr;
    struct hostent * pHost;
    SOCKADDR_IN * pSockAddr;
    PICA_BR_ADDRESS pAddress;
    int AddrCount;
    int i;

    TRACE(( TC_PD, TT_API1, "TcpLocalAddress MS: enter"));

    /*
     *  Get host name of current computer
     *
    if ( gethostname( HostName, sizeof(HostName) ) == SOCKET_ERROR )
        goto noaddr;*/

    /*
     *  Get list of addresses for current computer
     *
    if ( (pHost = gethostbyname( HostName )) == NULL )
        goto noaddr;*/

    /*
     *  Get hostent
     */
    if ( (pHost = gethostent()) == NULL ) {
       TRACE(( TC_PD, TT_API1, "TcpLocalAddress MS: gethostent() failed" ));
       goto noaddr;
    }

    /*
     *  Count the number of addresses
     */
    AddrCount = 0;
    while ( pHost->h_addr_list[AddrCount] )
        AddrCount++;

    /*
     *  Make sure we have at least one address
     */
    if ( AddrCount == 0 )
        goto noaddr;

    /*
     *  Allocate memory for address list
     */
    if ( (pAddress = malloc( AddrCount * sizeof(ICA_BR_ADDRESS) )) == NULL )
        goto noaddr;

    memset( pAddress, 0, AddrCount * sizeof(ICA_BR_ADDRESS) );

    /*
     *  Copy addresses
     */
    for ( i=0; i < AddrCount; i++ ) {
        memcpy( &IPAddr, pHost->h_addr_list[i], sizeof(ULONG) );
        pSockAddr = (SOCKADDR_IN *) pAddress[i].Address;
        pSockAddr->sin_family      = AF_INET;
        pSockAddr->sin_port        = TcpPort;
        pSockAddr->sin_addr.s_addr = IPAddr;
        TRACE(( TC_PD, TT_API1, "TcpLocalAddress[%u] 0x%08lx %u", i, IPAddr, TcpPort ));
    }

    *ppAddress  = pAddress;
    *pAddrCount = AddrCount;

    return;

/*=============================================================================
==   Error returns
=============================================================================*/

    /*
     *  no addresses to return
     */
noaddr:
#endif
    *ppAddress = NULL;
    *pAddrCount = 0;
    TRACE(( TC_PD, TT_API1, "TcpLocalAddress MS: no addresses" ));
}
