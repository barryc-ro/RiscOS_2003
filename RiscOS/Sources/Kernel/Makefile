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
C_EXP_HDR = <cexport$dir>.Global.h

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
ARMASM  = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -To $@ -From
ARMASMFLAGS = -depend !Depend -g ${THROWBACK}
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Kernel
SOURCE    = s.GetAll
TARGET    = rm.Kernel
AIFDBG    = aif.Kernel
GPADBG    = GPA
OBJECTS   = o.GetAll
EXPORTS   = ${EXP_HDR}.EnvNumbers \
            ${EXP_HDR}.ModHand \
            ${EXP_HDR}.PublicWS \
            ${EXP_HDR}.RISCOS \
            ${EXP_HDR}.Variables \
            ${EXP_HDR}.VduExt \
            ${EXP_HDR}.HALEntries \
            ${EXP_HDR}.OSEntries \
            ${C_EXP_HDR}.RISCOS

#
# Generic rules:
#
.SUFFIXES: .o .s
.s.o:; ${ARMASM} ${ARMASMFLAGS} -o $@ $<

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

debug: ${GPADBG}
        @echo ${COMPONENT}: debug image built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} s.TMOSHelp
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${WIPE} aif ${WFLAGS}
        ${RM} ${GPADBG}
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

${TARGET}: ${OBJECTS}
        ${LD} -bin -o ${TARGET} ${OBJECTS}

${AIFDBG}: ${OBJECTS}
        ${MKDIR} aif
        ${LD} -aif -bin -d -o ${AIFDBG} ${OBJECTS}

${GPADBG}: ${AIFDBG}
        ToGPA -s ${AIFDBG} ${GPADBG}

s.TMOSHelp: ${TOKENS} HelpStrs
        ${TOKENISE} ${TOKENS} HelpStrs $@

o.GetAll: s.TMOSHelp

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
        
${EXP_HDR}.HALEntries: hdr.HALEntries
        ${CP} hdr.HALEntries $@ ${CPFLAGS}
        
${EXP_HDR}.OSEntries: hdr.OSEntries
        ${CP} hdr.OSEntries $@ ${CPFLAGS}
        
${C_EXP_HDR}.RISCOS: hdr.RISCOS
        ${MKDIR} ${C_EXP_HDR}
        perl Build:Hdr2H hdr.RISCOS $@

# Dynamic dependencies:
o.GetAll: s.GetAll Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:CPU.ARM600 Hdr:IO.IOMD Hdr:IO.IOMD Hdr:MEMM.ARM600 Hdr:VIDC.VIDC20 Version VersionASM Hdr:CMOS Hdr:Heap Hdr:PublicWS Hdr:KernelWS Hdr:HALEntries Hdr:OSEntries Hdr:Services Hdr:FSNumbers Hdr:HighFSI Hdr:NewErrors Hdr:Proc Hdr:Sprite Hdr:KeyWS Hdr:RS423 Hdr:ModHand Hdr:Variables Hdr:EnvNumbers Hdr:UpCall Hdr:Sound Hdr:Pointer Hdr:Podule Hdr:VduExt Hdr:Buffer Hdr:Font Hdr:DevNos Hdr:Territory Hdr:Portable Hdr:MsgTrans Hdr:PaletteV Hdr:Wimp Hdr:ColourTran Hdr:Debug Hdr:nvram Hdr:PortMan s.PMF.DEF s.Copro15ops s.Kernel s.NewIRQs s.Oscli s.SysComms s.MosDict s.TMOSHelp s.HeapMan s.ModHand s.Unsqueeze s.ArthurSWIs s.ChangeDyn s.HAL s.Arthur2 s.LibKern s.Utility s.MoreComms s.Convrsions s.MoreSWIs s.ExtraSWIs s.HeapSort s.Arthur3 s.SWINaming s.TickEvents s.KbdResPC s.NewReset s.MsgCode s.Middle s.Super1 s.ARM600 s.MemInfo s.vdu.vduhint s.vdu.VduDriver s.vdu.VduDecl s.vdu.VduGrafDec s.vdu.VduModes s.vdu.VduSWIs s.vdu.VduPalette s.vdu.vdupalxx s.vdu.VduPlot s.vdu.VduGrafA s.vdu.VduGrafB s.vdu.VduGrafC s.vdu.VduGrafD s.vdu.VduGrafE s.vdu.VduGrafF s.vdu.VduGrafG s.vdu.VduGrafH s.vdu.VduGrafI s.vdu.VduGrafJ s.vdu.VduGrafK s.vdu.VduGrafL s.vdu.VduWrch s.vdu.Vdu23 s.vdu.VduPointer s.vdu.Vdu5 s.vdu.VduCurSoft s.vdu.VduTTX s.vdu.VduFontL1 s.PMF.osinit s.PMF.oseven s.PMF.osbyte s.PMF.osword s.PMF.realtime s.PMF.convdate s.PMF.i2cutils s.PMF.oswrch s.PMF.buffer s.PMF.key s.PMF.mouse s.AMBControl.AMB s.AMBControl.Options s.AMBControl.Workspace s.AMBControl.main s.AMBControl.allocate s.AMBControl.deallocate s.AMBControl.growshrink s.AMBControl.mapslot s.AMBControl.mapsome s.AMBControl.readinfo s.AMBControl.growp s.AMBControl.shrinkp s.AMBControl.service s.AMBControl.Memory s.AMBControl.memmap
