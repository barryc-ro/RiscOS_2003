# Makefile for Cipher
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 18 Jul 1997  RWB 	  New for Cipher
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h

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

# Debug flags

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}

#DFLAGS  = -DUSE_TINY

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
OSLIB      = OSLIB:o.OSLib
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.,TCPIPLibs:

# Program specific options:
#
COMPONENT = Cipher
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS      =	\
o.module	\
o.desfunc	\
o.md5		\
o.header


#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

all: rm.${COMPONENT} dirs
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        ${WIPE} rm.${COMPONENT} ${WFLAGS}
        @echo ${COMPONENT}: cleaned
dirs:
	${MKDIR} o
	${MKDIR} rm
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} dirs
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
	${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}: ${OBJS} ${CLIB} dirs
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        Access rm.${COMPONENT} RW/R

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	h.cipher
o.module:	h.header
o.module:	h.desfunc
o.module:	h.md5
o.desfunc:	c.desfunc
o.desfunc:	h.module
o.desfunc:	h.desfunc
o.desfunc:	h.destbl
o.md5:	c.md5
o.md5:	TCPIPLibs:sys.h.types
o.md5:	TCPIPLibs:sys.h.cdefs
o.md5:	TCPIPLibs:machine.h.endian
o.md5:	TCPIPLibs:machine.h.ansi
o.md5:	TCPIPLibs:machine.h.types
o.md5:	TCPIPLibs:netinet.h.in
o.md5:	h.md5
