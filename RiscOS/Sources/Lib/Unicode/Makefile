# Makefile for Unicode
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
UNICODELIBDIR = ${LIBDIR}.Unicode
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
INCLUDES  =
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = ucodelib
COMPONENTZ = ucodelibz
COMPONENTD = ucodelibd
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
TARGETD    = ${COMPONENTD}
SOURCE     = 
EXPORTS    = 

OBJS = autojp.o \
        unictype.o \
	utf8.o \
	encoding.o \
	iso2022.o \
	eightbit.o \
	shiftjis.o \
	bigfive.o \
	enc_utf8.o \
	enc_ascii.o \
	enc_utf16.o \
	enc_ucs4.o

OBJSZ = \
	oz.autojp \
	oz.unictype \
	oz.utf8 \
	oz.encoding \
	oz.iso2022 \
	oz.eightbit \
	oz.shiftjis \
	oz.bigfive \
	oz.enc_utf8 \
	oz.enc_ascii \
	oz.enc_utf16 \
	oz.enc_ucs4

OBJSD = \
	od.autojp \
	od.unictype \
	od.utf8 \
	od.encoding \
	od.iso2022 \
	od.eightbit \
	od.shiftjis \
	od.bigfive \
	od.enc_utf8 \
	od.enc_ascii \
	od.enc_utf16 \
	od.enc_ucs4

HDRS = autojp.h charsets.h encoding.h iso10646.h languages.h unictype.h utf8.h

LIBS = ${TARGET} ${TARGETD} ${TARGETZ} 

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;     ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=0 -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DROM=1 -DDEBUG=0 -ff -zM -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=1         -o $@ $<
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
        ${RM} c.unictype
        ${RM} mkunictype
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.autojp   ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.charsets ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.encoding ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.iso10646 ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.languages ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.unictype ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.utf8     ${UNICODELIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${UNICODELIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETD} ${UNICODELIBDIR}.o.* ${CPFLAGS}
	${CP} ${TARGETZ} ${UNICODELIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)


local_dirs:
	${MKDIR} o
	${MKDIR} oz
	${MKDIR} od

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${UNICODELIBDIR}
        ${MKDIR} ${UNICODELIBDIR}.h
        ${MKDIR} ${UNICODELIBDIR}.o

unictype.c: mkunictype data.UNIDATA2
	mkunictype < data.UNIDATA2 > c.unictype
	echo Made unictype.c

mkunictype: mkunictype.o ${CLIB}
	${LD} -o $@ mkunictype.o ${CLIB}

unictype.o od.unictype oz.unictype: unictype.c

links:
	./mklinks
	echo Made links

#od.unictype: unictype.c
#	${CC} ${CCFLAGS} -DROM=0 -DDEBUG=1         -o $@ unictype.c

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
o.mkunictype:	c.mkunictype
o.mkunictype:	h.unictype
o.mkunictype:	h.iso10646
o.mkunictype:	c.mkunictype
o.mkunictype:	h.unictype
o.mkunictype:	h.iso10646
oz.utf8:	c.utf8
oz.utf8:	h.utf8
oz.utf8:	h.iso10646
oz.encoding:	c.encoding
oz.encoding:	C:h.swis
oz.encoding:	C:h.kernel
oz.encoding:	h.encpriv
oz.encoding:	h.encoding
oz.encoding:	h.iso10646
oz.encoding:	h.enc_ascii
oz.encoding:	h.iso2022
oz.encoding:	h.shiftjis
oz.encoding:	h.bigfive
oz.encoding:	h.eightbit
oz.encoding:	h.enc_utf8
oz.encoding:	h.enc_utf16
oz.encoding:	h.enc_ucs4
oz.encoding:	h.charsets
oz.encoding:	VersionNum
oz.iso2022:	c.iso2022
oz.iso2022:	C:h.swis
oz.iso2022:	C:h.kernel
oz.iso2022:	h.encpriv
oz.iso2022:	h.encoding
oz.iso2022:	h.iso10646
oz.iso2022:	h.iso2022
oz.eightbit:	c.eightbit
oz.eightbit:	h.encpriv
oz.eightbit:	h.encoding
oz.eightbit:	h.iso10646
oz.eightbit:	h.eightbit
oz.shiftjis:	c.shiftjis
oz.shiftjis:	h.encpriv
oz.shiftjis:	h.encoding
oz.shiftjis:	h.iso10646
oz.shiftjis:	h.iso2022
oz.shiftjis:	h.shiftjis
oz.bigfive:	c.bigfive
oz.bigfive:	h.encpriv
oz.bigfive:	h.encoding
oz.bigfive:	h.iso10646
oz.bigfive:	h.iso2022
oz.bigfive:	h.bigfive
oz.enc_utf8:	c.enc_utf8
oz.enc_utf8:	h.encpriv
oz.enc_utf8:	h.encoding
oz.enc_utf8:	h.iso10646
oz.enc_utf8:	h.utf8
oz.enc_utf8:	h.iso10646
oz.enc_utf8:	h.iso2022
oz.enc_utf8:	h.enc_utf8
oz.enc_ascii:	c.enc_ascii
oz.enc_ascii:	h.encpriv
oz.enc_ascii:	h.encoding
oz.enc_ascii:	h.iso10646
oz.enc_ascii:	h.enc_ascii
oz.enc_utf16:	c.enc_utf16
oz.enc_utf16:	h.encpriv
oz.enc_utf16:	h.encoding
oz.enc_utf16:	h.iso10646
oz.enc_utf16:	h.iso2022
oz.enc_utf16:	h.enc_utf16
oz.enc_ucs4:	c.enc_ucs4
oz.enc_ucs4:	h.encpriv
oz.enc_ucs4:	h.encoding
oz.enc_ucs4:	h.iso10646
oz.enc_ucs4:	h.enc_ucs4
