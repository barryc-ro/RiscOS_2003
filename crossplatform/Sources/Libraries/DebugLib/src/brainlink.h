/**************************************************************************/
/* File:    brainlink.h                                                   */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_BRAINLINK_H_INCLUDED
#define DEBUGLIB_BRAINLINK_H_INCLUDED

#ifdef ENABLE_BRAINLINK_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

extern bool debug_brainlink_init (void);
extern void debug_brainlink_output (const char *buffer, size_t len);
extern void debug_brainlink_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_brainlink_init NULL
#define debug_brainlink_output NULL
#define debug_brainlink_quit NULL

#endif /* ENABLE_BRAINLINK_SUPPORT */

#endif /* DEBUGLIB_BRAINLINK_H_INCLUDED */
