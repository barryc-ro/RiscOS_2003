# Makefile for modgen ONLY
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

COMPONENT = modgen

DEFINES = ${OPTIONS}

CFLAGS  = -c -g ${DEFINES} ${INCLUDES} ${DEPENDS}
WFLAGS  = -rf

#
# Libraries
#
CLIB      = 
CLX       = ${CLXPATH}/clx.a

#
# Include files
#
INCLUDES = -I${CLXPATH}

#
# Program specific options:
#
TARGET    = ${COMPONENT}

OBJS      = modgen.o


#
# Rule patterns
#
.SUFFIXES:  .o .c
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# Target 
#
${TARGET}: ${OBJS} ${CLX} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${CLX} ${CLIB}

all: ${TARGET}
	@echo ${COMPONENT}: all complete

clean:
	${WIPE} ${WFLAGS} *.o
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

realclean: clean
	${RM} modgen.c
	${RM} VersionNum
	@echo ${COMPONENT}: cleaned softlinks too

# Dynamic dependencies:
