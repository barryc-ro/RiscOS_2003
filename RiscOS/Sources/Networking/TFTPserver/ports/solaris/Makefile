# Makefile for Multicaster (application)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Jul-99  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

ARCH=x86solaris

#
# Generic options:
#
AWK     = awk
MKDIR   = mkdir
CC      = gcc
LD      = gcc
RM      = -rm
WIPE    = -rm
CD	= cd
TOUCH   = touch

COMPONENT = mtftpd

#
# Internet version compatibility
#

DEFINES = ${COMPAT} ${OPTIONS} -DARCHTYPES="\"${ARCH}.h\""

CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS}
WFLAGS  = -rf

#
# Libraries
#
CLIB      =
INETLIBS  = -lsocket -lnsl

#
# Include files
#
INCLUDES =

#
# Program specific options:
#
TARGET    = exe.${COMPONENT}

OBJS      = tftpserver.o main.o Strcmp_ci.o Strdup.o momanager.o \
mofile.o moobject.o configure.o i18n.o icmpsocket.o ${ARCH}.o

#
# Rule patterns
#
.SUFFIXES:  .o .c
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# build a TFTP server
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
	${WIPE} ${WFLAGS} *.o
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${INETLIBS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${INETLIBS} ${CLIB}

# Dynamic dependencies:
