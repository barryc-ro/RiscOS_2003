# Makefile for Sound DMA Handler (Sound0)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
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
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Sound0
SOURCE    = s.Sound0
TARGET    = rm.${MACHINE}.Sound0
EXPORTS   = ${EXP_HDR}.Sound

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} s.TokHelpSrc
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.SoundDMA
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.SoundDMA.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.SoundDMA.Messages }

${TARGET}: ${SOURCE} s.TokHelpSrc
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

s.TokHelpSrc: Hdr:Tokens HelpSrc
        tokenise Hdr:Tokens HelpSrc $@

${EXP_HDR}.Sound: hdr.Sound
        ${CP} hdr.Sound $@ ${CPFLAGS}

# Dynamic dependencies:
rm.Morris.Sound0: Hdr:ListOpts
rm.Morris.Sound0: Hdr:Macros
rm.Morris.Sound0: Hdr:System
rm.Morris.Sound0: Hdr:SWIs
rm.Morris.Sound0: Hdr:CPU.Generic26
rm.Morris.Sound0: Hdr:IO.GenericIO
rm.Morris.Sound0: Hdr:RISCOS
rm.Morris.Sound0: Hdr:Machine.<Machine>
rm.Morris.Sound0: Hdr:VIDC.VIDC20
rm.Morris.Sound0: Hdr:MEMM.ARM600
rm.Morris.Sound0: Hdr:IO.IOMDL
rm.Morris.Sound0: Hdr:CMOS
rm.Morris.Sound0: Hdr:ModHand
rm.Morris.Sound0: Hdr:Services
rm.Morris.Sound0: Hdr:FSNumbers
rm.Morris.Sound0: Hdr:NewErrors
rm.Morris.Sound0: Hdr:PublicWS
rm.Morris.Sound0: Hdr:Tokens
rm.Morris.Sound0: Hdr:DevNos
rm.Morris.Sound0: Hdr:Proc
rm.Morris.Sound0: Hdr:VduExt
rm.Morris.Sound0: Hdr:MsgTrans
rm.Morris.Sound0: Hdr:HostFS
rm.Morris.Sound0: Hdr:DDVMacros
rm.Morris.Sound0: Hdr:NDRDebug
rm.Morris.Sound0: Hdr:Sound
rm.Morris.Sound0: Hdr:Portable
rm.Morris.Sound0: Version
rm.Morris.Sound0: s.TokHelpSrc
rm.Morris.Sound0: s.Sound0_SA
rm.Omega.Sound0: Hdr:ListOpts
rm.Omega.Sound0: Hdr:Macros
rm.Omega.Sound0: Hdr:System
rm.Omega.Sound0: Hdr:SWIs
rm.Omega.Sound0: Hdr:CPU.Generic26
rm.Omega.Sound0: Hdr:IO.GenericIO
rm.Omega.Sound0: Hdr:RISCOS
rm.Omega.Sound0: Hdr:Machine.<Machine>
rm.Omega.Sound0: Hdr:VIDC.VIDC20
rm.Omega.Sound0: Hdr:MEMM.ARM600
rm.Omega.Sound0: Hdr:IO.IOMDL
rm.Omega.Sound0: Hdr:CMOS
rm.Omega.Sound0: Hdr:ModHand
rm.Omega.Sound0: Hdr:Services
rm.Omega.Sound0: Hdr:FSNumbers
rm.Omega.Sound0: Hdr:NewErrors
rm.Omega.Sound0: Hdr:PublicWS
rm.Omega.Sound0: Hdr:Tokens
rm.Omega.Sound0: Hdr:DevNos
rm.Omega.Sound0: Hdr:Proc
rm.Omega.Sound0: Hdr:VduExt
rm.Omega.Sound0: Hdr:MsgTrans
rm.Omega.Sound0: Hdr:HostFS
rm.Omega.Sound0: Hdr:DDVMacros
rm.Omega.Sound0: Hdr:NDRDebug
rm.Omega.Sound0: Hdr:Sound
rm.Omega.Sound0: Hdr:Portable
rm.Omega.Sound0: Version
rm.Omega.Sound0: s.TokHelpSrc
rm.RCA.Sound0: Hdr:ListOpts
rm.RCA.Sound0: Hdr:Macros
rm.RCA.Sound0: Hdr:System
rm.RCA.Sound0: Hdr:SWIs
rm.RCA.Sound0: Hdr:CPU.Generic26
rm.RCA.Sound0: Hdr:IO.GenericIO
rm.RCA.Sound0: Hdr:RISCOS
rm.RCA.Sound0: Hdr:Machine.<Machine>
rm.RCA.Sound0: Hdr:VIDC.VIDC20
rm.RCA.Sound0: Hdr:MEMM.ARM600
rm.RCA.Sound0: Hdr:IO.IOMDL
rm.RCA.Sound0: Hdr:CMOS
rm.RCA.Sound0: Hdr:ModHand
rm.RCA.Sound0: Hdr:Services
rm.RCA.Sound0: Hdr:FSNumbers
rm.RCA.Sound0: Hdr:NewErrors
rm.RCA.Sound0: Hdr:PublicWS
rm.RCA.Sound0: Hdr:Tokens
rm.RCA.Sound0: Hdr:DevNos
rm.RCA.Sound0: Hdr:Proc
rm.RCA.Sound0: Hdr:VduExt
rm.RCA.Sound0: Hdr:MsgTrans
rm.RCA.Sound0: Hdr:HostFS
rm.RCA.Sound0: Hdr:DDVMacros
rm.RCA.Sound0: Hdr:NDRDebug
rm.RCA.Sound0: Hdr:Sound
rm.RCA.Sound0: Hdr:Portable
rm.RCA.Sound0: Version
rm.RCA.Sound0: s.TokHelpSrc
