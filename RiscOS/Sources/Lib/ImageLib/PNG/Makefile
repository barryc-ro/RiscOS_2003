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
DFLAGS    = -DRISCOS -DPNG_DEBUG=0

#
# Program specific options:
#
COMPONENT  = o.libpng-lib
COMPONENTZ = o.libpng-lzm
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
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

pngtest: o.pngtest ${TARGET} ${CLIB} ^.ZLib.o.zlib_lib
        ${LD} -o pngtest o.pngtest ${TARGET} ${CLIB} ^.ZLib.o.zlib_lib


# Dynamic dependencies:
