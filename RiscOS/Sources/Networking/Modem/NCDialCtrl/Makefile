# Makefile for NCDialCtrl
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR	= cdir
AS	= objasm
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
WIPE	= -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} -DDONT_BUILD_RESOURCES -DSTRING_SCRIPT ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
Datatoaofflags = -d "" -l _len -dp -s -p

#
# Libraries
#
CLIB	  = CLIB:o.stubs
RLIB	  = RISCOSLIB:o.risc_oslib
RSTUBS	  = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM	  = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

LIBS	 = TCPIPLibs:o.inetlibzm

DFLAGS	 =

#
# Program specific options:
#
COMPONENT = NCDialCtrl
TARGET	  = aof.NCDialCtrl
TARGETSA  = rm.NCDialCtrl
OBJS	  = o.headermod o.module o.filegen o.swiven
RDIR	  = ResModem.Resources.NCDialCtrl
RESDIR	  = <resource$dir>.Resources.NCDialCtrl
EXPORTS	  =

.SUFFIXES: .d

#
# Rule patterns
#
.c.o:;	    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} $< -o $@ h.cmhg_modem
.s.o:;	    ${AS} ${AFLAGS} $< $@
.d.o:;	    datatoaof -f $< -o $@ $(Datatoaofflags)

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
	${WIPE} map.* ${WFLAGS}
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RESDIR}
	${CP} ${RDIR} ${RESDIR} ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

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

${TARGETSA}: ${OBJS} ${CLIB}
	${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}
#	${LD} -v -map -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
