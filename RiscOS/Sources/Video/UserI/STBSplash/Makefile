# Makefile for STBSplash
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
SHRINK  = ShrinkWrapZipCompress
NOP     = @|

AFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${THROWBACK} ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

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

LIBS     =

DFLAGS   = -DUserIF_${USERIF}

#
# Program specific options:
#
COMPONENT = STBSplash
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
OBJS      = o.dynamiclib o.main o.icon o.error o.ModHead
EXPORTS   =
RDIR      = ${RESDIR}.${COMPONENT}

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ -d h.$* $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: ram
        ${NOP}

#
# RISC OS ROM build rules:
#
rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom module built

ram: ${RAMTARGET} dirs
        @echo ${COMPONENT}: ram module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ram
        ${MKDIR} ${INSTDIR}
        ${CP} ${RAMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o      ${WFLAGS}
        ${WIPE} aof    ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} rm     ${WFLAGS}
        ${RM} h.ModHead
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        @echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RDIR}
	${CP} LocalUserIFRes:Screen ${RDIR}.Screen ${CPFLAGS}
	${CP} LocalUserIFRes:STB ${RDIR}.STB ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied
	@echo

dirs:
        ${MKDIR} o
        ${MKDIR} linked
        ${MKDIR} aof
        ${MKDIR} rm
        
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}
	Access $@ rw/r

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

h.ModHead: o.ModHead ;@

# Dynamic dependencies:
