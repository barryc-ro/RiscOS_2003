# Makefile for SampleExp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Mar-1997  RWB 	  Nicked from SCInter

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

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
DFLAGS  =

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
INCLUDES = -IC:

# Program specific options:
#
COMPONENT = SampleExp
TARGET    = aof.${COMPONENT}

OBJS      =	\
o.bits	  	\
o.decode	\
o.decomp	\
o.filter	\
o.frame		\
o.generate	\
o.tables	\


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

#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

dirs:
        ${MKDIR} o
        ${MKDIR} aof
        ${MKDIR} rm
        ${MKDIR} linked
        ${MKDIR} map

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}


#
# Dynamic dependencies:
o.bits:	c.bits
o.bits:	h.mpa
o.bits:	h.bits
o.bits:	h.use_asm
o.decode:	c.decode
o.decode:	h.mpa
o.decode:	h.fxptparams
o.decode:	h.soundtypes
o.decode:	h.frame
o.decode:	h.tables
o.decode:	h.bits
o.decode:	h.decode
o.decomp:	c.decomp
o.decomp:	C:h.kernel
o.decomp:	h.mpa
o.decomp:	h.fxptparams
o.decomp:	h.soundtypes
o.decomp:	h.frame
o.decomp:	h.tables
o.decomp:	h.bits
o.decomp:	h.decode
o.decomp:	h.generate
o.filter:	c.filter
o.filter:	h.mpa
o.filter:	h.fxptparams
o.filter:	h.soundtypes
o.filter:	h.sdata
o.filter:	h.filter
o.filter:	h.use_asm
o.frame:	c.frame
o.frame:	h.mpa
o.frame:	h.frame
o.frame:	h.tables
o.generate:	c.generate
o.generate:	h.mpa
o.generate:	h.fxptparams
o.generate:	h.soundtypes
o.generate:	h.sdata
o.generate:	h.filter
o.generate:	h.generate
o.generate:	h.use_asm
o.tables:	c.tables
o.tables:	h.mpa
o.tables:	h.fxptparams
o.tables:	h.soundtypes
o.tables:	h.frame
o.tables:	h.tables
