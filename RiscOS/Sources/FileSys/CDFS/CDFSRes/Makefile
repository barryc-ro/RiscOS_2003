# Makefile for CDFS Resources Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Feb-95  AMcC         Created.
# 29-Nov-95  RWB          Added install: option
#  6-Jan-96  NDT          Moved list file to machine (new) directory
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
PL       = perl

CPFLAGS  = ~cfr~v
WFLAGS   = ~cr~v

#
# Program specific options:
#
COMPONENT   = CDFSRes
VERSIONVAR  = CDFSRes$Version
VERSION     = <CDFSRes$Version>
PLSCRIPT    = build:getversion

TARGET      = rm.${BUILD}.CDFSRes
MODULE_HELP = "CDFSResources"
MODULE_NAME = CDFSResources
FILELIST    = Resources.${LOCALE}.${BUILD}.List

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
	${WIPE} rm.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${FILELIST} versionvar
	${MKDIR} rm.${BUILD}
        Do ${MODGEN} $@ ${MODULE_NAME} ${MODULE_HELP} ${VERSION} -via ${FILELIST}

versionvar:
        ${PL} ${PLSCRIPT} ${VERSIONVAR}

# Dynamic dependencies:
