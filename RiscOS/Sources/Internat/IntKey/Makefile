# Makefile for International Keyboard
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 23-Aug-94  AMcC         Updated to build International Version (from Opal)
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD      = link
RM      = remove
WIPE    = wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit ${KBD} -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = IntKey
SOURCE    = Source.IntKeyMod
TARGET    = rm.IntKey
OBJECT    = o.IntKey

KBD       = -PD "Keyboard SETS \"${KEYBOARD}\""

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
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${OBJECT}: ${SOURCE}
        If "<IKHG$Dir>"="" Then Set IKHG$Dir ^.!IKHG
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ &FFA

# Dynamic dependencies:
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
o.IntKey: !IKHG.Source.Chars.Latin1
o.IntKey: Source.KeyStruct
o.IntKey: !IKHG.Source.UK
o.IntKey: !IKHG.Source.Italy
o.IntKey: !IKHG.Source.Spain
o.IntKey: !IKHG.Source.France
o.IntKey: !IKHG.Source.Germany
o.IntKey: !IKHG.Source.Greece
o.IntKey: <IKHG$Dir>.Source.Chars.Greek
o.IntKey: !IKHG.Source.Sweden
o.IntKey: !IKHG.Source.Denmark
o.IntKey: !IKHG.Source.Norway
o.IntKey: !IKHG.Source.Canada
o.IntKey: !IKHG.Source.Turkey
o.IntKey: <IKHG$Dir>.Source.Chars.Latin3
o.IntKey: !IKHG.Source.Russia
o.IntKey: <IKHG$Dir>.Source.Chars.Cyrillic
o.IntKey: !IKHG.Source.Russia2
o.IntKey: <IKHG$Dir>.Source.Chars.Cyrillic2
o.IntKey: !IKHG.Source.Israel
o.IntKey: <IKHG$Dir>.Source.Chars.Hebrew
o.IntKey: !IKHG.Source.LatinAm
o.IntKey: !IKHG.Source.Belgium
o.IntKey: !IKHG.Source.Netherlands
o.IntKey: !IKHG.Source.Switzerland
o.IntKey: !IKHG.Source.USA
o.IntKey: !IKHG.Source.DvorakUK
o.IntKey: !IKHG.Source.DvorakUSA
o.IntKey: !IKHG.Source.DvorakLH
o.IntKey: !IKHG.Source.DvorakRH
