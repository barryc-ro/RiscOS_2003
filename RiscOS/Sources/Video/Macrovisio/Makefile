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
rm.MacroC: Hdr.GetAll
rm.MacroC: Hdr:ListOpts
rm.MacroC: Hdr:Macros
rm.MacroC: Hdr:System
rm.MacroC: Hdr:SWIs
rm.MacroC: Hdr:CPU.Generic26
rm.MacroC: Hdr:IO.GenericIO
rm.MacroC: Hdr:RISCOS
rm.MacroC: Hdr:Machine.<Machine>
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
