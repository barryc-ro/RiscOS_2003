#  Makefile for the STBBoot directory structure component
# -======================================================-
#
# Initial version: 25th Feb 2002, SAR

COMPONENT    = STBBoot
INSTAPP      = ${INSTDIR}.!${COMPONENT}
LOCALRES     = Resources.${UserIF}.${Locale}
TRANSRES     = Translate.${UserIF}.${Locale}
TRANSMAP     = ${TRANSRES}.Mappings
NOP          = @|
CP2FLAGS     = F~R~C~V~N

CHMODFLAGS   = -R 0755
TRANSFLAGS   = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: install_${TYPE} translate_${TYPE} dirs_${TYPE}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	@echo Component ${COMPONENT} installed (disc).

install_Core:
	${CP} ${LOCALRES}.Applet     ${INSTAPP}            ${CPFLAGS}
	${CP} ${LOCALRES}.Choices    ${INSTAPP}.Choices    ${CP2FLAGS}
	${CP} ${LOCALRES}.Desktop    ${INSTAPP}.Desktop    ${CP2FLAGS}
	${CP} ${LOCALRES}.Library    ${INSTAPP}.Library    ${CPFLAGS}
	${CP} ${LOCALRES}.PreDesktop ${INSTAPP}.PreDesktop ${CP2FLAGS}
	${CP} ${LOCALRES}.Utilities  ${INSTAPP}.Utilities  ${CPFLAGS}

install_STB3:
	${CP} ${LOCALRES}.PreDesktop.300      ${INSTAPP}.PreDesktop.300 ${CPFLAGS}

install_STB4:
	${CP} ${LOCALRES}.PreDesktop.400                        ${INSTAPP}.PreDesktop.400 ${CPFLAGS}
	${CP} ^.^.^.^.HWSupport.NVRAM.resources.${Machine}.Tags ${INSTAPP}.Utilities.Tags ${CPFLAGS}

install_Teletext:
	${CP} ${LOCALRES}.PreDesktop.Teletext ${INSTAPP}.PreDesktop.400 ${CPFLAGS}

install_USB:
	${CP} ${LOCALRES}.PreDesktop.USB      ${INSTAPP}.PreDesktop.400 ${CPFLAGS}

install_RCMM:
	${CP} ${LOCALRES}.Choices.RCMM        ${INSTAPP}.Choices        ${CPFLAGS}
	${CP} ${LOCALRES}.PreDesktop.RCMM     ${INSTAPP}.PreDesktop.400 ${CPFLAGS}

install_ARMovie:
        ${CP} ${LOCALRES}.PreDesktop.ARMovie  ${INSTAPP}.PreDesktop     ${CPFLAGS}

dirs_Core:
	${MKDIR} ${INSTAPP}.Choices
	${MKDIR} ${INSTAPP}.Desktop
	${MKDIR} ${INSTAPP}.Library
	${MKDIR} ${INSTAPP}.PreDesktop
	${MKDIR} ${INSTAPP}.Resources
	${MKDIR} ${INSTAPP}.Utilities

dirs_STB3:
	${MKDIR} ${INSTAPP}.Desktop.300
	${MKDIR} ${INSTAPP}.PreDesktop.300
	${MKDIR} ${INSTAPP}.Resources.300.Misc

dirs_STB4:
	${MKDIR} ${INSTAPP}.Desktop.400
	${MKDIR} ${INSTAPP}.PreDesktop.400
	${MKDIR} ${INSTAPP}.Resources.400.Misc
	${MKDIR} ${INSTAPP}.Utilities

dirs_Teletext:
	${MKDIR} ${INSTAPP}.PreDesktop.400

dirs_USB:
	${MKDIR} ${INSTAPP}.PreDesktop.400

dirs_RCMM:
	${MKDIR} ${INSTAPP}.Choices
	${MKDIR} ${INSTAPP}.PreDesktop.400

dirs_ARMovie:
        ${MKDIR} ${INSTAPP}.PreDesktop

translate_Core:
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Applet.!Run          -o ${INSTAPP}.!Run

translate_STB3:
	${NOP}

translate_STB4:
	${NOP}

translate_Teletext:
	${NOP}

translate_USB:
	${NOP}

translate_RCMM:
	${NOP}

translate_ARMovie:
        ${NOP}

# Dynamic dependencies:
