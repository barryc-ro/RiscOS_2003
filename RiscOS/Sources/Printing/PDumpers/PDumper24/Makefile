# Makefile for PDumper24 (Specialised 24 pin Printer dumper)
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
COMPONENT = PDumper24
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
rm.PDumper24: ^.Generic.s.Header
rm.PDumper24: hdr:ListOpts
rm.PDumper24: hdr:Macros
rm.PDumper24: hdr:System
rm.PDumper24: Hdr:SWIs
rm.PDumper24: Hdr:CPU.Generic26
rm.PDumper24: Hdr:IO.GenericIO
rm.PDumper24: Hdr:RISCOS
rm.PDumper24: hdr:FSNumbers
rm.PDumper24: hdr:HighFSI
rm.PDumper24: hdr:Modhand
rm.PDumper24: hdr:NewErrors
rm.PDumper24: hdr:PDriver
rm.PDumper24: hdr:PDriverReg
rm.PDumper24: hdr:PDumper
rm.PDumper24: hdr:PDumperReg
rm.PDumper24: hdr:Services
rm.PDumper24: hdr:Sprite
rm.PDumper24: hdr:ColourTran
rm.PDumper24: hdr:Proc
rm.PDumper24: hdr:MsgTrans
rm.PDumper24: hdr:HostFS
rm.PDumper24: hdr:NDRDebug
rm.PDumper24: hdr:DDVMacros
rm.PDumper24: hdr:HostDebug
rm.PDumper24: s.Version
rm.PDumper24: ^.Version
rm.PDumper24: s.Macros
rm.PDumper24: ^.Generic.s.Macros
rm.PDumper24: s.Workspace
rm.PDumper24: s.BranchCode
rm.PDumper24: ^.Generic.s.BranchCode
rm.PDumper24: s.24Bit
rm.PDumper24: ^.Generic.s.24Bit
rm.PDumper24: s.SetDriver
rm.PDumper24: ^.Generic.s.SetDriver
rm.PDumper24: s.OutputDump
rm.PDumper24: ^.Generic.s.OutputDump
rm.PDumper24: s.ColourSet
rm.PDumper24: ^.Generic.s.ColourSet
rm.PDumper24: s.StartPage
rm.PDumper24: ^.Generic.s.SendExtra
rm.PDumper24: s.EndPage
rm.PDumper24: ^.Generic.s.EndPage
rm.PDumper24: s.AbortJob
rm.PDumper24: ^.Generic.s.AbortJob
rm.PDumper24: s.StartJob
rm.PDumper24: ^.Generic.s.StartJob
rm.PDumper24: s.MiscOp
rm.PDumper24: ^.Generic.s.MiscOp
