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
            ${C_EXP_HDR}.RISCOS \
            ${C_EXP_HDR}.HALEntries

#
# Generic rules:
#
.SUFFIXES: .o .s
.s.o:; ${ARMASM} ${ARMASMFLAGS} -o $@ $< -list list.$*

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

${C_EXP_HDR}.HALEntries: hdr.HALEntries
        ${MKDIR} ${C_EXP_HDR}
        perl Build:Hdr2H hdr.HALEntries $@

# Dynamic dependencies:
