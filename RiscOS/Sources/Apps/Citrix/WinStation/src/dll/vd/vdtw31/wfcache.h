
/*******************************************************************************
*
*   WFCACHE.H
*
*   Thin Wire Windows - object caching (windows) header
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:16:54   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 14:40:16   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 14:02:46   kurtp
*  update
*  
*******************************************************************************/

#ifndef __WFCACHE_H__
#define __WFCACHE_H__

//used to hold information for allocated cache areas
//16 segments for 1 meg.  128 for 8 meg
typedef struct _LARGE_CACHE_SEGMENTS
{
   HGLOBAL  hsegment;
   LPBYTE   lpsegment;
} LARGE_CACHE_SEGMENTS, far * LPLARGE_CACHE_SEGMENTS, near * PLARGE_CACHE_SEGMENTS;


extern LPBYTE lptiny_cache;


//initialize cache data areas
BOOL  TWCache_Init(UINT chunks_2K);

//Destroy cache data areas
BOOL  TWCache_Destroy(void);

/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
//external interfaces

//returns a far pointer to the tiny_cache entry representing
//the client cache object handle
//handle should be a UINT
#define lp32ByteObject(handle) ((LPBYTE) ((lptiny_cache) + (handle << 5)))


//NOTE: The interfaces are different than the DOS client both in what
//    they do and the parameters that they take
// We optimized these interfaces to the fact that the thinwire code can
// get direct addressability to the cache data so the interfaces pass back
// a pointer instead of copy data.
// We also simplify the apis to take parameters like the client cache object
// handle, the client cache chain handle, and the chunk type.  The DOS apis
// allowed for more general usage which we never needed.
//
// We trust the caller to trustfully use the pointer and not go into
// someone elses cache area.
//
// We added an api to say when a write operation is complete.
// We do this for future DIM support as well as to make the interface
// theoretically device independent, though the interface will be
// inefficient if the hardware does not support direct addressability.
// If the hardware does not support direct addressability then the DOS
// cache interfaces are more efficient.  In that case, the DOS implementation,
// which is significantly more comples, needs to be used because if the cache
// area is not directly addressable, there is probably more than one type
// of backing store used to make it work.
//
// The interface is specified to only allow one outstanding pointer at a time
// even though more than one will work.  The write complete assumes last
// pointer and one must be done before the next write pointer is obtained
// We are on the honor system for the reads.
//
// On write operations we are given the size for _512B and _2K objects
// and give back the size on a read.  We still do this because some code
// relies on this and it could be useful for making the dim support efficient
// Because we try to optimize bitmap operations and move the data directly
// into the cache we probably don't know the size of the data until it
// is in the cache (rle case).  We know the maximimum size will fit into the
// cache.  In order to be able to get a pointer without knowing the exact
// size you can reset the size of the object when you call the api to say
// the write is complete.  For a chained object, the write complete call
// gives the real size of the last block of the chain.
//


//note: these apis are used for chunk_types:
//       _2K, _512B, _128B, and monoptr
//

//    For Write:
//    object_handle - the client cache object handle (12 bit value)
//    chunk_type    - see twcommon.h. values _2K, _512B, _128B, or monoptr
//    size          -
//       for _128B and monoptr it is ignored
//       for _512B and _2K the size is saved and given back on a read
//                         for the same object_handle.  Currently not really
//                         used for function but will be for dims so do it
//                         correctly.  !!!!!!! Can be zero for
//                         _512B or _2K not chained -
//                         assuming finishedTWCacheWrite sets the correct size
//       for _2K chained - make 0 for all write operations except for the
//                         last chain block which should be the size for the
//                         chain_handle.  Can decide which is the last block
//                         even if ask for size is 0 by using finishedTWCacheWrite
//       NOTE: Size can be readjusted when calling finishedTWCahceWrite
//             This is used when dont know the exact size when getting pointer
//             Still must know the maximum size will fit in the block
//
//    chain_handle   - ONLY USED FOR _2K and CHAINED operations
//       for _2K should be the same value as object_handle
//       for CHAINED if the first block then same as object_handle
//                   if not the first block then keep object_handle the same
//                   as previous block and use the new chain handle
//                   keep the size 0 until at the last block of the chain
//        NOTE: no other write operations allowed while in the middle of
//              writing a chained _2K object
//
//    return code LPBYTE is a far pointer to where the write can be done to
//       the caller promises only to stay within the max size implied by
//       chunk_type.  In addition, the caller promises that "size" is correct
//
//       IF ERROR then returned pointer is NULL
//
//    only 1 write outstanding at a time is allowed.  When the write of an
//       object is complete then call finishedTWCacheWrite.  Do not call
//       in the middle of a _2K chain operation.  Only call at the end.
//       Set parm to 0 if leave size of write the same.  Else set to actual
//       size.  If a chained block then adjusts the size of the last block
//       So when doing chained operation can ask for 0 size block to say
//       in the middle of a chain and then use finishedTWCacheWrite to say
//       you are really at the end of the chain and this is the size of the
//       last block.
//
//
//    For Read:
//    object_handle and chunk_type same as write.
//    lpsize is a pointer to a UINT that will be given the size of
//          object_handle only if chunk_type is _512B or _2K.
//          If a chained _2K operation then the returned size is 0 except for
//          the last block of the chain.
//    chain_index is only used for chunk_type _2K.
//       If NOT a chained operation then must be 0.
//       If chained then its the sequence block number of the chain which
//          starts at 0 and goes up by 1 for every block in the chain.
//          A chained object does not have to get accessed sequentially by
//          block number, but if it is, the access is more efficient!
//
//    return code LPBYTE is a far pointer to where the read can be done from.
//       The caller promises to stay within the size implied by chunk_type
//       and/or the returned size.  Only 1 pointer at a time should be
//       assumed to be ok to use.  The next read invalidates the previous
//       read pointer.  If its a chained object, the size returned is 0
//       until get the last block, where the size is the size of the last
//       block.
//
//    IF ERROR then the returned pointer is NULL





LPBYTE   lpTWCacheWrite(UINT object_handle, CHUNK_TYPE chunk_type, UINT size,
                        UINT chain_handle);

BOOL     finishedTWCacheWrite(UINT size);


LPBYTE   lpTWCacheRead(UINT object_handle, CHUNK_TYPE chunk_type,
                       LPUINT lpsize, UINT chain_index);



//the design of the control area is as follows:
//we still use 5 bytes per 2K cache chunk
//we change the format from the dos implementation to more efficiently
//access information, without requiring a pack/unpack
//Because we only allow 1 write operation in progress and because
//reads of chained objects will be clumped within the same operation
//we will keep optimization information for the chained object in global
//variables instead of in the cache header.  This will allow the information
//that is kept to be put in a more efficient format.  The only gotcha is
//that we need to check to see if a chained object anchor is written over
//which would invalidate the entire chained object.
//we will also add an invalid type and an unchained 2K type
//and use them to do more assert checking.
//

//the following is the list of control area types.

typedef enum _CACHE_TYPE
{
   invalid,
   random,              //used for monoptr and _128B
   small_512,               //used for 4 _512B in _2K
   nochain2K,             //2K chunk, not chained
   begin_chain,         //beginning of chain
   middle_chain,
   end_chain
} CACHE_TYPE;


//the first byte (BYTE 0) of the control area has the type in the low order
//4 bits of that byte

//NOTE:!!!!!The size is always kept as a 0 based 1 value
//          BYTE 0 ALWAYS HAS THE TYPE in the low order nibble

/////////////////////////////////////////////////////////////
//                    RANDOM

//if the type is random, then no other information is kept in the 5 bytes
//used to support 128B and monoptr

////////////////////////////////////////////////////////////
//
//                      SMALL_512

//if the type is small_512 then the 0 based 1 size value for the 4 512 byte
//chunks is kept in the control info as follows

//bits 7,6,5,4 of byte 0 have the high order bit (bit 9) of the size
//of chunks 3,2,1,0 respectively.  So bit 9 of chunk 0 size is bit 4
//of byte 0.
//the low order byte of the size is found in bytes 1 - 4 of the 5 bytes
//where byte 1 is chunk 0 and byte 4 is chunk 3.

/////////////////////////////////////////////////////////////////////

//                      nochain2K
//
//bytes 1 and 2 are a word which has the size of the _2K object
//byte 1 is lsb and byte 2 is msb

////////////////////////////////////////////////////////////////////

//                      begin_chain
//
//bytes 1 and 2 are a word which has the chain handle of the next
//_2K chunk in the chain.  kept as 0 until we have the next chain handle
//byte 1 is lsb and byte 2 is msb

///////////////////////////////////////////////////////////////////

//                      middle_chain
//same as begin chain

///////////////////////////////////////////////////////////////////

//                      end_chain
//
//bytes 1 and 2 have the size of the last _2K chunk of the chain
//byte 1 is lsb and byte 2 is lsb.

///////////////////////////////////////////////////////////////////

//to get from the object handle or chain handle to the correct 5 byte
//control area and/or the correct _2K slot number:

//if _2K or chained:
//      multiply the handle (a 12 bit value which is the _2K slot #) by 5.
//      the maximum value is 4K * 5 which is << 64K.
//if small_512:
//      divide the handle by 4 to get the _2K slot # and multiply by 5
//      the 512B chunk number is the low order 2 bits of the handle
//if _128B:
//      divide the handle by 16 to get the _2K slot # and multiple by 5
//      the 128B chunk in the _2K is the low order 4 bits of the handle
//if monoptr:
//      the _2K slot # is always 0 (divide handle by 8 which will be 0)
//      the low order 3 bits will be the 1 out of 8 256 byte area in the slot

//

//to translate from the object handle/chain handle to a cache address
//add this number of 0's to the right of the handle
//if _2K or chained        11
//if small_512             9
//if 128B                  7
//if monoptr               8

#endif //__WFCACHE_H__
