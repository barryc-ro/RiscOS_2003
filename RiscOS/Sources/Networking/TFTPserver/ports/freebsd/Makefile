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

ARCH=freebsd

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
LEX	= flex
YACC	= bison

COMPONENT = mtftpd

#
# Internet version compatibility
#

DEFINES = ${COMPAT} ${OPTIONS} -DARCHTYPES="\"${ARCH}.h\""

CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS}
WFLAGS  = -rf
LEXFLAGS= -i

#
# Libraries
#
CLIB      =
#INETLIBS  = -lsocket -lnsl -lresolv

SUPPORT   = inet_net_pton.o inet_net_ntop.o
#
# Include files
#
INCLUDES =

#
# Program specific options:
#
TARGET    = exe.${COMPONENT}

OBJS      = tftpserver.o main.o Strcmp_ci.o Strdup.o momanager.o \
mofile.o moobject.o configure.o i18n.o icmpsocket.o parser.o lexer.o \
${ARCH}.o ${SUPPORT}


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
	${RM} parser.c
	${RM} lexer.c
	${RM} parser.h
	@echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${INETLIBS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${INETLIBS} ${CLIB}

parser.c parser.h: parser.y
	${YACC} ${YACCFLAGS} --name-prefix=parser -d --output=parser.c parser.y

lexer.c: lexer.l
	${LEX} ${LEXFLAGS} -t >$@ lexer.l

# Dynamic dependencies:
