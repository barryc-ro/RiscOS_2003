/************************************************************************/
/* File:    parse.h                                                     */
/* Purpose: Level string parser thingy                                  */
/*                                                                      */
/* Copyright [1999] Pace Micro Technology PLC.  All rights reserved.    */
/*                                                                      */
/* The copyright in this material is owned by Pace Micro Technology PLC */
/* ("Pace").  This material is regarded as a highly confidential trade  */
/* secret of Pace.  It may not be reproduced, used, sold or in any      */
/* other way exploited or transferred to any third party without the    */
/* prior written permission of Pace.                                    */
/************************************************************************/

#ifndef DEBUGLIB_PARSE_H_INCLUDED
#define DEBUGLIB_PARSE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*debug_level_setter)(const char *, unsigned int);

extern void debug_parse_levels(debug_level_setter, char * /*variable value*/);

#ifndef _INR
   /* These have been copied from RISC OS's swis.h */
#  define _FLAGS     0x10 /*use with _RETURN() or _OUT()*/
#  define _IN(i)     (1U << (i))
#  define _INR(i,j)  (~0 << (i) ^ ~0 << (j) + 1)
#  define _OUT(i)    ((i) != _FLAGS? 1U << 31 - (i): 1U << 21)
#  define _OUTR(i,j) (~0U >> (i) ^ ~0U >> (j) + 1)
#endif

#define _LEVEL_EXCLUDED(i) (_OUT(i))
#define _LEVEL_INCLUDED(i) (_IN(i))

#define _LEVELS_EXCLUDED(i,j) (_OUTR(i,j))
#define _LEVELS_INCLUDED(i,j) (_INR(i,j))

#ifdef __cplusplus
}
#endif

#endif /* DEBUGLIB_PARSE_H_INCLUDED */
