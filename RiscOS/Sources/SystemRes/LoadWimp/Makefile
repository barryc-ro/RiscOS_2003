# Makefile for LoadWimp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 21-Aug-98      SNB    Derived from System makefile
#

#
# Program specific options:
#
COMPONENT  = LoadWimp
INSTCOMP   = ${INSTDIR}.${COMPONENT}

# Generic options:
#
MKDIR   = cdir
ATTR    = -attr
CP      = copy
RM      = remove
AS      = objasm
LD      = link
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
AFLAGS  = ${THROWBACK} -depend !Depend -stamp -quit
LDFLAGS = -bin

FILES=\
  utl.${COMPONENT}

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc)

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        ${CP} utl.${COMPONENT}   ${INSTCOMP}           ${CPFLAGS}
        ${ATTR} -directories +wr -ww ${INSTDIR}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} utl.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

.s.o:;	${AS} ${AFLAGS} -o $@ $<

utl.${COMPONENT}: o.${COMPONENT}
	${LD} ${LDFLAGS} -o $@ $?
	SetType $@ &FFC


#---------------------------------------------------------------------------
# Dynamic dependencies:
