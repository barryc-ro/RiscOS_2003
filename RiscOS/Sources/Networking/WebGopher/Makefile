# Makefile for webgopher
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
EXP_HDR = <Cexport$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CMHG    = cmhg
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ffah -zM ${INCLUDES} ${THROWBACK} ${DFLAGS} -wpc
CPFLAGS = ~cfr~v
CMHGFLAGS = -depend !Depend -p ${THROWBACK}
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
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
DEBUGLIB  = <Lib$Dir>.DebugLib.o.DebugLibzm
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

DIRS      = o._dirs

DFLAGS   = -DCOMPAT_INET4

#
# Program specific options:
#
COMPONENT = webgopher
COMPONENTD= ${COMPONENT}D
TARGET    = aof.${COMPONENT}
OBJS      = modhead.o files.o gopher.o
OBJSD     = modhead.o od.files od.gopher
LIBS      = ${CLIB} ${INETLIB} ${SOCKLIB}
LIBSD     = ${CLIB} ${DEBUGLIB} ${INETLIB} ${SOCKLIB}
EXPORTS   = h.gopherpub

#
# Rule patterns
#
.SUFFIXES:  .o .od .s .h .cmhg .c .z
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;     ${CC} ${CFLAGS} -DDEBUGLIB -Fn -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}
debug: rm.${COMPONENTD}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        ${CP} h.gopherpub ${EXP_HDR}.h.webgopher ${CPFLAGS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${RM} h.ModHead
        ${STRIP}
        @echo ${COMPONENT}: cleaned

${DIRS}:
        @${MKDIR} aof
        @${MKDIR} linked
        @${MKDIR} map
        @${MKDIR} o
        @${MKDIR} od
        @${MKDIR} rm
        create ${DIRS}
        stamp  ${DIRS}

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB} ${DIRS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB} ${SOCKLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Non-debug RAM target
rm.${COMPONENT}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} -o $@ -module ${OBJS} ${LIBS}

# Debug RAM target
rm.${COMPONENTD}: ${OBJSD} ${LIBSD} ${DIRS}
        ${LD} -o $@ -module ${OBJSD} ${LIBSD}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Static dependencies

o.gopher:  h.ModHead
od.gopher: h.ModHead

# Dynamic dependencies:
