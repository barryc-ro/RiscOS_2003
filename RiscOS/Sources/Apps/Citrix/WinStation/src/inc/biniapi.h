/******************************************************************************
*
*   BINIAPI.H
*
*   Header file for Buffered INI APIs.
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis
*
*   $Log$
*  
*     Rev 1.9   15 Apr 1997 18:44:42   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.8   06 May 1996 18:36:04   jeffm
*  update
*  
*     Rev 1.7   30 May 1995 08:26:44   butchd
*  update
*
*******************************************************************************/
#ifndef __BINIAPI_H__
#define __BINIAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
==   Macros
=============================================================================*/

/*=============================================================================
==   External functions provided by LoadLibraries()
=============================================================================*/

int cdecl WFCAPI BIniLoad( PPLIBPROCEDURE );
int cdecl WFCAPI BIniUnload( VOID );

/*=============================================================================
==   API entry points
=============================================================================*/

#define BINI__GETSTRING      0
#define BINI__GETINT         1
#define BINI__GETLONG        2
#define BINI__GETBOOL        3
#define BINI__GETSECTLEN     4
#define BINI__COUNT          5

typedef int  (WFCAPI FNBGETPRIVATEPROFILESTRING)( PCHAR, PCHAR, PCHAR, PCHAR, int );
typedef FNBGETPRIVATEPROFILESTRING far * PFNBGETPRIVATEPROFILESTRING;
typedef int  (WFCAPI FNBGETPRIVATEPROFILEINT)( PCHAR, PCHAR, int );
typedef FNBGETPRIVATEPROFILEINT far * PFNBGETPRIVATEPROFILEINT;
typedef long (WFCAPI FNBGETPRIVATEPROFILELONG)( PCHAR, PCHAR, long );
typedef FNBGETPRIVATEPROFILELONG far * PFNBGETPRIVATEPROFILELONG;
typedef int  (WFCAPI FNBGETPRIVATEPROFILEBOOL)( PCHAR, PCHAR, int );
typedef FNBGETPRIVATEPROFILEBOOL far * PFNBGETPRIVATEPROFILEBOOL;
typedef int  (WFCAPI FNBGETSECTIONLENGTH)( PCHAR );
typedef FNBGETSECTIONLENGTH far * PFNBGETSECTIONLENGTH;

#ifdef BINILIB

FNBGETPRIVATEPROFILESTRING bGetPrivateProfileString;
FNBGETPRIVATEPROFILEINT    bGetPrivateProfileInt;
FNBGETPRIVATEPROFILELONG   bGetPrivateProfileLong;
FNBGETPRIVATEPROFILEBOOL   bGetPrivateProfileBool;
FNBGETSECTIONLENGTH        bGetSectionLength;
        
#else

extern PPLIBPROCEDURE pBIniProcedures;

#define bGetPrivateProfileString ((PFNBGETPRIVATEPROFILESTRING)(pBIniProcedures[BINI$GETSTRING]))
#define bGetPrivateProfileInt    ((PFNBGETPRIVATEPROFILEINT)(pBIniProcedures[BINI$GETINT]))
#define bGetPrivateProfileLong   ((PFNBGETPRIVATEPROFILELONG)(pBIniProcedures[BINI$GETLONG]))
#define bGetPrivateProfileBool   ((PFNBGETPRIVATEPROFILEBOOL)(pBIniProcedures[BINI$GETBOOL]))
#define bGetSectionLength        ((PFNBGETSECTIONLENGTH)(pBIniProcedures[BINI$GETSECTLEN]))

#endif

#ifdef __cplusplus
}
#endif

#endif //__BINIAPI_H__
