#  Makefile for STB Baseline options pages
# -=======================================-
#
# Initial version: 21 Jan 2002, SAR

COMPONENT = Territory
INSTAPP   = ${INSTDIR}.!${COMPONENT}
INSTOPTS  = ${INSTAPP}.<STB$OS>.${Locale}
INSTPICS  = ${INSTOPTS}.pics

TRANSLATE  = translate
TRANSFLAGS = ${THROWBACK} -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë" -m TerrTrans:Mappings

include Makefiles:StdTools

clean:
	@echo Component ${COMPONENT} cleaned.

install: unset_vars translate resources dirs set_vars
        @echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTOPTS}
	${MKDIR} ${INSTPICS}

set_vars: set_paths_${BROWSER} set_OS_${Machine}
	@echo Set OS for ${Machine} to <STB$OS>

set_OS_STB3:
	@set STB$OS 300

set_OS_STB400:
	@set STB$OS 400

set_paths_V1:
	@set TerrRes$Path   Resources.V1.${UserIF}.${Locale}.,Resources.V1.${UserIF}.UK.,Resources.V1.Common.${Locale}.,Resources.V1.Common.UK.,Resources.Common.
	@set TerrTrans$Path Translate.V1.${UserIF}.${Locale}.,Translate.V1.${UserIF}.UK.,Translate.V1.Common.${Locale}.,Translate.V1.Common.UK.,Translate.Common.
	@echo Set paths for V1 broser

set_paths_V2:
	@set TerrRes$Path   Resources.V2.${UserIF}.${Locale}.,Resources.V2.${UserIF}.UK.,Resources.V2.Common.${Locale}.,Resources.V2.Common.UK.,Resources.Common.
	@set TerrTrans$Path Translate.V2.${UserIF}.${Locale}.,Translate.V2.${UserIF}.UK.,Translate.V2.Common.${Locale}.,Translate.V2.Common.UK.,Translate.Common.
	@echo Set paths for V2 broser

unset_vars:
	@unset TerrRes$Path
	@unset TerrTrans$Path
	@unset STB$OS

resources:
	IfThere TerrRes:!Run      Then ${CP} TerrRes:!Run      ${INSTAPP}.!Run           ${CPFLAGS}
	IfThere TerrRes:!RunImage Then ${CP} TerrRes:!RunImage ${INSTAPP}.!RunImage      ${CPFLAGS}
	IfThere TerrRes:banners   Then ${CP} TerrRes:banners   ${INSTOPTS}.pics.banners  ${CPFLAGS}
	IfThere TerrRes:misc      Then ${CP} TerrRes:misc      ${INSTOPTS}.pics.misc     ${CPFLAGS}
	IfThere TerrRes:teleopts  Then ${CP} TerrRes:teleopts  ${INSTOPTS}.pics.teleopts ${CPFLAGS}

translate:
	IfThere TerrTrans:Acknow     Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Acknow      -o ${INSTOPTS}.Acknow
	IfThere TerrTrans:BSD        Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:BSD         -o ${INSTOPTS}.BSD
	IfThere TerrTrans:ImageLib   Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:ImageLib    -o ${INSTOPTS}.ImageLib
	IfThere TerrTrans:Options    Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Options     -o ${INSTOPTS}.Options
	IfThere TerrTrans:Printing   Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Printing    -o ${INSTOPTS}.Printing
	IfThere TerrTrans:Printing2  Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Printing2   -o ${INSTOPTS}.Printing2
	IfThere TerrTrans:Sounds     Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Sounds      -o ${INSTOPTS}.Sounds
	IfThere TerrTrans:SSL        Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:SSL         -o ${INSTOPTS}.SSL
	IfThere TerrTrans:StartUp    Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:StartUp     -o ${INSTOPTS}.StartUp
	IfThere TerrTrans:Software   Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Software    -o ${INSTOPTS}.Software
	IfThere TerrTrans:Television Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Television  -o ${INSTOPTS}.Television
	IfThere TerrTrans:Text       Then ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Text        -o ${INSTOPTS}.Text

# Dynamic dependencies:
