/************************************************************************** 
*    CMOEM.H
*       This header contains the definitions related to the
*       OEM specific interfaces used by the Client Manager VD.
*
*       Structure definitions may be shared between the ICA client
*       and Citrix Server.  ICA Client interfaces are only defined
*       when _CM_CLIENT_ is defined.
*
*   Includeded by:
*       Client Manager VD
*       OEM File System Layer
* 
*   Author:   Brad Anderson
*   Date:     10/29/97
*
*   Copyright (C) Citrix Systems, Inc  1997
*
*   $Log$
*  
*     Rev 1.4   25 Apr 1998 10:58:08   brada
*  Move citrix specific macros to new cmctx.h
*  
*     Rev 1.3   02 Feb 1998 13:10:52   brada
*  Add support to close file handles
*  
*      Rev 1.2   26 Jan 1998 18:27:28   brada
*   Allow client to choose UNICODE or Multibyte strings
*
**************************************************************************/

#ifndef _CMOEM_H_
#define _CMOEM_H_

#include "citrix/cmctx.h"

#define _CMOEMAPI __cdecl


/*
 *  ICA Client Update Data Structures
 */
typedef struct _CMOEM *PCMOEM;

/*
 *  Client Manager File System APIs
 */

typedef int (_CMOEMAPI *POEMINITIALIZE)(PCMOEM, LPULONG Version);
typedef int (_CMOEMAPI *POEMQUERYCLIENTINFO)(PCMOEM, PCMCLIENTINFO);
typedef int (_CMOEMAPI *POEMQUERYUPDATEINFO)(PCMOEM, PCMUPDATEINFO);
typedef int (_CMOEMAPI *POEMSETCLIENTVERSION)(PCMOEM, ULONG);
typedef int (_CMOEMAPI *POEMCREATECACHEFILE)(PCMOEM, LPBYTE, ULONG, ULONG, ULONG, LPULONG);
typedef int (_CMOEMAPI *POEMCLOSECACHEFILE)(PCMOEM, LPBYTE, ULONG, ULONG);
typedef int (_CMOEMAPI *POEMQUERYFILE)(PCMOEM, LPBYTE, PCMFILEINFO);
typedef int (_CMOEMAPI *POEMWRITECACHEFILE)(PCMOEM, LPBYTE, ULONG, ULONG, LPBYTE, ULONG);
typedef int (_CMOEMAPI *POEMFLUSHCACHEFILE)(PCMOEM, LPBYTE, ULONG);
typedef int (_CMOEMAPI *POEMMOVEFILE)(PCMOEM, LPBYTE, ULONG);
typedef int (_CMOEMAPI *POEMDELETEFILE)(PCMOEM, LPBYTE, ULONG);
typedef int (_CMOEMAPI *POEMCOMMITCACHEFILE)(PCMOEM, LPBYTE, ULONG, ULONG, ULONG);
typedef int (_CMOEMAPI *POEMBEGINUPDATE)(PCMOEM, ULONG, ULONG);
typedef int (_CMOEMAPI *POEMCOMPLETEUPDATE)(PCMOEM, ULONG);
typedef int (_CMOEMAPI *POEMTERMINATE)(PCMOEM, ULONG);


typedef struct _CMOEMFUNCTIONS {
    POEMINITIALIZE pOemInitialize;
    POEMQUERYCLIENTINFO pOemQueryClientInfo;
    POEMQUERYUPDATEINFO pOemQueryUpdateInfo;
    POEMSETCLIENTVERSION pOemSetClientVersion;
    POEMCREATECACHEFILE pOemCreateCacheFile;
    POEMCLOSECACHEFILE pOemCloseCacheFile;
    POEMQUERYFILE pOemQueryFile;
    POEMWRITECACHEFILE pOemWriteCacheFile;
    POEMFLUSHCACHEFILE pOemFlushCacheFile;
    POEMMOVEFILE pOemMoveFile;
    POEMDELETEFILE pOemDeleteFile;
    POEMCOMMITCACHEFILE pOemCommitCacheFile;
    POEMBEGINUPDATE pOemBeginUpdate;
    POEMCOMPLETEUPDATE pOemCompleteUpdate;
    POEMTERMINATE pOemTerminate;
} CMOEMFUNCTIONS, *PCMOEMFUNCTIONS;

typedef struct _CMOEM {
    void *pContext;                 //  OEM specific context info
    PCMOEMFUNCTIONS pFunctions;     //  OEM function pointers
} CMOEM, *PCMOEM;


/* Macros for calling OEM functions */
#define CallOemInitialize(cmoem, ver) \
    ((cmoem)->pFunctions->pOemInitialize ? \
    (cmoem)->pFunctions->pOemInitialize(cmoem, ver) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemBeginUpdate(cmoem, flags, cnt) \
    ((cmoem)->pFunctions->pOemBeginUpdate ? \
    (cmoem)->pFunctions->pOemBeginUpdate(cmoem, flags, cnt) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemCompleteUpdate(cmoem, flags) \
    ((cmoem)->pFunctions->pOemCompleteUpdate ? \
    (cmoem)->pFunctions->pOemCompleteUpdate(cmoem, flags) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemTerminate(cmoem, flags) \
    ((cmoem)->pFunctions->pOemTerminate ? \
    (cmoem)->pFunctions->pOemTerminate(cmoem, flags) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemQueryUpdateInfo(cmoem, fi) \
    ((cmoem)->pFunctions->pOemQueryUpdateInfo ? \
    (cmoem)->pFunctions->pOemQueryUpdateInfo(cmoem, fi) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemQueryClientInfo(cmoem, clientinfo) \
    ((cmoem)->pFunctions->pOemQueryClientInfo ? \
    (cmoem)->pFunctions->pOemQueryClientInfo(cmoem, clientinfo) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemQueryFile(cmoem, name, info) \
    ((cmoem)->pFunctions->pOemQueryFile ? \
    (cmoem)->pFunctions->pOemQueryFile(cmoem, name, info) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemMoveFile(cmoem, file, flags) \
    ((cmoem)->pFunctions->pOemMoveFile ? \
    (cmoem)->pFunctions->pOemMoveFile(cmoem, file, flags) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemCreateCacheFile(cmoem, file, flags, oemfl, size, pID) \
    ((cmoem)->pFunctions->pOemCreateCacheFile ? \
    (cmoem)->pFunctions->pOemCreateCacheFile(cmoem, file, flags, oemfl, size, pID) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemCloseCacheFile(cmoem, file, id, flags) \
    ((cmoem)->pFunctions->pOemCloseCacheFile ? \
    (cmoem)->pFunctions->pOemCloseCacheFile(cmoem, file, id, flags) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemCommitCacheFile(cmoem, name, ID, flags, oemfl) \
    ((cmoem)->pFunctions->pOemCommitCacheFile ? \
    (cmoem)->pFunctions->pOemCommitCacheFile(cmoem, name, ID, flags, oemfl) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemFlushCacheFile(cmoem, file, flags) \
    ((cmoem)->pFunctions->pOemFlushCacheFile ? \
    (cmoem)->pFunctions->pOemFlushCacheFile(cmoem, file, flags) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemWriteCacheFile(cmoem, name, ID, off, buf, size) \
    ((cmoem)->pFunctions->pOemWriteCacheFile ? \
    (cmoem)->pFunctions->pOemWriteCacheFile(cmoem, name, ID, off, buf, size) : VDCM_STATUS_NOT_SUPPORTED)

#define CallOemDeleteFile(cmoem, name, flag) \
    ((cmoem)->pFunctions->pOemDeleteFile ? \
    (cmoem)->pFunctions->pOemDeleteFile(cmoem, name, flag) : VDCM_STATUS_NOT_SUPPORTED)

#ifdef _CM_CLIENT_


#endif /* _CM_CLIENT_ */

#endif /* _CMOEM_H */

