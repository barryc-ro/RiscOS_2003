# Makefile for IPTeletext
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 28-Aug-01  MIB    Created
#

.SUFFIXES: .c .o .s

#
# Component specific options:
#
COMPONENT  = IPTeletext
RAM_MODULE = rm.${COMPONENT}

#
# Generic options:
#
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
TLD     = tlink
MKDIR   = do mkdir -p
MODSQZ  = modsqz
RESGEN  = resgen
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

AFLAGS   = -depend !Depend -Stamp -quit -CloseExec
CFLAGS   = ${THROWBACK} -c -depend !Depend ${INCLUDES} -zM -fah -zps1 ${DFLAGS}
INCLUDES = -Itbox:,C:

#
# Libraries
#
CLIB      = CLIB:o.stubs

OBJS =\
 o.modhdr\
 o.swihandlers\
 o.pehandlers\
 o.freepacket\
 o.messages\
 o.main

RESF=o.msgs

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:; ${CMHG} -depend !Depend -p -d h.module -o $@ $<
.s.o:;    ${AS} ${AFLAGS} -o $@ $<

#
# Main rules:
#
#
all: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Explicit rules
#

h.module: o.modhdr
c.main: h.module

${RESF}: LocalRes:Messages
        resgen messages_file ${RESF} LocalRes:Messages Resources.${COMPONENT}.Messages

${RAM_MODULE}: ${OBJS} ${CLIB} ${RESF} dirs
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${RESF}
        Access $@ RW/R
        @echo ${COMPONENT}: module built (disc).

install: ${RAM_MODULE}
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed (disc).

dirs:
        ${MKDIR} rm
        ${MKDIR} ${INSTDIR}


#---------------------------------------------------------------------------
# Dynamic dependencies:
