/******************************************************************************
*
*  KBDAPI.H
*
*  Header file for Keyboard APIs.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry
*
*  $Log$
*  
*     Rev 1.10   15 Apr 1997 18:45:16   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.9   26 Jun 1996 14:45:50   brucef
*  Added DOS-specific API and structure to manage user 
*  preferences for keyboard behavior.
*  
*     Rev 1.8   06 May 1996 18:38:54   jeffm
*  update
*  
*     Rev 1.7   02 May 1995 13:39:42   butchd
*  update
*
*******************************************************************************/

#ifndef __KBDAPI_H__
#define __KBDAPI_H__

/*=============================================================================
==   External functions provided by LoadLibraries()
=============================================================================*/

int cdecl WFCAPI KbdLoad( PPLIBPROCEDURE );
int cdecl WFCAPI KbdUnload( VOID );

/*=============================================================================
==   Typedefs and structures
=============================================================================*/

/*
 *  KbdClass enum
 */
typedef enum _KBDCLASS {
   Kbd_Scan,
   Kbd_Ascii,
   Kbd_Closed
} KBDCLASS;

/*
 * Keyboard Preferences (DOS Client Only)
 */
typedef struct _KBDPREFERENCES {
   short KeyboardDelay;
   short KeyboardSpeed;
} KBDPREFERENCES, far *PKBDPREFERENCES;
/*=============================================================================
==   C runtime library routines
=============================================================================*/

#define KBD__GETMODE        0
#define KBD__SETMODE        1
#define KBD__READAVAIL      2
#define KBD__READCHAR       3
#define KBD__READSCAN       4
#define KBD__PUSH           5
#define KBD__SETLEDS        6
#define KBD__REGISTERHOTKEY 7
#define KBD__ADDHOOK        8
#define KBD__REMOVEHOOK     9
#define KBD__FLUSH         10
#define KBD__LOADPREFERENCES 11
#define KBD__COUNT         12

#ifdef KBDLIB

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int WFCAPI KbdGetMode( KBDCLASS * pKbdClass );
int WFCAPI KbdSetMode( KBDCLASS KbdClass );
int WFCAPI KbdReadAvail( int * pCountAvail );
int WFCAPI KbdReadChar( int * pCharCode, int * pShiftState );
int WFCAPI KbdReadScan( int * pScanCode, int * pShiftState );
int WFCAPI KbdPush( int ScanCode, int ShiftState, int Char );
int WFCAPI KbdSetLeds( int ShiftState );
int WFCAPI KbdRegisterHotkey( int HotkeyId, int ScanCode, int ShiftState );
int WFCAPI KbdAddHook( LPVOID pProcedure );
int WFCAPI KbdRemoveHook( LPVOID pProcedure );
int WFCAPI KbdFlush( VOID );
int WFCAPI KbdLoadPreferences( PKBDPREFERENCES pPref );

#else

/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int (PWFCAPI PKBDGETMODE       )( KBDCLASS * pKbdClass );
typedef int (PWFCAPI PKBDSETMODE       )( KBDCLASS KbdClass );
typedef int (PWFCAPI PKBDREADAVAIL     )( int * pCountAvail );
typedef int (PWFCAPI PKBDREADCHAR      )( int * pCharCode, int * pShiftState );
typedef int (PWFCAPI PKBDREADSCAN      )( int * pScanCode, int * pShiftState );
typedef int (PWFCAPI PKBDPUSH          )( int ScanCode, int ShiftState, int Char );
typedef int (PWFCAPI PKBDSETLEDS       )( int ShiftState );
typedef int (PWFCAPI PKBDREGISTERHOTKEY)( int HotkeyId, int ScanCode, int ShiftState );
typedef int (PWFCAPI PKBDADDHOOK       )( LPVOID pProcedure );
typedef int (PWFCAPI PKBDREMOVEHOOK    )( LPVOID pProcedure );
typedef int (PWFCAPI PKBDFLUSH         )( VOID );
typedef int (PWFCAPI PKBDLOADPREFERENCES)( PKBDPREFERENCES pPref );


extern PPLIBPROCEDURE pKbdProcedures;


#define KbdGetMode        ((PKBDGETMODE       )(pKbdProcedures[KBD__GETMODE]))
#define KbdSetMode        ((PKBDSETMODE       )(pKbdProcedures[KBD__SETMODE]))
#define KbdReadAvail      ((PKBDREADAVAIL     )(pKbdProcedures[KBD__READAVAIL]))
#define KbdReadChar       ((PKBDREADCHAR      )(pKbdProcedures[KBD__READCHAR]))
#define KbdReadScan       ((PKBDREADSCAN      )(pKbdProcedures[KBD__READSCAN]))
#define KbdPush           ((PKBDPUSH          )(pKbdProcedures[KBD__PUSH]))
#define KbdSetLeds        ((PKBDSETLEDS       )(pKbdProcedures[KBD__SETLEDS]))
#define KbdRegisterHotkey ((PKBDREGISTERHOTKEY)(pKbdProcedures[KBD__REGISTERHOTKEY]))
#define KbdAddHook        ((PKBDADDHOOK       )(pKbdProcedures[KBD__ADDHOOK]))
#define KbdRemoveHook     ((PKBDREMOVEHOOK    )(pKbdProcedures[KBD__REMOVEHOOK]))
#define KbdFlush          ((PKBDFLUSH         )(pKbdProcedures[KBD__FLUSH]))
#define KbdLoadPreferences ((PKBDLOADPREFERENCES)(pKbdProcedures[KBD__LOADPREFERENCES]))

#endif
#endif //__KBDAPI_H__
