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


/*============================================================================
==      Global Data
=============================================================================*/



/*============================================================================
==  External Function used
=============================================================================*/
extern void CdmDosError( int RetVal, PUSHORT pErrorClass, PUSHORT pExtError );
#ifdef  WIN16
extern int _dos_getmediaid( BYTE DriveNumber, PULONG pId );
#endif



/*============================================================================
==  Forward References
=============================================================================*/

#ifdef WIN16

// This is a standard MS-DOS structure.
typedef struct tagMEDIAID
{
    WORD  wInfoLevel;
    DWORD dwSerialNum;     // Serial number
    char  VolLabel[11];    // ASCII volume label
    char  FileSysType[8];  // File system type
} MEDIAID, far *LPMEDIAID;

// This is a standard DPMI structure.
typedef struct tagREALMODEREG {
    DWORD rmEDI, rmESI, rmEBP, Reserved, rmEBX, rmEDX, rmECX, rmEAX;
    WORD  rmCPUFlags, rmES, rmDS, rmFS, rmGS, rmIP, rmCS, rmSP, rmSS;
} REALMODEREG, FAR *LPREALMODEREG;

BOOL GetMediaID (WORD Drive, LPMEDIAID lpMediaID);

#endif



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

    TRACE(( TC_CDM, TT_API3, "CdmDosVolumeInfo: DriveId %d", DriveId));

    /*
     * Get the diskette change info before calling DOS so that it
     * does not "grab" the change line status
     */
    if( CdmHasDriveChanged( (UCHAR)DriveId ) ) {
        *pFlags = (USHORT)DRIVE_CHANGED;
    }
    else {
        *pFlags = 0;
    }

    ret = _dos_getdiskfree( (UCHAR)DriveId, &t );

    if( ret == 0 ) {

         /*
          *   Now get the MediaId byte if possible for the serial number
          *
          *       On a single floppy real DOS machine, it causes a
          *       prompt for the 'B:' disk that has to be responded
          *       to.
          */
#ifdef  DOS
        (void) _dos_getmediaid( (BYTE)DriveId, pSerialNumber );
#else
        {
            MEDIAID MediaID;

            //  get media id
            if ( GetMediaID( DriveId, (LPMEDIAID) &MediaID ) ) {
                *pSerialNumber = MediaID.dwSerialNum;
            }
            else {
                *pSerialNumber = 0x00000000;
            }
        }
#endif
        TRACE(( TC_CDM, TT_API3, "CdmVolInfo: SerialNumber 0x%lx ", *pSerialNumber ));

        BytesCluster = (ULONG)((ULONG)t.bytes_per_sector * (ULONG)t.sectors_per_cluster);
        *pAllocationSize = BytesCluster;
        *pBytesFree = (ULONG)(BytesCluster * (ULONG)t.avail_clusters);
        *pVolumeSize = (ULONG)((ULONG)t.total_clusters * BytesCluster);

        *pResult = CDM_MAKE_STATUS( CDM_ERROR_NONE, CDM_DOSERROR_NOERROR );
        return;
    }
    else {
        CdmDosError( ret, &ErrClass, &ErrCode );
        *pResult = CDM_MAKE_STATUS( ErrClass, ErrCode );
        TRACE(( TC_CDM, TT_API3, "CdmDosVolumeInfo: Error getting DiskFree %d", *pResult));
        return;
    }
}

/*****************************************************************************
 *
 *  CdmIsDriveRemovable
 *
 *   Returns TRUE if a drive is removable, or FALSE if
 *   it is not, or an invalid drive.
 *
 * ENTRY:
 *   DriveNumber (input)
 *     Drive number to query. 0 means default, 1 is A:, 2 is B:, etc.
 *
 * EXIT:
 *   TRUE - This is a removeable drive
 *   FALSE - This is not a removeable drive, or an invalid one
 *
 ****************************************************************************/

BOOLEAN CdmIsDriveRemovable(
    UCHAR DriveNumber
    )
{
    BOOLEAN rc = FALSE;

    _asm{
        push    ds
        mov     ax,4408h
        mov     bl,DriveNumber
        int     21h
        pop     ds
        jc      ErrorIDR

        cmp     ax,0
        jne     ErrorIDR

        mov     rc,TRUE
ErrorIDR:
    }

    return( rc );
}

/*****************************************************************************
 *
 *  CdmIsDriveRemote
 *
 *   Returns true or false to indicate whether the drive
 *   is "remote" or a network drive.
 *   If the drive letter is invalid, this function returns FALSE.
 *
 * ENTRY:
 *   DriveNumber (input)
 *     Drive number to query. 0 means default, 1 is A:, 2 is B:, etc.
 *
 * EXIT:
 *   TRUE - This is a network drive
 *   FALSE - This is not a network drive
 *
 ****************************************************************************/

BOOLEAN CdmIsDriveRemote(
    UCHAR DriveNumber
    )
{
    BOOLEAN rc = FALSE;

    _asm{
        push    ds
        mov     ax,4409h
        mov     bl,DriveNumber
        int     21h
        pop     ds
        jc      ErrorIDR2

        test    dx,1000h
        jz      ErrorIDR2

        mov     rc,TRUE
ErrorIDR2:
    }

    return( rc );
}


/*****************************************************************************
 *
 *  CdmIsDriveCdRom
 *
 *   Returns true or false to indicate whether the drive
 *   is a CD-ROM device.
 *   If the drive letter is invalid, this function returns FALSE.
 *
 * ENTRY:
 *   DriveNumber (input)
 *     Drive number to query. 0 means default, 1 is A:, 2 is B:, etc.
 *
 * EXIT:
 *   TRUE - This is a CD-ROM device
 *   FALSE - This is not a CD-ROM device
 *
 ****************************************************************************/

BOOLEAN CdmIsDriveCdRom(
    UCHAR DriveNumber
    )
{
    BOOLEAN rc = FALSE;

#define MAKEULONGL( low, high ) ((ULONG)(((USHORT)(low))|((ULONG)((USHORT)(high)))<<16))
#define MAKEFARP(selector, offset) ((PVOID)MAKEULONGL(offset, selector))

    /*
     * Finding out whether a disk is a CD-ROM can be tricky.
     *
     * They show up as "network" drives under DOS since the MSCDEX
     * DOS CD-ROM extensions install as a network redirector, as opposed
     * to a block device since CD-ROM's have a different file system layout.
     *
     * We will determine if a drive is a CD-ROM drive or not by calling the
     * CD-ROM extentions INT to see if MSCDEX is loaded and what drives are
     * CD-ROM drives.  We have to decrement the drive number by one because
     * we used the convention of A=1, B=2, etc and this function uses A=0,
     * B=1, etc.
     *
     */

    //
    // INT 2FH Function 15H, Subfunction 0BH from
    // page 19-18 of PC Interrupts.
    //


    _asm{
        push    ds
        mov     ax,150Bh
        mov     ch,00h
        mov     cl,DriveNumber         ;load and dec the drive number
        dec     cl                     ;we use A=1 and this use A=0
        int     2Fh
        jc      ErrorICD

        cmp     bx,0xadad              ;see if MSCDEX is loaded
        jne     ErrorICD

        cmp     ax,0000h               ;see if drive is a CD-ROM drive
        je      ErrorICD

        mov     rc,TRUE
ErrorICD:
        pop     ds
    }

    return( rc );
}

/*****************************************************************************
 *
 *  CdmLogicalDriveSkip
 *
 *   Returns TRUE if a drive this is a logical drive (floppy)
 *   that should be skipped.
 *
 *   On PC systems with only one floppy, DOS pretends to have an
 *   A: and B: disk by prompting the user to change the disks.
 *   We disable this 'feature' by setting the first drive A: to the
 *   physical drive, and skip over the B: drive unless there is an
 *   actual physical second drive on the system.
 *
 * ENTRY:
 *   DriveNumber (input)
 *     Drive number to query. 0 means default, 1 is A:, 2 is B:, etc.
 *
 * EXIT:
 *   TRUE - This drive should be skipped over and not returned as valid.
 *   FALSE - This drive is valid.
 *
 *   NOTE: This sets the Logical drive map for the A: drive to the
 *         physical drive if there is only 1 floppy on the system.
 *
 ****************************************************************************/

BOOLEAN CdmLogicalDriveSkip(
    UCHAR DriveNumber
    )
{
    BOOLEAN     rc = FALSE;

    // Only the first two disks (floppies) have this mapping problem
    if( (DriveNumber == 1) || (DriveNumber == 2) ) {

        _asm{
            push    ds
            mov     ax,440eh
            mov     bl,DriveNumber
            int     21h                 // ds:bx -> drive parameter block
            jc      FalseLDS

            cmp     al,0                // There are no other logical drives on this volume
            je      FalseLDS

            // There are other logical drives on this volume,
            // if its the second floppy return TRUE. If its the first
            // floppy, set it as the active one and return FALSE.

            mov     bl,DriveNumber
            cmp     bl,2
            je      TrueLDS

            mov     ax,440Fh        // Set Logical Drive Map
            int     21h
            jmp     FalseLDS

TrueLDS:
            mov     rc,TRUE
FalseLDS:
            pop     ds
        }
    }

    return( rc );
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

int
CdmDosGetDrives ( LPBYTE pBuffer, USHORT ByteCount, PUSHORT pDriveCount )
{
    int RemoteFlag;
    int ValidFlag;
    int i;
    VDCLIENTDRIVES Cd;
    int DriveCount = 0;

    /*
     * Count how many drives are valid by going through each
     * letter an issuing the is drive remote test to each.
     *
     * If a drive is valid, we do further testing to find
     * out more about it.
     *
     * If the drive is either the A: or B: (floppy) drives, we must
     * test the DOS logical drive mapping and force it to A: on single
     * floppy machines. Otherwise an uncatchable DOS prompt will be put
     * up over the clients graphics screen asking the user to insert another
     * floppy for the 'virtual volume'.
     *
     * NOTE: The more detailed tests have been defined out since
     *       they cause unacceptable delays when a drive is a network
     *       mapped drive to a server that has gone down.
     *
     */

     for( i = 1; i <= MAX_DOS_DRIVES ; i++ ) {

         memset( &Cd, 0, sizeof(VDCLIENTDRIVES) );
         ValidFlag = FALSE;

        _asm{
            push    ds
            mov     ax,4409h
            mov     bx,i
            int     21h
            jc      InValidDGD

            mov     ValidFlag,TRUE
            mov     RemoteFlag,dx
InValidDGD:
            pop     ds
        }


         if( !ValidFlag ) {
             continue; // invalid drive
         }

         // valid drive

         Cd.DriveLetter = (UCHAR)('A'+ (i - 1));
         Cd.Flags = 0;

         if( RemoteFlag & 0x1000 ) {
             Cd.Flags |= (ULONG)CDM_REMOTE;
         }

         //
         // Now check the logical drive map
         // to prevent DOS from prompting Insert drive A:
         // over our graphics screen in the client
         //

         if( CdmLogicalDriveSkip( (UCHAR)i ) ) {
             continue;  // We should skip this, no physical drive
         }

         //
         // Now get the device parameters
         //
         // If this is a network drive, we do not test to
         // see if its a removable drive since we can not reliably
         // tell. All mappings appears as fixed drives.
         //

         if( !(Cd.Flags & (ULONG)CDM_REMOTE) ) {

             if( CdmIsDriveRemovable( (UCHAR)i ) ) {
                 // removable drive
                 Cd.Flags |= (ULONG)CDM_REMOVEABLE;
             }
             else {
                 Cd.Flags |= (ULONG)CDM_FIXED;
             }
         }

         //
         // Test to see if this is a CD-ROM device
         //
         if( CdmIsDriveCdRom( (UCHAR)i ) ) {
             Cd.Flags |= (ULONG)CDM_CDROM;

             // The function 0x4408 returns fixed drive
             // on a CD-ROM, so fix this here
             Cd.Flags &= (ULONG)~CDM_FIXED;
             Cd.Flags &= (ULONG)~CDM_REMOTE;
             Cd.Flags |= (ULONG)CDM_REMOVEABLE;
         }

         /*
          *  Copy data to callers buffer
          */
         if ( pBuffer ) {

             if ( ByteCount < sizeof(VDCLIENTDRIVES) )
                 return( CLIENT_ERROR_BUFFER_TOO_SMALL );

             memcpy( pBuffer, &Cd, sizeof(VDCLIENTDRIVES) );
             pBuffer += sizeof(VDCLIENTDRIVES);
             ByteCount -= sizeof(VDCLIENTDRIVES);
         }

         /*
          *  Increment Drive Count
          */
         DriveCount++;
     }

     if ( pDriveCount )
         *pDriveCount = DriveCount;

     return( CLIENT_STATUS_SUCCESS );
}
/*****************************************************************************
 *
 *  CdmHasDriveChanged
 *
 *   Returns TRUE if a drive is has changed.
 *   This returns if int 13H AX=16H returns change line active, or
 *   drive not ready.
 *
 * ENTRY:
 *   DriveNumber (input)
 *     Drive number to query. 0 means default, 1 is A:, 2 is B:, etc.
 *
 * EXIT:
 *   TRUE - The diskette may have been changed.
 *   FALSE - No hardware indication that the diskette may have changed.
 *
 ****************************************************************************/

BOOLEAN
CdmHasDriveChanged(
    UCHAR DriveNumber
    )
{
    return( FALSE );
#ifdef  FUTURE
    BYTE    ChangeLine;

    _asm{
        push    ds
        mov     ax,1600h
        mov     dl,DriveNumber-1
        int     13h
        mov     ChangeLine,ah
        pop     ds
    }

    if( ChangeLine == 0x06 ) {
        // Diskette has changed
        TRACE(( TC_CDM, TT_API4, "CdmHasDriveChanged: Drive %d Has CHANGED!\n",DriveNumber));
        return( TRUE );
    }
    else if( ChangeLine == 0x80 ) {
        // Drive not ready, may be changed
        TRACE(( TC_CDM, TT_API4, "CdmHasDriveChanged: Drive %d NOT READY, MAYBE CHANGED!\n",DriveNumber));
        return( TRUE );
    }
    else if( ChangeLine == 0x00 ) {
        TRACE(( TC_CDM, TT_API4, "CdmHasDriveChanged: Drive %d Change Line not active 0x%x\n",DriveNumber,ChangeLine));
        return( FALSE );
    }
    else {
        TRACE(( TC_CDM, TT_API4, "CdmHasDriveChanged: Unknown return 0x%x Drive %d\n",ChangeLine,DriveNumber));
        return( FALSE );
    }
#endif
}


#ifdef WIN16
//********************************************************************
// RealInt()
//
// Simulate an interrupt in real mode using DPMI function 0300h
// When the interrupt is simulated in real mode, the registers will
// contain the values in lpRealModeReg. When the interrupt returns,
// lpRealModeReg will contain the values from real mode.
//
//********************************************************************

BOOL RealInt (BYTE intnum, LPREALMODEREG lpRealModeReg)
{
   BOOL bRetVal = TRUE;

   _asm
   {
       mov  ax, 0300h  // Simulate real mode interrupt
       mov  bl, intnum // Interrupt number to simulate
       mov  bh, 0      // Flags
       mov  cx, 0      // Number of words to copy on stack
       les  di, lpRealModeReg
       int  31h
       jnc  Done
       mov  bRetVal, FALSE
   Done:
   }
   return bRetVal;
}

//********************************************************************
// GetMediaID()
//
// Get Media ID by simulating an Interrupt 21h, AX=440Dh, CX=0866h in
// real mode. Set up RealModeReg to contain a real mode pointer to a
// MediaID structure.
//********************************************************************

BOOL GetMediaID (WORD      Drive,
                 LPMEDIAID lpMediaID)
{
   REALMODEREG RealModeReg;
   DWORD       dwGlobalDosBuffer;
   LPMEDIAID   lpRMMediaID;
   BOOL        bRetVal;

   // (1) Get a real mode addressable buffer for the MediaID structure.
   //
   dwGlobalDosBuffer = GlobalDosAlloc(sizeof(MEDIAID));
   if (dwGlobalDosBuffer == NULL)
   {
       return FALSE;
   }

   // (2) Initialize the real mode register structure.
   //
   memset(&RealModeReg, 0, sizeof(RealModeReg));
   (WORD)RealModeReg.rmEAX = 0x440D;  // IOCTL for Block Device
   (WORD)RealModeReg.rmEBX = Drive;   // 0 = default, 1 = A, 2 = B
   (WORD)RealModeReg.rmECX = 0x0866;  // Get Media ID
   // Set the real mode DS:DX to a real mode pointer to the buffer.
   // The offset in DX is zero from the memset().
   RealModeReg.rmDS  = HIWORD(dwGlobalDosBuffer);

   // (3) Simulate the real mode interrupt.
   //
   if (RealInt(0x21, &RealModeReg) &&         // Int simulation ok?
       (RealModeReg.rmCPUFlags & 0x0001)==0)  // Carry clear?
   {
       // (4) Copy the content of the real mode addressable buffer
       //     to the protected mode destination buffer.
       //
       lpRMMediaID = (LPMEDIAID) MAKELP(LOWORD(dwGlobalDosBuffer), 0);
       *lpMediaID = *lpRMMediaID;  // Structure copy
       bRetVal = TRUE;
   }
   else
   {
       bRetVal = FALSE;
   }

   // (5) Free the real mode addressable buffer.
   //
   GlobalDosFree(LOWORD(dwGlobalDosBuffer));

   return bRetVal;
}
#endif
