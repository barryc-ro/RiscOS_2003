# Makefile for ITable
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 05Dec95      diz          Created
# [ changes - see source control logs or s.itable ]
# 14 Aug 2000  jrf          Removed requirement for resources
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
Linkflags = -rmf -o $@

#
# Libraries
#

AsmUtilLib = C:AsmUtils.o.AsmUtilszm

#
# Program specific options:
#
COMPONENT = ITable
SOURCE    = s.ITable
TARGET    = rm.${COMPONENT}
EXPORTS   = ${EXP_HDR}.${COMPONENT}

RESDIR = <resource$dir>.Resources.${COMPONENT}

#
# Rule patterns
#
.SUFFIXES:	.d_o .o .s .c .c++ .i .cmhg .inst .io

.c++.o:;    ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c++.d_o:;  ${C++} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.d_o:;    ${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@
.c++.i:;    ${C++} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} ${DFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${CFLAGS} ${DFLAGS} -W -o $@ $<

#
# Objects
#
OBJS	=	\
o.itable

#
# Generic rules:
#
rom: ${TARGET} ${OBJS} dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
	${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        | Removed because we're using the ColourTrans resource file now
        |${MKDIR} ${RESDIR}
        |${MKDIR} ${RESDIR}.Tables
        |${CP} Tables.8desktop ${RESDIR}.Tables.8desktop ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_Dir tables

dirs:
	${MKDIR} o

${TARGET}: ${OBJS}
	${LD} ${Linkflags} ${OBJS} ${AsmUtilLib}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
