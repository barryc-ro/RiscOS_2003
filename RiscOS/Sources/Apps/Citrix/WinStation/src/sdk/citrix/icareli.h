
/***************************************************************************
*
*  ICARELI.H
*
*     This file contains definitions for the ICA 3.0 Reliable Protocol (pdreli)
*
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen (8/4/94)
*
*  $Log$
*  
*     Rev 1.11   21 Apr 1997 16:57:02   TOMA
*  update
*  
*     Rev 1.10   02 Feb 1996 18:11:04   bradp
*  update
*  
*     Rev 1.9   31 May 1995 18:11:02   bradp
*  update
*  
*     Rev 1.8   26 May 1995 18:05:18   bradp
*  update
*  
*     Rev 1.7   06 Dec 1994 17:57:36   bradp
*  update
*  
*  
****************************************************************************/

/*=============================================================================
==   Defines 
=============================================================================*/

#define MAXIMUM_RETRANSMIT_TIMEOUT  30000   // 30 seconds
#define DEFAULT_RETRANSMIT_TIMEOUT  500     // .5 second
#define ACK_TIMEOUT                 120000  // 2 minutes
#define DEFAULT_CONGESTION_WINDOW   1       // 1 outbuf
#define DEFAULT_RETRANSMIT_DELTA    100     // 1/10 second


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *   Frame Formats
 *
 *   DATA                    <flags> <seq> <data> <data> ...
 *   DATA (piggyback ack)    <flags> <ack seq> <seq> <data> <data> ...
 *   ACK                     <flags> <ack seq>
 *   NAK                     <flags> <nak seq>
 *
 */

/*
 *  ICA data frame header
 *  -- this header followed by additional data
 */
typedef struct _ICAHEADER {
    BYTE Flags;         // frame header flags
    BYTE Sequence;      // sequence number
} ICAHEADER, * PICAHEADER;

/*
 *  ICAHEADER 'Flags'
 */
#define ICA_NAK             0x01 // NAK frame
#define ICA_ACK             0x02 // ACK frame
#define ICA_DATA            0x04 // DATA frame
#define ICA_PIGGYACK        0x10 // data frame contains a piggybacked ACK 
#define ICA_RESET           0x20 // reset sequence numbers

