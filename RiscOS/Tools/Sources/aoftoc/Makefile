# Makefile for aoftoc
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
WFLAGS  = ~CF~V

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
DFLAGS    = -DDDE

#
# Program specific options:
#
COMPONENT  = aoftoc
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS=	o.aoftoc

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

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

local_dirs: 
	${MKDIR} o

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
	${SQUEEZE} ${COMPONENT}

# Dynamic dependencies:
