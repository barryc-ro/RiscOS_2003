
/*******************************************************************************
*
*  CWIN.H
*
*  Include file for Character Windows Library  (cwin.lib)
*
*  Copyright Citrix Systems Inc. 1990
*
*  Author:      Brad Pedersen
*
*  $Log$
*  
*     Rev 1.29   21 Apr 1997 16:56:40   TOMA
*  update
*  
*     Rev 1.28   26 Jul 1995 10:54:44   thanhl
*  update
*  
*     Rev 1.27   05 Jul 1995 16:08:04   thanhl
*  update
*  
*     Rev 1.26   12 May 1995 13:16:38   butchd
*  update
*  
*     Rev 1.25   11 May 1995 16:19:34   butchd
*  update
*  
*     Rev 1.24   27 Feb 1995 11:25:46   marcb
*  update
*  
*     Rev 1.23   01 Feb 1995 17:52:06   marcb
*  update
*  
*     Rev 1.22   28 Nov 1994 18:24:22   marcb
*  update
*  
*     Rev 1.21   23 Nov 1994 11:27:24   marcb
*  update
*  
*     Rev 1.20   22 Nov 1994 11:25:54   thanhl
*  update
*  
*     Rev 1.19   14 Nov 1994 13:29:18   marcb
*  update
*
*******************************************************************************/

#ifdef DOS
#ifdef DEBUG
#define DOSDEBUG
#endif
#endif

#ifndef DOSDEBUG
#pragma intrinsic(memcpy,memcmp,memset,strcpy,strcmp,strlen,strcat)
#endif


/*=============================================================================
==   Defines
=============================================================================*/

#ifndef NO_ERROR
#define NO_ERROR 0
#endif

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#define WM_POLL 0x4000

/*
 * Character Window styles (WinStyle)
 */
#define SW_HIDENA           20


#define CWS_CHILD           0x00000001L // window - child window
#define CWS_BORDER          0x00000002L // window - border
#define CWS_MENU            0x00000004L // window - contains active menu
#define CWS_SHADOW          0x00000008L // window - has a shadow
#define CWS_SAVEBACKGROUND  0x00000010L // window - save text behind window
#define CWS_CLIP            0x00000020L // window - enable clipping

#define CWS_EDIT            0x00000040L // control - editbox
#define CWS_BUTTON          0x00000080L // control - pushbutton
#define CWS_CHECKBOX        0x00000100L // control - checkbox button
#define CWS_LISTBOX         0x00000200L // control - listbox
#define CWS_COMBOBOX        0x00000400L // control - combobox
#define CWS_LTEXT           0x00000800L // control - left justified text
#define CWS_RTEXT           0x00001000L // control - right justified text
#define CWS_CTEXT           0x00002000L // control - centered text
#define CWS_RADIOBUTTON     0x00004000L // control - radio button

#define CWS_HSCROLL         0x00008000L // style - horizontal scroll
#define CWS_VSCROLL         0x00010000L // style - vertical scroll
#define CWS_UPPERCASE       0x00020000L // style - uppercase only
#define CWS_HIDDEN          0x00040000L // style - hidden (etc. password)
#define CWS_INT             0x00080000L // style - numeric input only

#define CWS_DEFPUSHBUTTON   0x00100000L // style - pushbutton only
#define CWS_NOTVISIBLE      0x00200000L // style - initialize to not CWS_VISIBLE
#define CWS_READONLY        0x00400000L // style - read-only
#define CWS_GROUPBOX        0x00800000L // style - group box 
#define CWS_MODAL           0x01000000L // style - modal window
#define CWS_MULTILINE       0x02000000L // style - multi line edit field 
#define CWS_notused6        0x04000000L // currently not used

#define CWS_MSGBOX          0x08000000L // flag - message box window
#define CWS_VISIBLE         0x10000000L // flag - window is visible
#define CWS_DISABLED        0x20000000L // flag - window is disabled
#define CWS_GROUP           0x40000000L // flag - window (control) begins a group
#define CWS_TABSTOP         0x80000000L // flag - window (control) can get focus

#define CWS_TEXT    (CWS_LTEXT|CWS_CTEXT|CWS_RTEXT|CWS_GROUPBOX)// mask: all static text controls
#define CWS_STATIC  (CWS_TEXT)                                  // mask: all static controls
#define CWS_BUTTONS (CWS_BUTTON|CWS_CHECKBOX|CWS_RADIOBUTTON)   // mask: all button controls

/*
 * Menu Item flags (ItemFlag)
 */
#define IF_STRING          0x0000
#define IF_ENABLED         0x0000
#define IF_GRAYED          0x0001
#define IF_DISABLED        0x0002
#define IF_UNCHECKED       0x0000
#define IF_CHECKED         0x0008
#define IF_POPUP           0x0010
#define IF_MENUBARBREAK    0x0020
#define IF_MENUBREAK       0x0040
#define IF_SEPARATOR       0x0800

/*
 *   Screen Colors
 */
#define FG_BLACK     0x00
#define FG_BLUE      0x01
#define FG_GREEN     0x02
#define FG_PURPLE    0x03
#define FG_RED       0x04
#define FG_MAGENTA   0x05
#define FG_WHITE     0x07

#define FG_CYAN      0x03
#define FG_BROWN     0x06
#define FG_GRAY      0x08
#define FG_LBLUE     0x09
#define FG_LGREEN    0x0A
#define FG_LCYAN     0x0B
#define FG_LRED      0x0C
#define FG_LMAGENTA  0x0D
#define FG_YELLOW    0x0E
#define FG_HIWHITE   0x0F

#define BG_BLACK     0x00
#define BG_BLUE      0x10
#define BG_GREEN     0x20
#define BG_PURPLE    0x30
#define BG_RED       0x40
#define BG_MAGENTA   0x50
#define BG_WHITE     0x70

#define BG_CYAN      0x30
#define BG_BROWN     0x60
#define BG_GRAY      0x80
#define BG_LBLUE     0x90
#define BG_LGREEN    0xA0
#define BG_LCYAN     0xB0
#define BG_LRED      0xC0
#define BG_LMAGENTA  0xD0
#define BG_YELLOW    0xE0
#define BG_HIWHITE   0xF0

#define INTENSITY    0x08
#define FLASHING     0x80


/*=============================================================================
==   CITRIX MsgBox extension defines

     These are new idResponse values only
=============================================================================*/
    // Start with numbers well above Windows numbers

#define IDTIMEOUT 32000    // The MsgBox timed out before a user response
#define IDASYNC   32001    // The request was for an Async message box, no return
#define IDERROR   32002    // an error occured that resulted in not displaying
#define IDCOUNTEXCEEDED  32003  // to many in queue for winstation already
#define IDDESKTOPERROR   32004  // current desktop is not default

/*=============================================================================
==   Error Defines
=============================================================================*/

#define ERROR_CWIN_NOT_ENOUGH_MEMORY     300
#define ERROR_CWIN_MENU_ITEM_NOT_FOUND   301
#define ERROR_CWIN_MENU_NOT_FOUND        302
#define ERROR_CWIN_WINDOW_NOT_FOUND      303
#define ERROR_CWIN_DIALOG_ITEM_NOT_FOUND 304
#define ERROR_CWIN_BAD_DIALOG_ITEM       305
#define ERROR_CWIN_CONTROL_PENDING       306


/*=============================================================================
==   Typedefs
=============================================================================*/

typedef struct _DESK * PDESK;
typedef struct _WINDOW * PWINDOW;
typedef struct _MENU2 * PMENU2;
typedef struct _MESSAGE * PMESSAGE;

typedef int (*PMSGFUNC)( PWINDOW, PMESSAGE );
typedef void (*PHELPFUNC)( PWINDOW, int );


/*=============================================================================
==   structures
=============================================================================*/

/*
 *  Save Rectangle structure
 */
typedef struct _SAVERECT {
   RECT Rect;                   // data rectangle on screen to save
   CHAR_INFO * pBuffer;         // pointer to saved screen data
} SAVERECT, * PSAVERECT;


/*
 *  Message structure
 */
typedef struct _MESSAGE {
   int MsgType;                 // message type
   int Parm1;                   // parameter 1
   int Parm2;                   // parameter 2
   int Parm3;                   // parameter 3
   long lParm;                  // long parameter
} MESSAGE;


/*
 *  Menu Append structure  (optional parameter to MenuCreate)
 */
typedef struct _MENU2APPEND {
   int Type;                    // type of menu item  (IF_?)
   long hItem;                  // handle of menu item
   int HelpCode;                // help code for context sensitive help
   char * pText;                // text for menu item
} MENU2APPEND, * PMENU2APPEND;


/*
 *  Dialog window structure
 */
typedef struct _DIALOGDATA {
   int ItemId;          // dialog id
   int X;               // x coordinate of control
   int Y;               // y coordinate of control
   int Width;           // width of control
   int Height;          // height of control
   long WinStyle;       // window style
   PWINDOW pWin;        // pointer to window structure
   int HelpCode;        // help code for context sensitive help
   char * pText;        // field text
} DIALOGDATA, * PDIALOGDATA;

/*=============================================================================
==   Public Functions
=============================================================================*/

#ifdef DOS
int   DeskCreate( int, PDESK *, int );
void  DeskDelete( PDESK, int );
#else
int   DeskCreate( int, PDESK * );
void  DeskDelete( PDESK );
#endif
void  DeskRegisterHelpFunc( PDESK, PHELPFUNC );
PWINDOW DeskFocusWindow( PDESK );
void  DeskRestore( PDESK pDesk, PWINDOW pWin );

int WindowCreate( PDESK, char *, long, int, int, int, int, int, PWINDOW,
                  PMSGFUNC, long, int, int, PWINDOW * );
void WindowDelete( PWINDOW );
void WindowShow( PWINDOW, int );
int WindowSetColor( PWINDOW, int );
int WindowNextMessage( PWINDOW, PMESSAGE );
int WindowMessageParm( PWINDOW, int, int, int, int, long );
int WindowPostMessageParm( PWINDOW, int, int, int, int, long );

#ifndef DOS
void WindowTextOutW( PWINDOW, int, int, WCHAR *, ... );
#endif
void WindowTextOutA( PWINDOW, int, int, char *, ... );

#ifdef UNICODE
#define WindowTextOut WindowTextOutW
#else
#define WindowTextOut WindowTextOutA
#endif

void WindowDrawText( PWINDOW, char *, int, RECT *, int );
int WindowSetMsgFunc( PWINDOW, PMSGFUNC, long );
void WindowSetUserData( PWINDOW, long );
long WindowGetUserData( PWINDOW );
long WindowGetStyle( PWINDOW );
long WindowSetStyle( PWINDOW, long );

#ifndef DOS
void WindowGetTextW( PWINDOW, WCHAR *, int );
#endif
void WindowGetTextA( PWINDOW, char *, int );

#ifdef UNICODE
#define WindowGetText WindowGetTextW
#else
#define WindowGetText WindowGetTextA
#endif

#ifndef DOS
int WindowSetTextW( PWINDOW, WCHAR * );
#endif
int WindowSetTextA( PWINDOW, char * );

#ifdef UNICODE
#define WindowSetText WindowSetTextW
#else
#define WindowSetText WindowSetTextA
#endif

PWINDOW WindowParent( PWINDOW );
BOOL WindowEnum( PDESK, WNDENUMPROC, LPARAM );
int WindowProcessInput( PDESK, UINT, UINT, LONG );
int WindowValidateHandle( PDESK, PWINDOW );

int MenuCreate( PWINDOW, PMENU2APPEND, PMENU2 * );
void MenuDelete( PMENU2 );
int MenuAppend( PMENU2, int, long, int, char * );
void MenuSet( PWINDOW, PMENU2, int, int );
PMENU2 MenuGet( PWINDOW );

void EditGetTextLength( PWINDOW, int * );
void EditLimitTextLength( PWINDOW, int );

#ifndef DOS
void EditGetTextW( PWINDOW, WCHAR *, int );
#endif
void EditGetTextA( PWINDOW, char *, int );

#ifdef UNICODE
#define EditGetText EditGetTextW
#else
#define EditGetText EditGetTextA
#endif

#ifndef DOS
int EditSetTextW( PWINDOW, WCHAR * );
#endif
int EditSetTextA( PWINDOW, char * );

#ifdef UNICODE
#define EditSetText EditSetTextW
#else
#define EditSetText EditSetTextA
#endif

#ifndef DOS
int EditMessageW( PWINDOW, PMESSAGE );
#endif
int EditMessageA( PWINDOW, PMESSAGE );

#ifdef UNICODE
#define EditMessage EditMessageW
#else
#define EditMessage EditMessageA
#endif

#ifndef DOS
int ListLineCreateW( PWINDOW, int, int, long, WCHAR* );
#endif
int ListLineCreateA( PWINDOW, int, int, long, char * );

#ifdef UNICODE
#define ListLineCreate ListLineCreateW
#else
#define ListLineCreate ListLineCreateA
#endif

void ListLineDelete( PWINDOW, int );

#ifndef DOS
void ListLineGetTextW( PWINDOW, int, WCHAR *, int );
#endif
void ListLineGetTextA( PWINDOW, int, char *, int );

#ifdef UNICODE
#define ListLineGetText ListLineGetTextW
#else
#define ListLineGetText ListLineGetTextA
#endif

void ListGotoLine( PWINDOW, int );

#ifndef DOS
int ListMessageW( PWINDOW, PMESSAGE );
#endif
int ListMessageA( PWINDOW, PMESSAGE );

#ifdef UNICODE
#define ListMessage ListMessageW
#else
#define ListMessage ListMessageA
#endif

#ifndef DOS
int MsgBoxW( PDESK, PWINDOW, WCHAR *, WCHAR *, int, int, long );
#endif
int MsgBoxA( PDESK, PWINDOW, char *, char *, int, int, long );

#ifdef UNICODE
#define MsgBox MsgBoxW
#else
#define MsgBox MsgBoxA
#endif

#ifndef DOS
int DialogCreateW( PDESK, PWINDOW, char *, long, int, int, int, int, int,
                   PMSGFUNC, PDIALOGDATA, long );
#endif
int DialogCreateA( PDESK, PWINDOW, char *, long, int, int, int, int, int,
                   PMSGFUNC, PDIALOGDATA, long );

#ifdef UNICODE
#define DialogCreate DialogCreateW
#else
#define DialogCreate DialogCreateA
#endif

void DialogDelete( PWINDOW, int );

#ifndef DOS
PWINDOW ModelessDialogCreateW( PDESK, PWINDOW, char *, long, int, int, int, int,
                           int, PMSGFUNC, PDIALOGDATA, long );
#endif
PWINDOW ModelessDialogCreateA( PDESK, PWINDOW, char *, long, int, int, int, int,
                           int, PMSGFUNC, PDIALOGDATA, long );
#ifdef UNICODE
#define ModelessDialogCreate ModelessDialogCreateW
#else
#define ModelessDialogCreate ModelessDialogCreateA
#endif

PWINDOW DialogGetItemWindow( PWINDOW, int );

#ifndef DOS
int DialogGetItemTextW( PWINDOW, int, WCHAR *, int );
#endif
int DialogGetItemTextA( PWINDOW, int, char *, int );

#ifdef UNICODE
#define DialogGetItemText DialogGetItemTextW
#else
#define DialogGetItemText DialogGetItemTextA
#endif

#ifndef DOS
int DialogSetItemTextW( PWINDOW, int, WCHAR * );
#endif
int DialogSetItemTextA( PWINDOW, int, char * );

#ifdef UNICODE
#define DialogSetItemText DialogSetItemTextW
#else
#define DialogSetItemText DialogSetItemTextA
#endif

int DialogGetItemInt( PWINDOW, int, int * );
int DialogSetItemInt( PWINDOW, int, int );
int DialogGetCheckBox( PWINDOW, int, int * );
int DialogSetCheckBox( PWINDOW, int, int );
int DialogCheckRadioButton( PWINDOW, int, int, int );

void DevClearScreen( PDESK, PWINDOW );
int  DevGetVideoSize( PDESK, int *, int * );
BOOL DevIsKeyDown(int);
int  DevRectFill( PWINDOW, RECT * );
int  DevRectSave( PWINDOW, RECT *, PSAVERECT * );
void DevRectDestroy( PSAVERECT * );
int  DevRectRestore( PWINDOW, PSAVERECT * );
int  DevCaretEnable( PDESK );
int  DevCaretDisable( PDESK );
int  DevCaretPosition( PDESK, int, int );
void DevGetClipRect( PDESK, RECT * );
void DevSetClipRect( PDESK, RECT * );

void LineMoveTo( PWINDOW, int, int );
void LineVLine( PWINDOW, int );
void LineHLine( PWINDOW, int );
void LineRectangle( PWINDOW, int, int, int, int );
void LineSetWidth( PWINDOW, int );

// Input handling and message processing * NEW *
BOOL MessageGet( PDESK, PMESSAGE, PWINDOW, int, int, long );
BOOL MessageTranslate( PDESK, PMESSAGE );
LONG MessageDispatch( PDESK, PMESSAGE );
void MessageFlushQueue( PDESK );


/*
 *  Message box structure
 */
typedef struct _MSGBOXAPI {
   int ButtonCount;			// number of buttons
   char **ButtonName;
   unsigned int *ButtonMap;
   int *ButtonReturn;
} MSGBOXAPI, * PMSGBOXAPI;

/*-------------------------------------------------------------------------
-- TRACE STUFF
--------------------------------------------------------------------------*/
// For use with TC_CWIN class
#define TT_CWIN_BUTTON_API1   0x00000001
#define TT_CWIN_BUTTON_API2   0x00000002
#define TT_CWIN_DEVVIO_API1   0x00000004
#define TT_CWIN_DEVVIO_API2   0x00000008
#define TT_CWIN_DIALOG_API1   0x00000010
#define TT_CWIN_DIALOG_API2   0x00000020
#define TT_CWIN_EDIT_API1     0x00000040
#define TT_CWIN_EDIT_API2     0x00000080
#define TT_CWIN_LIST_API1     0x00000100
#define TT_CWIN_LIST_API2     0x00000200
#define TT_CWIN_MENU_API1     0x00000400
#define TT_CWIN_MENU_API2     0x00000800
#define TT_CWIN_MOUSE_API1    0x00001000
#define TT_CWIN_MOUSE_API2    0x00002000
#define TT_CWIN_MSGBOX_API1   0x00004000
#define TT_CWIN_MSGBOX_API2   0x00008000
#define TT_CWIN_WINDOW_API1   0x00010000
#define TT_CWIN_WINDOW_API2   0x00020000


