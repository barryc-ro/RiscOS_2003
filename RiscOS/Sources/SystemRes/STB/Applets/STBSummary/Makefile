# Makefile for !STBSummry
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name  Description
# ----           ----  -----------
# 25th Feb 2002  SAR   Created.
#

COMPONENT  = STBSummary
INSTAPP    = ${INSTDIR}.${TARGET}
LOCALRES   = Resources.${UserIF}.${Locale}

CHMODFLAGS = -R 0755

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: dirs
	${CP} ${LOCALRES}.Applet ${INSTAPP} ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	@echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTAPP}

# Dynamic dependencies:
