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

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

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
rm.Falcon.Kernel: Hdr:ListOpts
rm.Falcon.Kernel: Hdr:Macros
rm.Falcon.Kernel: Hdr:System
rm.Falcon.Kernel: Hdr:SWIs
rm.Falcon.Kernel: Hdr:CPU.Generic26
rm.Falcon.Kernel: Hdr:IO.GenericIO
rm.Falcon.Kernel: Hdr:RISCOS
rm.Falcon.Kernel: Hdr:Machine.<Machine>
rm.Falcon.Kernel: Hdr:ImageSize.<ImageSize>
rm.Falcon.Kernel: Hdr:CPU.ARM600
rm.Falcon.Kernel: Hdr:IO.IOMDL
rm.Falcon.Kernel: Hdr:IO.IOMD
rm.Falcon.Kernel: Hdr:MEMM.ARM600
rm.Falcon.Kernel: Hdr:VIDC.VIDC20
rm.Falcon.Kernel: Version
rm.Falcon.Kernel: Hdr:CMOS
rm.Falcon.Kernel: Hdr:Heap
rm.Falcon.Kernel: Hdr:PublicWS
rm.Falcon.Kernel: Hdr:KernelWS
rm.Falcon.Kernel: Hdr:Services
rm.Falcon.Kernel: Hdr:FSNumbers
rm.Falcon.Kernel: Hdr:HighFSI
rm.Falcon.Kernel: Hdr:NewErrors
rm.Falcon.Kernel: Hdr:Proc
rm.Falcon.Kernel: Hdr:Sprite
rm.Falcon.Kernel: Hdr:KeyWS
rm.Falcon.Kernel: Hdr:RS423
rm.Falcon.Kernel: Hdr:ModHand
rm.Falcon.Kernel: Hdr:Variables
rm.Falcon.Kernel: Hdr:EnvNumbers
rm.Falcon.Kernel: Hdr:UpCall
rm.Falcon.Kernel: Hdr:Sound
rm.Falcon.Kernel: Hdr:Pointer
rm.Falcon.Kernel: Hdr:Podule
rm.Falcon.Kernel: Hdr:VduExt
rm.Falcon.Kernel: Hdr:Buffer
rm.Falcon.Kernel: Hdr:Font
rm.Falcon.Kernel: Hdr:DevNos
rm.Falcon.Kernel: Hdr:Territory
rm.Falcon.Kernel: Hdr:Portable
rm.Falcon.Kernel: Hdr:MsgTrans
rm.Falcon.Kernel: Hdr:PaletteV
rm.Falcon.Kernel: Hdr:Wimp
rm.Falcon.Kernel: Hdr:ColourTran
rm.Falcon.Kernel: Hdr:Debug
rm.Falcon.Kernel: s.PMF.DEF
rm.Falcon.Kernel: s.Copro15ops
rm.Falcon.Kernel: s.Kernel
rm.Falcon.Kernel: TestSrc.Begin
rm.Falcon.Kernel: s.Morris
rm.Falcon.Kernel: TestSrc.ExtIO
rm.Falcon.Kernel: TestSrc.ExtCmd
rm.Falcon.Kernel: TestSrc.Mem1IOMD
rm.Falcon.Kernel: TestSrc.Mem2
rm.Falcon.Kernel: TestSrc.Mem3
rm.Falcon.Kernel: TestSrc.Mem4
rm.Falcon.Kernel: TestSrc.Mem5
rm.Falcon.Kernel: TestSrc.Vidc
rm.Falcon.Kernel: TestSrc.Ioc
rm.Falcon.Kernel: TestSrc.Cmos
rm.Falcon.Kernel: TestSrc.Arm3
rm.Falcon.Kernel: TestSrc.ToggleLED
rm.Falcon.Kernel: TestSrc.LEDDelay
rm.Falcon.Kernel: TestSrc.ShowIOMDRs
rm.Falcon.Kernel: s.NewIRQs
rm.Falcon.Kernel: s.Oscli
rm.Falcon.Kernel: s.SysComms
rm.Falcon.Kernel: s.MosDict
rm.Falcon.Kernel: s.TMOSHelp
rm.Falcon.Kernel: s.HeapMan
rm.Falcon.Kernel: s.ModHand
rm.Falcon.Kernel: s.Unsqueeze
rm.Falcon.Kernel: s.ArthurSWIs
rm.Falcon.Kernel: s.ChangeDyn
rm.Falcon.Kernel: s.Arthur2
rm.Falcon.Kernel: s.Utility
rm.Falcon.Kernel: s.MoreComms
rm.Falcon.Kernel: s.Convrsions
rm.Falcon.Kernel: s.MoreSWIs
rm.Falcon.Kernel: s.ExtraSWIs
rm.Falcon.Kernel: s.HeapSort
rm.Falcon.Kernel: s.Arthur3
rm.Falcon.Kernel: s.SWINaming
rm.Falcon.Kernel: s.TickEvents
rm.Falcon.Kernel: s.KbdResPC
rm.Falcon.Kernel: s.NewReset
rm.Falcon.Kernel: s.MsgCode
rm.Falcon.Kernel: s.Middle
rm.Falcon.Kernel: s.Super1
rm.Falcon.Kernel: s.ARM600
rm.Falcon.Kernel: s.MemInfo
rm.Falcon.Kernel: s.vdu.VduDriver
rm.Falcon.Kernel: s.vdu.VduDecl
rm.Falcon.Kernel: s.vdu.VduGrafDec
rm.Falcon.Kernel: s.vdu.VduModes
rm.Falcon.Kernel: s.vdu.VduSWIs
rm.Falcon.Kernel: s.vdu.VduPalette
rm.Falcon.Kernel: s.Vdu.VduPal20
rm.Falcon.Kernel: s.vdu.VduPlot
rm.Falcon.Kernel: s.vdu.VduGrafA
rm.Falcon.Kernel: s.vdu.VduGrafB
rm.Falcon.Kernel: s.vdu.VduGrafC
rm.Falcon.Kernel: s.vdu.VduGrafD
rm.Falcon.Kernel: s.vdu.VduGrafE
rm.Falcon.Kernel: s.vdu.VduGrafF
rm.Falcon.Kernel: s.vdu.VduGrafG
rm.Falcon.Kernel: s.vdu.VduGrafH
rm.Falcon.Kernel: s.vdu.VduGrafI
rm.Falcon.Kernel: s.vdu.VduGrafJ
rm.Falcon.Kernel: s.vdu.VduGrafK
rm.Falcon.Kernel: s.vdu.VduGrafL
rm.Falcon.Kernel: s.vdu.VduWrch
rm.Falcon.Kernel: s.vdu.Vdu23
rm.Falcon.Kernel: s.vdu.VduPointer
rm.Falcon.Kernel: s.vdu.Vdu5
rm.Falcon.Kernel: s.vdu.VduCurSoft
rm.Falcon.Kernel: s.vdu.VduTTX
rm.Falcon.Kernel: s.vdu.VduFontL1
rm.Falcon.Kernel: s.PMF.osinit
rm.Falcon.Kernel: s.PMF.oseven
rm.Falcon.Kernel: s.PMF.osbyte
rm.Falcon.Kernel: s.PMF.osword
rm.Falcon.Kernel: s.PMF.realtime
rm.Falcon.Kernel: s.PMF.convdate
rm.Falcon.Kernel: s.PMF.i2cutils
rm.Falcon.Kernel: s.PMF.oswrch
rm.Falcon.Kernel: s.PMF.buffer
rm.Falcon.Kernel: s.PMF.key
rm.Falcon.Kernel: s.PMF.mouse
rm.Falcon.Kernel: s.AMBControl.AMB
rm.Falcon.Kernel: s.AMBControl.Options
rm.Falcon.Kernel: s.AMBControl.Workspace
rm.Falcon.Kernel: s.AMBControl.main
rm.Falcon.Kernel: s.AMBControl.allocate
rm.Falcon.Kernel: s.AMBControl.deallocate
rm.Falcon.Kernel: s.AMBControl.growshrink
rm.Falcon.Kernel: s.AMBControl.mapslot
rm.Falcon.Kernel: s.AMBControl.mapsome
rm.Falcon.Kernel: s.AMBControl.readinfo
rm.Falcon.Kernel: s.AMBControl.growp
rm.Falcon.Kernel: s.AMBControl.shrinkp
rm.Falcon.Kernel: s.AMBControl.service
rm.Falcon.Kernel: s.AMBControl.Memory
rm.Falcon.Kernel: s.AMBControl.memmap
rm.Omega.Kernel: Hdr:ListOpts
rm.Omega.Kernel: Hdr:Macros
rm.Omega.Kernel: Hdr:System
rm.Omega.Kernel: Hdr:SWIs
rm.Omega.Kernel: Hdr:CPU.Generic26
rm.Omega.Kernel: Hdr:IO.GenericIO
rm.Omega.Kernel: Hdr:RISCOS
rm.Omega.Kernel: Hdr:Machine.<Machine>
rm.Omega.Kernel: Hdr:ImageSize.<ImageSize>
rm.Omega.Kernel: Hdr:CPU.ARM600
rm.Omega.Kernel: Hdr:IO.IOMDL
rm.Omega.Kernel: Hdr:IO.IOMD
rm.Omega.Kernel: Hdr:MEMM.ARM600
rm.Omega.Kernel: Hdr:VIDC.VIDC20
rm.Omega.Kernel: Version
rm.Omega.Kernel: Hdr:CMOS
rm.Omega.Kernel: Hdr:Heap
rm.Omega.Kernel: Hdr:PublicWS
rm.Omega.Kernel: Hdr:KernelWS
rm.Omega.Kernel: Hdr:Services
rm.Omega.Kernel: Hdr:FSNumbers
rm.Omega.Kernel: Hdr:HighFSI
rm.Omega.Kernel: Hdr:NewErrors
rm.Omega.Kernel: Hdr:Proc
rm.Omega.Kernel: Hdr:Sprite
rm.Omega.Kernel: Hdr:KeyWS
rm.Omega.Kernel: Hdr:RS423
rm.Omega.Kernel: Hdr:ModHand
rm.Omega.Kernel: Hdr:Variables
rm.Omega.Kernel: Hdr:EnvNumbers
rm.Omega.Kernel: Hdr:UpCall
rm.Omega.Kernel: Hdr:Sound
rm.Omega.Kernel: Hdr:Pointer
rm.Omega.Kernel: Hdr:Podule
rm.Omega.Kernel: Hdr:VduExt
rm.Omega.Kernel: Hdr:Buffer
rm.Omega.Kernel: Hdr:Font
rm.Omega.Kernel: Hdr:DevNos
rm.Omega.Kernel: Hdr:Territory
rm.Omega.Kernel: Hdr:Portable
rm.Omega.Kernel: Hdr:MsgTrans
rm.Omega.Kernel: Hdr:PaletteV
rm.Omega.Kernel: Hdr:Wimp
rm.Omega.Kernel: Hdr:ColourTran
rm.Omega.Kernel: Hdr:Debug
rm.Omega.Kernel: s.PMF.DEF
rm.Omega.Kernel: s.Copro15ops
rm.Omega.Kernel: s.Kernel
rm.Omega.Kernel: s.Morris
rm.Omega.Kernel: s.NewIRQs
rm.Omega.Kernel: s.Oscli
rm.Omega.Kernel: s.SysComms
rm.Omega.Kernel: s.MosDict
rm.Omega.Kernel: s.TMOSHelp
rm.Omega.Kernel: s.HeapMan
rm.Omega.Kernel: s.ModHand
rm.Omega.Kernel: s.Unsqueeze
rm.Omega.Kernel: s.ArthurSWIs
rm.Omega.Kernel: s.ChangeDyn
rm.Omega.Kernel: s.Arthur2
rm.Omega.Kernel: s.Utility
rm.Omega.Kernel: s.MoreComms
rm.Omega.Kernel: s.Convrsions
rm.Omega.Kernel: s.MoreSWIs
rm.Omega.Kernel: s.ExtraSWIs
rm.Omega.Kernel: s.HeapSort
rm.Omega.Kernel: s.Arthur3
rm.Omega.Kernel: s.SWINaming
rm.Omega.Kernel: s.TickEvents
rm.Omega.Kernel: s.KbdResPC
rm.Omega.Kernel: s.NewReset
rm.Omega.Kernel: s.MsgCode
rm.Omega.Kernel: s.Middle
rm.Omega.Kernel: s.Super1
rm.Omega.Kernel: s.ARM600
rm.Omega.Kernel: s.MemInfo
rm.Omega.Kernel: s.vdu.VduDriver
rm.Omega.Kernel: s.vdu.VduDecl
rm.Omega.Kernel: s.vdu.VduGrafDec
rm.Omega.Kernel: s.vdu.VduModes
rm.Omega.Kernel: s.vdu.VduSWIs
rm.Omega.Kernel: s.vdu.VduPalette
rm.Omega.Kernel: s.Vdu.VduPal20
rm.Omega.Kernel: s.vdu.VduPlot
rm.Omega.Kernel: s.vdu.VduGrafA
rm.Omega.Kernel: s.vdu.VduGrafB
rm.Omega.Kernel: s.vdu.VduGrafC
rm.Omega.Kernel: s.vdu.VduGrafD
rm.Omega.Kernel: s.vdu.VduGrafE
rm.Omega.Kernel: s.vdu.VduGrafF
rm.Omega.Kernel: s.vdu.VduGrafG
rm.Omega.Kernel: s.vdu.VduGrafH
rm.Omega.Kernel: s.vdu.VduGrafI
rm.Omega.Kernel: s.vdu.VduGrafJ
rm.Omega.Kernel: s.vdu.VduGrafK
rm.Omega.Kernel: s.vdu.VduGrafL
rm.Omega.Kernel: s.vdu.VduWrch
rm.Omega.Kernel: s.vdu.Vdu23
rm.Omega.Kernel: s.vdu.VduPointer
rm.Omega.Kernel: s.vdu.Vdu5
rm.Omega.Kernel: s.vdu.VduCurSoft
rm.Omega.Kernel: s.vdu.VduTTX
rm.Omega.Kernel: s.vdu.VduFontL1
rm.Omega.Kernel: s.PMF.osinit
rm.Omega.Kernel: s.PMF.oseven
rm.Omega.Kernel: s.PMF.osbyte
rm.Omega.Kernel: s.PMF.osword
rm.Omega.Kernel: s.PMF.realtime
rm.Omega.Kernel: s.PMF.convdate
rm.Omega.Kernel: s.PMF.i2cutils
rm.Omega.Kernel: s.PMF.oswrch
rm.Omega.Kernel: s.PMF.buffer
rm.Omega.Kernel: s.PMF.key
rm.Omega.Kernel: s.PMF.mouse
rm.Omega.Kernel: s.AMBControl.AMB
rm.Omega.Kernel: s.AMBControl.Options
rm.Omega.Kernel: s.AMBControl.Workspace
rm.Omega.Kernel: s.AMBControl.main
rm.Omega.Kernel: s.AMBControl.allocate
rm.Omega.Kernel: s.AMBControl.deallocate
rm.Omega.Kernel: s.AMBControl.growshrink
rm.Omega.Kernel: s.AMBControl.mapslot
rm.Omega.Kernel: s.AMBControl.mapsome
rm.Omega.Kernel: s.AMBControl.readinfo
rm.Omega.Kernel: s.AMBControl.growp
rm.Omega.Kernel: s.AMBControl.shrinkp
rm.Omega.Kernel: s.AMBControl.service
rm.Omega.Kernel: s.AMBControl.Memory
rm.Omega.Kernel: s.AMBControl.memmap
rm.Morris.Kernel: Hdr:ListOpts
rm.Morris.Kernel: Hdr:Macros
rm.Morris.Kernel: Hdr:System
rm.Morris.Kernel: Hdr:SWIs
rm.Morris.Kernel: Hdr:CPU.Generic26
rm.Morris.Kernel: Hdr:IO.GenericIO
rm.Morris.Kernel: Hdr:RISCOS
rm.Morris.Kernel: Hdr:Machine.<Machine>
rm.Morris.Kernel: Hdr:ImageSize.<ImageSize>
rm.Morris.Kernel: Hdr:CPU.ARM600
rm.Morris.Kernel: Hdr:IO.IOMDL
rm.Morris.Kernel: Hdr:IO.IOMD
rm.Morris.Kernel: Hdr:MEMM.ARM600
rm.Morris.Kernel: Hdr:VIDC.VIDC20
rm.Morris.Kernel: Version
rm.Morris.Kernel: Hdr:CMOS
rm.Morris.Kernel: Hdr:Heap
rm.Morris.Kernel: Hdr:PublicWS
rm.Morris.Kernel: Hdr:KernelWS
rm.Morris.Kernel: Hdr:Services
rm.Morris.Kernel: Hdr:FSNumbers
rm.Morris.Kernel: Hdr:HighFSI
rm.Morris.Kernel: Hdr:NewErrors
rm.Morris.Kernel: Hdr:Proc
rm.Morris.Kernel: Hdr:Sprite
rm.Morris.Kernel: Hdr:KeyWS
rm.Morris.Kernel: Hdr:RS423
rm.Morris.Kernel: Hdr:ModHand
rm.Morris.Kernel: Hdr:Variables
rm.Morris.Kernel: Hdr:EnvNumbers
rm.Morris.Kernel: Hdr:UpCall
rm.Morris.Kernel: Hdr:Sound
rm.Morris.Kernel: Hdr:Pointer
rm.Morris.Kernel: Hdr:Podule
rm.Morris.Kernel: Hdr:VduExt
rm.Morris.Kernel: Hdr:Buffer
rm.Morris.Kernel: Hdr:Font
rm.Morris.Kernel: Hdr:DevNos
rm.Morris.Kernel: Hdr:Territory
rm.Morris.Kernel: Hdr:Portable
rm.Morris.Kernel: Hdr:MsgTrans
rm.Morris.Kernel: Hdr:PaletteV
rm.Morris.Kernel: Hdr:Wimp
rm.Morris.Kernel: Hdr:ColourTran
rm.Morris.Kernel: Hdr:Debug
rm.Morris.Kernel: s.PMF.DEF
rm.Morris.Kernel: s.Copro15ops
rm.Morris.Kernel: s.Kernel
rm.Morris.Kernel: s.Morris
rm.Morris.Kernel: s.NewIRQs
rm.Morris.Kernel: s.Oscli
rm.Morris.Kernel: s.SysComms
rm.Morris.Kernel: s.MosDict
rm.Morris.Kernel: s.TMOSHelp
rm.Morris.Kernel: s.HeapMan
rm.Morris.Kernel: s.ModHand
rm.Morris.Kernel: s.Unsqueeze
rm.Morris.Kernel: s.ArthurSWIs
rm.Morris.Kernel: s.ChangeDyn
rm.Morris.Kernel: s.Arthur2
rm.Morris.Kernel: s.Utility
rm.Morris.Kernel: s.MoreComms
rm.Morris.Kernel: s.Convrsions
rm.Morris.Kernel: s.MoreSWIs
rm.Morris.Kernel: s.ExtraSWIs
rm.Morris.Kernel: s.HeapSort
rm.Morris.Kernel: s.Arthur3
rm.Morris.Kernel: s.SWINaming
rm.Morris.Kernel: s.TickEvents
rm.Morris.Kernel: s.KbdResPC
rm.Morris.Kernel: s.NewReset
rm.Morris.Kernel: s.MsgCode
rm.Morris.Kernel: s.Middle
rm.Morris.Kernel: s.Super1
rm.Morris.Kernel: s.ARM600
rm.Morris.Kernel: s.MemInfo
rm.Morris.Kernel: s.vdu.VduDriver
rm.Morris.Kernel: s.vdu.VduDecl
rm.Morris.Kernel: s.vdu.VduGrafDec
rm.Morris.Kernel: s.vdu.VduModes
rm.Morris.Kernel: s.vdu.VduSWIs
rm.Morris.Kernel: s.vdu.VduPalette
rm.Morris.Kernel: s.Vdu.VduPal20
rm.Morris.Kernel: s.vdu.VduPlot
rm.Morris.Kernel: s.vdu.VduGrafA
rm.Morris.Kernel: s.vdu.VduGrafB
rm.Morris.Kernel: s.vdu.VduGrafC
rm.Morris.Kernel: s.vdu.VduGrafD
rm.Morris.Kernel: s.vdu.VduGrafE
rm.Morris.Kernel: s.vdu.VduGrafF
rm.Morris.Kernel: s.vdu.VduGrafG
rm.Morris.Kernel: s.vdu.VduGrafH
rm.Morris.Kernel: s.vdu.VduGrafI
rm.Morris.Kernel: s.vdu.VduGrafJ
rm.Morris.Kernel: s.vdu.VduGrafK
rm.Morris.Kernel: s.vdu.VduGrafL
rm.Morris.Kernel: s.vdu.VduWrch
rm.Morris.Kernel: s.vdu.Vdu23
rm.Morris.Kernel: s.vdu.VduPointer
rm.Morris.Kernel: s.vdu.Vdu5
rm.Morris.Kernel: s.vdu.VduCurSoft
rm.Morris.Kernel: s.vdu.VduTTX
rm.Morris.Kernel: s.vdu.VduFontL1
rm.Morris.Kernel: s.PMF.osinit
rm.Morris.Kernel: s.PMF.oseven
rm.Morris.Kernel: s.PMF.osbyte
rm.Morris.Kernel: s.PMF.osword
rm.Morris.Kernel: s.PMF.realtime
rm.Morris.Kernel: s.PMF.convdate
rm.Morris.Kernel: s.PMF.i2cutils
rm.Morris.Kernel: s.PMF.oswrch
rm.Morris.Kernel: s.PMF.buffer
rm.Morris.Kernel: s.PMF.key
rm.Morris.Kernel: s.PMF.mouse
rm.Morris.Kernel: s.AMBControl.AMB
rm.Morris.Kernel: s.AMBControl.Options
rm.Morris.Kernel: s.AMBControl.Workspace
rm.Morris.Kernel: s.AMBControl.main
rm.Morris.Kernel: s.AMBControl.allocate
rm.Morris.Kernel: s.AMBControl.deallocate
rm.Morris.Kernel: s.AMBControl.growshrink
rm.Morris.Kernel: s.AMBControl.mapslot
rm.Morris.Kernel: s.AMBControl.mapsome
rm.Morris.Kernel: s.AMBControl.readinfo
rm.Morris.Kernel: s.AMBControl.growp
rm.Morris.Kernel: s.AMBControl.shrinkp
rm.Morris.Kernel: s.AMBControl.service
rm.Morris.Kernel: s.AMBControl.Memory
rm.Morris.Kernel: s.AMBControl.memmap
