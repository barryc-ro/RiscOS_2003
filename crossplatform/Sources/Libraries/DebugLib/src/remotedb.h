/**************************************************************************/
/* File:    remotedb.h                                                    */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_REMOTEDB_H_INCLUDED
#define DEBUGLIB_REMOTEDB_H_INCLUDED

#ifdef ENABLE_REMOTEDB_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

extern bool debug_remotedb_init (void);
extern void debug_remotedb_output (const char *, size_t);
extern void debug_remotedb_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_remotedb_init NULL
#define debug_remotedb_output NULL
#define debug_remotedb_quit NULL

#endif /* ENABLE_REMOTEDB_SUPPORT */

#endif /* DEBUGLIB_REMOTEDB_H_INCLUDED */
