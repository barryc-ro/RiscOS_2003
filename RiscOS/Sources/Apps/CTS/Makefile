# Makefile for CTS Plugin
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 19-Apr-99  SNB    Created
#
.SUFFIXES: .c .o .od

#
# Component specific options:
#
COMPONENT = CTS

EXPDIR = Export
TARGET = aif.!RunImage

#
# Generic options:
#
CHMOD	= access
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
TOUCH   = create
SQZ	= squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS =
CFLAGS	 = -c -depend !Depend ${INCLUDES} -ffah -Wp ${DFLAGS} ${THROWBACK}
DFLAGS	 = -D${SYSTEM}
INCLUDES = -Itbox:,TCPIPLibs:,C:

#
# Libraries
#
LIBDIR	  = <Lib$Dir>
CLIB	  = CLIB:o.stubs
TBOXLIBS  = TBox:o.eventlib TBox:o.wimplib TBox:o.toolboxlib
TCPIPLIBS = TCPIPLIBS:o.inetlib TCPIPLIBS:o.socklib5 TCPIPLIBS:o.unixlib
DEBUGLIB  = ${LIBDIR}.DebugLib.o.DebugLib

OBJS =\
 o.client\
 o.ctsbrowser\
 o.plugclient\
 o.statetable\
 o.utils

DEBOBJS =\
 od.client\
 od.ctsbrowser\
 od.plugclient\
 od.statetable\
 od.utils

DIRS=o._dirs

#
# Rule patterns
#
.c.o:;	  ${CC} ${CFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} ${EXTRAFLAGS} -DDEBUGLIB -fn -o $@ $<

#
# Main rules:
#
#
all: ${TARGET}
	@echo ${COMPONENT}: !RunImage built

trace: ${TARGET}T
	@echo ${COMPONENT}: !RunImageT built

export: all
	${MKDIR} ${EXPDIR}
	${MKDIR} ${EXPDIR}.!${COMPONENT}
	${CP} Resources.${LOCALE}.* ${EXPDIR}.!${COMPONENT}.* ${CPFLAGS}
	${SQZ} ${TARGET} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	${WIPE} od ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS} ${DIRS}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${TBOXLIBS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R

${TARGET}T: ${DEBOBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS} ${DEBUGLIB} ${DIRS}
        ${mkdir} aif
	${LD} -aif -o $@ ${DEBOBJS} ${TBOXLIBS} ${DEBUGLIB} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R

traceinst: trace
	${MKDIR} ${EXPDIR}
	${MKDIR} ${EXPDIR}.!${COMPONENT}
	${CP} Resources.${LOCALE}.* ${EXPDIR}.!${COMPONENT}.* ${CPFLAGS}
	${SQZ} ${TARGET}T ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

${DIRS}:
	${mkdir} o
	${mkdir} od
	${touch} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
