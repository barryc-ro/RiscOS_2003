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
CPPFLAGS= -c -depend !Depend -ff ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
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

CMD=cmd.cpp
CMD_C=cmd.c.c++
CMD_O=cmd.o.c++

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

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} cmd.o ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${CMD}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} cmd.o

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} local_dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}
	${SQUEEZE} $@

${CMD}: ${CMD_O} ${CLIB} local_dirs
        ${LD} ${LDFLAGS} -o $@ ${CMD_O} ${CLIB}
	${SQUEEZE} $@

# Dynamic dependencies:
