# Makefile for ZLib
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

CCFLAGS = -c -depend !Depend -ffah -throwback ${INCLUDES} ${DFLAGS}
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
DFLAGS    = -DRISCOS

#
# Program specific options:
#
COMPONENT  = zlib_lib
COMPONENTZ = zlib_libzm
SOURCE     = ...
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.adler32\
 o.compress\
 o.crc32\
 o.deflate\
 o.gzio\
 o.infblock\
 o.infcodes\
 o.inffast\
 o.inflate\
 o.inftrees\
 o.infutil\
 o.trees\
 o.uncompress\
 o.zutil

OBJSZ =\
 z.adler32\
 z.compress\
 z.crc32\
 z.deflate\
 z.gzio\
 z.infblock\
 z.infcodes\
 z.inffast\
 z.inflate\
 z.inftrees\
 z.infutil\
 z.trees\
 z.uncompress\
 z.zutil

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
all:	${COMPONENT} ${COMPONENTZ}

#
# RISC OS ROM build rules:
#
clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} z.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

local_dirs:
	${MKDIR} o
	${MKDIR} z
	
#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o o.${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o o.${COMPONENTZ} ${OBJSZ}

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
o.deflate:	c.deflate
o.deflate:	h.deflate
o.deflate:	h.zutil
o.deflate:	h.zlib
o.deflate:	h.zconf
o.gzio:	c.gzio
o.gzio:	h.zutil
o.gzio:	h.zlib
o.gzio:	h.zconf
o.infblock:	c.infblock
o.infblock:	h.zutil
o.infblock:	h.zlib
o.infblock:	h.zconf
o.infblock:	h.infblock
o.infblock:	h.inftrees
o.infblock:	h.infcodes
o.infblock:	h.infutil
o.infcodes:	c.infcodes
o.infcodes:	h.zutil
o.infcodes:	h.zlib
o.infcodes:	h.zconf
o.infcodes:	h.inftrees
o.infcodes:	h.infblock
o.infcodes:	h.infcodes
o.infcodes:	h.infutil
o.infcodes:	h.inffast
o.inffast:	c.inffast
o.inffast:	h.zutil
o.inffast:	h.zlib
o.inffast:	h.zconf
o.inffast:	h.inftrees
o.inffast:	h.infblock
o.inffast:	h.infcodes
o.inffast:	h.infutil
o.inffast:	h.inffast
o.inflate:	c.inflate
o.inflate:	h.zutil
o.inflate:	h.zlib
o.inflate:	h.zconf
o.inflate:	h.infblock
o.inftrees:	c.inftrees
o.inftrees:	h.zutil
o.inftrees:	h.zlib
o.inftrees:	h.zconf
o.inftrees:	h.inftrees
o.infutil:	c.infutil
o.infutil:	h.zutil
o.infutil:	h.zlib
o.infutil:	h.zconf
o.infutil:	h.infblock
o.infutil:	h.inftrees
o.infutil:	h.infcodes
o.infutil:	h.infutil
o.trees:	c.trees
o.trees:	h.deflate
o.trees:	h.zutil
o.trees:	h.zlib
o.trees:	h.zconf
o.uncompress:	c.uncompress
o.uncompress:	h.zlib
o.uncompress:	h.zconf
o.zutil:	c.zutil
o.zutil:	h.zutil
o.zutil:	h.zlib
o.zutil:	h.zconf
z.adler32:	c.adler32
z.adler32:	h.zlib
z.adler32:	h.zconf
z.compress:	c.compress
z.compress:	h.zlib
z.compress:	h.zconf
z.crc32:	c.crc32
z.crc32:	h.zlib
z.crc32:	h.zconf
z.deflate:	c.deflate
z.deflate:	h.deflate
z.deflate:	h.zutil
z.deflate:	h.zlib
z.deflate:	h.zconf
z.gzio:	c.gzio
z.gzio:	h.zutil
z.gzio:	h.zlib
z.gzio:	h.zconf
z.infblock:	c.infblock
z.infblock:	h.zutil
z.infblock:	h.zlib
z.infblock:	h.zconf
z.infblock:	h.infblock
z.infblock:	h.inftrees
z.infblock:	h.infcodes
z.infblock:	h.infutil
z.infcodes:	c.infcodes
z.infcodes:	h.zutil
z.infcodes:	h.zlib
z.infcodes:	h.zconf
z.infcodes:	h.inftrees
z.infcodes:	h.infblock
z.infcodes:	h.infcodes
z.infcodes:	h.infutil
z.infcodes:	h.inffast
z.inffast:	c.inffast
z.inffast:	h.zutil
z.inffast:	h.zlib
z.inffast:	h.zconf
z.inffast:	h.inftrees
z.inffast:	h.infblock
z.inffast:	h.infcodes
z.inffast:	h.infutil
z.inffast:	h.inffast
z.inflate:	c.inflate
z.inflate:	h.zutil
z.inflate:	h.zlib
z.inflate:	h.zconf
z.inflate:	h.infblock
z.inftrees:	c.inftrees
z.inftrees:	h.zutil
z.inftrees:	h.zlib
z.inftrees:	h.zconf
z.inftrees:	h.inftrees
z.infutil:	c.infutil
z.infutil:	h.zutil
z.infutil:	h.zlib
z.infutil:	h.zconf
z.infutil:	h.infblock
z.infutil:	h.inftrees
z.infutil:	h.infcodes
z.infutil:	h.infutil
z.trees:	c.trees
z.trees:	h.deflate
z.trees:	h.zutil
z.trees:	h.zlib
z.trees:	h.zconf
z.uncompress:	c.uncompress
z.uncompress:	h.zlib
z.uncompress:	h.zconf
z.zutil:	c.zutil
z.zutil:	h.zutil
z.zutil:	h.zlib
z.zutil:	h.zconf
