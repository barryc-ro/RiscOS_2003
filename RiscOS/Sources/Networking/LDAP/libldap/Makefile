# Makefile for libldap
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
COMPONENT  = LDAP
COMPONENTZ = LDAPzm
TARGET     = alf.${COMPONENT}
TARGETZ    = alf.${COMPONENTZ}
SOURCE     = 
EXPORTS    = 

OBJS  =\
o.abandon \
o.add \
o.bind \
o.cache \
o.charray \
o.charset \
o.compare \
o.compat \
o.control \
o.countvalues \
o.delete \
o.disptmpl \
o.dsparse \
o.error \
o.extendop \
o.free \
o.freevalues \
o.friendly \
o.getattr \
o.getdn \
o.getdxbyname \
o.getentry \
o.getfilter \
o.getoption \
o.getvalues \
o.memcache \
o.message \
o.modify \
o.open \
o.os-ip \
o.psearch \
o.referral \
o.regex \
o.rename \
o.request \
o.reslist \
o.result \
o.saslbind \
o.sbind \
o.search \
o.setoption \
o.sort \
o.sortctrl \
o.srchpref \
o.tmplout \
o.tmpltest \
o.ufn \
o.unbind \
o.unescape \
o.url \
o.utf8 \
o.vlistctrl


OBJSZ =\
oz.abandon \
oz.add \
oz.bind \
oz.cache \
oz.charray \
oz.charset \
oz.compare \
oz.compat \
oz.control \
oz.countvalues \
oz.delete \
oz.disptmpl \
oz.dsparse \
oz.error \
oz.extendop \
oz.free \
oz.freevalues \
oz.friendly \
oz.getattr \
oz.getdn \
oz.getdxbyname \
oz.getentry \
oz.getfilter \
oz.getoption \
oz.getvalues \
oz.memcache \
oz.message \
oz.modify \
oz.open \
oz.os-ip \
oz.psearch \
oz.referral \
oz.regex \
oz.rename \
oz.request \
oz.reslist \
oz.result \
oz.saslbind \
oz.sbind \
oz.search \
oz.setoption \
oz.sort \
oz.sortctrl \
oz.srchpref \
oz.tmplout \
oz.tmpltest \
oz.ufn \
oz.unbind \
oz.unescape \
oz.url \
oz.utf8 \
oz.vlistctrl


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
