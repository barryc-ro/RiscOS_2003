# Makefile for Matrix Keyboard driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-00  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

#
# Generic options:
#

COMPONENT = MatrixKey
CMHGNAME  = header

#
# Internet version compatibility
#
#COMPAT = -DCOMPAT_INET4

DEFINES = ${COMPAT} ${OPTIONS}

CFLAGS    = -ffca -c -zM -zps1 -wp ${DEFINES} ${INCLUDES}
CMHGFLAGS = -p ${DEPENDS}

include Makefiles:StdTools

DIRS = o._dirs
#
# Libraries
#
include Makefiles:ModuleLibs

RAMLIBS = ${DEBUGLIB} ${NET5LIBS} ${C++LIB} ${CLIB} ${ASMUTILS}
ROMLIBS = ${DEBUGLIB} ${NET5LIBS} ${C++LIB} ${ROMCSTUBS} ${ASMUTILS}


#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =\
 o.ADTs\
 o.driver\
 o.keyboard\
 o.test\
 $(CMHGOBJ)

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .c++ .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c++.o:;    ${C++} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@


#
# build the module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
	${MKDIR} ${INSTDIR}
#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS} ${RAMLIBS} ${DIRS}
	${MKDIR} rm
        ${LD} -c++ -o $@ -rmf ${OBJS} ${RAMLIBS}
        $(MODSQZ) $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${ROMLIBS}
	${MKDIR} aof
        ${LD} -c++ -o $@ -aof ${OBJS} ${ROMLIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -c++ -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

o.keyboard o.module: VersionNum h.${CMHGNAME}

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
