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
DFLAGS    = 

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

OBJS=	o.bytesex o.chunkfls o.disass o.dtos o.err o.fname \
	o.ieeeflt o.prgname o.re o.wholefls 

OBJSD=	od.bytesex od.chunkfls od.disass od.dtos od.err od.fname \
	od.ieeeflt od.prgname od.re od.wholefls 

OBJSZ=	oz.bytesex oz.chunkfls oz.disass oz.dtos oz.err oz.fname \
	oz.ieeeflt oz.prgname oz.re oz.wholefls 

HDRS=	h.aif h.aof h.aout h.asdfmt h.asdllfmt h.bytesex h.chunkfls h.chunkfmt \
	h.disass h.err h.fname h.host_scr h.ieeeflt h.libflfmt h.ovrflfmt \
	h.prgname h.re h.scr_drv h.set_host h.wholefls

LIBS = ${TARGET} ${TARGETD} ${TARGETZ}

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;     ${CC} ${CCFLAGS} -DROM=0 -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DROM=1 -ff -zM -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -DROM=0         -o $@ $<
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
	${CP} h.aif ${CLXLIBDIR}.h.aif ${CPFLAGS}
	${CP} h.aof ${CLXLIBDIR}.h.aof ${CPFLAGS}
	${CP} h.aout ${CLXLIBDIR}.h.aout ${CPFLAGS}
	${CP} h.asdfmt ${CLXLIBDIR}.h.asdfmt ${CPFLAGS}
	${CP} h.asdllfmt ${CLXLIBDIR}.h.asdllfmt ${CPFLAGS}
	${CP} h.bytesex ${CLXLIBDIR}.h.bytesex ${CPFLAGS}
	${CP} h.chunkfls ${CLXLIBDIR}.h.chunkfls ${CPFLAGS}
	${CP} h.chunkfmt ${CLXLIBDIR}.h.chunkfmt ${CPFLAGS}
	${CP} h.disass ${CLXLIBDIR}.h.disass ${CPFLAGS}
	${CP} h.err ${CLXLIBDIR}.h.err ${CPFLAGS}
	${CP} h.fname ${CLXLIBDIR}.h.fname ${CPFLAGS}
	${CP} h.host_scr ${CLXLIBDIR}.h.host_scr ${CPFLAGS}
	${CP} h.ieeeflt ${CLXLIBDIR}.h.ieeeflt ${CPFLAGS}
	${CP} h.libflfmt ${CLXLIBDIR}.h.libflfmt ${CPFLAGS}
	${CP} h.ovrflfmt ${CLXLIBDIR}.h.ovrflfmt ${CPFLAGS}
	${CP} h.prgname ${CLXLIBDIR}.h.prgname ${CPFLAGS}
	${CP} h.re ${CLXLIBDIR}.h.re ${CPFLAGS}
	${CP} h.scr_drv ${CLXLIBDIR}.h.scr_drv ${CPFLAGS}
	${CP} h.set_host ${CLXLIBDIR}.h.set_host ${CPFLAGS}
	${CP} h.wholefls ${CLXLIBDIR}.h.wholefls ${CPFLAGS}
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
o.bytesex:	c.bytesex
o.bytesex:	h.bytesex
o.bytesex:	c.bytesex
o.bytesex:	h.bytesex
o.chunkfls:	c.chunkfls
o.chunkfls:	h.chunkfls
o.chunkfls:	h.chunkfmt
o.chunkfls:	h.bytesex
o.disass:	c.disass
o.disass:	h.disass
o.dtos:	c.dtos
o.dtos:	h.ieeeflt
o.err:	c.err
o.err:	h.err
o.fname:	c.fname
o.fname:	h.fname
o.ieeeflt:	c.ieeeflt
o.ieeeflt:	h.ieeeflt
o.prgname:	c.prgname
o.prgname:	h.prgname
o.re:	c.re
o.re:	h.re
o.wholefls:	c.wholefls
o.wholefls:	h.wholefls
od.bytesex:	c.bytesex
od.bytesex:	h.bytesex
od.bytesex:	c.bytesex
od.bytesex:	h.bytesex
od.chunkfls:	c.chunkfls
od.chunkfls:	h.chunkfls
od.chunkfls:	h.chunkfmt
od.chunkfls:	h.bytesex
od.disass:	c.disass
od.disass:	h.disass
od.dtos:	c.dtos
od.dtos:	h.ieeeflt
od.err:	c.err
od.err:	h.err
od.fname:	c.fname
od.fname:	h.fname
od.ieeeflt:	c.ieeeflt
od.ieeeflt:	h.ieeeflt
od.prgname:	c.prgname
od.prgname:	h.prgname
od.re:	c.re
od.re:	h.re
od.wholefls:	c.wholefls
od.wholefls:	h.wholefls
