# Makefile for FTP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 10-Feb-98  SNB    Created
# 06-Oct-98  SNB    Updated to use LocalRes and link 5.08
#

#
# Component specific options:
#
COMPONENT  = FTP
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
MKDIR   = cdir
CC      = ${MEMCHECKCC} cc
CMHG    = cmhg
CP      = copy
LD      = link
TLD	= tlink
RM      = remove
WIPE    = -wipe
AS	= objasm

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -NoCache 
CFLAGS   = -c -depend !Depend ${INCLUDES} -throwback -zM -Wp -ffah -zps1 ${DFLAGS}
DFLAGS   = -D${SYSTEM} -DCOMPAT_INET4 -UTML -UTRACE -DMachine=<Machine>
ROMFLAGS = -DROM
INCLUDES = -ITCPIPLibs:,C:
DEBFLAGS = -DTRACE -DUSE_SYSLOG

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
DEBLIBS   = <syslog$dir>.c-veneer.o.syslog
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm

OBJS =\
 o.asmbits\
 o.base64\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.header\
 o.module\
 o.processdir\
 o.protocol\
 o.readdata\
 o.rewrite\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.FTPHdr

RAM_OBJS =\
 o.asmbits\
 o.base64\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.header\
 o.moduleRAM\
 o.processdir\
 o.protocol\
 o.readdata\
 o.rewrite\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.FTPHdr

DBG_OBJS =\
 o.asmbits\
 od.base64\
 od.commands\
 od.connect\
 od.dns\
 od.generic\
 od.header\
 od.moduleRAM\
 od.processdir\
 od.protocol\
 od.readdata\
 od.rewrite\
 od.ses_ctrl\
 od.start\
 od.status\
 od.stop\
 od.URLclient\
 od.utils\
 o.FTPHdr

MSGSF=o.msgs

IMGFILES=LocalRes:Dir LocalRes:DirGrey LocalRes:File LocalRes:FileGrey

#
# Rule patterns
#
.SUFFIXES: .o .od .c .cmhg .s

.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} ${ROMFLAGS} ${DEBFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -p -o $@ $< -d $*.h
.s.o:;	  ${AS} ${AFLAGS} -o $@ $<

FTPHdr.h: FTPHdr.o
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

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages ${IMGFILES}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TCPIPLIBS} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${MSGSF}: LocalRes:Messages ${IMGFILES} MkRes
        TokenCheck LocalRes:Messages
	MkRes messages_file ${MSGSF} LocalRes ${RESFSDIR}

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
        Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB}
	${mkdir} rm
	${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} ${DEBLIBS}
	Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DEBFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
