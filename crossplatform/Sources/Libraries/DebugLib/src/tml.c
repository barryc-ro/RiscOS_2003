/**************************************************************************/
/* File:    tml.c                                                         */
/* Purpose: Routines for the TML output method.                           */
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

#ifdef ENABLE_TML_SUPPORT

#include "misc.h"
#include "globals.h"
#include "tml.h"

/**********************/
/* Exported functions */


/************************************************************************/
/* debug_tml_init                                                       */
/*                                                                      */
/* Function to initialise TML Podule                                    */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_tml_init (void)
{
  /* Check if the TML module is loaded */
  if (debug_misc_ensure_module ("TML_HostFS") == true)
    return true;
  else
    return false;
}


/************************************************************************/
/* debug_tml_output                                                     */
/*                                                                      */
/* Function outputs the data from the library to the TML podule.        */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_tml_output (const char *buffer, size_t len)
{
  size_t count;

  for (count = 0; count < len; count++)
  {
    _swix (HostFS_WriteC, _IN(0), (int) buffer[count]);

    /* Check to see if \n has been passed.  If so, add a CR */
    if (buffer[count] == '\n')
      _swix (HostFS_WriteC, _IN(0), '\r');
  }
}


/************************************************************************/
/* debug_TML_quit                                                       */
/*                                                                      */
/* Function to terminate TML debugging                                  */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_tml_quit (void)
{
  /* Nothing to do */
}

#endif /* ENABLE_TML_SUPPORT */
