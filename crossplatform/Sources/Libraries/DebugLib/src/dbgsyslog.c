/**************************************************************************/
/* File:    BrainLink.c                                                   */
/* Purpose: Routines for the BrainLink output method.                     */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

#include "include.h"
#include "syslog.h"

#ifdef ENABLE_SYSLOG_SUPPORT

#include "globals.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#ifdef HAVE_SYS_SYSLOG_H
#include <sys/syslog.h>
#endif

/* Annoyingly, not all syslogs have this option */
#ifndef LOG_PID
#define LOG_PID (0)
#endif

#ifndef LOG_LOCAL0
#  define DEFAULT_FAC LOG_DAEMON|LOG_PID
#else
#  define DEFAULT_FAC LOG_LOCAL0|LOG_PID
#endif

/************************************************************************/
/* debug_syslog_init                                                    */
/*                                                                      */
/* Function to initialise syslog debugging.                             */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_syslog_init (void)
{
  if (debug_current_options.syslog_param_2
     +debug_current_options.syslog_param_3 == 0)
   {
      debug_current_options.syslog_param_2 = 0;
      debug_current_options.syslog_param_3 = DEFAULT_FAC;
   }

   if (debug_current_options.syslog_level == 0)
   {
     debug_current_options.syslog_level = LOG_DEBUG;
   }

   openlog(debug_current_options.taskname,
           debug_current_options.syslog_param_2,
           debug_current_options.syslog_param_3);

   return true;
}


/************************************************************************/
/* debug_syslog_output                                                  */
/*                                                                      */
/* Function outputs the data from the library to the syslog module.     */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_syslog_output (const char *buffer, size_t len)
{
  /* Not yet implemented */
  syslog(debug_current_options.syslog_level, "%.*s", len, buffer);
}



/************************************************************************/
/* debug_syslog_quit                                                    */
/*                                                                      */
/* Function to terminate syslog                                         */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_syslog_quit (void)
{
  closelog();
}

#endif /* ENABLE_SYSLOG_SUPPORT */
