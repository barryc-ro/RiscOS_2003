
#include "heapintern.h"

/*

  calloc calls malloc, then zeroes out the allocated chunk.

*/

#if __STD_C
Void_t* cALLOc(size_t n, size_t elem_size)
#else
Void_t* cALLOc(n, elem_size) size_t n; size_t elem_size;
#endif
{
  mchunkptr p;
  size_t csz;

  size_t sz = n * elem_size;
  Void_t* mem = mALLOc (sz);

  if (mem == 0)
    return 0;
  else
  {
    p = mem2chunk(mem);

    if(chunk_is_mmapped(p)) /* no clearing is necessary */
      return mem;

    csz = chunksize(p);
    MALLOC_ZERO(mem, csz - SIZE_SZ);
    return mem;
  }
}
