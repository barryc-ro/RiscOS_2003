
/***************************************************************************
*
*   PDMODEM.H
*
*   This module contains private PD defines and structures
*
*   Copyright 1994, Citrix Systems Inc.
*
*   Author: Kurt Perry
*
*   Log: See VLOG
*
*************************************************************************/

/*=============================================================================
==   Defines
=============================================================================*/

#define STATE_RESET                  0
#define STATE_INITIALIZE             1
#define STATE_WAIT_OK                2
#define STATE_WAIT_OK_FINAL          3
#define STATE_DIAL                   4
#define STATE_REDIAL                 5
#define STATE_WAIT_CONNECT           6
#define STATE_CONNECTED              7
#define STATE_NO_WAIT_REDIAL         8
#define STATE_NO_WAIT_REDIAL_BUSY    9
#define STATE_NO_RESPONSE           10
#define STATE_NO_DIALTONE           11
#define STATE_NO_CARRIER            12
#define STATE_VOICE                 13
#define STATE_BUSY                  14
#define STATE_IDLE                  15
#define STATE_TERMINATE             16
#define STATE_ERROR                 17
#define STATE_HUNGUP                18
#define STATE_FINISHED              19
#define STATE_LISTEN                20
#define STATE_NULL                  -1


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Modem PD structure
 */
typedef struct _PDMODEM {

    //  current state and flags
    int     iCurrentState;
    int     fFocus:1;
    int     fHungup:1;
    int     fCallback:1;
    int     fEchoTTY:1;

    char * pModemName;

    //  modem strings
    char *  pszModemRespBuffer;

    //  modem strings
    char *  pszInitString;
    char *  pszDialString;
    char *  pszHangString;
    char *  pszConnString;
    char *  pszAnswString;

    //  current modem string
    char *  pszCurrString;

    //  redial data
    unsigned        cRetryTimeout;
    unsigned        cTimeLeft;
    unsigned long   ulRetryTimeout;
    unsigned long   ulStartTime;
    unsigned long   ulStopTime;

} PDMODEM, * PPDMODEM;


/* SJM: rename local functions to prevent clashes */

#define DeviceOutBufAlloc	PdModemDeviceOutBufAlloc
#define DeviceOutBufError	PdModemDeviceOutBufError
#define DeviceOutBufFree	PdModemDeviceOutBufFree
#define DeviceSetInfo		PdModemDeviceSetInfo
#define DeviceQueryInfo		PdModemDeviceQueryInfo

#define DeviceProcessInput	PdModemDeviceProcessInput

#define DevicePoll		PdModemDevicePoll


