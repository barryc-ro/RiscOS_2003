# Makefile for AMU
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Oct-98  SJM          Created.
# 09-Nov-99  NB           Added install: rule to build FrontEnd res.
#

#
# Paths
#
CLXDIR  = C:CLX
LIBDIR	= ${INSTDIR}.Library.Acorn
INSTAPP    = ${INSTDIR}.Apps.!AMU
#
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
SQZ     = squeeze

CCFLAGS = -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK} -Wc
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V
SQZFLAGS= -f

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
CLXLIB    = ${CLXDIR}.o.clxlib

DIRS=o._dirs

#
# Include files
#
INCLUDES  = -IC: -Iriscos -I${CLXDIR}
DFLAGS    = -DDDE -DNESTEDMAKEFILES

#
# Program specific options:
#
COMPONENT  = amu
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	amu.o

LIBS=	${CLXLIB} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
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
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
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
	create $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Static dependencies:

amu.o: md.c

# Dynamic dependencies:
