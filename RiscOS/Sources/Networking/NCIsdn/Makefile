# Makefile for NCEthernet
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = NCIsdn
VERSION   = 0.28
SOURCE    = Contents
TARGET    = rm.NCIsdn
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}


#
# Export Paths for Messages module
#
RESDIR  = <resource$dir>.Resources2.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
MODGEN  = modgen
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} clean
        ${MODGEN} $@ ${COMPONENT} ${COMPONENT} ${VERSION} -via ${SOURCE}

${EXP_HDR}.${COMPONENT}:

# Dynamic dependencies:
