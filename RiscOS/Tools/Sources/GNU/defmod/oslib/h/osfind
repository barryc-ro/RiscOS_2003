#ifndef osfind_H
#define osfind_H

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

#ifndef types_H
   #include "types.h"
#endif

#ifndef fileswitch_H
   #include "fileswitch.h"
#endif

#ifndef os_H
   #include "os.h"
#endif

#define OSFind_Close 0x0
#define OSFind_Openin 0x40
#define OSFind_OpeninPath 0x41
#define OSFind_OpeninPathVar 0x42
#define OSFind_OpeninNoPath 0x43
#define OSFind_Openout 0x80
#define OSFind_OpenoutPath 0x81
#define OSFind_OpenoutPathVar 0x82
#define OSFind_OpenoutNoPath 0x83
#define OSFind_Openup 0xC0
#define OSFind_OpenupPath 0xC1
#define OSFind_OpenupPathVar 0xC2
#define OSFind_OpenupNoPath 0xC3

#define osfind_PATH                             0x1u
#define osfind_PATH_VAR                         0x2u
#define osfind_NO_PATH                          0x3u
#define osfind_ERROR_IF_ABSENT ((bits) 0x8u)
#define osfind_ERROR_IF_DIR ((bits) 0x4u)

#define xosfind_close(file) _swix (OS_Find, _IN (0) | _IN (1), \
      OSFind_Close, (os_f) (file))

#define xosfind_openin(flags, file_name, path, file) \
   _swix (OS_Find, _IN (0) | _IN (1) | _IN (2) | _OUT (0), \
         OSFind_Openin | (bits) (flags), (char *) (file_name), \
         (char *) (path), (os_f *) (file))

#define xosfind_openout(flags, file_name, path, file) \
   _swix (OS_Find, _IN (0) | _IN (1) | _IN (2) | _OUT (0), \
         OSFind_Openout | (bits) (flags), (char *) (file_name), \
         (char *) (path), (os_f *) (file))

#endif
