# Makefile for DHCP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 22-Mar-99  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = ShoutCast
CMHGNAME  = header  

EXPORTS =

DEPENDS = -depend !Depend ${THROWBACK}

DEFINES = ${OPTIONS}

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ff -c -zM ${DEFINES} ${INCLUDES} ${DEPENDS}
CMHGFLAGS = -p ${DEPENDS}

DIRS = o._dirs

#
# Libraries
#
include Makefiles:ModuleLibs

#
# Include files
#
#INCLUDES = -ITCPIPLibs:, -I<Lib$Dir>.ThirdParty.WSS.BaseLib, -I<Obey$Dir>.^.Common
INCLUDES = -Itbox:,C:,TCPIPLibs:,<Obey$Dir>.^.Common,<Lib$Dir>.ThirdParty.WSS.BaseLib

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

BASELIB   = <Lib$Dir>.ThirdParty.WSS.BaseLib.o.BaseLibZM
WSSLIB    = <Lib$Dir>.ThirdParty.WSS.WSSLib.o.WSSLibZM

OBJS      =	\
 o.main		\
 o.shout     	\
 o.http      	\
 o.tcpbsd	\
 o.gethost_  	\
 o.gethost      \
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build a RAM shoutcast module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
	${MKDIR} ${INSTDIR}
#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


o.header: cmhg.header
	cmhg -o o.header -p -d @.^.Common.h.SHOUTcast cmhg.header

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
