
/*************************************************************************
*
* sock.c
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 09/16/94
*
* Log:
*
*
*
*************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <string.h>

/* Get the client includes */
#include "../../../inc/client.h"
#include "citrix/ica.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/clib.h"
#endif

#include "../../../inc/wdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/iniapi.h"
#include "../inc/td.h"
#include "tdtcpro.h"

#include "sys/types.h"
#undef AF_IPX			// to prevent redefinition error - this can never be used anyway
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "sys/errno.h"
#include "sys/ioctl.h"

#include "socklib.h"

/* ---------------------------------------------------------------------------- */

int  Call( PPD, ULONG, UINT, int * );
int  Send( PPD, int, char *, int, PUSHORT );
int  Receive( PPD, int, char *, int, int * );
void Hangup( PPD, int);
int  Check( PPD, int);

#define close_socket(a) socketclose(a)

/*******************************************************************************
 *
 * Call
 *
 *  attempt to connect to server
 *
 *
 * ENTRY:
 *     InetAddr (input)
 *        server address to call in NETWORK byte order
 *
 *     Port (input)
 *        Which port (service) to connect to
 *
 *     pSocket (output)
 *        address to return socket handle
 *
 * EXIT:
 *      CLIENT_STATUS_SUCCESS - succesful
 *
 *
 ******************************************************************************/

int Call( PPD pPd, ULONG InetAddr, UINT Port, int * pSocket )
{
   int rc;
   int sock;
   int arg;
   struct linger Linger;
   struct sockaddr_in addr;
   int fKeepAlive;

   memset( (char *)&addr, (int)0, sizeof( struct sockaddr ) );

   // construct our address
   addr.sin_family = AF_INET;
   addr.sin_port = htons( Port );
   addr.sin_addr.s_addr = InetAddr;

   TRACE((TC_TD,TT_API3, "Call: create socket"));

    // create a socket and bind it
   sock = socket( AF_INET, SOCK_STREAM, 0 );

   if( sock < 0 ) {
       // socket create error
       rc = errno;
       goto badconnect;
   }

   TRACE((TC_TD,TT_API3, "Call: connect"));

   rc = connect( sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in) );

   if( rc < 0 ) {
       // connect error
       rc = errno;
       close_socket( sock );
       goto badconnect;
   }

   fKeepAlive = 1;
   rc = setsockopt( sock, SOL_SOCKET, SO_KEEPALIVE, &fKeepAlive, sizeof(fKeepAlive) );

   if( rc ) {
       rc = errno;
       close_socket( sock );
       goto badconnect;
   }

   // set socket options
   Linger.l_onoff = 0;
   Linger.l_linger = 1;
   rc = setsockopt( sock, SOL_SOCKET, SO_LINGER, &Linger, sizeof(struct linger) );

   if( rc ) {
       rc = errno;
       close_socket( sock );
       goto badconnect;
   }

#ifdef HAS_NODELAY
   arg = 1;
   rc = setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, , sizeof(arg) );

   if( rc ) {
       rc = errno;
       close_socket( sock );
       goto badconnect;
   }
#endif

   // set non blocking mode

   TRACE((TC_TD,TT_API3, "Call: ioctl FIONBIO"));
   
   arg = 1;
   rc = socketioctl( sock, FIONBIO, (char *)&arg );

   TRACE((TC_TD,TT_API3, "Call: done"));

   if( rc ) {
       rc = errno;
       close_socket( sock );
       goto badconnect;
   }

   // Success
   *pSocket = sock;
   return( rc );

/*=============================================================================
==   Error returns
=============================================================================*/

badconnect:
   *pSocket = 0;
   return( rc );
}

/*******************************************************************************
 *
 * Send
 *
 *  send session message
 *
 *
 * ENTRY:
 *     Socket (input)
 *        socket handle
 *
 *     pBuffer (input)
 *        pointer to data
 *
 *     ByteCount (input)
 *        byte count of data
 *
 *     pBytesRead (output)
 *        number of bytes read from the connection
 *
 * EXIT:
 *      CLIENT_STATUS_SUCCESS - succesful
 *
 ******************************************************************************/

int Send( PPD pPd, int Socket, char * pBuffer, int ByteCount, PUSHORT pBytesRead )
{
   int rc;

   if( ByteCount == 0 ) {
       *pBytesRead = 0;
       return( CLIENT_STATUS_SUCCESS );
   }

   //
   // The  MSsockets do not support a socket option of
   // TCP_NODELAY, but this MSG_PUSH flag is in the header
   // socket.h, but not documented under Winsock.hlp.
   //
   rc = send( Socket, pBuffer, ByteCount, 0 /* MSG_PUSH */ );

   if( rc < 0 ) {
       *pBytesRead = 0;
       TRACE((TC_TD,TT_API3, "PDTCP: send rc=%d errno %d\n", rc, errno));
       if (errno == ECONNRESET) {       /* Connection reset by peer? */
           // ignore host initiated resets.
           // We will catch connection drops on the receive side.
           return( CLIENT_STATUS_SUCCESS );
       }
       rc = errno;
       ASSERT( rc == EWOULDBLOCK, rc );
   }
   else {
       *pBytesRead = rc;

       if( rc < ByteCount )
           rc = EWOULDBLOCK;
       else
           rc = CLIENT_STATUS_SUCCESS;
   }

   return(rc);
}

/*******************************************************************************
 *
 * Receive
 *
 *  receive session message
 *
 *
 * ENTRY:
 *     Socket (input)
 *        socket handle
 *     pBuffer (input)
 *        pointer to receive buffer
 *     pByteCount (input/output)
 *        byte count of buffer and actual bytes received
 *
 * EXIT:
 *      CLIENT_STATUS_SUCCESS - succesful
 *
 ******************************************************************************/

int Receive( PPD pPd, int Socket, char * pBuffer, int ByteCount, int *pAmountRead)
{
   int AmountRead;
   int rc;

   AmountRead = recv( Socket, pBuffer, ByteCount, 0 );

   /* error return */
   if ( AmountRead <= 0 ) {
      *pAmountRead = 0;
      rc = CLIENT_STATUS_NO_DATA;
   }
   else {
      *pAmountRead = AmountRead;
      rc = CLIENT_STATUS_SUCCESS;
   }

   return( rc );
}

/*******************************************************************************
 *
 * Hangup
 *
 *  terminate session
 *
 * ENTRY:
 *     Socket (input)
 *        TCPIP connection number to terminate
 *
 ******************************************************************************/

void Hangup( PPD pPd, int Socket )
{
    int rc;
    int i;

   /* terminate connection */

    rc = close_socket( Socket );

    ASSERT( rc == 0, rc );

    return;
}

