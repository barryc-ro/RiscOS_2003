/**************************************************************************/
/* File:   options.h                                                      */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

#ifndef DEBUGLIB_OPTIONS_H_INCLUDED
#define DEBUGLIB_OPTIONS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */

/* Default ddumpbuf width */
#define DumpWidth_DefaultWidth 16u

/* Default padding limit for area name in debug output */
#define AreaPadLimit_DefaultLimit 16u

/* Types */

typedef struct debug_options
{
  char *taskname;
  char *filename;
  debug_device device;
  debug_device raw_device;
  debug_device trace_device;
  bool taskname_prefix;
  bool area_level_prefix;
  bool stamp_debug;
  bool screen_cornering;
  bool unbuffered_files;
  bool threadname_prefix;
  bool serial_lf;
  int serial_port_speed;
  int serial_port_number;
  int syslog_param_2;
  int syslog_param_3;
  int syslog_level;
  size_t dump_width;
  size_t area_pad_limit;
} debug_options;

/* Functions */

void debug_options_initialise (void);
void debug_get_internal_options (debug_options *);
void debug_set_internal_options (debug_options);
void debug_set_internal_options_raw (void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUGLIB_OPTIONS_H_INCLUDED */
