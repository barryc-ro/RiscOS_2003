# Makefile for NCFresco - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
RESRES_ALT = <Resource$Dir>.Resources.NCFrescoA
INSTALLDIR = ${INSTDIR}.!NCFresco
INSTRESDIR = ${INSTALLDIR}.Resources.${Locale}

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CP      = copy
SQ      = squeeze
RM      = remove
LD	= link
TR	= translate
GETVERS = perl bin.GetVersion

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
TRFLAGS = ${THROWBACK} -marko "��" -markc "��" -htmlo "��" -htmlc "��" -m NCFrescoROTransRes:Mappings
SQFLAGS = 

#
# Program specific options:
#
COMPONENT = NCFresco
TARGET    = aof.ncfre-${TYPE}

ABSSYM    = RISC_OSLib:o.c_abssym

BBETYPE = fresco
bbe-fresco: bbe-generic
  BBE_Export_Dir bin
  ${CP} VersionNum ${BBEDIR}.VersionNum ${CPFLAGS}
  ${CP} VERSION ${BBEDIR}.VERSION ${CPFLAGS}

#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

clean:
        @echo ${COMPONENT}: cleaned

# Resources phase

resources: resources_${RES} resources_base
        @echo ${COMPONENT}: all resource files copied

resources_:
        @echo ${COMPONENT}: dummy alt resource files copied

resources_alt:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	Set NCFrescoROAltRes$Path NCFrescoRORes:Alt.,NCFrescoRORes:
	Set NCFrescoROAltTransRes$Path NCFrescoROTransRes:Alt.,NCFrescoROTransRes:
	${MKDIR} ${RESRES_ALT}
	IfThere NCFrescoRORes:Alt.Sprites Then \
	  ${CP} NCFrescoRORes:Alt.Sprites          ${RESRES_ALT}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROAltTransRes:Messages -o ${RESRES_ALT}.Messages ${TRFLAGS}
	${CP} NCFrescoROAltRes:Res                 ${RESRES_ALT}.Res      ${CPFLAGS}
	Unset NCFrescoRORes$Path
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoROAltRes$Path
	Unset NCFrescoROAltTransRes$Path
	@echo ${COMPONENT}: alt resource files copied

resources_base:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.html
	${CP} NCFrescoRORes:!BootROM    	${RESAPP}.!Boot    ${CPFLAGS}
	${CP} NCFrescoRORes:!RunROM     	${RESAPP}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!Sprites    	${RESAPP}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites     	${RESRES}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${RESRES}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         	${RESRES}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     	${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      	${RESRES}.Config   ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${RESRES}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${RESRES}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${RESRES}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${RESRES}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${RESRES}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${RESRES}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsalert   -o ${RESRES}.html.jsalert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsprompt  -o ${RESRES}.html.jsprompt  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsokcncl  -o ${RESRES}.html.jsokcncl  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${RESRES}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${RESRES}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${RESRES}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${RESRES}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${RESRES}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${RESRES}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${RESRES}.html.url       ${TRFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: resource files copied

# Install phases

install: install_${ITYPE} install_common
	@echo ${COMPONENT}: all files installed

install_: install_application
	@echo ${COMPONENT}: default install

install_common:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${MKDIR} ${INSTALLDIR}
	${MKDIR} ${INSTRESDIR}.html
	${CP} NCFrescoRORes:!Boot			${INSTALLDIR}.!Boot     ${CPFLAGS}
	IfThere NCFrescoRORes:!Help Then ${CP} NCFrescoRORes:!Help ${INSTALLDIR}.!Help  ${CPFLAGS}
	${CP} NCFrescoRORes:!Sprites			${INSTALLDIR}.!Sprites  ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites			${INSTRESDIR}.Sprites   ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${INSTRESDIR}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res				${INSTRESDIR}.Res       ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap			${INSTALLDIR}.MimeMap   ${CPFLAGS}
	${CP} NCFrescoRORes:Config			${INSTALLDIR}.Config    ${CPFLAGS}
	${CP} ${Lib$Dir}.LocaleRes.LocaleRes		${INSTALLDIR}.LocaleRes ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${INSTALLDIR}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${INSTRESDIR}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${INSTRESDIR}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${INSTRESDIR}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${INSTRESDIR}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${INSTRESDIR}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsalert   -o ${INSTRESDIR}.html.jsalert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsprompt  -o ${INSTRESDIR}.html.jsprompt  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsokcncl  -o ${INSTRESDIR}.html.jsokcncl  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.optfonts  -o ${INSTRESDIR}.html.optfonts  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.opttext   -o ${INSTRESDIR}.html.opttext   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${INSTRESDIR}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${INSTRESDIR}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${INSTRESDIR}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${INSTRESDIR}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${INSTRESDIR}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${INSTRESDIR}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${INSTRESDIR}.html.url       ${TRFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: common files installed

install_application:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${CP} NCFrescoRORes:!Run			${INSTALLDIR}.!Run      ${CPFLAGS}
	${SQ} abs.!RunIm-${TYPE}			${INSTALLDIR}.!RunImage ${SQFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: application files installed

install_module:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${CP} NCFrescoRORes:!RunRAM			${INSTALLDIR}.!Run      ${CPFLAGS}
	${CP} rm.ncfrrm-${TYPE}				${INSTALLDIR}.!RunImage ${CPFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: module files installed

# Export phases

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
        ${CP} h.plugin ${EXP_HDR}.h.plugin ${CPFLAGS}
        ${CP} h.ANT_URL ${EXP_HDR}.h.ANT_URL ${CPFLAGS}
        ${CP} h.ANT_Msg ${EXP_HDR}.h.ANT_Msg ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

#
# Final link for the ROM Image (using given base address)
#

rom_link: dirs
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for ${TYPE} build complete

dirs:
	${MKDIR} linked

# Dynamic dependencies:
