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
#include "brainlink.h"

#ifdef ENABLE_BRAINLINK_SUPPORT

#ifndef BrainLink_Debug
#  define BrainLink_Debug (0x055280u)
#endif

/************************************************************************/
/* debug_brainlink_init                                                 */
/*                                                                      */
/* Function to initialise BrainLink debugging.                          */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_brainlink_init (void)
{
  return true;
}


/************************************************************************/
/* debug_brainlink_output                                               */
/*                                                                      */
/* Function outputs the data from the library to the BrainLink module.  */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_brainlink_output (const char *buffer, size_t len)
{
  (void) _swix (BrainLink_Debug, _INR (0, 2), 0, buffer, (unsigned int)len);
}



/************************************************************************/
/* debug_brainlink_quit                                                 */
/*                                                                      */
/* Function to terminate BrainLink                                      */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_brainlink_quit (void)
{
  /* Nothing to do */
}

#endif /* ENABLE_BRAINLINK_SUPPORT */
