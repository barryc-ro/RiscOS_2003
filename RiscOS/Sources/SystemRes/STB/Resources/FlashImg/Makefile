#  Makefile for the FlashImg component
# -===================================-
#
# Initial version: 12th Feb 2002, SAR

COMPONENT = FlashImg
INSTIMG   = ${INSTDIR}.${COMPONENT}
LOCALRES  = Resources.${Machine}

SETTYPE   = SetType
TYPE_TEXT = Text

include Makefiles:StdTools

clean:
        @echo Component ${COMPONENT} cleaned.

install:
        ${MKDIR}   ${INSTDIR}
        ${CP}      ${LOCALRES}.${COMPONENT}   ${INSTIMG}   ${CPFLAGS}
        ${SETTYPE} ${INSTIMG} ${TYPE_TEXT}
        @echo Component ${COMPONENT} installed (disc).

# Dynamic dependencies:
