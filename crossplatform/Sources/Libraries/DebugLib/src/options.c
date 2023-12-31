/**************************************************************************/
/* File:    options.c                                                     */
/* Purpose: This file contains the routines for debugging options.        */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

/* -------------------------------------- LIBRARY IMPORTS --------------------------------------- */
#include "include.h"
#include "serial.h"
#include "options.h"
#include "globals.h"


void debug_set_trace_device (debug_device device)
{
  debug_current_options.trace_device = device;
}

void debug_set_taskname_prefix (bool on)
{
  debug_current_options.taskname_prefix = on;
}

void debug_set_threadname_prefix (bool on)
{
  debug_current_options.threadname_prefix = on;
}

void debug_set_area_level_prefix (bool on)
{
  debug_current_options.area_level_prefix = on;
}

void debug_set_stamp_debug (bool on)
{
  debug_current_options.stamp_debug = on;
}

void debug_set_screen_cornering (bool on)
{
  debug_current_options.screen_cornering = on;
}

void debug_set_unbuffered_files (bool on)
{
  debug_current_options.unbuffered_files = on;
}

#ifdef ENABLE_SERIAL_SUPPORT
void debug_set_serial_lf (bool on)
{
  debug_current_options.serial_lf = on;
}
#endif /* ENABLE_SERIAL_SUPPORT */

#ifdef ENABLE_SERIAL_SUPPORT
void debug_set_serial_port_speed (int speed)
{
  debug_current_options.serial_port_speed = speed;
}
#endif /* ENABLE_SERIAL_SUPPORT */

#ifdef ENABLE_SERIAL_SUPPORT
void debug_set_serial_port_number (int num)
{
  debug_current_options.serial_port_number = num;
}
#endif /* ENABLE_SERIAL_SUPPORT */

#ifdef ENABLE_SYSLOG_SUPPORT
void debug_set_syslog_openlog_params (int p2, int p3, int lev)
{
  if (p2+p3) {
    debug_current_options.syslog_param_2 = p2;
    debug_current_options.syslog_param_3 = p3;
  }
  if (lev) {
    debug_current_options.syslog_level = lev;
  }
}
#endif /* ENABLE_SYSLOG_SUPPORT */

void debug_set_dump_width (size_t width)
{
  debug_current_options.dump_width = width;
}

void debug_set_area_pad_limit (size_t limit)
{
  debug_current_options.area_pad_limit = limit;
}

/************************************************************************/
/* debug_options_initialise                                             */
/*                                                                      */
/* Function initialises the library's options to default values         */
/*                                                                      */
/* Parameters: none                                                     */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_options_initialise (void)
{
  debug_current_options.taskname = NULL;
  debug_current_options.filename = NULL;
  debug_current_options.device = NULL_OUTPUT;
  debug_current_options.raw_device = NULL_OUTPUT;
  debug_current_options.trace_device = NULL_OUTPUT;
  debug_current_options.taskname_prefix = true;
  debug_current_options.area_level_prefix = false;
  debug_current_options.stamp_debug = false;
  debug_current_options.screen_cornering = false;
  debug_current_options.threadname_prefix = false;
  debug_current_options.unbuffered_files = false;
#ifdef ENABLE_SERIAL_SUPPORT
  debug_current_options.serial_lf = true;
  debug_current_options.serial_port_speed = SerialPort_DefaultSpeed;
  debug_current_options.serial_port_number = SerialPort_DefaultPort;
#endif /* ENABLE_SERIAL_SUPPORT */
#ifdef ENABLE_SYSLOG_SUPPORT
  debug_current_options.syslog_param_2 = 0;
  debug_current_options.syslog_param_3 = 0;
  debug_current_options.syslog_level = 0;
#endif /* ENABLE_SYSLOG_SUPPORT */
  debug_current_options.dump_width = DumpWidth_DefaultWidth;
  debug_current_options.area_pad_limit = AreaPadLimit_DefaultLimit;
}


/************************************************************************/
/* debug_set_options                                                    */
/*                                                                      */
/* Function sets up debuglib options.                                   */
/*                                                                      */
/* Parameters: options - Bit 0 Set,    Enable taskname prefixing (def). */
/*                             Unset,  Disable taskname prefixing.      */
/*                     - Bit 1 Set,    Add \r to serial output (def).   */
/*                             Unset,  No \r.                           */
/*                     - Bit 2 Set,    Enable printf "screen cornering" */
/*                                     i.e. each successive "printf"    */
/*                                     output is sent to the top-left   */
/*                                     corner of the screen (default)   */
/*                             Unset,  Disable "screen cornering"       */
/*                     - Bit 3 Set,    Enable timestamping              */
/*                             Unset,  Disable timestamping (def.)      */
/*                     - Bit 4 Set,    Enable thread id prefixing       */
/*                             Unset,  Disable thread id prefix (def.)  */
/*             sport   - Serial port to use (1|2) - def = 1.            */
/*             sspeed  - Serial port speed (300 - 115200) - def = 9600. */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_set_options (unsigned int options, int sport, int sspeed)
{
  /* Task prefixing */
  if (options & DebugLib_Options_TasknamePrefix_On)
    debug_current_options.taskname_prefix = true;
  else
    debug_current_options.taskname_prefix = false;

  /* Thread prefixing */
  if (options & DebugLib_Options_ThreadnamePrefix_On)
    debug_current_options.threadname_prefix = true;
  else
    debug_current_options.threadname_prefix = false;

#ifdef ENABLE_SERIAL_SUPPORT
  /* Serial port \r */
  if (options & DebugLib_Options_SerialPortLF_On)
    debug_current_options.serial_lf = true;
  else
    debug_current_options.serial_lf = false;

  /* Serial port number */
  if ((sport > 0) && (sport < 3))
    debug_current_options.serial_port_number = sport;

  /* Serial port speed */
  if ((sspeed >= 300) && (sspeed <= 115200))
    debug_current_options.serial_port_speed = sspeed;
#endif /* ENABLE_SERIAL_SUPPORT */

  /* Printf "screen cornering (TM)" */
  if (options & DebugLib_Options_ScreenCornering_On)
    debug_current_options.screen_cornering = true;
  else
    debug_current_options.screen_cornering = false;

  if (options & DebugLib_Options_StampDebug_On)
    debug_current_options.stamp_debug = true;
  else
    debug_current_options.stamp_debug = false;
}


/************************************************************************/
/* debug_get_internal_options                                           */
/*                                                                      */
/* Function reads in the current options into the specified var.        */
/*                                                                      */
/* Parameters: opts   - where to store the options.                     */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_get_internal_options (debug_options *opts)
{
  *opts = debug_current_options;
}


/************************************************************************/
/* debug_set_internal_options                                           */
/*                                                                      */
/* Function sets the current options to whatever the var is set to.     */
/*                                                                      */
/* Parameters: opts   - the options to set to.                          */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_set_internal_options (debug_options opts)
{
  debug_current_options = opts;
}


/************************************************************************/
/* debug_set_internal_options_raw                                       */
/*                                                                      */
/* Function sets the options to nothing.                                */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_set_internal_options_raw (void)
{
  debug_current_options.taskname_prefix = false;
  debug_current_options.area_level_prefix = false;
  debug_current_options.stamp_debug = false;
  debug_current_options.screen_cornering = false;
}
