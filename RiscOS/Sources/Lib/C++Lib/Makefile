# Makefile for c++lib
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
VPATH   = @ complex generic new os patch static stream
#
LIBDIR  = <Lib$Dir>
CPPLIBDIR = ${LIBDIR}.c++lib
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CPP     = c++
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -faho ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v ${INCLUDES} ${DFLAGS} ${THROWBACK}
CPPFLAGS = -c -depend !Depend
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

#
# Include files
#
INCLUDES  = -IC:
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = c++lib
COMPONENTZ = c++libz
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
SOURCE     = 
EXPORTS    = 

OBJS=   o.abs o.arg o.cos o.error o.exp o.io o.log \
	o.oper o.polar o.pow o.sin o.sqrt o.generic \
	o.placenew o._arr_map o._delete o._handler o._new o._vec o.file \
	o.memccpy o.dtors o._main o.pure o._ctor o._dtor o.cstreams \
	o.filebuf o.flt o.fstream o.in o.intin o.ios_compat o.manip \
	o.oldformat o.out o.rawin o.sbuf_dbp o.stdiobuf o.stream o.streambuf \
	o.strstream o.swstdio

OBJSZ=  oz.abs oz.arg oz.cos oz.error oz.exp oz.io oz.log \
	oz.oper oz.polar oz.pow oz.sin oz.sqrt oz.generic \
	oz.placenew oz._arr_map oz._delete oz._handler oz._new oz._vec oz.file \
	oz.memccpy oz.dtors oz._main oz.pure oz._ctor oz._dtor oz.cstreams \
	oz.filebuf oz.flt oz.fstream oz.in oz.intin oz.ios_compat oz.manip \
	oz.oldformat oz.out oz.rawin oz.sbuf_dbp oz.stdiobuf oz.stream oz.streambuf \
	oz.strstream oz.swstdio

LIBS = ${TARGET}

#
# Rule patterns
#
.SUFFIXES: .oz
.c.o:;     ${CC}  ${CCFLAGS}  -DROM=0 -ff     -o $@ $<
.c.oz:;    ${CC}  ${CCFLAGS}  -DROM=1 -ff -zM -o $@ $<
.c++.o:;   ${CPP} ${CPPFLAGS} -DROM=0 -ff     -o $@ $<
.c++.oz:;  ${CPP} ${CPPFLAGS} -DROM=1 -ff -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${LIBS}

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
        ${WIPE} oz ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENTZ}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.common ${CPPLIBDIR}.h.common ${CPFLAGS}
	${CP} h.complex ${CPPLIBDIR}.h.complex ${CPFLAGS}
	${CP} h.float ${CPPLIBDIR}.h.float ${CPFLAGS}
	${CP} h.fstream ${CPPLIBDIR}.h.fstream ${CPFLAGS}
	${CP} h.generic ${CPPLIBDIR}.h.generic ${CPFLAGS}
	${CP} h.iomanip ${CPPLIBDIR}.h.iomanip ${CPFLAGS}
	${CP} h.iostream ${CPPLIBDIR}.h.iostream ${CPFLAGS}
	${CP} h.libc ${CPPLIBDIR}.h.libc ${CPFLAGS}
	${CP} h.malloc ${CPPLIBDIR}.h.malloc ${CPFLAGS}
	${CP} h.memory ${CPPLIBDIR}.h.memory ${CPFLAGS}
	${CP} h.new ${CPPLIBDIR}.h.new ${CPFLAGS}
	${CP} h.osfcn ${CPPLIBDIR}.h.osfcn ${CPFLAGS}
	${CP} h.stdiostream ${CPPLIBDIR}.h.stdiostream ${CPFLAGS}
	${CP} h.stream ${CPPLIBDIR}.h.stream ${CPFLAGS}
	${CP} h.strstream ${CPPLIBDIR}.h.strstream ${CPFLAGS}
	${CP} h.values ${CPPLIBDIR}.h.values ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${CPPLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} oz

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${CPPLIBDIR}
        ${MKDIR} ${CPPLIBDIR}.h
        ${MKDIR} ${CPPLIBDIR}.o

#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

# Dynamic dependencies:
