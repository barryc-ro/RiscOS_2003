/**************************************************************************/
/* File:    serial.h                                                      */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/
#ifndef DEBUGLIB_SERIAL_H_INCLUDED
#define DEBUGLIB_SERIAL_H_INCLUDED

#ifdef ENABLE_SERIAL_SUPPORT

#ifdef __cplusplus
extern "C" {
#endif

/* Default Serial Port Behaviour */
#define SerialPort_DefaultPort  1
#define SerialPort_DefaultSpeed 57600

/* Functions */

extern bool debug_serial_init (void);
extern void debug_serial_output (const char *, size_t);
extern void debug_serial_quit (void);

#ifdef __cplusplus
}
#endif

#else

#define debug_serial_init NULL
#define debug_serial_output NULL
#define debug_serial_quit NULL

#endif /* ENABLE_SERIAL_SUPPORT */

#endif /* DEBUGLIB_SERIAL_H_INCLUDED */
