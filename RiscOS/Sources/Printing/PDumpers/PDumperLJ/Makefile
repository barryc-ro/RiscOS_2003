# Makefile for PDumperLJ (Printer dumper for LaserJets)
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
COMPONENT = PDumperLJ
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
rm.PDumperLJ: ^.Generic.s.Header
rm.PDumperLJ: hdr:ListOpts
rm.PDumperLJ: hdr:Macros
rm.PDumperLJ: hdr:System
rm.PDumperLJ: Hdr:SWIs
rm.PDumperLJ: Hdr:CPU.Generic26
rm.PDumperLJ: Hdr:IO.GenericIO
rm.PDumperLJ: Hdr:RISCOS
rm.PDumperLJ: hdr:FSNumbers
rm.PDumperLJ: hdr:HighFSI
rm.PDumperLJ: hdr:Modhand
rm.PDumperLJ: hdr:NewErrors
rm.PDumperLJ: hdr:PDriver
rm.PDumperLJ: hdr:PDriverReg
rm.PDumperLJ: hdr:PDumper
rm.PDumperLJ: hdr:PDumperReg
rm.PDumperLJ: hdr:Services
rm.PDumperLJ: hdr:Sprite
rm.PDumperLJ: hdr:ColourTran
rm.PDumperLJ: hdr:Proc
rm.PDumperLJ: hdr:MsgTrans
rm.PDumperLJ: hdr:HostFS
rm.PDumperLJ: hdr:NDRDebug
rm.PDumperLJ: hdr:DDVMacros
rm.PDumperLJ: hdr:HostDebug
rm.PDumperLJ: s.Version
rm.PDumperLJ: ^.Version
rm.PDumperLJ: s.Macros
rm.PDumperLJ: ^.Generic.s.Macros
rm.PDumperLJ: s.Workspace
rm.PDumperLJ: s.BranchCode
rm.PDumperLJ: ^.Generic.s.BranchCode
rm.PDumperLJ: s.24Bit
rm.PDumperLJ: ^.Generic.s.24Bit
rm.PDumperLJ: s.SetDriver
rm.PDumperLJ: ^.Generic.s.SetDriver
rm.PDumperLJ: s.OutputDump
rm.PDumperLJ: ^.Generic.s.OutputDump
rm.PDumperLJ: s.ColourSet
rm.PDumperLJ: ^.Generic.s.ColourSet
rm.PDumperLJ: s.StartPage
rm.PDumperLJ: ^.Generic.s.SendExtra
rm.PDumperLJ: s.EndPage
rm.PDumperLJ: s.AbortJob
rm.PDumperLJ: ^.Generic.s.AbortJob
rm.PDumperLJ: s.StartJob
rm.PDumperLJ: s.MiscOp
rm.PDumperLJ: ^.Generic.s.MiscOp
