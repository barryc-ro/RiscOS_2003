# Makefile for SerialCom
#

#
# Paths
#
EXP_HDR = <CExport$Dir>.h

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
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

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
INCLUDES = -IC:

DFLAGS    =
#DFLAGS   = -throwback -DDEBUG=1

#
# Program specific options:
#
COMPONENT = SerialCom
O         = o
TDIR      = aof
TARGET    = ${TDIR}.${COMPONENT}
RAMTDIR   = rm
RAMTARGET = ${RAMTDIR}.${COMPONENT}
LINKEDDIR = linked
MAPDIR    = map
EXPORTS   = ${EXP_HDR}.${COMPONENT}
OBJS      =\
 ${O}.header\
 ${O}.module\
 ${O}.command\
 ${O}.msgfile\
 ${O}.error\
 ${O}.trace

.SUFFIXES: .${O}

#
# Rule patterns
#
.c.${O}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${O}:;   ${CMHG} -o $@ $<
.s.${O}:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ${RAMTARGET}

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
        ${WIPE} ${O} ${WFLAGS}
        ${WIPE} ${LINKEDDIR} ${WFLAGS}
        ${WIPE} ${MAPDIR} ${WFLAGS}
        ${WIPE} ${TDIR} ${WFLAGS}
        ${WIPE} ${RAMTDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${O} ${TDIR}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link: ${LINKEDDIR} ${MAPDIR}
        ${LD} -o ${LINKEDDIR}.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > ${MAPDIR}.${COMPONENT}
        truncate ${MAPDIR}.${COMPONENT} ${LINKEDDIR}.${COMPONENT}
        ${CP} ${LINKEDDIR}.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB} ${O} ${RAMTDIR}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: export.h.${COMPONENT}
#       ${CP} export.h.${COMPONENT} $@ ${CPFLAGS}

${O}:
        ${MKDIR} ${O}

${TDIR}:
        ${MKDIR} ${TDIR}

${RAMTDIR}:
        ${MKDIR} ${RAMTDIR}

${LINKEDDIR}:
        ${MKDIR} ${LINKEDDIR}

${MAPDIR}:
        ${MKDIR} ${MAPDIR}

# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	h.module
o.module:	h.command
o.module:	h.msgfile
o.module:	h.trace
o.command:	c.command
o.command:	C:h.kernel
o.command:	C:h.swis
o.command:	h.module
o.command:	h.command
o.command:	h.error
o.command:	h.msgfile
o.command:	h.trace
o.msgfile:	c.msgfile
o.msgfile:	C:h.kernel
o.msgfile:	C:h.swis
o.msgfile:	h.msgfile
o.msgfile:	h.error
o.msgfile:	h.trace
o.error:	c.error
o.error:	h.error
o.trace:	c.trace
o.trace:	C:h.swis
o.trace:	C:h.kernel
