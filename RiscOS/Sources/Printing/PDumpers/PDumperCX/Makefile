# Makefile for PDumperCX
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 24-Jul-95  MJS         Created.
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
COMPONENT = PDumperCX
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
rm.PDumperCX: ^.Generic.s.Header
rm.PDumperCX: hdr:ListOpts
rm.PDumperCX: hdr:Macros
rm.PDumperCX: hdr:System
rm.PDumperCX: Hdr:SWIs
rm.PDumperCX: Hdr:CPU.Generic26
rm.PDumperCX: Hdr:IO.GenericIO
rm.PDumperCX: Hdr:RISCOS
rm.PDumperCX: hdr:FSNumbers
rm.PDumperCX: hdr:HighFSI
rm.PDumperCX: hdr:Modhand
rm.PDumperCX: hdr:NewErrors
rm.PDumperCX: hdr:PDriver
rm.PDumperCX: hdr:PDriverReg
rm.PDumperCX: hdr:PDumper
rm.PDumperCX: hdr:PDumperReg
rm.PDumperCX: hdr:Services
rm.PDumperCX: hdr:Sprite
rm.PDumperCX: hdr:ColourTran
rm.PDumperCX: hdr:Proc
rm.PDumperCX: hdr:MsgTrans
rm.PDumperCX: hdr:HostFS
rm.PDumperCX: hdr:NDRDebug
rm.PDumperCX: hdr:DDVMacros
rm.PDumperCX: hdr:HostDebug
rm.PDumperCX: s.Version
rm.PDumperCX: ^.Version
rm.PDumperCX: s.Macros
rm.PDumperCX: ^.Generic.s.Macros
rm.PDumperCX: s.Workspace
rm.PDumperCX: s.BranchCode
rm.PDumperCX: ^.Generic.s.BranchCode
rm.PDumperCX: s.24Bit
rm.PDumperCX: s.SetDriver
rm.PDumperCX: ^.Generic.s.SetDriver
rm.PDumperCX: s.OutputDump
rm.PDumperCX: s.Prepstrips
rm.PDumperCX: s.ColourSet
rm.PDumperCX: ^.Generic.s.ColourSet
rm.PDumperCX: s.StartPage
rm.PDumperCX: ^.Generic.s.SendExtra
rm.PDumperCX: s.EndPage
rm.PDumperCX: ^.Generic.s.EndPage
rm.PDumperCX: s.AbortJob
rm.PDumperCX: s.StartJob
rm.PDumperCX: s.OutputPal
rm.PDumperCX: s.MiscOp
rm.PDumperCX: ^.Generic.s.MiscOp
