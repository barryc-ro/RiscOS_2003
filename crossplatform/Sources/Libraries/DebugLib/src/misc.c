/**************************************************************************/
/* File:    misc.c                                                        */
/* Purpose: Routines for the file output method.                          */
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
#include "misc.h"
#include "globals.h"

#ifdef __riscos
#  define ENVVAR_PREFIX "DebugLib$"
#else
#  define ENVVAR_PREFIX "DebugLib_"
#endif

/* Duplicates a string.  Returns a pointer to a malloced copy of the
   string passed as the parameter. */
char *debug_misc_strdup (const char *to_copy)
{
  char *copy = NULL;
  int len;

  if (to_copy == NULL)
    return NULL;

  len = strlen (to_copy) + 1;

  copy = malloc (len);
  if (!copy)
    return NULL;

  return memcpy (copy, to_copy, len);
}


#ifdef __riscos
/* An OS_ReadVarVal veneer */
static void debug_misc_getenv (const char *variable, char *buffer, int buffer_size,
                  int *nbytes)
{
  if (buffer == NULL)
  {
    _kernel_swi_regs r;

     r.r[0] = (int)variable;
     r.r[1] = NULL;
     r.r[2] = -1;
     r.r[3] = 0;
     r.r[4] = 0;

     _kernel_swi (OS_ReadVarVal, &r, &r);

     if (nbytes != NULL)
     {
       if (r.r[2] == 0)
         *nbytes = 0;
       else
       {
         *nbytes = ~r.r[2] + 1;
         internal_dprintf (("__DebugLib", "buf=NULL, nbytes=%d\n", *nbytes));
       }
     }
  }
  else
  {
    int len;

    _swix (OS_ReadVarVal, _INR(0,4) | _OUT(2),
           variable, buffer, buffer_size, 0, 0,
           &len);

    buffer[len] = '\0';

    if (nbytes != NULL)
    {
      *nbytes = len + 1;
      internal_dprintf (("__DebugLib", "buf!=NULL, nbytes=%d\n", *nbytes));
    }
  }
}
#endif


/* Version of debug_misc_getenv that mallocs its own space */
char *debug_misc_getenv_malloc (const char *variable)
{
#ifdef __riscos
  char *str;
  int len;

  debug_misc_getenv (variable, NULL, 0, &len);

  if (len == 0)
    return NULL;
  else
  {
    str = malloc (len);
    debug_misc_getenv (variable, str, len, &len);

    return str;
  }
#else
  char *var = getenv(variable);
  if (!var) {
    return NULL;
  }
  else {
    return debug_misc_strdup(var);
  }
#endif
}

/************************************************************************/
/* debug_misc_ensure_module                                             */
/*                                                                      */
/* Function checks to see if a module is loaded.  Stops debug output    */
/* going to unpredictable places.  RISC OS only                         */
/*                                                                      */
/* Parameters: name - Module name.                                      */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
#ifdef __riscos
bool debug_misc_ensure_module (const char *name)
{
  _kernel_oserror       *er;
  _kernel_swi_regs      regs;

  /* Check that module is present */
  regs.r[0] = 18;
  regs.r[1] = (int) name;

  er = _kernel_swi (OS_Module, &regs, &regs);

  if (er != NULL)
  {
    /* Module not present */
    return false;
  }
  else
  {
    /* Module present */
    return true;
  }
}
#endif

/* Like debug_misc_getenv, but appends the current taskname to the
 * passed variable, and prepends ENVVAR_PREFIX.
 */
char *debug_misc_getenv_task_specific(const char *prefix)
{
  /* Optimise for short tasknames */
  char sysvar[16/* vars we look up */ + sizeof(ENVVAR_PREFIX) + 24 /* task */];
  char *psysvar, *ptasksuffix, *result = NULL;
  size_t prefix_len = strlen(prefix);
  size_t task_len = debug_current_options.taskname ?
    strlen(debug_current_options.taskname) : 0;
  size_t total_len = prefix_len + task_len + sizeof(ENVVAR_PREFIX) + sizeof("_");

  if (total_len > sizeof(sysvar)) {
    psysvar = malloc(total_len);
  }
  else {
    psysvar = sysvar;
  }
  sprintf(psysvar, "%s%s", ENVVAR_PREFIX, prefix);
  ptasksuffix = psysvar + strlen(psysvar);
  if (task_len > 0) {
    sprintf(ptasksuffix, "_%s", debug_current_options.taskname);
  }

  result = debug_misc_getenv_malloc(sysvar);
  if (result == NULL) {
    *ptasksuffix = '\0';
    result = debug_misc_getenv_malloc(sysvar);
  }

  /* Free up the temporary memory if we had any */
  if (psysvar != sysvar) free(psysvar);

  return result;
}
