/*
 * C compiler file arm/target.h, version 13a
 * Copyright (C) Codemist Ltd., 1988.
 * Copyright (C) Acorn Computers Ltd., 1988
 * Copyright (C) Advanced Risc Machines Ltd., 1991
 */

/*
 * RCS $Revision$
 * Checkin $Date$
 * Revising $Author$
 */

#ifndef _target_LOADED
#define _target_LOADED 1

#define TARGET_IS_ARM 1

#ifndef TARGET_MACHINE
/* Allow for definition of deviant ARMs, such as the APRM, in "options.h" */
#  define TARGET_MACHINE "ARM"
#endif

#define TARGET_PREDEFINES { "__arm", \
                            "__CLK_TCK=100", \
                            "__JMP_BUF_SIZE=22" }

#ifndef SOFTWARE_FLOATING_POINT
#   define TARGET_HAS_IEEE
#endif

#ifndef TARGET_IS_BIG_ENDIAN
#  ifndef TARGET_ENDIANNESS_CONFIGURABLE
#    define TARGET_IS_LITTLE_ENDIAN       1
#  endif
#endif

#ifndef NO_DEBUGGER
#  define TARGET_HAS_DEBUGGER           1
#endif
#define TARGET_HAS_COND_EXEC            1
#define TARGET_HAS_SCALED_ADDRESSING    1
#define TARGET_HAS_NEGATIVE_INDEXING    1
#  define target_scalable(n,m)          1
   /* whether we can scale by n bits for an object of size m */
#define TARGET_HAS_SCALED_OPS           1
#define TARGET_HAS_SWITCH_BRANCHTABLE   1
#define TARGET_HAS_TAILCALL             1
#define TARGET_HAS_TAILCALLR            1
#define TARGET_HAS_FP_LITERALS          1
    /* TARGET_HAS_FP_LITERALS invites fpliteral() which is a fn for the ARM */
#define TARGET_HAS_MULTIPLY             1
#define TARGET_HAS_ROTATE               1
#define TARGET_HAS_BLOCKMOVE            1
#define TARGET_ALLOWS_COMPARE_CSES      1
#define TARGET_FP_ARGS_IN_FP_REGS       1

#define TARGET_GEN_NEEDS_VOLATILE_INFO  1
#define TARGET_INLINES_MONADS           1

#define TARGET_HAS_PROFILE              1
/*#define TARGET_COUNT_IS_PROC          1*/
/* count is not a normal function (no frame required in its caller,     */
/* destroys only r14 and ip).                                           */

#define TARGET_LACKS_HALFWORD_STORE     1
#define TARGET_LACKS_UNSIGNED_FIX       1

#define TARGET_ADDRESSES_UNSIGNED       1
#define TARGET_LDRK_MIN                 (-0xfffL)
#define TARGET_LDRK_MAX                 0xfffL
#define TARGET_LDRFK_MIN                (-0x3fcL)
#define TARGET_LDRFK_MAX                0x3fcL
/* The following mechanism is experimental, but fixes an IP use bug.    */
/* Note that the quantum is always(?) available as (-LDRxK_MAK) & 15.   */
#define TARGET_LDRK_QUANTUM 1
#  define target_ldrk_quantum(len,flt)  ((flt) ? 4 : 1)

/* help please on the 0-1 controversy */
#define R_A1            0L
#define R_F0           16L
#define R_V1            4L
#define NARGREGS        4L

#ifdef TARGET_IS_HELIOS
#define NVARREGS        5L
#define MAXGLOBINTREG   5L
#define R_DP            9L  /* Helios needs a static base register */
#else
#define NVARREGS        8L  /* Includes R_SL & R_FP (only sometimes varregs) */
#define MAXGLOBINTREG   6L
#define TARGET_HAS_BSS  1
   #ifndef TARGET_IS_ACORN_RISC_OS
   #define CONST_DATA_IN_CODE 1 
   #endif
#endif

#define NINTREGS       16L  /* same as smallest fp reg, usually R_F0 */
#define NFLTARGREGS     4L
#define NFLTVARREGS     4L
#define R_FV1           (R_F0+NFLTARGREGS)
#define MAXGLOBFLTREG   4L

#define R_P1            R_A1

/*
 * ALLOCATION_ORDER is pretty suspect - it defines an order for looking at
 * registers and is intended to interact well with the copy-avoidance code.
 * When the copy-avoidance code is better this may not be needed any more.
 * The following lines are a temporary admission of defeat
 */

/* We choose to use SL = 10, FP = 11, IP = 12, SP = 13 here (= APCS_R) so
   that SL and FP (sometimes var regs) are contiguous with the var regs.
 */

#define R_IP                0xcL    /* temp + used in call (nb not necessarily a real register number) */
#define ALLOCATION_ORDER    {0,1,2,3, 12,14, 4,5,6,7,8,9,10,11, \
                             16,17,18,19, 20,21,22,23, \
                             255}

#define R_LR                0xeL    /* link addr in fn calls or work reg */

#define TARGET_SPECIAL_ARG_REG R_IP

/* emphasise non-obvious defaults in mip/defaults.h */
#ifndef alignof_double
#  define alignof_double    4
#endif
#ifndef alignof_struct
#  define alignof_struct    4
#endif

#ifndef COMPILING_ON_ARM
   extern char *target_lib_name(const char *, int32);
#  define target_lib_name_(x,e) target_lib_name(x,e)
#endif
extern char *target_asm_options(int32);
#define target_asm_options_(x) target_asm_options(x)

#ifndef TARGET_HAS_COFF
#  ifndef TARGET_HAS_AOUT
#    define TARGET_HAS_AOF 1
#  endif
#endif

#ifdef TARGET_HAS_AOF
#  define TARGET_HAS_ADCON_AREA 1
#  define TARGET_HAS_MULTIPLE_CODE_AREAS 1
#endif

#ifndef INTEGER_LOAD_MAX_DEFAULT
#  define INTEGER_LOAD_MAX_DEFAULT  2
#endif

#ifndef LDM_REGCOUNT_MAX_DEFAULT
#  define LDM_REGCOUNT_MAX_DEFAULT 16
#endif
#ifndef LDM_REGCOUNT_MIN_DEFAULT
#  define LDM_REGCOUNT_MIN_DEFAULT  3
#endif

#endif

/* end of arm/target.h */
