# Makefile for SNMP Agent (application)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Jun-99  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

VPATH = @ ^.mibimpl

#
# Generic options:
#
AWK     = awk
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir
TOUCH   = create

COMPONENT = SNMPAgent
CMHGNAME  = SNMPAgent

EXPORTS = ${EXP_HDR}.h.${COMPONENT}

DEPENDS = -depend !Depend ${THROWBACK}

#
# Internet version compatibility
#
COMPAT = -DCOMPAT_INET4


DEFINES = ${COMPAT} ${OPTIONS} -UTRACE -DRISCOSAPP

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -Wspan -g ${DEFINES} ${INCLUDES} ${DEPENDS}
CMHGFLAGS = -p ${DEPENDS}

LDFLAGS = #-D
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

DIRS = o._dirs

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIBS  = TCPIPLibs:o.unixlib TCPIPLibs:o.inetlib TCPIPLibs:o.socklib
SNMPLIBS  = C:SNMP.o.snmplib
DEBUGLIB  = C:DebugLib.o.DebugLib

#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:,C:SNMP,C:SNMP.config,C:SNMP.agent,C:SNMP.mibgroup

#
# Program specific options:
#
TARGET    = aif.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =	\
 o.snmpd	\
 o.snmp_vars	\
 o.agent_read_config	\
 o.snmp_agent	\
 o.snmp2p	\
 \
 o.acl_vars	\
 o.system_mib	\
 o.view_parse	\
 

DEBOBJS   =	\
 od.snmpd	\
 od.snmp_vars	\
 od.agent_read_config	\
 od.snmp_agent	\
 od.snmp2p	\
 \
 od.acl_vars	\
 od.system_mib	\
 od.view_parse	\
 \

#
# Rule patterns
#
.SUFFIXES:  .o .od .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;      ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -o o.$* $< $@
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build an SNMP agent module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

trace: ${TARGET}D
        @echo ${COMPONENT}: export complete (trace)

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${INETLIBS} ${SNMPLIBS} ${CLIB} ${DIRS}
	${MKDIR} aif
        ${LD} -o $@ ${LDFLAGS} -aif ${OBJS} ${SNMPLIBS} ${INETLIBS} ${CLIB}
        Access $@ RW/R

${TARGET}D: ${DEBOBJS} ${INETLIBS} ${SNMPLIBS} ${DIRS} ${CLIB} ${DEBUGLIB}
	${MKDIR} aif
	${LD} -o $@ ${LDFLAGS} ${DEBOBJS} ${DEBUGLIB} ${SNMPLIBS} ${INETLIBS} ${CLIB}
	Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${SNMPLIBS} ${INETLIBS} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${SNMPLIBS} ${INETLIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${DIRS}:
	${MKDIR} o
	${MKDIR} od
	${TOUCH} $@

# Dynamic dependencies:
