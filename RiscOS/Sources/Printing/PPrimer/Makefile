# Makefile for PPrimer
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13-Apr-95  JRH          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

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
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM ${INCLUDES} ${DFLAGS} ${OPTS}
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
SOCKLIB   = TCPIPLibs:o.socklibzm
INETLIB   = TCPIPLibs:o.inetlibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm
LIBS      = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB    = <Lib$Dir>.c++lib.o.c++libz
LIBSD     = <Lib$Dir>.DebugLib.o.debuglibzm
ASMUTILS  = C:AsmUtils.o.AsmUtilsZM

#
# Include files
#
INCLUDES = -IC: -IC:AsmUtils

# DFLAGS should be NULL for any non-debugging release
DFLAGS    =

#
# Program specific options:
#
COMPONENT = PPrimer
TARGET    = aof.PPrimer
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RESDIR    = <resource$dir>.Resources.${COMPONENT}
OBJS      =	\
 o.cmos		\
 o.db		\
 o.error	\
 o.file		\
 o.main		\
 o.supported	\
 o.text		\
 o.header	\
 o.misc 	\
 o.module	\
 o.prime

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.PPrimer

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
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} ${TARGET}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}
	TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.PPrimer: ${OBJS} ${CLIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${ASMUTILS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
