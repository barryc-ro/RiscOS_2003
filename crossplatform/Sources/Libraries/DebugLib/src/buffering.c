/**************************************************************************/
/* File:    buffering.c                                                   */
/* Purpose: Buffered output control for DebugLib.                         */
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
#include "debug.h"
#include "globals.h"
#include "buffering.h"

#ifdef MemCheck_MEMCHECK
  #include "MemCheck:MemCheck.h"
#endif


/* ---------------------------------- PRE-PROCESSOR DIRECTIVES ---------------------------------- */

#define DebugBuffer_DA_Name "DebugLib Buffer"

/* -------------------------------------- GLOBAL VARIABLES -------------------------------------- */


/* --------------------------------------- LOCAL VARIABLES -------------------------------------- */

/* In RISC OS builds:
 *   da is the dynamic area handle
 *   size is the da size
 *   base points to the first byte of the d.a.
 * In other builds:
 *   da remains zero at all times
 *   size is the size of the allocated buffer
 *   base points to the first byte of the malloc block which can be freed.
 */
static struct {
  int            enabled;
  int            atexit_registered;
  int            da;
  int            size;
  char          *base;
} debug_buff_status;

#ifdef __riscos
static _kernel_oserror global_er;
#endif

/* ----------------------------------------- FUNCTIONS ------------------------------------------ */


/*******************/
/* Local functions */


/************************************************************************/
/* debug_buffer_terminate                                               */
/*                                                                      */
/* Function will close the dynamic area down.                           */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
static void debug_buffer_terminate(void)
{
  debug_buff_status.enabled = false;

#ifdef __riscos
  if (debug_buff_status.da != 0)
  {
    _kernel_swi_regs       regs;
    _kernel_oserror       *er;
    regs.r[0] = 1;
    regs.r[1] = debug_buff_status.da;

    er = _kernel_swi(OS_DynamicArea, &regs, &regs);

    if (er != NULL)
      printf("Message: %s\n", er->errmess);

    #ifdef MemCheck_MEMCHECK
      MemCheck_UnRegisterMiscBlock (debug_buff_status.base);
    #endif
  }
  debug_buff_status.da = 0;
#else
  if (debug_buff_status.base) {
    free(debug_buff_status.base);
  }
#endif
  debug_buff_status.base = NULL;
}


/************************************************************************/
/* debug_output_buffer_on                                               */
/*                                                                      */
/* Function enables buffering of output in DebugLib.  This is stored in */
/* a dynamic area until such times as the user calls                    */
/* debug_output_buffer_off().                                           */
/*                                                                      */
/* Parameters: flags    - flags word - set to 0.                        */
/*             nbytes   - size of Dynamic Area to create.               */
/*                                                                      */
/* Returns:    _kernel_oserror for error, else NULL (RISC OS)           */
/*             0 for success, -1 on error (anything else)               */
/*                                                                      */
/************************************************************************/
DEBUGLIB_OUTPUT_BUFFER_ON_RETTYPE debug_output_buffer_on(int flags, int nbytes)
{
#ifdef __riscos
  _kernel_oserror       *er = NULL;
  _kernel_oserror       *er_ptr = NULL;
  _kernel_swi_regs       regs;

  IGNORE(flags);

  if (!debug_buff_status.atexit_registered)
    atexit(debug_buffer_terminate);
  debug_buff_status.atexit_registered = 1;

  regs.r[0] = 0;
  regs.r[1] = -1;
  regs.r[2] = nbytes;
  regs.r[3] = -1;
  regs.r[4] = 1<<7;
  regs.r[5] = nbytes;
  regs.r[6] = 0;
  regs.r[7] = 0;
  regs.r[8] = (int) DebugBuffer_DA_Name;

  er = _kernel_swi(OS_DynamicArea, &regs, &regs);

  if (er != NULL)
  {
    global_er.errnum = er->errnum;
    strcpy(global_er.errmess, er->errmess);
    er_ptr = &global_er;
    return er_ptr;
  }
  else
  {
    #ifdef MemCheck_MEMCHECK
      MemCheck_RegisterMiscBlock ((void *)regs.r[3], nbytes);
    #endif
    debug_buff_status.da = regs.r[1];
    debug_buff_status.enabled = true;
    debug_buff_status.size = nbytes;
    debug_buff_status.base = (char *) regs.r[3];
    debug_buff_status.base[0] = '\0';
    return NULL;
  }
#else
  debug_buff_status.da = 0;
  debug_buff_status.enabled = true;
  debug_buff_status.size = nbytes;
  debug_buff_status.base = malloc(nbytes);
  if (debug_buff_status.base) {
    debug_buff_status.base[0] = '\0';
    return 0;
  }
  else {
    errno = ENOMEM;
    return -1;
  }
#endif
}


/************************************************************************/
/* debug_output_buffer_off                                              */
/*                                                                      */
/* Function turns off buffering of output.                              */
/*                                                                      */
/* Parameters: flags   - not used.                                      */
/*             device  - DebugLib stream to use to output the buffer,   */
/*                       and all data from this point on.               */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_output_buffer_off(int flags, debug_device device)
{
  /* Set enabled to false so that debug_output_device & debug_output will do
   * something
   */
  debug_buff_status.enabled = false;

  /* If bit in the flags word is set, we just want to output the debug to
     the current device */
  if (flags & DebugLib_OutputBufferOff_Flag_UseCurrentDevice)
    device = debug_current_options.device;

  /* Set the debug device */
  debug_set_device (device);

  debug_output (0, "", debug_current_options.device, debug_buff_status.base);

  /* Now that everything has been output, terminate the buffer */
  debug_buffer_terminate();
}


/************************************************************************/
/* debug_buffer_startup                                                 */
/*                                                                      */
/* Function prepares the buffering code.                                */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_buffer_startup(void)
{
  debug_buff_status.da = 0;
  debug_buff_status.enabled = false;
  debug_buff_status.size = 0;
  debug_buff_status.base = NULL;
}


/************************************************************************/
/* debug_buffer_stream                                                  */
/*                                                                      */
/* Function buffers the text passed in into the setup dynamic area.     */
/*                                                                      */
/* Parameters: stream.                                                  */
/*                                                                      */
/* Returns:    true or false                                            */
/*                                                                      */
/************************************************************************/
int debug_buffer_stream(const char *stream)
{
  if ((strlen(stream) + strlen(debug_buff_status.base)) < debug_buff_status.size)
  {
    strcat(debug_buff_status.base, stream);
    return true;
  }
  else
  {
    return false;
  }
}

/************************************************************************/
/* debug_buffer_enabled                                                 */
/*                                                                      */
/* Returns enable state of the buffering system                         */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    int.                                                     */
/*                                                                      */
/************************************************************************/
int debug_buffer_enabled(void)
{
  return debug_buff_status.enabled;
}
