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
PL       = perl
WIPE    = -wipe

CPFLAGS  = ~cfr~v
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT   = AUNMsgs
VERSIONVAR  = AUNMsgs$Version
VERSION     = <AUNMsgs$Version>
TARGET      = rm.AUNMsgs
MODULE_HELP = "AUN Messages"
FILELIST    = Resources.${LOCALE}.<Machine>
PLSCRIPT    = build:getversion



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
        @echo ${COMPONENT}: cleaned

${TARGET}: ${FILELIST} versionvar
        Do ${MODGEN} $@ ${COMPONENT} ${MODULE_HELP} ${VERSION} -via ${FILELIST}

versionvar:
        ${PL} ${PLSCRIPT} ${VERSIONVAR}

# Dynamic dependencies:
