# Makefile for Squeeze
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 10-Feb-99  SNB          Created.
#

LIBDIR	= ${INSTDIR}.Library.Acorn
INSTAPP    = ${INSTDIR}.Apps.!Squeeze

# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe
SQUEEZE = squeeze

CCFLAGS = -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
CLXLIB    = C:CLX.o.clxlib

ODIR      = o._dirs

#
# Include files
#
INCLUDES  = -IC: -IC:CLX
DFLAGS    = -DDDE -DDEBUGGING=0

#
# Program specific options:
#
COMPONENT  = squeeze/xpand
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

SQZOBJS    = squeeze.o unsqueeze.o unsqrm.o ${LIBS}
XPANDOBJS  = xpand.o asmcall.o ${LIBS}

LIBS=	${CLXLIB} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<

#
# Main target
#
all: xpand squeeze
	@echo ${COMPONENT}: all built

squeeze: ${SQZOBJS} ${ODIR}
	${LD} -o $@ ${SQZOBJS}
	squeeze $@

xpand: ${XPANDOBJS} ${ODIR}
	${LD} -o $@ ${XPANDOBJS}
	squeeze $@

xpand.o: squeeze.h

clean:
	${RM} squeeze
	${RM} xpand
	${WIPE} o ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${ODIR}:
	${MKDIR} o
	${TOUCH} $@

install_tool: all
	${CP} squeeze ${LIBDIR}.squeeze ${CPFLAGS}
	${CP} xpand ${LIBDIR}.xpand ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

install: install_tool install_dirs
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Setup ${INSTAPP}.!Setup ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBDIR}.Docs

# Dynamic dependencies:
