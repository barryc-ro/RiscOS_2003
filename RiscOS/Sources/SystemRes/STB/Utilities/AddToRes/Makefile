#  Makefile for the AddToRes utility
# -=================================-
#
# Initial version: 19th Feb 2002, SAR

COMPONENT = AddToRes

include Makefiles:StdTools

clean:
        @echo Component ${COMPONENT} cleaned.

install:
        ${MKDIR} ${INSTDIR}
        ${CP}    Resources.${COMPONENT} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo Locale ${Locale} installed (disc).

# Dynamic dependencies:
