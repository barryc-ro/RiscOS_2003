# Makefile for Finger
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 10-Feb-98  SNB    Created
#

#
# Component specific options:
#
COMPONENT  = Finger
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}


RESFSDIR   = Resources.URL.${COMPONENT}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}

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

CFLAGS   = -c -depend !Depend ${INCLUDES} -throwback -zM -Wp -ffah -zps1 ${DFLAGS}
DFLAGS   = -D${SYSTEM} -DCOMPAT_INET4 -UTML -UTRACE
ROMFLAGS = -DROM
INCLUDES = -ITCPIPLibs:,C:

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLIBS:o.socklibzm

OBJS =\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.headers\
 o.module\
 o.protocol\
 o.readdata\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.writedata\
 o.FingerHdr

RAM_OBJS =\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.headers\
 o.moduleRAM\
 o.protocol\
 o.readdata\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.URLclient\
 o.utils\
 o.writedata\
 o.FingerHdr

MSGSF=o.msgs

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $< -d $*.h

FingerHdr.h: FingerHdr.o
	  @|
	  
#
# Main rules:
#
#
all: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: Resources.${LOCALE}.Messages
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.URL
        ${CP} Resources.${LOCALE}.* ${RESDIR}.URL.* ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TCPIPLIBS} ${ROMCSTUBS}

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

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS}
	${mkdir} rm
        ${TLD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS}
        Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
