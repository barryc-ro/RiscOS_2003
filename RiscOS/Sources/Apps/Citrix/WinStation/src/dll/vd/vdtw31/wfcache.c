
/*******************************************************************************
*
*   WFCACHE.C
*
*   Thin Wire Windows - Cache Code
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Jeff Krantz (jeffk)
*
*   $Log$
*   Revision 1.1  1998/01/19 19:13:00  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.3   15 Apr 1997 18:16:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   30 May 1996 16:56:40   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:33:54   kurtp
*  update
*  
*******************************************************************************/

//#ifdef WIN16
//#pragma code_seg("WFCACHE_TEXT")
//#endif

#include <stdlib.h>

#include "wfglobal.h"

#include "../../../inc/clib.h"

static HGLOBAL  htiny_cache;

static allocated_chunks_2K;      //number of 2K chunks allocated for large c

static UINT segments_needed;     //number 60K segments needed to hold

static HGLOBAL  hcontrol_area;   //the large cache control area
static LPBYTE   lpcontrol_area;

//60K = 61440
//8MB = 8388608
//need 137 60K cache segments to fit

#ifdef WIN16
static LARGE_CACHE_SEGMENTS large_cache_segments[137];
#endif

#if defined(WIN32) || defined(RISCOS)
static LARGE_CACHE_SEGMENTS large_cache_segments[1];      //only need 1 segment for 32 bit mode
#endif

//the following global variables are used to keep track of the state
//of the current write operation for 2 purposes
//1) need to have a finished call before next write operation
//2) want to optimized writes of chained objects
//
static CACHE_TYPE current_write_type = invalid;  //set to invalid when none
static LPBYTE     lpcurrent_write_control_area;   //points to current 5 byte
                                                //control area. if chained
                                                //then current chain_index
static UINT    current_write_object_handle;  //current object_handle. if chained
                                             //its still the object handle and
                                             //NOT the chain handle

//the following global variables are used to keep track of the state
//of the current read operation if its 2K orchained to allow optimized reads
//of the next chain index or the current item over again
//the handle gets reset to 0 if any write operation or unchained read happens.
static UINT    current_read_object_handle;
static LPBYTE  lpcurrent_read_control_area;    //points to current 5 byte control
                                             //area corresponding to handle
                                             //and chain_index
static UINT    current_read_chain_index;

//initializes all of the thinwire cache data areas, locks them, and sets
//everything up so can handle caching requests.

//the tiny cache is always assumed to be 32K.
//the number of 2K chunks desired in the large cache is passed in as a parameter
//largest size is 8M or 4096 2K chunks
//
//returns TRUE if was able to allocate and setup everything
//if returns FALSE then failed somewhere and all allocated stuff is deleted

BOOL  TWCache_Init(UINT chunks_2K)
{
   UINT   i,j;

   HGLOBAL  hretcode;

#ifdef WIN16
   DWORD sizealloc;
   j = 137;
#endif

#if defined(WIN32) || defined(RISCOS)
   j = 1;
#endif


   for (i=0; i<j ; i++) {
      large_cache_segments[i].hsegment = NULL;
      large_cache_segments[i].lpsegment = NULL;
   }

   htiny_cache = GlobalAlloc(GMEM_MOVEABLE, 32768);
   ASSERT(htiny_cache, 0);
   if (!htiny_cache) {
      return(FALSE);
   }
   lptiny_cache = (LPBYTE) GlobalLock(htiny_cache);
   ASSERT(lptiny_cache, 0);
   if (!lptiny_cache) {
      hretcode = GlobalFree(htiny_cache);
      ASSERT(!hretcode, 0);
      return(FALSE);
   }

   //got the tiny cache allocated, now do the large_cache
   TRACE((TC_TW,TT_TW_CACHE+TT_TW_ENTRY_EXIT,"TW: tiny cache successfully setup\n"));

   if (chunks_2K == 0) {
      TRACE((TC_TW,TT_TW_CACHE+TT_TW_ENTRY_EXIT,"TW: no large cache requested\n"));
      return(TRUE);
   }

   //can assume there is a large cache requested
   allocated_chunks_2K = chunks_2K;

   //first allocate the control area which is 5 bytes per 2K chunk
   hcontrol_area = GlobalAlloc(GMEM_MOVEABLE, chunks_2K * 5);
   ASSERT(hcontrol_area,0);
   if (!hcontrol_area) {
      GlobalUnlock(htiny_cache);
      hretcode = GlobalFree(htiny_cache);
      ASSERT(!hretcode, 0);
      return(FALSE);
   }
   lpcontrol_area = (LPBYTE) GlobalLock(hcontrol_area);
   ASSERT(lpcontrol_area,0);
   if (!lpcontrol_area) {
      GlobalUnlock(htiny_cache);
      hretcode = GlobalFree(htiny_cache);
      ASSERT(!hretcode, 0);
      hretcode = GlobalFree(hcontrol_area);
      ASSERT(!hretcode, 0);
      return(FALSE);
   }

#if defined(WIN32) || defined(RISCOS)
   segments_needed = 1;
   TRACE((TC_TW,TT_TW_CACHE+TT_TW_ENTRY_EXIT,"TW: Allocating large cache size=%u\n",
                                 2048 * chunks_2K));
   large_cache_segments[0].hsegment = GlobalAlloc(GMEM_MOVEABLE,
                                       2048 * chunks_2K);
   ASSERT(large_cache_segments[0].hsegment, 0);
   if (!large_cache_segments[0].hsegment) {
      GlobalUnlock(htiny_cache);
      hretcode = GlobalFree(htiny_cache);
      ASSERT(!hretcode, 0);
      GlobalUnlock(hcontrol_area);
      hretcode = GlobalFree(hcontrol_area);
      ASSERT(!hretcode, 0);
      return(FALSE);
   }
   large_cache_segments[0].lpsegment = GlobalLock(large_cache_segments[0].hsegment);
   ASSERT(large_cache_segments[0].lpsegment, 0);
   if (!large_cache_segments[0].lpsegment) {
      hretcode = GlobalFree(large_cache_segments[0].hsegment);
      ASSERT(!hretcode, 0);
      GlobalUnlock(htiny_cache);
      hretcode = GlobalFree(htiny_cache);
      ASSERT(!hretcode, 0);
      GlobalUnlock(hcontrol_area);
      hretcode = GlobalFree(hcontrol_area);
      ASSERT(!hretcode, 0);
      return(FALSE);
   }
#endif

#ifdef WIN16
//note that there are 30 2K chunks in 60K

   segments_needed = ((chunks_2K - 1) / 30) + 1;

   TRACE((TC_TW,TT_TW_CACHE+TT_TW_ENTRY_EXIT,"TW: Allocating large cache. # 60K segments=%u\n",
                                          segments_needed));
   for (i=0 ; i<segments_needed ; i++) {
      if (i != segments_needed-1) {
         //its not the last segment, so the desired size is 60K
         sizealloc = 61440;
      }
      else {
         //its the last chunk, so compute the desired size
         sizealloc = (chunks_2K % 30) * 2048;      //jkfix
         //if the modulo is 0 then its really 60K
         if (sizealloc == 0) {
            sizealloc = 61440;
         }
         TRACE((TC_TW,TT_TW_CACHE,"TW: last segment size=%u\n",sizealloc));
      }
      //ok, allocate and lock it.  If failure release previous resources
      // Richa: Added 16 to the segment size because we found a video driver
      // that accesses past the end of the segment by one byte.  We'll let
      // the driver do this so the user doesn't trap and blame us!
      large_cache_segments[i].hsegment = GlobalAlloc(GMEM_MOVEABLE,
                                       sizealloc+16);
      large_cache_segments[i].lpsegment = GlobalLock(large_cache_segments[i].hsegment);

      //error checking
      ASSERT(large_cache_segments[i].hsegment && large_cache_segments[i].lpsegment, 0);
      if (!(large_cache_segments[i].hsegment && large_cache_segments[i].lpsegment)) {
         GlobalUnlock(htiny_cache);
         hretcode = GlobalFree(htiny_cache);
         ASSERT(!hretcode, 0);
         GlobalUnlock(hcontrol_area);
         hretcode = GlobalFree(hcontrol_area);
         ASSERT(!hretcode, 0);
         for (j=0 ; j<=1 ; j++ ) {
            GlobalUnlock(large_cache_segments[j].hsegment);
            hretcode = GlobalFree(large_cache_segments[j].hsegment);
            ASSERT(!hretcode, 0);
         }
         return(FALSE);
      }

   }
#endif


   TRACE((TC_TW,TT_TW_CACHE,"TW: large cache setup, segments_needed=%u, chunks_2K allocated=%u\n",
                           segments_needed, chunks_2K));
   return(TRUE);

}


BOOL  TWCache_Destroy()
{
   UINT i;

   HGLOBAL  hretcode1, hretcode2, hretcode3;

   hretcode2 = NULL;

   for (i=0; i<segments_needed ; i++ ) {

      GlobalUnlock(large_cache_segments[i].hsegment);
      hretcode1 = GlobalFree(large_cache_segments[i].hsegment);
      ASSERT(!hretcode1, 0);
      if (hretcode1) {
         hretcode2 = hretcode1;
      }
   }

   GlobalUnlock(htiny_cache);
   hretcode1 = GlobalFree(htiny_cache);
   ASSERT(!hretcode1, 0);

   GlobalUnlock(hcontrol_area);
   hretcode3 = GlobalFree(hcontrol_area);
   ASSERT(!hretcode3, 0);

   return(!(hretcode1 || hretcode2 || hretcode3));

}



//see twcache.h for prolog description

LPBYTE lpTWCacheWrite(UINT object_handle, CHUNK_TYPE chunk_type,
                      UINT size, UINT chain_handle)
{
#ifdef WIN16
   LPBYTE   lpcache;
#endif
   BYTE     btemp;

   current_read_object_handle = 0;

   TRACE((TC_TW,TT_TW_CACHE,
         "TW: CACHE Write, object_handle=%u, chunk_type="));

   switch (chunk_type) {
   case _32B:
      //invalid for this interface
      ASSERT(0,0);
      return(NULL);

   case monoptr:
      TRACE((TC_TW,TT_TW_CACHE,"monoptr\n"));
      ASSERT(current_write_type == invalid, 0);
      current_write_type = random;
      lpcurrent_write_control_area = lpcontrol_area;
      current_write_object_handle = object_handle;

      //update the control area
      ASSERT(object_handle < 8, 0);
      *lpcontrol_area = (BYTE) (CACHE_TYPE) random;

      //address calculated the same for WIN16 and WIN32
      return(large_cache_segments[0].lpsegment + (object_handle << 8));

   case _128B:
      TRACE((TC_TW,TT_TW_CACHE,"_128B\n"));
      ASSERT(current_write_type == invalid, 0);
      current_write_type = random;
      lpcurrent_write_control_area = lpcontrol_area +
                                              ((object_handle >> 4) * 5);
      current_write_object_handle = object_handle;

      *lpcurrent_write_control_area = (BYTE) (CACHE_TYPE) random;

#ifdef WIN16
      lpcache = large_cache_segments[(object_handle >> 4) / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"using cache segment %lx, ", lpcache));

      lpcache += ((object_handle >> 4) % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"2K chunk address %lx\n", lpcache));

      TRACE((TC_TW,TT_TW_CACHE,"_128B object address %lx\n",
                        lpcache + ((object_handle & 0x0f) << 7)));
      return(lpcache + ((object_handle & 0x0f) << 7));
#endif

#if defined(WIN32) || defined(RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"_128B object address %lx\n",
            large_cache_segments[0].lpsegment + (object_handle << 7) ));
      return(large_cache_segments[0].lpsegment +
                        (object_handle << 7) );

#endif

   case _512B:
      TRACE((TC_TW,TT_TW_CACHE,"_512B, size=%u\n", size));
      ASSERT(current_write_type == invalid, 0);
      current_write_type = small_512;
      lpcurrent_write_control_area = lpcontrol_area +
                                             ((object_handle >> 2) * 5);
      current_write_object_handle = object_handle;

      *lpcurrent_write_control_area &= 0xf0;
      *lpcurrent_write_control_area |= (BYTE) (CACHE_TYPE) small_512;

      //set the size in the control area
      *(lpcurrent_write_control_area + (object_handle & 0x03) + 1) =
                                                   (BYTE) (size- 1);
      btemp = 1 << (4 + (object_handle & 0x03));

      if (size > 256) {       //note that 256 gets represented as 255
         *lpcurrent_write_control_area |= btemp;
      }
      else *lpcurrent_write_control_area &= ~btemp;

#ifdef WIN16
      lpcache = large_cache_segments[(object_handle >> 2) / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"using cache segment %lx, ", lpcache));

      lpcache += ((object_handle >> 2) % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"2K chunk address %lx\n", lpcache));

      TRACE((TC_TW,TT_TW_CACHE,"TW: _512B object address %lx\n",
            lpcache + ((object_handle & 0x03) << 9)));

      return(lpcache + ((object_handle & 0x03) << 9));
#endif

#if defined(WIN32) || defined(RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"_512B object address %lx\n",
            large_cache_segments[0].lpsegment + (object_handle << 9) ));
      return(large_cache_segments[0].lpsegment +
                        (object_handle << 9) );
#endif

   case _2K:
      TRACE((TC_TW,TT_TW_CACHE,"_2K, size=%u, chain_handle=%u\n", size, chain_handle));

      switch (current_write_type) {
      case random:
      case small_512:
      case end_chain:
         ASSERT(0,0);
         TRACE((TC_TW,TT_TW_CACHE,
         "TW:      INVALID!!!, current_write_type is end_chain, random or small_512\n"));
         current_write_type = invalid;
         //fall through to invalid

      case invalid:
         TRACE((TC_TW,TT_TW_CACHE,
               "TW:     current_write_type is invalid so nochain2K or begin_chain\n"));
         ASSERT(object_handle == chain_handle, 0);    //can't be middle or end of chain yet

         lpcurrent_write_control_area = lpcontrol_area + (object_handle * 5);
         current_write_object_handle = object_handle;

         //we make the case nochain2K whether the size is 0 or not
         //because a finished command can still make it nochain2K so
         //it doesn't have to be chained.  The next command we get will
         //figure out what the correct thing to do is
         //the size will be 0 if still need to determine the correct case
         //if the size is nonzero it must be nochain2K
         //note that a 0 in the size field is still valid for nochain2K
         //because of 0 based 1 but we use that for error checking anyway

         current_write_type = nochain2K;
         *lpcurrent_write_control_area = nochain2K;

         if (size != 0) {
            TRACE((TC_TW,TT_TW_CACHE,"TW: the size is not 0 so must be nochain2K\n"));
            write_word(lpcurrent_write_control_area + 1, size - 1);
         }
         else
	 {
	     write_word(lpcurrent_write_control_area + 1, 0);
	 }
	 
         //note that for all cases we break and fall through to get the cache pointer
         //from the chain_handle
         break;

      case nochain2K:
         //if the size of the control area is 0 then we are ok
         //because the previous case should really have been begin_chain
      case middle_chain:
         //same logic for middle_chain except if previous control area
         //size is not 0 then we have an internal logic error instead
         //of api usage error
         //also don't set the previous control area type
         //because its already set to middle_chain

#ifdef DEBUG
         if (current_write_type == nochain2K) {
            TRACE((TC_TW,TT_TW_CACHE,
                 "TW: current_write_type is nochain2K so error or begin_chain\n"));
         }
         else TRACE((TC_TW,TT_TW_CACHE,
                  "TW: current_write_type is middle_chain\n"));
#endif

         TRACE((TC_TW,TT_TW_CACHE,"TW: current_write_object_handle=%u\n",
                                 current_write_object_handle));

         if (read_word(lpcurrent_write_control_area + 1) != 0) {
            TRACE((TC_TW,TT_TW_CACHE,
               "TW:    INVALID because size of previous control area is nonzero\n"));
            ASSERT(0,0);
            return(NULL);
         }

         //previous size is 0 so really a begin_chain case
         ASSERT(object_handle == current_write_object_handle, 0);

         if (current_write_type == nochain2K) {
            //if middle_chain then field already correct as middle chain
            *lpcurrent_write_control_area = begin_chain;
         }

         write_word(lpcurrent_write_control_area + 1, chain_handle);

         //point to new control area and then figure out case
         lpcurrent_write_control_area = lpcontrol_area + (chain_handle * 5);

         if (size == 0) {
            //must be middle chain
            *lpcurrent_write_control_area = middle_chain;
            current_write_type = middle_chain;
            write_word(lpcurrent_write_control_area + 1, 0);
         }
         else {
            //must be end of chain
            *lpcurrent_write_control_area = end_chain;
            current_write_type = end_chain;
            write_word(lpcurrent_write_control_area+ 1, size - 1);
         }

         //note that for all cases we break and fall through to get the
         //cache pointer from the chain_handle
         break;

      case begin_chain:
         //can never happen because we flag it as nochain2K until either
         //at middle or end_chain
         ASSERT(0,0);
         return(NULL);

      default:
         TRACE((TC_TW,TT_TW_CACHE,"TW: current_write_type is UNKNOWN\n"));
         ASSERT(0,0);
         return(NULL);
      }

      //return and address based on chain_handle
#ifdef WIN16
      lpcache = large_cache_segments[chain_handle / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"TW: using cache segment %lx, ", lpcache));

      lpcache += (chain_handle % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"TW: _2K object address %lx\n", lpcache));

      return(lpcache);
#endif

#if defined( WIN32 ) || defined (RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"TW: _2K object address %lx\n",
            large_cache_segments[0].lpsegment + (chain_handle << 11) ));
      return(large_cache_segments[0].lpsegment +
                        (chain_handle << 11 ));
#endif


   }
   return 0;
}


BOOL finishedTWCacheWrite(UINT size)
{
   BYTE btemp;

   TRACE((TC_TW,TT_TW_CACHE,
   "TW: finishedTWCacheWrite size=%u, current_write_type=",size));

   ASSERT(current_write_type != invalid, 0);

   switch (current_write_type) {
   case random:
      TRACE((TC_TW,TT_TW_CACHE,"random\n"));
      break;

   case small_512:
      TRACE((TC_TW,TT_TW_CACHE,"small_512\n"));
      if (size) {
         //we need to put the size in the current control area
         *(lpcurrent_write_control_area + (current_write_object_handle & 0x03) + 1) =
                                                      (BYTE) (size- 1);
         btemp = 1 << (4 + (current_write_object_handle & 0x03));

         if (size > 256) {       //note that 256 gets represented as 255
            *lpcurrent_write_control_area |= btemp;
         }
         else *lpcurrent_write_control_area &= ~btemp;
      }
      break;

   case nochain2K:
   case end_chain:
      TRACE((TC_TW,TT_TW_CACHE,"nochain2K or end_chain\n"));
      //cant assert on an existing size of 0 and no size because
      //a size field of 0 means 1 because of 0 base 1
      if (size) {
         //we need to put the size in the current control area
         write_word(lpcurrent_write_control_area + 1, size-1);
      }
      break;

   case middle_chain:
      TRACE((TC_TW,TT_TW_CACHE,"middle_chain\n"));
      *lpcurrent_write_control_area = end_chain;
      //cant assert on an existing size of 0 and no size because
      //a size field of 0 means 1 because of 0 base 1
      if (size) {
         //we need to put the size in the current control area
         write_word(lpcurrent_write_control_area + 1, size-1);
      }
      break;

   case begin_chain:
      TRACE((TC_TW,TT_TW_CACHE,"INVALID begin_chain\n"));
      ASSERT(0,0);
      current_write_type = invalid;
      return(FALSE);
      break;

   default:
      TRACE((TC_TW,TT_TW_CACHE,"UNKNOWN\n"));
      ASSERT(0,0);
      current_write_type = invalid;
      return(FALSE);
      break;

   }

   //common processing
   current_write_type = invalid;
   return(TRUE);

}


//see twcache.h for prolog description

LPBYTE lpTWCacheRead(UINT object_handle, CHUNK_TYPE chunk_type,
                     LPUINT lpsize, UINT chain_index)
{
#ifdef WIN16
   LPBYTE   lpcache;
#endif
   LPBYTE lpbyte;
   BYTE   btemp;
   UINT   address_handle;
   UINT   working_chain_index;


   TRACE((TC_TW,TT_TW_CACHE,
        "TW: Cache Read, object handle=%u, chunk_type=",object_handle));

   switch (chunk_type) {
   case _32B:
      current_read_object_handle = 0;
      TRACE((TC_TW,TT_TW_CACHE,"_32B, INVALID\n"));
      ASSERT(0,0)
      return(NULL);

   case monoptr:
      current_read_object_handle = 0;
      TRACE((TC_TW,TT_TW_CACHE,"monoptr\n"));
      ASSERT(object_handle < 8, 0);

      if (*lpcontrol_area != (BYTE) (CACHE_TYPE) random)
         TRACE((TC_TW,TT_TW_CACHE,"TW: inconsistancy of read chunk_type with cache state\n"));

      //address calculated the same for WIN16 and WIN32
      return(large_cache_segments[0].lpsegment + (object_handle << 8));

   case _128B:
      current_read_object_handle = 0;
      TRACE((TC_TW,TT_TW_CACHE,"_128B\n"));

      lpbyte = lpcontrol_area + ((object_handle >> 4) * 5);

      if (*lpbyte != (BYTE) (CACHE_TYPE) random) {
         TRACE((TC_TW,TT_TW_CACHE,"TW: inconsistancy of read chunk_type with cache state\n"));
         ASSERT(0,0);
         return(NULL);
      }

#ifdef WIN16
      lpcache = large_cache_segments[(object_handle >> 4) / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"using cache segment %lx, ", lpcache));

      lpcache += ((object_handle >> 4) % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"TW: 2K chunk address %lx\n", lpcache));

      TRACE((TC_TW,TT_TW_CACHE,"TW: _128B object address %lx\n",
                        lpcache + ((object_handle & 0x0f) << 7)));
      return(lpcache + ((object_handle & 0x0f) << 7));
#endif

#if defined( WIN32 ) || defined(RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"_128B object address %lx\n",
            large_cache_segments[0].lpsegment + (object_handle << 7) ));
      return(large_cache_segments[0].lpsegment +
                        (object_handle << 7) );

#endif

   case _512B:
      current_read_object_handle = 0;
      TRACE((TC_TW,TT_TW_CACHE,"_512B\n"));

      lpbyte = lpcontrol_area + ((object_handle >> 2) * 5);

      if (((*lpbyte) & 0x0f) != (BYTE) (CACHE_TYPE) small_512) {
         TRACE((TC_TW,TT_TW_CACHE,"TW: inconsistancy of read chunk_type with cache state\n"));
         ASSERT(0,0);
         return(NULL);
      }

      //set the size from the control area

      btemp = 1 << (4 + (object_handle & 0x03));
      if (*lpbyte & btemp) {
         *lpsize = 256;
      }
      else *lpsize = 0;

      *lpsize += (*(lpbyte + (object_handle & 0x03) + 1)) + 1;

#ifdef WIN16
      lpcache = large_cache_segments[(object_handle >> 2) / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"TW: using cache segment %lx, ", lpcache));

      lpcache += ((object_handle >> 2) % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"TW: 2K chunk address %lx\n", lpcache));

      TRACE((TC_TW,TT_TW_CACHE,"TW: _512B object address %lx\n",
            lpcache + ((object_handle & 0x03) << 9)));

      return(lpcache + ((object_handle & 0x03) << 9));
#endif

#if defined( WIN32 ) || defined(RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"TW: _512B object address %lx\n",
            large_cache_segments[0].lpsegment + (object_handle << 9) ));
      return(large_cache_segments[0].lpsegment +
                        (object_handle << 9) );
#endif

   case _2K:
      //this is the case where we may have a quick path to the chain handle
      //and therefor to the cache address

      TRACE((TC_TW,TT_TW_CACHE,"_2K\n"));

      address_handle = 0;        //if still 0 then need to do operation
                                 //the slow way even if handles match

      if (current_read_object_handle == object_handle) {
         //o.k., this could be the optimized case
         if (chain_index == current_read_chain_index) {
            //its the same one again
            if ((*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) nochain2K) ||
                (*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) end_chain) )  {
               *lpsize = read_word(lpcurrent_read_control_area + 1) + 1;
            }
            else *lpsize = 0;    //assume begin_chain or middle_chain

            //we can derive the chain handle from the control area index
//jkfix            address_handle = (lpcontrol_area - lpcurrent_read_control_area) /
                   address_handle = (lpcurrent_read_control_area - lpcontrol_area) /
                              5;
            ASSERT(((lpcontrol_area - lpcurrent_read_control_area) % 5) == 0, 0);
         }

         else if (chain_index == current_read_chain_index + 1) {
            //our control area pointer has next area control info
            current_read_chain_index++;

            ASSERT((*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) begin_chain)
                   || (*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) middle_chain),0);

            address_handle = read_word(lpcurrent_read_control_area + 1);
            lpcurrent_read_control_area = lpcontrol_area + (address_handle * 5);

            //figure out what size to return
            if (*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) end_chain) {
               *lpsize = read_word(lpcurrent_read_control_area + 1) + 1;
            }
            else *lpsize = 0;
         }
      }

      //when get here is address handle is nonzero then we are ready to return
      //the address

      if (address_handle == 0) {
         current_read_object_handle = object_handle;
         current_read_chain_index = chain_index;

         working_chain_index = 0;
         address_handle = object_handle;
         lpcurrent_read_control_area = lpcontrol_area + (address_handle * 5);

         //when leave the while loop, address handle has the chain_handle
         //and lp_current_read_control_area points to the control_area
         //represented by the chain_index and the object_handle


         while (working_chain_index != chain_index) {
            address_handle = read_word(lpcurrent_read_control_area + 1);
            lpcurrent_read_control_area = lpcontrol_area + (address_handle * 5);
            working_chain_index++;
         }

         if ((*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) nochain2K) ||
             (*lpcurrent_read_control_area == (BYTE) (CACHE_TYPE) end_chain) )  {
            *lpsize = read_word(lpcurrent_read_control_area + 1) + 1;
         }
         else *lpsize = 0;    //assume begin_chain or middle_chain

      }

      //address handle has the handle of the 2K area
      //return and address based on chain_handle
#ifdef WIN16
      lpcache = large_cache_segments[address_handle / 30].lpsegment;
      TRACE((TC_TW,TT_TW_CACHE,"TW: using cache segment %lx, ", lpcache));

      lpcache += (address_handle % 30) << 11;
      TRACE((TC_TW,TT_TW_CACHE,"TW: _2K object address %lx\n", lpcache));

      return(lpcache);
#endif

#if defined( WIN32 ) || defined (RISCOS)

      TRACE((TC_TW,TT_TW_CACHE,"TW: _2K object address %lx\n",
            large_cache_segments[0].lpsegment + (address_handle << 11) ));
      return(large_cache_segments[0].lpsegment +
                        (address_handle << 11 ));
#endif

   default:
      ASSERT(0,0);
      return(NULL);

   }

}


