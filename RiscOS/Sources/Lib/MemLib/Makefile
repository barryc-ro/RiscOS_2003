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
o.memflex:	c.memflex
o.memflex:	C:h.kernel
o.memflex:	C:h.swis
o.memflex:	C:h.os
o.memflex:	h.memflex
o.memflex:	h.memheap
o.memflex:	h.dynamic
o.subflex:	c.subflex
o.subflex:	C:h.kernel
o.subflex:	C:h.os
o.subflex:	h.memflex
o.subflex:	h.subflex
od.memflex:	c.memflex
od.memflex:	C:h.kernel
od.memflex:	C:h.swis
od.memflex:	C:h.os
od.memflex:	h.memflex
od.memflex:	h.memheap
od.memflex:	h.dynamic
od.subflex:	c.subflex
od.subflex:	C:h.kernel
od.subflex:	C:h.os
od.subflex:	h.memflex
od.subflex:	h.subflex
oz.memflex:	c.memflex
oz.memflex:	C:h.kernel
oz.memflex:	C:h.swis
oz.memflex:	C:h.os
oz.memflex:	h.memflex
oz.memflex:	h.memheap
oz.memflex:	h.dynamic
oz.subflex:	c.subflex
oz.subflex:	C:h.kernel
oz.subflex:	C:h.os
oz.subflex:	h.memflex
oz.subflex:	h.subflex
o.calloc:	c.calloc
o.calloc:	h.heapintern
o.calloc:	C:h.kernel
o.calloc:	h.dynamic
o.calloc:	C:h.os
o.calloc:	h.memheap
o.calloc:	h.mallinfo
o.calloc:	c.calloc
o.calloc:	h.heapintern
o.calloc:	C:h.kernel
o.calloc:	h.dynamic
o.calloc:	C:h.os
o.calloc:	h.memheap
o.calloc:	h.mallinfo
o.malloc:	c.malloc
o.malloc:	C:h.os
o.malloc:	C:h.kernel
o.malloc:	h.memflex
o.malloc:	h.dynamic
o.malloc:	h.heapintern
o.malloc:	C:h.kernel
o.malloc:	h.dynamic
o.malloc:	h.memheap
o.malloc:	h.mallinfo
o.malloc:	VersionNum
o.mallocsize:	c.mallocsize
o.mallocsize:	h.heapintern
o.mallocsize:	C:h.kernel
o.mallocsize:	h.dynamic
o.mallocsize:	C:h.os
o.mallocsize:	h.memheap
o.mallocsize:	h.mallinfo
o.mallocstat:	c.mallocstat
o.mallocstat:	h.heapintern
o.mallocstat:	C:h.kernel
o.mallocstat:	h.dynamic
o.mallocstat:	C:h.os
o.mallocstat:	h.memheap
o.mallocstat:	h.mallinfo
o.realloc:	c.realloc
o.realloc:	h.heapintern
o.realloc:	C:h.kernel
o.realloc:	h.dynamic
o.realloc:	C:h.os
o.realloc:	h.memheap
o.realloc:	h.mallinfo
o.valloc:	c.valloc
o.valloc:	h.heapintern
o.valloc:	C:h.kernel
o.valloc:	h.dynamic
o.valloc:	C:h.os
o.valloc:	h.memheap
o.valloc:	h.mallinfo
o.valloc:	h.dynamic
od.calloc:	c.calloc
od.calloc:	h.heapintern
od.calloc:	C:h.kernel
od.calloc:	h.dynamic
od.calloc:	C:h.os
od.calloc:	h.memheap
od.calloc:	h.mallinfo
od.malloc:	c.malloc
od.malloc:	C:h.os
od.malloc:	C:h.kernel
od.malloc:	h.memflex
od.malloc:	h.dynamic
od.malloc:	h.heapintern
od.malloc:	C:h.kernel
od.malloc:	h.dynamic
od.malloc:	h.memheap
od.malloc:	h.mallinfo
od.malloc:	VersionNum
od.mallocsize:	c.mallocsize
od.mallocsize:	h.heapintern
od.mallocsize:	C:h.kernel
od.mallocsize:	h.dynamic
od.mallocsize:	C:h.os
od.mallocsize:	h.memheap
od.mallocsize:	h.mallinfo
od.mallocstat:	c.mallocstat
od.mallocstat:	h.heapintern
od.mallocstat:	C:h.kernel
od.mallocstat:	h.dynamic
od.mallocstat:	C:h.os
od.mallocstat:	h.memheap
od.mallocstat:	h.mallinfo
od.realloc:	c.realloc
od.realloc:	h.heapintern
od.realloc:	C:h.kernel
od.realloc:	h.dynamic
od.realloc:	C:h.os
od.realloc:	h.memheap
od.realloc:	h.mallinfo
od.valloc:	c.valloc
od.valloc:	h.heapintern
od.valloc:	C:h.kernel
od.valloc:	h.dynamic
od.valloc:	C:h.os
od.valloc:	h.memheap
od.valloc:	h.mallinfo
od.valloc:	h.dynamic
oz.calloc:	c.calloc
oz.calloc:	h.heapintern
oz.calloc:	C:h.kernel
oz.calloc:	h.dynamic
oz.calloc:	C:h.os
oz.calloc:	h.memheap
oz.calloc:	h.mallinfo
oz.calloc:	c.calloc
oz.calloc:	h.heapintern
oz.calloc:	C:h.kernel
oz.calloc:	h.dynamic
oz.calloc:	C:h.os
oz.calloc:	h.memheap
oz.calloc:	h.mallinfo
oz.malloc:	c.malloc
oz.malloc:	C:h.os
oz.malloc:	C:h.kernel
oz.malloc:	h.memflex
oz.malloc:	h.dynamic
oz.malloc:	h.heapintern
oz.malloc:	C:h.kernel
oz.malloc:	h.dynamic
oz.malloc:	h.memheap
oz.malloc:	h.mallinfo
oz.malloc:	VersionNum
oz.mallocsize:	c.mallocsize
oz.mallocsize:	h.heapintern
oz.mallocsize:	C:h.kernel
oz.mallocsize:	h.dynamic
oz.mallocsize:	C:h.os
oz.mallocsize:	h.memheap
oz.mallocsize:	h.mallinfo
oz.mallocstat:	c.mallocstat
oz.mallocstat:	h.heapintern
oz.mallocstat:	C:h.kernel
oz.mallocstat:	h.dynamic
oz.mallocstat:	C:h.os
oz.mallocstat:	h.memheap
oz.mallocstat:	h.mallinfo
oz.realloc:	c.realloc
oz.realloc:	h.heapintern
oz.realloc:	C:h.kernel
oz.realloc:	h.dynamic
oz.realloc:	C:h.os
oz.realloc:	h.memheap
oz.realloc:	h.mallinfo
oz.valloc:	c.valloc
oz.valloc:	h.heapintern
oz.valloc:	C:h.kernel
oz.valloc:	h.dynamic
oz.valloc:	C:h.os
oz.valloc:	h.memheap
oz.valloc:	h.mallinfo
oz.valloc:	h.dynamic
