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
TOKENISE=Tokenise
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

#
# Generic rules:
#
install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

all: ${TARGETSA}
        @echo ${COMPONENT}: standalone module built

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
