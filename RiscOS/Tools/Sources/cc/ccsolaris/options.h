/*
 * options.h -- compiler configuration options set at compile time
 * Copyright (C) 1991, 1992 Advanced RISC Machines Ltd. All rights reserved.
 */

#ifndef _options_LOADED
#define _options_LOADED

/*
 * The following conditional settings allow the produced compiler (TARGET)
 * to depend on the HOST (COMPILING_ON) environment.
 * Note that we choose to treat this independently of the target-machine /
 * host-machine issue.
 */

#include "VersionNum"
#define NON_RELEASE_VSN Module_MajorVersion " (Acorn Computers Ltd) " Module_MinorVersion

#define TARGET_ENDIANNESS_CONFIGURABLE 1
#define TARGET_DEFAULT_BIGENDIAN 0       /* 1 => bigendian default */
                                         /* 0 => littleendian default */
                                         /* unset => defaults to host */

#define PCS_DEFAULTS	0
                      /* + PCS_CALLCHANGESPSR */  /* 32 bit */  \
                      /* + PCS_FPE3 */                  \
                      /* + PCS_NOSTACKCHECK */          \
                      /* + PCS_REENTRANT */             \
                      /* + PCS_FPREGARGS */

/*
 * 950216 KWelton
 *
 * Add a couple of IDJ bug fixes - do profile counts inline, rather
 * than calling __fn_entry, __fn_exit; allow -e<letter> to disable
 * some errors
 */
#define PROFILE_COUNTS_INLINE 1
#define DISABLE_ERRORS !

#define TARGET_SYSTEM     "RISC OS"
#define TARGET_IS_RISC_OS 1
#define TARGET_IS_ACORN_RISC_OS
#define TARGET_HAS_DIVREM_FUNCTION 1 /* divide fn also returns remainder.*/
#define TARGET_HAS_DIV_10_FUNCTION 1 /* fast divide by 10                */
                                     /* the last two would be in target.h*/
                                     /* but are OS-dependent too. */

/* #define DO_NOT_EXPLOIT_REGISTERS_PRESERVED_BY_CALLEE 1 */
#define MOVC_KILLS_REGISTER_PRESERVED_BY_CALLEE_EXPLOITATION 1

/* #define TARGET_STACK_MOVES_ONCE 1  / * Experimental option */

#ifndef DRIVER_OPTIONS         /* -D__arm done by TARGET_PREDEFINES */
#  define DRIVER_OPTIONS       {"-D__riscos", "-D__acorn", NULL}
#endif

/* to avoid conflict with host compilers */
#define C_INC_VAR  "ARMINC"
#define C_LIB_VAR  "ARMLIB"

#ifndef RELEASE_VSN
#  define ENABLE_ALL          1 /* -- to enable all debugging options */
#endif

/* mac-specific options - find a better home for these sometime! */
#ifdef macintosh
#  define NO_STATIC_BANNER 1
   pascal void SpinCursor(short increment);        /* copied from CursorCtl.h */
#  define ExecuteOnSourceBufferFill()   SpinCursor(1)
#endif

#endif

/* end of ccixos/options.h */
