# Makefile for WebServe
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 11-Feb-98  SNB    Created
#

#
# Component specific options:
#
COMPONENT = WebServe

EXPDIR = Export
TARGET = aif.!RunImage

#
# Generic options:
#
CHMOD   = access
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
SQZ     = squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS =
CFLAGS	 = -c -depend !Depend ${INCLUDES} -ffah -Wp ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 -UTML -UTRACE
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

#
# Rule patterns
#
.c.o:;	  ${CC} ${CFLAGS} -o $@ $<

#
# Main rules:
#
#
all: ${TARGET}
	@echo ${COMPONENT}: !RunImage built

export: all
	${MKDIR} ${EXPDIR}
	${CP} CommonBase.* ${EXPDIR}.* ${CPFLAGS}
	${CP} Resources.${LOCALE}.* ${EXPDIR}.* ${CPFLAGS}
	${SQZ} ${TARGET} ${EXPDIR}.!${COMPONENT}.!RunImage ${SQZFLAGS}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET}: ${OBJS} ${CLIB} ${TBOXLIBS} ${TCPIPLIBS}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${TBOXLIBS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R


#---------------------------------------------------------------------------
# Dynamic dependencies:
