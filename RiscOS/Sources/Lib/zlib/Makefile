# Makefile for zlib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Mar-98  KJB          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>
ZLIBDIR = ${LIBDIR}.zlib
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

CCFLAGS = -c -depend !Depend ${INCLUDES} ${DFLAGS} ${THROWBACK}
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
INCLUDES  =
DFLAGS    = -DRISCOS

#
# Program specific options:
#
COMPONENT  = zlib
COMPONENTZ = zlibzm
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
SOURCE     = 
EXPORTS    = 

OBJS = \
 o.adler32 \
 o.compress \
 o.crc32 \
 o.gzio \
 o.uncompr \
 o.deflate \
 o.trees \
 o.zutil \
 o.inflate \
 o.infblock \
 o.inftrees \
 o.infcodes \
 o.infutil \
 o.inffast

OBJSZ = \
 oz.adler32 \
 oz.compress \
 oz.crc32 \
 oz.gzio \
 oz.uncompr \
 oz.deflate \
 oz.trees \
 oz.zutil \
 oz.inflate \
 oz.infblock \
 oz.inftrees \
 oz.infcodes \
 oz.infutil \
 oz.inffast

HDRS = zlib.h zconf.h

LIBS = ${TARGET} ${TARGETZ} 

#
# Rule patterns
#
.SUFFIXES: .o .oz .od .c .h .s
.c.o:;     ${CC} ${CCFLAGS} -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -ff -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build the library:
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
	${CP} h.zlib  ${ZLIBDIR}.h.* ${CPFLAGS}
	${CP} h.zconf ${ZLIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${ZLIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETZ} ${ZLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)


o.local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${TOUCH} o.local_dirs

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${ZLIBDIR}
        ${MKDIR} ${ZLIBDIR}.h
        ${MKDIR} ${ZLIBDIR}.o

links:
	./mklinks
	echo Made links

#
# Final link
#
${TARGET}: ${OBJS} o.local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} o.local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

# Dynamic dependencies:
