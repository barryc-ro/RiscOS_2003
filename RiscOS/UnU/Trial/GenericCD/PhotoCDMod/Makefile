# Makefile for PhotoCD
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28aug95    diz    	  Created
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

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
COMPONENT = PhotoCD
SOURCE    = s.PhotoCD
TARGET    = rm.PhotoCD
EXPORTS   = ${EXP_HDR}.${COMPONENT}

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

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.PhotoCD: h.RegNames
rm.PhotoCD: h.SWInames
rm.PhotoCD: h.Macros
rm.PhotoCD: s.pcdformats
rm.PhotoCD: s.constants
rm.PhotoCD: s.switches
rm.PhotoCD: s.workspace
rm.PhotoCD: s.pcdswis
rm.PhotoCD: s.getblock
rm.PhotoCD: s.gbubuf
rm.PhotoCD: s.huff
