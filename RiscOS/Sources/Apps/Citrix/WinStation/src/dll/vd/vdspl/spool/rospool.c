
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
#ifdef  DOS
#include "../../../inc/clib.h"
#include "../../../inc/dos.h"
#endif
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
    //DOC_INFO_1 DocInfo;
} PRINTER_ENTRY, *PPRINTER_ENTRY;

/*=============================================================================
 ==   Functions Used
 ============================================================================*/


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

   OpenPrinters[Context] = p;

   *ppHandle = Context;

   TRACE(( TC_CPM, TT_API1, "CpmPrinterOpen: Printer %s opened successfully",pName ));

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
   BOOL rc;
   USHORT Result;
   PPRINTER_ENTRY p;
   DWORD Error;
   ULONG AmountWrote;

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

   /*
    * Write the data to the printer
    */
   write(p->hPrinter, pBuf, Count);

   *pAmountWrote = (USHORT)Count;

#if 0
   if( !rc || (*pAmountWrote <= 0) ) {
       Error = GetLastError();
       TRACE(( TC_CPM, TT_API1, "CpmPrinterWrite: Error in WritePrinter %d, AmountWrote %d",Error,*pAmountWrote));
       Result = CpmMapError( Error );
       return( Result );
   }
#endif
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

   /*
    * Range check the printer handle
    */
   if( (hPrinter > MAX_PRINTERS) ||
       (OpenPrinters[hPrinter] == NULL) ) {
       Result = CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_INVALIDHANDLE );
       return( Result );
   }

   *pStatus = OpenPrinters[hPrinter]->HostStatus;

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

USHORT
CpmEnumPrinter(
    USHORT    Index,
    PCHAR     pBuf,
    USHORT    MaxBytes,
    PUSHORT   pCount,
    PUSHORT   pReturnSize
    )
{
    char *name, *ptr;
    PENUMSTRUCT p;
    int size;

    ASSERT( pCount != NULL, (int)pCount );
    ASSERT( pReturnSize != NULL, (int)pReturnSize );

    *pCount = 0;
    *pReturnSize = 0;

    /* will only return 1 file ever */
    if (Index != 0)
	return CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOFILES);

    /* if no driver configured*/
    if (_swix(PDriver_Info, _OUT(4), &name))
	return CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOFILES);

    size = sizeof_ENUMSTRUCT + strlen(name);
    /* check space in output buffer */
    if (size > MaxBytes)
        return CPM_MAKE_STATUS( CPM_ERROR_INVALID, CPM_DOSERROR_BADLENGTH );

    memset(p, 0, sizeof_ENUMSTRUCT);
    ptr = (char *)p + sizeof_ENUMSTRUCT;

    if (name)
    {
	p->NameSize = strlen(name);		// Size of the printer name following this header
	memcpy(ptr, name, p->NameSize);
	ptr += p->NameSize;
    }

#if 0
    p->DriverSize;     // Size of the driver name following the printer name
    p->PortSize;       // Size of the port name following the driver name
    p->CommentSize;    // Size of the comment field following the port name
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
#if 0
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

     case ERROR_PRINT_CANCELLED:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_PRINTFILEDEL );
        break;

     case ERROR_NO_SPOOL_SPACE:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_PRINTNOSPACE );
        break;

     case ERROR_PRINTQ_FULL:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_PRINTQUEFULL );
        break;

     case ERROR_OUT_OF_PAPER:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_OUTOFPAPER );
        break;

     case ERROR_NOT_READY:
        Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_NOTREADY );
        break;

     case ERROR_FILE_NOT_FOUND:
     case ERROR_PATH_NOT_FOUND:
     case ERROR_INVALID_NAME:
     case ERROR_SPOOL_FILE_NOT_FOUND:
       Result = CPM_MAKE_STATUS( CPM_ERROR_NOTFOUND, CPM_DOSERROR_NOTFOUND );
       break;

     default:
       Result = CPM_MAKE_STATUS( CPM_ERROR_RESOURCE, CPM_DOSERROR_UNKNOWN );
       break;
    }

    TRACE(( TC_CPM, TT_API1, "CpmMapError: Win32 %d, CPMWIRE 0x%x",Error,Result));

    return( Result );
}

#endif

