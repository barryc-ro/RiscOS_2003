# Makefile for ModeFiles
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Feb-96  JRH          Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module ${THROWBACK} -To ${TARGET} -From
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TDIR      = rm.${MACHINE}
TARGET    = ${TDIR}.MF${TYPE}
RDIR      = ${RESDIR}.Monitors
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
        ${WIPE} ${TDIR} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} LocalRes:${MACHINE} ${RDIR} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TDIR}.MFPAL: ${SOURCE} s.PAL
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.PAL

${TDIR}.MFNTSC: ${SOURCE} s.NTSC
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.NTSC

${TDIR}.MF: ${SOURCE} s.ModeFiles
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.ModeFiles

# Dynamic dependencies:
