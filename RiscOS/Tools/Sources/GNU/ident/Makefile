# Makefile for GNU/ident
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 21-Mar-00  SNB          Created.
#

#
# Paths
#
LIBDIR  = ${INSTDIR}.Library.GNU
INSTAPP = ${INSTDIR}.Apps.!GNUIdent

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
SQZ     = squeeze
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS} ${THROWBACK}
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
CLXLIB    = C:CLX.o.clxlib

DIRS      = o._dirs

#
# Include files
#
INCLUDES  = -IC:CLX -IC: -Iriscos
DFLAGS    = -DRCS_lint=0

#
# Program specific options:
#
COMPONENT  = ident
CAP_COMP   = GNUIdent
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS= o.ident o.rcsmap
OBJSI= i.ident i.rcsmap
OBJSINST= oi.ident oi.rcsmap

HDRS=h.rcsbase

LIBS= ${CLXLIB} ${CLIB}

#
# Rule patterns
#
.SUFFIXES: .o .oi .s .c .c++ .i .cmhg .inst
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<
.c.i:;     ${CC} ${CCFLAGS} -c -C -E $< >> $@
.inst.oi:; ${CC} ${CCFLAGS} -o $@ $<

#
# build the target:
#
all: ${TARGET}
        @echo ${COMPONENT}: ram module built

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
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
	${MKDIR} ${LIBDIR}.Docs



${TARGET}: ${OBJS} ${DIRS}
        ${LD} -o $@ ${OBJS} ${CLIB}
        ${SQZ} ${SQZFLAGS} $@

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

# Dynamic dependencies:
