#  Makefile for the STBBoot directory structure component
# -======================================================-
#
# Initial version: 25th Feb 2002, SAR

COMPONENT    = STBBoot
INSTAPP      = ${INSTDIR}.!${COMPONENT}
LOCALRES     = Resources.${UserIF}.${Locale}
TRANSRES     = Translate.${UserIF}.${Locale}
TRANSMAP     = ${TRANSRES}.Mappings

CHMODFLAGS   = -R 0755
TRANSFLAGS   = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: translate dirs
	${CP} ${LOCALRES}.Applet     ${INSTAPP}            ${CPFLAGS}
	${CP} ${LOCALRES}.Choices    ${INSTAPP}.Choices    ${CPFLAGS}
	${CP} ${LOCALRES}.Desktop    ${INSTAPP}.Desktop    ${CPFLAGS}
	${CP} ${LOCALRES}.Library    ${INSTAPP}.Library    ${CPFLAGS}
	${CP} ${LOCALRES}.PreDesktop ${INSTAPP}.PreDesktop ${CPFLAGS}
	${CP} ${LOCALRES}.Utilities  ${INSTAPP}.Utilities  ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	@echo Locale ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTAPP}.Choices
	${MKDIR} ${INSTAPP}.Desktop.300
	${MKDIR} ${INSTAPP}.Desktop.400
	${MKDIR} ${INSTAPP}.Library
	${MKDIR} ${INSTAPP}.PreDesktop.300
	${MKDIR} ${INSTAPP}.PreDesktop.400
	${MKDIR} ${INSTAPP}.Resources
	${MKDIR} ${INSTAPP}.Resources.300.Misc
	${MKDIR} ${INSTAPP}.Resources.400.Misc
	${MKDIR} ${INSTAPP}.Utilities

translate:
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Applet.!Run          -o ${INSTAPP}.!Run
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Desktop.NCFresco     -o ${INSTAPP}.Desktop.NCFresco
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Utilities.Desktop    -o ${INSTAPP}.Utilities.Desktop
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Utilities.PreDesktop -o ${INSTAPP}.Utilities.PreDesktop

# Dynamic dependencies:
