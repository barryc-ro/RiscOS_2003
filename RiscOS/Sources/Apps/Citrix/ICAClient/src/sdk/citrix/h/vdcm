/***************************************************************************
*   VDCM.H
*      This is the protocol header for the Client Manager Virtual Driver.
*
*   Author:   Brad Anderson
*   Date:     10/29/97
*
*   Copyright (C) Citrix Systems, Inc  1997
*
*   $Log$
*  
*     Rev 1.13   26 Apr 1998 14:20:16   brada
*  Add update in progress status value
*  
*     Rev 1.12   31 Mar 1998 14:25:26   brada
*  Remove dead code
*  
*     Rev 1.11   24 Mar 1998 11:32:10   brada
*  Add macro to declare function names in debug build
*  
*     Rev 1.10   20 Feb 1998 15:50:32   brada
*  Add support to distinguish secure clients in model number
*  
*     Rev 1.9   16 Feb 1998 16:18:16   kalyanv
*  added the time out status
*  
*     Rev 1.8   04 Feb 1998 17:19:52   brada
*  Get virtual driver name from WD
*  
*     Rev 1.7   02 Feb 1998 13:08:04   brada
*  Add end-of-file flag
*  
*      Rev 1.6   26 Jan 1998 18:26:08   brada
*   Add Multi-Language support
*
***************************************************************************/

#ifndef _VDCM_H_
#define _VDCM_H_


/* Temporary #defines */
#define TC_CM 1
#define TT_INIT  TT_API1

/* Debug Defines */
#ifdef DEBUG
#define DeclareFName(func)  const char fName[] = func;
#else
#define DeclareFName(func)
#endif

#pragma pack(1)

/*
 *  ICA 3.0 Data Structures
 */

#define VDCM_DRIVER_NAME      VIRTUAL_CM

/*
 *  Version of the Client Management Interfaces
 */

#define VDCM_MIN_VERSION    0x01
#define VDCM_MAX_VERSION    0x01
#define VDCM_VERSION        VDCM_MAX_VERSION

#ifdef _CM_CLIENT

/*
 * VDCM_C2H
 */
 
typedef struct _VDCM_C2H {
    VD_C2H Header;
    USHORT cbMaxDataSize;
    USHORT oClasses;
    BYTE   ClassCnt;
} VDCM_C2H, *PVDCM_C2H;

#endif


/*
 * ICA Client Model Numbers for Citrix WINDOS Clients (ProductID=1)
 * 
 * The low 6-bits identify the model, the high 8-bits are
 * reserved for the language.
 */

#define CLIENT_MODEL_CTX_DOS       0x0001
#define CLIENT_MODEL_CTX_WIN16     0x0002
#define CLIENT_MODEL_CTX_WIN32     0x0003
#define CLIENT_MODEL_CTX_WINCE     0x0004
#define CLIENT_MODEL_CTX_JAVA      0x0005

/*
 *  Citrix Language Macros
 *
 *  The High 8-bits of the Citrix model numbers will identify the language
 *  for the client.
 */

#define CM_LANG_MODEL_US   0x0000
#define CM_LANG_MODEL_FR   0x0100
#define CM_LANG_MODEL_GR   0x0200
#define CM_LANG_MODEL_SP   0x0300
#define CM_LANG_MODEL_JP   0x0400

/*
 *  Citrix Secure ICA Macros
 *
 *  The 7 and 8 bits of the Citrix model numbers will identify the secure
 *  client model.
 */

#define CM_SECURE_MODEL_BASE   0x0000
#define CM_SECURE_MODEL_40     0x0040
#define CM_SECURE_MODEL_56     0x0080
#define CM_SECURE_MODEL_128    0x00c0

/*
 * ICA Client Version
 */

typedef ULONG CMCLIENTVERSION;

#define VdcmSetMajor(ver, x) (ver = (((ver) & 0xffffff) | ((x) << 24)))
#define VdcmSetMinor(ver, x) (ver = (((ver) & 0xff00ffff) | ((x) & 0xff) << 16)))
#define VdcmSetBuild(ver, x) (ver = (((ver) & 0xffff0000) | ((x) & 0xffff)))
#define VdcmGetMajor(ver) ((ver) >> 24)
#define VdcmGetMinor(ver) (((ver) >> 16) & 0xff)
#define VdcmGetBuild(ver) ((ver) & 0xffff)

/*
 *  Error codes
 */

#define VDCM_STATUS_SUCCESS                      0
#define VDCM_STATUS_NOT_FOUND                    1
#define VDCM_STATUS_NO_SPACE                     2
#define VDCM_STATUS_NOT_SUPPORTED                3
#define VDCM_STATUS_CRC_MISMATCH                 4
#define VDCM_STATUS_PROTOCOL_ERROR               5
#define VDCM_STATUS_INVALID_PARAMETER            6
#define VDCM_STATUS_INVALID_VALUE                7
#define VDCM_STATUS_NO_ACCESS                    8
#define VDCM_STATUS_EXISTS                       9
#define VDCM_STATUS_FAILURE                     10
#define VDCM_STATUS_NOT_ENOUGH_MEMORY           11
#define VDCM_STATUS_UNABLE_TO_MOVE_FILES        12
#define VDCM_STATUS_TIME_OUT                    13
#define VDCM_STATUS_UPDATE_IN_PROGRESS          14

/*
 * Constants
 */
#define MAX_VDCM_BUFFER 2048


/*
 * VDCM_HEADER
 *    Basic header for all Client Manager Packets
 */

typedef struct _VDCM_HEADER {
   USHORT ByteCount;
   BYTE Class;
   BYTE Command;
   BYTE Version;
   BYTE Handle;
   USHORT Reserved;
} VDCM_HEADER, *PVDCM_HEADER;


/*
 *  Command Classes
 */
#define VDCM_CLASS_CM          0      //  ICA Client Management Base Class
#define VDCM_CLASS_ICU         1      //  ICA Client Update Class

/*************************************************************************
    Client Manager Protocol   (VDCM_CLASS_CM)
**************************************************************************/

#define CM_INIT     0        //  Special init packet for all classes

typedef struct _ICA_CM_INIT {
    VDCM_HEADER Header;
    USHORT ProtocolVersion;
} ICA_CM_INIT, *PICA_CM_INIT;


/*************************************************************************
    ICA Client Update Protocol
**************************************************************************/

/*
 * ICA Client Update Virtual Driver Packets
 */

#define CMICU_UPDATE_QUERY       0x11   // H->C Initial query of capabilities
#define CMICU_UPDATE_INFO        0x12   // C->H General capability information
#define CMICU_UPDATE_BEGIN       0x13   // H->C Initiate an update
#define CMICU_UPDATE_COMPLETE    0x14   // H->C Update completed download
#define CMICU_UPDATE_CLOSE       0x15   // H->C Host is terminating communication
#define CMICU_FILE_QUERY         0x16   // H->C Query file information
#define CMICU_FILE_UPDATE        0x17   // H->C Mark a file for update
#define CMICU_FILE_INFO          0x18   // C->H Returned file information
#define CMICU_FILE_SEND          0x19   // H->C Send file information
#define CMICU_REPLY              0x1A   // C->H Returns acknowledgements
#define CMICU_ERROR_REPLY        0x1B   // C->H Returns an error code


/*
 *  CMICU_REPLY is a packet that indicates an error occurred.
 */
typedef struct _ICA_CMICU_REPLY {
   VDCM_HEADER Header;
   LONG Value;
   USHORT Flags;
} ICA_CMICU_REPLY, *PICA_CMICU_REPLY;


/*
 *  CMICU_ERROR_REPLY is a packet that indicates an error occurred.
 */
typedef struct _ICA_CMICU_ERROR_REPLY {
   VDCM_HEADER Header;
   USHORT Flags;
   USHORT ErrorCode;
} ICA_CMICU_ERROR_REPLY, *PICA_CMICU_ERROR_REPLY;


/*
 * ICA_CMICU_UPDATE_QUERY
 *     This packet is sent by the host to query general capabilities of
 *     the client for performing automatic client updates.
 */
typedef struct _ICA_CMICU_UPDATE_QUERY {
  VDCM_HEADER Header;
} ICA_CMICU_UPDATE_QUERY, *PICA_CMICU_UPDATE_QUERY;


/*
 * CMICU_UPDATE_INFO is sent by the client to provide information
 * to the host about the clients capabilities including availabe space.
 */

typedef struct _ICA_CMICU_UPDATE_INFO {
   VDCM_HEADER Header;
   USHORT Flags;
   USHORT ClientProductID;
   USHORT ClientModel;
   USHORT MaxBufferSize;
   ULONG ClientVersion;
   ULONG AvailableSpace;                    // Note 4G max
   ULONG BlockSize;
   ULONG FileDirSpace;
   ULONG ClientCodePage;
} ICA_CMICU_UPDATE_INFO, *PICA_CMICU_UPDATE_INFO;

#define CMICU_FLAG_PROCESSING_UPDATE 0x0001  // Processing an update (cancelable)
                                             // No update allowed
#define CMICU_FLAG_UPDATES_DISABLED  0x0002  // No updates allowed
#define CMICU_FLAG_PROCESSING_COMMIT 0x0004  // Processing an update (not cancelable)
#define CMICU_FLAG_UNICODE_PROTOCOL  0x0008  // Use unicode protocol
#define CMICU_FLAG_DOUBLE_DISK_SPACE 0x0100  // Double disk space requirements 


/*
 * CMICU_UPDATE_BEGIN is sent by the host when a down level client is
 * detected and an update is requested.
 * 
 */
 
typedef struct _ICA_CMICU_UPDATE_BEGIN {
   VDCM_HEADER Header;
   USHORT Flags;
   USHORT FileCnt;                         // Number of files to be updated
} ICA_CMICU_UPDATE_BEGIN, *PICA_CMICU_UPDATE_BEGIN;


/*
 * CMICU_FILE_QUERY is sent by the Host to get the latest version
 * formation for the specified file.  The file name specified is
 * relative to a install directory for the client.  The install
 * directory may be considered the current directory when running
 * the client.  The install directory is determined by the client.
 */

typedef struct _ICA_CMICU_FILE_QUERY {
  VDCM_HEADER Header;
  USHORT oFileName;                 // Name of file
  USHORT FileNameLen;               // Length of the file name
} ICA_CMICU_FILE_QUERY, *PICA_CMICU_FILE_QUERY;

/*
 * CMICU_FILE_INFO is sent by the Client to supply the host with the 
 * version information for the requested file.
 */

typedef struct _ICA_CMICU_FILE_INFO {
   VDCM_HEADER Header;
   USHORT FileFlags;
   USHORT Reserved;
   ULONG  FileSize;
   ULONG  FileCRC;
   ULONG  CachedFileSize;
   ULONG  CachedFileCRC;
} ICA_CMICU_FILE_INFO, *PICA_CMICU_FILE_INFO;

/*
 * CMICU_FILE_UPDATE is sent by the Host to begin an update of a file.
 * The file name specified is relative to a install directory for the
 * client.  The install directory may be considered the current directory
 * when running the client.  The install directory is determined by the client.
 *
 * FileOp specifies whether the file is be create, deleted, or removed 
 * from cache.  Created file return a handle in a reply message.
 */

typedef struct _ICA_CMICU_FILE_UPDATE {
  VDCM_HEADER Header;
  USHORT FileOp;
  USHORT oFileName;                 // Name of file
  USHORT FileNameLen;               // Length of the file name
  USHORT FileFlags;
  ULONG  FileSize;                  // Expected size of the file
  USHORT OEMFlags;
} ICA_CMICU_FILE_UPDATE, *PICA_CMICU_FILE_UPDATE;

#define CMICU_FILEOP_CREATE       0x0001
#define CMICU_FILEOP_DELETE       0x0002
#define CMICU_FILEOP_FLUSH_CACHE  0x0003
#define CMICU_FILEOP_OPEN         0x0004

/*
 * CMICU_FILE_SEND is sent by the Host to supply the file data to 
 * the client.  The maximum number of bytes that can be sent per
 * packet is provided by the CMICU_UPDATE_INFO packet.
 */

typedef struct _ICA_CMICU_FILE_SEND {
   VDCM_HEADER Header;
   USHORT Flags;
   USHORT FileID;
   USHORT oFileData;
   USHORT FileDataLen;
   ULONG  FileOffset;
} ICA_CMICU_FILE_SEND, *PICA_CMICU_FILE_SEND;

/* Flags for send operations */
#define CMFILE_EOF                 0x0001


/*
 * CMICU_UPDATE_COMPLETE is sent by the host to tell the client to proceed with
 * updating the specified file ID's.  It is assumed that these files can be
 * updated without dependencies on other files.  Generally, this will be issued 
 * after all the files are downloaded.
 */

typedef struct _ICA_CMICU_UPDATE_COMPLETE {
    VDCM_HEADER Header;
    USHORT Flags;
} ICA_CMICU_UPDATE_COMPLETE, *PICA_CMICU_UPDATE_COMPLETE;

#define CMICU_UPDFL_UPDATE              0x00001
#define CMICU_UPDFL_ABORT               0x00002  // Abort the given changes
#define CMICU_UPDFL_DISCONNECT          0x00004  // Disconnecting; save state
#define CMICU_UPDFL_LASTGROUP           0x00008  // Last file group committed


#endif /* _VDCM_H_ */
