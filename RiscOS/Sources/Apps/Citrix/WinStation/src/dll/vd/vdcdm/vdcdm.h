
#ifndef __VDCDM_H__
#define __VDCDM_H__

/*======================================================================
 =  Common Stuff
 ======================================================================*/

/*
 *  RISCOS Stuff
 */
#ifdef  RISCOS

typedef struct _finddata_t  FIND;
typedef FIND far *          PFIND;

#define DOSEXTERR(a)        dosexterr( a )
#define OPEN(a,b,c,d)       c = open( a, b )
#define CREATNEW(a,b,c,d)   c = creatnew( a, b )
#define CREAT(a,b,c,d)      c = creat( a, b )
#define DELETEFILE(a)       remove( a )
#define RENAMEFILE(a,b)     rename( a, b )
#define SEEK(a,b,c)         lseek( a, b, c )
#define CLOSE(a)            close( a )
#define FSREAD(a,b,c,d,e)   d = read( a, b, c )
#define FSWRITE(a,b,c,d,e)  d = write( a, b, c )
#define FINDFIRST(a,b,c,d)  c = _findfirst( a, b )
#define FINDNEXT(a,b,c)     c = _findnext( a, b )
#define FINDCLOSE(a)	    _findclose(a)
#endif

/*
 *  Dos Stuff
 */
#ifdef  DOS

typedef struct find_t       FIND;
typedef FIND far *          PFIND;

#define DOSEXTERR(a)        dosexterr( a )
#define OPEN(a,b,c,d)       d = _dos_open( a, b, &c )
#define CREATNEW(a,b,c,d)   d = _dos_creatnew( a, b, &c )
#define CREAT(a,b,c,d)      d = _dos_creat( a, b, &c )
#define DELETEFILE(a)       _dos_delete( a )
#define RENAMEFILE(a,b)     dos_rename( a, b )
#define SEEK(a,b,c)         _dos_seek( a, b, c )
#define CLOSE(a)            _dos_close( a )
#define FSREAD(a,b,c,d,e)   e = _dos_read( a, b, c, &d )
#define FSWRITE(a,b,c,d,e)  e = _dos_write( a, b, c, &d )
#define FINDFIRST(a,b,c,d)  d = _dos_findfirst(a, (_A_NORMAL | _A_RDONLY |    \
                                                   _A_HIDDEN |                \
                                                   _A_SUBDIR | _A_ARCH ), b )
#define FINDNEXT(a,b,c)     c = _dos_findnext( b )
#define FINDCLOSE(a)
#endif

/*
 *  Win16 Stuff
 */
#ifdef  WIN16

typedef struct find_t       FIND;
typedef FIND far *          PFIND;

#define DOSEXTERR(a)        _dosexterr( a )

#define OPEN(a,b,c,d)       d = _dos_open( a, b, &c )
#define CREATNEW(a,b,c,d)   d = _dos_creatnew( a, b, &c )
#define CREAT(a,b,c,d)      d = _dos_creat( a, b, &c )
#define DELETEFILE(a)       remove( a )
#define RENAMEFILE(a,b)     rename( a, b )
#define SEEK(a,b,c)         _dos_seek( a, b, c )
#define CLOSE(a)            _dos_close( a )
#define FSREAD(a,b,c,d,e)   e = _dos_read( a, b, c, &d )
#define FSWRITE(a,b,c,d,e)  e = _dos_write( a, b, c, &d )
#define FINDFIRST(a,b,c,d)  d = _dos_findfirst(a, (_A_NORMAL | _A_RDONLY |    \
                                                   _A_HIDDEN |                \
                                                   _A_SUBDIR | _A_ARCH ), b )
#define FINDNEXT(a,b,c)     c = _dos_findnext( b )
#define FINDCLOSE(a)

#endif

/*
 *  Win32 Stuff
 */
#ifdef  WIN32

//typedef struct _finddata_t  FIND;
typedef WIN32_FIND_DATA     FIND;
typedef LPWIN32_FIND_DATA   PFIND;

#define OPEN(a,b,c,d)       c = _lopen( a, b )
#define CREATNEW(a,b,c,d)   c = _lcreat( a, b )
#define CREAT(a,b,c,d)      c = _lcreat( a, b )
#define DELETEFILE(a)       remove( a )
#define RENAMEFILE(a,b)     rename( a, b )
#define SEEK(a,b,c)         _llseek( a, b, c )
#define CLOSE(a)            _lclose( a )
#define FSREAD(a,b,c,d,e)   d = _lread( a, b, c )
#define FSWRITE(a,b,c,d,e)  d = _lwrite( a, b, c )
#define FINDFIRST(a,b,c,d)  c = w32CDMFindFirstFile( a, b )
#define FINDNEXT(a,b,c)     c = !w32CDMFindNextFile( a, b )
#define FINDCLOSE(a)        FindClose( a )

#endif


/*======================================================================
 =  CDMDOSIO.C stuff
 ======================================================================*/

/*
 * Flags for CdmDosVolumeInfo
 */

#define DRIVE_CHANGED 0x0001

/*
 * Prototypes for functions that call out into the operating
 * system specific I/O interfaces
 *
 * NOTE: The names of these functions implies DOS 3.x semantics, not
 *       necessarly a DOS 3.x implementation. The operating system specific
 *       interface module is responsible for mapping its semantics to the
 *       DOS 3.x semantics, error codes, etc.
 *
 */

void CdmDosCreate( PCHAR, USHORT, USHORT, USHORT, USHORT, PUSHORT, PUSHORT );
void CdmDosOpen( PCHAR, USHORT, USHORT, PUSHORT, PUSHORT );
void CdmDosClose( USHORT, PUSHORT );
void CdmDosRead( USHORT, PCHAR, USHORT, ULONG, PUSHORT, PUSHORT, PUSHORT, PUSHORT );
void CdmDosWrite( USHORT, PCHAR, USHORT, ULONG, PUSHORT, PUSHORT );
void CdmDosFindFirstIndex( PCHAR, USHORT, USHORT, USHORT, PCHAR, USHORT, PUSHORT, UCHAR, PCHAR, PUSHORT, PUSHORT);
void CdmDosFindNext( USHORT, USHORT, USHORT, PCHAR, USHORT, PUSHORT, UCHAR, PUCHAR, PUSHORT );
void CdmDosFindClose( USHORT, PUSHORT );
void CdmDosGetAttrEx( PCHAR, USHORT, PUSHORT, PULONG, PUSHORT, PUSHORT, PUSHORT );
void CdmDosSetAttr( PCHAR, USHORT, USHORT, PUSHORT );
void CdmDosGetDateTime( USHORT, PUSHORT, PUSHORT, PUSHORT );
void CdmDosSetDateTime( USHORT, USHORT, USHORT, PUSHORT );
void CdmDosDelete( PCHAR, USHORT, PUSHORT );
void CdmDosRename( PCHAR, USHORT, PCHAR, USHORT, PUSHORT );
void CdmDosCreateDir( PCHAR, USHORT, USHORT, USHORT, PUSHORT );
void CdmDosDeleteDir( PCHAR, USHORT, PUSHORT );
void CdmDosReadCond( USHORT, PCHAR, USHORT, ULONG, PUSHORT, PUSHORT, PUSHORT, PUSHORT, PUSHORT );
void CdmDosFileLock( USHORT, ULONG, ULONG, UCHAR, PUSHORT );
void CdmDosFileUnLock( USHORT, ULONG, ULONG, PUSHORT );
void CdmDosFileChangeSize( USHORT, ULONG, PUSHORT );
void CdmDosSeek( USHORT, ULONG, UCHAR, PULONG, PUSHORT );
void CdmDosVolumeInfo( USHORT, USHORT, PULONG, PULONG, PULONG, PULONG, PUSHORT, PUSHORT );
BOOLEAN CdmHasDriveChanged( UCHAR DriveNumber );
UCHAR _dos_mygetharderr( void );




/*======================================================================
 =  CDMTRANS stuff
 ======================================================================*/

/*
 * Definition of our internal transport structure. This is passed
 * as a PVOID to the clients, and they will pass it in on all calls
 * to identify the unique transport.
 */
typedef struct _CDMTRANSPORT {
    int             State;     // The state of the connection.
    PCHAR           Buf;       // The output buffer
    unsigned int    BufSize;   // The size of the output buffer
    unsigned int    BufCount;  // Number of bytes currently in buffer
} CDMTRANSPORT, *PCDMTRANSPORT;

// Define connection states

#define CDMTRANSPORT_UNINITIALIZED 0
#define CDMTRANSPORT_CONNECTED     1
#define CDMTRANSPORT_DISCONNECTED  2
#define CDMTRANSPORT_ERROR         3

// Define Error codes

#define CDMTRANSPORT_ERROR_NOCONNECTION 1
#define CDMTRANSPORT_ERROR_OVERFLOW     2
#define CDMTRANSPORT_ERROR_UNKNOWN      3

/*
 * Define the maximum PDU size we will handle
 * Actual PDU size will be less since the main init function passes
 * the size to use at connect time.
 */

#define CDM_MAXTRANSPORT_SIZE 4096

/*
 * Structure for allocating buffers for deferred sends or requests.
 *
 * Since the ICA may not be able to send when we have a request, we need
 * to store either the request or the reply until a later time when space
 * (send window) is available.
 * Since we are running in a polled DOS environment, we can not block waiting
 * for this condition. This structure allows us to allocate the space we need
 * for processing the request/reply at a later point in time.
 *
 * These buffers are allocated at CDM Open time, and represent the maximum
 * number of requests that can be outstanding against the CDM server from
 * the NT host.
 *
 */
typedef struct _CDMQUEUE {
    int              Type;      // Is this a request or a reply
    struct _CDMQUEUE *Next;     // Next one in list
    LPVOID           Transport; // Transport Handle
    PCHAR            Buf;       // The buffer with the queued data
    unsigned int     BufCount;  // The size of the queued data
    unsigned int     BufSize;   // The size of the buffer available for queued data
} CDMQUEUE, *PCDMQUEUE;

// Define Queue types

#define CDMQUEUE_TYPE_FREE    0
#define CDMQUEUE_TYPE_REQUEST 1
#define CDMQUEUE_TYPE_REPLY   2


/*
 *  Function prototypes for CDMTRANS.C
 */
int SetupTransport( USHORT TotalByteCount, USHORT MaxByteCount, LPVOID * pCdmTransportHandle );
BOOLEAN DeleteTransport( LPVOID pTransport );
int CdmTransportSendData( LPVOID  pTransport, PCHAR  pBuf, USHORT Size, USHORT Final );
USHORT CdmTransportMaxSize( LPVOID pTransport );
USHORT CdmTransportReceiveData( LPVOID  pTransport, PCHAR  pBuf, USHORT Size );
int CdmTransportQueueData( LPVOID pTransport, int Type, PCHAR pBuf, USHORT Size );
void CdmTransportDoDeferred( void );
BOOLEAN CdmTransportQueueReady( void );
BOOLEAN CdmTransportAllocSendBuf( LPVOID );
USHORT CdmWireDataReceive( LPVOID  pTransport, PCHAR  pBuf, USHORT Size );


/*======================================================================
 =  Context stuff
 ======================================================================*/
/*
 * This is the structure used to manage a per open file/Find context
 * on the DOS server.
 *
 * The FileId's (Context) passed back to the client on successful open,
 * create, and findfirst calls is an index into this structure.
 *
 */
#define MAX_OPEN_CONTEXT 20

/*
 * Structure for managing an open file
 *
 * Since directory entry structures are 43 bytes, while file entries
 * are only 2, the memory is allocated separatly depending on the type.
 *
 * This allows the more frequent open files to not consume more memory
 * than they have to.
 */

typedef struct _FILEENT {
    int     DosHandle;
    // add other info as needed here
    BOOL    OpenForWriting;
    USHORT  Attributes;
    BOOL    fAttributesChanged;
    BOOL    fAttributesValid;
    USHORT  NameSize;
    CHAR    pName[256+1];
} FILEENT, *PFILEENT;

typedef struct _OPENCONTEXT {

    int Type;
    unsigned short DirIndex; // Current directory index
    HANDLE  FindHandle;             // Only for WIN32
#ifdef PERF_PROFILE
    BOOL    IsNullFile;
#endif
    BOOL    FindBufferFull; // for push back
    union {
        PFILEENT pFileEnt;
        PFIND    pFindBuf;
    } x;
} OPENCONTEXT, *POPENCONTEXT;


// Values for Type
#define OPENCONTEXT_INVALID 0  // Free entry
#define OPENCONTEXT_FILE    1
#define OPENCONTEXT_FIND    2

/*
 * This defines the maximum pathname that we attempt to handle.
 * It is currently limited in what can fit into a 64K memory
 * region, but may be limited by DOS itself to a smaller number.
 *
 * NOTE: The following value must leave room for the NULL terminaling
 *       byte without overflowing the segment.
 */
#define MAX_PATH_NAME 65534

/*
 *  Function prototypes for the context stuff
 */
USHORT AllocateContext( int Type );
POPENCONTEXT ValidateContext( USHORT FileId );
void FreeContext( USHORT FileId );
void CdmDosCloseAllContexts( VOID );
BOOL ContextSetAttributes( PCHAR pName, USHORT PathNameSize, USHORT Attributes );
void ContextGetAttributes( PCHAR pName, USHORT PathNameSize, PUSHORT pAttributes, PULONG pFileSize );


/*======================================================================
 =  DOSIO.C stuff (DOSONLY)
 ======================================================================*/
#ifdef  DOS
int _mkdir( PCHAR pName );
int _rmdir( PCHAR pName );
int dos_rename( PCHAR pOldName, PCHAR pNewName );
long _dos_seek( int Handle, ULONG  Offset, int Whence, );
int _dos_lock( int Handle, int Operation, ULONG Offset, ULONG Size );
UCHAR _dos_mygetharderr( void );
#endif

/*======================================================================
 =  IO.C stuff
 ======================================================================*/
int _dos_rootftime( UINT * pFileDate, UINT * pFileTime );
void CdmDosError( int RetVal, PUSHORT pErrorClass, PUSHORT pExtError );


#ifdef  WIN32
/*======================================================================
 =  WIN32IO.C stuff
 ======================================================================*/
int _dos_lock( int Handle, USHORT Operation, ULONG Offset, ULONG Size );
int _dos_getfileattr( PCHAR pPath, UINT *pAttribute );
int _dos_setfileattr( PCHAR pPath, UINT Attribute );
int _dos_getftime( int Handle, UINT *pDate, UINT *pTime );
int _dos_setftime( int Handle, UINT Date, UINT Time );
HANDLE w32CDMFindFirstFile(LPCTSTR lpszSearchFile, LPWIN32_FIND_DATA lpffd );
BOOL w32CDMFindNextFile(HANDLE nexthandle, LPWIN32_FIND_DATA lpffd );
#endif

#endif //__VDCDM_H__
