# Makefile for !AudioCmd
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name  Description
# ----           ----  -----------
# 25th Feb 2002  SAR   Created.
#

COMPONENT  = AudioCmd
INSTAPP    = ${INSTDIR}.${TARGET}
INSTMOD    = ${INSTDIR}.${MOD}
LOCALRES   = Resources.${UserIF}.${Locale}

CHMODFLAGS = -R 0755

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: dirs
	${CP} ${LOCALRES}.Applet  ${INSTAPP} ${CPFLAGS}
	${CP} ${LOCALRES}.Modules ${INSTMOD} ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	@echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTMOD}

# Dynamic dependencies:
