# Makefile for resolver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#
.SUFFIXES: .c .o .s .od

#
# Paths
#
EXP_HDR = <Cexport$dir>

#
# Generic options:
#
include Makefiles:StdTools

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -W -c -depend !Depend -zM -zps1 ${THROWBACK} -pcc -fahn ${INCLUDES} ${DFLAGS} -DCOMPAT_INET4
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CMHGFLAGS = -depend !Depend -p
#LDFLAGS = -map -v

#
# Libraries
#
include Makefiles:ModuleLibs

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:,C:AsmUtils

DFLAGS   = -Darm -DROM=1

#
# Program specific options:
#
COMPONENT = Resolver
TARGET    = aof.${COMPONENT}
OBJS      = res_query.o Events.o \
	    Cache.o ExtLib.o Hosts.o Resolver.o \
	    res_comp.o res_init.o res_mkquer.o  \
	    StrCaseCmp.o SWI.o
DEBOBJS   = od.res_query od.Events \
	    od.Cache o.ExtLib od.Hosts o.Resolver \
	    od.res_comp od.res_init od.res_mkquer  \
	    od.StrCaseCmp Stubs.o od.SWI
LIBS      = ${DESKLIB} ${NET4LIBS} ${ASMUTILS}
EXPORTS   =

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;      ${CC} ${CFLAGS} -DDEBUG -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT}

ramtrace: rm.${COMPONENT}D

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

install: rm.${COMPONENT} 
	${CP} rm.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.${COMPONENT}
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${LIBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${LIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${C_ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.${COMPONENT}: ${OBJS} ${CLIB} ${LIBS}
        ${LD} ${LDFLAGS} -o $@ -module ${OBJS} ${CLIB} ${LIBS}

rm.${COMPONENT}D: ${DEBOBJS} ${CLIB} ${LIBS}
        ${LD} ${LDFLAGS} -o $@ -module ${DEBOBJS} ${CLIB} ${LIBS}

# Dynamic dependencies:
