# Makefile for HeroNames module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Mar-00  SNB          Created.

#
# Generic options:
#
CP      = copy
CPFLAGS = ~cfr~v

#
# Program specific options:
#
DATA      = LocalUserIFRes:HeroNames

#
# Generic rules:
#
install_rom: ${DATA}
        ${CP} ${DATA} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

# Dynamic dependencies:
