/**************************************************************************/
/* File:    printf.c                                                      */
/* Purpose: Routines for the Printf output method.                        */
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
#include "misc.h"
#include "globals.h"
#include "printf.h"

#ifdef ENABLE_PRINTF_SUPPORT

/**********************/
/* Exported functions */


/************************************************************************/
/* debug_printf_output                                                  */
/*                                                                      */
/* Function outputs the data from the library to stdout.                */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_printf_output (const char *buffer, size_t len)
{
#ifdef __riscos
  size_t count;

  /* If we want "Screen cornering (TM)", do it. */
  if (debug_current_options.screen_cornering == 1)
  {
    _swix (OS_WriteC, _IN(0), 4);
    _swix (OS_WriteC, _IN(0), 26);
  }

  for (count = 0; count < len; count++)
  {
    _swix (OS_WriteC, _IN(0), (int) buffer[count]);

    /* Check to see if \n has been passed.  If so, add a CR */
    if (buffer[count] == '\n')
      _swix (OS_WriteC, _IN(0), '\r');
  }
#else
  (void) fwrite(buffer, len, 1, stdout);
#endif
}

/************************************************************************/
/* debug_printf_init                                                    */
/*                                                                      */
/* Function to initialise printf debugging.                             */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_printf_init (void)
{
  return true;
}



/************************************************************************/
/* debug_printf_quit                                                    */
/*                                                                      */
/* Function to terminate printf debugging                               */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_printf_quit (void)
{
  /* Nothing to do */
}

#endif /* ENABLE_PRINTF_SUPPORT */
