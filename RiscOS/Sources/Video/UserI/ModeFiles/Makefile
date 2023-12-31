# Makefile for ModeFiles
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Feb-96  JRH          Created.
# 15-Jun-00  BJGA         Changed to use objasm.
#

#
# Paths
#
EXP_HDR = <export$dir>2

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -ABSolute -NOWarn -depend !Depend ${THROWBACK} ${OPTS} -o ${OBJECT}
LDFLAGS = -bin -o ${TARGET}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = ModeFiles
SOURCE    = s.ModeFiles
TDIR      = rm.${MACHINE}
ODIR      = o.${MACHINE}
TARGET    = ${TDIR}.MF${TYPE}
OBJECT    = ${ODIR}.MF${TYPE}
RDIR      = ${RESDIR}.Monitors
OPTS      = -PreDefine "SA_flag SETS \"${STANDALONE}\"" -PreDefine "Machine SETS \"${MACHINE}\""
EXPORTS   = ${EXP_HDR}.ModeFiles
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

install: install_${STANDALONE}

install_TRUE: ${TARGET}
        ${MKDIR} ${INSTDIR}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ram module installed

install_:
        ${MKDIR} ${INSTDIR}
        ${CP} LocalRes:<UserIF> ${INSTDIR} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${WIPE} ${TDIR} ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RDIR}
        ${CP} LocalRes:${MACHINE} ${RDIR} ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

BBETYPE = modefiles
bbe-modefiles: bbe-generic-resources-get-alias
        BBE_Export_Dir Resources.${LOCALE}.${MACHINE}

${TDIR}.MF ${TDIR}.MFPAL: ${SOURCE} s.PAL
        ${MKDIR} ${ODIR}
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.PAL
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${TDIR}.MFNTSC: ${SOURCE} s.NTSC
        ${MKDIR} ${ODIR}
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.NTSC
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${TDIR}.MFSlave: ${SOURCE} s.Slave
        ${MKDIR} ${ODIR}
        ${MKDIR} ${TDIR}
        ${AS} ${ASFLAGS} s.Slave
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ Module
        Access $@ rw/r

${EXP_HDR}.ModeFiles: hdr.ModeFiles
        ${CP} hdr.ModeFiles $@ ${CPFLAGS}
        
# Dynamic dependencies:
