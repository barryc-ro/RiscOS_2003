# Makefile for URL
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 10-Feb-98  SNB    Created
# 06-Oct-98  SNB    Updated to use LocalRes, link 5.08
#

#
# Component specific options:
#
COMPONENT  = URL
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}-tr


RESFSDIR   = Resources.URL.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.URL

#
# Generic options:
#
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
TLD	= tlink
RM	= remove
WIPE	= -wipe
MODSQZ  = modsqz

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS	 = -c -depend !Depend ${INCLUDES} -zM -Wp -ffah -zps1 ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 -UTML -UDIRECTSWI
ROMFLAGS = -DROM
INCLUDES = -ITCPIPLibs:,C:

#
# Libraries
#
ANSILIB	  = CLib:o.ansilib
CLIB	  = CLIB:o.stubs
RLIB	  = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM	  = RISC_OSLib:o.abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm

OBJS =\
 o.directswi\
 o.enumerate\
 o.generic\
 o.geturl\
 o.module\
 o.parseurl\
 o.protocol\
 o.proxy\
 o.readdata\
 o.register\
 o.status\
 o.stop\
 o.svcprint\
 o.utils\
 o.URLHdr

RAM_OBJS =\
 o.directswi\
 o.enumerate\
 o.generic\
 o.geturl\
 o.moduleRAM\
 o.parseurl\
 o.protocol\
 o.proxy\
 o.readdata\
 o.register\
 o.status\
 o.stop\
 o.svcprint\
 o.utils\
 o.URLHdr

DBG_OBJS =\
 o.directswi\
 od.enumerate\
 od.generic\
 od.geturl\
 od.moduleRAM\
 od.parseurl\
 od.protocol\
 od.proxy\
 od.readdata\
 od.register\
 od.status\
 od.stop\
 od.svcprint\
 od.utils\
 o.URLHdr

MSGSF=o.msgs

DBGFLAGS=-DTRACE -DUSE_SYSLOG

#
# Rule patterns
#
.SUFFIXES: .c .s .o .od .i .oi .inst
.c.o:;	  ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.c.i:;    ${CC} ${CFLAGS} ${ROMFLAGS} -C -E $< >$@
.s.o:;	  objasm -depend !Depend -throwback -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} ${ROMFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -p -o $@ $<
.cmhg.h:; ${CMHG} -p -d $@ $<

od.protocol protocol.o: URLHdr.h


#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (RAM)

install: ${RAM_MODULE}
	${MKDIR} ${INSTDIR}.310.Modules.Networking
	${MKDIR} ${INSTDIR}.310.Modules.Networking.URL
        ${CP} ${RAM_MODULE} ${INSTDIR}.310.Modules.Networking.URL.${COMPONENT}
	@echo ${COMPONENT}: Module installed (Disc)

ramtrace: ${DBG_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (RAM - trace)

rom: ${ROM_MODULE} o.dirs
	@echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages LocalRes:ResList LocalRes:images
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	TokenCheck LocalRes:Messages
	${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied to Messages module

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} i
	${MKDIR} oi
	${MKDIR} inst
	create o.dirs

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} od ${WFLAGS}
	${WIPE} i ${WFLAGS}
	${WIPE} oi ${WFLAGS}
	${WIPE} inst ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS}
	${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${TCPIPLIBS} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link: o.dirs
	${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${MSGSF}: LocalRes:Messages LocalRes:ResList
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} -via LocalRes:ResList

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
	${mkdir} rm
	${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
	${MODSQZ} $@
	Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
	${mkdir} rm
	${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} c:o.syslog
	Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DBGFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
