# Makefile for CLIProtect
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
COMPONENT = CLIProt
SOURCE    = s.GetAll
TARGET    = rm.${COMPONENT}
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
rm.CLIProt: Hdr:ListOpts
rm.CLIProt: Hdr:Macros
rm.CLIProt: Hdr:System
rm.CLIProt: Hdr:SWIs
rm.CLIProt: Hdr:CPU.Generic26
rm.CLIProt: Hdr:IO.GenericIO
rm.CLIProt: Hdr:RISCOS
rm.CLIProt: Hdr:ModHand
rm.CLIProt: Hdr:Services
rm.CLIProt: Hdr:MsgTrans
rm.CLIProt: Hdr:ResourceFS
rm.CLIProt: Hdr:NdrDebug
rm.CLIProt: Hdr:DDVMacros
rm.CLIProt: Hdr:Proc
rm.CLIProt: Version
rm.CLIProt: s.Module
