# Makefile for AcornSSL
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 20-Mar-98  SNB    Created
#

#
# Component specific options:
#
COMPONENT  = AcornSSL
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}tr


RESFSDIR   = Resources.URL.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.URL

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
TLD	= tlink
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c -depend !Depend ${INCLUDES} -wp -zM -ffah -zps1 ${DFLAGS} -Wd
DFLAGS   = -D${SYSTEM} -DCOMPAT_INET4 -DRISCOS -DNO_DES -DNO_IDEA
ROMFLAGS = -DROM
INCLUDES = -ITCPIPLibs:,SSL:,C:
DEBFLAGS = -DTRACE -DUSE_SYSLOG

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm
CRYPTLIBS = <Lib$Dir>.SSLeay.o.ssl <Lib$Dir>.SSLeay.o.libcrypto
DEBLIBS   = <syslog$dir>.c-veneer.o.syslog


OBJS =\
 o.acornssl\
 o.module\
 o.protocol\
 o.ses_ctrl\
 o.utils\
 o.SSLHdr

RAM_OBJS =\
 o.acornssl\
 o.moduleRAM\
 o.protocol\
 o.ses_ctrl\
 o.utils\
 o.SSLHdr

DBG_OBJS =\
 od.acornssl\
 od.moduleRAM\
 od.protocol\
 od.ses_ctrl\
 od.utils\
 o.SSLHdr

MSGSF=o.msgs

#
# Rule patterns
#
.SUFFIXES: .o .od .c

.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} ${ROMFLAGS} ${DEBFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $< -d $*.h

SSLHdr.h: SSLHdr.o
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

resources: Resources.${LOCALE}.Messages
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.* ${RESDIR}.${COMPONENT}.* ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${INETLIB} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${INETLIB} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM} \
              -map > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${MSGSF}: @.Resources.${LOCALE}.Messages
	resgen messages_file ${MSGSF} Resources.${LOCALE}.Messages ${RESFSDIR}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} ${CRYPTLIBS}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${CRYPTLIBS} ${TCPIPLIBS} ${CLIB}
        Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} ${CRYPTLIBS} ${DEBLIBS}
	${mkdir} rm
        ${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${CRYPTLIBS} ${TCPIPLIBS} ${CLIB} ${DEBLIBS}
        Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DEBFLAGS} -o $@ module.c



#---------------------------------------------------------------------------
# Dynamic dependencies:
