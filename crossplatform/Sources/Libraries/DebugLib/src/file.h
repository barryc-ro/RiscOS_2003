/**************************************************************************/
/* File:   file.h                                                         */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_FILE_H_INCLUDED
#define DEBUGLIB_FILE_H_INCLUDED

#ifdef ENABLE_FILE_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif


extern bool debug_file_init (void);
extern void debug_file_output (const char *, size_t);
extern void debug_file_quit (void);


#ifdef __cplusplus
}
#endif

#else

#define debug_file_init NULL
#define debug_file_output NULL
#define debug_file_quit NULL

#endif /* ENABLE_FILE_SUPPORT */

#endif /* DEBUGLIB_FILE_H_INCLUDED */
