# Makefile for NCFresco2, HTTP2, WebFTP2 - resource export, export headers
# and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
RESRES_ALT = <Resource$Dir>.Resources.NCFrescoA
INSTALLDIR = ${INSTDIR}.!NCFresco
INSTALLRESDIR = ${INSTALLDIR}.Resources
INSTALLRESLOCALEDIR=${INSTALLRESDIR}.${LOCALE}
#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CP      = copy
SQ      = squeeze
RM      = remove
WIPE    = -wipe
LD      = link
TR      = translate
GETVERS = perl bin.GetVersion
NOP     = @|

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
TRFLAGS = ${THROWBACK} -marko "{<" -markc ">}" -htmlo "êê" -htmlc "ëë" -m NCFrescoROTransRes:Mappings
SQFLAGS =
WFLAGS  = ~CFR~V

#
# Program specific options:
#
COMPONENT ?= NCFresco2
TARGET    ?= NCFresco

ABSSYM    = RISC_OSLib:o.c_abssym

#
# Generic rules:
#
rom:
        @echo ${COMPONENT}: rom module built

clean:
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Resources phases

resources: resources_${TARGET}

resources_:
        @echo Need a defined target!

# NCFresco resources

resources_NCFresco: resources_NCFresco_${RES} resources_NCFresco_base
        @echo ${COMPONENT}: all resource files copied

resources_NCFresco_:
        @echo ${COMPONENT}: dummy alt resource files copied

resources_NCFresco_alt:
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

resources_NCFresco_base:
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
	${CP} NCFrescoRORes:fontmap/txt 	${RESRES}.fontmap/txt ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${RESRES}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.alert     -o ${RESRES}.html.alert     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.error     -o ${RESRES}.html.error     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favmove   -o ${RESRES}.html.favmove   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favrename -o ${RESRES}.html.favrename ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${RESRES}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscatr  -o ${RESRES}.html.favscatr  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscats  -o ${RESRES}.html.favscats  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${RESRES}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.global/css -o ${RESRES}.html.global/css ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${RESRES}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${RESRES}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histt     -o ${RESRES}.html.histt     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.message    -o ${RESRES}.html.message   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.okcncl    -o ${RESRES}.html.okcncl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${RESRES}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${RESRES}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.prompt    -o ${RESRES}.html.prompt    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${RESRES}.html.url       ${TRFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: base resource files copied

# HTTP resources

resources_HTTP:
        @echo ${COMPONENT}: resource files copied

# WebFTP resources

resources_WebFTP:
        @echo ${COMPONENT}: resource files copied

#
# Install phases

install: install_${TARGET}

install_:
        @echo Need a defined target!

# NCFresco install

install_NCFresco: install_NCFresco_${ITYPE} install_NCFresco_common
        @echo ${COMPONENT}: all files installed

install_NCFresco_: install_NCFresco_application
        @echo ${COMPONENT}: default install

install_NCFresco_common:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${MKDIR} ${INSTALLDIR}
	${MKDIR} ${INSTALLRESDIR}
	${MKDIR} ${INSTALLRESLOCALEDIR}
	${MKDIR} ${INSTALLRESLOCALEDIR}.html
	${CP} NCFrescoRORes:!Boot		${INSTALLDIR}.!Boot             ${CPFLAGS}
	IfThere NCFrescoRORes:!Help Then ${CP} NCFrescoRORes:!Help ${INSTALLDIR}.!Help  ${CPFLAGS}
	${CP} NCFrescoRORes:!Sprites		${INSTALLDIR}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites		${INSTALLRESLOCALEDIR}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${INSTALLRESLOCALEDIR}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res			${INSTALLRESLOCALEDIR}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap		${INSTALLDIR}.MimeMap           ${CPFLAGS}
	${CP} NCFrescoRORes:Config		${INSTALLDIR}.Config            ${CPFLAGS}
	${CP} NCFrescoRORes:fontmap/txt		${INSTALLDIR}.fontmap/txt       ${CPFLAGS}
	${CP} <Lib$Dir>.LocaleRes.LocaleRes	${INSTALLDIR}.LocaleRes         ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${INSTALLDIR}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.alert     -o ${INSTALLRESLOCALEDIR}.html.alert     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.error     -o ${INSTALLRESLOCALEDIR}.html.error     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favmove   -o ${INSTALLRESLOCALEDIR}.html.favmove   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favrename -o ${INSTALLRESLOCALEDIR}.html.favrename ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${INSTALLRESLOCALEDIR}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscatr  -o ${INSTALLRESLOCALEDIR}.html.favscatr  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscats  -o ${INSTALLRESLOCALEDIR}.html.favscats  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${INSTALLRESLOCALEDIR}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.global/css -o ${INSTALLRESLOCALEDIR}.html.global/css ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${INSTALLRESLOCALEDIR}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${INSTALLRESLOCALEDIR}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histt     -o ${INSTALLRESLOCALEDIR}.html.histt     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.message   -o ${INSTALLRESLOCALEDIR}.html.message   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.okcncl    -o ${INSTALLRESLOCALEDIR}.html.okcncl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${INSTALLRESLOCALEDIR}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${INSTALLRESLOCALEDIR}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.prompt    -o ${INSTALLRESLOCALEDIR}.html.prompt    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${INSTALLRESLOCALEDIR}.html.url       ${TRFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: common files installed

install_NCFresco_application:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${CP} NCFrescoRORes:!Run		${INSTALLDIR}.!Run        ${CPFLAGS}
	${CP} abs.!RunIm-${TYPE}		${INSTALLDIR}.!RunImage   ${CPFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: application files installed

install_NCFresco_module:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	${CP} NCFrescoRORes:!RunRAM		${INSTALLDIR}.!Run        ${CPFLAGS}
	${CP} rm.ncfrrm-${TYPE}			${INSTALLDIR}.!RunImage   ${CPFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
	@echo ${COMPONENT}: module files installed

install_NCFresco_: install_NCFresco_application

# HTTP install

install_HTTP: install_HTTP_${TYPE} install_dir
        ${NOP}

install_dir:
        ${MKDIR} ${INSTDIR}

install_HTTP_: install_HTTP_i install_HTTP_sx install_HTTP_s
        ${NOP}

install_HTTP_i:
        ${CP} rm.HTTP ${INSTDIR}.${TARGET}Mod ${CPFLAGS}
        @echo ${COMPONENT} (insecure): file installed

install_HTTP_sx:
        ${CP} rm.HTTPsx ${INSTDIR}.${TARGET}sxMod ${CPFLAGS}
        @echo ${COMPONENT} (export): file installed

install_HTTP_s:
        ${CP} rm.HTTPs ${INSTDIR}.${TARGET}sMod ${CPFLAGS}
        @echo ${COMPONENT} (strong): file installed

# WebFTP install

install_WebFTP: install_dir
        ${CP} rm.WebFTP ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: files installed

#
# Export phases

export: export_${TARGET}
        ${NOP}

export_NCFresco: export_NCFresco_${PHASE}
        @echo ${COMPONENT}: export complete

export_NCFresco_libs:
        @echo ${COMPONENT}: export complete(libs)

export_NCFresco_hdrs:
        ${CP} h.plugin ${EXP_HDR}.h.plugin ${CPFLAGS}
        ${CP} h.ANT_URL ${EXP_HDR}.h.ANT_URL ${CPFLAGS}
        ${CP} h.ANT_Msg ${EXP_HDR}.h.ANT_Msg ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

export_HTTP: export_HTTP_${PHASE}
        ${NOP}

export_HTTP_libs:
        @echo ${COMPONENT}: export complete(libs)

export_HTTP_hdrs:
        ${CP} h.accstatus ${EXP_HDR}.h.accstatus ${CPFLAGS}
        ${CP} h.httpdefs ${EXP_HDR}.h.httpdefs ${CPFLAGS}
        ${CP} h.httpmod ${EXP_HDR}.h.httpmod ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

export_WebFTP: export_WebFTP_${PHASE}
        @echo ${COMPONENT}: export complete

export_WebFTP_libs:
        @echo ${COMPONENT}: export complete(libs)

export_WebFTP_hdrs:
        ${CP} h.accstatus ${EXP_HDR}.h.accstatus ${CPFLAGS}
        ${CP} h.webftpdefs ${EXP_HDR}.h.webftpdefs ${CPFLAGS}
        ${CP} h.webftpmod ${EXP_HDR}.h.webftpmod ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

#
# Final link for the ROM Image (using given base address)
#

rom_link: rom_link_${TARGET}

rom_link_NCFresco: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.ncfre-${TYPE} ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for ${TYPE} build complete

rom_link_HTTP: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.HTTP${TYPE} ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for ${TYPE} build complete

rom_link_WebFTP: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.WebFTP ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

dirs:
        ${MKDIR} linked

# Dynamic dependencies:
