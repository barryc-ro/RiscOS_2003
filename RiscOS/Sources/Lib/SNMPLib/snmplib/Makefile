# Makefile for libsnmp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Jun-99  SNB          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>
SNMPLIBDIR = ${LIBDIR}.SNMP


VPATH = @ riscos


#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -Wspan ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES  = -ITCPIPLibs:,C:SNMP,C:SNMP.config,C:,riscos
DFLAGS    = -DRISCOS -DHAVE_CONFIG_H

#
# Program specific options:
#
COMPONENT  = snmplib
COMPONENTZ = snmplibzm
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
SOURCE     =
EXPORTS    =

OBJS =\
 o.acl \
 o.acl_parse \
 o.asn1 \
 o.context \
 o.context_parse \
 o.int64 \
 o.md5 \
 o.mib \
 o.parse \
 o.party \
 o.party_parse \
 o.read_config \
 o.snmp \
 o.snmp_api \
 o.snmp_auth \
 o.snmp_client \
 o.snmp_debug \
 o.system \
 o.vacm \
 o.view \
 o.riscos

OBJSZ =\
 oz.acl \
 oz.acl_parse \
 oz.asn1 \
 oz.context \
 oz.context_parse \
 oz.int64 \
 oz.md5 \
 oz.mib \
 oz.parse \
 oz.party \
 oz.party_parse \
 oz.read_config \
 oz.snmp \
 oz.snmp_api \
 oz.snmp_auth \
 oz.snmp_client \
 oz.snmp_debug \
 oz.system \
 oz.vacm \
 oz.view \
 oz.riscos

LIBS = ${TARGET} ${TARGETZ}

#
# Rule patterns
#
.SUFFIXES: .o .oz .od .c .h .s
.c.o:;     ${CC} ${CCFLAGS} -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -ff -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#o.riscos: ^.s.c.riscos
#	${CC} ${CCFLAGS} -ff     -o $@ ^.s.c.riscos
#
#oz.riscos: ^.s.c.riscos
#	${CC} ${CCFLAGS} -ff -zM -o $@ ^.s.c.riscos

#
# build the library:
#
all:	${LIBS}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENTZ}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${SNMPLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETZ} ${SNMPLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)


o.local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${TOUCH} $@

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${SNMPLIBDIR}
        ${MKDIR} ${SNMPLIBDIR}.o

links:
	./mklinks
	echo Made links

#
# Final link
#
${TARGET}: ${OBJS} o.local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} o.local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

# Dynamic dependencies:
