/**************************************************************************/
/* File:    Buffering.h                                                   */
/* Purpose: Buffer output header file.                                    */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_BUFFERING_H_INCLUDED
#define DEBUGLIB_BUFFERING_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

extern void debug_buffer_startup (void);
extern int debug_buffer_stream (const char *stream);
extern int debug_buffer_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUGLIB_BUFFERING_H_INCLUDED */
