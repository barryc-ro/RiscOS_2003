
/***************************************************************************
*
*  PDRELI.H
*
*  This module contains private PD defines and structures 
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* $Author$  Brad Pedersen
*
* $Log$
*  
*     Rev 1.9   15 Apr 1997 16:52:46   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.8   27 Sep 1995 13:46:42   bradp
*  update
*  
*     Rev 1.7   08 Mar 1995 16:42:12   bradp
*  update
*  
*     Rev 1.6   19 Oct 1994 14:29:48   bradp
*  update
*  
*     Rev 1.5   17 Sep 1994 15:47:24   bradp
*  update
*  
*     Rev 1.4   01 Sep 1994 17:31:14   bradp
*  update
*  
*     Rev 1.3   25 Aug 1994 11:05:24   bradp
*  update
*  
*  
****************************************************************************/


/*=============================================================================
==   Structures
=============================================================================*/

/*
 *  Error correction PD structure
 */
typedef struct _PDRELI {

    /* ini file parameters */
    ULONG MaxRetryTime;         // maximum time to attempt retries (msec)
    ULONG RetransmitTimeDelta;  // increment timeout by this value (msec)
    ULONG DelayedAckTime;       // delayed ack timeout (msec)

    BYTE TransmitSequence;      // transmit sequence number
    BYTE ReceiveExpectedSeq;    // next expected receive sequence number
    BYTE LastAckSequence;       // last received ack sequence number
    BYTE LastNakSequence;       // last received nak sequence number
                                
    LONG  RoundTripTime;        // smoothed round trip time
    LONG  RoundTripDeviation;   // round trip deviation estimator
    ULONG RetransmitTimeout;    // retransmition timeout (msec)
    ULONG TotalRetransmitTime;  // accumulative time of timeouts (msec)
    ULONG StartRecvRetryTime;   // starting time of receive retries (msec)
    ULONG ErrorFeedbackTime;    // time to return error retry status (msec)

    int CongestionWindow;       // number of outbufs that can be transmitted
    int CongestionCount;        // .. used to adjust CongestionWindow
    int SlowStartThreshold;     // threshold for slow start algorithm
    int OutBufAckWaitCount;     // number of outbufs waiting for ack
                                
    POUTBUF pOutBufHead;        // head of outbufs waiting for ack queue
    ULONG TimerRetransmit;      // timer - retransmit 
    ULONG TimerNak;             // timer - nak 
    ULONG TimerAck;             // timer - delayed ack 

    BUSHORT fNakRetransmit : 1;  // retransmit is due to a nak (not a timeout)
    BUSHORT fAckNow : 1;         // disable delayed acks
    BUSHORT fNakSent : 1;        // nak was sent flag (only send one nak)

} PDRELI, * PPDRELI;

/* SJM: rename local functions to prevent clashes */

#define DeviceOutBufAlloc	PdReliDeviceOutBufAlloc
#define DeviceOutBufError	PdReliDeviceOutBufError
#define DeviceOutBufFree	PdReliDeviceOutBufFree
#define DeviceSetInfo		PdReliDeviceSetInfo
#define DeviceQueryInfo		PdReliDeviceQueryInfo

#define DeviceProcessInput	PdReliDeviceProcessInput

