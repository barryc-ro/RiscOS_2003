
/*************************************************************************
*
* Printer.h
*
* Header file for printer functions for client printer mapping
*
* copyright notice: Copyright 1995, Citrix Systems Inc.
*
* Author:  John Richardson 09/19/95
*
* $Log$
*  
*     Rev 1.1   15 Apr 1997 18:05:16   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   13 Oct 1995 17:32:48   JohnR
*  Initial revision.
*
*
*
*************************************************************************/

/*
 * Maximum number of printer we support open at one time
 */
#define MAX_PRINTERS 10

/*
 * Define Printer status bits masks returned from STATUS
 *
 * The low order byte replicates the BIOS INT 17H, AH=02H Status bits
 *
 * The high order byte is software defined by the LPT support library
 *
 */

#define LPT_TIMEOUT      0x0001
#define LPT_RESERVED1    0x0002
#define LPT_RESERVED2    0x0004
#define LPT_IOERROR      0x0008
#define LPT_SELECT       0x0010
#define LPT_OUTOFPAPER   0x0020
#define LPT_ACK          0x0040
#define LPT_NOTBUSY      0x0080

USHORT
CpmEnumPrinter(
    USHORT    Index,
    PCHAR     pBuf,    // return buffer is in CPMWIRE format for ENUMPRINTER reply
    USHORT    MaxBytes,
    PUSHORT   pCount,
    PUSHORT   pReturnSize
    );

USHORT
CpmOpenPrinter(
    PCHAR pPathNameBuf,
    int   *pHandle
    );

USHORT
CpmClosePrinter(
    int hPrinter
    );

USHORT
CpmWritePrinter(
    int     hPrinter,
    PCHAR   pBuf,
    USHORT  Length,
    PUSHORT pAmountWrote
    );

USHORT
CpmPollPrinter(
    int    hPrinter,
    PULONG pStatus,
    int    *pUsefull
    );


