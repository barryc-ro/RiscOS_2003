/**************************************************************************/
/* File:   debugit.h                                                      */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_DEBUGIT_H_INCLUDED
#define DEBUGLIB_DEBUGIT_H_INCLUDED

#ifdef ENABLE_DEBUGIT_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

/* DebugIt SWIs */
#define DebugIt_WriteC 0x4ba82

extern bool debug_debugit_init (void);
extern void debug_debugit_output (const char *, size_t);
extern void debug_debugit_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_debugit_init NULL
#define debug_debugit_output NULL
#define debug_debugit_quit NULL

#endif /* ENABLE_DEBUGIT_SUPPORT */

#endif /* DEBUGLIB_DEBUGIT_H_INCLUDED */
