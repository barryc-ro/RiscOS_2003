# Makefile for Interlace
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name       Description
# ----       ----       -----------
# 27-Nov-95  TMD	Created.
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
ROMTARGETS =

DISCTARGETS =	${ROMTARGETS} \
		rm.Macrovisio

INSTROMTARGETS =

INSTALLTARGETS = ${INSTROMTARGETS} \
		 ${INSTDIR}.Macrovisio \

#
# Generic rules:
#
rom: ${ROMTARGETS}
        @echo ${COMPONENT}: rom modules built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${INSTROMTARGETS}
        @echo ${COMPONENT}: rom modules installed

all: ${DISCTARGETS}
	@echo ${COMPONENT}: disc modules built

install: ${INSTALLTARGETS}
	@echo ${COMPONENT}: disc modules installed

${INSTDIR}.Macrovisio: rm.Macrovisio
	${CP} $? $@ ${CPFLAGS}

rm.Macrovisio: Data.YUVTable
	${AS} ${ASFLAGS} -From Hdr.Macrovisio -To rm.Macrovisio

Data.YUVTable: BASIC.GenYUVTab
	Run BASIC.GenYUVTab

# Dynamic dependencies:
rm.Macrovisio: Hdr.GetAll
rm.Macrovisio: Hdr:ListOpts
rm.Macrovisio: Hdr:Macros
rm.Macrovisio: Hdr:System
rm.Macrovisio: Hdr:SWIs
rm.Macrovisio: Hdr:CPU.Generic26
rm.Macrovisio: Hdr:IO.GenericIO
rm.Macrovisio: Hdr:RISCOS
rm.Macrovisio: Hdr:Machine.<Machine>
rm.Macrovisio: Hdr:Machine.<Machine>
rm.Macrovisio: Hdr:IO.IOMDL
rm.Macrovisio: Hdr:ModHand
rm.Macrovisio: Hdr:Services
rm.Macrovisio: Hdr:Proc
rm.Macrovisio: Hdr:FSNumbers
rm.Macrovisio: Hdr:NewErrors
rm.Macrovisio: Hdr:MsgTrans
rm.Macrovisio: Hdr:PaletteV
rm.Macrovisio: Hdr:IIC
rm.Macrovisio: Hdr:ColourTran
rm.Macrovisio: Hdr:Debug
rm.Macrovisio: s.Module
