/**************************************************************************/
/* File:    printf.h                                                      */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_PRINTF_H_INCLUDED
#define DEBUGLIB_PRINTF_H_INCLUDED

#ifdef ENABLE_PRINTF_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif


extern bool debug_printf_init (void);
extern void debug_printf_output (const char *, size_t);
extern void debug_printf_quit (void);


#ifdef __cplusplus
}
#endif

#else

#define debug_printf_init NULL
#define debug_printf_output NULL
#define debug_printf_quit NULL

#endif /* ENABLE_PRINTF_SUPPORT */

#endif /* DEBUGLIB_PRINTF_H_INCLUDED */
