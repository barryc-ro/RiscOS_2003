# Makefile for International Keyboard
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 23-Aug-94  AMcC         Updated to build International Version (from Opal)
# 01-Sep-98  KJB          Changed for new keygen tool
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC	= cc
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend ${THROWBACK} -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit ${KBD} -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = IntKey
SOURCE    = Source.IntKeyMod
LAYOUTS   = s.UK s.Italy s.Spain s.France s.Germany s.Esperanto s.Greece \
            s.Swed-Finl s.Denmark s.Norway s.Iceland s.Turkey s.Russia \
            s.Israel s.LatinAm s.Belgium s.Netherlands s.Japan \
            s.Switzerland s.Wales s.USA s.DvorakUK s.DvorakUSA
TARGET    = rm.IntKey
OBJECT    = o.IntKey

KBD       = -PD "Keyboard SETS \"${KEYBOARD}\""

.SUFFIXES:
.SUFFIXES: .o .s .c .layout

.layout.s:;  keygen -depend !Depend ${THROWBACK} ${EXTRAKEYS} $< $@
.c.o:;	${MKDIR} o ; ${CC} ${CCFLAGS} -o $@ $<

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} s ${WFLAGS}
        ${RM} ${TARGET}
	${RM} keygen
	${RM} keyconvert
        @echo ${COMPONENT}: cleaned

keygen: o.keygen o.throwback o.unicdata
	${MKDIR} s
	${LD} -o keygen o.keygen o.throwback o.unicdata C:o.Stubs

keyconvert: o.keyconvert o.unicdata
	${MKDIR} s
	${LD} -o keyconvert o.keyconvert o.unicdata C:o.Stubs

${LAYOUTS}: keygen

${OBJECT}: ${SOURCE} ${LAYOUTS}
        If "<IKHG$Dir>"="" Then Set IKHG$Dir ^.!IKHG
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ &FFA

# Dynamic dependencies:
o.throwback:	c.throwback
o.throwback:	C:h.swis
o.throwback:	C:h.kernel
o.throwback:	h.throwback
o.unicdata:	c.unicdata
o.unicdata:	C:Unicode.h.iso10646
s.UK: layout.UK
s.UK: inc.Invariant
s.UK: inc.FNLazarus
s.Italy: layout.Italy
s.Italy: inc.Invariant
s.Italy: inc.FNLazarus
s.Spain: layout.Spain
s.Spain: inc.Invariant
s.Spain: inc.FNLazarus
s.France: layout.France
s.France: inc.Invariant
s.France: inc.FNLazarus
s.Germany: layout.Germany
s.Germany: inc.Invariant
s.Germany: inc.FNLazarus
s.Esperanto: layout.Esperanto
s.Esperanto: inc.Invariant
s.Esperanto: inc.FNLazarus
s.Greece: layout.Greece
s.Greece: inc.Invariant
s.Greece: inc.FNLazarus
s.Swed-Finl: layout.Swed-Finl
s.Swed-Finl: inc.Invariant
s.Swed-Finl: inc.FNLazarus
s.Denmark: layout.Denmark
s.Denmark: inc.Invariant
s.Denmark: inc.FNLazarus
s.Norway: layout.Norway
s.Norway: inc.Invariant
s.Norway: inc.FNLazarus
s.Iceland: layout.Iceland
s.Iceland: inc.Invariant
s.Iceland: inc.FNLazarus
s.Turkey: layout.Turkey
s.Turkey: inc.Invariant
s.Turkey: inc.FNLazarus
s.Russia: layout.Russia
s.Russia: inc.Invariant
s.Russia: inc.Invariant
s.Russia: inc.FNLazarus
s.Israel: layout.Israel
s.Israel: inc.Invariant
s.Israel: inc.FNLazarus
s.LatinAm: layout.LatinAm
s.LatinAm: inc.Invariant
s.LatinAm: inc.FNLazarus
s.Belgium: layout.Belgium
s.Belgium: inc.Invariant
s.Belgium: inc.FNLazarus
s.Netherlands: layout.Netherlands
s.Netherlands: inc.Invariant
s.Netherlands: inc.FNLazarus
s.Japan: layout.Japan
s.Japan: inc.Invariant
s.Japan: inc.Invariant
s.Japan: inc.FNLazarus
s.Switzerland: layout.Switzerland
s.Switzerland: inc.Invariant
s.Switzerland: inc.FNLazarus
s.Wales: layout.Wales
s.Wales: inc.Invariant
s.Wales: inc.FNLazarus
s.USA: layout.USA
s.USA: inc.Invariant
s.USA: inc.FNLazarus
s.DvorakUK: layout.DvorakUK
s.DvorakUK: inc.Invariant
s.DvorakUK: inc.FNLazarus
s.DvorakUSA: layout.DvorakUSA
s.DvorakUSA: inc.Invariant
s.DvorakUSA: inc.FNLazarus
o.keygen:	c.keygen
o.keygen:	C:Unicode.h.iso10646
o.keygen:	h.unicdata
o.keygen:	h.structures
o.keygen:	C:Unicode.h.iso10646
o.keygen:	h.throwback
o.IntKey: Source.IntKeyMod
o.IntKey: VersionASM
o.IntKey: Source.IntKeyBody
o.IntKey: Hdr:ListOpts
o.IntKey: Hdr:Macros
o.IntKey: Hdr:System
o.IntKey: Hdr:SWIs
o.IntKey: Hdr:CPU.Generic26
o.IntKey: Hdr:IO.GenericIO
o.IntKey: Hdr:RISCOS
o.IntKey: Hdr:ModHand
o.IntKey: Hdr:Services
o.IntKey: Hdr:Proc
o.IntKey: Source.Accents
o.IntKey: Source.KeyStruct
o.IntKey: s.UK
o.IntKey: s.Italy
o.IntKey: s.Spain
o.IntKey: s.France
o.IntKey: s.Germany
o.IntKey: s.Esperanto
o.IntKey: s.Greece
o.IntKey: s.Swed-Finl
o.IntKey: s.Denmark
o.IntKey: s.Norway
o.IntKey: s.Iceland
o.IntKey: s.Turkey
o.IntKey: s.Russia
o.IntKey: s.Israel
o.IntKey: s.LatinAm
o.IntKey: s.Belgium
o.IntKey: s.Netherlands
o.IntKey: s.Japan
o.IntKey: s.Switzerland
o.IntKey: s.Wales
o.IntKey: s.USA
o.IntKey: s.DvorakUK
o.IntKey: s.DvorakUSA
