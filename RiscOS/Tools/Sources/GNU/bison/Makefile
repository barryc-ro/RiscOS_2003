# Makefile for GNU bison
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
TDOCSDIR= ${TINSTDIR}.bisonparsers
DOCSFILE= ${TINSTDIR}.docs.${COMPONENT}

AWKSCRIPT=parsers.awkscript
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

CCFLAGS = -c -depend !Depend -ffa ${INCLUDES} ${DFLAGS} ${THROWBACK} -Wcdpans
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V
SQZFLAGS = -f
AWKFLAGS = -f ${AWKSCRIPT}


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
CLXLIB    = ${CLXDIR}.o.clxlib

DIRS=o._dirs
VERSION=o._version

SIMPLE=parsers.simple
HAIRY=parsers.hairy

#
# Include files
#
INCLUDES  = -IC: -ITCPIPLibs: -I${CLXDIR}
DFLAGS    = -DDDE -DHAVE_CONFIG_H -DPARAMS(x)=x

#
# Program specific options:
#
COMPONENT  = bison
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 


OBJS=	LR0.o allocate.o closure.o conflicts.o derives.o files.o getargs.o\
	gram.o lalr.o lex.o main.o nullable.o output.o print.o reader.o\
	reduce.o symtab.o warshall.o getopt.o getopt1.o

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
all:	${TARGET} ${SIMPLE} ${HAIRY}
	@|

install_tool: all ${AWKSCRIPT}
	${MKDIR} ${TDOCSDIR}
	${CP} ${TARGET} ${TINSTDIR}.${COMPONENT} ${CPFLAGS}
	${CP} ${SIMPLE} ${TDOCSDIR}.bison/simple ${CPFLAGS}
	${CP} ${HAIRY}  ${TDOCSDIR}.bison/hairy	${CPFLAGS}
	do ${AWK} ${AWKFLAGS}  >${DOCSFILE} <Docs.${COMPONENT}
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
        ${RM} ${SIMPLE}
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
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@


# Static dependencies:

${SIMPLE}: parsers.simplesrc ${AWKSCRIPT}
	${AWK} ${AWKFLAGS} <parsers.simplesrc >$@

# Dynamic dependencies:
