/*************************************************************************
*
*   reduce.c
*
*   ICA 3.0 WinStation driver
*
*   Copyright 1998, Citrix Systems Inc.
*
*   Author: Henry Collins
*
*************************************************************************/

#include "windows.h"

#include <stdio.h>
#ifdef i386
#include <io.h>
#include <fcntl.h>
#endif

#include "../../inc/client.h"
#include "../../inc/wdapi.h"
#include "../wd/inc/wd.h"

#include "../../inc/reducapi.h"

/* Algorithms: 1 = HashTable, 2 = Direct, 3 = Perverse v1, 4 = Perverse v2 */

#define ALGORITHM   4

#define CRC_INIT_VALUE  0xFFFFFFFF


/* Format of compressed data is a 2-byte (little-endian) header:
       if top bit of header is set 
       then 
            it is the inverse of the number of bytes of data, 
            and the data is not compressed,
       else 
            it is the number of items in the message:
            Each item starts with a single bit:

           0 = not a string, followed by 
               8 bits = value for the byte
        or
           1 = is a string. followed by
               encoding of string length
               encoding of distance  of string from head of queue

               length encoding
               ---------------
               2 bits for string length
               0 = string length 2
               1 = string length 3
               2 = string length 4
               3 = string length > 4
               in the last case this is followed by a 3 byte field
               0-6 = string lengths 5-11
               7 = string length > 11
               in the last case this is followed by a 5 byte field
               0-30 = string lengths 12-42
               31 = string length > 42
               in the last case this is followed by an 8 byte field
               0-254 = string lengths 43-297
               255 = string length > 297
               in the last case this is followed by a 13 byte field
               containing the 13-bit string length

               distance encoding
               ------------------

               if string length == 2
                   3 bits for nr of bits in distance, and
                   n bits = distance excluding top bit
               else
                   distance is biased by a distanceAdjustment (depending on buffer size)
                   4 bits for nr of bits in distance, and
                   n bits = distance excluding top bit
*/          


#if (ALGORITHM == 3)

#define CACHE_BITS   15
#define CACHE_SIZE   ((ULONG)1 << CACHE_BITS)
#define CACHE_MASK   (CACHE_SIZE - 1)

#define BUCKET_BITS   6
#define BUCKET_SIZE   ((ULONG)1 << BUCKET_BITS)
#define BUCKET_MASK   (BUCKET_SIZE - 1)

#define NR_OF_BUCKETS_BITS   (CACHE_BITS - BUCKET_BITS)
#define NR_OF_BUCKETS        ((ULONG)1 << NR_OF_BUCKETS_BITS)
#define NR_OF_BUCKETS_MASK   (NR_OF_BUCKETS - 1)

#define TAG_BITS   (8 + 8 - NR_OF_BUCKETS_BITS)
#define TAG_SIZE   ((ULONG)1 << TAG_BITS)
#define TAG_MASK   (TAG_SIZE - 1)

#define SKEW_BITS   (8 - TAG_BITS)

#define TAG_OF(x)   ((x) >> SKEW_BITS)   

#define SEQ_SHIFT_BITS  (8 + TAG_BITS)

#define KEY_MASK    (((ULONG)1 << SEQ_SHIFT_BITS) - 1)

#define SAFETY_MARGIN   10 /* enough to avoid all boundary conditions */

#define MAX_SAFE_DISTANCE   (CACHE_SIZE - SAFETY_MARGIN)

#endif /* ALGORITHM 3 */


#if (ALGORITHM == 4)

#define NR_OF_BUCKETS_BITS   4
#define NR_OF_BUCKETS        ((ULONG)1 << NR_OF_BUCKETS_BITS)
#define NR_OF_BUCKETS_MASK   (NR_OF_BUCKETS - 1)

#define TAG_BITS   (8 + 8 + 8 - NR_OF_BUCKETS_BITS)
#define TAG_SIZE   ((ULONG)1 << TAG_BITS)
#define TAG_MASK   (TAG_SIZE - 1)

#define SKEW_BITS   11
#define SKEW_SIZE   ((ULONG)1 << SKEW_BITS)
#define SKEW_MASK   (SKEW_SIZE - 1)

#define SAFETY_MARGIN   10 /* enough to avoid all boundary conditions */

#define MAX_SAFE_DISTANCE   (cacheSize - SAFETY_MARGIN)

typedef struct _SLOT {
    ULONG       seqInfo; 
    ULONG       tagInfo; 
} SLOT;

#endif /* ALGORITHM 4 */

#if (ALGORITHM == 1)

const ULONG CrcTable[256] = 
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#endif /* ALGORITHM 1 */


/*************************************************************************
*
*   Reducer/ExpanderDataQueueMemSize
*
*   Returns the actual size of buffer needed for reduction or expansion
*   (as opposed to the nominal (1 << power of 2))
*   This is because in the case of reduction it also includes lookup info
*
*   NOTE: Zero is returned if the algorithm will not support the requested size
*
*************************************************************************/

ULONG WFCAPI ReducerDataQueueMemSize(
    ULONG receiverPowerOf2,        /* receiver history buffer size */
    ULONG circularBufferPowerOf2)  /* only relevant for ALG 3 */
{
#if (ALGORITHM == 1)
    return (((ULONG)1 << receiverPowerOf2) +       /* history buffer */
            ((ULONG)1 << (receiverPowerOf2 + 1))); /* lookup table */
#endif

#if (ALGORITHM == 2)
    return (
        ((ULONG)1 << receiverPowerOf2) +           /* history buffer memory (bytes) */
        ((ULONG)1 << receiverPowerOf2) +           /* hash4 table memory (ULONGS) */
        ((ULONG)1 << (receiverPowerOf2 - 1)));     /* hash2 table memory (ULONGS) */
#endif

#if (ALGORITHM == 3)
    if (receiverPowerOf2 != CACHE_BITS)
    {
        return(0); /* no can do */
    }
    else
    {
        return (
        ((ULONG)1 << circularBufferPowerOf2) + /* circular buffer */
        (32) +                          /* state variables */
        (NR_OF_BUCKETS * 32) +          /* navigation data */
        (CACHE_SIZE * 4));              /* cache slots */
    }
#endif

#if (ALGORITHM == 4)
    if ((receiverPowerOf2 < 10) || (receiverPowerOf2 > 15))
    {
        return(0); /* no can do */
    }
    else
    {
        return (        
        ((ULONG)1 << circularBufferPowerOf2) +     /* circular buffer */
        (32) +                              /* state variables */
        (((ULONG)1 << receiverPowerOf2) * 8) +     /* the cache slots */
        (2 * NR_OF_BUCKETS));               /* nextFreeSloNrs array */
    }
#endif

}


ULONG WFCAPI ExpanderDataQueueMemSize(ULONG receiverPowerOf2)
{
    return (
        ((ULONG)1 << receiverPowerOf2));           /* history buffer memory (bytes) */
}


/*************************************************************************
*
*   InitDataQueueVariables
*
*   Initialises the data queue structure
*
*************************************************************************/

void WFCAPI InitReducerDataQueueVariables(
    PDATA_QUEUE dqp,               /* basic data queue control info */
    ULONG receiverPowerOf2,        /* receiver history buffer size */
    ULONG circularBufferPowerOf2,  /* only relevant for ALG 3 */
    LPUCHAR     buffer,            /* the memory */
    ULONG       maxNewData)        /* the batch limit */
{

#if ((ALGORITHM == 3) || (ALGORITHM == 4))
    maxNewData = (ULONG)1 << (circularBufferPowerOf2 - 1);
#endif

    dqp->Buffer = buffer;
    dqp->MaxNewData = maxNewData;

#if ((ALGORITHM == 1) || (ALGORITHM == 2))
    dqp->BufferLen = ((ULONG)1 << receiverPowerOf2);
    dqp->BufferMask = ((ULONG)1 << receiverPowerOf2) - 1;
#endif
    
#if ((ALGORITHM == 3) || (ALGORITHM == 4))
    dqp->BufferLen = ((ULONG)1 << circularBufferPowerOf2);
    dqp->BufferMask = ((ULONG)1 << circularBufferPowerOf2) - 1;
#endif
 
#if (ALGORITHM == 4)
    {
        PUCHAR bytePtr;
    
        bytePtr = buffer + ((ULONG)1 << circularBufferPowerOf2);
        *(ULONG*)(bytePtr) = receiverPowerOf2; /* cacheBits */
        *(ULONG*)(bytePtr + 4) = ((ULONG)1 << receiverPowerOf2); /* currentSequenceNr */
        *(ULONG*)(bytePtr + 8) = 0; /* prevByte */
        *(ULONG*)(bytePtr + 12) = 0; /* prevPrevByte */
        *(SLOT**)(bytePtr + 16) = (SLOT*)(bytePtr + 20); /* dummy location */
    }
#endif
        
    if (receiverPowerOf2 <= 16)
    {
        dqp->DistanceAdjustment = -1;
        dqp->MinimumDistanceBits = 1;
    }
    else
    {
        dqp->DistanceAdjustment = ((ULONG)1 << (receiverPowerOf2 - 16)) - 2;
        dqp->MinimumDistanceBits = receiverPowerOf2 - 15;
    };

    dqp->WriteBase = 0;
    dqp->WriteReached = 0;
    dqp->WriteLimit = maxNewData;
}


void WFCAPI InitExpanderDataQueueVariables(
    PDATA_QUEUE dqp,        /* basic data queue control info */
    ULONG receiverPowerOf2, /* receiver history buffer size */
    LPUCHAR     buffer,     /* the memory */
    ULONG       maxNewData) /* the batch limit */
{
   
    dqp->Buffer = buffer;
    dqp->MaxNewData = maxNewData;
    dqp->BufferLen = ((ULONG)1 << receiverPowerOf2);
    dqp->BufferMask = ((ULONG)1 << receiverPowerOf2) - 1;
    if (receiverPowerOf2 <= 16)
    {
        dqp->DistanceAdjustment = -1;
        dqp->MinimumDistanceBits = 1;
    }
    else
    {
        dqp->DistanceAdjustment = ((ULONG)1 << (receiverPowerOf2 - 16)) - 2;
        dqp->MinimumDistanceBits = receiverPowerOf2 - 15;
    };

    dqp->WriteBase = 0;
    dqp->WriteReached = 0;
    dqp->WriteLimit = maxNewData;
}



/*************************************************************************
*
*   ConvertDataQueueIntoBuffer
*
*   The reducer function
*
*   Consumes buffered data from the queue and packs it into an outbuf
*   Only takes enough to fill the outbuf and updates outbuf byte count
*   Does not update any queue indices
*
*   Returns the number of bytes consumed
*
*************************************************************************/

#define ADD_BITS(n, v) {pendingBits |= ((v) << nrOfPendingBits); nrOfPendingBits += (n);}
#define FLUSH_BITS() {while (nrOfPendingBits >= 8){*outPtr++ = (UCHAR)pendingBits; pendingBits >>= 8; nrOfPendingBits -= 8;}}

#if ((ALGORITHM == 1) || (ALGORITHM == 2))

#if (ALGORITHM == 1)
#define CRC_BYTE(a,b) (a = CrcTable[(ULONG)((((b))) ^ (UCHAR)(a))] ^ ((a)>>8))
#define stringSkipFactor    4  /* this is a CPU/reduction tradeoff factor */
#endif

#if (ALGORITHM == 2)
#define stringSkipFactor    1  /* this is a CPU/reduction tradeoff factor */
#endif

/* algorithms 1 and 2 */
/* ------------------ */

ULONG WFCAPI ConvertDataQueueIntoBuffer(
    PDATA_QUEUE dqp,                /* basic data queue control info */
    OUTBUF FAR * pOutBuf,           /* output buffer */
    ULONG       tailIndex,          /* base of data to read */
    ULONG       headIndex,          /* end of data region to read */
    ULONG       limitIndex          /* safety region limit */
)
{
    ULONG historyBufferMask;
    LPUCHAR outPtr, endOutPtr, historyBuffer;
    ULONG v, stringLength, nrOfBits, bufferLen, outputBufferLength;
    ULONG hash, length, nrOfItems;
    ULONG nrOfPendingBits, pendingBits;
    ULONG index, distance, maxSafeDistance, inputLength;
    ULONG trailingIndex, originalTailIndex, matchIndex;
    ULONG nrOfBytes, firstPortion;
    LPUCHAR outputBuffer;
    LONG  distanceAdjustment;
    ULONG minimumDistanceBits;

#if (ALGORITHM == 1)
    ULONG *hash2Table, hash2Mask, *hash4Table, hash4Mask;
    ULONG hash2Crc, hash4Crc, *hash2Ptr, *hash4Ptr, crc;
#endif

#if (ALGORITHM == 2)
    USHORT *lookupTable;
    ULONG lookupIndex;
#endif

 
    outputBuffer = pOutBuf->pBuffer;
    outputBufferLength = pOutBuf->MaxByteCount;
    originalTailIndex = tailIndex;

    historyBuffer = dqp->Buffer;
    bufferLen = dqp->BufferLen;
    historyBufferMask = bufferLen - 1;

#if (ALGORITHM == 1)
    hash2Table = (ULONG*)(historyBuffer + bufferLen); 
    hash4Table = (ULONG*)((PUCHAR)hash2Table + (bufferLen >> 1));
    hash4Mask = historyBufferMask >> 2;
    hash2Mask = historyBufferMask >> 3;
#endif

#if (ALGORITHM == 2)
    lookupTable = (USHORT*)(historyBuffer + bufferLen); 
#endif

    distanceAdjustment = dqp->DistanceAdjustment;
    minimumDistanceBits = dqp->MinimumDistanceBits;


#if (ALGORITHM == 1)
    /* redo hashes from tail end of last conversion */
    /* -------------------------------------------- */
    
    trailingIndex = tailIndex - 4;
    while (trailingIndex != tailIndex)
    {
        index = trailingIndex & historyBufferMask;
        crc = CRC_INIT_VALUE;
  	        
        CRC_BYTE(crc, historyBuffer[index]);
  
        CRC_BYTE(crc, historyBuffer[index + 1]);
        hash = crc & hash2Mask;
        hash2Table[hash] = trailingIndex ^ crc;

        CRC_BYTE(crc, historyBuffer[index + 2]);
    
        CRC_BYTE(crc, historyBuffer[index + 3]);
        hash = crc & hash4Mask;
        hash4Table[hash] = trailingIndex ^ crc;

        trailingIndex++;
    }     
#endif

            
    /* leave space for 2-byte header to encode nr of items */
    outPtr = outputBuffer + 2;
    outputBufferLength -= 2;

    /* leave paranoid 8 bytes margin at end for final item to flush itself */
    endOutPtr = outPtr + outputBufferLength - 8;

    /* calculate max safe distance back from front of queue */
    maxSafeDistance = (tailIndex - limitIndex) & historyBufferMask; 

    nrOfPendingBits = 0;
    pendingBits = 0;
    nrOfItems = 0;

    while ((tailIndex != headIndex) && (outPtr < endOutPtr))
    {
        nrOfItems++;
 
#if (ALGORITHM == 1)

        /* calculate incoming CRCs */
        /* ----------------------- */
  
        index = tailIndex & historyBufferMask;
        crc = CRC_INIT_VALUE;
    
        CRC_BYTE(crc, historyBuffer[index]);
  
        /* read may overrun into hash2Table area but no problem */
        CRC_BYTE(crc, historyBuffer[index + 1]);
        hash2Ptr = &hash2Table[crc & hash2Mask];
        hash2Crc = crc;       
   
        CRC_BYTE(crc, historyBuffer[index + 2]);
    
        CRC_BYTE(crc, historyBuffer[index + 3]);
        hash4Ptr = &hash4Table[crc & hash4Mask];       
        hash4Crc = crc;       

 
        /* look for matches */
        /* ---------------- */

        matchIndex = *hash4Ptr ^ hash4Crc;
        *hash4Ptr = tailIndex ^ hash4Crc;
        distance = tailIndex - matchIndex;
        if (distance < maxSafeDistance && distance > 1)
        {
            /* very very high probability of success */
            *hash2Ptr = tailIndex ^ hash2Crc;
            goto stringProbablyFound;
        }    
        
        matchIndex = *hash2Ptr ^ hash2Crc;
        *hash2Ptr = tailIndex ^ hash2Crc;
        distance = tailIndex - matchIndex;

#endif

#if (ALGORITHM == 2)

        /* lookup incoming string */
        /* ---------------------- */

        index = tailIndex & historyBufferMask;
        lookupIndex = (historyBuffer[index] + ((ULONG)(historyBuffer[index+1]) << 8)) & historyBufferMask;
        matchIndex = lookupTable[lookupIndex];
        lookupTable[lookupIndex] = (USHORT)tailIndex;
        distance = (tailIndex - matchIndex) & historyBufferMask;
        
#endif

        if (distance < maxSafeDistance && distance > 0)
        {  
            /* very high probability of success */
            if (distance == 1)
            {
                /* possible case of repeated characters not visible far */
                /* enough back due to premature hash table updating */

                matchIndex--;
                distance++;
            }
            goto stringProbablyFound;
        }   

 
        /* string not found */
        /* ---------------- */
 		
stringNotFound:
	
        v = historyBuffer[tailIndex & historyBufferMask];
		
        ADD_BITS(9, v << 1);
        FLUSH_BITS();	
        tailIndex++;
        continue;


        /* string probably found */
        /* --------------------- */
		           
stringProbablyFound:

        /* work out length of string */
        index = tailIndex & historyBufferMask;
        matchIndex &= historyBufferMask;
        inputLength = headIndex - tailIndex;
        if (inputLength > 0x1FFF)
        {
            inputLength = 0x1FFF; /* maximum supported string length */
        }
        if (((index | matchIndex) + inputLength) <= bufferLen)
        {
            PUCHAR mPtr, tPtr, endPtr;

            /* no possible wrap problem */
            mPtr = &historyBuffer[matchIndex];
            tPtr = &historyBuffer[index];
            endPtr = tPtr + inputLength;
            while (tPtr < endPtr)
            {
                if (*mPtr != *tPtr)
                {
                    break;
                }
                tPtr++;
                mPtr++;
            }
            stringLength = tPtr - endPtr + inputLength;
        }   
        else
        {
            /* possible wrap do carefully */
            stringLength = 0;
            index = tailIndex;
            while ((historyBuffer[index & historyBufferMask] == 
                historyBuffer[matchIndex & historyBufferMask]) &&
                (stringLength < inputLength))
            {
                index++;
                matchIndex++;
                stringLength++;
            }
        }

        if (stringLength <= 2)
        {
            if ((distance > 511) || (stringLength < 2))
            {
                goto stringNotFound;
            }

            /* Work out distance - encode length and distance */
            /* ---------------------------------------------- */

            v = distance >> 2;  /* 2 is minimum distance here */
            nrOfBits = 2;       /* minimum bits so far */
            while (v >= 4)
            {
                v >>= 3;
                nrOfBits += 3;
            }
            while (v > 0)
            {
                v >>= 1;
                nrOfBits += 1;
            }

            ADD_BITS((1 + 2 + 3), 0x1 + ((nrOfBits - 2) << 3));
            distance ^= (1 << (nrOfBits - 1)); /* clear top bit */
            ADD_BITS(nrOfBits - 1, distance);
            FLUSH_BITS();
        }
        else
        {
            length = stringLength - 2;
            distance += distanceAdjustment;

            /* encode string length */
            /* -------------------- */
            
            if (length < 3)
            {
                ADD_BITS(3, 1 | (length << 1));
            }
            else
            {
                length -= 3;
                if (length < 7)
                {
                    ADD_BITS(6, 0x7 | (length << 3));
                }
                else
                {
                    length -= 7;
                    if (length < 31)
                    {
                        ADD_BITS(11, 0x3f | (length << 6));
                    }
                    else
                    {
                        length -= 31;
                        if (length < 255)
                        {
                            ADD_BITS(19, 0x7ff | (length << 11));
                        }
                        else
                        {
                            ADD_BITS(19, 0x7ffff);
                            FLUSH_BITS();
                            ADD_BITS(13, stringLength); 
                        }
                    }
                }
            }
            FLUSH_BITS();


            /* encode distance */
            /* --------------- */
	
            v = distance >> minimumDistanceBits;
            nrOfBits = minimumDistanceBits; /* minimum possible so far */
            while (v >= 8)
            {
                v >>= 4;
                nrOfBits += 4;
            }
            while (v > 0)
            {
                v >>= 1;
                nrOfBits += 1;
            }
            ADD_BITS(4, nrOfBits - minimumDistanceBits);
 
            distance ^= ((ULONG)1 << (nrOfBits - 1)); /* clear top bit */
            ADD_BITS(nrOfBits - 1, distance);
	    
            FLUSH_BITS();
        }

#if (ALGORITHM == 1)

        /* update hash tables within string */
        /* -------------------------------- */

        trailingIndex = tailIndex;
        tailIndex += stringLength;
        length = stringLength;
       	while (length > stringSkipFactor)
        {
            /* do sparse hash table updates in middle of string */
            /* ------------------------------------------------ */
            
            trailingIndex += stringSkipFactor;
            length -= stringSkipFactor;
            index = trailingIndex & historyBufferMask;
            crc = CRC_INIT_VALUE;
  	        
            CRC_BYTE(crc, historyBuffer[index]);
  
            CRC_BYTE(crc, historyBuffer[index + 1]);
            hash = crc & hash2Mask;
            hash2Table[hash] = trailingIndex ^ crc;
        }           

#endif

#if (ALGORITHM == 2)

        /* update lookup table entries within string */
        /* ----------------------------------------- */

        trailingIndex = tailIndex;
        tailIndex += stringLength;
        length = stringLength;
       	while (length > stringSkipFactor)
        {
            /* do sparse table updates in middle of string */
            /* ------------------------------------------- */
            
            trailingIndex += stringSkipFactor;
            length -= stringSkipFactor;
            index = trailingIndex & historyBufferMask;
            lookupIndex = (historyBuffer[index] + ((ULONG)(historyBuffer[index+1]) << 8)) & historyBufferMask;
            lookupTable[lookupIndex] = (USHORT)trailingIndex;
        }                    
        
#endif
    
    }  
    
    if (nrOfPendingBits != 0)
    {
         *outPtr++ = (UCHAR)pendingBits;
    }

    /* finish off buffer */
    /* ----------------- */
 
    /* work out how many bytes we should have been able to send raw */
    nrOfBytes = outputBufferLength;
    if ((ULONG)(headIndex - originalTailIndex) < nrOfBytes)
    {
        nrOfBytes = (headIndex - originalTailIndex);
    }

    if (((ULONG)(tailIndex - originalTailIndex) < nrOfBytes) ||
    ((ULONG)(outPtr - outputBuffer - 2) > nrOfBytes))
    {
        /* raw would have been better - so revert to raw data */ 

        /* header = inverse of nr of bytes */
        v = nrOfBytes ^ 0xFFFF;
        *(outputBuffer) = (UCHAR)v;
        *(outputBuffer + 1) = (UCHAR)(v >> 8);

        outPtr = outputBuffer + 2;
        index = originalTailIndex & historyBufferMask;
        if ((index + nrOfBytes) <= (historyBufferMask + 1))
        {
            /* contiguous memory copy */
            memcpy(outPtr, &(historyBuffer[index]), (ULONG)nrOfBytes);
        }
        else
        {
            firstPortion = (historyBufferMask - index + 1);
            memcpy(outPtr, &(historyBuffer[index]), (ULONG)firstPortion);
            memcpy(outPtr + firstPortion, &(historyBuffer[0]), (ULONG)(nrOfBytes - firstPortion));
        }

        outPtr += nrOfBytes;
        tailIndex = originalTailIndex + nrOfBytes;
    }
    else
    {
        /* we have reduced it - send reduced data */

        /* header = number of items */
        *(outputBuffer) = (UCHAR)nrOfItems;
        *(outputBuffer + 1) = (UCHAR)(nrOfItems >> 8);
    }

    pOutBuf->ByteCount = outPtr - outputBuffer; /* nr of bytes output */
    return (tailIndex - originalTailIndex);     /* nr of bytes consumed */
}

#endif /* ALGORITHMS 1 and 2 */


#if ((ALGORITHM == 3) || (ALGORITHM == 4))

#if (ALGORITHM == 3)

#define AddSlot(base, tg, next) {\
    ULONG  hash, group, index;\
    LPULONG longPtr; \
    LPUCHAR bytePtr;\
    ULONG v0, v1, v2;\
    bytePtr = &lookup[(base) >> 1];\
    index = (currentSequenceNr & (BUCKET_MASK - 3));\
    group = (base) + index;\
    hash = (tg) ^ (next);\
    hash = (hash ^ (hash >> 3)) & 0x1F;\
    *(bytePtr + hash) = index;\
    longPtr = &cache[group];\
    v0 = *longPtr;\
    v1 = *(longPtr + 1);\
    v2 = *(longPtr + 2);\
    *longPtr = (tg) | ((next) << TAG_BITS) | ((currentSequenceNr) << (SEQ_SHIFT_BITS));\
    *(longPtr + 1) = v0;\
    *(longPtr + 2) = v1;\
    *(longPtr + 3) = v2;\
    currentSequenceNr++;}

#endif


#if (ALGORITHM == 4)

#define TAG(t) ((t) & TAG_MASK)

#define LOOKUP_NR(t) (((t) ^ ((t) >> SKEW_BITS)) & bucketMask)

#define BUCKET_NR(x) (((x) ^ ((x) >> NR_OF_BUCKETS_BITS)) & NR_OF_BUCKETS_MASK)

#define AddSlot(b1, b2, b3) {\
    ULONG triplet, bx, bucketNr, tag, slotNr, lookupNr, prevTagInfo;\
    SLOT FAR *slotPtr, FAR *baseSlotPtr;\
    bx = (b1) ^ (b2) ^ (b3);\
    triplet = (b1) | ((b2) << 8) | ((b3) << 16);\
    bucketNr = BUCKET_NR(bx);\
    tag = TAG(triplet);\
    slotNr = nextFreeSlotNrs[bucketNr];\
    nextFreeSlotNrs[bucketNr] = (USHORT)((slotNr + 1) & bucketMask);\
    baseSlotPtr = cache + (bucketNr << bucketBits);\
    slotPtr = baseSlotPtr + slotNr;\
    prevTagInfo = slotPtr->tagInfo;\
    slotPtr->seqInfo = (currentSequenceNr << SKEW_BITS);\
    prevTagInfo &= SKEW_MASK;\
    slotPtr->tagInfo = prevTagInfo | (tag << SKEW_BITS);\
    prevSlotPtr->seqInfo |= slotNr;\
    prevSlotPtr = slotPtr;\
    lookupNr = LOOKUP_NR(tag);\
    slotPtr = baseSlotPtr + lookupNr;\
    prevTagInfo = slotPtr->tagInfo;\
    prevTagInfo &= (~SKEW_MASK);\
    slotPtr->tagInfo = prevTagInfo | slotNr;\
    currentSequenceNr++;}

#endif


/* algorithms 3 amd 4 */
/* ------------------ */

ULONG WFCAPI ConvertDataQueueIntoBuffer(
    PDATA_QUEUE dqp,                /* basic data queue control info */
    OUTBUF FAR *pOutBuf,            /* output buffer */
    ULONG       tailIndex,          /* base of data to read */
    ULONG       headIndex,          /* end of data region to read */
    ULONG       limitIndex          /* safety region limit */
)
{
    ULONG bufferMask;
    LPUCHAR outPtr, endOutPtr, buffer;
    ULONG v, stringLength, nrOfBits, bufferLen, outputBufferLength;
    ULONG length, nrOfItems;
    ULONG nrOfPendingBits, pendingBits;
    ULONG index, distance;
    ULONG originalTailIndex;
    ULONG nrOfBytes, firstPortion;
    LPUCHAR outputBuffer;
    LONG  distanceAdjustment;
    ULONG minimumDistanceBits;
    
    LPUCHAR bytePtr;
    ULONG slotNr, sequenceNr;
    ULONG firstByte, secondByte, thirdByte, matchByte, tag;
    ULONG currentSequenceNr;

#if (ALGORITHM == 3)

    ULONG hash, baseSlotNr, key, slotValue, group;
    LPULONG cache, longPtr;
    LPUCHAR lookup;
    ULONG prevBaseSlotNr;
    ULONG prevByte; 
    ULONG prevTag;
 
#endif

#if (ALGORITHM == 4)

    SLOT   FAR *cache;
    USHORT FAR *nextFreeSlotNrs;
    ULONG  prevByte;
    ULONG  prevPrevByte;
    SLOT   FAR *prevSlotPtr;

    ULONG triplet, bucketNr, seqInfo, tagInfo, lookupNr, bx;
    SLOT FAR *slotPtr, FAR *baseSlotPtr;

    ULONG cacheBits;
    ULONG cacheSize;
    ULONG bucketBits;
    ULONG bucketSize;
    ULONG bucketMask;

#endif

    outputBuffer = pOutBuf->pBuffer;
    outputBufferLength = pOutBuf->MaxByteCount;
    originalTailIndex = tailIndex;

    buffer = dqp->Buffer;
    bufferLen = dqp->BufferLen;
    bufferMask = bufferLen - 1;

    distanceAdjustment = dqp->DistanceAdjustment;
    minimumDistanceBits = dqp->MinimumDistanceBits;
            
    /* leave space for 2-byte header to encode nr of items */
    outPtr = outputBuffer + 2;
    outputBufferLength -= 2;

    /* leave paranoid 8 bytes margin at end for final item to flush itself */
    endOutPtr = outPtr + outputBufferLength - 8;

    nrOfPendingBits = 0;
    pendingBits = 0;
    nrOfItems = 0;


 
#if (ALGORITHM == 3)

    /* pick up the state variables */
    /* --------------------------- */

    bytePtr = buffer + bufferLen;
    currentSequenceNr = *(LPULONG)bytePtr;
    prevBaseSlotNr = *(LPULONG)(bytePtr + 4);
    prevTag = *(LPULONG)(bytePtr + 8);
    prevByte = *(LPULONG)(bytePtr + 12);

    lookup = bytePtr + 32; /* skip standard amount */
    cache = (LPULONG)(lookup + (NR_OF_BUCKETS * 32));

    if (currentSequenceNr == 0)
    {   
        /* set initial value */
        currentSequenceNr = CACHE_SIZE;
    }

    /* if the sequence numbers are in danger of wrapping - purge cache */
    /* --------------------------------------------------------------- */

    if ((currentSequenceNr + (headIndex - tailIndex)) >= ((CACHE_SIZE * 4) - SAFETY_MARGIN))
    {
        ULONG threshold, value;
        LPULONG ptr, endPtr;

        /* weed out dead stuff and rebase live stuff */
        /* ----------------------------------------- */

        threshold = (currentSequenceNr - CACHE_SIZE) << SEQ_SHIFT_BITS;
        ptr = &cache[0];
        endPtr = ptr + CACHE_SIZE;
        while (ptr < endPtr)
        {
            value = *ptr;
            if (value < threshold)
            {
                value &= (((ULONG)1 << SEQ_SHIFT_BITS) - 1);
            }
            else
            {
                value -= ((CACHE_SIZE * 2) << (SEQ_SHIFT_BITS));
            }
            *ptr++ = value;
        }
        currentSequenceNr -= (CACHE_SIZE * 2);
    }

#endif

#if (ALGORITHM == 4)

    /* pick up the state variables */
    /* --------------------------- */

    bytePtr = buffer + bufferLen;
    cacheBits = *(LPULONG)bytePtr;
    currentSequenceNr = *(LPULONG)(bytePtr + 4);
    prevByte = *(LPULONG)(bytePtr + 8);
    prevPrevByte = *(LPULONG)(bytePtr + 12);
    prevSlotPtr = *(SLOT FAR **)(bytePtr + 16);
    bytePtr += 32; /* add standard amount */

    cacheSize = ((ULONG)1 << cacheBits);
    bucketBits = (cacheBits - 4);
    bucketSize = ((ULONG)1 << bucketBits);
    bucketMask = (bucketSize - 1);

    cache = (SLOT FAR *)bytePtr;
    bytePtr += (cacheSize * 8);
    nextFreeSlotNrs = (USHORT FAR *)bytePtr;

 
    /* if the sequence numbers are in danger of wrapping - purge cache */
    /* --------------------------------------------------------------- */

    if (currentSequenceNr > (cacheSize * 15))
    {
        ULONG threshold, decrement;
        SLOT  FAR *slotPtr,  FAR *endSlotPtr;

        /* weed out dead stuff and rebase live stuff */
        threshold = (currentSequenceNr - cacheSize) << SKEW_BITS;
        decrement = ((cacheSize * 14) << SKEW_BITS);
        slotPtr = cache;
        endSlotPtr = cache + cacheSize;
        while (slotPtr < endSlotPtr)
        {
            seqInfo = slotPtr->seqInfo;
            if (seqInfo <= threshold)
            {
                seqInfo = 0;
            }
            else
            {
                seqInfo -= decrement;
            }
            slotPtr->seqInfo = seqInfo;
            slotPtr++;
        }
        currentSequenceNr -= (cacheSize * 14);
    }

#endif


    /* now start the real work */
    /* ----------------------- */

    while ((tailIndex != headIndex) && (outPtr < endOutPtr))
    {
        nrOfItems++;

        firstByte  = buffer[tailIndex++ & bufferMask];

        if ((ULONG)(headIndex - tailIndex) <= 1)
        {
            /* too close to the end to be a string */
            goto stringNotFound;
        }

        secondByte = buffer[tailIndex & bufferMask];
        thirdByte  = buffer[(tailIndex + 1) & bufferMask];

#if (ALGORITHM == 3)
        
        tag = TAG_OF(firstByte);
        baseSlotNr = (firstByte ^ (secondByte << SKEW_BITS)) << BUCKET_BITS;


        /* look for 3 byte string */
        /* ---------------------- */

        hash = tag ^ thirdByte;
        hash = (hash ^ (hash >> 3)) & 0x1F;
        bytePtr = &(lookup[baseSlotNr >> 1]) + hash;
        slotNr = baseSlotNr + *bytePtr;
        key = (thirdByte << TAG_BITS) + tag;
        longPtr = &cache[slotNr];
        
        slotValue = *(longPtr);
        if ((slotValue & KEY_MASK) == key)
        {
            goto string3ProbablyFound;
        }

        slotValue = *(longPtr + 1);
        if ((slotValue & KEY_MASK) == key)
        {
            goto string3ProbablyFound;
        }

        slotValue = *(longPtr + 2);
        if ((slotValue & KEY_MASK) == key)
        {
            goto string3ProbablyFound;
        }

        slotValue = *(longPtr + 3);
        if ((slotValue & KEY_MASK) == key)
        {
            goto string3ProbablyFound;
        }
        
#endif

#if (ALGORITHM == 4)

        bx = firstByte ^ secondByte ^ thirdByte;
        triplet = firstByte | (secondByte << 8) | (thirdByte << 16);
        bucketNr = BUCKET_NR(bx);
        tag = TAG(triplet);
        baseSlotPtr = cache + (bucketNr << bucketBits);
 
        /* look for tag inside bucket */
        /* -------------------------- */

        lookupNr = LOOKUP_NR(tag);
        slotPtr = baseSlotPtr + lookupNr;
        slotNr = slotPtr->tagInfo & SKEW_MASK;
        slotPtr = baseSlotPtr + slotNr;
        tagInfo = slotPtr->tagInfo;

        if ((tagInfo >> SKEW_BITS) == tag)
        {
            goto string3ProbablyFound;
        }

#endif

        /* string not found */
        /* ---------------- */
 		
stringNotFound:
	
#if (ALGORITHM == 3)
        
        /* link in previous slot */
        /* --------------------- */

        AddSlot(prevBaseSlotNr, prevTag, firstByte);


        /* leave info for intermediate slot */
        /* -------------------------------- */
        
        prevBaseSlotNr = (prevByte ^ (firstByte << SKEW_BITS)) << BUCKET_BITS;
        prevTag = TAG_OF(prevByte);
        prevByte = firstByte;

#endif

#if (ALGORITHM == 4)

        /* link in previous cell */
        /* --------------------- */

        AddSlot(prevPrevByte, prevByte, firstByte);
 

        /* leave info for intermediate cell */
        /* -------------------------------- */
        
        prevPrevByte = prevByte;
        prevByte = firstByte;

#endif

        /* encode new value */
        /* ---------------- */

        ADD_BITS(9, (firstByte << 1));
        FLUSH_BITS();	
        continue;


        /* string length 3+ probably found */
        /* ------------------------------- */
		           
string3ProbablyFound:

#if (ALGORITHM == 3)
        
        /* check currency */
        /* -------------- */

        sequenceNr = slotValue >> SEQ_SHIFT_BITS;
        distance = currentSequenceNr - sequenceNr + 2;
        if (distance >= MAX_SAFE_DISTANCE)
        {
            goto stringNotFound;
        }

        stringLength = 3;
        sequenceNr++;
        tailIndex += 2;
 

        /* link in previous slot */
        /* --------------------- */

        AddSlot(prevBaseSlotNr, prevTag, firstByte);


        /* link in intermediate slot */
        /* ------------------------- */
        
        prevBaseSlotNr = (prevByte ^ (firstByte << SKEW_BITS)) << BUCKET_BITS;
        AddSlot(prevBaseSlotNr, TAG_OF(prevByte), secondByte);
        

       /* link in current slot */
       /* -------------------- */
        
        AddSlot(baseSlotNr, tag, thirdByte);
 
        
        /* leave info for following slot */
        /* ----------------------------- */
        
        prevBaseSlotNr = (secondByte ^ (thirdByte << SKEW_BITS)) << BUCKET_BITS;
        prevTag = TAG_OF(secondByte);
        prevByte = thirdByte;


        /* check for further matches */
        /* ------------------------- */

        while (tailIndex != headIndex)
        {
            /* look for proper match in following slot */
            /* --------------------------------------- */

            group = prevBaseSlotNr + (sequenceNr & (BUCKET_MASK - 3)); 
            longPtr = &cache[group];
            
            slotValue = *(longPtr);
            if (sequenceNr == (slotValue >> SEQ_SHIFT_BITS))
            {
                goto sequenceNrFound;
            }

            slotValue = *(longPtr + 1);
            if (sequenceNr == (slotValue >> SEQ_SHIFT_BITS))
            {
                goto sequenceNrFound;
            }

            slotValue = *(longPtr + 2);
            if (sequenceNr == (slotValue >> SEQ_SHIFT_BITS))
            {
                goto sequenceNrFound;
            }
           
            slotValue = *(longPtr + 3);
            if (sequenceNr != (slotValue >> SEQ_SHIFT_BITS))
            {
                break;
            }

sequenceNrFound:

            /* we should continue if the next byte matches */
            /* ------------------------------------------- */

            matchByte = buffer[tailIndex & bufferMask];

            if ((matchByte == ((slotValue >> TAG_BITS) & 0xFF)))
            {
                tailIndex++;
                stringLength++;
                sequenceNr++;

                /* link in previous slot */
                /* --------------------- */

                AddSlot(prevBaseSlotNr, prevTag, matchByte);
 

                /* leave info for current slot */
                /* --------------------------- */

                prevBaseSlotNr = (prevByte ^ (matchByte << SKEW_BITS)) << BUCKET_BITS;
                prevTag = TAG_OF(prevByte);
                prevByte = matchByte;

                continue;
            }
            break;
        }

#endif /* ALGORITHM 3 */
        

#if (ALGORITHM == 4)

        /* check currency */
        /* -------------- */

        seqInfo = slotPtr->seqInfo;
        sequenceNr = seqInfo >> SKEW_BITS;
        distance = currentSequenceNr - sequenceNr + 2;

        if (distance >= MAX_SAFE_DISTANCE)
        {
            goto stringNotFound;
        }

        stringLength = 3;
        sequenceNr++;
        tailIndex += 2;


       /* link in previous cell */
        /* --------------------- */

        AddSlot(prevPrevByte, prevByte, firstByte);
        

        /* link in intermediate cell */
        /* ------------------------- */

        AddSlot(prevByte, firstByte, secondByte);
                

       /* link in current cell */
       /* -------------------- */
        
        AddSlot(firstByte, secondByte, thirdByte);

        
        /* leave info for following cell */
        /* ----------------------------- */
        
        prevPrevByte = secondByte;
        prevByte = thirdByte;

 
        /* check for further matches */
        /* ------------------------- */

        while (tailIndex != headIndex)
        {
            /* look for proper match in following cell */
            /* --------------------------------------- */

            matchByte = buffer[tailIndex & bufferMask];
            bx = prevPrevByte ^ prevByte ^ matchByte;
            triplet = prevPrevByte | (prevByte << 8) | (matchByte << 16);
            bucketNr = BUCKET_NR(bx);
            tag = TAG(triplet);
            baseSlotPtr = cache + (bucketNr << bucketBits);            
            slotNr = seqInfo & SKEW_MASK;
            slotPtr = baseSlotPtr + slotNr;
            seqInfo = slotPtr->seqInfo;
            tagInfo = slotPtr->tagInfo;
            if (((seqInfo >> SKEW_BITS) == sequenceNr) &&
                ((tagInfo >> SKEW_BITS) == tag))
            {
                tailIndex++;
                stringLength++;
                sequenceNr++;

                /* link in previous cell */
                /* --------------------- */

                AddSlot(prevPrevByte, prevByte, matchByte);

                    
                /* leave info for current cell */
                /* --------------------------- */

                prevPrevByte = prevByte;
                prevByte = matchByte;
                continue;
            }
            break;
        }

#endif /* ALGORITHM 4 */

        length = stringLength - 2;   
        distance += distanceAdjustment;
       

        /* encode string length */
        /* -------------------- */
            
        if (length < 3)
        {
            ADD_BITS(3, 1 | (length << 1));
        }
        else
        {
            length -= 3;
            if (length < 7)
            {
                ADD_BITS(6, 0x7 | (length << 3));
            }
            else
            {
                length -= 7;
                if (length < 31)
                {
                    ADD_BITS(11, 0x3f | (length << 6));
                }
                else
                {
                    length -= 31;
                    if (length < 255)
                    {
                        ADD_BITS(19, 0x7ff | (length << 11));
                    }
                    else
                    {
                        ADD_BITS(19, 0x7ffff);
                        FLUSH_BITS();
                        ADD_BITS(13, stringLength); 
                    }
                }
            }
        }
        FLUSH_BITS();


        /* encode distance */
        /* --------------- */
	
        v = distance >> minimumDistanceBits;
        nrOfBits = minimumDistanceBits; /* minimum possible so far */
        while (v >= 8)
        {
            v >>= 4;
            nrOfBits += 4;
        }
        while (v > 0)
        {
            v >>= 1;
            nrOfBits += 1;
        }
        ADD_BITS(4, nrOfBits - minimumDistanceBits);
 
        distance ^= ((ULONG)1 << (nrOfBits - 1)); /* clear top bit */
        ADD_BITS(nrOfBits - 1, distance);
	    
        FLUSH_BITS();
    }  
    

    /* flush any last bits */
    /* ------------------- */

    if (nrOfPendingBits != 0)
    {
         *outPtr++ = (UCHAR)pendingBits;
    }


    /* finish off buffer */
    /* ----------------- */
 
    /* work out how many bytes we should have been able to send raw */
    nrOfBytes = outputBufferLength;
    if ((ULONG)(headIndex - originalTailIndex) < nrOfBytes)
    {
        nrOfBytes = (headIndex - originalTailIndex);
    }

    if (((ULONG)(tailIndex - originalTailIndex) < nrOfBytes) ||
    ((ULONG)(outPtr - outputBuffer - 2) > nrOfBytes))
    {
        /* raw would have been better - so revert to raw data,  */ 
        /* but we may need to put any unprocessed bytes through */
        /* the mill, so as to keep a common agreed state        */

        while ((ULONG)(tailIndex - originalTailIndex) < nrOfBytes)
        {

#if (ALGORITHM == 3)

           firstByte  = buffer[tailIndex++ & bufferMask];
           AddSlot(prevBaseSlotNr, prevTag, firstByte);
           prevBaseSlotNr = (prevByte ^ (firstByte << SKEW_BITS)) << BUCKET_BITS;
           prevTag = TAG_OF(prevByte);
           prevByte = firstByte;

#endif

#if (ALGORITHM == 4)

           firstByte  = buffer[tailIndex++ & bufferMask];
           AddSlot(prevPrevByte, prevByte, firstByte);
           prevPrevByte = prevByte;
           prevByte = firstByte;
  
#endif        
        }

        /* header = inverse of nr of bytes */
        v = nrOfBytes ^ 0xFFFF;
        *(outputBuffer) = (UCHAR)v;
        *(outputBuffer + 1) = (UCHAR)(v >> 8);

        outPtr = outputBuffer + 2;
        index = originalTailIndex & bufferMask;
        if ((index + nrOfBytes) <= (bufferMask + 1))
        {
            /* contiguous memory copy */
            memcpy(outPtr, &(buffer[index]), (ULONG)nrOfBytes);
        }
        else
        {
            firstPortion = (bufferMask - index + 1);
            memcpy(outPtr, &(buffer[index]), (ULONG)firstPortion);
            memcpy(outPtr + firstPortion, &(buffer[0]), (ULONG)(nrOfBytes - firstPortion));
        }

        outPtr += nrOfBytes;
        tailIndex = originalTailIndex + nrOfBytes;
    }
    else
    {
        /* we have reduced it - send reduced data */

        /* header = number of items */
        *(outputBuffer) = (UCHAR)nrOfItems;
        *(outputBuffer + 1) = (UCHAR)(nrOfItems >> 8);
    }


   /* save state variables */
    /* -------------------- */

#if (ALGORITHM == 3)

    bytePtr = buffer + bufferLen;
    *(LPULONG)bytePtr = currentSequenceNr;
    *(LPULONG)(bytePtr + 4) = prevBaseSlotNr;
    *(LPULONG)(bytePtr + 8) = prevTag;
    *(LPULONG)(bytePtr + 12) = prevByte;

#endif

#if (ALGORITHM == 4)

    bytePtr = buffer + bufferLen;
    *(LPULONG)(bytePtr + 4) = currentSequenceNr;
    *(LPULONG)(bytePtr + 8) = prevByte;
    *(LPULONG)(bytePtr + 12) = prevPrevByte;
    *(SLOT FAR **)(bytePtr + 16) = prevSlotPtr;

#endif


    /* and return counts */
    pOutBuf->ByteCount = outPtr - outputBuffer; /* nr of bytes output */
    return (tailIndex - originalTailIndex);     /* nr of bytes consumed */

}

#endif /* ALGORITHM 3 */



/*************************************************************************
*
*   ConvertBufferIntoDataQueue
*
*   The expander function
*
*   Unpacks a buffer into the history data 
*   Assumes there are no space or limit problems
*   Updates the queue->WriteReached index
*
*************************************************************************/

#define CONSUME_BITS(n) {futureBits >>= (n); nrOfFutureBits -= (n);}

void WFCAPI ConvertBufferIntoDataQueue(
    LPUCHAR     inputBuffer,       /* buffer to process */
    ULONG       inputBufferLength, /* its length */
    PDATA_QUEUE dqp)               /* basic data queue control info */
{
    ULONG historyBufferMask, minimumDistanceBits, bufferLen;
    LONG  distanceAdjustment;
    ULONG futureBits, nrOfFutureBits, length, stringLength, distance;
    ULONG nrOfBits, topBit, index, headIndex, nrOfItems, firstPortion;  
    LPUCHAR inPtr, endInPtr, historyBuffer;
    ULONG mIndex, hIndex;
    LPUCHAR mPtr, hPtr, endMPtr;
    
    historyBuffer = dqp->Buffer;
    historyBufferMask = dqp->BufferMask;
    bufferLen = dqp->BufferLen;
    headIndex = dqp->WriteReached;

    distanceAdjustment = dqp->DistanceAdjustment;
    minimumDistanceBits = dqp->MinimumDistanceBits;

    inPtr = inputBuffer;
    endInPtr = inPtr + inputBufferLength;
 
    nrOfItems = *inPtr | ((ULONG)*(inPtr + 1) << 8);
    inPtr += 2;
    if (nrOfItems & 0x8000)
    {
        /* a negative header - this means we just have the original bytes */
        /* -------------------------------------------------------------- */
 
        nrOfItems ^= 0xFFFF; /* = number of bytes to copy */

        /*
         * If number of items is corrupt, ignore the buffer.
         */
        if ( nrOfItems > bufferLen ) {
            return;
        }
 
        /* copy into history buffer */
        index = headIndex & historyBufferMask;
        if ((index + nrOfItems) <= bufferLen)
        {
            /* contiguous memory copy */
            memcpy(&(historyBuffer[index]), inPtr, (ULONG)nrOfItems);
        }
        else
        {
            /* split memory copy */
            firstPortion = bufferLen - index;
            memcpy(&(historyBuffer[index]), inPtr, (ULONG)firstPortion);
            memcpy(&(historyBuffer[0]), inPtr + firstPortion, (ULONG)(nrOfItems - firstPortion));
        }
        headIndex += nrOfItems;
       
    }
    else
    {
        /* a positive header - we have the number of items to interpret */
        /* ------------------------------------------------------------ */

        futureBits = 0;
        nrOfFutureBits = 0;
    
        while (nrOfItems--)
        {
            /* load more bits */
            /* -------------- */
        
            while ((nrOfFutureBits < 24) && (inPtr < endInPtr))
            {
                futureBits |= ((ULONG)(*inPtr++)) << nrOfFutureBits;
                nrOfFutureBits += 8;
            }
         
            if (futureBits & 1)
            {
                /* decode string length */
                /* -------------------- */
            
                length = (futureBits >> 1) & 0x3;
                CONSUME_BITS(3);
 
                if (length == 0) /* means length 2*/
                {
                    /* special case string length of 2 - do distance */
                    /* --------------------------------------------- */

                    stringLength = 2;
                    nrOfBits = futureBits & 7;
                    CONSUME_BITS(3);
                    nrOfBits += 2; /* remove the bias */
	                topBit = ((ULONG)1 << (nrOfBits - 1));
                    distance = futureBits & (topBit - 1);
                    distance |= topBit;
                    CONSUME_BITS(nrOfBits - 1);
                }
                else
                {
                    /* general case for other lengths */
                    /* ------------------------------ */
                    
                    if (length < 3)
                    {
                        stringLength = length + 2;
                    }
                    else
                    {
                        length = futureBits & 7;
                        CONSUME_BITS(3);
                        if (length < 7)
                        {
                            stringLength = length + 5;
                        }
                        else
                        {                        
                            length = futureBits & 0x1f;
                            CONSUME_BITS(5);
                            if (length < 31)
                            {
                                stringLength = length + 12;
                            }
                            else
                            {
                                length = futureBits & 0xff;
                                CONSUME_BITS(8);
                                if (length < 255)
                                {
                                    stringLength = length + 43;
                                }
                                else
                                {
                                    /* load more bits */
                                    while ((nrOfFutureBits < 24) && (inPtr < endInPtr))
                                    {
                                        futureBits |= (ULONG)(*inPtr++) << nrOfFutureBits;
                                        nrOfFutureBits += 8;
                                    }
                                    stringLength = futureBits & 0x1FFF;                                   
                                    CONSUME_BITS(13);
                                }
                            }
                        }
                    }

                    /* load more bits */
                    while ((nrOfFutureBits < 24) && (inPtr < endInPtr))
                    {
                        futureBits |= (ULONG)(*inPtr++) << nrOfFutureBits;
                        nrOfFutureBits += 8;
                    }


                    /* decode distance */
                    /* --------------- */

                    nrOfBits = futureBits & 0xf;
                    CONSUME_BITS(4);
                    nrOfBits += minimumDistanceBits;
                	
                    topBit = ((ULONG)1 << (nrOfBits - 1));
                    distance = futureBits & (topBit - 1);
                    distance |= topBit;
                    CONSUME_BITS(nrOfBits - 1);
                    distance -= distanceAdjustment; 
                }

           
                /* copy previous string to head of history buffer */
                /* ---------------------------------------------- */

                mIndex = (headIndex - distance) & historyBufferMask;
                hIndex = headIndex & historyBufferMask;
                if (((hIndex | mIndex) + stringLength) <= bufferLen)
                {
                    /* definitely no wrap problems */

                    /* Note: Do not be tempted to use memcpy or memmove here */
                    /* The dominant string length is 2 and there may be overlaps */
                    /* It will either not work or be slower */

                    mPtr = &historyBuffer[mIndex];
                    hPtr = &historyBuffer[hIndex];
                    headIndex += stringLength;
                    endMPtr = mPtr + stringLength;
                    while (mPtr < endMPtr)
                    {
                        *hPtr++ = *mPtr++;
                    }
                }
                else
                {
                    /* wrap problems */
                    while (stringLength > 0)
                    {
                        historyBuffer[headIndex & historyBufferMask] =
                            historyBuffer[mIndex & historyBufferMask];
                        stringLength--;
                        mIndex++;
                        headIndex++;
                    }
                }
            }
            else
            {
                /* decode new byte value */
                /* --------------------- */
            
                historyBuffer[headIndex & historyBufferMask] = (UCHAR)((futureBits >> 1) & 0xFF);
                headIndex++;
                CONSUME_BITS(9);
            }
        }                      
    }

    dqp->WriteReached = headIndex;
} 


/*************************************************************************
*
*   ExtractNewDataFromQueue
*
*   Works out the nr of contiguous slices that the new data consists of 
*   This will be 1 (normal case), 2 (buffer wrap) or 0 (no new data) 
*   Corresponding to the number of slices, the pointers and lengths of 
*   the slices are returned in the memory provided. 
*   It returns the number of slices 
*
*************************************************************************/


ULONG WFCAPI ExtractNewDataFromQueue(
    PDATA_QUEUE dqp,               /* basic data queue control info */
    UCHAR   FAR **startPointersPtr, /* where start pointers will be stored */
    ULONG   FAR *lengthsPtr)        /* where length will be stored */
{
    ULONG nrOfBytes, tailIndex, headIndex, index, firstPortion;
    ULONG nrOfSlices = 0;

    
    tailIndex = dqp->WriteBase;
    headIndex = dqp->WriteReached;

    nrOfBytes = (headIndex - tailIndex);
    if (nrOfBytes == 0)
    {
        return(0);
    }
    else if ( nrOfBytes > dqp->BufferLen ) {
        /*
         * Bad data.
         * We've probably smashed the history buffer, but let's just ignore the data and
         * try to continue.
         */
        dqp->WriteReached = dqp->WriteBase;
        return(0);
    }

    index = tailIndex & dqp->BufferMask;
    if ((index + nrOfBytes) <= (dqp->BufferLen))
    {
        /* single contiguous slice */
        *startPointersPtr = &(dqp->Buffer[index]);
        *lengthsPtr = nrOfBytes;
        nrOfSlices = 1;
    }
    else
    {
        /* split memory - 2 slices */
        firstPortion = (dqp->BufferLen - index);
        *startPointersPtr++ = &(dqp->Buffer[index]);
        *lengthsPtr++ = firstPortion;
        *startPointersPtr = &(dqp->Buffer[0]);
        *lengthsPtr = nrOfBytes - firstPortion;
        nrOfSlices = 2;
    }

    dqp->WriteBase = tailIndex + nrOfBytes;
    dqp->WriteLimit = dqp->WriteLimit + nrOfBytes;
    return (nrOfSlices);
}

 
