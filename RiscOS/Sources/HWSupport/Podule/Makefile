# Makefile for Podule {Expansion cards}
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 28-Jun-94  AMcC         Added extra development rules.
# 30-Aug-94  AMcC         Added resources: target
# 16-Jan-96  JRH          Makes different targets for different machines
# 11-Jan-00  PMS          Converted to use objasm instead of aasm so that we
#                         can pass in assembly-time constants to control
#                         fake podule header for Funai 5 Ethernet NC
#                         which doesn't have a podule ROM. Paul Skirrow.
#
# Paths
#
EXP_HDR = <export$dir>
EXP_CHDR = <CExport$Dir>.h

#
# Generic options:
#

MKDIR   = cdir
AS      = objasm
CP      = copy
MODGEN   = modgen
LD      = link
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -PreDefine ${OPTIONS} -To $@ -From
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Podule
SOURCE    = Commands.s Errors.s GetAll.s HelpTokens.s Interface.s Module.s MsgCode.s ROMExtend.s
TARGET    = rm.${MACHINE}.Podule
ROMTARGET = Podule.o
EXPORTS   = ${EXP_HDR}.${COMPONENT} \
            ${EXP_CHDR}.${COMPONENT}

MESSAGESMOD = rm.PoduleMsgs
MESSAGES  = LocalRes:Messages
RESPATH   = Resources.Podule.Messages
MODNAME   = PoduleManagerMessages
MODHELP   = PoduleManagerMessages

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

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} ${MESSAGES} ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

clean:
        ${RM} ${TARGET}
        ${RM} ${MESSAGESMOD}
        ${RM} s.HelpTokens
        ${RM} setversion
        @echo ${COMPONENT}: cleaned

s.HelpTokens: ${TOKENS} HelpTexts
        ${TOKENISE} ${TOKENS} HelpTexts $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

${EXP_CHDR}.${COMPONENT}: h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Target 
#
${TARGET}: ${SOURCE}
        ${AS} ${AFLAGS} GetAll.s Podule.o 
        ${MKDIR} rm.${MACHINE}
        ${LD} -o $@ -bin Podule.o
	SetType $@ &FFA

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
	SetType $@ &FFA
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# 
# Generate a Messages module
# Note: need to set alias to ensure that <Version> is expanded
#
msgmod: ${MESSAGESMOD}
        @echo ${COMPONENT}: messages module built

${MESSAGESMOD}: ${MESSAGES} setversion
        setversion
        set alias$modgen_ver ${MODGEN} $@ ${MODNAME} ${MODHELP} <Version> ${MESSAGES} ${RESPATH}
        modgen_ver
        unset alias$modgen
        unset Version

setversion: Time+Date
        ${AS} ${ASFLAGS} utils.getversion
        settype setversion obey

#
# Extra Development rules:
#
install: ${TARGET}
        Access ${TARGET} R/r
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ${TARGET} installed

maketime:
        MakeTime

version:
        Version



# Dynamic dependencies:
