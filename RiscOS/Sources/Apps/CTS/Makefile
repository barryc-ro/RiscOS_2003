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
 o.ctsmessage\
 o.ctsbrowser\
 o.ctsui\
 o.ctsutils\
 o.plugclient\
 o.smartcard\
 o.statetable\
 o.utils

DEBOBJS =\
 od.client\
 od.ctsmessage\
 od.ctsbrowser\
 od.ctsui\
 od.ctsutils\
 od.plugclient\
 od.smartcard\
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

export: coreres all
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

traceinst: coreres trace
	${SQZ} ${TARGET}T ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

coreres:
	${MKDIR} ${EXPDIR}
	${MKDIR} ${EXPDIR}.!${COMPONENT}
	${CP} LocalRes:!Boot ${EXPDIR}.!${COMPONENT}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${EXPDIR}.!${COMPONENT}.!Run ${CPFLAGS}
	${CP} LocalRes:Messages ${EXPDIR}.!${COMPONENT}.Messages ${CPFLAGS}
	${CP} LocalRes:Res ${EXPDIR}.!${COMPONENT}.Res ${CPFLAGS}
	${MKDIR} ${EXPDIR}.!${COMPONENT}.WebPages
	${CP} LocalRes:WebPages.01-1 ${EXPDIR}.!${COMPONENT}.WebPages.01-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.01-2 ${EXPDIR}.!${COMPONENT}.WebPages.01-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.01-3 ${EXPDIR}.!${COMPONENT}.WebPages.01-3 ${CPFLAGS}
	${CP} LocalRes:WebPages.02   ${EXPDIR}.!${COMPONENT}.WebPages.02   ${CPFLAGS}
	${CP} LocalRes:WebPages.03   ${EXPDIR}.!${COMPONENT}.WebPages.03   ${CPFLAGS}
	${CP} LocalRes:WebPages.04   ${EXPDIR}.!${COMPONENT}.WebPages.04   ${CPFLAGS}
	${CP} LocalRes:WebPages.05   ${EXPDIR}.!${COMPONENT}.WebPages.05   ${CPFLAGS}
	${CP} LocalRes:WebPages.06   ${EXPDIR}.!${COMPONENT}.WebPages.06   ${CPFLAGS}
	${CP} LocalRes:WebPages.11   ${EXPDIR}.!${COMPONENT}.WebPages.11   ${CPFLAGS}
	${CP} LocalRes:WebPages.21   ${EXPDIR}.!${COMPONENT}.WebPages.21   ${CPFLAGS}
	${CP} LocalRes:WebPages.22   ${EXPDIR}.!${COMPONENT}.WebPages.22   ${CPFLAGS}
	${CP} LocalRes:WebPages.23   ${EXPDIR}.!${COMPONENT}.WebPages.23   ${CPFLAGS}
	${CP} LocalRes:WebPages.24   ${EXPDIR}.!${COMPONENT}.WebPages.24   ${CPFLAGS}
	${CP} LocalRes:WebPages.25   ${EXPDIR}.!${COMPONENT}.WebPages.25   ${CPFLAGS}
	${CP} LocalRes:WebPages.26   ${EXPDIR}.!${COMPONENT}.WebPages.26   ${CPFLAGS}
	${CP} LocalRes:WebPages.27-1 ${EXPDIR}.!${COMPONENT}.WebPages.27-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.27-2 ${EXPDIR}.!${COMPONENT}.WebPages.27-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.27-3 ${EXPDIR}.!${COMPONENT}.WebPages.27-3 ${CPFLAGS}
	${CP} LocalRes:WebPages.28-1 ${EXPDIR}.!${COMPONENT}.WebPages.28-1 ${CPFLAGS}
	${CP} LocalRes:WebPages.28-2 ${EXPDIR}.!${COMPONENT}.WebPages.28-2 ${CPFLAGS}
	${CP} LocalRes:WebPages.30   ${EXPDIR}.!${COMPONENT}.WebPages.30   ${CPFLAGS}

${DIRS}:
	${mkdir} o
	${mkdir} od
	${touch} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
