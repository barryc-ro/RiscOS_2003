# Makefile for AcornSSL
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 20-Mar-98  SNB    Created
# 31-Mar-98  SNB    Header export added - plus no longer relies on SSL$Path being set
#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.URL
EXP_HDR = <cexport$dir>

#
# Component specific options:
#
COMPONENT  = AcornSSL
ROM_MODULE = aof.${COMPONENT}
RAM_MODULE = rm.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}tr

EXPORTS    = ${EXP_HDR}.h.${COMPONENT}

RESFSDIR   = Resources.URL.${COMPONENT}


#
# Generic options:
#
AS      = objasm
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

CFLAGS   = -c -depend !Depend ${INCLUDES} -wp -zM -ffah -zps1 ${DFLAGS} -Wd ${THROWBACK}
DFLAGS   = -D${SYSTEM} -DCOMPAT_INET4 -DRISCOS -DNO_DES -DNO_IDEA
AFLAGS   = -throwback -depend !Depend
CMHGFLAGS= -p -depend !Depend
ROMFLAGS = -DROM
INCLUDES = -IC:AsmUtils -ITCPIPLibs:,C:SSLeay,C:
DEBFLAGS = -DTRACE -DUSE_SYSLOG

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_abssym
TCPIPLIBS = TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm
CRYPTLIBS = <Lib$Dir>.SSLeay.o.ssl <Lib$Dir>.SSLeay.o.libcrypto
DEBLIBS   = <syslog$dir>.c-veneer.o.syslog
ASMUTILS  = C:AsmUtils.o.AsmUtilsZM


OBJS =\
 o.acornssl\
 o.memory\
 o.module\
 o.protocol\
 o.ses_ctrl\
 o.utils\
 o.SSLHdr

RAM_OBJS =\
 o.acornssl\
 o.memory\
 o.moduleRAM\
 o.protocol\
 o.ses_ctrl\
 o.utils\
 o.SSLHdr

DBG_OBJS =\
 od.acornssl\
 od.memory\
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
.s.o:;	  ${AS} ${AFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} ${CMHGFLAGS} -o $@ $< -d $*.h

SSLHdr.h: VersionNum cmhg.SSLHdr
	${CMHG} ${CMHGFLAGS} cmhg.SSLHdr -d $@

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

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

export_hdrs: export
	@|

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: LocalRes:Messages
	TokenCheck LocalRes:Messages
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} LocalRes:Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.* ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.${COMPONENT}Hdr ${WFLAGS}
        ${RM} h.SSLHdr
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${EXP_HDR}.h.${COMPONENT}: h.SSLHdr
	awk -- "/.define AcornSSL\_00/,/endif/" h.SSLHdr { > $@ }
	@SetType $@ Text

${ROM_MODULE}: ${OBJS} ${INETLIB} ${ROMCSTUBS} ${ASMUTILS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${INETLIB} ${ASMUTILS} ${ROMCSTUBS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${MSGSF}: LocalRes:Messages
	TokenCheck LocalRes:Messages
	resgen messages_file ${MSGSF} LocalRes:Messages ${RESFSDIR}.Messages

${RAM_MODULE}: ${RAM_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} ${CRYPTLIBS} ${ASMUTILS}
	${mkdir} rm
        ${LD} -o $@ -module ${RAM_OBJS} ${MSGSF} ${CRYPTLIBS} ${TCPIPLIBS} ${ASMUTILS} ${CLIB}
        modsqz $@
        Access $@ RW/R

${DBG_MODULE}: ${DBG_OBJS} ${MSGSF} ${TCPIPLIBS} ${CLIB} ${CRYPTLIBS} ${DEBLIBS} ${ASMUTILS}
	${mkdir} rm
        ${LD} -o $@ -module ${DBG_OBJS} ${MSGSF} ${CRYPTLIBS} ${TCPIPLIBS} ${ASMUTILS} ${CLIB} ${DEBLIBS}
        Access $@ RW/R

o.moduleRAM: module.c
	$(CC) ${CFLAGS} -o $@ module.c

od.moduleRAM: module.c
	$(CC) ${CFLAGS} ${DEBFLAGS} -o $@ module.c



#---------------------------------------------------------------------------
# Dynamic dependencies:
