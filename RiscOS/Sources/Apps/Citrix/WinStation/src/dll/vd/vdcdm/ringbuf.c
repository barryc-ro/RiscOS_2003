/*************************************************************************
*
* ringbuf.c
*
* Routines to manipulate a ring buffer
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author:  Brad Pedersen 05/5/94
*
* $Log$
*  
*     Rev 1.9   09 Jul 1997 16:09:30   davidp
*  Added include for ica30.h because of Hydrix surgery
*  
*     Rev 1.8   15 Apr 1997 18:02:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.8   21 Mar 1997 16:09:10   bradp
*  update
*  
*     Rev 1.7   08 May 1996 14:08:24   jeffm
*  update
*  
*     Rev 1.6   11 Dec 1995 19:29:52   JohnR
*  Fixed bug in advancing of buffer pointer in ringbufwrite
*  
*
*     Rev 1.5   24 Jun 1995 18:31:44   richa
*
*     Rev 1.4   17 Apr 1995 12:45:52   marcb
*  update
*
*************************************************************************/

/*
 *  Includes
 */

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
//#include "cdmtrans.h"


/*=============================================================================
==   Functions Defined
=============================================================================*/

STATIC int RingBufCreate( USHORT );
STATIC void RingBufDelete( VOID );
STATIC int RingBufWrite( LPBYTE, USHORT );
STATIC int RingBufEmpty( VOID );
STATIC int RingBufReadAvail( PUSHORT );
STATIC int RingBufRead( VOID );


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Ring buffer header structure
 */
typedef struct _RINGBUF {
    LPBYTE pMem;                 // pointer to alllocate memory for buffer
    USHORT ByteCount;           // size of ring buffer in bytes
    USHORT Head;                // head offset (read from here)
    USHORT Tail;                // tail offset (write to here)
} RINGBUF, * PRINGBUF;


/*=============================================================================
==   Local defines
=============================================================================*/

#define ADVANCE(_off,_inc,_size) \
{                                \
    _off += _inc;                \
    if ( _off >= _size )         \
        _off -= _size;           \
}


/*=============================================================================
==   Local Data
=============================================================================*/

static RINGBUF Ring = {0};

extern PVOID pWd;
extern POUTBUFAPPENDPROC OutBufAppend;


/*******************************************************************************
 *
 *  RingBufCreate
 *
 *    Create a ring buffer of the specified size
 *
 * ENTRY:
 *    ByteCount (input)
 *       size of ring buffer in bytes
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
RingBufCreate( USHORT ByteCount )
{

    TRACE(( TC_CDM, TT_ICOOK, "RingBuf of %d bytes created", ByteCount ));

    /*
     *  Allocate memory for ring buffer
     */
    Ring.ByteCount = ByteCount + 1;
    if ( (Ring.pMem = malloc( Ring.ByteCount )) == NULL )
        return( CLIENT_ERROR_NO_MEMORY );

    /*
     *  Initialize head and tail pointer
     *    head==tail -> empty ring buffer
     */
    Ring.Head = 0;
    Ring.Tail = 0;

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  RingBufDelete
 *
 *    Delete ring buffer
 *
 * ENTRY:
 *    nothing
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

void STATIC 
RingBufDelete()
{
    /*
     *  Free ring buffer memory
     */
    free( Ring.pMem );

    memset( &Ring, 0, sizeof(RINGBUF) );
}


/*******************************************************************************
 *
 *  RingBufWrite
 *
 *    Write data to ring buffer advancing tail pointer
 *
 * ENTRY:
 *    pBuffer (input)
 *       pointer to data to write
 *    ByteCount (input)
 *       length of data in bytes
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int STATIC 
RingBufWrite( LPBYTE pBuffer, USHORT ByteCount )
{
    USHORT Count;
    USHORT TotalCount;

    /*
     *  Make sure data will fit in ring buffer
     */
    if ( Ring.Tail < Ring.Head ) {
        TRACE(( TC_CDM, TT_ICOOK, "CDM: RingBufWrap Tail %d, Head %d, Count %d", Ring.Tail, Ring.Head, Ring.ByteCount ));
        Count = Ring.Head - Ring.Tail;
        TotalCount = Count;
    } else {
        TRACE(( TC_CDM, TT_ICOOK, "CDM: RingBufNoWrap Tail %d, Head %d, Count %d", Ring.Tail, Ring.Head, Ring.ByteCount ));
        Count = Ring.ByteCount - Ring.Tail;
        TotalCount = Count + Ring.Head;
    }

    if ( ByteCount + 2 >= TotalCount ) {
        TRACE(( TC_CDM, TT_ICOOK, "CDM: RingBuf OverFlow ByteCount %d, TotalCount %d", ByteCount, TotalCount ));
        ASSERT( FALSE, 0 );
        return( CLIENT_ERROR_QUEUE_FULL );
    }

    /*
     *  Save byte count in ring buffer
     */
    if ( Count > 1 ) {

        *((PUSHORT)&Ring.pMem[Ring.Tail]) = ByteCount;
        ADVANCE( Ring.Tail, 2, Ring.ByteCount );

    } else {
        TRACE(( TC_CDM, TT_ICOOK, "1 byte at end! Tail %d, Head %d", Ring.Tail, Ring.Head ));
        Ring.pMem[Ring.Tail] = LSB(ByteCount);
        ADVANCE( Ring.Tail, 1, Ring.ByteCount );

        Ring.pMem[Ring.Tail] = MSB(ByteCount);
        ADVANCE( Ring.Tail, 1, Ring.ByteCount );
    }

    /*
     *  Write data to ring buffer
     */
    if ( Ring.Tail < Ring.Head )
        Count = Ring.Head - Ring.Tail;
    else
        Count = Ring.ByteCount - Ring.Tail;

    if ( ByteCount < Count )
        Count = ByteCount;

    memcpy( &Ring.pMem[Ring.Tail], pBuffer, Count );
    ADVANCE( Ring.Tail, Count, Ring.ByteCount );
    ByteCount -= Count;
    pBuffer += Count;

    /*
     *  Buffer wrapped - write remaining data
     */
    if ( ByteCount > 0 ) {
        TRACE(( TC_CDM, TT_ICOOK, "RingBuf WrapWrite ByteCount %d, Count %d, Tail %d, Head %d", ByteCount, Count, Ring.Tail, Ring.Head ));
        memcpy( &Ring.pMem[Ring.Tail], pBuffer, ByteCount );
        ADVANCE( Ring.Tail, ByteCount, Ring.ByteCount );
    }

    TRACE(( TC_CDM, TT_ICOOK, "RingBufWrite ByteCount %d, Tail %d, Head %d", ByteCount, Ring.Tail, Ring.Head ));

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  RingBufEmpty
 *
 *    Check if the ring buffer is empty
 *
 * ENTRY:
 *    nothing
 *
 * EXIT:
 *    TRUE - empty
 *    FALSE - not empty
 *
 ******************************************************************************/

int STATIC 
RingBufEmpty()
{
    return( (Ring.Head == Ring.Tail) ? TRUE : FALSE );
}


/*******************************************************************************
 *
 *  RingBufReadAvail
 *
 *    Check if any read data exists on ring buffer head
 *
 * ENTRY:
 *    pBytesAvail (output)
 *       address to return number of bytes available on next read
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - data exists
 *    CLIENT_STATUS_NO_DATA - no read data exists
 *
 ******************************************************************************/

int STATIC 
RingBufReadAvail( PUSHORT pBytesAvail )
{
    USHORT HeadSave;
    USHORT Count;

    /*
     *  Check if any read data is available
     */
    if ( Ring.Head == Ring.Tail ) {
        *pBytesAvail = 0;
        return( CLIENT_STATUS_NO_DATA );
    }

    if ( Ring.Head < Ring.Tail )
        Count = Ring.Tail - Ring.Head;
    else
        Count = Ring.ByteCount - Ring.Head;

    /*
     *  Return number of bytes available
     */
    if ( Count > 1 ) {

        *pBytesAvail = *((PUSHORT)&Ring.pMem[Ring.Head]);

    } else {

        HeadSave = Ring.Head;

        *((LPBYTE)pBytesAvail) = Ring.pMem[Ring.Head];      // lsb
        ADVANCE( Ring.Head, 1, Ring.ByteCount );
        *((LPBYTE)pBytesAvail+1) = Ring.pMem[Ring.Head];    // msb

        Ring.Head = HeadSave;
    }

    return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  RingBufRead
 *
 *    Read data from ring buffer advancing head pointer
 *    -- all data is appended to current output buffer
 *
 * ENTRY:
 *    nothing
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - data exists
 *    CLIENT_STATUS_NO_DATA - no read data exists
 *
 ******************************************************************************/

int STATIC 
RingBufRead()
{
    USHORT BytesAvail;
    USHORT Count;
    int rc;

    /*
     *  Check if any read data is available
     */
    if ( rc = RingBufReadAvail( &BytesAvail ) ) {
        return( rc );
    }

    /*
     *  Advance head past byte count
     */
    ADVANCE( Ring.Head, 2, Ring.ByteCount );

    /*
     *  Get read data from ring buffer head
     */
    if ( Ring.Head < Ring.Tail ) {
        Count = Ring.Tail - Ring.Head;
        TRACE(( TC_CDM, TT_ICOOK, "RingBufRead Head %d < Tail %d, BytesAvail %d, Count %d", Ring.Head, Ring.Tail, BytesAvail, Count ));
    }
    else {
        Count = Ring.ByteCount - Ring.Head;
        TRACE(( TC_CDM, TT_ICOOK, "RingBufRead Head %d >= Tail %d, BytesAvail %d, Count %d, ByteCount %d", Ring.Head, Ring.Tail, BytesAvail, Count, Ring.ByteCount ));
    }

    if ( BytesAvail < Count ) {
        Count = BytesAvail;
    }

    if ( rc = OutBufAppend( pWd, &Ring.pMem[Ring.Head], Count ) ) {
        TRACE(( TC_CDM, TT_ICOOK, "RingBufRead OutBufAppend Failed %d", rc));
        return( rc );
    }

    ADVANCE( Ring.Head, Count, Ring.ByteCount );
    BytesAvail -= Count;

    TRACE(( TC_CDM, TT_ICOOK, "RingBufRead Head %d, Tail %d, BytesAvail %d", Ring.Head, Ring.Tail, BytesAvail));

    /*
     *  Buffer wrapped - read remaining data
     */
    if ( BytesAvail > 0 ) {

	if ( rc = OutBufAppend( pWd, &Ring.pMem[Ring.Head], BytesAvail ) ) {
            TRACE(( TC_CDM, TT_ICOOK, "OutBufAppend Failed! rc %d", rc));
            return( rc );
	}

        ADVANCE( Ring.Head, BytesAvail, Ring.ByteCount );
    }

    return( CLIENT_STATUS_SUCCESS );
}

