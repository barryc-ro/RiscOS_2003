
/*************************************************************************
*
* lptapi.h
*
* Parallel port library header
*
* Copyright 1994, Citrix Systems Inc.
*
*  Author: Brad Pedersen (4/4/94)
*
* $Log$
*  
*     Rev 1.11   15 Apr 1997 18:45:26   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.10   06 May 1996 18:39:40   jeffm
*  update
*  
*     Rev 1.9   18 Jul 1995 11:12:22   JohnR
*  update
*
*     Rev 1.8   10 Jul 1995 11:18:58   JohnR
*  update
*
*     Rev 1.7   02 May 1995 13:39:54   butchd
*  update
*
*************************************************************************/

#ifndef __LPTAPI_H__
#define __LPTAPI_H__

#ifdef old_code
#define LPT$OPEN        0
#define LPT$CLOSE       1
#define LPT$WRITE       2
#define LPT$STATUS      3
#define LPT$COUNT       4
#endif

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

// Software defined
#define LPT_QUEUE_EMPTY  0x0100    // No more buffered data in queue
#define LPT_QUEUE_FULL   0x0200    // No more buffered data can fit in queue

/*=============================================================================
==   Internal Routines
=============================================================================*/

#ifdef old_code
int WFCAPI LptLoad( PPLIBPROCEDURE );
int WFCAPI LptUnload( VOID );
#endif


/*=============================================================================
==   External Routines
=============================================================================*/

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs below
 */
int WFCAPI LptOpen( int Port );
int WFCAPI LptClose( int Port );
int WFCAPI LptWriteBlock( int Port, char *pBuf, int Count, int *pAmountWrote );
int WFCAPI LptStatus( int Port, int * Status );

#ifdef old_code
/*
 * These routines have been moved from the engine to
 * vdcpm. This block of code could be removed if LPT
 * support is not needed in the engine in the future.
 */

/*
 * Note: These function typedefs must be maintained in sync with the
 *       above function prototypes
 */
typedef int (PWFCAPI PLPTOPEN  )( int Port );
typedef int (PWFCAPI PLPTCLOSE )( int Port );
typedef int (PWFCAPI PLPTWRITE )( int Port, BYTE c );
typedef int (PWFCAPI PLPTSTATUS)( int Port, int * Status );

extern PPLIBPROCEDURE pLptProcedures;

#define LptOpen     ((PLPTOPEN  )(pLptProcedures[LPT$OPEN]))
#define LptClose    ((PLPTCLOSE )(pLptProcedures[LPT$CLOSE]))
#define LptWrite    ((PLPTWRITE )(pLptProcedures[LPT$WRITE]))
#define LptStatus   ((PLPTSTATUS)(pLptProcedures[LPT$STATUS]))

#endif

#endif //__LPTAPI_H__
