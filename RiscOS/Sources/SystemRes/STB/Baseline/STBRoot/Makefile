#  Makefile for the STB Baseline root directory structure component
# -================================================================-
#
# Initial version: 25th Feb 2002, SAR

COMPONENT    = STBRoot
LOCALRES     = Resources.${UserIF}.${Locale}
TRANSRES     = Translate.${UserIF}.${Locale}
TRANSMAP     = ${TRANSRES}.Mappings

CHMODFLAGS   = -R 0755
CHMODRWFLAGS = -R 0777
TRANSFLAGS   = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: translate dirs
	${CP} ${LOCALRES}.Root    ${INSTDIR}         ${CPFLAGS}
	${CP} ${LOCALRES}.Content ${INSTDIR}.Content ${CPFLAGS}
	${CHMOD} ${CHMODFLAGS}   ${INSTDIR}
	${CHMOD} ${CHMODRWFLAGS} ${INSTDIR}.Public
	@echo Locale ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTDIR}.Apps
	${MKDIR} ${INSTDIR}.Content.HTML.images
	${MKDIR} ${INSTDIR}.Content.HTML.JScript
	${MKDIR} ${INSTDIR}.Content.HTML.SendMsg
	${MKDIR} ${INSTDIR}.Content.HTML.Test
	${MKDIR} ${INSTDIR}.Content.Java
	${MKDIR} ${INSTDIR}.Content.MPEG
	${MKDIR} ${INSTDIR}.Content.Sounds
	${MKDIR} ${INSTDIR}.Content.Splash
	${MKDIR} ${INSTDIR}.Public
	${MKDIR} ${INSTDIR}.Versions

translate:
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.blank/html   -o ${INSTDIR}.Content.HTML.JScript.blank/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.ctrl/html    -o ${INSTDIR}.Content.HTML.JScript.ctrl/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.plugplay     -o ${INSTDIR}.Content.HTML.JScript.plugplay/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.simple       -o ${INSTDIR}.Content.HTML.JScript.simple/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.tv/html      -o ${INSTDIR}.Content.HTML.JScript.tv/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.JScript.vod/html     -o ${INSTDIR}.Content.HTML.JScript.vod/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Root.!TFTPBoot       -o ${INSTDIR}.!TFTPBoot
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.SendMsg.Demo         -o ${INSTDIR}.Content.HTML.SendMsg.Demo/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.SendMsg.Heartbeat    -o ${INSTDIR}.Content.HTML.SendMsg.Heartbeat/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.SendMsg.InStby       -o ${INSTDIR}.Content.HTML.SendMsg.InStby/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.SendMsg.OutStby      -o ${INSTDIR}.Content.HTML.SendMsg.OutStby/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Test.Audio           -o ${INSTDIR}.Content.HTML.Test.Audio/html
	${TRANSLATE} ${TRANSFLAGS} -m ${TRANSMAP} -i ${TRANSRES}.Test.EnvVar          -o ${INSTDIR}.Content.HTML.Test.EnvVar/html

# Dynamic dependencies:
