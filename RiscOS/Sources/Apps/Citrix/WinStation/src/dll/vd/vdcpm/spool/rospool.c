
/***************************************************************************
*
*  rospool.c
*
*  This module contains the source for parallel and serial printer support
*  for Windows
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author: John Richardson 06/26/95
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
#include <string.h>


/*  Get CLIB includes */
#include "../../../../inc/client.h"
#ifdef  DOS
#include "../../../inc/clib.h"
#include "../../../inc/dos.h"
#endif
#include "../../../../inc/logapi.h"
#include "../../../../inc/lptapi.h"
#include "../common/cpmstr.h"


#define MAX_PRINTER_PORTS  1
//#define COM_PORTS_START    4


/*=============================================================================
==   Functions Defined
=============================================================================*/

int WFCAPI LptOpen( int );
int WFCAPI LptClose( int );
int WFCAPI LptWriteBlock( int Port, char *pBuf, int Count, int *pAmountWrote );
int WFCAPI LptStatus( int, int * );

/*=============================================================================
 ==   Functions Used
 ============================================================================*/

/*=============================================================================
 ==   Local static data
 ============================================================================*/

typedef struct _PORTENTRY {
    PCHAR  Name;
    int hPrinter;
    DWORD  JobId;
    BOOL   Open;
    int    HostStatus;
} PORTENTRY, *PPORTENTRY;

/*
 * Initialize the port entries
 */

static PORTENTRY Ports[MAX_PRINTER_PORTS] = {
       { "printer:", 0, 0, FALSE, 0 }
   };


/*=============================================================================
 ==   Global Data
 ============================================================================*/

//extern CHAR gcDefaultQueueName[];


/*******************************************************************************
 *
 *  LptOpen
 *
 * ENTRY:
 *    Port (input)
 *      Port identifier for printer port to open
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS : successfull
 *
 ******************************************************************************/

int WFCAPI
LptOpen( int Port )
{
   BOOL       rc;
   DWORD      Result;
   int     hPrinter;
   CHAR       Buf[512];
   PPORTENTRY p;

   /*
    * Range check the port value
    */

   if( Port > MAX_PRINTER_PORTS ) {
       return CLIENT_ERROR_INVALID_HANDLE;
   }

   if ( Ports[Port].Open != FALSE ) {
      return CLIENT_ERROR_OPEN_FAILED;
   }

   TRACE(( TC_CPM, TT_API1, "LptSpoolOpen: open %s", Ports[Port].Name ));

   p = &Ports[Port];

   /*
    * Open the printer
    */
   if ((hPrinter = open(Ports[Port].Name, O_WRONLY)) == -1)
   {
       return CLIENT_ERROR_PORT_NOT_AVAILABLE;
   }

   /*
    * Store the printer handle
    */
   p->hPrinter = hPrinter;

   /*
    * Setup our port entry
    */
   p->HostStatus = LPT_SELECT | LPT_NOTBUSY;

   /*
    * Mark the port as open
    */
   p->Open = TRUE;

   TRACE(( TC_CPM, TT_API1, "LptSpoolOpen: Port %s opened successfully",p->Name ));

   return ( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 *  LptClose
 *
 * ENTRY:
 *    Port (input)
 *       Printer port identifier to close
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS : successfull
 *
 ******************************************************************************/

int WFCAPI
LptClose( int Port )
{
   PPORTENTRY p;
   BOOL       rc;
   int        Status;

   /*
    * Range check the port value
    */

   if( Port > MAX_PRINTER_PORTS ) {
       return CLIENT_ERROR_INVALID_HANDLE;
   }

   /*
    * See if this printer port is really open
    */

   if( Ports[Port].Open == FALSE ) {
       return CLIENT_ERROR_NOT_OPEN;
   }

   TRACE(( TC_CPM, TT_API1, "LptSpoolClose: close %s", Ports[Port].Name ));

   p = &Ports[Port];


   /*
    * Close the printer handle
    */
   close( p->hPrinter );

   /*
    * Mark it closed
    */
   p->hPrinter = 0;
   p->Open = FALSE;

   return( Status );
}

/*******************************************************************************
 *
 *  LptWriteBlock
 *
 * ENTRY:
 *    Port (input)
 *      Which port to write data to
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

int WFCAPI
LptWriteBlock( int Port, char *pBuf, int Count, int *pAmountWrote )
{
   BOOL rc;
   PPORTENTRY p;

   /*
    * Range check the port
    */

   if( Port > MAX_PRINTER_PORTS ) {
       return CLIENT_ERROR_INVALID_HANDLE;
   }

   /*
    * Make sure its open
    */

   if ( Ports[Port].Open == FALSE ) {
      return CLIENT_ERROR_NOT_OPEN;
   }

   p = &Ports[Port];

   TRACE(( TC_CPM, TT_API2, "LptSpoolWrite: Called for Port %d",Port));

   /*
    * Write the data to the printer
    */
   write(p->hPrinter, pBuf, Count);
   *pAmountWrote = (USHORT)Count;

#if 0
   if( !rc || (*pAmountWrote <= 0) ) {
       TRACE(( TC_CPM, TT_API1, "LptSpoolWrite: Error in WritePrinter %d, AmountWrote %d",GetLastError(),*pAmountWrote));
       return( CLIENT_ERROR_IO_ERROR );
   }
#endif
   
   return( CLIENT_STATUS_SUCCESS );
}

/*******************************************************************************
 *
 * LptStatus
 *
 *   This routine returns the current printer status byte.
 *
 *   The lower byte contains the defined status bytes from the BIOS, while
 *   the higher status byte contains software status such as buffer empty.
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

int WFCAPI
LptStatus( int Port, int *pStatus )
{

   /*
    * Range check the port
    */
   if( Port > MAX_PRINTER_PORTS ) {
       TRACE(( TC_CPM, TT_API1, "LptSpoolStatus: Port %d invalid",Port));
       return CLIENT_ERROR_INVALID_HANDLE;
   }

   /*
    * Make sure its open
    */
   if ( Ports[Port].Open == FALSE ) {
      TRACE(( TC_CPM, TT_API1, "LptSpoolStatus: Port %d not open",Port));
      return CLIENT_ERROR_NOT_OPEN;
   }

   *pStatus = Ports[Port].HostStatus;

   return( CLIENT_STATUS_SUCCESS );
}

#if 0
/*****************************************************************************
 *
 *  GetDefaultPrinter
 *
 *   Get the users default printer
 *
 * ENTRY:
 *   Param1 (input/output)
 *     Comments
 *
 * EXIT:
 *   STATUS_SUCCESS - no error
 *
 ****************************************************************************/

BOOL
GetDefaultPrinter(
    LPTSTR pPrinterName,
    DWORD  cbPrinterNameSize
    )
{
    DWORD Result;
    PCHAR ptr;

    Result = GetProfileString(
                 "windows",
		 "device",
		 "",     // default
                 pPrinterName,
		 cbPrinterNameSize
		 );

    if( Result >= 1 ) {

        //
	// The string returned is of the form
	//
	// "PrinterName,winspool,LPTx:"
	//
        ptr = strtok( pPrinterName, "," );
        if( ptr == NULL ) {
            return( FALSE );
	}

        TRACE(( TC_CPM, TT_API1, "GetDefaultPrinter: Found Printer Name %s",ptr));
	return( TRUE );
    }
    else {
        return( FALSE );
    }
}
#endif


