# Makefile for PNGLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 1997-06-25 BAL          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
WIPE    = wipe

CCFLAGS = -c -depend !Depend -ffah -throwback ${INCLUDES} ${DFLAGS}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES  = -IC:,^.ZLib
DFLAGS    = -DRISCOS

#
# Program specific options:
#
COMPONENT  = libpng-lib
COMPONENTZ = libpng-lzm
SOURCE     = ...
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.png\
 o.pngerror\
 o.pngget\
 o.pngmem\
 o.pngpread\
 o.pngread\
 o.pngrio\
 o.pngrtran\
 o.pngrutil\
 o.pngset\
 o.pngtrans\
 o.pngwio\
 o.pngwrite\
 o.pngwtran\
 o.pngwutil

OBJSZ =\
 z.png\
 z.pngerror\
 z.pngget\
 z.pngmem\
 z.pngpread\
 z.pngread\
 z.pngrio\
 z.pngrtran\
 z.pngrutil\
 z.pngset\
 z.pngtrans\
 z.pngwio\
 z.pngwrite\
 z.pngwtran\
 z.pngwutil

#
# Rule patterns
#
.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${COMPONENT} ${COMPONENTZ}

#
# RISC OS ROM build rules:
#
clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} z.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

local_dirs:
	${MKDIR} o
	${MKDIR} z

#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o o.${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o o.${COMPONENTZ} ${OBJSZ}

# Dynamic dependencies:
o.png:	c.png
o.png:	h.png
o.png:	h.zlib
o.png:	h.zconf
o.png:	h.pngconf
o.pngerror:	c.pngerror
o.pngerror:	h.png
o.pngerror:	h.zlib
o.pngerror:	h.zconf
o.pngerror:	h.pngconf
o.pngget:	c.pngget
o.pngget:	h.png
o.pngget:	h.zlib
o.pngget:	h.zconf
o.pngget:	h.pngconf
o.pngmem:	c.pngmem
o.pngmem:	h.png
o.pngmem:	h.zlib
o.pngmem:	h.zconf
o.pngmem:	h.pngconf
o.pngpread:	c.pngpread
o.pngpread:	h.png
o.pngpread:	h.zlib
o.pngpread:	h.zconf
o.pngpread:	h.pngconf
o.pngread:	c.pngread
o.pngread:	h.png
o.pngread:	h.zlib
o.pngread:	h.zconf
o.pngread:	h.pngconf
o.pngrio:	c.pngrio
o.pngrio:	h.png
o.pngrio:	h.zlib
o.pngrio:	h.zconf
o.pngrio:	h.pngconf
o.pngrtran:	c.pngrtran
o.pngrtran:	h.png
o.pngrtran:	h.zlib
o.pngrtran:	h.zconf
o.pngrtran:	h.pngconf
o.pngrutil:	c.pngrutil
o.pngrutil:	h.png
o.pngrutil:	h.zlib
o.pngrutil:	h.zconf
o.pngrutil:	h.pngconf
o.pngset:	c.pngset
o.pngset:	h.png
o.pngset:	h.zlib
o.pngset:	h.zconf
o.pngset:	h.pngconf
o.pngtrans:	c.pngtrans
o.pngtrans:	h.png
o.pngtrans:	h.zlib
o.pngtrans:	h.zconf
o.pngtrans:	h.pngconf
o.pngwio:	c.pngwio
o.pngwio:	h.png
o.pngwio:	h.zlib
o.pngwio:	h.zconf
o.pngwio:	h.pngconf
o.pngwrite:	c.pngwrite
o.pngwrite:	h.png
o.pngwrite:	h.zlib
o.pngwrite:	h.zconf
o.pngwrite:	h.pngconf
o.pngwtran:	c.pngwtran
o.pngwtran:	h.png
o.pngwtran:	h.zlib
o.pngwtran:	h.zconf
o.pngwtran:	h.pngconf
o.pngwutil:	c.pngwutil
o.pngwutil:	h.png
o.pngwutil:	h.zlib
o.pngwutil:	h.zconf
o.pngwutil:	h.pngconf
z.png:	c.png
z.png:	h.png
z.png:	h.zlib
z.png:	h.zconf
z.png:	h.pngconf
z.pngerror:	c.pngerror
z.pngerror:	h.png
z.pngerror:	h.zlib
z.pngerror:	h.zconf
z.pngerror:	h.pngconf
z.pngget:	c.pngget
z.pngget:	h.png
z.pngget:	h.zlib
z.pngget:	h.zconf
z.pngget:	h.pngconf
z.pngmem:	c.pngmem
z.pngmem:	h.png
z.pngmem:	h.zlib
z.pngmem:	h.zconf
z.pngmem:	h.pngconf
z.pngpread:	c.pngpread
z.pngpread:	h.png
z.pngpread:	h.zlib
z.pngpread:	h.zconf
z.pngpread:	h.pngconf
z.pngread:	c.pngread
z.pngread:	h.png
z.pngread:	h.zlib
z.pngread:	h.zconf
z.pngread:	h.pngconf
z.pngrio:	c.pngrio
z.pngrio:	h.png
z.pngrio:	h.zlib
z.pngrio:	h.zconf
z.pngrio:	h.pngconf
z.pngrtran:	c.pngrtran
z.pngrtran:	h.png
z.pngrtran:	h.zlib
z.pngrtran:	h.zconf
z.pngrtran:	h.pngconf
z.pngrutil:	c.pngrutil
z.pngrutil:	h.png
z.pngrutil:	h.zlib
z.pngrutil:	h.zconf
z.pngrutil:	h.pngconf
z.pngset:	c.pngset
z.pngset:	h.png
z.pngset:	h.zlib
z.pngset:	h.zconf
z.pngset:	h.pngconf
z.pngtrans:	c.pngtrans
z.pngtrans:	h.png
z.pngtrans:	h.zlib
z.pngtrans:	h.zconf
z.pngtrans:	h.pngconf
z.pngwio:	c.pngwio
z.pngwio:	h.png
z.pngwio:	h.zlib
z.pngwio:	h.zconf
z.pngwio:	h.pngconf
z.pngwrite:	c.pngwrite
z.pngwrite:	h.png
z.pngwrite:	h.zlib
z.pngwrite:	h.zconf
z.pngwrite:	h.pngconf
z.pngwtran:	c.pngwtran
z.pngwtran:	h.png
z.pngwtran:	h.zlib
z.pngwtran:	h.zconf
z.pngwtran:	h.pngconf
z.pngwutil:	c.pngwutil
z.pngwutil:	h.png
z.pngwutil:	h.zlib
z.pngwutil:	h.zconf
z.pngwutil:	h.pngconf
