/**************************************************************************/
/* File:    include.h                                                     */
/* Purpose: General header file for DebugLib.                             */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

#ifndef DEBUGLIB_INCLUDE_H_INCLUDED
#define DEBUGLIB_INCLUDE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_STDBOOL_H
#  include <stdbool.h>
#endif

#ifndef DEBUGLIB
   /* We would like the real declarations please */
#  define DEBUGLIB
#endif

#include <DebugLib/DebugLib.h>

#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#  include <string.h>
#endif

#ifdef HAVE_TIME_H
#  include <time.h>
#endif
#ifdef TIME_WITH_SYS_TIME
#  include <sys/time.h>
#endif

#ifdef HAVE_ERRNO_H
#  include <errno.h>
#endif
#ifdef HAVE_SYS_ERRNO_H
#  include <sys/errno.h>
#endif

#ifdef __riscos
/* CLib 5 Libraries */
#  include "kernel.h"
#  include "swis.h"
#endif

/* Turn on the PDebug code */
#ifndef PDebug_DEBUG
#define PDebug_DEBUG 1
#endif

/* Turn on the RemoteDB code */
#ifndef REMOTE_DEBUG
#define REMOTE_DEBUG 1
#endif

/* Turn on the trace code */
#ifndef Trace_TRACE
#define Trace_TRACE  1
#endif

/* "Unused" macro */
#define IGNORE(a) ((void)(a))

/* DebugLib debugging macro */
#ifdef DEBUG
  #define internal_dprintf(a) _dprintf a
#else
  #define internal_dprintf(a)
#endif

#ifdef __cplusplus
}
#endif


#endif /* DEBUGLIB_INCLUDE_H_INCLUDED */
