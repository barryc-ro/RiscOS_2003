# Makefile for MPEGCaptions module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 26-Apr-99      BJGA   Created
#

#
# Program specific options:
#
COMPONENT  = MPEGCaptns
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}

#
# Generic options:
#
MKDIR   = mkdir -p
CC      = cc
ATTR    = -attr
AS      = objasm
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

CFLAGS   = -c -ffah -zM ${INCLUDES} -depend !Depend ${THROWBACK}
INCLUDES = -IC:
OFLAGS   = -depend !Depend ${THROWBACK}
CMHGFLAGS= -p ${THROWBACK}

#
# Libraries
#
CLIB     = C:o.stubs
ROMCSTUBS= RISC_OSLib:o.romcstubs
C_ABSSYM = RISC_OSLib:o.c_abssym

OBJS     = Module.o ModHdr.o

#
# Rule patterns
#
.c.o:;    ${CC} ${CFLAGS} -o $@ $<
.s.o:;    ${AS} ${OFLAGS} -o $@ $<

#
# Main rules:
#
#
ram: ${RAM_MODULE}
        ${RM} Messages
        @echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_ram: ${RAM_MODULE}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        |
        access ${INSTDIR}.${COMPONENT} RW/W
        |
        @echo ${COMPONENT}: Module installed (RAM)

install_rom: ${ROM_MODULE}
        ${MKDIR} ${INSTDIR}
        |
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        |
        access ${INSTDIR}.${COMPONENT} RW/W
        |
        @echo ${COMPONENT}: Module installed (ROM)

install: install_ram

clean:
        ${WIPE} linked ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${RM} h.ModHdr
        -stripdepnd
        @echo ${COMPONENT}: cleaned

${RAM_MODULE}: rm ${OBJS} o ${CLIB}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB}
        access $@ RW/R

${ROM_MODULE}: aof ${OBJS} o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${C_ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

ModHdr.o: cmhg.ModHdr
	${CMHG} ${CMHGFLAGS} -o $@ $? h.ModHdr

aof:
	${MKDIR} aof

rm:
	${MKDIR} rm

o:
	${MKDIR} o

#---------------------------------------------------------------------------
# Dynamic dependencies:
