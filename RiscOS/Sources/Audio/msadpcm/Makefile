# Makefile for msadpcm
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#

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

AFLAGS = -depend !Depend -Stamp -quit ${ADFLAGS} -predefine "Debugging SETL {FALSE}"
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${DFLAGS}
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
INCLUDES = -IOS:,C:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}

#
# Program specific options:
#
COMPONENT = msadpcm
TARGET    = aof.${COMPONENT}
OBJS=	  o.modhdr o.msadpcmasm
ROMOBJS=  o.modhdrz o.msadpcmasm

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

o.modhdrz: s.modhdr
	${AS} ${AFLAGS} s.modhdr $@ -Predefine "ROM SETL {TRUE}"

o.modhdr: s.modhdr
	${AS} ${AFLAGS} s.modhdr $@ -Predefine "ROM SETL {FALSE}"

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

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
${TARGET}: ${ROMOBJS}
        ${LD} -o $@ -aof ${ROMOBJS}
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

rm.${COMPONENT}: ${OBJS}
        ${LD} -o $@ -module ${OBJS}

# Dynamic dependencies:
o.modhdr: s.modhdr
o.modhdr: os:hdr.types
o.modhdr: os:hdr.os
o.modhdr: os:hdr.osmodule
o.modhdr: hdr:soundfile
o.modhdr: OS:Hdr.OSGBPB
o.modhdr: hdr.Proc
o.modhdr: hdr.msadpcm
o.modhdrz: s.modhdr
o.modhdrz: os:hdr.types
o.modhdrz: os:hdr.os
o.modhdrz: os:hdr.osmodule
o.modhdrz: hdr:soundfile
o.modhdrz: OS:Hdr.OSGBPB
o.modhdrz: hdr.Proc
o.modhdrz: hdr.msadpcm
o.msadpcmasm: s.msadpcmasm
o.msadpcmasm: os:hdr.types
o.msadpcmasm: os:hdr.os
o.msadpcmasm: os:hdr.osmodule
o.msadpcmasm: hdr:soundfile
o.msadpcmasm: OS:Hdr.OSGBPB
o.msadpcmasm: hdr.proc
o.msadpcmasm: hdr.msadpcm
