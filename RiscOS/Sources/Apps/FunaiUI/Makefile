# Makefile for Boot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Jun-96  SMC          Created.
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
COMPONENT = BocaNC2UI
APP       = !Boot
SOURCE    =
TARGET    =

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.${APP}
RESRES = <Resource$Dir>.Resources
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
        ${CP} Resources.<UserIf>.${LOCALE}.${APP}   ${RESAPP}   ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.NCFresco ${RESRES}.NCFresco ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.NVRAM.Tags ${RESRES}.NVRAM.Tags ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.ScrSaver ${RESRES}.ScrSaver ${CPFLAGS}
        ${CP} Resources.<UserIf>.${LOCALE}.Wimp.Sprites ${RESRES}.Wimp.Sprites ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied
        
# Dynamic dependencies:
