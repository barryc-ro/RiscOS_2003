# Makefile for srcbuild
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name         Description
# ----        ----         -----------
# 19 Aug 1997 RWB 	   new srcbuild
# 05 Sep 1997 RWB          added install
# 30 Apr 1999 DSC          Made to install into new library structure.
# 09 Sep 1999 JPB          Added rules to auto create and clean "o"
#                          directory
#

#
# Generic options:
#
CINCLUDES = -IC:CLX -IC:
CDEFINES  = -DRISCOS
CFLAGS    = -Wc

include Makefiles:StdTools

#
# Libraries
#
include Makefiles:AppLibs

# Program specific options:
#
COMPONENT = srcbuild
TARGET    = ${COMPONENT}

OBJS =\
 srcbuild.o\
 riscos.o\
 build.o\
 parse.o

LIBS = ${CLXLIB} ${CLIB}

DIRS      =o._dirs

#
# Rule patterns
#
include Makefiles:StdRules

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install: ${TARGET}
	${CP} ${TARGET} ${INSTDIR}.Library.Build.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: installed

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

#
# Target
#
${TARGET}: ${OBJS} ${DIRS}
        ${LD} -o $@ ${OBJS} ${LIBS}
        ${CHMOD} a+rx $@

#
# Dynamic dependencies:
