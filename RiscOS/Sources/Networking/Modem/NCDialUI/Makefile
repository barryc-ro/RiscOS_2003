# Makefile for NCDialUI
#

#
# Paths
#
EXP_HDR = <export$dir>

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
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} -DDONT_BUILD_RESOURCES ${DFLAGS} ${OPTS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
Datatoaofflags = -d "" -l _len -dp -s -p

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
LIBSD     = <Lib$Dir>.DebugLib.o.debuglibzm

#
# Include files
#
INCLUDES = -IC:

LIBS     =

DFLAGS   =

#
# Program specific options:
#
COMPONENT = NCDialUI
TARGET    = aof.NCDialUI
TARGETSA  = rm.NCDialUI
OBJS      = o.module o.filegen o.headermod o.swiven
RDIR      = ResModem.Resources.DiallerUI
RESDIR    = <resource$dir>.Resources.DiallerUI
EXPORTS   = 

.SUFFIXES: .d

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.d.o:;      datatoaof -f $< -o $@ $(Datatoaofflags)

#
# build a relocatable module:
#
all: ${TARGETSA}

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

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} h.HeaderMod
        ${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}
        ${CP} ${RDIR} ${RESDIR} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

BBETYPE = ncdialui
bbe-ncdialui: bbe-generic
  BBE_Export_Dir ResModem

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} HeaderMod.h
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${TARGETSA}: ${OBJS} ${CLIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} HeaderMod.h
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
