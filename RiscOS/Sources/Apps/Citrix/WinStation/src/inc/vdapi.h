/***************************************************************************
*
*  VDAPI.H
*
*  This module contains external virtual driver API defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Andy 4/6/94
*
*  $Log$
*  
*     Rev 1.35   15 Apr 1997 18:46:04   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.34   06 May 1996 19:16:34   jeffm
*  update
*  
*     Rev 1.33   20 Dec 1995 15:07:56   bradp
*  update
*  
*     Rev 1.32   08 Nov 1995 09:45:24   marcb
*  update
*  
*     Rev 1.31   25 Sep 1995 16:31:52   kurtp
*  update
*  
*     Rev 1.30   25 Sep 1995 12:33:02   kurtp
*  update
*  
*     Rev 1.29   13 May 1995 15:24:24   marcb
*  update
*  
*     Rev 1.28   08 May 1995 16:43:06   butchd
*  update
*  
*     Rev 1.27   03 May 1995 12:52:02   marcb
*  update
*  
*     Rev 1.26   02 May 1995 13:41:00   butchd
*  update
*
****************************************************************************/
#ifndef __VDAPI_H__
#define __VDAPI_H__

/*
 *  Index into PD procedure array
 */
//      DLL$LOAD                 0
//      DLL$UNLOAD               1
//      DLL$OPEN                 2
//      DLL$CLOSE                3
//      DLL$INFO                 4
//      DLL$POLL                 5
#define VD__QUERYINFORMATION   	 6
#define VD__SETINFORMATION     	 7
#define VD__COUNT              	 8   /* number of external procedures */


/*
 *  VdOpen structure
 */
typedef struct _VDOPEN {
    PPLIBPROCEDURE pModuleProcedures;
    PPLIBPROCEDURE pClibProcedures;
    PPLIBPROCEDURE pMouProcedures;
    PPLIBPROCEDURE pTimerProcedures;
    PPLIBPROCEDURE pLptProcedures;
    PPLIBPROCEDURE pXmsProcedures;
    PPLIBPROCEDURE pLogProcedures;
    PPLIBPROCEDURE pBIniProcedures;
    PPLIBPROCEDURE pKbdProcedures;
    LPVOID pIniSection;
    PDLLLINK pWdLink;
    ULONG ChannelMask;      // bit 0 = Virtual_Screen
    PPLIBPROCEDURE pfnWFEngPoll;
    PPLIBPROCEDURE pfnStatusMsgProc;
    PPLIBPROCEDURE pDeviceProcedures;
} VDOPEN, far * PVDOPEN;


/*
 *  VdInformationClass enum
 *   additional information classes can be defined for a given
 *   VD, as long as the number used is greater than that generated
 *   by this enum list
 */

typedef enum _VDINFOCLASS {
    VdLastError,
    VdKillFocus,
    VdSetFocus,
    VdMousePosition,
    VdThinWireCache,
    VdThinWire20Entries,
    VdDisableModule,
    VdFlush,
    VdInitWindow,
    VdDestroyWindow,
    VdPaint,
    VdThinwireStack,
    VdRealizePaletteFG,   // inform client to realize it's foreground palette
    VdRealizePaletteBG,   // inform client to realize it's background palette
    VdInactivate,         // client is about to lose input focus
    VdGetSecurityAccess,  // cdm security info
    VdSetSecurityAccess   // cdm security info
} VDINFOCLASS;

/*
 *  VdQueryInformation structure
 */
typedef struct _VDQUERYINFORMATION {
    VDINFOCLASS VdInformationClass;
    LPVOID pVdInformation;
    int VdInformationLength;
    int VdReturnLength;
} VDQUERYINFORMATION, * PVDQUERYINFORMATION;

/*
 *  VdSetInformation structure
 */
typedef struct _VDSETINFORMATION {
    VDINFOCLASS VdInformationClass;
    LPVOID pVdInformation;
    int VdInformationLength;
} VDSETINFORMATION, * PVDSETINFORMATION;

/*
 *  VdLastError
 */
typedef struct _VDLASTERROR {
    int Error;
    char Message[ MAX_ERRORMESSAGE ];
} VDLASTERROR, * PVDLASTERROR;

/*
 *  VdThinWireCache
 */
typedef struct _VDTWCACHE {
    ULONG ulXms;
    ULONG ulLowMem;
    ULONG ulDASD;
    ULONG ulTiny;
    char  pszCachePath[MAXPATH+1];
} VDTWCACHE, * PVDTWCACHE;

typedef struct _MOUSEPOSITION {
   USHORT X;
   USHORT Y;
} MOUSEPOSITION, far * PMOUSEPOSITION;

/*
 * VD Flush
 */
typedef struct _VDFLUSH {
    UCHAR Channel;
    UCHAR Mask;
} VDFLUSH, * PVDFLUSH;

/*
 * VD Thinwire Stack
 */
typedef struct _VDTWSTACK {
    LPBYTE pTop;
    LPBYTE pMiddle;
    LPBYTE pBottom;
} VDTWSTACK, * PVDTWSTACK;


// entrypoints
typedef int  (PWFCAPI PTW20PKT)( LPVOID, USHORT );
typedef int  (PWFCAPI PTW20ENTRY)( VOID );

/* Query Information
 *  VdThinWire20Entries
 */
typedef struct _VDTW20ENTRIES {
   PTW20PKT    pfnTWDisplayPacket;
   PTW20PKT    pfnTWSystemPacket;
   PTW20ENTRY  pfnTWWindowsInit;
   PTW20ENTRY  pfnTWWindowsSetFocus;
   PTW20ENTRY  pfnTWWindowsKillFocus;
   PTW20ENTRY  pfnTWWindowsDisable;
   PTW20ENTRY  pfnTWWindowsDestroy;
   PTW20ENTRY  pfnThinwireSetup;
} VDTW20ENTRIES, far *PVDTW20ENTRIES;

/*
 *  VdGet/VdSetSecurityAccess (ULONG)
 */

#define CDM_SECURITY_ACCESS_NONE         0x00000000L

//  benign access
#define CDM_SECURITY_ACCESS_FIND_FIRST   0x00000001L
#define CDM_SECURITY_ACCESS_OPEN_READ    0x00000002L
#define CDM_SECURITY_ACCESS_GET_FINFO    0x00000004L
#define CDM_SECURITY_ACCESS_VOL_INFO     0x00000008L
#define CDM_SECURITY_ACCESS_READ         0x0000000FL    // all above

//  destructive access
#define CDM_SECURITY_ACCESS_OPEN_WRITE   0x00010000L
#define CDM_SECURITY_ACCESS_OPEN_CREATE  0x00020000L
#define CDM_SECURITY_ACCESS_SET_FINFO    0x00040000L
#define CDM_SECURITY_ACCESS_REMOVE       0x00080000L
#define CDM_SECURITY_ACCESS_RENAME       0x00100000L
#define CDM_SECURITY_ACCESS_CREATE_DIR   0x00200000L
#define CDM_SECURITY_ACCESS_REMOVE_DIR   0x00400000L
#define CDM_SECURITY_ACCESS_WRITE        0x007F0000L    // all above

//  all access
#define CDM_SECURITY_ACCESS_ALL          (CDM_SECURITY_ACCESS_READ|CDM_SECURITY_ACCESS_WRITE)


#endif //__VDAPI_H__
