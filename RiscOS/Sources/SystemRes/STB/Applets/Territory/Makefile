#  Makefile for STB Baseline options pages
# -=======================================-
#
# Initial version: 21 Jan 2002, SAR

COMPONENT = Territory
INSTAPP   = ${INSTDIR}.!${COMPONENT}
INSTOPTS  = ${INSTAPP}.<STB$OS>.${Locale}
INSTPICS  = ${INSTOPTS}.pics
HTMLDIR   = ${INSTHTML}.<STB$OS>.${Locale}
CMNRESDIR = ${INSTAPP}.Resources

IFTHERE    = IfThere
IFTHEN     = Then
IFELSE     = Else
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
	${MKDIR} ${HTMLDIR}
	${MKDIR} ${CMNRESDIR}
	${MKDIR} ${CMNRESDIR}.Canada1
	${MKDIR} ${CMNRESDIR}.France
	${MKDIR} ${CMNRESDIR}.Germany
	${MKDIR} ${CMNRESDIR}.Norway
	${MKDIR} ${CMNRESDIR}.Spain

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
	${IFTHERE} TerrRes:!Run      ${IFTHEN} ${CP} TerrRes:!Run      ${INSTAPP}.!Run               ${CPFLAGS}
	${IFTHERE} TerrRes:!RunImage ${IFTHEN} ${CP} TerrRes:!RunImage ${INSTAPP}.!RunImage          ${CPFLAGS}
	${IFTHERE} TerrRes:banners   ${IFTHEN} ${CP} TerrRes:banners   ${INSTOPTS}.pics.banners      ${CPFLAGS}
	${IFTHERE} TerrRes:misc      ${IFTHEN} ${CP} TerrRes:misc      ${INSTOPTS}.pics.misc         ${CPFLAGS}
	${IFTHERE} TerrRes:teleopts  ${IFTHEN} ${CP} TerrRes:teleopts  ${INSTOPTS}.pics.teleopts     ${CPFLAGS}
	${IFTHERE} TerrRes:Canada1   ${IFTHEN} ${CP} TerrRes:Canada1   ${CMNRESDIR}.Canada1.Messages ${CPFLAGS}
	${IFTHERE} TerrRes:France    ${IFTHEN} ${CP} TerrRes:France    ${CMNRESDIR}.France.Messages  ${CPFLAGS}
	${IFTHERE} TerrRes:Germany   ${IFTHEN} ${CP} TerrRes:Germany   ${CMNRESDIR}.Germany.Messages ${CPFLAGS}
	${IFTHERE} TerrRes:Norway    ${IFTHEN} ${CP} TerrRes:Norway    ${CMNRESDIR}.Norway.Messages  ${CPFLAGS}
	${IFTHERE} TerrRes:Spain     ${IFTHEN} ${CP} TerrRes:Spain     ${CMNRESDIR}.Spain.Messages   ${CPFLAGS}
	${IFTHERE} TerrRes:JScript   ${IFTHEN} ${CP} TerrRes:JScript   ${INSTHTML}.JScript           ${CPFLAGS}
	${IFTHERE} TerrRes:SendMsg   ${IFTHEN} ${CP} TerrRes:SendMsg   ${INSTHTML}.SendMsg           ${CPFLAGS}
	${IFTHERE} TerrRes:Test      ${IFTHEN} ${CP} TerrRes:Test      ${INSTHTML}.Test              ${CPFLAGS}

translate:
	${IFTHERE} TerrTrans:Acknow            ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Acknow            -o ${INSTOPTS}.Acknow
	${IFTHERE} TerrTrans:BSD               ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:BSD               -o ${INSTOPTS}.BSD
	${IFTHERE} TerrTrans:ImageLib          ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:ImageLib          -o ${INSTOPTS}.ImageLib
	${IFTHERE} TerrTrans:Options           ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Options           -o ${INSTOPTS}.Options
	${IFTHERE} TerrTrans:Printing          ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Printing          -o ${INSTOPTS}.Printing
	${IFTHERE} TerrTrans:Printing2         ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Printing2         -o ${INSTOPTS}.Printing2
	${IFTHERE} TerrTrans:Sounds            ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Sounds            -o ${INSTOPTS}.Sounds
	${IFTHERE} TerrTrans:SSL               ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:SSL               -o ${INSTOPTS}.SSL
	${IFTHERE} TerrTrans:StartUp           ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:StartUp           -o ${INSTOPTS}.StartUp
	${IFTHERE} TerrTrans:Software          ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Software          -o ${INSTOPTS}.Software
	${IFTHERE} TerrTrans:Television        ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Television        -o ${INSTOPTS}.Television
	${IFTHERE} TerrTrans:Text              ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:Text              -o ${INSTOPTS}.Text
	${IFTHERE} TerrTrans:html.body/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.body/html    -o ${HTMLDIR}.body/html
	${IFTHERE} TerrTrans:html.body_h/html  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.body_h/html  -o ${HTMLDIR}.body_h/html
	${IFTHERE} TerrTrans:html.home/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.home/html    -o ${HTMLDIR}.home/html
	${IFTHERE} TerrTrans:html.home_h/html  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.home_h/html  -o ${HTMLDIR}.home_h/html
	${IFTHERE} TerrTrans:html.index/html   ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.index/html   -o ${HTMLDIR}.index/html
	${IFTHERE} TerrTrans:html.index_h/html ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.index_h/html -o ${HTMLDIR}.index_h/html
	${IFTHERE} TerrTrans:html.side/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.side/html    -o ${HTMLDIR}.side/html
	${IFTHERE} TerrTrans:html.top/html     ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.top/html     -o ${HTMLDIR}.top/html
	${IFTHERE} TerrTrans:html.homecss/css  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.homecss/css  -o ${HTMLDIR}.homecss/css

# Dynamic dependencies:
