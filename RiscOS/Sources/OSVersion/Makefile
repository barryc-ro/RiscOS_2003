# Makefile for OSVersion
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-May-96  SMC          Created.
# 10 Sep 1997 RWB	  Switch Version number on System and UserIF
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
TARGET    = rm.${SYSTEM}.${UserIF}.OSVersion
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
	${MKDIR} rm.${SYSTEM}
	${MKDIR} rm.${SYSTEM}.${UserIF}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.RCA.RCA.OSVersion: Hdr:ListOpts
rm.RCA.RCA.OSVersion: Hdr:Macros
rm.RCA.RCA.OSVersion: Hdr:System
rm.RCA.RCA.OSVersion: Hdr:SWIs
rm.RCA.RCA.OSVersion: Hdr:CPU.Generic26
rm.RCA.RCA.OSVersion: Hdr:IO.GenericIO
rm.RCA.RCA.OSVersion: Hdr:RISCOS
rm.RCA.RCA.OSVersion: Hdr:ModHand
rm.RCA.RCA.OSVersion: Hdr:Machine.<Machine>
rm.RCA.RCA.OSVersion: Hdr:Machine.Machine
rm.RCA.RCA.OSVersion: Hdr:Proc
rm.RCA.RCA.OSVersion: s.<System>.<UserIF>
