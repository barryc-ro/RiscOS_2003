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
COMPONENT  = awt
MODULE = aof.$(COMPONENT)

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

${MODULE}: $(OBJS)
	$(MKDIR) aof
        ${LD} -o $@ -aof ${OBJS}
        
clean:
        ${WIPE} o.* ${WFLAGS}
      	@echo ${COMPONENT}: cleaned
	
#---------------------------------------------------------------------------
# Dynamic dependencies:
