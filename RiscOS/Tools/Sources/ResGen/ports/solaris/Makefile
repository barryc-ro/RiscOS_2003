# Makefile for modgen and resgen ONLY
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Oct-99  SNB          Created
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
TARGET1   = ${COMPONENT}
TARGET2   = resgen
TARGET3   = bin2c

OBJS      = modgen.o
OBJS2     = resgen.o
OBJS3     = bin2c.o

#
# Rule patterns
#
.SUFFIXES:  .o .c
.c.o:;      ${CC} ${CFLAGS} -o $@ $<


#
# Target 
#
all: ${TARGET1} ${TARGET2} ${TARGET3}
	@echo ${TARGET1} ${TARGET2} ${TARGET3}: all complete

${TARGET1}: ${OBJS} ${CLX} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${CLX} ${CLIB}

${TARGET2}: ${OBJS2} ${CLX} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS2} ${CLX} ${CLIB}

${TARGET3}: ${OBJS3} ${CLX} ${CLIB}
	${LD} -o $@ ${LDFLAGS} ${OBJS3} ${CLX} ${CLIB}

clean:
	${WIPE} ${WFLAGS} *.o
	${RM} ${TARGET1}
	${RM} ${TARGET2}
	${RM} ${TARGET3}
	@echo ${TARGET1} ${TARGET2} ${TARGET3}: cleaned

realclean: clean
	${RM} modgen.c
	${RM} resgen.c
	${RM} bin2c.c
	${RM} VersionNum
	@echo ${TARGET1} ${TARGET2} ${TARGET3}: cleaned softlinks too

# Dynamic dependencies:
