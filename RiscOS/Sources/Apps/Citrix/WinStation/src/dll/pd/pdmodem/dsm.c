
/*************************************************************************
*
*  DSM.C
*
*  Modme Protocol Driver - dialing state machine
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Kurt Perry (7/8/94)
*
*  $Log$
*  
*     Rev 1.23   15 Apr 1997 16:52:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.23   21 Mar 1997 16:07:22   bradp
*  update
*  
*     Rev 1.22   22 May 1996 16:27:44   jeffm
*  update
*  
*     Rev 1.21   30 Jan 1996 18:39:46   bradp
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

#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/logapi.h"
#include "../inc/pd.h"

#include "pdmodem.h"


/*=============================================================================
==   Local Defines
=============================================================================*/

#define  DELAY_CALLBACK   1000L
#define  DELAY_ONE_SEC    1000L
#define  DELAY_TILDE       500L
#define  DELAY_DTR_PULSE   500L
#define  MAX_POLL           50          //  five seconds
#define  DELAY_IDLE_POLL   100L

#define  CR             0x0D
#define  LF             0x0A

#define  MWS_OK            "OK"

#define  MGR_DATA          0
#define  MGR_NODATA        1
#define  MGR_TIMEOUT       2

#define  BUFLEN          128
#define  INPUT_INCREMENT 128


/*=============================================================================
==   External procedures defined
=============================================================================*/


int  ModemOpen( PPDMODEM, PPDOPEN );
void ModemHangup( PPD, PPDMODEM );
int  ModemProcessInput( PPD, LPBYTE, USHORT );
int  ModemStateMachine( PPD, PPDMODEM, PDLLPOLL );

static int    WaitString( PPDMODEM, char *, int, int );
static int    GetModemOutput( PPDMODEM );
static char * SendString( PPD, PPDMODEM, int );
static int    PutChar( PPD, char );
static int    GetChar( PPDMODEM, char * );


/*=============================================================================
==   External procedures used
=============================================================================*/

extern int PdNext( PPD, USHORT, PVOID );
extern int OutBufAppend( PPD, POUTBUF, LPBYTE, USHORT );
extern int OutBufWrite( PPD );
extern int SetInfo( PPD, SETINFOCLASS, LPBYTE, USHORT );

extern int  GetModemStrings( PPDMODEM, PPDOPEN );
extern void DestroyModemString( PPDMODEM );

/*=============================================================================
==   Defines and structures
=============================================================================*/

typedef struct _MODEM_RESPONSE {
    char * pszString;
    int    iNextState;
} MODEM_RESPONSE, * PMODEM_RESPONSE;

/*=============================================================================
==   Local data
=============================================================================*/

//  input buffer things
static char   vachBuffer[BUFLEN];
static int    vcbBufLim;
static int    vcbBufLen;
static int    vcbInputChars   = 0;
static int    vcbInputSize    = 0;



//  modem resonses
MODEM_RESPONSE  vModemResponses[] = {

    { NULL,             STATE_NULL },
    { "ERROR",          STATE_ERROR },
    { "ABORTED",        STATE_ERROR },
    { "NO CARRIER",     STATE_NO_CARRIER },
    { "NO DIALTONE",    STATE_NO_DIALTONE },
    { "NO DIAL TONE",   STATE_NO_DIALTONE },
    { "BUSY",           STATE_BUSY },
    { "VOICE",          STATE_VOICE },

};
#define MAXRESPONSE (sizeof(vModemResponses)/sizeof(vModemResponses[0]))


/*******************************************************************************
 *
 *  ModemOpen
 *
 *  Initialize modem state machine and data structure
 *
 *  ENTRY:
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ModemOpen( PPDMODEM pPdModem, PPDOPEN pPdOpen )
{
    int rc;

    TRACE(( TC_MODEM, TT_API1, "PdModem: ModemOpen" ));

    /*
     *  Set starting state and focus
     */
    pPdModem->iCurrentState = STATE_RESET;
    pPdModem->fFocus        = TRUE;

    /*
     *  Get modem strings and timeouts
     */
    rc = GetModemStrings( pPdModem, pPdOpen );

    return( rc );
}


/*******************************************************************************
 *
 *  ModemCallback
 *
 *  Initialize modem state machine and data structure for callback
 *
 *  ENTRY:
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ModemCallback( PPD pPd )
{
    PPDMODEM pPdModem = pPd->pPrivate;

    TRACE(( TC_MODEM, TT_API1, "PdModem: ModemCallback" ));

    /*
     *  Set starting state and focus
     */
    pPdModem->iCurrentState = STATE_LISTEN;
    pPdModem->fFocus        = TRUE;

    /*
     *  Give ourselves a little time
     */
    pPdModem->ulStartTime   = Getmsec();
    pPdModem->ulStopTime    = pPdModem->ulStartTime + DELAY_CALLBACK;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  ModemHangup
 *
 *  Send modem hangup string
 *
 *  ENTRY:
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void
ModemHangup( PPD pPd, PPDMODEM pPdModem )
{
    PDSETINFORMATION PdSetInformation;

    TRACE(( TC_MODEM, TT_API1, "PdModem: ModemHangup" ));

    /*
     *  If not not hungup then hangup
     */
    if ( !pPdModem->fHungup ) {

        /*
         *  Drop DTR
         */
        PdSetInformation.PdInformationClass = PdDisconnect;
        PdNext( pPd, PD__SETINFORMATION, &PdSetInformation );

        /*
         *  Wait
         */
        Delay( DELAY_DTR_PULSE );

        /*
         *  Raise DTR
         */
        PdSetInformation.PdInformationClass = PdConnect;
        PdNext( pPd, PD__SETINFORMATION, &PdSetInformation );

        /*
         *  Hangup the connection.
         */
        pPdModem->pszCurrString = pPdModem->pszHangString;
        (void) SendString( pPd, pPdModem, TRUE );

        Delay( DELAY_ONE_SEC );

        /*
         *  Set hungup state
         */
        pPdModem->fHungup = TRUE;
    }
}


/*******************************************************************************
 *
 *  ModemProcessInput
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to Pd data structure
 *    pBuffer (input)
 *       Points to input buffer containing data.
 *    ByteCount (input)
 *       Number of bytes of data
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ModemProcessInput( PPD pPd, LPBYTE pBuffer, USHORT ByteCount )
{
    PPDMODEM pPdModem = pPd->pPrivate;

    TRACE(( TC_MODEM, TT_API1, "PdModem: Modem Input: bc %u", ByteCount ));

    /*
     *  if state is listen just ditch input
     */
    if ( pPdModem->iCurrentState == STATE_LISTEN )
        return( CLIENT_STATUS_NO_DATA );

    /*
     *  Save data in modem response buffer
     *  -- free the buffer in DestroyModemString on connection termination
     */
    while ( (vcbInputChars + (int) ByteCount) > vcbInputSize ) {
         vcbInputSize += INPUT_INCREMENT;
         pPdModem->pszModemRespBuffer = realloc( pPdModem->pszModemRespBuffer, vcbInputSize );
         if (pPdModem->pszModemRespBuffer == NULL) 
             return( CLIENT_ERROR_NO_MEMORY );
    }
    memcpy( &pPdModem->pszModemRespBuffer[vcbInputChars], pBuffer, ByteCount );
    vcbInputChars += ByteCount;

    return( CLIENT_STATUS_NO_DATA );
}


/******************************************************************************
 *
 *  SendString
 *
 *  String sending routine
 *
 *  ENTRY:
 *  pPd (input)
 *      Pointer to pd data structure
 *  pPdModem
 *      pointer to the structure PDMODEM
 *  fWait (input)
 *      wait after CRLF or not
 *
 *  EXIT:
 *
 *
 ******************************************************************************/

static char *
SendString( PPD pPd, PPDMODEM pPdModem, int fWait )
{
    int  i;
    int  ctrl=FALSE;
    int  cbString;
    int  cbWritten = 0;
    int  cPoll;
    char c;
    char * pszRetString = pPdModem->pszCurrString;


    /*
     * Send the string one char at a time.
     */
    cbString = (int) strlen( pPdModem->pszCurrString );
    for ( i=0; i < cbString; i++ ) {

        //  CRLF ?
        if ( ctrl ) {

            //  convert ^? to binary
            if ( *pszRetString != '^' && *pszRetString != '\\' )
                c = (char) (toupper(*pszRetString) - 'A' + 1);
            else
                c = *pszRetString;

            /*
             *  send character down the line, wait for buffer
             */
            cPoll = 0;
            while ( PutChar( pPd, c ) && cPoll++ < MAX_POLL )
                Delay( DELAY_IDLE_POLL );

            ++cbWritten;

            //  reset state
            ctrl = FALSE;

            //  Delay after a CR and break on !fWait
            if ( c == CR ) {

                //  delay
                Delay( DELAY_TILDE/2L );

                //  return on
                if ( !fWait ) {
                    ++pszRetString;
                    break;
                }
            }
        }
        else if ( *pszRetString == '^' || *pszRetString == '\\' ) {

            ctrl = TRUE;
        }
        else if ( *pszRetString == '~' ) {

            //  Write output buffer if anything was buffered
            if ( cbWritten ) {
                (void) OutBufWrite( pPd );
                cbWritten = 0;
            }

            Delay( DELAY_TILDE );
        }
        else {

            /*
             *  Send character, wait for buffer
             */
            cPoll = 0;
            while ( PutChar( pPd, *pszRetString ) && cPoll++ < MAX_POLL )
                Delay( DELAY_IDLE_POLL );

            ++cbWritten;
        }

        //  next char in string
        ++pszRetString;
    }

    //  Write output buffer if anything was buffered
    if ( cbWritten )
        (void) OutBufWrite( pPd );

    //  if nothing left then NULL
    return( strlen(pszRetString) ? pszRetString : NULL );
}


/******************************************************************************
 *
 *  GetModemOutput
 *
 *  Get the data returned from the modem
 *
 *  ENTRY:
 *  timeout
 *      command timeout value in seconds
 *
 *  EXIT:
 *
 *
 ******************************************************************************/

static int
GetModemOutput( PPDMODEM pPdModem )
{
   char * p;
   ULONG  t;
   int i;
   int rc = MGR_NODATA;
   unsigned char c;

   /*
    *  Shift left till first CR or LF
    */
   for ( i=0; i<vcbBufLim; i++ ) {
      if ( vachBuffer[i] == CR || vachBuffer[i] == LF ) {
         p = &vachBuffer[i];
         vcbBufLim -= (int) (++p - vachBuffer);
         memmove( vachBuffer, p, vcbBufLim );
         vachBuffer[vcbBufLim] = '\0';
         if ( vcbBufLim )
            rc = MGR_DATA;
         break;
      }
   }

   /*
    *  Need to make room for more chars
    */
   if ( vcbBufLim == vcbBufLen ) {
      i = vcbBufLen >> 2;
      vcbBufLim -= i;
      memmove( vachBuffer, &vachBuffer[i], vcbBufLen - i );
      vachBuffer[vcbBufLim] = '\0';
   }

   /*
    *  Read from input buffer
    */
   for (; ; ) {

      /*
       *  Get a character
       */
      if ( GetChar( pPdModem, (char *)&c ) == CLIENT_STATUS_NO_DATA )
         break;

      /*
       *  Got a character save it
       */
      vachBuffer[vcbBufLim++] = c;
      vachBuffer[vcbBufLim] = '\0';
      rc = MGR_DATA;

      /*
       *  Buffer full
       */
      if ( vcbBufLim >= vcbBufLen )
         break;

   }

   /*
    *  Check for timeout on no data condition
    */
   if ( rc == MGR_NODATA ) {
      t = Getmsec();
      if ( t > pPdModem->ulStopTime ) {
         rc = MGR_TIMEOUT;
      }
      if ( pPdModem->iCurrentState == STATE_WAIT_CONNECT ) {
          if ( rc == MGR_TIMEOUT ) {
              pPdModem->cTimeLeft = 0;
          }
          else {
              pPdModem->cTimeLeft = (USHORT) ((pPdModem->ulStopTime - t) / 1000L);
          }
      }
   }

   return( rc );
}


/*******************************************************************************
 *
 *  WaitString
 *
 *  Wait for a particular response from modem
 *
 *  ENTRY:
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
WaitString( PPDMODEM pPdModem, char * pszString, int iNextState, int iTimeoutState )
{
    int i;
    int rc = pPdModem->iCurrentState;

    /*
     *  Stuff incomming search string into response array[0]
     */
    vModemResponses[0].pszString = pszString;
    vModemResponses[0].iNextState = iNextState;

    /*
     *  Get modem output and do state transition
     */
    switch ( GetModemOutput( pPdModem ) ) {

    case MGR_DATA :

        /*
         *  Data available, check response array to see if we move on it
         */
        for ( i=0; i<MAXRESPONSE; i++ ) {

            if ( !strnicmp( vachBuffer, vModemResponses[i].pszString, strlen(vModemResponses[i].pszString) ) ) {
                rc = vModemResponses[i].iNextState;
                break;
            }
        }

        TRACE(( TC_MODEM, TT_API3, "PdModem: WaitString [%s](%u) vs [%s](%u), %s",
                vModemResponses[0].pszString, strlen(vModemResponses[0].pszString),
                vachBuffer, strlen(vachBuffer), 
                (i == MAXRESPONSE) ? "" : "MATCH" ));
    
        break;

    case MGR_TIMEOUT :

        rc = iTimeoutState;
        TRACE(( TC_MODEM, TT_API3, "PdModem: WaitString TIMEOUT" ));
        break;

    }

    return( rc );
}


/*******************************************************************************
 *
 *  PutChar
 *
 *  write a character
 *
 * ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    c (input)
 *       character to write
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
PutChar( PPD pPd, char c )
{
    int rc;

    /*
     *  Append output character
     */
    rc = OutBufAppend( pPd, NULL, &c, 1 );
    if ( rc == CLIENT_STATUS_SUCCESS && pPd->pOutBufCurrent )
        pPd->pOutBufCurrent->fControl = TRUE;

    return( rc );
}


/*******************************************************************************
 *
 *  GetChar
 *
 *  read a character
 *
 * ENTRY:
 *    s (output)
 *       pointer for read to
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static int
GetChar( PPDMODEM pPdModem, char * s )
{
    int rc = CLIENT_STATUS_NO_DATA;

    //  chars avail?
    if ( vcbInputChars ) {

        //  return current char
        *s = *pPdModem->pszModemRespBuffer;

        //  slide up
        memmove( pPdModem->pszModemRespBuffer,
                 &pPdModem->pszModemRespBuffer[1], --vcbInputChars );

        //  success
        rc = CLIENT_STATUS_SUCCESS;
    }

    return( rc );
}


/*******************************************************************************
 *
 *  ModemStateMachine
 *
 *  modem state machine
 *
 *  ENTRY:
 *    pPd (input)
 *       Pointer to pd data structure
 *    pPdPoll (input/output)
 *       pointer to the structure DLLPOLL
 *
 *  EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
ModemStateMachine( PPD pPd, PPDMODEM pPdModem, PDLLPOLL pPdPoll )
{
    int rc = CLIENT_STATUS_SUCCESS;

    /*
     *  State transititon switch
     */
    switch ( pPdModem->iCurrentState ) {

        case STATE_RESET :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_RESET" ));

            //  init current string and timeouts
            pPdModem->pszCurrString = pPdModem->pszInitString;
            pPdModem->ulStartTime   = Getmsec();
            pPdModem->ulStopTime    = pPdModem->ulStartTime + (10L * 1000L);

            //  set next state
            pPdModem->iCurrentState = STATE_INITIALIZE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_INIT;
            break;

        case STATE_LISTEN :

            TRACE(( TC_MODEM, TT_API3, "PdModem: STATE_LISTEN pre" ));

            //  wait for hangup
            if ( Getmsec() < (clock_t) pPdModem->ulStopTime )
                return( CLIENT_STATUS_NO_DATA );

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_LISTEN post" ));

            //  hangup
            ModemHangup( pPd, pPdModem );

            //  init buffer junk
            vcbBufLen     = BUFLEN;
            vcbBufLim     = 0;
            vcbInputChars = 0;

            //  start callback
            pPdModem->fCallback = TRUE;

            //  init current string
            pPdModem->pszCurrString = pPdModem->pszAnswString;

            //  send answer string
            SendString( pPd, pPdModem, TRUE );

            //  set next state and timeouts
            pPdModem->fHungup       = FALSE;
            pPdModem->iCurrentState = STATE_WAIT_CONNECT;
            pPdModem->ulStartTime   = Getmsec();
            pPdModem->ulStopTime    = pPdModem->ulStartTime + (pPdModem->ulRetryTimeout * 2);

            //  set return code
            rc = CLIENT_STATUS_MODEM_WAITING;
            break;

        case STATE_INITIALIZE :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_INITIALIZE" ));

            //  init buffer junk
            vcbBufLen     = BUFLEN;
            vcbBufLim     = 0;
            vcbInputChars = 0;

            //  send init string, if more string left then WAIT_OK not FINAL
            if ( (pPdModem->pszCurrString = SendString( pPd, pPdModem, FALSE )) == NULL )
                pPdModem->iCurrentState = STATE_WAIT_OK_FINAL;
            else
                pPdModem->iCurrentState = STATE_WAIT_OK;


            //  set return code
            rc = CLIENT_STATUS_MODEM_INIT;
            break;

        case STATE_WAIT_OK :

            TRACE(( TC_MODEM, TT_API4, "PdModem: STATE_WAIT_OK" ));

            //  get next state
            pPdModem->iCurrentState = WaitString( pPdModem, MWS_OK, STATE_INITIALIZE, STATE_NO_RESPONSE );

            //  set return code
            rc = CLIENT_STATUS_MODEM_INIT;
            break;

        case STATE_WAIT_OK_FINAL :

            TRACE(( TC_MODEM, TT_API4, "PdModem: STATE_WAIT_OK_FINAL" ));

            //  set next state by input
            pPdModem->iCurrentState = WaitString( pPdModem, MWS_OK, STATE_DIAL, STATE_NO_RESPONSE );

            //  set return code
            rc = (pPdModem->iCurrentState == STATE_DIAL) ? CLIENT_STATUS_MODEM_DIALING
                                                         : CLIENT_STATUS_MODEM_INIT;
            break;

        case STATE_DIAL :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_DIAL" ));

            //  init buffer junk
            vcbBufLen     = BUFLEN;
            vcbBufLim     = 0;
            vcbInputChars = 0;

            //  wait before dialing
            Delay( DELAY_ONE_SEC );

            //  init current string
            pPdModem->pszCurrString = pPdModem->pszDialString;

            //  send dialing string
            SendString( pPd, pPdModem, TRUE );

            //  set next state and timeouts
            pPdModem->fHungup       = FALSE;
            pPdModem->iCurrentState = STATE_WAIT_CONNECT;
            pPdModem->ulStartTime   = Getmsec();
            pPdModem->ulStopTime    = pPdModem->ulStartTime + pPdModem->ulRetryTimeout;

            //  set return code
            rc = CLIENT_STATUS_MODEM_WAITING;
            break;

        case STATE_REDIAL :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_REDIAL" ));

            //  hangup the connection
            ModemHangup( pPd, pPdModem );

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_RESET;

            //  set return code
            rc = CLIENT_STATUS_MODEM_REDIALING;
            break;

        case STATE_WAIT_CONNECT :

//          TRACE(( TC_MODEM, TT_API4, "PdModem: STATE_WAIT_CONNECT" ));

            //  fCallback?
            if ( pPdModem->fCallback ) {

                //  set next state by input
                pPdModem->iCurrentState = WaitString( pPdModem, pPdModem->pszConnString, STATE_CONNECTED, STATE_TERMINATE );

                //  only three valid states
                if ( !(pPdModem->iCurrentState == STATE_TERMINATE || pPdModem->iCurrentState == STATE_CONNECTED) ) {
                    pPdModem->iCurrentState = STATE_WAIT_CONNECT;
                }

                //  set return code
                rc = CLIENT_STATUS_NO_DATA;
            }
            else {

                //  set next state by input
                pPdModem->iCurrentState = WaitString( pPdModem, pPdModem->pszConnString, STATE_CONNECTED, STATE_NO_WAIT_REDIAL );

                //  set return code
                rc = CLIENT_STATUS_MODEM_WAITING;
            }
            break;

        case STATE_CONNECTED :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_CONNECTED" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_FINISHED;

            //  fCallback?
            if ( pPdModem->fCallback ) {

                //  notify wd of connected state
                SetInfo( pPd, CallbackComplete, NULL, 0 );

                //  no data
                rc = CLIENT_STATUS_NO_DATA;

            } else {

                TRACE(( TC_PD, TT_API1, "PdModem: Send CLIENT_STATUS_TTY_CONNECTED" ));
                rc = CLIENT_STATUS_TTY_CONNECTED;
            }
            break;

        case STATE_FINISHED :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_FINISHED" ));

            //  set focus and return code
            pPdModem->fFocus = FALSE;

            //  fCallback?
            if ( pPdModem->fCallback ) 
                pPdModem->fCallback = FALSE;
            else
                pPd->fSendStatusConnect = TRUE;

            //  set return code
            rc = CLIENT_STATUS_NO_DATA;
            break;

        case STATE_NO_WAIT_REDIAL :

            TRACE(( TC_MODEM, TT_API1, "PdModem: STATE_NO_WAIT_REDIAL" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_REDIAL;

            //  set return code
            rc = CLIENT_STATUS_MODEM_REDIALING;
            break;

        case STATE_NO_WAIT_REDIAL_BUSY :

            TRACE(( TC_MODEM, TT_API1, "PdModem: STATE_NO_WAIT_REDIAL_BUSY" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_REDIAL;

            //  set return code
            rc = CLIENT_STATUS_MODEM_BUSY;
            break;

        case STATE_NO_RESPONSE :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_NO_RESPONSE" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_IDLE;

            //  modem is still hung up (not responding)
            pPdModem->fHungup = TRUE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_NO_RESPONSE;
            break;

        case STATE_NO_DIALTONE :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_NO_DIALTONE" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_IDLE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_NO_DIAL_TONE;
            break;

        case STATE_NO_CARRIER :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_NO_CARRIER" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_NO_WAIT_REDIAL;

            //  set return code
            rc = CLIENT_STATUS_MODEM_REDIALING;
            break;

        case STATE_VOICE :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_VOICE" ));

            //  set next state
            pPdModem->iCurrentState = STATE_IDLE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_VOICE;
            break;

        case STATE_BUSY :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_BUSY" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_NO_WAIT_REDIAL_BUSY;

            //  set return code
            rc = CLIENT_STATUS_MODEM_BUSY;
            break;

        case STATE_IDLE :

            //  no next state just hang around
            Delay( DELAY_IDLE_POLL );

            //  set return code
            rc = CLIENT_STATUS_SUCCESS;
            break;

        case STATE_TERMINATE :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_TERMINATE" ));

            //  hangup the connection
            ModemHangup( pPd, pPdModem );

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_IDLE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_TERMINATE;
            break;

        case STATE_ERROR :

            TRACE(( TC_MODEM, TT_API2, "PdModem: STATE_ERROR" ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_IDLE;

            //  set return code
            rc = CLIENT_STATUS_MODEM_ERROR;
            break;

        default :

            TRACE(( TC_MODEM, TT_ERROR, "PdModem: invalid state %u", pPdModem->iCurrentState ));

            //  set next state unconditionally
            pPdModem->iCurrentState = STATE_ERROR;

            //  set return code
            rc = CLIENT_STATUS_MODEM_ERROR;
            break;
    }

    return( rc );
}
