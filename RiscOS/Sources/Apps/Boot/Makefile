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
COMPONENT = Boot
APP       = !Boot
SOURCE    =
TARGET    =

#
# Export Paths for Messages module
#
RESAPP = <resource$dir>.${APP}

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
	IfThere Resources.${UserIF}.${LOCALE}.${APP} Then \
        ${CP}   Resources.${UserIF}.${LOCALE}.${APP} ${RESAPP} ${CPFLAGS} Else \
        ${CP}   Resources.${UserIF}.UK.${APP}        ${RESAPP} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

# Dynamic dependencies:
