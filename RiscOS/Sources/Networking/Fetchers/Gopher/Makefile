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
COMPONENT  = Gopher
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
AS	= objasm

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -NoCache 
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
 o.asmbits\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.module\
 o.processdir\
 o.readdata\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.utils\
 o.GopherHdr

RAM_OBJS =\
 o.asmbits\
 o.commands\
 o.connect\
 o.dns\
 o.generic\
 o.moduleRAM\
 o.processdir\
 o.readdata\
 o.ses_ctrl\
 o.start\
 o.status\
 o.stop\
 o.utils\
 o.GopherHdr

MSGSF=o.msgs

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} ${ROMFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -o $@ $< -d $*.h
.s.o:;	  ${AS} ${AFLAGS} -o $@ $<


GopherHdr.h: GopherHdr.o
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

${MSGSF}: Resources.${LOCALE}.Messages Resources.${LOCALE}.File Resources.${LOCALE}.Dir 
	MkRes messages_file ${MSGSF} Resources.${LOCALE} ${RESFSDIR}

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS}
	${mkdir} rm
        ${TLD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS}
        Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c


#---------------------------------------------------------------------------
# Dynamic dependencies:
