#define MAXROWS         400
#define MAXCOLS         80

#define MAXBLOCK        80

#define MAXLEN_TEMPSTR  81

// cursor states

#define CS_HIDE         0x00
#define CS_SHOW         0x01

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_TAB       '\t'
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// data structures

typedef struct tagCAMINFO
{
	BYTE	abScreen[ MAXROWS * MAXCOLS ] ;
	BOOL	fNewLine, fAutoWrap;
	WORD	wCursorState ;
	HFONT	hCAMFont ;
	LOGFONT	lfCAMFont ;
	DWORD	rgbFGColor ;
	int		xSize, ySize, xScroll, yScroll, xOffset, yOffset,
			nColumn, nRow, xChar, yChar ;
} CAMINFO, *NPCAMINFO ;

// macros ( for easier readability )

#define GETHINST( hWnd )  ((HINSTANCE) GetWindowWord( hWnd, GWW_HINSTANCE ))

#define SCREEN( x ) (x.abScreen)
#define CURSORSTATE( x ) (x.wCursorState)
#define HCAMFONT( x ) (x.hCAMFont)
#define LFCAMFONT( x ) (x.lfCAMFont)
#define FGCOLOR( x ) (x.rgbFGColor)
#define XSIZE( x ) (x.xSize)
#define YSIZE( x ) (x.ySize)
#define XSCROLL( x ) (x.xScroll)
#define YSCROLL( x ) (x.yScroll)
#define XOFFSET( x ) (x.xOffset)
#define YOFFSET( x ) (x.yOffset)
#define COLUMN( x ) (x.nColumn)
#define ROW( x ) (x.nRow)
#define XCHAR( x ) (x.xChar)
#define YCHAR( x ) (x.yChar )

#define SET_PROP( x, y, z )  SetProp( x, MAKEINTATOM( y ), z )
#define GET_PROP( x, y )     GetProp( x, MAKEINTATOM( y ) )
#define REMOVE_PROP( x, y )  RemoveProp( x, MAKEINTATOM( y ) )

// function prototypes (private)

void	CreateCAMInfo( HWND ) ;
void	DestroyCAMInfo(void) ;
BOOL	ResetCAMScreen( HWND) ;
BOOL	KillCAMFocus( HWND ) ;
BOOL	PaintCAM( HWND ) ;
BOOL	SetCAMFocus( HWND ) ;
BOOL	ScrollCAMHorz( HWND, WORD, WORD ) ;
BOOL	ScrollCAMVert( HWND, WORD, WORD ) ;
BOOL	SizeCAM( HWND, WORD, WORD ) ;
BOOL	ProcessCAMCharacter( HWND, BYTE ) ;
BOOL	MoveCAMCursor( HWND ) ;
