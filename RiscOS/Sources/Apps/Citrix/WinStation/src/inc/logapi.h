/******************************************************************************
*
*  LOGAPI.H
*
*     Header file for Logging and Tracing
*
*
*  Copyright Citrix Systems Inc. 1990
*
*  $Author:      Brad Pedersen
*
*  logapi.h,v
*  Revision 1.1  1998/01/12 11:36:51  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.20   15 Apr 1997 18:45:22   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.19   06 May 1996 18:39:10   jeffm
*  update
*  
*     Rev 1.18   22 Dec 1995 15:13:02   bradp
*  update
*  
*     Rev 1.17   25 May 1995 15:52:34   butchd
*  update
*
*******************************************************************************/

#ifndef __LOGAPI_H__
#define __LOGAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
==   Logging and Tracing flag definitions
=============================================================================*/

#include "logflags.h"


/*=============================================================================
==   Logging structures
=============================================================================*/

typedef struct _LOGOPEN {
   ULONG LogFlags;
   ULONG LogClass;
   ULONG LogEnable;
   FILEPATH LogFile;
} LOGOPEN, far *PLOGOPEN;


/*=============================================================================
==   External functions called by config library
=============================================================================*/

int WFCAPI LogLoad( PPLIBPROCEDURE );


/*=============================================================================
==   Logging functions
=============================================================================*/

#if 0
#define LOG$OPEN            0
#define LOG$CLOSE           1
#define LOG$PRINTF          2
#define LOG$BUFFER          3
#define LOG$ASSERT          4
#endif

#ifdef LOGLIB
/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int  WFCAPI LogOpen( PLOGOPEN pLogOpen );
int  WFCAPI LogClose( void );
void WFCAPI LogPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, ... );
void WFCAPI LogVPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, PVOID arg_marker);
void WFCAPI LogBuffer( ULONG LogClass, ULONG LogEnable,
                       LPVOID pBuffer, ULONG ByteCount );
void WFCAPI LogAssert( PCHAR, PCHAR, int, int );
void LogErr( void *err, PCHAR pFileName, int LineNumber );

#else
/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int  (PWFCAPI PFNLOGOPEN)( PLOGOPEN pLogOpen );
typedef int  (PWFCAPI PLOGCLOSE)( void );
typedef void (PWFCAPI PLOGPRINTF)( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, ... );
typedef void (PWFCAPI PLOGBUFFER)( ULONG LogClass, ULONG LogEnable,
                                   LPVOID pBuffer, ULONG ByteCount );
typedef void (PWFCAPI PLOGASSERT)( PCHAR, PCHAR, int, int );

extern PPLIBPROCEDURE pLogProcedures;

#define LogOpen           ((PFNLOGOPEN)(pLogProcedures[LOG$OPEN]))
#define LogClose          ((PLOGCLOSE)(pLogProcedures[LOG$CLOSE]))
#define LogPrintf         if (pLogProcedures) ((PLOGPRINTF)(pLogProcedures[LOG$PRINTF])) 
#define LogBuffer         if (pLogProcedures) ((PLOGBUFFER)(pLogProcedures[LOG$BUFFER])) 
#define LogAssert         if (pLogProcedures) ((PLOGASSERT)(pLogProcedures[LOG$ASSERT])) 

#endif

#ifdef __cplusplus
}
#endif

#endif //__LOGAPI_H__
