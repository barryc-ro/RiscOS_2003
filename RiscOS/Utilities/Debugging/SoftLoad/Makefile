# Makefile for SoftLoad
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Aug-00  SNB          Created
#

CINCLUDES = -IC:
DIRS = o._dirs
ASFLAGS = -NoWarn -CPU ARM6

#
# Generic options:
#
include Makefiles:StdTools
include Makefiles:AppLibs
include Makefiles:AppStdRule

#
# Program specific options:
#
TARGET    = ${COMPONENT}

OBJS      = o.softload o.startnew

#
# build the tool
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
        ${MKDIR} ${INSTDIR}

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${DIRS}
        ${LD} -o $@ -aif ${OBJS} ${CLIB}
        $(SQZ) $@
        Access $@ RW/R

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

# Dynamic dependencies:
