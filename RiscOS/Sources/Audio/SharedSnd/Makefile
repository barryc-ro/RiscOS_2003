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
SL        = ${SOURCE}.o
SSLIB     = ${SL}.Drivers ${SL}.Handler ${SL}.Linear ${SL}.Memory ${SL}.Module \
            ${SL}.SampleRate ${SL}.SWIS ${SL}.Vars ${SL}.Volume

.o.c:
	 set tokenisefile $*
	 utils.detoken1; BASIC

.c.o:
	 set tokenisefile $*
	 utils.token1; BASIC

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
        ${WIPE} o.* ~cv
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: o.SharedSoun makebasic
        Set MakeSSnd$Dir @
        Run o.SharedSoun
        ${WIPE} o.* ~cv

import: Drivers.c Handler.c Linear.c Memory.c Module.c SampleRate.c SWIS.c Vars.c Volume.c SharedSoun.c
	echo made text files

makebasic: Drivers.o Handler.o Linear.o Memory.o Module.o SampleRate.o SWIS.o Vars.o Volume.o SharedSoun.o
	echo made basic files

# Dynamic dependencies:
