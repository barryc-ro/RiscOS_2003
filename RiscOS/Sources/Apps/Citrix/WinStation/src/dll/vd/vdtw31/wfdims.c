
/*******************************************************************************
*
*   WFDIMS.C
*
*   Thinwire Windows - BitBlt Disk Cache Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1997
*
*   Author: Kurt Perry (kurtp)
*
*   $Log$
*   Revision 1.2  1998/01/27 18:39:29  smiddle
*   Lots more work on Thinwire, resulting in being able to (just) see the
*   log on screen on the test server.
*
*   Version 0.03. Tagged as 'WinStation-0_03'
*
*   Revision 1.1  1998/01/19 19:13:03  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.10   29 Apr 1997 14:55:54   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.9   28 Apr 1997 19:01:18   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.8   28 Apr 1997 14:57:38   kurtp
*  I fixed a bug in this file, update, duh!
*  
*     Rev 1.7   15 Apr 1997 18:16:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   30 Jan 1997 13:00:58   kurtp
*  update
*  
*     Rev 1.5   29 Jan 1997 15:51:46   kurtp
*  update
*  
*     Rev 1.4   28 Jan 1997 16:05:50   kurtp
*  update
*  
*     Rev 1.3   28 Jan 1997 13:45:26   kurtp
*  update
*  
*     Rev 1.2   27 Jan 1997 17:39:40   kurtp
*  update
*  
*     Rev 1.1   27 Jan 1997 11:43:48   kurtp
*  update
*  
*     Rev 1.0   15 Jan 1997 15:38:22   kurtp
*  Initial revision.
*  
*******************************************************************************/


#include "wfglobal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/clib.h"

#include "swis.h"

#define WS	"#"
#define WS8	WS WS WS WS  WS WS WS WS

#define WIN32

/*=============================================================================
==   Defines and typedefs
=============================================================================*/

/*
 *  Max cache file info per ICA packet
 */
#define MAX_CACHE_FILE_LIST 128

/*
 *  Mininimum valid DIM file size (Header plus 2K)
 */
#define MIN_DIM_FILE_SIZE (sizeof(DIM_HEADER) + 2048)

/*
 *  iLastSection flags
 */
#define LAST_SECTION_FLAG   ((ULONG) -1)

/*
 *  DIM FileIO equates
 */
#define DIM_FILE_FOUND       0
#define DIM_FILE_NOT_FOUND  -1
#define DIM_FILE_CLOSED     -1

/*
 *  DIM priority equates
 */
#define DIMS_PRIORITY_0         0
#define DIMS_PRIORITY_1         1
#define DIMS_PRIORITY_MAX       2


/*=============================================================================
==   Function Prototypes
=============================================================================*/

INT   twDIMCacheStream( PVD, UCHAR, UCHAR, PCACHE_FILE_CONTEXT);
INT   twDIMCacheResize( PVD, ULONG );
INT   twDIMCacheDisable( PVD );

ULONG twGetDiskFreeSpace( void );
//typedef BOOL (* pfGETDISKFREESPACEEX )
//               (LPCTSTR,PULARGE_INTEGER,PULARGE_INTEGER,PULARGE_INTEGER);


/*=============================================================================
==   Local Variables
=============================================================================*/

/*
 *  Section buffer
 */
static BYTE lpbDIMBuffer[SECTION_SIZE];

/*
 *  Current disk usage
 */
static ULONG vcbDiskUsage = (ULONG) 0;

static CHAR vszFileName[260];
static DIM_HEADER gDIMHeader;
static int ghDIMHandle = DIM_FILE_CLOSED;

#define MAX_CONSECUTIVE_IO_ERRORS 10
static int vcFileIoErrors = 0;
static BOOL vfFirstDim = TRUE;


/*=============================================================================
==   Global Variables
=============================================================================*/

extern PCHAR vpszDimCachePath;
extern ULONG vDimCacheSize;
extern PVD   vpVd;


/*=============================================================================
==  Debug Stuff
=============================================================================*/

//#define REPORT_USAGE ReportUsage();
#define REPORT_USAGE {}

#if 0
void
ReportUsage()
{
    char buf[128];

    wsprintf( buf, "DiskUsage: %lu bytes of %lu total", vcbDiskUsage, vDimCacheSize );
    MessageBox( NULL, buf, "ReportUsage", MB_OK );
}
#endif

/*******************************************************************************
 *
 *  Function: ValidateDiskSpace
 *
 *  Purpose: Validate available disk space for Disk Cache
 *
 *  Entry:
 *     
 *  Exit:
 *
 ******************************************************************************/

void
ValidateDiskSpace()
{
    ULONG cbFreeSpace;

    /*
     *  Get current disk free space
     */
    cbFreeSpace = twGetDiskFreeSpace();

    /*
     *  Is there available DIM space left?
     */
    if ( vcbDiskUsage < vDimCacheSize ) {

        /*
         *  Is there enough disk space left?
         */
        if ( cbFreeSpace < (vDimCacheSize - vcbDiskUsage) ) {

            /*
             *  No, caculate total available DIM space
             */
            vDimCacheSize = vcbDiskUsage + cbFreeSpace - (cbFreeSpace / 10);

            /*
             *  Notify Host
             */
            if ( vpVd ) {
                twDIMCacheResize( vpVd, vDimCacheSize );
            }
        }
    }

    REPORT_USAGE
}


/*******************************************************************************
 *
 *  Function: OpenCacheFileHandle
 *
 *  Purpose: Open the cache file by Signature and restore the cache file info
 *           into gDIMHeader
 *
 *  Entry:
 *      sigH(input)    
 *      sigL(input)
 *      oflag(input) open file flag
 *
 *  Exit:
 *      return the cache file handle
 *      error return DIM_FILE_NOT_FOUND
 *
 ******************************************************************************/

int
OpenCacheFileHandle( ULONG sigH, ULONG sigL, int oflag )
{

#ifdef WIN32

    struct _finddata_t FileInfo;
    long hFind;
    int hFile;

    /* SJM: protect against protocol errors */
    if ( !vpszDimCachePath )
	return DIM_FILE_NOT_FOUND;

    /*
     *  Is DIM file open, if so is it the one we are looking for?
     */
    if ( ghDIMHandle != DIM_FILE_CLOSED ) {

        if ( (gDIMHeader.sigH != sigH) || (gDIMHeader.sigL != sigL) ) {
            _close( ghDIMHandle );
            ghDIMHandle = DIM_FILE_CLOSED;
        }
        else {
            TRACE((TC_TW, TT_TW_DIM, 
                   "OpenCacheFileHandle: find cache file from gDIMHandle\n"));
            return ghDIMHandle;
        }
    }

    /*
     *  Generate FAT style DIM filename, look for collisions too
     */
    wsprintf( vszFileName, "%s%08lX_" WS, vpszDimCachePath, sigH ); 

    TRACE((TC_TW, TT_TW_DIM, "OpenCacheFileHandle: filename %s\n",vszFileName));

    /*
     *  DIM file not already open, look to see if it exists
     */
    if( (hFind = _findfirst(vszFileName, &FileInfo)) == -1L ){
        TRACE((TC_TW, TT_TW_DIM, 
               "OpenCacheFileName: file can not be found, filename %s\n",
               vszFileName));
        return DIM_FILE_NOT_FOUND;
    };

    /*
     *  Check all <sn:hi>.DM? files, check for exact match in header
     */
    do {

        /*
         *  Invalid file size, minimum is header plus 2K
         */
        if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
            continue;       
        }

        /*
         *  Read in header
         */
        wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name ); 
        if ( (hFile= _open( vszFileName, oflag )) != -1 ) {

            TRACE((TC_TW, TT_TW_DIM, 
                   "OpenCacheFileHandle: find file name %s\n", 
                   vszFileName));

            memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );
            _lseek( hFile, 0, SEEK_SET  );
            _read( hFile, &gDIMHeader, sizeof(DIM_HEADER) );

            /*
             *  Check signature level
             */
            if ( gDIMHeader.sigLevel == DIM_SIGNATURE_LEVEL ) {
    
                /*
                 *  Exact match
                 */
                if ( (gDIMHeader.flag == DIM_FILE_FLAG) && 
                     (gDIMHeader.sigH == sigH) && 
                     (gDIMHeader.sigL == sigL) ) {
    
                    ghDIMHandle=hFile;
                    _findclose(hFind);
    
                    TRACE((TC_TW, TT_TW_DIM, 
                           "OpenCacheFileHandle: return the cache file sigH[%X] sigL[%X] Handle[%d] \n", 
                           gDIMHeader.sigH, gDIMHeader.sigL, hFile));
    
                    return hFile;
                }
            }

            /*
             *  No match
             */
            _close(hFile);
        }

    } while (_findnext(hFind, &FileInfo)==0);

    /*
     *  Not found, close and return
     */
    _findclose(hFind);

    return DIM_FILE_NOT_FOUND;


#else    // WIN16 and DOS

    int  hFile;
    struct find_t FileInfo;
   
    /*
     *  Is DIM file open, if so is it the one we are looking for?
     */
    if ( ghDIMHandle != DIM_FILE_CLOSED ) {

        if ( (gDIMHeader.sigH != sigH) || (gDIMHeader.sigL != sigL) ) {

            _close( ghDIMHandle );
            ghDIMHandle = DIM_FILE_CLOSED;
        }
        else {

            TRACE((TC_TW, TT_TW_DIM, 
                   "OpenCacheFileHandle: find cache file from gDIMHANDLE\n"));

            return ghDIMHandle;    // found the handle
        }
    }

    /*
     *  Generate FAT style DIM filename, look for collisions too
     */
    wsprintf( vszFileName, "%s%04X%04X_" WS, vpszDimCachePath, 
              (USHORT)(sigH>>16), (USHORT)(sigH&0xffff) ); 

    TRACE((TC_TW, TT_TW_DIM, "OpenCacheFileHandle: filename %s\n",vszFileName));
    
    /*
     *  DIM file not already open, look to see if it exists
     */
    if( _dos_findfirst(vszFileName, _A_NORMAL, &FileInfo) != 0 ) {
        return DIM_FILE_NOT_FOUND;
    };
   
    /*
     *  Check all <sn:hi>.DM? files, check for exact match in header
     */
    do {

        /*
         *  Invalid file size, minimum is header plus 2K
         */
        if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
            continue;       
        }

        /*
         *  Read in header
         */
        wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name ); 
        if ( (hFile= _open( vszFileName, oflag )) != -1 ) {

            memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );            
            _lseek( hFile, 0, SEEK_SET  );
            _read( hFile, &gDIMHeader, sizeof(DIM_HEADER) );    

            TRACE((TC_TW, TT_TW_DIM, 
                   "OpenCacheFileHandle: find signature SigH[%04X%04X] sigL [%04X%04X]", 
                   (USHORT)(gDIMHeader.sigH>>16), (USHORT)(gDIMHeader.sigH & 0xffff), 
                   (USHORT)(gDIMHeader.sigL>>16), (USHORT) (gDIMHeader.sigL & 0xffff)));
                          
            /*
             *  Check signature level
             */
            if ( gDIMHeader.sigLevel == DIM_SIGNATURE_LEVEL ) {
    
                /*
                 *  Exact match?
                 */
                if ( (gDIMHeader.flag == DIM_FILE_FLAG) && 
                     (gDIMHeader.sigH == sigH) && 
                     (gDIMHeader.sigL == sigL) ) {
    
                    ghDIMHandle=hFile;
                    return hFile;
                }
            }

            /*
             *  No match
             */
            _close(hFile);
        }

    } while ( _dos_findnext(&FileInfo)==0);
   
    /*
     *  DIM not found
     */
    return DIM_FILE_NOT_FOUND;

#endif

}


/*******************************************************************************
 *
 *  Function: FindCacheFileName
 *
 *  Purpose:
 *
 *  Entry:
 *      sigH (input)
 *      sigL (input)
 * 
 *  Exit:
 *      0 find the file
 *      DIM_FILE_NOT_FOUND cannot find the file
 *
 ******************************************************************************/

int
FindCacheFileName( ULONG sigH, ULONG sigL, PULONG pcbFileSize ) 
{

#ifdef WIN32

    struct _finddata_t FileInfo;
    DIM_HEADER  DIMHeader;
    long hFind;
    int hFile;
    
    /*
     *  Initialize 
     */
    *pcbFileSize = (ULONG) 0;

    /* SJM: protect against protocol errors */
    if ( !vpszDimCachePath )
	return DIM_FILE_NOT_FOUND;

    /*
     *  Generate FAT filename
     */
    wsprintf( vszFileName, "%s%08lX_" WS, vpszDimCachePath, sigH ); 
    TRACE((TC_TW, TT_TW_DIM, "FindCacheFileName: filename %s\n",vszFileName));
    
    /*
     *  Find first match
     */
    if( (hFind = _findfirst(vszFileName, &FileInfo)) == -1L ) {
       return DIM_FILE_NOT_FOUND;
    };
   
    /*
     *  Search collision list
     */
    do {

        /*
         *  Invalid file size, minimum is header plus 2K
         */
        if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
            continue;       
        }

        /*
         *  Read in header
         */
        wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name ); 
        if ( (hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {

            TRACE((TC_TW, TT_TW_DIM, 
                   "FindCacheFileName: matched filename %s\n",vszFileName));

            memset( &DIMHeader, 0, sizeof(DIM_HEADER) );  
            _lseek( hFile, 0, SEEK_SET  );
            _read( hFile, &DIMHeader, sizeof(DIM_HEADER) );

            TRACE((TC_TW, TT_TW_DIM, 
                   "FindCacheFileName: find signature[ %X, %X]\n", 
                   DIMHeader.sigH, DIMHeader.sigL));

            /*
             *  Check signature level
             */
            if ( DIMHeader.sigLevel == DIM_SIGNATURE_LEVEL ) {
    
                /*
                 *  Exact match
                 */
                if ( (DIMHeader.flag == DIM_FILE_FLAG) && 
                     (DIMHeader.sigH == sigH) && 
                     (DIMHeader.sigL == sigL)) {             
    
                    /*
                     *  Done with handles
                     */
                    _close (hFile);
                    _findclose(hFind);
    
                    /*
                     *  Return file size
                     */
                    *pcbFileSize = FileInfo.size;
    
                    return DIM_FILE_FOUND;
                }
            }

            _close(hFile);
        }

    } while (_findnext(hFind, &FileInfo)==0);

    /*
     *  Done 
     */
    _findclose(hFind);

    return DIM_FILE_NOT_FOUND;

#else    // for WIN16/DOS

    int  hFile;
    struct find_t FileInfo;
    DIM_HEADER  DIMHeader;
    
    /*
     *  Initialize 
     */
    *pcbFileSize = (ULONG) 0;

    /*
     *  Generate FAT filename
     */
    wsprintf( vszFileName, "%s%04X%04X_" WS, vpszDimCachePath, 
              (USHORT)(sigH>>16), (USHORT)(sigH&0xffff) ); 
    TRACE((TC_TW, TT_TW_DIM, "FindCacheFileName: filename %s\n",vszFileName));
    
    /*
     *  Find first match
     */
    if( _dos_findfirst(vszFileName,_A_NORMAL, &FileInfo) != 0 ) {
       return DIM_FILE_NOT_FOUND;
    };
   
    /*
     *  Search collision list
     */
    do {

        /*
         *  Invalid file size, minimum is header plus 2K
         */
        if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
            continue;       
        }

        /*
         *  Read in header
         */
       wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name); 
       if ( ( hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {

           memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );
           _lseek( hFile, 0, SEEK_SET  );
           _read( hFile, &DIMHeader, sizeof(DIM_HEADER) );

            TRACE((TC_TW, TT_TW_DIM, 
                   "FindCacheFileName: find signature[ %X, %X]\n", 
                   DIMHeader.sigH, DIMHeader.sigL));

            /*
             *  Check signature level
             */
            if ( DIMHeader.sigLevel == DIM_SIGNATURE_LEVEL ) {
    
                /*
                 *  Exact match
                 */
                if ( (DIMHeader.flag == DIM_FILE_FLAG) && 
                     (DIMHeader.sigH == sigH) && 
                     (DIMHeader.sigL == sigL) ) {
    
                    /* 
                     *  Done with file
                     */
                    _close(hFile);
    
                    /*
                     *  Return file size
                     */
                    *pcbFileSize = FileInfo.size;
    
                    return DIM_FILE_FOUND;
                }
            }

            _close(hFile);
       }

    } while ( _dos_findnext(&FileInfo)==0 );
   
    return DIM_FILE_NOT_FOUND;

#endif

}


/*******************************************************************************
 *
 *  Function: CreateCacheFileHandle
 *
 *  Purpose: open the cache file by Signature and restore the cache file info
 *           into gDIMHeader
 *
 *  Entry:
 *      sigH(input)    
 *      sigL(input)
 *      
 *
 *  Exit:
 *      return the cache file handle
 *      error return DIM_FILE_NOT_FOUND
 *
 ******************************************************************************/

int
CreateCacheFileHandle( ULONG sigH, ULONG sigL )
{
    int hFile;
    CHAR namelist[26];     // store list of all used file name
    char count = 0;
    BOOL bFound = FALSE;
    BOOL bUniq  = TRUE;
    char cBase;
    int i;

#ifdef WIN32

    struct _finddata_t FileInfo;
    long hFind;

    /* SJM: protect against protocol errors */
    if ( !vpszDimCachePath )
	return DIM_FILE_NOT_FOUND;

    /*
     *  Find first match
     */
    wsprintf( vszFileName, "%s%08lX_" WS, vpszDimCachePath, sigH); 
    if( (hFind = _findfirst(vszFileName, &FileInfo)) != -1L ){        

        /*
         * Not unique
         */
        bUniq = FALSE;

        /*
         *  Walk collision list
         */
        do {

            /*
             *  Validate file length
             */
            if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
                namelist[count++] = FileInfo.name[11];  // do not use this file name
                continue;       
            }

            /*
             *  Read in header info
             */
            wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name); 
            if ( ( hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {
       
                // read signature header
                memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );
                _lseek( hFile, 0, SEEK_SET  );
                _read( hFile, &gDIMHeader, sizeof(DIM_HEADER) );
     
                /*
                 *  Not exact match, create collision list
                 */
                if ( (gDIMHeader.flag != DIM_FILE_FLAG) || 
                     (gDIMHeader.sigLevel != DIM_SIGNATURE_LEVEL) ||
                     (gDIMHeader.sigH != sigH) || 
                     (gDIMHeader.sigL != sigL) ) {
                    // find comflict file name, different cache file
                    namelist[count++] = vszFileName[strlen(vszFileName)-1];
                }
                else {         

                    /*
                     *  Exact match, find original cache file; rewrite it
                     */
                    bFound = TRUE;
                    wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name); 
                    _findclose(hFind);
                    _close(hFile);

                    /*
                     *  Remove current file usage from total
                     */
                    vcbDiskUsage -= FileInfo.size;

                    break;
                }

                _close(hFile);
            } 
            else {   // file can not open; do not use this file name
                namelist[count++] = vszFileName[strlen(vszFileName)-1];
            }
        } while ( _findnext(hFind, &FileInfo) == 0 );   

        _findclose(hFind);
    }

#else   // for win16

    struct find_t FileInfo;
   
    /*
     *  Find first match
     */
    wsprintf( vszFileName, "%s%04X%04X_" WS, vpszDimCachePath,
              (USHORT)(sigH>>16), (USHORT)(sigH&0xffff) ); 
    if( _dos_findfirst(vszFileName,_A_NORMAL, &FileInfo) == 0 ) {

        /*
         * Not unique
         */
        bUniq = FALSE;

        /*
         *  Walk collision list
         */
        do {

            /*
             *  Validate file length
             */
            if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
                namelist[count++] = FileInfo.name[11];  // do not use this file name
                continue;       
            }

            /*
             *  Read in header info
             */
            wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name ); 
            if ( ( hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {
       
                // read signature header
                memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );
                _lseek( hFile, 0, SEEK_SET  );
                _read( hFile, &gDIMHeader, sizeof(DIM_HEADER) );
          
                /*
                 *  Not exact match, create collision list
                 */
                if ( (gDIMHeader.flag != DIM_FILE_FLAG) || 
                     (gDIMHeader.sigLevel != DIM_SIGNATURE_LEVEL) ||
                     (gDIMHeader.sigH != sigH) || 
                     (gDIMHeader.sigL != sigL) ) {
                    // find comflict file name, different cache file
                    namelist[count++] = vszFileName[strlen(vszFileName)-1];
                }
                else {         

                    /*
                     *  Find original cache file; rewrite it
                     */
                    bFound = TRUE;
                    wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name ); 
                    _close(hFile);

                    /*
                     *  Remove current file usage from total
                     */
                    vcbDiskUsage -= FileInfo.size;

                    break;
                }

                _close(hFile);
            } 
            else {   // file can not open; do not use this file name
                namelist[count++] = vszFileName[strlen(vszFileName)-1];
            }

        } while ( _dos_findnext(&FileInfo) == 0 );      

    }

#endif

    /*
     *  Form new cache file name
     */
    if (!bFound) { 

        /*
         *  Try to prioritize and identify first dim
         */
        if ( (vfFirstDim == TRUE) && (bUniq == TRUE) ) {
            cBase = '0';
            vfFirstDim = FALSE;
        }
        else {
    
            cBase = 'A';
            if (count > 0 )  {  
                // find next available file name    
                for ( i = 0; i < count; i++) {
                    if (namelist[i] == cBase) cBase++;       
                }
            }
        }

        vszFileName[strlen(vszFileName)-1] = cBase;
        vszFileName[strlen(vszFileName)] = '\0';
    }

    /*
     *  Create new cache file
     */
    if ( (hFile = _open( vszFileName, 
                         O_CREAT|O_TRUNC|O_RDWR|O_BINARY/*,
                         S_IREAD|S_IWRITE*/ )) == -1 ) {

        //  cannot create new file
        TRACE((TC_TW, TT_TW_DIM, 
               "CreateCacheFileHandle: create new file fails, name [%s]",
               vszFileName));

        //  count io errors, if too many then punt
        if ( ++vcFileIoErrors > MAX_CONSECUTIVE_IO_ERRORS ) {
            if ( vpVd ) {
                (void) twDIMCacheDisable( vpVd );
            }
        }

        return DIM_FILE_NOT_FOUND;
    }

    //  reset file io error count
    vcFileIoErrors = 0;

    /*
     *  Write file signature header
     */
    memset( &gDIMHeader, 0, sizeof(DIM_HEADER) );
    gDIMHeader.cbHeader     = sizeof(DIM_HEADER);
    gDIMHeader.sigLevel     = DIM_SIGNATURE_LEVEL;
    gDIMHeader.flag         = DIM_FILE_FLAG;
    gDIMHeader.sigH         = sigH;
    gDIMHeader.sigL         = sigL;
    gDIMHeader.iLastSection = LAST_SECTION_FLAG;
    ghDIMHandle             = hFile ;
    _lseek( hFile, 0, SEEK_SET  );
    _write( hFile, &gDIMHeader, sizeof(DIM_HEADER) );

    /*
     *  Validate available disk space 
     */
    ValidateDiskSpace();

    return hFile;
}


/*******************************************************************************
 *
 *  Function: lpTWDIMCacheRead
 *
 *  Purpose:  read one section from cache file 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     return pointer to read buffer
 *     NULL for read error
 *
 ******************************************************************************/

LPBYTE
lpTWDIMCacheRead( ULONG sigH, ULONG sigL, LPUINT pbytecount, UINT section )
{
    int hFile;
 
    /*
     *  Try and get handle to cache file
     */
    if ( (hFile = OpenCacheFileHandle(sigH, sigL, O_RDONLY|O_BINARY)) == DIM_FILE_NOT_FOUND ) {

       TRACE((TC_TW, TT_TW_DIM, "lpTWDIMCacheRead: Can not open cache file SigH[%04X%04X] sigL [%04X%04X]", 
              (USHORT)(sigH>>16), (USHORT)(sigH & 0xffff), 
              (USHORT)(sigL>>16), (USHORT)(sigL & 0xffff)));       
                   
        *pbytecount = 0;

        //  count io errors, if too many then punt
        if ( ++vcFileIoErrors > MAX_CONSECUTIVE_IO_ERRORS ) {
            if ( vpVd ) {
                (void) twDIMCacheDisable( vpVd );
            }
        }

        return (NULL);
    }

    //  reset file io error count
    vcFileIoErrors = 0;

    //  clear first dim flag
    vfFirstDim = FALSE;

    /*
     *  Read in requested section
     */
    _lseek( hFile, 
            (gDIMHeader.cbHeader + ((ULONG)section * (ULONG)SECTION_SIZE)), 
            SEEK_SET );
    _read( hFile, lpbDIMBuffer, SECTION_SIZE );

    /*
     *  Return zero on all sections except the last one
     */
    if ( section == (UINT) gDIMHeader.iLastSection ) {
        *pbytecount = (UINT) gDIMHeader.cbLastSection;
    }
    else {
        *pbytecount = 0;
    }

    return( (LPBYTE) lpbDIMBuffer );
}



/*******************************************************************************
 *
 *  Function: finishedTWDIMCacheRead
 *
 *  Purpose:  close cache file
 *
 *  Entry:
 *     
 *
 *  Exit:
 *
 ******************************************************************************/

VOID   
finishedTWDIMCacheRead()
{

    /*
     *  Complete file write
     */
    if ( ghDIMHandle != DIM_FILE_CLOSED ) {    

        _close( ghDIMHandle );
        ghDIMHandle = DIM_FILE_CLOSED;
    }
}


/*******************************************************************************
 *
 *  Function: TWDIMCacheInit
 *
 *  Purpose:  
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     return pointer to write buffer
 *            NULL for write error
 *
 ******************************************************************************/

BOOL
TWDIMCacheInit( PVD pvd, BOOL fContinue ) 
{

#ifdef WIN32

    struct _finddata_t FileInfo;
    DIM_HEADER DIMHeader;
    int hFile;
    INT rc = CLIENT_STATUS_SUCCESS;
    UCHAR count=0;

static long hFind;
static PCACHE_FILE_CONTEXT pcf;
static int iPriorityDims = DIMS_PRIORITY_0;
static BOOL fPriorityDims = TRUE;

    /*
     *  Continuation?
     */
    if ( fContinue ) {
        goto w32_keep_going;
    }
     
    /*
     *  Create and initialize cache file context records
     */
    if ( !(pcf = (PCACHE_FILE_CONTEXT) malloc(MAX_CACHE_FILE_LIST * sizeof_CACHE_FILE_CONTEXT)) ) {                   
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        return(FALSE);
    }
    else {
        memset( &DIMHeader, 0, sizeof(DIM_HEADER) );  
    }
    
    /*
     *  Initialize usage
     */
    vcbDiskUsage = (ULONG) 0;

    /*
     *  Priority Dims followed by rest of Dims
     */
    for ( /* static assignment above */;
          iPriorityDims < DIMS_PRIORITY_MAX; 
          iPriorityDims++ ) {
    
        /*
         *  Stream the DIM handles to host
         */
        if ( (iPriorityDims == DIMS_PRIORITY_0) ) {
            wsprintf( vszFileName, "%s" WS8 "_0", vpszDimCachePath ); 
        }
        else {
            wsprintf( vszFileName, "%s" WS8 "_" WS, vpszDimCachePath ); 
        }
        TRACE((TC_TW, TT_TW_DIM, "TWDIMCacheInit: filename %s\n",vszFileName));
    
        if( (hFind = _findfirst(vszFileName, &FileInfo)) != -1L ) {
           
            /*
             *  Walk file list
             */
            do {
    
                /*
                 *  Validate file length
                 */
                if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
                    continue;       
                }

                /*
                 *  Skip priority 0 dims when processing priority 1,
                 *  they have already been sent.
                 */
                if ( (iPriorityDims == DIMS_PRIORITY_1) &&
                     (FileInfo.name[strlen(FileInfo.name)-1] == '0') ) {
                    continue;
                }
                 
                /*
                 *  Read in header
                 */
                wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name); 
                if ( ( hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {
    
                    TRACE((TC_TW, TT_TW_DIM, 
                           "TWDIMCacheInit: matched filename %s\n",
                           vszFileName));
    
                    //  read header from file
                    memset( &DIMHeader, 0, sizeof(DIM_HEADER) );  
                    _lseek( hFile, 0, SEEK_SET  );
                    _read( hFile, &DIMHeader, sizeof(DIM_HEADER) );
                
                    TRACE((TC_TW, TT_TW_DIM, 
                           "TWDIMCacheInit: find signature[ %X, %X]\n", 
                           DIMHeader.sigH, DIMHeader.sigL));
    
                    /*
                     *  Validate DIM file and version
                     */
                    if ( (gDIMHeader.flag     == DIM_FILE_FLAG) || 
                         (gDIMHeader.sigLevel == DIM_SIGNATURE_LEVEL) ) {
    
			CACHE_FILE_CONTEXT tmp_pcf;
                        /*
                         *  Add to context records bound for host
			 *  SJM: Go via temporay structure due to alignment problems
                         */
                        tmp_pcf.Size = FileInfo.size ;
                        tmp_pcf.Flags = FileInfo.attrib & _A_RDONLY;      
                        tmp_pcf.SignatureLevel = gDIMHeader.sigLevel;
                        *((PULONG)&(tmp_pcf.Filehandle[0])) =  DIMHeader.sigH;
                        *((PULONG)&(tmp_pcf.Filehandle[4])) =  DIMHeader.sigL;
			memcpy(pcf + count * sizeof_CACHE_FILE_CONTEXT, &tmp_pcf, sizeof_CACHE_FILE_CONTEXT);
        
                        /*
                         *  Update disk usage
                         */
                        vcbDiskUsage += FileInfo.size;
        
                        /*
                         *  ICA packet full, ship it off to the host
                         */
                        if ( ++count == MAX_CACHE_FILE_LIST ) {
        
                            TRACE((TC_TW, TT_TW_DIM, 
                                   "TWDIMCacheInit: send out DIM Cache file list, count [%i]\n",
                                   count));
        
                            twDIMCacheStream( pvd, count, DIMHeader.sigLevel, pcf );    
                            _close(hFile);
    
                            /*
                             *  Set up continuation, more files
                             */
                            return(TRUE);
                        }       
                    }
    
                    _close(hFile);
                }
    
w32_keep_going:     /* null statement */;
    
            } while ( _findnext(hFind, &FileInfo) == 0 );
        
            /*
             *  Ship out remainder
             */
            if ( count ) {                                       
                TRACE((TC_TW, TT_TW_DIM, 
                       "TWDIMCacheInit: send out DIM Cache file list, count [%i]\n",
                        count));
                twDIMCacheStream( pvd, count, DIMHeader.sigLevel, pcf );    
            }

            /*
             *  Ship out priority 0 dims first
             */
            if ( fPriorityDims ) {
                fPriorityDims = FALSE;
                return(TRUE);
            }

            /*
             *  Done with find first handle
             */
            _findclose(hFind);
        }
    }

    free(pcf);

    REPORT_USAGE

    /*
     *  No more files
     */
    return(FALSE);

#else   // for win16

    DIM_HEADER  DIMHeader;   
    int hFile;
    INT rc = CLIENT_STATUS_SUCCESS;
    UCHAR count=0;

static struct find_t FileInfo;   
static PCACHE_FILE_CONTEXT pcf;
static int iPriorityDims = DIMS_PRIORITY_0;
static BOOL fPriorityDims = TRUE;
   
    /*
     *  Continuation?
     */
    if ( fContinue ) {
        goto w16_keep_going;
    }
     
    /*
     *  Create and initialize cache file context records
     */
    if ( !(pcf = (PCACHE_FILE_CONTEXT) malloc(MAX_CACHE_FILE_LIST * sizeof(CACHE_FILE_CONTEXT))) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        return(FALSE);
    }
    else {
        memset( &DIMHeader, 0, sizeof(DIM_HEADER) );  
    }
    
    /*
     *  Priority Dims followed by rest of Dims
     */
    for ( /* static assignment above */;
          iPriorityDims < DIMS_PRIORITY_MAX; 
          iPriorityDims++ ) {
    
        /*
         *  Stream the DIM handles to host
         */
        if ( (iPriorityDims == DIMS_PRIORITY_0) ) {
            wsprintf( vszFileName, "%s" WS8 "_0", vpszDimCachePath ); 
        }
        else {
            wsprintf( vszFileName, "%s" WS8 "_" WS, vpszDimCachePath ); 
        }
        TRACE((TC_TW, TT_TW_DIM, "TWDIMCacheInit: filename %s\n",vszFileName));

        if( _dos_findfirst(vszFileName,_A_NORMAL, &FileInfo) == 0 ) {    
           
            /*
             *  Walk file list
             */
            do {
        
                /*
                 *  Validate file length
                 */
                if ( FileInfo.size < MIN_DIM_FILE_SIZE ) {
                    continue;       
                }
                 
                /*
                 *  Skip priority 0 dims when processing priority 1,
                 *  they have already been sent.
                 */
                if ( (iPriorityDims == DIMS_PRIORITY_1) &&
                     (FileInfo.name[strlen(FileInfo.name)-1] == '0') ) {
                    continue;
                }
	       
                /*
                 *  Read in header
                 */
                wsprintf( vszFileName, "%s%s", vpszDimCachePath, FileInfo.name); 
                if ( ( hFile= _open( vszFileName, O_RDONLY|O_BINARY)) != -1 ) {       
    
                    TRACE(( TC_TW, TT_TW_DIM, "TWDIMCacheInit: matched filename %s\n",vszFileName));
    
                    memset( &DIMHeader, 0, sizeof(DIM_HEADER) );  
                    _lseek( hFile, 0, SEEK_SET  );
                    _read( hFile, &DIMHeader, sizeof(DIM_HEADER) );
                 
                    TRACE((TC_TW, TT_TW_DIM, 
                           "TWDIMCacheInit: find signature [%04X%04X , %04X%04X]\n", 
                           (USHORT)(DIMHeader.sigH>>16), (USHORT)(DIMHeader.sigH & 0xffff), 
                           (USHORT)(DIMHeader.sigL>>16), (USHORT) (DIMHeader.sigL & 0xffff)));       
            
                    /*
                     *  Validate DIM file and version
                     */
                    if ( (gDIMHeader.flag     == DIM_FILE_FLAG) || 
                         (gDIMHeader.sigLevel == DIM_SIGNATURE_LEVEL) ) {
    
                        /*
                         *  add to context records bound for host
                         */
                        pcf[count].Size = FileInfo.size ;
                        pcf[count].Flags = FileInfo.attrib & _A_RDONLY;      
                        (ULONG) *((PULONG)&(pcf[count].Filehandle[0])) =  DIMHeader.sigH;
                        (ULONG) *((PULONG)&(pcf[count].Filehandle[4])) =  DIMHeader.sigL;
                        if ( ++count == MAX_CACHE_FILE_LIST ) {
        
                            TRACE((TC_TW, TT_TW_DIM, 
                                   "TWDIMCacheInit: send out DIM Cache file list, count [%i]\n",
                                   count));
        
                            twDIMCacheStream( pvd, count, DIMHeader.sigLevel, pcf );    
                            _close(hFile);
    
                            /*
                             *  Set up continuation, more files
                             */
                            return(TRUE);
                        }       
                    }
    
                    _close(hFile);
                }
           
w16_keep_going:     /* null statement */;
    
            } while ( _dos_findnext(&FileInfo) == 0 );
        
            /*
             *  Ship out remainder
             */
            if (count) {                                       
                TRACE((TC_TW, TT_TW_DIM, 
                       "TWDIMCacheInit: send out DIM Cache file list, count [%i]\n",
                       count));
                twDIMCacheStream( pvd, count, DIMHeader.sigLevel, pcf );    
            }

            /*
             *  Ship out priority 0 dims first
             */
            if ( fPriorityDims ) {
                fPriorityDims = FALSE;
                return(TRUE);
            }
        }
    }
       
    free(pcf);

    REPORT_USAGE

    /*
     *  No more files
     */
    return(FALSE);

#endif

}


/*******************************************************************************
 *
 *  Function: lpTWDIMCacheWrite
 *
 *  Purpose:  write one section to cache file 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     return pointer to write buffer
 *            NULL for write error
 *
 ******************************************************************************/

LPBYTE 
lpTWDIMCacheWrite( ULONG sigH, ULONG sigL, UINT section )
{

    int hFile;  
 
    TRACE((TC_TW, TT_TW_DIM, "lpTWDIMCacheWrite: section [%i]",  section));      

    /*
     *  Get handle to cache file HI:LO.DM?
     */
    if ( (hFile = OpenCacheFileHandle(sigH, sigL, O_RDWR|O_BINARY)) == DIM_FILE_NOT_FOUND ) {       
 
        TRACE((TC_TW, TT_TW_DIM, 
               "lpTWDIMCacheWrite: Can not find cache file SigH[%04X%04X] sigL [%04X%04X]", 
               (USHORT)(sigH>>16), (USHORT)( sigH & 0xffff), 
               (USHORT)(sigL>>16), (USHORT) ( sigL & 0xffff)));       

        if ( (hFile = CreateCacheFileHandle(sigH, sigL)) == DIM_FILE_NOT_FOUND ) {       

            TRACE((TC_TW, TT_TW_DIM, 
                   "lpTWDIMCacheWrite:  Can not Create new cache file SigH[%04X%04X] sigL [%04X%04X]", 
                   (USHORT)(sigH>>16), (USHORT)( sigH & 0xffff), 
                   (USHORT)(sigL>>16), (USHORT) ( sigL & 0xffff)));

            return( (LPBYTE) lpbDIMBuffer );
        }

        TRACE((TC_TW, TT_TW_DIM, 
               "lpTWDIMCacheWrite: Create new cache file SigH[%04X%04X] sigL [%04X%04X]", 
               (USHORT)(sigH>>16), (USHORT)( sigH & 0xffff), 
               (USHORT)(sigL>>16), (USHORT) ( sigL & 0xffff)));
    }
                                                 
    /*
     *  Flush last section
     */
    if ( (gDIMHeader.iLastSection != LAST_SECTION_FLAG) && 
         (gDIMHeader.iLastSection != (ULONG) section) ) {

        _lseek( hFile, 
                (gDIMHeader.cbHeader + ((ULONG)gDIMHeader.iLastSection * (ULONG)SECTION_SIZE)), 
                SEEK_SET );
        _write( hFile, lpbDIMBuffer, SECTION_SIZE );

        TRACE((TC_TW, TT_TW_DIM, 
               "lpTWDIMCacheWrite: flush section %i",
               section));
    }

    /*
     *  Save current section index, may be last
     */
    gDIMHeader.iLastSection = (ULONG)section;  

    return( (LPBYTE) lpbDIMBuffer );
}



/*******************************************************************************
 *
 *  Function: finishedTWDIMCacheWrite
 *
 *  Purpose:  DIM write complete routine
 *
 *  Entry:
 *     
 *
 *  Exit:
 *
 ******************************************************************************/


BOOL   
finishedTWDIMCacheWrite( UINT cBytes )
{
    ULONG cbFileUsage;

    /*
     *  Complete current file write
     */
    if ( ghDIMHandle != DIM_FILE_CLOSED ) {
    
        /*
         *  Flush last section
         */
        if ( (gDIMHeader.iLastSection != LAST_SECTION_FLAG) ) {
            _lseek( ghDIMHandle, 
                    ((ULONG)sizeof(DIM_HEADER) + ((ULONG)gDIMHeader.iLastSection * (ULONG)SECTION_SIZE)), 
                    SEEK_SET );
            _write( ghDIMHandle, lpbDIMBuffer, SECTION_SIZE );
        }

        /*
         *  Complete header
         */
        gDIMHeader.cbLastSection = (ULONG)cBytes;
//      gDIMHeader.flag = DIM_FILE_FLAG;
    
        /*
         *  Write header information
         */
        _lseek( ghDIMHandle, 0, SEEK_SET  );
        _write( ghDIMHandle, &gDIMHeader, sizeof(DIM_HEADER) );
    
        /*
         *  Update disk usage
         */
        if ( (cbFileUsage = (ULONG) _filelength( ghDIMHandle )) != -1 ) {
            vcbDiskUsage += cbFileUsage;
            REPORT_USAGE
        }

        /*
         *  Complete file io
         */
        _close( ghDIMHandle );
        ghDIMHandle = DIM_FILE_CLOSED;
    
        return( TRUE );
    }

    ASSERT( ghDIMHandle != DIM_FILE_CLOSED, ghDIMHandle );


    return( FALSE );
}


/*******************************************************************************
 *
 *  Function: fTWDIMCacheRemove
 *
 *  Purpose:  DIM Remove File Routine
 *  Entry:
 *     
 *
 *  Exit:
 *       return TRUE for success
 *              FALSE for error
 *
 ******************************************************************************/

BOOL   
fTWDIMCacheRemove( ULONG sigH, ULONG sigL )
{   
    ULONG cbFileSize;

    TRACE((TC_TW, TT_TW_DIM, "ftWDIMCacheRemove: delete file %s", vszFileName));

    /* 
     *  Ditch requested DIM file
     */
    if ( FindCacheFileName( sigH, sigL, &cbFileSize ) == 0 ) {

        /*
         *  Remove it
         */
        if ( _unlink( vszFileName ) != -1 ) {
    
            /*
             *  Update disk usage
             */
            vcbDiskUsage -= cbFileSize;
        }

        return( TRUE );
    }

    REPORT_USAGE

    return( FALSE );    
}


/*******************************************************************************
 *
 *  Function: twDIMCacheError
 *
 *  Purpose: Send a DIM Cache error packet to host 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

INT
twDIMCacheError( PVD pvd, CACHE_FILE_HANDLE fh  )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    PICA_CACHE_ERROR pCacheInfo;
 
    /*
     *  Allocate ICA packet
     */
    WdSetInfo.WdInformationLength = sizeof_ICA_CACHE_ERROR;
    if ( !(pCacheInfo = (PICA_CACHE_ERROR)malloc( WdSetInfo.WdInformationLength)) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        goto done;
    }
    else {
        memset( pCacheInfo, 0, WdSetInfo.WdInformationLength );
    }
 
    /*
     *  Fill in the Cache command info
     */
    pCacheInfo->ByteCount = (USHORT)(WdSetInfo.WdInformationLength - sizeof(pCacheInfo->ByteCount));
    pCacheInfo->Command = PACKET_COMMAND_CACHE_ERROR;
    memcpy(pCacheInfo->FileHandle, fh, sizeof(CACHE_FILE_HANDLE));
 
    TRACE((TC_TW, TT_TW_DIM, 
           "twDIMCacheError: Command %u [%02x %02x %02x %02x]",
           pCacheInfo->Command,pCacheInfo->FileHandle[0],pCacheInfo->FileHandle[1],
           pCacheInfo->FileHandle[2],pCacheInfo->FileHandle[3]));
 
    /*       
     *  Fill in the SetInfo packet
     */
    WdSetInfo.WdInformationClass  = WdCache;
    WdSetInfo.pWdInformation      = pCacheInfo;   
 
    /*
     *  Ship it off
     */
    if ( rc = WdCall(pvd, WD__SETINFORMATION, &WdSetInfo) ) {
        TRACE((TC_WENG, TT_ERROR, 
               "twDIMCacheError:  Class WdDIMS,  Error (%d)", 
               rc));
    }
 
done:

    return( rc );
}


/*******************************************************************************
 *
 *  Function: twDIMCacheResize
 *
 *  Purpose: send dim cache resize packet to host
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

INT
twDIMCacheResize( PVD pvd, ULONG Size )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    PICA_CACHE_RESIZE pCacheInfo;
 
    /*
     *  Allocate ICA packet
     */
    WdSetInfo.WdInformationLength = sizeof_ICA_CACHE_RESIZE;
    if ( !(pCacheInfo = (PICA_CACHE_RESIZE)malloc( WdSetInfo.WdInformationLength)) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        goto done;
    }
    else {
        memset( pCacheInfo, 0, WdSetInfo.WdInformationLength );
    }
 
    /* 
     *  Fill in the Cache command info
     */
    pCacheInfo->ByteCount = (USHORT)(WdSetInfo.WdInformationLength - sizeof(pCacheInfo->ByteCount));
    pCacheInfo->Command = PACKET_COMMAND_CACHE_RESIZE;   
    write_long(pCacheInfo->Size, Size);
 
    TRACE((TC_TW, TT_TW_DIM,
           "twDIMCacheResize: Command %u, size %u\n",
           pCacheInfo->Command,pCacheInfo->Size));
 
    /*
     *  Fill in the SetInfo packet
     */
    WdSetInfo.WdInformationClass  = WdCache;
    WdSetInfo.pWdInformation      = pCacheInfo;   
 
    /*
     *  Ship it off
     */
    if ( rc = WdCall(pvd, WD__SETINFORMATION, &WdSetInfo) ) {
        TRACE((TC_WENG, TT_ERROR, 
               "twDIMCacheResize:  Class WdDIMS,  Error (%d)", 
               rc));
    }
 
done:

    return( rc );
}


/*******************************************************************************
 *
 *  Function: twDIMCacheStream
 *
 *  Purpose: Send a DIM Cache delete packet to host 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

INT
twDIMCacheStream( PVD pvd, UCHAR Count, UCHAR SigLev, PCACHE_FILE_CONTEXT fContextList)
{
    INT rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    PICA_CACHE_STREAM pCacheInfo;
 
    /*
     *  Size packet
     */
    WdSetInfo.WdInformationLength = (USHORT) sizeof_ICA_CACHE_STREAM +
                                    (USHORT) Count * 
                                    (USHORT) sizeof_CACHE_FILE_CONTEXT;

    ASSERT(WdSetInfo.WdInformationLength < 2041, WdSetInfo.WdInformationLength);
 
    /*
     *  Allocate ICA packet
     */
    if ( !(pCacheInfo = (PICA_CACHE_STREAM) malloc( WdSetInfo.WdInformationLength)) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        goto done;
    }
    else {
        memset( pCacheInfo, 0, WdSetInfo.WdInformationLength );
    }
 
    /* 
     *  Fill in the Cache command info
     */
    pCacheInfo->ByteCount = (USHORT)(WdSetInfo.WdInformationLength - sizeof(pCacheInfo->ByteCount));
    pCacheInfo->Command = PACKET_COMMAND_CACHE_STREAM;
    pCacheInfo->Count = Count;
 
    memcpy(pCacheInfo->FileList, fContextList, Count * sizeof_CACHE_FILE_CONTEXT);
 
    /* 
     *  Fill in the SetInfo packet
     */
    WdSetInfo.WdInformationClass  = WdCache;
    WdSetInfo.pWdInformation      = pCacheInfo;   
 
    /* 
     *  Ship it off
     */
    if ( rc = WdCall(pvd, WD__SETINFORMATION, &WdSetInfo) ) {
        TRACE((TC_WENG, TT_ERROR, 
               "twDIMCacheStream:  Class WdDIMS,  Error (%d)", 
               rc));
    }

done:

    return( rc );
}


/*******************************************************************************
 *
 *  Function: twDIMCacheDisable
 *
 *  Purpose: Send a DIM Cache disable packet to host 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

INT   
twDIMCacheDisable( PVD pvd )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    PICA_CACHE_DISABLE pCacheInfo;
 
    /*
     *  Allocate ICA packet
     */
    WdSetInfo.WdInformationLength = sizeof_ICA_CACHE_DISABLE;
    if ( !(pCacheInfo = (PICA_CACHE_DISABLE)malloc( WdSetInfo.WdInformationLength)) ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        ASSERT( 0, rc );
        goto done;
    }
    else {
        memset( pCacheInfo, 0, WdSetInfo.WdInformationLength );
    }
 
    /* 
     *  Fill in the Cache command info
     */
    pCacheInfo->ByteCount = (USHORT)(WdSetInfo.WdInformationLength - sizeof(pCacheInfo->ByteCount));
    pCacheInfo->Command = PACKET_COMMAND_CACHE_DISABLE;   
 
    TRACE((TC_TW, TT_TW_DIM,
           "twDIMCacheDisable: Command %u\n",
           pCacheInfo->Command));
 
    /*
     *  Fill in the SetInfo packet
     */
    WdSetInfo.WdInformationClass  = WdCache;
    WdSetInfo.pWdInformation      = pCacheInfo;   
 
    /*
     *  Ship it off
     */
    if ( rc = WdCall(pvd, WD__SETINFORMATION, &WdSetInfo) ) {
        TRACE((TC_WENG, TT_ERROR, 
               "twDIMCacheDisable:  Class WdDIMS,  Error (%d)", 
               rc));
    }
 
    /*
     *  Reset error count
     */
    vcFileIoErrors = 0;

done:

    return( rc );
}


/*******************************************************************************
 *
 *  Function: twGetDiskFreeSpace
 *
 *  Purpose: get the disk space availabe for DIM caching
 *
 *  Entry:
 *     
 *
 *  Exit: 
 *
 ******************************************************************************/

ULONG
twGetDiskFreeSpace() 
{

#ifdef WIN16

    USHORT DriveNumber;
    struct diskfree_t diskfreeInfo;
    ULONG  freespace;
 
    _dos_getdrive( &DriveNumber );
    if ( _dos_getdiskfree(DriveNumber,&diskfreeInfo) != CLIENT_STATUS_SUCCESS ) {

        TRACE((TC_TW, TT_TW_DIM,
               "twGetDimSpace: can not get current driver free space"));

        return 0;
    }

    freespace = (ULONG) diskfreeInfo.sectors_per_cluster * 
                (ULONG) diskfreeInfo.bytes_per_sector *
                (ULONG) diskfreeInfo.avail_clusters ;   

    TRACE((TC_TW, TT_TW_DIM, 
           "twGetDimSpace: get current driver free space %08X",
           freespace));

    return  freespace;
    
#elif defined(RISCOS)

    int len = strlen(vpszDimCachePath);
    int size;

    vpszDimCachePath[len-1] = '\0';
    LOGERR(_swix(OS_FSControl, _INR(0,1) | _OUT(2), 49, vpszDimCachePath, &size));
    vpszDimCachePath[len-1] = '.';

    return size;
    
#elif defined(WIN32)
   ULONG freespace;
   static pfGETDISKFREESPACEEX lpfnGetDiskFreeSpaceEx = NULL;
   static BOOL bFirstCall = TRUE;
   
   //
   // For the first time call this function, we need to determine the OS version.
   // For Window NT or Window 95 OSR 2 and up, we need to use GetDiskFreeSpaceEx()
   // API function. Otherwise, if GetDiskFreeSpace() API is not available, we need 
   // to use Get GetDiskFreeSpace() API function.
   // The GetDiskFreeSpace() API may return incorrect values for volumes that are 
   // larger than 2 gigabytes.       Billg
   //
   if ( bFirstCall ) {   

       OSVERSIONINFO VersionInfo;
       HANDLE hlib;
 
       bFirstCall = FALSE;         
       VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
       if (GetVersionEx(&VersionInfo)) { 
            if ((VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) ||
               ((VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
                (((USHORT)(VersionInfo.dwBuildNumber)) >= 1000 ))) {
      
                if ( hlib = LoadLibrary("KERNEL32.DLL") ) {
                    lpfnGetDiskFreeSpaceEx = (pfGETDISKFREESPACEEX)
                                 GetProcAddress( hlib, "GetDiskFreeSpaceEx" );
                }
            }
        }
    }

    // if GetDiskFreeSpace() available, use it
    if ( lpfnGetDiskFreeSpaceEx ) {

        ULARGE_INTEGER FreeBytesAvailableToCaller,  // variable to receive free bytes on disk available to the caller
                       TotalNumberOfBytes,	    // variable to receive number of bytes on disk
                       TotalNumberOfFreeBytes;	    // variable to receive free bytes on disk

        if( (*lpfnGetDiskFreeSpaceEx)( NULL, 
                                       &FreeBytesAvailableToCaller,
                                       &TotalNumberOfBytes, 
                                       &TotalNumberOfFreeBytes ) ) {
                                            
            freespace = (FreeBytesAvailableToCaller.HighPart == 0) ?
                        (ULONG) FreeBytesAvailableToCaller.LowPart :
                        (ULONG) (0xFFFFFFFF);

            TRACE((TC_TW, TT_TW_DIM,
                   "twGetDimSpace: Free space avilable from GetDiskFreeSpaceEx %08X \n",
                   freespace));

        }
        else {
          
            freespace = 0;
    
            TRACE((TC_TW, TT_TW_DIM, 
                   "twGetDimSpace: GetDiskFreeSpaceEx() fails, ERROR (%08X)\n", 
                   GetLastError()));

        }
    }
    else   // we can only use GetDiskFreeSpace() API          
    {

        DWORD SectorsPerCluster,	// sectors per cluster 
              BytesPerSector,	        //  bytes per sector 
              NumberOfFreeClusters,	// number of free clusters  
              TotalNumberOfClusters; 	// total number of clusters  
   
        if ( GetDiskFreeSpace( NULL, 
                               &SectorsPerCluster, 
                               &BytesPerSector, 
                               &NumberOfFreeClusters, 
                               &TotalNumberOfClusters ) ) {

            freespace = (ULONG) SectorsPerCluster *
                        (ULONG) BytesPerSector *
                        (ULONG) NumberOfFreeClusters;

            TRACE((TC_TW, TT_TW_DIM, 
                   "twGetDimSpace: Free space avilable from GetDiskFreeSpace %08X \n",
                   freespace));

        }
        else {

            freespace = 0;

            TRACE((TC_TW, TT_TW_DIM,
                   "twGetDimSpace: GetDIskFreeSpace() fails, ERROR(%08X) \n", 
                   GetLastError()));      

        }
    }
   
   return freespace;    

#endif

}

/*
 *  UNUSED, DEAD CODE FOLLOWS
 */

#ifdef notdef
/*******************************************************************************
 *
 *  Function: twDIMCacheDelete
 *
 *  Purpose: Send a DIM Cache write error packet to host 
 *
 *  Entry:
 *     
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

INT
twDIMCacheDelete( PVD pvd, CACHE_FILE_HANDLE fh  )
{
    INT rc = CLIENT_STATUS_SUCCESS;
    WDSETINFORMATION WdSetInfo;
    PICA_CACHE_DELETE pCacheInfo;
 
    /*
     *  Allocate ICA packet
     */
    WdSetInfo.WdInformationLength = sizeof(ICA_CACHE_DELETE);
    if ( !(pCacheInfo = (PICA_CACHE_DELETE)malloc( WdSetInfo.WdInformationLength)) ) {
         rc = CLIENT_ERROR_NO_MEMORY;
         ASSERT( 0, rc );
         goto done;
    }
    else {
        memset( pCacheInfo, 0, WdSetInfo.WdInformationLength );
    }
 
    /* 
     *  Fill in the Cache command info
     */
    pCacheInfo->ByteCount = (USHORT)(WdSetInfo.WdInformationLength - sizeof(pCacheInfo->ByteCount));
    pCacheInfo->Command = PACKET_COMMAND_CACHE_DELETE;
    memcpy(pCacheInfo->FileHandle, fh, sizeof(CACHE_FILE_HANDLE));
 
    TRACE((TC_TW, TT_TW_DIM,
           "twDIMCacheDelete: Command %u [%02x %02x %02x %02x]",
           pCacheInfo->Command,pCacheInfo->FileHandle[0],pCacheInfo->FileHandle[1],
           pCacheInfo->FileHandle[2],pCacheInfo->FileHandle[3] 
                   ));
 
    /*
     *  Fill in the SetInfo packet
     */
    WdSetInfo.WdInformationClass  = WdCache;
    WdSetInfo.pWdInformation      = pCacheInfo;   
 
    /*
     *  Ship it off
     */
    if ( rc = WdCall(pvd, WD__SETINFORMATION, &WdSetInfo) ) {
        TRACE((TC_WENG, TT_ERROR, 
               "twDIMCacheDelete:  Class WdDIMS,  Error (%d)", 
               rc));
    }
 
done:

    return( rc );
}
#endif
