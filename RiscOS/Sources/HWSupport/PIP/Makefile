# Makefile for PIP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Mar-98  AJP          Created.
#
# Paths
#
EXP_HDR = <export$dir>
#
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
WIPE	= -wipe
#
CCFLAGS = -list -c -depend !Depend -ffah -zps1 -zM ${INCLUDES} ${DFLAGS}
CMFLAGS = -depend !Depend
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
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
#
# Include files
#
INCLUDES = -IC:
#
# DFLAGS should be NULL for any non-debugging release
DFLAGS    =
#
# Program specific options:
#
COMPONENT = PIP
SOURCE    = ...
TARGET    = aof.${COMPONENT}
# EXPORTS   = ${EXP_HDR}.${COMPONENT}
RESOURCES = Resources.${LOCALE}.Messages
#
OBJS =\
 o.modhdr\
 o.module\
 o.tl750\
 o.command\
 o.misc\
 o.swihandler\
 o.screen\
 o.podule\
 o.cursor\
 o.fileops\
 o.tile\
 o.pipasm\
 o.msgs
#
c.module: h.modhdr
#
# Rule patterns
#
.SUFFIXES: .o .s .h .cmhg .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<
#
# build a relocatable module:
#
all: rm.${COMPONENT}
#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built
#
export: ${EXPORTS}
        @echo ${COMPONENT}: export complete
#
install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed
#
clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} h.modhdr
        @echo ${COMPONENT}: cleaned
#
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}
#
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB}

# ${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}
# Dynamic dependencies:
