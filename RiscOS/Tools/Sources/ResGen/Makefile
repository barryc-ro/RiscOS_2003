# Makefile for modgen, resgen, binaof and binasm
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 07-Oct-99  SNB          Created.
#

#
# Paths
#
LIBDIR	= ${INSTDIR}.Library

MODGEN     = modgen
RESGEN     = resgen
BINAOF     = binaof
BINASM     = binasm

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
LDFLAGS = -aif
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

#
# Include files
#
INCLUDES  = -IC: -IC:CLX
DFLAGS    =

#
# Program specific options:
#
COMPONENT  = ${resgen} ${modgen} ${binaof} ${binasm}
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

LIBS       =   ${CLXLIB} ${CLIB}


#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<

#
# Main target
#
all: ${COMPONENT}
        @echo ${COMPONENT}: all built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

install: ${COMPONENT}
	${MKDIR} ${LIBDIR}.Acorn.Docs
	${MKDIR} ${LIBDIR}.Build.Docs
	${CP} ${RESGEN} ${LIBDIR}.Acorn.${RESGEN} ${CPFLAGS}
	${CP} ${MODGEN} ${LIBDIR}.Build.${MODGEN} ${CPFLAGS}
	${CP} ${BINAOF} ${LIBDIR}.Build.${BINAOF} ${CPFLAGS}
	${CP} ${BINASM} ${LIBDIR}.Build.${BINASM} ${CPFLAGS}
	${CP} Docs.${RESGEN} ${LIBDIR}.Acorn.Docs.${RESGEN} ${CPFLAGS}
	${CP} Docs.${MODGEN} ${LIBDIR}.Build.Docs.${MODGEN} ${CPFLAGS}
	${CP} Docs.${BINAOF} ${LIBDIR}.Build.Docs.${BINAOF} ${CPFLAGS}
	${CP} Docs.${BINASM} ${LIBDIR}.Build.Docs.${BINASM} ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${RESGEN}
        ${RM} ${MODGEN}
        ${RM} ${BINAOF}
        ${RM} ${BINASM}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)


${MODGEN}: modgen.o ${CLXLIB} ${CLIB}
	${LD} ${LDFLAGS} -o $@ modgen.o ${CLXLIB} ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@

${RESGEN}: resgen.o ${CLXLIB} ${CLIB}
	${LD} ${LDFLAGS} -o $@ resgen.o ${CLXLIB} ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@

${BINAOF}: binaof.o
	${LD} ${LDFLAGS} -o $@ binaof.o ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@

${BINASM}: binasm.o
	${LD} ${LDFLAGS} -o $@ binasm.o ${CLIB}
	${SQUEEZE} ${SQZFLAGS} $@

# Dynamic dependencies:
