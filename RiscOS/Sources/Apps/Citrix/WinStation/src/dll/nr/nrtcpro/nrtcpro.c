
/*************************************************************************
*
*   NRTCPMS.C
*
*   Name Resolver for Microsoft TCP/IP
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Terry Treder (1/7/95)
*
*   Log:    see VLOG
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
#include "citrix/ica-c2h.h"

#include "../../../inc/clib.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/logapi.h"
#include "../inc/nr.h"

#include "sys/types.h"
#undef AF_IPX
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "sys/errno.h"

#define NO_NRDEVICE_DEFINES
#include "../../../inc/nrdevice.h"
#include "../../../inc/nrdevicep.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

static int DeviceNameToAddress( PNR, PNAMEADDRESS );

PLIBPROCEDURE NrTcpRODeviceProcedures[NRDEVICE__COUNT] =
{
    (PLIBPROCEDURE)DeviceNameToAddress
};

/*=============================================================================
==   Internal Functions
=============================================================================*/

unsigned long Inet_Addr( char * );

/*
 *   TCP error messages
 */
static LPBYTE pNrProtocolName = "TCP";

/*******************************************************************************
 *
 *  DeviceNameToAddress
 *
 *   DeviceNameToAddress converts an application server name to an address that
 *   can be used by the transport drvier
 *
 * ENTRY:
 *    pNr (input)
 *       pointer to name resolver data structure
 *    pNameAddress (input/output)
 *       pointer to the structure NAMEADDRESS
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
DeviceNameToAddress( PNR pNr, PNAMEADDRESS pNameAddress )
{
   int rc = CLIENT_STATUS_SUCCESS;
   long netaddr;
   unsigned char *pTmp;
   struct hostent *phe;
   struct sockaddr_in sa;

   memset( pNameAddress->Address, 0, sizeof(pNameAddress->Address) );

   pNr->pProtocolName = pNrProtocolName;

   netaddr = Inet_Addr( pNameAddress->Name );

   if ( netaddr == (-1L) ) {

      //
      // If its not a direct internet address, try the DNS
      //

      phe = gethostbyname( pNameAddress->Name );
      if ( phe == NULL ) {

         rc = ERROR_HOST_NOT_FOUND; // inetlib doesn't set errno here

         TRACE((TC_PD,TT_API1, "NRTCPMS: Could not lookup host :%s:",pNameAddress->Name));
	    goto error;
	}

        memcpy(&sa.sin_addr, phe->h_addr, phe->h_length);
        netaddr = sa.sin_addr.s_addr;
    }

   pTmp = (unsigned char *)&netaddr;
   TRACE(( TC_PD, TT_API2, "DeviceNameToAddress: %s [%u.%u.%u.%u] 0x%lx", pNameAddress->Name, pTmp[0],pTmp[1],pTmp[2],pTmp[3], netaddr));
   memcpy((char *)pNameAddress->Address,pTmp,4);

error:
   TRACE(( TC_PD, TT_API2, "DeviceNameToAddress: rc = %d", rc ));
   return rc;
}

/*******************************************************************************
 *
 *  Inet_Addr
 *
 *  Convert an ASCII dotted decimal internet address into
 *  binary.
 *
 *
 * ENTRY:
 *    Addr (input)
 *       Pointer to ASCII string representing address
 *
 * EXIT:
 *     Converted internet address
 *
 *
 ******************************************************************************/

unsigned long
Inet_Addr( char *addr )
{
    char *Buf, *ptr;
    int len;
    unsigned char c;
    unsigned long Ret = 0L;

    len = strlen( addr );

    if( len == 0 ) return( 0xFFFFFFFFL );

    Buf = strdup( addr );
    if( Buf == NULL ) return( 0xFFFFFFFFL );

    // Get the first entry

    ptr = strtok( Buf, "." );
    if( ptr == NULL ) {
        free( Buf );
	return( 0xFFFFFFFFL );
    }

    c = (unsigned char)atoi( ptr );
    Ret |= (ULONG)(((ULONG)c << 24L) & 0xFF000000L);

    // Get the second entry

    ptr = strtok( 0, "." );
    if( ptr == NULL ) {
        free( Buf );
	return( 0xFFFFFFFFL );
    }

    c = (unsigned char)atoi( ptr );
    Ret |= (ULONG)(((ULONG)c << 16L) & 0x00FF0000L);

    // Get the third entry

    ptr = strtok( 0, "." );
    if( ptr == NULL ) {
        free( Buf );
	return( 0xFFFFFFFFL );
    }

    c = (unsigned char)atoi( ptr );
    Ret |= (ULONG)(((ULONG)c << 8L) & 0x0000FF00L);

    // Get the last entry

    ptr = strtok( 0, "." );
    if( ptr == NULL ) {
        free( Buf );
	return( 0xFFFFFFFFL );
    }

    c = (unsigned char)atoi( ptr );
    Ret |= (ULONG)((ULONG)c  & 0x000000FFL);

    free( Buf );

    return( htonl(Ret) );
}

