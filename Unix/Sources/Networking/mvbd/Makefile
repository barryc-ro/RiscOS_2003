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

COMPONENT = mvbd

DEFINES = ${COMPAT} ${OPTIONS}
CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS} -Wall 
WFLAGS  = -rf
LEXFLAGS= -i

#
# Libraries
#
CLIB      = -lm
#INETLIBS  = -lsocket -lnsl -lresolv

SUPPORT   = inet_net_pton.o inet_net_ntop.o

#
# Include files
#
INCLUDES =

#
# Program specific options:
#
TARGET    = ${COMPONENT}

OBJS      = ${SUPPORT} main.o parser.o lexer.o Strdup.o Strcmp_ci.o mofile.o \
i18n.o configure.o platform.o moobject.o


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
${TARGET}: ${OBJS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${INETLIBS} ${CLIB}

parser.c parser.h: parser.y
	${YACC} ${YACCFLAGS} --name-prefix=parser -d --output=parser.c parser.y

lexer.c: lexer.l
	${LEX} ${LEXFLAGS} -t >$@ lexer.l

# Dynamic dependencies:
