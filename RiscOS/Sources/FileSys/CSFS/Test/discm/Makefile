# Makefile for CSFS disc module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-1997  RWB 	  New for CSFS disc module


#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
CD	= dir
DEFMOD  = DefMod
AR	= LibFile

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
#DFLAGS  = -DARM -D_DEBUG
DFLAGS  = -DARM -DUSE_TINY
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

# Program specific options:
#
COMPONENT = DiscM
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.disc	  \
o.discm	  \
o.header  \
o.fs	  \


#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} $< $@
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} dirs

dirs:
        ${MKDIR} o
        ${MKDIR} rm

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
o.disc:	c.disc
o.disc:	C:h.kernel
o.disc:	C:h.swis
o.disc:	h.str
o.disc:	h.etc
o.disc:	h.fs
o.disc:	h.csfs
o.disc:	h.disc
o.discm:	c.discm
o.discm:	C:h.kernel
o.discm:	C:h.swis
o.discm:	h.consts
o.discm:	C:debug.h.remote
o.discm:	h.csfs
o.discm:	h.etc
o.discm:	h.disc
o.discm:	h.discm
o.fs:	c.fs
o.fs:	C:h.kernel
o.fs:	C:h.swis
o.fs:	h.Consts
o.fs:	C:debug.h.remote
o.fs:	h.RMInfo
o.fs:	h.veneers
o.fs:	h.Interface
o.fs:	h.fsentries
o.fs:	h.ModuleWrap
o.fs:	h.str
o.fs:	h.fs
