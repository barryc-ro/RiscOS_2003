#ifndef m_H
#define m_H

/*m.h - redirection for memory allocation functions*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright © 1994 Jonathan Coxhead*/

/*
      OSLib is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

      OSLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
   along with this programme; if not, write to the Free Software
   Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139, U S A.
*/

/*From CLib*/
#include <stdlib.h>

/*From Support*/
#ifndef realloc_H
   #include "realloc.h"
#endif

#ifndef trace_H
   #include "trace.h"
#endif

/*To use this module, you must call m_ALLOC, m_FREE, m_REALLOC in place
   of malloc, free, realloc at ALL PLACES in the programme. Then it is
   possible to change your allocation discipline by changing the values of
   these macros.*/

#if TRACE
   extern void *m_alloc (char *file, int line, int);

   extern void *m_calloc (char *file, int line, int, int);

   extern void m_free (char *file, int line, void *ptr, int size);

   extern void *m_realloc (char *file, int line, void *ptr, int old_size,
         int size);

   extern void m_summary (char *file, int line);

   extern void *m_validate_address (char *file, int line, void *ptr);

   #define m_ALLOC(size) \
         m_alloc (__FILE__, __LINE__, size)

   #define m_CALLOC(count, size) \
         m_calloc (__FILE__, __LINE__, count, size)

   #define m_FREE(ptr, size) \
         m_free (__FILE__, __LINE__, ptr, size)

   #define m_REALLOC(ptr, old_size, size) \
         m_realloc (__FILE__, __LINE__, ptr, old_size, size)

   #define m_SUMMARY() \
         m_summary (__FILE__, __LINE__)

   #define m_VALIDATE_ADDRESS(ptr) \
         m_validate_address (__FILE__, __LINE__, ptr)
#else
   #define m_ALLOC(size)                  malloc (size)
   #define m_CALLOC(count, size)          calloc (count, size)
   #define m_FREE(ptr, size)              free (ptr)
   #define m_REALLOC(ptr, old_size, size) REALLOC (ptr, size)
   #define m_SUMMARY()                    SKIP
   #define m_VALIDATE_ADDRESS(ptr)        ((void *) (ptr))
#endif

#endif
