# Makefile for VSPFile
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Aug-95  TMD          Created.
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
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = VSPFile
SOURCE    = Hdr.GetAll
TARGET    = rm.VSPFile

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

all: ${TARGET}
        @echo ${COMPONENT}: disc module built

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: disc module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -From ${SOURCE} -To $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.VSPFile: Version
rm.VSPFile: hdr.VSP<VSP$Type>
rm.VSPFile: Hdr:ListOpts
rm.VSPFile: Hdr:Macros
rm.VSPFile: Hdr:System
rm.VSPFile: Hdr:SWIs
rm.VSPFile: Hdr:CPU.Generic26
rm.VSPFile: Hdr:IO.GenericIO
rm.VSPFile: Hdr:RISCOS
rm.VSPFile: Hdr:CMOS
rm.VSPFile: Hdr:ModHand
rm.VSPFile: Hdr:Services
rm.VSPFile: Hdr:Proc
rm.VSPFile: Hdr:FSNumbers
rm.VSPFile: Hdr:NewErrors
rm.VSPFile: Hdr:MsgTrans
rm.VSPFile: Hdr:Debug
rm.VSPFile: Hdr:HighFSI
rm.VSPFile: Hdr:MPEGCtrl
rm.VSPFile: Hdr:VSP
rm.VSPFile: hdr.defs
rm.VSPFile: s.Module
