/**************************************************************************/
/* File:    debug.h                                                       */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_DEBUG_H_INCLUDED
#define DEBUGLIB_DEBUG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */

/* Size of DebugLib's internal debug buffer */
#define DEBUG_BUFFER_SIZE 2048

/* Copyright message */
#define COPYRIGHT_MESSAGE PACKAGE " is (c) Pace Micro Technology plc. 1997-2001"

/* Functions */

extern void debug_output (unsigned int, const char *, debug_device, const char *);


#ifdef __cplusplus
}
#endif

#endif /* DEBUGLIB_DEBUG_H_INCLUDED */
