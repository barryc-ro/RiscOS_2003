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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit ${KBD} -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = IntKey
SOURCE    = Source.IntKeyMod
LAYOUTS   = s.UK s.Italy s.Spain s.France s.Germany s.Greece \
            s.Swed-Finl s.Denmark s.Norway s.Turkey s.Russia \
            s.Israel s.LatinAm s.Belgium s.Netherlands s.Japan \
            s.Switzerland s.Wales s.USA s.DvorakUK s.DvorakUSA
TARGET    = rm.IntKey
OBJECT    = o.IntKey

KBD       = -PD "Keyboard SETS \"${KEYBOARD}\""

.SUFFIXES:
.SUFFIXES: .o .s .c .layout

.layout.s:;  keygen ${THROWBACK} $< $@
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
o.keygen:	c.keygen
o.keygen:	C:Unicode.h.iso10646
o.keygen:	h.unicdata
o.keygen:	h.structures
o.keygen:	C:Unicode.h.iso10646
o.keygen:	h.throwback
o.keyconvert:	c.keyconvert
o.keyconvert:	C:h.swis
o.keyconvert:	C:h.kernel
o.keyconvert:	C:Unicode.h.iso10646
o.keyconvert:	h.unicdata
o.IntKey: Source.IntKeyMod
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
o.IntKey: s.Greece
o.IntKey: s.Swed-Finl
o.IntKey: s.Denmark
o.IntKey: s.Norway
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
