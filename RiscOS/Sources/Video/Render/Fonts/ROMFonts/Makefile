# Makefile for ROMFonts
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
RM      = remove
LD      = link
WIPE    = -wipe
MKDIR   = cdir
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -PD "DoJapanese SETL ${JAPANESE}" -o $@
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
SOURCE    = s.ROMFonts
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
	${MKDIR} <Resource$Dir>.Fonts
        @Set Alias$MFD ${MKDIR} <Resource$Dir>.Fonts.%0
	@Set Alias$CFF ${CP} Fonts.%0 <Resource$Dir>.Fonts.%0 ${CPFLAGS}
	Obey Config.${SYSTEM}
        @echo ${COMPONENT}: resource files copied

o.ROMFonts: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

rm.ROMFonts: o.ROMFonts
        ${LD} ${LDFLAGS} o.ROMFonts
        SetType ${TARGET} &FFA

o.MaruGothic:
	ResGen Data o.MaruGothic -via via.MaruGothic
rm.MaruGothic: o.MaruGothic
	${LD} ${LDFLAGS} o.MaruGothic

# Dynamic dependencies:
