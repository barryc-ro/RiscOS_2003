# Makefile for dummy DLL module build
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 22/4/97  RLougher Created
#

#
# Component specific options:
#
COMPONENT  = JavaJPEG
ROM_MODULE = aof.$(COMPONENT)

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

OBJS = \
	o.DLL

#
# Main rules:
#

SUFFIXES = .s .o

.s.o:;	objasm $< $@
	
rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)
clean:
        ${WIPE} aof ${WFLAGS}
       @echo ${COMPONENT}: cleaned
	
${ROM_MODULE}: $(OBJS)
	$(MKDIR) aof
        ${LD} -o $@ -aof ${OBJS}

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROM_MODULE}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
