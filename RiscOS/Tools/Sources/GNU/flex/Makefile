# Makefile for GNU flex
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Jul-99  SNB          Created.
#

#
# Paths
#
CLXDIR  = C:CLX
TINSTDIR= <Tools$Dir>.GNU
DOCSFILE= ${TINSTDIR}.docs.${COMPONENT}

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
SQZ	= squeeze
AWK	= awk
WIPE    = -wipe
BFLEX   = bflex
FLEX    = gnu.flex
YACC	= GNU.bison

CCFLAGS = -c -depend !Depend -ffa ${INCLUDES} ${DFLAGS} ${THROWBACK} -Wabcdpns
YACCCFLAGS = ${CCFLAGS} -DYYDEBUG=0
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V
SQZFLAGS = -f
AWKFLAGS = -f ${AWKSCRIPT}
YACCFLAGS =


#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
#UNIXLIB	  = TCPIPLibs:o.unixlib
CLXLIB    = ${CLXDIR}.o.clxlib

DIRS=o._dirs

#
# Include files
#
INCLUDES  = -IC: -ITCPIPLibs: -I${CLXDIR}
DFLAGS    = -DDDE -DHAVE_CONFIG_H -DPARAMS(x)=x

#
# Program specific options:
#
COMPONENT  = flex
TARGET     = ${COMPONENT}
BOOT	   = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	ccl.o dfa.o ecs.o gen.o main.o misc.o nfa.o sym.o tblcmp.o yylex.o

BOOTOBJS=	skel.o initscan.o initparse.o
GENOBJS=	parse.o skel.o scan.o


LIBS=	${CLXLIB} ${UNIXLIB} ${CLIB}

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
	@|

${BFLEX}: ${OBJS} ${BOOTOBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${BOOTOBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
	@echo Bootstrap flex tool has been built successfully

install_tool: all
	${CP} ${TARGET} ${TINSTDIR}.${COMPONENT} ${CPFLAGS}
	${CP} Docs.${COMPONENT} ${DOCSFILE} ${CPFLAGS}
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
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${BFLEX}
        ${RM} slk.flexsed
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
${TARGET}: ${OBJS} ${GENOBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${GENOBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Static dependencies:

o.skel: c.skel

c.skel: skl.flex
	sed -f skl.sedscript <skl.flex >skl.flexsed
	awk -f skl.awkscript <skl.flexsed >$@

scan.o: scan.c
	${CC} ${CCFLAGS} -o $@ $*.c

initscan.o: initscan.c parse.h
	${CC} ${CCFLAGS} -o $@ $*.c

scan.c: l.scan ${BFLEX}
	${BFLEX} -t >c.scan l.scan

h.parse c.parse: y.parse
	${YACC} ${YACCFLAGS} --name-prefix=$* -d --output=$*.c y.$*

o.parse: c.parse
	${CC} ${YACCCFLAGS} -o $@ c.$*

# Dynamic dependencies:
