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

/*=============================================================================
 ==   typedefs
 ============================================================================*/

typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
typedef HANDLE *PHANDLE;

typedef unsigned int SHANDLE;               //
typedef SHANDLE far * PSHANDLE;             //
typedef (far *PFN)();                       //

typedef HANDLE FAR *LPHANDLE;      //  x   x


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


typedef void *HINSTANCE;
typedef void *HWND;

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

/* From winuser.h */

#define SW_SHOWNORMAL       1

#endif

/* eof windows.h */

