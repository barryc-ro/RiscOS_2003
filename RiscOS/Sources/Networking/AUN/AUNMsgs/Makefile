# Makefile for AUN Messages Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Feb-95  AMcC         Created.
#

#
# Generic options:
#
CP       = copy
MKDIR    = cdir
MODGEN   = modgen
MODSQZ   = modsqz
RM       = remove
SCANRES  = scanres
WIPE     = -wipe

CPFLAGS  = ~cfr~v
WFLAGS   = ~cr~v
 
#
# Program specific options:
#
COMPONENT   = AUNMsgs
VERSION     = 0.07
TARGET      = rm.${BUILD}.AUNMsgs
MODULE_HELP = "AUN Messages"
FILELIST    = Resources.${LOCALE}.${BUILD}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${FILELIST}
	${MKDIR} rm.${BUILD}
        ${MODGEN} $@ ${COMPONENT} ${MODULE_HELP} ${VERSION} -via ${FILELIST}

# Dynamic dependencies:
