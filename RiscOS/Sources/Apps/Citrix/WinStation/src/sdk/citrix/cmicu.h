/**********************************************************************
*   CMICU.H
*      This module contains the definitions for using the Client
*      Manager ICA Client Update DLL.
*
*   Includeded by:
*       Client Manager DLL
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
*     Rev 1.9   20 Feb 1998 12:27:12   kalyanv
*  decresed the time out to 20 seconds
*  
*     Rev 1.8   16 Feb 1998 19:00:38   kalyanv
*  increased the time out value
*  
*     Rev 1.7   16 Feb 1998 16:19:28   kalyanv
*  added the Readevent description
*  
*     Rev 1.6   02 Feb 1998 13:39:46   brada
*  Add VdcmUseCacheFile
*  
*     Rev 1.5   02 Feb 1998 13:09:42   brada
*  Size file and oem flags appropriately
*  
*     Rev 1.4   27 Jan 1998 11:15:00   brada
*  Fixed up version update mode types
*
**********************************************************************/

#ifndef _CMICU_H_
#define _CMICU_H_

#define _CMAPI __cdecl

#define CM_VERSION     0x0100
#define CMICU_TIMEOUT  20000

/*
 *  CMCLIENTINFO
 */

typedef struct _CMCLIENTINFO {
    USHORT ProductID;               //  Values assigned by Citrix
    USHORT Model;                   //  Values assigned by Vendor
    ULONG  Version;                 //  xx.yy.zzzz value assigned by Vendor
} CMCLIENTINFO, *PCMCLIENTINFO;


/*
 *  CMFILEINFO
 */
typedef struct _CMFILEINFO {
    ULONG FileSize;
    USHORT FileFlags;
    USHORT OEMFlags;
    ULONG FileDate;
    ULONG FileGroup;
    ULONG FileCRC;
    ULONG CachedFileSize;
    ULONG CachedFileCRC;
} CMFILEINFO, *PCMFILEINFO;

/*
 *  FileFlags
 */

/* Flags used to describe file update */
#define CMFILE_DO_NOT_UPDATE      0x0001 /* Do not update file */
#define CMFILE_EXISTENCE_REQUIRED 0x0002 /* Client file must exist for update */
#define CMFILE_MERGE_FILE         0x0004 /* Merge new file with old file */ 
#define CMFILE_EXECUTE_FILE       0x0008 /* Execute the given file */
#define CMFILE_DELETE_FILE        0x0010 /* The file is to be removed */
#define CMFILE_UPDATE_IMMEDIATE   0x0020 /* File will be used during update */

/* Flags returned by the client */
#define CMFILE_RDONLY              0x0100

/*
 *  CMUPDATEINFO
 *
 *  See vdcm.h for update flags
 */
typedef struct _CMUPDATEINFO {
   ULONG Flags;
   ULONG FreeSpace;
   ULONG BlockSize;
   ULONG FileDirSpace;
   int   ClientCodePage;
} CMUPDATEINFO, *PCMUPDATEINFO;

/* File Groups */
#define CMFS_UNASSIGNED_GROUP     0
#define CMFS_DEFAULT_GROUP        1


/*
 *  UI choices for ICA Client Update to provide users
 */

typedef enum {
    CMICU_Prompt_User_For_Update = 1,
    CMICU_Notify_User_of_Update,
    CMICU_Update_No_Choice,
    CMICU_Max_UI_Mode
} CMICU_GUI_MODE;

/*
 * Update modes identify which clients should be updated
 */

typedef enum {
    CMICU_Update_All_Versions = 1, //  All clients including newer versions
    CMICU_Update_Earlier_Versions  //  Only older clients
} CMICU_VERSION_UPDATE_MODE;


/*
 *  Client Manager Automatic Client Update Interfaces
 */
#ifndef _CM_CLIENT

#ifdef __cplusplus
extern "C" {
#endif

#define CMICU_READ_EVENT_NAME L"CMICU_VdcmRead_Event"

/* ICA Client Virtual Driver Interfaces */
 
int _CMAPI VdcmInitialize( PULONG Version );
int _CMAPI VdcmQueryClientInfo( PCMCLIENTINFO pClientInfo, int Length );
int _CMAPI VdcmQueryUpdateInfo( PCMUPDATEINFO pClientInfo, int Length );
int _CMAPI VdcmUpdateBegin(int FileCnt, int Flags);
int _CMAPI VdcmUpdateComplete(int Flags);
int _CMAPI VdcmDisconnect(int Flags);
int _CMAPI VdcmTerminate(int Flags);

/* ICA Client Virtual Driver File Interfaces */

int _CMAPI VdcmQueryClientFileW(PWCHAR pName, PCMFILEINFO FileInfo, int Length);
#ifdef UNICODE
#define VdcmQueryClientFile VdcmQueryClientFileW
#else
#define VdcmQueryClientFile VdcmQueryClientFileA
#endif

int _CMAPI VdcmSendFileW(PWCHAR pName, PWCHAR pSrc, USHORT Flags, USHORT OEMFlags);
#ifdef UNICODE
#define VdcmSendFile VdcmSendFileW
#else
#define VdcmSendFile VdcmSendFileA
#endif

int _CMAPI VdcmUseCacheFileW(PWCHAR pName, PWCHAR pSrc, USHORT Flags, USHORT OEMFlags);
#ifdef UNICODE
#define VdcmUseCacheFile VdcmUseCacheFileW
#else
#define VdcmUseCacheFile VdcmUseCacheFileA
#endif

int _CMAPI VdcmDeleteFileW(PWCHAR pName, ULONG Flags);
#ifdef UNICODE
#define VdcmDeleteFile VdcmDeleteFileW
#else
#define VdcmDeleteFile VdcmDeleteFileA
#endif

int _CMAPI VdcmFlushFileW(PWCHAR pName);
#ifdef UNICODE
#define VdcmFlushFile VdcmFlushFileW
#else
#define VdcmFlushFile VdcmFlushFileA
#endif

int _CMAPI VdcmCloseFile(HANDLE Handle);

#ifdef __cplusplus
}
#endif

#endif /* _CM_CLIENT */
#endif /* _CMICU_H_ */


