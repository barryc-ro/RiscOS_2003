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
rm.Phoebe.Kernel: Hdr:ListOpts
rm.Phoebe.Kernel: Hdr:Macros
rm.Phoebe.Kernel: Hdr:System
rm.Phoebe.Kernel: Hdr:SWIs
rm.Phoebe.Kernel: Hdr:CPU.Generic26
rm.Phoebe.Kernel: Hdr:IO.GenericIO
rm.Phoebe.Kernel: Hdr:RISCOS
rm.Phoebe.Kernel: Hdr:Machine.<Machine>
rm.Phoebe.Kernel: Hdr:ImageSize.<ImageSize>
rm.Phoebe.Kernel: Hdr:CPU.ARM600
rm.Phoebe.Kernel: Hdr:IO.IOMDL
rm.Phoebe.Kernel: Hdr:IO.IOMD
rm.Phoebe.Kernel: Hdr:MEMM.ARM600
rm.Phoebe.Kernel: Hdr:VIDC.VIDC20
rm.Phoebe.Kernel: Version
rm.Phoebe.Kernel: hdr:FileCore
rm.Phoebe.Kernel: hdr:RAMFS
rm.Phoebe.Kernel: Hdr:CMOS
rm.Phoebe.Kernel: Hdr:Heap
rm.Phoebe.Kernel: Hdr:PublicWS
rm.Phoebe.Kernel: Hdr:KernelWS
rm.Phoebe.Kernel: Hdr:Services
rm.Phoebe.Kernel: Hdr:FSNumbers
rm.Phoebe.Kernel: Hdr:HighFSI
rm.Phoebe.Kernel: Hdr:NewErrors
rm.Phoebe.Kernel: Hdr:Proc
rm.Phoebe.Kernel: Hdr:Sprite
rm.Phoebe.Kernel: Hdr:KeyWS
rm.Phoebe.Kernel: Hdr:RS423
rm.Phoebe.Kernel: Hdr:ModHand
rm.Phoebe.Kernel: Hdr:Variables
rm.Phoebe.Kernel: Hdr:EnvNumbers
rm.Phoebe.Kernel: Hdr:UpCall
rm.Phoebe.Kernel: Hdr:Sound
rm.Phoebe.Kernel: Hdr:Pointer
rm.Phoebe.Kernel: Hdr:Podule
rm.Phoebe.Kernel: Hdr:VduExt
rm.Phoebe.Kernel: Hdr:Buffer
rm.Phoebe.Kernel: Hdr:Font
rm.Phoebe.Kernel: Hdr:DevNos
rm.Phoebe.Kernel: Hdr:Territory
rm.Phoebe.Kernel: Hdr:Portable
rm.Phoebe.Kernel: Hdr:MsgTrans
rm.Phoebe.Kernel: Hdr:PaletteV
rm.Phoebe.Kernel: Hdr:Wimp
rm.Phoebe.Kernel: Hdr:ColourTran
rm.Phoebe.Kernel: Hdr:Debug
rm.Phoebe.Kernel: s.PMF.DEF
rm.Phoebe.Kernel: s.Copro15ops
rm.Phoebe.Kernel: s.Kernel
rm.Phoebe.Kernel: s.Morris
rm.Phoebe.Kernel: s.NewIRQs
rm.Phoebe.Kernel: s.Oscli
rm.Phoebe.Kernel: s.SysComms
rm.Phoebe.Kernel: s.MosDict
rm.Phoebe.Kernel: s.TMOSHelp
rm.Phoebe.Kernel: s.HeapMan
rm.Phoebe.Kernel: s.ModHand
rm.Phoebe.Kernel: s.Unsqueeze
rm.Phoebe.Kernel: s.ArthurSWIs
rm.Phoebe.Kernel: s.ChangeDyn
rm.Phoebe.Kernel: s.Arthur2
rm.Phoebe.Kernel: s.Utility
rm.Phoebe.Kernel: s.MoreComms
rm.Phoebe.Kernel: s.Convrsions
rm.Phoebe.Kernel: s.MoreSWIs
rm.Phoebe.Kernel: s.ExtraSWIs
rm.Phoebe.Kernel: s.HeapSort
rm.Phoebe.Kernel: s.Arthur3
rm.Phoebe.Kernel: s.SWINaming
rm.Phoebe.Kernel: s.TickEvents
rm.Phoebe.Kernel: s.KbdResPC
rm.Phoebe.Kernel: s.NewReset
rm.Phoebe.Kernel: s.MsgCode
rm.Phoebe.Kernel: s.Middle
rm.Phoebe.Kernel: s.Super1
rm.Phoebe.Kernel: s.ARM600
rm.Phoebe.Kernel: s.MemInfo
rm.Phoebe.Kernel: s.vdu.VduDriver
rm.Phoebe.Kernel: s.vdu.VduDecl
rm.Phoebe.Kernel: s.vdu.VduGrafDec
rm.Phoebe.Kernel: s.vdu.VduModes
rm.Phoebe.Kernel: s.vdu.VduSWIs
rm.Phoebe.Kernel: s.vdu.VduPalette
rm.Phoebe.Kernel: s.Vdu.VduPal20
rm.Phoebe.Kernel: s.vdu.VduPlot
rm.Phoebe.Kernel: s.vdu.VduGrafA
rm.Phoebe.Kernel: s.vdu.VduGrafB
rm.Phoebe.Kernel: s.vdu.VduGrafC
rm.Phoebe.Kernel: s.vdu.VduGrafD
rm.Phoebe.Kernel: s.vdu.VduGrafE
rm.Phoebe.Kernel: s.vdu.VduGrafF
rm.Phoebe.Kernel: s.vdu.VduGrafG
rm.Phoebe.Kernel: s.vdu.VduGrafH
rm.Phoebe.Kernel: s.vdu.VduGrafI
rm.Phoebe.Kernel: s.vdu.VduGrafJ
rm.Phoebe.Kernel: s.vdu.VduGrafK
rm.Phoebe.Kernel: s.vdu.VduGrafL
rm.Phoebe.Kernel: s.vdu.VduWrch
rm.Phoebe.Kernel: s.vdu.Vdu23
rm.Phoebe.Kernel: s.vdu.VduPointer
rm.Phoebe.Kernel: s.vdu.Vdu5
rm.Phoebe.Kernel: s.vdu.VduCurSoft
rm.Phoebe.Kernel: s.vdu.VduTTX
rm.Phoebe.Kernel: s.vdu.VduFontL1
rm.Phoebe.Kernel: s.PMF.osinit
rm.Phoebe.Kernel: s.PMF.oseven
rm.Phoebe.Kernel: s.PMF.osbyte
rm.Phoebe.Kernel: s.PMF.osword
rm.Phoebe.Kernel: s.PMF.realtime
rm.Phoebe.Kernel: s.PMF.convdate
rm.Phoebe.Kernel: s.PMF.i2cutils
rm.Phoebe.Kernel: s.PMF.oswrch
rm.Phoebe.Kernel: s.PMF.buffer
rm.Phoebe.Kernel: s.PMF.key
rm.Phoebe.Kernel: s.PMF.mouse
rm.Phoebe.Kernel: s.AMBControl.AMB
rm.Phoebe.Kernel: s.AMBControl.Options
rm.Phoebe.Kernel: s.AMBControl.Workspace
rm.Phoebe.Kernel: s.AMBControl.main
rm.Phoebe.Kernel: s.AMBControl.allocate
rm.Phoebe.Kernel: s.AMBControl.deallocate
rm.Phoebe.Kernel: s.AMBControl.growshrink
rm.Phoebe.Kernel: s.AMBControl.mapslot
rm.Phoebe.Kernel: s.AMBControl.mapsome
rm.Phoebe.Kernel: s.AMBControl.readinfo
rm.Phoebe.Kernel: s.AMBControl.growp
rm.Phoebe.Kernel: s.AMBControl.shrinkp
rm.Phoebe.Kernel: s.AMBControl.service
rm.Phoebe.Kernel: s.AMBControl.Memory
rm.Phoebe.Kernel: s.AMBControl.memmap
