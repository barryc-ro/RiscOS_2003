# Makefile for SharedSnd
#

#
# Paths
#
EXP_HDR = <export$dir>
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
COMPONENT = SSound
TARGET    = Modules.${COMPONENT}
SOURCE    = Source
SL        = ${SOURCE}.SSLib
SSLIB     = ${SL}.Drivers ${SL}.Handler ${SL}.Linear ${SL}.Memory ${SL}.Module \
            ${SL}.SampleRate ${SL}.SWIS ${SL}.Vars ${SL}.Volume

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
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}.SharedSoun ${SSLIB} vars
        Run ${SOURCE}.SharedSoun

vars:
        Set MakeSSnd$Dir ${SOURCE}

# Dynamic dependencies:
