/**************************************************************************/
/* File:    file.c                                                        */
/* Purpose: Routines for the File output method.                          */
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
#include "file.h"

#ifdef ENABLE_FILE_SUPPORT

#ifdef __riscos
#  define DEFAULT_FILENAME "<Wimp$ScrapDir>.DebugLib"
#else
#  define DEFAULT_FILENAME "/tmp/DebugLib"
#endif

/* -------------------------------------- LOCAL VARIABLES --------------------------------------- */

static FILE *debug_file_pointer = NULL;          /* File pointer */

/**********************/
/* Exported functions */


/************************************************************************/
/* debug_file_init                                                      */
/*                                                                      */
/* Function to initialise File Debugging                                */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    TRUE or FALSE.                                           */
/*                                                                      */
/************************************************************************/
bool debug_file_init (void)
{
  char *fname;

  if (debug_file_pointer == NULL)
  {
    /* Device currently not initialised */
    if (strlen (debug_current_options.filename) == 0)
    {
      /* No filename specified at startup, so save in WimpScrap */
      fname = DEFAULT_FILENAME;
    }
    else
    {
      /* Filename specified in debug_initialise() so use it */
      fname = debug_current_options.filename;
    }

    debug_file_pointer = fopen (fname, "a");
  }

  return true;
}


/************************************************************************/
/* debug_file_output                                                    */
/*                                                                      */
/* Function is the output routine for file debug                        */
/*                                                                      */
/* Parameters: buffer   - text to output.                               */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_file_output (const char *buffer, size_t len)
{
  if (debug_file_pointer)
  {
    fwrite (buffer, sizeof (char), len, debug_file_pointer);
    if (debug_current_options.unbuffered_files == true)
      fflush (debug_file_pointer);
  }
}


/************************************************************************/
/* debug_file_quit                                                      */
/*                                                                      */
/* Function to terminate  File Debugging                                */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_file_quit (void)
{
  if (debug_file_pointer)
  {
    fclose (debug_file_pointer);
    debug_file_pointer = NULL;
  }
}

#endif /* ENABLE_FILE_SUPPORT */
