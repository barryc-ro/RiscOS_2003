# Makefile for Unicode Resources
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Nov-97  SJM          Created.
# 28-Nov-97  ADH          Added !Sprites, !Sprites22, !Sprites23

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

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

install: 
        ${MKDIR} ${INSTDIR}
        ${CP} Resources.Encodings ${INSTDIR}.Encodings  ${CPFLAGS}
        ${CP} LocalRes:!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} LocalRes:!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} LocalRes:!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} LocalRes:!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} LocalRes:!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} LocalRes:!Sprites23 ${INSTDIR}.!Sprites23 ${CPFLAGS}

module:
	modgen UnicodeRes UnicodeRes UnicodeRes 0.13 -via viafile
	echo built softload UnicodeRes 

# Dynamic dependencies:
