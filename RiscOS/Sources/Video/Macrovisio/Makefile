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
WIPE    = wipe
STRIP   = -stripdepnd
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Macrovisio
ROMTARGET =	rm.${MACHINE}.${COMPONENT}

DISCTARGETS =	${ROMTARGET} \
		rm.MacroA \
		rm.MacroB \
		rm.MacroC \
		rm.MacroD

#
# Generic rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom modules built

clean:
	${RM} rm.MacroA
	${RM} rm.MacroB
	${RM} rm.MacroC
	${RM} rm.MacroD
	-${WIPE} rm.${MACHINE} ~CFR~V
	${RM} data.YUVTable
	${STRIP}
        @echo ${COMPONENT}: module cleaned

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom modules installed

all: ${DISCTARGETS}
	@echo ${COMPONENT}: disc modules built

install: ${DISCTARGETS}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${CP} rm.MacroA ${INSTDIR}.MacroA ${CPFLAGS}
        ${CP} rm.MacroB ${INSTDIR}.MacroB ${CPFLAGS}
        ${CP} rm.MacroC ${INSTDIR}.MacroC ${CPFLAGS}
        ${CP} rm.MacroD ${INSTDIR}.MacroD ${CPFLAGS}
	@echo ${COMPONENT}: disc modules installed

#
# ROM variants
#

rm.Falcon.Macrovisio: rm.MacroA
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Peregrine.Macrovisio: rm.MacroC
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.STB3.Macrovisio: rm.MacroD
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.MacroA: Data.YUVTable Hdr.MacroA
	${MKDIR} rm
	${AS} ${ASFLAGS} -From Hdr.MacroA -To rm.MacroA

rm.MacroB: Data.YUVTable Hdr.MacroB
	${MKDIR} rm
	${AS} ${ASFLAGS} -From Hdr.MacroB -To rm.MacroB

rm.MacroC: Data.YUVTable Hdr.MacroC
	${MKDIR} rm
	${AS} ${ASFLAGS} -From Hdr.MacroC -To rm.MacroC

rm.MacroD: Data.YUVTable Hdr.MacroD
	${MKDIR} rm
	${AS} ${ASFLAGS} -From Hdr.MacroD -To rm.MacroD

Data.YUVTable: BASIC.GenYUVTab
	Run BASIC.GenYUVTab

# Dynamic dependencies:
