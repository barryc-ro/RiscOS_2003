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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

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

resources:
        ${MKDIR} ${RESAPP}
        ${MKDIR} ${RESDIR}
        ${CP} Resources.Encodings            ${RESDIR}.Encodings  ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Boot      ${RESAPP}.!Boot      ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Run       ${RESAPP}.!Run       ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Help      ${RESAPP}.!Help      ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites   ${RESAPP}.!Sprites   ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites22 ${RESAPP}.!Sprites22 ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites23 ${RESAPP}.!Sprites23 ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

install: 
        ${MKDIR} ${INSTDIR}
        ${CP} Resources.Encodings            ${INSTDIR}.Encodings  ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} Resources.${LOCALE}.!Sprites23 ${INSTDIR}.!Sprites23 ${CPFLAGS}

module:
	modgen UnicodeRes UnicodeRes UnicodeRes 0.03 -via viafile
	echo built softload UnicodeRes 

# Dynamic dependencies:
