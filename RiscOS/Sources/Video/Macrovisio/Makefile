# Makefile for Macrovision
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name       Description
# ----       ----       -----------
# 27-Nov-95  TMD	Created.
# 12-Sep-96  TMD	Added variant C for STB2/2
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
COMPONENT = Macrovisio
ROMTARGET =	rm.${MACHINE}.${COMPONENT}

DISCTARGETS =	${ROMTARGETS} \
		rm.MacroA \
		rm.MacroB \
		rm.MacroC

INSTROMTARGETS = ${INSTDIR}.${COMPONENT}

INSTALLTARGETS = ${INSTROMTARGETS} \
		 ${INSTDIR}.MacroA \
		 ${INSTDIR}.MacroB \
		 ${INSTDIR}.MacroC

#
# Generic rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom modules built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTROMTARGETS} ${CPFLAGS}
        @echo ${COMPONENT}: rom modules installed

all: ${DISCTARGETS}
	@echo ${COMPONENT}: disc modules built

install: ${INSTALLTARGETS}
	@echo ${COMPONENT}: disc modules installed

${INSTDIR}.MacroA: rm.MacroA
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.MacroB: rm.MacroB
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.MacroC: rm.MacroC
	${CP} $? $@ ${CPFLAGS}

#
# ROM variants
#

rm.Falcon.Macrovisio: rm.MacroA
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Peregrine.Macrovisio: rm.MacroC
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.MacroA: Data.YUVTable Hdr.MacroA
	${AS} ${ASFLAGS} -From Hdr.MacroA -To rm.MacroA

rm.MacroB: Data.YUVTable Hdr.MacroB
	${AS} ${ASFLAGS} -From Hdr.MacroB -To rm.MacroB

rm.MacroC: Data.YUVTable Hdr.MacroC
	${AS} ${ASFLAGS} -From Hdr.MacroC -To rm.MacroC

Data.YUVTable: BASIC.GenYUVTab
	Run BASIC.GenYUVTab

# Dynamic dependencies:
rm.MacroA: Hdr.GetAll
rm.MacroA: Hdr:ListOpts
rm.MacroA: Hdr:Macros
rm.MacroA: Hdr:System
rm.MacroA: Hdr:SWIs
rm.MacroA: Hdr:CPU.Generic26
rm.MacroA: Hdr:IO.GenericIO
rm.MacroA: Hdr:RISCOS
rm.MacroA: Hdr:Machine.<Machine>
rm.MacroA: Hdr:IO.IOMDL
rm.MacroA: Hdr:ModHand
rm.MacroA: Hdr:Services
rm.MacroA: Hdr:Proc
rm.MacroA: Hdr:FSNumbers
rm.MacroA: Hdr:NewErrors
rm.MacroA: Hdr:MsgTrans
rm.MacroA: Hdr:PaletteV
rm.MacroA: Hdr:IIC
rm.MacroA: Hdr:ColourTran
rm.MacroA: Hdr:Debug
rm.MacroA: Hdr:Macrovisio
rm.MacroA: Hdr:MPEG2Card
rm.MacroA: Hdr:MPEGVideo
rm.MacroA: s.Module
rm.MacroB: Hdr.GetAll
rm.MacroB: Hdr:ListOpts
rm.MacroB: Hdr:Macros
rm.MacroB: Hdr:System
rm.MacroB: Hdr:SWIs
rm.MacroB: Hdr:CPU.Generic26
rm.MacroB: Hdr:IO.GenericIO
rm.MacroB: Hdr:RISCOS
rm.MacroB: Hdr:Machine.<Machine>
rm.MacroB: Hdr:IO.IOMDL
rm.MacroB: Hdr:ModHand
rm.MacroB: Hdr:Services
rm.MacroB: Hdr:Proc
rm.MacroB: Hdr:FSNumbers
rm.MacroB: Hdr:NewErrors
rm.MacroB: Hdr:MsgTrans
rm.MacroB: Hdr:PaletteV
rm.MacroB: Hdr:IIC
rm.MacroB: Hdr:ColourTran
rm.MacroB: Hdr:Debug
rm.MacroB: Hdr:Macrovisio
rm.MacroB: Hdr:MPEG2Card
rm.MacroB: Hdr:MPEGVideo
rm.MacroB: s.Module
rm.MacroC: Hdr.GetAll
rm.MacroC: Hdr:ListOpts
rm.MacroC: Hdr:Macros
rm.MacroC: Hdr:System
rm.MacroC: Hdr:SWIs
rm.MacroC: Hdr:CPU.Generic26
rm.MacroC: Hdr:IO.GenericIO
rm.MacroC: Hdr:RISCOS
rm.MacroC: Hdr:Machine.<Machine>
rm.MacroC: Hdr:IO.IOMDL
rm.MacroC: Hdr:ModHand
rm.MacroC: Hdr:Services
rm.MacroC: Hdr:Proc
rm.MacroC: Hdr:FSNumbers
rm.MacroC: Hdr:NewErrors
rm.MacroC: Hdr:MsgTrans
rm.MacroC: Hdr:PaletteV
rm.MacroC: Hdr:IIC
rm.MacroC: Hdr:ColourTran
rm.MacroC: Hdr:Debug
rm.MacroC: Hdr:Macrovisio
rm.MacroC: Hdr:MPEG2Card
rm.MacroC: Hdr:MPEGVideo
rm.MacroC: s.Module
