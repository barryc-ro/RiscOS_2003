# Makefile for Portable (Morris based)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 22-Sep-94  RCM          Created.
# 06-Jan-95  AMcC         Added resources rule
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
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Portable
SOURCE    = s.GetAll
TARGET    = rm.${COMPONENT}
TARGETSA  = rm.${COMPONENT}SA
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}

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

resources:
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        ${CP} ${LDIR}.LCD       ${RESDIR}.LCD       ${CPFLAGS}
        ${CP} ${LDIR}.SelectLCD ${RESDIR}.SelectLCD ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

#
# Makes a stand-alone version with messages bound in
#
standalone: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

${TARGETSA}: s.StandAlone ${LDIR}.Messages
        ${AS} ${ASFLAGS} s.StandAlone

# Dynamic dependencies:
