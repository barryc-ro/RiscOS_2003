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
TARGET    = Modules.${COMPONENT}
SOURCE    = Source
O         = ${SOURCE}.TempObj
OBJS      = ${O}.DIn ${O}.DPro ${O}.Out ${O}.Sched

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
        ${WIPE} ${O}.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}.MPlayCode ${OBJS} vars
        Run ${SOURCE}.MPlayCode

vars:
        Set MPlayBuild$Dir ${SOURCE}

${O}.Din: ${SOURCE}.DInCode
        Run ${SOURCE}.DInCode

${O}.DPro: ${SOURCE}.Process
        Run ${SOURCE}.Process

${O}.Out: ${SOURCE}.OutputCode
        Run ${SOURCE}.OutputCode

${O}.Sched: ${SOURCE}.Schedule
        Run ${SOURCE}.Schedule

# Dynamic dependencies:
