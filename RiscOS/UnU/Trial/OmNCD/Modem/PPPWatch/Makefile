# Makefile for Omega PPP Module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-01-96   PSW	  Created.
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
WIPE    = -wipe

CPFLAGS  = ~cfr~v
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT   = PPPWatch
TARGET      = rm.PPPWatch

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: 
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

${TARGET}:
	${MODSQZ} ${TARGET}
	
# Dynamic dependencies:
