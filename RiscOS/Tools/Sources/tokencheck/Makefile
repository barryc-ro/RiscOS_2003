# Makefile for TokenCheck
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 22-May-00  SNB          Created.
#

#
# Paths
#
CLXDIR  = C:CLX
CLXLIB  = ${CLXDIR}.o.clxlib

#
# Generic options:
#

CFLAGS = -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK} -Wc

#
# Libraries
#
include Makefiles:AppLibs

DIRS=o._dirs

#
# Include files
#
INCLUDES  = -IC: -I${CLXDIR} -IC:
DFLAGS    =

#
# Program specific options:
#
OBJS       = tokencheck.o

LIBS       = ${CLXLIB} ${CLIB}

#
# Rule patterns
#
include Makefiles:StdTools
include Makefiles:StdRules

#
# build a the library:
#
all:    ${COMPONENT}

#
# RISC OS ROM build rules:
#
rom:
        @echo ${COMPONENT}: rom module built

export:
        @echo ${COMPONENT}: export complete

install_ram: ${COMPOMENT}
        ${CP} ${COMPONENT} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: ram build installed
        
clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
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
${COMPONENT}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Static dependencies:

# Dynamic dependencies:
