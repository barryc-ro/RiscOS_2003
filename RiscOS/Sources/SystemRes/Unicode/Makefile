# Makefile for Unicode Resources
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Nov-97  SJM          Created.
# 28-Nov-97  ADH          Added !Sprites, !Sprites22, !Sprites23
# 19-Nov-99  SNB          Updated to allow slimmed down STB !Unicode to be built.
#                         Passing OPTIONS=STB builds a cut down structure.

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CHMOD   = attr ${CHMODFLAGS}
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v
CHMODFLAGS = +ow +or +wr -ww

#
# Program specific options:
#
COMPONENT = Unicode
APP       = !${COMPONENT}
TARGET    =

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.Apps.${APP}
RESDIR = <resource$dir>.Resources.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned

resources: resources-${SUBSET}
        @echo ${COMPONENT}: resource files copied

resources-Japan: resources-
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*CNS* ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*KS*  ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*GB*  ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.BigFive              ${WFLAGS}

resources-NoCJK: resources-
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.* ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.BigFive          ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.Microsoft.CP932  ${WFLAGS}

resources-:
        ${MKDIR} ${RESAPP}
        ${MKDIR} ${RESDIR}
        ${CP} Resources.Encodings ${RESDIR}.Encodings  ${CPFLAGS}
        ${CP} LocalRes:!Boot      ${RESAPP}.!Boot      ${CPFLAGS}
        ${CP} LocalRes:!Run       ${RESAPP}.!Run       ${CPFLAGS}
        ${CP} LocalRes:!Help      ${RESAPP}.!Help      ${CPFLAGS}
        ${CP} LocalRes:!Sprites   ${RESAPP}.!Sprites   ${CPFLAGS}
        ${CP} LocalRes:!Sprites22 ${RESAPP}.!Sprites22 ${CPFLAGS}
        ${CP} LocalRes:!Sprites23 ${RESAPP}.!Sprites23 ${CPFLAGS}

install_STB:
        ${CP} LocalUserIFRes:!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} LocalUserIFRes:!Run       ${INSTDIR}.!Run       ${CPFLAGS}

install_:
        ${CP} LocalRes:!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} LocalRes:!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} LocalRes:!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} LocalRes:!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} LocalRes:!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} LocalRes:!Sprites23 ${INSTDIR}.!Sprites23 ${CPFLAGS}
        
install_dirs:
        ${MKDIR} ${INSTDIR}

install: install_${OPTIONS} install_dirs
        ${CP} Resources.Encodings ${INSTDIR}.Encodings  ${CPFLAGS}
        ${CHMOD} ${INSTDIR}
	echo ${COMPONENT}: Installed.

module:
	modgen UnicodeRes UnicodeRes UnicodeRes 0.18 -via viafile
	echo built softload UnicodeRes 

BBETYPE = unicode
bbe-unicode: bbe-unicode-${SUBSET}
        @| Need to cope with Japanese exports correctly

bbe-unicode-: bbe-generic
        BBE_Export_Dir Resources.Encodings
        @|

bbe-unicode-Japan: bbe-generic
        BBE_Export_Dir Resources.Encodings
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*CNS* ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*KS*  ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.ISO2022.G94x94.*GB*  ${WFLAGS}
        ${WIPE} ${RESDIR}.Encodings.BigFive              ${WFLAGS}
  
# Dynamic dependencies:
