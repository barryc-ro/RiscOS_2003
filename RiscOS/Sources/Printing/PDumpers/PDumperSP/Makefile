# Makefile for PDumperSP (Simple Printer dumper that outputs monochrome sprites)
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
COMPONENT = PDumperSP
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
rm.PDumperSP: ^.Generic.s.Header
rm.PDumperSP: hdr:ListOpts
rm.PDumperSP: hdr:Macros
rm.PDumperSP: hdr:System
rm.PDumperSP: Hdr:SWIs
rm.PDumperSP: Hdr:CPU.Generic26
rm.PDumperSP: Hdr:IO.GenericIO
rm.PDumperSP: Hdr:RISCOS
rm.PDumperSP: hdr:FSNumbers
rm.PDumperSP: hdr:HighFSI
rm.PDumperSP: hdr:Modhand
rm.PDumperSP: hdr:NewErrors
rm.PDumperSP: hdr:PDriver
rm.PDumperSP: hdr:PDriverReg
rm.PDumperSP: hdr:PDumper
rm.PDumperSP: hdr:PDumperReg
rm.PDumperSP: hdr:Services
rm.PDumperSP: hdr:Sprite
rm.PDumperSP: hdr:ColourTran
rm.PDumperSP: hdr:Proc
rm.PDumperSP: hdr:MsgTrans
rm.PDumperSP: hdr:HostFS
rm.PDumperSP: hdr:NDRDebug
rm.PDumperSP: hdr:DDVMacros
rm.PDumperSP: hdr:HostDebug
rm.PDumperSP: s.Version
rm.PDumperSP: ^.Version
rm.PDumperSP: s.Macros
rm.PDumperSP: ^.Generic.s.Macros
rm.PDumperSP: s.Workspace
rm.PDumperSP: s.BranchCode
rm.PDumperSP: ^.Generic.s.BranchCode
rm.PDumperSP: s.24Bit
rm.PDumperSP: ^.Generic.s.24Bit
rm.PDumperSP: s.SetDriver
rm.PDumperSP: ^.Generic.s.SetDriver
rm.PDumperSP: s.OutputDump
rm.PDumperSP: ^.Generic.s.OutputDump
rm.PDumperSP: s.ColourSet
rm.PDumperSP: ^.Generic.s.ColourSet
rm.PDumperSP: s.StartPage
rm.PDumperSP: s.EndPage
rm.PDumperSP: s.AbortJob
rm.PDumperSP: s.StartJob
rm.PDumperSP: s.MiscOp
rm.PDumperSP: ^.Generic.s.MiscOp
