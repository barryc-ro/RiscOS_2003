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
RM      = -rm -f
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
LDFLAGS = -g
LEXFLAGS= -i

#
# Libraries
#
CLIB      =
INETLIBS  = -lsocket -lnsl

SUPPORT   = inet_net_pton.o inet_net_ntop.o
#
# Include files
#
INCLUDES =

#
# Program specific options:
#
TARGET    = ${COMPONENT}

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

realclean: clean
	find . -type l -exec rm {} \;

#
# Target 
#
${TARGET}: ${OBJS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${INETLIBS} -lresolv ${CLIB}

parser.c parser.h: parser.y
	${YACC} --defines ${YACCFLAGS} -p parser --output=parser.c parser.y

lexer.c: lexer.l
	${LEX} ${LEXFLAGS} -t >$@ lexer.l

# Dynamic dependencies:
