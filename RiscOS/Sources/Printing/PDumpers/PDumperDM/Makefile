# Makefile for PDumperDM
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-May-94  AMcC         Created.
# 27 Jun 1997 RWB	  Build seperatly from rest of dumpers
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
COMPONENT = PDumperDM
EXPORT    = ${EXP_HDR}.PDumper
SOURCE    = s.Header
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
clean:
	${RM} ${TARGET}
	${RM} rm

all: ${TARGET}
        @echo ${COMPONENT}: module built
        
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORT}
        @echo ${COMPONENT}: export complete

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	Access ${INSTDIR}.${COMPONENT} LR/r
        @echo ${COMPONENT}: installed (disc)

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed 

${TARGET}: ${SOURCE}
	${MKDIR} rm
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}

${EXP_HDR}.PDumper: ^.hdr.PDumper
        ${CP} ^.hdr.PDumper $@ ${CPFLAGS}

# Dynamic dependencies:
rm.PDumperDM: ^.Generic.s.Header
rm.PDumperDM: hdr:ListOpts
rm.PDumperDM: hdr:Macros
rm.PDumperDM: hdr:System
rm.PDumperDM: Hdr:SWIs
rm.PDumperDM: Hdr:CPU.Generic26
rm.PDumperDM: Hdr:IO.GenericIO
rm.PDumperDM: Hdr:RISCOS
rm.PDumperDM: hdr:FSNumbers
rm.PDumperDM: hdr:HighFSI
rm.PDumperDM: hdr:Modhand
rm.PDumperDM: hdr:NewErrors
rm.PDumperDM: hdr:PDriver
rm.PDumperDM: hdr:PDriverReg
rm.PDumperDM: hdr:PDumper
rm.PDumperDM: hdr:PDumperReg
rm.PDumperDM: hdr:Services
rm.PDumperDM: hdr:Sprite
rm.PDumperDM: hdr:ColourTran
rm.PDumperDM: hdr:Proc
rm.PDumperDM: hdr:MsgTrans
rm.PDumperDM: hdr:HostFS
rm.PDumperDM: hdr:NDRDebug
rm.PDumperDM: hdr:DDVMacros
rm.PDumperDM: hdr:HostDebug
rm.PDumperDM: s.Version
rm.PDumperDM: ^.Version
rm.PDumperDM: s.Macros
rm.PDumperDM: ^.Generic.s.Macros
rm.PDumperDM: s.Workspace
rm.PDumperDM: s.BranchCode
rm.PDumperDM: ^.Generic.s.BranchCode
rm.PDumperDM: s.24Bit
rm.PDumperDM: ^.Generic.s.24Bit
rm.PDumperDM: s.SetDriver
rm.PDumperDM: ^.Generic.s.SetDriver
rm.PDumperDM: s.OutputDump
rm.PDumperDM: ^.Generic.s.OutputDump
rm.PDumperDM: s.DumpIX
rm.PDumperDM: s.DumpJX
rm.PDumperDM: s.ColourSet
rm.PDumperDM: ^.Generic.s.ColourSet
rm.PDumperDM: s.StartPage
rm.PDumperDM: ^.Generic.s.SendExtra
rm.PDumperDM: s.EndPage
rm.PDumperDM: ^.Generic.s.EndPage
rm.PDumperDM: s.AbortJob
rm.PDumperDM: ^.Generic.s.AbortJob
rm.PDumperDM: s.StartJob
rm.PDumperDM: ^.Generic.s.StartJob
rm.PDumperDM: s.MiscOp
rm.PDumperDM: ^.Generic.s.MiscOp
