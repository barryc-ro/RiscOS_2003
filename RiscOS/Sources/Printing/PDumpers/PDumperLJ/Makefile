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
rm.PDumperLJ: ^.Generic.s.Header hdr:ListOpts hdr:Macros hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS hdr:FSNumbers hdr:HighFSI hdr:Modhand hdr:NewErrors hdr:PDriver hdr:PDriverReg hdr:PDumper hdr:PDumperReg hdr:Services hdr:Sprite hdr:ColourTran hdr:Proc hdr:MsgTrans hdr:HostFS hdr:NDRDebug hdr:DDVMacros hdr:HostDebug s.Version ^.VersionASM s.Macros ^.Generic.s.Macros s.Workspace s.BranchCode ^.Generic.s.BranchCode s.24Bit ^.Generic.s.24Bit s.SetDriver ^.Generic.s.SetDriver s.OutputDump ^.Generic.s.OutputDump s.ColourSet ^.Generic.s.ColourSet s.StartPage ^.Generic.s.SendExtra s.EndPage s.AbortJob ^.Generic.s.AbortJob s.StartJob ^.Generic.s.StartJob s.MiscOp ^.Generic.s.MiscOp
