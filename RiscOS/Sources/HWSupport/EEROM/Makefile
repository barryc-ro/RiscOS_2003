# Makefile for EEROM
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
COMPONENT = EEROM
SOURCE    = s.EEROM
TARGET    = rm.EEROM
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

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#       ${MKDIR} ${RESDIR}.${COMPONENT}
#       ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#       @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.EEROM: Hdr:ListOpts
rm.EEROM: Hdr:Macros
rm.EEROM: Hdr:System
rm.EEROM: Hdr:SWIs
rm.EEROM: Hdr:CPU.Generic26
rm.EEROM: Hdr:IO.GenericIO
rm.EEROM: Hdr:RISCOS
rm.EEROM: Hdr:Machine.<Machine>
rm.EEROM: Hdr:ImageSize.<ImageSize>
rm.EEROM: Hdr:CPU.ARM600
rm.EEROM: Hdr:IO.IOMDL
rm.EEROM: Hdr:IO.IOMD
rm.EEROM: Hdr:MEMM.ARM600
rm.EEROM: Hdr:VIDC.VIDC20
rm.EEROM: Hdr:ModHand
rm.EEROM: Hdr:Proc
rm.EEROM: Hdr:CMOS
rm.EEROM: Hdr:PublicWS
rm.EEROM: s.i2cutils
