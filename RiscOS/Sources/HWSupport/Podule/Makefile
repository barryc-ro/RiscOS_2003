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
MODGEN   = modgen
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = Podule
SOURCE    = s.GetAll
TARGET    = rm.${MACHINE}.Podule
EXPORTS   = ${EXP_HDR}.${COMPONENT}

MESSAGESMOD = rm.PoduleMsgs
MESSAGES  = Resources.${LOCALE}.Messages
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

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} ${MESSAGES} ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

clean:
        ${RM} ${TARGET}
        ${RM} ${MESSAGESMOD}
        ${RM} s.HelpTokens
        ${RM} setversion
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE} s.HelpTokens
        ${MKDIR} rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

s.HelpTokens: ${TOKENS} HelpTexts
        ${TOKENISE} ${TOKENS} HelpTexts $@

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


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
setversion: Time+Date
rm.Falcon.Podule: Hdr:ListOpts
rm.Falcon.Podule: Hdr:Macros
rm.Falcon.Podule: Hdr:System
rm.Falcon.Podule: Hdr:SWIs
rm.Falcon.Podule: Hdr:CPU.Generic26
rm.Falcon.Podule: Hdr:IO.GenericIO
rm.Falcon.Podule: Hdr:RISCOS
rm.Falcon.Podule: Hdr:Machine.<Machine>
rm.Falcon.Podule: Hdr:IO.IOMDL
rm.Falcon.Podule: Hdr:CMOS
rm.Falcon.Podule: Hdr:ModHand
rm.Falcon.Podule: Hdr:Debug
rm.Falcon.Podule: Hdr:Podule
rm.Falcon.Podule: Hdr:FSNumbers
rm.Falcon.Podule: Hdr:HighFSI
rm.Falcon.Podule: Hdr:NewErrors
rm.Falcon.Podule: Hdr:Services
rm.Falcon.Podule: Hdr:Tokens
rm.Falcon.Podule: Hdr:Proc
rm.Falcon.Podule: Hdr:Symbols
rm.Falcon.Podule: Hdr:MsgTrans
rm.Falcon.Podule: Hdr:PoduleReg
rm.Falcon.Podule: Time+Date
rm.Falcon.Podule: s.Module
rm.Falcon.Podule: s.Interface
rm.Falcon.Podule: s.Commands
rm.Falcon.Podule: s.Errors
rm.Falcon.Podule: s.ROMExtend
rm.Falcon.Podule: HelpTexts
rm.Falcon.Podule: s.MsgCode
rm.Omega.Podule: Hdr:ListOpts
rm.Omega.Podule: Hdr:Macros
rm.Omega.Podule: Hdr:System
rm.Omega.Podule: Hdr:SWIs
rm.Omega.Podule: Hdr:CPU.Generic26
rm.Omega.Podule: Hdr:IO.GenericIO
rm.Omega.Podule: Hdr:RISCOS
rm.Omega.Podule: Hdr:Machine.<Machine>
rm.Omega.Podule: Hdr:IO.IOMDL
rm.Omega.Podule: Hdr:CMOS
rm.Omega.Podule: Hdr:ModHand
rm.Omega.Podule: Hdr:Debug
rm.Omega.Podule: Hdr:Podule
rm.Omega.Podule: Hdr:FSNumbers
rm.Omega.Podule: Hdr:HighFSI
rm.Omega.Podule: Hdr:NewErrors
rm.Omega.Podule: Hdr:Services
rm.Omega.Podule: Hdr:Tokens
rm.Omega.Podule: Hdr:Proc
rm.Omega.Podule: Hdr:Symbols
rm.Omega.Podule: Hdr:MsgTrans
rm.Omega.Podule: Hdr:PoduleReg
rm.Omega.Podule: Time+Date
rm.Omega.Podule: s.Module
rm.Omega.Podule: s.Interface
rm.Omega.Podule: s.Commands
rm.Omega.Podule: s.Errors
rm.Omega.Podule: s.ROMExtend
rm.Omega.Podule: HelpTexts
rm.Omega.Podule: s.MsgCode
rm.Peregrine.Podule: Hdr:ListOpts
rm.Peregrine.Podule: Hdr:Macros
rm.Peregrine.Podule: Hdr:System
rm.Peregrine.Podule: Hdr:SWIs
rm.Peregrine.Podule: Hdr:CPU.Generic26
rm.Peregrine.Podule: Hdr:IO.GenericIO
rm.Peregrine.Podule: Hdr:RISCOS
rm.Peregrine.Podule: Hdr:Machine.<Machine>
rm.Peregrine.Podule: Hdr:Machine.<Machine>
rm.Peregrine.Podule: Hdr:IO.IOMDL
rm.Peregrine.Podule: Hdr:CMOS
rm.Peregrine.Podule: Hdr:ModHand
rm.Peregrine.Podule: Hdr:Debug
rm.Peregrine.Podule: Hdr:Podule
rm.Peregrine.Podule: Hdr:FSNumbers
rm.Peregrine.Podule: Hdr:HighFSI
rm.Peregrine.Podule: Hdr:NewErrors
rm.Peregrine.Podule: Hdr:Services
rm.Peregrine.Podule: Hdr:Tokens
rm.Peregrine.Podule: Hdr:Proc
rm.Peregrine.Podule: Hdr:Symbols
rm.Peregrine.Podule: Hdr:MsgTrans
rm.Peregrine.Podule: Hdr:PoduleReg
rm.Peregrine.Podule: Time+Date
rm.Peregrine.Podule: s.Module
rm.Peregrine.Podule: s.Interface
rm.Peregrine.Podule: s.Commands
rm.Peregrine.Podule: s.Errors
rm.Peregrine.Podule: s.ROMExtend
rm.Peregrine.Podule: HelpTexts
rm.Peregrine.Podule: s.MsgCode
