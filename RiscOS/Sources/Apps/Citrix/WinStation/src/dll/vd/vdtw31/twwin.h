
/**************************************************************************\
*
*   TWWIN.H
*
*   Thin Wire Windows - Function Prototypes
*
*   Copyright (c) Citrix Systems Inc. 1993-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*   Revision 1.1  1998/01/19 19:12:57  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.7   15 Apr 1997 18:16:36   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   29 Jan 1997 15:51:42   kurtp
*  update
*  
*     Rev 1.5   28 Jan 1997 17:49:52   kurtp
*  update
*  
*     Rev 1.4   13 Jan 1997 16:52:08   kurtp
*  Persistent Cache
*  
*     Rev 1.3   08 May 1996 14:49:26   jeffm
*  update
*  
*     Rev 1.2   03 Jan 1996 14:02:34   kurtp
*  update
*  
\***************************************************************************/

#ifndef __TWWIN_H__
#define __TWWIN_H__

#include "../inc/vd.h"
void TWCmdStrokePath( HWND hWnd, HDC hdc );                 // 0x80
void TWCmdStrokePathStraight( HWND hWnd, HDC hdc );         // 0x81
void TWCmdTextOutNoClip( HWND hWnd, HDC hdc );              // 0x82
void TWCmdInit( HWND hWnd, HDC hdc );                       // 0x83
void TWCmdTextOutRclClip( HWND hWnd, HDC hdc );             // 0x84
void TWCmdTextOutCmplxClip( HWND hWnd, HDC hdc );           // 0x85
void TWCmdTextOutRclExtra( HWND hWnd, HDC hdc );            // 0x86
void TWCmdPalette( HWND hWnd, HDC hdc );                    // 0x87
#ifndef DOS
void TWCmdInitializeThinwireClient( HWND hWnd, HDC hdc );   // 0x01
void TWCmdBitBltSourceROP3NoClip( HWND hWnd, HDC hdc );     // 0x02
void TWCmdBitBltNoSrcROP3NoClip( HWND hWnd, HDC hdc );      // 0x03
void TWCmdBitBltScrToScrROP3( HWND hWnd, HDC hdc );         // 0x04
void TWCmdPointerSetShape( HWND hWnd, HDC hdc );            // 0x05
void TWCmdBitBltNoSrcROP3CmplxClip( HWND hWnd, HDC hdc );   // 0x06
void TWCmdSSBSaveBitmap( HWND hWnd, HDC hdc );              // 0x07
void TWCmdSSBRestoreBitmap( HWND hWnd, HDC hdc );           // 0x08
void TWCmdBitBltSourceROP3SimpleClip( HWND hWnd, HDC hdc ); // 0x09
void TWCmdBitBltSourceROP3CmplxClip( HWND hWnd, HDC hdc );  // 0x0A
BOOL BltSrcRop3Noclip( HDC hdc );
BOOL BltNosrcRop3Noclip( HDC hdc );
BOOL BltScrToScrRop3( HDC hdc );
BOOL BltNosrcRop3Cmplxclip( HDC hdc );
BOOL BltSrcRop3Simpleclip( HDC hdc );
BOOL BltSrcRop3Cmplxclip( HDC hdc );
#endif

#define MAX( a, b ) (( (b) > (a) ) ? (b) : (a) )
#define MIN( a, b ) (( (b) < (a) ) ? (b) : (a) )
#define ABS( a ) ( ((a) < 0 ) ? (-1*(a)) : (a) )

#define TWSetJmpSaveStack( Buf )        setjmpSaveStack( Buf )
#define TWSetJmpNewStack( Buf,a )       setjmpNewStack( Buf,a )
#define TWSetJmpNewStack2( Buf )        setjmpNewStack2( Buf )
#define TWLongJmpChangeStack( Buf, rc ) longjmpChangeStack( Buf, rc )

#define TWCmdReturn( rc ) TWLongJmpChangeStack( vjmpComplete, rc )


/*-------------------------------------------------------------------------
-- PROTOTYPES
--------------------------------------------------------------------------*/
BOOL far NewWindowsCommand(CHAR Cmd);
BOOL far ResumeWindowsCommand(CHAR DataByte);
CHAR far SuspendWindowsCommand(void);
BOOL far GetNextTWCmdBytes( void * pData, int cbData );

/* int  far _cdecl setjmpNewStack(jmp_buf3); */
/* int  far _cdecl setjmpNewStack2(jmp_buf3); */
/* int  far _cdecl setjmpSaveStack(jmp_buf3); */
void far _cdecl longjmpChangeStack(jmp_buf3, int);

// To temporarily remove a trace statement, add a 'D' in front of the macro
#define DTRACE( _arg ) { }
#define DTRACEBUF( _arg ) { }

//jkbetter - if we do a create and destroy dc state of the windows client
//some of the resources may make more sense to destroy when the dc goes
//away and recreate them when the dc is recreated
//assuming this happens infrequently like when the focus is given and taken
//Whether we can do this or not depends on the host state
//if the host keeps state then we cannot do this!
//
BOOL ThinInitOnce(UINT chunks_2K); //called once when app started to initialize
                            //and create all thinwire persistant resources

BOOL ThinDestroyOnce(void);     //called to delete all thinwire resource
                            //when the app is going away

int TWInitWindow( PVD pVd, HWND hWnd );
int TWDestroyWindow( HWND hWnd );
int TWPaint( PVD pVd, HWND hWnd );
INT  wfnEnumRects( HWND, HDC, LPRECT FAR *, INT *, LPRECT );
VOID wfnFreeRects( LPRECT );
int WdCall( PVD pVd, USHORT ProcIndex, PVOID pParam );


////////////////////////////////////////////////////////////////////
// dim routines and structures
////////////////////////////////////////////////////////////////////

//
//  DIM File Header
//
// SJM: although our structure won't be the same as a Citrix client one,
// due to structure padding, this doesn't matter since it is only on the
// client.
//#pragma pack(1)
typedef struct _DIM_HEADER {  
    BYTE    cbHeader;           //  0 - 0
    BYTE    sigLevel;           //  1 - 1
    ULONG   flag;               //  2 - 5
    ULONG   sigH;               //  6 - 9
    ULONG   sigL;               // 10 - 13
    ULONG   iLastSection;       // 14 - 17
    ULONG   cbLastSection;      // 18 - 21
    BYTE    reserved[8];        // 22 - 31 - SJM: was 10, reduced to account for earlier padding
} DIM_HEADER, *PDIM_HEADER;
//#pragma pack()


//
//  Size of DIM_HEADER
//

#define DIM_SYSTEM_OVERHEAD sizeof(DIM_HEADER)


//
// DIM File Flag 
//
#define DIM_FILE_FLAG 0x534D4944  // string "DIMS" as the starting string at DIM file


//
//  Currently supported DIM signature level
//

#define DIM_SIGNATURE_LEVEL 0


//
//  Size of DIM Cache Section (same as _2K on RAM cache)
//
//  NOTE: It must be _2K so we can re-use the RAM cache routines!
//

#define SECTION_SIZE    2048


#endif //__TWWIN_H__
