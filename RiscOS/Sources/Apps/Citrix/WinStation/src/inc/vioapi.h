/******************************************************************************
*
*  VIOAPI.H
*
*  Header file for Video APIs.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry
*
*  $Log$
*  
*     Rev 1.13   15 Apr 1997 18:46:06   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.12   06 May 1996 18:43:28   jeffm
*  update
*  
*     Rev 1.11   04 May 1995 18:30:38   kurtp
*  update
*  
*     Rev 1.10   03 May 1995 12:52:04   marcb
*  update
*  
*     Rev 1.9   02 May 1995 13:41:08   butchd
*  update
*
*******************************************************************************/

#ifndef __VIOAPI_H__
#define __VIOAPI_H__

/*=============================================================================
==   External functions provided by vioapi
=============================================================================*/

int cdecl WFCAPI VioLoad( PPLIBPROCEDURE );
int cdecl WFCAPI VioUnload( VOID );


/*=============================================================================
==   Typedefs and structures
=============================================================================*/

typedef HWND HVIO;
typedef HVIO FAR * PHVIO;
typedef CHAR * PCH;
typedef USHORT SEL;
typedef SEL * PSEL;

/* structure for VioSet/GetCurType() */
typedef struct _VIOCURSORINFO { /* vioci */
        USHORT   yStart;
        USHORT   cEnd;
        USHORT   cx;
        USHORT   attr;
        } VIOCURSORINFO;
typedef VIOCURSORINFO FAR *PVIOCURSORINFO;

/* VIOMODEINFO.color constants */
#define COLORS_2        0x0001
#define COLORS_4        0x0002
#define COLORS_16       0x0004

/* structure for VioSet/GetMode() */
typedef struct _VIOMODEINFO {   /* viomi */
        USHORT cb;
        UCHAR  fbType;
        UCHAR  color;
        USHORT col;
        USHORT row;
        USHORT hres;
        USHORT vres;
        UCHAR  fmt_ID;
        UCHAR  attrib;
        ULONG  buf_addr;
        ULONG  buf_length;
        ULONG  full_length;
        ULONG  partial_length;
        PCH    ext_data_addr;
        } VIOMODEINFO;
typedef VIOMODEINFO FAR *PVIOMODEINFO;

#define VGMT_OTHER      0x01
#define VGMT_GRAPHICS   0x02
#define VGMT_DISABLEBURST       0x04


/* structure for VioGetPhysBuf() */
typedef struct _VIOPHYSBUF {    /* viopb */
        LPBYTE    pBuf;
        ULONG    cb;
        SEL      asel[1];
        } VIOPHYSBUF;
typedef VIOPHYSBUF FAR *PVIOPHYSBUF;


#define ANSI_ON                    1
#define ANSI_OFF                   0

#define VSRWI_SAVEANDREDRAW        0
#define VSRWI_REDRAW               1

#define VSRWN_SAVE                 0
#define VSRWN_REDRAW               1

#define UNDOI_GETOWNER             0
#define UNDOI_RELEASEOWNER         1

#define UNDOK_ERRORCODE            0
#define UNDOK_TERMINATE            1

#define VMWR_POPUP                 0
#define VMWN_POPUP                 0

#define LOCKIO_NOWAIT              0
#define LOCKIO_WAIT                1

#define LOCK_SUCCESS               0
#define LOCK_FAIL                  1

#define VP_NOWAIT       0x0000
#define VP_WAIT 0x0001
#define VP_OPAQUE       0x0000
#define VP_TRANSPARENT  0x0002

/* VIOCONFIGINFO.adapter constants */
#define DISPLAY_MONOCHROME      0x0000
#define DISPLAY_CGA             0x0001
#define DISPLAY_EGA             0x0002
#define DISPLAY_VGA             0x0003
#define DISPLAY_8514A           0x0007
#define DISPLAY_IMAGEADAPTER    0x0008
#define DISPLAY_XGA             0x0009

/* VIOCONFIGINFO.display constants */
#define MONITOR_MONOCHROME      0x0000
#define MONITOR_COLOR           0x0001
#define MONITOR_ENHANCED        0x0002
#define MONITOR_8503            0x0003
#define MONITOR_851X_COLOR      0x0004
#define MONITOR_8514            0x0009
#define MONITOR_FLATPANEL       0x000A
#define MONITOR_8507_8604       0x000B
#define MONITOR_8515            0x000C
#define MONITOR_9515            0x000F
#define MONITOR_9517            0x0011
#define MONITOR_9518            0x0012

/* structure for VioGetConfig() */
typedef struct _VIOCONFIGINFO { /* vioin */
        USHORT  cb;
        USHORT  adapter;
        USHORT  display;
        ULONG   cbMemory;
        USHORT  Configuration;
        USHORT  VDHVersion;
        USHORT  Flags;
        ULONG   HWBufferSize;
        ULONG   FullSaveSize;
        ULONG   PartSaveSize;
        USHORT  EMAdaptersOFF;
        USHORT  EMDisplaysOFF;
        } VIOCONFIGINFO;
typedef VIOCONFIGINFO FAR *PVIOCONFIGINFO;

#define VIO_CONFIG_CURRENT         0
#define VIO_CONFIG_PRIMARY         1
#define VIO_CONFIG_SECONDARY       2

/* structure for VioGet/SetFont() */
typedef struct _VIOFONTINFO {   /* viofi */
        USHORT  cb;
        USHORT  type;
        USHORT  cxCell;
        USHORT  cyCell;
        LPVOID   pbData;
        USHORT  cbData;
        } VIOFONTINFO;
typedef VIOFONTINFO FAR *PVIOFONTINFO;

#define VGFI_GETCURFONT            0
#define VGFI_GETROMFONT            1

int WFCAPI        VioGetFont (PVIOFONTINFO pviofi, HVIO hvio);
int WFCAPI        VioSetFont (PVIOFONTINFO pviofi, HVIO hvio);

int WFCAPI        VioGetCp (USHORT usReserved, PUSHORT pIdCodePage, HVIO hvio);
int WFCAPI        VioSetCp (USHORT usReserved, USHORT idCodePage, HVIO hvio);

int WFCAPI        VioReadCellStr (PCH pchCellStr, PUSHORT pcb, USHORT usRow,
                                USHORT usColumn, HVIO hvio);
int WFCAPI        VioReadCharStr (PCH pchCellStr, PUSHORT pcb, USHORT usRow,
                                USHORT usColumn, HVIO hvio);

typedef struct _VIOPALSTATE {   /* viopal */
        USHORT  cb;
        USHORT  type;
        USHORT  iFirst;
        USHORT  acolor[1];
        }VIOPALSTATE;
typedef VIOPALSTATE FAR *PVIOPALSTATE;

typedef struct _VIOOVERSCAN {   /* vioos */
        USHORT  cb;
        USHORT  type;
        USHORT  color;
        }VIOOVERSCAN;
typedef VIOOVERSCAN FAR *PVIOOVERSCAN;

typedef struct _VIOINTENSITY {  /* vioint */
        USHORT  cb;
        USHORT  type;
        USHORT  fs;
        }VIOINTENSITY;
typedef VIOINTENSITY FAR *PVIOINTENSITY;

typedef struct _VIOCOLORREG {  /* viocreg */
        USHORT  cb;
        USHORT  type;
        USHORT  firstcolorreg;
        USHORT  numcolorregs;
        PCH     colorregaddr;
        }VIOCOLORREG;
typedef VIOCOLORREG FAR *PVIOCOLORREG;

typedef struct _VIOSETULINELOC {  /* viouline */
        USHORT  cb;
        USHORT  type;
        USHORT  scanline;
        }VIOSETULINELOC;
typedef VIOSETULINELOC FAR *PVIOSETULINELOC;

typedef struct _VIOSETTARGET {  /* viosett */
        USHORT  cb;
        USHORT  type;
        USHORT  defaultalgorithm;
        }VIOSETTARGET;
typedef VIOSETTARGET FAR *PVIOSETTARGET;

typedef struct _VIOSAVESCREEN {  /* viosave/resorescreen */
        USHORT        cbVideoBuf;
        PCHAR         pchVideoBuf;
        VIOMODEINFO   ModeInfo;
        VIOOVERSCAN   Overscan;
        VIOCURSORINFO CursorInfo;
        USHORT        Row;
        USHORT        Col;
        }VIOSAVESCREEN;
typedef VIOSAVESCREEN FAR *PVIOSAVESCREEN;

/*=============================================================================
==   C runtime library routines
=============================================================================*/

#define VIO__CLEARSCREEN       0
#define VIO__SAVESCREEN        1
#define VIO__RESTORESCREEN     2
#define VIO__DESTROYSCREEN     3
#define VIO__BEEP              4
#define VIO__ADDHOOK           5
#define VIO__REMOVEHOOK        6
#define VIO__GETMODE           7
#define VIO__SETMODE           8
#define VIO__GETCONFIG         9
#define VIO__GETSTATE         10
#define VIO__SETSTATE         11
#define VIO__WRTTTY           12
#define VIO__WRTCHARSTRATT    13
#define VIO__WRTCHARSTR       14
#define VIO__WRTCELLSTR       15
#define VIO__WRTNATTR         16
#define VIO__WRTNCELL         17
#define VIO__WRTNCHAR         18
#define VIO__GETCURPOS        19
#define VIO__SETCURPOS        20
#define VIO__GETCURTYPE       21
#define VIO__SETCURTYPE       22
#define VIO__SCROLLUP         23
#define VIO__SCROLLDN         24
#define VIO__SCROLLLF         25
#define VIO__SCROLLRT         26
#define VIO__READCELL         27
#define VIO__READCHAR         28
#define VIO__INITWINDOW       29
#define VIO__DESTROYWINDOW    30
#define VIO__PAINT            31
#define VIO__COUNT            32


#ifdef VIOLIB

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int WFCAPI        VioClearScreen ( VOID );
int WFCAPI        VioSaveScreen (PVIOSAVESCREEN pSaveScreen);
int WFCAPI        VioRestoreScreen (PVIOSAVESCREEN pSaveScreen);
int WFCAPI        VioDestroyScreen (PVIOSAVESCREEN pSaveScreen);
int WFCAPI        VioBeep (USHORT usFrequency, USHORT usDuration);
int WFCAPI        VioAddHook (LPVOID pProcedure);
int WFCAPI        VioRemoveHook (LPVOID pProcedure);
int WFCAPI        VioGetMode (PVIOMODEINFO pvioModeInfo, HVIO hvio);
int WFCAPI        VioSetMode (PVIOMODEINFO pvioModeInfo, HVIO hvio);
int WFCAPI        VioGetConfig (USHORT usConfigId, PVIOCONFIGINFO pvioin,
                                HVIO hvio);
int WFCAPI        VioGetState (LPVOID pState, HVIO hvio);
int WFCAPI        VioSetState (LPVOID pState, HVIO hvio);
int WFCAPI        VioWrtTTY (PCH pch, USHORT cb, HVIO hvio);
int WFCAPI        VioWrtCharStrAtt (PCH pch, USHORT cb, USHORT usRow,
                                    USHORT usColumn, LPBYTE pAttr, HVIO hvio);
int WFCAPI        VioWrtCharStr (PCH pchStr, USHORT cb, USHORT usRow,
                                 USHORT usColumn, HVIO hvio);
int WFCAPI        VioWrtCellStr (PCH pchCellStr, USHORT cb, USHORT usRow,
                                 USHORT usColumn, HVIO hvio);
int WFCAPI        VioWrtNAttr (LPBYTE pAttr, USHORT cb, USHORT usRow,
                               USHORT usColumn, HVIO hvio);
int WFCAPI        VioWrtNCell (LPBYTE pCell, USHORT cb, USHORT usRow,
                               USHORT usColumn, HVIO hvio);
int WFCAPI        VioWrtNChar (PCH pchChar, USHORT cb, USHORT usRow,
                               USHORT usColumn, HVIO hvio);
int WFCAPI        VioGetCurPos (PUSHORT pusRow, PUSHORT pusColumn, HVIO hvio);
int WFCAPI        VioSetCurPos (USHORT usRow, USHORT usColumn, HVIO hvio);
int WFCAPI        VioGetCurType (PVIOCURSORINFO pvioCursorInfo, HVIO hvio);
int WFCAPI        VioSetCurType (PVIOCURSORINFO pvioCursorInfo, HVIO hvio);
int WFCAPI        VioScrollUp (USHORT usTopRow, USHORT usLeftCol,
                               USHORT usBotRow, USHORT usRightCol,
                               USHORT cbLines, LPBYTE pCell, HVIO hvio);
int WFCAPI        VioScrollDn (USHORT usTopRow, USHORT usLeftCol,
                               USHORT usBotRow, USHORT usRightCol,
                               USHORT cbLines, LPBYTE pCell, HVIO hvio);
int WFCAPI        VioScrollLf (USHORT usTopRow, USHORT usLeftCol,
                               USHORT usBotRow, USHORT usRightCol,
                               USHORT cbCol, LPBYTE pCell, HVIO hvio);
int WFCAPI        VioScrollRt (USHORT usTopRow, USHORT usLeftCol,
                               USHORT usBotRow, USHORT usRightCol,
                               USHORT cbCol, LPBYTE pCell, HVIO hvio);
int WFCAPI        VioReadCellStr (PCH pchCellStr, PUSHORT pcb, USHORT usRow,
                                  USHORT usColumn, HVIO hvio);
int WFCAPI        VioReadCharStr (PCH pchCellStr, PUSHORT pcb, USHORT usRow,
                                  USHORT usColumn, HVIO hvio);
int WFCAPI        VioInitWindow( HWND hWnd, INT rowTotal, INT columnTotal, BOOL fTTY );
int WFCAPI        VioDestroyWindow( HWND hWnd );
int WFCAPI        VioPaint( HWND hWnd );

#else

/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int (PWFCAPI PFNVIOCLEARSCREEN) ( VOID );
typedef int (PWFCAPI PFNVIOSAVESCREEN) ( PVIOSAVESCREEN );
typedef int (PWFCAPI PFNVIORESTORESCREEN) ( PVIOSAVESCREEN );
typedef int (PWFCAPI PFNVIODESTROYSCREEN) ( PVIOSAVESCREEN );
typedef int (PWFCAPI PFNVIOBEEP) (USHORT, USHORT );
typedef int (PWFCAPI PFNVIOADDHOOK) ( LPVOID );
typedef int (PWFCAPI PFNVIOREMOVEHOOK) ( LPVOID );
typedef int (PWFCAPI PFNVIOGETMODE) ( PVIOMODEINFO, HVIO );
typedef int (PWFCAPI PFNVIOSETMODE) ( PVIOMODEINFO, HVIO );
typedef int (PWFCAPI PFNVIOGETCONFIG) ( USHORT, PVIOCONFIGINFO, HVIO );
typedef int (PWFCAPI PFNVIOGETSTATE) ( LPVOID, HVIO );
typedef int (PWFCAPI PFNVIOSETSTATE) ( LPVOID, HVIO );
typedef int (PWFCAPI PFNVIOWRTTTY) ( PCH, USHORT, HVIO );
typedef int (PWFCAPI PFNVIOWRTCHARSTRATT) ( PCH, USHORT, USHORT, 
                                            USHORT, LPBYTE, HVIO );
typedef int (PWFCAPI PFNVIOWRTCHARSTR) ( PCH, USHORT, USHORT, 
                                         USHORT, HVIO );
typedef int (PWFCAPI PFNVIOWRTCELLSTR) ( PCH, USHORT, USHORT,
                                         USHORT, HVIO );
typedef int (PWFCAPI PFNVIOWRTNATTR) ( LPBYTE, USHORT, USHORT, 
                                       USHORT, HVIO );
typedef int (PWFCAPI PFNVIOWRTNCELL) ( LPBYTE, USHORT, USHORT, 
                                       USHORT, HVIO );
typedef int (PWFCAPI PFNVIOWRTNCHAR) ( PCH, USHORT, USHORT, 
                                       USHORT, HVIO );
typedef int (PWFCAPI PFNVIOGETCURPOS) ( PUSHORT, PUSHORT, HVIO );
typedef int (PWFCAPI PFNVIOSETCURPOS) ( USHORT, USHORT, HVIO );
typedef int (PWFCAPI PFNVIOGETCURTYPE) ( PVIOCURSORINFO, HVIO );
typedef int (PWFCAPI PFNVIOSETCURTYPE) ( PVIOCURSORINFO, HVIO );
typedef int (PWFCAPI PFNVIOSCROLLUP) ( USHORT, USHORT, USHORT, USHORT,
                                       USHORT, LPBYTE, HVIO );
typedef int (PWFCAPI PFNVIOSCROLLDN) ( USHORT, USHORT, USHORT, USHORT,
                                       USHORT, LPBYTE, HVIO );
typedef int (PWFCAPI PFNVIOSCROLLLF) ( USHORT, USHORT, USHORT, USHORT,
                                       USHORT, LPBYTE, HVIO );
typedef int (PWFCAPI PFNVIOSCROLLRT) ( USHORT, USHORT, USHORT, USHORT,
                                       USHORT, LPBYTE, HVIO );
typedef int (PWFCAPI PFNVIOREADCELLSTR) ( PCH, PUSHORT, USHORT,
                                          USHORT, HVIO );
typedef int (PWFCAPI PFNVIOREADCHARSTR) ( PCH, PUSHORT, USHORT,
                                          USHORT, HVIO );
typedef int (PWFCAPI PFNVIOINITWINDOW)( HWND hWnd, INT rowTotal,
                                               INT columnTotal, BOOL fTTY );
typedef int (PWFCAPI PFNVIODESTROYWINDOW)( HWND hWnd );
typedef int (PWFCAPI PFNVIOPAINT)( HWND hWnd );

extern PPLIBPROCEDURE pVioProcedures;

#define VioClearScreen    ((PFNVIOCLEARSCREEN) (pVioProcedures[VIO__CLEARSCREEN]))
#define VioSaveScreen     ((PFNVIOSAVESCREEN) (pVioProcedures[VIO__SAVESCREEN]))
#define VioRestoreScreen  ((PFNVIORESTORESCREEN) (pVioProcedures[VIO__RESTORESCREEN]))
#define VioDestroyScreen  ((PFNVIODESTROYSCREEN) (pVioProcedures[VIO__DESTROYSCREEN]))
#define VioBeep           ((PFNVIOBEEP) (pVioProcedures[VIO__BEEP]))
#define VioAddHook        ((PFNVIOADDHOOK) (pVioProcedures[VIO__ADDHOOK]))
#define VioRemoveHook     ((PFNVIOREMOVEHOOK) (pVioProcedures[VIO__REMOVEHOOK]))
#define VioGetMode        ((PFNVIOGETMODE) (pVioProcedures[VIO__GETMODE]))
#define VioSetMode        ((PFNVIOSETMODE) (pVioProcedures[VIO__SETMODE]))
#define VioGetConfig      ((PFNVIOGETCONFIG) (pVioProcedures[VIO__GETCONFIG]))
#define VioGetState       ((PFNVIOGETSTATE) (pVioProcedures[VIO__GETSTATE]))
#define VioSetState       ((PFNVIOSETSTATE) (pVioProcedures[VIO__SETSTATE]))
#define VioWrtTTY         ((PFNVIOWRTTTY) (pVioProcedures[VIO__WRTTTY]))
#define VioWrtCharStrAtt  ((PFNVIOWRTCHARSTRATT) (pVioProcedures[VIO__WRTCHARSTRATT]))
#define VioWrtCharStr     ((PFNVIOWRTCHARSTR) (pVioProcedures[VIO__WRTCHARSTR]))
#define VioWrtCellStr     ((PFNVIOWRTCELLSTR) (pVioProcedures[VIO__WRTCELLSTR]))
#define VioWrtNAttr       ((PFNVIOWRTNATTR) (pVioProcedures[VIO__WRTNATTR]))
#define VioWrtNCell       ((PFNVIOWRTNCELL) (pVioProcedures[VIO__WRTNCELL]))
#define VioWrtNChar       ((PFNVIOWRTNCHAR) (pVioProcedures[VIO__WRTNCHAR]))
#define VioGetCurPos      ((PFNVIOGETCURPOS) (pVioProcedures[VIO__GETCURPOS]))
#define VioSetCurPos      ((PFNVIOSETCURPOS) (pVioProcedures[VIO__SETCURPOS]))
#define VioGetCurType     ((PFNVIOGETCURTYPE) (pVioProcedures[VIO__GETCURTYPE]))
#define VioSetCurType     ((PFNVIOSETCURTYPE) (pVioProcedures[VIO__SETCURTYPE]))
#define VioScrollUp       ((PFNVIOSCROLLUP) (pVioProcedures[VIO__SCROLLUP]))
#define VioScrollDn       ((PFNVIOSCROLLDN) (pVioProcedures[VIO__SCROLLDN]))
#define VioScrollLf       ((PFNVIOSCROLLLF) (pVioProcedures[VIO__SCROLLLF]))
#define VioScrollRt       ((PFNVIOSCROLLRT) (pVioProcedures[VIO__SCROLLRT]))
#define VioReadCellStr    ((PFNVIOREADCELLSTR) (pVioProcedures[VIO__READCELL]))
#define VioReadCharStr    ((PFNVIOREADCHARSTR) (pVioProcedures[VIO__READCHAR]))
#define VioInitWindow     ((PFNVIOINITWINDOW) (pVioProcedures[VIO__INITWINDOW]))
#define VioDestroyWindow  ((PFNVIODESTROYWINDOW) (pVioProcedures[VIO__DESTROYWINDOW]))
#define VioPaint          ((PFNVIOPAINT) (pVioProcedures[VIO__PAINT]))

#endif

#endif //__VIOAPI_H__
