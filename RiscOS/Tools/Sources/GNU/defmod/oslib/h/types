#ifndef types_H
#define types_H

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

#include <kernel.h>

typedef unsigned int bits, bytes;
typedef int osbool;
typedef unsigned char byte;

#ifndef NULL
   #define NULL ((void *) 0)
#endif

#define FALSE ((osbool) 0)
#define TRUE  ((osbool) 1)

#define NONE ((bits) 0)
#define ALL  (~(bits) 0)

#define SKIP 0
   /*may be used as a "don't care" value for |int|, |... *|, |bits| etc*/

#define SIG_LIMIT 11 /*largest signal number + 1*/

#ifndef EXECUTE_ON_UNIX
#ifndef __swis_h
   #define _C (1U << 29)
   #define _Z (1U << 30)
   #define _N (1U << 31)
#endif
#endif

#define UNKNOWN 1
   /*may be used to declare arrays of unknown size*/

#define AS .
#define ASREF ->
   /*may be used for components of a union*/

#define _ ,
   /*may be used to separate arguments of a macro*/

/*These may be used to suppress compiler warnings*/
#define NOT_USED(x) {x = x;}
#define UNSET(x)    {(void) &x;}

#ifdef  __swi
#undef  __swi
#define __swi(x) extern
#endif

#endif
