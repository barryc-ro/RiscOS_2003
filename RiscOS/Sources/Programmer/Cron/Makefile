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
MESSAGES   = LocalRes:Messages
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
	TokenCheck ${MESSAGES}
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
