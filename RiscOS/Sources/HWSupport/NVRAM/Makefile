# Makefile for NVRAM
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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
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
INCLUDES = -IC:

#DFLAGS   = -dDEBUG
DFLAGS   =

#
# Program specific options:
#
COMPONENT = NVRAM
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
OBJS      = header.o module.o nvram.o msgfile.o parse.o trace.o
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RDIR      = ${RESDIR}.NVRAM

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} resources.<System>.Tags ${RDIR}.Tags ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

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

${RAMTARGET}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: export.h.${COMPONENT}
        ${CP} export.h.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.module
o.module:	h.nvram
o.module:	h.msgfile
o.module:	h.error
o.module:	h.trace
o.nvram:	c.NVRAM
o.nvram:	C:h.kernel
o.nvram:	C:h.swis
o.nvram:	h.module
o.nvram:	h.nvram
o.nvram:	h.msgfile
o.nvram:	h.parse
o.nvram:	h.error
o.nvram:	h.trace
o.msgfile:	c.msgfile
o.msgfile:	C:h.kernel
o.msgfile:	C:h.swis
o.msgfile:	h.msgfile
o.msgfile:	h.error
o.msgfile:	h.trace
o.parse:	c.parse
o.parse:	h.nvram
o.parse:	C:h.kernel
o.parse:	h.trace
o.trace:	c.trace
o.trace:	C:h.swis
o.trace:	C:h.kernel
