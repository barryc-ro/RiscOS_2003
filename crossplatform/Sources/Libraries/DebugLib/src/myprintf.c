/**************************************************************************/
/* File:    myprintf.c                                                    */
/* Purpose: Routines for the PowerTV monitor output method.               */
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
#include "myprintf.h"

#ifdef ENABLE_MYPRINTF_SUPPORT

static int (*myPrintf)(const char *, ...) = 0;

/************************************************************************/
/* debug_myprintf_init                                                  */
/*                                                                      */
/* Function to initialise the user printf callback debugging.           */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_myprintf_init (void)
{
  return true;
}


/************************************************************************/
/* debug_set_myprintf_callback                                          */
/*                                                                      */
/* Function to initialise the user printf callback debugging.           */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
void debug_set_myprintf_callback(int (*function)(const char *, ...))
{
  myPrintf = function;
}



/************************************************************************/
/* debug_myprintf_output                                                */
/*                                                                      */
/* Function outputs the data from the library to the user's callback    */
/*                                                                      */
/* Parameters: buffer - text to output.                                 */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_myprintf_output (const char *buffer, size_t len)
{
  /* Not implemented */
  if (myPrintf != 0) {
    (void) myPrintf("%.*s", len, buffer);
  }
}



/************************************************************************/
/* debug_myprintf_quit                                                  */
/*                                                                      */
/* Function to terminate the user printf callback                       */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void                                                     */
/*                                                                      */
/************************************************************************/
void debug_myprintf_quit (void)
{
  /* Nothing to do */
}

#endif /* ENABLE_MYPRINTF_SUPPORT */
