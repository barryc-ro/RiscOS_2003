# Makefile for SCInterface
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Jun-96  RWB 	  New for SCInterface
# 11-Sep-96  RWB	  Corrected a major flaw in ROM build

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
INCLUDES = -IC: 

# Program specific options:
#
COMPONENT = SCInter
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS      =	\
o.module	\
o.db		\
o.sctrans	\
o.irqs		\
o.path		\
o.orga		\
o.incard	\
o.schlumb	\
o.header

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} $< $@ 
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
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
rm.${COMPONENT}: ${OBJS} ${SERIALLIB}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB} 

o.header: cmhg.header
        $(CMHG) cmhg.header $@ h.header
        	
${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.sctrans
o.module:	h.module
o.module:	h.irqs
o.module:	h.header
o.module:	h.scinter
o.module:	h.cards
o.module:	h.path
o.db:	c.db
o.sctrans:	c.sctrans
o.sctrans:	h.module
o.sctrans:	C:h.kernel
o.sctrans:	h.sctrans
o.irqs: s.irqs
o.irqs: Hdr:ListOpts
o.irqs: Hdr:Macros
o.irqs: Hdr:System
o.irqs: Hdr:SWIs
o.irqs: Hdr:CPU.Generic26
o.irqs: Hdr:IO.GenericIO
o.irqs: Hdr:RISCOS
o.irqs: Hdr:ModHand
o.path:	c.path
o.path:	C:h.kernel
o.path:	C:h.swis
o.path:	h.module
o.path:	h.scinter
o.orga:	c.orga
o.orga:	C:h.kernel
o.orga:	C:h.string
o.orga:	h.module
o.orga:	h.scinter
o.orga:	h.sctrans
o.incard:	c.incard
o.incard:	C:h.kernel
o.incard:	C:h.string
o.incard:	h.module
o.incard:	h.scinter
o.incard:	h.sctrans
o.schlumb:	c.schlumb
o.schlumb:	C:h.kernel
o.schlumb:	C:h.string
o.schlumb:	h.module
o.schlumb:	h.scinter
o.schlumb:	h.sctrans
