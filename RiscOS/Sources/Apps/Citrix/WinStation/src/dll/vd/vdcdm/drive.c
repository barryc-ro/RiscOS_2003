/*************************************************************************
*
* drive.c
*
* This is the DOS/WIN16 drive info apis.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 01/20/94
*
* Log:
*
*
*************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"

#include "../../../inc/clib.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/vd.h"
#include "../../wd/inc/wd.h"
#include "citrix/cdmwire.h" // Wire protocol definitions

#include "vdcdm.h"
//#include "cdmserv.h"
//#include "cdmdosio.h"

/*
 * This is the maximum number of DOS drives ('A' - 'Z')
 */
#define MAX_DOS_DRIVES   26

typedef struct
{
    int flags;
    char *name;
} riscos_drive_info;

#define sharefs_ATTR_DISC                       (0x1u)
#define sharefs_ATTR_DISCP                      (0x2u)
#define sharefs_ATTR_DISC_RO                    (0x4u)
#define sharefs_ATTR_HIDDEN                     (0x8u)
#define sharefs_ATTR_SUBDIR_AUTH                (0x10u)
#define sharefs_ATTR_CDROM                      (0x20u)
#define sharefs_ATTR_CDROM_AUTH                 (0x40u)
#define sharefs_FILE_TYPE_DISCP                 0xBD9u
#define sharefs_FILE_TYPE_DISC                  0xBDAu
#define sharefs_FILE_TYPE_SUBDIR                0xFAFu
#define sharefs_FILE_TYPE_CDROM                 0xFADu
#define sharefs_FILE_TYPE_DISCR                 0xFB4u
#define sharefs_FILE_TYPE_NO_DISC               0xFB5u
#define sharefs_SHARE_DISC                      (0x0u)
#define sharefs_SHARE_DISCP                     (0x1u)
#define sharefs_SHARE_DISC_RO                   (0x2u)
#define sharefs_SHARE_HIDDEN                    (0x4u)
#define sharefs_SHARE_SUBDIR_AUTH               (0x8u)
#define sharefs_SHARE_CDROM                     (0x10u)
#define sharefs_SHARE_CDROM_AUTH                (0x20u)
#define sharefs_NO_MORE                         (-1)
#define sharefs_ENUMERATE_PROTECTED             (0x1u)
#define sharefs_ENUMERATE_READ_ONLY             (0x2u)
#define sharefs_ENUMERATE_HIDDEN                (0x4u)
#define sharefs_ENUMERATE_SUBDIR                (0x8u)
#define sharefs_ENUMERATE_CDROM                 (0x10u)
#define sharefs_ENUMERATE_AUTHENTICATED         (0x80000000u)

#define SHAREFS_DRIVE_FORMAT	"SHARE::%s.$"
#define FILECORE_DRIVE_FORMAT	"%s::%d.$"
#define CD_DRIVE_FORMAT	    	"CDFS::%d.$"
#define NFS_DRIVE_FORMAT    	"NFS::%s.$"

/*============================================================================
==      Global Data
=============================================================================*/

static riscos_drive_info drives[MAX_DOS_DRIVES];
static int DriveCount = 0;

/*============================================================================
==  External Function used
=============================================================================*/
extern void CdmDosError( int RetVal, PUSHORT pErrorClass, PUSHORT pExtError );

/*============================================================================
==  Forward References
=============================================================================*/

/*****************************************************************************
 *
 *  CdmDosVolumeInfo
 *
 *  Handle a DOS volume information request
 *
 * ENTRY:
 *
 *   DriveId
 *     Dos drive ID to query volume information on
 *
 *   Flags
 *     Flags specifying information desired. Currently undefined.
 *
 *   pVolumeSize
 *     Pointer to variable to return the volume size in.
 *
 *   pBytesFree
 *     Pointer to variable to return the number of byte free in.
 *
 *   pAllocationSize
 *     Pointer to variable to return the allocation unit (cluster) size
 *
 *   pSerialNumber
 *     Pointer to unique volume serial number to detect disk changes.
 *
 *   pFlags  (output)
 *     Pointer to place various flags such as diskette changed.
 *
 *   pResult (output)
 *     Pointer to variable to place the Result code, as defined in
 *     cdmwire.h
 *
 * EXIT:
 *  no return value, all errors returned though the pResult argument
 *
 ****************************************************************************/

void
CdmDosVolumeInfo(
    USHORT  DriveId,
    USHORT  Flags,
    PULONG  pVolumeSize,
    PULONG  pBytesFree,
    PULONG  pAllocationSize,
    PULONG  pSerialNumber,
    PUSHORT pFlags,
    PUSHORT pResult
    )
{
    int ret;
    USHORT ErrClass, ErrCode;
    struct diskfree_t t;
    ULONG BytesCluster;

    int free_space, disc_size;
    riscos_drive_info *info;

    TRACE(( TC_CDM, TT_API3, "CdmDosVolumeInfo: DriveId %d", DriveId));

    info = &drives[DriveId - 1];
    if (_swix(OS_FSControl, _INR(0,1) | _OUT(0) | _OUT(2),
    	    49, info->name,
    	    &free_space, &disc_size) != NULL)
    {
    	CdmDosError( ret, &ErrClass, &ErrCode );
    	*pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        TRACE(( TC_CDM, TT_API3, "CdmDosVolumeInfo: Error getting DiskFree %d", *pResult));
        return;
    }
    
    pVolumeSize = disc_size;
    pBytesFree = free_space;

    /* could find the allocation unit for a file by using OS_File 24 but it
     * only works on files so we'd have to find a file first!
     */
    pAllocationSize = 2048;

    *pSerialNumber = 0x00000000;
    *pFlags = info->flags & CDM_REMOVEABLE ? (USHORT)DRIVE_CHANGED : 0;

    *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
}

/*****************************************************************************
 *
 *  CdmDosGetDrives
 *
 *   Get a list of the current drives supported.
 *
 * ENTRY:
 *   pBuf (output)
 *     pointer to buffer for Client Drives structures to describe
 *     the valid drives.
 *
 *   pBufCount (input/output)
 *     Number of bytes in the buffer, also used to return buffer
 *     size required.
 *
 *   pCount (output)
 *     Number of drives available.
 *
 * EXIT:
 *   CLIENT_STATUS_SUCCESS - no error
 *
 ****************************************************************************/

/*
 * Assume that we provide in the INI file the list of available file systems.
 * Known ones will be scanned using custom code.
 * Unknown ones will be assumed to be FileCore filesystems and so the
 * X_Drives SWI will be looked up by name.
 *
 * Note that pBuffer is not word-aligned.

 * eg ADFS, SCSI, IDEFS, CDFS, RAMFS, SHAREFS, NFS

 * Known filesystems are
 
   CDFS
   SHAREFS
   NFS

 */

static int scan_filecore(riscos_drive_info *info, int count_left, const char *name)
{
    char buffer[32];
    int drives_swi;
    int i, n_floppy, n_hard;
    int name_len;

    name_len = strlen(name);

    strcpy(buffer, name);
    strcpy(buffer + name_len, "_Drives");

    if (_swix(OS_SWINumberFromString, _IN(1) | _OUT(0), buffer, &drives_swi) != NULL)
    	return 0;

    if (_swix(drives_swi, _OUTR(1,2), &n_floppy, &n_hard) != NULL)
    	return 0;

    if (n_floppy > count_left)
    	n_floppy = count_left;
    count_left -= n_floppy;

    if (n_hard > count_left)
    	n_hard = count_left;
    count_left -= n_hard;

    buffer[name_len] = 0;

    for (i = 0; i < n_floppy; i++)
    {
        if ((info->name = malloc(name_len + sizeof(FILECORE_DRIVE_FORMAT) - 3) ) == NULL)
            return -1;

    	sprintf(info->name, FILECORE_DRIVE_FORMAT, buffer, i);
        info->flags = CDM_REMOVEABLE;
    }

    for (i = 0; i < n_hard; i++)
    {
        if ((info->name = malloc(name_len + sizeof(FILECORE_DRIVE_FORMAT) - 3) ) == NULL)
            return -1;

    	sprintf(info->name, FILECORE_DRIVE_FORMAT, buffer, i + 4);
        info->flags = CDM_FIXED;
    }
   
    return n_floppy + n_hard;
}

static int scan_cdfs(riscos_drive_info *info, int count_left)
{
    int i, n;

    if (_swix(CD_GetNumberOfDrives, _OUT(0), &n) != NULL || n == 0)
    	return 0;

    if (n > count_left)
    	n = count_left;    

    for (i = 0; i < n; i++)
    {
        if ((info[i].name = malloc(sizeof(CD_DRIVE_FORMAT))) == NULL)
            return -1;

        sprintf(info[i].name, CD_DRIVE_FORMAT, i);
        info[i].flags = CDM_CDROM | CDM_REMOVEABLE;
    }
   
    return n;
}

static int scan_sharefs(riscos_drive_info *info, int count_left)
{
    int i, context;
    const char *obj_name, *dir_path;
    int attr, authentication;

    context = 0;
    i = 0;
    do
    {
    	if (_swix(ShareFS_EnumerateShares, _IN(0) | _IN(4) | _OUTR(1,5), 
    	    sharefs_ENUMERATE_PROTECTED | sharefs_ENUMERATE_READ_ONLY | sharefs_ENUMERATE_CDROM | sharefs_ENUMERATE_SUBDIR, 
    	    context,
    	    &obj_name, &dir_path, &attr, &context, &authentication) != NULL)
    	{
    	    return 0;
    	}

    	info->flags = CDM_REMOTE;
    	if (attr & (sharefs_ATTR_CDROM | sharefs_ATTR_CDROM_AUTH))
    	    info->flags = CDM_CDROM;
    	else
            info->flags = CDM_FIXED;

    	if ((info->name = malloc(sizeof(SHAREFS_DRIVE_FORMAT) - 2 + strlen(obj_name))) == NULL)
    	    return -1;

    	sprintf(info->name, SHAREFS_DRIVE_FORMAT, obj_name);

    	info++;
    	i++;
    }
    while (context != sharefs_NO_MORE && i < count_left);

    return i;
}

#if 0
static int scan_nfs(riscos_drive_info *info, int count_left)
{
    char buffer[64];
    int i, context;

    context = 0;
    i = 0;
    do
    {
    	if (_swix(NFS_MountList, _INR(0,1) | _OUT(1), 
    	    buffer, context,
    	    &context) != NULL)
    	{
    	    return 0;
    	}

    	info->flags = CDM_REMOTE | CDM_FIXED;
    	if ((info->name = malloc(sizeof(NFS_DRIVE_FORMAT) - 2 + strlen(buffer))) == NULL)
    	    return -1;

    	sprintf(info->name, NFS_DRIVE_FORMAT, buffer);

    	info++;
    	i++;
    }
    while (context != 0 && i < count_left);

    return i;
}
#endif

int
CdmDosGetDrives ( LPBYTE pBuffer, USHORT ByteCount, PUSHORT pDriveCount, const char *fs_list )
{
    int RemoteFlag;
    int ValidFlag;
    int i;
    VDCLIENTDRIVES Cd;
    char *fs, *f;

    DriveCount = 0;

    if ((fs = strupr(strdup(fs_list))) == NULL)
    	return CLIENT_ERROR_OUT_OF_MEMORY;
    
    if ((f = strtok(fs, " ,")) != NULL) do
    {
        if (strcmp(f, "CDFS") == 0)
            i = scan_cdfs(drives + DriveCount, MAX_DOS_DRIVES - DriveCount);
        else if (strcmp(f, "SHAREFS") == 0)
            i = scan_sharefs(drives + DriveCount, MAX_DOS_DRIVES - DriveCount);
        else 
            i = scan_filecore(drives + DriveCount, MAX_DOS_DRIVES - DriveCount, f);

    	if (i == -1)
    	{
    	    free(fs);
    	    return CLIENT_ERROR_OUT_OF_MEMORY;
    	}
    	
    	DriveCount += i;
    }
    while ((f = strtok(NULL, " ,")) != NULL);
    
    if (pDriveCount)
    	*pDriveCount = DriveCount;
    
    if (pBuffer)
    {
        VDCLIENTDRIVES d;
        for (i = 0; i < DriveCount; i++)
        {
             if ( ByteCount < sizeof(VDCLIENTDRIVES) )
                 return( CLIENT_ERROR_BUFFER_TOO_SMALL );

            d.DriveLetter = (UCHAR)('A'+ i);
            d.Flags = drives[i].flags;
    	    memcpy(pBuffer + i * sizeof(d), &d, sizeof(d));

    	    ByteCount -= sizeof(VDCLIENTDRIVES);
    	}
    }

    return( CLIENT_STATUS_SUCCESS );
}

