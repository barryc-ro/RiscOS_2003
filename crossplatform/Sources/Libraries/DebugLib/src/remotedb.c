/**************************************************************************/
/* File:    remotedb.c                                                    */
/* Purpose: Routines for the RemoteDB output method.                      */
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
#include "globals.h"
#include "remotedb.h"

#ifdef ENABLE_REMOTEDB_SUPPORT

/* -------------------------------------- LOCAL VARIABLES --------------------------------------- */

remote_debug_session *debug_remotedb_info = NULL;           /* RemoteDB handler */

/**********************/
/* Exported functions */


/************************************************************************/
/* debug_remotedb_init                                                  */
/*                                                                      */
/* Function to initialise RemoteDB                                      */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_remotedb_init (void)
{
#ifndef NOSOCKETS
  if (debug_remotedb_info == NULL)
    remote_debug_open (debug_current_options.taskname, (&debug_remotedb_info));
  return true;
#else
  return true;
#endif
}


/************************************************************************/
/* debug_remotedb_output                                                */
/*                                                                      */
/* Function is the output routine for RemoteDB                          */
/*                                                                      */
/* Parameters: buffer   - text to output.                               */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_remotedb_output (const char *buffer, size_t len)
{
#ifndef NOSOCKETS
  if (debug_remotedb_info != NULL)
    remote_debug_print_line (1u, debug_remotedb_info, buffer, len);
#else
  IGNORE (buffer); IGNORE (len);
#endif
}


/************************************************************************/
/* debug_remotedb_quit                                                  */
/*                                                                      */
/* Function to terminate RemoteDB                                       */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void         .                                           */
/*                                                                      */
/************************************************************************/
void debug_remotedb_quit (void)
{
#ifndef NOSOCKETS
  if (debug_remotedb_info != NULL)
  {
    remote_debug_close (debug_remotedb_info);
    debug_remotedb_info = NULL;
  }
#endif
}

#endif /* ENABLE_REMOTEDB_SUPPORT */
