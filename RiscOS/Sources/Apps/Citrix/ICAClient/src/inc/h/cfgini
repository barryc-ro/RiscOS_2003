
/*************************************************************************
*
*   CFGINI.H
*
*   Configuration INI library header file
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (4/12/95)
*
*   $Log$
*  
*     Rev 1.7   15 Apr 1997 18:44:44   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   06 May 1996 18:35:54   jeffm
*  update
*  
*     Rev 1.5   24 Jul 1995 18:31:42   butchd
*  CPR 1566: fix command line overrides
*
*************************************************************************/

#ifndef __CFGINI_H__
#define __CFGINI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Typedefs and defines.
 */
typedef struct _CFGINIOVERRIDE {
    LPSTR pszKey;
    LPSTR pszValue;
} CFGINIOVERRIDE, * PCFGINIOVERRIDE;

/*
 *  Library routines
 */
int WFCAPI CfgIniLoad( HANDLE, LPSTR, PCFGINIOVERRIDE,
                       LPSTR, LPSTR, LPSTR, LPSTR );

/*
 *  Call thru pointer for DLL UI usage
 */
typedef int (PWFCAPI PFNCFGINILOAD)( HANDLE, LPSTR, PCFGINIOVERRIDE,
                                     LPSTR, LPSTR, LPSTR, LPSTR );
extern PFNCFGINILOAD pfnCfgIniLoad;

#ifdef __cplusplus
}
#endif
#endif //__CFGINI_H__
