# Makefile for Prototype OHCI driver
#
# $Id$

COMPONENT       = ProtoOHCI
TARGET          = ${COMPONENT}
DIRS            = o._dirs
RAMMESSAGES     = o.msgs
MESSAGELOCATION = Resources.${COMPONENT}.Messages
OBJS    = o.modhead o.cmodule o.controller o.memory o.usbd o.bufman ${RAMMESSAGES}
LIBS    = ${DEBUGLIB} ${NET5LIBS} ${CALLXLIB} ${ASMUTILS} tbox:o.wimplib c:tboxlibint.o.tboxlibm

CFLAGS          += -zm -zps1 -ffah ${COPTIONS}
CINCLUDES       = -Itbox:,OS:,C:
CMHGDEFINES     = -DCOMPONENT=${COMPONENT}

include StdTools
include ModStdRule
include ModuleLibs
include RAMCModule
include ROMCModule

aif.${COMPONENT}: ${RAM_OBJS} ${RAM_LIBS} ${CLIB}  o._dirs
        link -base 0 -aif -bin -d -o $@ ${RAM_OBJS} ${RAM_LIBS} ${CLIB}

gpa.${COMPONENT}: aif.${COMPONENT}
        togpa aif.${COMPONENT} $@

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} gpa ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        ${RM}   h.modhead

o._dirs:
        ${MKDIR} o
        ${MKDIR} c
        ${MKDIR} gpa
        ${MKDIR} aif
        ${TOUCH} o._dirs

o.cmodule o.usbd:       h.modhead

# Static dependancies

${RAMMESSAGES}: Resources.UK.Messages
        resgen resource_files ${RAMMESSAGES} Resources.UK.Messages ${MESSAGELOCATION}

# Dynamic dependencies:
