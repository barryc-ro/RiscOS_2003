# Makefile for MemLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Oct-97  SJM          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>
MEMLIBDIR = ${LIBDIR}.MemLib
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

CCFLAGS = -c -depend !Depend -fah ${INCLUDES} ${DFLAGS}
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

#
# Include files
#
INCLUDES  = -IC:
DFLAGS    = -DRISCOS -DACORN_BUILD

#
# Program specific options:
#
COMPONENT  = memlib
COMPONENTZ = memlibz
COMPONENTD = memlibd
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
TARGETD    = ${COMPONENTD}
SOURCE     = 
EXPORTS    = 

OBJS = calloc.o malloc.o mallocsize.o mallocstat.o memflex.o realloc.o subflex.o valloc.o dynamic.o
OBJSD = od.calloc od.malloc od.mallocsize od.mallocstat od.memflex od.realloc od.subflex od.valloc dynamic.o
OBJSZ = oz.calloc oz.malloc oz.mallocsize oz.mallocstat oz.memflex oz.realloc oz.subflex oz.valloc dynamic.o

HDRS = mallinfo.h memflex.h memheap.h subflex.h dynamic.h

LIBS = ${TARGET} ${TARGETD} ${TARGETZ}

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;     ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=0 -ff         -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DROM=1 -DDEBUG=0 -ff -zM	  -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=1 -DMEM_DEBUG -o $@ $<
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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENTZ}
        ${RM} ${COMPONENTD}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.mallinfo ${MEMLIBDIR}.h.* ${CPFLAGS}
	${CP} h.memflex ${MEMLIBDIR}.h.* ${CPFLAGS}
	${CP} h.memheap ${MEMLIBDIR}.h.* ${CPFLAGS}
	${CP} h.subflex ${MEMLIBDIR}.h.* ${CPFLAGS}
	${CP} h.dynamic  ${MEMLIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${MEMLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETD} ${MEMLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETZ} ${MEMLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} od

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${MEMLIBDIR}
        ${MKDIR} ${MEMLIBDIR}.h
        ${MKDIR} ${MEMLIBDIR}.o

#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${TARGETD}: ${OBJSD} local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTD} ${OBJSD}

# Dynamic dependencies:
