
/***************************************************************************
*
*  DLL.H
*
*  This module contains client DLL access definitions
*
*  Copyright 1994, Citrix Systems Inc.
*
*  Author: Butch Davis (5/22/95) - from client.h
*
*  $Log$
*  
*     Rev 1.3   15 Apr 1997 18:45:04   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   06 May 1996 19:15:32   jeffm
*  update
*  
*     Rev 1.1   22 May 1995 18:19:04   butchd
*  update
*  
*     Rev 1.0   22 May 1995 13:19:46   butchd
*  Initial revision.
*
****************************************************************************/

#ifndef __DLL_H__
#define __DLL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  DLL link structure
 */
typedef struct _DLLLINK {
// NOTE: 1st six elements must be same as MINIDLL
    USHORT Segment;                 // starting seg of mem allocated for dll (aligned)
    USHORT DllSize;                 // actual size of dll in paragraphs
    USHORT ProcCount;               // number of procedures in call table
    PVOID pProcedures;              // pointer to procedure call table
    PVOID pData;                    // pointer to dll data structure
    PUCHAR pMemory;                 // pointer to malloced memory (not aligned)
    BYTE ModuleName[13];            // client module name (8.3)
    USHORT ModuleDate;              // module date in dos format
    USHORT ModuleTime;              // module time in dos format
    ULONG ModuleSize;               // module file size in bytes
    struct _DLLLINK * pNext;        // pointer to next DLLLINK structure
#if 0 // ndef DOS
    HINSTANCE hinst;
#endif
    // everything after here is not included for the ModuleEnum call.
    // be sure to readjust the ENUM_DLLLINK_SIZE if you add new fields
    // below here
    ULONG DllFlags;                 // DLL flags (embed..)
} DLLLINK, * PDLLLINK;

//
// we can only copy some of the information for ModuleEnum
// since we want to stay compatible with different WDs.
// this keeps us compatible with old calls to ModuleEnum
// from WDICA???.DLL
//
#define ENUM_DLLLINK_SIZE (sizeof(DLLLINK)-sizeof(ULONG))  

// Dll flags definition (32 of them possible)
#define DLL_FLAGS_EMBED    1

typedef int (PWFCAPI PLOADPROCEDURE)( PDLLLINK, USHORT );
typedef int (PWFCAPI PDLLPROCEDURE)( PVOID, PVOID );

#define MODULE__LOAD            0
#define MODULE__UNLOAD          1
#define MODULE__ENUM            2
#define MODULE__CALL            3

int WFCAPI ModuleInit( char *, PDLLLINK, PPLIBPROCEDURE );
extern PPLIBPROCEDURE pModuleProcedures;
#ifdef DLLLIB

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int WFCAPI ModuleLoad( char *, PDLLLINK );
int WFCAPI ModuleUnload( PDLLLINK );
int WFCAPI ModuleEnum( LPBYTE, USHORT, PUSHORT );
int WFCAPI ModuleCall( PDLLLINK, USHORT, PVOID );

int ModuleLookup( PCHAR pName, PLIBPROCEDURE *pfnLoad, PPLIBPROCEDURE *pfnTable );
    
#else

/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int (PWFCAPI PMODULELOAD)( char *, PDLLLINK );
typedef int (PWFCAPI PMODULEUNLOAD)( PDLLLINK );
typedef int (PWFCAPI PMODULEENUM)( LPBYTE, USHORT, PUSHORT );
typedef int (PWFCAPI PMODULECALL)( PDLLLINK, USHORT, PVOID );


#define ModuleLoad     ((PMODULELOAD)  (pModuleProcedures[MODULE__LOAD]))
#define ModuleUnload   ((PMODULEUNLOAD)(pModuleProcedures[MODULE__UNLOAD]))
#define ModuleEnum     ((PMODULEENUM)  (pModuleProcedures[MODULE__ENUM]))
#define ModuleCall     ((PMODULECALL)  (pModuleProcedures[MODULE__CALL]))

#endif

/*
 *  ExeOpen structure
 */
typedef struct _EXEOPEN {
    PVOID pIniSection;
} EXEOPEN, * PEXEOPEN;


/*=============================================================================
 ==   Common dll entry points for all dlls
 ============================================================================*/

#define DLL__LOAD                 0
#define DLL__UNLOAD               1
#define DLL__OPEN                 2
#define DLL__CLOSE                3
#define DLL__INFO                 4
#define DLL__POLL                 5

/*
 *  DLL__CLOSE structure
 */
typedef struct _DLLCLOSE {
    int NotUsed;
} DLLCLOSE, * PDLLCLOSE;

/*
 *  DLL__INFO structure
 */
typedef struct _DLLINFO {
    LPBYTE pBuffer;
    USHORT ByteCount;
} DLLINFO, * PDLLINFO;

/*
 *  DLL__POLL structure
 */
typedef struct _DLLPOLL {
    ULONG CurrentTime;          // current time in msec
} DLLPOLL, * PDLLPOLL;

typedef INT (PWFCAPI PEMBEDLOAD)( PDLLLINK pLink );

#ifdef WIN16
typedef INT (CALLBACK *PDLLMAIN)( HINSTANCE, WORD, WORD, LPSTR);
#endif
#ifdef WIN32
typedef INT (WINAPI *PDLLMAIN)( HINSTANCE, DWORD, LPVOID);
#endif

typedef struct _EMBEDDLL {
  char      *DllName;                  // name of DLL
  PEMBEDLOAD DllLoad;                  // DLL load function
  PVOID      pfnMain;                  // DLL LIBMAIN or DLLMain entry
} EMBEDDLL, *PEMBEDDLL;




#ifdef __cplusplus
}
#endif

#endif //__DLL_H__
