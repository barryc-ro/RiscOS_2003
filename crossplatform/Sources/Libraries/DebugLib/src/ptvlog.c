/**************************************************************************/
/* File:    ptvlog.c                                                      */
/* Purpose: Routines for the ptvlog output method.                        */
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
#include "ptvlog.h"

#ifdef ENABLE_PTVLOG_SUPPORT

#include <pk.h>
#include <ptvdefs.h>
#include <monitor.h>
#include <log.h>

log_DeclareID (ptv_log_id);

/************************************************************************/
/* debug_ptvlog_init                                                    */
/*                                                                      */
/* Function to initialise PTV debugging.                                */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_ptvlog_init (void)
{
  log_Register(ptv_log_id, debug_current_options.taskname);
  output_fn = log_output;
  return true;
}


/************************************************************************/
/* debug_ptvlog_output                                                  */
/*                                                                      */
/* Function outputs the data from the library to the PTV module.        */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_ptvlog_output (const char *buffer, size_t len)
{
  log_Message (1, (kLog_Info, ptv_log_id, "%.*s", len, debug_buffer));
}



/************************************************************************/
/* debug_ptvlog_quit                                                    */
/*                                                                      */
/* Function to terminate PTV                                            */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_ptvlog_quit (void)
{
  /* Nothing to do */
}

#endif /* ENABLE_PTVLOG_SUPPORT */
