# Makefile for crc32
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
RM      = -rm
WIPE    = -rm
CD	= cd
TOUCH   = touch
STRIP   = strip

COMPONENT = crc

#
# Internet version compatibility
#

DEFINES = ${OPTIONS}

CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS}
WFLAGS  = -rf

#
# Libraries
#
CLIB      =

#
# Include files
#
INCLUDES = 

#
# Program specific options:
#
TARGET    = ${COMPONENT}

OBJS      = calc.o prog.o

#
# Rule patterns
#
.SUFFIXES:  .o .c
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# build crc32
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
${TARGET}: ${OBJS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${CLIB}
	${STRIP} $@

# Dynamic dependencies:
