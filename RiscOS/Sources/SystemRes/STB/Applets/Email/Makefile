# Makefile for !Email
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name  Description
# ----           ----  -----------
# 25th Feb 2002  SAR   Created.
#

COMPONENT   = EmailApplet
INSTAPP     = ${INSTDIR}.!STBBoot
INST_CHOICE = ${INSTAPP}.Choices.NCMail
INST_DESK   = ${INSTAPP}.Desktop
INST_PRE    = ${INSTAPP}.PreDesktop
INST_TEST   = ${INSTDIR}.Content.HTML.Test
LOCALRES    = Resources.${UserIF}.${Locale}

CHMODFLAGS  = -R 0755

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: dirs
	${CP} ${LOCALRES}.Choices    ${INST_CHOICE} ${CPFLAGS}
	${CP} ${LOCALRES}.Desktop    ${INST_DESK}   ${CPFLAGS}
	${CP} ${LOCALRES}.PreDesktop ${INST_PRE}    ${CPFLAGS}
	${CP} ${LOCALRES}.Test       ${INST_TEST}   ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS} ${INSTAPP}
	${CHMOD} ${CHMODFLAGS} ${INST_TEST}
	@echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INST_CHOICE}
	${MKDIR} ${INST_DESK}
	${MKDIR} ${INST_PRE}
	${MKDIR} ${INST_TEST}

# Dynamic dependencies:
