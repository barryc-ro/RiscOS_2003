///////////////////////////////////////////////////////////////////////////////
//
//  MIAPI.H
//
//  Header file for Memory INI APIs
//
//  Copyright Citrix Systems Inc. 1997
//
//  Author: David Pope  6 Jun 97
//
//  $Log : $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIAPI_H__
#define __MIAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MEMINI_STR_LEN    40
#define MAXLINEBUF           128


/*=============================================================================
==   External functions provided by LoadLibraries()
=============================================================================*/

int cdecl WFCAPI MemIniLoad( PPLIBPROCEDURE );
int cdecl WFCAPI MemIniUnload( VOID );

/*=============================================================================
==   API entry points
=============================================================================*/

#define MEMINI__SETPROFILEPOINTER        0
#define MEMINI__SETSECTIONPOINTER        1
#define MEMINI__GETPROFILESTRING         2
#define MEMINI__GETPROFILEINT            3
#define MEMINI__GETPROFILELONG           4
#define MEMINI__GETPROFILEBOOL           5 
#define MEMINI__RELEASEPROFILE           6

#define MEMINI__COUNT                    7 

typedef int (WFCAPI FNMISETPROFILEPOINTER)( PCHAR );
typedef FNMISETPROFILEPOINTER far * PFNMISETPROFILEPOINTER;

typedef int (WFCAPI FNMISETSECTIONPOINTER)( PCHAR );
typedef FNMISETSECTIONPOINTER far * PFNMISETSECTIONPOINTER;

typedef int (WFCAPI FNMIGETPRIVATEPROFILESTRING)( PCHAR, PCHAR, PCHAR,
                                                  PCHAR, int);
typedef FNMIGETPRIVATEPROFILESTRING far * PFNMIGETPRIVATEPROFILESTRING;

typedef int (WFCAPI FNMIGETPRIVATEPROFILEINT)( PCHAR, PCHAR, int);
typedef FNMIGETPRIVATEPROFILEINT far * PFNMIGETPRIVATEPROFILEINT;

typedef LONG (WFCAPI FNMIGETPRIVATEPROFILELONG)( PCHAR, PCHAR, long);
typedef FNMIGETPRIVATEPROFILELONG far * PFNMIGETPRIVATEPROFILELONG;

typedef int (WFCAPI FNMIGETPRIVATEPROFILEBOOL)( PCHAR, PCHAR, BOOL);
typedef FNMIGETPRIVATEPROFILEBOOL far * PFNMIGETPRIVATEPROFILEBOOL;

typedef int (WFCAPI FNMIRELEASEPROFILE)(void );
typedef FNMIRELEASEPROFILE far * PFNMIRELEASEPROFILE;

#define MEMINILIB
    
#ifdef MEMINILIB
// compiling the library itself

FNMISETPROFILEPOINTER           miSetProfilePointer;
FNMISETSECTIONPOINTER           miSetSectionPointer;
FNMIGETPRIVATEPROFILESTRING     miGetPrivateProfileString;
FNMIGETPRIVATEPROFILEINT        miGetPrivateProfileInt;
FNMIGETPRIVATEPROFILELONG       miGetPrivateProfileLong;
FNMIGETPRIVATEPROFILEBOOL       miGetPrivateProfileBool;
FNMIRELEASEPROFILE              miReleaseProfile;

#else
// compiling someone else who uses the library

extern PPLIBPROCEDURE pMemIniProcedures;

#define miSetProfilePointer \
    ((PFNMISETPROFILEPOINTER)(pMemIniProcedures[MEMINI__SETPROFILEPOINTER]))
#define miSetSectionPointer \
    ((PFNMISETSECTIONPOINTER)(pMemIniProcedures[MEMINI__SETSECTIONPOINTER]))
#define miGetPrivateProfileString \
    ((PFNMIGETPRIVATEPROFILESTRING)(pMemIniProcedures[MEMINI__GETPROFILESTRING]))
#define miGetPrivateProfileInt \
    ((PFNMIGETPRIVATEPROFILEINT)(pMemIniProcedures[MEMINI__GETPROFILEINT]))
#define miGetPrivateProfileLong \
    ((PFNMIGETPRIVATEPROFILELONG)(pMemIniProcedures[MEMINI__GETPROFILELONG]))
#define miGetPrivateProfileBool \
    ((PFNMIGETPRIVATEPROFILEBOOL)(pMemIniProcedures[MEMINI__GETPROFILEBOOL]))
#define miReleaseProfile \
    ((PFNMIRELEASEPROFILE)(pMemIniProcedures[MEMINI__RELEASEPROFILE]))

#endif  // MEMINILIB

#ifdef __cplusplus
}
#endif

#endif //__MIAPI_H__
