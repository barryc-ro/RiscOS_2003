/*****************************************************************************
*
*   WINDOW.C
*
*   Windows client engine window procedure
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Marc Bloomfield (marcb) 27-Mar-1995
*
*   $Log$
*  
*     Rev 1.136   21 Aug 1997 15:10:50   terryt
*  1.6 hotfixes
*  
*     Rev 1.135   14 Aug 1997 15:07:50   kurtp
*  fix full screen, again
*  
*     Rev 1.134   12 Aug 1997 22:34:36   tariqm
*  win16 fix
*  
*     Rev 1.133   06 Aug 1997 20:51:00   kurtp
*  fix windows full screen mode
*  
*     Rev 1.132   05 Aug 1997 20:27:10   kurtp
*  Add Full Screen Window support
*  
*     Rev 1.131   21 Jul 1997 19:32:44   tariqm
*  Update...
*  
*     Rev 1.130   Jul 11 1997 20:55:38   scottc
*  sends alt-key before mouse clicks when needed
*  
*     Rev 1.129   10 Jul 1997 22:53:20   tariqm
*  Connection Status..
*  
*     Rev 1.128   27 May 1997 14:27:16   terryt
*  client double click support
*  
*     Rev 1.127   08 May 1997 14:28:48   kurtp
*  Fix nc double click positioning
*  
*     Rev 1.126   15 Apr 1997 18:18:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.125   04 Mar 1997 17:41:42   terryt
*  client shift states
*  
*     Rev 1.124   20 Feb 1997 14:09:52   butchd
*  Added client data to connection status dialog
*  
*     Rev 1.123   31 Jan 1997 18:36:16   BradA
*  Fixed the client to send scan codes for each shift key when one was
*  released and the other was down.  Windows does not provide the up code
*  for the first shift key released when both were held down simultaneously.
*  
*     Rev 1.122   19 Aug 1996 18:19:32   jeffm
*  fix for full drag redraw craziness
*  
*     Rev 1.121   13 Aug 1996 12:02:40   jeffm
*  Fixes for WIN16 version tag, DesiredHRES VRES being -1, Alt-TAB for WIN16, and IDLE detection for 
*  Autodisconnect
*  
*     Rev 1.120   22 Jul 1996 13:31:40   jeffm
*  Fix 16-bit build break
*  
*     Rev 1.119   17 Jul 1996 19:34:28   jeffm
*  Fix for redraw of security popup
*  
*     Rev 1.118   16 Jul 1996 18:14:02   jeffm
*  Moving browser off window and back on again
*  
*     Rev 1.117   16 Jul 1996 17:59:22   jeffm
*  Cannot erase background for plugin window
*  
*     Rev 1.116   16 Jul 1996 17:38:54   jeffm
*  Erase background of update rectangle
*  
*     Rev 1.115   16 Jul 1996 15:21:52   jeffm
*  Draging connection dialog on win95 fix
*  
*     Rev 1.114   16 Jul 1996 10:33:04   marcb
*  update
*  
*     Rev 1.113   15 Jul 1996 13:03:36   jeffm
*  Hangup only if Internet Explorer not present
*  
*     Rev 1.112   12 Jul 1996 18:24:50   jeffm
*  Tell autodialer that we are done
*  
*     Rev 1.111   12 Jul 1996 17:13:08   jeffm
*  Plugin window watchdog timer for GPFs in Explorer
*  
*     Rev 1.110   10 Jul 1996 19:42:20   jeffm
*  Added setcapture, releasecapture to fix problem with child window getting mouse input
*  
*     Rev 1.109   02 Jul 1996 15:59:36   jeffm
*  Beep to let the user know that we don't want to change the language.
*  
*     Rev 1.108   27 Jun 1996 18:16:14   jeffm
*  Flip version to correct way
*  
*     Rev 1.107   26 Jun 1996 16:17:16   jeffm
*  Get rid of compiler warnings for unused vars in Win16
*  
*     Rev 1.106   26 Jun 1996 15:46:56   marcb
*  update
*  
*     Rev 1.105   21 Jun 1996 14:20:24   jeffm
*  Added version code for connection dialog
*  
*     Rev 1.104   20 Jun 1996 13:48:30   jeffm
*  Fixed palette issue with plugin.  Need to realize palette for every WM_PAINT.
*  
*     Rev 1.103   13 Jun 1996 18:25:32   jeffm
*  Added support for border / no border for plugin window
*  
*     Rev 1.102   13 Jun 1996 12:06:30   marcb
*  update
*  
*     Rev 1.101   12 Jun 1996 17:38:50   jeffm
*  update
*  
*     Rev 1.100   11 Jun 1996 14:54:10   jeffm
*  update
*  
*     Rev 1.98   01 Jun 1996 12:28:06   unknown
*  update
*  
*     Rev 1.97   28 May 1996 19:57:26   jeffm
*  update
*  
*     Rev 1.96   24 May 1996 13:30:46   kurtp
*  update
*  
*     Rev 1.95   21 May 1996 17:51:36   jeffm
*  update
*  
*     Rev 1.94   21 May 1996 13:34:06   kurtp
*  Fix alt-tab fix
*  
*     Rev 1.93   20 May 1996 17:47:06   jeffm
*  update
*  
*     Rev 1.88   08 May 1996 14:11:36   kurtp
*  update
*  
*     Rev 1.87   01 May 1996 15:30:46   kurtp
*  update
*  
*     Rev 1.86   09 Feb 1996 14:31:30   jeffm
*  remove savebits window class
*  
*     Rev 1.85   07 Feb 1996 17:05:04   kurtp
*  update
*  
*     Rev 1.84   07 Feb 1996 15:42:22   kurtp
*  update
*  
*     Rev 1.83   01 Feb 1996 14:26:58   kurtp
*  update
*  
*     Rev 1.82   17 Jan 1996 18:24:26   kurtp
*  update
*  
*     Rev 1.81   10 Jan 1996 16:59:16   kurtp
*  update
*  
*     Rev 1.80   09 Nov 1995 21:42:54   kurtp
*  update
*  
*     Rev 1.79   07 Nov 1995 14:23:14   butchd
*  update
*  
*     Rev 1.78   07 Nov 1995 09:26:02   butchd
*  update
*  
*     Rev 1.77   06 Nov 1995 19:03:48   kurtp
*  
*     Rev 1.76   06 Nov 1995 18:21:34   kurtp
*  
*     Rev 1.75   06 Nov 1995 17:55:48   kurtp
*  
*     Rev 1.74   06 Nov 1995 08:17:38   butchd
*  update
*  
*     Rev 1.73   27 Oct 1995 10:46:00   kurtp
*  update
*  
*     Rev 1.72   26 Oct 1995 16:37:50   kurtp
*  update
*  
*     Rev 1.71   06 Oct 1995 10:53:32   kurtp
*  update
*  
*     Rev 1.70   29 Sep 1995 11:52:16   butchd
*  update
*  
*     Rev 1.69   28 Sep 1995 08:29:30   butchd
*  update
*  
*     Rev 1.68   26 Sep 1995 17:00:00   kurtp
*  update
*  
*     Rev 1.67   25 Sep 1995 15:28:26   bradp
*  update
*  
*  
****************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <string.h>
/* #include <shellapi.h> */      // for ExtractIcon()

#ifdef WIN16
#include <ver.h>
#endif

#include "../../inc/client.h"
#include "../../inc/clib.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/mouapi.h"
#include "../../inc/kbdapi.h"
#include "../../inc/biniapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/logapi.h"

#define DOS

/*=============================================================================
==   Local Functions
=============================================================================*/
LRESULT MainWM_Char( HWND hWnd, USHORT CharCode, USHORT RepeatCount, BOOL fExtended );
LRESULT MainWM_SetFocus( VOID );
LRESULT MainWM_KillFocus( VOID );
LRESULT MainWM_Mouse( HWND hWnd, UINT message, USHORT xPos, USHORT yPos );
LRESULT MainWM_QueryNewPalette( VOID );
LRESULT MainWM_PaletteChanged( VOID );
void SendScanCode( USHORT ScanCode, USHORT RepeatCount, BOOL fExtended );
void FlushScanCode( void );
BOOL InitOnce(HINSTANCE hInstance, PWFEOPEN pWFEOpen);
HWND InitInstance( HINSTANCE hInstance, INT nCmdShow,
                   PWFEOPEN pWFEOpen );
BOOL WFCAPI wKeyboardHookProc( INT code, WPARAM wParam, LPARAM lParam );

#ifndef DOS
void CALLBACK ClientStatDialogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime);
BOOL CALLBACK ClientStatDialogProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);
void CreateClientStatDlg(HWND hWnd);
void DestroyClientStatDlg(HWND hWnd);
void UpdateClientStatDlg(HWND hWnd, BOOL bReset);

#ifdef HEARTBEAT_ENABLED
void DrawCaptionBar(HWND hWnd, BOOL bActive);
void DrawHeartbeat(HWND hWnd, LPRECT pRect, BOOL bRedraw);
#endif

void DrawLED( HWND hWnd, int DlgID, BOOL bOn);
int  GetIOStatus(PIOSTATUS pIOStatus);
#endif
#ifdef WIN32
void CALLBACK ParentWatchdogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime);
void CALLBACK WinSockIdleTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime);
VOID SendAutoDisconnect();
#endif


/*=============================================================================
==   External Functions
=============================================================================*/
int  KbdCheckHotkey( unsigned char, int );
#ifndef DOS
void LocalPaint( HWND hWnd );
extern BOOL CenterPopup( HWND, HWND );
extern int  EMGetStringResource(int iResID, LPSTR szString, int cbStr);
#endif

/*=============================================================================
==  External Vars
=============================================================================*/
extern BOOL gbContinuePolling;
extern BOOL gbDirectClose;
extern HWND ghWFE;
extern USHORT G_MouseTimer;
extern USHORT G_KeyboardTimer;
extern USHORT g_border;

#ifndef DOS
extern BOOL gbIPCEngine;
extern HICON ghCustomIcon;
extern BOOL  gfRemoveTitleBar;
extern char gpszServiceConnection[LENGTH_SERVICECONNECTION+1];
extern HINSTANCE ghPrevInstance;
extern HCURSOR   vhCursorVis;

extern UINT      g_MsgEngineInit;
extern UINT      g_MsgEngineUninit;
extern UINT      g_MsgNewhWnd;
extern HWND      g_hWndPlugin;

char                gpszClassName[LENGTH_SERVICECONNECTION+1+1] = { 0 };
extern BOOL         gfIPCShutdown;
extern BOOL         gfProcessMessages;
extern FILEPATH     gszExeName;
extern ENCRYPTIONLEVEL g_szEncryptionLevelConnStatus;
#ifdef DEBUG
extern BOOL vfLogging;
#endif
#endif

/*=============================================================================
==   Local Vars
=============================================================================*/

// #include "resource.h"

#define IDM_CLIENT_STATUS 111              // client status menu item id
#define CLIENTSTAT_MSEC   250              // msec between updates
#define PARENT_WATCHDOG_TIMEOUT  5000      // watchdog timeout for parent proc
#define WMPAINT_REDRAW_DELAY_MSEC  100     // force WM_PAINT to slow down when bursting

#ifdef WIN32
// defines specific to Internet Explorer
//
#define INTERNET_EXPLORER_CLASS_NAME                    "IEFrame"
#define AUTODISCONNECT_MONITOR_CLASS_NAME		"MS_AutodialMonitor"
#define WM_WINSOCK_ACTIVITY                             WM_USER+100
#define WM_BROWSER_EXITING				WM_USER+103
#define WINSOCK_IDLE_TIMEOUT                            45000
#endif


#ifdef HEARTBEAT_ENABLED
#define NUM_BITMAPS       1
#define NUM_BITMAPCOLORS  9

#define BITMAP_MSEC       100              // msec between updates

int BitmapIds[NUM_BITMAPS] = { IDB_BITMAP1};
static HBITMAP hBitmaps[NUM_BITMAPS] = {NULL};
static COLORREF BitmapColors[NUM_BITMAPCOLORS] = {
    RGB( 000, 128, 128),                       // dark blue
    RGB( 000, 144, 144),
    RGB( 000, 160, 160),
    RGB( 000, 176, 176),
    RGB( 000, 192, 192),
    RGB( 000, 208, 208),
    RGB( 000, 224, 224),
    RGB( 000, 240, 240),
    RGB( 000, 255, 255),                       // bright blue
    };
static HBRUSH hbrBitmap[NUM_BITMAPCOLORS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int giBitmap=0;                            // index into bitmap handle array

RECT rectHeart;                            // Heartbeat rectangle area
#endif //HEARTBEAT_ENABLED

#ifdef WIN16
static FARPROC lpfnClientStatDlgProc=NULL;
#endif
BOOL   gbClientStatusDlg = FALSE;            // client status on/off switch
HWND   ghClientStatDlg   = NULL;             // client status dialog handle
int    ghClientStatTimer = 0;                // client status timer
HBRUSH ghLEDBrush = NULL;                    // red brush
BOOL   gbActiveWindow=TRUE;                  // active window flag
IOSTATUS PrevIOStatus={sizeof(IOSTATUS),0,0,0,0,0,0};  // previous IO status
IOSTATUS ResetIOStatus={sizeof(IOSTATUS),0,0,0,0,0,0}; // status at last reset request

char gszDefTitle[] = "Citrix WinFrame";  // The title bar text
char *gszTitle = NULL;                   // dynamic window title
#ifndef DOS
static ULONG KeyboardTime = 0;
static USHORT gShiftState = 0;
static USHORT gTrueShiftState = 0;
static int  vcxBorder;
static int  vcyBorder;
static int  vcxSave;
static int  vcySave;
static struct {
          USHORT left;
          USHORT right;
          USHORT either;
          UCHAR  rightScan;
       } gStates[] = {
        { KSS_LEFTSHIFT, KSS_RIGHTSHIFT, KSS_EITHERSHIFT, 0x36 }, // VK_SHIFT    0x10
        { KSS_LEFTCTRL,  KSS_RIGHTCTRL,  KSS_EITHERCTRL,  0    }, // VK_CONTROL  0x11
        { KSS_LEFTALT,   KSS_RIGHTALT,   KSS_EITHERALT,   0    }, // VK_ALT      0x12
       };
extern HINSTANCE ghInstance;

#define IDT_MOUSEMOVE       1

#define KBD_LSHIFT_UP_CODE      0xaa
#define KBD_RSHIFT_UP_CODE      0xb6

// change cruntime string calls to windows string calls
#define strlen lstrlen

#endif

#ifdef WIN32
HANDLE g_hParentProc;
UINT   g_hWatchdogTimer;
UINT   g_hWinSockIdleTimer;
BOOL   fDragOn        = FALSE;
#endif

#ifndef DOS
/*******************************************************************************
 *
 *  Function: MainWndProc
 *
 *  Purpose: client window procedure
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     message (input)
 *        type of message 
 *     wParam (input)
 *        message-specific paramater
 *     lParam (input)
 *        message-specific paramater
 *
 *  Exit:
 *     message specific return code
 *
 ******************************************************************************/
LRESULT WFCCALLBACK
MainWndProc( HWND hWnd,       
             UINT message,    
             WPARAM wParam,   
             LPARAM lParam )   
{
       LRESULT rc = 0;
       PWFEINSTANCE pInstanceData = (PWFEINSTANCE)GetWindowLong( hWnd, GWL_INSTANCEDATA );
#ifndef DOS
       int    cx;
       int    cy;
       RECT   rect;
       RECT   rectCapture;
static BOOL   fMouseCaptured = FALSE;
static BOOL   fTimerSet      = FALSE;
static USHORT mMouse;
static USHORT xMouse;
static USHORT yMouse;
#ifdef WIN32
       BOOL bResult;
       RECT rWorkArea;
#endif
#ifdef HEARTBEAT_ENABLE
       POINT pt;
#endif
#endif

   switch (message) {
#ifdef WIN32
      case WM_INPUTLANGCHANGEREQUEST:
           //do not let the user change the keyboard language
           MessageBeep( MB_OK );
           return(FALSE);
           break;

      // we do not want the window size to change on Win95 if the user has
      // changed the desktop size
      case WM_GETMINMAXINFO:
           {
            LPMINMAXINFO lpmmi;
            INT cx,cy;

            lpmmi = (LPMINMAXINFO) lParam;

            /*
             *  Get client size
             */
            cx = (INT) GetWindowLong( hWnd, GWL_WINDOWWIDTH ) +
                 (INT) GetWindowLong( hWnd, GWL_CXBORDER );
            cy = (INT) GetWindowLong( hWnd, GWL_WINDOWHEIGHT ) +
                 (INT) GetWindowLong( hWnd, GWL_CYBORDER );
            
            /*
             *  Add in window dressing stuff
             */
            if ( GetWindowLong( hWnd, GWL_TITLEPRESENT)){
            
               /*
                *  Add in title 
                */
               cy += (INT) GetWindowLong( hWnd, GWL_CYTITLE );
               } 

           if((cx!=0) && (cy!=0)) {
              lpmmi->ptMaxSize.x = cx;
              lpmmi->ptMaxSize.y = cy;
              lpmmi->ptMaxTrackSize.x = cx;
              lpmmi->ptMaxTrackSize.y = cy;
              lpmmi->ptMaxPosition.x = vcxSave;
              lpmmi->ptMaxPosition.y = vcySave;
              }

           /*
            *  Center window on desktop
            */
#ifdef WIN32
           // Get the limits of the "work area".
           bResult = SystemParametersInfo( SPI_GETWORKAREA, sizeof(RECT), &rWorkArea, 0);
           if ( bResult) {
              vcxBorder = ((rWorkArea.right - rWorkArea.left) - cx) / 2;
              vcyBorder = ((rWorkArea.bottom - rWorkArea.top) - cy) / 2;
           }
           else 
#endif
           {
           vcxBorder = (GetSystemMetrics(SM_CXSCREEN) - cx) /2;
           vcyBorder = (GetSystemMetrics(SM_CYSCREEN) - cy) /2;
           }
       
           /*
            *  Hide border on full screen
            */
           if ( vcxBorder < 0 ) 
              vcxBorder = - (GetWindowLong( hWnd, GWL_CXBORDER ) / 2);  
       
           if ( vcyBorder < 0 ) 
              vcyBorder = - (GetWindowLong( hWnd, GWL_CYBORDER ) / 2);
        
           /*
            *  Save current position
            */
           GetWindowRect( hWnd, &rect );
           vcxSave = rect.left;
           vcySave = rect.top;

           return(0);
           }
           break;
#endif
      case WM_ERASEBKGND:
          {
          RECT rcUpdate;
          HDC  hdc;

#ifdef WIN32
          if(fDragOn || !g_hWndPlugin) {
#else
          if(!g_hWndPlugin) {
#endif
             GetClientRect( hWnd, &rcUpdate);
             hdc = (HDC)wParam;
             FillRect( hdc, &rcUpdate, GetStockObject( WHITE_BRUSH ));
             }

          return(1);
          }
          break;

      case WM_KEYDOWN:
      case WM_KEYUP:
          wKeyboardHookProc( 0 , wParam, lParam);
          break;

      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
          wKeyboardHookProc( 0, wParam, lParam);
#ifdef WIN16
          /*
           * pass on Alt-Tab and Alt-Esc to Windows
           */
          if( (wParam==VK_TAB) || (wParam == VK_ESCAPE) ) {
              DefWindowProc( hWnd, message, wParam, lParam );
          }
#endif
          break;

     // ignore system chars
     case WM_SYSCHAR:
         break;

     case WM_NCACTIVATE:
        DefWindowProc( hWnd, message, wParam, lParam );

#ifdef HEARTBEAT_ENABLE
        DrawCaptionBar( hWnd, wParam);
#endif
        gbActiveWindow = wParam;
        return(TRUE);
        break;
      	      
     case WM_NCPAINT:
        DefWindowProc( hWnd, message, wParam, lParam );
#ifdef HEARTBEAT_ENABLE
        DrawCaptionBar( hWnd, gbActiveWindow);
#endif
        return(FALSE);
        break;

     case WM_NCLBUTTONDOWN:

#ifdef HEARTBEAT_ENABLE
        if(wParam == HTCAPTION) {

           // convert location to client window
#ifdef WIN32
           pt.x = MAKEPOINTS(lParam).x;
           pt.y = MAKEPOINTS(lParam).y;
#else
           pt.x = LOWORD(lParam);
           pt.y = HIWORD(lParam);
#endif
           ScreenToClient(hWnd,&pt);
           }
#endif

#ifdef WIN32
        fDragOn = TRUE;
#endif

        DefWindowProc( hWnd, message, wParam, lParam );

#ifdef WIN32
        fDragOn = FALSE;
#endif

#ifdef HEARTBEAT_ENABLE
        if(wParam == HTCAPTION) {
           if((pt.x >= rectHeart.left) &&
              (pt.x <= rectHeart.right)) {

              if(gbClientStatusDlg == FALSE) {
                 // create connection status dialog
                 CreateClientStatDlg(hWnd);
                 UpdateClientStatDlg(hWnd, FALSE);
                 }
              else {
                 // delete connection status dialog
                 DestroyClientStatDlg(hWnd);
                 }
              }
           }
#endif
        break;

      case WM_SYSCOMMAND:
         if ( wParam == SC_CLOSE )
            gbDirectClose = TRUE;

         // if user wants client status dialog, give it to them
         if( wParam == IDM_CLIENT_STATUS) {

            if(gbClientStatusDlg == FALSE) {
               // create connection status dialog
               CreateClientStatDlg(hWnd);
               UpdateClientStatDlg(hWnd, FALSE);
               }
            else {
               // delete connection status dialog
               DestroyClientStatDlg(hWnd);
               }

            }
         goto defwndproc;   // handle as normal

      case WM_CREATE:
         break;

      case WM_NCLBUTTONDBLCLK :
         
         // do not bother if iconic
         if ( !IsIconic( hWnd ) ) {

             GetWindowRect( hWnd, &rect );

             if ( (rect.left == vcxBorder) && (rect.top == vcyBorder) ) {
                 cx = vcxSave;
                 cy = vcySave;
             }
             else {
                 cx = vcxBorder;
                 cy = vcyBorder;
             }

             SetWindowPos(  hWnd, NULL, cx, cy, 0, 0,
                            SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

             vcxSave = rect.left;
             vcySave = rect.top;
         }
         break;

      case WM_DESTROY:  // message: window being destroyed
         {
#ifdef WIN32
            // tell the autodialer that we are out of here
            SendAutoDisconnect();
#endif
            gfIPCShutdown     = TRUE;  // Disable all IPC to UI
            gfProcessMessages = FALSE; // Disable all Peek-Dispatch loops
            TRACE(( TC_WENG, TT_L1, "WEngx.Exe MainWinProc: WM_DESTROY entered" ));
            wdDisconnect(); // Disconnect if not already disconnected
#ifndef DOS
            if ( fTimerSet ) {
                KillTimer( hWnd, IDT_MOUSEMOVE );
                fTimerSet = FALSE;
            }
            if(gbIPCEngine) 
               ipcDestroyWindow();
#endif
            wdDestroyWindow( hWnd );
            gbContinuePolling = FALSE; // Abort primary poll loop
            ghWFE = NULL;
            if ( pInstanceData ) {
                SetWindowLong( hWnd, GWL_INSTANCEDATA, (LONG)NULL ); // Couldn't hurt
                WinFree( pInstanceData );
            }
#ifndef DOS
            UnregisterClass( gpszClassName, ghInstance );
            if ( ghCustomIcon )
                DestroyIcon( ghCustomIcon );
            PostQuitMessage( 0 ); // Exit the app when this window ends
            ClipCursor( NULL );
#endif
            TRACE(( TC_WENG, TT_L1, "WEngx.Exe MainWinProc: WM_DESTROY exited" ));
         }
         break;

      case WM_KILLFOCUS:
         return( MainWM_KillFocus() );
         break;
      
      case WM_SETFOCUS:
         return( MainWM_SetFocus() );
         break;
           
#ifndef DOS // BUGBUG: Allow for DOS
      case WM_TIMER :

          /* 
           *  Mouse move timer?
           */
          if ( wParam == IDT_MOUSEMOVE ) {

              /*
               *  Stop timer
               */
              KillTimer( hWnd, IDT_MOUSEMOVE );
              fTimerSet = FALSE;

              /*
               *  Send last mouse position
               */

#ifdef WIN16
// Check and see if the alt-key needs to be sent before the mouse data              
             wKeyboardHookProc( mMouse, (WPARAM) 0 , (LPARAM) 0 );
#endif                   

             (void) MainWM_Mouse( hWnd, mMouse, xMouse, yMouse );
          }
#ifdef HEARTBEAT_ENABLE
          if ( wParam == IDB_BITMAP1 ) {
             DrawHeartbeat( hWnd, &rectHeart, FALSE);
             }
#endif
          break;

      case WM_PALETTECHANGED :

          /*
           *  Ignore palette changed message generated by us
           */
          if ( (HWND)wParam == hWnd )
              break;

          /*
           *  Notify client to update it's palette, background
           */
          return( MainWM_PaletteChanged() );

      case WM_QUERYNEWPALETTE :

          /*
           *  Notify client to update it's palette, foreground
           */
          return( MainWM_QueryNewPalette() );

      case WM_CLOSE:
         {
            LRESULT fConfirmClose;
static      BOOL    vfClosing = FALSE; 

            if ( !vfClosing ) {   // Bit-bucket nested close requests
                vfClosing = TRUE;

                /*
                 * Don't confirm close unless user initiated it directly.
                 */
                if ( gbDirectClose )
                    fConfirmClose = StatusMsgProc( ghWFE, CLIENT_STATUS_QUERY_CLOSE );
               
                if ( !gbDirectClose || fConfirmClose ) {
                   /*
                    * Disconnect from Wd
                    */
                   wdDisconnect();
                   StatusMsgProc( ghWFE, CLIENT_STATUS_CONNECTION_BROKEN_CLIENT );
                   DestroyWindow( hWnd );
                } else {
                   vfClosing     = FALSE;
                }
                gbDirectClose = FALSE;  // in case we said 'no'; reset the flag
                TRACE(( TC_WENG, TT_L1,
                        "WEngx.Exe MainWinProc: WM_CLOSE fConfirmClose(%08lX)",
                        fConfirmClose ));
                ClipCursor( NULL );
            }
         }
         break;

      case WM_WFENG_DESTROY:
         TRACE(( TC_WENG, TT_L1, "WEngx.Exe MainWinProc: WM_WFENG_DESTROY entered" ));
         if ( wParam == WFENG_IPC_VERSION ) {
             DestroyWindow( hWnd );
         } else {
             TRACE(( TC_WENG, TT_ERROR,
                     "WEngx.Exe MainWinProc: Error - WM_WFENG_DESTROY not processed" ));
         }
         break;
#endif

#ifdef WIN32
      /*
       *  Bugfix for CPR1943
       */
      case WM_ENTERSIZEMOVE :
          fDragOn = TRUE;
          goto defwndproc;

      //  really WM_EXITSIZEMOVE
       case WM_SIZE :
          if ( wParam == SIZE_RESTORED ) {
              MainWM_QueryNewPalette();
          }

      case WM_EXITSIZEMOVE :
          fDragOn = FALSE;
          InvalidateRgn( hWnd, NULL, TRUE);
          goto defwndproc;

#elif WIN16
      //  really WM_EXITSIZEMOVE
      case WM_SIZE :
          if ( wParam == SIZE_RESTORED ) {
              MainWM_QueryNewPalette();
          }
          break;
#endif

      /*
       * These messages only processed if connection made and we're not minimized
       * Note: New 'case WM_xxx' handlers must be added twice!
       */
      case WM_PAINT:
      case WM_CHAR:
      case WM_MOUSEMOVE:
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:

         TRACE(( TC_WENG, TT_L1,
                 "WEngx.Exe: MainWinProc - connectonly message screening (%04X)",
                 message ));
         if ( !IsIconic( hWnd ) && (gState & WFES_CONNECTED) ) {
              TRACE(( TC_WENG, TT_L1,
                      "WEngx.Exe: MainWinProc - connectonly message (%04X)",
                      message ));
            switch (message) {
              case WM_PAINT:
                 {
                    RECT rcUpdate;
                    static DWORD dwTickCnt,dwPrevTickCnt;
                    DWORD dwTicks;

                    // don't do anything if no update rectangle
                    if(!GetUpdateRect( hWnd, &rcUpdate, TRUE)) 
                        return(0);

                    // if we get too many WM_PAINT messages too fast,
                    // let's slow them down to give the server a chance
                    // to catch up. this also consolidates the WM_PAINT
                    // messages into something to do later.
                    dwTickCnt = GetTickCount();
                    dwTicks = dwTickCnt - dwPrevTickCnt;
                    if(dwTicks < WMPAINT_REDRAW_DELAY_MSEC) {
                        return(0);
                    }
                    dwPrevTickCnt = dwTickCnt;
                    
#ifdef WIN32
                  // realize palette for plugin for WM_PAINT messages
                  if(g_hWndPlugin) {
                     MainWM_QueryNewPalette();
                     }
                  /*
                   *  Bugfix for CPR1943
                   */
                  if ( fDragOn ) 
                  {
                      return(0);
                  }
                  else 
#endif
                  {
                      wdPaint( hWnd );
                  }
                 }
                  break;
           
               case WM_CHAR:

                  return( MainWM_Char( hWnd, (USHORT)wParam, (USHORT)lParam,
                                       !!(lParam & 0x1000000) ) );
                  break;

               case WM_MOUSEMOVE:
               case WM_LBUTTONDBLCLK:
               case WM_LBUTTONDOWN:
               case WM_LBUTTONUP:
               case WM_MBUTTONDBLCLK:
               case WM_MBUTTONDOWN:
               case WM_MBUTTONUP:
               case WM_RBUTTONDBLCLK:
               case WM_RBUTTONDOWN:
               case WM_RBUTTONUP:
#ifndef DOS

#ifdef DEBUG
                    /*
                     *  Use message not used to toggle logging
                     */
                    if( (message == WM_RBUTTONDOWN) && 
                        (GetKeyState(VK_SHIFT) & (SHORT) 0x8000) ) {
              
                       /*
                        *  Toggle logging suspend/resume
                        */
                       MessageBeep( MB_OK );
                  
                       if ( vfLogging == TRUE)
                          TRACE(( TC_ALL, TT_ERROR, "WFEngx.Exe: suspend logging" ));
                  
                       vfLogging = (vfLogging == TRUE) ? FALSE : TRUE;
                  
                       if ( vfLogging == TRUE)
                           TRACE(( TC_ALL, TT_ERROR, "WFEngx.Exe: resume logging" ));
                   }
#endif
                   /*
                    *  If there is a queued mouse button down
                    *  then send it now!
                    */
                   if ( (fTimerSet == TRUE) &&
                        (mMouse == WM_LBUTTONDOWN ||
                         mMouse == WM_MBUTTONDOWN ||
                         mMouse == WM_LBUTTONDBLCLK ||
                         mMouse == WM_MBUTTONDBLCLK ||
                         mMouse == WM_RBUTTONDBLCLK ||
                         mMouse == WM_RBUTTONDOWN) ) {

                       KillTimer( hWnd, IDT_MOUSEMOVE );
                       fTimerSet = FALSE;

#ifdef WIN16
// Check and see if the alt-key needs to be sent before the mouse data
                       wKeyboardHookProc( mMouse, (WPARAM) 0 , (LPARAM) 0 );
#endif                   
                       MainWM_Mouse( hWnd, mMouse, xMouse, yMouse );
                   }

                   /*
                    *  Mouse capture logic
                    */
                   if ( message == WM_LBUTTONDOWN ||
                        message == WM_MBUTTONDOWN ||
                        message == WM_LBUTTONDBLCLK ||
                        message == WM_MBUTTONDBLCLK ||
                        message == WM_RBUTTONDBLCLK ||
                        message == WM_RBUTTONDOWN ) {

                        /*
                         *  Capture mouse only once
                         */
                        if ( !fMouseCaptured ) {
                            GetClientRect( hWnd, &rectCapture );
                            MapWindowPoints( hWnd, HWND_DESKTOP, (LPPOINT) &rectCapture, 2 );
                            ClipCursor( &rectCapture );
                            SetCapture(hWnd);
                            fMouseCaptured = TRUE;
                        }
 
                        /*
                         *  Never process first mouse button down, 
                         *  defer processing till timer or mouse message
                         */
                        if ( !fTimerSet ) {

                            /*
                             *  Start timer
                             */
                            SetTimer( hWnd, IDT_MOUSEMOVE, G_MouseTimer, NULL );
                            fTimerSet = TRUE;
                        }

                        /*
                         *  Save mouse button down
                         */
                        mMouse = message;
                        xMouse = LOWORD( lParam );
                        yMouse = HIWORD( lParam );
 
                        break;
                   }
                   else if ( message == WM_LBUTTONUP ||
                             message == WM_MBUTTONUP ||
                             message == WM_RBUTTONUP ) {
                        
                        WPARAM fwKeys = MK_LBUTTON | MK_MBUTTON | MK_RBUTTON;

                        /*
                         *  Remove our flag
                         */
                        if ( message == WM_LBUTTONUP )
                            fwKeys &= ~(MK_LBUTTON);
                        else if ( message == WM_MBUTTONUP )
                            fwKeys &= ~(MK_MBUTTON);
                        else if ( message == WM_RBUTTONUP )
                            fwKeys &= ~(MK_RBUTTON);

                        /*
                         *  Release mouse when all buttons are up
                         */
                        if ( !(wParam & fwKeys) ) {
                            ClipCursor( NULL );
                            ReleaseCapture();
                            fMouseCaptured = FALSE;
                        }
                   }
                   else if ( message == WM_MOUSEMOVE ) {

                        // take over for old mouse hook code
                        if(vhCursorVis)
                           SetCursor(vhCursorVis);

                        /*
                         *  Check if message is just a SetCursorPos echo
                         */
                        if ( ((INT) GetWindowLong(hWnd, GWL_MOUSE_X) == (INT) LOWORD(lParam)) &&
                             ((INT) GetWindowLong(hWnd, GWL_MOUSE_Y) == (INT) HIWORD(lParam)) ) {
 
                            SetWindowLong( hWnd, GWL_MOUSE_X, (LONG) -1 );
                            SetWindowLong( hWnd, GWL_MOUSE_Y, (LONG) -1 );
                            break;
                        }
 
                        /*
                         *  Check if we missed the SetCursorPos echo, 
                         *  collesing of mouse events could make this happen!
                         */
                        else if ( (GetWindowLong(hWnd, GWL_MOUSE_X) != (LONG) -1) ||
                                  (GetWindowLong(hWnd, GWL_MOUSE_Y) != (LONG) -1) ) {
                            SetWindowLong( hWnd, GWL_MOUSE_X, (LONG) -1 );
                            SetWindowLong( hWnd, GWL_MOUSE_Y, (LONG) -1 );
                        }
 
                        /*
                         *  Never process mouse movements, defer processing
                         */
                        if ( !fTimerSet ) {
                            SetTimer( hWnd, IDT_MOUSEMOVE, G_MouseTimer, NULL );
                            fTimerSet = TRUE;
                        }
 
                        /*
                         *  Save mouse position
                         */
                        mMouse = message;
                        xMouse = LOWORD( lParam );
                        yMouse = HIWORD( lParam );
 
                        break;
                   }

                   /*
                    *  Any moves queued?
                    */
                   if ( fTimerSet ) {

                       /*
                        *  Stop timer
                        */
                       KillTimer( hWnd, IDT_MOUSEMOVE );
                       fTimerSet = FALSE;
                   }
#endif

                   return( MainWM_Mouse( hWnd, message, LOWORD( lParam ),
                                         HIWORD( lParam ) ) );
                   break;
            }
         } // Fall through...
#ifndef DOS
         else {
            switch ( message ) {
                case WM_PAINT:
                    LocalPaint( hWnd );
                    break;
                // default: Fall through... 
            }
         }
#endif

      default:          // Passes it on if unproccessed
defwndproc:

#ifndef DOS
         /*
          * The plugin is telling us to go away
          */
         if ( message == g_MsgEngineUninit ) {
             DestroyWindow( hWnd );
             break;

         /*
          * The plugin window is changing
          */
         } else if ( message == g_MsgNewhWnd ) {
             g_hWndPlugin = (HWND)lParam;
             break;
         }
#endif

         return (DefWindowProc(hWnd, message, wParam, lParam));
         break;
   }

   return ( (LRESULT)0 );
}


/*******************************************************************************
 *
 *  Function: LocalPaint
 *
 *  Purpose: local desktop painting of window
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *
 *  Exit:
 *
 ******************************************************************************/
void
LocalPaint( HWND hWnd )   
{
    PAINTSTRUCT ps;
    HDC hdc  = BeginPaint( hWnd, &ps );
    RECT rect;

    GetClientRect( hWnd, &rect );
    FillRect( hdc, &rect, GetStockObject(WHITE_BRUSH) );
    EndPaint( hWnd, &ps );
}
#endif  // ifndef DOS


/*******************************************************************************
 *
 *  Function: MainWM_Char
 *
 *  Purpose: WM_CHAR processing for client window (MainWndProc)
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     CharCode (input)
 *        character code 
 *     RepeatCount (input)
 *        character repeat count
 *     fExtended (input)
 *        TRUE if extended character
 *
 *  Exit:
 *     0 to indicate WM_CHAR message was processed
 *
 ******************************************************************************/
LRESULT MainWM_Char( HWND hWnd, USHORT CharCode, USHORT RepeatCount, BOOL fExtended )
{
    USHORT i;
    USHORT cCh;
#define MAX_CHARS 10        // must be even!
    UCHAR aCh[MAX_CHARS];

    for ( i=0, cCh=0; i < RepeatCount; i++ ) {

        if ( fExtended ) {
            aCh[cCh++] = (UCHAR) 0xE0;
        }

        aCh[cCh++] = (UCHAR) CharCode;

        if ( cCh == MAX_CHARS ) {
            cCh = 0;
            if ( wdCharCode( aCh, MAX_CHARS ) ) {
                break;
            }
        }
    }

    //  flush 
    if ( cCh ) {
        (void) wdCharCode( aCh, cCh );
    }

    return( (LRESULT)0 );
}


#ifndef DOS
/*******************************************************************************
 *
 *  Function: SendScanCode
 *
 *  Purpose: send scan code down the pipe
 *
 *  Entry:
 *     ScanCode (input)
 *        scan code 
 *     RepeatCount (input)
 *        character repeat count
 *     fExtended (input)
 *        TRUE if extended character
 *
 *  Exit:
 *     nothing
 *
 ******************************************************************************/

#define MAX_SCANS 40        // must be even!

void 
SendScanCode( USHORT ScanCode, USHORT RepeatCount, BOOL fExtended )
{
    static USHORT cSc = 0;
    static UCHAR aSc[MAX_SCANS];
    ULONG CurrentTime;
    USHORT i;

    for ( i=0; i < RepeatCount; i++ ) {
    
        if ( fExtended ) 
            aSc[cSc++] = (UCHAR) 0xE0;
    
        aSc[cSc++] = (UCHAR) ScanCode;
    
        if ( cSc == MAX_SCANS ) {
            cSc = 0;
            if ( wdScanCode( aSc, MAX_SCANS ) ) 
                break;
        }
    }
    
    
    /* 
     *  If we have characters or the timer is armed 
     *  -- send characters to host every "G_KeyboardTimer" milli-seconds
     */
    if ( cSc > 0 || KeyboardTime > 0 ) {

        CurrentTime = Getmsec();
        if ( KeyboardTime == 0 ) 
            KeyboardTime = CurrentTime + G_KeyboardTimer;
    
        if ( CurrentTime >= KeyboardTime ) {
    
            if ( cSc > 0 ) {
                /* re-arm timer and write characters */
                KeyboardTime = CurrentTime + G_KeyboardTimer;
                (void) wdScanCode( aSc, cSc );
                cSc = 0;
            } else {
                /* cancel timer */
                KeyboardTime = 0;
            }
        }
    }
}


/*******************************************************************************
 *
 *  Function: FlushScanCode
 *
 *  Purpose: send all queued scan codes down the pipe
 *
 *  Entry:
 *     nothing
 *
 *  Exit:
 *     nothing
 *
 ******************************************************************************/

VOID 
FlushScanCode()
{
    KeyboardTime = 1;
    SendScanCode( 0, 0, FALSE );
}
#endif


/*******************************************************************************
 *
 *  Function: MainWM_SetFocus
 *
 *  Purpose: WM_SETFOCUS processing for client window (MainWndProc)
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 to indicate WM_SETFOCUS message was processed
 *
 ******************************************************************************/
LRESULT MainWM_SetFocus( VOID )
{
#ifndef DOS
   gShiftState     = 0;
   gTrueShiftState = 0;
   // WFCKeyHook( pLogProcedures, wKeyboardHookProc );
   (VOID) wdWindowSwitch(); // Allow WD to resync keyboard indicators
#endif
   TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: MainWM_SetFocus" ));

   return( (LRESULT)0 );
}

/*******************************************************************************
 *
 *  Function: MainWM_KillFocus
 *
 *  Purpose: WM_KILLFOCUS processing for client window (MainWndProc)
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 to indicate WM_KILLFOCUS message was processed
 *
 ******************************************************************************/
LRESULT MainWM_KillFocus( VOID )
{
#ifndef DOS
   INT rc;
   WDSETINFORMATION WdSetInfo;

   /*
    * If we're losing the focus with a shift key down, send the break
    */
   if ( gTrueShiftState ) {
       USHORT i = 0;
       UCHAR  pBreaks[6+2+1];
       if ( gTrueShiftState & KSS_LEFTSHIFT ) {
           pBreaks[i++] = 0xaa;
       }
       if ( gTrueShiftState & KSS_LEFTCTRL ) {
           pBreaks[i++] = 0x9d;
       }
       if ( gTrueShiftState & KSS_LEFTALT ) {
           pBreaks[i++] = 0xb8;
       }
       if ( gTrueShiftState & KSS_RIGHTSHIFT ) {
           pBreaks[i++] = 0xb6;
       }
       if ( gTrueShiftState & KSS_RIGHTCTRL ) {
           pBreaks[i++] = 0xe0;
           pBreaks[i++] = 0x9d;
       }
       if ( gTrueShiftState & KSS_RIGHTALT ) {
           pBreaks[i++] = 0xe0;
           pBreaks[i++] = 0xb8;
       }
       TRACE(( TC_WENG, TT_L1, "WFEngx.Exe: MainWM_KillFocus: Sending %u break(s)", i ));
       FlushScanCode();
       wdScanCode( pBreaks, i );
   }
   // WFCKeyUnhook();

    /*
     * Make sure there is a WD out there
     */
    if ( !( gState & WFES_LOADEDWD ) ) {
        goto done;
    }
 
   /*
    *  Notify the WD/VD of removal of the input focus
    */
   WdSetInfo.WdInformationClass = WdInactivate;
   if ( rc = ModuleCall( &gWdLink, WD$SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: MainWM_KillFocus: Error (%d) from WdSetInfo-WdInactivate", rc ));
   }

done:
#endif

   return( (LRESULT)0 );
}

/*******************************************************************************
 *
 *  Function: CreateMainWindow
 *
 *  Purpose: Create main windows client window
 *
 *  Entry:
 *     hInstance (input)
 *        instance handle
 *     nCmdShow (input)
 *        show window flag
 *     pWFEOpen (input)
 *        WFEOpen paramaters
 *
 *  Exit:
 *     window handle, or NULL if error occurred
 *
 ******************************************************************************/
HWND CreateMainWindow( HINSTANCE hInstance, INT nCmdShow,
                       PWFEOPEN pWFEOpen )
{
   HWND hWnd = NULL;

   // only register the window class if this is the first instance
#ifndef DOS
   if( gbIPCEngine || !ghPrevInstance) { 
#else
      {
#endif
      if ( !InitOnce(hInstance, pWFEOpen)) { // Initialize shared things
         TRACE(( TC_WENG, TT_ERROR,
                 "WEngx.Exe: InitOnce - window registration failed" ));
         ASSERT( 0, 0 );
         goto done;
         }
      }

   if (!(hWnd = InitInstance(hInstance, nCmdShow, pWFEOpen ))) { // Initialize instance things
      TRACE(( TC_WENG, TT_ERROR,
              "WEngx.Exe: InitInstance - window registration failed" ));
      ASSERT( 0, 0 );
      goto done;
   }

#ifdef WIN32
    // for plugin window, we need to watch to see if the parent window
    // is still alive.  If the browser GPFs, then we need to clean up.
    if(g_hWndPlugin) {
        DWORD dwThreadId;
        DWORD dwParentProcId=0;
  
        // get process id of plugin window
        dwThreadId = GetWindowThreadProcessId( g_hWndPlugin, &dwParentProcId);
        if(dwParentProcId) {
  
           // open handle to process to get exitcode info
           g_hParentProc = OpenProcess( PROCESS_QUERY_INFORMATION,
                                       FALSE,
                                       dwParentProcId);
  
           // set a periodic timer to check if parent still lives
           if(g_hParentProc)
               g_hWatchdogTimer = SetTimer( NULL, 
                                            0, 
                                            PARENT_WATCHDOG_TIMEOUT, 
                                            (TIMERPROC)ParentWatchdogTimer);
        }
    }

   // if we find the autodisconnect window, we need to start a timer
   // to notify the autodisconnect window when there is activity with
   // winsock
   if(FindWindow(AUTODISCONNECT_MONITOR_CLASS_NAME,NULL)) {

       g_hWinSockIdleTimer = SetTimer( NULL, 
                                       0, 
                                       WINSOCK_IDLE_TIMEOUT, 
                                       (TIMERPROC)WinSockIdleTimer);
      
   }
   
#endif

done:
   return( hWnd );
}

/*******************************************************************************
 *
 *  Function: InitOnce
 *
 *  Purpose: One time initialization
 *
 *  Entry:
 *     hInstance (input)
 *        instance handle
 *     pWFEOpen (input)
 *        WFEOpen paramaters
 *
 *  Exit:
 *     TRUE if successful, or FALSE if error occurred
 *
 ******************************************************************************/
BOOL InitOnce(HINSTANCE hInstance, PWFEOPEN pWFEOpen)
{
#ifdef DOS
   return( TRUE );
#else
   WNDCLASS  wc;

   /*
    * If launcher is API Version 2 or above, fetch the custom icon, if
    * specified.  We'll use the built-in icon if we can't get a custom one.
    */
   if ( pWFEOpen->Version >= 2 ) {

      if ( pWFEOpen->pszIconPath && *pWFEOpen->pszIconPath ) {
         ghCustomIcon = ExtractIcon( hInstance, 
                                     pWFEOpen->pszIconPath, 
                                     pWFEOpen->uIconIndex );
      }
   }

#ifndef DOS
   if(!gbIPCEngine) 
      lstrcpy( gpszClassName, WFENG_NONIPCCLASS );
   else
#endif
      wsprintf( gpszClassName, "w%s", gpszServiceConnection );

   // Fill in window class structure with parameters that describe the
   // main window.

   wc.style         = CS_OWNDC | CS_DBLCLKS;  // Class style(s).
   wc.lpfnWndProc   = (WNDPROC)MainWndProc;   // Window Procedure
   wc.cbClsExtra    = 0;                      // No per-class extra data.
   wc.cbWndExtra    = GWL_RESERVEDBYTES;      // Window desired extra data
   wc.hInstance     = hInstance;              // Owner of this class
   wc.hIcon         = (HICON) ghCustomIcon;
   wc.hCursor       = (HCURSOR) NULL;
   wc.hbrBackground = NULL;
   wc.lpszMenuName  = NULL;                   // Menu from .RC
   wc.lpszClassName = gpszClassName;          // Name to register as

   TRACE(( TC_WENG, TT_L1, "WEngx.Exe InitOnce: hInstance(%08lX) class(%s) custom icon(%08lX)",
           (ULONG)hInstance, wc.lpszClassName, ghCustomIcon ));

   // Register the window class and return success/failure code.
   return ( RegisterClass(&wc) );
#endif
}

/*******************************************************************************
 *
 *  Function: InitInstance
 *
 *  Purpose: Per-instance initialization
 *
 *  Entry:
 *     hInstance (input)
 *        instance handle
 *     nCmdShow (input)
 *        ShowWindow flags
 *     pWFEOpen (input)
 *        WFEOpen paramaters
 *
 *  Exit:
 *     client window handle, or NULL if error occurred
 *
 ******************************************************************************/
HWND InitInstance( HINSTANCE hInstance, INT nCmdShow,
                   PWFEOPEN pWFEOpen )
{
    HWND hWnd = (HWND)TRUE; 
    PWFEINSTANCE pInstanceData = NULL;
    BOOL fDestroyWindow = FALSE;
#ifndef DOS
    HMENU hSysMenu=NULL;
#endif

#ifndef DOS
    INT  x=0, y=0, cx, cy;
#ifdef WIN32
    BOOL bResult;
    RECT rWorkArea;
#endif
    INT  cxScreen = GetSystemMetrics(SM_CXSCREEN);
    INT  cyScreen = GetSystemMetrics(SM_CYSCREEN);
    INT  uWidth, uHeight;

    DWORD WindowStyle;
    HWND  hWndParent; 

    INT cxBorder;
    INT cyBorder;
    INT cyTitle; 
#ifdef WIN32
    OSVERSIONINFO OSVersionInfo;
#endif          

   // only look for plugin window parent if API version 4 or above
   if((pWFEOpen->Version >= 4) && pWFEOpen->reserved) {
       hWndParent = ((PWFEOPENP)pWFEOpen->reserved)->hWndPlugin;
   }
   else
       hWndParent = (HWND)NULL;


#ifdef WIN32
   OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   if(GetVersionEx(&OSVersionInfo) && (OSVersionInfo.dwMajorVersion >= 4)) {
      cxBorder = 2 * (GetSystemMetrics(SM_CXEDGE)+GetSystemMetrics(SM_CXBORDER));
      cyBorder = 2 * (GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYBORDER));
      cyTitle  = GetSystemMetrics(SM_CYCAPTION);   
      }
   else
#endif
       {
       cxBorder = 2 * (GetSystemMetrics(SM_CXBORDER));
       cyBorder = 2 * (GetSystemMetrics(SM_CYBORDER));
       cyTitle  = GetSystemMetrics(SM_CYCAPTION)-1;
       }

#ifdef WIN32
    // Get the limits of the "work area".
    bResult = SystemParametersInfo( SPI_GETWORKAREA, sizeof(RECT), &rWorkArea, 0);
    if ( bResult) {
       cxScreen = rWorkArea.right - rWorkArea.left;
       cyScreen = rWorkArea.bottom - rWorkArea.top;
       x = rWorkArea.left;
       y = rWorkArea.top;
       }
#endif

    TRACE(( TC_WENG, TT_L1, "WEngx.Exe InitInstance: hWndParent(%X)", hWndParent ));

    /*      
     *  verify client size
     */
    if ( !(uWidth  = (INT)pWFEOpen->uInitialWidth) || 
         !(uHeight = (INT)pWFEOpen->uInitialHeight) ||
         (uWidth > MAX_DESIREDHRES) ||
         (uHeight > MAX_DESIREDVRES) ||
         (uHeight < 0) || (uWidth  < 0) ) {

        /*
         *  Size (-1,-1) is key to full screen
         */
        if ( (((USHORT)uWidth  & 0xffff) == 0xffff) && 
             (((USHORT)uHeight & 0xffff) == 0xffff) ) {
            uWidth  = GetSystemMetrics(SM_CXSCREEN); 
            uHeight = GetSystemMetrics(SM_CYSCREEN); 
            gfRemoveTitleBar = TRUE;
        }
        else {
            uWidth  = DEF_DESIREDHRES; 
            uHeight = DEF_DESIREDVRES; 
        }
    }

    /*
     *  Caption?
     */
    if ( !(pWFEOpen->pszTitle) || !*pWFEOpen->pszTitle ) {
       gszTitle = gszDefTitle;
       }
    else
       gszTitle = strdup(pWFEOpen->pszTitle);

   lstrcpy( gpszTitle, gszTitle ); 


    if ( hWndParent ) {
        /*                                  
         * Embedded in a browser window     
         */                                 

        // Border enabled
        if(g_border == TRUE) {
           cxBorder = 2 * (GetSystemMetrics(SM_CXBORDER)); 
           cyBorder = 2 * (GetSystemMetrics(SM_CYBORDER)); 
           WindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
                         WS_CLIPCHILDREN | WS_BORDER;
           }
        // border disabled
        else {
           cxBorder = 0;
           cyBorder = 0;
           WindowStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | 
                         WS_CLIPCHILDREN ;
           }

        x = y = 0;
        cyTitle  = 0;
        vcxSave = vcxBorder = x;
        vcySave = vcyBorder = y;
        cx = uWidth + cxBorder; 
        cy = uHeight + cyBorder;
#ifdef DEBUG
        {
            RECT rcl;
            GetWindowRect( hWndParent, &rcl );
            TRACE(( TC_WENG, TT_L1, "Plugin Parent: l(%d) r(%d) t(%d) b(%d)",
                     rcl.left, rcl.right, rcl.top, rcl.bottom ));
            GetClientRect( hWndParent, &rcl );
            TRACE(( TC_WENG, TT_L1, "Plugin Parent client: l(%d) r(%d) t(%d) b(%d)",
                     rcl.left, rcl.right, rcl.top, rcl.bottom ));
        }
#endif
    } else {
        /*
         * In our own window
         */
        WindowStyle = WS_CAPTION | WS_SYSMENU /*| WS_VISIBLE*/ | WS_MINIMIZEBOX;

        /*
         * Remove title bar and restrict to max window size
         */
        if ( gfRemoveTitleBar ) {
            uWidth  = (uWidth>cxScreen)  ? cxScreen : uWidth;
            uHeight = (uHeight>cyScreen) ? cyScreen : uHeight;
        }
        else {
            uWidth  = (uWidth>cxScreen) ? cxScreen : uWidth;
            uHeight = ((uHeight+cyTitle) > cyScreen) ? (cyScreen-cyTitle) : uHeight;
        }
    
        /*
         *  Add in border size and title
         */
        cx = uWidth  + cxBorder;
    
        cy = uHeight + cyTitle + cyBorder;
    
        /*
         *  Center window on desktop
         */
        if(x==0)
           x = (cxScreen - cx) /2;
    
        if(y==0)
           y = (cyScreen - cy) /2;
    
    
        /*
         *  Hide border on full screen
         */
        if ( x < 0 ) 
           x  = - (cxBorder/2); 
    
        if ( y < 0 ) 
           y  = - (cyBorder/2); 
     
        /*
         *  Save original position
         */
        vcxSave = vcxBorder = x;
        vcySave = vcyBorder = y;
    }

    /*
     * Create client window
     */
    hWnd = CreateWindow(
            gpszClassName,         // See RegisterClass() call.
            gpszTitle,             // Text for window title bar.
            WindowStyle,
            x, y, cx, cy,      
            hWndParent,
            NULL,                // Use the window class menu.
            hInstance,           // This instance owns this window.
            NULL                 // We don't use any data in our WM_CREATE
    );
 

    // If window could not be created, return "failure"
    if ( !hWnd ) {
       goto done;
    }

#ifdef DEBUG
        {
            RECT rcl;
            GetWindowRect( hWnd, &rcl );
            TRACE(( TC_WENG, TT_L1, "Client rect: l(%d) r(%d) t(%d) b(%d)",
                     rcl.left, rcl.right, rcl.top, rcl.bottom ));
            TRACE(( TC_WENG, TT_L1, "Created: x(%d) y(%d) cx(%d) cy(%d)",
                     x, y, cx, cy ));
        }
#endif

    // enable the Status system menu item
    hSysMenu = GetSystemMenu( hWnd, FALSE);
    if(hSysMenu) {
       #define MAX_MENULEN  100
       UCHAR szBuf[MAX_MENULEN];
       EMGetStringResource( IDS_MENU_CONNECT_STATUS,szBuf,sizeof(szBuf));
       InsertMenu( hSysMenu,
                   SC_CLOSE,
                   MF_BYCOMMAND | MF_ENABLED | MF_STRING,
                   IDM_CLIENT_STATUS,
                   szBuf);

       InsertMenu( hSysMenu,
                   SC_CLOSE,
                   MF_SEPARATOR | MF_BYCOMMAND,
                   0,
                   NULL);
       }

#ifdef HEARTBEAT_ENABLED
    // timer for changing bitmap
    SetTimer(hWnd, IDB_BITMAP1, BITMAP_MSEC, NULL);
#endif //HEARTBEAT_ENABLED

    /*
     *  Save current client size and border info
     */
    SetWindowLong( hWnd, GWL_WINDOWWIDTH,  (LONG)uWidth );
    SetWindowLong( hWnd, GWL_WINDOWHEIGHT, (LONG)uHeight );
    SetWindowLong( hWnd, GWL_CXBORDER,     (LONG)cxBorder );
    SetWindowLong( hWnd, GWL_CYBORDER,     (LONG)cyBorder);
    SetWindowLong( hWnd, GWL_CYTITLE,      (LONG)cyTitle);
    SetWindowLong( hWnd, GWL_TITLEPRESENT, (LONG)TRUE );
#endif

    /*
     * Copy the WFEngOpen parameters for our instance data
     */
    if ( !(pInstanceData = (PWFEINSTANCE)WinAlloc( sizeof( WFEINSTANCE ) ) ) ) {
        fDestroyWindow = TRUE;
        goto done;
    }
    gbVdAddrToWd = FALSE;
#ifndef DOS
    pInstanceData->hWndPlugin = hWndParent;

    /*
     * Tell the plugin window who we are
     */
    if ( hWndParent && g_MsgEngineInit ) {
        SendMessage( hWndParent, g_MsgEngineInit, (WPARAM)0, (LPARAM)hWnd );
    }
#endif
    SetWindowLong( hWnd, GWL_INSTANCEDATA, (LONG)pInstanceData );

    TRACE(( TC_WENG, TT_L1,
            "WEngx.Exe: Initinstance: hWnd(%08lX) pInstanceData(%08lX)",
              (ULONG)hWnd, (ULONG)pInstanceData ));

done:
#ifndef DOS
    if ( fDestroyWindow ) {
        DestroyWindow( hWnd );
        hWnd = NULL;
    }
#endif
    return ( hWnd );
}

#ifndef WIN32
/*******************************************************************************
 *
 *  Function: GetLastError
 *
 *  Purpose: Win16 doesn't give you a last error so there's not much we can do
 *           other than say something like a out of memory error occurred
 *
 *  Entry:
 *     void
 *
 *  Exit:
 *     error code
 *
 ******************************************************************************/
DWORD GetLastError( VOID )
{
    return( (DWORD)CLIENT_ERROR_NO_MEMORY );
}
#endif

/*******************************************************************************
 *
 *  Function: MainWM_Mouse
 *
 *  Purpose: Mouse message processing for client window (MainWndProc)
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *     message (input)
 *        mouse event
 *     xPos (input)
 *        x-cordinate of mouse hot spot (client coordinate space)
 *     yPos (input)
 *        y-cordinate of mouse hot spot (client coordinate space) 
 *
 *  Exit:
 *     0 to indicate mouse message was processed
 *
 ******************************************************************************/
LRESULT MainWM_Mouse( HWND hWnd, UINT message, USHORT xPos, USHORT yPos )
{
    MOUSEDATA MouseData;
    ULONG ulWidth  = GetWindowLong( hWnd, GWL_WINDOWWIDTH );
    ULONG ulHeight = GetWindowLong( hWnd, GWL_WINDOWHEIGHT );

    /*
     *  Only move when there is a client area
     */
    if ( ulWidth == 0 || ulHeight == 0)
        return( (LRESULT) 0 );

    /*
     *  As per Thanh add one for round off
     */
    MouseData.X = (USHORT)(((ULONG)xPos * (ULONG)0x10000) / ulWidth) + 1;
    MouseData.Y = (USHORT)(((ULONG)yPos * (ULONG)0x10000) / ulHeight) + 1;

    switch ( message ) {
       case WM_MOUSEMOVE:
           MouseData.cMouState = MOU_STATUS_MOVED;
           break;

       case WM_LBUTTONDOWN:
           MouseData.cMouState = MOU_STATUS_B1DOWN;
           break;

       case WM_LBUTTONUP:
           MouseData.cMouState = MOU_STATUS_B1UP;
           break;

       case WM_RBUTTONDOWN:
           MouseData.cMouState = MOU_STATUS_B2DOWN;
           break;

       case WM_RBUTTONUP:
           MouseData.cMouState = MOU_STATUS_B2UP;
           break;

       case WM_MBUTTONDOWN:
           MouseData.cMouState = MOU_STATUS_B3DOWN;
           break;

       case WM_MBUTTONUP:
           MouseData.cMouState = MOU_STATUS_B3UP;
           break;

       case WM_LBUTTONDBLCLK:
           MouseData.cMouState = MOU_STATUS_B1DOWN | MOU_STATUS_DBLCLK; 
           break;

       case WM_RBUTTONDBLCLK:
           MouseData.cMouState = MOU_STATUS_B2DOWN | MOU_STATUS_DBLCLK; 
           break;

       case WM_MBUTTONDBLCLK:
           MouseData.cMouState = MOU_STATUS_B3DOWN | MOU_STATUS_DBLCLK; 
           break;

    }

    wdMouseData( (LPVOID)&MouseData, 1 );

    TRACE(( TC_WENG, TT_L2,
            "WEngx.Exe: MainWM_Mouse - (%u, %u) -> (%u, %u) w:(%u) h:(%u) msg(%04X)",
            xPos, yPos, MouseData.X, MouseData.Y, (INT) ulWidth, (INT) ulHeight,
            message ));

    return( (LRESULT)0 );
}

#ifndef DOS
/*******************************************************************************
 *
 * wKeyboardHookProc
 *
 * Worker for keyboard hook procedure
 *
 * Note: Hook procedures for system events must reside in a DLL.
 *       This worker routine is used to service the real keyboard
 *       hook procedure in WFCKEY.DLL
 *
 * ENTRY:
 *    code (input)
 *       process message flag
 *    wParam (input)
 *       virtual key code
 *    lParam (input)
 *       keyboard message information
 *
 * EXIT:
 *    0 - let system process, 1 - discard message
 *
 * NOTES:
 *    Windows hack:  When both shift keys are pressed and released
 *    windows only calls the keyboard hook procedure with the scan
 *    code of the last shift key released.  The sendShiftScan,
 *    fLShiftDown, and fRShiftDown variables are used to detect
 *    this condition and send a release scan code for the other
 *    shift key.
 ******************************************************************************/

#define KEY_RELEASED            0x80000000L
#define KEY_EXTENDED            0x01000000L
#define VK_ALT                  VK_MENU

BOOL WFCAPI 
wKeyboardHookProc( INT code, WPARAM wParam, LPARAM lParam )
{
    BOOL         fEatMessage = TRUE;
    INT          i;
    BOOL         fRightKey;
    INT          Hotkey;
    UCHAR        ScanCode    = (UCHAR)((lParam >> 16) & 0xFF);
    USHORT       RepeatCount = (USHORT)(lParam & 0xFFFF);
    BOOL         fExtended   = !!(lParam & KEY_EXTENDED);
    BOOL         fBreak      = !!(lParam & KEY_RELEASED);
    BOOL         fSendKey    = TRUE;
    UCHAR        sendShiftScan = 0;
    static BOOL  fLShiftDown = 0;
    static BOOL  fRShiftDown = 0;
    KBDCLASS     KbdMode;
    static BOOL   fSentAltKey = FALSE;
#ifdef WIN16
    static BOOL   fHoldAltKey = FALSE;
    static UCHAR  lastScanCode;
    static USHORT lastRepeatCount;
    static BOOL   lastfExtended;
#endif

#ifdef WIN32
    //  ugly WinNT hack
    if ( (ScanCode & 0x80) ) {
        return( fEatMessage );
    }

#endif


#ifdef WIN16   
    // Code is called to send the alt-key before a mouse button click is sent
    // Problem occured with alt-dblclick.  Alt-key wasn't being sent    scottc
    if( (code == WM_LBUTTONDOWN) || (code == WM_MBUTTONDOWN) || (code == WM_RBUTTONDOWN) ) {
    
           if ( fHoldAltKey ) {
               fHoldAltKey = FALSE;
               SendScanCode( lastScanCode, lastRepeatCount, lastfExtended );
           }
           return( FALSE );
    }
#endif
    
    /*
     * Update shift state
     */
    switch ( wParam ) {

        //  numlock is not really an extended key
        case VK_NUMLOCK:
            fExtended = FALSE;
            break;

        case VK_ALT:
        case VK_SHIFT:
        case VK_CONTROL:
            i = wParam - VK_SHIFT; // 0-based index in gStates table

            /*
             * If a right scancode is not specified, that means the
             * right key is an extended key with the same scancode
             */
            if ( gStates[i].rightScan ) {

                fRightKey = ( ScanCode == gStates[i].rightScan );
            } else {
                fRightKey = fExtended;
            }

            if ( fBreak ) {
               if ( fRightKey ) {
                   gTrueShiftState &= ~gStates[i].right;
                   if ( wParam == VK_SHIFT ) {
                       fRShiftDown = FALSE;
                       if ( fLShiftDown ) {
                           sendShiftScan = KBD_LSHIFT_UP_CODE;
                           gTrueShiftState &= ~gStates[i].left;
                           fLShiftDown = FALSE;
                       }
                   }
               } else {
                   gTrueShiftState &= ~gStates[i].left;
                   if ( wParam == VK_SHIFT ) {
                       fLShiftDown = FALSE;
                       if ( fRShiftDown ) {
                           sendShiftScan = KBD_RSHIFT_UP_CODE;
                           gTrueShiftState &= ~gStates[i].right;
                           fRShiftDown = FALSE;
                       }
                   }
               }
            } else {
               if ( fRightKey ) {
                   gTrueShiftState |= gStates[i].right;
                   if ( wParam == VK_SHIFT ) {
                       fRShiftDown = TRUE;
                   }
               } else {
                   gTrueShiftState |= gStates[i].left;
                   if ( wParam == VK_SHIFT ) {
                       fLShiftDown = TRUE;
                   }
               }
            }
            gShiftState = gTrueShiftState;
            if ( gShiftState & (gStates[i].left | gStates[i].right) ) {
                gShiftState |= gStates[i].either;
            }
            break;

        case VK_SNAPSHOT:
            {
               UCHAR pszPrintScreen[] = "\xe0\x2a\xe0\x37\xe0\xb7\xe0\xaa";
#ifdef WIN16
                if ( fHoldAltKey ) {
                    fHoldAltKey = FALSE;
                    SendScanCode( lastScanCode, lastRepeatCount, lastfExtended );
                }
#endif               
               TRACE(( TC_WENG, TT_L1,
                       "WFEngx.Exe: wKeyboardHookProc: Print Screen key detected" ));
               FlushScanCode();
               wdScanCode( pszPrintScreen, (USHORT)strlen(pszPrintScreen) );
               fSendKey = FALSE;
            }
            break;

        case VK_PAUSE :
            {
               UCHAR pszPause[] = "\xe1\x1d\x45\xe1\x9d\xc5";

               TRACE(( TC_WENG, TT_L1,
                       "WFEngx.Exe: wKeyboardHookProc: Pause key detected" ));
               FlushScanCode();
               wdScanCode( pszPause, (USHORT)strlen(pszPause) );
               fSendKey = FALSE;
            }
            break;
    }


    TRACE(( TC_WENG, TT_L1,
            "wKeyboardHookProc: code(%d) wParam(%04X) lParam(%08lX) scan(%02X) truestate(%04X) state(%04X)",
            code, wParam, lParam, ScanCode, gTrueShiftState, gShiftState ));

    /*
     * See if we got a hotkey
     */
    if ( fBreak ) {
        ScanCode |= 0x80; // Turn the scancode into a break code
    } else {
#ifndef DOS
       if(gbIPCEngine) 
#endif
          if ( (Hotkey = KbdCheckHotkey( ScanCode, (INT)gShiftState )) ) {
#ifdef WIN16
           if ( fHoldAltKey ) {
               fHoldAltKey = FALSE;
               SendScanCode( lastScanCode, lastRepeatCount, lastfExtended );
           }
#endif
              StatusMsgProc( ghWFE, Hotkey );
              TRACE(( TC_WENG, TT_L1,
                      "WFEngx.Exe: wKeyboardHookProc: Hotkey detected(%d)",
                       Hotkey ));
              
              fSendKey = FALSE;
          }
    } 

    /*
     *  TTY Mode or SCAN Mode
     */
    KbdGetMode( &KbdMode );

    /*
     *  TTY Mode
     */
    if ( KbdMode == Kbd_Ascii ) {
        fEatMessage = FALSE;
    }

    /*
     * SCAN Mode
     */
    else {

#ifdef WIN16
        /*
         *  Check for ALT-TAB on WIN16
         */
        if ( (wParam == VK_ALT) && (fBreak != TRUE) ) {
            fHoldAltKey     = TRUE;
            fSendKey        = FALSE;
            fEatMessage     = FALSE;
            lastScanCode    = ScanCode;
            lastRepeatCount = RepeatCount;
            lastfExtended   = fExtended;
        }
        
        if ( wParam == VK_TAB ){
            if ( (GetKeyState(VK_ALT) & (SHORT) 0x8000) &&
                !(GetKeyState(VK_CONTROL) & (SHORT) 0x8000) ) {
                fHoldAltKey = FALSE;
                fSendKey    = FALSE;
                fEatMessage = FALSE;
                fSentAltKey = TRUE;
            }
        }
        else if ( wParam == VK_ESCAPE ) {
            if ( (GetKeyState(VK_ALT) & (SHORT) 0x8000) &&
                !(GetKeyState(VK_CONTROL) & (SHORT) 0x8000) ) {
                fSendKey    = FALSE;
                fEatMessage = FALSE;
                fSentAltKey = TRUE;
            }
        }
        else if ( (wParam == VK_ALT) && (fSentAltKey == TRUE) ) {
            fHoldAltKey = FALSE;
            fSendKey    = FALSE;
            fEatMessage = FALSE;
            fSentAltKey = FALSE;
            SetFocus(NULL);
        }
        else if ( wParam == 0 ) {
            fSentAltKey = FALSE;
        }
        else if ( fSendKey && fHoldAltKey ) {
            fHoldAltKey = FALSE;
            SendScanCode( lastScanCode, lastRepeatCount, lastfExtended );
        }

#endif
        /*
         * Send the scan code to the host
         */
        if ( fSendKey && !fSentAltKey) {
            SendScanCode( ScanCode, RepeatCount, fExtended );
            if ( sendShiftScan ) {
                SendScanCode( sendShiftScan, 1, FALSE );
            }
        }
    }

    return( fEatMessage );
}


/*******************************************************************************
 *
 *  Function: MainWM_PaletteChanged
 *
 *  Purpose:  Notify client to realize it's palette, background
 *
 *  Entry:
 *
 *  Exit:
 *
 ******************************************************************************/

LRESULT 
MainWM_PaletteChanged()
{
    UINT cChanged = 0;
    INT  rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
  
    TRACE(( TC_WENG, TT_L2, "WEngx.Exe: MainWM_PaletteChanged" ));
  
    /*
     * Make sure there is a WD out there
     */
    if ( !( gState & WFES_LOADEDWD ) ) {
        goto done;
    }
 
    WdSetInfo.WdInformationClass  = WdRealizePaletteBG;
    WdSetInfo.pWdInformation      = (LPVOID)&cChanged;
    WdSetInfo.WdInformationLength = sizeof(UINT);
    if ( rc = ModuleCall( &gWdLink, WD$SETINFORMATION, &WdSetInfo ) ) {
        TRACE(( TC_WENG, TT_ERROR,
           "WEngx.Exe: MainWM_PaletteChanged: Error (%d) from WdSetInfo-WdRealizePaletteBG", rc ));
        goto done;
    }
  
done:
    return( (LRESULT)cChanged );
}


/*******************************************************************************
 *
 *  Function: MainWM_QueryNewPalette
 *
 *  Purpose:  Notify client to realize it's palette, foreground
 *
 *  Entry:
 *
 *  Exit:
 *
 ******************************************************************************/

LRESULT 
MainWM_QueryNewPalette()
{
    UINT cChanged = 0;
    INT  rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
  
    TRACE(( TC_WENG, TT_L2, "WEngx.Exe: MainWM_QueryNewPalette" ));
  
    /*
     * Make sure there is a WD out there
     */
    if ( !( gState & WFES_LOADEDWD ) ) {
        goto done;
    }
 
    WdSetInfo.WdInformationClass  = WdRealizePaletteFG;
    WdSetInfo.pWdInformation      = (LPVOID)&cChanged;
    WdSetInfo.WdInformationLength = sizeof(UINT);
    if ( rc = ModuleCall( &gWdLink, WD$SETINFORMATION, &WdSetInfo ) ) {
        TRACE(( TC_WENG, TT_ERROR,
           "WEngx.Exe: MainWM_QueryNewPalette: Error (%d) from WdSetInfo-WdRealizePaletteFG", rc ));
        goto done;
    }
  
done:
    return( (LRESULT)cChanged );
}

/* 
 * ClientStatDialogTimer
 *
 * Timer event handler for client status dialog
 *
 */
void CALLBACK ClientStatDialogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime)
{
   if(iEvent == IDD_CLIENTSTATDLG) {
      UpdateClientStatDlg(GetParent(hdlg),FALSE);
      }
}

/*
 * ClientStatDialogProc
 *
 * Client Status dialog procedure
 */
BOOL CALLBACK ClientStatDialogProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
   BOOL bOurMsg=FALSE;

   switch(msg) {
       case WM_INITDIALOG:
           {
#define MAX_VERSTRING 25
              char szVer[MAX_VERSTRING];
              char szFileVer[MAX_VERSTRING];
              char szVerStr[MAX_VERSTRING*2];
              DOMAIN szServer;
              DOMAIN szDomain;
              USERNAME szUsername;
              char szDomainAndUsername[ DOMAIN_LENGTH + 2 + USERNAME_LENGTH + 1 ];
              char szUnknown[256];
              DWORD dwHandle;
              DWORD cbVer;
              UINT  uTemp;

              // change the version tag based on current version
              EMGetStringResource( IDS_CLIENTVERSION,szVer,sizeof(szVer));

              // get the version of the file to include in string
              szFileVer[0] = '\0';
              cbVer = GetFileVersionInfoSize( gszExeName, (DWORD *)&dwHandle);
              if(cbVer) {
                 void *pBuf;

                 pBuf = (void *)WinAlloc((UINT) cbVer);
                 if(pBuf) {
                    VS_FIXEDFILEINFO *pVerFileInfo;

                    if(GetFileVersionInfo ( gszExeName, dwHandle, cbVer, pBuf)) {

                       uTemp = sizeof(VS_FIXEDFILEINFO);
                       if(VerQueryValue( pBuf, "\\", (void **)&pVerFileInfo, &uTemp)) {
                          wsprintf(szFileVer, "%u.%u.%u.%u",
                                   HIWORD(pVerFileInfo->dwFileVersionMS),
                                   LOWORD(pVerFileInfo->dwFileVersionMS),
                                   LOWORD(pVerFileInfo->dwFileVersionLS),
                                   HIWORD(pVerFileInfo->dwFileVersionLS));

                          }

                       }
                    WinFree(pBuf);
                    }

                 }
              wsprintf(szVerStr, szVer, szFileVer);
              SetDlgItemText( hdlg, IDC_CLIENTVERSION, szVerStr);

              // get server, domain, and username and output
              EMGetStringResource(IDS_UNKNOWN,szUnknown,sizeof(szUnknown));
              wdGetClientDataServer(szServer, sizeof(szServer));
              wdGetClientDataDomain(szDomain, sizeof(szDomain));
              wdGetClientDataUsername(szUsername, sizeof(szUsername));

              SetDlgItemText( hdlg, IDC_CONNECTEDON, *szServer ? szServer : szUnknown );
              wsprintf(szDomainAndUsername, "%s\\%s", szDomain, szUsername);
              SetDlgItemText( hdlg, IDC_CONNECTEDAS, *szUsername ? szDomainAndUsername : szUnknown );
              SetDlgItemText( hdlg, IDC_ENCRYPTIONLEVELSESSION, *szUsername ? g_szEncryptionLevelConnStatus: szUnknown);
           }

           // center dialog in parent window
           CenterPopup(hdlg,GetParent(hdlg));
           break;

       case WM_COMMAND:
           switch(wParam) {
               case IDOK:
                  DestroyClientStatDlg(GetParent(hdlg));
                  return(TRUE);
                  break;

               case IDC_RESETCOUNT:
                   UpdateClientStatDlg(GetParent(hdlg), TRUE);
                   break;

               default:
                   break;
               }
           break;

       case WM_SYSCOMMAND:
           if(wParam == SC_CLOSE ) {
              DestroyClientStatDlg(GetParent(hdlg));
              }
           break;

       case WM_NCLBUTTONDOWN:
#ifdef WIN32
           fDragOn = TRUE;
#endif
           DefWindowProc( hdlg, msg, wParam, lParam );
           bOurMsg = TRUE;
#ifdef WIN32
           fDragOn = FALSE;
#endif
           break;
           
       default:
           break;
       }

   return(bOurMsg);
}

/*
 * CreateClientStatDlg
 *
 * Create the client status dialog
 *
 */
void CreateClientStatDlg(HWND hWnd) 
{

   // create the status dialog box
#ifdef WIN16
   lpfnClientStatDlgProc = MakeProcInstance(ClientStatDialogProc,ghInstance);
#endif
#ifdef WIN32
   ghClientStatDlg = CreateDialog(ghInstance, 
                                  MAKEINTRESOURCE(IDD_CLIENTSTATDLG),
                                  hWnd,
                                  ClientStatDialogProc);
#else
   ghClientStatDlg = CreateDialog(ghInstance, 
                                  MAKEINTRESOURCE(IDD_CLIENTSTATDLG),
                                  hWnd,
                                  lpfnClientStatDlgProc);
#endif
   if(ghClientStatDlg) {
      // start timer
      ghClientStatTimer = SetTimer(ghClientStatDlg, 
                                   IDD_CLIENTSTATDLG, 
                                   CLIENTSTAT_MSEC, 
                                   (TIMERPROC)ClientStatDialogTimer);

      // show the status dialog
      ShowWindow(ghClientStatDlg,SW_SHOW);
      UpdateWindow(ghClientStatDlg);
      }

   // turn on the check mark
   CheckMenuItem(GetSystemMenu(hWnd,FALSE), 
                 IDM_CLIENT_STATUS,
                 MF_BYCOMMAND | MF_CHECKED);

   // create a green brush for LEDs
   ghLEDBrush = CreateSolidBrush(RGB(0,255,0));

   // init the previous IO status to zero
   memset(&PrevIOStatus,0,sizeof(IOSTATUS));

   // mark dialog as being active
   gbClientStatusDlg = TRUE;
}

/*
 * DestroyClientStatDlg
 *
 */
void DestroyClientStatDlg(HWND hWnd) 
{

    // kill the timer
    if(ghClientStatTimer) {
       KillTimer(ghClientStatDlg, IDD_CLIENTSTATDLG);
       ghClientStatTimer = 0;
       }

    // kill the red brush
    if(ghLEDBrush) {
       DeleteObject(ghLEDBrush);
       }

    // kill the dialog
    if(ghClientStatDlg) {
       DestroyWindow(ghClientStatDlg);
       ghClientStatDlg = NULL;
       }

#ifdef WIN16
    // destroy the proc instance
    if(lpfnClientStatDlgProc) {
       FreeProcInstance(lpfnClientStatDlgProc);
       lpfnClientStatDlgProc = NULL;
       }
#endif
       

    CheckMenuItem(GetSystemMenu(ghWFE,FALSE), 
                  IDM_CLIENT_STATUS,
                  MF_BYCOMMAND | MF_UNCHECKED);

    // invalidate parent to force redraw
    InvalidateRgn( ghWFE, NULL, TRUE);

    gbClientStatusDlg = FALSE;
}

/*
 * UpdateClientStatDlg
 *
 * Update client status dialog
 *
 */
void UpdateClientStatDlg(HWND hWnd, BOOL bReset)
{
   CHAR szBuf[12];
   IOSTATUS IOStatus;
   INT rc = CLIENT_STATUS_SUCCESS;
   BOOL bBytesSent=FALSE;
   BOOL bBytesRecv=FALSE;

   // if user requested reset, capture current status for new base numbers
   if(bReset) {
      GetIOStatus(&ResetIOStatus);

      // init the previous IO status to zero
      memset(&PrevIOStatus,0,sizeof(IOSTATUS));
      }

   /*
    * Get current IOStatus
    */
   rc = GetIOStatus(&IOStatus);
   if(rc != CLIENT_STATUS_SUCCESS)
      return;


   if(ghClientStatDlg) {

      // update the sent and received fields
      if(PrevIOStatus.BytesRecv != IOStatus.BytesRecv) {
         ULONG BytesRecv = IOStatus.BytesRecv - ResetIOStatus.BytesRecv;
         ULONG FramesRecv = IOStatus.FramesRecv - ResetIOStatus.FramesRecv;
         wsprintf(szBuf,"%lu",BytesRecv);
         SetDlgItemText(ghClientStatDlg, IDC_TOTALRECV, szBuf);
         PrevIOStatus.BytesRecv = IOStatus.BytesRecv;
         if(FramesRecv==0) 
            FramesRecv++;
         wsprintf(szBuf,"%lu",BytesRecv/FramesRecv);
         SetDlgItemText(ghClientStatDlg, IDC_RCVBYTESFRAME, szBuf);
         bBytesRecv = TRUE;
         }
      
      if(PrevIOStatus.BytesSent != IOStatus.BytesSent) {
         ULONG BytesSent = IOStatus.BytesSent - ResetIOStatus.BytesSent;
         ULONG FramesSent = IOStatus.FramesSent - ResetIOStatus.FramesSent;
         wsprintf(szBuf,"%lu",BytesSent);
         SetDlgItemText(ghClientStatDlg, IDC_TOTALSENT, szBuf);
         PrevIOStatus.BytesSent = IOStatus.BytesSent;
         if(FramesSent==0) 
            FramesSent++;
         wsprintf(szBuf,"%lu",BytesSent/FramesSent);
         SetDlgItemText(ghClientStatDlg, IDC_SNDBYTESFRAME, szBuf);
         bBytesSent = TRUE;
         }

      // update the packets fields
      if(PrevIOStatus.FramesSent != IOStatus.FramesSent) {
         ULONG FramesSent = IOStatus.FramesSent - ResetIOStatus.FramesSent;
         wsprintf(szBuf,"%lu",FramesSent);
         SetDlgItemText(ghClientStatDlg, IDC_TOTALPKTSENT, szBuf);
         PrevIOStatus.FramesSent = IOStatus.FramesSent;
         }
      
      if(PrevIOStatus.FramesRecv != IOStatus.FramesRecv) {
         ULONG FramesRecv = IOStatus.FramesRecv - ResetIOStatus.FramesRecv;
         wsprintf(szBuf,"%lu",FramesRecv);
         SetDlgItemText(ghClientStatDlg, IDC_TOTALPKTRECV, szBuf);
         PrevIOStatus.FramesRecv = IOStatus.FramesRecv;
         }

      // update the error fields
      if(PrevIOStatus.FrameRecvError != IOStatus.FrameRecvError) {
         ULONG FrameRecvError = IOStatus.FrameRecvError - ResetIOStatus.FrameRecvError;
         wsprintf(szBuf,"%lu",FrameRecvError);
         SetDlgItemText(ghClientStatDlg, IDC_RECVERR, szBuf);
         PrevIOStatus.FrameRecvError = IOStatus.FrameRecvError;
         }
      
      if(PrevIOStatus.FrameSendError != IOStatus.FrameSendError) {
         ULONG FrameSendError = IOStatus.FrameSendError - ResetIOStatus.FrameSendError;
         wsprintf(szBuf,"%lu",FrameSendError);
         SetDlgItemText(ghClientStatDlg, IDC_SENTERR, szBuf);
         PrevIOStatus.FrameSendError = IOStatus.FrameSendError;
         }

      // update the LEDs
      DrawLED(ghClientStatDlg,IDC_SENDLED,bBytesSent);
      DrawLED(ghClientStatDlg,IDC_RECVLED,bBytesRecv);
      }
}

#ifdef HEARTBEAT_ENABLE
/*
 * DrawCaptionBar
 *
 * Draw caption (title) bar
 *
 */
void DrawCaptionBar(HWND hWnd, BOOL bActive) 
{
   RECT rc1,rc2;
   int x,y;
#ifdef WIN32
   OSVERSIONINFO OSVersionInfo;
#endif

   // don't bother drawing if we are an icon
   if(IsIconic(hWnd))
      return;

   // Let Windows do what it usually does. Let the window caption
   // be empty to avoid any Windows-initiated caption bar drawing
   GetWindowRect( hWnd, (LPRECT)&rc2 );
   	    
   // Compute the caption bar's origin. This window has a system box
   // a minimize box, a maximize box, and has a resizeable frame
      
   x = GetSystemMetrics(SM_CXSIZE) + GetSystemMetrics( SM_CXBORDER );
   y = GetSystemMetrics(SM_CYBORDER);
   rc1.left = x;
   rc1.top = y;
   					       
   // 2*x gives twice the bitmap+border. Since there are
   // only two bitmaps, two borders, and one frame at the end of the
   // caption bar, subtract a frame to account for this.
      
   // adjust position of our heartbeat indicatore based on which operating
   // system we are on.  Win31 and WinNT should be next to the minimize 
   // button.  Win95 needs to allow for two more buttons (CLOSE and MAXIMIZE).
#ifdef WIN16
   rc1.right = (rc2.right - rc2.left) - 2*x;
#else
   OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   if(GetVersionEx(&OSVersionInfo)) {
      if(OSVersionInfo.dwMajorVersion == 3)
         rc1.right = (rc2.right - rc2.left) - 2*x;
      else
         rc1.right = (rc2.right - rc2.left) - 4*x;
      }
   else
      rc1.right = (rc2.right - rc2.left) - 4*x;
#endif

   rc1.bottom = GetSystemMetrics( SM_CYSIZE ) + y;
   		        
   // calculate size of heartbeat area
   rectHeart.left = rc1.right;
   rectHeart.top  = y+2;
   rectHeart.bottom = rc1.bottom;
   rectHeart.right = rc1.right+x;

   // draw the heartbeat
   DrawHeartbeat(hWnd, &rectHeart, TRUE);
}

/*
 * DrawHeartbeat
 *
 * Show some signs of life in title bar
 *
 */
void DrawHeartbeat(HWND hWnd, LPRECT pRect, BOOL bRedraw)
{
   HDC     hBitDC=NULL;
   HDC     hDC=NULL;
   HBITMAP hBmp=NULL;
   HBRUSH  hBrush=NULL;
static ULONG BytesSent=0;
static ULONG BytesRecv=0;
   IOSTATUS IOStatus;
   int rc;

   // don't bother drawing if we are an icon
   if(IsIconic(hWnd))
      return;

   // don't bother drawing if we are invisible
   if(IsWindowVisible(hWnd)==FALSE)
      return;

   // only change bitmap if we have sent or received data since last time
   rc = GetIOStatus(&IOStatus);
   if(rc == CLIENT_STATUS_SUCCESS) {
      if( (BytesRecv != IOStatus.BytesRecv) ||
          (BytesSent != IOStatus.BytesSent)) {
         giBitmap = ++giBitmap % NUM_BITMAPCOLORS;
         BytesSent = IOStatus.BytesSent;
         BytesRecv = IOStatus.BytesRecv;
         }
      // send / receive hasn't changed and we don't need to redraw, leave
      else if(bRedraw == FALSE)
         return;
      }
   else
      return;

   // if nothing has been sent or received, don't draw anything
   if((IOStatus.BytesRecv == 0) && (IOStatus.BytesSent == 0)) {
      return;
      }

   // load bitmaps if not yet loaded
   if(hBitmaps[0]==NULL) {
      int i;
      for(i=0;i<NUM_BITMAPS;i++) {
         hBitmaps[i] = LoadBitmap(ghInstance,MAKEINTRESOURCE(BitmapIds[i]));
         }
      }

   // load brushes if not yet loaded
   if(hbrBitmap[0]==NULL) {
      int i;
      for(i=0;i<NUM_BITMAPCOLORS;i++) {
         hbrBitmap[i] = CreateSolidBrush(BitmapColors[i]);
         }
      }


   // only one bitmap, just different colors
   hBmp = hBitmaps[0];

   // select bitmap color
   hBrush = hbrBitmap[giBitmap];
  
   // get DC for entire window
   hDC = GetWindowDC( hWnd );
   if(hDC==NULL)
      return;

   hBitDC = CreateCompatibleDC(hDC);

   if(hBitDC && hBmp) {
      HBITMAP hOldBmp=NULL;
      HBRUSH  hOldBrush=NULL;
      BITMAP  Bmp;
  
      // select bitmap into source DC
      hOldBmp = SelectObject(hBitDC,hBmp);

      // select brush into source DC
      hOldBrush = SelectObject(hBitDC, hBrush);

      GetObject( hBmp, sizeof(BITMAP), &Bmp);

      // change color of bitmap 
      ExtFloodFill( hBitDC, 
                    Bmp.bmWidth/2,                  // middle of bitmap
                    Bmp.bmHeight/2,                 // middle of bitmap
                    RGB(0,0,0),                     // black is border
                    FLOODFILLBORDER);
  
      BitBlt(hDC,                 // destination DC
             pRect->left,         // upper left x
             pRect->top,          // upper left y
             pRect->right,        // width
             pRect->bottom,       // height
             hBitDC,              // source DC
             0,                   // upper left x
             0,                   // upper left y
             SRCCOPY);            // raster op (copy)
  
      SelectObject(hBitDC, hOldBmp);
      SelectObject(hBitDC, hOldBrush);
      }
  
   if(hBitDC)                
      DeleteDC(hBitDC);

   ReleaseDC(hWnd, hDC);

}
#endif //HEARTBEAT_ENABLE

/*
 * DrawLED
 *
 * Draw our LEDs in the status dialog box
 */
void DrawLED( HWND hWnd, int DlgID, BOOL bOn)
{
   HBRUSH hBrush;
   HWND   hWndLED;
   HDC  hDC;
   RECT rect;

   if(bOn) 
      hBrush = ghLEDBrush;
   else
      hBrush = GetStockObject(WHITE_BRUSH);

   hWndLED = GetDlgItem(hWnd, DlgID);
   if(hWndLED) {
      GetClientRect(hWndLED,&rect);
      hDC = GetDC(hWndLED);
      if(hDC) {
         FillRect(hDC, &rect, hBrush);
         ReleaseDC(hWndLED,hDC);
         }
      }
}

/*
 * GetIOStatus
 *
 * Get current IO status from WD/PD
 *
 */
int GetIOStatus(PIOSTATUS pIOStatus)
{
   int rc=CLIENT_STATUS_SUCCESS;
   WDQUERYINFORMATION WdQueryInfo;

   memset( &WdQueryInfo, 0, sizeof(WdQueryInfo) );
   memset( pIOStatus, 0, sizeof(IOSTATUS));
   pIOStatus->IOStatusSize = sizeof(IOSTATUS);
   WdQueryInfo.pWdInformation      = pIOStatus;
   WdQueryInfo.WdInformationLength = sizeof(IOSTATUS);
   WdQueryInfo.WdInformationClass  = WdIOStatus;
   if ( rc = ModuleCall( &gWdLink, WD$QUERYINFORMATION, &WdQueryInfo ) ) {
      TRACE(( TC_WENG, TT_ERROR,
            "WENG: GetIOStatus: Error (%d) from WdQueryInfo-WdIOStatus", rc ));
      ASSERT( 0, 0 );
      return(rc);
      }

   return(rc);
}

#endif

#ifdef WIN32
/* 
 * ParentWatchdogTimer
 *
 * Timer event handler for making sure parent is still alive.
 * If parent is dead, we are going to quit on the spot.
 *
 */
void CALLBACK ParentWatchdogTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime)
{
   DWORD dwExitCode;

   if(g_hParentProc==NULL)
      return;

   // verify that parent still lives
   if(GetExitCodeProcess( g_hParentProc, &dwExitCode)) {
      if(dwExitCode!= STILL_ACTIVE) {
         CloseHandle( g_hParentProc);
         KillTimer( NULL, g_hWatchdogTimer);
         gbDirectClose = FALSE;
         g_hWndPlugin = NULL;

         // we don't have a window anymore so all we can do is exit
         ExitProcess(1);
         }
      }

}

/*
 * SendAutoDisconnect
 *
 * Tell Autodialer that we are done
 *
 */
VOID SendAutoDisconnect()
{
   HWND hwndAutodisconnectMonitor;
   HWND hwndIE;
       
   // if embedded, don't send disconnect
   if(g_hWndPlugin)
      return;

   // if internet explorer is still running, don't hangup
   hwndIE = FindWindow(INTERNET_EXPLORER_CLASS_NAME, NULL);
   if(hwndIE != NULL) 
      return;

   hwndAutodisconnectMonitor  = FindWindow(AUTODISCONNECT_MONITOR_CLASS_NAME,
						NULL);
   if (NULL != hwndAutodisconnectMonitor) {
      SendMessage(hwndAutodisconnectMonitor,WM_BROWSER_EXITING, 0, 0);
      }
}

/* 
 * WinSockIdleTimer
 *
 * Timer event handler for periodically notifying the Autodisconnect
 * process that there is WINSOCK traffic so we don't get hungup on.
 *
 * All parameters are ignored
 *
 */
void CALLBACK WinSockIdleTimer(HWND hdlg, UINT msg, WPARAM iEvent, DWORD dwTime)
{
    static ULONG BytesRecv=0;
    static ULONG BytesSent=0;
    IOSTATUS CurIOStatus = {sizeof(IOSTATUS),0,0,0,0,0,0};
    HWND hwndAutodisconnectMonitor;

    // determine if anything has been sent or received since last time
    GetIOStatus(&CurIOStatus);
    if( (BytesRecv != CurIOStatus.BytesRecv) ||
        (BytesSent != CurIOStatus.BytesSent)) {

       // if activity found, tell the autodisconnect window
       hwndAutodisconnectMonitor  = FindWindow(AUTODISCONNECT_MONITOR_CLASS_NAME, NULL);
       if (NULL != hwndAutodisconnectMonitor) {
           SendMessage(hwndAutodisconnectMonitor,WM_WINSOCK_ACTIVITY, 0, 0);
       }
       BytesRecv = CurIOStatus.BytesRecv;
       BytesSent = CurIOStatus.BytesSent;
    }

}
#endif
