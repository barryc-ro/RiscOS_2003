/**************************************************************************/
/* File:    dadebug.h                                                     */
/* Purpose: Routines for the DADebug output method.                       */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_DADEBUG_H_INCLUDED
#define DEBUGLIB_DADEBUG_H_INCLUDED

#ifdef ENABLE_DADEBUG_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

extern bool debug_dadebug_init (void);
extern void debug_dadebug_output (const char *, size_t);
extern void debug_dadebug_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_dadebug_init NULL
#define debug_dadebug_output NULL
#define debug_dadebug_quit NULL

#endif /* ENABLE_DADEBUG_SUPPORT */

#endif /* DEBUGLIB_DADEBUG_H_INCLUDED */
