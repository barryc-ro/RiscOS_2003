//
//
// Once this header file settles, place the latest version in \jr\wire.txt
// and \\nt\ntsrc\doc\wire.txt
//
//

/*************************************************************************
*
* cdmwire.h
*
* Wire Protocol Header for CITRIX Client Drive Mapping Redirector.
*
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 01/12/94
*
* Log:
*
*
*
*************************************************************************/


#ifndef _CDMWIRE_
#define _CDMWIRE_

/*
 * All of the structures in this header must be packed. Otherwise, they
 * would have to manually be padded to 4 byte size multiples.
 *
 * Parameters are alaigned on natural boundries, and should pose no
 * problems to RISC architectures with alignment restrictions.
 */

// #pragma pack(1)

/*
 * General structures and formats used by the wire protocol.
 */
#define CDM_VERSION1     0x0001
#define CDM_VERSION2     0x0002
#define CDM_VERSION3     0x0003
#define CDM_VERSION4     0x0004

#define CDM_MIN_VERSION  CDM_VERSION1
#define CDM_MAX_VERSION  CDM_VERSION4

/*
 * This is the time format used by the protocol. It is the raw time format that
 * DOS stores in the directory and returns. Note that the file server could be
 * in a completely different time zone, since it could be a worldwide dial in.
 *
 * The SMB time for older servers is identical to this in LanManager. There are
 * NT routines for converting to/from this format and NT's 100ns 64 bit view of
 * time.
 *
 * A time is a USHORT, in which time is represented by 2 bytes as follows in
 * little endian order:
 *
 *            < Bit Position >
 * 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
 * h  h  h  h  h  m  m  m  m  m  m  x  x  x  x  x
 *
 * hh is the binary number of hours (0-23)
 * mm is the binary number of minutes (0-59)
 * xx is the binary number of two-second increments
 *
 */

/*
 * This is the date format used by the protocol. It is the raw date format that
 * DOS stores in the directory and returns. Note that the file server could be
 * in a completely different time zone, since it could be a worldwide dial in.
 * Any localization or conversion is to be done by the the NT re-director.
 *
 * The SMB date for older servers is identical to this in LanManager. There are
 * NT routines for converting to/from this format and NT's 100ns 64 bit view of
 * time.
 *
 * The day of week is not included here, but must be calculated.
 *
 * A date is a USHORT, in which it is represented by 2 bytes as follows in
 * little endian order:
 *
 *            < Bit Position >
 * 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
 * y  y  y  y  y  y  y  m  m  m  m  d  d  d  d  d
 *
 * mm is the month (1-12)
 * dd is the day of the month (1-31)
 * yy is the year since 1980 (0-119, which means 1980-2099)
 *
 */

/*
 * File Create/Open  AccessModes
 *
 *  Only one of each set may be specifed at a time.
 *
 *  These track the DOS 3.x Open File (3DH) modes
 *
 *  The Bit positions are:
 *
 *                  <I> <S> <R> <A>
 * Open Mode Bits    7  654  3  210
 *
 * <I> Is the Inherit flag, this is not used in this protocol.
 * <S> Is the sharing mode
 * <R> Is reserved
 * <A> Is the Access (Read/Write) mode
 *
 */
// File Sharing Modes
#define CDM_COMPATIBILITY_MODE 0x0000
#define CDM_EXCLUSIVE_MODE     0x0010
#define CDM_DENYWRITE_MODE     0x0020
#define CDM_DENYREAD_MODE      0x0030
#define CDM_DENYNONE_MODE      0x0040

#define CDM_SHARE_MASK         0x0070

// Access Modes
#define CDM_READACCESS         0x0000
#define CDM_WRITEACCESS        0x0001
#define CDM_READWRITE          0x0002

#define CDM_ACCESS_MASK        0x0007

/*
 * File Create Mode.
 *
 * This does not map exactly into a single DOS 3.x call. The server will use a
 * combination of DOS open (3DH), DOS Create (3CH), and DOS Create New File
 * (5BH) to perform the specified actions.
 *
 * This has been done to minimize the number of calls to the server when using
 * NtCreateFile(), which allows the flexibility of specifying all the different
 * actions in one call.
 *
 * Valid combinations are:
 *  CDM_CREATE_NEWFILE|CDM_TRUNCATE  // always create new, destroy old file
 *  CDM_CREATE_NEWFILE|CDM_OPEN_EXISTING // open a file, existing, or new
 *  CDM_OPEN_EXISTING                // Open file, fail if it does not exist
 *  CDM_CREATE_NEWFILE               // Create a new file, fail if one exists.
 *
 */
#define CDM_CREATE_NEWFILE 0x0001  // Create new file, if one does not exist
#define CDM_OPEN_EXISTING  0x0002  // Open existing file only, or fail
#define CDM_TRUNCATE       0x0004  // Truncate existing file


/*
 * DOS 3.x File Attributes. These are the ones actually stored with the file by
 * the FAT file system. Any extended ones in reserved bit positions are invalid
 * for the wire protocol, and must be handled by the NT re-director.
 *
 * NOTE: CDM_ATTR_DIRECTORY and CDM_ATTR_VOLUMELABEL are invalid when set in a
 *       Create, or SetAttr call. They are returned in a FindFirst/FindNext, or
 *       a GetAttr only.
 *
 */
#define CDM_ATTR_READONLY    0x0001
#define CDM_ATTR_HIDDEN      0x0002
#define CDM_ATTR_SYSTEM      0x0004
#define CDM_ATTR_VOLUMELABEL 0x0008
#define CDM_ATTR_DIRECTORY   0x0010
#define CDM_ATTR_ARCHIVE     0x0020

/*
 * These describe the WHENCE for use in the SEEK request.
 *
 * NOTE: Seeking on files for reading and writing makes no sense with this wire
 *       protocol. Every read and write request supplies the offset with the
 *       request and ignores (and changes) the current file position on the
 *       server. The server is free to move this pointer at its will, with no
 *       guarantee's to the client. This has been done to elimate sending a
 *       SEEK request, followed by a READ or WRITE request on the high latency
 *       network.
 *
 *       The SEEK command has been supplied since WHENCE == 2 allows the
 *       current files size to be returned.
 *
 *       The current server will reject WHENCE == CDM_SEEK_BEGINING, and
 *       CDM_SEEK_CURRENT.
 */
#define CDM_SEEK_BEGINING 0x0000
#define CDM_SEEK_CURRENT  0x0001
#define CDM_SEEK_END      0x0002


/*
 * This is the structure returned from FindFirst/FindNext
 *
 * NOTE: This is not padded to a 32 bit boundry. Since it follows in the data
 *       stream after name strings, it will not always be on a 32 bit boundry.
 *       It must be copied into a 32 bit aligned structure in memory on
 *       processors that can not access non 32 bit aligned structures.
 *
 *       Padding could be enforced by padding all names to be a multiple of
 *       four in lenght in the data stream.
 *
 *       Names are NOT NULL Terminated, and the '.' is embedded into the
 *       name if it exists.
 *
 */

typedef struct _FINDSTRUCT {
    USHORT WriteTime;      // Time and date of last write
    USHORT WriteDate;
    ULONG  FileSize;
    USHORT Attributes;     // File Attributes (SYSTEM, READONLY, etc.)
    UCHAR  NameSize;       // Size of the file name
} FINDSTRUCT, *PFINDSTRUCT;

#define sizeof_FINDSTRUCT	11

/*
 * WINDOWS NT and WINDOWS 95 return both the 8.3 name, and
 * the long file name. To properly maintain this linkage,
 * we have to return both.
 *
 * CDM_VERSION4 or greater clients return this structure. If the client
 * does not support long file names, LongNameSize is 0.
 */
typedef struct _FINDSTRUCT_LONG {
    USHORT WriteTime;      // Time and date of last write
    USHORT WriteDate;
    ULONG  FileSize;
    USHORT Attributes;     // File Attributes (SYSTEM, READONLY, etc.)
    UCHAR  NameSize;       // Size of the short file name
    UCHAR  LongNameSize;   // Size of the long file name
} FINDSTRUCT_LONG, *PFINDSTRUCT_LONG;

/*
 * Here we define the minimum name size that must be supported
 * following a FINDSTRUCT. This can be less, but this has been defined
 * to catch transport providers which can not send a full reply header,
 * FINDSTRUCT, and this minimum name size.
 */
#define FIND_NAMESIZE_MIN  12  // This allows for DOS FAT 8.3

/*
 * Error Classes returned from the server.
 *
 * These are the same as the DOS 3.x error classes, and are the most used by
 * clients of the wire protcol. The detailed DOS 3.x errors are usually not
 * needed, or recommended to keep the client abstract enough so that new
 * non-DOS file servers could be used.
 *
 */
#define CDM_ERROR_NONE      0x0000   // No error.
#define CDM_ERROR_RESOURCE  0x0001   // Out of open files, memory, etc.
#define CDM_ERROR_TEMPORARY 0x0002   // Can resolve if try again later
#define CDM_ERROR_NOACCESS  0x0003   // Permission problem
#define CDM_ERROR_INTERNAL  0x0004   // Internal DOS (or Server) error.
#define CDM_ERROR_HARDWARE  0x0005   // Hardware error (I/O error, offline )
#define CDM_ERROR_SYSTEM    0x0006   // General system failure (or Server)
#define CDM_ERROR_INVALID   0x0007   // Invalid request, parameter, etc.
#define CDM_ERROR_NOTFOUND  0x0008   // File, etc. not found.
#define CDM_ERROR_BADFORMAT 0x0009   // Name, or parameters in incorrect format.
#define CDM_ERROR_LOCKED    0x000A   // File/Item locked
#define CDM_ERROR_MEDIA     0x000B   // Media failure, CRC, wrong disk, etc.
#define CDM_ERROR_EXISTS    0x000C   // Collision w/existing item (file, etc.)
#define CDM_ERROR_EOF       0x000D   // End of file on file, directory
#define CDM_ERROR_UNKNOWN   0x000E   // No error class available

/*
 * These are detailed DOS 3.x error codes which may be useful to clients of the
 * wire protocol. All error codes are passed through directly from DOS, but
 * only 'interesting' ones are defined here. Additional ones may be defined as
 * needed by a specific situation.
 *
 * It is recommened that the above Error Class be used for error handling,
 * since these are generic enough to be provided by any given file server. The
 * described detailed error codes below will attempt to be supplied by all
 * servers as well.
 *
 * All possible DOS error codes may not be returned by all possible WinView CDM
 * servers. IE: MacIntosh. The important one CDM_DOSERROR_UNKNOWN means that
 * the Error Class is the only information available, and it is legal for a
 * server to always return when there is an error.
 *
 * These DOS error code were gotten from DOS Programmers Reference 3ed edition
 * by Dettman+Johnson, published by QUE.
 *
 * These are defined for up to DOS 5
 */
#define CDM_DOSERROR_NOERROR       0x0000  // No Error
#define CDM_DOSERROR_INVALIDFUNC   0x0001  // Invalid Function
#define CDM_DOSERROR_NOTFOUND      0x0002  // File not found
#define CDM_DOSERROR_PATHNOTFOUND  0x0003  // Path not found
#define CDM_DOSERROR_NOHANDLES     0x0004  // No more file handles left.
#define CDM_DOSERROR_ACCESSDENIED  0x0005  // No access to file, readonly, etc.
#define CDM_DOSERROR_INVALIDHANDLE 0x0006  // Invalid Handle
#define CDM_DOSERROR_MEMCBDEST     0x0007  // Memory control blocks destroyed
#define CDM_DOSERROR_NOMEM         0x0008  // Insufficient memory
#define CDM_DOSERROR_INVALIDMEM    0x0009  // Invalid memory block address
#define CDM_DOSERROR_INVALIDENV    0x000A  // (10) Invalid environment
#define CDM_DOSERROR_INVALIDFORM   0x000B  // (11) Invalid format
#define CDM_DOSERROR_BADACCESS     0x000C  // (12) An Invalid access code was specifed
#define CDM_DOSERROR_INVALIDDATA   0x000D  // (13) Invalid Data
                                           // (14) is reserved
#define CDM_DOSERROR_BADDRIVE      0x000F  // (15) An Invalid drive was specifed
#define CDM_DOSERROR_DIRBUSY       0x0010  // (16) Attempt to remove current dir
#define CDM_DOSERROR_NOTSAMEDEVICE 0x0011  // (17) Not same device
#define CDM_DOSERROR_NOFILES       0x0012  // (18) No more files left (FILES=xx in CONFIG.SYS)
#define CDM_DOSERROR_WRITEPROTECT  0x0013  // (19) Write Protected disk
#define CDM_DOSERROR_UNKNOWNUNIT   0x0014  // (20) Unknown unit
#define CDM_DOSERROR_NOTREADY      0x0015  // (21) Drive Not Ready
#define CDM_DOSERROR_UNKNOWNCMD    0x0016  // (22) Unknown command
#define CDM_DOSERROR_CRCERROR      0x0017  // (23) CRC error (bad sector)
#define CDM_DOSERROR_BADLENGTH     0x0018  // (24) Bad request structure length
#define CDM_DOSERROR_SEEKERROR     0x0019  // (25) Seek error
#define CDM_DOSERROR_UNKNOWNMEDIA  0x001A  // (26) Unknown Media (FS)
#define CDM_DOSERROR_SECTORNOTFND  0x001B  // (27) Sector not found
#define CDM_DOSERROR_OUTOFPAPER    0x001C  // (28) Out of paper
#define CDM_DOSERROR_WRITEFAULT    0x001D  // (29) Write fault
#define CDM_DOSERROR_READFAULT     0x001E  // (30) Read Fault
#define CDM_DOSERROR_GENERALFAIL   0x001F  // (31) General Failure
#define CDM_DOSERROR_SHARE         0x0020  // (32) Sharing violation
#define CDM_DOSERROR_LOCK          0x0021  // (33) Lock violation
#define CDM_DOSERROR_INVALIDDISK   0x0022  // (34) Invalid disk change
#define CDM_DOSERROR_NOFCB         0x0023  // (35) FCB unavailable
#define CDM_DOSERROR_SHAREOVERFLW  0x0024  // (36) Sharing buffer overflow
#define CDM_DOSERROR_CODEPGMIS     0x0025  // (37) Code Page Mismatch
#define CDM_DOSERROR_ERROREOF      0x0026  // (38) Error handling EOF
#define CDM_DOSERROR_HANDDSKFULL   0x0027  // (39) Handle Disk Full
                                           // (40-49) are reserved
#define CDM_DOSERROR_NETNOTSUP     0x0032  // (50) Network request no supported
#define CDM_DOSERROR_NOTLISTENING  0x0033  // (51) Remove computer not listening
#define CDM_DOSERROR_DUPNETNAME    0x0034  // (52) Duplicate Name on Network
#define CDM_DOSERROR_NETNMNOTFND   0x0035  // (53) Network name not found
#define CDM_DOSERROR_NETBUSY       0x0036  // (54) Network busy
#define CDM_DOSERROR_NETNODEVICE   0x0037  // (55) Network device no longer exists
#define CDM_DOSERROR_NETBCMDLMT    0x0038  // (56) NetBios Cmd Limit exceeded
#define CDM_DOSERROR_NETCARDERR    0x0039  // (57) Network adapter error
#define CDM_DOSERROR_NETBADRESP    0x003A  // (58) Bad network response
#define CDM_DOSERROR_NETUNEXPERR   0x003B  // (59) Unexpected network error
#define CDM_DOSERROR_NETINCADAPT   0x003C  // (60) Network incompatible adapter
#define CDM_DOSERROR_PRINTQUEFULL  0x003D  // (61) Print queue full
#define CDM_DOSERROR_PRINTNOSPACE  0x003E  // (62) Not enough Print file space
#define CDM_DOSERROR_PRINTFILEDEL  0x003F  // (63) Print file deleted
#define CDM_DOSERROR_NETNAMEDEL    0x0040  // (64) Network name deleted
#define CDM_DOSERROR_NACCESSDENIED 0x0041  // (65) Access denied
#define CDM_DOSERROR_NETINCDEVICE  0x0042  // (66) Network device type incorrect
#define CDM_DOSERROR_NETNAMENTFND  0x0043  // (67) Network name not found
#define CDM_DOSERROR_NETNAMELIM    0x0044  // (68) Network name limit exceeded
#define CDM_DOSERROR_NETBSESSLMT   0x0045  // (69) NetBios session limit exceeded
#define CDM_DOSERROR_TMPPAUSED     0x0046  // (70) Temporarily paused
#define CDM_DOSERROR_NETREQNTACC   0x0047  // (71) Network request not accepted
#define CDM_DOSERROR_REDIRPAUSED   0x0048  // (72) Print or disk redirection is paused
                                           // (73-79) Reserved
#define CDM_DOSERROR_EXISTS        0x0050  // (80) File exists
                                           // (81) Reserved
#define CDM_DOSERROR_ERRMKDIR      0x0052  // (82) Can not make directory entry
#define CDM_DOSERROR_INT24FAIL     0x0053  // (83) Fail on int 24
#define CDM_DOSERROR_REDIREXCD     0x0054  // (84) To many redirections
#define CDM_DOSERROR_REDIRDUP      0x0055  // (85) Duplicate redirection
#define CDM_DOSERROR_INVALPASSWD   0x0056  // (86) Invalid password
#define CDM_DOSERROR_INVALPARM     0x0057  // (87) Invalid parameter
#define CDM_DOSERROR_NETDATAFLT    0x0058  // (88) Network data fault
#define CDM_DOSERROR_UNKNOWN       0x00FF  // No DOS error code available

/*
 * Format of the "Status" word returned in the CDM protocol.
 *
 * The "Status" word is returned from all operations that have a reply. This
 * status word contains both the DOS error class (byte 0), and the DOS error
 * code (byte 1).
 *
 * As noted above, the DOS error code could be 0xFF at any time, so if DOS
 * error codes are desired, the following code will ensure compatibility with
 * future WinView Clients (such as MacIntosh)
 *
 * if( CDM_DOSERROR_CODE( Status ) == CDM_DOSERROR_UNKNOWN ) {
 *     // Handle error based on CDM_ERROR_CODE( Status )
 * }
 * else {
 *     // Handle specific error from CDM_DOSERROR_CODE( Status )
 * }
 *
 * This allows a client re-director to provide more detailed information or
 * actions if the DOS error code is available. IE: CDM_DOSERROR_NOFILES is
 * something the user can correct for future sessions by changing the FILES=xx
 * value in their CONFIG.SYS file. This may not apply to a MacIntosh client, so
 * these systems would just return the CDM_ERROR_RESOURCE error with a DOS
 * error code of CDM_DOSERROR_UNKNOWN. The re-director could provide NT error
 * codes based on the detailed information, or the general information, to the
 * user.
 *
 */
#define CDM_ERROR_CODE( Status) ( Status & 0x00FF )
#define CDM_DOSERROR_CODE( Status ) ( ((Status >> 8) & 0x00FF) )
#define CDM_MAKE_STATUS(Error, DosError) ( (unsigned short)((Error & 0x00FF) | (DosError << 8)) )

/*
 * The numbering of the following CDM protocol requests and replies are
 * done in the following way:
 *
 * Requests number from 0-63, replies number from 128-191.
 *
 * This has been done to allow the use of dispatch tables from the on
 * both the client and the server sides without having to have every other
 * entry be empty.
 *
 * NOTE: Values from 64-127, and 192-255 are reserved for printer, com, and
 *       DDE mapping.
 */


/*
 * The Create Request supplies all of the file mode, and attribute information
 * needed to create a given file, and return a "handle" usable for I/O on
 * that file.
 *
 * NOTE: The full path name from the root to and including the file name is
 * given following this structure in the communications stream.
 *
 * The full path name must be supplied, and not a relative path name. The
 * client (WinView Server) is responsible for maintaining knowledge of
 * "current directory" for applications that specify relative path names.
 *
 * If we relied on the File Server (WinView Client) to maintain the
 * "current directory", it would have to allocate memory to maintain this
 * state for every process that has a directory as a current directory
 * on that file system. Since its memory is limited (DOS 640K), we have the
 * larger memory server maintain this information. This is the same technique
 * used for "directory handles".
 *
 * Implementation note: On the server side, this function must be implemented
 * as calls to _dos_creat, or _dos_createnew depending on CreateMode. Once the
 * file has been created, the dos file handle returned from _dos_creat* must be
 * closed, and the new file opened with _dos_open and the supplied AccessMode.
 * This allows the create file call to be compatible with the semantics of
 * _dos_open, and not have to send multiple protocol requests over the wire to
 * get the proper object.
 */

typedef struct _CDM_CREATE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;   // Length of pathname string following this structure
    USHORT AccessMode;     // Access mode. Read/Write, etc.
    USHORT CreateMode;     // Create mode (Truncate, open existing, etc.)
    USHORT Attributes;     // File Attributes to set (system, readonly, etc.)
    USHORT CreateDate;     // HOST date and Time in DOS format to optionally
    USHORT CreateTime;     // use on the client.
} CDM_CREATE_REQUEST, *PCDM_CREATE_REQUEST;
#define CDM_TYPE_CREATE 0

#define sizeof_CDM_CREATE_REQUEST	14

/*
 * Reply returned from file server after a CDM_CREATE_REQUEST was specified
 */

typedef struct _CDM_CREATE_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // File is open after creation
    USHORT Result;        // Result Code(s)
} CDM_CREATE_REQUEST_REPLY, *PCDM_CREATE_REQUEST_REPLY;
#define CDM_TYPE_CREATE_REPLY 128

#define sizeof_CDM_CREATE_REQUEST_REPLY	6

/*
 * Open the given file specified by the full pathname from the root of
 * the volume. Relative pathnames are not supported. See CDM_CREATE_FILE
 * for more information.
 */

typedef struct _CDM_OPEN_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT PathNameSize;  // Length of pathname string following this structure
    USHORT AccessMode;    // Access mode. Read/Write, etc.

} CDM_OPEN_REQUEST, *PCDM_OPEN_REQUEST;
#define CDM_TYPE_OPEN CDM_TYPE_CREATE+1

#define sizeof_CDM_OPEN_REQUEST	6

/*
 * Reply from a CDM_OPEN_REQUEST
 */
typedef struct _CDM_OPEN_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Handle for open file
    USHORT Result;        // Result Code(s)

} CDM_OPEN_REQUEST_REPLY, *PCDM_OPEN_REQUEST_REPLY;
#define CDM_TYPE_OPEN_REPLY CDM_TYPE_CREATE_REPLY+1

#define sizeof_CDM_OPEN_REQUEST_REPLY	6

/*
 * Close the given file releasing the Context value.
 *
 * NOTE: There is no reply to this request. It is expected that the server
 *       will always close the context, and we can free it immediately
 *       after issuing this call.
 *
 *       This also means that the re-director file client must wait until all
 *       outstanding command request packets have returned until it frees
 *       its internal data structures after issuing this packet.
 *
 */
typedef struct _CDM_CLOSE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;

} CDM_CLOSE_REQUEST, *PCDM_CLOSE_REQUEST;
#define CDM_TYPE_CLOSE CDM_TYPE_OPEN+1

/*
 * There is no CDM_CLOSE_REPLY protocol packet.
 * The number is reserved here by this definition.
 */
#define CDM_TYPE_CLOSE_REPLY CDM_TYPE_OPEN_REPLY+1

/*
 * Read from the file represented by context the amount of data at file offset
 * given.
 */
typedef struct _CDM_READ_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  ReadOffset;
    USHORT ReadSize;

} CDM_READ_REQUEST, *PCDM_READ_REQUEST;
#define CDM_TYPE_READ CDM_TYPE_CLOSE+1

#define sizeof_CDM_READ_REQUEST	10

/*
 * Reply from a CDM_READ_REQUEST
 *
 * The data read from the file server follows this header and is represented
 * by ReturnSize.
 */
typedef struct _CDM_READ_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    USHORT TimeStamp;     // Time stamp for cache consistency
    USHORT DateStamp;     // Date stamp for cache consistency
    USHORT ReturnSize;    // Size of read data following this structure
    USHORT Result;        // Result Code(s)

} CDM_READ_REQUEST_REPLY, *PCDM_READ_REQUEST_REPLY;
#define CDM_TYPE_READ_REPLY CDM_TYPE_CLOSE_REPLY+1


/*
 * Write to the file represented by context the amount of data at file offset
 * given.
 *
 * The write data follows this header in the data stream.
 */
typedef struct _CDM_WRITE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  WriteOffset;
    USHORT WriteSize;

} CDM_WRITE_REQUEST, *PCDM_WRITE_REQUEST;
#define CDM_TYPE_WRITE CDM_TYPE_READ+1

#define sizeof_CDM_WRITE_REQUEST	10

/*
 * Reply from a CDM_WRITE_REQUEST
 */
typedef struct _CDM_WRITE_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;        // Which file the read reply is for
    USHORT WroteSize;      // Size of data actually written
    USHORT Result;         // Result Code(s)

} CDM_WRITE_REQUEST_REPLY, *PCDM_WRITE_REQUEST_REPLY;
#define CDM_TYPE_WRITE_REPLY CDM_TYPE_READ_REPLY+1

/*
 * Find the first file in a given directory. Returns a handle to be
 * used for further scanning if all of the directory entries have not been read.
 *
 * This function returns CDM_ERROR_NOTFOUND, CDM_DOSERROR_NOFILES when all of
 * the directory entries have been read into the supplied buffer, with the
 * variable NumberReturned in the FINDFIND_REQUEST_REPLY structure being set to
 * the number returned. This number is always valid for the above return code,
 * with no entries being found in the given directory setting this count to 0.
 * This count is invalid on any error other than CDM_ERROR_NOTFOUND and
 * CDM_ERROR_NONE, and should not be depended on to be set to a valid value.
 *
 * Searching of the directory will continue until either the specified
 * maximum number of entries has been found, the total maximum buffer size
 * has been filled regardless of the number of entries (names are not fixed
 * length), or no more files are to found in the directory with the given
 * search pattern.
 *
 * When more files remain to be found in the given directory, a valid FileId
 * that can be used in a FindNext call is returned in the Context field of the
 * reply structure, and the error code is set to CDM_ERROR_NONE,
 * CDM_DOSERROR_NOERROR. A valid Context is returned only in this case. All
 * other error return values return an invalid context, and there are no files
 * open on the server on behalf of the client.
 *
 * The returned Context handle counts against open file handles allowed on
 * the file server, so it should be closed with FindClose, or by calling
 * FindNext until CDM_ERROR_NOTFOUND, CDM_DOSERROR_NOFILES is returned. The
 * caller is responsible for calling FindClose if FindNext is not called, or
 * called until NOFILES is returned.
 *
 * Note that there are no attributes in the search, only pathname pattern
 * matching using DOS semantics. This means that all files, including hidden
 * and system are returned. It is up to the file client to do any local
 * operating system specific attribute filtering of the files returned.
 *
 * If the file client operating system has any semantic differences from the
 * DOS file name matching, then always pass "*.*" if any wild card specifiers
 * are present to return all entries from the server, so that the local file
 * client can filter the files according to its own rules.
 *
 * The structures here have been defined to be independent of the file name
 * length. The main reply header will contain the count of entries that follow
 * the header, and each entry describes the length of the name string that
 * follows that particular entry. Names are not padded, or null terminated,
 * saving transmission space and time. Only the actual existing characters for
 * the name are returned.
 *
 */
typedef struct _CDM_FINDFIRST_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests

    USHORT PathNameSize;     // Length of pathname string to start search
    USHORT BufferSize;       // Length of buffer available to return entries in
    UCHAR  NumberRequested;  // The (maximum) number of entries requested

} CDM_FINDFIRST_REQUEST, *PCDM_FINDFIRST_REQUEST;
#define CDM_TYPE_FINDFIRST CDM_TYPE_WRITE+1

#define sizeof_CDM_FINDFIRST_REQUEST	7

/*
 * Reply from a CDM_FINDFIRST_REQUEST
 */
typedef struct _CDM_FINDFIRST_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests
    USHORT Context;        // Which file the read reply is for

    USHORT BytesReturned;  // The total number of bytes to read into ret. buffer
    USHORT Result;         // Result Code(s)
    UCHAR  NumberReturned; // Number of FINDSTRUCT entries following this header

} CDM_FINDFIRST_REQUEST_REPLY, *PCDM_FINDFIRST_REQUEST_REPLY;
#define CDM_TYPE_FINDFIRST_REPLY CDM_TYPE_WRITE_REPLY+1

#define sizeof_CDM_FINDFIRST_REQUEST_REPLY	9

/*
 * Find the next file represented by the open FindFirst Context.
 *
 * This routine closes the Context handle on any error return other than
 * CDM_ERROR_NONE. This includes the CDM_ERROR_NOTFOUND case.
 * The old (now invalid) Context is returned so that the client can match
 * up the request with the reply in its internal data structures if it needs.
 * Since the Context Id could be re-used at any time, clients should structure
 * internal data structures around the MpxId, and not rely on the Context
 * value for internal data structure lookups. A (future) multi-threaded server
 * could have re-assigned the Context to another request from a multi-threaded
 * client immediately. It is returned for client internal consistency checks
 * only. (Assertions, etc.)
 *
 * The NumberReturned field of the CDM_FINDNEXT_REQUEST_REPLY structure is
 * valid on the return of the CDM_ERROR_NOERROR and CDM_ERROR_NOTFOUND cases.
 *
 * Searching of the directory will continue until either the specified
 * maximum number of entries has been found, the total maximum buffer size
 * has been filled regardless of the number of entries (names are not fixed
 * length), or no more files are to found in the directory with the given
 * search pattern.
 */
typedef struct _CDM_FINDNEXT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests
    USHORT Context;

    USHORT BufferSize;       // Length of buffer available to return entries in
    UCHAR  NumberRequested;  // The (maximum) number of entries requested

} CDM_FINDNEXT_REQUEST, *PCDM_FINDNEXT_REQUEST;
#define CDM_TYPE_FINDNEXT CDM_TYPE_FINDFIRST+1

#define sizeof_CDM_FINDNEXT_REQUEST	7

/*
 * Reply from a CDM_FINDNEXT_REQUEST
 *
 * NOTE: When Result == ( CDM_ERROR_NOTFOUND, CDM_DOSERROR_NOFILES ),
 *       NumberReturned is still valid, and may be greater than zero, denoting
 *       the entries returned till EOF. The Context handle has automaticly been
 *       closed by the server, and is no longer valid. It may be re-assigned to
 *       another file at any time, so it should no longer be used.
 */
typedef struct _CDM_FINDNEXT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests
    USHORT Context;        // Which file the read reply is for

    USHORT BytesReturned;  // The total number of bytes to read into ret. buffer
    USHORT Result;         // Result Code(s)
    UCHAR  NumberReturned; // Number of FINDSTRUCT entries following this header

} CDM_FINDNEXT_REQUEST_REPLY, *PCDM_FINDNEXT_REQUEST_REPLY;
#define CDM_TYPE_FINDNEXT_REPLY CDM_TYPE_FINDFIRST_REPLY+1

#define sizeof_CDM_FINDNEXT_REQUEST_REPLY	9

/*
 * Close the given FindFirst Context, freeing its resources on the server.
 *
 * NOTE: There is no reply to this request. The client re-director must make
 *       sure any outstanding FindNext() calls have returned before freeing
 *       any internal data structures. Also note that if a FindNext() hits
 *       EOF, then the context was closed automaticly.
 */
typedef struct _CDM_FINDCLOSE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;

} CDM_FINDCLOSE_REQUEST, *PCDM_FINDCLOSE_REQUEST;
#define CDM_TYPE_FINDCLOSE CDM_TYPE_FINDNEXT+1

/*
 * There is no wire protocol reply for a FINDCLOSE request.
 * This definition reserves its number.
 */
#define CDM_TYPE_FINDCLOSE_REPLY CDM_TYPE_FINDNEXT_REPLY+1

/*
 * Get file attributes given the pathname. DOS does not support a handle
 * based version of this call.
 */
typedef struct _CDM_GETATTR_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;       // size of the file pathname that follows

} CDM_GETATTR_REQUEST, *PCDM_GETATTR_REQUEST;
#define CDM_TYPE_GETATTR CDM_TYPE_FINDCLOSE+1

/*
 * Reply from a CDM_GETATTR_REQUEST
 */
typedef struct _CDM_GETATTR_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)
    ULONG  FileSize;       // File size
    USHORT Attributes;     // Attributes of the file.
    USHORT FileDate;
    USHORT FileTime;

} CDM_GETATTR_REQUEST_REPLY, *PCDM_GETATTR_REQUEST_REPLY;
#define CDM_TYPE_GETATTR_REPLY CDM_TYPE_FINDCLOSE_REPLY+1

#define sizeof_CDM_GETAATR_REQUEST_REPLY	14

/*
 * Set file attributes
 *
 * This sets the files attributes based on the pathname. DOS does not supply a
 * handle based version of this call.
 *
 */
typedef struct _CDM_SETATTR_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;   // size of the file pathname that follows
    USHORT Attributes;     // The new attributes

} CDM_SETATTR_REQUEST, *PCDM_SETATTR_REQUEST;
#define CDM_TYPE_SETATTR CDM_TYPE_GETATTR+1

#define sizeof_CDM_SETATTR_REQUEST	6

/*
 * Reply from a CDM_SETATTR_REQUEST
 */
typedef struct _CDM_SETATTR_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)

} CDM_SETATTR_REQUEST_REPLY, *PCDM_SETATTR_REQUEST_REPLY;
#define CDM_TYPE_SETATTR_REPLY CDM_TYPE_GETATTR_REPLY+1


/*
 * Get file date and time given the handle.
 */
typedef struct _CDM_GETDATETIME_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;        // Open file context returned from OPEN, CREATE

} CDM_GETDATETIME_REQUEST, *PCDM_GETDATETIME_REQUEST;
#define CDM_TYPE_GETDATETIME CDM_TYPE_SETATTR+1

/*
 * Reply from a CDM_GETDATETIME_REQUEST
 */
typedef struct _CDM_GETDATETIME_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    USHORT FileDate;
    USHORT FileTime;
    USHORT Result;         // Result Code(s)

} CDM_GETDATETIME_REQUEST_REPLY, *PCDM_GETDATETIME_REQUEST_REPLY;
#define CDM_TYPE_GETDATETIME_REPLY CDM_TYPE_SETATTR_REPLY+1

#define sizeof_CDM_GETDATETIME_REQUEST_REPLY	10

/*
 * Set file date and/or time
 *
 * This sets the files date and/or time based on the Context handle. DOS does
 * not supply a pathname based version of this call.
 *
 */
typedef struct _CDM_SETDATETIME_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;        // Open file context
    USHORT FileDate;
    USHORT FileTime;

} CDM_SETDATETIME_REQUEST, *PCDM_SETDATETIME_REQUEST;
#define CDM_TYPE_SETDATETIME CDM_TYPE_GETDATETIME+1

/*
 * Reply from a CDM_SETDATETIME_REQUEST
 */
typedef struct _CDM_SETDATETIME_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    USHORT Result;         // Result Code(s)

} CDM_SETDATETIME_REQUEST_REPLY, *PCDM_SETDATETIME_REQUEST_REPLY;
#define CDM_TYPE_SETDATETIME_REPLY CDM_TYPE_GETDATETIME_REPLY+1

#define sizeof_CDM_SETDATETIME_REQUEST_REPLY	6

/*
 * Delete the given file name
 */
typedef struct _CDM_DELETE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;      // size of the file pathname that follows

} CDM_DELETE_REQUEST, *PCDM_DELETE_REQUEST;
#define CDM_TYPE_DELETE CDM_TYPE_SETDATETIME+1

/*
 * Reply from a CDM_DELETE_REQUEST
 */
typedef struct _CDM_DELETE_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)

} CDM_DELETE_REQUEST_REPLY, *PCDM_DELETE_REQUEST_REPLY;
#define CDM_TYPE_DELETE_REPLY CDM_TYPE_SETDATETIME_REPLY+1

/*
 * Rename the given file name to the new file name
 */
typedef struct _CDM_RENAME_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests

    USHORT OldPathNameSize;  // size of current file pathname following this struct
    USHORT NewPathNameSize;  // size of new file pathname following the current pathname

} CDM_RENAME_REQUEST, *PCDM_RENAME_REQUEST;
#define CDM_TYPE_RENAME CDM_TYPE_DELETE+1

#define sizeof_CDM_RENAME_REQUEST	6

/*
 * Reply from a CDM_RENAME_REQUEST
 */
typedef struct _CDM_RENAME_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)

} CDM_RENAME_REQUEST_REPLY, *PCDM_RENAME_REQUEST_REPLY;
#define CDM_TYPE_RENAME_REPLY CDM_TYPE_DELETE_REPLY+1

/*
 * Create a directory file
 *
 * A created directory file does not have any open file, or FindFirst Context
 * to close.
 *
 * NOTE: Must verify what attributes/modes may be set on directories on current DOS
 *       systems after creation.
 */
typedef struct _CDM_CREATEDIR_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;   // Length of pathname string following this structure
    USHORT AccessMode;     // Access mode. Read/Write, etc.
    USHORT Attributes;     // File Attributes to set (system, readonly, etc.)
    USHORT CreateDate;     // HOST date and Time in DOS format to optionally
    USHORT CreateTime;     // use on the client.
} CDM_CREATEDIR_REQUEST, *PCDM_CREATEDIR_REQUEST;
#define CDM_TYPE_CREATEDIR CDM_TYPE_RENAME+1

/*
 * Reply returned from file server after a CDM_CREATEDIR_REQUEST was specified
 */

typedef struct _CDM_CREATEDIR_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)
} CDM_CREATEDIR_REQUEST_REPLY, *PCDM_CREATEDIR_REQUEST_REPLY;
#define CDM_TYPE_CREATEDIR_REPLY CDM_TYPE_RENAME_REPLY+1

/*
 * Delete Directory file
 */
typedef struct _CDM_DELETEDIR_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT PathNameSize;   // Length of pathname string following this structure

} CDM_DELETEDIR_REQUEST, *PCDM_DELETEDIR_REQUEST;
#define CDM_TYPE_DELETEDIR CDM_TYPE_CREATEDIR+1

/*
 * Reply returned from file server after a CDM_DELETEDIR_REQUEST was specified
 */

typedef struct _CDM_DELETEDIR_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Result;        // Result Code(s)
} CDM_DELETEDIR_REQUEST_REPLY, *PCDM_DELETEDIR_REQUEST_REPLY;
#define CDM_TYPE_DELETEDIR_REPLY CDM_TYPE_CREATEDIR_REPLY+1

/*
 * There is no wire protocol packet for the client (request) side of the
 * notify cache packet. This define reserves its position.
 */
#define CDM_TYPE_NOTIFYCACHE_REQUEST CDM_TYPE_DELETEDIR+1

/*
 * This call allows the file server to send update information to the file
 * client if it has a reason to know that the cache on a given open file or
 * directory needs to be invalidated.
 *
 * (Currently not sent, allows a future file server to monitor file changes
 *  without the file client having to check every n seconds on a cache hit.)
 *
 * NOTE: Since this is sent from the file server to the file client, its
 *       function number is in the REPLY number space, not request.
 */
typedef struct _CDM_NOTIFYCACHE {
    UCHAR  h_type;
    UCHAR  MpxId;       // Not really needed, but keep all packets consistent

    USHORT Context;     // Which open file/directory that has been updated on the server.
    USHORT Flags;       // Flags to be defined.
} CDM_NOTIFYCACHE, *PCDM_NOTIFYCACHE;
#define CDM_TYPE_NOTIFYCACHE_REPLY CDM_TYPE_DELETEDIR_REPLY+1

#define sizeof_CDM_NOTIFYCACHE	6
/*
 * Read from the file represented by context the amount of data at file offset
 * IF the timestamp on the file is later than the given one.
 */
typedef struct _CDM_READCOND_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;       // Unique ID to allow async server requests

    USHORT Context;
    USHORT TimeStamp;   // Comparision TimeStamp
    USHORT DateStamp;   // Comparision DateStamp
    ULONG  ReadOffset;
    ULONG  Crc32;       // 32 bit CRC for comparision
    USHORT ReadSize;

} CDM_READCOND_REQUEST, *PCDM_READCOND_REQUEST;
#define CDM_TYPE_READCOND CDM_TYPE_NOTIFYCACHE_REQUEST+1

#define sizeof_CDM_READCOND_REQUEST	18

/*
 * Reply from a CDM_READCOND_REQUEST
 *
 * Data is only returned if the TimeStamp supplied in CDM_READCOND_REQUEST
 * is older than the time stamp on the file.
 *
 * If data is returned, the new file timestamp is supplied in the header,
 * with the new file data following.
 *
 * DOS File Time Update NOTE:
 *
 * If a new file is created, and its time is set with the time set
 * call, this new time will be the files time when closed REGARDLESS of
 * the time of any further writes in the file.
 *
 * This behavior should be accounted for in any cache consistency strategies.
 *
 */
typedef struct _CDM_READCOND_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    USHORT TimeStamp;     // (new)Time stamp for cache consistency
    USHORT DateStamp;     // (new)Date stamp for cache consistency
    USHORT ReturnSize;    // Size of read data if result == success.
    USHORT Result;        // Result Code(s)
    USHORT Flags;         // Result flags

#define READCOND_VALIDATED 0x0001  // The data was validated, nothing xfered.

} CDM_READCOND_REQUEST_REPLY, *PCDM_READCOND_REQUEST_REPLY;
#define CDM_TYPE_READCOND_REPLY CDM_TYPE_NOTIFYCACHE_REPLY+1

#define sizeof_CDM_READCOND_REQUEST_REPLY	14

/*
 * Handle a file lock request.
 *
 * The current server only supports LOCKTYPE_DOS. (Exclusive locks)
 */
typedef struct _CDM_FILELOCK_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;
    ULONG  LockStart;
    ULONG  LockSize;
    UCHAR  Type;
#define LOCKTYPE_DOS  1   // Simple locks
#define LOCKTYPE_OS2  2   // Multiple read/single writer locks

} CDM_FILELOCK_REQUEST, *PCDM_FILELOCK_REQUEST;
#define CDM_TYPE_FILELOCK CDM_TYPE_READCOND+1

#define sizeof_CDM_FILELOCK_REQUEST	13

/*
 * Reply from a CDM_FILELOCK_REQUEST
 */
typedef struct _CDM_FILELOCK_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    USHORT Result;        // Result Code(s)

} CDM_FILELOCK_REQUEST_REPLY, *PCDM_FILELOCK_REQUEST_REPLY;
#define CDM_TYPE_FILELOCK_REPLY CDM_TYPE_READCOND_REPLY+1

#define sizeof_CDM_FILELOCK_REQUEST_REPLY	16

/*
 * Handle a file unlock request.
 */
typedef struct _CDM_FILEUNLOCK_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  LockStart;
    ULONG  LockSize;

} CDM_FILEUNLOCK_REQUEST, *PCDM_FILEUNLOCK_REQUEST;
#define CDM_TYPE_FILEUNLOCK CDM_TYPE_FILELOCK+1

/*
 * Reply from a CDM_FILEUNLOCK_REQUEST
 */
typedef struct _CDM_FILEUNLOCK_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    USHORT Result;        // Result Code(s)

} CDM_FILEUNLOCK_REQUEST_REPLY, *PCDM_FILEUNLOCK_REQUEST_REPLY;
#define CDM_TYPE_FILEUNLOCK_REPLY CDM_TYPE_FILELOCK_REPLY+1

#define sizeof_CDM_FILEUNLOCK_REQUEST_REPLY	6

/*
 * Handle a change file size request.
 */
typedef struct _CDM_FILECHANGESIZE_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  NewSize;

} CDM_FILECHANGESIZE_REQUEST, *PCDM_FILECHANGESIZE_REQUEST;
#define CDM_TYPE_FILECHANGESIZE CDM_TYPE_FILEUNLOCK+1

/*
 * Reply from a CDM_FILECHANGESIZE_REQUEST
 */
typedef struct _CDM_FILECHANGESIZE_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    USHORT Result;        // Result Code(s)

} CDM_FILECHANGESIZE_REPLY, *PCDM_FILECHANGESIZE_REPLY;
#define CDM_TYPE_FILECHANGESIZE_REPLY CDM_TYPE_FILEUNLOCK_REPLY+1

#define sizeof_CDM_FILECHANGESIZE_REPLY	6

/*
   This is a seek packet used to get the current file size from the DOS 3.x
   LSEEK (int 21 42H) call, with a whence == 2.

   NOTE: whence == 0, or 1 is still valid, but makes no sense since no
   read or write wire protocol requests rely on a current file position.
*/

typedef struct _CDM_SEEK_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Context;
    ULONG  NewOffset;
    UCHAR  Whence;

} CDM_SEEK_REQUEST, *PCDM_SEEK_REQUEST;
#define CDM_TYPE_SEEK CDM_TYPE_FILECHANGESIZE+1

#define sizeof_CDM_SEEK_REQUEST	9

/*
 * Reply from a CDM_SEEK_REQUEST
 */
typedef struct _CDM_SEEK_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Context;       // Which file the read reply is for
    ULONG  FileSize;      // valid if whence == 2 in request.
    USHORT Result;        // Result Code(s)

} CDM_SEEK_REQUEST_REPLY, *PCDM_SEEK_REQUEST_REPLY;
#define CDM_TYPE_SEEK_REPLY CDM_TYPE_FILECHANGESIZE_REPLY+1

#define sizeof_CDM_SEEK_REQUEST_REPLY	10

/*
 * This is the volume information request call. The "DIR" command shows the
 * available space on the volume, so this is used to return it.
 *
 * NOTE: May want to piggy back this on another request in the future, such as
 *       FindFirst/FindNext. This may not be much overhead because this request
 *       can be sent into the data stream following a FindFirst/FindNext request
 *       without waiting for the reply.
 */

typedef struct _CDM_VOLUMEINFO_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT PathNameSize;
    USHORT Flags;         // Flags to be defined

} CDM_VOLUMEINFO_REQUEST, *PCDM_VOLUMEINFO_REQUEST;
#define CDM_TYPE_VOLUMEINFO CDM_TYPE_SEEK+1

#define sizeof_CDM_VOLUMEINFO_REQUEST	6


/*
 * Reply from a CDM_VOLUMEINFO_REQUEST
 */
typedef struct _CDM_VOLUMEINFO_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)
    ULONG  VolumeSize;     // Total size of volume
    ULONG  BytesFree;      // number of bytes free
    ULONG  AllocationSize; // Bytes per allocation unit (sector, cluster, etc.)
    ULONG  SerialNumber;   // Volume serial number (useful for floppy cache ctl)

} CDM_VOLUMEINFO_REQUEST_REPLY, *PCDM_VOLUMEINFO_REQUEST_REPLY;
#define CDM_TYPE_VOLUMEINFO_REPLY CDM_TYPE_SEEK_REPLY+1

/*
 * Reply from a CDM_VOLUMEINFO_REQUEST on a Version 2 (or greater)
 * CDM protocol.
 *
 * This has the same type index. The uniqueness is determined by
 * the max version of the host and client we are communicating with.
 */
typedef struct _CDM_VOLUMEINFO2_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)
    ULONG  VolumeSize;     // Total size of volume
    ULONG  BytesFree;      // number of bytes free
    ULONG  AllocationSize; // Bytes per allocation unit (sector, cluster, etc.)
    ULONG  SerialNumber;   // Volume serial number (useful for floppy cache ctl)
    ULONG  Flags;          // Flags such as disk changed

} CDM_VOLUMEINFO2_REQUEST_REPLY, *PCDM_VOLUMEINFO2_REQUEST_REPLY;
#define CDM_TYPE_VOLUMEINFO_REPLY CDM_TYPE_SEEK_REPLY+1

/*
 * Reply from a CDM_VOLUMEINFO_REQUEST on a Version 3 (or greater)
 * CDM protocol.
 *
 * This has the same type index. The uniqueness is determined by
 * the max version of the host and client we are communicating with.
 */
typedef struct _CDM_VOLUMEINFO3_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;          // Unique ID to allow async server requests

    USHORT Result;         // Result Code(s)
    ULONG  VolumeSize;     // Total size of volume
    ULONG  BytesFree;      // number of bytes free
    ULONG  AllocationSize; // Bytes per allocation unit (sector, cluster, etc.)
    ULONG  SerialNumber;   // Volume serial number (useful for floppy cache ctl)
    ULONG  Flags;          // Flags such as disk changed
    USHORT VolumeLabelSize; // Volume lable follows this structure if != 0
    USHORT Reserved;       // Pad properly.

} CDM_VOLUMEINFO3_REQUEST_REPLY, *PCDM_VOLUMEINFO3_REQUEST_REPLY;
#define CDM_TYPE_VOLUMEINFO_REPLY CDM_TYPE_SEEK_REPLY+1

// Flags
#define DISKETTE_CHANGED  0x00000001L
#define VOLUME_REMOVEABLE  0x00000002L // Currently only sent on connect

/*
 * CDM Connect request
 *
 * This is sent from the host to the client.
 *
 * This is sent as the first request on all versions later than
 * build 89 and allows for cache control and backward compatibility
 * with older host/client versions.
 *
 * If the settings differ from ones set by the client, the host
 * settings are the ones actually used, and can be due to an administrator
 * setting on the host in a the registry, or a user profile setting.
 *
 */
typedef struct _CDM_CONNECT_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;         // Unique ID to allow async server requests

    USHORT Size;          // Size of this request header
    USHORT VersionLow;
    USHORT VersionHigh;
    USHORT Flags;         // Flags to be defined

    USHORT CacheFlags;
    USHORT CacheTimeout0;
    USHORT CacheTimeout1;
    USHORT CacheXferSize;

} CDM_CONNECT_REQUEST, *PCDM_CONNECT_REQUEST;
#define CDM_TYPE_CONNECT CDM_TYPE_VOLUMEINFO+1

/*
 * Values for CacheFlags
 */
#define CONNECT_NOCACHE          0x0001
#define CONNECT_NOWRITEALLOCATE  0x0002

/*
 * Values for the ValidFlags
 */
#define CONNECT_VALID_CACHEFLAGS 0x0001
#define CONNECT_VALID_TIMEOUT0   0x0002
#define CONNECT_VALID_TIMEOUT1   0x0004
#define CONNECT_VALID_XFERSIZE   0x0008

/*
 * Max volume label length that can be sent
 * from the client to the host on version 3 (DOS level)
 */
#define CDM_MAX_VOLUMELABEL        11   // Like DOS


/*
 * Clients at CDM_VERSION4 or greater reply to the connect request
 * now.
 *
 * This way we can control the exchange of capability information at
 * the CDM level.
 *
 * This has been designed so that we do not have to keep upping the
 * CDM protocol level for minor capability changes such as long file
 * names.
 */
typedef struct _CDM_CONNECT_REQUEST_REPLY {
    UCHAR  h_type;
    UCHAR  MpxId;
    USHORT Result;
    USHORT ReplyVersion; // Reply version
    USHORT ReplySize;    // Size of this packet
    USHORT Capabilities;
} CDM_CONNECT_REQUEST_REPLY, *PCDM_CONNECT_REQUEST_REPLY;

#define CDM_TYPE_CONNECT_REPLY CDM_TYPE_VOLUMEINFO_REPLY+1

#define sizeof_CDM_CONNECT_REQUEST_REPLY	10

/*
 * Defines for connect reply version
 */
#define CDM_CONNECT_REPLY_VERSION1 1

/*
 * Values for capabilities
 *
 * NOTE: Long file names also implies upper and lower case
 *       storage, but not sensitivity. (WIN32 handling)
 */
#define CDM_LONGFILE_NAMES 0x0001
#define CDM_ANSI_CHARS     0x0002


/*
 * Do a findfirst request starting at a specific index into
 * the directory.
 *
 * The CDM FINDFIRST/FINDNEXT calls return a sequence of FINDSTRUCT's
 * for each file in the directory scanned. This call allows the sequence
 * of to begin at any user supplied index before transfering FINDSTRUCTS
 * over the wire. The server maintains the current index on the open
 * find context until it is closed or returns NO_MORE_FILES.
 *
 * The handle returned from this call is compatible with FINDNEXT and
 * FINDCLOSE, where FINDNEXT will return files at the contexts current
 * index. FINDFIRST is now implemented on the DOS server as
 * FINDFIRSTINDEX with a starting index of 0.
 */

typedef struct _CDM_FINDFIRSTINDEX_REQUEST {
    UCHAR  h_type;
    UCHAR  MpxId;            // Unique ID to allow async server requests

    USHORT PathNameSize;     // Length of pathname string to start search
    USHORT BufferSize;       // Length of buffer available to return entries in
    USHORT Index;            // Starting index
    UCHAR  NumberRequested;  // The (maximum) number of entries requested

} CDM_FINDFIRSTINDEX_REQUEST, *PCDM_FINDFIRSTINDEX_REQUEST;
#define CDM_TYPE_FINDFIRSTINDEX CDM_TYPE_CONNECT+1

#define sizeof_CDM_FINDFIRSTINDEX_REQUEST	9

/*
 * Reply from a CDM_FINDFIRSTINDEX_REQUEST
 *
 * This is the same as a FINDFIRST reply
 */
#define CDM_FINDFIRSTINDEX_REQUEST_REPLY CDM_FINDFIRST_REQUEST_REPLY
#define CDM_TYPE_FINDFIRSTINDEX_REPLY CDM_TYPE_FINDFIRST_REPLY

/*
 * We must reserve the number for the reply entry to keep
 * the sequence space intact
 */
#define CDM_FINDFIRSTINDEX_REPLY_RESERVE CDM_TYPE_CONNECT_REPLY+1

/*
 * This is the maximum packet size define used by transport drivers
 * for calculating buffer space and timeout parameters.
 */
#define CDM_MAX_PACKET_SIZE (sizeof(CDM_VOLUMEINFO_REQUEST_REPLY))

/*
 * This is the defined maximum number of requests that the HOST
 * can request from the remote CDM Server.
 *
 * This is a number that will be configurable on the client
 * up to this maximum number.
 *
 * NOTE: This is the defined built in maximum, the registry can
 *       restrict a given installation to less than this.
 */
#define CDM_MAXREQUEST_COUNT  16

#define CDM_MAX_PATH_LEN      255

//#pragma pack()

#endif // _CDMWIRE_

