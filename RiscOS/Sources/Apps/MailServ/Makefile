# Makefile for Connect/MailServ
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 28-Apr-98  SNB    Created
#

#
# Component specific options:
#
#COMPONENT = Connect
COMPONENT = MailServ

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
CFLAGS	 = -c -depend !Depend ${INCLUDES} -fa -Wp -throwback ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 ${TRFLAGS} -UTIMEBOMBED ${BFLAGS}
TRFLAGS  = -UTML -UTRACE -UDEBUG_ON
BFLAGS   = -DBUILD_${COMPONENT}
INCLUDES = -IC:,TCPIPLibs:

#
# Libraries
#
CLIB	  = CLIB:o.stubs
TCPIPLIBS = TCPIPLIBS:o.inetlib TCPIPLIBS:o.socklib


OBJS =\
	o.config \
	o.database \
	o.error \
	o.file \
	o.finger \
	o.getifaddr \
	o.log \
	o.main \
	o.message \
	o.nglist \
	o.nntp \
	o.pop3 \
	o.scan \
	o.script \
	o.smtp \
	o.token \
	o.userlist \
	o.users \
	o.wimp 

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

${TARGET}: ${OBJS} ${CLIB} ${TCPIPLIBS}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R


#---------------------------------------------------------------------------
# Dynamic dependencies:
