# Makefile for ImageLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 1997-06-25 BAL          Created.
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
ZLIB      = ZLib.o.zlib_lib
ZLIBZ     = ZLib.o.zlib_libzm
LIBS      = ${PNGLIB} ${PNGLIBZ} ${ZLIB} ${ZLIBZ}

#
# Include files
#
INCLUDES = -Itbox:,C:
DFLAGS    = -DRISCOS

#
# Program specific options:
#
COMPONENT  = ImageLib
COMPONENTZ = ImageLibzm
SOURCE     = ...
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.cfsi\
 o.gif\
 o.gif_stream\
 o.imagetrans\
 o.jpeg\
 o.lzw_comp\
 o.lzw_decomp\
 o.png\
 o.render\
 o.riscospal\
 o.rml_misc\
 o.sprite\
 o.xbm
 
OBJSZ =\
 z.cfsi\
 z.gif\
 z.gif_stream\
 z.imagetrans\
 z.jpeg\
 z.lzw_comp\
 z.lzw_decomp\
 z.png\
 z.render\
 z.riscospal\
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
all:	${COMPONENT} ${COMPONENTZ} ${LIBS}

${PNGLIB}:
	dir PNG
	amu ${MFLAGS}
	back

${PNGLIBZ}:
	dir PNG
	amu ${MFLAGS}
	back

${ZLIB}:
	dir ZLib
	amu ${MFLAGS}
	back

${ZLIBZ}:
	dir ZLib
	amu ${MFLAGS}
	back

#
# RISC OS ROM build rules:
#
export: export_${PHASE}

clean:
        ${WIPE}		o.*		${WFLAGS}
        ${WIPE}		z.*		${WFLAGS}
        ${RM}		${TARGET}
	${RM}		${TARGETZ}	
        @echo ${COMPONENT}: cleaned

clean_all:
	Dir		PNG
	amu		clean
	back
	Dir		ZLIB
	amu		clean
	back
        @echo ${COMPONENT}: cleaned
        ${WIPE}		o.*		${WFLAGS}
        ${WIPE}		z.*		${WFLAGS}
        ${RM}		${TARGET}
	${RM}		${TARGETZ}

export_hdrs: h.imagetrans dirs
	${CP} h.imagetrans	ImageLib:h.imagetrans	~CFLNR~V
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	o.${TARGET}	ImageLib:o.${TARGET}	~CFLNR~V
	${CP}	o.${TARGETZ}	ImageLib:o.${TARGETZ}	~CFLNR~V
	${CP}	${PNGLIB}	ImageLib:o.libpng-lib	~CFLNR~V
	${CP}	${PNGLIBZ}	ImageLib:o.libpng-lzm	~CFLNR~V
	${CP}	${ZLIB}		ImageLib:o.zlib_lib	~CFLNR~V
	${CP}	${ZLIBZ}	ImageLib:o.zlib_libzm	~CFLNR~V
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z
	
dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.ImageLib
        ${MKDIR} ImageLib:h
        ${MKDIR} ImageLib:o
        
#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o o.${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o o.${COMPONENTZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.cfsi:	c.cfsi
o.cfsi:	C:h.swis
o.cfsi:	C:h.kernel
o.cfsi:	h.imagetrans
o.cfsi:	tbox:h.flex
o.cfsi:	h.internal
o.cfsi:	h.cfsi
o.gif:	c.gif
o.gif:	C:h.swis
o.gif:	C:h.kernel
o.gif:	h.defines
o.gif:	h.imagetrans
o.gif:	tbox:h.flex
o.gif:	h.datastruct
o.gif:	h.gif
o.gif:	h.gif_stream
o.gif:	h.render
o.gif:	h.rml_misc
o.gif_stream:	c.gif_stream
o.gif_stream:	C:h.kernel
o.gif_stream:	C:h.swis
o.gif_stream:	tbox:h.wimp
o.gif_stream:	h.imagetrans
o.gif_stream:	tbox:h.flex
o.gif_stream:	h.internal
o.gif_stream:	h.defines
o.gif_stream:	h.datastruct
o.gif_stream:	h.gif_stream
o.gif_stream:	h.sprite
o.gif_stream:	tbox:h.flex
o.gif_stream:	h.lzw_comp
o.gif_stream:	h.lzw_decomp
o.gif_stream:	h.render
o.gif_stream:	h.rml_misc
o.imagetrans:	c.imagetrans
o.imagetrans:	C:h.swis
o.imagetrans:	C:h.kernel
o.imagetrans:	h.imagetrans
o.imagetrans:	tbox:h.flex
o.imagetrans:	h.internal
o.imagetrans:	h.defines
o.imagetrans:	h.datastruct
o.imagetrans:	h.gif
o.imagetrans:	h.cfsi
o.imagetrans:	h.jpeg
o.imagetrans:	h.xbm
o.imagetrans:	h.png
o.imagetrans:	h.sprite
o.imagetrans:	PNG.h.png
o.imagetrans:	PNG.h.zlib
o.imagetrans:	PNG.h.zconf
o.imagetrans:	PNG.h.pngconf
o.jpeg:	c.jpeg
o.jpeg:	C:h.swis
o.jpeg:	C:h.kernel
o.jpeg:	h.imagetrans
o.jpeg:	tbox:h.flex
o.jpeg:	h.internal
o.jpeg:	h.jpeg
o.jpeg:	h.sprite
o.lzw_comp:	c.lzw_comp
o.lzw_comp:	C:h.swis
o.lzw_comp:	C:h.kernel
o.lzw_comp:	tbox:h.flex
o.lzw_comp:	h.defines
o.lzw_comp:	h.lzw_comp
o.lzw_comp:	h.imagetrans
o.lzw_comp:	h.datastruct
o.lzw_comp:	h.render
o.lzw_comp:	h.rml_misc
o.lzw_decomp:	c.lzw_decomp
o.lzw_decomp:	C:h.swis
o.lzw_decomp:	C:h.kernel
o.lzw_decomp:	tbox:h.flex
o.lzw_decomp:	h.defines
o.lzw_decomp:	h.lzw_decomp
o.png:	c.png
o.png:	C:h.swis
o.png:	C:h.kernel
o.png:	h.imagetrans
o.png:	tbox:h.flex
o.png:	h.internal
o.png:	h.png
o.png:	h.sprite
o.png:	PNG.h.png
o.png:	PNG.h.zlib
o.png:	PNG.h.zconf
o.png:	PNG.h.pngconf
o.png:	h.riscospal
o.render:	c.render
o.render:	C:h.swis
o.render:	C:h.kernel
o.render:	tbox:h.flex
o.render:	h.defines
o.render:	tbox:h.wimp
o.render:	h.imagetrans
o.render:	h.datastruct
o.render:	h.render
o.render:	h.lzw_comp
o.render:	h.lzw_decomp
o.render:	h.rml_misc
o.riscospal:	c.riscospal
o.riscospal:	PNG.h.png
o.riscospal:	PNG.h.zlib
o.riscospal:	PNG.h.zconf
o.riscospal:	PNG.h.pngconf
o.riscospal:	h.riscospal
o.rml_misc:	c.rml_misc
o.rml_misc:	C:h.swis
o.rml_misc:	C:h.kernel
o.rml_misc:	h.rml_misc
o.rml_misc:	h.defines
o.sprite:	c.sprite
o.sprite:	C:h.swis
o.sprite:	C:h.kernel
o.sprite:	tbox:h.flex
o.sprite:	h.imagetrans
o.sprite:	h.internal
o.sprite:	h.sprite
o.sprite:	h.defines
o.sprite:	h.datastruct
o.sprite:	h.gif
o.xbm:	c.xbm
o.xbm:	C:h.swis
o.xbm:	C:h.kernel
o.xbm:	tbox:h.flex
o.xbm:	h.imagetrans
o.xbm:	h.internal
o.xbm:	h.xbm
o.xbm:	h.defines
o.xbm:	h.datastruct
o.xbm:	h.gif
z.cfsi:	c.cfsi
z.cfsi:	C:h.swis
z.cfsi:	C:h.kernel
z.cfsi:	h.imagetrans
z.cfsi:	tbox:h.flex
z.cfsi:	h.internal
z.cfsi:	h.cfsi
z.gif:	c.gif
z.gif:	C:h.swis
z.gif:	C:h.kernel
z.gif:	h.defines
z.gif:	h.imagetrans
z.gif:	tbox:h.flex
z.gif:	h.datastruct
z.gif:	h.gif
z.gif:	h.gif_stream
z.gif:	h.render
z.gif:	h.rml_misc
z.gif_stream:	c.gif_stream
z.gif_stream:	C:h.kernel
z.gif_stream:	C:h.swis
z.gif_stream:	tbox:h.wimp
z.gif_stream:	h.imagetrans
z.gif_stream:	tbox:h.flex
z.gif_stream:	h.internal
z.gif_stream:	h.defines
z.gif_stream:	h.datastruct
z.gif_stream:	h.gif_stream
z.gif_stream:	h.sprite
z.gif_stream:	tbox:h.flex
z.gif_stream:	h.lzw_comp
z.gif_stream:	h.lzw_decomp
z.gif_stream:	h.render
z.gif_stream:	h.rml_misc
z.imagetrans:	c.imagetrans
z.imagetrans:	C:h.swis
z.imagetrans:	C:h.kernel
z.imagetrans:	h.imagetrans
z.imagetrans:	tbox:h.flex
z.imagetrans:	h.internal
z.imagetrans:	h.defines
z.imagetrans:	h.datastruct
z.imagetrans:	h.gif
z.imagetrans:	h.cfsi
z.imagetrans:	h.jpeg
z.imagetrans:	h.xbm
z.imagetrans:	h.png
z.imagetrans:	h.sprite
z.imagetrans:	PNG.h.png
z.imagetrans:	PNG.h.zlib
z.imagetrans:	PNG.h.zconf
z.imagetrans:	PNG.h.pngconf
z.jpeg:	c.jpeg
z.jpeg:	C:h.swis
z.jpeg:	C:h.kernel
z.jpeg:	h.imagetrans
z.jpeg:	tbox:h.flex
z.jpeg:	h.internal
z.jpeg:	h.jpeg
z.jpeg:	h.sprite
z.lzw_comp:	c.lzw_comp
z.lzw_comp:	C:h.swis
z.lzw_comp:	C:h.kernel
z.lzw_comp:	tbox:h.flex
z.lzw_comp:	h.defines
z.lzw_comp:	h.lzw_comp
z.lzw_comp:	h.imagetrans
z.lzw_comp:	h.datastruct
z.lzw_comp:	h.render
z.lzw_comp:	h.rml_misc
z.lzw_decomp:	c.lzw_decomp
z.lzw_decomp:	C:h.swis
z.lzw_decomp:	C:h.kernel
z.lzw_decomp:	tbox:h.flex
z.lzw_decomp:	h.defines
z.lzw_decomp:	h.lzw_decomp
z.png:	c.png
z.png:	C:h.swis
z.png:	C:h.kernel
z.png:	h.imagetrans
z.png:	tbox:h.flex
z.png:	h.internal
z.png:	h.png
z.png:	h.sprite
z.png:	PNG.h.png
z.png:	PNG.h.zlib
z.png:	PNG.h.zconf
z.png:	PNG.h.pngconf
z.png:	h.riscospal
z.render:	c.render
z.render:	C:h.swis
z.render:	C:h.kernel
z.render:	tbox:h.flex
z.render:	h.defines
z.render:	tbox:h.wimp
z.render:	h.imagetrans
z.render:	h.datastruct
z.render:	h.render
z.render:	h.lzw_comp
z.render:	h.lzw_decomp
z.render:	h.rml_misc
z.riscospal:	c.riscospal
z.riscospal:	PNG.h.png
z.riscospal:	PNG.h.zlib
z.riscospal:	PNG.h.zconf
z.riscospal:	PNG.h.pngconf
z.riscospal:	h.riscospal
z.rml_misc:	c.rml_misc
z.rml_misc:	C:h.swis
z.rml_misc:	C:h.kernel
z.rml_misc:	h.rml_misc
z.rml_misc:	h.defines
z.sprite:	c.sprite
z.sprite:	C:h.swis
z.sprite:	C:h.kernel
z.sprite:	tbox:h.flex
z.sprite:	h.imagetrans
z.sprite:	h.internal
z.sprite:	h.sprite
z.sprite:	h.defines
z.sprite:	h.datastruct
z.sprite:	h.gif
z.xbm:	c.xbm
z.xbm:	C:h.swis
z.xbm:	C:h.kernel
z.xbm:	tbox:h.flex
z.xbm:	h.imagetrans
z.xbm:	h.internal
z.xbm:	h.xbm
z.xbm:	h.defines
z.xbm:	h.datastruct
z.xbm:	h.gif
