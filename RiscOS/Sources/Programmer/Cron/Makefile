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
TARGET     = rm.${COMPONENT}
TARGETSA   = rm.SA${COMPONENT}
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
        ${AS} ${THROWBACK} ${ASFLAGS} ${SOURCE}
        
${TARGETSA}: ${SOURCESA} ${TOKENISED}
	${MKDIR} rm
        ${AS} ${THROWBACK} ${ASFLAGS} ${SOURCESA}

${TOKENISED}: ${MESSAGES}
	${CP} ${MESSAGES} ${TOKENISED} ${CPFLAGS}
#	${TOKENISE} ${TOKENFILE} ${MESSAGES} ${TOKENISED}

# Dynamic dependencies:
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
