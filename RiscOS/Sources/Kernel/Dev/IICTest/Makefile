# Makefile for IICTest
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
COMPONENT = IICTest
SOURCE    = s.IICTest
TARGET    = rm.IICTest
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

resources:
#       ${MKDIR} ${RESDIR}.${COMPONENT}
#       ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
#       @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.IICTest: Hdr:ListOpts
rm.IICTest: Hdr:Macros
rm.IICTest: Hdr:System
rm.IICTest: Hdr:SWIs
rm.IICTest: Hdr:CPU.Generic26
rm.IICTest: Hdr:IO.GenericIO
rm.IICTest: Hdr:RISCOS
rm.IICTest: Hdr:Machine.<Machine>
rm.IICTest: Hdr:ImageSize.<ImageSize>
rm.IICTest: Hdr:CPU.ARM600
rm.IICTest: Hdr:IO.IOMDL
rm.IICTest: Hdr:IO.IOMD
rm.IICTest: Hdr:MEMM.ARM600
rm.IICTest: Hdr:VIDC.VIDC20
rm.IICTest: Hdr:ModHand
rm.IICTest: Hdr:Proc
rm.IICTest: Hdr:CMOS
rm.IICTest: ^.^.hdr.PublicWS
rm.IICTest: ^.^.hdr.KernelWS
rm.IICTest: ^.^.s.PMF.i2cutils
