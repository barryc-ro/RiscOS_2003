/*************************************************************************
*
*   wdica.c
*
*   ICA 3.0 WinStation driver
*
*   Copyright 1995, Citrix Systems Inc.
*
*   Author: Brad Pedersen (4/8/94)
*
*   wdica.c,v
*   Revision 1.1  1998/01/12 11:36:27  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
*  
*     Rev 1.107   11 Jun 1997 10:17:22   terryt
*  client double click support
*  
*     Rev 1.106   27 May 1997 18:50:16   terryt
*  client double click support
*  
*     Rev 1.105   30 Apr 1997 14:38:10   terryt
*  shift states again
*  
*     Rev 1.104   26 Apr 1997 23:35:20   kurtp
*  I fix a bug in this file, update
*  
*     Rev 1.103   21 Mar 1997 16:09:56   bradp
*  update
*  
*     Rev 1.102   04 Mar 1997 17:43:56   terryt
*  client shift states
*  
*     Rev 1.101   06 Feb 1997 10:19:52   richa
*  Put in the security check for a secure host and the internet client.
*  
*  
*     Rev 1.100   13 Jan 1997 16:39:36   kurtp
*  Persistent Cache
*  
*     Rev 1.99   19 Nov 1996 19:01:08   jeffm
*  don't free VC structure that we need
*  
*     Rev 1.98   13 Nov 1996 09:03:08   richa
*  Updated Virtual channel code.
*  
*     Rev 1.96   04 Nov 1996 09:46:52   richa
*  added open virtual channel structures.
*  
*     Rev 1.95   28 Aug 1996 15:09:26   marcb
*  update
*  
*     Rev 1.94   27 Apr 1996 15:56:18   andys
*  soft keyboard
*
*     Rev 1.93   29 Jan 1996 20:30:02   bradp
*  update
*
*     Rev 1.92   29 Jan 1996 20:12:24   bradp
*  update
*
*     Rev 1.91   29 Jan 1996 17:56:40   bradp
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
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#ifdef DOS
#include "../../../inc/dos.h"
#include "../../../inc/xmsapi.h"
#endif

#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/vioapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/wd.h"
#include "wdica.h"

#define NO_WDEMUL_DEFINES
#include "../inc/wdemul.h"
#include "../inc/wdemulp.h"

/*=============================================================================
==   Defines
=============================================================================*/

//  give the host 8 seconds to reply to stop xmit request
#define TIMEOUT_STOP_REQUEST    8000L


/*=============================================================================
==   External Functions Defined
=============================================================================*/

int EmulOpen( PWD, PWDOPEN );
int EmulClose( PWD );
int EmulInfo( PWD, PDLLINFO );
int EmulPoll( PWD, PDLLPOLL, int );
int EmulSetInformation( PWD, PWDSETINFORMATION );
int WFCAPI AppendVdHeader( PWD, USHORT, USHORT );
int VdCall( PWD, USHORT, USHORT, PVOID );
int DllCall( PDLLLINK, USHORT, PVOID );
int WdKbdSetMode( PWDICA, KBDCLASS );


/*=============================================================================
==   External Functions Used
=============================================================================*/

int WFCAPI OutBufReserve( PWD, USHORT );
int WFCAPI OutBufAppend( PWD, LPBYTE, USHORT );
int WFCAPI OutBufWrite( PWD );

int AppendICAPacket( PWD, USHORT, LPBYTE, USHORT );
int MouWrite( PWD, PMOUSEDATA, USHORT );
int IcaWrite( PWD, USHORT, LPBYTE, USHORT );
int SendNextInitResponse( PWD );
int IcaDetectWrite( PWD );
void IcaSetText( PWD, LPBYTE, USHORT );
int _GetValidTextModes( PWDTEXTMODE, USHORT, PUSHORT );

int KbdWrite( PWD, PUCHAR, USHORT );
void IcaSetLed( PWD, LPBYTE, USHORT );
void TerminateAck( PWD );

int WFCAPI WdICA30EmulProcessInput( PWD, LPBYTE, USHORT );
void WdICA30EmulWriteTTY( PWD, LPBYTE, USHORT );
int  WFCAPI WdICA30EmulOutBufAlloc( PWD, POUTBUF * );
void WFCAPI WdICA30EmulOutBufError( PWD, POUTBUF );
void WFCAPI WdICA30EmulOutBufFree( PWD, POUTBUF );
int  WFCAPI WdICA30EmulSetInfo( PWD, SETINFOCLASS, LPBYTE, USHORT );
int  WFCAPI WdICA30EmulQueryInfo( PWD, QUERYINFOCLASS, LPBYTE, USHORT );

/*******************************************************************************/

PLIBPROCEDURE WdICA30EmulProcedures[WDEMUL__COUNT] =
{
    (PLIBPROCEDURE)EmulOpen,
    (PLIBPROCEDURE)EmulClose,
    (PLIBPROCEDURE)EmulInfo,
    (PLIBPROCEDURE)WdICA30EmulProcessInput,
    (PLIBPROCEDURE)EmulPoll,
    (PLIBPROCEDURE)EmulSetInformation,
    (PLIBPROCEDURE)WdICA30EmulSetInfo,
    (PLIBPROCEDURE)WdICA30EmulQueryInfo,
    (PLIBPROCEDURE)WdICA30EmulOutBufAlloc,
    (PLIBPROCEDURE)WdICA30EmulOutBufError,
    (PLIBPROCEDURE)WdICA30EmulOutBufFree,
    (PLIBPROCEDURE)WdICA30EmulWriteTTY
};

/*=============================================================================
==   Local Data
=============================================================================*/

WDICA WdIcaData = {0};

UCHAR aShiftBreaks[] = { 0xb8,        //  Left  Alt
                         0xe0, 0xb8,  //  Right Alt
                         0x9d,        //  Left  Ctrl
                         0xe0, 0x9d,  //  Right ctrl
                         0xaa,        //  Left  Shift
                         0xb6,        //  Rigth Shift
};

#define MAX_SHIFT_BREAKS (sizeof(aShiftBreaks)/sizeof(aShiftBreaks[0]))

/*
 *  DefaultMode
 */
USHORT  DefaultMode = 0;

/*
 *  Product ID
 */
USHORT  ProductID = CLIENTID_CITRIX_DOS;


/*=============================================================================
==   Global Data
=============================================================================*/

extern WDTEXTMODE G_TextModes[];
extern PWD_VCBIND  paWD_VcBind;
extern USHORT iVc_Map;


/*******************************************************************************
 *
 *  EmulOpen
 *
 *  WdOpen calls EmulOpen
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdOpen (input/output)
 *       pointer to the structure WDOPEN
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
EmulOpen( PWD pWd, PWDOPEN pWdOpen )
{
    PWDICA pIca;

    /*
     *  Return the number of supported virtual channels
     */
    pWdOpen->MaxVirtualChannels = Virtual_Maximum;

    /*
     *  Initialize ICA data structure
     */
    pIca = &WdIcaData;
    pWd->pPrivate = pIca;
    memset( pIca, 0, sizeof(WDICA) );

    pIca->GlobalAttr = 0x07;
    pIca->TextIndex = 1;            // Text 80x25

    pIca->fEchoTTY = bGetPrivateProfileBool( pWdOpen->pIniSection,
                                             INI_ECHO_TTY,
                                             DEF_ECHO_TTY );

    pIca->ulTTYDelay = bGetPrivateProfileLong( pWdOpen->pIniSection,
                                               INI_ECHO_TTY_DELAY,
                                               DEF_ECHO_TTY_DELAY );

    /*
     *  Set screen dimensions in mouse library
     *  Let mouse driver determine screen dims (0,0)
     */
#ifdef DOS
    (void) MouseSetScreenDimensions(0,0);

    /*
     *  Check for mouse
     */
    if ( MouseReset() == CLIENT_STATUS_SUCCESS )
        pIca->fMouse = TRUE;

    if ( bGetPrivateProfileLong( pWdOpen->pIniSection,
                                 INI_MOUSEDOUBLECLICKTIMER,
                                 DEF_MOUSEDOUBLECLICKTIMER ) )
        pIca->fDoubleClickDetect = TRUE;
    else
        pIca->fDoubleClickDetect = FALSE;

    /*
     *  Get volume serial number of boot drive
     */
    _dos_getmediaid( _dos_bootdrive(), &pIca->SerialNumber );

#else
    /*
     *  BUGBUG: Mouse is always on in GUI
     */
    pIca->fMouse = TRUE;
#ifdef RISCOS
    pIca->fDoubleClickDetect = FALSE;
#else
    pIca->fDoubleClickDetect = TRUE;
#endif
#endif

    /*
     *  Put keyboard into default mode (ascii)
     */
    return( WdKbdSetMode( pIca, Kbd_Ascii ) );
}


/*******************************************************************************
 *
 *  EmulClose
 *
 *  WdClose calls EmulClose
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
EmulClose( PWD pWd )
{
    pWd->pPrivate = NULL;
    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  EmulInfo
 *
 *    This routine is called to get ICA 3.0 information
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to procotol driver data structure
 *    pWdInfo (output)
 *       pointer to the structure DLLINFO
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
EmulInfo( PWD pWd, PDLLINFO pWdInfo )
{
    PWD_C2H pWdData;
    PMODULE_C2H pHeader;
    USHORT ByteCount;
    USHORT VCByteCount = iVc_Map * sizeof_WD_VCBIND;
    PWDICA pIca;
#ifdef DOS
    struct dosdate_t dosdate;
#endif

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  Get byte count necessary to hold data
     */
    (void) _GetValidTextModes( NULL, 0, &ByteCount );
    ByteCount += sizeof(WD_C2H) + VCByteCount;

    /*
     *  Check if buffer is big enough
     */
    if ( pWdInfo->ByteCount < ByteCount ) {
        pWdInfo->ByteCount = ByteCount;
        return( CLIENT_ERROR_BUFFER_TOO_SMALL );
    }

    /*
     *  Initialize default data
     */
    pWdInfo->ByteCount = ByteCount;
    pWdData = (PWD_C2H) pWdInfo->pBuffer;
    memset( pWdData, 0, ByteCount );

    /*
     *  Initialize module header
     */
    pHeader = (PMODULE_C2H) pWdData;
    pHeader->ByteCount = ByteCount;
    pHeader->ModuleClass = Module_WinstationDriver;
    pHeader->VersionL = VERSION_CLIENTL_WD;
    pHeader->VersionH = VERSION_CLIENTH_WD;
    strcpy( pHeader->HostModuleName, "WDICA" );

    /*
     *  Initialize valid text modes
     */
    (void) _GetValidTextModes( (PWDTEXTMODE) (pWdInfo->pBuffer + sizeof(WD_C2H)),
                               (USHORT) (pWdInfo->ByteCount - (sizeof(WD_C2H) + VCByteCount)),
                               (PUSHORT) &ByteCount );

    pWdData->TextModeCount = ByteCount / sizeof(WDTEXTMODE);
    if ( pWdData->TextModeCount > 0 )
        pWdData->oTextMode = sizeof(WD_C2H);

    /*
     *  Set up vcbind data after the mode data, Older hosts will 
     *  ignore this data.
     */
    if ( iVc_Map ) {
        LPBYTE  pVcBindData;
	int i;

        pWdData->VcBindCount = iVc_Map;
        pWdData->oVcBind = sizeof(WD_C2H) + ByteCount;
        pVcBindData = (LPBYTE)pWdData + pWdData->oVcBind;

	/* need to copy item by item as the paWD_VcBind array isn't packed */
	//memcpy( pVcBindData, (LPBYTE)paWD_VcBind, iVc_Map * sizeof_WD_VCBIND) );
	for (i = 0; i < iVc_Map; i++, pVcBindData += sizeof_WD_VCBIND)
	    memcpy(pVcBindData, &paWD_VcBind[i], sizeof_WD_VCBIND );
        // We're done with array so free it/NULL it.
        // we get queries multiple times so we shouldn't free this
        // free( paWD_VcBind );
        // paWD_VcBind = NULL; //paranoia
    }


    pWdData->fEnableGraphics   = (pIca->pVdLink && pIca->pVdLink[Virtual_ThinWire]) ? TRUE : FALSE;
    pWdData->fEnableMouse      = pIca->fMouse;
    pWdData->fDoubleClickDetect = pIca->fDoubleClickDetect;
    pWdData->SerialNumber      = pIca->SerialNumber;
    pWdData->ICABufferLength   = pWd->InputBufferLength;
    pWdData->OutBufCountHost   = pWd->OutBufCountHost;
    pWdData->OutBufCountClient = pWd->OutBufCountClient;
    pWdData->OutBufDelayHost   = (USHORT) -1;
    pWdData->OutBufDelayClient = (USHORT) -1;
    pWdData->ClientProductId   = ProductID;

    TRACE(( TC_WD, TT_API1, "IcaInfo: pVdLink[TW] %p enable graphics %d", pIca->pVdLink[Virtual_ThinWire], pWdData->fEnableGraphics));
    
    /*
     *  Get the current date and time on the client computer
     *
     *  NOTE: dosdate_t is 5 bytes and CurrentDate is 4 bytes
     */

    {
	time_t t = time(NULL);
	struct tm *tm = gmtime(&t);
	pWdData->CurrentDate.day   = tm->tm_mday;
	pWdData->CurrentDate.month = tm->tm_mon + 1;
	pWdData->CurrentDate.year  = tm->tm_year - 10;

	pWdData->CurrentTime.hour    = tm->tm_hour;
	pWdData->CurrentTime.minute  = tm->tm_min;
	pWdData->CurrentTime.second  = tm->tm_sec;
	pWdData->CurrentTime.hsecond = 0;
    }

#ifdef DOS
    _dos_getdate( (struct dosdate_t *) &dosdate );
    pWdData->CurrentDate.day   = dosdate.day;
    pWdData->CurrentDate.month = dosdate.month;
    pWdData->CurrentDate.year  = dosdate.year;
    _dos_gettime( (struct dostime_t *) &pWdData->CurrentTime );
#endif

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  EmulPoll
 *
 *  WdPoll calls EmulPoll to process keyboard and mouse input
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdPoll (input)
 *       pointer to poll structure
 *    PdStatus (input)
 *       protocol driver status
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
EmulPoll( PWD pWd, PDLLPOLL pWdPoll, int PdStatus )
{
    PWDICA pIca;
    USHORT Channel;
    int BusyCount = 0;
    BYTE str[2];
    int rc;
#ifndef DOS
    PWDCachePac pCachePac;
#endif

    pIca = (PWDICA) pWd->pPrivate;

    /*
     *  Poll all loaded virtual drivers
     */
    for ( Channel = 0; Channel < Virtual_Maximum; Channel++ ) {
        if ( pIca->pVdLink && pIca->pVdLink[Channel] ) {
            if ( rc = VdCall( pWd, Channel, DLL__POLL, pWdPoll ) ) {
                if ( rc != CLIENT_STATUS_NO_DATA )
                    goto set_text_and_return;
            } else {
                BusyCount++;
            }
        }
    }

    /*
     *  Send init response packet (make connection to host)
     */
    if ( pIca->ModuleCount > 0 ) {
        BusyCount++;
        if ( rc = SendNextInitResponse( pWd ) )
            goto set_text_and_return;
    }

    /*
     *  If we are connected to host check local kbd buffer
     */
    if ( pWd->fFocus && (pWd->fTTYConnected || pWd->fConnected) ) {

        /*
         *  Check if buffered keyboard data is available
         */
        if ( pIca->pKbdBuffer && pIca->cbKbdBuffer ) {
            BusyCount++;
            rc = KbdWrite( pWd, (PUCHAR) NULL, 0 );
        }

        /*
         *  Check if buffered mouse data is available
         */
        if ( pIca->pMouBuffer && pIca->cbMouBuffer && pWd->fConnected ) {
            BusyCount++;
            rc = MouWrite( pWd, (PMOUSEDATA) NULL, 0 );
        }
    }

    /*
     *  If we are connected to winframe server
     */
    if ( pWd->fConnected ) {

        /*
         *  Reconnect to new appserver (load-balance)
         */
#ifdef future  // for future load balancing
        if ( pWd->pAppServer ) {
            rc = CLIENT_STATUS_RECONNECT_TO_HOST;
            goto set_text_and_return;
        }
#endif

        /*
         *  Send stop request packet
         */
        if ( pIca->fSendStopRequest ) {
            BusyCount++;
            rc = IcaWrite( pWd, PACKET_STOP_REQUEST, NULL, 0 );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "STOP_REQUEST" ));
                pIca->fSendStopRequest = FALSE;

                //  set timeout stop request to now plus N seconds
                pWd->TimeoutStopRequest = pWdPoll->CurrentTime + TIMEOUT_STOP_REQUEST;

            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {

                goto set_text_and_return;
            }
            else if ( pWd->TimeoutStopRequest == 0L ) {

                //  set timeout stop request to now plus N seconds
                pWd->TimeoutStopRequest = pWdPoll->CurrentTime + TIMEOUT_STOP_REQUEST;
            }
        }

        /*
         *  Send redisplay request packet
         */
        if ( pIca->fSendRedisplay ) {
            BusyCount++;
            rc = IcaWrite( pWd, PACKET_REDISPLAY, NULL, 0 );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "REDISPLAY" ));
                pIca->fSendRedisplay = FALSE;
            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                goto set_text_and_return;
            }
        }

        /*
         *  Send redraw request packet
         */
        if ( pIca->fSendRedraw ) {
            BusyCount++;
            rc = IcaWrite( pWd, PACKET_REDRAW, (LPBYTE)pIca->pRedraw,
                                               (USHORT)pIca->cbRedraw );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "REDRAW pRedraw(%08lX)", pIca->pRedraw ));
                pIca->fSendRedraw = FALSE;
                free( pIca->pRedraw );
            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                goto set_text_and_return;
            }
        }

        /*
         *  Send terminate packet
         */
        if ( pIca->fSendTerminate ) {
            BusyCount++;
            str[0] = (BYTE) 1; // byte count
            str[1] = (BYTE) TERMINATE_LOGOFF;
            rc = IcaWrite( pWd, PACKET_TERMINATE, str, 2 );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "SEND TERMINATE" ));
                pIca->fSendTerminate = FALSE;
            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                goto set_text_and_return;
            }
        }

        /*
         *  Send softkey
         */
        if ( pIca->fSendRaiseSoftkey || pIca->fSendLowerSoftkey ) {
            BusyCount++;
            str[0] = (BYTE) SOFTKEY_RAISE;
            if ( pIca->fSendLowerSoftkey )
                str[0] = (BYTE) 0;
            rc = IcaWrite( pWd, PACKET_SOFT_KEYBOARD, str, 1 );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "SEND SOFT KEYBOARD %c", str[0] ));
                pIca->fSendRaiseSoftkey = FALSE;
                pIca->fSendLowerSoftkey = FALSE;
            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                goto set_text_and_return;
            }
        }

#ifndef DOS
        /*
         *  Send Cache Command packet  //billg
         */
        if (pIca->fSendCacheCommand ) {

            while(pIca->CachePacCount > 0 ) {

                TRACE(( TC_WD, TT_API1, "Sending %x (count %u)", pIca->pCacheHeader, pIca->CachePacCount ));                  

                if (pIca->pCacheHeader != NULL ) {

                    // send out the first packa
                    pCachePac=pIca->pCacheHeader;
                  
                    rc=IcaWrite(pWd, PACKET_COMMAND_CACHE,
                          (LPBYTE)pCachePac->pCache,(USHORT)pCachePac->cbCache );                  

                    if ( rc == CLIENT_STATUS_SUCCESS ) {
                        TRACE(( TC_WD, TT_API1, "Send Cache Command success (bytes %u)", pCachePac->cbCache));                  
                        pIca->CachePacCount-- ;   
                        pIca->pCacheHeader=pCachePac->Next; 
                        free( pCachePac->pCache );                      
                    }
                    else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                        TRACE(( TC_WD, TT_API1, "Send Cache Commmand fails" ));                  
                        pIca->CachePacCount-- ;   
                        pIca->pCacheHeader=pCachePac->Next; 
                        free (pCachePac);
                        goto set_text_and_return ;
                    }
                    else {
                        TRACE(( TC_WD, TT_API1, "IcaWrite, no out bufs avail"));                  
                        break;
                    }
                    free (pCachePac);
                }

            }

            if ( pIca->CachePacCount == 0 ) {
                pIca->fSendCacheCommand = 0;
            }
        }
#endif

        /*
         *  Send terminate ack packet
         */
        if ( pIca->fSendTerminateAck ) {
            BusyCount++;
            str[0] = (BYTE) 1; // byte count
            str[1] = (BYTE) TERMINATE_ACK;
            rc = IcaWrite( pWd, PACKET_TERMINATE, str, 2 );
            if ( rc == CLIENT_STATUS_SUCCESS ) {
                TRACE(( TC_WD, TT_API1, "SEND TERMINATE ACK" ));
                pIca->fSendTerminateAck = FALSE;
                TerminateAck( pWd );
            } else if ( rc != CLIENT_ERROR_NO_OUTBUF ) {
                goto set_text_and_return;
            }
        }

        /*
         *  Check for secured host
         */
        if ( (pIca->fHostNotSecured) && (ProductID == CLIENTID_CITRIX_INTERNET) ) {
            return( CLIENT_ERROR_HOST_NOT_SECURED );
        }

    } else {

        /*
         *  Check ica detect timer
         */
        if ( pIca->TimerIcaDetect &&
             (pWdPoll->CurrentTime >= pIca->TimerIcaDetect) ) {

            /* send ica detection sequence to host */
            IcaDetectWrite( pWd );

            /* arm timer for 2 seconds */
            pIca->TimerIcaDetect = pWdPoll->CurrentTime + 2000;
        }

    }

    /*
     *  Received terminate packet from host
     */
    if ( pIca->fTerminate ) {
        rc = CLIENT_STATUS_CONNECTION_BROKEN_HOST;
#ifdef DOS
        if ( !pWd->fFocus )
            goto no_set_text;
#endif
        goto set_text_and_return;
    }

    /*
     *  Received stop ok packet from host
     */
    if ( pWd->fReceivedStopOk ) {
        pWd->fReceivedStopOk = FALSE;
        rc = CLIENT_STATUS_KILL_FOCUS;
        goto set_text_and_return;
    }

    /*
     *  Stop ok packet from host lost
     */
    if ( pWd->TimeoutStopRequest ) {
        if ( pWdPoll->CurrentTime > pWd->TimeoutStopRequest ) {
            pWd->TimeoutStopRequest = 0L;
            rc = CLIENT_STATUS_KILL_FOCUS;
            goto set_text_and_return;
        }
    }

    // last check for status's
    if ( pWd->fSendStatusBeeped ) {
        pWd->fSendStatusBeeped = FALSE;
        return( CLIENT_STATUS_BEEPED );
    }

    /*
     *  Normal return path
     */
    return( BusyCount > 0 ? CLIENT_STATUS_SUCCESS : CLIENT_STATUS_NO_DATA );

    /*
     *  Use this path for return codes that force UI foreground
     */
set_text_and_return:

    /*
     *  Do not set mode yet
     */
    if ( rc == CLIENT_STATUS_HOTKEY1 || rc == CLIENT_STATUS_HOTKEY2 || rc == CLIENT_STATUS_HOTKEY3 )
        return( rc );

    /*
     *  Set text mode
     */
#if defined(DOS) || defined(RISCOS)
    if (!DefaultMode) {
        IcaSetText( pWd, NULL, 0 );
    }

no_set_text:

#endif

    return( rc );
}


/*******************************************************************************
 *
 *  EmulSetInformation
 *
 *   Sets information for winstation driver
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    pWdSetInformation (input/output)
 *       pointer to the structure WDSETINFORMATION
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
EmulSetInformation( PWD pWd, PWDSETINFORMATION pWdSetInformation )
{
    PWDVDWRITEHOOK pWdVdWriteHook;
    PVDWRITEHOOK pVdWriteHook;
#ifndef DOS
    VDSETINFORMATION Info;
    PWDCachePac pCachePac, pCache;
#endif
    PWDICA pIca;
    PSCANCODE pSC;
    PCHARCODE pCC;
    PMOUSEINFO pMI;
    BYTE       bLED;
    int rc = CLIENT_STATUS_SUCCESS;

    DTRACE(( TC_UI, TT_API4, "EmulSetInformation: %d", pWdSetInformation->WdInformationClass ));

    pIca = (PWDICA) pWd->pPrivate;

    switch ( pWdSetInformation->WdInformationClass ) {

        /*
         *  Called after all virtual drivers have been loaded and opened
         */
        case WdVdAddress :

            /*
             *  Save addresses of the loaded virtual drivers
             */
            ASSERT( pWdSetInformation->WdInformationLength == (sizeof(PDLLLINK) * Virtual_Maximum), 0 );
            pIca->pVdLink = pWdSetInformation->pWdInformation;
            break;

        case WdDisconnect :
            pIca->fSendTerminate = TRUE;
            break;

        case WdKillFocus :

            /*  sync up the host shift states */
            if ( pWd->fConnected )
                KbdWrite( pWd, aShiftBreaks, MAX_SHIFT_BREAKS );

            /*  free any buffered keys */
            if ( pIca->pKbdBuffer ) {
                free( pIca->pKbdBuffer );
                pIca->pKbdBuffer      = NULL;
                pIca->cbKbdBuffer     = 0;
                pIca->cbKbdBufferSize = 0;
            }

            /*  free any buffered mouse */
            if ( pIca->pMouBuffer ) {
                free( pIca->pMouBuffer );
                pIca->pMouBuffer      = NULL;
                pIca->cbMouBuffer     = 0;
                pIca->cbMouBufferSize = 0;
            }

	    /* close down the keyboard support */
	    KbdClose();
	    
            if ( pWd->fConnected ) {
                pIca->fSendStopRequest = TRUE;
            } else {
                /* set a short timeout for TTY mode (not connected) */
                pWd->TimeoutStopRequest = Getmsec();
            }
            break;

        case WdSetFocus :

	    /* reopen the keyboard */
	    KbdReopen();
	    
            /*
             *  Restore saved kbd mode
             */
            (void) WdKbdSetMode( pIca, pIca->KbdMode );
            pIca->fSendRedisplay = TRUE;

            /*
             *  Reset stored LED states
             */
            bLED = (BYTE) pIca->ShiftState;
            IcaSetLed( pWd, &bLED, 1 );

            break;

        case WdWindowSwitch :

            /*
             *  Reset stored LED states (if neccessary)
             *
             *  Maybe this could have been done by a WdSetFocus, but we
             *  haven't been using that message for windows clients in this case.
             */
            bLED = (BYTE) pIca->ShiftState;
            IcaSetLed( pWd, &bLED, 1 );

            break;

        case WdRaiseSoftkey :
            if ( pWd->fConnected )
                pIca->fSendRaiseSoftkey = TRUE;
            else
                rc = CLIENT_ERROR_NOT_CONNECTED;
            break;

        case WdLowerSoftkey :
            if ( pWd->fConnected )
                pIca->fSendLowerSoftkey = TRUE;
            else
                rc = CLIENT_ERROR_NOT_CONNECTED;
            break;

        case WdSetProductID :
            ProductID = *(PUSHORT)(pWdSetInformation->pWdInformation);

            break;

        case WdSetDefaultMode :
            DefaultMode = *(PUSHORT)(pWdSetInformation->pWdInformation);

            break;

        case WdClientData :
            break;

        case WdVirtualWriteHook :

            pVdWriteHook = (PVDWRITEHOOK) pWdSetInformation->pWdInformation;
            ASSERT( pWdSetInformation->WdInformationLength == sizeof(VDWRITEHOOK), 0 );
            ASSERT( pVdWriteHook->Type < Virtual_Maximum, pVdWriteHook->Type );

            /*  register virtual write hook */
            pWdVdWriteHook = &pIca->VdWriteHook[ pVdWriteHook->Type ];
            pWdVdWriteHook->pData = pVdWriteHook->pVdData;
            pWdVdWriteHook->pProc = pVdWriteHook->pProc;

            /* return pointer to ica outbuf routines */
            pVdWriteHook->pWdData             = pWd;
            pVdWriteHook->pOutBufReserveProc  = (POUTBUFRESERVEPROC) OutBufReserve;
            pVdWriteHook->pOutBufAppendProc   = (POUTBUFAPPENDPROC) OutBufAppend;
            pVdWriteHook->pOutBufWriteProc    = (POUTBUFWRITEPROC) OutBufWrite;
            pVdWriteHook->pAppendVdHeaderProc = (PAPPENDVDHEADERPROC) AppendVdHeader;
            pVdWriteHook->MaximumWriteSize    = pWd->InputBufferLength;
            break;

        case WdCharCode :

            /*
             *  Filter out Chars till connected after ICA has been detected
             *
             *  WE MUST ALLOW CHARACTERS BEFORE ICA DETECTION!
             *
             *  The user must be allowed to type to get by PAD's and
             *  NetWare Connect (NASI) queries.
             *
             */
            if ( !pWd->fTTYConnected && !pWd->fConnected )
                break;

            /*
             *  Send ASCII keystrokes down the wire
             */
            if ( pIca->KbdMode == Kbd_Ascii ) {

                pCC = (PCHARCODE) pWdSetInformation->pWdInformation;
                rc = KbdWrite( pWd, (PUCHAR) pCC->pCharCodes, pCC->cCharCodes );
            }

            break;

        case WdScanCode :

            /*
             *  Filter out Scans till connected
             */
            if ( !pWd->fConnected )
                break;

            /*
             *  Send SCAN keystrokes down the wire
             */
            if ( pIca->KbdMode == Kbd_Scan ) {

#ifndef DOS
                if ( !pIca->fKbdShiftStateSet ) {

                    BYTE bLED = (BYTE) pIca->ShiftState;

                    IcaSetLed( pWd, &bLED, 1 );
                    pIca->fKbdShiftStateSet = TRUE;
                }
#endif

                pSC = (PSCANCODE) pWdSetInformation->pWdInformation;
                rc = KbdWrite( pWd, (PUCHAR) pSC->pScanCodes, pSC->cScanCodes );
            }

            break;

        case WdMouseInfo :

            /*
             *  Filter out Mouse till connected
             */
            if ( !pWd->fConnected )
                break;

            /*
             *  Send mouse info down the wire
             */
            pMI = (PMOUSEINFO) pWdSetInformation->pWdInformation;
            rc = MouWrite( pWd, (PMOUSEDATA)pMI->pMouseData, pMI->cMouseData );

            break;

#if 1 // !defined( DOS ) && !defined( RISCOS )
        case WdInitWindow :

            //  Get window handle
            pIca->hVio = (HWND)(ULONG) pWdSetInformation->pWdInformation;

            Info.VdInformationClass  = VdInitWindow;
            Info.pVdInformation      = pWdSetInformation->pWdInformation;
            Info.VdInformationLength = pWdSetInformation->WdInformationLength;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            if ( rc == CLIENT_ERROR_VD_NOT_FOUND ) {

                //  BUGBUG: Don't hardcode row/columns
//                rc = VioInitWindow( pIca->hVio, 25, 80, FALSE );
		rc = CLIENT_STATUS_SUCCESS;
            }
            break;

        case WdDestroyWindow :

            Info.VdInformationClass  = VdDestroyWindow;
            Info.pVdInformation      = pWdSetInformation->pWdInformation;
            Info.VdInformationLength = pWdSetInformation->WdInformationLength;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            if ( rc == CLIENT_ERROR_VD_NOT_FOUND ) {

//                rc = VioDestroyWindow( pIca->hVio );
		rc = CLIENT_STATUS_SUCCESS;
            }

            //  Destroy window handle
            pIca->hVio = NULL;
            break;

        case WdPaint :

            /*
             *  Handle text and graphics differently
             */
            if ( pIca->fGraphics ) {

                Info.VdInformationClass  = VdPaint;
                Info.pVdInformation      = pWdSetInformation->pWdInformation;
                Info.VdInformationLength = pWdSetInformation->WdInformationLength;
                rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
                if ( rc == CLIENT_ERROR_VD_NOT_FOUND ) {

		rc = CLIENT_STATUS_SUCCESS;
//                    rc = VioPaint( pIca->hVio );
                }
            }
            else {

//                rc = VioPaint( pIca->hVio );
            }
            break;

        case WdRedraw:

            if ( !pWd->fConnected ) {
                free( pIca->pRedraw ); // Discard update rectangle info
                break;
            }

            pIca->pRedraw  = (PWDREDRAW)pWdSetInformation->pWdInformation;
            pIca->cbRedraw = (USHORT)pWdSetInformation->WdInformationLength;
            if ( pWd->HostVersionH >= 3 ) {

               /*
                * Tell the host to redraw the specified update rectangles
                */
               pIca->fSendRedraw = TRUE;

            } else {
               WDSETINFORMATION WdSetInformation;

               /*
                * Redraw the entire display
                * For host version 1 we must first remove the focus.
                * For later hosts, the SetFocus alone is sufficient.
                */
               free( pIca->pRedraw ); // Discard update rectangle info
               if ( pWd->HostVersionH < 2 ) {
                   WdSetInformation.WdInformationClass = WdKillFocus;
                   EmulSetInformation( pWd, &WdSetInformation );
               }
               WdSetInformation.WdInformationClass = WdSetFocus;
               EmulSetInformation( pWd, &WdSetInformation );

            }

            /*
             *  We need this hack because focus is not set when
             *  the client gets focus, and the LEDs might have
             *  changed while we were gone.  In IcaSetLed we check
             *  that the LEDs have actually changed before we try
             *  and set them, no harm no foul.
             *
             *  Reset stored LED states
             */
            bLED = (BYTE) pIca->ShiftState;
            IcaSetLed( pWd, &bLED, 1 );
            break;

         case WdCache:   //billg

            if ( (!pWd->fConnected) || (pWd->HostVersionH < 3)) {
                free(pWdSetInformation->pWdInformation); // Discard cache information
                TRACE(( TC_WD, TT_API1, "wdica: Discard WdCache, no connected yet or no supported "));
                break;
            }

            if (!(pCachePac = (PWDCachePac) malloc( sizeof(WDCachePac)))) {
                TRACE(( TC_WD, TT_API1, "wdica: can not allocate memory" ));
                rc=CLIENT_ERROR_NO_MEMORY;
                free(pWdSetInformation->pWdInformation); 
                break;                  
            }            

            pCachePac->pCache = pWdSetInformation->pWdInformation;
            pCachePac->cbCache = ( USHORT )pWdSetInformation->WdInformationLength;
            pCachePac->Next = NULL;

            // put on cache packet queue
            if ( pIca->CachePacCount == 0 ) {  // first packet
                pIca->pCacheHeader = pCachePac;
            }
            else {
                pCache = pIca->pCacheHeader;
                while (pCache->Next != NULL) {  // put at the end of queue
                    pCache=pCache->Next;
                }
                pCache->Next=pCachePac;
            }
            pIca->CachePacCount++;

            /*
             * Tell the host to about cache information 
             */
            pIca->fSendCacheCommand = TRUE;
            TRACE(( TC_WD, TT_API1, "wdica: set fSendCacheCommand" ));
            break;

        case WdThinwireStack :
            Info.VdInformationClass  = VdThinwireStack;
            Info.pVdInformation      = pWdSetInformation->pWdInformation;
            Info.VdInformationLength = pWdSetInformation->WdInformationLength;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            break;

        case WdRealizePaletteFG :
            Info.VdInformationClass  = VdRealizePaletteFG;
            Info.pVdInformation      = pWdSetInformation->pWdInformation;
            Info.VdInformationLength = pWdSetInformation->WdInformationLength;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            break;

        case WdRealizePaletteBG :
            Info.VdInformationClass  = VdRealizePaletteBG;
            Info.pVdInformation      = pWdSetInformation->pWdInformation;
            Info.VdInformationLength = pWdSetInformation->WdInformationLength;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            break;

        case WdInactivate :
            Info.VdInformationClass  = VdInactivate;
            Info.pVdInformation      = NULL;
            Info.VdInformationLength = 0;
            rc = VdCall( pWd, Virtual_ThinWire, VD__SETINFORMATION, &Info );
            break;
#endif
    }

    return( rc );
}


/*******************************************************************************
 *
 *  AppendVdHeader
 *
 *   Append a virtual write header to the current output buffer
 *
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    Channel (input)
 *       virtual channel id
 *    ByteCount (input)
 *       number of bytes of data in buffer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
AppendVdHeader( PWD pWd, USHORT Channel, USHORT ByteCount )
{
    BYTE str[3];
    int rc;

    /*
     *  Adjust byte count to include virtual channel id
     */
    ByteCount++;

    /*
     *  Append virtual packet header
     */
    if ( ByteCount <= 2 ) {
        str[0] = (BYTE) Channel;
        rc = AppendICAPacket( pWd, PACKET_VIRTUAL_WRITE0, str, 1 );
    } else if ( ByteCount < 256 ) {
        str[0] = (BYTE) ByteCount;
        str[1] = (BYTE) Channel;
        rc = AppendICAPacket( pWd, PACKET_VIRTUAL_WRITE1, str, 2 );
    } else {
        str[0] = LSB(ByteCount);
        str[1] = MSB(ByteCount);
        str[2] = (BYTE) Channel;
        rc = AppendICAPacket( pWd, PACKET_VIRTUAL_WRITE2, str, 3 );
    }

    return( rc );
}


/*******************************************************************************
 *
 *  DllCall
 *
 *    DllCall is used to call an external procedure in a loaded dll.
 *
 * ENTRY:
 *    pLink (input)
 *       pointer to the structure DLLLINK
 *    ProcIndex (input)
 *       index of dll function to call
 *    pParam (input/output)
 *       pointer to parameter for function
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
DllCall( PDLLLINK pLink, USHORT ProcIndex, PVOID pParam )
{
    PDLLPROCEDURE pProcedure;

#ifdef DEBUG
    ASSERT( pLink->pProcedures, 0 );
    if ( ProcIndex >= pLink->ProcCount )
        return( CLIENT_ERROR_BAD_PROCINDEX );
#endif

    pProcedure = ((PDLLPROCEDURE *) pLink->pProcedures)[ ProcIndex ];
    ASSERT( pProcedure, 0 );

    return( (*pProcedure)( pLink->pData, pParam ) );
}


/*******************************************************************************
 *
 *  VdCall
 *
 *    VdCall is used to call the virtual driver
 *
 * ENTRY:
 *    pWd (input)
 *       pointer to winstation driver data structure
 *    VdChannel (input)
 *       virtual channel id
 *    ProcIndex (input)
 *       index of dll function to call
 *    pParam (input/output)
 *       pointer to parameter for function
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
VdCall( PWD pWd, USHORT VdChannel, USHORT ProcIndex, PVOID pParam )
{
    PDLLLINK pLink;
    PWDICA pIca;

    pIca = (PWDICA) pWd->pPrivate;

    ASSERT( VdChannel < Virtual_Maximum, VdChannel );
    pLink = pIca->pVdLink ? pIca->pVdLink[ VdChannel ] : NULL;

    if ( (pLink == NULL) || (pLink->pProcedures == NULL) )
        return( CLIENT_ERROR_VD_NOT_FOUND );

    return( DllCall( pLink, ProcIndex, pParam ) );
}


/*******************************************************************************
 *
 *  WdKbdSetMode
 *
 *  Sets the virtual kbd mode, for DOS also set the kbd device driver mode.
 *
 * ENTRY:
 *    pIca (input)
 *       pointer to winstation driver connection data structure
 *    KbdMode (input)
 *       kbd mode to set (ascii/scancode)
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
WdKbdSetMode( PWDICA pIca, KBDCLASS KbdMode )
{

    /*
     *  Save away in local data
     */
    pIca->KbdMode = KbdMode;

    /*
     *  For DOS really set the device driver mode
     */
    return( KbdSetMode( pIca->KbdMode ) );
}
