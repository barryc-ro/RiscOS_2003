/**************************************************************************/
/* File:   misc.h                                                         */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

#ifndef DEBUGLIB_MISC_H_INCLUDED
#define DEBUGLIB_MISC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

char *debug_misc_strdup (const char *);
char *debug_misc_getenv_malloc (const char *);
bool debug_misc_ensure_module (const char *name);
char *debug_misc_getenv_task_specific(const char *prefix);

#ifdef __cplusplus
}
#endif

#endif /* DEBUGLIB_MISC_H_INCLUDED */
