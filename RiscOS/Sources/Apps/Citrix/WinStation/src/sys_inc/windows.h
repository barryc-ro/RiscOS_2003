/* > windows.h
 *
 * This isn't really windows.h but will contain some compatibility
 * defines that would have been in windows.h or one of its included files
 */


#ifndef __windows_h
#define __windows_h

#define MAX_PATH          260

#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#define far
#define near
#define pascal
#define __cdecl
#define _cdecl
#define cdecl
#define FAR
#define NEAR
#define PASCAL
#define __inline

#define CDECL
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY
#define APIPRIVATE

#define CONST               const

typedef int                 BOOL;

typedef char BYTE;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef LPBYTE far	    *PPBYTE;

typedef char UCHAR;
typedef UCHAR *PUCHAR;

typedef short SHORT;                        //      x

typedef unsigned short USHORT;
typedef USHORT *PUSHORT;

typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef UINT far *LPUINT;                   //      x

typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;

typedef int                 INT;
typedef int near            *PINT;
typedef int far             *LPINT;

typedef unsigned short      WORD;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;

typedef long		    LONG;
typedef long far            *LPLONG;

typedef unsigned long ULONG;
typedef ULONG *PULONG;

typedef unsigned long       DWORD;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;

typedef void VOID;                          //  x   x
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef UCHAR		    BOOLEAN;

typedef char *PSZ;
typedef char *LPSTR;

// some typedefs that replace the vaious sized bitfields around

typedef int BWORD;
typedef int BBYTE;
typedef int BCHAR;
typedef int BUCHAR;
typedef int BSHORT;
typedef int BUSHORT;
typedef int BLONG;
typedef int BULONG;


/*=============================================================================
 ==   typedefs
 ============================================================================*/

typedef void *HANDLE;
#define DECLARE_HANDLE(name) typedef struct name##__ *H##name
typedef HANDLE *PHANDLE;

typedef unsigned int SHANDLE;               //
typedef SHANDLE far * PSHANDLE;             //
typedef (far *PFN)();                       //

typedef HANDLE FAR *LPHANDLE;      //  x   x

typedef DWORD COLORREF;

DECLARE_HANDLE(RGN);
DECLARE_HANDLE(PEN);
DECLARE_HANDLE(GLOBAL);
DECLARE_HANDLE(BRUSH);
DECLARE_HANDLE(BITMAP);
DECLARE_HANDLE(CURSOR);
DECLARE_HANDLE(PALETTE);
DECLARE_HANDLE(GDIOBJ);
DECLARE_HANDLE(DC);
DECLARE_HANDLE(INSTANCE);
DECLARE_HANDLE(WND);

/* Types use for passing & returning polymorphic values */
typedef UINT WPARAM;
typedef LONG LPARAM;
typedef LONG LRESULT;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/* ---------------------------------------------------------------------------------------------------- */

/* from WTYPES.H */

#define __RPC_FAR

typedef struct  tagRECT
    {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
    }	RECT;

typedef struct tagRECT __RPC_FAR *PRECT;

typedef struct tagRECT __RPC_FAR *LPRECT;

typedef const RECT __RPC_FAR *LPCRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

/* ---------------------------------------------------------------------------------------------------- */

/* configuration options can go here for lack of anywhere better */

/* These ones say that the functions are directly availabel rather than through function pointers arrays */

#define BINILIB
#define INILIB
#define LOGLIB
#define DLLLIB

#define VIOLIB
#define TIMERLIB
#define KBDLIB
#define MOUSELIB

/* ---------------------------------------------------------------------------------------------------- */

/* From winuser.h */

#define SW_SHOWNORMAL       1

/* ---------------------------------------------------------------------------------------------------- */

#ifndef NOSYSMETRICS

/*
 * GetSystemMetrics() codes
 */
#define SM_CXSCREEN             0
#define SM_CYSCREEN             1
#define SM_CXVSCROLL            2
#define SM_CYHSCROLL            3
#define SM_CYCAPTION            4
#define SM_CXBORDER             5
#define SM_CYBORDER             6
#define SM_CXDLGFRAME           7
#define SM_CYDLGFRAME           8
#define SM_CYVTHUMB             9
#define SM_CXHTHUMB             10
#define SM_CXICON               11
#define SM_CYICON               12
#define SM_CXCURSOR             13
#define SM_CYCURSOR             14
#define SM_CYMENU               15
#define SM_CXFULLSCREEN         16
#define SM_CYFULLSCREEN         17
#define SM_CYKANJIWINDOW        18
#define SM_MOUSEPRESENT         19
#define SM_CYVSCROLL            20
#define SM_CXHSCROLL            21
#define SM_DEBUG                22
#define SM_SWAPBUTTON           23
#define SM_RESERVED1            24
#define SM_RESERVED2            25
#define SM_RESERVED3            26
#define SM_RESERVED4            27
#define SM_CXMIN                28
#define SM_CYMIN                29
#define SM_CXSIZE               30
#define SM_CYSIZE               31
#define SM_CXFRAME              32
#define SM_CYFRAME              33
#define SM_CXMINTRACK           34
#define SM_CYMINTRACK           35
#define SM_CXDOUBLECLK          36
#define SM_CYDOUBLECLK          37
#define SM_CXICONSPACING        38
#define SM_CYICONSPACING        39
#define SM_MENUDROPALIGNMENT    40
#define SM_PENWINDOWS           41
#define SM_DBCSENABLED          42
#define SM_CMOUSEBUTTONS        43

#define SM_CXFIXEDFRAME           SM_CXDLGFRAME  /* ;win40 name change */
#define SM_CYFIXEDFRAME           SM_CYDLGFRAME  /* ;win40 name change */
#define SM_CXSIZEFRAME            SM_CXFRAME     /* ;win40 name change */
#define SM_CYSIZEFRAME            SM_CYFRAME     /* ;win40 name change */

#define SM_SECURE               44
#define SM_CXEDGE               45
#define SM_CYEDGE               46
#define SM_CXMINSPACING         47
#define SM_CYMINSPACING         48
#define SM_CXSMICON             49
#define SM_CYSMICON             50
#define SM_CYSMCAPTION          51
#define SM_CXSMSIZE             52
#define SM_CYSMSIZE             53
#define SM_CXMENUSIZE           54
#define SM_CYMENUSIZE           55
#define SM_ARRANGE              56
#define SM_CXMINIMIZED          57
#define SM_CYMINIMIZED          58
#define SM_CXMAXTRACK           59
#define SM_CYMAXTRACK           60
#define SM_CXMAXIMIZED          61
#define SM_CYMAXIMIZED          62
#define SM_NETWORK              63
#define SM_CLEANBOOT            67
#define SM_CXDRAG               68
#define SM_CYDRAG               69

#define SM_SHOWSOUNDS           70
#define SM_CXMENUCHECK          71   /* Use instead of GetMenuCheckMarkDimensions()! */
#define SM_CYMENUCHECK          72
#define SM_SLOWMACHINE          73
#define SM_MIDEASTENABLED       74

#define SM_CMETRICS             75

extern int GetSystemMetrics(int nIndex);

#endif /* !NOSYSMETRICS */

/* ---------------------------------------------------------------------------------------------------- */

/* Binary raster ops */
#define R2_BLACK            1   /*  0       */
#define R2_NOTMERGEPEN      2   /* DPon     */
#define R2_MASKNOTPEN       3   /* DPna     */
#define R2_NOTCOPYPEN       4   /* PN       */
#define R2_MASKPENNOT       5   /* PDna     */
#define R2_NOT              6   /* Dn       */
#define R2_XORPEN           7   /* DPx      */
#define R2_NOTMASKPEN       8   /* DPan     */
#define R2_MASKPEN          9   /* DPa      */
#define R2_NOTXORPEN        10  /* DPxn     */
#define R2_NOP              11  /* D        */
#define R2_MERGENOTPEN      12  /* DPno     */
#define R2_COPYPEN          13  /* P        */
#define R2_MERGEPENNOT      14  /* PDno     */
#define R2_MERGEPEN         15  /* DPo      */
#define R2_WHITE            16  /*  1       */
#define R2_LAST             16

/* Ternary raster operations */
#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (DWORD)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (DWORD)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
#define NOTSRCERASE         (DWORD)0x001100A6 /* dest = (NOT src) AND (NOT dest) */
#define MERGECOPY           (DWORD)0x00C000CA /* dest = (source AND pattern)     */
#define MERGEPAINT          (DWORD)0x00BB0226 /* dest = (NOT source) OR dest     */
#define PATCOPY             (DWORD)0x00F00021 /* dest = pattern                  */
#define PATPAINT            (DWORD)0x00FB0A09 /* dest = DPSnoo                   */
#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
#define DSTINVERT           (DWORD)0x00550009 /* dest = (NOT dest)               */
#define BLACKNESS           (DWORD)0x00000042 /* dest = BLACK                    */
#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE                    */

/* Pen Styles */
#define PS_SOLID            0
/* #define PS_DASH             1*/       /* -------  */
/* #define PS_DOT              2 */      /* .......  */
/* #define PS_DASHDOT          3 */      /* _._._._  */
/* #define PS_DASHDOTDOT       4 */      /* _.._.._  */
/* #define PS_NULL             5 */
/* #define PS_INSIDEFRAME      6 */
/* #define PS_USERSTYLE        7 */
/* #define PS_ALTERNATE        8 */
/* #define PS_STYLE_MASK       0x0000000F */

/* Stock Logical Objects */
#define WHITE_BRUSH         0
/* #define LTGRAY_BRUSH        1 */
/* #define GRAY_BRUSH          2 */
/* #define DKGRAY_BRUSH        3 */
/* #define BLACK_BRUSH         4 */
#define NULL_BRUSH          5
/* #define HOLLOW_BRUSH        NULL_BRUSH */
#define WHITE_PEN           6
#define BLACK_PEN           7
#define NULL_PEN            8
/* #define OEM_FIXED_FONT      10 */
/* #define ANSI_FIXED_FONT     11 */
/* #define ANSI_VAR_FONT       12 */
/* #define SYSTEM_FONT         13 */
/* #define DEVICE_DEFAULT_FONT 14 */
#define DEFAULT_PALETTE     15
/* #define SYSTEM_FIXED_FONT   16 */
/* #define DEFAULT_GUI_FONT    17 */
#define STOCK_LAST          17

/* Region Flags */
#define ERROR               0
#define NULLREGION          1
#define SIMPLEREGION        2
#define COMPLEXREGION       3
#define RGN_ERROR ERROR

/* CombineRgn() Styles */
//#define RGN_AND             1
#define RGN_OR              2
/* #define RGN_XOR             3 */
/* #define RGN_DIFF            4 */
/* #define RGN_COPY            5 */
/* #define RGN_MIN             RGN_AND */
/* #define RGN_MAX             RGN_COPY */



typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef RGBQUAD FAR* LPRGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, FAR *LPBITMAPINFO, *PBITMAPINFO;


/* Bitmap Header Definition */
typedef struct tagBITMAP
  {
    LONG        bmType;
    LONG        bmWidth;
    LONG        bmHeight;
    LONG        bmWidthBytes;
    WORD        bmPlanes;
    WORD        bmBitsPixel;
    LPVOID      bmBits;
  } BITMAP, *PBITMAP, NEAR *NPBITMAP, FAR *LPBITMAP;


/* DIB color table identifiers */

#define DIB_RGB_COLORS      0 /* color table in RGBs */
#define DIB_PAL_COLORS      1 /* color table in palette indices */

/* constants for Get/SetSystemPaletteUse() */

#define SYSPAL_ERROR    0
#define SYSPAL_STATIC   1
#define SYSPAL_NOSTATIC 2

/* constants for CreateDIBitmap */
#define CBM_INIT        0x04L   /* initialize bitmap */

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))

/* palette entry flags */

#define PC_RESERVED     0x01    /* palette index used for animation */
#define PC_EXPLICIT     0x02    /* palette index is explicit to device */
#define PC_NOCOLLAPSE   0x04    /* do not match color to system palette */

#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
#define BKMODE_LAST         2


typedef struct tagPALETTEENTRY {
    BYTE        peRed;
    BYTE        peGreen;
    BYTE        peBlue;
    BYTE        peFlags;
} PALETTEENTRY, *PPALETTEENTRY, FAR *LPPALETTEENTRY;

/* Logical Palette */
typedef struct tagLOGPALETTE {
    WORD        palVersion;
    WORD        palNumEntries;
    PALETTEENTRY        palPalEntry[1];
} LOGPALETTE, *PLOGPALETTE, NEAR *NPLOGPALETTE, FAR *LPLOGPALETTE;

typedef struct _ICONINFO {
    BOOL    fIcon;
    DWORD   xHotspot;
    DWORD   yHotspot;
    HBITMAP hbmMask;
    HBITMAP hbmColor;
} ICONINFO;
typedef ICONINFO *PICONINFO;

/* Global Memory Flags */
/* #define GMEM_FIXED          0x0000 */
#define GMEM_MOVEABLE       0x0002
/* #define GMEM_NOCOMPACT      0x0010 */
/* #define GMEM_NODISCARD      0x0020 */
/* #define GMEM_ZEROINIT       0x0040 */
/* #define GMEM_MODIFY         0x0080 */
/* #define GMEM_DISCARDABLE    0x0100 */
/* #define GMEM_NOT_BANKED     0x1000 */
/* #define GMEM_SHARE          0x2000 */
/* #define GMEM_DDESHARE       0x2000 */
/* #define GMEM_NOTIFY         0x4000 */
/* #define GMEM_LOWER          GMEM_NOT_BANKED */
/* #define GMEM_VALID_FLAGS    0x7F72 */
/* #define GMEM_INVALID_HANDLE 0x8000 */


typedef SHORT WCHAR;    // wc,   16-bit UNICODE character

typedef struct _RECTL       /* rcl */
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECTL, *PRECTL;


#define LF_FACESIZE         32

typedef struct tagLOGFONTW
{
    LONG      lfHeight;
    LONG      lfWidth;
    LONG      lfEscapement;
    LONG      lfOrientation;
    LONG      lfWeight;
    BYTE      lfItalic;
    BYTE      lfUnderline;
    BYTE      lfStrikeOut;
    BYTE      lfCharSet;
    BYTE      lfOutPrecision;
    BYTE      lfClipPrecision;
    BYTE      lfQuality;
    BYTE      lfPitchAndFamily;
    WCHAR     lfFaceName[LF_FACESIZE];
} LOGFONTW, *PLOGFONTW, NEAR *NPLOGFONTW, FAR *LPLOGFONTW;

typedef struct _POINTL      /* ptl  */
{
    LONG  x;
    LONG  y;
} POINTL, *PPOINTL;

/* ---------------------------------------------------------------------------------------------------- */

extern BOOL BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
extern int  CombineRgn(HRGN, HRGN, HRGN, int);
extern HBITMAP CreateBitmap(int, int, UINT, UINT, CONST VOID *);
extern HBITMAP CreateBitmapIndirect(CONST BITMAP *);
extern HBITMAP CreateCompatibleBitmap(HDC, int, int);
extern HDC  CreateCompatibleDC(HDC);
extern HBITMAP CreateDIBitmap(HDC, CONST BITMAPINFOHEADER *, DWORD, CONST VOID *, CONST BITMAPINFO *, UINT);
extern HBRUSH CreateDIBPatternBrush(HGLOBAL, UINT);
extern HPALETTE CreatePalette(CONST LOGPALETTE *pal);
extern HPEN CreatePen(int, int, COLORREF);
extern HRGN CreateRectRgn(int, int, int, int);
extern HRGN CreateRectRgnIndirect(CONST RECT *);
extern HBRUSH CreateSolidBrush(COLORREF color);

extern BOOL DeleteDC(HDC);
extern BOOL DeleteObject(LPVOID);
extern int FillRect(HDC hDC, CONST RECT *lprc, HBRUSH hbr);

extern HDC GetDC(HWND hwnd);
extern int GetDIBits(HDC, HBITMAP, UINT, UINT, LPVOID, LPBITMAPINFO, UINT);
extern int GetRgnBox(HRGN, LPRECT);
extern HGDIOBJ GetStockObject(int);
extern UINT GetSystemPaletteEntries(HDC, UINT, UINT, LPPALETTEENTRY);
extern UINT GetSystemPaletteUse(HDC);
extern BOOL IntersectRect(LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2);

extern BOOL PatBlt(HDC, int, int, int, int, DWORD);
extern UINT RealizePalette(HDC);
extern int ReleaseDC(HWND hwnd, HDC dc);
extern int SelectClipRgn(HDC, HRGN);
extern LPVOID SelectObject(HDC, LPVOID);
extern HPALETTE SelectPalette(HDC, HPALETTE, BOOL);
extern COLORREF SetBkColor(HDC, COLORREF);
extern int SetDIBitsToDevice(HDC, int, int, DWORD, DWORD, int,int, UINT, UINT, CONST VOID *, CONST BITMAPINFO *, UINT);
extern int SetROP2(HDC, int);
extern BOOL SetBrushOrgEx(HDC, int, int, LPPOINT);
extern UINT SetSystemPaletteUse(HDC, UINT);
extern BOOL UnrealizeObject(HGDIOBJ);
extern COLORREF SetTextColor(HDC, COLORREF);

extern BOOL MoveToEx(HDC hdc, int x, int y, LPPOINT pPoint);
extern BOOL LineTo(HDC hdc, int x, int y);

extern void RestoreScreen(void);

extern HCURSOR SetCursor(HCURSOR hCursor);
//extern BOOL GetCursorPos(LPPOINT lpPoint);
extern HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, CONST VOID *pvANDPlane, CONST VOID *pvXORPlane);
extern BOOL DestroyCursor(HCURSOR hCursor);

/* ---------------------------------------------------------------------------------------------------- */

#endif

/* eof windows.h */

