# Makefile for unixlib module linkable version
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 05-May-95  RWB          Mangled.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -pcc -depend !Depend ${INCLUDES} -zM -Darm
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

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
INCLUDES = -IC: -I^ -I^.^.AUN.include

#
# Program specific options:
#
COMPONENT = unixlibzm
TARGET    = unixlibzm
OBJS      =	  \
  o.stubs

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a linkable rpclib
#
all: ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS}
         libfile unixlibzm -c 30 20
         libfile -i unixlibzm o.*
         libfile -o -l unixlibzm


# Dynamic dependencies:
o.stubs:	c.stubs
o.stubs:	^.^.AUN.include.h.pwd
o.stubs:	^.^.AUN.include.sys.h.time
o.stubs:	^.^.AUN.include.sys.h.errno
o.stubs:	^.^.AUN.include.sys.^.h.errno
o.stubs:	C:h.kernel
o.stubs:	C:h.kernel
o.stubs:	C:h.swis
