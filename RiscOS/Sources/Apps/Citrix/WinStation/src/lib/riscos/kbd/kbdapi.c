
/*************************************************************************
*
*  KBDAPI.C
*
*  Keyboard API routines.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (3/29/1994)
*
*  kbdapi.c,v
*  Revision 1.1  1998/01/12 11:37:34  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.25   15 Apr 1997 18:50:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.24   26 Jun 1996 15:23:28   brucef
*  Add DOS-specific API to manage user preferences for
*  keyboard behavior.
*  
*     Rev 1.23   17 Jan 1996 17:01:58   kurtp
*  update
*  
*     Rev 1.22   04 Nov 1995 15:36:20   andys
*  add check for kbdpush queue in readavail()
*  
*     Rev 1.21   21 Jul 1995 16:15:42   kurtp
*  update
*  
*     Rev 1.20   19 Jul 1995 12:16:16   kurtp
*  update
*  
*     Rev 1.19   19 Jul 1995 12:10:46   kurtp
*  update
*  
*     Rev 1.18   24 Jun 1995 17:10:46   butchd
*  update
*  
*************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../../inc/client.h"
#ifdef  DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"
#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/kbdapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../../../inc/wengapip.h"

#include "swis.h"

#include "KeyWatch.h"

/*=============================================================================
==   Local Functions Used
=============================================================================*/

int  KbdCheckHotkey( unsigned char, int );

/*=============================================================================
==   External Functions Used
=============================================================================*/

/*=============================================================================
==   External Data
=============================================================================*/

/*=============================================================================
==   Local Data
=============================================================================*/

#define TASK_HANDLE 0

typedef struct
{
    int flags;
    int scan_code;
    int key_code;
} key_event;

// keyboard hotkey structure
typedef struct _HOTKEY {
   unsigned char ScanCode;
   int ShiftState;
   int HotkeyId;
} HOTKEY, * PHOTKEY;

static KBDCLASS CurrentKbdMode = Kbd_Closed;
static HOTKEY   achHotkey[WFENG_NUM_HOTKEYS];

static void set_mode(void)
{
    if ( CurrentKbdMode == Kbd_Closed )
    {
	LOGERR(_swix(KeyWatch_DeregisterScan, _INR(0,1), 0, TASK_HANDLE));
    }
    else
    {
      // use task handle of 0 since it doesn't really matter
	LOGERR(_swix(KeyWatch_RegisterScan, _INR(0,1),
		    KeyWatch_RegisterScan_PS2 | KeyWatch_RegisterScan_CONSUME_BUFFER |
		    (CurrentKbdMode == Kbd_Ascii ? KeyWatch_RegisterScan_BUFFER_CODES : KeyWatch_RegisterScan_SCAN_CODES),
		    TASK_HANDLE));
    }
}

/*******************************************************************************
 *
 * KbdOpen
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdOpen(void)
{
   int i;

   TRACE((TC_KEY, TT_API1, "KbdOpen: mode %d", CurrentKbdMode));
   
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {

      // set current keyboard mode
      CurrentKbdMode = Kbd_Ascii;
      set_mode();
   }

   //  Initialize hotkeys to no-op
   for ( i=0; i < WFENG_NUM_HOTKEYS; i++ )
   {
      achHotkey[i].ScanCode   = 0;
      achHotkey[i].ShiftState = 0;
      achHotkey[i].HotkeyId   = CLIENT_STATUS_SUCCESS;
   }

   return( CLIENT_STATUS_SUCCESS );
}

int WFCAPI
KbdReopen(void)
{
   int i;

   TRACE((TC_KEY, TT_API1, "KbdReopen: mode %d", CurrentKbdMode));
   
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {

      // set current keyboard mode
      CurrentKbdMode = Kbd_Ascii;
      set_mode();
   }

   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdClose
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdClose(void)
{
   TRACE((TC_KEY, TT_API1, "KbdClose: mode %d", CurrentKbdMode));

   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // set current mode
   CurrentKbdMode = Kbd_Closed;
   set_mode();

   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdLoadPreferences
 *
 * ENTRY:
 *    * pPref
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdLoadPreferences( PKBDPREFERENCES pPref )
{
   int rc = CLIENT_STATUS_SUCCESS;

   TRACE((TC_KEY, TT_API1, "KbdOLoadPreferences: "));

   //gKbdPreferences = *pPref;  // Note: Structure assignment/copy

   return( rc );
}

/*******************************************************************************
 *
 * KbdGetMode
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdGetMode( KBDCLASS * pKbdClass )
{
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // set current keyboard mode
   *pKbdClass = CurrentKbdMode;

   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdSetMode
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdSetMode( KBDCLASS KbdClass )
{
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // set current mode
   if (CurrentKbdMode != KbdClass)
   {
       CurrentKbdMode = Kbd_Closed;
       set_mode();

       CurrentKbdMode = KbdClass;
       set_mode();
   }
   // ok
   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdReadAvail
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdReadAvail( int * pCountAvail )
{
    int n_entries;
    
    // init return vaule
    * pCountAvail = 0;

    // kbd open?
    if ( CurrentKbdMode == Kbd_Closed ) {
	TRACE((TC_KEY, TT_API1, "KbdReadAvail: kbd closed"));
        return( CLIENT_ERROR_NOT_OPEN );
    }

    LOGERR(_swix(KeyWatch_Poll, _INR(0,2) | _OUT(4),
	  0, TASK_HANDLE, NULL,
	  &n_entries));

    *pCountAvail = n_entries;
    
    DTRACE((TC_KEY, TT_API1, "KbdReadAvail: n %d", *pCountAvail));
    
    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdReadChar
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 *
 *  WARNING!  For KbdPush() things HotKeys dont get detected properly.
 *
 ******************************************************************************/

static int convert_shift_state(int flags)
{
    int state = 0;
    if (flags & KeyWatch_State_LEFT_SHIFT_DOWN)
	state |= KSS_LEFTSHIFT;
    if (flags & KeyWatch_State_RIGHT_SHIFT_DOWN)
	state |= KSS_RIGHTSHIFT;
    if (flags & KeyWatch_State_LEFT_CTRL_DOWN)
	state |= KSS_LEFTCTRL | KSS_EITHERCTRL;
    if (flags & KeyWatch_State_RIGHT_CTRL_DOWN)
	state |= KSS_RIGHTCTRL | KSS_EITHERCTRL;
    if (flags & KeyWatch_State_LEFT_ALT_DOWN)
	state |= KSS_LEFTALT | KSS_EITHERALT;
    if (flags & KeyWatch_State_RIGHT_ALT_DOWN)
	state |= KSS_RIGHTALT | KSS_EITHERALT;
    return state;
}

int WFCAPI
KbdReadChar( int * pChar, int *pShiftState )
{
    char buf[64];
    int n_entries;
    int Hotkey;
    
    // make sure we are in Ascii mode
    if ( CurrentKbdMode != Kbd_Ascii ) {
	TRACE((TC_KEY, TT_API1, "KbdReadChar: wrong mode"));
        return( CLIENT_ERROR_INVALID_MODE );
    }

    // get next event
    LOGERR(_swix(KeyWatch_Poll, _INR(0,4) | _OUT(4),
	  0, TASK_HANDLE, buf, sizeof(buf), 1,
	  &n_entries));

    if (n_entries == 1)
	return CLIENT_STATUS_NO_DATA;
    else
    {
	const key_event *k = (const key_event *)buf;
	int shift_state = convert_shift_state(k->flags);

	// check for hotkeys
	if ((k->flags & KeyWatch_Event_SCAN_CODE_VALID) &&
	    (Hotkey = KbdCheckHotkey( k->scan_code, shift_state )) != 0)
	{
	    LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );

	    return Hotkey;
	}

	if ((k->flags & KeyWatch_Event_KEY_CODE_VALID) == 0)
	    return CLIENT_STATUS_NO_DATA;

	*pChar = k->key_code;
	*pShiftState = shift_state;
    }

    LogPrintf( LOG_CLASS, LOG_KEYBOARD,
               "KEYBOARD: char (%02X)(%c) shift (%04X)",
               *pChar, *pChar, *pShiftState );

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdReadScan
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 *  WARNING!  For KbdPush() things HotKeys dont get detected properly.
 ******************************************************************************/

int WFCAPI
KbdReadScan( int * pScanCode, int * pShiftState )
{
    char buf[64];
    int n_entries;
    int Hotkey;
    
    // make sure we are in Ascii mode
    if ( CurrentKbdMode != Kbd_Scan ) {
	TRACE((TC_KEY, TT_API1, "KbdReadScan: wrong mode"));
        return( CLIENT_ERROR_INVALID_MODE );
    }

    // get next event
    LOGERR(_swix(KeyWatch_Poll, _INR(0,4) | _OUT(4),
	  0, TASK_HANDLE, buf, sizeof(buf), 1,
	  &n_entries));

    if (n_entries == 1)
	return CLIENT_STATUS_NO_DATA;
    else
    {
	const key_event *k = (const key_event *)buf;
	int shift_state = convert_shift_state(k->flags);

	if ((k->flags & KeyWatch_Event_SCAN_CODE_VALID) == 0)
	    return CLIENT_STATUS_NO_DATA;

	// check for hotkeys
	if ((k->flags & KeyWatch_Event_KEY_GOING_UP) == 0 &&
	    (Hotkey = KbdCheckHotkey( k->scan_code, shift_state )) != 0)
	{
	    LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );

	    return Hotkey;
	}

	*pScanCode = k->scan_code;
	*pShiftState = shift_state;
    }

    LogPrintf( LOG_CLASS, LOG_KEYBOARD,
               "KEYBOARD: scan (%02X) shift (%04X)",
               *pScanCode, *pScanCode, *pShiftState );

    return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdSetLeds
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

#define led_SCROLL_LOCK_ON	(1<<1)
#define led_NUM_LOCK_OFF	(1<<2)
#define led_CAPS_LOCK_OFF	(1<<4)

#define led_MASK		(led_SCROLL_LOCK_ON | led_NUM_LOCK_OFF | led_CAPS_LOCK_OFF)

int WFCAPI
KbdSetLeds( int ShiftState )
{
    int state = 0;

    TRACE((TC_KEY, TT_API1, "KbdSetLeds: %x", ShiftState));

    if (ShiftState & KSS_SCROLLLOCKON)
	state |= led_SCROLL_LOCK_ON;

    if ((ShiftState & KSS_NUMLOCKON) == 0)
	state |= led_NUM_LOCK_OFF;

    if ((ShiftState & KSS_CAPSLOCKON) == 0)
	state |= led_CAPS_LOCK_OFF;

    LOGERR(_swix(OS_Byte, _INR(0,2), 202, state, ~led_MASK));
    LOGERR(_swix(OS_Byte, _IN(0), 118));

    return( CLIENT_STATUS_SUCCESS );
}

int KbdGetLeds( void )
{
    int state;
    int ShiftState = 0;

    LOGERR(_swix(OS_Byte, _INR(0,2) | _OUT(1),
		 202, 0, 255,
		 &state));

    if (state & led_SCROLL_LOCK_ON)
	ShiftState |= KSS_SCROLLLOCKON;

    if ((state & led_NUM_LOCK_OFF) == 0)
	ShiftState |= KSS_NUMLOCKON;

    if ((state & led_CAPS_LOCK_OFF) == 0)
	ShiftState |= KSS_CAPSLOCKON;

    return ShiftState;
}

/*******************************************************************************
 *
 * KbdRegisterHotkey
 *
 * ENTRY:
 *    HotkeyID (input)
 *       hotkey id
 *    ScanCode (input)
 *       scan code
 *    ShiftState (input)
 *       key shift state
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdRegisterHotkey( int HotkeyId, int ScanCode, int ShiftState )
{
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // validate hotkey id
   if ( HotkeyId < CLIENT_STATUS_HOTKEY1 ||
        HotkeyId > (CLIENT_STATUS_HOTKEY1 + WFENG_NUM_HOTKEYS - 1 ) ) {
      return( CLIENT_ERROR_INVALID_MODE );
   }

   // save hotkey
   achHotkey[HotkeyId - CLIENT_STATUS_HOTKEY1].HotkeyId = HotkeyId;
   achHotkey[HotkeyId - CLIENT_STATUS_HOTKEY1].ScanCode = ScanCode;
   achHotkey[HotkeyId - CLIENT_STATUS_HOTKEY1].ShiftState = ShiftState;

   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdPush
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdPush( int ScanCode, int ShiftState, int Char )
{
   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdFlush
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdFlush( void )
{
#ifdef DOS
   // flush kbd buf
   KeyBdFlushInput();
#endif
   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdCheckHotkey
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
KbdCheckHotkey( unsigned char ScanCode, int ShiftState )
{
   int i;

   // check all hotkeys
   for ( i=0; i < WFENG_NUM_HOTKEYS; i++ ) {
      // check hotkey
      if ( (achHotkey[i].ScanCode   == ScanCode) &&
           (achHotkey[i].ShiftState ==
            ((((ShiftState & KSS_EITHERSHIFT) ? 
             (ShiftState | KSS_EITHERSHIFT) : ShiftState))
                & (KSS_EITHERSHIFT | KSS_EITHERALT | KSS_EITHERCTRL))) ) {
         return( achHotkey[i].HotkeyId );
      }
   }

   return( CLIENT_STATUS_SUCCESS );
}

