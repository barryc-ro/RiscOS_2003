# Makefile for SCInterface
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Jun-96  RWB 	  New for SCInterface
# 11-Sep-96  RWB	  Corrected a major flaw in ROM build
# 07 May 1997 RWB	  Now supports remote debug library
# 22 May 1997 RWB	  Changed debug build

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
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

DFLAGS  = -DUSE_TINY ${DEBUG}
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = SCInter
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

.SUFFIXES:   .od

OBJS      =	\
o.module	\
o.sctrans	\
o.irqs		\
o.path		\
o.orga		\
o.schlumb	\
o.incard	\
o.header

DOBJS      =	\
od.module	\
od.sctrans	\
od.irqs		\
od.path		\
od.orga		\
od.schlumb	\
od.incard	\
od.header

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@
.c.od:;     ${CC} ${CFLAGS} -o $@ $< -DREMOTE_DEBUG
.cmhg.od:;  ${CMHG} ${CMHGFLAGS} -o $@ $< h.$* -DREMOTE_DEBUG
.s.od:;     ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

debug: rm.${COMPONENT}D

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
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
dirs:
        ${MKDIR} o
        ${MKDIR} od
        ${MKDIR} rm
        ${MKDIR} linked
        ${MKDIR} map
        ${MKDIR} aof
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs
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
rm.${COMPONENT}: ${OBJS} dirs
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        Access rm.${COMPONENT} RW/R

rm.${COMPONENT}D: ${DOBJS} ${REMOTEDB} dirs
        ${LD} -rmf -o $@ ${DOBJS} ${CLIB} ${REMOTEDB}
        Access rm.${COMPONENT}D RW/R

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
o.cmd:	c.cmd
o.cmd:	h.module
o.cmd:	C:debug.h.remote
o.cmd:	h.cmd
o.cmd:	h.sctrans
od.cmd:	c.cmd
od.cmd:	h.module
od.cmd:	C:debug.h.remote
od.cmd:	h.cmd
od.cmd:	h.sctrans
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.sctrans
o.module:	h.module
o.module:	C:debug.h.remote
o.module:	h.irqs
o.module:	h.header
o.module:	h.scinter
o.module:	h.cards
o.module:	h.path
o.sctrans:	c.sctrans
o.sctrans:	h.module
o.sctrans:	C:debug.h.remote
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
o.path:	C:debug.h.remote
o.path:	h.scinter
o.orga:	c.orga
o.orga:	C:h.kernel
o.orga:	C:h.string
o.orga:	h.module
o.orga:	C:debug.h.remote
o.orga:	h.scinter
o.orga:	h.sctrans
o.schlumb:	c.schlumb
o.schlumb:	C:h.kernel
o.schlumb:	C:h.string
o.schlumb:	h.module
o.schlumb:	C:debug.h.remote
o.schlumb:	h.scinter
o.schlumb:	h.sctrans
o.incard:	c.incard
o.incard:	C:h.kernel
o.incard:	C:h.string
o.incard:	h.module
o.incard:	C:debug.h.remote
o.incard:	h.scinter
o.incard:	h.sctrans
od.sctrans:	c.sctrans
od.sctrans:	h.module
od.sctrans:	C:debug.h.remote
od.sctrans:	C:h.kernel
od.sctrans:	h.sctrans
od.irqs: s.irqs
od.irqs: Hdr:ListOpts
od.irqs: Hdr:Macros
od.irqs: Hdr:System
od.irqs: Hdr:SWIs
od.irqs: Hdr:CPU.Generic26
od.irqs: Hdr:IO.GenericIO
od.irqs: Hdr:RISCOS
od.irqs: Hdr:ModHand
od.path:	c.path
od.path:	C:h.kernel
od.path:	C:h.swis
od.path:	h.module
od.path:	C:debug.h.remote
od.path:	h.scinter
od.orga:	c.orga
od.orga:	C:h.kernel
od.orga:	C:h.string
od.orga:	h.module
od.orga:	C:debug.h.remote
od.orga:	h.scinter
od.orga:	h.sctrans
od.schlumb:	c.schlumb
od.schlumb:	C:h.kernel
od.schlumb:	C:h.string
od.schlumb:	h.module
od.schlumb:	C:debug.h.remote
od.schlumb:	h.scinter
od.schlumb:	h.sctrans
od.incard:	c.incard
od.incard:	C:h.kernel
od.incard:	C:h.string
od.incard:	h.module
od.incard:	C:debug.h.remote
od.incard:	h.scinter
od.incard:	h.sctrans
od.module:	c.module
od.module:	C:h.kernel
od.module:	C:h.swis
od.module:	h.sctrans
od.module:	h.module
od.module:	C:debug.h.remote
od.module:	h.irqs
od.module:	h.header
od.module:	h.scinter
od.module:	h.cards
od.module:	h.path
