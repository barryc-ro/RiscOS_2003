# Makefile for WebServe
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 11-Feb-98  SNB    Created
# 30-Apr-98  SNB    Added trace build option
#
.SUFFIXES: .c .o .od

#
# Component specific options:
#
COMPONENT = WebServe

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
CFLAGS	 = -c -depend !Depend ${INCLUDES} -ffah -Wp ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 -UTML
INCLUDES = -ITbox:,TCPIPLibs:,C:

#
# Libraries
#
CLIB	  = CLIB:o.stubs
TBOXLIBS  = TBox:o.eventlib TBox:o.wimplib TBox:o.toolboxlib
TCPIPLIBS = TCPIPLIBS:o.inetlib TCPIPLIBS:o.socklib TCPIPLIBS:o.unixlib


OBJS =\
 o.cache\
 o.checkloop\
 o.config\
 o.debug\
 o.dialler\
 o.filemanage\
 o.generic\
 o.headers\
 o.italkmsg\
 o.main\
 o.messages\
 o.network\
 o.process\
 o.quit\
 o.serve\
 o.snprintf\
 o.URLclient

DEBOBJS =\
 od.cache\
 od.checkloop\
 od.config\
 od.debug\
 od.dialler\
 od.filemanage\
 od.generic\
 od.headers\
 od.italkmsg\
 od.main\
 od.messages\
 od.network\
 od.process\
 od.quit\
 od.serve\
 od.snprintf\
 od.URLclient

#
# Rule patterns
#
.c.o:;	  ${CC} ${CFLAGS} -o $@ $<
.c.od:;	  ${CC} ${CFLAGS} -DTRACE -DUSE_SYSLOG -fn -o $@ $<

#
# Main rules:
#
#
all: ${TARGET}
	@echo ${COMPONENT}: !RunImage built

trace: ${TARGET}T

export: all
	${MKDIR} ${EXPDIR}
	${CP} CommonBase.* ${EXPDIR}.* ${CPFLAGS}
	${CP} Resources.${LOCALE}.* ${EXPDIR}.* ${CPFLAGS}
	${SQZ} ${TARGET} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	${WIPE} od.* ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${TBOXLIBS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R

${TARGET}T: ${DEBOBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS}
        ${mkdir} aif
	${LD} -aif -o $@ ${DEBOBJS} ${TBOXLIBS} ${TCPIPLIBS} ${CLIB} -Symbols symtab
	Access $@ RW/R


#---------------------------------------------------------------------------
# Dynamic dependencies:
