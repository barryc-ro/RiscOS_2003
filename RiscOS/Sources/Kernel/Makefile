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
        ${CP} Resources.${LOCALE}.<System>.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

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
rm.Boca.Kernel: Hdr:ListOpts
rm.Boca.Kernel: Hdr:Macros
rm.Boca.Kernel: Hdr:System
rm.Boca.Kernel: Hdr:SWIs
rm.Boca.Kernel: Hdr:CPU.Generic26
rm.Boca.Kernel: Hdr:IO.GenericIO
rm.Boca.Kernel: Hdr:RISCOS
rm.Boca.Kernel: Hdr:Machine.<Machine>
rm.Boca.Kernel: Hdr:Machine.Machine
rm.Boca.Kernel: Hdr:ImageSize.<ImageSize>
rm.Boca.Kernel: Hdr:CPU.ARM600
rm.Boca.Kernel: Hdr:IO.IOMDL
rm.Boca.Kernel: Hdr:IO.IOMD
rm.Boca.Kernel: Hdr:MEMM.ARM600
rm.Boca.Kernel: Hdr:VIDC.VIDC20
rm.Boca.Kernel: Version
rm.Boca.Kernel: Hdr:CMOS
rm.Boca.Kernel: Hdr:Heap
rm.Boca.Kernel: Hdr:PublicWS
rm.Boca.Kernel: Hdr:KernelWS
rm.Boca.Kernel: Hdr:Services
rm.Boca.Kernel: Hdr:FSNumbers
rm.Boca.Kernel: Hdr:HighFSI
rm.Boca.Kernel: Hdr:NewErrors
rm.Boca.Kernel: Hdr:Proc
rm.Boca.Kernel: Hdr:Sprite
rm.Boca.Kernel: Hdr:KeyWS
rm.Boca.Kernel: Hdr:RS423
rm.Boca.Kernel: Hdr:ModHand
rm.Boca.Kernel: Hdr:Variables
rm.Boca.Kernel: Hdr:EnvNumbers
rm.Boca.Kernel: Hdr:UpCall
rm.Boca.Kernel: Hdr:Sound
rm.Boca.Kernel: Hdr:Pointer
rm.Boca.Kernel: Hdr:Podule
rm.Boca.Kernel: Hdr:VduExt
rm.Boca.Kernel: Hdr:Buffer
rm.Boca.Kernel: Hdr:Font
rm.Boca.Kernel: Hdr:DevNos
rm.Boca.Kernel: Hdr:Territory
rm.Boca.Kernel: Hdr:Portable
rm.Boca.Kernel: Hdr:MsgTrans
rm.Boca.Kernel: Hdr:PaletteV
rm.Boca.Kernel: Hdr:Wimp
rm.Boca.Kernel: Hdr:ColourTran
rm.Boca.Kernel: Hdr:Debug
rm.Boca.Kernel: s.PMF.DEF
rm.Boca.Kernel: s.Kernel
rm.Boca.Kernel: TestSrc.Begin
rm.Boca.Kernel: s.Morris
rm.Boca.Kernel: TestSrc.ExtIO
rm.Boca.Kernel: TestSrc.ExtCmd
rm.Boca.Kernel: TestSrc.ROMCard
rm.Boca.Kernel: TestSrc.Mem1IOMD
rm.Boca.Kernel: TestSrc.Mem2
rm.Boca.Kernel: TestSrc.Mem3
rm.Boca.Kernel: TestSrc.Mem4
rm.Boca.Kernel: TestSrc.Mem5
rm.Boca.Kernel: TestSrc.Vidc
rm.Boca.Kernel: TestSrc.Ioc
rm.Boca.Kernel: TestSrc.Cmos
rm.Boca.Kernel: TestSrc.Arm3
rm.Boca.Kernel: TestSrc.ToggleLED
rm.Boca.Kernel: TestSrc.LEDDelay
rm.Boca.Kernel: TestSrc.ShowIOMDRs
rm.Boca.Kernel: s.NewIRQs
rm.Boca.Kernel: s.Oscli
rm.Boca.Kernel: s.SysComms
rm.Boca.Kernel: s.MosDict
rm.Boca.Kernel: s.TMOSHelp
rm.Boca.Kernel: s.HeapMan
rm.Boca.Kernel: s.ModHand
rm.Boca.Kernel: s.Unsqueeze
rm.Boca.Kernel: s.ArthurSWIs
rm.Boca.Kernel: s.ChangeDyn
rm.Boca.Kernel: s.Arthur2
rm.Boca.Kernel: s.Utility
rm.Boca.Kernel: s.MoreComms
rm.Boca.Kernel: s.Convrsions
rm.Boca.Kernel: s.MoreSWIs
rm.Boca.Kernel: s.ExtraSWIs
rm.Boca.Kernel: s.HeapSort
rm.Boca.Kernel: s.Arthur3
rm.Boca.Kernel: s.SWINaming
rm.Boca.Kernel: s.TickEvents
rm.Boca.Kernel: s.KbdResPC
rm.Boca.Kernel: s.NewReset
rm.Boca.Kernel: s.MsgCode
rm.Boca.Kernel: s.Middle
rm.Boca.Kernel: s.Super1
rm.Boca.Kernel: s.ARM600
rm.Boca.Kernel: s.MemInfo
rm.Boca.Kernel: s.vdu.VduDriver
rm.Boca.Kernel: s.vdu.VduDecl
rm.Boca.Kernel: s.vdu.VduGrafDec
rm.Boca.Kernel: s.vdu.VduModes
rm.Boca.Kernel: s.vdu.VduSWIs
rm.Boca.Kernel: s.vdu.VduPalette
rm.Boca.Kernel: s.Vdu.VduPal20
rm.Boca.Kernel: s.vdu.VduPlot
rm.Boca.Kernel: s.vdu.VduGrafA
rm.Boca.Kernel: s.vdu.VduGrafB
rm.Boca.Kernel: s.vdu.VduGrafC
rm.Boca.Kernel: s.vdu.VduGrafD
rm.Boca.Kernel: s.vdu.VduGrafE
rm.Boca.Kernel: s.vdu.VduGrafF
rm.Boca.Kernel: s.vdu.VduGrafG
rm.Boca.Kernel: s.vdu.VduGrafH
rm.Boca.Kernel: s.vdu.VduGrafI
rm.Boca.Kernel: s.vdu.VduGrafJ
rm.Boca.Kernel: s.vdu.VduGrafK
rm.Boca.Kernel: s.vdu.VduGrafL
rm.Boca.Kernel: s.vdu.VduWrch
rm.Boca.Kernel: s.vdu.Vdu23
rm.Boca.Kernel: s.vdu.VduPointer
rm.Boca.Kernel: s.vdu.Vdu5
rm.Boca.Kernel: s.vdu.VduCurSoft
rm.Boca.Kernel: s.vdu.VduTTX
rm.Boca.Kernel: s.vdu.VduFontL1
rm.Boca.Kernel: s.PMF.osinit
rm.Boca.Kernel: s.PMF.oseven
rm.Boca.Kernel: s.PMF.osbyte
rm.Boca.Kernel: s.PMF.osword
rm.Boca.Kernel: s.PMF.realtime
rm.Boca.Kernel: s.PMF.convdate
rm.Boca.Kernel: s.PMF.i2cutils
rm.Boca.Kernel: s.PMF.oswrch
rm.Boca.Kernel: s.PMF.buffer
rm.Boca.Kernel: s.PMF.key
rm.Boca.Kernel: s.PMF.mouse
