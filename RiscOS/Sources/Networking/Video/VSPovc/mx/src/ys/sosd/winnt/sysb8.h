/* Copyright (c) 1995 by Oracle Corporation.  All Rights Reserved.
 *
 * sysb8.h - Portable 64-bit Signed Integer Data Type
 *
 * FOR NATIVE SB8 USERS
 * void sysb8fromsb8(sysb8 *out, sb8 in);
 * sb8  sysb8tosb8(sysb8 *in);
 * sb8  sb8lit(<64-bit literal>);
 *
 * These two macros convert between the native sb8 type and the portable
 * sysb8 type.  These cause no actual conversion to take place, but serve
 * as markers to indicate the transition between the usage of the portable
 * type to a type that requires native 64-bit support.  They will produce
 * errors on platforms that do not provide such support.  THESE MACROS MAY
 * ONLY BE USED IN COMPONENTS THAT ARE REQUIRED TO RUN ON 64-BIT PLATFORMS.
 * THEIR USAGE IS DISCOURAGED.
 *
 * The macro sb8lit() must be used to surround usage of all 64-bit literals.
 * (e.g. sb8lit(0xf12345678)).  This serves to lexically mark these literals
 * so that compilers will accept and process them correctly.  Most of the
 * time, this involves appending "LL" to the literal; other times, it may
 * simply call for an sb8 cast.  Finally, if the compiler does not support
 * large literals (even though it supports large variables), this macro
 * could translate to a function that converts the stringized form.
 *
 * FOR PORTERS
 * This package is used as the interface to 64-bit integers used by OMS
 * software.  If your platform does not support native 64-bit types,
 * you should simply ensure that SYSB8_NATIVE is not defined.  This will
 * force the usage of the portable 64-bit package provided by OMS.  No
 * other changes should be required.
 *
 * If your platform does support native 64-bit types, ensure that the
 * SYSB8_NATIVE symbol is defined below.  A sample implementation for
 * these definitions is provided below, and it should require little
 * modification.  You will need to define sysb8zero, however.
 *
 * For other functions for which there is no native support, such as
 * sysb8fromstr() or sysb8fmt(), the suggested solution is to implement
 * a function that converts between the ysb8 structure and the native
 * sb8 format, and then use the corresponding function of the ysb8
 * portable library.
 *
 * A full description of each function appears in the man page sysb8(3).
 */

#ifndef SYSB8_ORACLE
#define SYSB8_ORACLE

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif

/*
 * Define SYSB8_NATIVE here if the platform supports 64-bit integers natively.
 * Define SYSB8_PRINT here if the platform supports 64-bit integers natively,
 * and the native sprintf() function is capable of printing these directly.
 */

#ifdef sun /* With GCC */
# define SYSB8_NATIVE
#endif

#ifdef NCUBE
# define SYSB8_NATIVE
# define SYSB8_PRINT
#endif

#ifdef hpux
# define SYSB8_NATIVE
#endif

#ifdef WIN32 /* With MSVC v4.0 */
# define SYSB8_NATIVE
#endif


#ifdef SYSB8_NATIVE

typedef __int64 sysb8;
#define sysb8add(o, u, v)      (*(o) = *(u) + *(v))
#define sysb8sub(o, u, v)      (*(o) = *(u) - *(v))
#define sysb8mul(o, u, v)      (*(o) = *(u) * *(v))
#define sysb8div(o, u, v)      (*(o) = *(u) / *(v))
#define sysb8rem(o, u, v)      (*(o) = *(u) % *(v))
#define sysb8neg(o, u)         (*(o) = (-*(u)))
#define sysb8srl(o, u, b)      (*(o) = (sysb8) \
				(((unsigned __int64) *(u)) >> (b)))
#define sysb8sll(o, u, b)      (*(o) = *(u) << (b))

/*
 * Note we have to use a complex name for the tmp variable in the below
 * macro to assure no one will call the macro with a u, v, q, or r that
 * expands to the same name.
 */
#define sysb8edv(u, v, q, r)   \
  do { sysb8 sysb8edvtmp; sysb8edvtmp = *(u) / *(v); *(r) = *(u) % *(v); \
	 *(q) = sysb8edvtmp; } while (0)

#define sysb8addb4(o, u, b)    (*(o) = *(u) + (sysb8) (b))
#define sysb8mulb4(o, u, b)    (*(o) = *(u) * (sysb8) (b))
#define sysb8ext(o, k)         (*(o) = ((sysb8) (k)))
#define sysb8msk(u)            ((sb4) *(u))
#define sysb8mak(o, h, l)      (*(o) = (((sysb8) (h)) << 32) + (l))
#define sysb8set(o, u)         (*(o) = *(u))
#define sysb8cmp(u, op, v)     (*(u) op *(v))
#define sysb8todbl(u)          ((double) *(u))
#define sysb8fromdbl(u, g)     (*(u) = (sysb8) (g))
#ifdef SYSB8_PRINT
# define sysb8fmt(b, f, v)      (sprintf((b), (f), (*v)), (b))
# define sysb8fromstr(o, s)     (sscanf((s), "%Li", (o)) > 0)
#else
char *sysb8fmt(char *buf, CONST char *fmt, sysb8 *val);
boolean sysb8fromstr(sysb8 *out, CONST char *str);
#endif

externref CONST sysb8 *CONST_W_PTR sysb8zero;

/*
 * The following uses of sb8 will be legal in some RTS components
 * because the platform supports native types.  Note that we use
 * #defines rather than typedefs to avoid conflicts with any possible
 * existing typedefs from sx.h.
 */

#define sysb8fromsb8(o, v)     (*(o) = (sysb8) (v))
#define sysb8tosb8(u)          ((sb8) *(u))
#define sb8lit(v)              v

#ifdef sb8
# undef sb8
#endif
#define sb8 sysb8

#ifdef SB8MAXVAL
# undef SB8MAXVAL
#endif
#define SB8MAXVAL ((sb8)sb8lit(9223372036854775807))

#ifdef SB8MINVAL
# undef SB8MINVAL
#endif
#define SB8MINVAL (-SB8MAXVAL-1)

#else /* SYSB8_NATIVE */

#define sysb8         ysb8
#define sysb8add      ysb8add
#define sysb8sub      ysb8sub
#define sysb8mul      ysb8mul
#define sysb8div      ysb8div
#define sysb8rem      ysb8rem
#define sysb8neg      ysb8neg
#define sysb8srl      ysb8srl
#define sysb8sll      ysb8sll
#define sysb8edv      ysb8edv
#define sysb8addb4    ysb8addb4
#define sysb8mulb4    ysb8mulb4
#define sysb8ext      ysb8ext
#define sysb8msk      ysb8msk
#define sysb8mak      ysb8mak
#define sysb8set      ysb8set
#define sysb8cmp      ysb8cmp
#define sysb8todbl    ysb8todbl
#define sysb8fromdbl  ysb8fromdbl
#define sysb8fmt      ysb8fmt
#define sysb8fromstr  ysb8fromstr
#define sysb8zero     ysb8zero

/*
 * The following uses of sb8 will illegal because the platform does not
 * properly support native types. Note the use of #defines rather than 
 * typedefs to avoid conflicts with any possible existing typedefs from 
 * sx.h.
 */

#ifdef sb8
# undef sb8
#endif
#define sb8 ILLEGAL_USE_OF_SB8S

#ifdef SB8MAXVAL
# undef SB8MAXVAL
#endif
#define SB8MAXVAL ILLEGAL_USE_OF_SB8S

#ifdef SB8MINVAL
# undef SB8MINVAL
#endif
#define SB8MINVAL ILLEGAL_USE_OF_SB8S

#define sysb8fromsb8(o, v)   ILLEGAL_USAGE_OF_SB8S
#define sysb8tosb8(u)        ILLEGAL_USAGE_OF_SB8S

#endif /* SYSB8_NATIVE */

/*
 * Override any _b8 conventions from sx.h.
 * OMS software adopts the following conventions regarding the use of
 * _b8 data types directly:
 *
 *   ub8 and eb8 are always prohibited.
 *
 *   sb8 is prohibited except in certain Real-Time Server components,
 *   where its use is highly discouraged.  It was felt that certain
 *   complex calculations would have their code obscured by the use 
 *   of the portable sysb8 constructs.
 *
 *   The use of sysb8 is required in all other places where a 64-bit
 *   data type is required.  This is especially true for interfaces.
 *
 *   If present, the sb8 data type must be capable of holding 64-bits
 *   or more.  It is not acceptable to merely define this as a 4-byte
 *   quantity.  If the platform can not support all native C operations
 *   on 64-bit integer data types, then a definition for sb8 must not 
 *   be provided.
 */

/* The following _b8 definitions are always illegal. */
#ifdef eb8
# undef eb8
#endif
#define eb8 ILLEGAL_USE_OF_EB8S

#ifdef ub8
# undef ub8
#endif
#define ub8 ILLEGAL_USE_OF_UB8S

#ifdef EB8MAXVAL
# undef EB8MAXVAL
#endif
#define EB8MAXVAL ILLEGAL_USE_OF_EB8S

#ifdef EB8MINVAL
# undef EB8MINVAL
#endif
#define EB8MINVAL ILLEGAL_USE_OF_EB8S

#ifdef UB8MAXVAL
# undef UB8MAXVAL
#endif
#define UB8MAXVAL ILLEGAL_USE_OF_UB8S

#ifdef UB8MINVAL
# undef UB8MINVAL
#endif
#define UB8MINVAL ILLEGAL_USE_OF_UB8S

#ifdef  MINEB8MAXVAL
# undef  MINEB8MAXVAL
#endif
#define MINEB8MAXVAL ILLEGAL_USE_OF_EB8S

#ifdef MAXEB8MINVAL
# undef MAXEB8MINVAL
#endif
#define MAXEB8MINVAL ILLEGAL_USE_OF_EB8S

#ifdef MINUB8MAXVAL
# undef MINUB8MAXVAL
#endif
#define MINUB8MAXVAL ILLEGAL_USE_OF_UB8S

#ifdef MAXUB8MINVAL
# undef MAXUB8MINVAL
#endif
#define MAXUB8MINVAL ILLEGAL_USE_OF_UB8S

#endif /* SYSB8_ORACLE */










