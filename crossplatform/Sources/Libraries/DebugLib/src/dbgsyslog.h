/**************************************************************************/
/* File:   dbgsyslog.h                                                    */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_DEBUGLIBSYSLOG_H_INCLUDED
#define DEBUGLIB_DEBUGLIBSYSLOG_H_INCLUDED

#ifdef ENABLE_SYSLOG_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

extern bool debug_syslog_init (void);
extern void debug_syslog_output (const char *, size_t);
extern void debug_syslog_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_syslog_init NULL
#define debug_syslog_output NULL
#define debug_syslog_quit NULL

#endif /* ENABLE_DEBUGLIBSYSLOG_SUPPORT */

#endif /* DEBUGLIB_DEBUGLIBSYSLOG_H_INCLUDED */
