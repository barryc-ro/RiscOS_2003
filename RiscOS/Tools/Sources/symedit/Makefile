# Makefile for symedit
#
# $Id$
#

#
# Paths
#
LIBDIR     = ${INSTDIR}.Library.Acorn
INSTAPP    = ${INSTDIR}.Apps.!SymEdit
CINCLUDES  = -IC:CLX
CFLAGS     = -ffah

#
# Generic options:
#
include Makefiles:StdTools
include Makefiles:AppLibs
include Makefiles:AppStdRule

#
# Program specific options:
#
COMPONENT = symedit
TARGET    = ${COMPONENT}
OBJS      = symedit.o
LIBS      = ${CLXLIB} ${CLIB}
DIRS=o._dirs

#
# build a the library:
#
all:      ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed
        
install_tool: ${TARGET}
        ${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: tool installed in library

install: install_tool install_dirs
        @echo ${COMPONENT}: install complete

install_dirs:
        ${MKDIR} ${INSTAPP}
        ${MKDIR} ${LIBDIR}.Docs

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Dynamic dependencies:
