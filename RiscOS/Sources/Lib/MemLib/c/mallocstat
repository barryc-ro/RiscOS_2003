
#include "heapintern.h"

/* Utility to update current_mallinfo for malloc_stats and mallinfo() */

static void malloc_update_mallinfo()
{
  int i;
  mbinptr b;
  mchunkptr p;
#ifdef DEBUG
  mchunkptr q;
#endif

  size_t avail = chunksize(top);
  int   navail = ((long)(avail) >= (long)MINSIZE)? 1 : 0;

  for (i = 1; i < NAV; ++i)
  {
    b = bin_at(i);
    for (p = last(b); p != b; p = p->bk)
    {
#ifdef DEBUG
      check_free_chunk(p);
      for (q = next_chunk(p);
           q < top && inuse(q) && (long)(chunksize(q)) >= (long)MINSIZE;
           q = next_chunk(q))
        check_inuse_chunk(q);
#endif
      avail += chunksize(p);
      navail++;
    }
  }

  current_mallinfo.ordblks = navail;
  current_mallinfo.uordblks = sbrked_mem - avail;
  current_mallinfo.fordblks = avail;
  current_mallinfo.hblks = n_mmaps;
  current_mallinfo.hblkhd = mmapped_mem;
  current_mallinfo.keepcost = chunksize(top);

}



/*

  malloc_stats:

    Prints on stderr the amount of space obtain from the system (both
    via sbrk and mmap), the maximum amount (which may be more than
    current if malloc_trim and/or munmap got called), the maximum
    number of simultaneous mmap regions used, and the current number
    of bytes allocated via malloc (or realloc, etc) but not yet
    freed. (Note that this is the number of bytes allocated, not the
    number requested. It will be larger than the number requested
    because of alignment and bookkeeping overhead.)

*/

void malloc_stats()
{
  malloc_update_mallinfo();
  fprintf(stderr, "max system bytes = %10u\n",
          (unsigned int)(max_total_mem));
  fprintf(stderr, "system bytes     = %10u\n",
          (unsigned int)(sbrked_mem + mmapped_mem));
  fprintf(stderr, "in use bytes     = %10u\n",
          (unsigned int)(current_mallinfo.uordblks + mmapped_mem));
#if HAVE_MMAP
  fprintf(stderr, "max mmap regions = %10u\n",
          (unsigned int)max_n_mmaps);
#endif
}

/*
  mallinfo returns a copy of updated current mallinfo.
*/

struct mallinfo mALLINFo()
{
  malloc_update_mallinfo();
  return current_mallinfo;
}




/*
  mallopt:

    mallopt is the general SVID/XPG interface to tunable parameters.
    The format is to provide a (parameter-number, parameter-value) pair.
    mallopt then sets the corresponding parameter to the argument
    value if it can (i.e., so long as the value is meaningful),
    and returns 1 if successful else 0.

    See descriptions of tunable parameters above.

*/

#if __STD_C
int mALLOPt(int param_number, int value)
#else
int mALLOPt(param_number, value) int param_number; int value;
#endif
{
  switch(param_number)
  {
    case M_TRIM_THRESHOLD:
      trim_threshold = value; return 1;
    case M_TOP_PAD:
      top_pad = value; return 1;
    case M_MMAP_THRESHOLD:
      mmap_threshold = value; return 1;
    case M_MMAP_MAX:
#if HAVE_MMAP
      n_mmaps_max = value; return 1;
#else
      if (value != 0) return 0; else  n_mmaps_max = value; return 1;
#endif

    default:
      return 0;
  }
}
