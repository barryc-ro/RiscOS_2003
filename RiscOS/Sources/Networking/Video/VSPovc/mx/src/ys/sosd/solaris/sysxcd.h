/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysxcd.h - OMN System-Dependent Byte Ordering Manipulation
 */

#ifndef SYSXCD_ORACLE
#define SYSXCD_ORACLE

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif

EXTC_START

/*
 * Byte Ordering
 *
 * SYNOPSIS
 * const sword SYSX_BYTE_ORDER = { SYSX_MSB, SYSX_LSB };
 *
 * DESCRIPTION
 * SYSX_BYTE_ORDER should be defined with the appropriate constant
 * indicating the native representation of integers on the platform.
 * Defining this constant correctly will automatically cause the
 * correct definitions to be selected in the rest of this header.
 *
 * We note here, however, that some platforms (notably, Intel x86)
 * offer single instructions that perform byte-swapping.  Also,
 * some platforms have no alignment restrictions.  Thus, many of
 * these macros may be optimized.
 */
#define SYSX_MSB          0             /* most-significant-byte-first order */
#define SYSX_LSB          1            /* least-significant-byte-first order */

#define SYSX_BYTE_ORDER   SYSX_MSB          /* byte ordering on this machine */

/*
 * Byte Swapping
 *
 * SYNOPSIS
 *
 * ub2  sysxGetSwap2(ub1 *buf);
 * ub4  sysxGetSwap4(ub1 *buf);
 * void sysxPutSwap2(ub1 *buf, ub2 val);
 * void sysxPutSwap4(ub1 *buf, ub4 val);
 *
 * DESCRIPTION
 * These routines provide data access to integers that are represented
 * with bytes that are swapped with respect to the native representation.
 * The scalar is always in native form, and the buffer points to the non-
 * native form.
 *
 * sysxGetSwap2() and sysxGetSwap4() read a two- or four-byte integer
 * stored in a swapped form from the position pointed to by buf and
 * return the value.  sysxPutSwap2() and sysxPutSwap4() write val to
 * the position pointed to by buf in swapped form.  The buf pointer
 * does not need any particular alignment.
 *
 * For example, given
 *
 *    buf:  0xaa 0xbb
 *
 * Then, on an LSB machine, sysxGetSwap2() returns the value 0xaabb.
 *
 * In conjunction with the SYSX_BYTE_ORDER symbol, one can force
 * a write of network (MSB) order with the following:
 *
 * if (SYSX_BYTE_ORDER == SYSX_MSB)
 *   *((ub2 *) buf) = val;   // this is safe only if buf is aligned
 * else
 *   sysxPutSwap2(buf, val);
 *
 * A normal compiler will eliminate the compile-time condition check
 * and only one generate code for one or the other assignment.
 */
#ifdef lint

ub2  sysxGetSwap2(ub1 *buf);
ub4  sysxGetSwap4(ub1 *buf);
void sysxPutSwap2(ub1 *buf, ub2 val);
void sysxPutSwap4(ub1 *buf, ub4 val);

#elif SYSX_BYTE_ORDER == SYSX_MSB

#define sysxGetSwap2(buf)      sysxg2(buf, 1, 0)
#define sysxGetSwap4(buf)      sysxg4(buf, 3, 2, 1, 0)
#define sysxPutSwap2(buf, val) sysxp2(buf, val, 1, 0)
#define sysxPutSwap4(buf, val) sysxp4(buf, val, 3, 2, 1, 0)

#elif SYSX_BYTE_ORDER == SYSX_LSB

#define sysxGetSwap2(buf)      sysxg2(buf, 0, 1)
#define sysxGetSwap4(buf)      sysxg4(buf, 0, 1, 2, 3)
#define sysxPutSwap2(buf, val) sysxp2(buf, val, 0, 1)
#define sysxPutSwap4(buf, val) sysxp4(buf, val, 0, 1, 2, 3)

#else
#error "ill-defined SYSX_BYTE_ORDER constant"
#endif

/*
 * Unaligned Access
 *
 * SYNOPSIS
 * ub2  sysxGet2(ub1 *buf);
 * ub4  sysxGet4(ub1 *buf);
 * void sysxPut2(ub1 *buf, ub2 val);
 * void sysxPut4(ub1 *buf, ub4 val);
 *
 * DESCRIPTION
 * These routines provide simple access to integer scalars at unaligned
 * addresses.  The scalar in the buffer is assumed to be in the machine's
 * native representation.
 *
 * For example, given
 *
 *    buf:  0xaa 0xbb
 *
 * Then, on an LSB machine, sysxGet2() returns the value 0xbbaa regardless
 * of the alignment of buf.
 */
#ifdef lint

ub2  sysxGet2(ub1 *buf);
ub4  sysxGet4(ub1 *buf);
void sysxPut2(ub1 *buf, ub2 val);
void sysxPut4(ub1 *buf, ub4 val);

#elif SYSX_BYTE_ORDER == SYSX_MSB

#define sysxGet2(buf)      sysxg2(buf, 0, 1)
#define sysxGet4(buf)      sysxg4(buf, 0, 1, 2, 3)
#define sysxPut2(buf, val) sysxp2(buf, val, 0, 1)
#define sysxPut4(buf, val) sysxp4(buf, val, 0, 1, 2, 3)

#elif SYSX_BYTE_ORDER == SYSX_LSB

#define sysxGet2(buf)      sysxg2(buf, 1, 0)
#define sysxGet4(buf)      sysxg4(buf, 3, 2, 1, 0)
#define sysxPut2(buf, val) sysxp2(buf, val, 1, 0)
#define sysxPut4(buf, val) sysxp4(buf, val, 3, 2, 1, 0)

#else
#error "ill-defined SYSX_BYTE_ORDER constant"
#endif

/*
 * Pointer Alignment
 *
 * SYNOPSIS
 * dvoid *sysxAlignPtr(dvoid *ptr, size_t boundary);
 *
 * DESCRIPTION
 * sysxAlignPtr() aligns the given pointer up to the given boundary.
 * The returned pointer should always be greater than or equal to
 * that which was passed in.
 */
#ifdef lint
dvoid *sysxAlignPtr(dvoid *ptr, size_t boundary);
#else
#define sysxAlignPtr(p, b)  ((dvoid *) ((((ub4) (p)) + ((b)-1)) & ~((b)-1)))
#endif

/*
 * Canonical Extraction Macros
 */
#define sysxg2(buf, i, j) \
  ((ub2) ((((ub2) ((buf)[i])) << 8) | (((ub2) ((buf)[j])))))
#define sysxg4(buf, i, j, k, l) \
  ((ub4) ((((ub4) ((buf)[i])) << 24) | (((ub4) ((buf)[j])) << 16) | \
	  (((ub4) ((buf)[k])) << 8) | ((ub4) (buf)[l])))
#define sysxp2(buf, val, i, j) \
  ((buf)[i] = (ub1) ((val) >> 8), (buf)[j] = (ub1) (val))
#define sysxp4(buf, val, i, j, k, l) \
  ((buf)[i] = (ub1) ((val) >> 24), (buf)[j] = (ub1) ((val) >> 16), \
   (buf)[k] = (ub1) ((val) >> 8), (buf)[l] = (ub1) (val))

/*
 * The following macros are obsolescent.  In place of sysxGetB2 and get
 * sysxGetUaB2(), use
 *
 * (SYSX_BYTE_ORDER == SYSX_MSB ? sysxGet2(buf) : sysxGetSwap2(buf)
 *
 * and similarly for the other macros.
 */

/*
 * NAME
 * sysxGetB2, sysxGetB4, sysxPutB2, sysxPutB4 - host/network conversion
 *
 * SYNOPSIS
 * ub2   sysxGetB2(ub1 *buf);
 * ub4   sysxGetB4(ub1 *buf);
 * void  sysxPutB2(ub1 *buf, ub2 val);
 * void  sysxPutB4(ub1 *buf, ub4 val);
 *
 * DESCRIPTION
 * These routines convert between host integer data and network integer
 * data.  sysxGetB2() and sysxGetB4() are expected to read a two- or four-
 * byte integer stored in network format from the position pointed to by
 * buf and return the value.  sysxPutB2() and sysxPutB4() are expected to
 * write val to the position pointed to by buf in network format.  In any
 * case, buf must always be "naturally aligned" for the data type.
 *
 * Network format for a two-byte integer consists of exactly two bytes
 * where the first byte is the high 8 bits and the second byte is the
 * low 8 bits.  Network format for a four-byte integer is similar.
 * The definitions for the corresponding unaligned access below may
 * be used for the aligned access as well and will work on any platform,
 * but faster implementations may be discovered.
 */
#define SYSX_BYTE_ORDER      SYSX_MSB
#ifdef lint
ub2  sysxGetB2(ub1 *buf);
ub4  sysxGetB4(ub1 *buf);
void sysxPutB2(ub1 *buf, ub2 val);
void sysxPutB4(ub1 *buf, ub4 val);
#else
#define sysxGetB2(buf)       *((ub2 *) (buf))
#define sysxGetB4(buf)       *((ub4 *) (buf))
#define sysxPutB2(buf, val)  (*((ub2 *) (buf)) = (val))
#define sysxPutB4(buf, val)  (*((ub4 *) (buf)) = (val))
#endif

/*
 * NAME
 * sysxGetUaB2, sysxGetUaB4, sysxPutUaB2, sysxPutUaB4 - get/put unaligned data
 *
 * DESCRIPTION
 * These routines perform the same function as above except they handle
 * unaligned pointers.  These may be slower than the above functions, so
 * only use them if your data is unaligned.  Note that these always convert
 * between native form (the scalar value put or returned), and MSB form
 * (the value in the buffer pointer).
 */
#ifdef lint
ub2  sysxGetUaB2(ub1 *buf);
ub4  sysxGetUaB4(ub1 *buf);
void sysxPutUaB2(ub1 *buf, ub2 val);
void sysxPutUaB4(ub1 *buf, ub4 val);
#else
#define sysxGetUaB2(buf) \
  ((ub2) ((((ub2) ((buf)[0])) << 8) | (((ub2) ((buf)[1])))))
#define sysxGetUaB4(buf) \
  ((ub4) ((((ub4) ((buf)[0])) << 24) | (((ub4) ((buf)[1])) << 16) | \
	  (((ub4) ((buf)[2])) << 8) | ((ub4) (buf)[3])))
#define sysxPutUaB2(buf, val)   \
  ((buf)[0] = (ub1) ((val) >> 8), (buf)[1] = (ub1) (val))
#define sysxPutUaB4(buf, val) \
  ((buf)[0] = (ub1) ((val) >> 24), (buf)[1] = (ub1) ((val) >> 16), \
   (buf)[2] = (ub1) ((val) >> 8), (buf)[3] = (ub1) (val))
#endif

EXTC_END
#endif /* SYSXCD_ORACLE */
