# Makefile for Cron
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 29-Mar-96 Rich Buckley  Created
# 25-Feb-96 Neil Turton   Stolen and adapted  :-)
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
#TOKENISE=Tokenise
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -Esc -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = Cron
SOURCE     = s.init
SOURCESA   = s.standalone
SOURCESD   = s.standdebug
TARGET     = rm.${COMPONENT}
TARGETSA   = rm.SA${COMPONENT}
TARGETSD   = rm.SA${COMPONENT}D
EXPORTS    = ${EXP_HDR}.${COMPONENT}
TOKENISED  = Resources.${LOCALE}.Tokenised
MESSAGES   = Resources.${LOCALE}.Messages
TOKENFILE  = ${RESDIR}.Resources.Kernel.Dictionary
RESDIR	   = <resource$dir>.Resources.Cron

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

debug: ${TARGETSD}
        @echo ${COMPONENT}: debug module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

resources:
	${MKDIR} ${RESDIR}
	${CP} ${MESSAGES} ${RESDIR}.Messages ${CPFLAGS}
	@echo Cron: Messages copied

${TARGET}: ${SOURCE}
	${MKDIR} rm
        ${AS} ${ASFLAGS} ${SOURCE} ${THROWBACK}
        
${TARGETSA}: ${SOURCESA} ${TOKENISED}
	${MKDIR} rm
        ${AS} ${ASFLAGS} ${SOURCESA} ${THROWBACK}

${TARGETSD}: ${SOURCESD} ${TOKENISED}
	${MKDIR} rm
        ${AS} ${ASFLAGS} ${SOURCESD} ${THROWBACK}

${TOKENISED}: ${MESSAGES}
	${CP} ${MESSAGES} ${TOKENISED} ${CPFLAGS}
#	${TOKENISE} ${TOKENFILE} ${MESSAGES} ${TOKENISED}

# Dynamic dependencies:
rm.Cron: Hdr:ListOpts
rm.Cron: Hdr:NdrDebug
rm.Cron: Hdr:Macros
rm.Cron: Hdr:System
rm.Cron: Hdr:SWIs
rm.Cron: Hdr:CPU.Generic26
rm.Cron: Hdr:IO.GenericIO
rm.Cron: Hdr:RISCOS
rm.Cron: Hdr:ModHand
rm.Cron: Hdr:Symbols
rm.Cron: Hdr:Proc
rm.Cron: Hdr:Messages
rm.Cron: Hdr:ResourceFS
rm.Cron: Hdr:MsgTrans
rm.Cron: Hdr:FSNumbers
rm.Cron: Hdr:NewErrors
rm.Cron: Hdr:Territory
rm.Cron: Version
rm.Cron: s.error
rm.Cron: s.main
rm.Cron: s.swis
rm.Cron: s.internat
rm.Cron: s.commands
rm.Cron: s.schedule
rm.Cron: s.callback
rm.Cron: s.execute
rm.SACronD: s.init
rm.SACronD: Hdr:ListOpts
rm.SACronD: Hdr:NdrDebug
rm.SACronD: Hdr:Macros
rm.SACronD: Hdr:System
rm.SACronD: Hdr:SWIs
rm.SACronD: Hdr:CPU.Generic26
rm.SACronD: Hdr:IO.GenericIO
rm.SACronD: Hdr:RISCOS
rm.SACronD: Hdr:ModHand
rm.SACronD: Hdr:Symbols
rm.SACronD: Hdr:Proc
rm.SACronD: Hdr:Messages
rm.SACronD: Hdr:ResourceFS
rm.SACronD: Hdr:MsgTrans
rm.SACronD: Hdr:FSNumbers
rm.SACronD: Hdr:NewErrors
rm.SACronD: Hdr:Territory
rm.SACronD: Hdr:Debug
rm.SACronD: Version
rm.SACronD: s.error
rm.SACronD: s.main
rm.SACronD: s.swis
rm.SACronD: s.internat
rm.SACronD: s.commands
rm.SACronD: s.schedule
rm.SACronD: s.callback
rm.SACronD: s.execute
rm.SACron: s.init
rm.SACron: Hdr:ListOpts
rm.SACron: Hdr:NdrDebug
rm.SACron: Hdr:Macros
rm.SACron: Hdr:System
rm.SACron: Hdr:SWIs
rm.SACron: Hdr:CPU.Generic26
rm.SACron: Hdr:IO.GenericIO
rm.SACron: Hdr:RISCOS
rm.SACron: Hdr:ModHand
rm.SACron: Hdr:Symbols
rm.SACron: Hdr:Proc
rm.SACron: Hdr:Messages
rm.SACron: Hdr:ResourceFS
rm.SACron: Hdr:MsgTrans
rm.SACron: Hdr:FSNumbers
rm.SACron: Hdr:NewErrors
rm.SACron: Hdr:Territory
rm.SACron: Version
rm.SACron: s.error
rm.SACron: s.main
rm.SACron: s.swis
rm.SACron: s.internat
rm.SACron: s.commands
rm.SACron: s.schedule
rm.SACron: s.callback
rm.SACron: s.execute
