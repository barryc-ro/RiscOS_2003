# Makefile for Kernel
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
ASFLAGS = -depend !Depend -Stamp -quit -To $@ -From
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Kernel
SOURCE    = s.GetAll
TARGET    = rm.${MACHINE}.Kernel
EXPORTS   = ${EXP_HDR}.EnvNumbers \
            ${EXP_HDR}.ModHand \
            ${EXP_HDR}.PublicWS \
            ${EXP_HDR}.RISCOS \
            ${EXP_HDR}.Variables \
            ${EXP_HDR}.VduExt

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} TMOSHelp
        @echo ${COMPONENT}: cleaned

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        Set Kernel$Messages LocalRes:Messages
        IfThere LocalRes:<UserIF>.Messages Then Set Kernel$Messages LocalRes:<UserIF>.Messages
        TokenCheck LocalRes:<UserIF>.Messages
        ${CP} <Kernel$Messages>  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        UnSet Kernel$Messages

resources-None: resources_common
	@

resources-: resources_common
        TokenCheck LocalRes:Messages
        print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${TARGET}: ${SOURCE} s.TMOSHelp
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}
        stamp $@

s.TMOSHelp: ${TOKENS} HelpStrs
        ${TOKENISE} ${TOKENS} HelpStrs $@

#
# Exported interface headers
#
${EXP_HDR}.EnvNumbers: hdr.EnvNumbers
        ${CP} hdr.EnvNumbers $@ ${CPFLAGS}

${EXP_HDR}.ModHand: hdr.ModHand
        ${CP} hdr.ModHand $@ ${CPFLAGS}

${EXP_HDR}.PublicWS: hdr.PublicWS
        ${CP} hdr.PublicWS $@ ${CPFLAGS}

${EXP_HDR}.RISCOS: hdr.RISCOS
        ${CP} hdr.RISCOS $@ ${CPFLAGS}

${EXP_HDR}.VduExt: hdr.VduExt
        ${CP} hdr.VduExt $@ ${CPFLAGS}

${EXP_HDR}.Variables: hdr.Variables
        ${CP} hdr.Variables $@ ${CPFLAGS}

# Dynamic dependencies:
rm.Funai.Kernel: Hdr:ListOpts
rm.Funai.Kernel: Hdr:Macros
rm.Funai.Kernel: Hdr:System
rm.Funai.Kernel: Hdr:SWIs
rm.Funai.Kernel: Hdr:CPU.Generic26
rm.Funai.Kernel: Hdr:IO.GenericIO
rm.Funai.Kernel: Hdr:RISCOS
rm.Funai.Kernel: Hdr:Machine.<Machine>
rm.Funai.Kernel: Hdr:Machine.Machine
rm.Funai.Kernel: Hdr:ImageSize.<ImageSize>
rm.Funai.Kernel: Hdr:CPU.ARM600
rm.Funai.Kernel: Hdr:IO.IOMDL
rm.Funai.Kernel: Hdr:IO.IOMD
rm.Funai.Kernel: Hdr:MEMM.ARM600
rm.Funai.Kernel: Hdr:VIDC.VIDC20
rm.Funai.Kernel: Version
rm.Funai.Kernel: VersionASM
rm.Funai.Kernel: Hdr:CMOS
rm.Funai.Kernel: Hdr:Heap
rm.Funai.Kernel: Hdr:PublicWS
rm.Funai.Kernel: Hdr:KernelWS
rm.Funai.Kernel: Hdr:Services
rm.Funai.Kernel: Hdr:FSNumbers
rm.Funai.Kernel: Hdr:HighFSI
rm.Funai.Kernel: Hdr:NewErrors
rm.Funai.Kernel: Hdr:Proc
rm.Funai.Kernel: Hdr:Sprite
rm.Funai.Kernel: Hdr:KeyWS
rm.Funai.Kernel: Hdr:RS423
rm.Funai.Kernel: Hdr:ModHand
rm.Funai.Kernel: Hdr:Variables
rm.Funai.Kernel: Hdr:EnvNumbers
rm.Funai.Kernel: Hdr:UpCall
rm.Funai.Kernel: Hdr:Sound
rm.Funai.Kernel: Hdr:Pointer
rm.Funai.Kernel: Hdr:Podule
rm.Funai.Kernel: Hdr:VduExt
rm.Funai.Kernel: Hdr:Buffer
rm.Funai.Kernel: Hdr:Font
rm.Funai.Kernel: Hdr:DevNos
rm.Funai.Kernel: Hdr:Territory
rm.Funai.Kernel: Hdr:Portable
rm.Funai.Kernel: Hdr:MsgTrans
rm.Funai.Kernel: Hdr:PaletteV
rm.Funai.Kernel: Hdr:Wimp
rm.Funai.Kernel: Hdr:ColourTran
rm.Funai.Kernel: Hdr:Debug
rm.Funai.Kernel: s.PMF.DEF
rm.Funai.Kernel: s.Copro15ops
rm.Funai.Kernel: s.Kernel
rm.Funai.Kernel: s.Morris
rm.Funai.Kernel: s.NewIRQs
rm.Funai.Kernel: s.Oscli
rm.Funai.Kernel: s.SysComms
rm.Funai.Kernel: s.MosDict
rm.Funai.Kernel: s.TMOSHelp
rm.Funai.Kernel: s.HeapMan
rm.Funai.Kernel: s.ModHand
rm.Funai.Kernel: s.Unsqueeze
rm.Funai.Kernel: s.ArthurSWIs
rm.Funai.Kernel: s.ChangeDyn
rm.Funai.Kernel: s.Arthur2
rm.Funai.Kernel: s.Utility
rm.Funai.Kernel: s.MoreComms
rm.Funai.Kernel: s.Convrsions
rm.Funai.Kernel: s.MoreSWIs
rm.Funai.Kernel: s.ExtraSWIs
rm.Funai.Kernel: s.HeapSort
rm.Funai.Kernel: s.Arthur3
rm.Funai.Kernel: s.SWINaming
rm.Funai.Kernel: s.TickEvents
rm.Funai.Kernel: s.KbdResPC
rm.Funai.Kernel: s.NewReset
rm.Funai.Kernel: s.MsgCode
rm.Funai.Kernel: s.Middle
rm.Funai.Kernel: s.Super1
rm.Funai.Kernel: s.ARM600
rm.Funai.Kernel: s.MemInfo
rm.Funai.Kernel: s.vdu.VduDriver
rm.Funai.Kernel: s.vdu.VduDecl
rm.Funai.Kernel: s.vdu.VduGrafDec
rm.Funai.Kernel: s.vdu.VduModes
rm.Funai.Kernel: s.vdu.VduSWIs
rm.Funai.Kernel: s.vdu.VduPalette
rm.Funai.Kernel: s.Vdu.VduPal20
rm.Funai.Kernel: s.vdu.VduPlot
rm.Funai.Kernel: s.vdu.VduGrafA
rm.Funai.Kernel: s.vdu.VduGrafB
rm.Funai.Kernel: s.vdu.VduGrafC
rm.Funai.Kernel: s.vdu.VduGrafD
rm.Funai.Kernel: s.vdu.VduGrafE
rm.Funai.Kernel: s.vdu.VduGrafF
rm.Funai.Kernel: s.vdu.VduGrafG
rm.Funai.Kernel: s.vdu.VduGrafH
rm.Funai.Kernel: s.vdu.VduGrafI
rm.Funai.Kernel: s.vdu.VduGrafJ
rm.Funai.Kernel: s.vdu.VduGrafK
rm.Funai.Kernel: s.vdu.VduGrafL
rm.Funai.Kernel: s.vdu.VduWrch
rm.Funai.Kernel: s.vdu.Vdu23
rm.Funai.Kernel: s.vdu.VduPointer
rm.Funai.Kernel: s.vdu.Vdu5
rm.Funai.Kernel: s.vdu.VduCurSoft
rm.Funai.Kernel: s.vdu.VduTTX
rm.Funai.Kernel: s.vdu.VduFontL1
rm.Funai.Kernel: s.PMF.osinit
rm.Funai.Kernel: s.PMF.oseven
rm.Funai.Kernel: s.PMF.osbyte
rm.Funai.Kernel: s.PMF.osword
rm.Funai.Kernel: s.PMF.realtime
rm.Funai.Kernel: s.PMF.convdate
rm.Funai.Kernel: s.PMF.i2cutils
rm.Funai.Kernel: s.PMF.oswrch
rm.Funai.Kernel: s.PMF.buffer
rm.Funai.Kernel: s.PMF.key
rm.Funai.Kernel: s.PMF.mouse
rm.Funai.Kernel: s.AMBControl.AMB
rm.Funai.Kernel: s.AMBControl.Options
rm.Funai.Kernel: s.AMBControl.Workspace
rm.Funai.Kernel: s.AMBControl.main
rm.Funai.Kernel: s.AMBControl.allocate
rm.Funai.Kernel: s.AMBControl.deallocate
rm.Funai.Kernel: s.AMBControl.growshrink
rm.Funai.Kernel: s.AMBControl.mapslot
rm.Funai.Kernel: s.AMBControl.mapsome
rm.Funai.Kernel: s.AMBControl.readinfo
rm.Funai.Kernel: s.AMBControl.growp
rm.Funai.Kernel: s.AMBControl.shrinkp
rm.Funai.Kernel: s.AMBControl.service
rm.Funai.Kernel: s.AMBControl.Memory
rm.Funai.Kernel: s.AMBControl.memmap
