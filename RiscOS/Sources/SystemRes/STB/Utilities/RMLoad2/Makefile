# Makefile for RMLoad2 transient
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 22-Jun-2000    SAR    Created
# 07-Sep-2000    SAR    Changed from BASIC Asm to objasm
#

#
# Program specific options:
#
COMPONENT  = RMLoad2
UTILITY    = util.${COMPONENT}

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
OBJS     = o.RMLoad2

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
        stripdepnd
        @echo ${COMPONENT}: cleaned

export_hdrs:
        @echo ${COMPONENT}: nothing to do.

export_libs:
        @echo ${COMPONENT}: nothing to do.

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
