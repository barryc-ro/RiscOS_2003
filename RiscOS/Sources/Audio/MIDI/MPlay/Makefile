# Makefile for MPlay
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
WIPE    = wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = MPlay
TARGET    = rm.${COMPONENT}
OBJS      = TempObj.DIn TempObj.DPro TempObj.Out TempObj.Sched

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
        ${WIPE} TempObj.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${OBJS} vars
        Run b.MPlayCode

vars:
        Set MPlayBuild$Dir @

TempObj.Din: b.DInCode
        Run b.DInCode

TempObj.DPro: b.Process
        Run b.Process

TempObj.Out: b.OutputCode
        Run b.OutputCode

TempObj.Sched: b.Schedule
        Run b.Schedule

# Dynamic dependencies:
