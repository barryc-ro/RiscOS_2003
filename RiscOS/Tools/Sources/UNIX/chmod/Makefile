# Makefile for chmod
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-Aug-00  SNB          Created.
#

#
# Paths
#
CLXDIR  = C:CLX
LIBDIR	= ${INSTDIR}.Library.UNIX

#
# Generic options:
#
include Makefiles:StdTools

SQZFLAGS= -f

#
# Libraries
#
include Makefiles:AppLibs

DIRS=o._dirs

#
# Include files
#
CINCLUDES = -IC: -I${CLXDIR}
DFLAGS    = -DDDE -DNESTEDMAKEFILES

#
# Program specific options:
#
COMPONENT  = chmod
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	chmod.o riscos.o

LIBS=	${CLXLIB} ${CLIB}

#
# Rule patterns
#
include Makefiles:StdRules

#
# build it
#
all:	${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed
        
install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool installed in library

install: install_tool install_dirs
	@echo ${COMPONENT}: installed

install_dirs:
	${MKDIR} ${LIBDIR}.Docs

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@

# Dynamic dependencies:
