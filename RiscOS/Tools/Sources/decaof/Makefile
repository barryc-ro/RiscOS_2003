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

LIBDIR	= ${INSTDIR}.Library.Acorn
INSTAPP    = ${INSTDIR}.Apps.!DecAOF

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
MKDIR   = do mkdir -p

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
DFLAGS    = -DEXECUTE_ON_RISCOS

#
# Program specific options:
#
COMPONENT  = decaof
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
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: installed in library

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
	${MKDIR} ${LIBDIR}
	${MKDIR} ${INSTAPP}

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
	${SQZ} ${SQZFLAGS} $@

# Dynamic dependencies:
