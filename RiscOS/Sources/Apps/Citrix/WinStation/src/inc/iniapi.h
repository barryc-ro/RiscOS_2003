/******************************************************************************
*
*   INIAPI.H
*
*   DOS INI Library header file
*
*   Copyright Citrix Systems Inc. 1994
*
*   Author: Kurt Perry
*
*   $Log$
*  
*     Rev 1.20   25 Feb 1998 16:46:00   TOMA
*  CE Merge
*  
*     Rev 1.20   14 Aug 1997 18:55:30   KenB
*  Fix GetProfileString reference for WinCE in DLL\VD\VDSPL\SPOOL\NTSPOOL.C
*  
*     Rev 1.19   15 Apr 1997 18:45:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.18   06 May 1996 18:38:36   jeffm
*  update
*  
*     Rev 1.17   08 Nov 1995 15:54:06   butchd
*  arranged ifdefs for Windows or DOS usage
*  
*     Rev 1.16   07 Nov 1995 17:56:32   KenB
*  add __cplusplus wrapper; only do macros if INILIB defined or DOS not defined
*  
*     Rev 1.15   25 May 1995 15:52:04   butchd
*  update
*  
*     Rev 1.14   02 May 1995 13:39:34   butchd
*  update
*
*******************************************************************************/
#ifndef __INIAPI_H__
#define __INIAPI_H__


#ifdef __cplusplus
extern "C" {
#endif

#if defined(DOS) || defined(WINCE) || defined(RISCOS)
/*=============================================================================
==   Macros
=============================================================================*/
#ifdef GetPrivateProfileString
#undef GetPrivateProfileString
#endif
#ifdef GetPrivateProfileInt
#undef GetPrivateProfileInt
#endif
#ifdef GetProfileString
#undef GetProfileString
#endif
#define GetPrivateProfileString dosGetPrivateProfileString
#define GetPrivateProfileInt dosGetPrivateProfileInt
#define GetProfileString(a,b,c,d,e) dosGetPrivateProfileString(a,b,c,d,e,NULL)

/*=============================================================================
==   External functions provided by LoadLibraries()
=============================================================================*/

int cdecl WFCAPI IniLoad( PPLIBPROCEDURE );
int cdecl WFCAPI IniUnload( VOID );

/*=============================================================================
==   C runtime library routines
=============================================================================*/

#define INI__GETSTRING      0
#define INI__GETINT         1
#define INI__GETLONG        2
#define INI__GETBOOL        3
#define INI__FLUSHCACHE     4
#define INI__GETENTRY       5
#define INI__GETPPPSEC      6
#define INI__REFRESH        7
#define INI__COUNT          8
#endif // DOS || WINCE || RISCOS

#if defined(INILIB) || !defined(DOS)

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int  WFCAPI dosGetPrivateProfileString( PCHAR, PCHAR, PCHAR, PCHAR, int, PCHAR );
int  WFCAPI dosGetPrivateProfileInt( PCHAR, PCHAR, int, PCHAR );
long WFCAPI GetPrivateProfileLong( PCHAR, PCHAR, long, PCHAR );
int  WFCAPI GetPrivateProfileBool( PCHAR, PCHAR, int, PCHAR );
void WFCAPI FlushPrivateProfileCache( VOID );
int  WFCAPI GetPrivateProfileEntry( PCHAR pszSection, int entry,
                                    PCHAR pszKey,     int cbKey,
                                    PCHAR pszValue,   int cbValue,
                                    PCHAR pszFile );
int  WFCAPI GetPrivatePrivateProfileSection( PCHAR   pszSection,
                                             PUSHORT poffBegin,
                                             PUSHORT poffEnd,
                                             PCHAR   pszFile );
int  WFCAPI RefreshProfile( PCHAR pszFile );

#else

/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int  (PWFCAPI PDOSGETPRIVATEPROFILESTRING)( PCHAR, PCHAR, PCHAR, PCHAR, int, PCHAR );
typedef int  (PWFCAPI PDOSGETPRIVATEPROFILEINT   )( PCHAR, PCHAR, int, PCHAR );
typedef long (PWFCAPI PGETPRIVATEPROFILELONG     )( PCHAR, PCHAR, long, PCHAR );
typedef int  (PWFCAPI PGETPRIVATEPROFILEBOOL     )( PCHAR, PCHAR, int, PCHAR );
typedef void (PWFCAPI PFLUSHPRIVATEPROFILECACHE  )( VOID );
typedef int  (PWFCAPI PGETPRIVATEPROFILEENTRY    )( PCHAR, int, PCHAR, int, PCHAR, int, PCHAR );
typedef int  (PWFCAPI PGETPRIVATEPRIVATEPROFILESECTION)( PCHAR, PUSHORT, PUSHORT, PCHAR );
typedef int  (PWFCAPI PREFRESHPROFILE            )( PCHAR );

extern PPLIBPROCEDURE pIniProcedures;

#define dosGetPrivateProfileString      ((PDOSGETPRIVATEPROFILESTRING)(pIniProcedures[INI$GETSTRING]))
#define dosGetPrivateProfileInt         ((PDOSGETPRIVATEPROFILEINT   )(pIniProcedures[INI$GETINT]))
#define GetPrivateProfileLong           ((PGETPRIVATEPROFILELONG     )(pIniProcedures[INI$GETLONG]))
#define GetPrivateProfileBool           ((PGETPRIVATEPROFILEBOOL     )(pIniProcedures[INI$GETBOOL]))
#define FlushPrivateProfileCache        ((PFLUSHPRIVATEPROFILECACHE  )(pIniProcedures[INI$FLUSHCACHE]))
#define GetPrivateProfileEntry          ((PGETPRIVATEPROFILEENTRY    )(pIniProcedures[INI$GETENTRY]))
#define GetPrivatePrivateProfileSection ((PGETPRIVATEPRIVATEPROFILESECTION)(pIniProcedures[INI$GETPPPSEC]))
#define RefreshProfile                  ((PREFRESHPROFILE            )(pIniProcedures[INI$REFRESH]))

#endif

#ifdef __cplusplus
}
#endif

#endif //__INIAPI_H__
