# Makefile for EraseCVS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06 May 1997  RWB        New for !EraseCVS
# 23 Sep 1999  SNB        Added install: phase.
#

#
# Generic options:
#
include Makefiles:StdTools

CHMOD      = access
CHMODFLAGS = RW/R

#
# Libraries
#
include Makefiles:AppLibs
#
# Include files
#
CINCLUDES = -ITbox:,C:

include Makefiles:StdRules

# Program specific options:
#
COMPONENT := EraseCVS
APP       = ${INSTDIR}.!${COMPONENT}
TARGET    = !RunImage

OBJS      = o.main o.cmdline
DIRS      = o._dirs

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
        ${MKDIR} ${INSTDIR}
        ${CP} Resources ${APP} ${CPFLAGS}
        ${SQZ} ${TARGET} ${APP}.${TARGET}
        ${CHMOD} ${APP} ${CHMODFLAGS}
        ${CHMOD} ${APP}.* ${CHMODFLAGS}
        @echo ${COMPONENT}: installed

#
# Target
#
${TARGET}: ${OBJS} ${TBOXLIBS} ${CLIB} ${DIRS}
        ${LD} -o $@ ${OBJS} ${TBOXLIBS} ${CLIB}

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

#
# Dynamic dependencies:
