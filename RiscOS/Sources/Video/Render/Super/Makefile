# Makefile for SuperSample
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
COMPONENT = Super
SOURCE    = s.Super
TARGET    = rm.Super
EXPORTS   = ${EXP_HDR}.Super

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
        ${RM} s.Matrix1
        ${RM} s.Matrix2
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} s.Matrix2 s.Matrix1
        ${AS} ${ASFLAGS} ${SOURCE}

s.Matrix1: GenMatrix1
        GenMatrix1

s.Matrix2: GenMatrix2
        GenMatrix2

${EXP_HDR}.Super: hdr.Super
        ${CP} hdr.Super $@ ${CPFLAGS}

# Dynamic dependencies:
