# Makefile for CLX
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 016-Oct-98  SJM          Created.
#

#
# Paths
#
LIBDIR = <Lib$Dir>
CLXLIBDIR = ${LIBDIR}.CLX
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

CCFLAGS = -c -depend !Depend -ffaho ${INCLUDES} ${DFLAGS}
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
DFLAGS    = -Dkernelheader="\"kernel.h\""

#
# Program specific options:
#
COMPONENT  = clxlib
COMPONENTZ = clxlibz
COMPONENTD = clxlibd
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
TARGETD    = ${COMPONENTD}
SOURCE     = 
EXPORTS    = 

OBJS=	o.bytesex o.chunkfls o.disass o.disass16 o.dtos o.err o.fname \
	o.host o.hash o.ieeeflt o.prgname o.re o.wholefls \
	o.filestamp o.dir o.wildcard o.dem o.symdefs o.pathmacro \
	o.mapping

OBJSD=	od.bytesex od.chunkfls od.disass od.disass16 od.dtos od.err od.fname \
	od.host od.hash od.ieeeflt od.prgname od.re od.wholefls \
	od.filestamp od.dir od.wildcard od.dem od.symdefs od.pathmacro \
	od.mapping

OBJSZ=	oz.bytesex oz.chunkfls oz.disass oz.disass16 oz.dtos oz.err oz.fname \
	oz.host oz.hash oz.ieeeflt oz.prgname oz.re oz.wholefls \
	oz.filestamp oz.dir oz.wildcard oz.dem oz.symdefs oz.pathmacro \
	oz.mapping

HDRS=	h.aif h.aof h.aout h.asdfmt h.asdllfmt h.bytesex h.chunkfls h.chunkfmt \
	h.disass h.err h.fname h.hash h.host h.ieeeflt h.libflfmt h.ovrflfmt \
	h.prgname h.re h.wholefls h.filestamp h.dir h.wildcard h.dem h.symdefs \
	h.pathmacro h.mapping

LIBS = ${TARGET} ${TARGETD} ${TARGETZ}

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;     ${CC} ${CCFLAGS} -DROM=0 -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DROM=1 -ff -zM -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -DROM=0 -g      -o $@ $<
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
        ${WIPE} od ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENTZ}
        ${RM} ${COMPONENTD}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.aif ${CLXLIBDIR}.h.aif ${CPFLAGS}
	${CP} h.aof ${CLXLIBDIR}.h.aof ${CPFLAGS}
	${CP} h.aout ${CLXLIBDIR}.h.aout ${CPFLAGS}
	${CP} h.asdfmt ${CLXLIBDIR}.h.asdfmt ${CPFLAGS}
	${CP} h.asdllfmt ${CLXLIBDIR}.h.asdllfmt ${CPFLAGS}
	${CP} h.bytesex ${CLXLIBDIR}.h.bytesex ${CPFLAGS}
	${CP} h.chunkfls ${CLXLIBDIR}.h.chunkfls ${CPFLAGS}
	${CP} h.chunkfmt ${CLXLIBDIR}.h.chunkfmt ${CPFLAGS}
	${CP} h.dem ${CLXLIBDIR}.h.dem ${CPFLAGS}
	${CP} h.disass ${CLXLIBDIR}.h.disass ${CPFLAGS}
	${CP} h.elf ${CLXLIBDIR}.h.elf ${CPFLAGS}
	${CP} h.err ${CLXLIBDIR}.h.err ${CPFLAGS}
	${CP} h.filestamp ${CLXLIBDIR}.h.filestamp ${CPFLAGS}
	${CP} h.dir ${CLXLIBDIR}.h.dir ${CPFLAGS}
	${CP} h.fname ${CLXLIBDIR}.h.fname ${CPFLAGS}
	${CP} h.hash ${CLXLIBDIR}.h.hash ${CPFLAGS}
	${CP} h.host ${CLXLIBDIR}.h.host ${CPFLAGS}
	${CP} h.ieeeflt ${CLXLIBDIR}.h.ieeeflt ${CPFLAGS}
	${CP} h.libflfmt ${CLXLIBDIR}.h.libflfmt ${CPFLAGS}
	${CP} h.mapping ${CLXLIBDIR}.h.mapping ${CPFLAGS}
	${CP} h.ovrflfmt ${CLXLIBDIR}.h.ovrflfmt ${CPFLAGS}
	${CP} h.pathmacro ${CLXLIBDIR}.h.pathmacro ${CPFLAGS}
	${CP} h.prgname ${CLXLIBDIR}.h.prgname ${CPFLAGS}
	${CP} h.re ${CLXLIBDIR}.h.re ${CPFLAGS}
	${CP} h.symdefs ${CLXLIBDIR}.h.symdefs ${CPFLAGS}
	${CP} h.wholefls ${CLXLIBDIR}.h.wholefls ${CPFLAGS}
	${CP} h.wildcard ${CLXLIBDIR}.h.wildcard ${CPFLAGS}
	${CP} VersionNum ${CLXLIBDIR}.CLXVersion ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${CLXLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETD} ${CLXLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETZ} ${CLXLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} od

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${CLXLIBDIR}
        ${MKDIR} ${CLXLIBDIR}.h
        ${MKDIR} ${CLXLIBDIR}.o

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
