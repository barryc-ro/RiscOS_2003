/**************************************************************************/
/* File:    dadebug.c                                                     */
/* Purpose: Routines for the DADebug output method.                       */
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

#ifdef ENABLE_DADEBUG_SUPPORT

#include "misc.h"
#include "dadebug.h"
#include "Global/SWIs.h"

#define DADebug_GetWriteCAddress (DADebugSWI_Base + 0)

static void (*DADWriteC) (char);

/**********************/
/* Exported functions */


/************************************************************************/
/* debug_dadebug_init                                                   */
/*                                                                      */
/* Function to initialise DADebug module                                */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    TRUE or FALSE.                                           */
/*                                                                      */
/************************************************************************/
bool debug_dadebug_init (void)
{
  /* Check if the DADebug module is loaded */
  if (debug_misc_ensure_module ("DADebug") == true)
  {
    if (_swix (DADebug_GetWriteCAddress, _OUT(0), &DADWriteC) == 0)
    {
      return true;
    }
  }
  return false;
}


/************************************************************************/
/* debug_dadebug_output                                                 */
/*                                                                      */
/* Function outputs the data from the library to the DADebug module.    */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_dadebug_output (const char *buffer, size_t len)
{
  size_t count;

  for (count = 0; count < len; count++)
  {
    /* Check to see if \n is the next character. If so, prefix it with a CR */
    if (buffer[count] == '\n')
    {
      DADWriteC ('\r');
    }

    DADWriteC (buffer[count]);
  }
}

#endif /* ENABLE_DEBUGDA_SUPPORT */
