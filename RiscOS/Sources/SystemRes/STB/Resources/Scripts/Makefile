#  Makefile for the Scripts component
# -==================================-
#
# Initial version: 09th Apr 2002, SAR

COMPONENT  = Scripts
INSTIMG    = ${INSTDIR}.${IMG}.${COMPONENT}
INSTHOOK   = ${INSTDIR}.${HOOK}.!Flash
LOCALRES   = Resources.${Machine}

CHMODFLAGS = -R 0755

PL         = perl
VERSIONVAR = ${COMPONENT}$Version
DATEVAR    = ${COMPONENT}$Date
PLSCRIPT   = Build:GetVersion

MAPPINGS   = Mappings
TRANSFLAGS = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë" -m ${MAPPINGS}

include Makefiles:StdTools

clean:
	${WIPE} bin ${WFLAGS}
	${RM} ${MAPPINGS}
	@echo Component ${COMPONENT} cleaned.

install: ram
	${MKDIR} ${INSTDIR}
	${CP} bin ${INSTDIR} ${CPFLAGS}
	@echo Component ${COMPONENT} installed (disc).

ram: make_mappings dirs
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:AC3-US/sh    -o bin.AC3-US/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:HwaCom/sh    -o bin.HwaCom/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:iMagicCAN/sh -o bin.iMagicCAN/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:iMagicUK/sh  -o bin.iMagicUK/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:Japan/sh     -o bin.Japan/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:TCC/sh       -o bin.TCC/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:V1Gen/sh     -o bin.V1Gen/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:V2Gen/sh     -o bin.V2Gen/sh
	${TRANSLATE} ${TRANSFLAGS} -i TranslateRes:replace/sh   -o bin.replace/sh
        ${CHMOD} ${CHMODFLAGS} bin
	@echo Component ${COMPONENT} built (ram).

dirs:
	${MKDIR} bin

make_mappings:
	${PL} ${PLSCRIPT} ${VERSIONVAR} tmp ${DATEVAR}
	${CP} TranslateRes:Mappings ${MAPPINGS} ${CPFLAGS}
	do echo Version:<${VERSIONVAR}>{ >> ${MAPPINGS} }
	do echo Date:<${DATEVAR}>{ >> ${MAPPINGS} }

# Dynamic dependencies:
