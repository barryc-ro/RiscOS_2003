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
STRIP   = stripdepnd

DFLAGS	 = ${OPTS}

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -Wp -ff ${INCLUDES} -DDONT_BUILD_RESOURCES -DSTRING_SCRIPT ${DFLAGS} ${DEVICE}
CMHGFLAGS = ${DFLAGS}
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
SOCKLIB	   = TCPIPLibs:o.socklib5zm
INETLIB	   = TCPIPLibs:o.inetlibzm
UNIXLIB    = TCPIPLibs:o.unixlibzm
NETLIBS    = ${UNIXLIB} ${INETLIB} ${SOCKLIB}
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz
LIBSD      = <Lib$Dir>.DebugLib.o.debuglibzm
ASMUTILS   = C:AsmUtils.o.AsmUtilsZM

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs: -IC:AsmUtils



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
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -p $< -o $@
.s.o:;	    ${AS} ${AFLAGS} $< $@
.d.o:;	    datatoaof -f $< -o $@ $(Datatoaofflags)

#
# build a relocatable module:
#
all: ${TARGETSA}

h.cmhg_modem: cmhg.HeaderMod
        ${CMHG} ${CMHGFLAGS} -p cmhg.HeaderMod -d h.cmhg_modem

c.module: h.cmhg_modem

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic-resources-get-alias
  BBE_Export_Dir ResModem

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
	${WIPE} aof ${WFLAGS}
	${WIPE} o.* ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${RM} h.cmhg_modem
	${RM} ${TARGET}
	${STRIP}
	@echo ${COMPONENT}: cleaned

resources:
	${MKDIR} ${RESDIR}
	${CP} ${RDIR} ${RESDIR} ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${NETLIBS} ${ROMCSTUBS} ${RLIB}
	${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${LIBS} ${NETLIBS} ${ASMUTILS} ${RLIB} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${TARGETSA}: ${OBJS} ${NETLIBS} ${CLIB}
        ${MKDIR} rm
	${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB} ${LIBSD} ${NETLIBS} ${RLIB} ${ASMUTILS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
