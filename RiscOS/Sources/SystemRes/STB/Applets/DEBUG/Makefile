# Makefile for !DEBUG
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name  Description
# ----           ----  -----------
# 25th Feb 2002  SAR   Created.
#

COMPONENT  = DEBUG
INSTAPP    = ${INSTDIR}.${TARGET}
LOCALRES   = Resources.${UserIF}.${Locale}

CHMODFLAGS = -R 0755

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: dirs
	${CP} ${LOCALRES}.Applet     ${INSTAPP}                     ${CPFLAGS}
	${CP} ${LOCALRES}.Choices    ${INSTDIR}.!STBBoot.Choices    ${CPFLAGS}
	${CP} ${LOCALRES}.PreDesktop ${INSTDIR}.!STBBoot.PreDesktop ${CPFLAGS}
	${CP} ${LOCALRES}.images     ${INSTAPP}.images              ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	${CHMOD} ${CHMODFLAGS} ${INSTDIR}.!STBBoot.Choices
	${CHMOD} ${CHMODFLAGS} ${INSTDIR}.!STBBoot.PreDesktop
	@echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTAPP}.images
	${MKDIR} ${INSTDIR}.!STBBoot.Choices
	${MKDIR} ${INSTDIR}.!STBBoot.PreDesktop

# Dynamic dependencies:
