
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

/*=============================================================================
==   Local Functions Used
=============================================================================*/

int  KbdCheckHotkey( unsigned char, int );
//static void KbdGetFifoKey( int *, int *, int * );

/*=============================================================================
==   External Functions Used
=============================================================================*/

extern int KeyBdOpen( void );
extern int KeyBdClose( void );
extern int KeyBdFlushInput( void );
extern int KeyBdGetStatus( void );
extern int KeyBdRead( unsigned char *, int, int *, int *, int );

/*=============================================================================
==   External Data
=============================================================================*/

char RebootFlag = FALSE;
char CtrlCFlag = FALSE;
char CtrlBreakFlag = FALSE;

/*=============================================================================
==   Local Data
=============================================================================*/

// keyboard hotkey structure
typedef struct _HOTKEY {
   unsigned char ScanCode;
   int ShiftState;
   int HotkeyId;
} HOTKEY, * PHOTKEY;

// keyboard hook procedure structure
typedef struct _KBDHOOK {
   PLIBPROCEDURE pProcedure;
   struct _KBDHOOK * pNext;
} KBDHOOK, * PKBDHOOK;

static PKBDHOOK pKbdRootHook = NULL;

#ifdef DOS

#define KEYBOARD_FUNCTION 0x16
KBDPREFERENCES gKbdPreferences;

#endif
static KBDCLASS CurrentKbdMode = Kbd_Closed;
static HOTKEY   achHotkey[WFENG_NUM_HOTKEYS];

/*
 * KbdPushChar junk
 */
#define  FIFO_INIT_SIZE    150      // *must* be divisible by 3!!!!
static int * pFifo = NULL;
static int iFifo = 0;
static int cFifo = 0;

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
KbdOpen()
{
   int i;

   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {

      // set current keyboard mode
      CurrentKbdMode = Kbd_Ascii;

#ifdef DOS
      // open kbd
      KeyBdOpen();

      // make sure hook chain is empty
      pKbdRootHook = NULL;
#endif
   }

   //  Initialize hotkeys to no-op
   for ( i=0; i < WFENG_NUM_HOTKEYS; i++ ) {
      achHotkey[i].ScanCode   = 0;
      achHotkey[i].ShiftState = 0;
      achHotkey[i].HotkeyId   = CLIENT_STATUS_SUCCESS;
   }
   //
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
KbdClose()
{
#ifdef DOS
   PKBDHOOK pKbdHook;
   PKBDHOOK pTempHook;
#endif

   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // set current mode
   CurrentKbdMode = Kbd_Closed;

#ifdef DOS
   // close kbd
   KeyBdClose();

   // remove all hooks
   for ( pKbdHook=pKbdRootHook; pKbdHook != NULL; ) {

      // move to next before freeing
      pTempHook = pKbdHook;
      pKbdHook=pKbdHook->pNext;

      // free memory
      free( pTempHook );
   }

   // make sure hook chain is empty
   pKbdRootHook = NULL;

   // free fifo and clear counters
   if ( pFifo ) {
      free( pFifo );
      pFifo = NULL;
   }
   cFifo = 0;
   iFifo = 0;
#endif
   // ok
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
#ifdef DOS
   union REGS regs;

   gKbdPreferences = *pPref;  // Note: Structure assignment/copy
   if ( (pPref->KeyboardDelay != -1) && (pPref->KeyboardSpeed != -1 ) ) {
      
      regs.h.ah = 0x03;
      regs.h.al = 0x05;
      regs.h.bh = (UCHAR) pPref->KeyboardDelay;
      regs.h.bl = (UCHAR) pPref->KeyboardSpeed;
      TRACE( (TC_KEY, TT_API1,
              "KbdLoadPreferences: Setting Typematic Rate = %d, and Delay = %d",
              regs.h.bl, regs.h.bh) );
      int86( KEYBOARD_FUNCTION, &regs, &regs );
   } else {
      TRACE( (TC_KEY, TT_API1, "KbdLoadPreferences: No typematic changes") );
   }


#endif
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

#ifdef DOS
   // flush kbd buf
   KeyBdFlushInput();

    // set special key flags
    RebootFlag    = FALSE;
    CtrlCFlag     = FALSE;
    CtrlBreakFlag = FALSE;
#endif

   // set current mode
   CurrentKbdMode = KbdClass;
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
    // init return vaule
    * pCountAvail = 0;

    // kbd open?
    if ( CurrentKbdMode == Kbd_Closed ) {
        return( CLIENT_ERROR_NOT_OPEN );
    }

#if 0
    // is there a key in the fifo buffer?
    if ( iFifo ) {
        // ignore the counts in the real keyboard
        // this is to make the keystroke stuff work for UIs that
        // just do KbdPush
        *pCountAvail = iFifo/3;
        return( CLIENT_STATUS_SUCCESS );
    }
#endif
    
    // currently Ascii?
    if ( CurrentKbdMode == Kbd_Ascii ) {

        // check for special flags
        if ( RebootFlag || CtrlCFlag || CtrlBreakFlag ) {

            // return key count
            *pCountAvail = 1;
        }
        else {
	    int flags;

	    _swix(OS_Byte, _INR(0,1) | _FLAGS, 152, 0, &flags);
//	    _swix(OS_Byte, _INR(0,1) | _OUT(1), 128, 255, &count);

	    if (flags & _C)
               return( CLIENT_STATUS_NO_DATA );

            // return key count
            *pCountAvail = 1;
#if 0
            // check input status
            regs.h.ah = 0x0b;
            intdos( &regs, &regs );

            // key ready?
            if ( regs.h.al == 0 ) {
               return( CLIENT_STATUS_NO_DATA );
            }
            // return key count
            *pCountAvail = 1;
#endif
        }
    }

    // currently Scan??
    else if ( CurrentKbdMode == Kbd_Scan ) {

	int flags;
	
        // check for special flag
        if ( RebootFlag ) {
            RebootFlag = FALSE;
            return( CLIENT_STATUS_REBOOT );
        }

	_swix(OS_Byte, _INR(0,1) | _FLAGS, 152, 0, &flags);

	if (flags & _C)
	    return( CLIENT_STATUS_NO_DATA );

	// return key count
	*pCountAvail = 1;
#if 0
        // check for key, and return count
        else if ( (*pCountAvail = KeyBdGetStatus()) == 0 ) {
            return( CLIENT_STATUS_NO_DATA );
#endif
    }

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

int WFCAPI
KbdReadChar( int * pChar, int *pShiftState )
{
#ifdef DOS
    int count;
    int Hotkey;
    unsigned char ScanCode = 0;
    union REGS regs;
    LPBYTE pSHFLGS  = (LPBYTE) 0x00400017;
    LPBYTE pSHFLGS2 = (LPBYTE) 0x00400018;
    LPBYTE pK101FL  = (LPBYTE) 0x00400096;
    static LastScanCode = 0;
#endif
    PKBDHOOK pKbdHook;
    
    // make sure we are in Ascii mode
    if ( CurrentKbdMode != Kbd_Ascii ) {
        return( CLIENT_ERROR_INVALID_MODE );
    }

    //  init shift state
    *pShiftState = 0;

    // is there a special key waiting?
    if ( RebootFlag ) {
        RebootFlag = FALSE;
        LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: reboot" );
        return( CLIENT_STATUS_REBOOT );
    }
    else if ( CtrlBreakFlag ) {
        CtrlBreakFlag = FALSE;
        LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Ctrl+Break" );
        return( CLIENT_STATUS_CTRLBREAK );
    }
    else if ( CtrlCFlag ) {
        *pChar = 3;
        CtrlCFlag = FALSE;
        goto done;
    }

#if 0
    // is there a key in the fifo buffer?
    if ( iFifo ) {
        int iScanCode = (int) ScanCode;

        // retrieve key from fifo
        KbdGetFifoKey( &iScanCode, pShiftState, pChar );

        // done
        goto done;
    }
#endif

    {
	int key, flag, Hotkey;

	_swix(OS_Byte,
	      _INR(0,2) | _OUTR(1,2),
	      129, 0, 0,
	      &key, &flag);

	if (flag == 0xff)
	    return CLIENT_STATUS_NO_DATA;
	
	if (flag == 0)
	    *pChar = key;

	if (key == 0x1B)
	    return HOTKEY_UI;

        if ( (Hotkey = KbdCheckHotkey( key, 0 )) )
	{
            // return no char
            *pChar = 0;

            // return code
            LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );
            return( Hotkey );
	}
    }

#if 0
// get scan code for non-destructive read
    do {

        // get key if avail
        (void) KeyBdRead( &ScanCode, 1, &count, pShiftState, FALSE );

        // return shift state, cannot trust BIOS on Unisys Junk Boxes
        *pShiftState = (unsigned short) *pSHFLGS;
        *pShiftState |= ((unsigned short) (*pSHFLGS2 & 0xf3) << 8);
        *pShiftState |= ((unsigned short) (*pK101FL & 0x06) << 10);
    
        // is it a hotkey? if it is there steal key from DOS
        if ( count && (Hotkey = KbdCheckHotkey( ScanCode, *pShiftState )) ) {

            // return no char
            *pChar = 0;

            // steal char from DOS
steal:
            regs.h.ah = 0x0b;
            intdos( &regs, &regs );

            // key ready?
            if ( regs.h.al != 0 ) {
                regs.h.ah = 0x08;
                intdos( &regs, &regs );

                // if it is an extended key, steal rest of key sequence
                if ( regs.h.al == 0 ) {
                    goto steal;
                }
            }

            // return code
            LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );
            return( Hotkey );
        }

        //  detect atomic alt key make/break (MENU KEY)
        else if ( count && ScanCode == 0xb8 && LastScanCode == 0x38 ) {

            LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Menu key" );
            return( CLIENT_STATUS_MENUKEY );
        }

        //  save last scan code for alt reporting
        if ( count && ScanCode != 0xE0 && ScanCode != 0x00 )
            LastScanCode = ScanCode;

    } while ( count );      // until no keys

    // check DOS input status
    regs.h.ah = 0x0b;
    intdos( &regs, &regs );

    // key ready?
    if ( regs.h.al == 0 ) {
        return( CLIENT_STATUS_NO_DATA );
    }

    // return shift state
    *pShiftState = (unsigned short) *pSHFLGS;
    *pShiftState |= ((unsigned short) (*pSHFLGS2 & 0xf3) << 8);
    *pShiftState |= ((unsigned short) (*pK101FL & 0x06) << 10);

    // read key
    regs.h.ah = 0x08;
    intdos( &regs, &regs );

    // return key
    *pChar = (unsigned int) regs.h.al;
#endif

    
    // call hook routines
    for ( pKbdHook=pKbdRootHook; pKbdHook != NULL; pKbdHook=pKbdHook->pNext ) {
        (pKbdHook->pProcedure)( 0, 0, *pChar );
    }
done:
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
    int Char;
    int count;
    int Hotkey;
    int ShiftState;
    unsigned char ScanCode;
    PKBDHOOK pKbdHook;
#ifdef DOS
    LPBYTE pSHFLGS  = (LPBYTE) 0x00400017;
    LPBYTE pSHFLGS2 = (LPBYTE) 0x00400018;
    LPBYTE pK101FL  = (LPBYTE) 0x00400096;
#endif
    // make sure we are in Scan mode
    if ( CurrentKbdMode != Kbd_Scan ) {
        return( CLIENT_ERROR_INVALID_MODE );
    }

    RebootFlag = FALSE;

#if 0
    // is there a key in the fifo buffer?
    if ( iFifo ) {

        // retrieve key from fifo
        KbdGetFifoKey( pScanCode, pShiftState, &Char );

        // done
        goto done;
    }
#endif
    
    ScanCode = 0;
    ShiftState = 0;
	
    {
	int key, flag;

	_swix(OS_Byte,
	      _INR(0,2) | _OUTR(1,2),
	      129, 0, 0,
	      &key, &flag);

	if (flag == 0xff)
	    return CLIENT_STATUS_NO_DATA;
	
	if (flag == 0)
	    ScanCode = key;

	if (key == 0x1B)
	    return HOTKEY_UI;

        if ( (Hotkey = KbdCheckHotkey( ScanCode, *pShiftState )) )
	{
            // return no char
            *pScanCode = 0;

            // return code
            LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );
            return( Hotkey );
	}
    }

#if 0
    // check for key
    if ( !KeyBdGetStatus() ) {
        return( CLIENT_STATUS_NO_DATA );
    }

    // get key
    (void) KeyBdRead( &ScanCode, 1, &count, &ShiftState, TRUE );

    // return shift state, cannot trust BIOS on Unisys Junk Boxes
    ShiftState = (unsigned short) *pSHFLGS;
    ShiftState |= ((unsigned short) (*pSHFLGS2 & 0xf3) << 8);
    ShiftState |= ((unsigned short) (*pK101FL & 0x06) << 10);
#endif

    // check hotkey
    if ( (Hotkey = KbdCheckHotkey( ScanCode, ShiftState )) ) {
        LogPrintf( LOG_CLASS, LOG_KEYBOARD, "KEYBOARD: Hotkey (%02X)", Hotkey );
        return( Hotkey );
    }

    // return key and shift
    *pScanCode   = (unsigned int) ScanCode;
    *pShiftState = ShiftState;

    // call hook routines
    for ( pKbdHook=pKbdRootHook; pKbdHook != NULL; pKbdHook=pKbdHook->pNext ) {
        (pKbdHook->pProcedure)( *pScanCode, *pShiftState, 0 );
    }

done:
    LogPrintf( LOG_CLASS, LOG_KEYBOARD,
               "KEYBOARD: scan code (%02X) shift (%04X)",
               *pScanCode, *pShiftState );

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

int WFCAPI
KbdSetLeds( int ShiftState )
{
    int state = 0;

    if (ShiftState & KSS_SCROLLLOCKON)
	state |= 1<<1;

    if ((ShiftState & KSS_NUMLOCKON) == 0)
	state |= 1<<2;

    if ((ShiftState & KSS_CAPSLOCKON) == 0)
	state |= 1<<4;

    _swix(OS_Byte, _INR(0,2), 202, state, ~((1<<1) | (1<<2) | (1<<4)));
    _swix(OS_Byte, _IN(0), 118);

    return( CLIENT_STATUS_SUCCESS );
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
#ifdef DOS
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }
#endif

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
 * KbdAddHook
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdAddHook( PVOID pProcedure )
{
   PKBDHOOK pKbdHook;

   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // check to see if this proc is already hooked
   for ( pKbdHook=pKbdRootHook; pKbdHook != NULL; pKbdHook=pKbdHook->pNext ) {

      // found?
      if ( (PKBDHOOK)pKbdHook->pProcedure == pProcedure ) {
         goto done;
      }
   }

   // allocate a hook structure
   if ( (pKbdHook = (PKBDHOOK) malloc( sizeof(KBDHOOK) )) != NULL ) {

       // initialize and link in at head
       pKbdHook->pProcedure = (PLIBPROCEDURE)pProcedure;
       pKbdHook->pNext = pKbdRootHook;
       pKbdRootHook = pKbdHook;
   }
   else {

       return( CLIENT_ERROR_NO_MEMORY );
   }

   // ok
done:
   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * KbdRemoveHook
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI
KbdRemoveHook( PVOID pProcedure )
{
   PKBDHOOK pKbdHook;
   PKBDHOOK pPrevKbdHook;

   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // find old hook
   for ( pKbdHook=pKbdRootHook, pPrevKbdHook = NULL;
         pKbdHook != NULL;
         pKbdHook=pKbdHook->pNext ) {

      // found hooked proc?
      if ( pKbdHook->pProcedure == (PLIBPROCEDURE)pProcedure ) {

         // remove from list
         if ( pPrevKbdHook == NULL ) {    // first
            pKbdRootHook = pKbdHook->pNext;
         }
         else {                           // other
            pPrevKbdHook->pNext = pKbdHook->pNext;
         }

         // free memory
         free( pKbdHook );

         // out of here
         goto done;
      }

      // maintain prev ptr
      pPrevKbdHook = pKbdHook;
   }

   // not found
   return( CLIENT_STATUS_NO_DATA );

   // ok
done:
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
#ifdef DOS
   // kbd open?
   if ( CurrentKbdMode == Kbd_Closed ) {
      return( CLIENT_ERROR_NOT_OPEN );
   }

   // need to alloc fifo buffer?
   if ( cFifo == 0 ) {
      iFifo = 0;
      cFifo = FIFO_INIT_SIZE;
      pFifo = (int *) malloc( sizeof(int) * cFifo );
   }

   // need to re-alloc fifo buffer?
   else if ( iFifo >= cFifo ) {
      cFifo += FIFO_INIT_SIZE;
      pFifo = (int *) realloc( (void *) pFifo, (sizeof(int) * cFifo) );
   }

   // alloc/realloc work?
   if ( pFifo == NULL ) {
      iFifo = 0;
      cFifo = 0;
      return( CLIENT_ERROR_NO_MEMORY );
   }

   // add character to fifo buffer
   *(pFifo + iFifo++) = ScanCode;
   *(pFifo + iFifo++) = ShiftState;
   *(pFifo + iFifo++) = Char;
#endif
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

#ifdef DOS
/*******************************************************************************
 *
 * KbdGetFifoKey
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

static void
KbdGetFifoKey( int * pScanCode, int * pShiftState, int * pChar )
{

   // remove character from fifo buffer
   *pScanCode   = *(pFifo + 0);
   *pShiftState = *(pFifo + 1);
   *pChar       = *(pFifo + 2);

   // adjust len
   iFifo -= 3;

   // empty? then free fifo buffer
   if ( iFifo == 0 ) {

      // free fifo and clear counters
      if ( pFifo ) {
         free( pFifo );
         pFifo = NULL;
      }
      cFifo = 0;
      iFifo = 0;
   }

   // otherwise shift down
   else {
      memcpy( (pFifo + 0), (pFifo + 3), (iFifo * sizeof(int)) );
   }
}
#endif
