# Makefile for decaof
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Sep-98  SNB          Created.
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

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS} ${THROWBACK}
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

#
# Include files
#
INCLUDES  = -IC:CLX -IC:
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = decaof
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS= o.decaof o.decasd 
OBJSI= i.decaof i.decasd 
OBJSINST= oi.decaof oi.decasd oi.instlib

HDRS=h.decasd 

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
        ${WIPE} oi ${WFLAGS}
        ${WIPE} i ${WFLAGS}
        ${WIPE} inst ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} oi
	${MKDIR} i
	${MKDIR} inst

preprocess: ${OBJSI} local_dirs
	@echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} inst.instlib local_dirs 
	${LD} ${LDFLAGS} -o ${COMPONENT}i ${OBJSINST} inst.instlib ${STUBS}
	@echo ${COMPONENT}: instrument build complete


#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${HDRS} local_dirs
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}


# Dynamic dependencies:
