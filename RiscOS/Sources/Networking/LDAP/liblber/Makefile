# Makefile for liblber
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Jul-2000 SNB          Created.
#

#
# Paths
#
LIBDIR = <Lib$Dir>.LDAP

#
# Generic options:
#
include Makefiles:StdTools

CCFLAGS = -c -depend !Depend -ff -Wpanc ${INCLUDES} ${DFLAGS}

#
# Libraries
#
include Makefiles:AppLibs

#
# Include files
#
INCLUDES  = -IC:LDAP -ITCPIPLibs: -IC:
DFLAGS    = -DMOZILLA_CLIENT -DNO_MDUPDATE -DMOZ_LDAP_SDK -DNEEDPROTOS
# -DLDAP_DEBUG

#
# Program specific options:
#
COMPONENT  = LBER
COMPONENTZ = LBERzm
TARGET     = alf.${COMPONENT}
TARGETZ    = alf.${COMPONENTZ}
SOURCE     = 
EXPORTS    = 
TESTS      = aif.encode aif.decode

OBJS  = o.bprint  o.decode  o.encode  o.io
OBJSZ = oz.bprint oz.decode oz.encode oz.io

LIBS = ${TARGET} ${TARGETZ}

LOCALDIRS = o._dirs

#
# Rule patterns
#
.SUFFIXES: .oz
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -o $@ $< -zM -zps1

#
# build the library:
#
all:    ${LIBS}

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
        ${WIPE} aif ${WFLAGS}
        ${WIPE} alf ${WFLAGS}
        @echo ${COMPONENT}: cleaned

export_hdrs:
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} export_dirs
	${CP} alf ${LIBDIR}.o ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

${LOCALDIRS}:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} alf
	${MKDIR} aif
        ${TOUCH} $@

aif.encode: etest.o ${TARGET}
	${LD} ${LDFLAGS} -o $@ etest.o ${TARGET} ${NET5LIBS} ${CLIB}

aif.decode: dtest.o ${TARGET}
	${LD} ${LDFLAGS} -o $@ dtest.o ${TARGET} ${NET5LIBS} ${CLIB}

export_dirs:
        ${MKDIR} ${LIBDIR}.h

#
# Final link
#
${TARGET}: ${OBJS} ${LOCALDIRS}
        ${AR} ${ARFLAGS} -o $@ ${OBJS}

${TARGETZ}: ${OBJSZ} ${LOCALDIRS}
	${AR} ${ARFLAGS} -o $@ ${OBJSZ}

# Dynamic dependencies:
