# Makefile for AcornHTTP
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 10-Feb-98  SNB    Created
#

#
# Component specific options:
#
COMPONENT  = HTTP
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

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
MCFLAGS = -C -E

CFLAGS	 = -c -depend !Depend ${INCLUDES} -zM -Wp -ffah -zps1 ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 -DCOOKIE -DCOMPRESSION -UTML ${OPTIONS}
ROMFLAGS = -DROM
INCLUDES = -ITCPIPLibs:,C:,C:zlib
DEBFLAGS = -DTRACE -DUSE_SYSLOG

#
# Libraries
#
ANSILIB	  = CLib:o.ansilib
CLIB	  = CLIB:o.stubs
ZLIB      = C:zlib.o.zlibzm
DEBLIBS   = <syslog$dir>.c-veneer.o.syslog
RLIB	  = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM	  = RISC_OSLib:o.abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm

OBJS =\
 o.config\
 o.connect\
 o.connpool\
 o.cookie\
 o.dates\
 o.decompress\
 o.dns\
 o.generic\
 o.header\
 o.hosttrack\
 o.module\
 o.protocol\
 o.readdata\
 o.security\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.writedata\
 o.HTTPHdr

RAM_OBJS =\
 o.config\
 o.connect\
 o.connpool\
 o.cookie\
 o.dates\
 o.decompress\
 o.dns\
 o.generic\
 o.header\
 o.hosttrack\
 o.moduleRAM\
 o.protocol\
 o.readdata\
 o.security\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.writedata\
 o.HTTPHdr

MCCABE_OBJS =\
 i.config\
 i.connect\
 i.connpool\
 i.cookie\
 i.dates\
 i.decompress\
 i.dns\
 i.generic\
 i.header\
 i.hosttrack\
 i.module\
 i.protocol\
 i.readdata\
 i.security\
 i.ses_ctrl\
 i.start\
 i.status\
 i.stop\
 i.URLclient\
 i.utils\
 i.writedata


DBG_OBJS =\
 od.config\
 od.connect\
 od.connpool\
 od.cookie\
 od.dates\
 od.decompress\
 od.dns\
 od.generic\
 od.header\
 od.hosttrack\
 od.moduleRAM\
 od.protocol\
 od.readdata\
 od.security\
 od.ses_ctrl\
 od.start\
 od.status\
 od.stop\
 od.URLclient\
 od.utils\
 od.writedata\
 o.HTTPHdr

MSGSF=o.msgs

#
# Rule patterns
#
.SUFFIXES: .o .od .i .c

.c.o:;	  ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} ${ROMFLAGS} ${DEBFLAGS} -o $@ $<
.c.i:;    ${CC} ${MCFLAGS} ${CFLAGS} ${ROMFLAGS} >$@ $<
.cmhg.o:; ${CMHG} -o $@ $< -p -d $*.h

HTTPHdr.h: HTTPHdr.o
	  @|

#
# Main rules:
#
#
all: ${RAM_MODULE}
	@echo ${COMPONENT}: Module built (RAM)

ramtrace: ${DBG_MODULE}
	@echo ${COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

mccabe: ${MCCABE_OBJS}
	@echo ${COMPONENT}: McCabe sources build (Disc)

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages
	${MKDIR} ${RESDIR}
	${MKDIR} ${RESDIR}.${COMPONENT}
	${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
	@echo ${COMPONENT}: resource files copied to Messages module

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} aof ${WFLAGS}
	${WIPE} rm ${WFLAGS}
	${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
	${WIPE} linked ${WFLAGS}
	${WIPE} map ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS} ${ZLIB}
	${MKDIR} aof
	${LD} -o $@ -aof ${OBJS} ${TCPIPLIBS} ${ZLIB} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
	${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete

${MSGSF}: LocalRes:Messages
	resgen messages_file ${MSGSF} LocalRes:Messages ${RESFSDIR}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${ZLIB} ${CLIB}
	${mkdir} rm
	${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${ZLIB} ${CLIB}
	Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${ZLIB} ${CLIB} ${DEBLIBS}
	${mkdir} rm
	${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${ZLIB} ${CLIB} ${DEBLIBS}
	Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DEBFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
