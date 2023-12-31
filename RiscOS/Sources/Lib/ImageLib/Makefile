# Makefile for ImageLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 1997-06-25 BAL          Created.
# 1999-03-09 ADH          'ImageLib:' changed to '${LIBDIR}.ImageLib.'.
# 2001-06-13 ADH          Wipes o and z directories as well as contents.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>

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
WIPE    = wipe

CCFLAGS = -c -depend !Depend -ff -throwback ${INCLUDES} ${DFLAGS}
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
PNGLIB    = PNG.o.libpng-lib
PNGLIBZ   = PNG.o.libpng-lzm
ZLIB      = C:zlib.o.zlib
ZLIBZ     = C:zlib.o.zlibzm
JPEGLIB   = JPEG.o.libjpeg
LIBS      = ${PNGLIB} ${PNGLIBZ} ${ZLIB} ${ZLIBZ} ${JPEGLIB}

#
# Include files
#
INCLUDES = -Itbox:,C:,PNG,C:zlib,JPEG
DFLAGS    = -DRISCOS -DPNG_DEBUG=0

#
# Program specific options:
#
COMPONENT  = ImageLib
COMPONENTZ = ImageLibzm
SOURCE     = ...
TARGET     = o.${COMPONENT}
TARGETZ    = o.${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.cfsi\
 o.draw\
 o.gif\
 o.gif_stream\
 o.imagetrans\
 o.jpeg\
 o.jpeg_int\
 o.jpeg_parse\
 o.lzw_comp\
 o.lzw_decomp\
 o.png\
 o.render\
 o.rml_misc\
 o.sprite\
 o.xbm

OBJSZ =\
 z.cfsi\
 z.draw\
 z.gif\
 z.gif_stream\
 z.imagetrans\
 z.jpeg\
 z.jpeg_int\
 z.jpeg_parse\
 z.lzw_comp\
 z.lzw_decomp\
 z.png\
 z.render\
 z.rml_misc\
 z.sprite\
 z.xbm

#
# Rule patterns
#
.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${TARGET} ${TARGETZ} ${LIBS}

${PNGLIB}:
	dir PNG
	amu ${MFLAGS} ${MAKECMDGOALS}
	back

${PNGLIBZ}:
	dir PNG
	amu ${MFLAGS} ${MAKECMDGOALS}
	back

${JPEGLIB}:
	dir JPEG
	amu ${MFLAGS} ${MAKECMDGOALS}
	back

#
# RISC OS ROM build rules:
#
export: export_${PHASE}

clean:
        ${WIPE}		o		${WFLAGS}
        ${WIPE}		z		${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all:
	Dir		PNG
	amu		clean
	back
	Dir		JPEG
	amu		clean
	back
        @echo ${COMPONENT}: cleaned
        ${WIPE}		o		${WFLAGS}
        ${WIPE}		z		${WFLAGS}

export_hdrs: h.imagetrans o.dirs
	${CP} h.imagetrans	${LIBDIR}.ImageLib.h.imagetrans	~CFLNR~V
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} o.dirs
	${CP}	${TARGET}	${LIBDIR}.ImageLib.${TARGET}	~CFLNR~V
	${CP}	${TARGETZ}	${LIBDIR}.ImageLib.${TARGETZ}	~CFLNR~V
	${CP}	${PNGLIB}	${LIBDIR}.ImageLib.o.libpng-lib	~CFLNR~V
	${CP}	${PNGLIBZ}	${LIBDIR}.ImageLib.o.libpng-lzm	~CFLNR~V
	${CP}	${JPEGLIB}	${LIBDIR}.ImageLib.o.libjpeg	~CFLNR~V
        @echo ${COMPONENT}: export complete (libs)

o.local_dirs:
	${MKDIR} o
	${MKDIR} z
	Create o.local_dirs
	
o.dirs: o.local_dirs
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.ImageLib
        ${MKDIR} ${LIBDIR}.ImageLib.h
        ${MKDIR} ${LIBDIR}.ImageLib.o
 	Create o.dirs

#
# Final link
#
${TARGET}: ${OBJS} o.local_dirs
        ${LB} ${LBFLAGS} -o o.${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} o.local_dirs
	${LB} ${LBFLAGS} -o o.${COMPONENTZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
