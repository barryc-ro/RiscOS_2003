# Makefile for cfront
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Oct-98  SJM          Created.
#

#
# Paths
#
TOOLSDIR=<Tools$Dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CPP     = c++
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe
SQUEEZE = squeeze

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS} ${THROWBACK}
CPPFLAGS= -c -depend !Depend -zpz0 -ff -Wp ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
LDFLAGS = -aif
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
CPPLIB    = C:c++lib.o.c++lib
#
# Include files
#
INCLUDES  = -IC: -IC:C++Lib
DFLAGS    = -DTHROWBACK

#
# Program specific options:
#
COMPONENT  = cfront
TARGET     = ${COMPONENT}
SOURCE     =
EXPORTS    =


OBJS=	o.alloc o.bits o.block o.dcl o.dcl2 o.dcl3 o.dcl4 o.del o.discrim \
        o.error o.expand o.expr o.expr2 o.expr3 o.find o.hash o.lalex o.lex \
        o.main o.norm o.norm2 o.print o.print2 o.repr o.simpl o.simpl2 \
        o.size o.table o.template o.tree_copy o.tree_walk o.typ o.typ2 o.ytab \
        o.throwback

LIBS=	${CPPLIB} ${CLIB}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<

.c++.o:;   ${CPP} ${CPPFLAGS} -o $@ $<

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
        ${CP} ${TARGET} ${TOOLSDIR}.Acorn.cfront ${CPFLAGS}
        @echo ${COMPONENT}: built and installed in library

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs:
	@echo ${COMPONENT}: export complete (hdrs)

export_libs:
        @echo ${COMPONENT}: export complete (libs)

#
# Special targets
#
o._dirs:
	${MKDIR} o
	create $@

o.ytab: c++.ytab
        @echo "WARNING - this file may take a long time to compile  "
        @echo "          due to the attempt to optimise the huge    "
        @echo "          switch statement in this source file.      "
        @echo "          Don't panic!                               "
        ${CPP} ${CPPFLAGS} -o $@ c++.$*

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} o._dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} $@

# Dynamic dependencies:
