# Makefile for stripdepnd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28-Sep-00  SNB          Created
#

#
# Generic options:
#
MKDIR   = mkdir -p
CC      = gcc
LD      = gcc
RM      = -rm -f
WIPE    = -rm 
CD      = cd
TOUCH   = touch
SQZ     = strip

COMPONENT = stripdepnd

#
# Internet version compatibility
#

DEFINES = ${OPTIONS}
CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS}
WFLAGS  = -rf
LDFLAGS = -g

#
# Libraries
#
CLIB      =

#
# Include files
#
INCLUDES  =

#
# Program specific options:
#
TARGET    = ${COMPONENT}

OBJS      = stripdepnd.o


#
# Rule patterns
#
.SUFFIXES:  .o .c
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# build the application
#
all: ${TARGET}
	@echo ${COMPONENT}: all complete

clean:
	${WIPE} ${WFLAGS} *.o
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

realclean: clean
	find . -type l -exec rm {} \;

#
# Target 
#
${TARGET}: ${OBJS} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${CLIB}
	${SQZ} $@

# Dynamic dependencies:
