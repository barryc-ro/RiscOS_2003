# Makefile for Funai2UI
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 11-Sep-98  SBF          Created from FunaiUI Makefile
#

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
COMPONENT = Funai2UI
APP       = !Boot
SOURCE    =
TARGET    =

#
# Export Paths for Messages module
#
RESAPP = <Resource$Dir>.${APP}
RESRES = <Resource$Dir>.Resources
#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: ROM module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: ROM module installed

clean:
        @echo ${COMPONENT}: cleaned

resources:
        ${CP} Resources.<UserIf>.${LOCALE}.${APP} ${RESAPP} ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.NVRAM.Tags ${RESRES}.NVRAM.Tags ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.Wimp.Sprites ${RESRES}.Wimp.Sprites ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied
        
# Dynamic dependencies:
