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

CCFLAGS = -c -depend !Depend -fah -Ws ${INCLUDES} ${DFLAGS}
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
	johab.o \
	bigfive.o \
	enc_utf8.o \
	enc_ascii.o \
	enc_utf16.o \
	enc_ucs4.o \
	acorn.o \
	combine.o \
	riscos.o

OBJSZ = \
	oz.autojp \
	oz.unictype \
	oz.utf8 \
	oz.encoding \
	oz.iso2022 \
	oz.eightbit \
	oz.shiftjis \
	oz.johab \
	oz.bigfive \
	oz.enc_utf8 \
	oz.enc_ascii \
	oz.enc_utf16 \
	oz.enc_ucs4 \
	oz.acorn \
	oz.combine \
	oz.riscos


OBJSD = \
	od.autojp \
	od.unictype \
	od.utf8 \
	od.encoding \
	od.iso2022 \
	od.eightbit \
	od.shiftjis \
	od.johab \
	od.bigfive \
	od.enc_utf8 \
	od.enc_ascii \
	od.enc_utf16 \
	od.enc_ucs4 \
	od.acorn \
	od.combine \
	od.riscos

HDRS = autojp.h charsets.h encoding.h iso10646.h languages.h unictype.h utf8.h combine.h

LIBS = ${TARGET} ${TARGETD} ${TARGETZ} 

#
# Rule patterns
#
.SUFFIXES: .o .oz .od .c .h .s
.c.o:;     ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=0 -ff     -o $@ $<
.c.oz:;    ${CC} ${CCFLAGS} -DROM=1 -DDEBUG=0 -ff -zM -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -DROM=0 -DDEBUG=1         -o $@ $<
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
        ${WIPE} od ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} ${COMPONENTZ}
        ${RM} ${COMPONENTD}
        ${RM} c.unictype
        ${RM} mkunictype
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.autojp   ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.charsets ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.combine  ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.encoding ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.iso10646 ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.languages ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.unictype ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.utf8     ${UNICODELIBDIR}.h.* ${CPFLAGS}
	${CP} h.utf16    ${UNICODELIBDIR}.h.* ${CPFLAGS}
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

links:
	./mklinks
	echo Made links

#
# Final link
#
${TARGET}: ${OBJS} unictype.c local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} unictype.c local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${TARGETD}: ${OBJSD} unictype.c local_dirs
	${LB} ${LBFLAGS} -o.autojp:	c.autojp
o.autojp:	h.charsets
o.autojp:	h.autojp
o.autojp:	c.autojp
o.autojp:	h.charsets
o.autojp:	h.autojp
o.unictype:	c.unictype
o.unictype:	h.unictype
o.unictype:	h.iso10646
o.utf8:	c.utf8
o.utf8:	h.utf8
o.utf8:	h.iso10646
o.encoding:	c.encoding
o.encoding:	C:h.swis
o.encoding:	C:h.kernel
o.encoding:	h.encpriv
o.encoding:	h.encoding
o.encoding:	h.iso10646
o.encoding:	h.enc_ascii
o.encoding:	h.iso2022
o.encoding:	h.shiftjis
o.encoding:	h.johab
o.encoding:	h.bigfive
o.encoding:	h.eightbit
o.encoding:	h.enc_utf8
o.encoding:	h.enc_utf16
o.encoding:	h.enc_ucs4
o.encoding:	h.acorn
o.encoding:	h.charsets
o.encoding:	h.languages
o.encoding:	VersionNum
o.iso2022:	c.iso2022
o.iso2022:	C:h.swis
o.iso2022:	C:h.kernel
o.iso2022:	h.encpriv
o.iso2022:	h.encoding
o.iso2022:	h.iso10646
o.iso2022:	h.iso2022
o.eightbit:	c.eightbit
o.eightbit:	h.encpriv
o.eightbit:	h.encoding
o.eightbit:	h.iso10646
o.eightbit:	h.eightbit
o.shiftjis:	c.shiftjis
o.shiftjis:	h.encpriv
o.shiftjis:	h.encoding
o.shiftjis:	h.iso10646
o.shiftjis:	h.iso2022
o.shiftjis:	h.shiftjis
o.johab:	c.johab
o.johab:	h.encpriv
o.johab:	h.encoding
o.johab:	h.iso10646
o.johab:	h.iso2022
o.johab:	h.johab
o.bigfive:	c.bigfive
o.bigfive:	h.encpriv
o.bigfive:	h.encoding
o.bigfive:	h.iso10646
o.bigfive:	h.iso2022
o.bigfive:	h.bigfive
o.enc_utf8:	c.enc_utf8
o.enc_utf8:	h.encpriv
o.enc_utf8:	h.encoding
o.enc_utf8:	h.iso10646
o.enc_utf8:	h.utf8
o.enc_utf8:	h.iso10646
o.enc_utf8:	h.iso2022
o.enc_utf8:	h.enc_utf8
o.enc_ascii:	c.enc_ascii
o.enc_ascii:	h.encpriv
o.enc_ascii:	h.encoding
o.enc_ascii:	h.iso10646
o.enc_ascii:	h.enc_ascii
o.enc_utf16:	c.enc_utf16
o.enc_utf16:	h.encpriv
o.enc_utf16:	h.encoding
o.enc_utf16:	h.iso10646
o.enc_utf16:	h.iso2022
o.enc_utf16:	h.enc_utf16
o.enc_ucs4:	c.enc_ucs4
o.enc_ucs4:	h.encpriv
o.enc_ucs4:	h.encoding
o.enc_ucs4:	h.iso10646
o.enc_ucs4:	h.enc_ucs4
o.acorn:	c.acorn
o.acorn:	h.encpriv
o.acorn:	h.encoding
o.acorn:	h.iso10646
o.acorn:	h.eightbit
o.acorn:	h.acorn
o.combine:	c.combine
o.combine:	h.combine
o.combine:	h.iso10646
o.riscos:	c.riscos
o.riscos:	C:h.kernel
o.riscos:	C:h.swis
o.riscos:	h.encpriv
o.riscos:	h.encoding
o.riscos:	h.iso10646
od.autojp:	c.autojp
od.autojp:	h.charsets
od.autojp:	h.autojp
od.unictype:	c.unictype
od.unictype:	h.unictype
od.unictype:	h.iso10646
od.utf8:	c.utf8
od.utf8:	h.utf8
od.utf8:	h.iso10646
od.encoding:	c.encoding
od.encoding:	C:h.swis
od.encoding:	C:h.kernel
od.encoding:	h.encpriv
od.encoding:	h.encoding
od.encoding:	h.iso10646
od.encoding:	h.enc_ascii
od.encoding:	h.iso2022
od.encoding:	h.shiftjis
od.encoding:	h.johab
od.encoding:	h.bigfive
od.encoding:	h.eightbit
od.encoding:	h.enc_utf8
od.encoding:	h.enc_utf16
od.encoding:	h.enc_ucs4
od.encoding:	h.acorn
od.encoding:	h.charsets
od.encoding:	h.languages
od.encoding:	VersionNum
od.iso2022:	c.iso2022
od.iso2022:	C:h.swis
od.iso2022:	C:h.kernel
od.iso2022:	h.encpriv
od.iso2022:	h.encoding
od.iso2022:	h.iso10646
od.iso2022:	h.iso2022
od.eightbit:	c.eightbit
od.eightbit:	h.encpriv
od.eightbit:	h.encoding
od.eightbit:	h.iso10646
od.eightbit:	h.eightbit
od.shiftjis:	c.shiftjis
od.shiftjis:	h.encpriv
od.shiftjis:	h.encoding
od.shiftjis:	h.iso10646
od.shiftjis:	h.iso2022
od.shiftjis:	h.shiftjis
od.johab:	c.johab
od.johab:	h.encpriv
od.johab:	h.encoding
od.johab:	h.iso10646
od.johab:	h.iso2022
od.johab:	h.johab
od.bigfive:	c.bigfive
od.bigfive:	h.encpriv
od.bigfive:	h.encoding
od.bigfive:	h.iso10646
od.bigfive:	h.iso2022
od.bigfive:	h.bigfive
od.enc_utf8:	c.enc_utf8
od.enc_utf8:	h.encpriv
od.enc_utf8:	h.encoding
od.enc_utf8:	h.iso10646
od.enc_utf8:	h.utf8
od.enc_utf8:	h.iso10646
od.enc_utf8:	h.iso2022
od.enc_utf8:	h.enc_utf8
od.enc_ascii:	c.enc_ascii
od.enc_ascii:	h.encpriv
od.enc_ascii:	h.encoding
od.enc_ascii:	h.iso10646
od.enc_ascii:	h.enc_ascii
od.enc_utf16:	c.enc_utf16
od.enc_utf16:	h.encpriv
od.enc_utf16:	h.encoding
od.enc_utf16:	h.iso10646
od.enc_utf16:	h.iso2022
od.enc_utf16:	h.enc_utf16
od.enc_ucs4:	c.enc_ucs4
od.enc_ucs4:	h.encpriv
od.enc_ucs4:	h.encoding
od.enc_ucs4:	h.iso10646
od.enc_ucs4:	h.enc_ucs4
od.acorn:	c.acorn
od.acorn:	h.encpriv
od.acorn:	h.encoding
od.acorn:	h.iso10646
od.acorn:	h.eightbit
od.acorn:	h.acorn
od.combine:	c.combine
od.combine:	h.combine
od.combine:	h.iso10646
od.riscos:	c.riscos
od.riscos:	C:h.kernel
od.riscos:	C:h.swis
od.riscos:	h.encpriv
od.riscos:	h.encoding
od.riscos:	h.iso10646
oz.autojp:	c.autojp
oz.autojp:	h.charsets
oz.autojp:	h.autojp
oz.autojp:	c.autojp
oz.autojp:	h.charsets
oz.autojp:	h.autojp
oz.unictype:	c.unictype
oz.unictype:	h.unictype
oz.unictype:	h.iso10646
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
oz.encoding:	h.johab
oz.encoding:	h.bigfive
oz.encoding:	h.eightbit
oz.encoding:	h.enc_utf8
oz.encoding:	h.enc_utf16
oz.encoding:	h.enc_ucs4
oz.encoding:	h.acorn
oz.encoding:	h.charsets
oz.encoding:	h.languages
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
oz.johab:	c.johab
oz.johab:	h.encpriv
oz.johab:	h.encoding
oz.johab:	h.iso10646
oz.johab:	h.iso2022
oz.johab:	h.johab
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
oz.acorn:	c.acorn
oz.acorn:	h.encpriv
oz.acorn:	h.encoding
oz.acorn:	h.iso10646
oz.acorn:	h.eightbit
oz.acorn:	h.acorn
oz.combine:	c.combine
oz.combine:	h.combine
oz.combine:	h.iso10646
oz.riscos:	c.riscos
oz.riscos:	C:h.kernel
oz.riscos:	C:h.swis
oz.riscos:	h.encpriv
oz.riscos:	h.encoding
oz.riscos:	h.iso10646
o.mkunictype:	c.mkunictype
o.mkunictype:	h.unictype
o.mkunictype:	h.iso10646
o.mkunictype:	c.mkunictype
o.mkunictype:	h.unictype
o.mkunictype:	h.iso10646
