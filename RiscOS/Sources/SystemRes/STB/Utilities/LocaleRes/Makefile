# Makefile for LocaleVar transient
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 13-Aug-2001    SAR    Created
# 14-Aug-2001    RJC    Added CPFLAGS to export_libs
#

#
# Program specific options:
#
COMPONENT  = LocaleVar
UTILITY    = util.${COMPONENT}
LIB_EX_DIR = <Lib$Dir>.${COMPONENT}

#
# Generic options:
#
MKDIR   = mkdir -p
ATTR    = attr
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = wipe
CHMOD   = attr $(CHMODFLAGS)

CPFLAGS    = ~cfr~v
WFLAGS     = ~c~v
CHMODFLAGS = +ow +or +wr -ww
OFLAGS     = -depend !Depend ${THROWBACK}

#
# Objects
#
OBJS     = o.LocaleVar

#
# Rule patterns
#
.s.o:;          ${AS} ${OFLAGS} -o $@ $<

#
# Default rule
#
all: ${UTILITY}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
clean:
        ifthere util     then ${WIPE} util     ${WFLAGS}
        ifthere o        then ${WIPE} o        ${WFLAGS}
        @echo ${COMPONENT}: cleaned

export: export_libs
        @echo ${COMPONENT}: nothing to do.

export_hdrs:
        @echo ${COMPONENT}: nothing to do.

export_libs: ${UTILITY}
        ${MKDIR} ${LIB_EX_DIR}
        ${CP} VersionNum ${LIB_EX_DIR}.LibVersion ${CPFLAGS}
        ${CP} util.${COMPONENT} ${LIB_EX_DIR}.${COMPONENT} ${CPFLAGS}
        @echo
        @echo ${COMPONENT}: export_libs complete.

resources:
        @echo ${COMPONENT}: nothing to do.

rom:
        @echo ${COMPONENT}: nothing to do.

install_rom:
        @echo ${COMPONENT}: nothing to do.

rom_link:
        @echo ${COMPONENT}: nothing to do.

#
# RISC OS disc build rules:
#
install: ${UTILITY} dirs
        ${CP} ${UTILITY} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        $(CHMOD) $(INSTDIR)
        @echo ${COMPONENT}: installed to ${INSTDIR}

dirs:
        ${MKDIR} ${INSTDIR}

#
# Other rules:
#
${UTILITY}: util ${OBJS} o
        ${LD} -o $@ -bin ${OBJS}
        SetType $@ Utility
        ${ATTR} -files +or +ow +wr -ww $@

util:
	${MKDIR} util

o:
	${MKDIR} o

#---------------------------------------------------------------------------
# Dynamic dependencies:
