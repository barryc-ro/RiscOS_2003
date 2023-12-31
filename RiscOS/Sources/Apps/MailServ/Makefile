# Makefile for Connect/MailServ
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 28-Apr-98  SNB    Created
# 05-May-98  SNB    Added dual build capability
# 23-Jul-98  SNB    !Connect provlist file generated automatically
# 05-Aug-98  SNB    Added provlist protection
#
.SUFFIXES: .o .o_su .c

#
# Component specific options:
#
# Two components - the single-user and multi-user - both are built
#
SU_COMPONENT = Connect
MU_COMPONENT = MailServ

SU_TARGET = aif.su_Image
MU_TARGET = aif.mu_Image

EXPDIR = Export

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
SED	= sed
AWK     = awk

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS =
CFLAGS	 = -c -depend !Depend ${INCLUDES} -faf -Wp -throwback ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DCOMPAT_INET4 ${TRFLAGS} -UTIMEBOMBED
TRFLAGS  = -UTML -UTRACE -UDEBUG_ON
INCLUDES = -IC:,TCPIPLibs:

SEDFLAGS = -e "s/MailServ/Connect/g"
# Must not change app name for SU !Mail (and legal reasons)
# -e "s/InterTalk Server/Connect/g"

# This is the exit code which awk will use if it detects "Acorn" in
# the provlist file.  It is used to fault builds to avoid "mistakes"...
AWKFLAGS = -v ERRORCODE=1 -f Resources.awkscript

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

SU_OBJS =\
	o_su.config \
	o_su.database \
	o_su.error \
	o_su.file \
	o_su.finger \
	o_su.getifaddr \
	o_su.log \
	o_su.main \
	o_su.message \
	o_su.nglist \
	o_su.nntp \
	o_su.pop3 \
	o_su.scan \
	o_su.script \
	o_su.smtp \
	o_su.token \
	o_su.userlist \
	o_su.users \
	o_su.wimp 

#
# Rule patterns
#
.c.o:;	  ${CC} ${CFLAGS} -USU -DBUILD_${MU_COMPONENT} -o $@ $<
.c.o_su:; ${CC} ${CFLAGS} -DSU -DBUILD_${SU_COMPONENT} -o $@ $<

#
# Main rules:
#
#
all: ${MU_TARGET} ${SU_TARGET}
	@echo ${SU_COMPONENT}: !RunImage built
	@echo ${MU_COMPONENT}: !RunImage built

commonbase: 
	${MKDIR} ${EXPDIR}
	${CP} CommonBase.* ${EXPDIR}.* ${CPFLAGS}
	${CP} Resources.${LOCALE}.* ${EXPDIR}.* ${CPFLAGS}
	@${MKDIR} ${EXPDIR}.!${SU_COMPONENT}.Providers
	@${MKDIR} ${EXPDIR}.!${MU_COMPONENT}.Providers
	${WIPE} ${EXPDIR}.!${SU_COMPONENT}.Providers ${WFLAGS}
	${WIPE} ${EXPDIR}.!${MU_COMPONENT}.Providers ${WFLAGS}

# This is far easier than maintaining multiple message files!
commonmessages: commonbase
	${SED} ${SEDFLAGS} Resources.${LOCALE}.!${MU_COMPONENT}.!Help > \
	  ${EXPDIR}.!${SU_COMPONENT}.!Help
	${SED} ${SEDFLAGS} Resources.${LOCALE}.!${MU_COMPONENT}.Messages > \
	  ${EXPDIR}.!${SU_COMPONENT}.Messages
	${SED} ${SEDFLAGS} CommonBase.!${MU_COMPONENT}.provlist > \
	  ${EXPDIR}.!${SU_COMPONENT}.provlist
	${AWK} ${AWKFLAGS} < Export.!${MU_COMPONENT}.provlist

export: commonmessages all
	${SQZ} ${SU_TARGET} ${EXPDIR}.!${SU_COMPONENT}.!RunImage ${SQZFLAGS}
	${SQZ} ${MU_TARGET} ${EXPDIR}.!${MU_COMPONENT}.!RunImage ${SQZFLAGS}
	@echo ${SU_COMPONENT}: !RunImage exported
	@echo ${MU_COMPONENT}: !RunImage exported

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} o_su.* ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	${WIPE} ${EXPDIR} ${WFLAGS}
	@echo ${MU_COMPONENT}: cleaned
	@echo ${SU_COMPONENT}: cleaned

${MU_TARGET}: ${OBJS} ${CLIB} ${TCPIPLIBS}
	${mkdir} aif
	${LD} -aif -o $@ ${OBJS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R

${SU_TARGET}: ${SU_OBJS} ${CLIB} ${TCPIPLIBS}
	${mkdir} aif
	${LD} -aif -o $@ ${SU_OBJS} ${TCPIPLIBS} ${CLIB}
	Access $@ RW/R


#---------------------------------------------------------------------------
# Dynamic dependencies:
