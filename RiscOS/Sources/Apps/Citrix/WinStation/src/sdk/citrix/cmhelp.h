/*
 *   CMHELP.H
 *      This module contains the definitions used by the CUD Helper
 *      function and DLL.
 *
 *   Included by:
 *       Client Update Database DLL
 * 
 *   Author:   Brad Anderson
 *   Date:     1/9/98
 *
 *   Copyright (C) Citrix Systems, Inc  1997, 1998
 *
 *   $Log  $
 *
 */

#ifndef _CMHELP_H_
#define _CMHELP_H_

#define _CMHLPAPI __cdecl

#ifdef __cplusplus
extern "C" {
#endif

/* ICA Client Update Helper Interfaces definitions */
typedef int (_CMHLPAPI *PCMHELPMOVEFILESA)(ULONG Flags,
                                        PCHAR pMediaDir,
                                        PCHAR pInstallDir);
typedef int (_CMHLPAPI *PCMHELPMOVEFILESW)(ULONG Flags,
                                        PWCHAR pMediaDir,
                                        PWCHAR pInstallDir);

#ifdef UNICODE
#define PCMHELPMOVEFILES PCMHELPMOVEFILESW
#else
#define PCMHELPMOVEFILES PCMHELPMOVEFILESA
#endif


/* Forword references */
typedef struct _CUDHELPERW *PCUDHELPERW;
typedef struct _CUDHELPERA *PCUDHELPERA;

typedef int (_CMHLPAPI *PCMHELPINITIALIZEW)(PCUDHELPERW);
typedef int (_CMHLPAPI *PCMHELPINITIALIZEA)(PCUDHELPERA);

#ifdef UNICODE
#define PCMHELPINITIALIZE PCMHELPINITIALIZEW
#else
#define PCMHELPINITIALIZE PCMHELPINITIALIZEA
#endif


typedef int (_CMHLPAPI *PCMHELPTERMINATE)();

typedef struct _CUDHELPERW {
    PCMHELPMOVEFILESW pHelpMoveFiles;
    PCMHELPTERMINATE pHelpTerminate;
} CUDHELPERW, *PCUDHELPERW;

typedef struct _CUDHELPERA {
    PCMHELPMOVEFILESA pHelpMoveFiles;
    PCMHELPTERMINATE pHelpTerminate;
} CUDHELPERA, *PCUDHELPERA;

#ifdef UNICODE
#define CUDHELPER CUDHELPERW
#define PCUDHELPER PCUDHELPERW
#else
#define CUDHELPER CUDHELPERA 
#define PCUDHELPER PCUDHELPERA 
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CMHELP_H_ */
