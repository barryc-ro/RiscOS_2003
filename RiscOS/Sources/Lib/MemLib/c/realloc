#include "heapintern.h"


/*

  Realloc algorithm:

    If the reallocation is for additional space, and the chunk can be
    extended, it is, else a malloc-copy-free sequence is taken.  There
    are several different ways that a chunk could be extended. All are
    tried:

       * Extending forward into following adjacent free chunk.
       * Shifting backwards, joining preceding adjacent space
       * Both shifting backwards and extending forward.
       * Extending into newly sbrked space


    If the reallocation is for less space, the trailing space is
    lopped off and freed.  Unless the #define REALLOC_ZERO_BYTES_FREES
    is set, realloc with a size argument of zero (re)allocates a
    minimum-sized chunk.

    Chunks that were obtained via mmap cannot be extended or shrunk.
    If their reallocation is for additional space, they are copied.
    If for less, they are just left alone.

    The old unix realloc convention of allowing the last-free'd chunk
    to be used as an argument to realloc is no longer supported.
    I don't know of any programs still relying on this feature,
    and allowing it would also allow too many other incorrect
    usages of realloc to be sensible.


*/


#if __STD_C
Void_t* rEALLOc(Void_t* oldmem, size_t bytes)
#else
Void_t* rEALLOc(oldmem, bytes) Void_t* oldmem; size_t bytes;
#endif
{
  size_t    nb;               /* padded request size */

  mchunkptr oldp;             /* chunk corresponding to oldmem */
  size_t    oldsize;          /* its size */

  mchunkptr newp;             /* chunk to return */
  size_t    newsize;          /* its size */
  Void_t*   newmem;           /* corresponding user mem */

  mchunkptr next;             /* next contiguous chunk after oldp */
  size_t    nextsize;         /* its size */

  mchunkptr prev;             /* previous contiguous chunk before oldp */
  size_t    prevsize;         /* its size */

  mchunkptr remainder;        /* holds split off extra space from newp */
  size_t    remainder_size;   /* its size */

  mchunkptr bck;              /* misc temp for linking */
  mchunkptr fwd;              /* misc temp for linking */

#ifdef REALLOC_ZERO_BYTES_FREES
  if (bytes == 0) { fREe(oldmem); return 0; }
#endif


  /* realloc of null is supposed to be same as malloc */
  if (oldmem == 0) return mALLOc(bytes);

  newp    = oldp    = mem2chunk(oldmem);
  newsize = oldsize = chunksize(oldp);


  nb = request2size(bytes);

#if HAVE_MMAP
  if (chunk_is_mmapped(oldp))
  {
    if(oldsize >= nb) return oldmem; /* do nothing */
    /* Must alloc, copy, free. */
    newmem = mALLOc(bytes);
    if (newmem == 0) return 0; /* propagate failure */
    MALLOC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
    munmap_chunk(oldp);
    return newmem;
  }
#endif

  check_inuse_chunk(oldp);

  if ((long)(oldsize) < (long)(nb))
  {

    /* Try expanding forward */

    next = chunk_at_offset(oldp, oldsize);
    if (next == top || !inuse(next))
    {
      nextsize = chunksize(next);

      /* Forward into top only if a remainder */
      if (next == top)
      {
        if ((long)(nextsize + newsize) >= (long)(nb + MINSIZE))
        {
          newsize += nextsize;
          top = chunk_at_offset(oldp, nb);
          set_head(top, (newsize - nb) | PREV_INUSE);
          set_head_size(oldp, nb);
          return chunk2mem(oldp);
        }
      }

      /* Forward into next chunk */
      else if (((long)(nextsize + newsize) >= (long)(nb)))
      {
        unlink(next, bck, fwd);
        newsize  += nextsize;
        goto split;
      }
    }
    else
    {
      next = 0;
      nextsize = 0;
    }

    /* Try shifting backwards. */

    if (!prev_inuse(oldp))
    {
      prev = prev_chunk(oldp);
      prevsize = chunksize(prev);

      /* try forward + backward first to save a later consolidation */

      if (next != 0)
      {
        /* into top */
        if (next == top)
        {
          if ((long)(nextsize + prevsize + newsize) >= (long)(nb + MINSIZE))
          {
            unlink(prev, bck, fwd);
            newp = prev;
            newsize += prevsize + nextsize;
            newmem = chunk2mem(newp);
            MALLOC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
            top = chunk_at_offset(newp, nb);
            set_head(top, (newsize - nb) | PREV_INUSE);
            set_head_size(newp, nb);
            return chunk2mem(newp);
          }
        }

        /* into next chunk */
        else if (((long)(nextsize + prevsize + newsize) >= (long)(nb)))
        {
          unlink(next, bck, fwd);
          unlink(prev, bck, fwd);
          newp = prev;
          newsize += nextsize + prevsize;
          newmem = chunk2mem(newp);
          MALLOC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
          goto split;
        }
      }

      /* backward only */
      if (prev != 0 && (long)(prevsize + newsize) >= (long)nb)
      {
        unlink(prev, bck, fwd);
        newp = prev;
        newsize += prevsize;
        newmem = chunk2mem(newp);
        MALLOC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
        goto split;
      }
    }

    /* Must allocate */

    newmem = mALLOc (bytes);

    if (newmem == 0)  /* propagate failure */
      return 0;

    /* Avoid copy if newp is next chunk after oldp. */
    /* (This can only happen when new chunk is sbrk'ed.) */

    if ( (newp = mem2chunk(newmem)) == next_chunk(oldp))
    {
      newsize += chunksize(newp);
      newp = oldp;
      goto split;
    }

    /* Otherwise copy, free, and exit */
    MALLOC_COPY(newmem, oldmem, oldsize - SIZE_SZ);
    fREe(oldmem);
    return newmem;
  }

 split:  /* split off extra room in old or expanded chunk */

  if (newsize - nb >= MINSIZE) /* split off remainder */
  {
    remainder = chunk_at_offset(newp, nb);
    remainder_size = newsize - nb;
    set_head_size(newp, nb);
    set_head(remainder, remainder_size | PREV_INUSE);
    set_inuse_bit_at_offset(remainder, remainder_size);
    fREe(chunk2mem(remainder)); /* let free() deal with it */
  }
  else
  {
    set_head_size(newp, newsize);
    set_inuse_bit_at_offset(newp, newsize);
  }

  check_inuse_chunk(newp);
  return chunk2mem(newp);
}

