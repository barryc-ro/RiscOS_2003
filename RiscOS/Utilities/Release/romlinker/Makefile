# Makefile for makeexprom
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name         Description
# ----        ----         -----------
# 16 Jan 2001 SNB          Created

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
include Makefiles:StdTools

DIRS            =o._dirs

#
# Include files
#

# Program specific options:
#
COMPONENT = romlinker
APPTARGET = ${COMPONENT}

OBJS =\
 makeexprom.o\
 makerom.o\
 memory.o\
 filereader.o\
 filewriter.o\
 romlinker.o

CINCLUDES = -IC:CLX
CDEFINES  = -DRISCOS

include Makefiles:AppStdRule
include Makefiles:AppLibs

#
# Build
#
all: ${APPTARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} ${APPTARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

install: ${APPTARGET}
        ${MKDIR} ${INSTDIR}.Library.Build.Docs
        ${CP} ${APPTARGET} ${INSTDIR}.Library.Build.${APPTARGET} ${CPFLAGS}
        ${CP} docs.Notes ${INSTDIR}.Library.Build.Docs.${APPTARGET} ${CPFLAGS}
        @echo ${COMPONENT}: installed

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

#
# Target
#
${APPTARGET}: ${OBJS} ${CLIB} ${CLXLIB} ${DIRS}
        ${LD} -o $@ ${OBJS} ${CLXLIB} ${CLIB}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@

#
# Dynamic dependencies:
