# Makefile for GNU defmod
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Nov-99  SNB          Created.
#

#
# Paths
#
LIBDIR  = ${INSTDIR}.Library.Build
INSTAPP = ${INSTDIR}.Apps.!DefMod

VPATH = @ sources bs support

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
SQZ	= squeeze
WIPE    = -wipe
YACC    = GNU.bison

CCFLAGS = -c -depend !Depend -ffa ${INCLUDES} ${THROWBACK} -DTRACE=0 -Wp
YACCDEFS= -DYYDEBUG=0 -DYYMAXDEPTH=0
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK} ${ASMINC}
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V
SQZFLAGS = -f
YACCFLAGS = -y


#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
UNIXLIB	  = TCPIPLibs:o.unixlib

DIRS=o._dirs

#
# Include files
#
INCLUDES  = -Jbs.,support.,C: -Ibs -Isources -Isupport -IC:
ASMINC    = -Ibs -Isources -Isupport

#
# Program specific options:
#
COMPONENT  = defmod
ICOMPONENT = !Run
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=o.asmhelp o.callback o.chelp o.def o.defmod o.cheader o.hdr \
      o.lookup o.objasm o.realloc o.resource o.riscos o.riscosa o.cstrong

LIBS=${CLIB}

#
# Rule patterns
#
.SUFFIXES: .o .c .y .s
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.y.c:;     ${YACC} ${YACCFLAGS} -o $@ $<

o.defmod: c.defmod
	${CC} ${CCFLAGS} ${YACCDEFS} -o $@ c.defmod

#
# build defmod
#
all:	${TARGET}
	@|

install: all
	${MKDIR} ${LIBDIR}.DefMod
	${CP} ${TARGET} ${LIBDIR}.DefMod.!Run ${CPFLAGS}
	@echo Installation of tool ${COMPONENT} complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} c ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} c
	${MKDIR} o
	create $@


#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Dynamic dependencies:
