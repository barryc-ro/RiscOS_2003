# Makefile for AHVC
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13/11/98   SMC          Created
#

#
# Generic options:
#
MKDIR   = cdir
CP      = copy
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = AHVC
APP       = SysEnv
RDIR	= Resources


#
# Generic rules:
#

all:
	@echo ${COMPONENT}: built

install: ${RDIR}
        ${CP} ${RDIR} ${INSTDIR}.${APP} ${CPFLAGS}
	@echo ${COMPONENT}: installed

resources:
        @echo ${COMPONENT}: resource files copied to Messages module

# Dynamic dependencies:
