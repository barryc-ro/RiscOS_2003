# Makefile for ResTest
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 03-Jun-98  SNB    Created
#
.SUFFIXES: .c .o .od

#
# Component specific options:
#
COMPONENT = ResTest

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
SQZ	= squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS =
CFLAGS	 = -c -depend !Depend ${INCLUDES} -ffa ${DFLAGS}
DFLAGS	 = -D${SYSTEM}
INCLUDES = -IC:tboxlibint,C:tboxlibs,TCPIPLibs:,C:

DEBLIB   = C:tboxlibint.o.debuglib
#<syslog$dir>.c-veneer.o.syslog

#
# Libraries
#
CLIB	  = CLIB:o.stubs


OBJS =\
 o.linklist\
 o.main\
 o.messages\
 o.tbox_tools\
 o.txt\
 o.wimplib\
 o.wimpmess

DEBOBJS =\
 od.linklist\
 od.main\
 od.messages\
 od.tbox_tools\
 od.txt\
 od.wimplib\
 od.wimpmess

#
# Rule patterns
#
.c.o:;	  ${CC} ${CFLAGS} -Ddebugging=0 -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} -DTRACE -UUSE_SYSLOG -Ddebugging=1 -fn -o $@ $<

#
# Main rules:
#
#
all: ${TARGET}
	@echo ${COMPONENT}: !RunImage built

trace: ${TARGET}T
	@echo ${COMPONENT}: !RunImage built (trace)

export: all
	${MKDIR} ${EXPDIR}
	${CP} Resources.* ${EXPDIR}.* ${CPFLAGS}
	${SQZ} ${TARGET} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS} ${CLIB}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${CLIB}
	Access $@ RW/R

${TARGET}T: ${DEBOBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS} ${DEBLIB}
        ${mkdir} aif
	${LD} -aif -o $@ ${DEBOBJS} ${CLIB} ${DEBLIB}
	Access $@ RW/R


#---------------------------------------------------------------------------
# Dynamic dependencies:
