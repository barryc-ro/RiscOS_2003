# Makefile for OSVersion
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-May-96  SMC          Created.
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
COMPONENT = OSVersion
SOURCE    = s.ModHead
TARGET    = rm.OSVersion
EXPORTS   =

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
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.OSVersion: Hdr:ListOpts
rm.OSVersion: Hdr:Macros
rm.OSVersion: Hdr:System
rm.OSVersion: Hdr:SWIs
rm.OSVersion: Hdr:CPU.Generic26
rm.OSVersion: Hdr:IO.GenericIO
rm.OSVersion: Hdr:RISCOS
rm.OSVersion: Hdr:ModHand
rm.OSVersion: Hdr:Machine.<Machine>
rm.OSVersion: Hdr:Proc
rm.OSVersion: Version
