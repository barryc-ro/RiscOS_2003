# Makefile for Vector
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 16 Aug 2000  JRF          BigSplit2

#
# Paths
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
DEFMOD  = DefMod
AR	= LibFile
CHMOD	= access

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${THROWBACK} ${INCLUDES} ${DFLAGS} ${OPTIONS}
CMHGFLAGS = -p ${DFLAGS} ${INCLUDES}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
DFLAGS  =
CHMODFLAGS = RW/R
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
INETLIB    = TCPIPLibs:o.inetlib
UNIXLIB    = TCPIPLibs:o.unixlib
REMOTEDB   = <Lib$Dir>.debug.o.remote
#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = Vector
TARGET    = abs.${COMPONENT}

.SUFFIXES:   .od

OBJS      =	o.vector
DOBJS	  =     od.vector

#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} $< $@
.s.o:;      ${AS} ${AFLAGS}  $< $@
.c.od:;     ${CC} ${CFLAGS} -o $@ $< -DDEBUG
.cmhg.od:;  ${CMHG} ${CMHGFLAGS} $< $@ -DDEBUG
.s.od:;     ${AS} ${AFLAGS} $< $@
#
# build a relocatable module:
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: complete

debug: abs.${COMPONENT}D
        @echo ${COMPONENT}D: complete

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} abs ${WFLAGS}
        @echo ${COMPONENT}: cleaned
dirs:
	${MKDIR} rm
	${MKDIR} o
	${MKDIR} od
	${MKDIR} linked
	${MKDIR} map
	${MKDIR} abs

#
# App target
#
abs.${COMPONENT}D: ${DOBJS} ${CLIB}
        ${LD} -o $@ ${DOBJS} ${CLIB}
        ${CHMOD} $@ ${CHMODFLAGS}

abs.${COMPONENT}: ${OBJS} ${CLIB}
        ${LD} -o $@ ${OBJS} ${CLIB}
        ${CHMOD} $@ ${CHMODFLAGS}

#
# Dynamic dependencies:
