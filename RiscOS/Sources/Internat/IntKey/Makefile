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
