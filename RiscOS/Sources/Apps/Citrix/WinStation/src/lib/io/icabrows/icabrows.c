
/*************************************************************************
*
*   ICABROWS.C
*
*   Common ICA Browser code
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Brad Pedersen (12/5/95)
*
*   $Log$
*  
*     Rev 1.12   30 Apr 1997 19:07:04   thanhl
*  update
*  
*     Rev 1.11   28 Apr 1997 19:19:28   thanhl
*  add BrPurgeInput support
*  
*     Rev 1.10   15 Apr 1997 18:52:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   10 Feb 1997 11:42:50   jeffm
*  multiple browser support
*  
*     Rev 1.8   08 Feb 1997 16:06:40   jeffm
*  multiple browser support
*  
*     Rev 1.7   04 Jul 1996 13:23:54   butchd
*  Only broadcast if no explicit browser address was specified
*  
*     Rev 1.6   20 Jan 1996 12:59:12   bradp
*  update
*  
*     Rev 1.5   15 Jan 1996 13:33:38   bradp
*  update
*  
*     Rev 1.4   03 Jan 1996 18:17:40   bradp
*  update
*  
*     Rev 1.3   22 Dec 1995 15:13:08   bradp
*  update
*  
*     Rev 1.2   20 Dec 1995 19:03:24   bradp
*  update
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
#include "citrix/icabrows.h"
#include "citrix/ibrowerr.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/logapi.h"



/*=============================================================================
==   External Functions Defined
=============================================================================*/

int BrRequestMasterBrowser( PICA_BR_ADDRESS );
int BrRead( int, PICA_BR_ADDRESS, void *, int, int * );
int BrWrite( PICA_BR_ADDRESS, void *, int );
int BrBroadcast( void *, int );
int BrPurgeInput( void );


/*=============================================================================
==   Internal Functions Defined
=============================================================================*/

int _ForceElection( void );


/*=============================================================================
==   Functions used
=============================================================================*/

int IoRead( PICA_BR_ADDRESS, void *, int, int * );
int IoWrite( PICA_BR_ADDRESS, void *, int );
int IoBroadcast( void *, int );
int IoSameAddress( PICA_BR_ADDRESS, PICA_BR_ADDRESS );
int IoLocateNearestServer( PICA_BR_ADDRESS );
int IoLocateNextNearestServer( PICA_BR_ADDRESS );


/*=============================================================================
==   Global Data
=============================================================================*/

extern BYTE            G_TcpBrowserAddress[];
extern BYTE            G_IpxBrowserAddress[];
extern BYTE            G_NetBiosBrowserAddress[];

extern ADDRESS         G_TcpBrowserAddrList[];
extern ADDRESS         G_IpxBrowserAddrList[];
extern ADDRESS         G_NetBiosBrowserAddrList[];

extern USHORT          G_RequestRetry;
extern USHORT          G_ReadTimeout;

// BYTE            G_TcpBrowserAddress[ ADDRESS_LENGTH+1 ];
// BYTE            G_IpxBrowserAddress[ ADDRESS_LENGTH+1 ];
// BYTE            G_NetBiosBrowserAddress[ ADDRESS_LENGTH+1 ];
// ADDRESS         G_TcpBrowserAddrList[MAX_BROWSERADDRESSLIST];     
// ADDRESS         G_IpxBrowserAddrList[MAX_BROWSERADDRESSLIST];     
// ADDRESS         G_NetBiosBrowserAddrList[MAX_BROWSERADDRESSLIST]; 
ICA_BR_ADDRESS  G_BrowserAddress = { 0 };
ICA_BR_ADDRESS  G_BrowserAddrList[MAX_BROWSERADDRESSLIST];
int             G_fBrowserAddress = FALSE;
int             G_fBrowserAddrList[MAX_BROWSERADDRESSLIST];
PICA_BR_ADDRESS G_pLocalAddr     = NULL;
int             G_LocalAddrCount = 0;
int             G_LanaNumber = 0;
// USHORT          G_RequestRetry;
// USHORT          G_ReadTimeout;
int             G_BrowserAddrListIndex = 0;


/*=============================================================================
==   Local Data
=============================================================================*/

ICA_BR_ADDRESS MasterAddress = { 0 };
int fMasterAddress = FALSE;


/*******************************************************************************
 *
 *  BrRequestMasterBrowser
 *
 *  get address of ICA master browser
 *
 *
 *  ENTRY:
 *     pAddress (output)
 *         address to return address of browser
 *
 *  EXIT:
 *     BR_ERROR_SUCCESS - no error
 *
 ******************************************************************************/

int
BrRequestMasterBrowser( PICA_BR_ADDRESS pAddress )
{
    ICA_BR_REQUEST_MASTER RequestMaster;
    ICA_BR_MASTER Master;
    ICA_BR_PING Ping;
    ICA_BR_ADDRESS Address;
    int fElection = FALSE;
    int fTryNearest = TRUE;
    int BytesRead;
    int i;
    int rc;

    if ( !fMasterAddress ) {

        for ( i=0; i < (int) G_RequestRetry; i++ ) {

            /*
             *  Initialize packet requesting master browser
             */
            TRACE(( TC_PD, TT_API1, "BR_REQUEST_MASTER" ));
            memset( &RequestMaster, 0, sizeof(ICA_BR_REQUEST_MASTER) );
            RequestMaster.Header.ByteCount = sizeof(ICA_BR_REQUEST_MASTER);
            RequestMaster.Header.Version   = 1;
            RequestMaster.Header.Command   = BR_REQUEST_MASTER;
            RequestMaster.Header.Signature = BR_SIGNATURE;
    
            /*
             *  Send either a directed write or a broadcast
             */
            if ( fTryNearest && IoLocateNearestServer( &Address ) == BR_ERROR_SUCCESS ) {

                RequestMaster.MasterReqFlags = 0;
                BrPurgeInput();
                rc = BrWrite( &Address, &RequestMaster, RequestMaster.Header.ByteCount );
                while( !rc && IoLocateNextNearestServer(&Address) == BR_ERROR_SUCCESS) {
                    rc = BrWrite( &Address, &RequestMaster, RequestMaster.Header.ByteCount );
                }

            } else if ( G_fBrowserAddress == FALSE ) {

                /*
                 * Only broadcast if no explicit browser address was specified.
                 */
                RequestMaster.MasterReqFlags = MASTERREQ_BROADCAST;
                BrPurgeInput();
                rc = BrBroadcast( &RequestMaster, RequestMaster.Header.ByteCount );
            }
            if ( rc )
                goto badwrite;

            /*
             *  Wait for Response
             */
            rc = BrRead( BR_MASTER, pAddress, &Master, sizeof(Master), &BytesRead );
            if ( rc == BR_ERROR_READ_TIMEOUT ) {
                fTryNearest = FALSE;
                continue;
            }
            if ( rc )
                goto badread;

            /*
             *  Get master address
             */
            Address = Master.Address;

            /*
             *  If master browser replied we are done
             */
            if ( IoSameAddress( pAddress, &Address ) ) 
                break;

            /*
             *  If non-master browser replied we need to ping the master to
             *  make sure it is still there
             */
            TRACE(( TC_PD, TT_API1, "BR_REQUEST_PING: (verify master)" ));
            memset( &Ping, 0, sizeof(Ping) );
            Ping.Header.ByteCount = sizeof(ICA_BR_PING);
            Ping.Header.Version   = 1;
            Ping.Header.Command   = BR_REQUEST_PING;
            Ping.Header.Signature = BR_SIGNATURE;
            Ping.DataLength = 0;
            Ping.oData = 0;
            BrPurgeInput();
            if ( rc = BrWrite( &Address, &Ping, Ping.Header.ByteCount ) )
                goto badwrite;
    
            /*
             *  Wait for Response
             */
            rc = BrRead( BR_PING, pAddress, &Ping, sizeof(Ping), &BytesRead );
            TRACE(( TC_PD, TT_API1, "BR_PING: (from master) rc=%u", rc ));
            if ( rc == BR_ERROR_SUCCESS ) 
                break;

            if ( rc == BR_ERROR_READ_TIMEOUT ) {

                /*
                 *  Check for Timeout
                 *  -- Send broadcast packet requesting election of new master
                 */
                if ( !fElection ) {
                    fElection = TRUE;
                    if ( rc = _ForceElection() )
                        goto badelection;
                    Delay( 10000 );
                }

            } else {

                /*
                 *  Check for read error
                 */
                goto badread;
            }
        }

        /*
         *  Check for timeout  (no master)
         */
        if ( i == (int)G_RequestRetry ) {
            rc = BR_ERROR_NO_MASTER;
            goto nomaster;
        }

        /*
         *  Save address for next time
         */
        MasterAddress  = Address;
        fMasterAddress = TRUE;
    }

    /*
     *  Return address of master browser
     */
    *pAddress = MasterAddress;

    return( BR_ERROR_SUCCESS );

/*=============================================================================
==   Error returns
=============================================================================*/

nomaster:
badread:
badelection:
badwrite:
    TRACE(( TC_PD, TT_ERROR, "BR_REQUEST_MASTER: failed, rc=%u", rc ));
    return( rc );
}


/*******************************************************************************
 *
 *  BrRead
 *
 *
 *  ENTRY:
 *     ExpectedCommand (input)
 *        expected command
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

#define DELAYTIME 10

int 
BrRead( int ExpectedCommand,
        PICA_BR_ADDRESS pAddress, 
        void * pBuffer, 
        int ByteCount, 
        int * pBytesRead )
{
    PICA_BR_HEADER pHeader;
    BYTE Buffer[ MAX_BR_BUFFER ];
    int MaxCount;
    int i;
    int rc;

    pHeader = (PICA_BR_HEADER) Buffer;

    for(;;) {

        /*
         *  Read data with timeout
         */
        MaxCount = (int) (G_ReadTimeout / DELAYTIME);
        for ( i=0; i < MaxCount; i++ ) { 

            if ( rc = IoRead( pAddress, Buffer, sizeof(Buffer), pBytesRead ) ) 
                return( rc );

            if ( *pBytesRead > 0 )
                break;

            Delay( DELAYTIME );
        }

        /*
         *  Check for read timeout
         */
        if ( i == MaxCount ) {
            TRACE(( TC_PD, TT_ERROR, "BrRead TIMEOUT" ));
            return( BR_ERROR_READ_TIMEOUT );
        }

        TRACE(( TC_PD, TT_API1, "BrRead: %u bytes, cmd 0x%x, expect 0x%x", 
                *pBytesRead, pHeader->Command, ExpectedCommand ));
        TRACEBUF(( TC_PD, TT_IRAW, Buffer, *pBytesRead ));

        /*
         *  Check header
         */
        if ( (pHeader->ByteCount >= sizeof(ICA_BR_HEADER)) &&
             ((int)pHeader->ByteCount == *pBytesRead) &&
             (pHeader->Signature == BR_SIGNATURE) ) {

            if ( pHeader->Command == BR_ERROR ) 
                return( (int) ((PICA_BR_ERROR)pHeader)->Error );

            if ( pHeader->Command == ExpectedCommand ) 
                break;
        }
    };

    /*
     *  Get optional source address
     */
    if ( pHeader->Address.Address[0] ) {
        memcpy( pAddress, pHeader->Address.Address, sizeof(ICA_BR_ADDRESS) );
    }

    /*
     *  Copy data
     */
    if ( ByteCount > *pBytesRead )
        ByteCount = *pBytesRead;
    memcpy( pBuffer, Buffer, ByteCount );

    return( BR_ERROR_SUCCESS );
}


/*******************************************************************************
 *
 *  BrWrite
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
BrWrite( PICA_BR_ADDRESS pAddress, 
         void * pBuffer, 
         int ByteCount )
{
    PICA_BR_HEADER pHeader;

    pHeader = (PICA_BR_HEADER) pBuffer;

    TRACE(( TC_PD, TT_API1, "BrWrite: %u bytes, cmd %u", ByteCount, pHeader->Command ));
    TRACEBUF(( TC_PD, TT_ORAW, pBuffer, ByteCount ));

    /*
     *  Initialize source address
     */
    if ( G_LocalAddrCount == 1 ) {
        memcpy( &pHeader->Address, G_pLocalAddr, sizeof(ICA_BR_ADDRESS) );
    }

    return( IoWrite( pAddress, pBuffer, ByteCount ) );
}


/*******************************************************************************
 *
 *  BrBroadcast
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
BrBroadcast( void * pBuffer, 
             int ByteCount )
{
    PICA_BR_HEADER pHeader;
    int i;
    int rc;

    TRACE(( TC_PD, TT_API1, "BrBroadcast: %u bytes", ByteCount ));
    TRACEBUF(( TC_PD, TT_ORAW, pBuffer, ByteCount ));

    if ( G_LocalAddrCount == 0 ) {
    
        /*
         *  Broadcast data
         */
        if ( rc = IoBroadcast( pBuffer, ByteCount ) ) 
            return( rc);

    } else {

        /*
         *  Send one packet for each local address
         */
        for ( i=0; i < G_LocalAddrCount; i++ ) {
    
            /*
             *  Initialize source address
             */
            pHeader = (PICA_BR_HEADER) pBuffer;
            memcpy( &pHeader->Address, &G_pLocalAddr[i], sizeof(ICA_BR_ADDRESS) );
    
            /*
             *  Broadcast data
             */
            if ( rc = IoBroadcast( pBuffer, ByteCount ) ) 
                return( rc );
        }
    }

    return( BR_ERROR_SUCCESS );
}


/*******************************************************************************
 *
 *  _ForceElection
 *
 *  Force an master browser election
 *
 *
 *  ENTRY:
 *     nothing
 *
 *  EXIT:
 *     BR_ERROR_SUCCESS - no error
 *
 ******************************************************************************/

int
_ForceElection()
{
    ICA_BR_ELECTION Election;

    /*
     *  Send broadcast packet requesting election of new master
     */
    memset( &Election, 0, sizeof(Election) );
    Election.Header.ByteCount = sizeof(Election);
    Election.Header.Version   = 1;
    Election.Header.Command   = BR_ELECTION;
    Election.Header.Signature = BR_SIGNATURE;
    return( BrBroadcast( &Election, Election.Header.ByteCount ) );
}



/*******************************************************************************
 *
 *  BrPurgeInput
 *
 *
 *  ENTRY:
 *
 *
 *  EXIT:
 *    BR_ERROR_SUCCESS - no error
 *    BR_ERROR_IO_ERROR - read failed
 *
 *
 ******************************************************************************/


int 
BrPurgeInput( void )
{
    BYTE Buffer[ MAX_BR_BUFFER ];
    ICA_BR_ADDRESS Address;
    int BytesRead;
    int rc;

    for(;;) {
        if ( rc = IoRead( &Address, Buffer, sizeof(Buffer), &BytesRead ) ) 
            return( rc );

        if ( BytesRead == 0 )
            break;
    }
    return( BR_ERROR_SUCCESS );
}
