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
# Paths
#
VPATH=  @ riscos
EXP_DIR		= <Tools$Dir>.Acorn
INSTDIR         = ${INSTDIR}.!${CAP_COMP}

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
SQZ	= squeeze
TOUCH   = create
WIPE    = -wipe
MKDIR   = mkdir -p

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
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = decaof
CAP_COMP   = DecAOF
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS= o.decaof o.decasd o.dem 
OBJSI= i.decaof i.decasd i.dem 
OBJSINST= oi.decaof oi.decasd oi.dem oi.instlib

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

install_tool: ${TARGET}
	${SQZ} ${SQZFLAGS} ${TARGET} ${EXP_DIR}.${COMPONENT}
        @echo ${COMPONENT}: installing in library

install: ${TARGET}
	${MKDIR} ${INSTDIR}
	${CP} LocalRes:!Boot ${INSTDIR}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTDIR}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTDIR}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTDIR}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTDIR}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTDIR}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTDIR}.Desc LocalRes:Desc

${DIRS}:
	${MKDIR} o
	${MKDIR} oi
	${MKDIR} i
	${MKDIR} inst
	${TOUCH} $@

preprocess: ${OBJSI} ${DIRS}
	@echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} inst.instlib ${DIRS}
	${LD} ${LDFLAGS} -o ${COMPONENT}i ${OBJSINST} inst.instlib ${STUBS}
	@echo ${COMPONENT}: instrument build complete


#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${HDRS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}


# Dynamic dependencies:
