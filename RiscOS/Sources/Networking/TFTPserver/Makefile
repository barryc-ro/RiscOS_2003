# Makefile for Multicaster (application)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Jul-99  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

VPATH = @ arch

ARCH=armriscos

#
# Generic options:
#
AWK     = awk
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
TOUCH   = create

COMPONENT = MultiBoot

EXPORTS = ${EXP_HDR}.h.${COMPONENT}

DEPENDS = -depend !Depend ${THROWBACK}

#
# Internet version compatibility
#

DEFINES = ${COMPAT} ${OPTIONS} -DARCHTYPES="\"${ARCH}.h\""

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -c -Wp ${DEFINES} ${INCLUDES} ${DEPENDS}
CMHGFLAGS = -p ${DEPENDS}

#LDFLAGS = -S symbols
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

DIRS = o._dirs

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIBS  = TCPIPLibs:o.unixlib TCPIPLibs:o.inetlib TCPIPLibs:o.socklib5
DEBUGLIB  = C:DebugLib.o.DebugLib

#
# Include files
#
INCLUDES = -Iarch,TCPIPLibs:,C:

#
# Program specific options:
#
TARGET    = aif.${COMPONENT}
ROMTARGET = aof.${COMPONENT}

OBJS      = tftpserver.o main.o Strcmp_ci.o Strdup.o momanager.o \
mofile.o moobject.o configure.o i18n.o icmpsocket.o ${ARCH}.o

DEBOBJS   =	\
 od.i18n	\
 od.tftpserver	\
 od.main	\
 od.Strcmp_ci	\
 od.Strdup	\
 od.momanager	\
 od.moobject	\
 od.mofile	\
 od.configure	\
 od.icmpsocket	\
 od.armriscos

#
# Rule patterns
#
.SUFFIXES:  .o .od .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;      ${CC} ${CFLAGS} -DDEBUGLIB -g -fn -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -o o.$* $< $@
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build an SNMP agent module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

trace: ${TARGET}D
        @echo ${COMPONENT}: export complete (trace)

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${INETLIBS} ${CLIB} ${DIRS}
	${MKDIR} aif
        ${LD} -o $@ ${LDFLAGS} -aif ${OBJS} ${INETLIBS} ${CLIB}
        Access $@ RW/R

${TARGET}D: ${DEBOBJS} ${INETLIBS} ${DIRS} ${CLIB} ${DEBUGLIB}
	${MKDIR} aif
	${LD} -o $@ ${LDFLAGS} ${DEBOBJS} ${DEBUGLIB} ${INETLIBS} ${CLIB}
	Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${INETLIBS} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${INETLIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${DIRS}:
	${MKDIR} o
	${MKDIR} od
	${TOUCH} $@

# Dynamic dependencies:
