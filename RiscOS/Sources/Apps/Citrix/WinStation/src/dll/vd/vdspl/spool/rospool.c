
/***************************************************************************
*
*  ntspool.c
*
*  This module contains the source for printer support
*  for Windows 95 and Windows NT.
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author: John Richardson 06/26/95
*          Major Rewrite   09/19/95
*
*  Log:
*
****************************************************************************/

#include "windows.h"
#include "fileio.h"

/*
 *  Get the standard C includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swis.h"

/*  Get CLIB includes */
#include "../../../../inc/client.h"
#include "../../../../inc/clib.h"
#include "../../../../inc/logapi.h"
#include "../../../../inc/lptapi.h"

/* Get our printer header */
#include "../common/printer.h"
#include "../common/splstr.h"

/*
 * Define abstract types used by cpmwire.h to describe the protocol
 */

#ifdef  OUTOUT
typedef unsigned long  ULONG;
typedef unsigned short USHORT;
typedef unsigned char  UCHAR;

typedef unsigned long  *PULONG;
typedef unsigned short *PUSHORT;
typedef unsigned char  *PUCHAR;
typedef void *PVOID;

typedef char  CHAR;
typedef char  *PCHAR;


#define TRUE  1
#define FALSE 0
#endif

typedef  int CPM_BOOLEAN;

#include "citrix/cpmwire.h"

/*
 * This describes our internal printer structure that
 * is returned as the "handle"
 */
typedef struct _PRINTER_ENTRY {
    int	   hPrinter;
    int    HostStatus;
    int	   IsParallel;
    //DOC_INFO_1 DocInfo;
} PRINTER_ENTRY, *PPRINTER_ENTRY;

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

USHORT CpmMapError( DWORD Error );

/*=============================================================================
 ==   Local static data
 ============================================================================*/

PPRINTER_ENTRY OpenPrinters[MAX_PRINTERS] = { NULL };

/*=============================================================================
 ==   Global Data
 ============================================================================*/

//extern CHAR gcDefaultQueueName[];


/*******************************************************************************
 *
 *  CpmOpenPrinter
 *
 * ENTRY:
 *    pName (input)
 *     Name of the printer to open
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS : successfull
 *
 ******************************************************************************/

USHORT
CpmOpenPrinter(
    PCHAR pName,
    int *ppHandle
    )
{
   int        Context;
   BOOL       rc;
   DWORD      Result;
   USHORT     RetVal;
   PPRINTER_ENTRY p;

   TRACE(( TC_CPM, TT_API1, "CpmPrinterOpen: open %s", pName ));

   /*
    * See if a free context entry is available
    */
   Context = CpmFindFreeContext();
   if( Context == (-1) ) {
       RetVal = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOFILES);
       return( RetVal );
   }

   /*
    * Allocate a printer handle
    */
   p = malloc(sizeof(PRINTER_ENTRY) );
   if( p == NULL ) {
       RetVal = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOMEM );
       return( RetVal );
   }

   /*
    * Open the printer
    */
   if ((p->hPrinter = open("printer:", O_WRONLY)) == -1)
   {
       free(p);
       return CPM_MAKE_STATUS( CPM_ERROR_UNKNOWN, CPM_DOSERROR_UNKNOWN );
   }

   p->IsParallel = GetPrinterType() == printer_PARALLEL;   
   
   OpenPrinters[Context] = p;

   *ppHandle = Context;

   TRACE(( TC_CPM, TT_API1, "CpmPrinterOpen: Printer %s opened successfully parallel %d", pName, p->IsParallel ));

   RetVal = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

   return ( RetVal );
}

/*******************************************************************************
 *
 *  CpmClosePrinter
 *
 * ENTRY:
 *    hPrinter (input)
 *       Printer handle to close
 *
 * EXIT:
 *
 ******************************************************************************/

USHORT
CpmClosePrinter( int hPrinter )
{
   BOOL       rc;
   USHORT     Result;
   PPRINTER_ENTRY p;

   /*
    * Range check the printer handle
    */
   if( (hPrinter > MAX_PRINTERS) ||
       (OpenPrinters[hPrinter] == NULL) ) {
       Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
       return( Result );
   }

   TRACE(( TC_CPM, TT_API1, "CpmPrinterClose: close Context %d", hPrinter ));

   p = OpenPrinters[hPrinter];

   /*
    * Close the printer handle
    */
   close(p->hPrinter);

   OpenPrinters[hPrinter] = NULL;

   /*
    * Free the resources
    */
   free( p );

   return( Result );
}

/*******************************************************************************
 *
 *  CpmWritePrinter
 *
 * ENTRY:
 *    hPrinter (input)
 *	 Printer handle returned from CpmOpenPrinter()
 *
 *    pBuf (input)
 *      Buffer with data to write
 *
 *    Length (input)
 *      Length of the data in the buffer
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS : successfull
 *
 ******************************************************************************/

USHORT
CpmWritePrinter(
    int     hPrinter,
    PCHAR   pBuf,
    USHORT  Count,
    PUSHORT pAmountWrote
    )
{
   USHORT Result;
   PPRINTER_ENTRY p;
   DWORD Error;
   int AmountWrote;

   /*
    * Range check the printer handle
    */
   if( (hPrinter > MAX_PRINTERS) ||
       (OpenPrinters[hPrinter] == NULL) ) {
       Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
       return( Result );
   }

   p = OpenPrinters[hPrinter];

   TRACE(( TC_CPM, TT_API2, "CpmPrinterWrite: Called for Context %d",hPrinter));

   /* Write the data to the printer Claim MediaNotPresent upcall
    * around write to suppress the NC dialogue box prompting you for
    * more paper, etc and just send an error back to the server */

   print_upcall_claim();
   AmountWrote = write(p->hPrinter, pBuf, Count);
   print_upcall_release();

   if( AmountWrote <= 0 ) {
       *pAmountWrote = AmountWrote < 0 ? 0 : AmountWrote;
       Error = GetLastError();
       TRACE(( TC_CPM, TT_API1, "CpmPrinterWrite: Error in WritePrinter %d, AmountWrote %d",Error,*pAmountWrote));
       Result = CpmMapError( Error );
       return( Result );
   }

   *pAmountWrote = AmountWrote;
   
   Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

   return( Result );
}

/*******************************************************************************
 *
 * CpmPollPrinter
 *
 * This routine is called for systems which printer polling
 * is required. This is not required on WIN32 clients.
 *
 * ENTRY:
 *   Port (input)
 *     Printer port number to return status on
 *
 *   pStatus (output)
 *     Pointer to variable to place the status.
 *
 * EXIT:
 *  CLIENT_STATUS_SUCCESS - Status byte was returned
 *
 *             Otherwise returns an error code representing the error.
 *
 ******************************************************************************/

USHORT
CpmPollPrinter(
    int   hPrinter,
    ULONG *pStatus,
    int   *pUsefull
    )
{
   USHORT Result;
   int HostStatus;

   /*
    * Range check the printer handle
    */
   if( (hPrinter > MAX_PRINTERS) ||
       (OpenPrinters[hPrinter] == NULL) ) {
       Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
       return( Result );
   }

   HostStatus = 0;
   if (OpenPrinters[hPrinter]->IsParallel)
   {
       LOGERR(_swix(Parallel_Op, _IN(0) | _OUT(2), 0, &HostStatus));
       HostStatus &= 0xF8;	// bottom 3 bits are shown as reserved in PRMII-482
   }
   
   *pStatus = HostStatus;

   Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );

   return( Result );
}

/*****************************************************************************
 *
 *  CpmEnumPrinter
 *
 *  Enumerate printers begining at the given Index.
 *
 *  The index is a logical entry that represents which position
 *  the printer is in.
 *
 *  If the index does not have a printer, since we have scanned
 *  all available ones, return CPM_DOSERROR_NOFILES.
 *
 *  Information returned on success is in newly allocated memory
 *  that the caller must free. Not all info must be returned.
 *  IE: Comment field, etc.
 *
 * ENTRY:
 *   Index (input)
 *     Index to start the enumeration at
 *
 *   pBuf (input/output)
 *     Variable to place allocated array of ENUMSTRUCT structures
 *     and strings. Format is defined in cpmwire.h for ENUMPRINTER_REPLY
 *
 *   MaxBytes (input)
 *     Maximum number of bytes for return data
 *
 *   pCount (input/output)
 *     Number of  enumerated entries returned.
 *
 *   pReturnSize
 *     Size of data marshalled into the buffer.
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

#define DEFAULT_STRING "WFCDefault"

/* Imported from app/main.c */
extern char *printer_name;
extern char *printer_type;

/* Imported from vdspl.c */
extern char gcDefaultQueueName[128];

USHORT
CpmEnumPrinter(
    USHORT    Index,
    PCHAR     pBuf,
    USHORT    MaxBytes,
    PUSHORT   pCount,
    PUSHORT   pReturnSize
    )
{
    char *ptr;
    PENUMSTRUCT p;
    int size;
    char *type;

    ASSERT( pCount != NULL, (int)pCount );
    ASSERT( pReturnSize != NULL, (int)pReturnSize );

    *pCount = 0;
    *pReturnSize = 0;

    /* will only return 1 file ever */
    if (Index != 0)
	return CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOFILES);

    /* if no driver configured*/
    if (printer_name == NULL)
	return CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOFILES);

    /* get size needed */
    size = sizeof_ENUMSTRUCT + (strlen(printer_name) + 1) + sizeof(DEFAULT_STRING);

    /* check space in output buffer */
    if (size > MaxBytes)
        return CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_BADLENGTH );

    p = (PENUMSTRUCT)pBuf;
    memset(p, 0, sizeof_ENUMSTRUCT);
    ptr = (char *)p + sizeof_ENUMSTRUCT;

    if (printer_name)
    {
	p->NameSize = strlen(printer_name) + 1;		// Size of the printer name following this header
	memcpy(ptr, printer_name, p->NameSize);
	ptr += p->NameSize;
    }

    type = NULL;
    if (gcDefaultQueueName[0])			// this is set in the ini file - allow the override first
	type = gcDefaultQueueName;
    else if (printer_type)			// this is set in the frontend from the PSPrinter messages
	type = printer_type;
    else 
	// on NCOS the printer name is the printer type so may match a
	// windows name if we're lucky.
	type = printer_name;

    if (type)
    {
	p->DriverSize = strlen(type) + 1;			// Size of the driver name following this header
	memcpy(ptr, type, p->DriverSize);
	ptr += p->DriverSize;
    }
    
    /* add comment to say this is the default printer */
    p->CommentSize = sizeof(DEFAULT_STRING);
    memcpy(ptr, DEFAULT_STRING, p->CommentSize);
    ptr += p->CommentSize;

#if 0
    p->DriverSize;     // Size of the driver name following the printer name
    p->PortSize;       // Size of the port name following the driver name
    p->Flags;          // Flags
    p->ExtraSize;      // Size of extra information if supplied following Comment
#endif

    *pCount = 1;
    *pReturnSize = ptr - (char *)p;

    return CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );
}

/*****************************************************************************
 *
 *  CpmFindFreeContext
 *
 *   Find the first free printer context entry
 *
 * ENTRY:
 *   Param1 (input/output)
 *     Comments
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

int
CpmFindFreeContext()
{
    int Context;

    for( Context=0; Context < MAX_PRINTERS; Context++ ) {
        if( OpenPrinters[Context] == NULL ) {
            return( Context );
	}
    }

    return( -1 );
}


/*****************************************************************************
 *
 *  CpmMapError
 *
 *   Map WIN32 errors to CPM wire errors
 *
 * ENTRY:
 *   Param1 (input/output)
 *     Comments
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

/*
 * Parallel device driver errors, extracted from the module source.
 */
 
#define ERROR_PARALLEL_BADSWI		0x13540
#define ERROR_PARALLEL_BADHARDWARE	0x13541
#define ERROR_PARALLEL_INUSE		0x13542
#define ERROR_PARALLEL_BADPARM		0x13543
#define ERROR_PARALLEL_BADCALL		0x13544
#define ERROR_PARALLEL_PAPEROUT		0x13545
#define ERROR_PARALLEL_OFFLINE		0x13546
#define ERROR_PARALLEL_OTHERERROR	0x13547

#define ERROR_ESCAPE			0x11
#define ERROR_PRINT_CANCELLED		0x5C9

USHORT
CpmMapError(
    DWORD Error
    )
{
    USHORT Result;

    switch( Error ) {

    case 0:  // No error
        Result = CPM_MAKE_STATUS( CPM_ERROR_NONE, CPM_DOSERROR_NOERROR );
        break;
#if 0
	// allowing these options will make paper out messages be generated
	// however no printer connected also generates paper out so they are
	// disabled for now
    case ERROR_PARALLEL_PAPEROUT: //ERROR_OUT_OF_PAPER:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_OUTOFPAPER );
        break;

    case ERROR_PARALLEL_OFFLINE: //ERROR_NOT_READY:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOTREADY );
        break;
#endif

    default:
       Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOTREADY );
       break;
    }

    TRACE(( TC_CPM, TT_API1, "CpmMapError: RISC OS %d, CPMWIRE 0x%x",Error,Result));

    return( Result );
}


