#  Makefile for STB Baseline options pages
# -=======================================-
#
# Initial version: 21 Jan 2002, SAR
#         updates: 14 Feb 2002, SAR
#    more updates: 01 Mar 2002, SAR

COMPONENT = Territory

INSTAPP   = ${INSTDIR}.${APP}.!${COMPONENT}
CMNLOCDIR = ${INSTAPP}.${Locale}

INSTHTML  = ${INSTDIR}.${HTML}.<STB$OS>.${Locale}
INSTCHOI  = ${INSTDIR}.${CHOICE}.${COMPONENT}

INSTOPTS  = ${INSTAPP}.<STB$OS>.${Locale}
INSTPICS  = ${INSTOPTS}.pics

MSGSDIR   = Messages

MSG_CHECK    = ^.^.^.^.Networking.CheckURL.Resources.${Locale}.,^.^.^.^.Networking.CheckURL.Resources.UK.
MSG_FILTER   = ^.^.^.^.Desktop.Filter.Resources.${Locale}.,^.^.^.^.Desktop.Filter.Resources.UK.
MSG_PARALLEL = ^.^.^.^.HWSupport.Parallel.Resources.${Locale}.,^.^.^.^.HWSupport.Parallel.Resources.UK.
MSG_PPRIMER  = ^.^.^.^.Printing.PPrimer.Resources.${Locale}.,^.^.^.^.Printing.PPrimer.Resources.UK.

IFTHERE    = IfThere
IFTHEN     = Then
IFELSE     = Else
CONCAT     = Do Print <123> >> 
CATFLAGS   = }
TRANSFLAGS = ${THROWBACK} -marko "««" -markc "»»" -htmlo "" -htmlc "‘‘" -m TerrTrans:Mappings

include Makefiles:StdTools

clean:
	${WIPE} ${MSGSDIR} ${WFLAGS}
	@echo Component ${COMPONENT} cleaned.

install: unset_vars translate resources dirs set_vars
        @echo Component ${COMPONENT} installed (disc).

dirs:
	${MKDIR} ${MSGSDIR}
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTOPTS}
	${MKDIR} ${INSTPICS}
	${MKDIR} ${INSTHTML}
	${MKDIR} ${INSTCHOI}
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTAPP}.Canada1
	${MKDIR} ${INSTAPP}.France
	${MKDIR} ${INSTAPP}.Germany
	${MKDIR} ${INSTAPP}.Norway
	${MKDIR} ${INSTAPP}.Spain
	${MKDIR} ${CMNLOCDIR}.CheckURL
	${MKDIR} ${CMNLOCDIR}.FilterMgr
	${MKDIR} ${CMNLOCDIR}.Parallel
	${MKDIR} ${CMNLOCDIR}.PPrimer

#
# System variables...
#
#   TerrRes$Path    - the path to scan down for the untranslated resources inside this component
#   TerrTrans$Path  - the path to scan down for the translated resources inside this component
#
#   The scanning order is:  Vn.UserIF.Locale   Vn.UserIF.UK   Vn.Common.Locale   Vn.Common.UK   Common
#
#   STB$OS          - the OS version number used to represent a Machine type (i.e. '300'='STB3', '400'='STB400')
#   Out$Path        - the location where built messages files are placed (required to keep command line short)
#   TerrMod$Path    - the path from this component to the Territory Module's resources directory (required to keep command line short)
#   Terr$Path       - the path from this component to a locale subset of the Territory Module's resources (i.e. "Canada1.,UK.")
#   Alias$TerrPath  - set the Terr$Path given a locale
#   Alias$Update    - (re)build a messages file given the locale. This appends the 'Messages' file onto the 'Territory' file and
#                     puts the result into the "Out:" directory
#
set_vars: set_paths_${BROWSER} set_OS_${Machine}
	@set Out$Path ${MSGSDIR}.
	@set TerrMod$Path ^.^.^.^.Internat.Territory.Module.Resources.
	@set Alias$TerrPath Set Terr$Path TerrMod:%0.,TerrMod:UK.
	@set Alias$Update "Echo Build file '%0'|MTerrPath %0|MPrint Terr:Territory { > Out:%0 }|MPrint Terr:Messages { >> Out:%0 }|MSetType Out:%0 Text"
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
	@unset Terr$Path
	@unset Alias$TerrPath
	@unset Alias$Update
	@unset TerrMod$Path
	@unset Out$Path
	@unset TerrRes$Path
	@unset TerrTrans$Path
	@unset STB$OS

resources: res_pprimer res_parallel res_filter res_check build_message_files
	${IFTHERE} TerrRes:!Boot      ${IFTHEN} ${CP} TerrRes:!Boot      ${INSTAPP}.!Boot            ${CPFLAGS}
	${IFTHERE} TerrRes:!Run       ${IFTHEN} ${CP} TerrRes:!Run       ${INSTAPP}.!Run             ${CPFLAGS}
	${IFTHERE} TerrRes:!RunImage  ${IFTHEN} ${CP} TerrRes:!RunImage  ${INSTAPP}.!RunImage        ${CPFLAGS}
	${IFTHERE} TerrRes:banners    ${IFTHEN} ${CP} TerrRes:banners    ${INSTOPTS}.pics.banners    ${CPFLAGS}
	${IFTHERE} TerrRes:misc       ${IFTHEN} ${CP} TerrRes:misc       ${INSTOPTS}.pics.misc       ${CPFLAGS}
	${IFTHERE} TerrRes:teleopts   ${IFTHEN} ${CP} TerrRes:teleopts   ${INSTOPTS}.pics.teleopts   ${CPFLAGS}
	${IFTHERE} TerrRes:Choices    ${IFTHEN} ${CP} TerrRes:Choices    ${INSTCHOI}                 ${CPFLAGS}
	${IFTHERE} TerrRes:JScript    ${IFTHEN} ${CP} TerrRes:JScript    ${INSTHTML}.JScript         ${CPFLAGS}
	${IFTHERE} TerrRes:SendMsg    ${IFTHEN} ${CP} TerrRes:SendMsg    ${INSTHTML}.SendMsg         ${CPFLAGS}
	${IFTHERE} TerrRes:Test       ${IFTHEN} ${CP} TerrRes:Test       ${INSTHTML}.Test            ${CPFLAGS}
	${IFTHERE} ${MSGSDIR}.Canada1 ${IFTHEN} ${CP} ${MSGSDIR}.Canada1 ${INSTAPP}.Canada1.Messages ${CPFLAGS}
	${IFTHERE} ${MSGSDIR}.France  ${IFTHEN} ${CP} ${MSGSDIR}.France  ${INSTAPP}.France.Messages  ${CPFLAGS}
	${IFTHERE} ${MSGSDIR}.Germany ${IFTHEN} ${CP} ${MSGSDIR}.Germany ${INSTAPP}.Germany.Messages ${CPFLAGS}
	${IFTHERE} ${MSGSDIR}.Norway  ${IFTHEN} ${CP} ${MSGSDIR}.Norway  ${INSTAPP}.Norway.Messages  ${CPFLAGS}
	${IFTHERE} ${MSGSDIR}.Spain   ${IFTHEN} ${CP} ${MSGSDIR}.Spain   ${INSTAPP}.Spain.Messages   ${CPFLAGS}

build_message_files:
	@Update Canada1
	@Update France
	@Update Germany
	@Update Norway
	@Update Spain

res_check:
	Set MSG_CHECK$Path ${MSG_CHECK}
	${IFTHERE} MSG_CHECK:Messages ${IFTHEN} ${CP} MSG_CHECK:Messages ${CMNLOCDIR}.CheckURL.Messages ${CPFLAGS}
	Unset MSG_CHECK$Path

res_filter:
	Set MSG_FILTER$Path ${MSG_FILTER}
	${IFTHERE} MSG_FILTER:Messages ${IFTHEN} ${CP} MSG_FILTER:Messages ${CMNLOCDIR}.FilterMgr.Messages ${CPFLAGS}
	${IFTHERE} MSG_FILTER:CmdHelp  ${IFTHEN} ${CONCAT} ${CMNLOCDIR}.FilterMgr.Messages ${CATFLAGS} MSG_FILTER:CmdHelp
	Unset MSG_FILTER$Path

res_parallel:
	Set MSG_PARALLEL$Path ${MSG_PARALLEL}
	${IFTHERE} MSG_PARALLEL:Messages ${IFTHEN} ${CP} MSG_PARALLEL:Messages ${CMNLOCDIR}.Parallel.Messages ${CPFLAGS}
	Unset MSG_PARALLEL$Path

res_pprimer:
	Set MSG_PPRIMER$Path ${MSG_PPRIMER}
	${IFTHERE} MSG_PPRIMER:Messages ${IFTHEN} ${CP} MSG_PPRIMER:Messages ${CMNLOCDIR}.PPrimer.Messages ${CPFLAGS}
	Unset MSG_PPRIMER$Path

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
	${IFTHERE} TerrTrans:options/css       ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:options/css       -o ${INSTOPTS}.options/css
	${IFTHERE} TerrTrans:html.body/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.body/html    -o ${INSTHTML}.body/html
	${IFTHERE} TerrTrans:html.body_h/html  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.body_h/html  -o ${INSTHTML}.body_h/html
	${IFTHERE} TerrTrans:html.home/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.home/html    -o ${INSTHTML}.home/html
	${IFTHERE} TerrTrans:html.home_h/html  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.home_h/html  -o ${INSTHTML}.home_h/html
	${IFTHERE} TerrTrans:html.index/html   ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.index/html   -o ${INSTHTML}.index/html
	${IFTHERE} TerrTrans:html.index_h/html ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.index_h/html -o ${INSTHTML}.index_h/html
	${IFTHERE} TerrTrans:html.side/html    ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.side/html    -o ${INSTHTML}.side/html
	${IFTHERE} TerrTrans:html.top/html     ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.top/html     -o ${INSTHTML}.top/html
	${IFTHERE} TerrTrans:html.homecss/css  ${IFTHEN} ${TRANSLATE} ${TRANSFLAGS} -i TerrTrans:html.homecss/css  -o ${INSTHTML}.homecss/css

# Dynamic dependencies:
