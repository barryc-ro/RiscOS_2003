#  Makefile for the FlashImg component
# -===================================-
#
# Initial version: 12th Feb 2002, SAR
#         Updated: 11th Mar 2002, SAR

COMPONENT  = FlashImg
INSTIMG    = ${INSTDIR}.${IMG}.${COMPONENT}
INSTHOOK   = ${INSTDIR}.${HOOK}.!Flash
LOCALRES   = Resources.${Machine}

SETTYPE    = SetType
TYPE_TEXT  = Text
CHMODFLAGS = -R 0755

include Makefiles:StdTools

clean:
        @echo Component ${COMPONENT} cleaned.

install:
        ${MKDIR}   ${INSTDIR}.${IMG}
        ${MKDIR}   ${INSTDIR}.${HOOK}
        ${CP}      ${LOCALRES}.${COMPONENT} ${INSTIMG}  ${CPFLAGS}
        ${CP}      ${LOCALRES}.!Flash       ${INSTHOOK} ${CPFLAGS}
        ${SETTYPE} ${INSTIMG}    ${TYPE_TEXT}
	${CHMOD}   ${CHMODFLAGS} ${INSTIMG}
	${CHMOD}   ${CHMODFLAGS} ${INSTHOOK}
        @echo Component ${COMPONENT} installed (disc).

# Dynamic dependencies:
