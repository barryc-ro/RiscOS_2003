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
.c.o:;     ${CC} ${CCFLAGS} -DDEBUG=0 -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DDEBUG=0 -ff -zM -o $@ $<
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
o.adler32:	c.adler32
o.adler32:	h.zlib
o.adler32:	h.zconf
o.compress:	c.compress
o.compress:	h.zlib
o.compress:	h.zconf
o.crc32:	c.crc32
o.crc32:	h.zlib
o.crc32:	h.zconf
o.uncompr:	c.uncompr
o.uncompr:	h.zlib
o.uncompr:	h.zconf
oz.adler32:	c.adler32
oz.adler32:	h.zlib
oz.adler32:	h.zconf
oz.compress:	c.compress
oz.compress:	h.zlib
oz.compress:	h.zconf
oz.crc32:	c.crc32
oz.crc32:	h.zlib
oz.crc32:	h.zconf
oz.uncompr:	c.uncompr
oz.uncompr:	h.zlib
oz.uncompr:	h.zconf
o.gzio:	c.gzio
o.gzio:	h.zutil
o.gzio:	h.zlib
o.gzio:	h.zconf
o.deflate:	c.deflate
o.deflate:	h.deflate
o.deflate:	h.zutil
o.deflate:	h.zlib
o.deflate:	h.zconf
o.trees:	c.trees
o.trees:	h.deflate
o.trees:	h.zutil
o.trees:	h.zlib
o.trees:	h.zconf
o.trees:	h.trees
o.zutil:	c.zutil
o.zutil:	h.zutil
o.zutil:	h.zlib
o.zutil:	h.zconf
o.inflate:	c.inflate
o.inflate:	h.zutil
o.inflate:	h.zlib
o.inflate:	h.zconf
o.inflate:	h.infblock
o.infblock:	c.infblock
o.infblock:	h.zutil
o.infblock:	h.zlib
o.infblock:	h.zconf
o.infblock:	h.infblock
o.infblock:	h.inftrees
o.infblock:	h.infcodes
o.infblock:	h.infutil
o.inftrees:	c.inftrees
o.inftrees:	h.zutil
o.inftrees:	h.zlib
o.inftrees:	h.zconf
o.inftrees:	h.inftrees
o.inftrees:	h.inffixed
o.infcodes:	c.infcodes
o.infcodes:	h.zutil
o.infcodes:	h.zlib
o.infcodes:	h.zconf
o.infcodes:	h.inftrees
o.infcodes:	h.infblock
o.infcodes:	h.infcodes
o.infcodes:	h.infutil
o.infcodes:	h.inffast
o.infutil:	c.infutil
o.infutil:	h.zutil
o.infutil:	h.zlib
o.infutil:	h.zconf
o.infutil:	h.infblock
o.infutil:	h.inftrees
o.infutil:	h.infcodes
o.infutil:	h.infutil
o.inffast:	c.inffast
o.inffast:	h.zutil
o.inffast:	h.zlib
o.inffast:	h.zconf
o.inffast:	h.inftrees
o.inffast:	h.infblock
o.inffast:	h.infcodes
o.inffast:	h.infutil
o.inffast:	h.inffast
oz.gzio:	c.gzio
oz.gzio:	h.zutil
oz.gzio:	h.zlib
oz.gzio:	h.zconf
oz.deflate:	c.deflate
oz.deflate:	h.deflate
oz.deflate:	h.zutil
oz.deflate:	h.zlib
oz.deflate:	h.zconf
oz.trees:	c.trees
oz.trees:	h.deflate
oz.trees:	h.zutil
oz.trees:	h.zlib
oz.trees:	h.zconf
oz.trees:	h.trees
oz.zutil:	c.zutil
oz.zutil:	h.zutil
oz.zutil:	h.zlib
oz.zutil:	h.zconf
oz.inflate:	c.inflate
oz.inflate:	h.zutil
oz.inflate:	h.zlib
oz.inflate:	h.zconf
oz.inflate:	h.infblock
oz.infblock:	c.infblock
oz.infblock:	h.zutil
oz.infblock:	h.zlib
oz.infblock:	h.zconf
oz.infblock:	h.infblock
oz.infblock:	h.inftrees
oz.infblock:	h.infcodes
oz.infblock:	h.infutil
oz.inftrees:	c.inftrees
oz.inftrees:	h.zutil
oz.inftrees:	h.zlib
oz.inftrees:	h.zconf
oz.inftrees:	h.inftrees
oz.inftrees:	h.inffixed
oz.infcodes:	c.infcodes
oz.infcodes:	h.zutil
oz.infcodes:	h.zlib
oz.infcodes:	h.zconf
oz.infcodes:	h.inftrees
oz.infcodes:	h.infblock
oz.infcodes:	h.infcodes
oz.infcodes:	h.infutil
oz.infcodes:	h.inffast
oz.infutil:	c.infutil
oz.infutil:	h.zutil
oz.infutil:	h.zlib
oz.infutil:	h.zconf
oz.infutil:	h.infblock
oz.infutil:	h.inftrees
oz.infutil:	h.infcodes
oz.infutil:	h.infutil
oz.inffast:	c.inffast
oz.inffast:	h.zutil
oz.inffast:	h.zlib
oz.inffast:	h.zconf
oz.inffast:	h.inftrees
oz.inffast:	h.infblock
oz.inffast:	h.infcodes
oz.inffast:	h.infutil
oz.inffast:	h.inffast
