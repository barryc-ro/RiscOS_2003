
/***************************************************************************
*
*  MOUAPI.H
*
*  This module contains internal MOULIB defines and structures
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Andy  (3/15/94)
*
*  mouapi.h,v
*  Revision 1.1  1998/01/12 11:36:52  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.16   11 Jun 1997 10:16:58   terryt
*  client double click support
*  
*     Rev 1.15   27 May 1997 14:25:38   terryt
*  client double click support
*  
*     Rev 1.14   15 Apr 1997 18:45:34   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.13   26 Jun 1996 14:51:02   brucef
*  Add API and structure to manage user preferences
*  for mouse behavior.
*  
*  
*     Rev 1.12   06 May 1996 19:28:00   jeffm
*  update
*  
*     Rev 1.11   01 May 1996 15:44:14   kurtp
*  update
*  
*     Rev 1.10   01 May 1996 15:31:02   kurtp
*  update
*  
*     Rev 1.9   18 Sep 1995 14:59:50   bradp
*  update
*  
*     Rev 1.8   08 May 1995 10:07:28   marcb
*  update
*  
*     Rev 1.7   02 May 1995 13:40:08   butchd
*  update
*
****************************************************************************/

#ifndef __MOUAPI_H__
#define __MOUAPI_H__

#define MOU__READ                 0
#define MOU__READABS              1
#define MOU__READAVAIL            2
#define MOU__PUSH                 3
#define MOU__POSITION             4
#define MOU__SHOWPOINTER          5
#define MOU__RESET                6
#define MOU__SETINFORMATION       7
#define MOU__ADDHOOK              8
#define MOU__REMOVEHOOK           9
#define MOU__SETSCREENDIMENSIONS 10
#define MOU__SETRANGES           11
#define MOU__POSITIONABS         12
#define MOU__LOADPREFERENCES     13
#define MOU__COUNT               14

#ifndef MOU_STATUS_MOVED
/*
 *  Mouse data structure
 */
typedef struct _MOUSEDATA {
    USHORT X;
    USHORT Y;
    BYTE cMouState;
} MOUSEDATA, * PMOUSEDATA;

#define sizeof_MOUSEDATA    5

// mouse status returned in MOUSEDATA structure
#define MOU_STATUS_MOVED    0x01
#define MOU_STATUS_B1DOWN   0x02
#define MOU_STATUS_B1UP     0x04
#define MOU_STATUS_B2DOWN   0x08
#define MOU_STATUS_B2UP     0x10
#define MOU_STATUS_B3DOWN   0x20
#define MOU_STATUS_B3UP     0x40
#define MOU_STATUS_DBLCLK   0x80
#endif

//
typedef enum _MOUSEHOOKCLASS {
   MouseHookRead,
   MouseHookInt
} MOUSEHOOKCLASS;

// data available from interrupt routine
typedef struct _MOUSERAWDATA {
   USHORT Event;
   USHORT ButtonState;
   USHORT X;
   USHORT Y;
   USHORT XMickey;
   USHORT YMickey;
} MOUSERAWDATA;
typedef MOUSERAWDATA far *PMOUSERAWDATA;

#define MOU_RAW_BUTTON1  0x01
#define MOU_RAW_BUTTON2  0x02
#define MOU_RAW_BUTTON3  0x04

// can be set only while no mouse opens; defaults below
typedef struct _MOUINFORMATION {
   USHORT uTimerGran;
   USHORT uQueueSize;
   USHORT uDoubleClickTimerGran;
   USHORT uDoubleClickWidth;
   USHORT uDoubleClickHeight;
} MOUINFORMATION;
typedef MOUINFORMATION far *PMOUINFORMATION;
#define MOUSE_DEFAULT_QSIZE 20
#define MOUSE_DEFAULT_TIMER 2
#define MOUSE_DEFAULT_DBLCLK_TIMER 0
#define MOUSE_DEFAULT_DBLCLK_WIDTH 4
#define MOUSE_DEFAULT_DBLCLK_HEIGHT 4

typedef struct _MOUPOSITION {
   USHORT X;
   USHORT Y;
} MOUPOSITION, far *PMOUPOSITION;

/*
 * Mouse Preferences (DOS Client Only)
 */
typedef struct _MOUPREFERENCES {
   BOOL   fSwapButtons;
   short  HorizSpeed;
   short  VertSpeed;
   short  DblSpeedThreshold;
} MOUPREFERENCES, far *PMOUPREFERENCES;

/*=============================================================================
==   External Functions Defined
=============================================================================*/

#ifndef MOUSELIB

/*
 * Note: These function typedefs must be maintained in sync with the
 *       below function prototypes
 */

typedef int (PWFCAPI PMOUSELOAD)( PPLIBPROCEDURE );
typedef int (PWFCAPI PMOUSEUNLOAD)( void );
typedef int (PWFCAPI PMOUSEREADAVAIL)( PUSHORT );                    
typedef int (PWFCAPI PMOUSEREAD)( PMOUSEDATA, PUSHORT );          
typedef int (PWFCAPI PMOUSEREADABS)( PMOUSEDATA, PUSHORT );       
typedef int (PWFCAPI PMOUSEPUSH)( PMOUSEDATA );                       
typedef int (PWFCAPI PMOUSEPOSITIONREL)( USHORT, USHORT );                      
typedef int (PWFCAPI PMOUSEPOSITIONABS)( USHORT, USHORT );                   
typedef int (PWFCAPI PMOUSESHOWPOINTER)( BOOL );                              
typedef int (PWFCAPI PMOUSEADDHOOK)( MOUSEHOOKCLASS, LPVOID );  
typedef int (PWFCAPI PMOUSEREMOVEHOOK)( MOUSEHOOKCLASS, LPVOID );
typedef int (PWFCAPI PMOUSESETINFORMATION)( PMOUINFORMATION );          
typedef int (PWFCAPI PMOUSERESET)( void );                                       
typedef int (PWFCAPI PMOUSESETSCREENDIMENSIONS)( USHORT, USHORT );
typedef int (PWFCAPI PMOUSESETRANGES)( USHORT, USHORT, USHORT, USHORT );    
typedef int (PWFCAPI PMOUSELOADPREFERENCES)( PMOUPREFERENCES );

extern PPLIBPROCEDURE pMouProcedures;

#define MouseReadAvail           ((PMOUSEREADAVAIL)(pMouProcedures[MOU__READAVAIL]))
#define MouseRead                ((PMOUSEREAD)(pMouProcedures[MOU__READ]))
#define MouseReadAbs             ((PMOUSEREADABS)(pMouProcedures[MOU__READABS]))
#define MousePush                ((PMOUSEPUSH)(pMouProcedures[MOU__PUSH]))
#define MousePosition            ((PMOUSEPOSITIONREL)(pMouProcedures[MOU__POSITION]))
#define MouseShowPointer         ((PMOUSESHOWPOINTER)(pMouProcedures[MOU__SHOWPOINTER]))
#define MouseAddHook             ((PMOUSEADDHOOK)(pMouProcedures[MOU__ADDHOOK]))
#define MouseRemoveHook          ((PMOUSEREMOVEHOOK)(pMouProcedures[MOU__REMOVEHOOK]))
#define MouseSetInformation      ((PMOUSESETINFORMATION)(pMouProcedures[MOU__SETINFORMATION]))
#define MouseReset               ((PMOUSERESET)(pMouProcedures[MOU__RESET]))
#define MouseSetScreenDimensions ((PMOUSESETSCREENDIMENSIONS)(pMouProcedures[MOU__SETSCREENDIMENSIONS]))
#define MouseSetRanges           ((PMOUSESETRANGES)(pMouProcedures[MOU__SETRANGES]))
#define MousePositionAbs         ((PMOUSEPOSITIONABS)(pMouProcedures[MOU__POSITIONABS]))
#define MouseLoadPreferences     ((PMOUSELOADPREFERENCES)(pMouProcedures[MOU__LOADPREFERENCES]))

#else


/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs above
 */

int WFCAPI MouseReadAvail( PUSHORT pCountAvail );
int WFCAPI MouseRead( PMOUSEDATA pMouse, PUSHORT puCount );
int WFCAPI MouseReadAbs( PMOUSEDATA pMouse, PUSHORT puCount );
int WFCAPI MousePush( PMOUSEDATA pMouseData );
int WFCAPI MousePosition( USHORT X, USHORT Y );
int WFCAPI MousePositionAbs( USHORT X, USHORT Y );
int WFCAPI MouseShowPointer( BOOL fOn);
int WFCAPI MouseAddHook( MOUSEHOOKCLASS HookClass, LPVOID fnProc );
int WFCAPI MouseRemoveHook( MOUSEHOOKCLASS HookClass, LPVOID fnProc );
int WFCAPI MouseSetInformation( PMOUINFORMATION pMouInfo );
int WFCAPI MouseReset( void );
int WFCAPI MouseSetScreenDimensions( USHORT uWidth, USHORT uHeight );
int WFCAPI MouseSetRanges( USHORT uHoriMin, USHORT uHoriMax,
                           USHORT uVertMin, USHORT uVertMax );
int WFCAPI MouseLoadPreferences( PMOUPREFERENCES pPref );

int MouseSetScreenOrigin(USHORT x, USHORT y);

#endif

int WFCAPI MouseLoad( PPLIBPROCEDURE pfnMouseProcedures );
int WFCAPI MouseUnload( void );

#endif //__MOUAPI_H__
