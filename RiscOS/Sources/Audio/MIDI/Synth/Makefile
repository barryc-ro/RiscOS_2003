# Makefile for Synth
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
COMPONENT  = Synth
COMPONENT2 = ESPDrvrsAS
TARGET1    = Modules.${COMPONENT}
TARGET2    = Source.${COMPONENT2}
S          = Source
SL         = ${S}.SynthLib
O          = ${S}.TempObj
OD         = ${O}.Drivers
SOURCES    = ${S}.BankCode ${S}.CommonVars ${S}.DriverJoin ${S}.MakeCommon \
             ${S}.MakeDriver ${S}.MakeEnv ${S}.MakeFill ${S}.MakePan \
             ${S}.MakeVolume ${S}.MMan ${S}.SynthMain ${S}.!Run
SYNTHLIB   = ${SL}.Bank ${SL}.Channel ${SL}.Driver ${SL}.Fill ${SL}.Init \
             ${SL}.Install ${SL}.Instrument ${SL}.Memory ${SL}.MIDI \
             ${SL}.Module ${SL}.PolyRate ${SL}.SWIs ${SL}.Tone ${SL}.Vars

#
# Generic rules:
#
rom: ${TARGET1}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET1}
        ${CP} ${TARGET1} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${CP} ${TARGET2} ${INSTDIR}.${COMPONENT2} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} ${O} ${WFLAGS}
        ${RM} ${TARGET1}
        ${RM} ${TARGET2}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET1}: ${SOURCES} ${SYNTHLIB} 
        ${MKDIR} ${O}
        ${MKDIR} ${OD}
        Run ${S}.!Run

# Dynamic dependencies:
