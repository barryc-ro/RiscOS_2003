# Makefile for Territory modules
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 21-Mar-01  SNB          Recreated.

#
# Program specific options:
#
COMPONENT ?= UK
TARGET    ?= ${COMPONENT}

ROM_SOURCE = Territory.s
ASFLAGS = -PD "Territory SETS \"${TARGET}\""

RESOURCES   = -private
PRIVATEMSGS = -private

#
# Generic options:
#
include StdTools
include AAsmModule

#
# Generic rules:
#
clean::
        ${WIPE} rm ${WFLAGS}
        @${ECHO} ${COMPONENT}: extra directories cleaned

${MERGEDMSGS}: Resources.${TARGET}.Territory LocalRes:Messages
        ${MKDIR} ${MERGEDMDIR}
        ${CP} Resources.${TARGET}.Territory $@ ${CPFLAGS}
        ${CAT} LocalRes:Messages { >> $@ }

resources: ${MERGEDMSGS}
        ${MKDIR} ${RESFSDIR}
        ${CP} ${MERGEDMSGS} ${RESFSDIR}.Messages ${CPFLAGS}
        @${ECHO} ${COMPONENT}: resources copied to Messages module

# BBE Rules
BBETYPE   = local
bbe-local: bbe-generic-resources-get-alias
  BBE_Export_File_In_Dir ${MERGEDMDIR} ${TARGET}
  BBE_Export_File VersionNum


# Dynamic dependencies:
